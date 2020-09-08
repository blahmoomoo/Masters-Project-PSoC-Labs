/*******************************************************************************
* File Name: Display.c
* Version 1.90
*
* Description:
*  This file provides source code for the Character LCD component's API.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CyLib.h"
#include "Display.h"


static void Display_WrDatNib(uint8 nibble) ;
static void Display_WrCntrlNib(uint8 nibble) ;

/* Stores the state of conponent. Indicates wherewer component is 
* in enable state or not.
*/
uint8 Display_enableState = 0u;

uint8 Display_initVar = 0u;


/*******************************************************************************
* Function Name: Display_Init
********************************************************************************
*
* Summary:
*  Perform initialization required for components normal work.
*  This function initializes the LCD hardware module as follows:
*        Enable 4-bit interface
*        Clear the display
*        Enable auto cursor increment
*        Resets the cursor to start position
*  Also loads custom character set to LCD if it was defined in the customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Display_Init(void) 
{
    /* INIT CODE */
    CyDelay(40u);                                                        /* Delay 40 ms */
    Display_WrCntrlNib(Display_DISPLAY_8_BIT_INIT);    /* Selects 8-bit mode */
    CyDelay(5u);                                                         /* Delay 5 ms */
    Display_WrCntrlNib(Display_DISPLAY_8_BIT_INIT);    /* Selects 8-bit mode */
    CyDelay(15u);                                                        /* Delay 15 ms */
    Display_WrCntrlNib(Display_DISPLAY_8_BIT_INIT);    /* Selects 8-bit mode */
    CyDelay(1u);                                                         /* Delay 1 ms */
    Display_WrCntrlNib(Display_DISPLAY_4_BIT_INIT);    /* Selects 4-bit mode */
    CyDelay(5u);                                                         /* Delay 5 ms */

    Display_WriteControl(Display_CURSOR_AUTO_INCR_ON);    /* Incr Cursor After Writes */
    Display_WriteControl(Display_DISPLAY_CURSOR_ON);      /* Turn Display, Cursor ON */
    Display_WriteControl(Display_DISPLAY_2_LINES_5x10);   /* 2 Lines by 5x10 Characters */
    Display_WriteControl(Display_DISPLAY_CURSOR_OFF);     /* Turn Display, Cursor OFF */
    Display_WriteControl(Display_CLEAR_DISPLAY);          /* Clear LCD Screen */
    Display_WriteControl(Display_DISPLAY_ON_CURSOR_OFF);  /* Turn Display ON, Cursor OFF */
    Display_WriteControl(Display_RESET_CURSOR_POSITION);  /* Set Cursor to 0,0 */
    CyDelay(5u);

    #if(Display_CUSTOM_CHAR_SET != Display_NONE)
        Display_LoadCustomFonts(Display_customFonts);
    #endif /* Display_CUSTOM_CHAR_SET != Display_NONE */
}


/*******************************************************************************
* Function Name: Display_Enable
********************************************************************************
*
* Summary:
*  Turns on the display.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
* Theory:
*  This finction has no effect when it called first time as
*  Display_Init() turns on the LCD.
*
*******************************************************************************/
void Display_Enable(void) 
{
    Display_DisplayOn();
    Display_enableState = 1u;
}


/*******************************************************************************
* Function Name: Display_Start
********************************************************************************
*
* Summary:
*  Perform initialization required for components normal work.
*  This function initializes the LCD hardware module as follows:
*        Enable 4-bit interface
*        Clear the display
*        Enable auto cursor increment
*        Resets the cursor to start position
*  Also loads custom character set to LCD if it was defined in the customizer.
*  If it was not the first call in this project then it just turns on the
*  display
*
*
* Parameters:
*  Display_initVar - global variable.
*
* Return:
*  Display_initVar - global variable.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Display_Start(void) 
{
    /* If not initialized then perform initialization */
    if(Display_initVar == 0u)
    {
        Display_Init();
        Display_initVar = 1u;
    }

    /* Turn on the LCD */
    Display_Enable();
}


/*******************************************************************************
* Function Name: Display_Stop
********************************************************************************
*
* Summary:
*  Turns off the display of the LCD screen.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Display_Stop(void) 
{
    /* Calls LCD Off Macro */
    Display_DisplayOff();
    Display_enableState = 0u;
}


