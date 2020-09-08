/*******************************************************************************
* File Name: Display.h
* Version 1.90
*
* Description:
*  This header file contains registers and constants associated with the
*  Character LCD component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CHARLCD_Display_H)
#define CY_CHARLCD_Display_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define Display_CONVERSION_ROUTINES     (1u)
#define Display_CUSTOM_CHAR_SET         (1u)

/* Custom character set types */
#define Display_NONE                     (0u)    /* No Custom Fonts      */
#define Display_HORIZONTAL_BG            (1u)    /* Horizontal Bar Graph */
#define Display_VERTICAL_BG              (2u)    /* Vertical Bar Graph   */
#define Display_USER_DEFINED             (3u)    /* User Defined Fonts   */


/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
} Display_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void Display_Init(void) ;
void Display_Enable(void) ;
void Display_Start(void) ;
void Display_Stop(void) ;
void Display_WriteControl(uint8 cByte) ;
void Display_WriteData(uint8 dByte) ;
void Display_PrintString(char8 const string[]) ;
void Display_Position(uint8 row, uint8 column) ;
void Display_PutChar(char8 character) ;
void Display_IsReady(void) ;
void Display_SaveConfig(void) ;
void Display_RestoreConfig(void) ;
void Display_Sleep(void) ;
void Display_Wakeup(void) ;

#if((Display_CUSTOM_CHAR_SET == Display_VERTICAL_BG) || \
                (Display_CUSTOM_CHAR_SET == Display_HORIZONTAL_BG))

    void  Display_LoadCustomFonts(uint8 const customData[])
                        ;

    void  Display_DrawHorizontalBG(uint8 row, uint8 column, uint8 maxCharacters, uint8 value)
                         ;

    void Display_DrawVerticalBG(uint8 row, uint8 column, uint8 maxCharacters, uint8 value)
                        ;

#endif /* ((Display_CUSTOM_CHAR_SET == Display_VERTICAL_BG) */

#if(Display_CUSTOM_CHAR_SET == Display_USER_DEFINED)

    void Display_LoadCustomFonts(uint8 const customData[])
                            ;

#endif /* ((Display_CUSTOM_CHAR_SET == Display_USER_DEFINED) */

#if(Display_CONVERSION_ROUTINES == 1u)

    /* ASCII Conversion Routines */
    void Display_PrintInt8(uint8 value) ;
    void Display_PrintInt16(uint16 value) ;
    void Display_PrintNumber(uint16 value) ; 

#endif /* Display_CONVERSION_ROUTINES == 1u */

/* Clear Macro */
#define Display_ClearDisplay() Display_WriteControl(Display_CLEAR_DISPLAY)

/* Off Macro */
#define Display_DisplayOff() Display_WriteControl(Display_DISPLAY_CURSOR_OFF)

/* On Macro */
#define Display_DisplayOn() Display_WriteControl(Display_DISPLAY_ON_CURSOR_OFF)


/***************************************
*           Global Variables
***************************************/

extern uint8 Display_initVar;
extern uint8 Display_enableState;
extern uint8 const CYCODE Display_customFonts[64u];


/***************************************
*           API Constants
***************************************/

/* Full Byte Commands Sent as Two Nibbles */
#define Display_DISPLAY_8_BIT_INIT       (0x03u)
#define Display_DISPLAY_4_BIT_INIT       (0x02u)
#define Display_DISPLAY_CURSOR_OFF       (0x08u)
#define Display_CLEAR_DISPLAY            (0x01u)
#define Display_CURSOR_AUTO_INCR_ON      (0x06u)
#define Display_DISPLAY_CURSOR_ON        (0x0Eu)
#define Display_DISPLAY_2_LINES_5x10     (0x2Cu)
#define Display_DISPLAY_ON_CURSOR_OFF    (0x0Cu)

