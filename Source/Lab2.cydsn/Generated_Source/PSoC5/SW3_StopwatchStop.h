/*******************************************************************************
* File Name: SW3_StopwatchStop.h  
* Version 1.90
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_SW3_StopwatchStop_H) /* Pins SW3_StopwatchStop_H */
#define CY_PINS_SW3_StopwatchStop_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "SW3_StopwatchStop_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_90 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 SW3_StopwatchStop__PORT == 15 && ((SW3_StopwatchStop__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    SW3_StopwatchStop_Write(uint8 value) ;
void    SW3_StopwatchStop_SetDriveMode(uint8 mode) ;
uint8   SW3_StopwatchStop_ReadDataReg(void) ;
uint8   SW3_StopwatchStop_Read(void) ;
uint8   SW3_StopwatchStop_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define SW3_StopwatchStop_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define SW3_StopwatchStop_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define SW3_StopwatchStop_DM_RES_UP          PIN_DM_RES_UP
#define SW3_StopwatchStop_DM_RES_DWN         PIN_DM_RES_DWN
#define SW3_StopwatchStop_DM_OD_LO           PIN_DM_OD_LO
#define SW3_StopwatchStop_DM_OD_HI           PIN_DM_OD_HI
#define SW3_StopwatchStop_DM_STRONG          PIN_DM_STRONG
#define SW3_StopwatchStop_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define SW3_StopwatchStop_MASK               SW3_StopwatchStop__MASK
#define SW3_StopwatchStop_SHIFT              SW3_StopwatchStop__SHIFT
#define SW3_StopwatchStop_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define SW3_StopwatchStop_PS                     (* (reg8 *) SW3_StopwatchStop__PS)
/* Data Register */
#define SW3_StopwatchStop_DR                     (* (reg8 *) SW3_StopwatchStop__DR)
/* Port Number */
#define SW3_StopwatchStop_PRT_NUM                (* (reg8 *) SW3_StopwatchStop__PRT) 
/* Connect to Analog Globals */                                                  
#define SW3_StopwatchStop_AG                     (* (reg8 *) SW3_StopwatchStop__AG)                       
/* Analog MUX bux enable */
#define SW3_StopwatchStop_AMUX                   (* (reg8 *) SW3_StopwatchStop__AMUX) 
/* Bidirectional Enable */                                                        
#define SW3_StopwatchStop_BIE                    (* (reg8 *) SW3_StopwatchStop__BIE)
/* Bit-mask for Aliased Register Access */
#define SW3_StopwatchStop_BIT_MASK               (* (reg8 *) SW3_StopwatchStop__BIT_MASK)
/* Bypass Enable */
#define SW3_StopwatchStop_BYP                    (* (reg8 *) SW3_StopwatchStop__BYP)
/* Port wide control signals */                                                   
#define SW3_StopwatchStop_CTL                    (* (reg8 *) SW3_StopwatchStop__CTL)
/* Drive Modes */
#define SW3_StopwatchStop_DM0                    (* (reg8 *) SW3_StopwatchStop__DM0) 
#define SW3_StopwatchStop_DM1                    (* (reg8 *) SW3_StopwatchStop__DM1)
#define SW3_StopwatchStop_DM2                    (* (reg8 *) SW3_StopwatchStop__DM2) 
/* Input Buffer Disable Override */
#define SW3_StopwatchStop_INP_DIS                (* (reg8 *) SW3_StopwatchStop__INP_DIS)
/* LCD Common or Segment Drive */
#define SW3_StopwatchStop_LCD_COM_SEG            (* (reg8 *) SW3_StopwatchStop__LCD_COM_SEG)
/* Enable Segment LCD */
#define SW3_StopwatchStop_LCD_EN                 (* (reg8 *) SW3_StopwatchStop__LCD_EN)
/* Slew Rate Control */
#define SW3_StopwatchStop_SLW                    (* (reg8 *) SW3_StopwatchStop__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define SW3_StopwatchStop_PRTDSI__CAPS_SEL       (* (reg8 *) SW3_StopwatchStop__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define SW3_StopwatchStop_PRTDSI__DBL_SYNC_IN    (* (reg8 *) SW3_StopwatchStop__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define SW3_StopwatchStop_PRTDSI__OE_SEL0        (* (reg8 *) SW3_StopwatchStop__PRTDSI__OE_SEL0) 
#define SW3_StopwatchStop_PRTDSI__OE_SEL1        (* (reg8 *) SW3_StopwatchStop__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define SW3_StopwatchStop_PRTDSI__OUT_SEL0       (* (reg8 *) SW3_StopwatchStop__PRTDSI__OUT_SEL0) 
#define SW3_StopwatchStop_PRTDSI__OUT_SEL1       (* (reg8 *) SW3_StopwatchStop__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define SW3_StopwatchStop_PRTDSI__SYNC_OUT       (* (reg8 *) SW3_StopwatchStop__PRTDSI__SYNC_OUT) 


#if defined(SW3_StopwatchStop__INTSTAT)  /* Interrupt Registers */

    #define SW3_StopwatchStop_INTSTAT                (* (reg8 *) SW3_StopwatchStop__INTSTAT)
    #define SW3_StopwatchStop_SNAP                   (* (reg8 *) SW3_StopwatchStop__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_SW3_StopwatchStop_H */


/* [] END OF FILE */
