/*******************************************************************************
* File Name: Pushbutton.h  
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

#if !defined(CY_PINS_Pushbutton_H) /* Pins Pushbutton_H */
#define CY_PINS_Pushbutton_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pushbutton_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v1_90 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pushbutton__PORT == 15 && ((Pushbutton__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    Pushbutton_Write(uint8 value) ;
void    Pushbutton_SetDriveMode(uint8 mode) ;
uint8   Pushbutton_ReadDataReg(void) ;
uint8   Pushbutton_Read(void) ;
uint8   Pushbutton_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Pushbutton_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Pushbutton_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Pushbutton_DM_RES_UP          PIN_DM_RES_UP
#define Pushbutton_DM_RES_DWN         PIN_DM_RES_DWN
#define Pushbutton_DM_OD_LO           PIN_DM_OD_LO
#define Pushbutton_DM_OD_HI           PIN_DM_OD_HI
#define Pushbutton_DM_STRONG          PIN_DM_STRONG
#define Pushbutton_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Pushbutton_MASK               Pushbutton__MASK
#define Pushbutton_SHIFT              Pushbutton__SHIFT
#define Pushbutton_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pushbutton_PS                     (* (reg8 *) Pushbutton__PS)
/* Data Register */
#define Pushbutton_DR                     (* (reg8 *) Pushbutton__DR)
/* Port Number */
#define Pushbutton_PRT_NUM                (* (reg8 *) Pushbutton__PRT) 
/* Connect to Analog Globals */                                                  
#define Pushbutton_AG                     (* (reg8 *) Pushbutton__AG)                       
/* Analog MUX bux enable */
#define Pushbutton_AMUX                   (* (reg8 *) Pushbutton__AMUX) 
/* Bidirectional Enable */                                                        
#define Pushbutton_BIE                    (* (reg8 *) Pushbutton__BIE)
/* Bit-mask for Aliased Register Access */
#define Pushbutton_BIT_MASK               (* (reg8 *) Pushbutton__BIT_MASK)
/* Bypass Enable */
#define Pushbutton_BYP                    (* (reg8 *) Pushbutton__BYP)
/* Port wide control signals */                                                   
#define Pushbutton_CTL                    (* (reg8 *) Pushbutton__CTL)
/* Drive Modes */
#define Pushbutton_DM0                    (* (reg8 *) Pushbutton__DM0) 
#define Pushbutton_DM1                    (* (reg8 *) Pushbutton__DM1)
#define Pushbutton_DM2                    (* (reg8 *) Pushbutton__DM2) 
/* Input Buffer Disable Override */
#define Pushbutton_INP_DIS                (* (reg8 *) Pushbutton__INP_DIS)
/* LCD Common or Segment Drive */
#define Pushbutton_LCD_COM_SEG            (* (reg8 *) Pushbutton__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pushbutton_LCD_EN                 (* (reg8 *) Pushbutton__LCD_EN)
/* Slew Rate Control */
#define Pushbutton_SLW                    (* (reg8 *) Pushbutton__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pushbutton_PRTDSI__CAPS_SEL       (* (reg8 *) Pushbutton__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pushbutton_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pushbutton__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pushbutton_PRTDSI__OE_SEL0        (* (reg8 *) Pushbutton__PRTDSI__OE_SEL0) 
#define Pushbutton_PRTDSI__OE_SEL1        (* (reg8 *) Pushbutton__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pushbutton_PRTDSI__OUT_SEL0       (* (reg8 *) Pushbutton__PRTDSI__OUT_SEL0) 
#define Pushbutton_PRTDSI__OUT_SEL1       (* (reg8 *) Pushbutton__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pushbutton_PRTDSI__SYNC_OUT       (* (reg8 *) Pushbutton__PRTDSI__SYNC_OUT) 


#if defined(Pushbutton__INTSTAT)  /* Interrupt Registers */

    #define Pushbutton_INTSTAT                (* (reg8 *) Pushbutton__INTSTAT)
    #define Pushbutton_SNAP                   (* (reg8 *) Pushbutton__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Pushbutton_H */


/* [] END OF FILE */
