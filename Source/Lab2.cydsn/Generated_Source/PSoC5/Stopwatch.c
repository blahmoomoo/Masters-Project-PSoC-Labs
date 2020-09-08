/*******************************************************************************
* File Name: Stopwatch.c
* Version 2.50
*
* Description:
*  The Timer component consists of a 8, 16, 24 or 32-bit timer with
*  a selectable period between 2 and 2^Width - 1.  The timer may free run
*  or be used as a capture timer as well.  The capture can be initiated
*  by a positive or negative edge signal as well as via software.
*  A trigger input can be programmed to enable the timer on rising edge
*  falling edge, either edge or continous run.
*  Interrupts may be generated due to a terminal count condition
*  or a capture event.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Stopwatch.h"

uint8 Stopwatch_initVar = 0u;


/*******************************************************************************
* Function Name: Stopwatch_Init
********************************************************************************
*
* Summary:
*  Initialize to the schematic state
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Stopwatch_Init(void) 
{
    #if(!Stopwatch_UsingFixedFunction)
            /* Interrupt State Backup for Critical Region*/
            uint8 Stopwatch_interruptState;
    #endif /* Interrupt state back up for Fixed Function only */

    #if (Stopwatch_UsingFixedFunction)
        /* Clear all bits but the enable bit (if it's already set) for Timer operation */
        Stopwatch_CONTROL &= Stopwatch_CTRL_ENABLE;

        /* Clear the mode bits for continuous run mode */
        #if (CY_PSOC5A)
            Stopwatch_CONTROL2 &= ((uint8)(~Stopwatch_CTRL_MODE_MASK));
        #endif /* Clear bits in CONTROL2 only in PSOC5A */

        #if (CY_PSOC3 || CY_PSOC5LP)
            Stopwatch_CONTROL3 &= ((uint8)(~Stopwatch_CTRL_MODE_MASK));
        #endif /* CONTROL3 register exists only in PSoC3 OR PSoC5LP */

        /* Check if One Shot mode is enabled i.e. RunMode !=0*/
        #if (Stopwatch_RunModeUsed != 0x0u)
            /* Set 3rd bit of Control register to enable one shot mode */
            Stopwatch_CONTROL |= 0x04u;
        #endif /* One Shot enabled only when RunModeUsed is not Continuous*/

        #if (Stopwatch_RunModeUsed == 2)
            #if (CY_PSOC5A)
                /* Set last 2 bits of control2 register if one shot(halt on
                interrupt) is enabled*/
                Stopwatch_CONTROL2 |= 0x03u;
            #endif /* Set One-Shot Halt on Interrupt bit in CONTROL2 for PSoC5A */

            #if (CY_PSOC3 || CY_PSOC5LP)
                /* Set last 2 bits of control3 register if one shot(halt on
                interrupt) is enabled*/
                Stopwatch_CONTROL3 |= 0x03u;
            #endif /* Set One-Shot Halt on Interrupt bit in CONTROL3 for PSoC3 or PSoC5LP */

        #endif /* Remove section if One Shot Halt on Interrupt is not enabled */

        #if (Stopwatch_UsingHWEnable != 0)
            #if (CY_PSOC5A)
                /* Set the default Run Mode of the Timer to Continuous */
                Stopwatch_CONTROL2 |= Stopwatch_CTRL_MODE_PULSEWIDTH;
            #endif /* Set Continuous Run Mode in CONTROL2 for PSoC5A */

            #if (CY_PSOC3 || CY_PSOC5LP)
                /* Clear and Set ROD and COD bits of CFG2 register */
                Stopwatch_CONTROL3 &= ((uint8)(~Stopwatch_CTRL_RCOD_MASK));
                Stopwatch_CONTROL3 |= Stopwatch_CTRL_RCOD;

                /* Clear and Enable the HW enable bit in CFG2 register */
                Stopwatch_CONTROL3 &= ((uint8)(~Stopwatch_CTRL_ENBL_MASK));
                Stopwatch_CONTROL3 |= Stopwatch_CTRL_ENBL;

                /* Set the default Run Mode of the Timer to Continuous */
                Stopwatch_CONTROL3 |= Stopwatch_CTRL_MODE_CONTINUOUS;
            #endif /* Set Continuous Run Mode in CONTROL3 for PSoC3ES3 or PSoC5A */

        #endif /* Configure Run Mode with hardware enable */

        /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        Stopwatch_RT1 &= ((uint8)(~Stopwatch_RT1_MASK));
        Stopwatch_RT1 |= Stopwatch_SYNC;

        /*Enable DSI Sync all all inputs of the Timer*/
        Stopwatch_RT1 &= ((uint8)(~Stopwatch_SYNCDSI_MASK));
        Stopwatch_RT1 |= Stopwatch_SYNCDSI_EN;

        /* Set the IRQ to use the status register interrupts */
        Stopwatch_CONTROL2 |= Stopwatch_CTRL2_IRQ_SEL;
    #endif /* Configuring registers of fixed function implementation */

    /* Set Initial values from Configuration */
    Stopwatch_WritePeriod(Stopwatch_INIT_PERIOD);
    Stopwatch_WriteCounter(Stopwatch_INIT_PERIOD);

    #if (Stopwatch_UsingHWCaptureCounter)/* Capture counter is enabled */
        Stopwatch_CAPTURE_COUNT_CTRL |= Stopwatch_CNTR_ENABLE;
        Stopwatch_SetCaptureCount(Stopwatch_INIT_CAPTURE_COUNT);
    #endif /* Configure capture counter value */

    #if (!Stopwatch_UsingFixedFunction)
        #if (Stopwatch_SoftwareCaptureMode)
            Stopwatch_SetCaptureMode(Stopwatch_INIT_CAPTURE_MODE);
        #endif /* Set Capture Mode for UDB implementation if capture mode is software controlled */

        #if (Stopwatch_SoftwareTriggerMode)
            if (0u == (Stopwatch_CONTROL & Stopwatch__B_TIMER__TM_SOFTWARE))
            {
                Stopwatch_SetTriggerMode(Stopwatch_INIT_TRIGGER_MODE);
            }
        #endif /* Set trigger mode for UDB Implementation if trigger mode is software controlled */

        /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
        /* Enter Critical Region*/
        Stopwatch_interruptState = CyEnterCriticalSection();

        /* Use the interrupt output of the status register for IRQ output */
        Stopwatch_STATUS_AUX_CTRL |= Stopwatch_STATUS_ACTL_INT_EN_MASK;

        /* Exit Critical Region*/
        CyExitCriticalSection(Stopwatch_interruptState);

        #if (Stopwatch_EnableTriggerMode)
            Stopwatch_EnableTrigger();
        #endif /* Set Trigger enable bit for UDB implementation in the control register*/

        #if (Stopwatch_InterruptOnCaptureCount)
             #if (!Stopwatch_ControlRegRemoved)
                Stopwatch_SetInterruptCount(Stopwatch_INIT_INT_CAPTURE_COUNT);
            #endif /* Set interrupt count in control register if control register is not removed */
        #endif /*Set interrupt count in UDB implementation if interrupt count feature is checked.*/

        Stopwatch_ClearFIFO();
    #endif /* Configure additional features of UDB implementation */

    Stopwatch_SetInterruptMode(Stopwatch_INIT_INTERRUPT_MODE);
}


