#include <project.h>
//Needed for sprintf
#include "stdio.h"

int main()
{
    //Put variables here
    
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
        //Read the ADC, filter the readings, and print the result to the LCD
        //Use the code below to print the mV reading to the LCD

        //Convert the number to a string with leading spaces
        sprintf(displayStr, "%4d", filteredReading);
        
        //Print the number to the display
        Display_Position(1, 4);
        Display_PrintString(displayStr);
    }
}
