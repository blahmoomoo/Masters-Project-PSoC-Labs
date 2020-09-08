/*******************************************************************************
* File Name: StopwatchStartFilter_PM.c
* Version 2.50
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "StopwatchStartFilter.h"
static StopwatchStartFilter_backupStruct StopwatchStartFilter_backup;


/*******************************************************************************
* Function Name: StopwatchStartFilter_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  StopwatchStartFilter_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void StopwatchStartFilter_SaveConfig(void) 
{
    #if (!StopwatchStartFilter_UsingFixedFunction)
        /* Backup the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            StopwatchStartFilter_backup.TimerUdb = StopwatchStartFilter_ReadCounter();
            StopwatchStartFilter_backup.TimerPeriod = StopwatchStartFilter_ReadPeriod();
            StopwatchStartFilter_backup.InterruptMaskValue = StopwatchStartFilter_STATUS_MASK;
            #if (StopwatchStartFilter_UsingHWCaptureCounter)
                StopwatchStartFilter_backup.TimerCaptureCounter = StopwatchStartFilter_ReadCaptureCount();
            #endif /* Backup the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Backup the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            StopwatchStartFilter_backup.TimerUdb = StopwatchStartFilter_ReadCounter();
            StopwatchStartFilter_backup.InterruptMaskValue = StopwatchStartFilter_STATUS_MASK;
            #if (StopwatchStartFilter_UsingHWCaptureCounter)
                StopwatchStartFilter_backup.TimerCaptureCounter = StopwatchStartFilter_ReadCaptureCount();
            #endif /* Back Up capture counter register  */
        #endif /* Backup non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!StopwatchStartFilter_ControlRegRemoved)
            StopwatchStartFilter_backup.TimerControlRegister = StopwatchStartFilter_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: StopwatchStartFilter_RestoreConfig
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
* Global variables:
*  StopwatchStartFilter_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void StopwatchStartFilter_RestoreConfig(void) 
{   
    #if (!StopwatchStartFilter_UsingFixedFunction)
        /* Restore the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            /* Interrupt State Backup for Critical Region*/
            uint8 StopwatchStartFilter_interruptState;

            StopwatchStartFilter_WriteCounter(StopwatchStartFilter_backup.TimerUdb);
            StopwatchStartFilter_WritePeriod(StopwatchStartFilter_backup.TimerPeriod);
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            StopwatchStartFilter_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            StopwatchStartFilter_STATUS_AUX_CTRL |= StopwatchStartFilter_STATUS_ACTL_INT_EN_MASK;
            /* Exit Critical Region*/
            CyExitCriticalSection(StopwatchStartFilter_interruptState);
            StopwatchStartFilter_STATUS_MASK =StopwatchStartFilter_backup.InterruptMaskValue;
            #if (StopwatchStartFilter_UsingHWCaptureCounter)
                StopwatchStartFilter_SetCaptureCount(StopwatchStartFilter_backup.TimerCaptureCounter);
            #endif /* Restore the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Restore the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            StopwatchStartFilter_WriteCounter(StopwatchStartFilter_backup.TimerUdb);
            StopwatchStartFilter_STATUS_MASK =StopwatchStartFilter_backup.InterruptMaskValue;
            #if (StopwatchStartFilter_UsingHWCaptureCounter)
                StopwatchStartFilter_SetCaptureCount(StopwatchStartFilter_backup.TimerCaptureCounter);
            #endif /* Restore Capture counter register*/
        #endif /* Restore up non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!StopwatchStartFilter_ControlRegRemoved)
            StopwatchStartFilter_WriteControlRegister(StopwatchStartFilter_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: StopwatchStartFilter_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  StopwatchStartFilter_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void StopwatchStartFilter_Sleep(void) 
{
    #if(!StopwatchStartFilter_ControlRegRemoved)
        /* Save Counter's enable state */
        if(StopwatchStartFilter_CTRL_ENABLE == (StopwatchStartFilter_CONTROL & StopwatchStartFilter_CTRL_ENABLE))
        {
            /* Timer is enabled */
            StopwatchStartFilter_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            StopwatchStartFilter_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    StopwatchStartFilter_Stop();
    StopwatchStartFilter_SaveConfig();
}


/*******************************************************************************
* Function Name: StopwatchStartFilter_Wakeup
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
*  StopwatchStartFilter_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void StopwatchStartFilter_Wakeup(void) 
{
    StopwatchStartFilter_RestoreConfig();
    #if(!StopwatchStartFilter_ControlRegRemoved)
        if(StopwatchStartFilter_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                StopwatchStartFilter_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
