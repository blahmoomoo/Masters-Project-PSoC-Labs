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
//Needed for sprintf
#include "stdio.h"

void main()
{
    //The sum of up to eight ADC readings, used for averaging
    int32 accumulatedVoltage = 0;
    
    //The number of samples currently summed together in accumulatedVoltage
    uint8 numSamples = 0;
    
    //Temporarily holds the string representation of the filtered
    //mV reading before printing it to the display
    char displayStr[5] = {'\0'};
    
    //Initialize the display with the strings that don't change
    Display_Start();
    Display_Position(0, 1);
    Display_PrintString("Potentiometer");
    Display_Position(1, 9);
    Display_PrintString("mV");
    
    //Initialize the ADC
    ADC_Start();
    ADC_StartConvert();
    
    //Loop forever
    for(;;)
    {
        //Wait for a sample to be ready
        ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
        
        //Read the sample, convert it to mV, and add it to the accumulated voltage
        accumulatedVoltage += ADC_CountsTo_mVolts( ADC_GetResult16() );
        
        //Increment the number of samples read
        ++numSamples;
        
        //When 8 samples have been read, average them and print to the display
        if(numSamples == 8)
        {
            //Right shifting 3 is the equivalent of dividing by 8, but faster
            accumulatedVoltage = accumulatedVoltage >> 3;
            
            //Convert the number to a string with leading spaces
            sprintf(displayStr, "%4ld", accumulatedVoltage);
            
            //Print the number to the display
            Display_Position(1, 4);
            Display_PrintString(displayStr);
            
            //Reset sample reading variables
            accumulatedVoltage = 0;
            numSamples = 0;
        }
    }
}

/* [] END OF FILE */
