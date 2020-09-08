/*******************************************************************************
* File Name: Stopwatch_PM.c
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

#include "Stopwatch.h"
static Stopwatch_backupStruct Stopwatch_backup;


/*******************************************************************************
* Function Name: Stopwatch_SaveConfig
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
*  Stopwatch_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Stopwatch_SaveConfig(void) 
{
    #if (!Stopwatch_UsingFixedFunction)
        /* Backup the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            Stopwatch_backup.TimerUdb = Stopwatch_ReadCounter();
            Stopwatch_backup.TimerPeriod = Stopwatch_ReadPeriod();
            Stopwatch_backup.InterruptMaskValue = Stopwatch_STATUS_MASK;
            #if (Stopwatch_UsingHWCaptureCounter)
                Stopwatch_backup.TimerCaptureCounter = Stopwatch_ReadCaptureCount();
            #endif /* Backup the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Backup the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            Stopwatch_backup.TimerUdb = Stopwatch_ReadCounter();
            Stopwatch_backup.InterruptMaskValue = Stopwatch_STATUS_MASK;
            #if (Stopwatch_UsingHWCaptureCounter)
                Stopwatch_backup.TimerCaptureCounter = Stopwatch_ReadCaptureCount();
            #endif /* Back Up capture counter register  */
        #endif /* Backup non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!Stopwatch_ControlRegRemoved)
            Stopwatch_backup.TimerControlRegister = Stopwatch_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Stopwatch_RestoreConfig
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
*  Stopwatch_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Stopwatch_RestoreConfig(void) 
{   
    #if (!Stopwatch_UsingFixedFunction)
        /* Restore the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            /* Interrupt State Backup for Critical Region*/
            uint8 Stopwatch_interruptState;

            Stopwatch_WriteCounter(Stopwatch_backup.TimerUdb);
            Stopwatch_WritePeriod(Stopwatch_backup.TimerPeriod);
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Stopwatch_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            Stopwatch_STATUS_AUX_CTRL |= Stopwatch_STATUS_ACTL_INT_EN_MASK;
            /* Exit Critical Region*/
            CyExitCriticalSection(Stopwatch_interruptState);
            Stopwatch_STATUS_MASK =Stopwatch_backup.InterruptMaskValue;
            #if (Stopwatch_UsingHWCaptureCounter)
                Stopwatch_SetCaptureCount(Stopwatch_backup.TimerCaptureCounter);
            #endif /* Restore the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Restore the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            Stopwatch_WriteCounter(Stopwatch_backup.TimerUdb);
            Stopwatch_STATUS_MASK =Stopwatch_backup.InterruptMaskValue;
            #if (Stopwatch_UsingHWCaptureCounter)
                Stopwatch_SetCaptureCount(Stopwatch_backup.TimerCaptureCounter);
            #endif /* Restore Capture counter register*/
        #endif /* Restore up non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!Stopwatch_ControlRegRemoved)
            Stopwatch_WriteControlRegister(Stopwatch_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Stopwatch_Sleep
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
*  Stopwatch_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Stopwatch_Sleep(void) 
{
    #if(!Stopwatch_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Stopwatch_CTRL_ENABLE == (Stopwatch_CONTROL & Stopwatch_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Stopwatch_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Stopwatch_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Stopwatch_Stop();
    Stopwatch_SaveConfig();
}


/*******************************************************************************
* Function Name: Stopwatch_Wakeup
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
*  Stopwatch_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Stopwatch_Wakeup(void) 
{
    Stopwatch_RestoreConfig();
    #if(!Stopwatch_ControlRegRemoved)
        if(Stopwatch_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Stopwatch_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
