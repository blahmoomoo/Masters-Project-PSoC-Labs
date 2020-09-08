/*******************************************************************************
* File Name: NoiseGenerator.h
* Version 2.40
*
* Description:
*  This file provides constants and parameter values for the PRS component.
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

#if !defined(CY_PRS_NoiseGenerator_H)
#define CY_PRS_NoiseGenerator_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"

/***************************************
*   Conditional Compilation Parameters
****************************************/

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */

#if !defined (CY_PSOC5A)
    #error Component PRS_v2_40 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5A) */

#define NoiseGenerator_PRS_SIZE                   (64u)
#define NoiseGenerator_RUN_MODE                   (0u)
#define NoiseGenerator_TIME_MULTIPLEXING_ENABLE   (1u)
#define NoiseGenerator_WAKEUP_BEHAVIOUR           (0u)

#define NoiseGenerator__CLOCKED 0
#define NoiseGenerator__APISINGLESTEP 1


#define NoiseGenerator__RESUMEWORK 1
#define NoiseGenerator__STARTAFRESH 0



/***************************************
*       Type defines
***************************************/

/* Structure to save registers before go to sleep */
typedef struct
{
    uint8 enableState;
    
    #if (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK)
        /* Save dff register for time mult */
        #if (NoiseGenerator_TIME_MULTIPLEXING_ENABLE)
            uint8 dffStatus;
        #endif  /* End NoiseGenerator_TIME_MULTIPLEXING_ENABLE */
    
        /* Save A0 and A1 registers are none-retention */
        #if(NoiseGenerator_PRS_SIZE <= 32u)
            uint32 seed;
            
        #else
            uint32 seedUpper;
            uint32 seedLower;
            
        #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */ 
        
    #endif  /* End (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK) */
    
} NoiseGenerator_BACKUP_STRUCT;

extern uint8 NoiseGenerator_initVar;

extern NoiseGenerator_BACKUP_STRUCT NoiseGenerator_backup;

#if ((NoiseGenerator_TIME_MULTIPLEXING_ENABLE) && (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK))
    extern uint8 NoiseGenerator_sleepState;
#endif  /* End ((NoiseGenerator_TIME_MULTIPLEXING_ENABLE) && 
          (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK)) */

                                     
/***************************************
*        Function Prototypes
****************************************/

void NoiseGenerator_Init(void) ;
void NoiseGenerator_Enable(void) ;
void NoiseGenerator_Start(void) ;
void NoiseGenerator_Stop(void) ;
void NoiseGenerator_SaveConfig(void) ;
void NoiseGenerator_Sleep(void) ;


#if ((NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK) && (NoiseGenerator_TIME_MULTIPLEXING_ENABLE))
    void NoiseGenerator_RestoreConfig(void) ;
    void NoiseGenerator_Wakeup(void) ;
#else
    void NoiseGenerator_RestoreConfig(void) ;
    void NoiseGenerator_Wakeup(void) ;
#endif  /* End ((NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK) && 
                (NoiseGenerator_TIME_MULTIPLEXING_ENABLE)) */

#if (NoiseGenerator_RUN_MODE == NoiseGenerator__APISINGLESTEP)
    void NoiseGenerator_Step(void) ;
#endif  /* End (NoiseGenerator_RUN_MODE == NoiseGenerator__APISINGLESTEP) */

#if (NoiseGenerator_PRS_SIZE <= 32u)    /* 8-32 bits PRS */
    uint32 NoiseGenerator_Read(void) ;
    void NoiseGenerator_WriteSeed(uint32 seed)  ;
    uint32 NoiseGenerator_ReadPolynomial(void) 
                                ;
    void NoiseGenerator_WritePolynomial(uint32 polynomial) 
                                          ;
    
#else                                    /* 33-64 bits PRS */
    uint32 NoiseGenerator_ReadUpper(void) ;
    uint32 NoiseGenerator_ReadLower(void) ;
    void NoiseGenerator_WriteSeedUpper(uint32 seed) ;
    void NoiseGenerator_WriteSeedLower(uint32 seed) ;
    uint32 NoiseGenerator_ReadPolynomialUpper(void) ;
    uint32 NoiseGenerator_ReadPolynomialLower(void) ;
    void NoiseGenerator_WritePolynomialUpper(uint32 polynomial) 
                                                ;
    void NoiseGenerator_WritePolynomialLower(uint32 polynomial) 
                                                ;
    
#endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */

#if (NoiseGenerator_RUN_MODE == NoiseGenerator__CLOCKED)
    #if (NoiseGenerator_PRS_SIZE <= 32u) /* 8-32 bits PRS */
        void NoiseGenerator_ResetSeedInit(uint32 seed)  
                                            ;
    #else
        void NoiseGenerator_ResetSeedInitUpper(uint32 seed) ;
        void NoiseGenerator_ResetSeedInitLower(uint32 seed) ;
    #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */
#endif  /* End (NoiseGenerator_RUN_MODE == NoiseGenerator__CLOCKED) */


/***************************************
*    Initial Parameter Constants
***************************************/

#if (NoiseGenerator_PRS_SIZE <= 32u)
    #define NoiseGenerator_DEFAULT_POLYNOM            (0x0u)
    #define NoiseGenerator_DEFAULT_SEED               (0xFFFFFFFFu)
    
#else
    #define NoiseGenerator_DEFAULT_POLYNOM_UPPER      (0xD8000000u)
    #define NoiseGenerator_DEFAULT_POLYNOM_LOWER      (0x0u)
    #define NoiseGenerator_DEFAULT_SEED_UPPER         (0xFFFFFFFFu)
    #define NoiseGenerator_DEFAULT_SEED_LOWER         (0xFFFFFFFFu)
    
#endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */


/***************************************
*           API Constants
***************************************/

#define NoiseGenerator_MASK                           (0xFFFFFFFFu)


/***************************************
*             Registers
***************************************/


#if (NoiseGenerator_RUN_MODE == NoiseGenerator__CLOCKED)
    #define NoiseGenerator_EXECUTE_DFF_RESET  \
        (NoiseGenerator_CONTROL_REG |= (NoiseGenerator_INIT_STATE | NoiseGenerator_CTRL_RESET_COMMON))
    
    #define NoiseGenerator_EXECUTE_DFF_SET    \
        (NoiseGenerator_CONTROL_REG |= NoiseGenerator_CTRL_RESET_COMMON)

