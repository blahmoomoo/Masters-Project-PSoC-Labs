/*******************************************************************************
* File Name: USBUART_std.c
* Version 2.60
*
* Description:
*  USB Standard request handler.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USBUART.h"
#include "USBUART_cdc.h"
#include "USBUART_pvt.h"
#if defined(USBUART_ENABLE_MIDI_STREAMING) 
    #include "USBUART_midi.h"
#endif /* End USBUART_ENABLE_MIDI_STREAMING*/


/***************************************
*   Static data allocation
***************************************/

#if defined(USBUART_ENABLE_FWSN_STRING)
    static volatile uint8 *USBUART_fwSerialNumberStringDescriptor;
    static volatile uint8 USBUART_snStringConfirm = USBUART_FALSE;
#endif  /* USBUART_ENABLE_FWSN_STRING */

#if defined(USBUART_ENABLE_FWSN_STRING)


    /*******************************************************************************
    * Function Name: USBUART_SerialNumString
    ********************************************************************************
    *
    * Summary:
    *  Application firmware may supply the source of the USB device descriptors
    *  serial number string during runtime.
    *
    * Parameters:
    *  snString:  pointer to string.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void  USBUART_SerialNumString(uint8 snString[]) 
    {
        USBUART_snStringConfirm = USBUART_FALSE;
        if(snString != NULL)
        {
            USBUART_fwSerialNumberStringDescriptor = snString;
            /* Check descriptor validation */
            if( (snString[0u] > 1u ) && (snString[1u] == USBUART_DESCR_STRING) )
            {
                USBUART_snStringConfirm = USBUART_TRUE;
            }
        }
    }

#endif  /* USBUART_ENABLE_FWSN_STRING */