/*******************************************************************************
* Function Name: Stopwatch_Enable
********************************************************************************
*
* Summary:
*  Enable the Timer
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Stopwatch_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (Stopwatch_UsingFixedFunction)
        Stopwatch_GLOBAL_ENABLE |= Stopwatch_BLOCK_EN_MASK;
        Stopwatch_GLOBAL_STBY_ENABLE |= Stopwatch_BLOCK_STBY_EN_MASK;
    #endif /* Set Enable bit for enabling Fixed function timer*/

    /* Remove assignment if control register is removed */
    #if (!Stopwatch_ControlRegRemoved || Stopwatch_UsingFixedFunction)
        Stopwatch_CONTROL |= Stopwatch_CTRL_ENABLE;
    #endif /* Remove assignment if control register is removed */
}


/*******************************************************************************
* Function Name: Stopwatch_Start
********************************************************************************
*
* Summary:
*  The start function initializes the timer with the default values, the
*  enables the timerto begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Stopwatch_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void Stopwatch_Start(void) 
{
    if(Stopwatch_initVar == 0u)
    {
        Stopwatch_Init();

        Stopwatch_initVar = 1u;   /* Clear this bit for Initialization */
    }

    /* Enable the Timer */
    Stopwatch_Enable();
}


/*******************************************************************************
* Function Name: Stopwatch_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the timer, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the timer.
*
*******************************************************************************/
void Stopwatch_Stop(void) 
{
    /* Disable Timer */
    #if(!Stopwatch_ControlRegRemoved || Stopwatch_UsingFixedFunction)
        Stopwatch_CONTROL &= ((uint8)(~Stopwatch_CTRL_ENABLE));
    #endif /* Remove assignment if control register is removed */

    /* Globally disable the Fixed Function Block chosen */
    #if (Stopwatch_UsingFixedFunction)
        Stopwatch_GLOBAL_ENABLE &= ((uint8)(~Stopwatch_BLOCK_EN_MASK));
        Stopwatch_GLOBAL_STBY_ENABLE &= ((uint8)(~Stopwatch_BLOCK_STBY_EN_MASK));
    #endif /* Disable global enable for the Timer Fixed function block to stop the Timer*/
}


