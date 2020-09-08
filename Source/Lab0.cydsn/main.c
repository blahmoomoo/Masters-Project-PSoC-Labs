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

#include <device.h>

void main()
{
    //The number of times the pushbutton has been pressed
    uint16 count = 0;
    
    //A "boolean" to keep track if the button was read as pressed
    //in the previous loop iteration
    uint8 pressed = 0;
    
    
    //Initialize the display
    Display_Start();
    Display_Position(0, 0);
    Display_PrintString("Count");
    Display_Position(1, 0);
    Display_PrintNumber(count);
    
    //Initialize the LED to off
    LED_Write(0);
    
    //Infinitely loop
    for(;;)
    {
        //Read the pushbutton only once
        uint8 buttonRead = Pushbutton_Read();
        
        //Determine if the count should be incremented
        //The pressed boolean is used to count only once per press
        //Print the new value of the count only if it changed
        //Printing the count continuously may cause the LCD screen to flicker
        if(buttonRead == 0 && !pressed)
        {
            pressed = 1;
            ++count;
            Display_Position(1, 0);
            Display_PrintNumber(count);
        }
        else if(buttonRead == 1 && pressed)
        {
            pressed = 0;
        }
        
        //Turn the LED on if the pushbutton is pressed, otherwise turn it off
        if(buttonRead == 0)
        {
            LED_Write(1);
        }
        else
        {
            LED_Write(0);
        }
    }
}

/* [] END OF FILE */