/*******************************************************************************
*  Function Name: Display_Position
********************************************************************************
*
* Summary:
*  Moves active cursor location to a point specified by the input arguments
*
* Parameters:
*  row:     Specific row of LCD module to be written
*  column:  Column of LCD module to be written
*
* Return:
*  None.
*
* Note:
*  This only applies for LCD displays which use the 2X40 address mode.
*  This results in Row 2 offset from row one by 0x28.
*  When there are more than 2 rows, each row must be fewer than 20 characters.
*
*******************************************************************************/
void Display_Position(uint8 row, uint8 column) 
{
    switch (row)
    {
        case (uint8)0:
            Display_WriteControl(Display_ROW_0_START + column);
            break;
        case (uint8) 1:
            Display_WriteControl(Display_ROW_1_START + column);
            break;
        case (uint8) 2:
            Display_WriteControl(Display_ROW_2_START + column);
            break;
        case (uint8) 3:
            Display_WriteControl(Display_ROW_3_START + column);
            break;
        default:
            /* if default case is hit, invalid row argument was passed.*/
            break;
    }
}


/*******************************************************************************
* Function Name: Display_PrintString
********************************************************************************
*
* Summary:
*  Writes a zero terminated string to the LCD.
*
* Parameters:
*  string:  pointer to head of char8 array to be written to the LCD module
*
* Return:
*  None.
*
*******************************************************************************/
void Display_PrintString(char8 const string[]) 
{
    uint8 indexU8 = 1u;
    char8 current = *string;

    /* Until null is reached, print next character */
    while((char8) '\0' != current)
    {
        Display_WriteData((uint8)current);
        current = string[indexU8];
        indexU8++;
    }
}


/*******************************************************************************
*  Function Name: Display_PutChar
********************************************************************************
*
* Summary:
*  Writes a single character to the current cursor position of the LCD module.
*  Custom character names (_CUSTOM_0 through
*  _CUSTOM_7) are acceptable as inputs.
*
* Parameters:
*  character:  character to be written to the LCD
*
* Return:
*  None.
*
*******************************************************************************/
void Display_PutChar(char8 character) 
{
    Display_WriteData((uint8)character);
}


/*******************************************************************************
*  Function Name: Display_WriteData
********************************************************************************
*
* Summary:
*  Writes a data byte to the LCD module's Data Display RAM.
*
* Parameters:
*  dByte:  byte to be written to LCD module.
*
* Return:
*  None.
*
*******************************************************************************/
void Display_WriteData(uint8 dByte) 
{
    uint8 nibble;

    Display_IsReady();
    nibble = dByte >> Display_NIBBLE_SHIFT;

    /* Write high nibble */
    Display_WrDatNib(nibble);

    nibble = dByte & Display_NIBBLE_MASK;
    /* Write low nibble */
    Display_WrDatNib(nibble);
}


/*******************************************************************************
*  Function Name: Display_WriteControl
********************************************************************************
*
* Summary:
*  Writes a command byte to the LCD module.
*
* Parameters:
*  cByte:   byte to be written to LCD module.
*
* Return:
*  None.
*
*******************************************************************************/
void Display_WriteControl(uint8 cByte) 
{
    uint8 nibble;

    Display_IsReady();

    nibble = cByte >> Display_NIBBLE_SHIFT;
    
    /* WrCntrlNib(High Nibble) */
    Display_WrCntrlNib(nibble);
    nibble = cByte & Display_NIBBLE_MASK;

    /* WrCntrlNib(Low Nibble) */
    Display_WrCntrlNib(nibble);
}