#define Display_RESET_CURSOR_POSITION    (0x03u)
#define Display_CURSOR_WINK              (0x0Du)
#define Display_CURSOR_BLINK             (0x0Fu)
#define Display_CURSOR_SH_LEFT           (0x10u)
#define Display_CURSOR_SH_RIGHT          (0x14u)
#define Display_CURSOR_HOME              (0x02u)
#define Display_CURSOR_LEFT              (0x04u)
#define Display_CURSOR_RIGHT             (0x06u)

/* Point to Character Generator Ram 0 */
#define Display_CGRAM_0                  (0x40u)

/* Point to Display Data Ram 0 */
#define Display_DDRAM_0                  (0x80u)

/* LCD Characteristics */
#define Display_CHARACTER_WIDTH          (0x05u)
#define Display_CHARACTER_HEIGHT         (0x08u)

#if(Display_CONVERSION_ROUTINES == 1u)
    #define Display_NUMBER_OF_REMAINDERS (0x05u)
    #define Display_TEN                  (0x0Au)
#endif /* Display_CONVERSION_ROUTINES == 1u */

/* Nibble Offset and Mask */
#define Display_NIBBLE_SHIFT             (0x04u)
#define Display_NIBBLE_MASK              (0x0Fu)

/* LCD Module Address Constants */
#define Display_ROW_0_START              (0x80u)
#define Display_ROW_1_START              (0xC0u)
#define Display_ROW_2_START              (0x94u)
#define Display_ROW_3_START              (0xD4u)

/* Custom Character References */
#define Display_CUSTOM_0                 (0x00u)
#define Display_CUSTOM_1                 (0x01u)
#define Display_CUSTOM_2                 (0x02u)
#define Display_CUSTOM_3                 (0x03u)
#define Display_CUSTOM_4                 (0x04u)
#define Display_CUSTOM_5                 (0x05u)
#define Display_CUSTOM_6                 (0x06u)
#define Display_CUSTOM_7                 (0x07u)

/* Other constants */
#define Display_BYTE_UPPER_NIBBLE_SHIFT   (0x04u)
#define Display_BYTE_LOWER_NIBBLE_MASK    (0x0Fu)
#define Display_U16_UPPER_BYTE_SHIFT      (0x08u)
#define Display_U16_LOWER_BYTE_MASK       (0xFFu)
#define Display_CUSTOM_CHAR_SET_LEN       (0x40u)


/***************************************
*             Registers
***************************************/

/* Device specific registers */
#if (CY_PSOC4)

    #define Display_PORT_DR_REG           (*(reg32 *) Display_LCDPort__DR)  /* Data Output Register */
    #define Display_PORT_DR_PTR           ( (reg32 *) Display_LCDPort__DR)
    #define Display_PORT_PS_REG           (*(reg32 *) Display_LCDPort__PS)  /* Pin State Register */
    #define Display_PORT_PS_PTR           ( (reg32 *) Display_LCDPort__PS)
    
    #define Display_PORT_PC_REG           (*(reg32 *) Display_LCDPort__PC)
    #define Display_PORT_PC_PTR           (*(reg32 *) Display_LCDPort__PC)
    
#else

    #define Display_PORT_DR_REG           (*(reg8 *) Display_LCDPort__DR)  /* Data Output Register */
    #define Display_PORT_DR_PTR           ( (reg8 *) Display_LCDPort__DR)
    #define Display_PORT_PS_REG           (*(reg8 *) Display_LCDPort__PS)  /* Pin State Register */
    #define Display_PORT_PS_PTR           ( (reg8 *) Display_LCDPort__PS)

    #define Display_PORT_DM0_REG          (*(reg8 *) Display_LCDPort__DM0) /* Port Drive Mode 0 */
    #define Display_PORT_DM0_PTR          ( (reg8 *) Display_LCDPort__DM0)
    #define Display_PORT_DM1_REG          (*(reg8 *) Display_LCDPort__DM1) /* Port Drive Mode 1 */
    #define Display_PORT_DM1_PTR          ( (reg8 *) Display_LCDPort__DM1)
    #define Display_PORT_DM2_REG          (*(reg8 *) Display_LCDPort__DM2) /* Port Drive Mode 2 */
    #define Display_PORT_DM2_PTR          ( (reg8 *) Display_LCDPort__DM2)

