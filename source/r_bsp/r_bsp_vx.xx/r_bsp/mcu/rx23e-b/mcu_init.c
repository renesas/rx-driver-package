/*
* Copyright (c) 2011 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : mcu_init.c
* Description  : Performs initialization common to all MCUs in this Group
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 10.03.2023 1.00     First Release
*         : 26.02.2025 1.01     Changed the disclaimer.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Get specifics on this MCU. */
#include "platform.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* RX MCUs come in different packages and different pin counts. 'Non-existent' pins that are not bonded out
 * need to be initialized to save power. The macros below define the non-existent pins on each port. If a pin
 * is non-existent then its value is set to a 1. These values are then ORed into the
 * direction registers to set non-existent pins as outputs which can help save power.
 */
#if   (BSP_PACKAGE_PINS == 100)
    /* HW Manual Table 20.3 */
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0x03)
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x1C)
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0xCF)
    #define BSP_PRV_PORT6_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT7_NE_PIN_MASK     (0xE0)
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0xE0)
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0xFC)
    #define BSP_PRV_PORTC_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0xE0)
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0xE0)
#elif (BSP_PACKAGE_PINS == 80)
    /* HW Manual Table 20.4 */
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0x0F)
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x03)
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x1C)
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0xCF)
    #define BSP_PRV_PORT6_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT7_NE_PIN_MASK     (0xFF)
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0xE0)
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0xFC)
    #define BSP_PRV_PORTC_NE_PIN_MASK     (0x04)
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0xFF)
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0xE0)
#elif (BSP_PACKAGE_PINS == 64)
    /* HW Manual Table 20.5 and 20.6 */
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0x0F)
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x3F)
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x1C)
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0xCF)
    #define BSP_PRV_PORT6_NE_PIN_MASK     (0x0F)
    #define BSP_PRV_PORT7_NE_PIN_MASK     (0xFF)
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0xFF)
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0xFC)
    #define BSP_PRV_PORTC_NE_PIN_MASK     (0x04)
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0xFF)
  #if (BSP_CFG_MCU_PART_VERSION == 0x14) || (BSP_CFG_MCU_PART_VERSION == 0x15)
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0xE0)
  #else
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0xFC)
  #endif
#elif (BSP_PACKAGE_PINS == 48)
    /* HW Manual Table 20.7 */
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0x0F)
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x3F)
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x1C)
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0xCF)
    #define BSP_PRV_PORT6_NE_PIN_MASK     (0x3F)
    #define BSP_PRV_PORT7_NE_PIN_MASK     (0xFF)
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0xFF)
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0xFC)
    #define BSP_PRV_PORTC_NE_PIN_MASK     (0x7F)
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0xFF)
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0xFF)
#elif (BSP_PACKAGE_PINS == 40)
    /* HW Manual Table 20.8 and 20.9 */
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0x0F)
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x3F)
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x1C)
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0xFF)
    #define BSP_PRV_PORT6_NE_PIN_MASK     (0xFF)
    #define BSP_PRV_PORT7_NE_PIN_MASK     (0xFF)
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0xFF)
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0xFC)
    #define BSP_PRV_PORTC_NE_PIN_MASK     (0x7F)
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0xFF)
  #if (BSP_CFG_MCU_PART_VERSION == 0x14) || (BSP_CFG_MCU_PART_VERSION == 0x15)
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0xFF)
  #else
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0xFE)
  #endif
#else
    #error "ERROR - This package is not defined in mcu_init.c"
#endif

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: bsp_non_existent_port_init
* Description  : For MCUs that do not have the maximum number of pins for their group (e.g. MCU with 100 pins when
*                maximum is 144 pins) these 'non-existent' pins that are not bonded out need to be initialized to save
*                power.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void bsp_non_existent_port_init (void)
{
    /* OR in missing pin masks from above. */

    /* Set PORT1.PDR */
    PORT1.PDR.BYTE |= BSP_PRV_PORT1_NE_PIN_MASK;

    /* Set PORT2.PDR */
    PORT2.PDR.BYTE |= BSP_PRV_PORT2_NE_PIN_MASK;

    /* Set PORT3.PDR */
    PORT3.PDR.BYTE |= BSP_PRV_PORT3_NE_PIN_MASK;

    /* Set PORT5.PDR */
    PORT5.PDR.BYTE |= BSP_PRV_PORT5_NE_PIN_MASK;

    /* Set PORT6.PDR */
    PORT6.PDR.BYTE |= BSP_PRV_PORT6_NE_PIN_MASK;

    /* Set PORT7.PDR */
    PORT7.PDR.BYTE |= BSP_PRV_PORT7_NE_PIN_MASK;

    /* Set PORTA.PDR */
    PORTA.PDR.BYTE |= BSP_PRV_PORTA_NE_PIN_MASK;

    /* Set PORTB.PDR */
    PORTB.PDR.BYTE |= BSP_PRV_PORTB_NE_PIN_MASK;

    /* Set PORTC.PDR */
    PORTC.PDR.BYTE |= BSP_PRV_PORTC_NE_PIN_MASK;

    /* Set PORTD.PDR */
    PORTD.PDR.BYTE |= BSP_PRV_PORTD_NE_PIN_MASK;

    /* Set PORTE.PDR */
    PORTE.PDR.BYTE |= BSP_PRV_PORTE_NE_PIN_MASK;
} /* End of function bsp_non_existent_port_init() */