/*******************************************************************************
* Function Name: USBUART_HandleStandardRqst
********************************************************************************
*
* Summary:
*  This Routine dispatches standard requests
*
* Parameters:
*  None.
*
* Return:
*  TRUE if request handled.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_HandleStandardRqst(void) 
{
    uint8 requestHandled = USBUART_FALSE;
    uint8 interfaceNumber;
    #if defined(USBUART_ENABLE_STRINGS)
        volatile uint8 *pStr = 0u;
        #if defined(USBUART_ENABLE_DESCRIPTOR_STRINGS)
            uint8 nStr;
            uint8 descrLength;
        #endif /* USBUART_ENABLE_DESCRIPTOR_STRINGS */
    #endif /* USBUART_ENABLE_STRINGS */
    static volatile uint8 USBUART_tBuffer[USBUART_STATUS_LENGTH_MAX];
    const T_USBUART_LUT CYCODE *pTmp;
    USBUART_currentTD.count = 0u;

    if ((CY_GET_REG8(USBUART_bmRequestType) & USBUART_RQST_DIR_MASK) == USBUART_RQST_DIR_D2H)
    {
        /* Control Read */
        switch (CY_GET_REG8(USBUART_bRequest))
        {
            case USBUART_GET_DESCRIPTOR:
                if (CY_GET_REG8(USBUART_wValueHi) == USBUART_DESCR_DEVICE)
                {
                    pTmp = USBUART_GetDeviceTablePtr();
                    USBUART_currentTD.pData = (volatile uint8 *)pTmp->p_list;
                    USBUART_currentTD.count = USBUART_DEVICE_DESCR_LENGTH;
                    requestHandled  = USBUART_InitControlRead();
                }
                else if (CY_GET_REG8(USBUART_wValueHi) == USBUART_DESCR_CONFIG)
                {
                    pTmp = USBUART_GetConfigTablePtr(CY_GET_REG8(USBUART_wValueLo));
                    USBUART_currentTD.pData = (volatile uint8 *)pTmp->p_list;
                    USBUART_currentTD.count = ((uint16)(USBUART_currentTD.pData)[ \
                                      USBUART_CONFIG_DESCR_TOTAL_LENGTH_HI] << 8u) | \
                                     (USBUART_currentTD.pData)[USBUART_CONFIG_DESCR_TOTAL_LENGTH_LOW];
                    requestHandled  = USBUART_InitControlRead();
                }
                #if defined(USBUART_ENABLE_STRINGS)
                else if (CY_GET_REG8(USBUART_wValueHi) == USBUART_DESCR_STRING)
                {
                    /* Descriptor Strings*/
                    #if defined(USBUART_ENABLE_DESCRIPTOR_STRINGS)
                        nStr = 0u;
                        pStr = (volatile uint8 *)&USBUART_STRING_DESCRIPTORS[0u];
                        while ( (CY_GET_REG8(USBUART_wValueLo) > nStr) && (*pStr != 0u) )
                        {
                            /* Read descriptor length from 1st byte */
                            descrLength = *pStr;
                            /* Move to next string descriptor */
                            pStr = &pStr[descrLength];
                            nStr++;
                        }
                    #endif /* End USBUART_ENABLE_DESCRIPTOR_STRINGS */
                    /* Microsoft OS String*/
                    #if defined(USBUART_ENABLE_MSOS_STRING)
                        if( CY_GET_REG8(USBUART_wValueLo) == USBUART_STRING_MSOS )
                        {
                            pStr = (volatile uint8 *)&USBUART_MSOS_DESCRIPTOR[0u];
                        }
                    #endif /* End USBUART_ENABLE_MSOS_STRING*/
                    /* SN string */
                    #if defined(USBUART_ENABLE_SN_STRING)
                        if( (CY_GET_REG8(USBUART_wValueLo) != 0u) &&
                            (CY_GET_REG8(USBUART_wValueLo) ==
                            USBUART_DEVICE0_DESCR[USBUART_DEVICE_DESCR_SN_SHIFT]) )
                        {
                            pStr = (volatile uint8 *)&USBUART_SN_STRING_DESCRIPTOR[0u];
                            #if defined(USBUART_ENABLE_FWSN_STRING)
                                if(USBUART_snStringConfirm != USBUART_FALSE)
                                {
                                    pStr = USBUART_fwSerialNumberStringDescriptor;
                                }
                            #endif  /* USBUART_ENABLE_FWSN_STRING */
                            #if defined(USBUART_ENABLE_IDSN_STRING)
                                /* Read DIE ID and generate string descriptor in RAM */
                                USBUART_ReadDieID(USBUART_idSerialNumberStringDescriptor);
                                pStr = USBUART_idSerialNumberStringDescriptor;
                            #endif    /* End USBUART_ENABLE_IDSN_STRING */
                        }
                    #endif    /* End USBUART_ENABLE_SN_STRING */
                    if (*pStr != 0u)
                    {
                        USBUART_currentTD.count = *pStr;
                        USBUART_currentTD.pData = pStr;
                        requestHandled  = USBUART_InitControlRead();
                    }
                }
                #endif /* End USBUART_ENABLE_STRINGS */
                else
                {
                    requestHandled = USBUART_DispatchClassRqst();
                }
                break;
            case USBUART_GET_STATUS:
                switch ((CY_GET_REG8(USBUART_bmRequestType) & USBUART_RQST_RCPT_MASK))
                {
                    case USBUART_RQST_RCPT_EP:
                        USBUART_currentTD.count = USBUART_EP_STATUS_LENGTH;
                        USBUART_tBuffer[0u] = USBUART_EP[ \
                                        CY_GET_REG8(USBUART_wIndexLo) & USBUART_DIR_UNUSED].hwEpState;
                        USBUART_tBuffer[1u] = 0u;
                        USBUART_currentTD.pData = &USBUART_tBuffer[0u];
                        requestHandled  = USBUART_InitControlRead();
                        break;
                    case USBUART_RQST_RCPT_DEV:
                        USBUART_currentTD.count = USBUART_DEVICE_STATUS_LENGTH;
                        USBUART_tBuffer[0u] = USBUART_deviceStatus;
                        USBUART_tBuffer[1u] = 0u;
                        USBUART_currentTD.pData = &USBUART_tBuffer[0u];
                        requestHandled  = USBUART_InitControlRead();
                        break;
                    default:    /* requestHandled is initialized as FALSE by default */
                        break;
                }
                break;
            case USBUART_GET_CONFIGURATION:
                USBUART_currentTD.count = 1u;
                USBUART_currentTD.pData = (volatile uint8 *)&USBUART_configuration;
                requestHandled  = USBUART_InitControlRead();
                break;
            case USBUART_GET_INTERFACE:
                USBUART_currentTD.count = 1u;
                USBUART_currentTD.pData = (volatile uint8 *)&USBUART_interfaceSetting[ \
                                                                            CY_GET_REG8(USBUART_wIndexLo)];
                requestHandled  = USBUART_InitControlRead();
                break;
            default: /* requestHandled is initialized as FALSE by default */
                break;
        }
    }
    else {
        /* Control Write */
        switch (CY_GET_REG8(USBUART_bRequest))
        {
            case USBUART_SET_ADDRESS:
                USBUART_deviceAddress = CY_GET_REG8(USBUART_wValueLo);
                requestHandled = USBUART_InitNoDataControlTransfer();
                break;
            case USBUART_SET_CONFIGURATION:
                USBUART_configuration = CY_GET_REG8(USBUART_wValueLo);
                USBUART_configurationChanged = USBUART_TRUE;
                USBUART_Config(USBUART_TRUE);
                requestHandled = USBUART_InitNoDataControlTransfer();
                break;
            case USBUART_SET_INTERFACE:
                if (USBUART_ValidateAlternateSetting() != 0u)
                {
                    interfaceNumber = CY_GET_REG8(USBUART_wIndexLo);
                    USBUART_interfaceNumber = interfaceNumber;
                    USBUART_configurationChanged = USBUART_TRUE;
                    #if ((USBUART_EP_MA == USBUART__MA_DYNAMIC) && \
                         (USBUART_EP_MM == USBUART__EP_MANUAL) )
                        USBUART_Config(USBUART_FALSE);
                    #else
                        USBUART_ConfigAltChanged();
                    #endif /* End (USBUART_EP_MA == USBUART__MA_DYNAMIC) */
                    /* Update handled Alt setting changes status */
                    USBUART_interfaceSetting_last[interfaceNumber] =
                         USBUART_interfaceSetting[interfaceNumber];
                    requestHandled = USBUART_InitNoDataControlTransfer();
                }
                break;
            case USBUART_CLEAR_FEATURE:
                switch (CY_GET_REG8(USBUART_bmRequestType) & USBUART_RQST_RCPT_MASK)
                {
                    case USBUART_RQST_RCPT_EP:
                        if (CY_GET_REG8(USBUART_wValueLo) == USBUART_ENDPOINT_HALT)
                        {
                            requestHandled = USBUART_ClearEndpointHalt();
                        }
                        break;
                    case USBUART_RQST_RCPT_DEV:
                        /* Clear device REMOTE_WAKEUP */
                        if (CY_GET_REG8(USBUART_wValueLo) == USBUART_DEVICE_REMOTE_WAKEUP)
                        {
                            USBUART_deviceStatus &= (uint8)~USBUART_DEVICE_STATUS_REMOTE_WAKEUP;
                            requestHandled = USBUART_InitNoDataControlTransfer();
                        }
                        break;
                    case USBUART_RQST_RCPT_IFC:
                        /* Validate interfaceNumber */
                        if (CY_GET_REG8(USBUART_wIndexLo) < USBUART_MAX_INTERFACES_NUMBER)
                        {
                            USBUART_interfaceStatus[CY_GET_REG8(USBUART_wIndexLo)] &=
                                                                (uint8)~(CY_GET_REG8(USBUART_wValueLo));
                            requestHandled = USBUART_InitNoDataControlTransfer();
                        }
                        break;
                    default:    /* requestHandled is initialized as FALSE by default */
                        break;
                }
                break;
            case USBUART_SET_FEATURE:
                switch (CY_GET_REG8(USBUART_bmRequestType) & USBUART_RQST_RCPT_MASK)
                {
                    case USBUART_RQST_RCPT_EP:
                        if (CY_GET_REG8(USBUART_wValueLo) == USBUART_ENDPOINT_HALT)
                        {
                            requestHandled = USBUART_SetEndpointHalt();
                        }
                        break;
                    case USBUART_RQST_RCPT_DEV:
                        /* Set device REMOTE_WAKEUP */
                        if (CY_GET_REG8(USBUART_wValueLo) == USBUART_DEVICE_REMOTE_WAKEUP)
                        {
                            USBUART_deviceStatus |= USBUART_DEVICE_STATUS_REMOTE_WAKEUP;
                            requestHandled = USBUART_InitNoDataControlTransfer();
                        }
                        break;
                    case USBUART_RQST_RCPT_IFC:
                        /* Validate interfaceNumber */
                        if (CY_GET_REG8(USBUART_wIndexLo) < USBUART_MAX_INTERFACES_NUMBER)
                        {
                            USBUART_interfaceStatus[CY_GET_REG8(USBUART_wIndexLo)] &=
                                                                (uint8)~(CY_GET_REG8(USBUART_wValueLo));
                            requestHandled = USBUART_InitNoDataControlTransfer();
                        }
                        break;
                    default:    /* requestHandled is initialized as FALSE by default */
                        break;
                }
                break;
            default:    /* requestHandled is initialized as FALSE by default */
                break;
        }
    }
    return(requestHandled);
}


