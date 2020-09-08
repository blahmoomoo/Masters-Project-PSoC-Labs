/*******************************************************************************
* File Name: Display_LCDPort.h  
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

#if !defined(CY_PINS_Display_LCDPort_H) /* Pins Display_LCDPort_H */
#define CY_PINS_Display_LCDPort_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Display_LCDPort_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_90 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Display_LCDPort__PORT == 15 && ((Display_LCDPort__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    Display_LCDPort_Write(uint8 value) ;
void    Display_LCDPort_SetDriveMode(uint8 mode) ;
uint8   Display_LCDPort_ReadDataReg(void) ;
uint8   Display_LCDPort_Read(void) ;
uint8   Display_LCDPort_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Display_LCDPort_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Display_LCDPort_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Display_LCDPort_DM_RES_UP          PIN_DM_RES_UP
#define Display_LCDPort_DM_RES_DWN         PIN_DM_RES_DWN
#define Display_LCDPort_DM_OD_LO           PIN_DM_OD_LO
#define Display_LCDPort_DM_OD_HI           PIN_DM_OD_HI
#define Display_LCDPort_DM_STRONG          PIN_DM_STRONG
#define Display_LCDPort_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Display_LCDPort_MASK               Display_LCDPort__MASK
#define Display_LCDPort_SHIFT              Display_LCDPort__SHIFT
#define Display_LCDPort_WIDTH              7u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Display_LCDPort_PS                     (* (reg8 *) Display_LCDPort__PS)
/* Data Register */
#define Display_LCDPort_DR                     (* (reg8 *) Display_LCDPort__DR)
/* Port Number */
#define Display_LCDPort_PRT_NUM                (* (reg8 *) Display_LCDPort__PRT) 
/* Connect to Analog Globals */                                                  
#define Display_LCDPort_AG                     (* (reg8 *) Display_LCDPort__AG)                       
/* Analog MUX bux enable */
#define Display_LCDPort_AMUX                   (* (reg8 *) Display_LCDPort__AMUX) 
/* Bidirectional Enable */                                                        
#define Display_LCDPort_BIE                    (* (reg8 *) Display_LCDPort__BIE)
/* Bit-mask for Aliased Register Access */
#define Display_LCDPort_BIT_MASK               (* (reg8 *) Display_LCDPort__BIT_MASK)
/* Bypass Enable */
#define Display_LCDPort_BYP                    (* (reg8 *) Display_LCDPort__BYP)
/* Port wide control signals */                                                   
#define Display_LCDPort_CTL                    (* (reg8 *) Display_LCDPort__CTL)
/* Drive Modes */
#define Display_LCDPort_DM0                    (* (reg8 *) Display_LCDPort__DM0) 
#define Display_LCDPort_DM1                    (* (reg8 *) Display_LCDPort__DM1)
#define Display_LCDPort_DM2                    (* (reg8 *) Display_LCDPort__DM2) 
/* Input Buffer Disable Override */
#define Display_LCDPort_INP_DIS                (* (reg8 *) Display_LCDPort__INP_DIS)
/* LCD Common or Segment Drive */
#define Display_LCDPort_LCD_COM_SEG            (* (reg8 *) Display_LCDPort__LCD_COM_SEG)
/* Enable Segment LCD */
#define Display_LCDPort_LCD_EN                 (* (reg8 *) Display_LCDPort__LCD_EN)
/* Slew Rate Control */
#define Display_LCDPort_SLW                    (* (reg8 *) Display_LCDPort__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Display_LCDPort_PRTDSI__CAPS_SEL       (* (reg8 *) Display_LCDPort__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Display_LCDPort_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Display_LCDPort__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Display_LCDPort_PRTDSI__OE_SEL0        (* (reg8 *) Display_LCDPort__PRTDSI__OE_SEL0) 
#define Display_LCDPort_PRTDSI__OE_SEL1        (* (reg8 *) Display_LCDPort__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Display_LCDPort_PRTDSI__OUT_SEL0       (* (reg8 *) Display_LCDPort__PRTDSI__OUT_SEL0) 
#define Display_LCDPort_PRTDSI__OUT_SEL1       (* (reg8 *) Display_LCDPort__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Display_LCDPort_PRTDSI__SYNC_OUT       (* (reg8 *) Display_LCDPort__PRTDSI__SYNC_OUT) 


#if defined(Display_LCDPort__INTSTAT)  /* Interrupt Registers */

    #define Display_LCDPort_INTSTAT                (* (reg8 *) Display_LCDPort__INTSTAT)
    #define Display_LCDPort_SNAP                   (* (reg8 *) Display_LCDPort__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Display_LCDPort_H */


/* [] END OF FILE */