#else
    #define NoiseGenerator_EXECUTE_DFF_RESET  \
        do { \
            NoiseGenerator_CONTROL_REG |= (NoiseGenerator_INIT_STATE | NoiseGenerator_CTRL_RESET_COMMON | \
            NoiseGenerator_CTRL_RISING_EDGE );  \
            NoiseGenerator_CONTROL_REG &= ((uint8)~(NoiseGenerator_CTRL_RESET_COMMON | \
            NoiseGenerator_CTRL_RISING_EDGE));    \
        } while (0)
    
    #define NoiseGenerator_EXECUTE_DFF_SET    \
        do { \
            NoiseGenerator_CONTROL_REG |= (NoiseGenerator_CTRL_RESET_COMMON | \
            NoiseGenerator_CTRL_RISING_EDGE);\
            NoiseGenerator_CONTROL_REG &= ((uint8)~(NoiseGenerator_CTRL_RESET_COMMON | \
            NoiseGenerator_CTRL_RISING_EDGE));    \
        } while (0)

    #define NoiseGenerator_EXECUTE_STEP       \
        do { \
            NoiseGenerator_CONTROL_REG |= NoiseGenerator_CTRL_RISING_EDGE; \
            NoiseGenerator_CONTROL_REG &= ((uint8)~NoiseGenerator_CTRL_RISING_EDGE);    \
        } while (0)
    
#endif  /* End (NoiseGenerator_RUN_MODE == NoiseGenerator__CLOCKED) */

