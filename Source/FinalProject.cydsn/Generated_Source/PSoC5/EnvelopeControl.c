/*******************************************************************************
* File Name: EnvelopeControl.c  
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

#include "EnvelopeControl.h"

#if !defined(EnvelopeControl_Sync_ctrl_reg__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
* Function Name: EnvelopeControl_Write
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
void EnvelopeControl_Write(uint8 control) 
{
    EnvelopeControl_Control = control;
}


/*******************************************************************************
* Function Name: EnvelopeControl_Read
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
uint8 EnvelopeControl_Read(void) 
{
    return EnvelopeControl_Control;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
