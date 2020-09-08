/*******************************************************************************
* File Name: SecondTimer.h
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

#if !defined(CY_Timer_v2_30_SecondTimer_H)
#define CY_Timer_v2_30_SecondTimer_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 SecondTimer_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define SecondTimer_Resolution                 8u
#define SecondTimer_UsingFixedFunction         1u
#define SecondTimer_UsingHWCaptureCounter      0u
#define SecondTimer_SoftwareCaptureMode        0u
#define SecondTimer_SoftwareTriggerMode        0u
#define SecondTimer_UsingHWEnable              0u
#define SecondTimer_EnableTriggerMode          0u
#define SecondTimer_InterruptOnCaptureCount    0u
#define SecondTimer_RunModeUsed                0u
#define SecondTimer_ControlRegRemoved          0u


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!SecondTimer_UsingFixedFunction)
        #if (CY_UDB_V0)
            uint8 TimerUdb;                 /* Timer internal counter value */
            uint8 TimerPeriod;              /* Timer Period value       */
            uint8 InterruptMaskValue;       /* Timer Compare Value */
            #if (SecondTimer_UsingHWCaptureCounter)
                uint8 TimerCaptureCounter;  /* Timer Capture Counter Value */
            #endif /* variable declaration for backing up Capture Counter value*/
        #endif /* variables for non retention registers in CY_UDB_V0 */

        #if (CY_UDB_V1)
            uint8 TimerUdb;
            uint8 InterruptMaskValue;
            #if (SecondTimer_UsingHWCaptureCounter)
                uint8 TimerCaptureCounter;
            #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */
        #endif /* (CY_UDB_V1) */

        #if (!SecondTimer_ControlRegRemoved)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */
}SecondTimer_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    SecondTimer_Start(void) ;
void    SecondTimer_Stop(void) ;

void    SecondTimer_SetInterruptMode(uint8 interruptMode) ;
uint8   SecondTimer_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define SecondTimer_GetInterruptSource() SecondTimer_ReadStatusRegister()

#if(!SecondTimer_ControlRegRemoved)
    uint8   SecondTimer_ReadControlRegister(void) ;
    void    SecondTimer_WriteControlRegister(uint8 control) \
        ;
#endif /* (!SecondTimer_ControlRegRemoved) */

uint8  SecondTimer_ReadPeriod(void) ;
void    SecondTimer_WritePeriod(uint8 period) \
    ;
uint8  SecondTimer_ReadCounter(void) ;
void    SecondTimer_WriteCounter(uint8 counter) \
    ;
uint8  SecondTimer_ReadCapture(void) ;
void    SecondTimer_SoftwareCapture(void) ;


#if(!SecondTimer_UsingFixedFunction) /* UDB Prototypes */
    #if (SecondTimer_SoftwareCaptureMode)
        void    SecondTimer_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!SecondTimer_UsingFixedFunction) */

    #if (SecondTimer_SoftwareTriggerMode)
        void    SecondTimer_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (SecondTimer_SoftwareTriggerMode) */
    #if (SecondTimer_EnableTriggerMode)
        void    SecondTimer_EnableTrigger(void) ;
        void    SecondTimer_DisableTrigger(void) ;
    #endif /* (SecondTimer_EnableTriggerMode) */

    #if(SecondTimer_InterruptOnCaptureCount)
        #if(!SecondTimer_ControlRegRemoved)
            void    SecondTimer_SetInterruptCount(uint8 interruptCount) \
                ;
        #endif /* (!SecondTimer_ControlRegRemoved) */
    #endif /* (SecondTimer_InterruptOnCaptureCount) */

    #if (SecondTimer_UsingHWCaptureCounter)
        void    SecondTimer_SetCaptureCount(uint8 captureCount) \
            ;
        uint8   SecondTimer_ReadCaptureCount(void) ;
    #endif /* (SecondTimer_UsingHWCaptureCounter) */

    void SecondTimer_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void SecondTimer_Init(void)          ;
