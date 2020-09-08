/*******************************************************************************
* File Name: CySYNTH_MASTER_CLK.h
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

#if !defined(CY_CLOCK_CySYNTH_MASTER_CLK_H)
#define CY_CLOCK_CySYNTH_MASTER_CLK_H

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

void CySYNTH_MASTER_CLK_Start(void) ;
void CySYNTH_MASTER_CLK_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void CySYNTH_MASTER_CLK_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void CySYNTH_MASTER_CLK_StandbyPower(uint8 state) ;
void CySYNTH_MASTER_CLK_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 CySYNTH_MASTER_CLK_GetDividerRegister(void) ;
void CySYNTH_MASTER_CLK_SetModeRegister(uint8 modeBitMask) ;
void CySYNTH_MASTER_CLK_ClearModeRegister(uint8 modeBitMask) ;
uint8 CySYNTH_MASTER_CLK_GetModeRegister(void) ;
void CySYNTH_MASTER_CLK_SetSourceRegister(uint8 clkSource) ;
uint8 CySYNTH_MASTER_CLK_GetSourceRegister(void) ;
#if defined(CySYNTH_MASTER_CLK__CFG3)
void CySYNTH_MASTER_CLK_SetPhaseRegister(uint8 clkPhase) ;
uint8 CySYNTH_MASTER_CLK_GetPhaseRegister(void) ;
#endif /* defined(CySYNTH_MASTER_CLK__CFG3) */

#define CySYNTH_MASTER_CLK_Enable()                       CySYNTH_MASTER_CLK_Start()
#define CySYNTH_MASTER_CLK_Disable()                      CySYNTH_MASTER_CLK_Stop()
#define CySYNTH_MASTER_CLK_SetDivider(clkDivider)         CySYNTH_MASTER_CLK_SetDividerRegister(clkDivider, 1u)
#define CySYNTH_MASTER_CLK_SetDividerValue(clkDivider)    CySYNTH_MASTER_CLK_SetDividerRegister((clkDivider) - 1u, 1u)
#define CySYNTH_MASTER_CLK_SetMode(clkMode)               CySYNTH_MASTER_CLK_SetModeRegister(clkMode)
#define CySYNTH_MASTER_CLK_SetSource(clkSource)           CySYNTH_MASTER_CLK_SetSourceRegister(clkSource)
#if defined(CySYNTH_MASTER_CLK__CFG3)
#define CySYNTH_MASTER_CLK_SetPhase(clkPhase)             CySYNTH_MASTER_CLK_SetPhaseRegister(clkPhase)
#define CySYNTH_MASTER_CLK_SetPhaseValue(clkPhase)        CySYNTH_MASTER_CLK_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(CySYNTH_MASTER_CLK__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define CySYNTH_MASTER_CLK_CLKEN              (* (reg8 *) CySYNTH_MASTER_CLK__PM_ACT_CFG)
#define CySYNTH_MASTER_CLK_CLKEN_PTR          ((reg8 *) CySYNTH_MASTER_CLK__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define CySYNTH_MASTER_CLK_CLKSTBY            (* (reg8 *) CySYNTH_MASTER_CLK__PM_STBY_CFG)
#define CySYNTH_MASTER_CLK_CLKSTBY_PTR        ((reg8 *) CySYNTH_MASTER_CLK__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define CySYNTH_MASTER_CLK_DIV_LSB            (* (reg8 *) CySYNTH_MASTER_CLK__CFG0)
#define CySYNTH_MASTER_CLK_DIV_LSB_PTR        ((reg8 *) CySYNTH_MASTER_CLK__CFG0)
#define CySYNTH_MASTER_CLK_DIV_PTR            ((reg16 *) CySYNTH_MASTER_CLK__CFG0)

/* Clock MSB divider configuration register. */
#define CySYNTH_MASTER_CLK_DIV_MSB            (* (reg8 *) CySYNTH_MASTER_CLK__CFG1)
#define CySYNTH_MASTER_CLK_DIV_MSB_PTR        ((reg8 *) CySYNTH_MASTER_CLK__CFG1)

/* Mode and source configuration register */
#define CySYNTH_MASTER_CLK_MOD_SRC            (* (reg8 *) CySYNTH_MASTER_CLK__CFG2)
#define CySYNTH_MASTER_CLK_MOD_SRC_PTR        ((reg8 *) CySYNTH_MASTER_CLK__CFG2)

#if defined(CySYNTH_MASTER_CLK__CFG3)
/* Analog clock phase configuration register */
#define CySYNTH_MASTER_CLK_PHASE              (* (reg8 *) CySYNTH_MASTER_CLK__CFG3)
#define CySYNTH_MASTER_CLK_PHASE_PTR          ((reg8 *) CySYNTH_MASTER_CLK__CFG3)
#endif /* defined(CySYNTH_MASTER_CLK__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define CySYNTH_MASTER_CLK_CLKEN_MASK         CySYNTH_MASTER_CLK__PM_ACT_MSK
#define CySYNTH_MASTER_CLK_CLKSTBY_MASK       CySYNTH_MASTER_CLK__PM_STBY_MSK

/* CFG2 field masks */
#define CySYNTH_MASTER_CLK_SRC_SEL_MSK        CySYNTH_MASTER_CLK__CFG2_SRC_SEL_MASK
#define CySYNTH_MASTER_CLK_MODE_MASK          (~(CySYNTH_MASTER_CLK_SRC_SEL_MSK))

#if defined(CySYNTH_MASTER_CLK__CFG3)
/* CFG3 phase mask */
#define CySYNTH_MASTER_CLK_PHASE_MASK         CySYNTH_MASTER_CLK__CFG3_PHASE_DLY_MASK
#endif /* defined(CySYNTH_MASTER_CLK__CFG3) */

#endif /* CY_CLOCK_CySYNTH_MASTER_CLK_H */


/* [] END OF FILE */
