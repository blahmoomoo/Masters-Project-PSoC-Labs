/*******************************************************************************
* File Name: SecondTimer_PM.c
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

#include "SecondTimer.h"
static SecondTimer_backupStruct SecondTimer_backup;


/*******************************************************************************
* Function Name: SecondTimer_SaveConfig
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
*  SecondTimer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void SecondTimer_SaveConfig(void) 
{
    #if (!SecondTimer_UsingFixedFunction)
        /* Backup the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            SecondTimer_backup.TimerUdb = SecondTimer_ReadCounter();
            SecondTimer_backup.TimerPeriod = SecondTimer_ReadPeriod();
            SecondTimer_backup.InterruptMaskValue = SecondTimer_STATUS_MASK;
            #if (SecondTimer_UsingHWCaptureCounter)
                SecondTimer_backup.TimerCaptureCounter = SecondTimer_ReadCaptureCount();
            #endif /* Backup the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Backup the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            SecondTimer_backup.TimerUdb = SecondTimer_ReadCounter();
            SecondTimer_backup.InterruptMaskValue = SecondTimer_STATUS_MASK;
            #if (SecondTimer_UsingHWCaptureCounter)
                SecondTimer_backup.TimerCaptureCounter = SecondTimer_ReadCaptureCount();
            #endif /* Back Up capture counter register  */
        #endif /* Backup non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!SecondTimer_ControlRegRemoved)
            SecondTimer_backup.TimerControlRegister = SecondTimer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: SecondTimer_RestoreConfig
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
*  SecondTimer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void SecondTimer_RestoreConfig(void) 
{   
    #if (!SecondTimer_UsingFixedFunction)
        /* Restore the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            /* Interrupt State Backup for Critical Region*/
            uint8 SecondTimer_interruptState;

            SecondTimer_WriteCounter(SecondTimer_backup.TimerUdb);
            SecondTimer_WritePeriod(SecondTimer_backup.TimerPeriod);
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            SecondTimer_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            SecondTimer_STATUS_AUX_CTRL |= SecondTimer_STATUS_ACTL_INT_EN_MASK;
            /* Exit Critical Region*/
            CyExitCriticalSection(SecondTimer_interruptState);
            SecondTimer_STATUS_MASK =SecondTimer_backup.InterruptMaskValue;
            #if (SecondTimer_UsingHWCaptureCounter)
                SecondTimer_SetCaptureCount(SecondTimer_backup.TimerCaptureCounter);
            #endif /* Restore the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Restore the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            SecondTimer_WriteCounter(SecondTimer_backup.TimerUdb);
            SecondTimer_STATUS_MASK =SecondTimer_backup.InterruptMaskValue;
            #if (SecondTimer_UsingHWCaptureCounter)
                SecondTimer_SetCaptureCount(SecondTimer_backup.TimerCaptureCounter);
            #endif /* Restore Capture counter register*/
        #endif /* Restore up non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!SecondTimer_ControlRegRemoved)
            SecondTimer_WriteControlRegister(SecondTimer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: SecondTimer_Sleep
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
*  SecondTimer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void SecondTimer_Sleep(void) 
{
    #if(!SecondTimer_ControlRegRemoved)
        /* Save Counter's enable state */
        if(SecondTimer_CTRL_ENABLE == (SecondTimer_CONTROL & SecondTimer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            SecondTimer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            SecondTimer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    SecondTimer_Stop();
    SecondTimer_SaveConfig();
}


/*******************************************************************************
* Function Name: SecondTimer_Wakeup
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
*  SecondTimer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void SecondTimer_Wakeup(void) 
{
    SecondTimer_RestoreConfig();
    #if(!SecondTimer_ControlRegRemoved)
        if(SecondTimer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                SecondTimer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
