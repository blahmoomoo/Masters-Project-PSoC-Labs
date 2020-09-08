/*******************************************************************************
* File Name: CyStopwatch_clock2.h
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

#if !defined(CY_CLOCK_CyStopwatch_clock2_H)
#define CY_CLOCK_CyStopwatch_clock2_H

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

void CyStopwatch_clock2_Start(void) ;
void CyStopwatch_clock2_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void CyStopwatch_clock2_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void CyStopwatch_clock2_StandbyPower(uint8 state) ;
void CyStopwatch_clock2_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 CyStopwatch_clock2_GetDividerRegister(void) ;
void CyStopwatch_clock2_SetModeRegister(uint8 modeBitMask) ;
void CyStopwatch_clock2_ClearModeRegister(uint8 modeBitMask) ;
uint8 CyStopwatch_clock2_GetModeRegister(void) ;
void CyStopwatch_clock2_SetSourceRegister(uint8 clkSource) ;
uint8 CyStopwatch_clock2_GetSourceRegister(void) ;
#if defined(CyStopwatch_clock2__CFG3)
void CyStopwatch_clock2_SetPhaseRegister(uint8 clkPhase) ;
uint8 CyStopwatch_clock2_GetPhaseRegister(void) ;
#endif /* defined(CyStopwatch_clock2__CFG3) */

#define CyStopwatch_clock2_Enable()                       CyStopwatch_clock2_Start()
#define CyStopwatch_clock2_Disable()                      CyStopwatch_clock2_Stop()
#define CyStopwatch_clock2_SetDivider(clkDivider)         CyStopwatch_clock2_SetDividerRegister(clkDivider, 1u)
#define CyStopwatch_clock2_SetDividerValue(clkDivider)    CyStopwatch_clock2_SetDividerRegister((clkDivider) - 1u, 1u)
#define CyStopwatch_clock2_SetMode(clkMode)               CyStopwatch_clock2_SetModeRegister(clkMode)
#define CyStopwatch_clock2_SetSource(clkSource)           CyStopwatch_clock2_SetSourceRegister(clkSource)
#if defined(CyStopwatch_clock2__CFG3)
#define CyStopwatch_clock2_SetPhase(clkPhase)             CyStopwatch_clock2_SetPhaseRegister(clkPhase)
#define CyStopwatch_clock2_SetPhaseValue(clkPhase)        CyStopwatch_clock2_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(CyStopwatch_clock2__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define CyStopwatch_clock2_CLKEN              (* (reg8 *) CyStopwatch_clock2__PM_ACT_CFG)
#define CyStopwatch_clock2_CLKEN_PTR          ((reg8 *) CyStopwatch_clock2__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define CyStopwatch_clock2_CLKSTBY            (* (reg8 *) CyStopwatch_clock2__PM_STBY_CFG)
#define CyStopwatch_clock2_CLKSTBY_PTR        ((reg8 *) CyStopwatch_clock2__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define CyStopwatch_clock2_DIV_LSB            (* (reg8 *) CyStopwatch_clock2__CFG0)
#define CyStopwatch_clock2_DIV_LSB_PTR        ((reg8 *) CyStopwatch_clock2__CFG0)
#define CyStopwatch_clock2_DIV_PTR            ((reg16 *) CyStopwatch_clock2__CFG0)

/* Clock MSB divider configuration register. */
#define CyStopwatch_clock2_DIV_MSB            (* (reg8 *) CyStopwatch_clock2__CFG1)
#define CyStopwatch_clock2_DIV_MSB_PTR        ((reg8 *) CyStopwatch_clock2__CFG1)

/* Mode and source configuration register */
#define CyStopwatch_clock2_MOD_SRC            (* (reg8 *) CyStopwatch_clock2__CFG2)
#define CyStopwatch_clock2_MOD_SRC_PTR        ((reg8 *) CyStopwatch_clock2__CFG2)

#if defined(CyStopwatch_clock2__CFG3)
/* Analog clock phase configuration register */
#define CyStopwatch_clock2_PHASE              (* (reg8 *) CyStopwatch_clock2__CFG3)
#define CyStopwatch_clock2_PHASE_PTR          ((reg8 *) CyStopwatch_clock2__CFG3)
#endif /* defined(CyStopwatch_clock2__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define CyStopwatch_clock2_CLKEN_MASK         CyStopwatch_clock2__PM_ACT_MSK
#define CyStopwatch_clock2_CLKSTBY_MASK       CyStopwatch_clock2__PM_STBY_MSK

/* CFG2 field masks */
#define CyStopwatch_clock2_SRC_SEL_MSK        CyStopwatch_clock2__CFG2_SRC_SEL_MASK
#define CyStopwatch_clock2_MODE_MASK          (~(CyStopwatch_clock2_SRC_SEL_MSK))

#if defined(CyStopwatch_clock2__CFG3)
/* CFG3 phase mask */
#define CyStopwatch_clock2_PHASE_MASK         CyStopwatch_clock2__CFG3_PHASE_DLY_MASK
#endif /* defined(CyStopwatch_clock2__CFG3) */

#endif /* CY_CLOCK_CyStopwatch_clock2_H */


/* [] END OF FILE */