/*******************************************************************************
* Function Name: Stopwatch_SetInterruptMode
********************************************************************************
*
* Summary:
*  This function selects which of the interrupt inputs may cause an interrupt.
*  The twosources are caputure and terminal.  One, both or neither may
*  be selected.
*
* Parameters:
*  interruptMode:   This parameter is used to enable interrups on either/or
*                   terminal count or capture.
*
* Return:
*  void
*
*******************************************************************************/
void Stopwatch_SetInterruptMode(uint8 interruptMode) 
{
    Stopwatch_STATUS_MASK = interruptMode;
}


/*******************************************************************************
* Function Name: Stopwatch_SoftwareCapture
********************************************************************************
*
* Summary:
*  This function forces a capture independent of the capture signal.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Side Effects:
*  An existing hardware capture could be overwritten.
*
*******************************************************************************/
void Stopwatch_SoftwareCapture(void) 
{
    /* Generate a software capture by reading the counter register */
    (void)Stopwatch_COUNTER_LSB;
    /* Capture Data is now in the FIFO */
}


/*******************************************************************************
* Function Name: Stopwatch_ReadStatusRegister
********************************************************************************
*
* Summary:
*  Reads the status register and returns it's state. This function should use
*  defined types for the bit-field information as the bits in this register may
*  be permuteable.
*
* Parameters:
*  void
*
* Return:
*  The contents of the status register
*
* Side Effects:
*  Status register bits may be clear on read.
*
*******************************************************************************/
uint8   Stopwatch_ReadStatusRegister(void) 
{
    return (Stopwatch_STATUS);
}


#if (!Stopwatch_ControlRegRemoved) /* Remove API if control register is removed */


/*******************************************************************************
* Function Name: Stopwatch_ReadControlRegister
********************************************************************************
*
* Summary:
*  Reads the control register and returns it's value.
*
* Parameters:
*  void
*
* Return:
*  The contents of the control register
*
*******************************************************************************/
uint8 Stopwatch_ReadControlRegister(void) 
{
    return ((uint8)Stopwatch_CONTROL);
}


/*******************************************************************************
* Function Name: Stopwatch_WriteControlRegister
********************************************************************************
*
* Summary:
*  Sets the bit-field of the control register.
*
* Parameters:
*  control: The contents of the control register
*
* Return:
*
*******************************************************************************/
void Stopwatch_WriteControlRegister(uint8 control) 
{
    Stopwatch_CONTROL = control;
}
#endif /* Remove API if control register is removed */