#if defined(USBUART_ENABLE_IDSN_STRING)

    /***************************************************************************
    * Function Name: USBUART_ReadDieID
    ****************************************************************************
    *
    * Summary:
    *  This routine read Die ID and generate Serial Number string descriptor.
    *
    * Parameters:
    *  descr:  pointer on string descriptor.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  No.
    *
    ***************************************************************************/
    void USBUART_ReadDieID(uint8 descr[]) 
    {
        uint8 i;
        uint8 j = 0u;
        uint8 value;
        const char8 CYCODE hex[16u] = "0123456789ABCDEF";


        /* Check descriptor validation */
        if( descr != NULL)
        {
            descr[0u] = USBUART_IDSN_DESCR_LENGTH;
            descr[1u] = USBUART_DESCR_STRING;

            /* fill descriptor */
            for(i = 2u; i < USBUART_IDSN_DESCR_LENGTH; i += 4u)
            {
                value = CY_GET_XTND_REG8((void CYFAR *)(USBUART_DIE_ID + j));
                j++;
                descr[i] = (uint8)hex[value >> 4u];
                descr[i + 2u] = (uint8)hex[value & 0x0Fu];
            }
        }
    }

#endif /* End USBUART_ENABLE_IDSN_STRING */


/*******************************************************************************
* Function Name: USBUART_ConfigReg
********************************************************************************
*
* Summary:
*  This routine configures hardware registers from the variables.
*  It is called from USBUART_Config() function and from RestoreConfig
*  after Wakeup.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void USBUART_ConfigReg(void) 
{
    uint8 ep;
    uint8 i;
    #if(USBUART_EP_MM == USBUART__EP_DMAAUTO)
        uint8 ep_type = 0u;
    #endif /* End USBUART_EP_MM == USBUART__EP_DMAAUTO */

    /* Set the endpoint buffer addresses */
    ep = USBUART_EP1;
    for (i = 0u; i < 0x80u; i+= 0x10u)
    {
        CY_SET_REG8((reg8 *)(USBUART_ARB_EP1_CFG_IND + i), USBUART_ARB_EPX_CFG_CRC_BYPASS |
                                                          USBUART_ARB_EPX_CFG_RESET);

        #if(USBUART_EP_MM != USBUART__EP_MANUAL)
            /* Enable all Arbiter EP Interrupts : err, buf under, buf over, dma gnt(mode2 only), in buf full */
            CY_SET_REG8((reg8 *)(USBUART_ARB_EP1_INT_EN_IND + i), USBUART_ARB_EPX_INT_MASK);
        #endif   /* End USBUART_EP_MM != USBUART__EP_MANUAL */

        if(USBUART_EP[ep].epMode != USBUART_MODE_DISABLE)
        {
            if((USBUART_EP[ep].addr & USBUART_DIR_IN) != 0u )
            {
                CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CR0_IND + i), USBUART_MODE_NAK_IN);
            }
            else
            {
                CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CR0_IND + i), USBUART_MODE_NAK_OUT);
                /* Prepare EP type mask for automatic memory allocation */
                #if(USBUART_EP_MM == USBUART__EP_DMAAUTO)
                    ep_type |= (uint8)(0x01u << (ep - USBUART_EP1));
                #endif /* End USBUART_EP_MM == USBUART__EP_DMAAUTO */
            }
        }
        else
        {
            CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CR0_IND + i), USBUART_MODE_STALL_DATA_EP);
        }

        #if(USBUART_EP_MM != USBUART__EP_DMAAUTO)
            CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CNT0_IND + i),   USBUART_EP[ep].bufferSize >> 8u);
            CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CNT1_IND + i),   USBUART_EP[ep].bufferSize & 0xFFu);

            CY_SET_REG8((reg8 *)(USBUART_ARB_RW1_RA_IND + i),     USBUART_EP[ep].buffOffset & 0xFFu);
            CY_SET_REG8((reg8 *)(USBUART_ARB_RW1_RA_MSB_IND + i), USBUART_EP[ep].buffOffset >> 8u);
            CY_SET_REG8((reg8 *)(USBUART_ARB_RW1_WA_IND + i),     USBUART_EP[ep].buffOffset & 0xFFu);
            CY_SET_REG8((reg8 *)(USBUART_ARB_RW1_WA_MSB_IND + i), USBUART_EP[ep].buffOffset >> 8u);
        #endif /* End USBUART_EP_MM != USBUART__EP_DMAAUTO */

        ep++;
    }

    #if(USBUART_EP_MM == USBUART__EP_DMAAUTO)
         /* BUF_SIZE depend on DMA_THRESS value: 55-32 bytes  44-16 bytes 33-8 bytes 22-4 bytes 11-2 bytes */
        USBUART_BUF_SIZE_REG = USBUART_DMA_BUF_SIZE;
        USBUART_DMA_THRES_REG = USBUART_DMA_BYTES_PER_BURST;   /* DMA burst threshold */
        USBUART_DMA_THRES_MSB_REG = 0u;
        USBUART_EP_ACTIVE_REG = USBUART_ARB_INT_MASK;
        USBUART_EP_TYPE_REG = ep_type;
        /* Cfg_cmp bit set to 1 once configuration is complete. */
        USBUART_ARB_CFG_REG = USBUART_ARB_CFG_AUTO_DMA | USBUART_ARB_CFG_AUTO_MEM |
                                       USBUART_ARB_CFG_CFG_CPM;
        /* Cfg_cmp bit set to 0 during configuration of PFSUSB Registers. */
        USBUART_ARB_CFG_REG = USBUART_ARB_CFG_AUTO_DMA | USBUART_ARB_CFG_AUTO_MEM;
    #endif /* End USBUART_EP_MM == USBUART__EP_DMAAUTO */

    CY_SET_REG8(USBUART_SIE_EP_INT_EN_PTR, 0xFFu);
}