/*******************************************************************************
* Function Name: Display_IsReady
********************************************************************************
*
* Summary:
*  Polls LCD until the ready bit is set.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Note:
*  Changes pins to High-Z.
*
*******************************************************************************/
void Display_IsReady(void) 
{
    uint8 value;

    /* Clear the LCD port*/
    Display_PORT_DR_REG &= ((uint8)(~Display_PORT_MASK));

    /* PSoC4 has a bit different port/pin organization for setting Drive Modes than
    * PSoC3/5.
    */
    #if (CY_PSOC4)
        
        /* Mask off data pins to clear old values out */
        value = Display_PORT_PC_REG & ((uint32) (~ Display_DM_DATA_MASK));
        /* Load in high Z values for data pins, others unchanged */
        Display_PORT_PC_REG = value | Display_HIGH_Z_DATA_DM;

    #else
    
        /* Change Port to High-Z Status on data pins */

        /* Mask off data pins to clear old values out */
        value = Display_PORT_DM0_REG & ((uint8)(~Display_DATA_MASK));
        /* Load in high Z values for data pins, others unchanged */
        Display_PORT_DM0_REG = value | (Display_HIGH_Z_DM0 & Display_DATA_MASK);

        /* Mask off data pins to clear old values out */
        value = Display_PORT_DM1_REG & ((uint8)(~Display_DATA_MASK));
        /* Load in high Z values for data pins, others unchanged */
        Display_PORT_DM1_REG = value;

        /* Mask off data pins to clear old values out */
        value = Display_PORT_DM2_REG & ((uint8)(~Display_DATA_MASK));
        /* Load in high Z values for data pins, others unchanged */
        Display_PORT_DM2_REG = value;
    
    #endif /* CY_PSOC4 */
    
    /* Make sure RS is low */
    Display_PORT_DR_REG &= ((uint8)(~Display_RS));

    /* Set R/W high to read */
    Display_PORT_DR_REG |= Display_RW;

    do
    {
        /* 40 ns delay required before rising Enable and 500ns between neighbour Enables */
        CyDelayUs(0u);

        /* Set E high */
        Display_PORT_DR_REG |= Display_E;

        /* 360 ns delay the setup time for data pins */
        CyDelayUs(1u);

        /* Get port state */
        value = Display_PORT_PS_REG;

        /* Set enable low */
        Display_PORT_DR_REG &= ((uint8)(~Display_E));

        /* This gives a true delay between disably Enable bit and poling Ready bit */
        CyDelayUs(0u);

        /* Extract ready bit */
        value &= Display_READY_BIT;

        /* Set E high as we in 4-bit interface we need extra oparation */
        Display_PORT_DR_REG |= Display_E;

        /* 360 ns delay the setup time for data pins */
        CyDelayUs(1u);

        /* Set enable low */
        Display_PORT_DR_REG &= ((uint8)(~Display_E));

        /* Repeat until bit 4 is not zero. */

    } while (value != 0u);

    /* Set R/W low to write */
    Display_PORT_DR_REG &= ((uint8)(~Display_RW));

    /* Clear the LCD port*/
    Display_PORT_DR_REG &= ((uint8)(~Display_PORT_MASK));

    #if (CY_PSOC4)
        
        /* Mask off data pins to clear old values out */
        value = Display_PORT_PC_REG & ((uint32)(~ Display_DM_DATA_MASK));
        /* Load in high Z values for data pins, others unchanged */
        Display_PORT_PC_REG = value | Display_STRONG_DATA_DM;

    #else

        /* Change Port to Output (Strong) on data pins */
        /* Mask off data pins to clear high z values out. Configure data pins 
        * to Strong Drive, others unchanged.
        */
        Display_PORT_DM0_REG &= ((uint8)(~Display_DATA_MASK));
        /* Mask off data pins to clear high z values out */
        value = Display_PORT_DM1_REG & ((uint8)(~Display_DATA_MASK));
        /* Configure data pins to Strong Drive, others unchanged */
        Display_PORT_DM1_REG = value | (Display_STRONG_DM1 & Display_DATA_MASK);

        /* Mask off data pins to clear high z values out */
        value = Display_PORT_DM2_REG & ((uint8)(~Display_DATA_MASK));
        /* Configure data pins to Strong Drive, others unchanged */
        Display_PORT_DM2_REG = value | (Display_STRONG_DM2 & Display_DATA_MASK);
    
    #endif /* CY_PSOC4 */
}


/*******************************************************************************
*  Function Name: Display_WrDatNib
********************************************************************************
*
* Summary:
*  Writes a data nibble to the LCD module.
*
* Parameters:
*  nibble:  byte containing nibble in least significant nibble to be written
*           to LCD module.
*
* Return:
*  None.
*
*******************************************************************************/
static void Display_WrDatNib(uint8 nibble) 
{
    Display_IsReady();

    /* RS shoul be low to select data register */
    Display_PORT_DR_REG |= Display_RS;
    /* Reset RW for write operation */
    Display_PORT_DR_REG &= ((uint8)(~Display_RW));

    /* Two following lines of code will provide us with 40ns delay */
    /* Clear data pins */
    Display_PORT_DR_REG &= ((uint8)(~Display_DATA_MASK));

    /* Write in data, bring E high*/
    #if(0u != Display_PORT_SHIFT) /* MISRA forbids shift by 0 so need to handle that */
        Display_PORT_DR_REG |= 
            (Display_E | ((uint8)(((uint8) nibble) << Display_PORT_SHIFT)));
    #else
        Display_PORT_DR_REG |= (Display_E | nibble);
    #endif /* (0u != Display_PORT_SHIFT) */

    /* Minimum of 230 ns delay */
    CyDelayUs(1u);

    Display_PORT_DR_REG &= ((uint8)(~Display_E));
}


