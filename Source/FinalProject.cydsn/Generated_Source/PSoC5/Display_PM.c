/*******************************************************************************
* File Name: Display_PM.c
* Version 1.90
*
* Description:
*  This file provides the API source code for the Static Segment LCD component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Display.h"


static Display_BACKUP_STRUCT Display_backup;


/*******************************************************************************
* Function Name: Display_SaveConfig
********************************************************************************
*
* Summary:
*  Does nothing, provided for consistency.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Display_SaveConfig(void) 
{
}


/*******************************************************************************
* Function Name: Display_RestoreConfig
********************************************************************************
*
* Summary:
*  Does nothing, provided for consistency.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Display_RestoreConfig(void) 
{
}


/*******************************************************************************
* Function Name: Display_Sleep
********************************************************************************
*
* Summary:
*  Prepares component for entering the sleep mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Display_Sleep(void) 
{
    Display_backup.enableState = Display_enableState;
    Display_SaveConfig();
    Display_Stop();
}


/*******************************************************************************
* Function Name: Display_Wakeup
********************************************************************************
*
* Summary:
*  Wakes component from sleep mode. Configures DMA and enables the component for
*  normal operation.
*
* Parameters:
*  Display_enableState - Global variable.
*
* Return:
*  Status one of standard status for PSoC3 Component
*       CYRET_SUCCESS - Function completed successfully.
*       CYRET_LOCKED - The object was locked, already in use. Some of TDs or
*                      a channel already in use.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Display_Wakeup(void) 
{
    Display_RestoreConfig();

    if(Display_backup.enableState == 1u)
    {
        Display_Enable();
    }
}


/* [] END OF FILE */
