/*******************************************************************************
* File Name: Output_clock.h
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

#if !defined(CY_CLOCK_Output_clock_H)
#define CY_CLOCK_Output_clock_H

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

void Output_clock_Start(void) ;
void Output_clock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void Output_clock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void Output_clock_StandbyPower(uint8 state) ;
void Output_clock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 Output_clock_GetDividerRegister(void) ;
void Output_clock_SetModeRegister(uint8 modeBitMask) ;
void Output_clock_ClearModeRegister(uint8 modeBitMask) ;
uint8 Output_clock_GetModeRegister(void) ;
void Output_clock_SetSourceRegister(uint8 clkSource) ;
uint8 Output_clock_GetSourceRegister(void) ;
#if defined(Output_clock__CFG3)
void Output_clock_SetPhaseRegister(uint8 clkPhase) ;
uint8 Output_clock_GetPhaseRegister(void) ;
#endif /* defined(Output_clock__CFG3) */

#define Output_clock_Enable()                       Output_clock_Start()
#define Output_clock_Disable()                      Output_clock_Stop()
#define Output_clock_SetDivider(clkDivider)         Output_clock_SetDividerRegister(clkDivider, 1u)
#define Output_clock_SetDividerValue(clkDivider)    Output_clock_SetDividerRegister((clkDivider) - 1u, 1u)
#define Output_clock_SetMode(clkMode)               Output_clock_SetModeRegister(clkMode)
#define Output_clock_SetSource(clkSource)           Output_clock_SetSourceRegister(clkSource)
#if defined(Output_clock__CFG3)
#define Output_clock_SetPhase(clkPhase)             Output_clock_SetPhaseRegister(clkPhase)
#define Output_clock_SetPhaseValue(clkPhase)        Output_clock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(Output_clock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define Output_clock_CLKEN              (* (reg8 *) Output_clock__PM_ACT_CFG)
#define Output_clock_CLKEN_PTR          ((reg8 *) Output_clock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define Output_clock_CLKSTBY            (* (reg8 *) Output_clock__PM_STBY_CFG)
#define Output_clock_CLKSTBY_PTR        ((reg8 *) Output_clock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define Output_clock_DIV_LSB            (* (reg8 *) Output_clock__CFG0)
#define Output_clock_DIV_LSB_PTR        ((reg8 *) Output_clock__CFG0)
#define Output_clock_DIV_PTR            ((reg16 *) Output_clock__CFG0)

/* Clock MSB divider configuration register. */
#define Output_clock_DIV_MSB            (* (reg8 *) Output_clock__CFG1)
#define Output_clock_DIV_MSB_PTR        ((reg8 *) Output_clock__CFG1)

/* Mode and source configuration register */
#define Output_clock_MOD_SRC            (* (reg8 *) Output_clock__CFG2)
#define Output_clock_MOD_SRC_PTR        ((reg8 *) Output_clock__CFG2)

#if defined(Output_clock__CFG3)
/* Analog clock phase configuration register */
#define Output_clock_PHASE              (* (reg8 *) Output_clock__CFG3)
#define Output_clock_PHASE_PTR          ((reg8 *) Output_clock__CFG3)
#endif /* defined(Output_clock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define Output_clock_CLKEN_MASK         Output_clock__PM_ACT_MSK
#define Output_clock_CLKSTBY_MASK       Output_clock__PM_STBY_MSK

/* CFG2 field masks */
#define Output_clock_SRC_SEL_MSK        Output_clock__CFG2_SRC_SEL_MASK
#define Output_clock_MODE_MASK          (~(Output_clock_SRC_SEL_MSK))

#if defined(Output_clock__CFG3)
/* CFG3 phase mask */
#define Output_clock_PHASE_MASK         Output_clock__CFG3_PHASE_DLY_MASK
#endif /* defined(Output_clock__CFG3) */

#endif /* CY_CLOCK_Output_clock_H */


/* [] END OF FILE */
