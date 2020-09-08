#include <project.h>

#define FALSE 0
#define TRUE 1

#define BUF_SIZE 64

int main()
{
    uint8 usbStarted = FALSE;
    uint8 usbBuffer[BUF_SIZE];
    uint16 usbBufCount;

    CyGlobalIntEnable;
    
    /* Start USBFS Operation with 3V operation */
    USBUART_Start(0, USBUART_3V_OPERATION);

    /* Start LCD */
    Display_Start();
    
    for(;;)
    {
        if(!usbStarted)
        {
            /* Wait for Device to enumerate */
            if(USBUART_GetConfiguration())
            {
                /* Enumeration is done, enable OUT endpoint for
                receive data from Host */
                USBUART_CDC_Init();
                usbStarted = TRUE;
            }
        }
        else
        {
            /* Check for input data from PC */
            if(USBUART_DataIsReady() != 0)
            {
                /* Read received data and re-enable OUT endpoint */
                usbBufCount = USBUART_GetAll(usbBuffer);
                if(usbBufCount != 0)
                {
                    /* Wait till component is ready to send more data */ 
                    while(USBUART_CDCIsReady() == 0);
                    /* Send data back to PC */
                    USBUART_PutData(usbBuffer, usbBufCount);
                }
            } 
        }
    }
}
