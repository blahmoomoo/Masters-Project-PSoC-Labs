/*******************************************************************************
* File Name: DisplayRefresh.h
* Version 2.50
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_Timer_v2_30_DisplayRefresh_H)
#define CY_Timer_v2_30_DisplayRefresh_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 DisplayRefresh_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define DisplayRefresh_Resolution                 8u
#define DisplayRefresh_UsingFixedFunction         1u
#define DisplayRefresh_UsingHWCaptureCounter      0u
#define DisplayRefresh_SoftwareCaptureMode        0u
#define DisplayRefresh_SoftwareTriggerMode        0u
#define DisplayRefresh_UsingHWEnable              0u
#define DisplayRefresh_EnableTriggerMode          0u
#define DisplayRefresh_InterruptOnCaptureCount    0u
#define DisplayRefresh_RunModeUsed                0u
#define DisplayRefresh_ControlRegRemoved          0u


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!DisplayRefresh_UsingFixedFunction)
        #if (CY_UDB_V0)
            uint8 TimerUdb;                 /* Timer internal counter value */
            uint8 TimerPeriod;              /* Timer Period value       */
            uint8 InterruptMaskValue;       /* Timer Compare Value */
            #if (DisplayRefresh_UsingHWCaptureCounter)
                uint8 TimerCaptureCounter;  /* Timer Capture Counter Value */
            #endif /* variable declaration for backing up Capture Counter value*/
        #endif /* variables for non retention registers in CY_UDB_V0 */

        #if (CY_UDB_V1)
            uint8 TimerUdb;
            uint8 InterruptMaskValue;
            #if (DisplayRefresh_UsingHWCaptureCounter)
                uint8 TimerCaptureCounter;
            #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */
        #endif /* (CY_UDB_V1) */

        #if (!DisplayRefresh_ControlRegRemoved)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */
}DisplayRefresh_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    DisplayRefresh_Start(void) ;
void    DisplayRefresh_Stop(void) ;

void    DisplayRefresh_SetInterruptMode(uint8 interruptMode) ;
uint8   DisplayRefresh_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define DisplayRefresh_GetInterruptSource() DisplayRefresh_ReadStatusRegister()

#if(!DisplayRefresh_ControlRegRemoved)
    uint8   DisplayRefresh_ReadControlRegister(void) ;
    void    DisplayRefresh_WriteControlRegister(uint8 control) \
        ;
#endif /* (!DisplayRefresh_ControlRegRemoved) */

uint8  DisplayRefresh_ReadPeriod(void) ;
void    DisplayRefresh_WritePeriod(uint8 period) \
    ;
uint8  DisplayRefresh_ReadCounter(void) ;
void    DisplayRefresh_WriteCounter(uint8 counter) \
    ;
uint8  DisplayRefresh_ReadCapture(void) ;
void    DisplayRefresh_SoftwareCapture(void) ;


#if(!DisplayRefresh_UsingFixedFunction) /* UDB Prototypes */
    #if (DisplayRefresh_SoftwareCaptureMode)
        void    DisplayRefresh_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!DisplayRefresh_UsingFixedFunction) */

    #if (DisplayRefresh_SoftwareTriggerMode)
        void    DisplayRefresh_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (DisplayRefresh_SoftwareTriggerMode) */
    #if (DisplayRefresh_EnableTriggerMode)
        void    DisplayRefresh_EnableTrigger(void) ;
        void    DisplayRefresh_DisableTrigger(void) ;
    #endif /* (DisplayRefresh_EnableTriggerMode) */

    #if(DisplayRefresh_InterruptOnCaptureCount)
        #if(!DisplayRefresh_ControlRegRemoved)
            void    DisplayRefresh_SetInterruptCount(uint8 interruptCount) \
                ;
        #endif /* (!DisplayRefresh_ControlRegRemoved) */
    #endif /* (DisplayRefresh_InterruptOnCaptureCount) */

    #if (DisplayRefresh_UsingHWCaptureCounter)
        void    DisplayRefresh_SetCaptureCount(uint8 captureCount) \
            ;
        uint8   DisplayRefresh_ReadCaptureCount(void) ;
    #endif /* (DisplayRefresh_UsingHWCaptureCounter) */

    void DisplayRefresh_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void DisplayRefresh_Init(void)          ;
