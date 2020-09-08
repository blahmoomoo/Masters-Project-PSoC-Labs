/***************************************************************************
* File Name: AudioSample_dma_dma.c  
* Version 1.70
*
*  Description:
*   Provides an API for the DMAC component. The API includes functions
*   for the DMA controller, DMA channels and Transfer Descriptors.
*
*
*   Note:
*     This module requires the developer to finish or fill in the auto
*     generated funcions and setup the dma channel and TD's.
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/
#include <CYLIB.H>
#include <CYDMAC.H>
#include <AudioSample_dma_dma.H>



/****************************************************************************
* 
* The following defines are available in Cyfitter.h
* 
* 
* 
* AudioSample_dma__DRQ_CTL_REG
* 
* 
* AudioSample_dma__DRQ_NUMBER
* 
* Number of TD's used by this channel.
* AudioSample_dma__NUMBEROF_TDS
* 
* Priority of this channel.
* AudioSample_dma__PRIORITY
* 
* True if AudioSample_dma_TERMIN_SEL is used.
* AudioSample_dma__TERMIN_EN
* 
* TERMIN interrupt line to signal terminate.
* AudioSample_dma__TERMIN_SEL
* 
* 
* True if AudioSample_dma_TERMOUT0_SEL is used.
* AudioSample_dma__TERMOUT0_EN
* 
* 
* TERMOUT0 interrupt line to signal completion.
* AudioSample_dma__TERMOUT0_SEL
* 
* 
* True if AudioSample_dma_TERMOUT1_SEL is used.
* AudioSample_dma__TERMOUT1_EN
* 
* 
* TERMOUT1 interrupt line to signal completion.
* AudioSample_dma__TERMOUT1_SEL
* 
****************************************************************************/


/* Zero based index of AudioSample_dma dma channel */
uint8 AudioSample_dma_DmaHandle = DMA_INVALID_CHANNEL;

/*********************************************************************
* Function Name: uint8 AudioSample_dma_DmaInitalize
**********************************************************************
* Summary:
*   Allocates and initialises a channel of the DMAC to be used by the
*   caller.
*
* Parameters:
*   BurstCount.
*       
*       
*   ReqestPerBurst.
*       
*       
*   UpperSrcAddress.
*       
*       
*   UpperDestAddress.
*       
*
* Return:
*   The channel that can be used by the caller for DMA activity.
*   DMA_INVALID_CHANNEL (0xFF) if there are no channels left. 
*
*
*******************************************************************/
uint8 AudioSample_dma_DmaInitialize(uint8 BurstCount, uint8 ReqestPerBurst, uint16 UpperSrcAddress, uint16 UpperDestAddress) 
{

    /* Allocate a DMA channel. */
    AudioSample_dma_DmaHandle = (uint8)AudioSample_dma__DRQ_NUMBER;

    /* Configure the channel. */
    (void)CyDmaChSetConfiguration(AudioSample_dma_DmaHandle,
                                  BurstCount,
                                  ReqestPerBurst,
                                  (uint8)AudioSample_dma__TERMOUT0_SEL,
                                  (uint8)AudioSample_dma__TERMOUT1_SEL,
                                  (uint8)AudioSample_dma__TERMIN_SEL);

    /* Set the extended address for the transfers */
    (void)CyDmaChSetExtendedAddress(AudioSample_dma_DmaHandle, UpperSrcAddress, UpperDestAddress);

    /* Set the priority for this channel */
    (void)CyDmaChPriority(AudioSample_dma_DmaHandle, (uint8)AudioSample_dma__PRIORITY);
    
    return AudioSample_dma_DmaHandle;
}

/*********************************************************************
* Function Name: void AudioSample_dma_DmaRelease
**********************************************************************
* Summary:
*   Frees the channel associated with AudioSample_dma.
*
*
* Parameters:
*   void.
*
*
*
* Return:
*   void.
*
*******************************************************************/
void AudioSample_dma_DmaRelease(void) 
{
    /* Disable the channel */
    (void)CyDmaChDisable(AudioSample_dma_DmaHandle);
}