/*******************************************************************************
*  Function Name: Display_WrCntrlNib
********************************************************************************
*
* Summary:
*  Writes a control nibble to the LCD module.
*
* Parameters:
*  nibble:  byte containing nibble in least significant nibble to be written
*           to LCD module.
*
* Return:
*  None.
*
*******************************************************************************/
static void Display_WrCntrlNib(uint8 nibble) 
{
    /* RS and RW shoul be low to select instruction register and  write operation respectively */
    Display_PORT_DR_REG &= ((uint8)(~(Display_RS | Display_RW)));

    /* Two following lines of code will give provide ua with 40ns delay */
    /* Clear data pins */
    Display_PORT_DR_REG &= ((uint8)(~Display_DATA_MASK));

    /* Write control data and set enable signal */
    #if(0u != Display_PORT_SHIFT) /* MISRA forbids shift by 0 so need to handle that */
        Display_PORT_DR_REG |= 
            (Display_E | ((uint8)(((uint8) nibble) << Display_PORT_SHIFT)));
    #else
        Display_PORT_DR_REG |= (Display_E | nibble);
    #endif /* (0u != Display_PORT_SHIFT) */

    /* Minimum of 230 ns delay */
    CyDelayUs(1u);

    Display_PORT_DR_REG &= ((uint8)(~Display_E));
}


#if(Display_CONVERSION_ROUTINES == 1u)

    /*******************************************************************************
    *  Function Name: Display_PrintInt8
    ********************************************************************************
    *
    * Summary:
    *  Print a byte as two ASCII characters.
    *
    * Parameters:
    *  value:  The byte to be printed out as ASCII characters.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void Display_PrintInt8(uint8 value) 
    {
        static char8 const CYCODE Display_hex[16u] = "0123456789ABCDEF";
        
        Display_PutChar((char8) Display_hex[value >> Display_BYTE_UPPER_NIBBLE_SHIFT]);
        Display_PutChar((char8) Display_hex[value & Display_BYTE_LOWER_NIBBLE_MASK]);
    }


    /*******************************************************************************
    *  Function Name: Display_PrintInt16
    ********************************************************************************
    *
    * Summary:
    *  Print a uint16 as four ASCII characters.
    *
    * Parameters:
    *  value:   The uint16 to be printed out as ASCII characters.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void Display_PrintInt16(uint16 value) 
    {
        Display_PrintInt8((uint8)(value >> Display_U16_UPPER_BYTE_SHIFT));
        Display_PrintInt8((uint8)(value & Display_U16_LOWER_BYTE_MASK));
    }


    /*******************************************************************************
    *  Function Name: Display_PrintNumber
    ********************************************************************************
    *
    * Summary:
    *  Print an uint32 value as a left-justified decimal value.
    *
    * Parameters:
    *  value:  The byte to be printed out as ASCII characters.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void Display_PrintNumber(uint16 value) 
    {

        char8 number[Display_NUMBER_OF_REMAINDERS];
        char8 temp[Display_NUMBER_OF_REMAINDERS];

        uint8 digIndex = 0u;
        uint8 numDigits;

        /* Load these in reverse order */
        while(value >= Display_TEN)
        {
            temp[digIndex] = (value % Display_TEN) + '0';
            value /= Display_TEN;
            digIndex++;
        }

        temp[digIndex] = (value % Display_TEN) + '0';
        numDigits = digIndex;

        /* While index is greater than or equal to zero copy number
        * from temporary array to number[].
        */
        while (digIndex != 0u)
        {
            number[numDigits - digIndex] = temp[digIndex];
            digIndex--;
        }
        
        /* Copy last digit */
        number[numDigits] = temp[0u];

        /* Null Termination */
        number[numDigits + 1u] = (char8) '\0';

        /* Print out number */
        Display_PrintString(&number[0u]);
    }

#endif /* Display_CONVERSION_ROUTINES == 1u */


/* [] END OF FILE */
