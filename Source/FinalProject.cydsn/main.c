/* ========================================
 *
 * Copyright (c) 2014, Stephen Hammack
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * The name of Stephen Hammack may not be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL STEPHEN HAMMACK BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <project.h>
#include "FS.h"
#include "inttypes.h"
#include "stdio.h"
#include "limits.h"
#include "math.h"

#define TRUE  1
#define FALSE 0
#define REGBUFFER_SIZE 100
#define FILE_LINE_LENGTH 50
#define FILEBUF_NUM_LINES 4
#define MIXER_DISABLE 0xFF


//State of the playback system in the main function
typedef enum {
	kSDCardCheck, kSDCardWait, kStopped, kFileSelect, kPlayStart, kPlaying, kEOF, kError
} PlayState;


PlayState state;

//Circular queue of register dumps for music playback
uint8 regBuffer[REGBUFFER_SIZE][16];
uint8 regBufStart;
uint8 regBufEnd;
uint8 regBufLength;

//The current register dump that is being played in the hardware
uint8* curReg;

//Indicates that registers have been updated, so the display should be updated
uint8 updateVisualization;



//Updates the hardware with the next register dump 50 times a second
//  during playback.
CY_ISR(RefreshIntervalISR)
{
	//Do nothing if playback hasn't been initiated
	if(state == kPlaying || state == kEOF)
	{
		if(regBufLength > 0)
		{
			//Get the next register dump off the queue
			uint8* nextReg = regBuffer[regBufStart];
			
			//Voice A Frequency
			uint16 voiceADiv = ( ( (((uint16)nextReg[1] & 0xF) << 8) | (uint16)nextReg[0] ) << 4 ) - 1;
			if(VoiceA_clock_GetDividerRegister() != voiceADiv)
			{
				VoiceA_clock_SetDividerRegister(voiceADiv, 0);
			}
			
			//Voice B Frequency
			uint16 voiceBDiv = ( ( (((uint16)nextReg[3] & 0xF) << 8) | (uint16)nextReg[2] ) << 4 ) - 1;
			if(VoiceB_clock_GetDividerRegister() != voiceBDiv)
			{
				VoiceB_clock_SetDividerRegister(voiceBDiv, 0);
			}
			
			//Voice C Frequency
			uint16 voiceCDiv = ( ( (((uint16)nextReg[5] & 0xF) << 8) | (uint16)nextReg[4] ) << 4 ) - 1;
			if(VoiceC_clock_GetDividerRegister() != voiceCDiv)
			{
				VoiceC_clock_SetDividerRegister(voiceCDiv, 0);
			}
			
			//Noise Frequency (shifted by 2 instead of 4 because PSG takes 4 clock cycles to produce next random bit)
			uint8 noiseDiv = ( ( nextReg[6] & 0x1F ) << 2) - 1;
			if(Noise_clock_GetDividerRegister() != noiseDiv)
			{
				Noise_clock_SetDividerRegister(noiseDiv, 0);
			}
			
			//Envelope timer period
			uint16 envPeriod = ((uint16)nextReg[12] << 8) | (uint16)nextReg[11];
			if(Envelope_timer_ReadPeriod() != envPeriod)
			{
				Envelope_timer_Stop();
				Envelope_timer_WritePeriod(envPeriod);
				//Envelope_timer_WriteCounter(envPeriod-1);  //Restart timer, skip tc
				Envelope_timer_Enable();
			}
			
			//Various registers
			VoiceA_Amplitude_Write(nextReg[8]);
			VoiceB_Amplitude_Write(nextReg[9]);
			VoiceC_Amplitude_Write(nextReg[10]);
			Mixer_Control_Write(nextReg[7]);
			if(EnvelopeControl_Read() != nextReg[13] && nextReg[13] != 0xFF)
			{
				EnvelopeControl_Write(nextReg[13]);
				EnvelopeReset_Write(1);
			}
			
			
			//Pop the register dump used off the circular queue
			curReg = nextReg;
			--regBufLength;
			if(++regBufStart >= REGBUFFER_SIZE)
			{
				regBufStart = 0;
			}
			
			//Request the main loop to update the display, if it has time
			updateVisualization = TRUE;
		}
		else
		{
			if(state == kPlaying)
			{
				Display_ClearDisplay();
				Display_Position(0, 0);
				Display_PrintString("Buffer ran out!");
				state = kError;
			}
			else
			{
				state = kStopped;
			}
		}
	}
}


//Buckets for the music visualization
uint16 BUCKET_VALS[] = {3072, 2560, 2048, 1792, 1536, 1280, 1024, 896, 768, 640, 512, 384, 256, 192, 128, 0};


//Takes the frequency of a voice and converts it to a position on the display
uint8 getBucketPos(uint16 val)
{
	val /= 16;
	int i;
	for(i=0; i<16; ++i)
	{
		if(val >= BUCKET_VALS[i])
		{
			return i;
		}
	}
	return 0;
}



//Prints a visualization of the music playback to the display
//The frequency of a voice determines its position on the display
//  and the volume of the voice determines how tall the bar displayed
//  is.  The volume is 15 if the envelope is used.  The voice is not
//  displayed if both tone and noise is disabled.
//Possible race condition if RefreshIntervalISR is called while this
//  is happening, but that should only cause a minor visual inconsistency.
//  No side-effects to the music since this does not affect anything related
//  to the playback.
void PrintVisualization()
{
	uint8 buckets[16];
	memset(buckets, 0, sizeof(buckets));
	
	uint16 value = VoiceA_clock_GetDividerRegister();
	value = getBucketPos(value);
	uint8 amplitude = (curReg[8]&0x10)? 16 :  VoiceA_Amplitude_Read();
	if(!(curReg[7] & 0b1 && curReg[7] & 0b1000) && buckets[value] < amplitude)
	{
		buckets[value] = amplitude;
	}
	
	value = VoiceB_clock_GetDividerRegister();
	value = getBucketPos(value);
	amplitude = (curReg[9]&0x10)? 16 :  VoiceB_Amplitude_Read();
	if(!(curReg[7] & 0b10 && curReg[7] & 0b10000) && buckets[value] < amplitude)
	{
		buckets[value] = amplitude;
	}
	
	value = VoiceC_clock_GetDividerRegister();
	value = getBucketPos(value);
	amplitude = (curReg[10]&0x10)? 16 :  VoiceC_Amplitude_Read();
	if(!(curReg[7] & 0b100 && curReg[7] & 0b100000) && buckets[value] < amplitude)
	{
		buckets[value] = amplitude;
	}
	
	int i;
	for(i=0; i<16; ++i)
	{
		Display_DrawVerticalBG(1, i, 2, buckets[i]);
	}
}



//Main function. Handles initiating playback and streaming data off the SD card.
int main()
{
	//Initialization
	Mixer_Control_Write(MIXER_DISABLE);
	
    Display_Start();
	NoiseGenerator_Start();
	Envelope_timer_Start();
	VoiceA_DAC_Start();
	VoiceB_DAC_Start();
	VoiceC_DAC_Start();
	RefreshInterval_isr_StartEx(RefreshIntervalISR);
	Audio_opamp_Start();
	FS_Init();
	
	
	state = kSDCardCheck;
	regBufStart = 0;
	regBufEnd = 0;
	regBufLength = 0;
	curReg = regBuffer[0];
	updateVisualization = FALSE;

    CyGlobalIntEnable;
	
	FS_FILE * pfile;
	FS_FIND_DATA fd;
	char filename[20];
	char tmpc;
	char fileBuffer[FILE_LINE_LENGTH*FILEBUF_NUM_LINES];
	int fileBufPos = 0;
	uint8 fileBufEmpty;
	int numRead;
	uint32 totalLinesRead;
	
	
	//State machine for the music playback
	for(;;)
    {
        switch(state)
		{
			//Check for the presence of the SD card (doesn't actually work, oh well)
			case kSDCardCheck:
			
			Mixer_Control_Write(MIXER_DISABLE);
			
			if(FS_GetNumVolumes() > 0)
			{
				state = kStopped;
			}
			else
			{
				Display_ClearDisplay();
				Display_Position(0, 0);
				Display_PrintString("Insert SD Card");
				state = kSDCardWait;
			}
			
			break;
			
			
			
			//Continue to check for the SD card without printing to display
			case kSDCardWait:
			
			if(FS_GetNumVolumes() > 0)
			{
				state = kStopped;
			}
			
			break;
			
			
			
			//Playback stopped; initialize file selection
			case kStopped:
			
			//Clear out the pushbutton readings in case they were pressed during playback
			Pushbuttons_Read();
			
			//Stop the hardware from producing sound
			Mixer_Control_Write(MIXER_DISABLE);
			EnvelopeControl_Write(0);
			
			Display_ClearDisplay();
			Display_Position(0, 0);
			
			//Initiates a search on the root of the file system for files
			tmpc = FS_FindFirstFile(&fd, "", filename, sizeof(filename));
			
			//If non-zero, there was an error. Automatically retry after two seconds
			if(tmpc)
			{
				Display_PrintString("No files found!");
				CyDelay(2000);
				state = kSDCardCheck;
			}
			//Otherwise, filename contains the name of the first file in the folder
			//  and fd contains the information needed to get the next file
			else
			{
				Display_PrintString("   Next  Play");
				Display_Position(1, 0);
				Display_PrintString(filename);
				state = kFileSelect;
			}
			
			break;
			
			
			
			//Wait for user input for selecting files
			case kFileSelect:
			
			tmpc = Pushbuttons_Read();
			
			//If the Next button was pressed, display the next file from the folder
			if(tmpc & 1)
			{
				//Get the next file. fd already has a reference to filename,
				//  so filename is updated
				tmpc = !FS_FindNextFile(&fd);
				
				//If there is no next file, start over from the beginning of the folder
				if(tmpc)
				{
					FS_FindClose(&fd);
					tmpc = FS_FindFirstFile(&fd, "", filename, sizeof(filename));
					if(tmpc)
					{
						state = kSDCardCheck;
					}
				}
				
				//If there wasn't an error if a restart was necessary, print the name
				//  of the next file
				if(state == kFileSelect)
				{
					Display_Position(1, 0);
					Display_PrintString("                ");
					Display_Position(1, 0);
					Display_PrintString(filename);
				}
			}
			
			//If the Play button was pressed, change state to initiate playback
			else if(tmpc & 2)
			{
				state = kPlayStart;
			}
			
			break;
			
			
			
			//User chose a file; initialize playback
			case kPlayStart:
			
			//Reset all playback-related variables
			regBufStart = 0;
			regBufEnd = 0;
			regBufLength = 0;
			curReg = regBuffer[0];
			fileBufPos = 0;
			totalLinesRead = 0;
			fileBufEmpty = TRUE;
			
			//Reset what needs to be reset in the hardware
			EnvelopeReset_Write(1);
			
			//Open the file chosen
			pfile = FS_FOpen(filename, "r");
			
			//If there was no error opening the file...
			if(pfile)
			{
				//Let the user know the playback is starting
				Display_ClearDisplay();
				Display_PrintString("Buffering...");
				
				//Buffer an initial reading from the file
				//File format: space-delimited plaintext 8-bit hex values; 16 per line,
				//  50 characters per line incl. newline. Each value is a register.
				numRead = FS_Read(pfile, fileBuffer, sizeof(fileBuffer));
				int x;
				
				//There's something wrong if the number of characters read isn't
				//  divisible by 50. Not a fool-proof check against improperly
				//  formatted files.
				if(numRead % FILE_LINE_LENGTH != 0)
				{
					FS_FClose(pfile);
					Display_ClearDisplay();
					Display_Position(0, 0);
					Display_PrintString("File read error!");
					CyDelay(2000);
					state = kSDCardCheck;
				}
				//Otherwise, convert the data read and put it into the circular queue
				else
				{
					//Multiple lines are read from the file. Process all of them.
					for(x = 0; x < numRead / FILE_LINE_LENGTH; ++x)
					{
						int i;
						for(i=0; i<16; ++i)
						{
							unsigned int tmpui;
							sscanf(&fileBuffer[i*3+x*FILE_LINE_LENGTH], "%x", &tmpui);
							regBuffer[regBufEnd][i] = (uint8)tmpui;
						}
						++regBufLength;
						++regBufEnd;
						++totalLinesRead;
					}
					
					//If there were fewer lines in the file than requested, this is
					//  the end of the file, so close the file and transition into kEOF
					//  to intiate playback
					if(numRead < FILE_LINE_LENGTH*FILEBUF_NUM_LINES)
					{
						FS_FClose(pfile);
						state = kEOF;
					}
					//Otherwise, transition to kPlaying to initiate playback and
					//  continue buffering from the file
					else
					{
						state = kPlaying;
					}
				}
			}
			//Print an error saying the file didn't open properly
			else
			{
				Display_ClearDisplay();
				Display_Position(0, 0);
				Display_PrintString("Unable to open!");
				CyDelay(2000);
				state = kSDCardCheck;
			}
			
			break;
			
			
			
			//Buffer register readings for playback (RefreshIntervalISR above handles actual playback)
			case kPlaying:
			
			//If the file buffer is empty, read the next set of lines from the file
			if(fileBufEmpty)
			{
				numRead = FS_Read(pfile, fileBuffer, sizeof(fileBuffer));
				fileBufEmpty = FALSE;
			}
			
			//If the next line expected from the file buffer is missing, there are no
			//  more lines in the file, so close the file and transition to kEOF
			if(numRead <= FILE_LINE_LENGTH*fileBufPos)
			{
				FS_FClose(pfile);
				state = kEOF;
			}
			//Otherwise
			else
			{
				//If there is room in the circular queue (excluding the last popped-off
				//  register dump so curBuf isn't destroyed), parse the next line from the
				//  file buffer and place it in the queue
				if(regBufLength < REGBUFFER_SIZE - 1)
				{
					int i;
					for(i=0; i<16; ++i)
					{
						unsigned int tmpui;
						sscanf(&fileBuffer[i*3+fileBufPos*FILE_LINE_LENGTH], "%x", &tmpui);
						regBuffer[regBufEnd][i] = (uint8)tmpui;
					}
					
					//Temporarily disable the refresh interval ISR while modifying the
					//  circular queue data, as this is a critical section
					RefreshInterval_isr_Disable();
					++regBufLength;
					++totalLinesRead;
					if(++regBufEnd >= REGBUFFER_SIZE)
					{
						regBufEnd = 0;
					}
					if(++fileBufPos >= FILEBUF_NUM_LINES)
					{
						fileBufPos = 0;
						fileBufEmpty = TRUE;
					}
					RefreshInterval_isr_Enable();
				}
				//If the circular queue is full and the RefreshInterval ISR occurred since
				//  the last visualization update, update the visualization
				else if(updateVisualization)
				{
					PrintVisualization();
					updateVisualization = FALSE;
				}
			}
			
			break;
			
			
			
			//End-of-file encountered
			case kEOF:
			
			//Wait for buffer to run out
			//This state will be transitioned out of by RefreshIntervalISR
			
			if(updateVisualization)
			{
				PrintVisualization();
				updateVisualization = FALSE;
			}
			
			break;
			
			
			
			//Unrecoverable error
			case kError:
			
			//Shut down output, then do nothing
			Mixer_Control_Write(MIXER_DISABLE);
			
			break;
		}
    }
}

/* [] END OF FILE */
