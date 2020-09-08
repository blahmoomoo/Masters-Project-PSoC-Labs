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

#define TRUE  1
#define FALSE 0

//Global boolean for indicating when the pushbutton was pressed
//Declared in SW2ISR.c
extern uint8 sw2_pressed;

//Global counter for how many times the pushbutton was pressed
//Declared in SW2ISR.c
extern uint16 sw2_count;

int main()
{
	//Initialize global variables
	sw2_count = 0;
	sw2_pressed = TRUE;  //Prints initial count
	
	//Initialize the display
	Display_Start();
	Display_Position(0, 0);
	Display_PrintString("SW2 Count");
	
	//Initialize the ISR and clears any pending interrupt
	//There may be a pending interrupt due to the power-on
	//  state of the input pin
	SW2ISR_Start();
	SW2ISR_ClearPending();

	//Enable interrupts
    CyGlobalIntEnable;
	
    for(;;)
    {
		//Do not update the display unless the pushbutton was pressed
        if(sw2_pressed)
		{
			//Disable interrupts to prevent a race condition
			//  when using global variables
			CyGlobalIntDisable;
			
			//Reset the pressed flag and print out the count
			//Since the count never decrements (unless it overflows),
			//  no need to clear anything
			sw2_pressed = FALSE;
			Display_Position(1, 0);
			Display_PrintDecUint16(sw2_count);
			
			//Re-enable interrupts
			CyGlobalIntEnable;
		}
    }
}

/* [] END OF FILE */