#if (NoiseGenerator_TIME_MULTIPLEXING_ENABLE)
    
    #if (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK)
        #define NoiseGenerator_STATUS                     (*(reg8 *) NoiseGenerator_Sts_StsReg__STATUS_REG )
        #define NoiseGenerator_STATUS_PTR                 ( (reg8 *) NoiseGenerator_Sts_StsReg__STATUS_REG )
    #endif  /* End (NoiseGenerator_WAKEUP_BEHAVIOUR == NoiseGenerator__RESUMEWORK) */
    
    #if (NoiseGenerator_PRS_SIZE <= 16u)      /* 16 bits PRS */
        /* Polynomial */
        #define NoiseGenerator_POLYNOM_A__D1_REG          (*(reg8 *) NoiseGenerator_b0_PRSdp_a__D1_REG )
        #define NoiseGenerator_POLYNOM_A__D1_PTR      	( (reg8 *) NoiseGenerator_b0_PRSdp_a__D1_REG )
        #define NoiseGenerator_POLYNOM_A__D0_REG          (*(reg8 *) NoiseGenerator_b0_PRSdp_a__D0_REG )
        #define NoiseGenerator_POLYNOM_A__D0_PTR      	( (reg8 *) NoiseGenerator_b0_PRSdp_a__D0_REG )
        /* Seed */
        #define NoiseGenerator_SEED_A__A1_REG             (*(reg8 *) NoiseGenerator_b0_PRSdp_a__A1_REG )
        #define NoiseGenerator_SEED_A__A1_PTR         	( (reg8 *) NoiseGenerator_b0_PRSdp_a__A1_REG )
        #define NoiseGenerator_SEED_A__A0_REG             (*(reg8 *) NoiseGenerator_b0_PRSdp_a__A0_REG )
        #define NoiseGenerator_SEED_A__A0_PTR         	( (reg8 *) NoiseGenerator_b0_PRSdp_a__A0_REG )
        /* Seed COPY */
        #define NoiseGenerator_SEED_COPY_A__A1_REG        (*(reg8 *) NoiseGenerator_b0_PRSdp_a__F1_REG )
        #define NoiseGenerator_SEED_COPY_A__A1_PTR    	( (reg8 *) NoiseGenerator_b0_PRSdp_a__F1_REG )
        #define NoiseGenerator_SEED_COPY_A__A0_REG        (*(reg8 *) NoiseGenerator_b0_PRSdp_a__F0_REG )
        #define NoiseGenerator_SEED_COPY_A__A0_PTR    	( (reg8 *) NoiseGenerator_b0_PRSdp_a__F0_REG )
        
    #elif (NoiseGenerator_PRS_SIZE <= 24u)      /* 24 bits PRS */
        /* Polynomial */
        #define NoiseGenerator_POLYNOM_B__D1_REG          (*(reg8 *) NoiseGenerator_b1_PRSdp_b__D1_REG )
        #define NoiseGenerator_POLYNOM_B__D1_PTR      	( (reg8 *) NoiseGenerator_b1_PRSdp_b__D1_REG )
        #define NoiseGenerator_POLYNOM_B__D0_REG          (*(reg8 *) NoiseGenerator_b1_PRSdp_b__D0_REG )
        #define NoiseGenerator_POLYNOM_B__D0_PTR      	( (reg8 *) NoiseGenerator_b1_PRSdp_b__D0_REG )
        #define NoiseGenerator_POLYNOM_A__D0_REG          (*(reg8 *) NoiseGenerator_b0_PRSdp_a__D0_REG )
        #define NoiseGenerator_POLYNOM_A__D0_PTR     	 	( (reg8 *) NoiseGenerator_b0_PRSdp_a__D0_REG )
        /* Seed */
        #define NoiseGenerator_SEED_B__A1_REG             (*(reg8 *) NoiseGenerator_b1_PRSdp_b__A1_REG )
        #define NoiseGenerator_SEED_B__A1_PTR         	( (reg8 *) NoiseGenerator_b1_PRSdp_b__A1_REG )
        #define NoiseGenerator_SEED_B__A0_REG             (*(reg8 *) NoiseGenerator_b1_PRSdp_b__A0_REG )
        #define NoiseGenerator_SEED_B__A0_PTR     	    ( (reg8 *) NoiseGenerator_b1_PRSdp_b__A0_REG )
        #define NoiseGenerator_SEED_A__A0_REG             (*(reg8 *) NoiseGenerator_b0_PRSdp_a__A0_REG )
        #define NoiseGenerator_SEED_A__A0_PTR 	        ( (reg8 *) NoiseGenerator_b0_PRSdp_a__A0_REG )
        /* Seed COPY */
        #define NoiseGenerator_SEED_COPY_B__A1_REG        (*(reg8 *) NoiseGenerator_b1_PRSdp_b__F1_REG )
        #define NoiseGenerator_SEED_COPY_B__A1_PTR 	    ( (reg8 *) NoiseGenerator_b1_PRSdp_b__F1_REG )
        #define NoiseGenerator_SEED_COPY_B__A0_REG        (*(reg8 *) NoiseGenerator_b1_PRSdp_b__F0_REG )
        #define NoiseGenerator_SEED_COPY_B__A0_PTR	    ( (reg8 *) NoiseGenerator_b1_PRSdp_b__F0_REG )
        #define NoiseGenerator_SEED_COPY_A__A0_REG        (*(reg8 *) NoiseGenerator_b0_PRSdp_a__F0_REG )
        #define NoiseGenerator_SEED_COPY_A__A0_PTR    	( (reg8 *) NoiseGenerator_b0_PRSdp_a__F0_REG )
        
    #elif (NoiseGenerator_PRS_SIZE <= 32u)      /* 32 bits PRS */
        /* Polynomial */
        #define NoiseGenerator_POLYNOM_B__D1_REG          (*(reg8 *) NoiseGenerator_b1_PRSdp_b__D1_REG )
        #define NoiseGenerator_POLYNOM_B__D1_PTR    		( (reg8 *) NoiseGenerator_b1_PRSdp_b__D1_REG )
        #define NoiseGenerator_POLYNOM_A__D1_REG          (*(reg8 *) NoiseGenerator_b0_PRSdp_a__D1_REG )
        #define NoiseGenerator_POLYNOM_A__D1_PTR     		( (reg8 *) NoiseGenerator_b0_PRSdp_a__D1_REG )
        #define NoiseGenerator_POLYNOM_B__D0_REG          (*(reg8 *) NoiseGenerator_b1_PRSdp_b__D0_REG )
        #define NoiseGenerator_POLYNOM_B__D0_PTR		    ( (reg8 *) NoiseGenerator_b1_PRSdp_b__D0_REG )
        #define NoiseGenerator_POLYNOM_A__D0_REG          (*(reg8 *) NoiseGenerator_b0_PRSdp_a__D0_REG )
        #define NoiseGenerator_POLYNOM_A__D0_PTR  	    ( (reg8 *) NoiseGenerator_b0_PRSdp_a__D0_REG )
        /* Seed */
        #define NoiseGenerator_SEED_B__A1_REG             (*(reg8 *) NoiseGenerator_b1_PRSdp_b__A1_REG )
        #define NoiseGenerator_SEED_B__A1_PTR    		    ( (reg8 *) NoiseGenerator_b1_PRSdp_b__A1_REG )
        #define NoiseGenerator_SEED_A__A1_REG         	(*(reg8 *) NoiseGenerator_b0_PRSdp_a__A1_REG )
        #define NoiseGenerator_SEED_A__A1_PTR      	    ( (reg8 *) NoiseGenerator_b0_PRSdp_a__A1_REG )
        #define NoiseGenerator_SEED_B__A0_REG             (*(reg8 *) NoiseGenerator_b1_PRSdp_b__A0_REG )
        #define NoiseGenerator_SEED_B__A0_PTR     	    ( (reg8 *) NoiseGenerator_b1_PRSdp_b__A0_REG )
        #define NoiseGenerator_SEED_A__A0_REG             (*(reg8 *) NoiseGenerator_b0_PRSdp_a__A0_REG )
        #define NoiseGenerator_SEED_A__A0_PTR     	    ( (reg8 *) NoiseGenerator_b0_PRSdp_a__A0_REG )
        /* Seed COPY */
        #define NoiseGenerator_SEED_COPY_B__A1_REG        (*(reg8 *) NoiseGenerator_b1_PRSdp_b__F1_REG )
        #define NoiseGenerator_SEED_COPY_B__A1_PTR  		( (reg8 *) NoiseGenerator_b1_PRSdp_b__F1_REG )
        #define NoiseGenerator_SEED_COPY_A__A1_REG        (*(reg8 *) NoiseGenerator_b0_PRSdp_a__F1_REG )
        #define NoiseGenerator_SEED_COPY_A__A1_PTR   	 	( (reg8 *) NoiseGenerator_b0_PRSdp_a__F1_REG )
        #define NoiseGenerator_SEED_COPY_B__A0_REG        (*(reg8 *) NoiseGenerator_b1_PRSdp_b__F0_REG )
        #define NoiseGenerator_SEED_COPY_B__A0_PTR  	  	( (reg8 *) NoiseGenerator_b1_PRSdp_b__F0_REG )
        #define NoiseGenerator_SEED_COPY_A__A0_REG        (*(reg8 *) NoiseGenerator_b0_PRSdp_a__F0_REG )
        #define NoiseGenerator_SEED_COPY_A__A0_PTR   	 	( (reg8 *) NoiseGenerator_b0_PRSdp_a__F0_REG )
        
    #elif (NoiseGenerator_PRS_SIZE <= 40u)      /* 40 bits PRS */
        /* Polynomial Upper */
        #define NoiseGenerator_POLYNOM_UPPER_C__D1_REG            (*(reg8 *) NoiseGenerator_b2_PRSdp_c__D1_REG )
        #define NoiseGenerator_POLYNOM_UPPER_C__D1_PTR        	( (reg8 *) NoiseGenerator_b2_PRSdp_c__D1_REG )
        /* Polynomial Lower */
        #define NoiseGenerator_POLYNOM_LOWER_B__D1_REG            (*(reg8 *) NoiseGenerator_b1_PRSdp_b__D1_REG )
        #define NoiseGenerator_POLYNOM_LOWER_B__D1_PTR        	( (reg8 *) NoiseGenerator_b1_PRSdp_b__D1_REG )
        #define NoiseGenerator_POLYNOM_LOWER_C__D0_REG            (*(reg8 *) NoiseGenerator_b2_PRSdp_c__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_C__D0_PTR        	( (reg8 *) NoiseGenerator_b2_PRSdp_c__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_B__D0_REG            (*(reg8 *) NoiseGenerator_b1_PRSdp_b__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_B__D0_PTR        	( (reg8 *) NoiseGenerator_b1_PRSdp_b__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_A__D0_REG            (*(reg8 *) NoiseGenerator_b0_PRSdp_a__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_A__D0_PTR        	( (reg8 *) NoiseGenerator_b0_PRSdp_a__D0_REG )
        /* Seed Upper */
        #define NoiseGenerator_SEED_UPPER_C__A1_REG               (*(reg8 *) NoiseGenerator_b2_PRSdp_c__A1_REG )
        #define NoiseGenerator_SEED_UPPER_C__A1_PTR           	( (reg8 *) NoiseGenerator_b2_PRSdp_c__A1_REG )
        /* Seed Lower */
        #define NoiseGenerator_SEED_LOWER_B__A1_REG               (*(reg8 *) NoiseGenerator_b1_PRSdp_b__A1_REG )
        #define NoiseGenerator_SEED_LOWER_B__A1_PTR           	( (reg8 *) NoiseGenerator_b1_PRSdp_b__A1_REG )
        #define NoiseGenerator_SEED_LOWER_C__A0_REG               (*(reg8 *) NoiseGenerator_b2_PRSdp_c__A0_REG )
        #define NoiseGenerator_SEED_LOWER_C__A0_PTR           	( (reg8 *) NoiseGenerator_b2_PRSdp_c__A0_REG )
        #define NoiseGenerator_SEED_LOWER_B__A0_REG               (*(reg8 *) NoiseGenerator_b1_PRSdp_b__A0_REG )
        #define NoiseGenerator_SEED_LOWER_B__A0_PTR           	( (reg8 *) NoiseGenerator_b1_PRSdp_b__A0_REG )
        #define NoiseGenerator_SEED_LOWER_A__A0_REG               (*(reg8 *) NoiseGenerator_b0_PRSdp_a__A0_REG )
        #define NoiseGenerator_SEED_LOWER_A__A0_PTR           	( (reg8 *) NoiseGenerator_b0_PRSdp_a__A0_REG )
        /* Seed COPY Upper */
        #define NoiseGenerator_SEED_UPPER_COPY_C__A1_REG          (*(reg8 *) NoiseGenerator_b2_PRSdp_c__F1_REG )
        #define NoiseGenerator_SEED_UPPER_COPY_C__A1_PTR      	( (reg8 *) NoiseGenerator_b2_PRSdp_c__F1_REG )
        /* Seed COPY Lower */
        #define NoiseGenerator_SEED_LOWER_COPY_B__A1_REG          (*(reg8 *) NoiseGenerator_b1_PRSdp_b__F1_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_B__A1_PTR      	( (reg8 *) NoiseGenerator_b1_PRSdp_b__F1_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_C__A0_REG          (*(reg8 *) NoiseGenerator_b2_PRSdp_c__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_C__A0_PTR      	( (reg8 *) NoiseGenerator_b2_PRSdp_c__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_B__A0_REG          (*(reg8 *) NoiseGenerator_b1_PRSdp_b__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_B__A0_PTR      	( (reg8 *) NoiseGenerator_b1_PRSdp_b__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_A__A0_REG          (*(reg8 *) NoiseGenerator_b0_PRSdp_a__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_A__A0_PTR      	( (reg8 *) NoiseGenerator_b0_PRSdp_a__F0_REG )
        
    #elif (NoiseGenerator_PRS_SIZE <= 48u)      /* 48 bits PRS */
        /* Polynomial Upper */
        #define NoiseGenerator_POLYNOM_UPPER_C__D1_REG            (*(reg8 *) NoiseGenerator_b2_PRSdp_c__D1_REG )
        #define NoiseGenerator_POLYNOM_UPPER_C__D1_PTR        	( (reg8 *) NoiseGenerator_b2_PRSdp_c__D1_REG )
        #define NoiseGenerator_POLYNOM_UPPER_B__D1_REG            (*(reg8 *) NoiseGenerator_b1_PRSdp_b__D1_REG )
        #define NoiseGenerator_POLYNOM_UPPER_B__D1_PTR        	( (reg8 *) NoiseGenerator_b1_PRSdp_b__D1_REG )
        /* Polynomial Lower */
        #define NoiseGenerator_POLYNOM_LOWER_A__D1_REG            (*(reg8 *) NoiseGenerator_b0_PRSdp_a__D1_REG )
        #define NoiseGenerator_POLYNOM_LOWER_A__D1_PTR        	( (reg8 *) NoiseGenerator_b0_PRSdp_a__D1_REG )
        #define NoiseGenerator_POLYNOM_LOWER_C__D0_REG            (*(reg8 *) NoiseGenerator_b2_PRSdp_c__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_C__D0_PTR        	( (reg8 *) NoiseGenerator_b2_PRSdp_c__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_B__D0_REG            (*(reg8 *) NoiseGenerator_b1_PRSdp_b__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_B__D0_PTR        	( (reg8 *) NoiseGenerator_b1_PRSdp_b__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_A__D0_REG            (*(reg8 *) NoiseGenerator_b0_PRSdp_a__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_A__D0_PTR        	( (reg8 *) NoiseGenerator_b0_PRSdp_a__D0_REG )
        /* Seed Upper */
        #define NoiseGenerator_SEED_UPPER_C__A1_REG               (*(reg8 *) NoiseGenerator_b2_PRSdp_c__A1_REG )
        #define NoiseGenerator_SEED_UPPER_C__A1_PTR           	( (reg8 *) NoiseGenerator_b2_PRSdp_c__A1_REG )
        #define NoiseGenerator_SEED_UPPER_B__A1_REG               (*(reg8 *) NoiseGenerator_b1_PRSdp_b__A1_REG )
        #define NoiseGenerator_SEED_UPPER_B__A1_PTR           	( (reg8 *) NoiseGenerator_b1_PRSdp_b__A1_REG )
        /* Seed Lower */
        #define NoiseGenerator_SEED_LOWER_A__A1_REG               (*(reg8 *) NoiseGenerator_b0_PRSdp_a__A1_REG )
        #define NoiseGenerator_SEED_LOWER_A__A1_PTR           	( (reg8 *) NoiseGenerator_b0_PRSdp_a__A1_REG )
        #define NoiseGenerator_SEED_LOWER_C__A0_REG               (*(reg8 *) NoiseGenerator_b2_PRSdp_c__A0_REG )
        #define NoiseGenerator_SEED_LOWER_C__A0_PTR           	( (reg8 *) NoiseGenerator_b2_PRSdp_c__A0_REG )
        #define NoiseGenerator_SEED_LOWER_B__A0_REG               (*(reg8 *) NoiseGenerator_b1_PRSdp_b__A0_REG )
        #define NoiseGenerator_SEED_LOWER_B__A0_PTR           	( (reg8 *) NoiseGenerator_b1_PRSdp_b__A0_REG )
        #define NoiseGenerator_SEED_LOWER_A__A0_REG               (*(reg8 *) NoiseGenerator_b0_PRSdp_a__A0_REG )
        #define NoiseGenerator_SEED_LOWER_A__A0_PTR           	( (reg8 *) NoiseGenerator_b0_PRSdp_a__A0_REG )
        /* Seed COPY Upper */
        #define NoiseGenerator_SEED_UPPER_COPY_C__A1_REG          (*(reg8 *) NoiseGenerator_b2_PRSdp_c__F1_REG )
        #define NoiseGenerator_SEED_UPPER_COPY_C__A1_PTR      	( (reg8 *) NoiseGenerator_b2_PRSdp_c__F1_REG )
        #define NoiseGenerator_SEED_UPPER_COPY_B__A1_REG          (*(reg8 *) NoiseGenerator_b1_PRSdp_b__F1_REG )
        #define NoiseGenerator_SEED_UPPER_COPY_B__A1_PTR      	( (reg8 *) NoiseGenerator_b1_PRSdp_b__F1_REG )
        /* Seed COPY Lower */
        #define NoiseGenerator_SEED_LOWER_COPY_A__A1_REG          (*(reg8 *) NoiseGenerator_b0_PRSdp_a__F1_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_A__A1_PTR      	( (reg8 *) NoiseGenerator_b0_PRSdp_a__F1_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_C__A0_REG          (*(reg8 *) NoiseGenerator_b2_PRSdp_c__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_C__A0_PTR      	( (reg8 *) NoiseGenerator_b2_PRSdp_c__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_B__A0_REG          (*(reg8 *) NoiseGenerator_b1_PRSdp_b__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_B__A0_PTR      	( (reg8 *) NoiseGenerator_b1_PRSdp_b__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_A__A0_REG          (*(reg8 *) NoiseGenerator_b0_PRSdp_a__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_A__A0_PTR      	( (reg8 *) NoiseGenerator_b0_PRSdp_a__F0_REG )
        
    #elif (NoiseGenerator_PRS_SIZE <= 56u)      /* 56 bits PRS */
        /* Polynom Upper */
        #define NoiseGenerator_POLYNOM_UPPER_D__D1_REG            (*(reg8 *) NoiseGenerator_b3_PRSdp_d__D1_REG )
        #define NoiseGenerator_POLYNOM_UPPER_D__D1_PTR        	( (reg8 *) NoiseGenerator_b3_PRSdp_d__D1_REG )
        #define NoiseGenerator_POLYNOM_UPPER_C__D1_REG            (*(reg8 *) NoiseGenerator_b2_PRSdp_c__D1_REG )
        #define NoiseGenerator_POLYNOM_UPPER_C__D1_PTR        	( (reg8 *) NoiseGenerator_b2_PRSdp_c__D1_REG )
        #define NoiseGenerator_POLYNOM_UPPER_B__D1_REG            (*(reg8 *) NoiseGenerator_b1_PRSdp_b__D1_REG )
        #define NoiseGenerator_POLYNOM_UPPER_B__D1_PTR        	( (reg8 *) NoiseGenerator_b1_PRSdp_b__D1_REG )
        /* Polynom Lower */
        #define NoiseGenerator_POLYNOM_LOWER_D__D0_REG            (*(reg8 *) NoiseGenerator_b3_PRSdp_d__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_D__D0_PTR        	( (reg8 *) NoiseGenerator_b3_PRSdp_d__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_C__D0_REG            (*(reg8 *) NoiseGenerator_b2_PRSdp_c__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_C__D0_PTR        	( (reg8 *) NoiseGenerator_b2_PRSdp_c__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_B__D0_REG            (*(reg8 *) NoiseGenerator_b1_PRSdp_b__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_B__D0_PTR        	( (reg8 *) NoiseGenerator_b1_PRSdp_b__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_A__D0_REG            (*(reg8 *) NoiseGenerator_b0_PRSdp_a__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_A__D0_PTR        	( (reg8 *) NoiseGenerator_b0_PRSdp_a__D0_REG )
        /* Seed Upper */
        #define NoiseGenerator_SEED_UPPER_D__A1_REG               (*(reg8 *) NoiseGenerator_b3_PRSdp_d__A1_REG )
        #define NoiseGenerator_SEED_UPPER_D__A1_PTR           	( (reg8 *) NoiseGenerator_b3_PRSdp_d__A1_REG )
        #define NoiseGenerator_SEED_UPPER_C__A1_REG               (*(reg8 *) NoiseGenerator_b2_PRSdp_c__A1_REG )
        #define NoiseGenerator_SEED_UPPER_C__A1_PTR           	( (reg8 *) NoiseGenerator_b2_PRSdp_c__A1_REG )
        #define NoiseGenerator_SEED_UPPER_B__A1_REG               (*(reg8 *) NoiseGenerator_b1_PRSdp_b__A1_REG )
        #define NoiseGenerator_SEED_UPPER_B__A1_PTR           	( (reg8 *) NoiseGenerator_b1_PRSdp_b__A1_REG )
        /* Seed Lower */
        #define NoiseGenerator_SEED_LOWER_D__A0_REG               (*(reg8 *) NoiseGenerator_b3_PRSdp_d__A0_REG )
        #define NoiseGenerator_SEED_LOWER_D__A0_PTR           	( (reg8 *) NoiseGenerator_b3_PRSdp_d__A0_REG )
        #define NoiseGenerator_SEED_LOWER_C__A0_REG               (*(reg8 *) NoiseGenerator_b2_PRSdp_c__A0_REG )
        #define NoiseGenerator_SEED_LOWER_C__A0_PTR           	( (reg8 *) NoiseGenerator_b2_PRSdp_c__A0_REG )
        #define NoiseGenerator_SEED_LOWER_B__A0_REG               (*(reg8 *) NoiseGenerator_b1_PRSdp_b__A0_REG )
        #define NoiseGenerator_SEED_LOWER_B__A0_PTR           	( (reg8 *) NoiseGenerator_b1_PRSdp_b__A0_REG )
        #define NoiseGenerator_SEED_LOWER_A__A0_REG               (*(reg8 *) NoiseGenerator_b0_PRSdp_a__A0_REG )
        #define NoiseGenerator_SEED_LOWER_A__A0_PTR           	( (reg8 *) NoiseGenerator_b0_PRSdp_a__A0_REG )
        /* COPY Seed Upper */
        #define NoiseGenerator_SEED_UPPER_COPY_D__A1_REG          (*(reg8 *) NoiseGenerator_b3_PRSdp_d__F1_REG )
        #define NoiseGenerator_SEED_UPPER_COPY_D__A1_PTR      	( (reg8 *) NoiseGenerator_b3_PRSdp_d__F1_REG )
        #define NoiseGenerator_SEED_UPPER_COPY_C__A1_REG          (*(reg8 *) NoiseGenerator_b2_PRSdp_c__F1_REG )
        #define NoiseGenerator_SEED_UPPER_COPY_C__A1_PTR      	( (reg8 *) NoiseGenerator_b2_PRSdp_c__F1_REG )
        #define NoiseGenerator_SEED_UPPER_COPY_B__A1_REG          (*(reg8 *) NoiseGenerator_b1_PRSdp_b__F1_REG )
        #define NoiseGenerator_SEED_UPPER_COPY_B__A1_PTR      	( (reg8 *) NoiseGenerator_b1_PRSdp_b__F1_REG )
        /* COPY Seed Lower */
        #define NoiseGenerator_SEED_LOWER_COPY_D__A0_REG          (*(reg8 *) NoiseGenerator_b3_PRSdp_d__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_D__A0_PTR      	( (reg8 *) NoiseGenerator_b3_PRSdp_d__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_C__A0_REG          (*(reg8 *) NoiseGenerator_b2_PRSdp_c__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_C__A0_PTR      	( (reg8 *) NoiseGenerator_b2_PRSdp_c__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_B__A0_REG          (*(reg8 *) NoiseGenerator_b1_PRSdp_b__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_B__A0_PTR      	( (reg8 *) NoiseGenerator_b1_PRSdp_b__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_A__A0_REG          (*(reg8 *) NoiseGenerator_b0_PRSdp_a__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_A__A0_PTR      	( (reg8 *) NoiseGenerator_b0_PRSdp_a__F0_REG )
        
    #else                                        /* 64 bits PRS */
        /* Polynom Upper */
        #define NoiseGenerator_POLYNOM_UPPER_D__D1_REG            (*(reg8 *) NoiseGenerator_b3_PRSdp_d__D1_REG )
        #define NoiseGenerator_POLYNOM_UPPER_D__D1_PTR        	( (reg8 *) NoiseGenerator_b3_PRSdp_d__D1_REG )
        #define NoiseGenerator_POLYNOM_UPPER_C__D1_REG            (*(reg8 *) NoiseGenerator_b2_PRSdp_c__D1_REG )
        #define NoiseGenerator_POLYNOM_UPPER_C__D1_PTR        	( (reg8 *) NoiseGenerator_b2_PRSdp_c__D1_REG )
        #define NoiseGenerator_POLYNOM_UPPER_B__D1_REG            (*(reg8 *) NoiseGenerator_b1_PRSdp_b__D1_REG )
        #define NoiseGenerator_POLYNOM_UPPER_B__D1_PTR        	( (reg8 *) NoiseGenerator_b1_PRSdp_b__D1_REG )
        #define NoiseGenerator_POLYNOM_UPPER_A__D1_REG            (*(reg8 *) NoiseGenerator_b0_PRSdp_a__D1_REG )
        #define NoiseGenerator_POLYNOM_UPPER_A__D1_PTR        	( (reg8 *) NoiseGenerator_b0_PRSdp_a__D1_REG )
        /* Polynom Lower */
        #define NoiseGenerator_POLYNOM_LOWER_D__D0_REG            (*(reg8 *) NoiseGenerator_b3_PRSdp_d__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_D__D0_PTR        	( (reg8 *) NoiseGenerator_b3_PRSdp_d__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_C__D0_REG            (*(reg8 *) NoiseGenerator_b2_PRSdp_c__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_C__D0_PTR        	( (reg8 *) NoiseGenerator_b2_PRSdp_c__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_B__D0_REG            (*(reg8 *) NoiseGenerator_b1_PRSdp_b__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_B__D0_PTR        	( (reg8 *) NoiseGenerator_b1_PRSdp_b__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_A__D0_REG            (*(reg8 *) NoiseGenerator_b0_PRSdp_a__D0_REG )
        #define NoiseGenerator_POLYNOM_LOWER_A__D0_PTR        	( (reg8 *) NoiseGenerator_b0_PRSdp_a__D0_REG )
        /* Seed Upper */
        #define NoiseGenerator_SEED_UPPER_D__A1_REG               (*(reg8 *) NoiseGenerator_b3_PRSdp_d__A1_REG )
        #define NoiseGenerator_SEED_UPPER_D__A1_PTR           	( (reg8 *) NoiseGenerator_b3_PRSdp_d__A1_REG )
        #define NoiseGenerator_SEED_UPPER_C__A1_REG               (*(reg8 *) NoiseGenerator_b2_PRSdp_c__A1_REG )
        #define NoiseGenerator_SEED_UPPER_C__A1_PTR           	( (reg8 *) NoiseGenerator_b2_PRSdp_c__A1_REG )
        #define NoiseGenerator_SEED_UPPER_B__A1_REG               (*(reg8 *) NoiseGenerator_b1_PRSdp_b__A1_REG )
        #define NoiseGenerator_SEED_UPPER_B__A1_PTR           	( (reg8 *) NoiseGenerator_b1_PRSdp_b__A1_REG )
        #define NoiseGenerator_SEED_UPPER_A__A1_REG               (*(reg8 *) NoiseGenerator_b0_PRSdp_a__A1_REG )
        #define NoiseGenerator_SEED_UPPER_A__A1_PTR           	( (reg8 *) NoiseGenerator_b0_PRSdp_a__A1_REG )
        /* Seed Lower */
        #define NoiseGenerator_SEED_LOWER_D__A0_REG               (*(reg8 *) NoiseGenerator_b3_PRSdp_d__A0_REG )
        #define NoiseGenerator_SEED_LOWER_D__A0_PTR           	( (reg8 *) NoiseGenerator_b3_PRSdp_d__A0_REG )
        #define NoiseGenerator_SEED_LOWER_C__A0_REG               (*(reg8 *) NoiseGenerator_b2_PRSdp_c__A0_REG )
        #define NoiseGenerator_SEED_LOWER_C__A0_PTR           	( (reg8 *) NoiseGenerator_b2_PRSdp_c__A0_REG )
        #define NoiseGenerator_SEED_LOWER_B__A0_REG               (*(reg8 *) NoiseGenerator_b1_PRSdp_b__A0_REG )
        #define NoiseGenerator_SEED_LOWER_B__A0_PTR           	( (reg8 *) NoiseGenerator_b1_PRSdp_b__A0_REG )
        #define NoiseGenerator_SEED_LOWER_A__A0_REG               (*(reg8 *) NoiseGenerator_b0_PRSdp_a__A0_REG )
        #define NoiseGenerator_SEED_LOWER_A__A0_PTR           	( (reg8 *) NoiseGenerator_b0_PRSdp_a__A0_REG )
        /* COPY Seed Upper */
        #define NoiseGenerator_SEED_UPPER_COPY_D__A1_REG          (*(reg8 *) NoiseGenerator_b3_PRSdp_d__F1_REG )
        #define NoiseGenerator_SEED_UPPER_COPY_D__A1_PTR      	( (reg8 *) NoiseGenerator_b3_PRSdp_d__F1_REG )
        #define NoiseGenerator_SEED_UPPER_COPY_C__A1_REG          (*(reg8 *) NoiseGenerator_b2_PRSdp_c__F1_REG )
        #define NoiseGenerator_SEED_UPPER_COPY_C__A1_PTR      	( (reg8 *) NoiseGenerator_b2_PRSdp_c__F1_REG )
        #define NoiseGenerator_SEED_UPPER_COPY_B__A1_REG          (*(reg8 *) NoiseGenerator_b1_PRSdp_b__F1_REG )
        #define NoiseGenerator_SEED_UPPER_COPY_B__A1_PTR      	( (reg8 *) NoiseGenerator_b1_PRSdp_b__F1_REG )
        #define NoiseGenerator_SEED_UPPER_COPY_A__A1_REG          (*(reg8 *) NoiseGenerator_b0_PRSdp_a__F1_REG )
        #define NoiseGenerator_SEED_UPPER_COPY_A__A1_PTR      	( (reg8 *) NoiseGenerator_b0_PRSdp_a__F1_REG )
        /* COPY Seed Lower */
        #define NoiseGenerator_SEED_LOWER_COPY_D__A0_REG          (*(reg8 *) NoiseGenerator_b3_PRSdp_d__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_D__A0_PTR      	( (reg8 *) NoiseGenerator_b3_PRSdp_d__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_C__A0_REG          (*(reg8 *) NoiseGenerator_b2_PRSdp_c__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_C__A0_PTR    		( (reg8 *) NoiseGenerator_b2_PRSdp_c__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_B__A0_REG          (*(reg8 *) NoiseGenerator_b1_PRSdp_b__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_B__A0_PTR      	( (reg8 *) NoiseGenerator_b1_PRSdp_b__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_A__A0_REG          (*(reg8 *) NoiseGenerator_b0_PRSdp_a__F0_REG )
        #define NoiseGenerator_SEED_LOWER_COPY_A__A0_PTR      	( (reg8 *) NoiseGenerator_b0_PRSdp_a__F0_REG )
    #endif  /* End (NoiseGenerator_PRS_SIZE <= 8u) */

