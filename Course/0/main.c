#include <project.h>

int main()
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
        //Insert the code from steps 7-8 here.
        //Use LED_Write(1) to turn on the LED when the button is pressed
        //and use LED_Write(0) turn the LED off otherwise.
        //The value of the pushbutton is read using Pushbutton_Read().
        //Pushbutton_Read() is 1 when not pressed, 0 when pressed.
        
        //Place the two lines below in your code to print the count.
        //Only print the count when it changes, otherwise the LCD may flicker
        Display_Position(1, 0);
        Display_PrintNumber(count);
    }
}
