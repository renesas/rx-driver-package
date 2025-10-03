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
*         : 27.07.2018 1.00     First Release
*         : 28.02.2019 1.01     Modified comment.
*                               Fixed coding style.
*         : 11.02.2022 1.02     Added support for RX66T with 48 pin package.
*         : 21.11.2023 1.03     Modified comment.
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
/* RX MCUs come in different packages and different pin counts.
 * Each bit of PORTm.PDR corresponds to each pin of port m; I/O direction can be specified in 1-bit units.
 * Each bit of PDR corresponding to port m that does not exist is reserved.
 * Also, each bit of PDR corresponding to P40 to P42, P44 to P46, PH0, PH4, and PE2 pins is reserved, because such pins
 * are input only.
 * Make settings of the reserved bit according to the description in section 20.4.1, Initialization of the Port 
 * Direction Register (PDR). These values are then ORed into the direction registers to set non-existent pins as 
 * outputs or inputs, which can help save power.
 */
#if BSP_PACKAGE_PINS == 144
    /* Refer User's Manual: Hardware Table 20.7. */
    #define BSP_PRV_PORT0_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT4_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT6_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT7_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT8_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT9_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTC_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTF_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTG_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTH_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTK_NE_PIN_MASK     (0x00)
#elif BSP_PACKAGE_PINS == 112
    /* Refer User's Manual: Hardware Table 20.8. */
    #define BSP_PRV_PORT0_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x60)
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x30)
    #define BSP_PRV_PORT4_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0x03)
    #define BSP_PRV_PORT6_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT7_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT8_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT9_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0xC0)
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTC_NE_PIN_MASK     (0x78)
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0x40)
    #define BSP_PRV_PORTF_NE_PIN_MASK     (0x0F)
    #define BSP_PRV_PORTG_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTH_NE_PIN_MASK     (0xEE)
    #define BSP_PRV_PORTK_NE_PIN_MASK     (0x07)
#elif BSP_PACKAGE_PINS == 100
    /* Refer User's Manual: Hardware Table 20.9 to 20.11. */
    #define BSP_PRV_PORT0_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0xFC)
#if (0xB == BSP_CFG_MCU_PART_FUNCTION) || (0xF == BSP_CFG_MCU_PART_FUNCTION)
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0xE0)
#else
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x60)
#endif
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x30)
    #define BSP_PRV_PORT4_NE_PIN_MASK     (0x00)
#if (0xB == BSP_CFG_MCU_PART_FUNCTION) || (0xF == BSP_CFG_MCU_PART_FUNCTION)
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0x00)
#else
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0x03)
#endif
    #define BSP_PRV_PORT6_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT7_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT8_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT9_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0xC0)
#if (0xC == BSP_CFG_MCU_PART_FUNCTION) || (0x10 == BSP_CFG_MCU_PART_FUNCTION)
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0x80)
#else
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0x00)
#endif
    #define BSP_PRV_PORTC_NE_PIN_MASK     (0x7F)
#if (0xC == BSP_CFG_MCU_PART_FUNCTION) || (0x10 == BSP_CFG_MCU_PART_FUNCTION)
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0x03)
#else
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0x00)
#endif
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0x40)
    #define BSP_PRV_PORTF_NE_PIN_MASK     (0x0F)
    #define BSP_PRV_PORTG_NE_PIN_MASK     (0x07)
    #define BSP_PRV_PORTH_NE_PIN_MASK     (0xEE)
    #define BSP_PRV_PORTK_NE_PIN_MASK     (0x07)
#elif BSP_PACKAGE_PINS == 80
    /* Refer User's Manual: Hardware Table 20.12. */
    #define BSP_PRV_PORT0_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0xFC)
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x78)
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x3C)
    #define BSP_PRV_PORT4_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0x03)
    #define BSP_PRV_PORT6_NE_PIN_MASK     (0x0B)
    #define BSP_PRV_PORT7_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT8_NE_PIN_MASK     (0x07)
    #define BSP_PRV_PORT9_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0xD7)
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0x80)
    #define BSP_PRV_PORTC_NE_PIN_MASK     (0x7F)
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0x03)
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0x63)
    #define BSP_PRV_PORTF_NE_PIN_MASK     (0x0F)
    #define BSP_PRV_PORTG_NE_PIN_MASK     (0x07)
    #define BSP_PRV_PORTH_NE_PIN_MASK     (0xEE)
    #define BSP_PRV_PORTK_NE_PIN_MASK     (0x07)
#elif BSP_PACKAGE_PINS == 64
    /* Refer User's Manual: Hardware Table 20.13. */
    #define BSP_PRV_PORT0_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0xFD)
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0xF8)
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x3F)
    #define BSP_PRV_PORT4_NE_PIN_MASK     (0x88)
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0x23)
    #define BSP_PRV_PORT6_NE_PIN_MASK     (0x0F)
    #define BSP_PRV_PORT7_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT8_NE_PIN_MASK     (0x07)
    #define BSP_PRV_PORT9_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0xFF)
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0x80)
    #define BSP_PRV_PORTC_NE_PIN_MASK     (0x7F)
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0x07)
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0x7B)
    #define BSP_PRV_PORTF_NE_PIN_MASK     (0x0F)
    #define BSP_PRV_PORTG_NE_PIN_MASK     (0x07)
    #define BSP_PRV_PORTH_NE_PIN_MASK     (0xEE)
    #define BSP_PRV_PORTK_NE_PIN_MASK     (0x07)
#elif BSP_PACKAGE_PINS == 48
    /* Refer User's Manual: Hardware Table 20.14. */
    #define BSP_PRV_PORT0_NE_PIN_MASK     (0x02)
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0xFC)
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0xFF)
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x3F)
    #define BSP_PRV_PORT4_NE_PIN_MASK     (0x80)
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0x3F)
    #define BSP_PRV_PORT6_NE_PIN_MASK     (0x0B)
    #define BSP_PRV_PORT7_NE_PIN_MASK     (0x01)
    #define BSP_PRV_PORT8_NE_PIN_MASK     (0x07)
    #define BSP_PRV_PORT9_NE_PIN_MASK     (0x6F)
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0xD7)
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0x80)
    #define BSP_PRV_PORTC_NE_PIN_MASK     (0x7F)
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0x57)
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0x7B)
    #define BSP_PRV_PORTF_NE_PIN_MASK     (0x0F)
    #define BSP_PRV_PORTG_NE_PIN_MASK     (0x07)
    #define BSP_PRV_PORTH_NE_PIN_MASK     (0xEE)
    #define BSP_PRV_PORTK_NE_PIN_MASK     (0x07)
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

    /* Set PORTC.PDR */
    PORTC.PDR.BYTE |= BSP_PRV_PORTC_NE_PIN_MASK;

    /* Set PORTD.PDR */
    PORTD.PDR.BYTE |= BSP_PRV_PORTD_NE_PIN_MASK;

    /* Set PORTE.PDR */
    PORTE.PDR.BYTE |= BSP_PRV_PORTE_NE_PIN_MASK;

    /* Set PORTF.PDR */
    PORTF.PDR.BYTE |= BSP_PRV_PORTF_NE_PIN_MASK;

    /* Set PORTG.PDR */
    PORTG.PDR.BYTE |= BSP_PRV_PORTG_NE_PIN_MASK;

    /* Set PORTH.PDR */
    PORTH.PDR.BYTE |= BSP_PRV_PORTH_NE_PIN_MASK;

    /* Set PORTK.PDR */
    PORTK.PDR.BYTE |= BSP_PRV_PORTK_NE_PIN_MASK;
} /* End of function bsp_non_existent_port_init() */