#else
    #if (CY_PSOC3 || CY_PSOC5)
        #if (NoiseGenerator_PRS_SIZE <= 32u)         /* 8-32 bits PRS */
            /* Polynomial */
            #define NoiseGenerator_POLYNOM_PTR      ( (reg32 *)\
                                                NoiseGenerator_sC32_PRSdp_u0__D0_REG )
            /* Seed */
            #define NoiseGenerator_SEED_PTR         ( (reg32 *)\
                                                NoiseGenerator_sC32_PRSdp_u0__A0_REG )
            /* Seed COPY */
            #define NoiseGenerator_SEED_COPY_PTR    ( (reg32 *)\
                                                NoiseGenerator_sC32_PRSdp_u0__F0_REG )
            
        #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */
    #else /* PSoC4 */
        #if (NoiseGenerator_PRS_SIZE <= 8u)         /* 8 bits PRS */
            /* Polynomial */
            #define NoiseGenerator_POLYNOM_PTR      ( (reg32 *)\
                                                NoiseGenerator_sC32_PRSdp_u0__D0_REG )
            /* Seed */
            #define NoiseGenerator_SEED_PTR         ( (reg32 *)\
                                                NoiseGenerator_sC32_PRSdp_u0__A0_REG )
            /* Seed COPY */
            #define NoiseGenerator_SEED_COPY_PTR    ( (reg32 *)\
                                                NoiseGenerator_sC32_PRSdp_u0__F0_REG )
            
        #elif (NoiseGenerator_PRS_SIZE <= 16u)         /* 16 bits PRS */
            /* Polynomial */
            #define NoiseGenerator_POLYNOM_PTR      ( (reg32 *)\
                                            NoiseGenerator_sC32_PRSdp_u0__16BIT_D0_REG )
            /* Seed */
            #define NoiseGenerator_SEED_PTR         ( (reg32 *)\
                                            NoiseGenerator_sC32_PRSdp_u0__16BIT_A0_REG )
            /* Seed COPY */
            #define NoiseGenerator_SEED_COPY_PTR    ( (reg32 *)\
                                            NoiseGenerator_sC32_PRSdp_u0__16BIT_F0_REG )

        #elif (NoiseGenerator_PRS_SIZE <= 24u)         /* 24 bits PRS */
            /* Polynomial */
            #define NoiseGenerator_POLYNOM_PTR      ( (reg32 *)\
                                                NoiseGenerator_sC32_PRSdp_u0__D0_REG )
            /* Seed */
            #define NoiseGenerator_SEED_PTR         ( (reg32 *)\
                                                NoiseGenerator_sC32_PRSdp_u0__A0_REG )
            /* Seed COPY */
            #define NoiseGenerator_SEED_COPY_PTR    ( (reg32 *)\
                                                NoiseGenerator_sC32_PRSdp_u0__F0_REG )
         
         #else                                          /* 32 bits PRS */
            /* Polynomial */
            #define NoiseGenerator_POLYNOM_PTR      ( (reg32 *)\
                                            NoiseGenerator_sC32_PRSdp_u0__32BIT_D0_REG )
            /* Seed */
            #define NoiseGenerator_SEED_PTR         ( (reg32 *)\
                                            NoiseGenerator_sC32_PRSdp_u0__32BIT_A0_REG )
            /* Seed COPY */
            #define NoiseGenerator_SEED_COPY_PTR    ( (reg32 *)\
                                            NoiseGenerator_sC32_PRSdp_u0__32BIT_F0_REG )
         
        #endif  /* End (NoiseGenerator_PRS_SIZE <= 32u) */

    #endif  /* End (CY_PSOC3 || CY_PSOC5) */
    