/*******************************************************************************
* Function Name: Stopwatch_ReadPeriod
********************************************************************************
*
* Summary:
*  This function returns the current value of the Period.
*
* Parameters:
*  void
*
* Return:
*  The present value of the counter.
*
*******************************************************************************/
uint32 Stopwatch_ReadPeriod(void) 
{
   #if(Stopwatch_UsingFixedFunction)
       return ((uint32)CY_GET_REG16(Stopwatch_PERIOD_LSB_PTR));
   #else
       return (CY_GET_REG32(Stopwatch_PERIOD_LSB_PTR));
   #endif /* (Stopwatch_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Stopwatch_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period: This value may be between 1 and (2^Resolution)-1.  A value of 0 will
*          result in the counter remaining at zero.
*
* Return:
*  void
*
*******************************************************************************/
void Stopwatch_WritePeriod(uint32 period) 
{
    #if(Stopwatch_UsingFixedFunction)
        uint16 period_temp = (uint16)period;
        CY_SET_REG16(Stopwatch_PERIOD_LSB_PTR, period_temp);
    #else
        CY_SET_REG32(Stopwatch_PERIOD_LSB_PTR, period);
    #endif /*Write Period value with appropriate resolution suffix depending on UDB or fixed function implementation */
}


/*******************************************************************************
* Function Name: Stopwatch_ReadCapture
********************************************************************************
*
* Summary:
*  This function returns the last value captured.
*
* Parameters:
*  void
*
* Return:
*  Present Capture value.
*
*******************************************************************************/
uint32 Stopwatch_ReadCapture(void) 
{
   #if(Stopwatch_UsingFixedFunction)
       return ((uint32)CY_GET_REG16(Stopwatch_CAPTURE_LSB_PTR));
   #else
       return (CY_GET_REG32(Stopwatch_CAPTURE_LSB_PTR));
   #endif /* (Stopwatch_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Stopwatch_WriteCounter
********************************************************************************
*
* Summary:
*  This funtion is used to set the counter to a specific value
*
* Parameters:
*  counter:  New counter value.
*
* Return:
*  void
*
*******************************************************************************/
void Stopwatch_WriteCounter(uint32 counter) \
                                   
{
   #if(Stopwatch_UsingFixedFunction)
        /* This functionality is removed until a FixedFunction HW update to
         * allow this register to be written
         */
        CY_SET_REG16(Stopwatch_COUNTER_LSB_PTR, (uint16)counter);
        
    #else
        CY_SET_REG32(Stopwatch_COUNTER_LSB_PTR, counter);
    #endif /* Set Write Counter only for the UDB implementation (Write Counter not available in fixed function Timer */
}


/*******************************************************************************
* Function Name: Stopwatch_ReadCounter
********************************************************************************
*
* Summary:
*  This function returns the current counter value.
*
* Parameters:
*  void
*
* Return:
*  Present compare value.
*
*******************************************************************************/
uint32 Stopwatch_ReadCounter(void) 
{

    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    (void)Stopwatch_COUNTER_LSB;

    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(Stopwatch_UsingFixedFunction)
        return ((uint32)CY_GET_REG16(Stopwatch_CAPTURE_LSB_PTR));
    #else
        return (CY_GET_REG32(Stopwatch_CAPTURE_LSB_PTR));
    #endif /* (Stopwatch_UsingFixedFunction) */
}


#if(!Stopwatch_UsingFixedFunction) /* UDB Specific Functions */

/*******************************************************************************
 * The functions below this point are only available using the UDB
 * implementation.  If a feature is selected, then the API is enabled.
 ******************************************************************************/


#if (Stopwatch_SoftwareCaptureMode)


/*******************************************************************************
* Function Name: Stopwatch_SetCaptureMode
********************************************************************************
*
* Summary:
*  This function sets the capture mode to either rising or falling edge.
*
* Parameters:
*  captureMode: This parameter sets the capture mode of the UDB capture feature
*  The parameter values are defined using the
*  #define Stopwatch__B_TIMER__CM_NONE 0
#define Stopwatch__B_TIMER__CM_RISINGEDGE 1
#define Stopwatch__B_TIMER__CM_FALLINGEDGE 2
#define Stopwatch__B_TIMER__CM_EITHEREDGE 3
#define Stopwatch__B_TIMER__CM_SOFTWARE 4
 identifiers
*  The following are the possible values of the parameter
*  Stopwatch__B_TIMER__CM_NONE        - Set Capture mode to None
*  Stopwatch__B_TIMER__CM_RISINGEDGE  - Rising edge of Capture input
*  Stopwatch__B_TIMER__CM_FALLINGEDGE - Falling edge of Capture input
*  Stopwatch__B_TIMER__CM_EITHEREDGE  - Either edge of Capture input
*
* Return:
*  void
*
*******************************************************************************/
void Stopwatch_SetCaptureMode(uint8 captureMode) 
{
    /* This must only set to two bits of the control register associated */
    captureMode = ((uint8)((uint8)captureMode << Stopwatch_CTRL_CAP_MODE_SHIFT));
    captureMode &= (Stopwatch_CTRL_CAP_MODE_MASK);

    /* Clear the Current Setting */
    Stopwatch_CONTROL &= ((uint8)(~Stopwatch_CTRL_CAP_MODE_MASK));

    /* Write The New Setting */
    Stopwatch_CONTROL |= captureMode;
}
#endif /* Remove API if Capture Mode is not Software Controlled */


#if (Stopwatch_SoftwareTriggerMode)


/*******************************************************************************
* Function Name: Stopwatch_SetTriggerMode
********************************************************************************
*
* Summary:
*  This function sets the trigger input mode
*
* Parameters:
*  triggerMode: Pass one of the pre-defined Trigger Modes (except Software)
    #define Stopwatch__B_TIMER__TM_NONE 0x00u
    #define Stopwatch__B_TIMER__TM_RISINGEDGE 0x04u
    #define Stopwatch__B_TIMER__TM_FALLINGEDGE 0x08u
    #define Stopwatch__B_TIMER__TM_EITHEREDGE 0x0Cu
    #define Stopwatch__B_TIMER__TM_SOFTWARE 0x10u
*
* Return:
*  void
*
*******************************************************************************/
void Stopwatch_SetTriggerMode(uint8 triggerMode) 
{
    /* This must only set to two bits of the control register associated */
    triggerMode &= Stopwatch_CTRL_TRIG_MODE_MASK;

    /* Clear the Current Setting */
    Stopwatch_CONTROL &= ((uint8)(~Stopwatch_CTRL_TRIG_MODE_MASK));

    /* Write The New Setting */
    Stopwatch_CONTROL |= (triggerMode | Stopwatch__B_TIMER__TM_SOFTWARE);

}
#endif /* Remove API if Trigger Mode is not Software Controlled */

#if (Stopwatch_EnableTriggerMode)


/*******************************************************************************
* Function Name: Stopwatch_EnableTrigger
********************************************************************************
*
* Summary:
*  Sets the control bit enabling Hardware Trigger mode
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Stopwatch_EnableTrigger(void) 
{
    #if (!Stopwatch_ControlRegRemoved)   /* Remove assignment if control register is removed */
        Stopwatch_CONTROL |= Stopwatch_CTRL_TRIG_EN;
    #endif /* Remove code section if control register is not used */
}


/*******************************************************************************
* Function Name: Stopwatch_DisableTrigger
********************************************************************************
*
* Summary:
*  Clears the control bit enabling Hardware Trigger mode
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Stopwatch_DisableTrigger(void) 
{
    #if (!Stopwatch_ControlRegRemoved)   /* Remove assignment if control register is removed */
        Stopwatch_CONTROL &= ((uint8)(~Stopwatch_CTRL_TRIG_EN));
    #endif /* Remove code section if control register is not used */
}
#endif /* Remove API is Trigger Mode is set to None */


