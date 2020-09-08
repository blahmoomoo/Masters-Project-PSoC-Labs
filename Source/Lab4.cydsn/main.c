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
#include "stdio.h"
#include "inttypes.h"

#define FALSE 0
#define TRUE 1

//The size of the USB buffer
#define BUF_SIZE 64

//The maximum number of digits to accept from the terminal/USB UART
#define NUMBUF_MAX 3

#define CHAR_NULL '\0'
#define CHAR_BACKSP 0x7F
#define CHAR_ENTER 0x0D
#define LOW_DIGIT '0'
#define HIGH_DIGIT '9'

//Prints the current brightness of the LED to the display
void printBrightnesses(uint8 led4Bright)
{
	Display_ClearDisplay();
	Display_Position(0, 0);
	Display_PrintString("LED4: ");
	Display_PrintNumber(led4Bright);
}

//Utility that writes a character to the USB UART
void usbPutChar(char c)
{
	/* Wait till component is ready to send more data to the PC */ 
    while(USBUART_CDCIsReady() == 0);
    /* Send data back to PC */
    USBUART_PutChar(c);
}

int main()
{
	//Create and initialize local variables
    uint8 usbStarted = FALSE;
    uint8 usbBuffer[BUF_SIZE];
    uint16 usbBufCount;
	char numBuf[NUMBUF_MAX+1] = "\0\0\0";
	uint8 numBufCount = 0;
	uint8 led4Bright = 0;

    CyGlobalIntEnable;
    
    /* Start USBFS Operation with 3V operation */
    USBUART_Start(0, USBUART_3V_OPERATION);

    /* Start LCD */
    Display_Start();
	
	//Initialize PWM
	PWM_Start();
	
	//Set the initial brightness of the LED
	PWM_WriteCompare1(led4Bright);
	
	//Print the LED brightness
	printBrightnesses(led4Bright);
    
    for(;;)
    {
        if(!usbStarted)
        {
            /* Wait for Device to enumerate */
            if(USBUART_GetConfiguration())
            {
                /* Enumeration is done, enable OUT endpoint for
                receive data from Host */
                USBUART_CDC_Init();
                usbStarted = TRUE;
            }
        }
        else
        {
            /* Check for input data from PC */
            if(USBUART_DataIsReady() != 0)
            {
                /* Read received data and re-enable OUT endpoint */
                usbBufCount = USBUART_GetAll(usbBuffer);
				int i;
				
				//Process each character individually
                for(i=0; i<usbBufCount; ++i)
				{
					//If the character is a digit and there is still room in the number buffer,
					//  place it in the buffer
					if(usbBuffer[i] >= LOW_DIGIT && usbBuffer[i] <= HIGH_DIGIT && numBufCount < NUMBUF_MAX)
					{
						numBuf[numBufCount] = usbBuffer[i];
						++numBufCount;
						usbPutChar(usbBuffer[i]);
					}
					//If the character is backspace and there are characters in the number
					//  buffer, remove a character from the buffer (make it null to mark the end)
					else if(usbBuffer[i] == CHAR_BACKSP && numBufCount > 0)
					{
						--numBufCount;
						numBuf[numBufCount] = CHAR_NULL;
						usbPutChar(CHAR_BACKSP);
					}
					//If the character is enter and there are characters in the number buffer,
					//  convert the number buffer to a uint8, update the brightness, and print it.
					//  Also, send a CRLF to the terminal so the user can start typing on the next line.
					else if(usbBuffer[i] == CHAR_ENTER && numBufCount > 0)
					{
						sscanf(numBuf, "%" SCNu8, &led4Bright);
						PWM_WriteCompare1(led4Bright);
						printBrightnesses(led4Bright);
						while(USBUART_CDCIsReady() == 0);
						USBUART_PutCRLF();
						int x;
						for(x = 0; x<NUMBUF_MAX; ++x)
						{
							numBuf[x] = CHAR_NULL;
						}
						numBufCount = 0;
					}
				}
            } 
        }
    }
}