void DisplayRefresh_Enable(void)        ;
void DisplayRefresh_SaveConfig(void)    ;
void DisplayRefresh_RestoreConfig(void) ;
void DisplayRefresh_Sleep(void)         ;
void DisplayRefresh_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define DisplayRefresh__B_TIMER__CM_NONE 0
#define DisplayRefresh__B_TIMER__CM_RISINGEDGE 1
#define DisplayRefresh__B_TIMER__CM_FALLINGEDGE 2
#define DisplayRefresh__B_TIMER__CM_EITHEREDGE 3
#define DisplayRefresh__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define DisplayRefresh__B_TIMER__TM_NONE 0x00u
#define DisplayRefresh__B_TIMER__TM_RISINGEDGE 0x04u
#define DisplayRefresh__B_TIMER__TM_FALLINGEDGE 0x08u
#define DisplayRefresh__B_TIMER__TM_EITHEREDGE 0x0Cu
#define DisplayRefresh__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define DisplayRefresh_INIT_PERIOD             1u
#define DisplayRefresh_INIT_CAPTURE_MODE       ((uint8)((uint8)1u << DisplayRefresh_CTRL_CAP_MODE_SHIFT))
#define DisplayRefresh_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << DisplayRefresh_CTRL_TRIG_MODE_SHIFT))
#if (DisplayRefresh_UsingFixedFunction)
    #define DisplayRefresh_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << DisplayRefresh_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << DisplayRefresh_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define DisplayRefresh_INIT_INTERRUPT_MODE (((uint8)((uint8)1u << DisplayRefresh_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << DisplayRefresh_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << DisplayRefresh_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (DisplayRefresh_UsingFixedFunction) */