#if(Stopwatch_InterruptOnCaptureCount)
#if (!Stopwatch_ControlRegRemoved)   /* Remove API if control register is removed */


/*******************************************************************************
* Function Name: Stopwatch_SetInterruptCount
********************************************************************************
*
* Summary:
*  This function sets the capture count before an interrupt is triggered.
*
* Parameters:
*  interruptCount:  A value between 0 and 3 is valid.  If the value is 0, then
*                   an interrupt will occur each time a capture occurs.
*                   A value of 1 to 3 will cause the interrupt
*                   to delay by the same number of captures.
*
* Return:
*  void
*
*******************************************************************************/
void Stopwatch_SetInterruptCount(uint8 interruptCount) 
{
    /* This must only set to two bits of the control register associated */
    interruptCount &= Stopwatch_CTRL_INTCNT_MASK;

    /* Clear the Current Setting */
    Stopwatch_CONTROL &= ((uint8)(~Stopwatch_CTRL_INTCNT_MASK));
    /* Write The New Setting */
    Stopwatch_CONTROL |= interruptCount;
}
#endif /* Remove API if control register is removed */
#endif /* Stopwatch_InterruptOnCaptureCount */


#if (Stopwatch_UsingHWCaptureCounter)


/*******************************************************************************
* Function Name: Stopwatch_SetCaptureCount
********************************************************************************
*
* Summary:
*  This function sets the capture count
*
* Parameters:
*  captureCount: A value between 2 and 127 inclusive is valid.  A value of 1
*                to 127 will cause the interrupt to delay by the same number of
*                captures.
*
* Return:
*  void
*
*******************************************************************************/
void Stopwatch_SetCaptureCount(uint8 captureCount) 
{
    Stopwatch_CAP_COUNT = captureCount;
}


/*******************************************************************************
* Function Name: Stopwatch_ReadCaptureCount
********************************************************************************
*
* Summary:
*  This function reads the capture count setting
*
* Parameters:
*  void
*
* Return:
*  Returns the Capture Count Setting
*
*******************************************************************************/
uint8 Stopwatch_ReadCaptureCount(void) 
{
    return ((uint8)Stopwatch_CAP_COUNT);
}
#endif /* Stopwatch_UsingHWCaptureCounter */


/*******************************************************************************
* Function Name: Stopwatch_ClearFIFO
********************************************************************************
*
* Summary:
*  This function clears all capture data from the capture FIFO
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Stopwatch_ClearFIFO(void) 
{
    while(0u != (Stopwatch_ReadStatusRegister() & Stopwatch_STATUS_FIFONEMP))
    {
        (void)Stopwatch_ReadCapture();
    }
}

#endif /* UDB Specific Functions */


/* [] END OF FILE */
