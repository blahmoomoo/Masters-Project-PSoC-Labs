/*******************************************************************************
* File Name: Envelope_counter.c  
* Version 2.40
*
*  Description:
*     The Counter component consists of a 8, 16, 24 or 32-bit counter with
*     a selectable period between 2 and 2^Width - 1.  
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "Envelope_counter.h"

uint8 Envelope_counter_initVar = 0u;


/*******************************************************************************
* Function Name: Envelope_counter_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void Envelope_counter_Init(void) 
{
        #if (!Envelope_counter_UsingFixedFunction && !Envelope_counter_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!Envelope_counter_UsingFixedFunction && !Envelope_counter_ControlRegRemoved) */
        
        #if(!Envelope_counter_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 Envelope_counter_interruptState;
        #endif /* (!Envelope_counter_UsingFixedFunction) */
        
        #if (Envelope_counter_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            Envelope_counter_CONTROL &= Envelope_counter_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                Envelope_counter_CONTROL2 &= ((uint8)(~Envelope_counter_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                Envelope_counter_CONTROL3 &= ((uint8)(~Envelope_counter_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (Envelope_counter_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                Envelope_counter_CONTROL |= Envelope_counter_ONESHOT;
            #endif /* (Envelope_counter_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            Envelope_counter_CONTROL2 |= Envelope_counter_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            Envelope_counter_RT1 &= ((uint8)(~Envelope_counter_RT1_MASK));
            Envelope_counter_RT1 |= Envelope_counter_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            Envelope_counter_RT1 &= ((uint8)(~Envelope_counter_SYNCDSI_MASK));
            Envelope_counter_RT1 |= Envelope_counter_SYNCDSI_EN;

        #else
            #if(!Envelope_counter_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = Envelope_counter_CONTROL & ((uint8)(~Envelope_counter_CTRL_CMPMODE_MASK));
            Envelope_counter_CONTROL = ctrl | Envelope_counter_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = Envelope_counter_CONTROL & ((uint8)(~Envelope_counter_CTRL_CAPMODE_MASK));
            
            #if( 0 != Envelope_counter_CAPTURE_MODE_CONF)
                Envelope_counter_CONTROL = ctrl | Envelope_counter_DEFAULT_CAPTURE_MODE;
            #else
                Envelope_counter_CONTROL = ctrl;
            #endif /* 0 != Envelope_counter_CAPTURE_MODE */ 
            
            #endif /* (!Envelope_counter_ControlRegRemoved) */
        #endif /* (Envelope_counter_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!Envelope_counter_UsingFixedFunction)
            Envelope_counter_ClearFIFO();
        #endif /* (!Envelope_counter_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        Envelope_counter_WritePeriod(Envelope_counter_INIT_PERIOD_VALUE);
        #if (!(Envelope_counter_UsingFixedFunction && (CY_PSOC5A)))
            Envelope_counter_WriteCounter(Envelope_counter_INIT_COUNTER_VALUE);
        #endif /* (!(Envelope_counter_UsingFixedFunction && (CY_PSOC5A))) */
        Envelope_counter_SetInterruptMode(Envelope_counter_INIT_INTERRUPTS_MASK);
        
        #if (!Envelope_counter_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)Envelope_counter_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            Envelope_counter_WriteCompare(Envelope_counter_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            Envelope_counter_interruptState = CyEnterCriticalSection();
            
            Envelope_counter_STATUS_AUX_CTRL |= Envelope_counter_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(Envelope_counter_interruptState);
            
        #endif /* (!Envelope_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Envelope_counter_Enable
********************************************************************************
* Summary:
*     Enable the Counter
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: 
*   If the Enable mode is set to Hardware only then this function has no effect 
*   on the operation of the counter.
*
*******************************************************************************/
void Envelope_counter_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (Envelope_counter_UsingFixedFunction)
        Envelope_counter_GLOBAL_ENABLE |= Envelope_counter_BLOCK_EN_MASK;
        Envelope_counter_GLOBAL_STBY_ENABLE |= Envelope_counter_BLOCK_STBY_EN_MASK;
    #endif /* (Envelope_counter_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!Envelope_counter_ControlRegRemoved || Envelope_counter_UsingFixedFunction)
        Envelope_counter_CONTROL |= Envelope_counter_CTRL_ENABLE;                
    #endif /* (!Envelope_counter_ControlRegRemoved || Envelope_counter_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: Envelope_counter_Start
