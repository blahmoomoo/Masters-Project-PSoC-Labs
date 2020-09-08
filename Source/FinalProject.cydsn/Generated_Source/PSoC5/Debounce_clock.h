/*******************************************************************************
* File Name: Debounce_clock.h
* Version 2.10
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_Debounce_clock_H)
#define CY_CLOCK_Debounce_clock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void Debounce_clock_Start(void) ;
void Debounce_clock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void Debounce_clock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void Debounce_clock_StandbyPower(uint8 state) ;
void Debounce_clock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 Debounce_clock_GetDividerRegister(void) ;
void Debounce_clock_SetModeRegister(uint8 modeBitMask) ;
void Debounce_clock_ClearModeRegister(uint8 modeBitMask) ;
uint8 Debounce_clock_GetModeRegister(void) ;
void Debounce_clock_SetSourceRegister(uint8 clkSource) ;
uint8 Debounce_clock_GetSourceRegister(void) ;
#if defined(Debounce_clock__CFG3)
void Debounce_clock_SetPhaseRegister(uint8 clkPhase) ;
uint8 Debounce_clock_GetPhaseRegister(void) ;
#endif /* defined(Debounce_clock__CFG3) */

#define Debounce_clock_Enable()                       Debounce_clock_Start()
#define Debounce_clock_Disable()                      Debounce_clock_Stop()
#define Debounce_clock_SetDivider(clkDivider)         Debounce_clock_SetDividerRegister(clkDivider, 1u)
#define Debounce_clock_SetDividerValue(clkDivider)    Debounce_clock_SetDividerRegister((clkDivider) - 1u, 1u)
#define Debounce_clock_SetMode(clkMode)               Debounce_clock_SetModeRegister(clkMode)
#define Debounce_clock_SetSource(clkSource)           Debounce_clock_SetSourceRegister(clkSource)
#if defined(Debounce_clock__CFG3)
#define Debounce_clock_SetPhase(clkPhase)             Debounce_clock_SetPhaseRegister(clkPhase)
#define Debounce_clock_SetPhaseValue(clkPhase)        Debounce_clock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(Debounce_clock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define Debounce_clock_CLKEN              (* (reg8 *) Debounce_clock__PM_ACT_CFG)
#define Debounce_clock_CLKEN_PTR          ((reg8 *) Debounce_clock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define Debounce_clock_CLKSTBY            (* (reg8 *) Debounce_clock__PM_STBY_CFG)
#define Debounce_clock_CLKSTBY_PTR        ((reg8 *) Debounce_clock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define Debounce_clock_DIV_LSB            (* (reg8 *) Debounce_clock__CFG0)
#define Debounce_clock_DIV_LSB_PTR        ((reg8 *) Debounce_clock__CFG0)
#define Debounce_clock_DIV_PTR            ((reg16 *) Debounce_clock__CFG0)

/* Clock MSB divider configuration register. */
#define Debounce_clock_DIV_MSB            (* (reg8 *) Debounce_clock__CFG1)
#define Debounce_clock_DIV_MSB_PTR        ((reg8 *) Debounce_clock__CFG1)

/* Mode and source configuration register */
#define Debounce_clock_MOD_SRC            (* (reg8 *) Debounce_clock__CFG2)
#define Debounce_clock_MOD_SRC_PTR        ((reg8 *) Debounce_clock__CFG2)

#if defined(Debounce_clock__CFG3)
/* Analog clock phase configuration register */
#define Debounce_clock_PHASE              (* (reg8 *) Debounce_clock__CFG3)
#define Debounce_clock_PHASE_PTR          ((reg8 *) Debounce_clock__CFG3)
#endif /* defined(Debounce_clock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define Debounce_clock_CLKEN_MASK         Debounce_clock__PM_ACT_MSK
#define Debounce_clock_CLKSTBY_MASK       Debounce_clock__PM_STBY_MSK

/* CFG2 field masks */
#define Debounce_clock_SRC_SEL_MSK        Debounce_clock__CFG2_SRC_SEL_MASK
#define Debounce_clock_MODE_MASK          (~(Debounce_clock_SRC_SEL_MSK))

#if defined(Debounce_clock__CFG3)
/* CFG3 phase mask */
#define Debounce_clock_PHASE_MASK         Debounce_clock__CFG3_PHASE_DLY_MASK
#endif /* defined(Debounce_clock__CFG3) */

#endif /* CY_CLOCK_Debounce_clock_H */


/* [] END OF FILE */
