/*******************************************************************************
* File Name: VoiceReading.c  
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

#include "VoiceReading.h"

#if !defined(VoiceReading_sts_sts_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: VoiceReading_Read
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
uint8 VoiceReading_Read(void) 
{ 
    return VoiceReading_Status;
}


/*******************************************************************************
* Function Name: VoiceReading_InterruptEnable
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
void VoiceReading_InterruptEnable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    VoiceReading_Status_Aux_Ctrl |= VoiceReading_STATUS_INTR_ENBL;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: VoiceReading_InterruptDisable
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
void VoiceReading_InterruptDisable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    VoiceReading_Status_Aux_Ctrl &= (uint8)(~VoiceReading_STATUS_INTR_ENBL);
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: VoiceReading_WriteMask
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
void VoiceReading_WriteMask(uint8 mask) 
{
    #if(VoiceReading_INPUTS < 8u)
    	mask &= (uint8)((((uint8)1u) << VoiceReading_INPUTS) - 1u);
	#endif /* End VoiceReading_INPUTS < 8u */
    VoiceReading_Status_Mask = mask;
}


/*******************************************************************************
* Function Name: VoiceReading_ReadMask
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
uint8 VoiceReading_ReadMask(void) 
{
    return VoiceReading_Status_Mask;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
