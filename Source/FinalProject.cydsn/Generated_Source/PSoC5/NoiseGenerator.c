/*******************************************************************************
* File Name: NoiseGenerator.c
* Version 2.40
*
* Description:
*  This file provides the source code to the API for the PRS component
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "NoiseGenerator.h"

uint8 NoiseGenerator_initVar = 0u;

NoiseGenerator_BACKUP_STRUCT NoiseGenerator_backup =
{
    0x00u, /* enableState; */

    #if (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK)
        /* Save dff register for time mult */
        #if (NoiseGenerator_TIME_MULTIPLEXING_ENABLE)
            NoiseGenerator_INIT_STATE, /* dffStatus; */
        #endif  /* End NoiseGenerator_TIME_MULTIPLEXING_ENABLE */

        /* Save A0 and A1 registers are none-retention */
        #if(NoiseGenerator_PRS_SIZE <= 32u)
            NoiseGenerator_DEFAULT_SEED, /* seed */

        #else
            NoiseGenerator_DEFAULT_SEED_UPPER, /* seedUpper; */
            NoiseGenerator_DEFAULT_SEED_LOWER, /* seedLower; */

        #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */

    #endif  /* End (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK) */
};


/*******************************************************************************
* Function Name: NoiseGenerator_Init
********************************************************************************
*
* Summary:
*  Initializes seed and polynomial registers with initial values.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void NoiseGenerator_Init(void) 
{
    /* Writes seed value and ponynom value provided for customizer */
    #if (NoiseGenerator_PRS_SIZE <= 32u)
        NoiseGenerator_WritePolynomial(NoiseGenerator_DEFAULT_POLYNOM);
        NoiseGenerator_WriteSeed(NoiseGenerator_DEFAULT_SEED);
        #if (NoiseGenerator_RUN_MODE == NoiseGenerator__CLOCKED)
            NoiseGenerator_ResetSeedInit(NoiseGenerator_DEFAULT_SEED);
        #endif  /* End (NoiseGenerator_RUN_MODE == NoiseGenerator__CLOCKED) */
            NoiseGenerator_Enable();
    #else
        NoiseGenerator_WritePolynomialUpper(NoiseGenerator_DEFAULT_POLYNOM_UPPER);
        NoiseGenerator_WritePolynomialLower(NoiseGenerator_DEFAULT_POLYNOM_LOWER);
        NoiseGenerator_WriteSeedUpper(NoiseGenerator_DEFAULT_SEED_UPPER);
        NoiseGenerator_WriteSeedLower(NoiseGenerator_DEFAULT_SEED_LOWER);
        #if (NoiseGenerator_RUN_MODE == NoiseGenerator__CLOCKED)
            NoiseGenerator_ResetSeedInitUpper(NoiseGenerator_DEFAULT_SEED_UPPER);
            NoiseGenerator_ResetSeedInitLower(NoiseGenerator_DEFAULT_SEED_LOWER);
        #endif  /* End (NoiseGenerator_RUN_MODE == NoiseGenerator__CLOCKED) */
            NoiseGenerator_Enable();
    #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */
}


/*******************************************************************************
* Function Name: NoiseGenerator_Enable
********************************************************************************
*
* Summary:
*  Starts PRS computation on rising edge of input clock.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void NoiseGenerator_Enable(void) 
{
        NoiseGenerator_CONTROL_REG |= NoiseGenerator_CTRL_ENABLE;
}


/*******************************************************************************
* Function Name: NoiseGenerator_Start
********************************************************************************
*
* Summary:
*  Initializes seed and polynomial registers with initial values. Computation
*  of PRS starts on rising edge of input clock.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  NoiseGenerator_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time NoiseGenerator_Start() is called. This allows
*  enable/disable component without re-initialization in all subsequent calls
*  to the NoiseGenerator_Start() routine.
*
*******************************************************************************/
void NoiseGenerator_Start(void) 
{
    /* Writes seed value and ponynom value provided from customizer */
    if (NoiseGenerator_initVar == 0u)
    {
        NoiseGenerator_Init();
        NoiseGenerator_initVar = 1u;
    }

    NoiseGenerator_Enable();
}


/*******************************************************************************
* Function Name: NoiseGenerator_Stop
********************************************************************************
*
* Summary:
*  Stops PRS computation.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void NoiseGenerator_Stop(void) 
{
    NoiseGenerator_CONTROL_REG &= ((uint8)~NoiseGenerator_CTRL_ENABLE);
}


#if (NoiseGenerator_RUN_MODE == NoiseGenerator__APISINGLESTEP)
    /*******************************************************************************
    * FUNCTION NAME: NoiseGenerator_Step
    ********************************************************************************
    *
    * Summary:
    *  Increments the PRS by one when API single step mode is used.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  void
    *
    *******************************************************************************/
    void NoiseGenerator_Step(void) 
    {
        #if (NoiseGenerator_TIME_MULTIPLEXING_ENABLE)
            /* Makes 4 pulse, 4x for Time Mult */
            NoiseGenerator_EXECUTE_STEP;
            NoiseGenerator_EXECUTE_STEP;
            NoiseGenerator_EXECUTE_STEP;
            NoiseGenerator_EXECUTE_STEP;

        #else
            /* One pulse without Time mult required */
            NoiseGenerator_EXECUTE_STEP;

        #endif  /* End NoiseGenerator_TIME_MULTIPLEXING_ENABLE */
    }
