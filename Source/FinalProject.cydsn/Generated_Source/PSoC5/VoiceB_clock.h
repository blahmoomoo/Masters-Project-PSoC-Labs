/*******************************************************************************
* File Name: VoiceB_clock.h
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

#if !defined(CY_CLOCK_VoiceB_clock_H)
#define CY_CLOCK_VoiceB_clock_H

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

void VoiceB_clock_Start(void) ;
void VoiceB_clock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void VoiceB_clock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void VoiceB_clock_StandbyPower(uint8 state) ;
void VoiceB_clock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 VoiceB_clock_GetDividerRegister(void) ;
void VoiceB_clock_SetModeRegister(uint8 modeBitMask) ;
void VoiceB_clock_ClearModeRegister(uint8 modeBitMask) ;
uint8 VoiceB_clock_GetModeRegister(void) ;
void VoiceB_clock_SetSourceRegister(uint8 clkSource) ;
uint8 VoiceB_clock_GetSourceRegister(void) ;
#if defined(VoiceB_clock__CFG3)
void VoiceB_clock_SetPhaseRegister(uint8 clkPhase) ;
uint8 VoiceB_clock_GetPhaseRegister(void) ;
#endif /* defined(VoiceB_clock__CFG3) */

#define VoiceB_clock_Enable()                       VoiceB_clock_Start()
#define VoiceB_clock_Disable()                      VoiceB_clock_Stop()
#define VoiceB_clock_SetDivider(clkDivider)         VoiceB_clock_SetDividerRegister(clkDivider, 1u)
#define VoiceB_clock_SetDividerValue(clkDivider)    VoiceB_clock_SetDividerRegister((clkDivider) - 1u, 1u)
#define VoiceB_clock_SetMode(clkMode)               VoiceB_clock_SetModeRegister(clkMode)
#define VoiceB_clock_SetSource(clkSource)           VoiceB_clock_SetSourceRegister(clkSource)
#if defined(VoiceB_clock__CFG3)
#define VoiceB_clock_SetPhase(clkPhase)             VoiceB_clock_SetPhaseRegister(clkPhase)
#define VoiceB_clock_SetPhaseValue(clkPhase)        VoiceB_clock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(VoiceB_clock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define VoiceB_clock_CLKEN              (* (reg8 *) VoiceB_clock__PM_ACT_CFG)
#define VoiceB_clock_CLKEN_PTR          ((reg8 *) VoiceB_clock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define VoiceB_clock_CLKSTBY            (* (reg8 *) VoiceB_clock__PM_STBY_CFG)
#define VoiceB_clock_CLKSTBY_PTR        ((reg8 *) VoiceB_clock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define VoiceB_clock_DIV_LSB            (* (reg8 *) VoiceB_clock__CFG0)
#define VoiceB_clock_DIV_LSB_PTR        ((reg8 *) VoiceB_clock__CFG0)
#define VoiceB_clock_DIV_PTR            ((reg16 *) VoiceB_clock__CFG0)

/* Clock MSB divider configuration register. */
#define VoiceB_clock_DIV_MSB            (* (reg8 *) VoiceB_clock__CFG1)
#define VoiceB_clock_DIV_MSB_PTR        ((reg8 *) VoiceB_clock__CFG1)

/* Mode and source configuration register */
#define VoiceB_clock_MOD_SRC            (* (reg8 *) VoiceB_clock__CFG2)
#define VoiceB_clock_MOD_SRC_PTR        ((reg8 *) VoiceB_clock__CFG2)

#if defined(VoiceB_clock__CFG3)
/* Analog clock phase configuration register */
#define VoiceB_clock_PHASE              (* (reg8 *) VoiceB_clock__CFG3)
#define VoiceB_clock_PHASE_PTR          ((reg8 *) VoiceB_clock__CFG3)
#endif /* defined(VoiceB_clock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define VoiceB_clock_CLKEN_MASK         VoiceB_clock__PM_ACT_MSK
#define VoiceB_clock_CLKSTBY_MASK       VoiceB_clock__PM_STBY_MSK

/* CFG2 field masks */
#define VoiceB_clock_SRC_SEL_MSK        VoiceB_clock__CFG2_SRC_SEL_MASK
#define VoiceB_clock_MODE_MASK          (~(VoiceB_clock_SRC_SEL_MSK))

#if defined(VoiceB_clock__CFG3)
/* CFG3 phase mask */
#define VoiceB_clock_PHASE_MASK         VoiceB_clock__CFG3_PHASE_DLY_MASK
#endif /* defined(VoiceB_clock__CFG3) */

#endif /* CY_CLOCK_VoiceB_clock_H */


/* [] END OF FILE */
