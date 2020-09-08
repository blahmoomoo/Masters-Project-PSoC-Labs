/*******************************************************************************
* File Name: StopwatchStartStatus.h  
* Version 1.80
*
* Description:
*  This file containts Status Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_STATUS_REG_StopwatchStartStatus_H) /* CY_STATUS_REG_StopwatchStartStatus_H */
#define CY_STATUS_REG_StopwatchStartStatus_H

#include "cytypes.h"
#include "CyLib.h"


/***************************************
*        Function Prototypes
***************************************/

uint8 StopwatchStartStatus_Read(void) ;
void StopwatchStartStatus_InterruptEnable(void) ;
void StopwatchStartStatus_InterruptDisable(void) ;
void StopwatchStartStatus_WriteMask(uint8 mask) ;
uint8 StopwatchStartStatus_ReadMask(void) ;


/***************************************
*           API Constants
***************************************/

#define StopwatchStartStatus_STATUS_INTR_ENBL    0x10u


/***************************************
*         Parameter Constants
***************************************/

/* Status Register Inputs */
#define StopwatchStartStatus_INPUTS              1


/***************************************
*             Registers
***************************************/

/* Status Register */
#define StopwatchStartStatus_Status             (* (reg8 *) StopwatchStartStatus_sts_sts_reg__STATUS_REG )
#define StopwatchStartStatus_Status_PTR         (  (reg8 *) StopwatchStartStatus_sts_sts_reg__STATUS_REG )
#define StopwatchStartStatus_Status_Mask        (* (reg8 *) StopwatchStartStatus_sts_sts_reg__MASK_REG )
#define StopwatchStartStatus_Status_Aux_Ctrl    (* (reg8 *) StopwatchStartStatus_sts_sts_reg__STATUS_AUX_CTL_REG )

#endif /* End CY_STATUS_REG_StopwatchStartStatus_H */


/* [] END OF FILE */