#endif  /* End (NoiseGenerator_RUN_MODE == NoiseGenerator__APISINGLESTEP) */


#if (NoiseGenerator_RUN_MODE == NoiseGenerator__CLOCKED)
    #if (NoiseGenerator_PRS_SIZE <= 32u) /* 8-32 bits PRS */
        /*******************************************************************************
        * FUNCTION NAME: NoiseGenerator_ResetSeedInit
        ********************************************************************************
        *
        * Summary:
        *  Increments the PRS by one when API single step mode is used.
        *
        * Parameters:
        *  void
        *
        * Return:
        *  void
        *
        *******************************************************************************/
        void NoiseGenerator_ResetSeedInit(uint32 seed)
                                            
        {
            uint8 enableInterrupts;

            /* Mask the Seed to cut unused bits */
            seed &= NoiseGenerator_MASK;

            /* Change AuxControl reg, need to be safety */
            enableInterrupts = CyEnterCriticalSection();

            #if (NoiseGenerator_TIME_MULTIPLEXING_ENABLE)
                /* Set FIFOs to single register */
                NoiseGenerator_AUX_CONTROL_A_REG |= NoiseGenerator_AUXCTRL_FIFO_SINGLE_REG;

                #if(NoiseGenerator_PRS_SIZE > 16u)       /* 17-32 bits PRS */
                    NoiseGenerator_AUX_CONTROL_B_REG |= NoiseGenerator_AUXCTRL_FIFO_SINGLE_REG;
                #endif  /* End (NoiseGenerator_PRS_SIZE <= 8u) */

                /* AuxControl reg settings are done */
                CyExitCriticalSection(enableInterrupts);

                /* Write Seed COPY */
                #if (NoiseGenerator_PRS_SIZE <= 16u)          /* 16 bits PRS */
                    NoiseGenerator_SEED_COPY_A__A1_REG = HI8(seed);
                    NoiseGenerator_SEED_COPY_A__A0_REG = LO8(seed);

                #elif (NoiseGenerator_PRS_SIZE <= 24u)        /* 24 bits PRS */
                    NoiseGenerator_SEED_COPY_B__A1_REG = LO8(HI16(seed));
                    NoiseGenerator_SEED_COPY_B__A0_REG = HI8(seed);
                    NoiseGenerator_SEED_COPY_A__A0_REG = LO8(seed);

                #else                                           /* 32 bits PRS */
                    NoiseGenerator_SEED_COPY_B__A1_REG = HI8(HI16(seed));
                    NoiseGenerator_SEED_COPY_A__A1_REG = LO8(HI16(seed));
                    NoiseGenerator_SEED_COPY_B__A0_REG = HI8(seed);
                    NoiseGenerator_SEED_COPY_A__A0_REG = LO8(seed);
                #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */

            #else
                /* Set FIFOs to single register */
                #if (NoiseGenerator_PRS_SIZE <= 8u)      /* 8 bits PRS */
                    NoiseGenerator_AUX_CONTROL_A_REG |= NoiseGenerator_AUXCTRL_FIFO_SINGLE_REG;

                #elif (NoiseGenerator_PRS_SIZE <= 16u)      /* 16 bits PRS */
                    NoiseGenerator_AUX_CONTROL_A_REG  |= NoiseGenerator_AUXCTRL_FIFO_SINGLE_REG;
                    NoiseGenerator_AUX_CONTROL_B_REG  |= NoiseGenerator_AUXCTRL_FIFO_SINGLE_REG;

                #elif (NoiseGenerator_PRS_SIZE <= 24u)      /* 24-39 bits PRS */
                    NoiseGenerator_AUX_CONTROL_A_REG  |= NoiseGenerator_AUXCTRL_FIFO_SINGLE_REG;
                    NoiseGenerator_AUX_CONTROL_B_REG  |= NoiseGenerator_AUXCTRL_FIFO_SINGLE_REG;
                    NoiseGenerator_AUX_CONTROL_C_REG  |= NoiseGenerator_AUXCTRL_FIFO_SINGLE_REG;

                #elif (NoiseGenerator_PRS_SIZE <= 32u)      /* 40-55 bits PRS */
                    NoiseGenerator_AUX_CONTROL_A_REG  |= NoiseGenerator_AUXCTRL_FIFO_SINGLE_REG;
                    NoiseGenerator_AUX_CONTROL_B_REG  |= NoiseGenerator_AUXCTRL_FIFO_SINGLE_REG;
                    NoiseGenerator_AUX_CONTROL_C_REG  |= NoiseGenerator_AUXCTRL_FIFO_SINGLE_REG;
                    NoiseGenerator_AUX_CONTROL_D_REG  |= NoiseGenerator_AUXCTRL_FIFO_SINGLE_REG;

                #endif  /* End (NoiseGenerator_PRS_SIZE <= 8u) */

                /* AuxControl reg setting are done */
                CyExitCriticalSection(enableInterrupts);

                /* Write Seed COPY */
                CY_SET_REG32(NoiseGenerator_SEED_COPY_PTR, seed);

            #endif  /* End (NoiseGenerator_TIME_MULTIPLEXING_ENABLE) */
        }

    #else

        /*******************************************************************************
        * FUNCTION NAME: NoiseGenerator_ResetSeedInitUpper
        ********************************************************************************
        *
        * Summary:
        *  Increments the PRS by one when API single step mode is used.
        *
        * Parameters:
        *  void
        *
        * Return:
        *  void
        *
        *******************************************************************************/
        void NoiseGenerator_ResetSeedInitUpper(uint32 seed) 
        {
			uint8 enableInterrupts;

			/* Mask the Seed Upper half to cut unused bits */
            seed &= NoiseGenerator_MASK;

			/* Change AuxControl reg, need to be safety */
            enableInterrupts = CyEnterCriticalSection();

            /* Set FIFOs to single register */
            NoiseGenerator_AUX_CONTROL_A_REG |= NoiseGenerator_AUXCTRL_FIFO_SINGLE_REG;
            NoiseGenerator_AUX_CONTROL_B_REG |= NoiseGenerator_AUXCTRL_FIFO_SINGLE_REG;
            NoiseGenerator_AUX_CONTROL_C_REG |= NoiseGenerator_AUXCTRL_FIFO_SINGLE_REG;

			#if (NoiseGenerator_PRS_SIZE > 48u)               /* 49-64 bits PRS */
                NoiseGenerator_AUX_CONTROL_D_REG |= NoiseGenerator_AUXCTRL_FIFO_SINGLE_REG;
            #endif  /* End (NoiseGenerator_PRS_SIZE <= 8u) */

            /* AuxControl reg settings are done */
            CyExitCriticalSection(enableInterrupts);

            /* Write Seed Upper COPY */
            #if (NoiseGenerator_PRS_SIZE <= 40u)          /* 40 bits PRS */
                NoiseGenerator_SEED_UPPER_COPY_C__A1_REG = LO8(seed);

            #elif (NoiseGenerator_PRS_SIZE <= 48u)        /* 48 bits PRS */
                NoiseGenerator_SEED_UPPER_COPY_C__A1_REG = HI8(seed);
                NoiseGenerator_SEED_UPPER_COPY_B__A1_REG = LO8(seed);

            #elif (NoiseGenerator_PRS_SIZE <= 56u)        /* 56 bits PRS */
                NoiseGenerator_SEED_UPPER_COPY_D__A1_REG = LO8(HI16(seed));
                NoiseGenerator_SEED_UPPER_COPY_C__A1_REG = HI8(seed);
                NoiseGenerator_SEED_UPPER_COPY_B__A1_REG = HI8(seed);

            #else                                           /* 64 bits PRS */
                NoiseGenerator_SEED_UPPER_COPY_D__A1_REG = HI8(HI16(seed));
                NoiseGenerator_SEED_UPPER_COPY_C__A1_REG = LO8(HI16(seed));
                NoiseGenerator_SEED_UPPER_COPY_B__A1_REG = HI8(seed);
                NoiseGenerator_SEED_UPPER_COPY_A__A1_REG = LO8(seed);

            #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */
        }


        /*******************************************************************************
        * FUNCTION NAME: NoiseGenerator_ResetSeedInitLower
        ********************************************************************************
        *
        * Summary:
        *  Increments the PRS by one when API single step mode is used.
        *
        * Parameters:
        *  void
        *
        * Return:
        *  void
        *
        *******************************************************************************/
        void NoiseGenerator_ResetSeedInitLower(uint32 seed) 
        {
            /* Write Seed Lower COPY */
            #if (NoiseGenerator_PRS_SIZE <= 40u)          /* 40 bits PRS */
                NoiseGenerator_SEED_LOWER_COPY_B__A1_REG = HI8(HI16(seed));
                NoiseGenerator_SEED_LOWER_COPY_C__A0_REG = LO8(HI16(seed));
                NoiseGenerator_SEED_LOWER_COPY_B__A0_REG = HI8(seed);
                NoiseGenerator_SEED_LOWER_COPY_A__A0_REG = LO8(seed);

            #elif (NoiseGenerator_PRS_SIZE <= 48u)        /* 48 bits PRS */
                NoiseGenerator_SEED_LOWER_COPY_A__A1_REG = HI8(HI16(seed));
                NoiseGenerator_SEED_LOWER_COPY_C__A0_REG = LO8(HI16(seed));
                NoiseGenerator_SEED_LOWER_COPY_B__A0_REG = HI8(seed);
                NoiseGenerator_SEED_LOWER_COPY_A__A0_REG = LO8(seed);

            #else                                           /* 64 bits PRS */
                NoiseGenerator_SEED_LOWER_COPY_D__A0_REG = HI8(HI16(seed));
                NoiseGenerator_SEED_LOWER_COPY_C__A0_REG = LO8(HI16(seed));
                NoiseGenerator_SEED_LOWER_COPY_B__A0_REG = HI8(seed);
                NoiseGenerator_SEED_LOWER_COPY_A__A0_REG = LO8(seed);

            #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */
        }

    #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */

