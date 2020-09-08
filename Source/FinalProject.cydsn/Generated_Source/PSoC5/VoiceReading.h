/*******************************************************************************
* File Name: VoiceReading.h  
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

#if !defined(CY_STATUS_REG_VoiceReading_H) /* CY_STATUS_REG_VoiceReading_H */
#define CY_STATUS_REG_VoiceReading_H

#include "cytypes.h"
#include "CyLib.h"


/***************************************
*        Function Prototypes
***************************************/

uint8 VoiceReading_Read(void) ;
void VoiceReading_InterruptEnable(void) ;
void VoiceReading_InterruptDisable(void) ;
void VoiceReading_WriteMask(uint8 mask) ;
uint8 VoiceReading_ReadMask(void) ;


/***************************************
*           API Constants
***************************************/

#define VoiceReading_STATUS_INTR_ENBL    0x10u


/***************************************
*         Parameter Constants
***************************************/

/* Status Register Inputs */
#define VoiceReading_INPUTS              4


/***************************************
*             Registers
***************************************/

/* Status Register */
#define VoiceReading_Status             (* (reg8 *) VoiceReading_sts_sts_reg__STATUS_REG )
#define VoiceReading_Status_PTR         (  (reg8 *) VoiceReading_sts_sts_reg__STATUS_REG )
#define VoiceReading_Status_Mask        (* (reg8 *) VoiceReading_sts_sts_reg__MASK_REG )
#define VoiceReading_Status_Aux_Ctrl    (* (reg8 *) VoiceReading_sts_sts_reg__STATUS_AUX_CTL_REG )

#endif /* End CY_STATUS_REG_VoiceReading_H */


/* [] END OF FILE */
