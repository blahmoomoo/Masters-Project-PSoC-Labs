/*******************************************************************************
* File Name: NoiseGenerator_PM.c
* Version 2.40
*
* Description:
*  This file provides Sleep APIs for PRS component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "NoiseGenerator.h"

#if ((NoiseGenerator_TIME_MULTIPLEXING_ENABLE) && (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK))
    uint8 NoiseGenerator_sleepState = NoiseGenerator_NORMAL_SEQUENCE;    
#endif  /* End ((NoiseGenerator_TIME_MULTIPLEXING_ENABLE) && 
          (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK)) */


/*******************************************************************************
* Function Name: NoiseGenerator_SaveConfig
********************************************************************************
*
* Summary:
*  Saves seed and polynomial registers.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global Variables:
*  NoiseGenerator_backup - modified when non-retention registers are saved.
*
*******************************************************************************/
void NoiseGenerator_SaveConfig(void) 
{    
    #if (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK)
        /* Save dff register for time mult */
        #if (NoiseGenerator_TIME_MULTIPLEXING_ENABLE)
            NoiseGenerator_backup.dffStatus = NoiseGenerator_STATUS;
            /* Clear normal dff sequence set */
            NoiseGenerator_sleepState &= ((uint8)~NoiseGenerator_NORMAL_SEQUENCE);
        #endif  /* End NoiseGenerator_TIME_MULTIPLEXING_ENABLE */
        
        /* Save A0 and A1 registers */
        #if (NoiseGenerator_PRS_SIZE <= 32u)
            NoiseGenerator_backup.seed = NoiseGenerator_Read();
            
        #else
            NoiseGenerator_backup.seedUpper = NoiseGenerator_ReadUpper();
            NoiseGenerator_backup.seedLower = NoiseGenerator_ReadLower();
            
        #endif     /* End (NoiseGenerator_PRS_SIZE <= 32u) */
        
    #endif  /* End (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK) */
}


/*******************************************************************************
* Function Name: NoiseGenerator_Sleep
********************************************************************************
*
* Summary:
*  Stops PRS computation and saves PRS configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global Variables:
*  NoiseGenerator_backup - modified when non-retention registers are saved.
*
*******************************************************************************/
void NoiseGenerator_Sleep(void) 
{
    /* Store PRS enable state */
    if(NoiseGenerator_IS_PRS_ENABLE(NoiseGenerator_CONTROL_REG))
    {
        NoiseGenerator_backup.enableState = 1u;
        NoiseGenerator_Stop();
    }
    else
    {
        NoiseGenerator_backup.enableState = 0u;
    }
    
    NoiseGenerator_SaveConfig();
}


/*******************************************************************************
* Function Name: NoiseGenerator_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores seed and polynomial registers.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
#if (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK)
    #if (NoiseGenerator_TIME_MULTIPLEXING_ENABLE)
        void NoiseGenerator_RestoreConfig(void) 
        {   
            /* Restore A0 and A1 registers */
            #if (NoiseGenerator_PRS_SIZE <= 32u)
                NoiseGenerator_WriteSeed(NoiseGenerator_backup.seed);
            #else
                NoiseGenerator_WriteSeedUpper(NoiseGenerator_backup.seedUpper);
                NoiseGenerator_WriteSeedLower(NoiseGenerator_backup.seedLower);
            #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */
            
            #if (NoiseGenerator_RUN_MODE == NoiseGenerator__CLOCKED)
                #if (NoiseGenerator_PRS_SIZE <= 32u)
                    NoiseGenerator_ResetSeedInit(NoiseGenerator_DEFAULT_SEED);                        
                #else
                    NoiseGenerator_ResetSeedInitUpper(NoiseGenerator_DEFAULT_SEED_UPPER);
                    NoiseGenerator_ResetSeedInitLower(NoiseGenerator_DEFAULT_SEED_LOWER); 
                #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */ 
            #endif  /* End (NoiseGenerator_RUN_MODE == NoiseGenerator__CLOCKED) */
            
            /* Restore dff state for time mult: use async set/reest */
            /* Set CI, SI, SO, STATE0, STATE1 */
            NoiseGenerator_CONTROL_REG = NoiseGenerator_backup.dffStatus;
            
            /* Make pulse, to set trigger to defined state */
            NoiseGenerator_EXECUTE_DFF_SET;
            
            /* Set normal dff sequence set */
            NoiseGenerator_sleepState |= NoiseGenerator_NORMAL_SEQUENCE;
        }
        
    #else
        void NoiseGenerator_RestoreConfig(void) 
        {   
            /* Restore A0 and A1 registers */
            #if (NoiseGenerator_PRS_SIZE <= 32u)
                NoiseGenerator_WriteSeed(NoiseGenerator_backup.seed);
            #else
                NoiseGenerator_WriteSeedUpper(NoiseGenerator_backup.seedUpper);
                NoiseGenerator_WriteSeedLower(NoiseGenerator_backup.seedLower);
            #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */
            
            #if (NoiseGenerator_RUN_MODE == NoiseGenerator__CLOCKED)
                #if (NoiseGenerator_PRS_SIZE <= 32u)
                    NoiseGenerator_ResetSeedInit(NoiseGenerator_DEFAULT_SEED);                        
                #else
                    NoiseGenerator_ResetSeedInitUpper(NoiseGenerator_DEFAULT_SEED_UPPER);
                    NoiseGenerator_ResetSeedInitLower(NoiseGenerator_DEFAULT_SEED_LOWER); 
                #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */ 
            #endif  /* End (NoiseGenerator_RUN_MODE == NoiseGenerator__CLOCKED) */
        }
        
    #endif  /* End (NoiseGenerator_TIME_MULTIPLEXING_ENABLE) */
    
#else
    void NoiseGenerator_RestoreConfig(void) 
    {
        NoiseGenerator_Init();
    }
    
#endif  /* End (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK) */


/*******************************************************************************
* Function Name: NoiseGenerator_Wakeup
********************************************************************************
*
* Summary:
*  Restores PRS configuration and starts PRS computation. 
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
#if ((NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK) && (NoiseGenerator_TIME_MULTIPLEXING_ENABLE))
    void NoiseGenerator_Wakeup(void) 
#else
    void NoiseGenerator_Wakeup(void) 
#endif  /* End ((NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK) && 
                (NoiseGenerator_TIME_MULTIPLEXING_ENABLE)) */
{
    NoiseGenerator_RestoreConfig();
    
    /* Restore PRS enable state */
    if (NoiseGenerator_backup.enableState != 0u)
    {
        NoiseGenerator_Enable();
    }
}

/* [] END OF FILE */
