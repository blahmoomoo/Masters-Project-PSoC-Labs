/*******************************************************************************
* File Name: SW2ISR.h
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
#if !defined(CY_ISR_SW2ISR_H)
#define CY_ISR_SW2ISR_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void SW2ISR_Start(void);
void SW2ISR_StartEx(cyisraddress address);
void SW2ISR_Stop(void);

CY_ISR_PROTO(SW2ISR_Interrupt);

void SW2ISR_SetVector(cyisraddress address);
cyisraddress SW2ISR_GetVector(void);

void SW2ISR_SetPriority(uint8 priority);
uint8 SW2ISR_GetPriority(void);

void SW2ISR_Enable(void);
uint8 SW2ISR_GetState(void);
void SW2ISR_Disable(void);

void SW2ISR_SetPending(void);
void SW2ISR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the SW2ISR ISR. */
#define SW2ISR_INTC_VECTOR            ((reg32 *) SW2ISR__INTC_VECT)

/* Address of the SW2ISR ISR priority. */
#define SW2ISR_INTC_PRIOR             ((reg8 *) SW2ISR__INTC_PRIOR_REG)

/* Priority of the SW2ISR interrupt. */
#define SW2ISR_INTC_PRIOR_NUMBER      SW2ISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable SW2ISR interrupt. */
#define SW2ISR_INTC_SET_EN            ((reg32 *) SW2ISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the SW2ISR interrupt. */
#define SW2ISR_INTC_CLR_EN            ((reg32 *) SW2ISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the SW2ISR interrupt state to pending. */
#define SW2ISR_INTC_SET_PD            ((reg32 *) SW2ISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the SW2ISR interrupt. */
#define SW2ISR_INTC_CLR_PD            ((reg32 *) SW2ISR__INTC_CLR_PD_REG)


#endif /* CY_ISR_SW2ISR_H */


/* [] END OF FILE */
