/*******************************************************************************
* File Name: VoiceA_clock.h
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

#if !defined(CY_CLOCK_VoiceA_clock_H)
#define CY_CLOCK_VoiceA_clock_H

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

void VoiceA_clock_Start(void) ;
void VoiceA_clock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void VoiceA_clock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void VoiceA_clock_StandbyPower(uint8 state) ;
void VoiceA_clock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 VoiceA_clock_GetDividerRegister(void) ;
void VoiceA_clock_SetModeRegister(uint8 modeBitMask) ;
void VoiceA_clock_ClearModeRegister(uint8 modeBitMask) ;
uint8 VoiceA_clock_GetModeRegister(void) ;
void VoiceA_clock_SetSourceRegister(uint8 clkSource) ;
uint8 VoiceA_clock_GetSourceRegister(void) ;
#if defined(VoiceA_clock__CFG3)
void VoiceA_clock_SetPhaseRegister(uint8 clkPhase) ;
uint8 VoiceA_clock_GetPhaseRegister(void) ;
#endif /* defined(VoiceA_clock__CFG3) */

#define VoiceA_clock_Enable()                       VoiceA_clock_Start()
#define VoiceA_clock_Disable()                      VoiceA_clock_Stop()
#define VoiceA_clock_SetDivider(clkDivider)         VoiceA_clock_SetDividerRegister(clkDivider, 1u)
#define VoiceA_clock_SetDividerValue(clkDivider)    VoiceA_clock_SetDividerRegister((clkDivider) - 1u, 1u)
#define VoiceA_clock_SetMode(clkMode)               VoiceA_clock_SetModeRegister(clkMode)
#define VoiceA_clock_SetSource(clkSource)           VoiceA_clock_SetSourceRegister(clkSource)
#if defined(VoiceA_clock__CFG3)
#define VoiceA_clock_SetPhase(clkPhase)             VoiceA_clock_SetPhaseRegister(clkPhase)
#define VoiceA_clock_SetPhaseValue(clkPhase)        VoiceA_clock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(VoiceA_clock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define VoiceA_clock_CLKEN              (* (reg8 *) VoiceA_clock__PM_ACT_CFG)
#define VoiceA_clock_CLKEN_PTR          ((reg8 *) VoiceA_clock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define VoiceA_clock_CLKSTBY            (* (reg8 *) VoiceA_clock__PM_STBY_CFG)
#define VoiceA_clock_CLKSTBY_PTR        ((reg8 *) VoiceA_clock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define VoiceA_clock_DIV_LSB            (* (reg8 *) VoiceA_clock__CFG0)
#define VoiceA_clock_DIV_LSB_PTR        ((reg8 *) VoiceA_clock__CFG0)
#define VoiceA_clock_DIV_PTR            ((reg16 *) VoiceA_clock__CFG0)

/* Clock MSB divider configuration register. */
#define VoiceA_clock_DIV_MSB            (* (reg8 *) VoiceA_clock__CFG1)
#define VoiceA_clock_DIV_MSB_PTR        ((reg8 *) VoiceA_clock__CFG1)

/* Mode and source configuration register */
#define VoiceA_clock_MOD_SRC            (* (reg8 *) VoiceA_clock__CFG2)
#define VoiceA_clock_MOD_SRC_PTR        ((reg8 *) VoiceA_clock__CFG2)

#if defined(VoiceA_clock__CFG3)
/* Analog clock phase configuration register */
#define VoiceA_clock_PHASE              (* (reg8 *) VoiceA_clock__CFG3)
#define VoiceA_clock_PHASE_PTR          ((reg8 *) VoiceA_clock__CFG3)
#endif /* defined(VoiceA_clock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define VoiceA_clock_CLKEN_MASK         VoiceA_clock__PM_ACT_MSK
#define VoiceA_clock_CLKSTBY_MASK       VoiceA_clock__PM_STBY_MSK

/* CFG2 field masks */
#define VoiceA_clock_SRC_SEL_MSK        VoiceA_clock__CFG2_SRC_SEL_MASK
#define VoiceA_clock_MODE_MASK          (~(VoiceA_clock_SRC_SEL_MSK))

#if defined(VoiceA_clock__CFG3)
/* CFG3 phase mask */
#define VoiceA_clock_PHASE_MASK         VoiceA_clock__CFG3_PHASE_DLY_MASK
#endif /* defined(VoiceA_clock__CFG3) */

#endif /* CY_CLOCK_VoiceA_clock_H */


/* [] END OF FILE */
