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
#include <globals.h>

asm (".global _printf_float");  //adds ~8000 bytes to the program

uint8 started_b;

int main()
{
	//Initialize the started flag to false
	started_b = FALSE;
	
    //Initialize all devices
	DisplayRefreshISR_Start();
	StopwatchStartISR_Start();
	StopwatchStopISR_Start();
	
	SecondTimer_Start();
	Stopwatch_Start();
	DisplayRefresh_Start();
	Display_Start();
	
	//Initialize the display
	Display_Position(0, 0);
	Display_PrintString("Stopped");
	
	//Due to the power-on state of the pushbutton pin, reset the timer
	//  and clear the start ISR in case they were triggered
	StopwatchReset_Write(1);
	StopwatchStartISR_ClearPending();
	
	//Enable handling of the interrupts
	CyGlobalIntEnable;

    for(;;)
    {
		//Everything else is handled in StopwatchStartISR,
		//  StopwatchStopISR, and DisplayRefreshISR
    }
}

/* [] END OF FILE */
