/*******************************************************************************
* File Name: USBUART_cdc.c
* Version 2.60
*
* Description:
*  USB HID Class request handler.
*
* Note:
*
********************************************************************************
* Copyright 2012-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "USBUART.h"

#if defined(USBUART_ENABLE_CDC_CLASS)

#include "USBUART_cdc.h"
#include "USBUART_pvt.h"


/***************************************
*    CDC Variables
***************************************/

volatile uint8 USBUART_lineCoding[USBUART_LINE_CODING_SIZE];
volatile uint8 USBUART_lineChanged;
volatile uint16 USBUART_lineControlBitmap;
volatile uint8 USBUART_cdc_data_in_ep;
volatile uint8 USBUART_cdc_data_out_ep;


/***************************************
*     Static Function Prototypes
***************************************/
static uint16 USBUART_StrLen(const char8 string[]) ;


/***************************************
* Custom Declarations
***************************************/

/* `#START CDC_CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */


/*******************************************************************************
* Function Name: USBUART_DispatchCDCClassRqst
********************************************************************************
*
* Summary:
*  This routine dispatches CDC class requests.
*
* Parameters:
*  None.
*
* Return:
*  requestHandled
*
* Global variables:
*   USBUART_lineCoding: Contains the current line coding structure.
*     It is set by the Host using SET_LINE_CODING request and returned to the
*     user code by the USBFS_GetDTERate(), USBFS_GetCharFormat(),
*     USBFS_GetParityType(), USBFS_GetDataBits() APIs.
*   USBUART_lineControlBitmap: Contains the current control signal
*     bitmap. It is set by the Host using SET_CONTROL_LINE request and returned
*     to the user code by the USBFS_GetLineControl() API.
*   USBUART_lineChanged: This variable is used as a flag for the
*     USBFS_IsLineChanged() API, to be aware that Host has been sent request
*     for changing Line Coding or Control Bitmap.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 USBUART_DispatchCDCClassRqst(void) 
{
    uint8 requestHandled = USBUART_FALSE;

    if ((CY_GET_REG8(USBUART_bmRequestType) & USBUART_RQST_DIR_MASK) == USBUART_RQST_DIR_D2H)
    {   /* Control Read */
        switch (CY_GET_REG8(USBUART_bRequest))
        {
            case USBUART_CDC_GET_LINE_CODING:
                USBUART_currentTD.count = USBUART_LINE_CODING_SIZE;
                USBUART_currentTD.pData = USBUART_lineCoding;
                requestHandled  = USBUART_InitControlRead();
                break;

            /* `#START CDC_READ_REQUESTS` Place other request handler here */

            /* `#END` */

            default:    /* requestHandled is initialized as FALSE by default */
                break;
        }
    }
    else if ((CY_GET_REG8(USBUART_bmRequestType) & USBUART_RQST_DIR_MASK) == \
                                                                            USBUART_RQST_DIR_H2D)
    {   /* Control Write */
        switch (CY_GET_REG8(USBUART_bRequest))
        {
            case USBUART_CDC_SET_LINE_CODING:
                USBUART_currentTD.count = USBUART_LINE_CODING_SIZE;
                USBUART_currentTD.pData = USBUART_lineCoding;
                USBUART_lineChanged |= USBUART_LINE_CODING_CHANGED;
                requestHandled = USBUART_InitControlWrite();
                break;

            case USBUART_CDC_SET_CONTROL_LINE_STATE:
                USBUART_lineControlBitmap = CY_GET_REG8(USBUART_wValueLo);
                USBUART_lineChanged |= USBUART_LINE_CONTROL_CHANGED;
                requestHandled = USBUART_InitNoDataControlTransfer();
                break;

            /* `#START CDC_WRITE_REQUESTS` Place other request handler here */

            /* `#END` */

            default:    /* requestHandled is initialized as FALSE by default */
                break;
        }
    }
    else
    {   /* requestHandled is initialized as FALSE by default */
    }

    return(requestHandled);
}


