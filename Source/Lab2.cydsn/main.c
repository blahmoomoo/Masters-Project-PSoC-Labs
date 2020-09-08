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

#include "stdio.h"
#include <project.h>

#define TRUE  1
#define FALSE 0
#define STOPWATCH_FREQ 10000

asm (".global _printf_float");  //adds ~8000 bytes to the program

int main()
{
	//Set to TRUE if the stopwatch has started, else FALSE
	uint8 started_b = FALSE;
	//Temporary storage for status register
	uint8 reg;
	//Capture/counter read value
	uint32 capture;
	//Capture/counter converted to seconds
	float seconds;
	//Temporary string for sprintf to use
	char tstr[16];
	
	//Start all components and init display
	SecondTimer_Start();
	Stopwatch_Start();
	Display_Start();
	StopwatchStart_Read();
	
	Display_Position(0, 0);
	Display_PrintString("Stopped");

    for(;;)
    {
		if(!started_b)
		{
			//If the stopwatch isn't known to have started yet,
			//see if the start button has been pressed. If it has
			//been pressed, the stopwatch has started.
			if(StopwatchStart_Read())
			{
				started_b = TRUE;
				Display_ClearDisplay();
				Display_Position(0, 0);
				Display_PrintString("Started");
			}
		}
		else
		{
			//Else, check to see if the stopwatch has been stopped.
			reg = Stopwatch_ReadStatusRegister();
			
			//If the stopwatch has been stopped, first read the capture
			if(reg & Stopwatch_STATUS_CAPTURE)
			{
				capture = Stopwatch_ReadCapture();
				started_b = FALSE;
			}
			//Else, just read the counter
			else
			{
				capture = Stopwatch_ReadCounter();
			}
			
			//Convert the capture/counter to seconds and print it
			capture = Stopwatch_ReadPeriod() - capture;
			if(capture != 0)
			{
				seconds = capture / (float)STOPWATCH_FREQ;
				Display_Position(1, 0);
				sprintf(tstr, "%1.4f", seconds);
				Display_PrintString(tstr);
			}
			
			//If the stopwatch has stopped, reset it
			if(!started_b)
			{
				StopwatchReset_Write(1);
				StopwatchStart_Read();
				Display_Position(0, 0);
				Display_PrintString("Stopped");
			}
		}
    }
}

/* [] END OF FILE */