#define DisplayRefresh_INIT_CAPTURE_COUNT      (2u)
#define DisplayRefresh_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << DisplayRefresh_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (DisplayRefresh_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define DisplayRefresh_STATUS         (*(reg8 *) DisplayRefresh_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define DisplayRefresh_STATUS_MASK    (*(reg8 *) DisplayRefresh_TimerHW__SR0 )
    #define DisplayRefresh_CONTROL        (*(reg8 *) DisplayRefresh_TimerHW__CFG0)
    #define DisplayRefresh_CONTROL2       (*(reg8 *) DisplayRefresh_TimerHW__CFG1)
    #define DisplayRefresh_CONTROL2_PTR   ( (reg8 *) DisplayRefresh_TimerHW__CFG1)
    #define DisplayRefresh_RT1            (*(reg8 *) DisplayRefresh_TimerHW__RT1)
    #define DisplayRefresh_RT1_PTR        ( (reg8 *) DisplayRefresh_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define DisplayRefresh_CONTROL3       (*(reg8 *) DisplayRefresh_TimerHW__CFG2)
        #define DisplayRefresh_CONTROL3_PTR   ( (reg8 *) DisplayRefresh_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define DisplayRefresh_GLOBAL_ENABLE  (*(reg8 *) DisplayRefresh_TimerHW__PM_ACT_CFG)
    #define DisplayRefresh_GLOBAL_STBY_ENABLE  (*(reg8 *) DisplayRefresh_TimerHW__PM_STBY_CFG)

    #define DisplayRefresh_CAPTURE_LSB         (* (reg16 *) DisplayRefresh_TimerHW__CAP0 )
    #define DisplayRefresh_CAPTURE_LSB_PTR       ((reg16 *) DisplayRefresh_TimerHW__CAP0 )
    #define DisplayRefresh_PERIOD_LSB          (* (reg16 *) DisplayRefresh_TimerHW__PER0 )
    #define DisplayRefresh_PERIOD_LSB_PTR        ((reg16 *) DisplayRefresh_TimerHW__PER0 )
    #define DisplayRefresh_COUNTER_LSB         (* (reg16 *) DisplayRefresh_TimerHW__CNT_CMP0 )
    #define DisplayRefresh_COUNTER_LSB_PTR       ((reg16 *) DisplayRefresh_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define DisplayRefresh_BLOCK_EN_MASK                     DisplayRefresh_TimerHW__PM_ACT_MSK
    #define DisplayRefresh_BLOCK_STBY_EN_MASK                DisplayRefresh_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define DisplayRefresh_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define DisplayRefresh_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define DisplayRefresh_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define DisplayRefresh_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define DisplayRefresh_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define DisplayRefresh_CTRL_ENABLE                        ((uint8)((uint8)0x01u << DisplayRefresh_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define DisplayRefresh_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define DisplayRefresh_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << DisplayRefresh_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define DisplayRefresh_CTRL_MODE_SHIFT                     0x01u
        #define DisplayRefresh_CTRL_MODE_MASK                     ((uint8)((uint8)0x07u << DisplayRefresh_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define DisplayRefresh_CTRL_RCOD_SHIFT        0x02u
        #define DisplayRefresh_CTRL_ENBL_SHIFT        0x00u
        #define DisplayRefresh_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define DisplayRefresh_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << DisplayRefresh_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define DisplayRefresh_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << DisplayRefresh_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define DisplayRefresh_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << DisplayRefresh_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define DisplayRefresh_CTRL_RCOD       ((uint8)((uint8)0x03u << DisplayRefresh_CTRL_RCOD_SHIFT))
        #define DisplayRefresh_CTRL_ENBL       ((uint8)((uint8)0x80u << DisplayRefresh_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define DisplayRefresh_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define DisplayRefresh_RT1_MASK                        ((uint8)((uint8)0x03u << DisplayRefresh_RT1_SHIFT))
    #define DisplayRefresh_SYNC                            ((uint8)((uint8)0x03u << DisplayRefresh_RT1_SHIFT))
    #define DisplayRefresh_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define DisplayRefresh_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << DisplayRefresh_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define DisplayRefresh_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << DisplayRefresh_SYNCDSI_SHIFT))

    #define DisplayRefresh_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << DisplayRefresh_CTRL_MODE_SHIFT))
    #define DisplayRefresh_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << DisplayRefresh_CTRL_MODE_SHIFT))
    #define DisplayRefresh_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << DisplayRefresh_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define DisplayRefresh_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define DisplayRefresh_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define DisplayRefresh_STATUS_TC_INT_MASK_SHIFT        (DisplayRefresh_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define DisplayRefresh_STATUS_CAPTURE_INT_MASK_SHIFT   (DisplayRefresh_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define DisplayRefresh_STATUS_TC                       ((uint8)((uint8)0x01u << DisplayRefresh_STATUS_TC_SHIFT))
    #define DisplayRefresh_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << DisplayRefresh_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define DisplayRefresh_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << DisplayRefresh_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define DisplayRefresh_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << DisplayRefresh_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define DisplayRefresh_STATUS              (* (reg8 *) DisplayRefresh_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define DisplayRefresh_STATUS_MASK         (* (reg8 *) DisplayRefresh_TimerUDB_rstSts_stsreg__MASK_REG)
    #define DisplayRefresh_STATUS_AUX_CTRL     (* (reg8 *) DisplayRefresh_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define DisplayRefresh_CONTROL             (* (reg8 *) DisplayRefresh_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(DisplayRefresh_Resolution <= 8u) /* 8-bit Timer */
        #define DisplayRefresh_CAPTURE_LSB         (* (reg8 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define DisplayRefresh_CAPTURE_LSB_PTR       ((reg8 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define DisplayRefresh_PERIOD_LSB          (* (reg8 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define DisplayRefresh_PERIOD_LSB_PTR        ((reg8 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define DisplayRefresh_COUNTER_LSB         (* (reg8 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__A0_REG )
        #define DisplayRefresh_COUNTER_LSB_PTR       ((reg8 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__A0_REG )
    #elif(DisplayRefresh_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define DisplayRefresh_CAPTURE_LSB         (* (reg16 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define DisplayRefresh_CAPTURE_LSB_PTR       ((reg16 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define DisplayRefresh_PERIOD_LSB          (* (reg16 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define DisplayRefresh_PERIOD_LSB_PTR        ((reg16 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define DisplayRefresh_COUNTER_LSB         (* (reg16 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__A0_REG )
            #define DisplayRefresh_COUNTER_LSB_PTR       ((reg16 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define DisplayRefresh_CAPTURE_LSB         (* (reg16 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__16BIT_F0_REG )
            #define DisplayRefresh_CAPTURE_LSB_PTR       ((reg16 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__16BIT_F0_REG )
            #define DisplayRefresh_PERIOD_LSB          (* (reg16 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__16BIT_D0_REG )
            #define DisplayRefresh_PERIOD_LSB_PTR        ((reg16 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__16BIT_D0_REG )
            #define DisplayRefresh_COUNTER_LSB         (* (reg16 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__16BIT_A0_REG )
            #define DisplayRefresh_COUNTER_LSB_PTR       ((reg16 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(DisplayRefresh_Resolution <= 24u)/* 24-bit Timer */
        #define DisplayRefresh_CAPTURE_LSB         (* (reg32 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define DisplayRefresh_CAPTURE_LSB_PTR       ((reg32 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define DisplayRefresh_PERIOD_LSB          (* (reg32 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define DisplayRefresh_PERIOD_LSB_PTR        ((reg32 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define DisplayRefresh_COUNTER_LSB         (* (reg32 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__A0_REG )
        #define DisplayRefresh_COUNTER_LSB_PTR       ((reg32 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define DisplayRefresh_CAPTURE_LSB         (* (reg32 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define DisplayRefresh_CAPTURE_LSB_PTR       ((reg32 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define DisplayRefresh_PERIOD_LSB          (* (reg32 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define DisplayRefresh_PERIOD_LSB_PTR        ((reg32 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define DisplayRefresh_COUNTER_LSB         (* (reg32 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__A0_REG )
            #define DisplayRefresh_COUNTER_LSB_PTR       ((reg32 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define DisplayRefresh_CAPTURE_LSB         (* (reg32 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__32BIT_F0_REG )
            #define DisplayRefresh_CAPTURE_LSB_PTR       ((reg32 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__32BIT_F0_REG )
            #define DisplayRefresh_PERIOD_LSB          (* (reg32 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__32BIT_D0_REG )
            #define DisplayRefresh_PERIOD_LSB_PTR        ((reg32 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__32BIT_D0_REG )
            #define DisplayRefresh_COUNTER_LSB         (* (reg32 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__32BIT_A0_REG )
            #define DisplayRefresh_COUNTER_LSB_PTR       ((reg32 *) DisplayRefresh_TimerUDB_sT8_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #if (DisplayRefresh_UsingHWCaptureCounter)
        #define DisplayRefresh_CAP_COUNT              (*(reg8 *) DisplayRefresh_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define DisplayRefresh_CAP_COUNT_PTR          ( (reg8 *) DisplayRefresh_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define DisplayRefresh_CAPTURE_COUNT_CTRL     (*(reg8 *) DisplayRefresh_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define DisplayRefresh_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) DisplayRefresh_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (DisplayRefresh_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define DisplayRefresh_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define DisplayRefresh_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define DisplayRefresh_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define DisplayRefresh_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define DisplayRefresh_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define DisplayRefresh_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << DisplayRefresh_CTRL_INTCNT_SHIFT))
    #define DisplayRefresh_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << DisplayRefresh_CTRL_TRIG_MODE_SHIFT))
    #define DisplayRefresh_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << DisplayRefresh_CTRL_TRIG_EN_SHIFT))
    #define DisplayRefresh_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << DisplayRefresh_CTRL_CAP_MODE_SHIFT))
    #define DisplayRefresh_CTRL_ENABLE                    ((uint8)((uint8)0x01u << DisplayRefresh_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define DisplayRefresh_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define DisplayRefresh_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define DisplayRefresh_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define DisplayRefresh_STATUS_TC_INT_MASK_SHIFT       DisplayRefresh_STATUS_TC_SHIFT
    #define DisplayRefresh_STATUS_CAPTURE_INT_MASK_SHIFT  DisplayRefresh_STATUS_CAPTURE_SHIFT
    #define DisplayRefresh_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define DisplayRefresh_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define DisplayRefresh_STATUS_FIFOFULL_INT_MASK_SHIFT DisplayRefresh_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define DisplayRefresh_STATUS_TC                      ((uint8)((uint8)0x01u << DisplayRefresh_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define DisplayRefresh_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << DisplayRefresh_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define DisplayRefresh_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << DisplayRefresh_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define DisplayRefresh_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << DisplayRefresh_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define DisplayRefresh_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << DisplayRefresh_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define DisplayRefresh_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << DisplayRefresh_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define DisplayRefresh_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << DisplayRefresh_STATUS_FIFOFULL_SHIFT))

    #define DisplayRefresh_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define DisplayRefresh_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define DisplayRefresh_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define DisplayRefresh_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define DisplayRefresh_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define DisplayRefresh_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_Timer_v2_30_DisplayRefresh_H */


/* [] END OF FILE */
