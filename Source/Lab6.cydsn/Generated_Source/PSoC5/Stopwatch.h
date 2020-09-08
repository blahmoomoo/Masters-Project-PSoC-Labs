/*******************************************************************************
* File Name: Stopwatch.h
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

#if !defined(CY_Timer_v2_30_Stopwatch_H)
#define CY_Timer_v2_30_Stopwatch_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 Stopwatch_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Timer_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/**************************************
*           Parameter Defaults
**************************************/

#define Stopwatch_Resolution                 32u
#define Stopwatch_UsingFixedFunction         0u
#define Stopwatch_UsingHWCaptureCounter      0u
#define Stopwatch_SoftwareCaptureMode        0u
#define Stopwatch_SoftwareTriggerMode        0u
#define Stopwatch_UsingHWEnable              0u
#define Stopwatch_EnableTriggerMode          1u
#define Stopwatch_InterruptOnCaptureCount    1u
#define Stopwatch_RunModeUsed                0u
#define Stopwatch_ControlRegRemoved          0u


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!Stopwatch_UsingFixedFunction)
        #if (CY_UDB_V0)
            uint32 TimerUdb;                 /* Timer internal counter value */
            uint32 TimerPeriod;              /* Timer Period value       */
            uint8 InterruptMaskValue;       /* Timer Compare Value */
            #if (Stopwatch_UsingHWCaptureCounter)
                uint8 TimerCaptureCounter;  /* Timer Capture Counter Value */
            #endif /* variable declaration for backing up Capture Counter value*/
        #endif /* variables for non retention registers in CY_UDB_V0 */

        #if (CY_UDB_V1)
            uint32 TimerUdb;
            uint8 InterruptMaskValue;
            #if (Stopwatch_UsingHWCaptureCounter)
                uint8 TimerCaptureCounter;
            #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */
        #endif /* (CY_UDB_V1) */

        #if (!Stopwatch_ControlRegRemoved)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */
}Stopwatch_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    Stopwatch_Start(void) ;
void    Stopwatch_Stop(void) ;

void    Stopwatch_SetInterruptMode(uint8 interruptMode) ;
uint8   Stopwatch_ReadStatusRegister(void) ;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define Stopwatch_GetInterruptSource() Stopwatch_ReadStatusRegister()

#if(!Stopwatch_ControlRegRemoved)
    uint8   Stopwatch_ReadControlRegister(void) ;
    void    Stopwatch_WriteControlRegister(uint8 control) \
        ;
#endif /* (!Stopwatch_ControlRegRemoved) */

uint32  Stopwatch_ReadPeriod(void) ;
void    Stopwatch_WritePeriod(uint32 period) \
    ;
uint32  Stopwatch_ReadCounter(void) ;
void    Stopwatch_WriteCounter(uint32 counter) \
    ;
uint32  Stopwatch_ReadCapture(void) ;
void    Stopwatch_SoftwareCapture(void) ;


#if(!Stopwatch_UsingFixedFunction) /* UDB Prototypes */
    #if (Stopwatch_SoftwareCaptureMode)
        void    Stopwatch_SetCaptureMode(uint8 captureMode) ;
    #endif /* (!Stopwatch_UsingFixedFunction) */

    #if (Stopwatch_SoftwareTriggerMode)
        void    Stopwatch_SetTriggerMode(uint8 triggerMode) ;
    #endif /* (Stopwatch_SoftwareTriggerMode) */
    #if (Stopwatch_EnableTriggerMode)
        void    Stopwatch_EnableTrigger(void) ;
        void    Stopwatch_DisableTrigger(void) ;
    #endif /* (Stopwatch_EnableTriggerMode) */

    #if(Stopwatch_InterruptOnCaptureCount)
        #if(!Stopwatch_ControlRegRemoved)
            void    Stopwatch_SetInterruptCount(uint8 interruptCount) \
                ;
        #endif /* (!Stopwatch_ControlRegRemoved) */
    #endif /* (Stopwatch_InterruptOnCaptureCount) */

    #if (Stopwatch_UsingHWCaptureCounter)
        void    Stopwatch_SetCaptureCount(uint8 captureCount) \
            ;
        uint8   Stopwatch_ReadCaptureCount(void) ;
    #endif /* (Stopwatch_UsingHWCaptureCounter) */

    void Stopwatch_ClearFIFO(void) ;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void Stopwatch_Init(void)          ;
