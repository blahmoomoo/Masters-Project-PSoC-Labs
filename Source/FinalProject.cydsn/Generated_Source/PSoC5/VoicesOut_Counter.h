/*******************************************************************************
* File Name: VoicesOut_Counter.h
* Version 1.0
*
* Description:
*  This header file contains registers and constants associated with the
*  Count7 component.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_COUNT7_VoicesOut_Counter_H)
#define CY_COUNT7_VoicesOut_Counter_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
* Function Prototypes
***************************************/
void  VoicesOut_Counter_Init(void) ;
void  VoicesOut_Counter_Enable(void) ;
void  VoicesOut_Counter_Start(void) ;
void  VoicesOut_Counter_Stop(void) ;
void  VoicesOut_Counter_WriteCounter(uint8 count) ;
uint8 VoicesOut_Counter_ReadCounter(void) ;
void  VoicesOut_Counter_WritePeriod(uint8 period) ;
uint8 VoicesOut_Counter_ReadPeriod(void) ;
void  VoicesOut_Counter_SaveConfig(void) ;
void  VoicesOut_Counter_RestoreConfig(void) ;
void  VoicesOut_Counter_Sleep(void) ;
void  VoicesOut_Counter_Wakeup(void) ;


/***************************************
*     Data Struct Definitions
***************************************/
/* Structure to save registers before go to sleep */
typedef struct
{
    uint8 enableState;
    uint8 count;
} VoicesOut_Counter_BACKUP_STRUCT;


/***************************************
*           Global Variables
***************************************/
extern VoicesOut_Counter_BACKUP_STRUCT VoicesOut_Counter_backup;
extern uint8 VoicesOut_Counter_initVar;


/***************************************
* Initial Parameter
***************************************/
#define VoicesOut_Counter_INITIAL_PERIOD               (2u)


/***************************************
* Registers
***************************************/
#define VoicesOut_Counter_COUNT_REG                    (*(reg8 *) VoicesOut_Counter_Counter7__COUNT_REG)
#define VoicesOut_Counter_COUNT_PTR                    ( (reg8 *) VoicesOut_Counter_Counter7__COUNT_REG)
#define VoicesOut_Counter_PERIOD_REG                   (*(reg8 *) VoicesOut_Counter_Counter7__PERIOD_REG)
#define VoicesOut_Counter_PERIOD_PTR                   ( (reg8 *) VoicesOut_Counter_Counter7__PERIOD_REG)
#define VoicesOut_Counter_AUX_CONTROL_REG              (*(reg8 *) VoicesOut_Counter_Counter7__CONTROL_AUX_CTL_REG)
#define VoicesOut_Counter_AUX_CONTROL_PTR              ( (reg8 *) VoicesOut_Counter_Counter7__CONTROL_AUX_CTL_REG)


/***************************************
* Register Constants
***************************************/
#define VoicesOut_Counter_COUNTER_START                (0x20u)

/* This constant is used to mask the TC bit (bit#7) in the Count register */
#define VoicesOut_Counter_COUNT_7BIT_MASK              (0x7Fu)


#endif /* CY_COUNT7_VoicesOut_Counter_H */


/* [] END OF FILE */
