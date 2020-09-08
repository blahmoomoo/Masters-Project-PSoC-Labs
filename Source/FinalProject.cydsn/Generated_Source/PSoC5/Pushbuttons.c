/*******************************************************************************
* File Name: Pushbuttons.c  
* Version 1.80
*
* Description:
*  This file contains API to enable firmware to read the value of a Status 
*  Register.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Pushbuttons.h"

#if !defined(Pushbuttons_sts_sts_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: Pushbuttons_Read
********************************************************************************
*
* Summary:
*  Reads the current value assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The current value in the Status Register.
*
*******************************************************************************/
uint8 Pushbuttons_Read(void) 
{ 
    return Pushbuttons_Status;
}


/*******************************************************************************
* Function Name: Pushbuttons_InterruptEnable
********************************************************************************
*
* Summary:
*  Enables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Pushbuttons_InterruptEnable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    Pushbuttons_Status_Aux_Ctrl |= Pushbuttons_STATUS_INTR_ENBL;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Pushbuttons_InterruptDisable
********************************************************************************
*
* Summary:
*  Disables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Pushbuttons_InterruptDisable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    Pushbuttons_Status_Aux_Ctrl &= (uint8)(~Pushbuttons_STATUS_INTR_ENBL);
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Pushbuttons_WriteMask
********************************************************************************
*
* Summary:
*  Writes the current mask value assigned to the Status Register.
*
* Parameters:
*  mask:  Value to write into the mask register.
*
* Return:
*  None.
*
*******************************************************************************/
void Pushbuttons_WriteMask(uint8 mask) 
{
    #if(Pushbuttons_INPUTS < 8u)
    	mask &= (uint8)((((uint8)1u) << Pushbuttons_INPUTS) - 1u);
	#endif /* End Pushbuttons_INPUTS < 8u */
    Pushbuttons_Status_Mask = mask;
}


/*******************************************************************************
* Function Name: Pushbuttons_ReadMask
********************************************************************************
*
* Summary:
*  Reads the current interrupt mask assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The value of the interrupt mask of the Status Register.
*
*******************************************************************************/
uint8 Pushbuttons_ReadMask(void) 
{
    return Pushbuttons_Status_Mask;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