#endif /* CY_PSOC4 */


/***************************************
*       Register Constants
***************************************/

/* SHIFT must be 1 or 0 */
#if (0 == Display_LCDPort__SHIFT)
    #define Display_PORT_SHIFT               (0x00u)
#else
    #define Display_PORT_SHIFT               (0x01u)
#endif /* (0 == Display_LCDPort__SHIFT) */

#define Display_PORT_MASK                ((uint8) (Display_LCDPort__MASK))

#if (CY_PSOC4)

    /* Hi-Z Digital is defined by value of "001b" and this should be set for
    * four data pins in this way we get - 0x00000249. Similar Strong drive
    * is defibed by "110b" so we get 0x00000DB6.
    */
    #define Display_HIGH_Z_DATA_DM           (0x00000249ul)
    #define Display_STRONG_DATA_DM           (0x00000DB6ul)
    #define Display_DATA_PINS_MASK           (0x00000FFFul)
    #define Display_DM_DATA_MASK             ((uint32)(Display_DATA_PINS_MASK << \
                                                          (Display_PORT_SHIFT * 3u)))
    
#else

    /* Drive Mode register values for High Z */
    #define Display_HIGH_Z_DM0               (0xFFu)
    #define Display_HIGH_Z_DM1               (0x00u)
    #define Display_HIGH_Z_DM2               (0x00u)

    /* Drive Mode register values for High Z Analog */
    #define Display_HIGH_Z_A_DM0             (0x00u)
    #define Display_HIGH_Z_A_DM1             (0x00u)
    #define Display_HIGH_Z_A_DM2             (0x00u)

    /* Drive Mode register values for Strong */
    #define Display_STRONG_DM0               (0x00u)
    #define Display_STRONG_DM1               (0xFFu)
    #define Display_STRONG_DM2               (0xFFu)

#endif /* CY_PSOC4 */

/* Pin Masks */
#define Display_RS                     ((uint8) \
                                                (((uint8) 0x20u) << Display_LCDPort__SHIFT))
#define Display_RW                     ((uint8) \
                                                (((uint8) 0x40u) << Display_LCDPort__SHIFT))
#define Display_E                      ((uint8) \
                                                (((uint8) 0x10u) << Display_LCDPort__SHIFT))
#define Display_READY_BIT              ((uint8) \
                                                (((uint8) 0x08u) << Display_LCDPort__SHIFT))
#define Display_DATA_MASK              ((uint8) \
                                                (((uint8) 0x0Fu) << Display_LCDPort__SHIFT))

/* These names are obsolete and will be removed in future revisions */
#define Display_PORT_DR                  Display_PORT_DR_REG
#define Display_PORT_PS                  Display_PORT_PS_REG
#define Display_PORT_DM0                 Display_PORT_DM0_REG
#define Display_PORT_DM1                 Display_PORT_DM1_REG
#define Display_PORT_DM2                 Display_PORT_DM2_REG


/***************************************
*       Obsolete function names
***************************************/
#if(Display_CONVERSION_ROUTINES == 1u)
    /* This function names are obsolete an they will be removed in future 
    * revisions of component.
    */
    #define Display_PrintDecUint16(x)   Display_PrintNumber(x)  
    #define Display_PrintHexUint8(x)    Display_PrintInt8(x)
    #define Display_PrintHexUint16(x)   Display_PrintInt16(x)        

#endif /* Display_CONVERSION_ROUTINES == 1u */

#endif /* CY_CHARLCD_Display_H */


/* [] END OF FILE */
