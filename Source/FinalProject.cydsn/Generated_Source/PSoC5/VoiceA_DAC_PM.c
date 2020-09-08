/*******************************************************************************
* File Name: VoiceA_DAC_PM.c  
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

#include "VoiceA_DAC.h"

static VoiceA_DAC_backupStruct VoiceA_DAC_backup;


/*******************************************************************************
* Function Name: VoiceA_DAC_SaveConfig
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
void VoiceA_DAC_SaveConfig(void) 
{
    if (!((VoiceA_DAC_CR1 & VoiceA_DAC_SRC_MASK) == VoiceA_DAC_SRC_UDB))
    {
        VoiceA_DAC_backup.data_value = VoiceA_DAC_Data;
    }
}


/*******************************************************************************
* Function Name: VoiceA_DAC_RestoreConfig
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
void VoiceA_DAC_RestoreConfig(void) 
{
    if (!((VoiceA_DAC_CR1 & VoiceA_DAC_SRC_MASK) == VoiceA_DAC_SRC_UDB))
    {
        if((VoiceA_DAC_Strobe & VoiceA_DAC_STRB_MASK) == VoiceA_DAC_STRB_EN)
        {
            VoiceA_DAC_Strobe &= (uint8)(~VoiceA_DAC_STRB_MASK);
            VoiceA_DAC_Data = VoiceA_DAC_backup.data_value;
            VoiceA_DAC_Strobe |= VoiceA_DAC_STRB_EN;
        }
        else
        {
            VoiceA_DAC_Data = VoiceA_DAC_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: VoiceA_DAC_Sleep
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
*  VoiceA_DAC_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void VoiceA_DAC_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(VoiceA_DAC_ACT_PWR_EN == (VoiceA_DAC_PWRMGR & VoiceA_DAC_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        VoiceA_DAC_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        VoiceA_DAC_backup.enableState = 0u;
    }
    
    VoiceA_DAC_Stop();
    VoiceA_DAC_SaveConfig();
}


/*******************************************************************************
* Function Name: VoiceA_DAC_Wakeup
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
*  VoiceA_DAC_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void VoiceA_DAC_Wakeup(void) 
{
    VoiceA_DAC_RestoreConfig();
    
    if(VoiceA_DAC_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        VoiceA_DAC_Enable();

        /* Restore the data register */
        VoiceA_DAC_SetValue(VoiceA_DAC_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
