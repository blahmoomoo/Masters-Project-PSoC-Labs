/*******************************************************************************
* File Name: DisplayRefreshISR.h
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
#if !defined(CY_ISR_DisplayRefreshISR_H)
#define CY_ISR_DisplayRefreshISR_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void DisplayRefreshISR_Start(void);
void DisplayRefreshISR_StartEx(cyisraddress address);
void DisplayRefreshISR_Stop(void);

CY_ISR_PROTO(DisplayRefreshISR_Interrupt);

void DisplayRefreshISR_SetVector(cyisraddress address);
cyisraddress DisplayRefreshISR_GetVector(void);

void DisplayRefreshISR_SetPriority(uint8 priority);
uint8 DisplayRefreshISR_GetPriority(void);

void DisplayRefreshISR_Enable(void);
uint8 DisplayRefreshISR_GetState(void);
void DisplayRefreshISR_Disable(void);

void DisplayRefreshISR_SetPending(void);
void DisplayRefreshISR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the DisplayRefreshISR ISR. */
#define DisplayRefreshISR_INTC_VECTOR            ((reg32 *) DisplayRefreshISR__INTC_VECT)

/* Address of the DisplayRefreshISR ISR priority. */
#define DisplayRefreshISR_INTC_PRIOR             ((reg8 *) DisplayRefreshISR__INTC_PRIOR_REG)

/* Priority of the DisplayRefreshISR interrupt. */
#define DisplayRefreshISR_INTC_PRIOR_NUMBER      DisplayRefreshISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable DisplayRefreshISR interrupt. */
#define DisplayRefreshISR_INTC_SET_EN            ((reg32 *) DisplayRefreshISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the DisplayRefreshISR interrupt. */
#define DisplayRefreshISR_INTC_CLR_EN            ((reg32 *) DisplayRefreshISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the DisplayRefreshISR interrupt state to pending. */
#define DisplayRefreshISR_INTC_SET_PD            ((reg32 *) DisplayRefreshISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the DisplayRefreshISR interrupt. */
#define DisplayRefreshISR_INTC_CLR_PD            ((reg32 *) DisplayRefreshISR__INTC_CLR_PD_REG)


#endif /* CY_ISR_DisplayRefreshISR_H */


/* [] END OF FILE */