#endif  /* End (NoiseGenerator_TIME_MULTIPLEXING_ENABLE) */


#if (NoiseGenerator_RUN_MODE == NoiseGenerator__CLOCKED)
    /* Aux control */
    #if (NoiseGenerator_TIME_MULTIPLEXING_ENABLE)    
        #if (NoiseGenerator_PRS_SIZE <= 16u)      /* 8-16 bits PRS */
            #define NoiseGenerator_AUX_CONTROL_A_REG      (*(reg8 *) NoiseGenerator_b0_PRSdp_a__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_A_PTR      ( (reg8 *) NoiseGenerator_b0_PRSdp_a__DP_AUX_CTL_REG )
            
        #elif (NoiseGenerator_PRS_SIZE <= 24u)      /* 24-39 bits PRS */
            #define NoiseGenerator_AUX_CONTROL_A_REG      (*(reg8 *) NoiseGenerator_b0_PRSdp_a__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_A_PTR      ( (reg8 *) NoiseGenerator_b0_PRSdp_a__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_B_REG      (*(reg8 *) NoiseGenerator_b1_PRSdp_b__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_B_PTR      ( (reg8 *) NoiseGenerator_b1_PRSdp_b__DP_AUX_CTL_REG )
            
        #elif (NoiseGenerator_PRS_SIZE <= 40u)      /* 40-55 bits PRS */
            #define NoiseGenerator_AUX_CONTROL_A_REG      (*(reg8 *) NoiseGenerator_b0_PRSdp_a__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_A_PTR      ( (reg8 *) NoiseGenerator_b0_PRSdp_a__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_B_REG      (*(reg8 *) NoiseGenerator_b1_PRSdp_b__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_B_PTR      ( (reg8 *) NoiseGenerator_b1_PRSdp_b__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_C_REG      (*(reg8 *) NoiseGenerator_b2_PRSdp_c__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_C_PTR      ( (reg8 *) NoiseGenerator_b2_PRSdp_c__DP_AUX_CTL_REG )

        #else                                         /* 56-64 bits PRS */
            #define NoiseGenerator_AUX_CONTROL_A_REG      (*(reg8 *) NoiseGenerator_b0_PRSdp_a__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_A_PTR      ( (reg8 *) NoiseGenerator_b0_PRSdp_a__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_B_REG      (*(reg8 *) NoiseGenerator_b1_PRSdp_b__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_B_PTR      ( (reg8 *) NoiseGenerator_b1_PRSdp_b__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_C_REG      (*(reg8 *) NoiseGenerator_b2_PRSdp_c__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_C_PTR      ( (reg8 *) NoiseGenerator_b2_PRSdp_c__DP_AUX_CTL_REG )
			#define NoiseGenerator_AUX_CONTROL_D_REG      (*(reg8 *) NoiseGenerator_b3_PRSdp_d__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_D_PTR      ( (reg8 *) NoiseGenerator_b3_PRSdp_d__DP_AUX_CTL_REG )     
																							
        #endif  /* End (NoiseGenerator_PRS_SIZE <= 8u) */
    
    #else
        #if (NoiseGenerator_PRS_SIZE <= 8u)      /* 8 bits PRS */
            #define NoiseGenerator_AUX_CONTROL_A_REG      (*(reg8 *) NoiseGenerator_sC8_PRSdp_u0__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_A_PTR      ( (reg8 *) NoiseGenerator_sC8_PRSdp_u0__DP_AUX_CTL_REG )
            
        #elif (NoiseGenerator_PRS_SIZE <= 16u)      /* 16 bits PRS */
            #define NoiseGenerator_AUX_CONTROL_A_REG      (*(reg8 *) NoiseGenerator_sC16_PRSdp_u0__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_A_PTR      ( (reg8 *) NoiseGenerator_sC16_PRSdp_u0__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_B_REG      (*(reg8 *) NoiseGenerator_sC16_PRSdp_u1__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_B_PTR      ( (reg8 *) NoiseGenerator_sC16_PRSdp_u1__DP_AUX_CTL_REG )
            
        #elif (NoiseGenerator_PRS_SIZE <= 24u)      /* 24-39 bits PRS */
            #define NoiseGenerator_AUX_CONTROL_A_REG      (*(reg8 *) NoiseGenerator_sC24_PRSdp_u0__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_A_PTR      ( (reg8 *) NoiseGenerator_sC24_PRSdp_u0__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_B_REG      (*(reg8 *) NoiseGenerator_sC24_PRSdp_u1__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_B_PTR      ( (reg8 *) NoiseGenerator_sC24_PRSdp_u1__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_C_REG      (*(reg8 *) NoiseGenerator_sC24_PRSdp_u2__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_C_PTR      ( (reg8 *) NoiseGenerator_sC24_PRSdp_u2__DP_AUX_CTL_REG )
                
        #elif (NoiseGenerator_PRS_SIZE <= 32u)      /* 40-55 bits PRS */
            #define NoiseGenerator_AUX_CONTROL_A_REG      (*(reg8 *) NoiseGenerator_sC32_PRSdp_u0__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_A_PTR      ( (reg8 *) NoiseGenerator_sC32_PRSdp_u0__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_B_REG      (*(reg8 *) NoiseGenerator_sC32_PRSdp_u1__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_B_PTR      ( (reg8 *) NoiseGenerator_sC32_PRSdp_u1__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_C_REG      (*(reg8 *) NoiseGenerator_sC32_PRSdp_u2__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_C_PTR      ( (reg8 *) NoiseGenerator_sC32_PRSdp_u2__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_D_REG      (*(reg8 *) NoiseGenerator_sC32_PRSdp_u3__DP_AUX_CTL_REG )
            #define NoiseGenerator_AUX_CONTROL_D_PTR      ( (reg8 *) NoiseGenerator_sC32_PRSdp_u3__DP_AUX_CTL_REG )
    
        #else                                         /* 56-64 bits PRS */
            /* Not supported */                    
        #endif  /* End (NoiseGenerator_PRS_SIZE <= 8u) */
    
    #endif  /* End (NoiseGenerator_TIME_MULTIPLEXING_ENABLE) */
    
