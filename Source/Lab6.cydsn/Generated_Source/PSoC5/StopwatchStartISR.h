/*******************************************************************************
* File Name: StopwatchStartISR.h
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
#if !defined(CY_ISR_StopwatchStartISR_H)
#define CY_ISR_StopwatchStartISR_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void StopwatchStartISR_Start(void);
void StopwatchStartISR_StartEx(cyisraddress address);
void StopwatchStartISR_Stop(void);

CY_ISR_PROTO(StopwatchStartISR_Interrupt);

void StopwatchStartISR_SetVector(cyisraddress address);
cyisraddress StopwatchStartISR_GetVector(void);

void StopwatchStartISR_SetPriority(uint8 priority);
uint8 StopwatchStartISR_GetPriority(void);

void StopwatchStartISR_Enable(void);
uint8 StopwatchStartISR_GetState(void);
void StopwatchStartISR_Disable(void);

void StopwatchStartISR_SetPending(void);
void StopwatchStartISR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the StopwatchStartISR ISR. */
#define StopwatchStartISR_INTC_VECTOR            ((reg32 *) StopwatchStartISR__INTC_VECT)

/* Address of the StopwatchStartISR ISR priority. */
#define StopwatchStartISR_INTC_PRIOR             ((reg8 *) StopwatchStartISR__INTC_PRIOR_REG)

/* Priority of the StopwatchStartISR interrupt. */
#define StopwatchStartISR_INTC_PRIOR_NUMBER      StopwatchStartISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable StopwatchStartISR interrupt. */
#define StopwatchStartISR_INTC_SET_EN            ((reg32 *) StopwatchStartISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the StopwatchStartISR interrupt. */
#define StopwatchStartISR_INTC_CLR_EN            ((reg32 *) StopwatchStartISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the StopwatchStartISR interrupt state to pending. */
#define StopwatchStartISR_INTC_SET_PD            ((reg32 *) StopwatchStartISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the StopwatchStartISR interrupt. */
#define StopwatchStartISR_INTC_CLR_PD            ((reg32 *) StopwatchStartISR__INTC_CLR_PD_REG)


#endif /* CY_ISR_StopwatchStartISR_H */


/* [] END OF FILE */
