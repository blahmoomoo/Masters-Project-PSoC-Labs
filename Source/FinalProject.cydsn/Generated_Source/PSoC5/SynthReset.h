/*******************************************************************************
* File Name: SynthReset.h  
* Version 1.70
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CONTROL_REG_SynthReset_H) /* CY_CONTROL_REG_SynthReset_H */
#define CY_CONTROL_REG_SynthReset_H

#include "cytypes.h"


/***************************************
*         Function Prototypes 
***************************************/

void    SynthReset_Write(uint8 control) ;
uint8   SynthReset_Read(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define SynthReset_Control        (* (reg8 *) SynthReset_Sync_ctrl_reg__CONTROL_REG )
#define SynthReset_Control_PTR    (  (reg8 *) SynthReset_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_SynthReset_H */


/* [] END OF FILE */
