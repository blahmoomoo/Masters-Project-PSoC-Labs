/*******************************************************************************
* File Name: DisplayRefresh_PM.c
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

#include "DisplayRefresh.h"
static DisplayRefresh_backupStruct DisplayRefresh_backup;


/*******************************************************************************
* Function Name: DisplayRefresh_SaveConfig
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
*  DisplayRefresh_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void DisplayRefresh_SaveConfig(void) 
{
    #if (!DisplayRefresh_UsingFixedFunction)
        /* Backup the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            DisplayRefresh_backup.TimerUdb = DisplayRefresh_ReadCounter();
            DisplayRefresh_backup.TimerPeriod = DisplayRefresh_ReadPeriod();
            DisplayRefresh_backup.InterruptMaskValue = DisplayRefresh_STATUS_MASK;
            #if (DisplayRefresh_UsingHWCaptureCounter)
                DisplayRefresh_backup.TimerCaptureCounter = DisplayRefresh_ReadCaptureCount();
            #endif /* Backup the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Backup the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            DisplayRefresh_backup.TimerUdb = DisplayRefresh_ReadCounter();
            DisplayRefresh_backup.InterruptMaskValue = DisplayRefresh_STATUS_MASK;
            #if (DisplayRefresh_UsingHWCaptureCounter)
                DisplayRefresh_backup.TimerCaptureCounter = DisplayRefresh_ReadCaptureCount();
            #endif /* Back Up capture counter register  */
        #endif /* Backup non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!DisplayRefresh_ControlRegRemoved)
            DisplayRefresh_backup.TimerControlRegister = DisplayRefresh_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: DisplayRefresh_RestoreConfig
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
*  DisplayRefresh_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void DisplayRefresh_RestoreConfig(void) 
{   
    #if (!DisplayRefresh_UsingFixedFunction)
        /* Restore the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            /* Interrupt State Backup for Critical Region*/
            uint8 DisplayRefresh_interruptState;

            DisplayRefresh_WriteCounter(DisplayRefresh_backup.TimerUdb);
            DisplayRefresh_WritePeriod(DisplayRefresh_backup.TimerPeriod);
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            DisplayRefresh_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            DisplayRefresh_STATUS_AUX_CTRL |= DisplayRefresh_STATUS_ACTL_INT_EN_MASK;
            /* Exit Critical Region*/
            CyExitCriticalSection(DisplayRefresh_interruptState);
            DisplayRefresh_STATUS_MASK =DisplayRefresh_backup.InterruptMaskValue;
            #if (DisplayRefresh_UsingHWCaptureCounter)
                DisplayRefresh_SetCaptureCount(DisplayRefresh_backup.TimerCaptureCounter);
            #endif /* Restore the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Restore the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            DisplayRefresh_WriteCounter(DisplayRefresh_backup.TimerUdb);
            DisplayRefresh_STATUS_MASK =DisplayRefresh_backup.InterruptMaskValue;
            #if (DisplayRefresh_UsingHWCaptureCounter)
                DisplayRefresh_SetCaptureCount(DisplayRefresh_backup.TimerCaptureCounter);
            #endif /* Restore Capture counter register*/
        #endif /* Restore up non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!DisplayRefresh_ControlRegRemoved)
            DisplayRefresh_WriteControlRegister(DisplayRefresh_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: DisplayRefresh_Sleep
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
*  DisplayRefresh_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void DisplayRefresh_Sleep(void) 
{
    #if(!DisplayRefresh_ControlRegRemoved)
        /* Save Counter's enable state */
        if(DisplayRefresh_CTRL_ENABLE == (DisplayRefresh_CONTROL & DisplayRefresh_CTRL_ENABLE))
        {
            /* Timer is enabled */
            DisplayRefresh_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            DisplayRefresh_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    DisplayRefresh_Stop();
    DisplayRefresh_SaveConfig();
}


/*******************************************************************************
* Function Name: DisplayRefresh_Wakeup
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
*  DisplayRefresh_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void DisplayRefresh_Wakeup(void) 
{
    DisplayRefresh_RestoreConfig();
    #if(!DisplayRefresh_ControlRegRemoved)
        if(DisplayRefresh_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                DisplayRefresh_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
