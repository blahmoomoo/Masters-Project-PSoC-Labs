/*******************************************************************************
* File Name: Envelope_counter_PM.c  
* Version 2.40
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
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

#include "Envelope_counter.h"

static Envelope_counter_backupStruct Envelope_counter_backup;


/*******************************************************************************
* Function Name: Envelope_counter_SaveConfig
********************************************************************************
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
*  Envelope_counter_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Envelope_counter_SaveConfig(void) 
{
    #if (!Envelope_counter_UsingFixedFunction)

        Envelope_counter_backup.CounterUdb = Envelope_counter_ReadCounter();

        #if (CY_UDB_V0)
            Envelope_counter_backup.CounterPeriod = Envelope_counter_ReadPeriod();
            Envelope_counter_backup.CompareValue = Envelope_counter_ReadCompare();
            Envelope_counter_backup.InterruptMaskValue = Envelope_counter_STATUS_MASK;
        #endif /* CY_UDB_V0 */

        #if(!Envelope_counter_ControlRegRemoved)
            Envelope_counter_backup.CounterControlRegister = Envelope_counter_ReadControlRegister();
        #endif /* (!Envelope_counter_ControlRegRemoved) */

    #endif /* (!Envelope_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Envelope_counter_RestoreConfig
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
*  Envelope_counter_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Envelope_counter_RestoreConfig(void) 
{      
    #if (!Envelope_counter_UsingFixedFunction)

        #if (CY_UDB_V0)
            uint8 Envelope_counter_interruptState;
        #endif  /* (CY_UDB_V0) */

       Envelope_counter_WriteCounter(Envelope_counter_backup.CounterUdb);

        #if (CY_UDB_V0)
            Envelope_counter_WritePeriod(Envelope_counter_backup.CounterPeriod);
            Envelope_counter_WriteCompare(Envelope_counter_backup.CompareValue);

            Envelope_counter_interruptState = CyEnterCriticalSection();
            Envelope_counter_STATUS_AUX_CTRL |= Envelope_counter_STATUS_ACTL_INT_EN_MASK;
            CyExitCriticalSection(Envelope_counter_interruptState);

            Envelope_counter_STATUS_MASK = Envelope_counter_backup.InterruptMaskValue;
        #endif  /* (CY_UDB_V0) */

        #if(!Envelope_counter_ControlRegRemoved)
            Envelope_counter_WriteControlRegister(Envelope_counter_backup.CounterControlRegister);
        #endif /* (!Envelope_counter_ControlRegRemoved) */

    #endif /* (!Envelope_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Envelope_counter_Sleep
********************************************************************************
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
*  Envelope_counter_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Envelope_counter_Sleep(void) 
{
    #if(!Envelope_counter_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Envelope_counter_CTRL_ENABLE == (Envelope_counter_CONTROL & Envelope_counter_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Envelope_counter_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Envelope_counter_backup.CounterEnableState = 0u;
        }
    #else
        Envelope_counter_backup.CounterEnableState = 1u;
        if(Envelope_counter_backup.CounterEnableState != 0u)
        {
            Envelope_counter_backup.CounterEnableState = 0u;
        }
    #endif /* (!Envelope_counter_ControlRegRemoved) */
    
    Envelope_counter_Stop();
    Envelope_counter_SaveConfig();
}


/*******************************************************************************
* Function Name: Envelope_counter_Wakeup
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
*  Envelope_counter_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Envelope_counter_Wakeup(void) 
{
    Envelope_counter_RestoreConfig();
    #if(!Envelope_counter_ControlRegRemoved)
        if(Envelope_counter_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Envelope_counter_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Envelope_counter_ControlRegRemoved) */
    
}


/* [] END OF FILE */
