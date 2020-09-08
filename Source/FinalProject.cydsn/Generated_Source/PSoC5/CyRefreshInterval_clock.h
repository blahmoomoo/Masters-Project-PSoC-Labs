/*******************************************************************************
* File Name: CyRefreshInterval_clock.h
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

#if !defined(CY_CLOCK_CyRefreshInterval_clock_H)
#define CY_CLOCK_CyRefreshInterval_clock_H

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

void CyRefreshInterval_clock_Start(void) ;
void CyRefreshInterval_clock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void CyRefreshInterval_clock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void CyRefreshInterval_clock_StandbyPower(uint8 state) ;
void CyRefreshInterval_clock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 CyRefreshInterval_clock_GetDividerRegister(void) ;
void CyRefreshInterval_clock_SetModeRegister(uint8 modeBitMask) ;
void CyRefreshInterval_clock_ClearModeRegister(uint8 modeBitMask) ;
uint8 CyRefreshInterval_clock_GetModeRegister(void) ;
void CyRefreshInterval_clock_SetSourceRegister(uint8 clkSource) ;
uint8 CyRefreshInterval_clock_GetSourceRegister(void) ;
#if defined(CyRefreshInterval_clock__CFG3)
void CyRefreshInterval_clock_SetPhaseRegister(uint8 clkPhase) ;
uint8 CyRefreshInterval_clock_GetPhaseRegister(void) ;
#endif /* defined(CyRefreshInterval_clock__CFG3) */

#define CyRefreshInterval_clock_Enable()                       CyRefreshInterval_clock_Start()
#define CyRefreshInterval_clock_Disable()                      CyRefreshInterval_clock_Stop()
#define CyRefreshInterval_clock_SetDivider(clkDivider)         CyRefreshInterval_clock_SetDividerRegister(clkDivider, 1u)
#define CyRefreshInterval_clock_SetDividerValue(clkDivider)    CyRefreshInterval_clock_SetDividerRegister((clkDivider) - 1u, 1u)
#define CyRefreshInterval_clock_SetMode(clkMode)               CyRefreshInterval_clock_SetModeRegister(clkMode)
#define CyRefreshInterval_clock_SetSource(clkSource)           CyRefreshInterval_clock_SetSourceRegister(clkSource)
#if defined(CyRefreshInterval_clock__CFG3)
#define CyRefreshInterval_clock_SetPhase(clkPhase)             CyRefreshInterval_clock_SetPhaseRegister(clkPhase)
#define CyRefreshInterval_clock_SetPhaseValue(clkPhase)        CyRefreshInterval_clock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(CyRefreshInterval_clock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define CyRefreshInterval_clock_CLKEN              (* (reg8 *) CyRefreshInterval_clock__PM_ACT_CFG)
#define CyRefreshInterval_clock_CLKEN_PTR          ((reg8 *) CyRefreshInterval_clock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define CyRefreshInterval_clock_CLKSTBY            (* (reg8 *) CyRefreshInterval_clock__PM_STBY_CFG)
#define CyRefreshInterval_clock_CLKSTBY_PTR        ((reg8 *) CyRefreshInterval_clock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define CyRefreshInterval_clock_DIV_LSB            (* (reg8 *) CyRefreshInterval_clock__CFG0)
#define CyRefreshInterval_clock_DIV_LSB_PTR        ((reg8 *) CyRefreshInterval_clock__CFG0)
#define CyRefreshInterval_clock_DIV_PTR            ((reg16 *) CyRefreshInterval_clock__CFG0)

/* Clock MSB divider configuration register. */
#define CyRefreshInterval_clock_DIV_MSB            (* (reg8 *) CyRefreshInterval_clock__CFG1)
#define CyRefreshInterval_clock_DIV_MSB_PTR        ((reg8 *) CyRefreshInterval_clock__CFG1)

/* Mode and source configuration register */
#define CyRefreshInterval_clock_MOD_SRC            (* (reg8 *) CyRefreshInterval_clock__CFG2)
#define CyRefreshInterval_clock_MOD_SRC_PTR        ((reg8 *) CyRefreshInterval_clock__CFG2)

#if defined(CyRefreshInterval_clock__CFG3)
/* Analog clock phase configuration register */
#define CyRefreshInterval_clock_PHASE              (* (reg8 *) CyRefreshInterval_clock__CFG3)
#define CyRefreshInterval_clock_PHASE_PTR          ((reg8 *) CyRefreshInterval_clock__CFG3)
#endif /* defined(CyRefreshInterval_clock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define CyRefreshInterval_clock_CLKEN_MASK         CyRefreshInterval_clock__PM_ACT_MSK
#define CyRefreshInterval_clock_CLKSTBY_MASK       CyRefreshInterval_clock__PM_STBY_MSK

/* CFG2 field masks */
#define CyRefreshInterval_clock_SRC_SEL_MSK        CyRefreshInterval_clock__CFG2_SRC_SEL_MASK
#define CyRefreshInterval_clock_MODE_MASK          (~(CyRefreshInterval_clock_SRC_SEL_MSK))

#if defined(CyRefreshInterval_clock__CFG3)
/* CFG3 phase mask */
#define CyRefreshInterval_clock_PHASE_MASK         CyRefreshInterval_clock__CFG3_PHASE_DLY_MASK
#endif /* defined(CyRefreshInterval_clock__CFG3) */

#endif /* CY_CLOCK_CyRefreshInterval_clock_H */


/* [] END OF FILE */
