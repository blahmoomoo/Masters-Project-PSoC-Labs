/*******************************************************************************
* File Name: VoiceC_DAC_PM.c  
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

#include "VoiceC_DAC.h"

static VoiceC_DAC_backupStruct VoiceC_DAC_backup;


/*******************************************************************************
* Function Name: VoiceC_DAC_SaveConfig
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
void VoiceC_DAC_SaveConfig(void) 
{
    if (!((VoiceC_DAC_CR1 & VoiceC_DAC_SRC_MASK) == VoiceC_DAC_SRC_UDB))
    {
        VoiceC_DAC_backup.data_value = VoiceC_DAC_Data;
    }
}


/*******************************************************************************
* Function Name: VoiceC_DAC_RestoreConfig
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
void VoiceC_DAC_RestoreConfig(void) 
{
    if (!((VoiceC_DAC_CR1 & VoiceC_DAC_SRC_MASK) == VoiceC_DAC_SRC_UDB))
    {
        if((VoiceC_DAC_Strobe & VoiceC_DAC_STRB_MASK) == VoiceC_DAC_STRB_EN)
        {
            VoiceC_DAC_Strobe &= (uint8)(~VoiceC_DAC_STRB_MASK);
            VoiceC_DAC_Data = VoiceC_DAC_backup.data_value;
            VoiceC_DAC_Strobe |= VoiceC_DAC_STRB_EN;
        }
        else
        {
            VoiceC_DAC_Data = VoiceC_DAC_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: VoiceC_DAC_Sleep
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
*  VoiceC_DAC_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void VoiceC_DAC_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(VoiceC_DAC_ACT_PWR_EN == (VoiceC_DAC_PWRMGR & VoiceC_DAC_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        VoiceC_DAC_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        VoiceC_DAC_backup.enableState = 0u;
    }
    
    VoiceC_DAC_Stop();
    VoiceC_DAC_SaveConfig();
}


/*******************************************************************************
* Function Name: VoiceC_DAC_Wakeup
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
*  VoiceC_DAC_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void VoiceC_DAC_Wakeup(void) 
{
    VoiceC_DAC_RestoreConfig();
    
    if(VoiceC_DAC_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        VoiceC_DAC_Enable();

        /* Restore the data register */
        VoiceC_DAC_SetValue(VoiceC_DAC_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