void Stopwatch_Enable(void)        ;
void Stopwatch_SaveConfig(void)    ;
void Stopwatch_RestoreConfig(void) ;
void Stopwatch_Sleep(void)         ;
void Stopwatch_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
#define Stopwatch__B_TIMER__CM_NONE 0
#define Stopwatch__B_TIMER__CM_RISINGEDGE 1
#define Stopwatch__B_TIMER__CM_FALLINGEDGE 2
#define Stopwatch__B_TIMER__CM_EITHEREDGE 3
#define Stopwatch__B_TIMER__CM_SOFTWARE 4



/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define Stopwatch__B_TIMER__TM_NONE 0x00u
#define Stopwatch__B_TIMER__TM_RISINGEDGE 0x04u
#define Stopwatch__B_TIMER__TM_FALLINGEDGE 0x08u
#define Stopwatch__B_TIMER__TM_EITHEREDGE 0x0Cu
#define Stopwatch__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define Stopwatch_INIT_PERIOD             4294967295u
#define Stopwatch_INIT_CAPTURE_MODE       ((uint8)((uint8)1u << Stopwatch_CTRL_CAP_MODE_SHIFT))
#define Stopwatch_INIT_TRIGGER_MODE       ((uint8)((uint8)1u << Stopwatch_CTRL_TRIG_MODE_SHIFT))
#if (Stopwatch_UsingFixedFunction)
    #define Stopwatch_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << Stopwatch_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)1 << Stopwatch_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define Stopwatch_INIT_INTERRUPT_MODE (((uint8)((uint8)0u << Stopwatch_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)1 << Stopwatch_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)0 << Stopwatch_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (Stopwatch_UsingFixedFunction) */