/***************************************
* Optional CDC APIs
***************************************/
#if (USBUART_ENABLE_CDC_CLASS_API != 0u)


    /*******************************************************************************
    * Function Name: USBUART_CDC_Init
    ********************************************************************************
    *
    * Summary:
    *  This function initialize the CDC interface to be ready for the receive data
    *  from the PC.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *   USBUART_lineChanged: Initialized to zero.
    *   USBUART_cdc_data_out_ep: Used as an OUT endpoint number.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void USBUART_CDC_Init(void) 
    {
        USBUART_lineChanged = 0u;
        USBUART_EnableOutEP(USBUART_cdc_data_out_ep);
    }


    /*******************************************************************************
    * Function Name: USBUART_PutData
    ********************************************************************************
    *
    * Summary:
    *  Sends a specified number of bytes from the location specified by a
    *  pointer to the PC.
    *
    * Parameters:
    *  pData: pointer to the buffer containing data to be sent.
    *  length: Specifies the number of bytes to send from the pData
    *  buffer. Maximum length will be limited by the maximum packet
    *  size for the endpoint.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *   USBUART_cdc_data_in_ep: CDC IN endpoint number used for sending
    *     data.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void USBUART_PutData(const uint8* pData, uint16 length) 
    {
        /* Limits length to maximum packet size for the EP */
        if(length > USBUART_EP[USBUART_cdc_data_in_ep].bufferSize)
        {
            /* Caution: Data will be lost if length is greater than Max Packet Length */
            length = USBUART_EP[USBUART_cdc_data_in_ep].bufferSize;
             /* Halt CPU in debug mode */
            CYASSERT(0u != 0u);
        }
        USBUART_LoadInEP(USBUART_cdc_data_in_ep, pData, length);
    }


    /*******************************************************************************
    * Function Name: USBUART_StrLen
    ********************************************************************************
    *
    * Summary:
    *  Calculates length of a null terminated string.
    *
    * Parameters:
    *  string: pointer to the string.
    *
    * Return:
    *  Length of the string
    *
    *******************************************************************************/
    static uint16 USBUART_StrLen(const char8 string[]) 
    {
        uint16 len = 0u;

        while (string[len] != (char8)0)
        {
            len++;
        }

        return (len);
    }


    /*******************************************************************************
    * Function Name: USBUART_PutString
    ********************************************************************************
    *
    * Summary:
    *  Sends a null terminated string to the PC.
    *
    * Parameters:
    *  string: pointer to the string to be sent to the PC
    *
    * Return:
    *  None.
    *
    * Global variables:
    *   USBUART_cdc_data_in_ep: CDC IN endpoint number used for sending
    *     data.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  This function will block if there is not enough memory to place the whole
    *  string, it will block until the entire string has been written to the
    *  transmit buffer.
    *
    *******************************************************************************/
    void USBUART_PutString(const char8 string[]) 
    {
        uint16 str_length;
        uint16 send_length;
        uint16 buf_index = 0u;

        /* Get length of the null terminated string */
        str_length = USBUART_StrLen(string);
        do
        {
            /* Limits length to maximum packet size for the EP */
            send_length = (str_length > USBUART_EP[USBUART_cdc_data_in_ep].bufferSize) ?
                          USBUART_EP[USBUART_cdc_data_in_ep].bufferSize : str_length;
             /* Enable IN transfer */
            USBUART_LoadInEP(USBUART_cdc_data_in_ep, (const uint8 *)&string[buf_index], send_length);
            str_length -= send_length;

            /* If more data are present to send */
            if(str_length > 0u)
            {
                buf_index += send_length;
                /* Wait for the Host to read it. */
                while(USBUART_EP[USBUART_cdc_data_in_ep].apiEpState ==
                                          USBUART_IN_BUFFER_FULL)
                {
                    ;
                }
            }
        }while(str_length > 0u);
    }


    /*******************************************************************************
    * Function Name: USBUART_PutChar
    ********************************************************************************
    *
    * Summary:
    *  Writes a single character to the PC.
    *
    * Parameters:
    *  txDataByte: Character to be sent to the PC.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *   USBUART_cdc_data_in_ep: CDC IN endpoint number used for sending
    *     data.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void USBUART_PutChar(char8 txDataByte) 
    {
        uint8 dataByte;
        dataByte = (uint8)txDataByte;

        USBUART_LoadInEP(USBUART_cdc_data_in_ep, &dataByte, 1u);
    }


    /*******************************************************************************
    * Function Name: USBUART_PutCRLF
    ********************************************************************************
    *
    * Summary:
    *  Sends a carriage return (0x0D) and line feed (0x0A) to the PC
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global variables:
    *   USBUART_cdc_data_in_ep: CDC IN endpoint number used for sending
    *     data.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void USBUART_PutCRLF(void) 
    {
        const uint8 CYCODE txData[] = {0x0Du, 0x0Au};

        USBUART_LoadInEP(USBUART_cdc_data_in_ep, (const uint8 *)txData, 2u);
    }


    /*******************************************************************************
    * Function Name: USBUART_GetCount
    ********************************************************************************
    *
    * Summary:
    *  This function returns the number of bytes that were received from the PC.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Returns the number of received bytes.
    *
    * Global variables:
    *   USBUART_cdc_data_out_ep: CDC OUT endpoint number used.
    *
    *******************************************************************************/
    uint16 USBUART_GetCount(void) 
    {
        uint16 bytesCount = 0u;

        if (USBUART_EP[USBUART_cdc_data_out_ep].apiEpState == USBUART_OUT_BUFFER_FULL)
        {
            bytesCount = USBUART_GetEPCount(USBUART_cdc_data_out_ep);
        }

        return(bytesCount);
    }


    /*******************************************************************************
    * Function Name: USBUART_DataIsReady
    ********************************************************************************
    *
    * Summary:
    *  Returns a nonzero value if the component received data or received
    *  zero-length packet. The GetAll() or GetData() API should be called to read
    *  data from the buffer and re-init OUT endpoint even when zero-length packet
    *  received.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  If the OUT packet received this function returns a nonzero value.
    *  Otherwise zero is returned.
    *
    * Global variables:
    *   USBUART_cdc_data_out_ep: CDC OUT endpoint number used.
    *
    *******************************************************************************/
    uint8 USBUART_DataIsReady(void) 
    {
        return(USBUART_EP[USBUART_cdc_data_out_ep].apiEpState);
    }


    /*******************************************************************************
    * Function Name: USBUART_CDCIsReady
    ********************************************************************************
    *
    * Summary:
    *  Returns a nonzero value if the component is ready to send more data to the
    *  PC. Otherwise returns zero. Should be called before sending new data to
    *  ensure the previous data has finished sending.This function returns the
    *  number of bytes that were received from the PC.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  If the buffer can accept new data then this function returns a nonzero value.
    *  Otherwise zero is returned.
    *
    * Global variables:
    *   USBUART_cdc_data_in_ep: CDC IN endpoint number used.
    *
    *******************************************************************************/
    uint8 USBUART_CDCIsReady(void) 
    {
        return(USBUART_EP[USBUART_cdc_data_in_ep].apiEpState);
    }


    /*******************************************************************************
    * Function Name: USBUART_GetData
    ********************************************************************************
    *
    * Summary:
    *  Gets a specified number of bytes from the input buffer and places it in a
    *  data array specified by the passed pointer.
    *  USBUART_DataIsReady() API should be called before, to be sure
    *  that data is received from the Host.
    *
    * Parameters:
    *  pData: Pointer to the data array where data will be placed.
    *  Length: Number of bytes to read into the data array from the RX buffer.
    *          Maximum length is limited by the the number of received bytes.
    *
    * Return:
    *  Number of bytes received.
    *
    * Global variables:
    *   USBUART_cdc_data_out_ep: CDC OUT endpoint number used.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 USBUART_GetData(uint8* pData, uint16 length) 
    {
        return(USBUART_ReadOutEP(USBUART_cdc_data_out_ep, pData, length));
    }


    /*******************************************************************************
    * Function Name: USBUART_GetAll
    ********************************************************************************
    *
    * Summary:
    *  Gets all bytes of received data from the input buffer and places it into a
    *  specified data array. USBUART_DataIsReady() API should be called
    *  before, to be sure that data is received from the Host.
    *
    * Parameters:
    *  pData: Pointer to the data array where data will be placed.
    *
    * Return:
    *  Number of bytes received.
    *
    * Global variables:
    *   USBUART_cdc_data_out_ep: CDC OUT endpoint number used.
    *   USBUART_EP[].bufferSize: EP max packet size is used as a length
    *     to read all data from the EP buffer.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 USBUART_GetAll(uint8* pData) 
    {
        return (USBUART_ReadOutEP(USBUART_cdc_data_out_ep, pData,
                                           USBUART_EP[USBUART_cdc_data_out_ep].bufferSize));
    }


    /*******************************************************************************
    * Function Name: USBUART_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Reads one byte of received data from the buffer.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Received one character.
    *
    * Global variables:
    *   USBUART_cdc_data_out_ep: CDC OUT endpoint number used.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 USBUART_GetChar(void) 
    {
         uint8 rxData;

        (void) USBUART_ReadOutEP(USBUART_cdc_data_out_ep, &rxData, 1u);

        return(rxData);
    }

    /*******************************************************************************
    * Function Name: USBUART_IsLineChanged
    ********************************************************************************
    *
    * Summary:
    *  This function returns clear on read status of the line.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  If SET_LINE_CODING or CDC_SET_CONTROL_LINE_STATE request received then not
    *  zero value returned. Otherwise zero is returned.
    *
    * Global variables:
    *  USBUART_transferState - it is checked to be sure then OUT data
    *    phase has been complete, and data written to the lineCoding or Control
    *    Bitmap buffer.
    *  USBUART_lineChanged: used as a flag to be aware that Host has been
    *    sent request for changing Line Coding or Control Bitmap.
    *
    *******************************************************************************/
    uint8 USBUART_IsLineChanged(void) 
    {
        uint8 state = 0u;

        /* transferState is checked to be sure then OUT data phase has been complete */
        if(USBUART_transferState == USBUART_TRANS_STATE_IDLE)
        {
            if(USBUART_lineChanged != 0u)
            {
                state = USBUART_lineChanged;
                USBUART_lineChanged = 0u;
            }
        }

        return(state);
    }


    /*******************************************************************************
    * Function Name: USBUART_GetDTERate
    ********************************************************************************
    *
    * Summary:
    *  Returns the data terminal rate set for this port in bits per second.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Returns a uint32 value of the data rate in bits per second.
    *
    * Global variables:
    *  USBUART_lineCoding: First four bytes converted to uint32
    *    depend on compiler, and returned as a data rate.
    *
    *******************************************************************************/
    uint32 USBUART_GetDTERate(void) 
    {
        uint32 rate;

        rate = USBUART_lineCoding[USBUART_LINE_CODING_RATE + 3u];
        rate = (rate << 8u) | USBUART_lineCoding[USBUART_LINE_CODING_RATE + 2u];
        rate = (rate << 8u) | USBUART_lineCoding[USBUART_LINE_CODING_RATE + 1u];
        rate = (rate << 8u) | USBUART_lineCoding[USBUART_LINE_CODING_RATE];

        return(rate);
    }


    /*******************************************************************************
    * Function Name: USBUART_GetCharFormat
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of stop bits.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Returns the number of stop bits.
    *
    * Global variables:
    *  USBUART_lineCoding: used to get a parameter.
    *
    *******************************************************************************/
    uint8 USBUART_GetCharFormat(void) 
    {
        return(USBUART_lineCoding[USBUART_LINE_CODING_STOP_BITS]);
    }


    /*******************************************************************************
    * Function Name: USBUART_GetParityType
    ********************************************************************************
    *
    * Summary:
    *  Returns the parity type for the CDC port.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Returns the parity type.
    *
    * Global variables:
    *  USBUART_lineCoding: used to get a parameter.
    *
    *******************************************************************************/
    uint8 USBUART_GetParityType(void) 
    {
        return(USBUART_lineCoding[USBUART_LINE_CODING_PARITY]);
    }


    /*******************************************************************************
    * Function Name: USBUART_GetDataBits
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of data bits for the CDC port.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Returns the number of data bits.
    *  The number of data bits can be 5, 6, 7, 8 or 16.
    *
    * Global variables:
    *  USBUART_lineCoding: used to get a parameter.
    *
    *******************************************************************************/
    uint8 USBUART_GetDataBits(void) 
    {
        return(USBUART_lineCoding[USBUART_LINE_CODING_DATA_BITS]);
    }


    /*******************************************************************************
    * Function Name: USBUART_GetLineControl
    ********************************************************************************
    *
    * Summary:
    *  Returns Line control bitmap.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Returns Line control bitmap.
    *
    * Global variables:
    *  USBUART_lineControlBitmap: used to get a parameter.
    *
    *******************************************************************************/
    uint16 USBUART_GetLineControl(void) 
    {
        return(USBUART_lineControlBitmap);
    }

#endif  /* End USBUART_ENABLE_CDC_CLASS_API*/


/*******************************************************************************
* Additional user functions supporting CDC Requests
********************************************************************************/

/* `#START CDC_FUNCTIONS` Place any additional functions here */

/* `#END` */

#endif  /* End USBUART_ENABLE_CDC_CLASS*/


/* [] END OF FILE */
