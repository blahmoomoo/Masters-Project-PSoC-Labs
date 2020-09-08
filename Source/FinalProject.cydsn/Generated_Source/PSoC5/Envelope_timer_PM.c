/*******************************************************************************
* File Name: Envelope_timer_PM.c
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

#include "Envelope_timer.h"
static Envelope_timer_backupStruct Envelope_timer_backup;


/*******************************************************************************
* Function Name: Envelope_timer_SaveConfig
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
*  Envelope_timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Envelope_timer_SaveConfig(void) 
{
    #if (!Envelope_timer_UsingFixedFunction)
        /* Backup the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            Envelope_timer_backup.TimerUdb = Envelope_timer_ReadCounter();
            Envelope_timer_backup.TimerPeriod = Envelope_timer_ReadPeriod();
            Envelope_timer_backup.InterruptMaskValue = Envelope_timer_STATUS_MASK;
            #if (Envelope_timer_UsingHWCaptureCounter)
                Envelope_timer_backup.TimerCaptureCounter = Envelope_timer_ReadCaptureCount();
            #endif /* Backup the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Backup the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            Envelope_timer_backup.TimerUdb = Envelope_timer_ReadCounter();
            Envelope_timer_backup.InterruptMaskValue = Envelope_timer_STATUS_MASK;
            #if (Envelope_timer_UsingHWCaptureCounter)
                Envelope_timer_backup.TimerCaptureCounter = Envelope_timer_ReadCaptureCount();
            #endif /* Back Up capture counter register  */
        #endif /* Backup non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!Envelope_timer_ControlRegRemoved)
            Envelope_timer_backup.TimerControlRegister = Envelope_timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Envelope_timer_RestoreConfig
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
*  Envelope_timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Envelope_timer_RestoreConfig(void) 
{   
    #if (!Envelope_timer_UsingFixedFunction)
        /* Restore the UDB non-rentention registers for CY_UDB_V0 */
        #if (CY_UDB_V0)
            /* Interrupt State Backup for Critical Region*/
            uint8 Envelope_timer_interruptState;

            Envelope_timer_WriteCounter(Envelope_timer_backup.TimerUdb);
            Envelope_timer_WritePeriod(Envelope_timer_backup.TimerPeriod);
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Envelope_timer_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            Envelope_timer_STATUS_AUX_CTRL |= Envelope_timer_STATUS_ACTL_INT_EN_MASK;
            /* Exit Critical Region*/
            CyExitCriticalSection(Envelope_timer_interruptState);
            Envelope_timer_STATUS_MASK =Envelope_timer_backup.InterruptMaskValue;
            #if (Envelope_timer_UsingHWCaptureCounter)
                Envelope_timer_SetCaptureCount(Envelope_timer_backup.TimerCaptureCounter);
            #endif /* Restore the UDB non-rentention register capture counter for CY_UDB_V0 */
        #endif /* Restore the UDB non-rentention registers for CY_UDB_V0 */

        #if (CY_UDB_V1)
            Envelope_timer_WriteCounter(Envelope_timer_backup.TimerUdb);
            Envelope_timer_STATUS_MASK =Envelope_timer_backup.InterruptMaskValue;
            #if (Envelope_timer_UsingHWCaptureCounter)
                Envelope_timer_SetCaptureCount(Envelope_timer_backup.TimerCaptureCounter);
            #endif /* Restore Capture counter register*/
        #endif /* Restore up non retention registers, interrupt mask and capture counter for CY_UDB_V1 */

        #if(!Envelope_timer_ControlRegRemoved)
            Envelope_timer_WriteControlRegister(Envelope_timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Envelope_timer_Sleep
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
*  Envelope_timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Envelope_timer_Sleep(void) 
{
    #if(!Envelope_timer_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Envelope_timer_CTRL_ENABLE == (Envelope_timer_CONTROL & Envelope_timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Envelope_timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Envelope_timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Envelope_timer_Stop();
    Envelope_timer_SaveConfig();
}


/*******************************************************************************
* Function Name: Envelope_timer_Wakeup
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
*  Envelope_timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Envelope_timer_Wakeup(void) 
{
    Envelope_timer_RestoreConfig();
    #if(!Envelope_timer_ControlRegRemoved)
        if(Envelope_timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Envelope_timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