/*******************************************************************************
* Function Name: USBUART_Config
********************************************************************************
*
* Summary:
*  This routine configures endpoints for the entire configuration by scanning
*  the configuration descriptor.
*
* Parameters:
*  clearAltSetting: It configures the bAlternateSetting 0 for each interface.
*
* Return:
*  None.
*
* USBUART_interfaceClass - Initialized class array for each interface.
*   It is used for handling Class specific requests depend on interface class.
*   Different classes in multiple Alternate settings does not supported.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Config(uint8 clearAltSetting) 
{
    uint8 ep;
    uint8 cur_ep;
    uint8 i;
    uint8 ep_type;
    const uint8 *pDescr;
    #if(USBUART_EP_MM != USBUART__EP_DMAAUTO)
        uint16 buffCount = 0u;
    #endif /* End USBUART_EP_MM != USBUART__EP_DMAAUTO */

    const T_USBUART_LUT CYCODE *pTmp;
    const T_USBUART_EP_SETTINGS_BLOCK CYCODE *pEP;

    /* Clear all of the endpoints */
    for (ep = 0u; ep < USBUART_MAX_EP; ep++)
    {
        USBUART_EP[ep].attrib = 0u;
        USBUART_EP[ep].hwEpState = 0u;
        USBUART_EP[ep].apiEpState = USBUART_NO_EVENT_PENDING;
        USBUART_EP[ep].epToggle = 0u;
        USBUART_EP[ep].epMode = USBUART_MODE_DISABLE;
        USBUART_EP[ep].bufferSize = 0u;
        USBUART_EP[ep].interface = 0u;

    }

    /* Clear Alternate settings for all interfaces */
    if(clearAltSetting != 0u)
    {
        for (i = 0u; i < USBUART_MAX_INTERFACES_NUMBER; i++)
        {
            USBUART_interfaceSetting[i] = 0x00u;
            USBUART_interfaceSetting_last[i] = 0x00u;
        }
    }

    /* Init Endpoints and Device Status if configured */
    if(USBUART_configuration > 0u)
    {
        pTmp = USBUART_GetConfigTablePtr(USBUART_configuration - 1u);
        /* Set Power status for current configuration */
        pDescr = (const uint8 *)pTmp->p_list;
        if((pDescr[USBUART_CONFIG_DESCR_ATTRIB] & USBUART_CONFIG_DESCR_ATTRIB_SELF_POWERED) != 0u)
        {
            USBUART_deviceStatus |=  USBUART_DEVICE_STATUS_SELF_POWERED;
        }
        else
        {
            USBUART_deviceStatus &=  (uint8)~USBUART_DEVICE_STATUS_SELF_POWERED;
        }
        /* Move to next element */
        pTmp = &pTmp[1u];
        ep = pTmp->c;  /* For this table, c is the number of endpoints configurations  */

        #if ((USBUART_EP_MA == USBUART__MA_DYNAMIC) && \
             (USBUART_EP_MM == USBUART__EP_MANUAL) )
            /* Configure for dynamic EP memory allocation */
            /* p_list points the endpoint setting table. */
            pEP = (T_USBUART_EP_SETTINGS_BLOCK *) pTmp->p_list;
            for (i = 0u; i < ep; i++)
            {
                /* Compare current Alternate setting with EP Alt*/
                if(USBUART_interfaceSetting[pEP->interface] == pEP->altSetting)
                {
                    cur_ep = pEP->addr & USBUART_DIR_UNUSED;
                    ep_type = pEP->attributes & USBUART_EP_TYPE_MASK;
                    if (pEP->addr & USBUART_DIR_IN)
                    {
                        /* IN Endpoint */
                        USBUART_EP[cur_ep].apiEpState = USBUART_EVENT_PENDING;
                        USBUART_EP[cur_ep].epMode = (ep_type == USBUART_EP_TYPE_ISOC) ?
                                                        USBUART_MODE_ISO_IN : USBUART_MODE_ACK_IN;
                        #if defined(USBUART_ENABLE_CDC_CLASS)
                            if(((pEP->bMisc == USBUART_CLASS_CDC_DATA) ||
                                (pEP->bMisc == USBUART_CLASS_CDC)) &&
                                (ep_type != USBUART_EP_TYPE_INT))
                            {
                                USBUART_cdc_data_in_ep = cur_ep;
                            }
                        #endif  /* End USBUART_ENABLE_CDC_CLASS*/
                        #if ( defined(USBUART_ENABLE_MIDI_STREAMING) && \
                                             (USBUART_MIDI_IN_BUFF_SIZE > 0) )
                            if((pEP->bMisc == USBUART_CLASS_AUDIO) &&
                               (ep_type == USBUART_EP_TYPE_BULK))
                            {
                                USBUART_midi_in_ep = cur_ep;
                            }
                        #endif  /* End USBUART_ENABLE_MIDI_STREAMING*/
                    }
                    else
                    {
                        /* OUT Endpoint */
                        USBUART_EP[cur_ep].apiEpState = USBUART_NO_EVENT_PENDING;
                        USBUART_EP[cur_ep].epMode = (ep_type == USBUART_EP_TYPE_ISOC) ?
                                                    USBUART_MODE_ISO_OUT : USBUART_MODE_ACK_OUT;
                        #if defined(USBUART_ENABLE_CDC_CLASS)
                            if(((pEP->bMisc == USBUART_CLASS_CDC_DATA) ||
                                (pEP->bMisc == USBUART_CLASS_CDC)) &&
                                (ep_type != USBUART_EP_TYPE_INT))
                            {
                                USBUART_cdc_data_out_ep = cur_ep;
                            }
                        #endif  /* End USBUART_ENABLE_CDC_CLASS*/
                        #if ( defined(USBUART_ENABLE_MIDI_STREAMING) && \
                                     (USBUART_MIDI_OUT_BUFF_SIZE > 0) )
                            if((pEP->bMisc == USBUART_CLASS_AUDIO) &&
                               (ep_type == USBUART_EP_TYPE_BULK))
                            {
                                USBUART_midi_out_ep = cur_ep;
                            }
                        #endif  /* End USBUART_ENABLE_MIDI_STREAMING*/
                    }
                    USBUART_EP[cur_ep].bufferSize = pEP->bufferSize;
                    USBUART_EP[cur_ep].addr = pEP->addr;
                    USBUART_EP[cur_ep].attrib = pEP->attributes;
                }
                pEP = &pEP[1u];
            }
        #else /* Config for static EP memory allocation  */
            for (i = USBUART_EP1; i < USBUART_MAX_EP; i++)
            {
                /* p_list points the endpoint setting table. */
                pEP = (const T_USBUART_EP_SETTINGS_BLOCK CYCODE *) pTmp->p_list;
                /* Find max length for each EP and select it (length could be different in different Alt settings) */
                /* but other settings should be correct with regards to Interface alt Setting */
                for (cur_ep = 0u; cur_ep < ep; cur_ep++)
                {
                    /* EP count is equal to EP # in table and we found larger EP length than have before*/
                    if(i == (pEP->addr & USBUART_DIR_UNUSED))
                    {
                        if(USBUART_EP[i].bufferSize < pEP->bufferSize)
                        {
                            USBUART_EP[i].bufferSize = pEP->bufferSize;
                        }
                        /* Compare current Alternate setting with EP Alt*/
                        if(USBUART_interfaceSetting[pEP->interface] == pEP->altSetting)
                        {
                            ep_type = pEP->attributes & USBUART_EP_TYPE_MASK;
                            if ((pEP->addr & USBUART_DIR_IN) != 0u)
                            {
                                /* IN Endpoint */
                                USBUART_EP[i].apiEpState = USBUART_EVENT_PENDING;
                                USBUART_EP[i].epMode = (ep_type == USBUART_EP_TYPE_ISOC) ?
                                                        USBUART_MODE_ISO_IN : USBUART_MODE_ACK_IN;
                                /* Find and init CDC IN endpoint number */
                                #if defined(USBUART_ENABLE_CDC_CLASS)
                                    if(((pEP->bMisc == USBUART_CLASS_CDC_DATA) ||
                                        (pEP->bMisc == USBUART_CLASS_CDC)) &&
                                        (ep_type != USBUART_EP_TYPE_INT))
                                    {
                                        USBUART_cdc_data_in_ep = i;
                                    }
                                #endif  /* End USBUART_ENABLE_CDC_CLASS*/
                                #if ( defined(USBUART_ENABLE_MIDI_STREAMING) && \
                                             (USBUART_MIDI_IN_BUFF_SIZE > 0) )
                                    if((pEP->bMisc == USBUART_CLASS_AUDIO) &&
                                       (ep_type == USBUART_EP_TYPE_BULK))
                                    {
                                        USBUART_midi_in_ep = i;
                                    }
                                #endif  /* End USBUART_ENABLE_MIDI_STREAMING*/
                            }
                            else
                            {
                                /* OUT Endpoint */
                                USBUART_EP[i].apiEpState = USBUART_NO_EVENT_PENDING;
                                USBUART_EP[i].epMode = (ep_type == USBUART_EP_TYPE_ISOC) ?
                                                    USBUART_MODE_ISO_OUT : USBUART_MODE_ACK_OUT;
                                /* Find and init CDC IN endpoint number */
                                #if defined(USBUART_ENABLE_CDC_CLASS)
                                    if(((pEP->bMisc == USBUART_CLASS_CDC_DATA) ||
                                        (pEP->bMisc == USBUART_CLASS_CDC)) &&
                                        (ep_type != USBUART_EP_TYPE_INT))
                                    {
                                        USBUART_cdc_data_out_ep = i;
                                    }
                                #endif  /* End USBUART_ENABLE_CDC_CLASS*/
                                #if ( defined(USBUART_ENABLE_MIDI_STREAMING) && \
                                             (USBUART_MIDI_OUT_BUFF_SIZE > 0) )
                                    if((pEP->bMisc == USBUART_CLASS_AUDIO) &&
                                       (ep_type == USBUART_EP_TYPE_BULK))
                                    {
                                        USBUART_midi_out_ep = i;
                                    }
                                #endif  /* End USBUART_ENABLE_MIDI_STREAMING*/
                            }
                            USBUART_EP[i].addr = pEP->addr;
                            USBUART_EP[i].attrib = pEP->attributes;

                            #if(USBUART_EP_MM == USBUART__EP_DMAAUTO)
                                break;      /* use first EP setting in Auto memory managment */
                            #endif /* End USBUART_EP_MM == USBUART__EP_DMAAUTO */
                        }
                    }
                    pEP = &pEP[1u];
                }
            }
        #endif /* End (USBUART_EP_MA == USBUART__MA_DYNAMIC) */

        /* Init class array for each interface and interface number for each EP.
        *  It is used for handling Class specific requests directed to either an
        *  interface or the endpoint.
        */
        /* p_list points the endpoint setting table. */
        pEP = (const T_USBUART_EP_SETTINGS_BLOCK CYCODE *) pTmp->p_list;
        for (i = 0u; i < ep; i++)
        {
            /* Configure interface number for each EP*/
            USBUART_EP[pEP->addr & USBUART_DIR_UNUSED].interface = pEP->interface;
            pEP = &pEP[1u];
        }
        /* Init pointer on interface class table*/
        USBUART_interfaceClass = USBUART_GetInterfaceClassTablePtr();
        /* Set the endpoint buffer addresses */

        #if(USBUART_EP_MM != USBUART__EP_DMAAUTO)
            for (ep = USBUART_EP1; ep < USBUART_MAX_EP; ep++)
            {
                USBUART_EP[ep].buffOffset = buffCount;
                 buffCount += USBUART_EP[ep].bufferSize;
            }
        #endif /* End USBUART_EP_MM != USBUART__EP_DMAAUTO */

        /* Configure hardware registers */
        USBUART_ConfigReg();
    } /* USBUART_configuration > 0 */
}