********************************************************************************
* Summary:
*  Enables the counter for operation 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  Envelope_counter_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void Envelope_counter_Start(void) 
{
    if(Envelope_counter_initVar == 0u)
    {
        Envelope_counter_Init();
        
        Envelope_counter_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    Envelope_counter_Enable();        
}


/*******************************************************************************
* Function Name: Envelope_counter_Stop
********************************************************************************
* Summary:
* Halts the counter, but does not change any modes or disable interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the counter.
*
*******************************************************************************/
void Envelope_counter_Stop(void) 
{
    /* Disable Counter */
    #if(!Envelope_counter_ControlRegRemoved || Envelope_counter_UsingFixedFunction)
        Envelope_counter_CONTROL &= ((uint8)(~Envelope_counter_CTRL_ENABLE));        
    #endif /* (!Envelope_counter_ControlRegRemoved || Envelope_counter_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (Envelope_counter_UsingFixedFunction)
        Envelope_counter_GLOBAL_ENABLE &= ((uint8)(~Envelope_counter_BLOCK_EN_MASK));
        Envelope_counter_GLOBAL_STBY_ENABLE &= ((uint8)(~Envelope_counter_BLOCK_STBY_EN_MASK));
    #endif /* (Envelope_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Envelope_counter_SetInterruptMode
********************************************************************************
* Summary:
* Configures which interrupt sources are enabled to generate the final interrupt
*
* Parameters:  
*  InterruptsMask: This parameter is an or'd collection of the status bits
*                   which will be allowed to generate the counters interrupt.   
*
* Return: 
*  void
*
*******************************************************************************/
void Envelope_counter_SetInterruptMode(uint8 interruptsMask) 
{
    Envelope_counter_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: Envelope_counter_ReadStatusRegister
********************************************************************************
* Summary:
*   Reads the status register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the status register
*
* Side Effects:
*   Status register bits may be clear on read. 
*
*******************************************************************************/
uint8   Envelope_counter_ReadStatusRegister(void) 
{
    return Envelope_counter_STATUS;
}


#if(!Envelope_counter_ControlRegRemoved)
/*******************************************************************************
* Function Name: Envelope_counter_ReadControlRegister
********************************************************************************
* Summary:
*   Reads the control register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
uint8   Envelope_counter_ReadControlRegister(void) 
{
    return Envelope_counter_CONTROL;
}


/*******************************************************************************
* Function Name: Envelope_counter_WriteControlRegister
********************************************************************************
* Summary:
*   Sets the bit-field of the control register.  This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
void    Envelope_counter_WriteControlRegister(uint8 control) 
{
    Envelope_counter_CONTROL = control;
}

#endif  /* (!Envelope_counter_ControlRegRemoved) */


#if (!(Envelope_counter_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: Envelope_counter_WriteCounter
********************************************************************************
* Summary:
*   This funtion is used to set the counter to a specific value
*
* Parameters:  
*  counter:  New counter value. 
*
* Return: 
*  void 
*
*******************************************************************************/
void Envelope_counter_WriteCounter(uint8 counter) \
                                   
{
    #if(Envelope_counter_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (Envelope_counter_GLOBAL_ENABLE & Envelope_counter_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        Envelope_counter_GLOBAL_ENABLE |= Envelope_counter_BLOCK_EN_MASK;
        CY_SET_REG16(Envelope_counter_COUNTER_LSB_PTR, (uint16)counter);
        Envelope_counter_GLOBAL_ENABLE &= ((uint8)(~Envelope_counter_BLOCK_EN_MASK));
    #else
        CY_SET_REG8(Envelope_counter_COUNTER_LSB_PTR, counter);
    #endif /* (Envelope_counter_UsingFixedFunction) */
}
#endif /* (!(Envelope_counter_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: Envelope_counter_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint8) The present value of the counter.
*
*******************************************************************************/
uint8 Envelope_counter_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(Envelope_counter_UsingFixedFunction)
		(void)CY_GET_REG16(Envelope_counter_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(Envelope_counter_COUNTER_LSB_PTR_8BIT);
	#endif/* (Envelope_counter_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(Envelope_counter_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Envelope_counter_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(Envelope_counter_STATICCOUNT_LSB_PTR));
    #endif /* (Envelope_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Envelope_counter_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) Present Capture value.
*
*******************************************************************************/
uint8 Envelope_counter_ReadCapture(void) 
{
    #if(Envelope_counter_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Envelope_counter_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(Envelope_counter_STATICCOUNT_LSB_PTR));
    #endif /* (Envelope_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Envelope_counter_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint8) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
*******************************************************************************/
void Envelope_counter_WritePeriod(uint8 period) 
{
    #if(Envelope_counter_UsingFixedFunction)
        CY_SET_REG16(Envelope_counter_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG8(Envelope_counter_PERIOD_LSB_PTR, period);
    #endif /* (Envelope_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Envelope_counter_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint8) Present period value.
*
*******************************************************************************/
uint8 Envelope_counter_ReadPeriod(void) 
{
    #if(Envelope_counter_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(Envelope_counter_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(Envelope_counter_PERIOD_LSB_PTR));
    #endif /* (Envelope_counter_UsingFixedFunction) */
}


#if (!Envelope_counter_UsingFixedFunction)
/*******************************************************************************
* Function Name: Envelope_counter_WriteCompare
********************************************************************************
* Summary:
* Changes the compare value.  The compare output will 
* reflect the new value on the next UDB clock.  The compare output will be 
* driven high when the present counter value compares true based on the 
* configured compare mode setting. 
*
* Parameters:  
*  Compare:  New compare value. 
*
* Return: 
*  void
*
*******************************************************************************/
void Envelope_counter_WriteCompare(uint8 compare) \
                                   
{
    #if(Envelope_counter_UsingFixedFunction)
        CY_SET_REG16(Envelope_counter_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG8(Envelope_counter_COMPARE_LSB_PTR, compare);
    #endif /* (Envelope_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Envelope_counter_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint8) Present compare value.
*
*******************************************************************************/
uint8 Envelope_counter_ReadCompare(void) 
{
    return (CY_GET_REG8(Envelope_counter_COMPARE_LSB_PTR));
}


#if (Envelope_counter_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: Envelope_counter_SetCompareMode
********************************************************************************
* Summary:
*  Sets the software controlled Compare Mode.
*
* Parameters:
*  compareMode:  Compare Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void Envelope_counter_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    Envelope_counter_CONTROL &= ((uint8)(~Envelope_counter_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    Envelope_counter_CONTROL |= compareMode;
}
#endif  /* (Envelope_counter_COMPARE_MODE_SOFTWARE) */


#if (Envelope_counter_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: Envelope_counter_SetCaptureMode
********************************************************************************
* Summary:
*  Sets the software controlled Capture Mode.
*
* Parameters:
*  captureMode:  Capture Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void Envelope_counter_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    Envelope_counter_CONTROL &= ((uint8)(~Envelope_counter_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    Envelope_counter_CONTROL |= ((uint8)((uint8)captureMode << Envelope_counter_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (Envelope_counter_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: Envelope_counter_ClearFIFO
********************************************************************************
* Summary:
*   This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void:
*
* Return: 
*  None
*
*******************************************************************************/
void Envelope_counter_ClearFIFO(void) 
{

    while(0u != (Envelope_counter_ReadStatusRegister() & Envelope_counter_STATUS_FIFONEMP))
    {
        (void)Envelope_counter_ReadCapture();
    }

}
#endif  /* (!Envelope_counter_UsingFixedFunction) */


/* [] END OF FILE */

