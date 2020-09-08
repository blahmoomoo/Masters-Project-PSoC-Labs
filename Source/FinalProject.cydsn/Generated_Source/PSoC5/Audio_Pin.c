/*******************************************************************************
* File Name: Audio_Pin.c  
* Version 1.90
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "Audio_Pin.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 Audio_Pin__PORT == 15 && ((Audio_Pin__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: Audio_Pin_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void Audio_Pin_Write(uint8 value) 
{
    uint8 staticBits = (Audio_Pin_DR & (uint8)(~Audio_Pin_MASK));
    Audio_Pin_DR = staticBits | ((uint8)(value << Audio_Pin_SHIFT) & Audio_Pin_MASK);
}


/*******************************************************************************
* Function Name: Audio_Pin_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to this drive mode.
*
* Return: 
*  None
*
*******************************************************************************/
void Audio_Pin_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(Audio_Pin_0, mode);
}


/*******************************************************************************
* Function Name: Audio_Pin_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro Audio_Pin_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 Audio_Pin_Read(void) 
{
    return (Audio_Pin_PS & Audio_Pin_MASK) >> Audio_Pin_SHIFT;
}


/*******************************************************************************
* Function Name: Audio_Pin_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 Audio_Pin_ReadDataReg(void) 
{
    return (Audio_Pin_DR & Audio_Pin_MASK) >> Audio_Pin_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(Audio_Pin_INTSTAT) 

    /*******************************************************************************
    * Function Name: Audio_Pin_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 Audio_Pin_ClearInterrupt(void) 
    {
        return (Audio_Pin_INTSTAT & Audio_Pin_MASK) >> Audio_Pin_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