void SecondTimer_Enable(void)        ;
void SecondTimer_SaveConfig(void)    ;
void SecondTimer_RestoreConfig(void) ;
void SecondTimer_Sleep(void)         ;
void SecondTimer_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define SecondTimer__B_TIMER__CM_NONE 0
#define SecondTimer__B_TIMER__CM_RISINGEDGE 1
#define SecondTimer__B_TIMER__CM_FALLINGEDGE 2
#define SecondTimer__B_TIMER__CM_EITHEREDGE 3
#define SecondTimer__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define SecondTimer__B_TIMER__TM_NONE 0x00u
#define SecondTimer__B_TIMER__TM_RISINGEDGE 0x04u
#define SecondTimer__B_TIMER__TM_FALLINGEDGE 0x08u
#define SecondTimer__B_TIMER__TM_EITHEREDGE 0x0Cu
#define SecondTimer__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define SecondTimer_INIT_PERIOD             124u
#define SecondTimer_INIT_CAPTURE_MODE       ((uint8)((uint8)1u << SecondTimer_CTRL_CAP_MODE_SHIFT))
#define SecondTimer_INIT_TRIGGER_MODE       ((uint8)((uint8)0u << SecondTimer_CTRL_TRIG_MODE_SHIFT))
#if (SecondTimer_UsingFixedFunction)
    #define SecondTimer_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << SecondTimer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)0 << SecondTimer_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define SecondTimer_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << SecondTimer_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << SecondTimer_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << SecondTimer_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (SecondTimer_UsingFixedFunction) */
