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
*         : 14.02.2014 1.00     First release
*         : 27.07.2018 1.10     Corrected the initialization settings of nonexistent ports.
*         : 28.02.2019 1.11     Fixed coding style.
*         : 26.02.2025 1.12     Changed the disclaimer.
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
 * need to be initialized to save power.
 */
#if   BSP_PACKAGE_PINS == 64
    /* HW Manual Table 17.3 */
    /* Do nothing, this is the maximum number of pins. */
#elif BSP_PACKAGE_PINS == 48
    /* HW Manual Table 17.4 */
    #define BSP_PRV_PORT0_NE_PIN_MASK     (0x28)
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x07)
    #define BSP_PRV_PORT4_NE_PIN_MASK     (0x18)
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0x30)
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0x01)
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0xC0)
    #define BSP_PRV_PORTC_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0x60)
    #define BSP_PRV_PORTH_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTJ_NE_PIN_MASK     (0x00)
#elif BSP_PACKAGE_PINS == 40
    /* HW Manual Table 17.5 */
    #define BSP_PRV_PORT0_NE_PIN_MASK     (0x28)
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x03)
    #define BSP_PRV_PORT4_NE_PIN_MASK     (0x19)
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0x30)
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0x01)
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0xE2)
    #define BSP_PRV_PORTC_NE_PIN_MASK     (0xEF)
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0xE0)
    #define BSP_PRV_PORTH_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTJ_NE_PIN_MASK     (0x00)
#elif BSP_PACKAGE_PINS == 36
    /* HW Manual Table 17.6 */
    #define BSP_PRV_PORT0_NE_PIN_MASK     (0x28)
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x40)
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x07)
    #define BSP_PRV_PORT4_NE_PIN_MASK     (0x59)
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0x30)
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0x03)
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0xE2)
    #define BSP_PRV_PORTC_NE_PIN_MASK     (0xEF)
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0xE0)
    #define BSP_PRV_PORTH_NE_PIN_MASK     (0x00)
    #define BSP_PRV_PORTJ_NE_PIN_MASK     (0x00)
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
#if BSP_PACKAGE_PINS != 64
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

    /* Set PORTA.PDR */
    PORTA.PDR.BYTE |= BSP_PRV_PORTA_NE_PIN_MASK;

    /* Set PORTB.PDR */
    PORTB.PDR.BYTE |= BSP_PRV_PORTB_NE_PIN_MASK;

    /* Set PORTC.PDR */
    PORTC.PDR.BYTE |= BSP_PRV_PORTC_NE_PIN_MASK;

    /* Set PORTE.PDR */
    PORTE.PDR.BYTE |= BSP_PRV_PORTE_NE_PIN_MASK;

    /* Set PORTH.PDR */
    PORTH.PDR.BYTE |= BSP_PRV_PORTH_NE_PIN_MASK;

    /* Set PORTJ.PDR */
    PORTJ.PDR.BYTE |= BSP_PRV_PORTJ_NE_PIN_MASK;
#else
    R_BSP_NOP();
#endif
} /* End of function bsp_non_existent_port_init() */