/*******************************************************************************
* Function Name: USBUART_ConfigAltChanged
********************************************************************************
*
* Summary:
*  This routine update configuration for the required endpoints only.
*  It is called after SET_INTERFACE request when Static memory allocation used.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_ConfigAltChanged(void) 
{
    uint8 ep;
    uint8 cur_ep;
    uint8 i;
    uint8 ep_type;
    uint8 ri;

    const T_USBUART_LUT CYCODE *pTmp;
    const T_USBUART_EP_SETTINGS_BLOCK CYCODE *pEP;


    /* Init Endpoints and Device Status if configured */
    if(USBUART_configuration > 0u)
    {
        pTmp = USBUART_GetConfigTablePtr(USBUART_configuration - 1u);
        pTmp = &pTmp[1u];
        ep = pTmp->c;  /* For this table, c is the number of endpoints configurations  */

        /* Do not touch EP which doesn't need reconfiguration */
        /* When Alt setting changed, the only required endpoints need to be reconfigured */
        /* p_list points the endpoint setting table. */
        pEP = (const T_USBUART_EP_SETTINGS_BLOCK CYCODE *) pTmp->p_list;
        for (i = 0u; i < ep; i++)
        {
            /*If Alt setting changed and new is same with EP Alt */
            if((USBUART_interfaceSetting[pEP->interface] !=
                USBUART_interfaceSetting_last[pEP->interface] ) &&
               (USBUART_interfaceSetting[pEP->interface] == pEP->altSetting) &&
               (pEP->interface == CY_GET_REG8(USBUART_wIndexLo)))
            {
                cur_ep = pEP->addr & USBUART_DIR_UNUSED;
                ri = ((cur_ep - USBUART_EP1) << USBUART_EPX_CNTX_ADDR_SHIFT);
                ep_type = pEP->attributes & USBUART_EP_TYPE_MASK;
                if ((pEP->addr & USBUART_DIR_IN) != 0u)
                {
                    /* IN Endpoint */
                    USBUART_EP[cur_ep].apiEpState = USBUART_EVENT_PENDING;
                    USBUART_EP[cur_ep].epMode = (ep_type == USBUART_EP_TYPE_ISOC) ?
                                                USBUART_MODE_ISO_IN : USBUART_MODE_ACK_IN;
                }
                else
                {
                    /* OUT Endpoint */
                    USBUART_EP[cur_ep].apiEpState = USBUART_NO_EVENT_PENDING;
                    USBUART_EP[cur_ep].epMode = (ep_type == USBUART_EP_TYPE_ISOC) ?
                                                USBUART_MODE_ISO_OUT : USBUART_MODE_ACK_OUT;
                }
                 /* Change the SIE mode for the selected EP to NAK ALL */
                 CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CR0_IND + ri), USBUART_MODE_NAK_IN_OUT);
                USBUART_EP[cur_ep].bufferSize = pEP->bufferSize;
                USBUART_EP[cur_ep].addr = pEP->addr;
                USBUART_EP[cur_ep].attrib = pEP->attributes;

                /* Clear the data toggle */
                USBUART_EP[cur_ep].epToggle = 0u;

                /* Dynamic reconfiguration for mode 3 transfer */
            #if(USBUART_EP_MM == USBUART__EP_DMAAUTO)
                /* In_data_rdy for selected EP should be set to 0 */
                * (reg8 *)(USBUART_ARB_EP1_CFG_IND + ri) &= (uint8)~USBUART_ARB_EPX_CFG_IN_DATA_RDY;

                /* write the EP number for which reconfiguration is required */
                USBUART_DYN_RECONFIG_REG = (cur_ep - USBUART_EP1) <<
                                                    USBUART_DYN_RECONFIG_EP_SHIFT;
                /* Set the dyn_config_en bit in dynamic reconfiguration register */
                USBUART_DYN_RECONFIG_REG |= USBUART_DYN_RECONFIG_ENABLE;
                /* wait for the dyn_config_rdy bit to set by the block,
                *  this bit will be set to 1 when block is ready for reconfiguration.
                */
                while((USBUART_DYN_RECONFIG_REG & USBUART_DYN_RECONFIG_RDY_STS) == 0u)
                {
                    ;
                }
                /* Once dyn_config_rdy bit is set, FW can change the EP configuration. */
                /* Change EP Type with new direction */
                if((pEP->addr & USBUART_DIR_IN) == 0u)
                {
                    USBUART_EP_TYPE_REG |= (uint8)(0x01u << (cur_ep - USBUART_EP1));
                }
                else
                {
                    USBUART_EP_TYPE_REG &= (uint8)~(uint8)(0x01u << (cur_ep - USBUART_EP1));
                }
                /* dynamic reconfiguration enable bit cleared, pointers and control/status
                *  signals for the selected EP is cleared/re-initialized on negative edge
                *  of dynamic reconfiguration enable bit).
                */
                USBUART_DYN_RECONFIG_REG &= (uint8)~USBUART_DYN_RECONFIG_ENABLE;
                /* The main loop has to re-enable DMA and OUT endpoint*/
            #else
                CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CNT0_IND + ri),
                                                                USBUART_EP[cur_ep].bufferSize >> 8u);
                CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CNT1_IND + ri),
                                                                USBUART_EP[cur_ep].bufferSize & 0xFFu);
                CY_SET_REG8((reg8 *)(USBUART_ARB_RW1_RA_IND + ri),
                                                                USBUART_EP[cur_ep].buffOffset & 0xFFu);
                CY_SET_REG8((reg8 *)(USBUART_ARB_RW1_RA_MSB_IND + ri),
                                                                USBUART_EP[cur_ep].buffOffset >> 8u);
                CY_SET_REG8((reg8 *)(USBUART_ARB_RW1_WA_IND + ri),
                                                                USBUART_EP[cur_ep].buffOffset & 0xFFu);
                CY_SET_REG8((reg8 *)(USBUART_ARB_RW1_WA_MSB_IND + ri),
                                                                USBUART_EP[cur_ep].buffOffset >> 8u);
            #endif /* End USBUART_EP_MM == USBUART__EP_DMAAUTO */
            }
            /* Get next EP element */
            pEP = &pEP[1u];
        }
    }   /* USBUART_configuration > 0 */
}


