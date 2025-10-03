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
*         : 01.02.2016 1.00     First Release
*         : 30.09.2016 1.01     Added setting for RX24T-512K
*         : 04.11.2016 1.02     Added setting for 64 Pin Package 
*         : 28.02.2019 1.03     Fixed coding style.
*         : 26.02.2025 1.04     Changed the disclaimer.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Get specifics on this MCU. */
#include "platform.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* RX MCUs come in different packages and different pin counts. For MCUs that do not have the maximum number of pins
 * for their group (e.g. MCU with 52 pins when maximum is 64 pins) these 'non-existent' pins that are not bonded out
 * need to be initialized to save power. The macros below define the non-existent pins on each port for smaller
 * pin count MCUs. If a pin is non-existent then its value is set to a 1. These values are then ORed into the
 * direction registers to set non-existent pins as outputs which can help save power.
 */
#if BSP_PACKAGE_PINS == 100
    #if (BSP_CFG_MCU_PART_MEMORY_SIZE == 0xA) || (BSP_CFG_MCU_PART_MEMORY_SIZE == 0x8)
        #define BSP_PRV_PORT0_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORT1_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORT2_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORT3_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORT4_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORT5_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORT6_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORT7_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORT8_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORT9_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORTA_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORTB_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORTD_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORTE_NE_PIN_MASK      (0x00)
    #else
        #define BSP_PRV_PORT0_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORT1_NE_PIN_MASK      (0xFC)
        #define BSP_PRV_PORT2_NE_PIN_MASK      (0xE0)
        #define BSP_PRV_PORT3_NE_PIN_MASK      (0x30)
        #define BSP_PRV_PORT4_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORT5_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORT6_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORT7_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORT8_NE_PIN_MASK      (0x18)
        #define BSP_PRV_PORT9_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORTA_NE_PIN_MASK      (0xC0)
        #define BSP_PRV_PORTB_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORTD_NE_PIN_MASK      (0x00)
        #define BSP_PRV_PORTE_NE_PIN_MASK      (0x40)

    #endif
#elif BSP_PACKAGE_PINS == 80
    #define BSP_PRV_PORT0_NE_PIN_MASK          (0x00)
    #define BSP_PRV_PORT1_NE_PIN_MASK          (0x00)
    #define BSP_PRV_PORT2_NE_PIN_MASK          (0x00)
    #define BSP_PRV_PORT3_NE_PIN_MASK          (0x0C)
    #define BSP_PRV_PORT4_NE_PIN_MASK          (0x00)
    #define BSP_PRV_PORT5_NE_PIN_MASK          (0x00)
    #define BSP_PRV_PORT6_NE_PIN_MASK          (0x3B)
    #define BSP_PRV_PORT7_NE_PIN_MASK          (0x00)
    #define BSP_PRV_PORT8_NE_PIN_MASK          (0x07)
    #define BSP_PRV_PORT9_NE_PIN_MASK          (0x00)
    #define BSP_PRV_PORTA_NE_PIN_MASK          (0x17)
    #define BSP_PRV_PORTB_NE_PIN_MASK          (0x80)
    #define BSP_PRV_PORTD_NE_PIN_MASK          (0x03)
    #define BSP_PRV_PORTE_NE_PIN_MASK          (0x23)

#else // BSP_PACKAGE_PINS == 64
    #define BSP_PRV_PORT0_NE_PIN_MASK          (0x00)
    #define BSP_PRV_PORT1_NE_PIN_MASK          (0x01)
    #define BSP_PRV_PORT2_NE_PIN_MASK          (0x01)
    #define BSP_PRV_PORT3_NE_PIN_MASK          (0x0C)
    #define BSP_PRV_PORT4_NE_PIN_MASK          (0x88)
    #define BSP_PRV_PORT5_NE_PIN_MASK          (0x20)
    #define BSP_PRV_PORT6_NE_PIN_MASK          (0x3F)
    #define BSP_PRV_PORT7_NE_PIN_MASK          (0x00)
    #define BSP_PRV_PORT8_NE_PIN_MASK          (0x07)
    #define BSP_PRV_PORT9_NE_PIN_MASK          (0x00)
    #define BSP_PRV_PORTA_NE_PIN_MASK          (0x3F)
    #define BSP_PRV_PORTB_NE_PIN_MASK          (0x81)
    #define BSP_PRV_PORTD_NE_PIN_MASK          (0x07)
    #define BSP_PRV_PORTE_NE_PIN_MASK          (0x3B)

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

    /* Set PORT0.PDR */
    PORT0.PDR.BYTE |= BSP_PRV_PORT0_NE_PIN_MASK;

    /* Set PORT1.PDR */
    PORT1.PDR.BYTE |= BSP_PRV_PORT1_NE_PIN_MASK;

    /* Set PORT2.PDR */
    PORT2.PDR.BYTE |= BSP_PRV_PORT2_NE_PIN_MASK;

    /* Set PORT3.PDR */
    PORT3.PDR.BYTE |= BSP_PRV_PORT3_NE_PIN_MASK;

    /* Set PORT4.PDR */
    PORT4.PDR.BYTE |= BSP_PRV_PORT4_NE_PIN_MASK;

    /* Set PORT5.PDR */
    PORT5.PDR.BYTE |= BSP_PRV_PORT5_NE_PIN_MASK;

    /* Set PORT6.PDR */
    PORT6.PDR.BYTE |= BSP_PRV_PORT6_NE_PIN_MASK;

    /* Set PORT7.PDR */
    PORT7.PDR.BYTE |= BSP_PRV_PORT7_NE_PIN_MASK;

    /* Set PORT8.PDR */
    PORT8.PDR.BYTE |= BSP_PRV_PORT8_NE_PIN_MASK;

    /* Set PORT9.PDR */
    PORT9.PDR.BYTE |= BSP_PRV_PORT9_NE_PIN_MASK;

    /* Set PORTA.PDR */
    PORTA.PDR.BYTE |= BSP_PRV_PORTA_NE_PIN_MASK;

    /* Set PORTB.PDR */
    PORTB.PDR.BYTE |= BSP_PRV_PORTB_NE_PIN_MASK;

    /* Set PORTD.PDR */
    PORTD.PDR.BYTE |= BSP_PRV_PORTD_NE_PIN_MASK;

    /* Set PORTE.PDR */
    PORTE.PDR.BYTE |= BSP_PRV_PORTE_NE_PIN_MASK;
} /* End of function bsp_non_existent_port_init() */

