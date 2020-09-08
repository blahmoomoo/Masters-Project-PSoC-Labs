/*******************************************************************************
* File Name: VoiceA_Amplitude.c  
* Version 1.70
*
* Description:
*  This file contains API to enable firmware control of a Control Register.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "VoiceA_Amplitude.h"

#if !defined(VoiceA_Amplitude_Sync_ctrl_reg__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
* Function Name: VoiceA_Amplitude_Write
********************************************************************************
*
* Summary:
*  Write a byte to the Control Register.
*
* Parameters:
*  control:  The value to be assigned to the Control Register.
*
* Return:
*  None.
*
*******************************************************************************/
void VoiceA_Amplitude_Write(uint8 control) 
{
    VoiceA_Amplitude_Control = control;
}


/*******************************************************************************
* Function Name: VoiceA_Amplitude_Read
********************************************************************************
*
* Summary:
*  Reads the current value assigned to the Control Register.
*
* Parameters:
*  None.
*
* Return:
*  Returns the current value in the Control Register.
*
*******************************************************************************/
uint8 VoiceA_Amplitude_Read(void) 
{
    return VoiceA_Amplitude_Control;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