/*******************************************************************************
* Function Name: USBUART_GetConfigTablePtr
********************************************************************************
*
* Summary:
*  This routine returns a pointer a configuration table entry
*
* Parameters:
*  c:  Configuration Index
*
* Return:
*  Device Descriptor pointer.
*
*******************************************************************************/
const T_USBUART_LUT CYCODE *USBUART_GetConfigTablePtr(uint8 c)
                                                        
{
    /* Device Table */
    const T_USBUART_LUT CYCODE *pTmp;

    pTmp = (const T_USBUART_LUT CYCODE *) USBUART_TABLE[USBUART_device].p_list;

    /* The first entry points to the Device Descriptor,
    *  the rest configuration entries.
	*/
    return( (const T_USBUART_LUT CYCODE *) pTmp[c + 1u].p_list );
}


/*******************************************************************************
* Function Name: USBUART_GetDeviceTablePtr
********************************************************************************
*
* Summary:
*  This routine returns a pointer to the Device table
*
* Parameters:
*  None.
*
* Return:
*  Device Table pointer
*
*******************************************************************************/
const T_USBUART_LUT CYCODE *USBUART_GetDeviceTablePtr(void)
                                                            
{
    /* Device Table */
    return( (const T_USBUART_LUT CYCODE *) USBUART_TABLE[USBUART_device].p_list );
}