#define SecondTimer_INIT_CAPTURE_COUNT      (2u)
#define SecondTimer_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << SecondTimer_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (SecondTimer_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define SecondTimer_STATUS         (*(reg8 *) SecondTimer_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define SecondTimer_STATUS_MASK    (*(reg8 *) SecondTimer_TimerHW__SR0 )
    #define SecondTimer_CONTROL        (*(reg8 *) SecondTimer_TimerHW__CFG0)
    #define SecondTimer_CONTROL2       (*(reg8 *) SecondTimer_TimerHW__CFG1)
    #define SecondTimer_CONTROL2_PTR   ( (reg8 *) SecondTimer_TimerHW__CFG1)
    #define SecondTimer_RT1            (*(reg8 *) SecondTimer_TimerHW__RT1)
    #define SecondTimer_RT1_PTR        ( (reg8 *) SecondTimer_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define SecondTimer_CONTROL3       (*(reg8 *) SecondTimer_TimerHW__CFG2)
        #define SecondTimer_CONTROL3_PTR   ( (reg8 *) SecondTimer_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define SecondTimer_GLOBAL_ENABLE  (*(reg8 *) SecondTimer_TimerHW__PM_ACT_CFG)
    #define SecondTimer_GLOBAL_STBY_ENABLE  (*(reg8 *) SecondTimer_TimerHW__PM_STBY_CFG)

    #define SecondTimer_CAPTURE_LSB         (* (reg16 *) SecondTimer_TimerHW__CAP0 )
    #define SecondTimer_CAPTURE_LSB_PTR       ((reg16 *) SecondTimer_TimerHW__CAP0 )
    #define SecondTimer_PERIOD_LSB          (* (reg16 *) SecondTimer_TimerHW__PER0 )
    #define SecondTimer_PERIOD_LSB_PTR        ((reg16 *) SecondTimer_TimerHW__PER0 )
    #define SecondTimer_COUNTER_LSB         (* (reg16 *) SecondTimer_TimerHW__CNT_CMP0 )
    #define SecondTimer_COUNTER_LSB_PTR       ((reg16 *) SecondTimer_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define SecondTimer_BLOCK_EN_MASK                     SecondTimer_TimerHW__PM_ACT_MSK
    #define SecondTimer_BLOCK_STBY_EN_MASK                SecondTimer_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define SecondTimer_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define SecondTimer_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define SecondTimer_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define SecondTimer_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define SecondTimer_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define SecondTimer_CTRL_ENABLE                        ((uint8)((uint8)0x01u << SecondTimer_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define SecondTimer_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define SecondTimer_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << SecondTimer_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define SecondTimer_CTRL_MODE_SHIFT                     0x01u
        #define SecondTimer_CTRL_MODE_MASK                     ((uint8)((uint8)0x07u << SecondTimer_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define SecondTimer_CTRL_RCOD_SHIFT        0x02u
        #define SecondTimer_CTRL_ENBL_SHIFT        0x00u
        #define SecondTimer_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define SecondTimer_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << SecondTimer_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define SecondTimer_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << SecondTimer_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define SecondTimer_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << SecondTimer_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define SecondTimer_CTRL_RCOD       ((uint8)((uint8)0x03u << SecondTimer_CTRL_RCOD_SHIFT))
        #define SecondTimer_CTRL_ENBL       ((uint8)((uint8)0x80u << SecondTimer_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define SecondTimer_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define SecondTimer_RT1_MASK                        ((uint8)((uint8)0x03u << SecondTimer_RT1_SHIFT))
    #define SecondTimer_SYNC                            ((uint8)((uint8)0x03u << SecondTimer_RT1_SHIFT))
    #define SecondTimer_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define SecondTimer_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << SecondTimer_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define SecondTimer_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << SecondTimer_SYNCDSI_SHIFT))

    #define SecondTimer_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << SecondTimer_CTRL_MODE_SHIFT))
    #define SecondTimer_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << SecondTimer_CTRL_MODE_SHIFT))
    #define SecondTimer_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << SecondTimer_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define SecondTimer_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define SecondTimer_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define SecondTimer_STATUS_TC_INT_MASK_SHIFT        (SecondTimer_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define SecondTimer_STATUS_CAPTURE_INT_MASK_SHIFT   (SecondTimer_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define SecondTimer_STATUS_TC                       ((uint8)((uint8)0x01u << SecondTimer_STATUS_TC_SHIFT))
    #define SecondTimer_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << SecondTimer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define SecondTimer_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << SecondTimer_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define SecondTimer_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << SecondTimer_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define SecondTimer_STATUS              (* (reg8 *) SecondTimer_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define SecondTimer_STATUS_MASK         (* (reg8 *) SecondTimer_TimerUDB_rstSts_stsreg__MASK_REG)
    #define SecondTimer_STATUS_AUX_CTRL     (* (reg8 *) SecondTimer_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define SecondTimer_CONTROL             (* (reg8 *) SecondTimer_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(SecondTimer_Resolution <= 8u) /* 8-bit Timer */
        #define SecondTimer_CAPTURE_LSB         (* (reg8 *) SecondTimer_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define SecondTimer_CAPTURE_LSB_PTR       ((reg8 *) SecondTimer_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define SecondTimer_PERIOD_LSB          (* (reg8 *) SecondTimer_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define SecondTimer_PERIOD_LSB_PTR        ((reg8 *) SecondTimer_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define SecondTimer_COUNTER_LSB         (* (reg8 *) SecondTimer_TimerUDB_sT8_timerdp_u0__A0_REG )
        #define SecondTimer_COUNTER_LSB_PTR       ((reg8 *) SecondTimer_TimerUDB_sT8_timerdp_u0__A0_REG )
    #elif(SecondTimer_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define SecondTimer_CAPTURE_LSB         (* (reg16 *) SecondTimer_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define SecondTimer_CAPTURE_LSB_PTR       ((reg16 *) SecondTimer_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define SecondTimer_PERIOD_LSB          (* (reg16 *) SecondTimer_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define SecondTimer_PERIOD_LSB_PTR        ((reg16 *) SecondTimer_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define SecondTimer_COUNTER_LSB         (* (reg16 *) SecondTimer_TimerUDB_sT8_timerdp_u0__A0_REG )
            #define SecondTimer_COUNTER_LSB_PTR       ((reg16 *) SecondTimer_TimerUDB_sT8_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define SecondTimer_CAPTURE_LSB         (* (reg16 *) SecondTimer_TimerUDB_sT8_timerdp_u0__16BIT_F0_REG )
            #define SecondTimer_CAPTURE_LSB_PTR       ((reg16 *) SecondTimer_TimerUDB_sT8_timerdp_u0__16BIT_F0_REG )
            #define SecondTimer_PERIOD_LSB          (* (reg16 *) SecondTimer_TimerUDB_sT8_timerdp_u0__16BIT_D0_REG )
            #define SecondTimer_PERIOD_LSB_PTR        ((reg16 *) SecondTimer_TimerUDB_sT8_timerdp_u0__16BIT_D0_REG )
            #define SecondTimer_COUNTER_LSB         (* (reg16 *) SecondTimer_TimerUDB_sT8_timerdp_u0__16BIT_A0_REG )
            #define SecondTimer_COUNTER_LSB_PTR       ((reg16 *) SecondTimer_TimerUDB_sT8_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(SecondTimer_Resolution <= 24u)/* 24-bit Timer */
        #define SecondTimer_CAPTURE_LSB         (* (reg32 *) SecondTimer_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define SecondTimer_CAPTURE_LSB_PTR       ((reg32 *) SecondTimer_TimerUDB_sT8_timerdp_u0__F0_REG )
        #define SecondTimer_PERIOD_LSB          (* (reg32 *) SecondTimer_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define SecondTimer_PERIOD_LSB_PTR        ((reg32 *) SecondTimer_TimerUDB_sT8_timerdp_u0__D0_REG )
        #define SecondTimer_COUNTER_LSB         (* (reg32 *) SecondTimer_TimerUDB_sT8_timerdp_u0__A0_REG )
        #define SecondTimer_COUNTER_LSB_PTR       ((reg32 *) SecondTimer_TimerUDB_sT8_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define SecondTimer_CAPTURE_LSB         (* (reg32 *) SecondTimer_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define SecondTimer_CAPTURE_LSB_PTR       ((reg32 *) SecondTimer_TimerUDB_sT8_timerdp_u0__F0_REG )
            #define SecondTimer_PERIOD_LSB          (* (reg32 *) SecondTimer_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define SecondTimer_PERIOD_LSB_PTR        ((reg32 *) SecondTimer_TimerUDB_sT8_timerdp_u0__D0_REG )
            #define SecondTimer_COUNTER_LSB         (* (reg32 *) SecondTimer_TimerUDB_sT8_timerdp_u0__A0_REG )
            #define SecondTimer_COUNTER_LSB_PTR       ((reg32 *) SecondTimer_TimerUDB_sT8_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define SecondTimer_CAPTURE_LSB         (* (reg32 *) SecondTimer_TimerUDB_sT8_timerdp_u0__32BIT_F0_REG )
            #define SecondTimer_CAPTURE_LSB_PTR       ((reg32 *) SecondTimer_TimerUDB_sT8_timerdp_u0__32BIT_F0_REG )
            #define SecondTimer_PERIOD_LSB          (* (reg32 *) SecondTimer_TimerUDB_sT8_timerdp_u0__32BIT_D0_REG )
            #define SecondTimer_PERIOD_LSB_PTR        ((reg32 *) SecondTimer_TimerUDB_sT8_timerdp_u0__32BIT_D0_REG )
            #define SecondTimer_COUNTER_LSB         (* (reg32 *) SecondTimer_TimerUDB_sT8_timerdp_u0__32BIT_A0_REG )
            #define SecondTimer_COUNTER_LSB_PTR       ((reg32 *) SecondTimer_TimerUDB_sT8_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #if (SecondTimer_UsingHWCaptureCounter)
        #define SecondTimer_CAP_COUNT              (*(reg8 *) SecondTimer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define SecondTimer_CAP_COUNT_PTR          ( (reg8 *) SecondTimer_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define SecondTimer_CAPTURE_COUNT_CTRL     (*(reg8 *) SecondTimer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define SecondTimer_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) SecondTimer_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (SecondTimer_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define SecondTimer_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define SecondTimer_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define SecondTimer_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define SecondTimer_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define SecondTimer_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define SecondTimer_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << SecondTimer_CTRL_INTCNT_SHIFT))
    #define SecondTimer_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << SecondTimer_CTRL_TRIG_MODE_SHIFT))
    #define SecondTimer_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << SecondTimer_CTRL_TRIG_EN_SHIFT))
    #define SecondTimer_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << SecondTimer_CTRL_CAP_MODE_SHIFT))
    #define SecondTimer_CTRL_ENABLE                    ((uint8)((uint8)0x01u << SecondTimer_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define SecondTimer_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define SecondTimer_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define SecondTimer_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define SecondTimer_STATUS_TC_INT_MASK_SHIFT       SecondTimer_STATUS_TC_SHIFT
    #define SecondTimer_STATUS_CAPTURE_INT_MASK_SHIFT  SecondTimer_STATUS_CAPTURE_SHIFT
    #define SecondTimer_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define SecondTimer_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define SecondTimer_STATUS_FIFOFULL_INT_MASK_SHIFT SecondTimer_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define SecondTimer_STATUS_TC                      ((uint8)((uint8)0x01u << SecondTimer_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define SecondTimer_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << SecondTimer_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define SecondTimer_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << SecondTimer_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define SecondTimer_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << SecondTimer_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define SecondTimer_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << SecondTimer_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define SecondTimer_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << SecondTimer_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define SecondTimer_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << SecondTimer_STATUS_FIFOFULL_SHIFT))

    #define SecondTimer_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define SecondTimer_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define SecondTimer_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define SecondTimer_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define SecondTimer_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define SecondTimer_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_Timer_v2_30_SecondTimer_H */


/* [] END OF FILE */
