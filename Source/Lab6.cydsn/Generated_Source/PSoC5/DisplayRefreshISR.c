/*******************************************************************************
* File Name: DisplayRefreshISR.c  
* Version 1.70
*
*  Description:
*   API for controlling the state of an interrupt.
*
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#include <cydevice_trm.h>
#include <CyLib.h>
#include <DisplayRefreshISR.h>

#if !defined(DisplayRefreshISR__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START DisplayRefreshISR_intc` */

#include <globals.h>
	
/* `#END` */

#ifndef CYINT_IRQ_BASE
#define CYINT_IRQ_BASE      16
#endif /* CYINT_IRQ_BASE */
#ifndef CYINT_VECT_TABLE
#define CYINT_VECT_TABLE    ((cyisraddress **) CYREG_NVIC_VECT_OFFSET)
#endif /* CYINT_VECT_TABLE */

/* Declared in startup, used to set unused interrupts to. */
CY_ISR_PROTO(IntDefaultHandler);


/*******************************************************************************
* Function Name: DisplayRefreshISR_Start
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void DisplayRefreshISR_Start(void)
{
    /* For all we know the interrupt is active. */
    DisplayRefreshISR_Disable();

    /* Set the ISR to point to the DisplayRefreshISR Interrupt. */
    DisplayRefreshISR_SetVector(&DisplayRefreshISR_Interrupt);

    /* Set the priority. */
    DisplayRefreshISR_SetPriority((uint8)DisplayRefreshISR_INTC_PRIOR_NUMBER);

    /* Enable it. */
    DisplayRefreshISR_Enable();
}


/*******************************************************************************
* Function Name: DisplayRefreshISR_StartEx
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it.
*
* Parameters:  
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void DisplayRefreshISR_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    DisplayRefreshISR_Disable();

    /* Set the ISR to point to the DisplayRefreshISR Interrupt. */
    DisplayRefreshISR_SetVector(address);

    /* Set the priority. */
    DisplayRefreshISR_SetPriority((uint8)DisplayRefreshISR_INTC_PRIOR_NUMBER);

    /* Enable it. */
    DisplayRefreshISR_Enable();
}


/*******************************************************************************
* Function Name: DisplayRefreshISR_Stop
********************************************************************************
*
* Summary:
*   Disables and removes the interrupt.
*
* Parameters:  
*
* Return:
*   None
*
*******************************************************************************/
void DisplayRefreshISR_Stop(void)
{
    /* Disable this interrupt. */
    DisplayRefreshISR_Disable();

    /* Set the ISR to point to the passive one. */
    DisplayRefreshISR_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: DisplayRefreshISR_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for DisplayRefreshISR.
*
*   Add custom code between the coments to keep the next version of this file
*   from over writting your code.
*
* Parameters:  
*
* Return:
*   None
*
*******************************************************************************/
CY_ISR(DisplayRefreshISR_Interrupt)
{
    /*  Place your Interrupt code here. */
    /* `#START DisplayRefreshISR_Interrupt` */
	uint32 capture;
	float seconds;
	char tstr[16];
	
	//If the stopwatch has started, print the current counter of the timer
	//  to the display
	if(started_b)
	{
		capture = Stopwatch_ReadCounter();
		capture = Stopwatch_ReadPeriod() - capture;
		seconds = capture / (float)STOPWATCH_FREQ;
		Display_Position(1, 0);
		sprintf(tstr, "%1.4f", seconds);
		Display_PrintString(tstr);
	}
	
    /* `#END` */
}


/*******************************************************************************
* Function Name: DisplayRefreshISR_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling DisplayRefreshISR_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use DisplayRefreshISR_StartEx instead.
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void DisplayRefreshISR_SetVector(cyisraddress address)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + (uint32)DisplayRefreshISR__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: DisplayRefreshISR_GetVector
********************************************************************************
*
* Summary:
*   Gets the "address" of the current ISR vector for the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress DisplayRefreshISR_GetVector(void)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + (uint32)DisplayRefreshISR__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: DisplayRefreshISR_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. Note calling DisplayRefreshISR_Start
*   or DisplayRefreshISR_StartEx will override any effect this method 
*   would have had. This method should only be called after 
*   DisplayRefreshISR_Start or DisplayRefreshISR_StartEx has been called. To set 
*   the initial priority for the component use the cydwr file in the tool.
*
* Parameters:
*   priority: Priority of the interrupt. 0 - 7, 0 being the highest.
*
* Return:
*   None
*
*******************************************************************************/
void DisplayRefreshISR_SetPriority(uint8 priority)
{
    *DisplayRefreshISR_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: DisplayRefreshISR_GetPriority
********************************************************************************
*
* Summary:
*   Gets the Priority of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Priority of the interrupt. 0 - 7, 0 being the highest.
*
*******************************************************************************/
uint8 DisplayRefreshISR_GetPriority(void)
{
    uint8 priority;


    priority = *DisplayRefreshISR_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: DisplayRefreshISR_Enable
********************************************************************************
*
* Summary:
*   Enables the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void DisplayRefreshISR_Enable(void)
{
    /* Enable the general interrupt. */
    *DisplayRefreshISR_INTC_SET_EN = DisplayRefreshISR__INTC_MASK;
}


/*******************************************************************************
* Function Name: DisplayRefreshISR_GetState
********************************************************************************
*
* Summary:
*   Gets the state (enabled, disabled) of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   1 if enabled, 0 if disabled.
*
*******************************************************************************/
uint8 DisplayRefreshISR_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*DisplayRefreshISR_INTC_SET_EN & (uint32)DisplayRefreshISR__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: DisplayRefreshISR_Disable
********************************************************************************
*
* Summary:
*   Disables the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void DisplayRefreshISR_Disable(void)
{
    /* Disable the general interrupt. */
    *DisplayRefreshISR_INTC_CLR_EN = DisplayRefreshISR__INTC_MASK;
}


/*******************************************************************************
* Function Name: DisplayRefreshISR_SetPending
********************************************************************************
*
* Summary:
*   Causes the Interrupt to enter the pending state, a software method of
*   generating the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void DisplayRefreshISR_SetPending(void)
{
    *DisplayRefreshISR_INTC_SET_PD = DisplayRefreshISR__INTC_MASK;
}


/*******************************************************************************
* Function Name: DisplayRefreshISR_ClearPending
********************************************************************************
*
* Summary:
*   Clears a pending interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void DisplayRefreshISR_ClearPending(void)
{
    *DisplayRefreshISR_INTC_CLR_PD = DisplayRefreshISR__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