/*******************************************************************************
* Function Name: USB_GetInterfaceClassTablePtr
********************************************************************************
*
* Summary:
*  This routine returns Interface Class table pointer, which contains
*  the relation between interface number and interface class.
*
* Parameters:
*  None.
*
* Return:
*  Interface Class table pointer.
*
*******************************************************************************/
const uint8 CYCODE *USBUART_GetInterfaceClassTablePtr(void)
                                                        
{
    const T_USBUART_LUT CYCODE *pTmp;
    uint8 currentInterfacesNum;

    pTmp = USBUART_GetConfigTablePtr(USBUART_configuration - 1u);
    currentInterfacesNum  = ((const uint8 *) pTmp->p_list)[USBUART_CONFIG_DESCR_NUM_INTERFACES];
    /* Third entry in the LUT starts the Interface Table pointers */
    /* The INTERFACE_CLASS table is located after all interfaces */
    pTmp = &pTmp[currentInterfacesNum + 2u];
    return( (const uint8 CYCODE *) pTmp->p_list );
}


/*******************************************************************************
* Function Name: USBUART_TerminateEP
********************************************************************************
*
* Summary:
*  This function terminates the specified USBFS endpoint.
*  This function should be used before endpoint reconfiguration.
*
* Parameters:
*  Endpoint number.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_TerminateEP(uint8 ep) 
{
    uint8 ri;

    ep &= USBUART_DIR_UNUSED;
    ri = ((ep - USBUART_EP1) << USBUART_EPX_CNTX_ADDR_SHIFT);

    if ((ep > USBUART_EP0) && (ep < USBUART_MAX_EP))
    {
        /* Set the endpoint Halt */
        USBUART_EP[ep].hwEpState |= (USBUART_ENDPOINT_STATUS_HALT);

        /* Clear the data toggle */
        USBUART_EP[ep].epToggle = 0u;
        USBUART_EP[ep].apiEpState = USBUART_NO_EVENT_ALLOWED;

        if ((USBUART_EP[ep].addr & USBUART_DIR_IN) != 0u)
        {
            /* IN Endpoint */
            CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CR0_IND + ri), USBUART_MODE_NAK_IN);
        }
        else
        {
            /* OUT Endpoint */
            CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CR0_IND + ri), USBUART_MODE_NAK_OUT);
        }
    }
}


