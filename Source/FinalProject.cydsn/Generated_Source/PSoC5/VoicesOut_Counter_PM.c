/*******************************************************************************
* File Name: VoicesOut_Counter_PM.c
* Version 1.0
*
* Description:
*  This file provides Low power mode APIs for Count7 component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "VoicesOut_Counter.h"


VoicesOut_Counter_BACKUP_STRUCT VoicesOut_Counter_backup;


/*******************************************************************************
* Function Name: VoicesOut_Counter_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component configuration and non-retention registers.
*  This function is called by the Count7_Sleep() function.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  VoicesOut_Counter_backup - used to save component configuration and non-
*  retention registers before enter sleep mode.
*
*******************************************************************************/
void VoicesOut_Counter_SaveConfig(void) 
{
    VoicesOut_Counter_backup.count = VoicesOut_Counter_COUNT_REG;
}


/*******************************************************************************
* Function Name: VoicesOut_Counter_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for low power mode
*  operation. The Count7_Sleep() API saves the current component state using
*  Count7_SaveConfig() and disables the counter.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void VoicesOut_Counter_Sleep(void) 
{
    if(0u != (VoicesOut_Counter_AUX_CONTROL_REG & VoicesOut_Counter_COUNTER_START))
    {
        VoicesOut_Counter_backup.enableState = 1u;
        VoicesOut_Counter_Stop();
    }
    else
    {
        VoicesOut_Counter_backup.enableState = 0u;
    }

    VoicesOut_Counter_SaveConfig();
}


/*******************************************************************************
* Function Name: VoicesOut_Counter_RestoreConfig
********************************************************************************
*
* Summary:
*  This function restores the component configuration and non-retention
*  registers. This function is called by the Count7_Wakeup() function.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global Variables:
*  VoicesOut_Counter_backup - used to save component configuration and
*  non-retention registers before exit sleep mode.
*
*******************************************************************************/
void VoicesOut_Counter_RestoreConfig(void) 
{
    VoicesOut_Counter_COUNT_REG = VoicesOut_Counter_backup.count;
}


/*******************************************************************************
* Function Name: VoicesOut_Counter_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when
*  Count7_Sleep() was called. The Count7_Wakeup() function calls the
*  Count7_RestoreConfig() function to restore the configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void VoicesOut_Counter_Wakeup(void) 
{
    VoicesOut_Counter_RestoreConfig();

    /* Restore enable state */
    if (VoicesOut_Counter_backup.enableState != 0u)
    {
        VoicesOut_Counter_Enable();
    }
}


/* [] END OF FILE */
