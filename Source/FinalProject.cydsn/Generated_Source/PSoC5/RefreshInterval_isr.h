/*******************************************************************************
* File Name: RefreshInterval_isr.h
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
#if !defined(CY_ISR_RefreshInterval_isr_H)
#define CY_ISR_RefreshInterval_isr_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void RefreshInterval_isr_Start(void);
void RefreshInterval_isr_StartEx(cyisraddress address);
void RefreshInterval_isr_Stop(void);

CY_ISR_PROTO(RefreshInterval_isr_Interrupt);

void RefreshInterval_isr_SetVector(cyisraddress address);
cyisraddress RefreshInterval_isr_GetVector(void);

void RefreshInterval_isr_SetPriority(uint8 priority);
uint8 RefreshInterval_isr_GetPriority(void);

void RefreshInterval_isr_Enable(void);
uint8 RefreshInterval_isr_GetState(void);
void RefreshInterval_isr_Disable(void);

void RefreshInterval_isr_SetPending(void);
void RefreshInterval_isr_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the RefreshInterval_isr ISR. */
#define RefreshInterval_isr_INTC_VECTOR            ((reg32 *) RefreshInterval_isr__INTC_VECT)

/* Address of the RefreshInterval_isr ISR priority. */
#define RefreshInterval_isr_INTC_PRIOR             ((reg8 *) RefreshInterval_isr__INTC_PRIOR_REG)

/* Priority of the RefreshInterval_isr interrupt. */
#define RefreshInterval_isr_INTC_PRIOR_NUMBER      RefreshInterval_isr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable RefreshInterval_isr interrupt. */
#define RefreshInterval_isr_INTC_SET_EN            ((reg32 *) RefreshInterval_isr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the RefreshInterval_isr interrupt. */
#define RefreshInterval_isr_INTC_CLR_EN            ((reg32 *) RefreshInterval_isr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the RefreshInterval_isr interrupt state to pending. */
#define RefreshInterval_isr_INTC_SET_PD            ((reg32 *) RefreshInterval_isr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the RefreshInterval_isr interrupt. */
#define RefreshInterval_isr_INTC_CLR_PD            ((reg32 *) RefreshInterval_isr__INTC_CLR_PD_REG)


#endif /* CY_ISR_RefreshInterval_isr_H */


/* [] END OF FILE */