/*******************************************************************************
* Function Name: USBUART_SetEndpointHalt
********************************************************************************
*
* Summary:
*  This routine handles set endpoint halt.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_SetEndpointHalt(void) 
{
    uint8 ep;
    uint8 ri;
    uint8 requestHandled = USBUART_FALSE;

    /* Set endpoint halt */
    ep = CY_GET_REG8(USBUART_wIndexLo) & USBUART_DIR_UNUSED;
    ri = ((ep - USBUART_EP1) << USBUART_EPX_CNTX_ADDR_SHIFT);

    if ((ep > USBUART_EP0) && (ep < USBUART_MAX_EP))
    {
        /* Set the endpoint Halt */
        USBUART_EP[ep].hwEpState |= (USBUART_ENDPOINT_STATUS_HALT);

        /* Clear the data toggle */
        USBUART_EP[ep].epToggle = 0u;
        USBUART_EP[ep].apiEpState |= USBUART_NO_EVENT_ALLOWED;

        if ((USBUART_EP[ep].addr & USBUART_DIR_IN) != 0u)
        {
            /* IN Endpoint */
            CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CR0_IND + ri), USBUART_MODE_STALL_DATA_EP |
                                                               USBUART_MODE_ACK_IN);
        }
        else
        {
            /* OUT Endpoint */
            CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CR0_IND + ri), USBUART_MODE_STALL_DATA_EP |
                                                               USBUART_MODE_ACK_OUT);
        }
        requestHandled = USBUART_InitNoDataControlTransfer();
    }

    return(requestHandled);
}


/*******************************************************************************
* Function Name: USBUART_ClearEndpointHalt
********************************************************************************
*
* Summary:
*  This routine handles clear endpoint halt.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_ClearEndpointHalt(void) 
{
    uint8 ep;
    uint8 ri;
    uint8 requestHandled = USBUART_FALSE;

    /* Clear endpoint halt */
    ep = CY_GET_REG8(USBUART_wIndexLo) & USBUART_DIR_UNUSED;
    ri = ((ep - USBUART_EP1) << USBUART_EPX_CNTX_ADDR_SHIFT);

    if ((ep > USBUART_EP0) && (ep < USBUART_MAX_EP))
    {
        /* Clear the endpoint Halt */
        USBUART_EP[ep].hwEpState &= (uint8)~(USBUART_ENDPOINT_STATUS_HALT);

        /* Clear the data toggle */
        USBUART_EP[ep].epToggle = 0u;
        /* Clear toggle bit for already armed packet */
        CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CNT0_IND + ri), CY_GET_REG8(
                    (reg8 *)(USBUART_SIE_EP1_CNT0_IND + ri)) & (uint8)~USBUART_EPX_CNT_DATA_TOGGLE);
        /* Return API State as it was defined before */
        USBUART_EP[ep].apiEpState &= (uint8)~USBUART_NO_EVENT_ALLOWED;

        if ((USBUART_EP[ep].addr & USBUART_DIR_IN) != 0u)
        {
            /* IN Endpoint */
            if(USBUART_EP[ep].apiEpState == USBUART_IN_BUFFER_EMPTY)
            {       /* Wait for next packet from application */
                CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CR0_IND + ri), USBUART_MODE_NAK_IN);
            }
            else    /* Continue armed transfer */
            {
                CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CR0_IND + ri), USBUART_MODE_ACK_IN);
            }
        }
        else
        {
            /* OUT Endpoint */
            if(USBUART_EP[ep].apiEpState == USBUART_OUT_BUFFER_FULL)
            {       /* Allow application to read full buffer */
                CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CR0_IND + ri), USBUART_MODE_NAK_OUT);
            }
            else    /* Mark endpoint as empty, so it will be reloaded */
            {
                CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CR0_IND + ri), USBUART_MODE_ACK_OUT);
            }
        }
        requestHandled = USBUART_InitNoDataControlTransfer();
    }

    return(requestHandled);
}


/*******************************************************************************
* Function Name: USBUART_ValidateAlternateSetting
********************************************************************************
*
* Summary:
*  Validates (and records) a SET INTERFACE request.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_ValidateAlternateSetting(void) 
{
    uint8 requestHandled = USBUART_TRUE;
    uint8 interfaceNum;
    const T_USBUART_LUT CYCODE *pTmp;
    uint8 currentInterfacesNum;

    interfaceNum = CY_GET_REG8(USBUART_wIndexLo);
    /* Validate interface setting, stall if invalid. */
    pTmp = USBUART_GetConfigTablePtr(USBUART_configuration - 1u);
    currentInterfacesNum  = ((const uint8 *) pTmp->p_list)[USBUART_CONFIG_DESCR_NUM_INTERFACES];

    if((interfaceNum >= currentInterfacesNum) || (interfaceNum >= USBUART_MAX_INTERFACES_NUMBER))
    {   /* Wrong interface number */
        requestHandled = USBUART_FALSE;
    }
    else
    {
        /* Save current Alt setting to find out the difference in Config() function */
        USBUART_interfaceSetting_last[interfaceNum] = USBUART_interfaceSetting[interfaceNum];
        USBUART_interfaceSetting[interfaceNum] = CY_GET_REG8(USBUART_wValueLo);
    }

    return (requestHandled);
}


/* [] END OF FILE */
