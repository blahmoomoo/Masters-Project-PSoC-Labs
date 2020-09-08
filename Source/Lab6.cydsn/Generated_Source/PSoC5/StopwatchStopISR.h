/*******************************************************************************
* File Name: StopwatchStopISR.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_StopwatchStopISR_H)
#define CY_ISR_StopwatchStopISR_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void StopwatchStopISR_Start(void);
void StopwatchStopISR_StartEx(cyisraddress address);
void StopwatchStopISR_Stop(void);

CY_ISR_PROTO(StopwatchStopISR_Interrupt);

void StopwatchStopISR_SetVector(cyisraddress address);
cyisraddress StopwatchStopISR_GetVector(void);

void StopwatchStopISR_SetPriority(uint8 priority);
uint8 StopwatchStopISR_GetPriority(void);

void StopwatchStopISR_Enable(void);
uint8 StopwatchStopISR_GetState(void);
void StopwatchStopISR_Disable(void);

void StopwatchStopISR_SetPending(void);
void StopwatchStopISR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the StopwatchStopISR ISR. */
#define StopwatchStopISR_INTC_VECTOR            ((reg32 *) StopwatchStopISR__INTC_VECT)

/* Address of the StopwatchStopISR ISR priority. */
#define StopwatchStopISR_INTC_PRIOR             ((reg8 *) StopwatchStopISR__INTC_PRIOR_REG)

/* Priority of the StopwatchStopISR interrupt. */
#define StopwatchStopISR_INTC_PRIOR_NUMBER      StopwatchStopISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable StopwatchStopISR interrupt. */
#define StopwatchStopISR_INTC_SET_EN            ((reg32 *) StopwatchStopISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the StopwatchStopISR interrupt. */
#define StopwatchStopISR_INTC_CLR_EN            ((reg32 *) StopwatchStopISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the StopwatchStopISR interrupt state to pending. */
#define StopwatchStopISR_INTC_SET_PD            ((reg32 *) StopwatchStopISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the StopwatchStopISR interrupt. */
#define StopwatchStopISR_INTC_CLR_PD            ((reg32 *) StopwatchStopISR__INTC_CLR_PD_REG)


#endif /* CY_ISR_StopwatchStopISR_H */


/* [] END OF FILE */