#endif  /* End (NoiseGenerator_RUN_MODE == NoiseGenerator__CLOCKED) */


#if(NoiseGenerator_PRS_SIZE <= 32u) /* 8-32 bits PRS */
    /*******************************************************************************
    * Function Name: NoiseGenerator_Read
    ********************************************************************************
    *
    * Summary:
    *  Reads PRS value.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  Returns PRS value.
    *
    * Side Effects:
    *  The seed value is cut according to mask = 2^(Resolution) - 1.
    *  For example if PRS Resolution is 14 bits the mask value is:
    *  mask = 2^(14) - 1 = 0x3FFFu.
    *  The seed value = 0xFFFFu is cut:
    *  seed & mask = 0xFFFFu & 0x3FFFu = 0x3FFFu.
    *
    *******************************************************************************/
    uint32 NoiseGenerator_Read(void) 
    {
        /* Read PRS */
        #if (NoiseGenerator_TIME_MULTIPLEXING_ENABLE)

            uint32 seed;

            #if (NoiseGenerator_PRS_SIZE <= 16u)          /* 16 bits PRS */
                seed = ((uint16) NoiseGenerator_SEED_A__A1_REG) << 8u;
                seed |= NoiseGenerator_SEED_A__A0_REG;

            #elif (NoiseGenerator_PRS_SIZE <= 24u)        /* 24 bits PRS */
                seed = ((uint32) (NoiseGenerator_SEED_B__A1_REG)) << 16u;
                seed |= ((uint32) (NoiseGenerator_SEED_B__A0_REG)) << 8u;
                seed |= NoiseGenerator_SEED_A__A0_REG;

            #else                                           /* 32 bits PRS */
                seed = ((uint32) NoiseGenerator_SEED_B__A1_REG) << 24u;
                seed |= ((uint32) NoiseGenerator_SEED_A__A1_REG) << 16u;
                seed |= ((uint32) NoiseGenerator_SEED_B__A0_REG) << 8u;
                seed |= NoiseGenerator_SEED_A__A0_REG;

            #endif  /* End (NoiseGenerator_PRS_SIZE <= 8u) */

            return (seed  & NoiseGenerator_MASK);

        #else

            return (CY_GET_REG32(NoiseGenerator_SEED_PTR) & NoiseGenerator_MASK);

        #endif  /* End (NoiseGenerator_TIME_MULTIPLEXING_ENABLE) */
    }


    /*******************************************************************************
    * Function Name: NoiseGenerator_WriteSeed
    ********************************************************************************
    *
    * Summary:
    *  Writes seed value.
    *
    * Parameters:
    *  seed:  Seed value.
    *
    * Return:
    *  void
    *
    * Side Effects:
    *  The seed value is cut according to mask = 2^(Resolution) - 1.
    *  For example if PRS Resolution is 14 bits the mask value is:
    *  mask = 2^(14) - 1 = 0x3FFFu.
    *  The seed value = 0xFFFFu is cut:
    *  seed & mask = 0xFFFFu & 0x3FFFu = 0x3FFFu.
    *
    *******************************************************************************/
    void NoiseGenerator_WriteSeed(uint32 seed) 
    {
        /* Masks the Seed to cut unused bits */
        seed &= NoiseGenerator_MASK;

        /* Writes Seed */
        #if (NoiseGenerator_TIME_MULTIPLEXING_ENABLE)

            #if (NoiseGenerator_PRS_SIZE <= 16u)          /* 16 bits PRS */
                NoiseGenerator_SEED_A__A1_REG = HI8(seed);
                NoiseGenerator_SEED_A__A0_REG = LO8(seed);

            #elif (NoiseGenerator_PRS_SIZE <= 24u)        /* 24 bits PRS */
                NoiseGenerator_SEED_B__A1_REG = LO8(HI16(seed));
                NoiseGenerator_SEED_B__A0_REG = HI8(seed);
                NoiseGenerator_SEED_A__A0_REG = LO8(seed);

            #else                                           /* 32 bits PRS */
                NoiseGenerator_SEED_B__A1_REG = HI8(HI16(seed));
                NoiseGenerator_SEED_A__A1_REG = LO8(HI16(seed));
                NoiseGenerator_SEED_B__A0_REG = HI8(seed);
                NoiseGenerator_SEED_A__A0_REG = LO8(seed);
            #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */

            /* Resets triggers */
            #if (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK)
                if((NoiseGenerator_sleepState & NoiseGenerator_NORMAL_SEQUENCE) != 0u)
                {
                    NoiseGenerator_EXECUTE_DFF_RESET;
                }
            #else
                NoiseGenerator_EXECUTE_DFF_RESET;
            #endif  /* (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK) */

        #else

            CY_SET_REG32(NoiseGenerator_SEED_PTR, seed);

        #endif  /* End (NoiseGenerator_TIME_MULTIPLEXING_ENABLE) */
    }


    /*******************************************************************************
    * Function Name: NoiseGenerator_ReadPolynomial
    ********************************************************************************
    *
    * Summary:
    *  Reads PRS polynomial value.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  Returns PRS polynomial value.
    *
    *******************************************************************************/
    uint32 NoiseGenerator_ReadPolynomial(void)
                                
    {
        /* Reads polynomial */
        #if (NoiseGenerator_TIME_MULTIPLEXING_ENABLE)

            uint32 polynomial;

            #if (NoiseGenerator_PRS_SIZE <= 16u)          /* 16 bits PRS */
                polynomial = ((uint16) NoiseGenerator_POLYNOM_A__D1_REG) << 8u;
                polynomial |= (NoiseGenerator_POLYNOM_A__D0_REG);

            #elif (NoiseGenerator_PRS_SIZE <= 24u)        /* 24 bits PRS */
                polynomial = ((uint32) NoiseGenerator_POLYNOM_B__D1_REG) << 16u;
                polynomial |= ((uint32) NoiseGenerator_POLYNOM_B__D0_REG) << 8u;
                polynomial |= NoiseGenerator_POLYNOM_A__D0_REG;

            #else                                           /* 32 bits PRS */
                polynomial = ((uint32) NoiseGenerator_POLYNOM_B__D1_REG) << 24u;
                polynomial |= ((uint32) NoiseGenerator_POLYNOM_A__D1_REG) << 16u;
                polynomial |= ((uint32) NoiseGenerator_POLYNOM_B__D0_REG) << 8u;
                polynomial |= NoiseGenerator_POLYNOM_A__D0_REG;

            #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */

            return polynomial;

        #else

            return CY_GET_REG32(NoiseGenerator_POLYNOM_PTR);

        #endif  /* End (NoiseGenerator_TIME_MULTIPLEXING_ENABLE) */
    }


    /*******************************************************************************
    * Function Name: NoiseGenerator_WritePolynomial
    ********************************************************************************
    *
    * Summary:
    *  Writes PRS polynomial value.
    *
    * Parameters:
    *  polynomial:  PRS polynomial.
    *
    * Return:
    *  void
    *
    * Side Effects:
    *  The polynomial value is cut according to mask = 2^(Resolution) - 1.
    *  For example if PRS Resolution is 14 bits the mask value is:
    *  mask = 2^(14) - 1 = 0x3FFFu.
    *  The polynomial value = 0xFFFFu is cut:
    *  polynomial & mask = 0xFFFFu & 0x3FFFu = 0x3FFFu.
    *
    *******************************************************************************/
    void NoiseGenerator_WritePolynomial(uint32 polynomial)
                                          
    {
        /* Mask polynomial to cut unused bits */
        polynomial &= NoiseGenerator_MASK;

        /* Write polynomial */
        #if (NoiseGenerator_TIME_MULTIPLEXING_ENABLE)

            #if (NoiseGenerator_PRS_SIZE <= 16u)          /* 16 bits PRS */

                NoiseGenerator_POLYNOM_A__D1_REG = HI8(polynomial);
                NoiseGenerator_POLYNOM_A__D0_REG = LO8(polynomial);

            #elif (NoiseGenerator_PRS_SIZE <= 24u)        /* 24 bits PRS */
                NoiseGenerator_POLYNOM_B__D1_REG = LO8(HI16(polynomial));
                NoiseGenerator_POLYNOM_B__D0_REG = HI8(polynomial);
                NoiseGenerator_POLYNOM_A__D0_REG = LO8(polynomial);

            #else                                           /* 32 bits PRS */
                NoiseGenerator_POLYNOM_B__D1_REG = HI8(HI16(polynomial));
                NoiseGenerator_POLYNOM_A__D1_REG = LO8(HI16(polynomial));
                NoiseGenerator_POLYNOM_B__D0_REG = HI8(polynomial);
                NoiseGenerator_POLYNOM_A__D0_REG = LO8(polynomial);

            #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */

            /* Resets triggers */
            #if (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK)
                if((NoiseGenerator_sleepState & NoiseGenerator_NORMAL_SEQUENCE) != 0u)
                {
                    NoiseGenerator_EXECUTE_DFF_RESET;
                }
            #else
                NoiseGenerator_EXECUTE_DFF_RESET;
            #endif  /* (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK) */

        #else

            CY_SET_REG32(NoiseGenerator_POLYNOM_PTR, polynomial);

        #endif  /* End (NoiseGenerator_TIME_MULTIPLEXING_ENABLE) */
    }

