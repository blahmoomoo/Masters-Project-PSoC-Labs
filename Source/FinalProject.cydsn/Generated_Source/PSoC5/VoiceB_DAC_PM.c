/*******************************************************************************
* File Name: VoiceB_DAC_PM.c  
* Version 1.90
*
* Description:
*  This file provides the power management source code to API for the
*  VDAC8.  
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "VoiceB_DAC.h"

static VoiceB_DAC_backupStruct VoiceB_DAC_backup;


/*******************************************************************************
* Function Name: VoiceB_DAC_SaveConfig
********************************************************************************
* Summary:
*  Save the current user configuration
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void VoiceB_DAC_SaveConfig(void) 
{
    if (!((VoiceB_DAC_CR1 & VoiceB_DAC_SRC_MASK) == VoiceB_DAC_SRC_UDB))
    {
        VoiceB_DAC_backup.data_value = VoiceB_DAC_Data;
    }
}


/*******************************************************************************
* Function Name: VoiceB_DAC_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void VoiceB_DAC_RestoreConfig(void) 
{
    if (!((VoiceB_DAC_CR1 & VoiceB_DAC_SRC_MASK) == VoiceB_DAC_SRC_UDB))
    {
        if((VoiceB_DAC_Strobe & VoiceB_DAC_STRB_MASK) == VoiceB_DAC_STRB_EN)
        {
            VoiceB_DAC_Strobe &= (uint8)(~VoiceB_DAC_STRB_MASK);
            VoiceB_DAC_Data = VoiceB_DAC_backup.data_value;
            VoiceB_DAC_Strobe |= VoiceB_DAC_STRB_EN;
        }
        else
        {
            VoiceB_DAC_Data = VoiceB_DAC_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: VoiceB_DAC_Sleep
********************************************************************************
* Summary:
*  Stop and Save the user configuration
*
* Parameters:  
*  void:  
*
* Return: 
*  void
*
* Global variables:
*  VoiceB_DAC_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void VoiceB_DAC_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(VoiceB_DAC_ACT_PWR_EN == (VoiceB_DAC_PWRMGR & VoiceB_DAC_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        VoiceB_DAC_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        VoiceB_DAC_backup.enableState = 0u;
    }
    
    VoiceB_DAC_Stop();
    VoiceB_DAC_SaveConfig();
}


/*******************************************************************************
* Function Name: VoiceB_DAC_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  VoiceB_DAC_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void VoiceB_DAC_Wakeup(void) 
{
    VoiceB_DAC_RestoreConfig();
    
    if(VoiceB_DAC_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        VoiceB_DAC_Enable();

        /* Restore the data register */
        VoiceB_DAC_SetValue(VoiceB_DAC_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
