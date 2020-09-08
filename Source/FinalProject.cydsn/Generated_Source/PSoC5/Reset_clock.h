/*******************************************************************************
* File Name: Reset_clock.h
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

#if !defined(CY_CLOCK_Reset_clock_H)
#define CY_CLOCK_Reset_clock_H

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

void Reset_clock_Start(void) ;
void Reset_clock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void Reset_clock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void Reset_clock_StandbyPower(uint8 state) ;
void Reset_clock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 Reset_clock_GetDividerRegister(void) ;
void Reset_clock_SetModeRegister(uint8 modeBitMask) ;
void Reset_clock_ClearModeRegister(uint8 modeBitMask) ;
uint8 Reset_clock_GetModeRegister(void) ;
void Reset_clock_SetSourceRegister(uint8 clkSource) ;
uint8 Reset_clock_GetSourceRegister(void) ;
#if defined(Reset_clock__CFG3)
void Reset_clock_SetPhaseRegister(uint8 clkPhase) ;
uint8 Reset_clock_GetPhaseRegister(void) ;
#endif /* defined(Reset_clock__CFG3) */

#define Reset_clock_Enable()                       Reset_clock_Start()
#define Reset_clock_Disable()                      Reset_clock_Stop()
#define Reset_clock_SetDivider(clkDivider)         Reset_clock_SetDividerRegister(clkDivider, 1u)
#define Reset_clock_SetDividerValue(clkDivider)    Reset_clock_SetDividerRegister((clkDivider) - 1u, 1u)
#define Reset_clock_SetMode(clkMode)               Reset_clock_SetModeRegister(clkMode)
#define Reset_clock_SetSource(clkSource)           Reset_clock_SetSourceRegister(clkSource)
#if defined(Reset_clock__CFG3)
#define Reset_clock_SetPhase(clkPhase)             Reset_clock_SetPhaseRegister(clkPhase)
#define Reset_clock_SetPhaseValue(clkPhase)        Reset_clock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(Reset_clock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define Reset_clock_CLKEN              (* (reg8 *) Reset_clock__PM_ACT_CFG)
#define Reset_clock_CLKEN_PTR          ((reg8 *) Reset_clock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define Reset_clock_CLKSTBY            (* (reg8 *) Reset_clock__PM_STBY_CFG)
#define Reset_clock_CLKSTBY_PTR        ((reg8 *) Reset_clock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define Reset_clock_DIV_LSB            (* (reg8 *) Reset_clock__CFG0)
#define Reset_clock_DIV_LSB_PTR        ((reg8 *) Reset_clock__CFG0)
#define Reset_clock_DIV_PTR            ((reg16 *) Reset_clock__CFG0)

/* Clock MSB divider configuration register. */
#define Reset_clock_DIV_MSB            (* (reg8 *) Reset_clock__CFG1)
#define Reset_clock_DIV_MSB_PTR        ((reg8 *) Reset_clock__CFG1)

/* Mode and source configuration register */
#define Reset_clock_MOD_SRC            (* (reg8 *) Reset_clock__CFG2)
#define Reset_clock_MOD_SRC_PTR        ((reg8 *) Reset_clock__CFG2)

#if defined(Reset_clock__CFG3)
/* Analog clock phase configuration register */
#define Reset_clock_PHASE              (* (reg8 *) Reset_clock__CFG3)
#define Reset_clock_PHASE_PTR          ((reg8 *) Reset_clock__CFG3)
#endif /* defined(Reset_clock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define Reset_clock_CLKEN_MASK         Reset_clock__PM_ACT_MSK
#define Reset_clock_CLKSTBY_MASK       Reset_clock__PM_STBY_MSK

/* CFG2 field masks */
#define Reset_clock_SRC_SEL_MSK        Reset_clock__CFG2_SRC_SEL_MASK
#define Reset_clock_MODE_MASK          (~(Reset_clock_SRC_SEL_MSK))

#if defined(Reset_clock__CFG3)
/* CFG3 phase mask */
#define Reset_clock_PHASE_MASK         Reset_clock__CFG3_PHASE_DLY_MASK
#endif /* defined(Reset_clock__CFG3) */

#endif /* CY_CLOCK_Reset_clock_H */


/* [] END OF FILE */