#endif  /* End (NoiseGenerator_RUN_MODE == NoiseGenerator__CLOCKED) */

#define NoiseGenerator_CONTROL_REG               (*(reg8 *) NoiseGenerator_ClkSp_CtrlReg__CONTROL_REG )
#define NoiseGenerator_CONTROL_PTR               ( (reg8 *) NoiseGenerator_ClkSp_CtrlReg__CONTROL_REG )
    

/***************************************
*       Register Constants
***************************************/

/* Control register definitions */

#define NoiseGenerator_CTRL_ENABLE                    (0x01u)
#define NoiseGenerator_CTRL_RISING_EDGE               (0x02u)
#define NoiseGenerator_CTRL_RESET_COMMON              (0x04u)
#define NoiseGenerator_CTRL_RESET_CI                  (0x08u)
#define NoiseGenerator_CTRL_RESET_SI                  (0x10u)
#define NoiseGenerator_CTRL_RESET_SO                  (0x20u)
#define NoiseGenerator_CTRL_RESET_STATE0              (0x40u)
#define NoiseGenerator_CTRL_RESET_STATE1              (0x80u)

#define NoiseGenerator_INIT_STATE                     ( NoiseGenerator_CTRL_RESET_CI |\
                                                          NoiseGenerator_CTRL_RESET_SI |\
                                                          NoiseGenerator_CTRL_RESET_SO |\
                                                          NoiseGenerator_CTRL_RESET_STATE0 |\
                                                          NoiseGenerator_CTRL_RESET_STATE1 )
                                                          
/* Status register definitions */
#define NoiseGenerator_STS_RESET_CI                   (0x08u)
#define NoiseGenerator_STS_RESET_SI                   (0x10u)
#define NoiseGenerator_STS_RESET_SO                   (0x20u)
#define NoiseGenerator_STS_RESET_STATE0               (0x40u)
#define NoiseGenerator_STS_RESET_STATE1               (0x80u)

/* Aux Control register definitions */
#define NoiseGenerator_AUXCTRL_FIFO_SINGLE_REG        (0x03u)

#define NoiseGenerator_NORMAL_SEQUENCE                (0x01u)

#define NoiseGenerator_IS_PRS_ENABLE(reg)             (((reg) & NoiseGenerator_CTRL_ENABLE) != 0u)

#endif  /* End CY_PRS_NoiseGenerator_H */


/* [] END OF FILE */
