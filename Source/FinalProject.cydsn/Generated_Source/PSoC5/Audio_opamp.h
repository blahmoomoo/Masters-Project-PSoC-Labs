/*******************************************************************************
* File Name: Audio_opamp.h  
* Version 1.90
*
* Description:
*  This file contains the function prototypes and constants used in
*  the OpAmp (Analog Buffer) Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_OPAMP_Audio_opamp_H) 
#define CY_OPAMP_Audio_opamp_H 

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h"

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component OpAmp_v1_90 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */


/***************************************
*   Data Struct Definition
***************************************/

/* Low power Mode API Support */
typedef struct
{
    uint8   enableState;
}   Audio_opamp_BACKUP_STRUCT;

/* Variable describes initial state of the component */
extern uint8 Audio_opamp_initVar;


/**************************************
*        Function Prototypes 
**************************************/

void Audio_opamp_Start(void)               ;
void Audio_opamp_Stop(void)                ;
void Audio_opamp_SetPower(uint8 power)     ;
void Audio_opamp_Sleep(void)               ;
void Audio_opamp_Wakeup(void)              ;
void Audio_opamp_SaveConfig(void)          ;
void Audio_opamp_RestoreConfig(void)       ;
void Audio_opamp_Init(void)                ;
void Audio_opamp_Enable(void)              ;


/**************************************
*           API Constants
**************************************/

/* Power constants for SetPower() function */
#define Audio_opamp_LPOCPOWER              (0x00u)
#define Audio_opamp_LOWPOWER               (0x01u)
#define Audio_opamp_MEDPOWER               (0x02u)
#define Audio_opamp_HIGHPOWER              (0x03u)


/**************************************
*           Parameter Defaults        
**************************************/
#define Audio_opamp_DEFAULT_POWER          (2u)
#define Audio_opamp_DEFAULT_MODE           (1u)


/**************************************
*             Registers
**************************************/

#define Audio_opamp_CR_REG                 (* (reg8 *) Audio_opamp_ABuf__CR)
#define Audio_opamp_CR_PTR                 (  (reg8 *) Audio_opamp_ABuf__CR)

#define Audio_opamp_MX_REG                 (* (reg8 *) Audio_opamp_ABuf__MX)
#define Audio_opamp_MX_PTR                 (  (reg8 *) Audio_opamp_ABuf__MX)

#define Audio_opamp_SW_REG                 (* (reg8 *) Audio_opamp_ABuf__SW)
#define Audio_opamp_SW_PTR                 (  (reg8 *) Audio_opamp_ABuf__SW)

/* Active mode power manager register */
#define Audio_opamp_PM_ACT_CFG_REG         (* (reg8 *) Audio_opamp_ABuf__PM_ACT_CFG)
#define Audio_opamp_PM_ACT_CFG_PTR         (  (reg8 *) Audio_opamp_ABuf__PM_ACT_CFG)

/* Alternative mode power manager register */
#define Audio_opamp_PM_STBY_CFG_REG        (* (reg8 *) Audio_opamp_ABuf__PM_STBY_CFG)
#define Audio_opamp_PM_STBY_CFG_PTR        (  (reg8 *) Audio_opamp_ABuf__PM_STBY_CFG)

/* ANIF.PUMP.CR1 Pump Configuration Register 1 */
#define Audio_opamp_PUMP_CR1_REG           (* (reg8 *) CYDEV_ANAIF_CFG_PUMP_CR1)
#define Audio_opamp_PUMP_CR1_PTR           (  (reg8 *) CYDEV_ANAIF_CFG_PUMP_CR1)

/* Trim register defines */
#define Audio_opamp_TR0_REG                (* (reg8 *) Audio_opamp_ABuf__TR0)
#define Audio_opamp_TR0_PTR                (  (reg8 *) Audio_opamp_ABuf__TR0)

#define Audio_opamp_TR1_REG                (* (reg8 *) Audio_opamp_ABuf__TR1)
#define Audio_opamp_TR1_PTR                (  (reg8 *) Audio_opamp_ABuf__TR1)


/**************************************
*       Register Constants
**************************************/

/* CX Analog Buffer Input Selection Register */

/* Power mode defines */

/* PM_ACT_CFG (Active Power Mode CFG Register) mask */ 
#define Audio_opamp_ACT_PWR_EN             Audio_opamp_ABuf__PM_ACT_MSK 
/* PM_STBY_CFG (Alternative Active Power Mode CFG Register) mask */ 
#define Audio_opamp_STBY_PWR_EN            Audio_opamp_ABuf__PM_STBY_MSK 
/* Power mask */
#define Audio_opamp_PWR_MASK               (0x03u)

/* MX Analog Buffer Input Selection Register */

/* Bit Field  MX_VN */
#define Audio_opamp_MX_VN_MASK             (0x30u)
#define Audio_opamp_MX_VN_NC               (0x00u)
#define Audio_opamp_MX_VN_AG4              (0x10u)
#define Audio_opamp_MX_VN_AG6              (0x20u)

/* Bit Field  MX_VP */
#define Audio_opamp_MX_VP_MASK             (0x0Fu)
#define Audio_opamp_MX_VP_NC               (0x00u)
#define Audio_opamp_MX_VP_VREF             (0x09u)
#define Audio_opamp_MX_VP_AG4              (0x01u)
#define Audio_opamp_MX_VP_AG5              (0x02u)
#define Audio_opamp_MX_VP_AG6              (0x03u)
#define Audio_opamp_MX_VP_AG7              (0x04u)
#define Audio_opamp_MX_VP_ABUS0            (0x05u)
#define Audio_opamp_MX_VP_ABUS1            (0x06u)
#define Audio_opamp_MX_VP_ABUS2            (0x07u)
#define Audio_opamp_MX_VP_ABUS3            (0x08u)

/* SW Analog Buffer Routing Switch Reg */

/* Bit Field  SW */
#define Audio_opamp_SW_MASK                (0x07u) 
#define Audio_opamp_SW_SWINP               (0x04u)  /* Enable positive input */
#define Audio_opamp_SW_SWINN               (0x02u)  /* Enable negative input */

/* Pump configuration register masks */
#define Audio_opamp_PUMP_CR1_CLKSEL        (0x40u)
#define Audio_opamp_PUMP_CR1_FORCE         (0x20u)
#define Audio_opamp_PUMP_CR1_AUTO          (0x10u)

#endif /* CY_OPAMP_Audio_opamp_H */


/* [] END OF FILE */