#define Stopwatch_INIT_CAPTURE_COUNT      (2u)
#define Stopwatch_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(1u - 1u) << Stopwatch_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (Stopwatch_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define Stopwatch_STATUS         (*(reg8 *) Stopwatch_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define Stopwatch_STATUS_MASK    (*(reg8 *) Stopwatch_TimerHW__SR0 )
    #define Stopwatch_CONTROL        (*(reg8 *) Stopwatch_TimerHW__CFG0)
    #define Stopwatch_CONTROL2       (*(reg8 *) Stopwatch_TimerHW__CFG1)
    #define Stopwatch_CONTROL2_PTR   ( (reg8 *) Stopwatch_TimerHW__CFG1)
    #define Stopwatch_RT1            (*(reg8 *) Stopwatch_TimerHW__RT1)
    #define Stopwatch_RT1_PTR        ( (reg8 *) Stopwatch_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define Stopwatch_CONTROL3       (*(reg8 *) Stopwatch_TimerHW__CFG2)
        #define Stopwatch_CONTROL3_PTR   ( (reg8 *) Stopwatch_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define Stopwatch_GLOBAL_ENABLE  (*(reg8 *) Stopwatch_TimerHW__PM_ACT_CFG)
    #define Stopwatch_GLOBAL_STBY_ENABLE  (*(reg8 *) Stopwatch_TimerHW__PM_STBY_CFG)

    #define Stopwatch_CAPTURE_LSB         (* (reg16 *) Stopwatch_TimerHW__CAP0 )
    #define Stopwatch_CAPTURE_LSB_PTR       ((reg16 *) Stopwatch_TimerHW__CAP0 )
    #define Stopwatch_PERIOD_LSB          (* (reg16 *) Stopwatch_TimerHW__PER0 )
    #define Stopwatch_PERIOD_LSB_PTR        ((reg16 *) Stopwatch_TimerHW__PER0 )
    #define Stopwatch_COUNTER_LSB         (* (reg16 *) Stopwatch_TimerHW__CNT_CMP0 )
    #define Stopwatch_COUNTER_LSB_PTR       ((reg16 *) Stopwatch_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define Stopwatch_BLOCK_EN_MASK                     Stopwatch_TimerHW__PM_ACT_MSK
    #define Stopwatch_BLOCK_STBY_EN_MASK                Stopwatch_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define Stopwatch_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define Stopwatch_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define Stopwatch_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define Stopwatch_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define Stopwatch_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define Stopwatch_CTRL_ENABLE                        ((uint8)((uint8)0x01u << Stopwatch_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define Stopwatch_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define Stopwatch_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << Stopwatch_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define Stopwatch_CTRL_MODE_SHIFT                     0x01u
        #define Stopwatch_CTRL_MODE_MASK                     ((uint8)((uint8)0x07u << Stopwatch_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define Stopwatch_CTRL_RCOD_SHIFT        0x02u
        #define Stopwatch_CTRL_ENBL_SHIFT        0x00u
        #define Stopwatch_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define Stopwatch_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << Stopwatch_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define Stopwatch_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << Stopwatch_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define Stopwatch_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << Stopwatch_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define Stopwatch_CTRL_RCOD       ((uint8)((uint8)0x03u << Stopwatch_CTRL_RCOD_SHIFT))
        #define Stopwatch_CTRL_ENBL       ((uint8)((uint8)0x80u << Stopwatch_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define Stopwatch_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define Stopwatch_RT1_MASK                        ((uint8)((uint8)0x03u << Stopwatch_RT1_SHIFT))
    #define Stopwatch_SYNC                            ((uint8)((uint8)0x03u << Stopwatch_RT1_SHIFT))
    #define Stopwatch_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define Stopwatch_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << Stopwatch_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define Stopwatch_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << Stopwatch_SYNCDSI_SHIFT))

    #define Stopwatch_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << Stopwatch_CTRL_MODE_SHIFT))
    #define Stopwatch_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << Stopwatch_CTRL_MODE_SHIFT))
    #define Stopwatch_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << Stopwatch_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define Stopwatch_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define Stopwatch_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define Stopwatch_STATUS_TC_INT_MASK_SHIFT        (Stopwatch_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define Stopwatch_STATUS_CAPTURE_INT_MASK_SHIFT   (Stopwatch_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define Stopwatch_STATUS_TC                       ((uint8)((uint8)0x01u << Stopwatch_STATUS_TC_SHIFT))
    #define Stopwatch_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << Stopwatch_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define Stopwatch_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << Stopwatch_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define Stopwatch_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << Stopwatch_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define Stopwatch_STATUS              (* (reg8 *) Stopwatch_TimerUDB_rstSts_stsreg__STATUS_REG )
    #define Stopwatch_STATUS_MASK         (* (reg8 *) Stopwatch_TimerUDB_rstSts_stsreg__MASK_REG)
    #define Stopwatch_STATUS_AUX_CTRL     (* (reg8 *) Stopwatch_TimerUDB_rstSts_stsreg__STATUS_AUX_CTL_REG)
    #define Stopwatch_CONTROL             (* (reg8 *) Stopwatch_TimerUDB_sCTRLReg_SyncCtl_ctrlreg__CONTROL_REG )
    
    #if(Stopwatch_Resolution <= 8u) /* 8-bit Timer */
        #define Stopwatch_CAPTURE_LSB         (* (reg8 *) Stopwatch_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define Stopwatch_CAPTURE_LSB_PTR       ((reg8 *) Stopwatch_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define Stopwatch_PERIOD_LSB          (* (reg8 *) Stopwatch_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define Stopwatch_PERIOD_LSB_PTR        ((reg8 *) Stopwatch_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define Stopwatch_COUNTER_LSB         (* (reg8 *) Stopwatch_TimerUDB_sT32_timerdp_u0__A0_REG )
        #define Stopwatch_COUNTER_LSB_PTR       ((reg8 *) Stopwatch_TimerUDB_sT32_timerdp_u0__A0_REG )
    #elif(Stopwatch_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define Stopwatch_CAPTURE_LSB         (* (reg16 *) Stopwatch_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define Stopwatch_CAPTURE_LSB_PTR       ((reg16 *) Stopwatch_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define Stopwatch_PERIOD_LSB          (* (reg16 *) Stopwatch_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define Stopwatch_PERIOD_LSB_PTR        ((reg16 *) Stopwatch_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define Stopwatch_COUNTER_LSB         (* (reg16 *) Stopwatch_TimerUDB_sT32_timerdp_u0__A0_REG )
            #define Stopwatch_COUNTER_LSB_PTR       ((reg16 *) Stopwatch_TimerUDB_sT32_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define Stopwatch_CAPTURE_LSB         (* (reg16 *) Stopwatch_TimerUDB_sT32_timerdp_u0__16BIT_F0_REG )
            #define Stopwatch_CAPTURE_LSB_PTR       ((reg16 *) Stopwatch_TimerUDB_sT32_timerdp_u0__16BIT_F0_REG )
            #define Stopwatch_PERIOD_LSB          (* (reg16 *) Stopwatch_TimerUDB_sT32_timerdp_u0__16BIT_D0_REG )
            #define Stopwatch_PERIOD_LSB_PTR        ((reg16 *) Stopwatch_TimerUDB_sT32_timerdp_u0__16BIT_D0_REG )
            #define Stopwatch_COUNTER_LSB         (* (reg16 *) Stopwatch_TimerUDB_sT32_timerdp_u0__16BIT_A0_REG )
            #define Stopwatch_COUNTER_LSB_PTR       ((reg16 *) Stopwatch_TimerUDB_sT32_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(Stopwatch_Resolution <= 24u)/* 24-bit Timer */
        #define Stopwatch_CAPTURE_LSB         (* (reg32 *) Stopwatch_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define Stopwatch_CAPTURE_LSB_PTR       ((reg32 *) Stopwatch_TimerUDB_sT32_timerdp_u0__F0_REG )
        #define Stopwatch_PERIOD_LSB          (* (reg32 *) Stopwatch_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define Stopwatch_PERIOD_LSB_PTR        ((reg32 *) Stopwatch_TimerUDB_sT32_timerdp_u0__D0_REG )
        #define Stopwatch_COUNTER_LSB         (* (reg32 *) Stopwatch_TimerUDB_sT32_timerdp_u0__A0_REG )
        #define Stopwatch_COUNTER_LSB_PTR       ((reg32 *) Stopwatch_TimerUDB_sT32_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define Stopwatch_CAPTURE_LSB         (* (reg32 *) Stopwatch_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define Stopwatch_CAPTURE_LSB_PTR       ((reg32 *) Stopwatch_TimerUDB_sT32_timerdp_u0__F0_REG )
            #define Stopwatch_PERIOD_LSB          (* (reg32 *) Stopwatch_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define Stopwatch_PERIOD_LSB_PTR        ((reg32 *) Stopwatch_TimerUDB_sT32_timerdp_u0__D0_REG )
            #define Stopwatch_COUNTER_LSB         (* (reg32 *) Stopwatch_TimerUDB_sT32_timerdp_u0__A0_REG )
            #define Stopwatch_COUNTER_LSB_PTR       ((reg32 *) Stopwatch_TimerUDB_sT32_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define Stopwatch_CAPTURE_LSB         (* (reg32 *) Stopwatch_TimerUDB_sT32_timerdp_u0__32BIT_F0_REG )
            #define Stopwatch_CAPTURE_LSB_PTR       ((reg32 *) Stopwatch_TimerUDB_sT32_timerdp_u0__32BIT_F0_REG )
            #define Stopwatch_PERIOD_LSB          (* (reg32 *) Stopwatch_TimerUDB_sT32_timerdp_u0__32BIT_D0_REG )
            #define Stopwatch_PERIOD_LSB_PTR        ((reg32 *) Stopwatch_TimerUDB_sT32_timerdp_u0__32BIT_D0_REG )
            #define Stopwatch_COUNTER_LSB         (* (reg32 *) Stopwatch_TimerUDB_sT32_timerdp_u0__32BIT_A0_REG )
            #define Stopwatch_COUNTER_LSB_PTR       ((reg32 *) Stopwatch_TimerUDB_sT32_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #if (Stopwatch_UsingHWCaptureCounter)
        #define Stopwatch_CAP_COUNT              (*(reg8 *) Stopwatch_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define Stopwatch_CAP_COUNT_PTR          ( (reg8 *) Stopwatch_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define Stopwatch_CAPTURE_COUNT_CTRL     (*(reg8 *) Stopwatch_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define Stopwatch_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) Stopwatch_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (Stopwatch_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define Stopwatch_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define Stopwatch_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define Stopwatch_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define Stopwatch_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define Stopwatch_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define Stopwatch_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << Stopwatch_CTRL_INTCNT_SHIFT))
    #define Stopwatch_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << Stopwatch_CTRL_TRIG_MODE_SHIFT))
    #define Stopwatch_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << Stopwatch_CTRL_TRIG_EN_SHIFT))
    #define Stopwatch_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << Stopwatch_CTRL_CAP_MODE_SHIFT))
    #define Stopwatch_CTRL_ENABLE                    ((uint8)((uint8)0x01u << Stopwatch_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define Stopwatch_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define Stopwatch_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define Stopwatch_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define Stopwatch_STATUS_TC_INT_MASK_SHIFT       Stopwatch_STATUS_TC_SHIFT
    #define Stopwatch_STATUS_CAPTURE_INT_MASK_SHIFT  Stopwatch_STATUS_CAPTURE_SHIFT
    #define Stopwatch_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define Stopwatch_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define Stopwatch_STATUS_FIFOFULL_INT_MASK_SHIFT Stopwatch_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define Stopwatch_STATUS_TC                      ((uint8)((uint8)0x01u << Stopwatch_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define Stopwatch_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << Stopwatch_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Stopwatch_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << Stopwatch_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Stopwatch_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << Stopwatch_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define Stopwatch_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << Stopwatch_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define Stopwatch_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << Stopwatch_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define Stopwatch_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << Stopwatch_STATUS_FIFOFULL_SHIFT))

    #define Stopwatch_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define Stopwatch_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define Stopwatch_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define Stopwatch_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define Stopwatch_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define Stopwatch_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_Timer_v2_30_Stopwatch_H */


/* [] END OF FILE */
