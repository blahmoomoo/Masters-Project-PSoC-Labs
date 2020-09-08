/*******************************************************************************
* File Name: StopwatchStopISR.c  
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
#include <StopwatchStopISR.h>

#if !defined(StopwatchStopISR__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START StopwatchStopISR_intc` */
	
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
* Function Name: StopwatchStopISR_Start
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
void StopwatchStopISR_Start(void)
{
    /* For all we know the interrupt is active. */
    StopwatchStopISR_Disable();

    /* Set the ISR to point to the StopwatchStopISR Interrupt. */
    StopwatchStopISR_SetVector(&StopwatchStopISR_Interrupt);

    /* Set the priority. */
    StopwatchStopISR_SetPriority((uint8)StopwatchStopISR_INTC_PRIOR_NUMBER);

    /* Enable it. */
    StopwatchStopISR_Enable();
}


/*******************************************************************************
* Function Name: StopwatchStopISR_StartEx
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
void StopwatchStopISR_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    StopwatchStopISR_Disable();

    /* Set the ISR to point to the StopwatchStopISR Interrupt. */
    StopwatchStopISR_SetVector(address);

    /* Set the priority. */
    StopwatchStopISR_SetPriority((uint8)StopwatchStopISR_INTC_PRIOR_NUMBER);

    /* Enable it. */
    StopwatchStopISR_Enable();
}


/*******************************************************************************
* Function Name: StopwatchStopISR_Stop
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
void StopwatchStopISR_Stop(void)
{
    /* Disable this interrupt. */
    StopwatchStopISR_Disable();

    /* Set the ISR to point to the passive one. */
    StopwatchStopISR_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: StopwatchStopISR_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for StopwatchStopISR.
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
CY_ISR(StopwatchStopISR_Interrupt)
{
    /*  Place your Interrupt code here. */
    /* `#START StopwatchStopISR_Interrupt` */
	uint32 capture;
	float seconds;
	char tstr[16];
	
	//If the stopwatch has started, set the flag to false, print the captured
	//  value of the timer, reset the timer, and print "Stopped"
	if(started_b)
	{
		started_b = FALSE;
		
		capture = Stopwatch_ReadCapture();
		capture = Stopwatch_ReadPeriod() - capture;
		seconds = capture / (float)STOPWATCH_FREQ;
		Display_Position(1, 0);
		sprintf(tstr, "%1.4f", seconds);
		Display_PrintString(tstr);
		
		StopwatchReset_Write(1);
		Display_Position(0, 0);
		Display_PrintString("Stopped");
	}
	
    /* `#END` */
}


/*******************************************************************************
* Function Name: StopwatchStopISR_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling StopwatchStopISR_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use StopwatchStopISR_StartEx instead.
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void StopwatchStopISR_SetVector(cyisraddress address)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + (uint32)StopwatchStopISR__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: StopwatchStopISR_GetVector
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
cyisraddress StopwatchStopISR_GetVector(void)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + (uint32)StopwatchStopISR__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: StopwatchStopISR_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. Note calling StopwatchStopISR_Start
*   or StopwatchStopISR_StartEx will override any effect this method 
*   would have had. This method should only be called after 
*   StopwatchStopISR_Start or StopwatchStopISR_StartEx has been called. To set 
*   the initial priority for the component use the cydwr file in the tool.
*
* Parameters:
*   priority: Priority of the interrupt. 0 - 7, 0 being the highest.
*
* Return:
*   None
*
*******************************************************************************/
void StopwatchStopISR_SetPriority(uint8 priority)
{
    *StopwatchStopISR_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: StopwatchStopISR_GetPriority
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
uint8 StopwatchStopISR_GetPriority(void)
{
    uint8 priority;


    priority = *StopwatchStopISR_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: StopwatchStopISR_Enable
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
void StopwatchStopISR_Enable(void)
{
    /* Enable the general interrupt. */
    *StopwatchStopISR_INTC_SET_EN = StopwatchStopISR__INTC_MASK;
}


/*******************************************************************************
* Function Name: StopwatchStopISR_GetState
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
uint8 StopwatchStopISR_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*StopwatchStopISR_INTC_SET_EN & (uint32)StopwatchStopISR__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: StopwatchStopISR_Disable
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
void StopwatchStopISR_Disable(void)
{
    /* Disable the general interrupt. */
    *StopwatchStopISR_INTC_CLR_EN = StopwatchStopISR__INTC_MASK;
}


/*******************************************************************************
* Function Name: StopwatchStopISR_SetPending
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
void StopwatchStopISR_SetPending(void)
{
    *StopwatchStopISR_INTC_SET_PD = StopwatchStopISR__INTC_MASK;
}


/*******************************************************************************
* Function Name: StopwatchStopISR_ClearPending
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
void StopwatchStopISR_ClearPending(void)
{
    *StopwatchStopISR_INTC_CLR_PD = StopwatchStopISR__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