#else   /* 33-64 bits PRS */

    /*******************************************************************************
    *  Function Name: NoiseGenerator_ReadUpper
    ********************************************************************************
    *
    * Summary:
    *  Reads upper half of PRS value. Only generated for 33-64-bit PRS.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  Returns upper half of PRS value.
    *
    * Side Effects:
    *  The upper half of seed value is cut according to
    *  mask = 2^(Resolution - 32) - 1.
    *  For example if PRS Resolution is 35 bits the mask value is:
    *  2^(35 - 32) - 1 = 2^(3) - 1 = 0x0000 0007u.
    *  The upper half of seed value = 0x0000 00FFu is cut:
    *  upper half of seed & mask = 0x0000 00FFu & 0x0000 0007u = 0x0000 0007u.
    *
    *******************************************************************************/
    uint32 NoiseGenerator_ReadUpper(void) 
    {
        uint32 seed;

        /* Read PRS Upper */
        #if (NoiseGenerator_PRS_SIZE <= 40u)          /* 40 bits PRS */
            seed = NoiseGenerator_SEED_UPPER_C__A1_REG;

        #elif (NoiseGenerator_PRS_SIZE <= 48u)        /* 48 bits PRS */
            seed = ((uint32) NoiseGenerator_SEED_UPPER_C__A1_REG) << 8u;
            seed |= NoiseGenerator_SEED_UPPER_B__A1_REG;

        #elif (NoiseGenerator_PRS_SIZE <= 56u)        /* 56 bits PRS */
            seed = ((uint32) NoiseGenerator_SEED_UPPER_D__A1_REG) << 16u;
            seed |= ((uint32) NoiseGenerator_SEED_UPPER_C__A1_REG) << 8u;
            seed |= NoiseGenerator_SEED_UPPER_B__A1_REG;

        #else                                           /* 64 bits PRS */
            seed = ((uint32) NoiseGenerator_SEED_UPPER_D__A1_REG) << 24u;
            seed |= ((uint32) NoiseGenerator_SEED_UPPER_C__A1_REG) << 16u;
            seed |= ((uint32) NoiseGenerator_SEED_UPPER_B__A1_REG) << 8u;
            seed |= NoiseGenerator_SEED_UPPER_A__A1_REG;

        #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */

        return (seed & NoiseGenerator_MASK);
    }


    /*******************************************************************************
    *  Function Name: NoiseGenerator_ReadLower
    ********************************************************************************
    *
    * Summary:
    *  Reads lower half of PRS value. Only generated for 33-64-bit PRS.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  Returns lower half of PRS value.
    *
    *******************************************************************************/
    uint32 NoiseGenerator_ReadLower(void) 
    {
        uint32 seed;

        /* Read PRS Lower */
        #if (NoiseGenerator_PRS_SIZE <= 40u)          /* 40 bits PRS */
            seed = ((uint32) NoiseGenerator_SEED_LOWER_B__A1_REG) << 24u;
            seed |= ((uint32) NoiseGenerator_SEED_LOWER_C__A0_REG) << 16u;
            seed |= ((uint32) NoiseGenerator_SEED_LOWER_B__A0_REG) << 8u;
            seed |= NoiseGenerator_SEED_LOWER_A__A0_REG;

        #elif (NoiseGenerator_PRS_SIZE <= 48u)        /* 48 bits PRS */
            seed = ((uint32) NoiseGenerator_SEED_LOWER_A__A1_REG) << 24u;
            seed |= ((uint32) NoiseGenerator_SEED_LOWER_C__A0_REG) << 16u;
            seed |= ((uint32) NoiseGenerator_SEED_LOWER_B__A0_REG) << 8u;
            seed |= NoiseGenerator_SEED_LOWER_A__A0_REG;

        #else                                           /* 64 bits PRS */
            seed = ((uint32) NoiseGenerator_SEED_LOWER_D__A0_REG) << 24u;
            seed |= ((uint32) NoiseGenerator_SEED_LOWER_C__A0_REG) << 16u;
            seed |= ((uint32) NoiseGenerator_SEED_LOWER_B__A0_REG) << 8u;
            seed |= NoiseGenerator_SEED_LOWER_A__A0_REG;

        #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */

        return seed;
    }


    /*******************************************************************************
    * Function Name: NoiseGenerator_WriteSeedUpper
    ********************************************************************************
    *
    * Summary:
    *  Writes upper half of seed value. Only generated for 33-64-bit PRS.
    *
    * Parameters:
    *  seed:  Upper half of seed value.
    *
    * Return:
    *  void
    *
    * Side Effects:
    *  The upper half of seed value is cut according to
    *  mask = 2^(Resolution - 32) - 1.
    *  For example if PRS Resolution is 35 bits the mask value is:
    *  2^(35 - 32) - 1 = 2^(3) - 1 = 0x0000 0007u.
    *  The upper half of seed value = 0x0000 00FFu is cut:
    *  upper half of seed & mask = 0x0000 00FFu & 0x0000 0007u = 0x0000 0007u.
    *
    *******************************************************************************/
    void NoiseGenerator_WriteSeedUpper(uint32 seed) 
    {
        /* Mask the Seed Upper half to cut unused bits */
        seed &= NoiseGenerator_MASK;

        /* Write Seed Upper */
        #if (NoiseGenerator_PRS_SIZE <= 40u)          /* 40 bits PRS */
            NoiseGenerator_SEED_UPPER_C__A1_REG = LO8(seed);

        #elif (NoiseGenerator_PRS_SIZE <= 48u)        /* 48 bits PRS */
            NoiseGenerator_SEED_UPPER_C__A1_REG = HI8(seed);
            NoiseGenerator_SEED_UPPER_B__A1_REG = LO8(seed);

        #elif (NoiseGenerator_PRS_SIZE <= 56u)        /* 56 bits PRS */
            NoiseGenerator_SEED_UPPER_D__A1_REG = LO8(HI16(seed));
            NoiseGenerator_SEED_UPPER_C__A1_REG = HI8(seed);
            NoiseGenerator_SEED_UPPER_B__A1_REG = HI8(seed);

        #else                                           /* 64 bits PRS */
            NoiseGenerator_SEED_UPPER_D__A1_REG = HI8(HI16(seed));
            NoiseGenerator_SEED_UPPER_C__A1_REG = LO8(HI16(seed));
            NoiseGenerator_SEED_UPPER_B__A1_REG = HI8(seed);
            NoiseGenerator_SEED_UPPER_A__A1_REG = LO8(seed);

        #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */

        /* Resets triggers */
        #if (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK)
            if((NoiseGenerator_sleepState & NoiseGenerator_NORMAL_SEQUENCE) != 0u)
            {
                NoiseGenerator_EXECUTE_DFF_RESET;
            }
        #else
            NoiseGenerator_EXECUTE_DFF_RESET;
        #endif  /* (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK) */
    }


    /*******************************************************************************
    * Function Name: NoiseGenerator_WriteSeedLower
    ********************************************************************************
    *
    * Summary:
    *  Writes lower half of seed value. Only generated for 33-64-bit PRS.
    *
    * Parameters:
    *  seed:  Lower half of seed value.
    *
    * Return:
    *  void
    *
    *******************************************************************************/
    void NoiseGenerator_WriteSeedLower(uint32 seed) 
    {
        /* Write Seed Lower */
        #if (NoiseGenerator_PRS_SIZE <= 40u)          /* 40 bits PRS */
            NoiseGenerator_SEED_LOWER_B__A1_REG = HI8(HI16(seed));
            NoiseGenerator_SEED_LOWER_C__A0_REG = LO8(HI16(seed));
            NoiseGenerator_SEED_LOWER_B__A0_REG = HI8(seed);
            NoiseGenerator_SEED_LOWER_A__A0_REG = LO8(seed);

        #elif (NoiseGenerator_PRS_SIZE <= 48u)        /* 48 bits PRS */
            NoiseGenerator_SEED_LOWER_A__A1_REG = HI8(HI16(seed));
            NoiseGenerator_SEED_LOWER_C__A0_REG = LO8(HI16(seed));
            NoiseGenerator_SEED_LOWER_B__A0_REG = HI8(seed);
            NoiseGenerator_SEED_LOWER_A__A0_REG = LO8(seed);

        #else                                           /* 64 bits PRS */
            NoiseGenerator_SEED_LOWER_D__A0_REG = HI8(HI16(seed));
            NoiseGenerator_SEED_LOWER_C__A0_REG = LO8(HI16(seed));
            NoiseGenerator_SEED_LOWER_B__A0_REG = HI8(seed);
            NoiseGenerator_SEED_LOWER_A__A0_REG = LO8(seed);

        #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */

        /* Resets triggers */
        #if (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK)
            if((NoiseGenerator_sleepState & NoiseGenerator_NORMAL_SEQUENCE) != 0u)
            {
                NoiseGenerator_EXECUTE_DFF_RESET;
            }
        #else
            NoiseGenerator_EXECUTE_DFF_RESET;
        #endif  /* (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK) */
    }


    /*******************************************************************************
    * Function Name: NoiseGenerator_ReadPolynomialUpper
    ********************************************************************************
    *
    * Summary:
    *  Reads upper half of PRS polynomial value. Only generated for 33-64-bit PRS.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  Returns upper half of PRS polynomial value.
    *
    *******************************************************************************/
    uint32 NoiseGenerator_ReadPolynomialUpper(void) 
    {
        uint32 polynomial;

        /* Read Polynomial Upper */
        #if (NoiseGenerator_PRS_SIZE <= 40u)          /* 40 bits PRS */
            polynomial = NoiseGenerator_POLYNOM_UPPER_C__D1_REG;

        #elif (NoiseGenerator_PRS_SIZE <= 48u)        /* 48 bits PRS */
            polynomial = ((uint32) NoiseGenerator_POLYNOM_UPPER_C__D1_REG) << 8u;
            polynomial |= NoiseGenerator_POLYNOM_UPPER_B__D1_REG;

        #elif (NoiseGenerator_PRS_SIZE <= 56u)        /* 56 bits PRS */
            polynomial = ((uint32) NoiseGenerator_POLYNOM_UPPER_D__D1_REG) << 16u;
            polynomial |= ((uint32) NoiseGenerator_POLYNOM_UPPER_C__D1_REG) << 8u;
            polynomial |= NoiseGenerator_POLYNOM_UPPER_B__D1_REG;

        #else                                           /* 64 bits PRS */
            polynomial = ((uint32) NoiseGenerator_POLYNOM_UPPER_D__D1_REG) << 24u;
            polynomial |= ((uint32) NoiseGenerator_POLYNOM_UPPER_C__D1_REG) << 16u;
            polynomial |= ((uint32) NoiseGenerator_POLYNOM_UPPER_B__D1_REG) << 8u;
            polynomial |= NoiseGenerator_POLYNOM_UPPER_A__D1_REG;

        #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */

        return polynomial;
    }


    /*******************************************************************************
    * Function Name: NoiseGenerator_ReadPolynomialLower
    ********************************************************************************
    *
    * Summary:
    *  Reads lower half of PRS polynomial value. Only generated for 33-64-bit PRS.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  Returns lower half of PRS polynomial value.
    *
    *******************************************************************************/
    uint32 NoiseGenerator_ReadPolynomialLower(void) 
    {
        uint32 polynomial;

        /* Read Polynomial Lower */
        #if (NoiseGenerator_PRS_SIZE <= 40u)          /* 40 bits PRS */
            polynomial = ( (uint32) NoiseGenerator_POLYNOM_LOWER_B__D1_REG) << 24u;
            polynomial |= ( (uint32) NoiseGenerator_POLYNOM_LOWER_C__D0_REG) << 16u;
            polynomial |= ( (uint32) NoiseGenerator_POLYNOM_LOWER_B__D0_REG) << 8u;
            polynomial |= NoiseGenerator_POLYNOM_LOWER_A__D0_REG;

        #elif (NoiseGenerator_PRS_SIZE <= 48u)        /* 48 bits PRS */
            polynomial = ((uint32) NoiseGenerator_POLYNOM_LOWER_A__D1_REG) << 24u;
            polynomial |= ((uint32) NoiseGenerator_POLYNOM_LOWER_C__D0_REG) << 16u;
            polynomial |= ((uint32) NoiseGenerator_POLYNOM_LOWER_B__D0_REG) << 8u;
            polynomial |= NoiseGenerator_POLYNOM_LOWER_A__D0_REG;

        #else                                           /* 64 bits PRS */
            polynomial = ((uint32) NoiseGenerator_POLYNOM_LOWER_D__D0_REG) << 24u;
            polynomial |= ((uint32) NoiseGenerator_POLYNOM_LOWER_C__D0_REG) << 16u;
            polynomial |= ((uint32) NoiseGenerator_POLYNOM_LOWER_B__D0_REG) << 8u;
            polynomial |= NoiseGenerator_POLYNOM_LOWER_A__D0_REG;

        #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */

        return polynomial;
    }


    /*******************************************************************************
    * Function Name: NoiseGenerator_WritePolynomialUpper
    ********************************************************************************
    *
    * Summary:
    *  Writes upper half of PRS polynomial value. Only generated for 33-64-bit PRS.
    *
    * Parameters:
    *  polynomial:  Upper half PRS polynomial value.
    *
    * Return:
    *  void
    *
    * Side Effects:
    *  The upper half of polynomial value is cut according to
    *  mask = 2^(Resolution - 32) - 1.
    *  For example if PRS Resolution is 35 bits the mask value is:
    *  2^(35 - 32) - 1 = 2^(3) - 1 = 0x0000 0007u.
    *  The upper half of polynomial value = 0x0000 00FFu is cut:
    *  upper half of polynomial & mask = 0x0000 00FFu & 0x0000 0007u = 0x0000 0007u.
    *
    *******************************************************************************/
    void NoiseGenerator_WritePolynomialUpper(uint32 polynomial)
                                                
    {
        /* Mask the polynomial upper half to cut unused bits */
        polynomial &= NoiseGenerator_MASK;

        /* Write Polynomial Upper */
        #if (NoiseGenerator_PRS_SIZE <= 40u)          /* 40 bits PRS */
            NoiseGenerator_POLYNOM_UPPER_C__D1_REG = LO8(polynomial);

        #elif (NoiseGenerator_PRS_SIZE <= 48u)        /* 48 bits PRS */
            NoiseGenerator_POLYNOM_UPPER_C__D1_REG = HI8(polynomial);
            NoiseGenerator_POLYNOM_UPPER_B__D1_REG = LO8(polynomial);

        #elif (NoiseGenerator_PRS_SIZE <= 56u)        /* 56 bits PRS */
            NoiseGenerator_POLYNOM_UPPER_D__D1_REG = LO8(HI16(polynomial));
            NoiseGenerator_POLYNOM_UPPER_C__D1_REG = HI8(polynomial);
            NoiseGenerator_POLYNOM_UPPER_B__D1_REG = LO8(polynomial);

        #else                                           /* 64 bits PRS */
            NoiseGenerator_POLYNOM_UPPER_D__D1_REG = HI8(HI16(polynomial));
            NoiseGenerator_POLYNOM_UPPER_C__D1_REG = LO8(HI16(polynomial));
            NoiseGenerator_POLYNOM_UPPER_B__D1_REG = HI8(polynomial);
            NoiseGenerator_POLYNOM_UPPER_A__D1_REG = LO8(polynomial);

        #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */

        /* Resets triggers */
        #if (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK)
            if((NoiseGenerator_sleepState & NoiseGenerator_NORMAL_SEQUENCE) != 0u)
            {
                NoiseGenerator_EXECUTE_DFF_RESET;
            }
        #else
            NoiseGenerator_EXECUTE_DFF_RESET;
        #endif  /* (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK) */
    }


    /*******************************************************************************
    * Function Name: NoiseGenerator_WritePolynomialLower
    ********************************************************************************
    *
    * Summary:
    *  Writes lower half of PRS polynomial value. Only generated for 33-64-bit PRS.
    *
    * Parameters:
    *  polynomial:  Lower half of PRS polynomial value.
    *
    * Return:
    *  void
    *
    *******************************************************************************/
    void NoiseGenerator_WritePolynomialLower(uint32 polynomial)
                                                
    {
        /* Write Polynomial Lower */
        #if (NoiseGenerator_PRS_SIZE <= 40u)          /* 40 bits PRS */
            NoiseGenerator_POLYNOM_LOWER_B__D1_REG = HI8(HI16(polynomial));
            NoiseGenerator_POLYNOM_LOWER_C__D0_REG = LO8(HI16(polynomial));
            NoiseGenerator_POLYNOM_LOWER_B__D0_REG = HI8(polynomial);
            NoiseGenerator_POLYNOM_LOWER_A__D0_REG = LO8(polynomial);

        #elif (NoiseGenerator_PRS_SIZE <= 48u)        /* 48 bits PRS */
            NoiseGenerator_POLYNOM_LOWER_A__D1_REG = HI8(HI16(polynomial));
            NoiseGenerator_POLYNOM_LOWER_C__D0_REG = LO8(HI16(polynomial));
            NoiseGenerator_POLYNOM_LOWER_B__D0_REG = HI8(polynomial);
            NoiseGenerator_POLYNOM_LOWER_A__D0_REG = LO8(polynomial);

        #else                                           /* 64 bits PRS */
            NoiseGenerator_POLYNOM_LOWER_D__D0_REG = HI8(HI16(polynomial));
            NoiseGenerator_POLYNOM_LOWER_C__D0_REG = LO8(HI16(polynomial));
            NoiseGenerator_POLYNOM_LOWER_B__D0_REG = HI8(polynomial);
            NoiseGenerator_POLYNOM_LOWER_A__D0_REG = LO8(polynomial);

        #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */

        /* Resets triggers */
        #if (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK)
            if((NoiseGenerator_sleepState & NoiseGenerator_NORMAL_SEQUENCE) != 0u)
            {
                NoiseGenerator_EXECUTE_DFF_RESET;
            }
        #else
            NoiseGenerator_EXECUTE_DFF_RESET;
        #endif  /* (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK) */
    }
#endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */


/* [] END OF FILE */
