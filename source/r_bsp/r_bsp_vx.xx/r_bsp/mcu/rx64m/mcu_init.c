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
*         : 07.08.2013 1.00     First Release
*         : 28.02.2019 1.01     Fixed coding style.
*         : 26.02.2025 1.02     Changed the disclaimer.
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
 * for their group (e.g. MCU with 100 pins when maximum is 144 pins) these 'non-existent' pins that are not bonded out
 * need to be initialized to save power. The macros below define the non-existent pins on each port for smaller
 * pin count MCUs. If a pin is non-existent then its value is set to a 1. These values are then ORed into the
 * direction registers to set non-existent pins as outputs which can help save power.
 */
#if   ((BSP_PACKAGE_PINS == 176) || (BSP_PACKAGE_PINS == 177))
    #define BSP_PRV_PORT0_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT4_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0x70)    /* Missing pins: P54 P55 P56 */
    #define BSP_PRV_PORT6_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT7_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT8_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT9_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORTC_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORTF_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORTG_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORTJ_NE_PIN_MASK     (0x00)    /* Missing pins: None */
#elif ((BSP_PACKAGE_PINS == 145) || (BSP_PACKAGE_PINS == 144))
    #define BSP_PRV_PORT0_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0x03)    /* Missing pins: P10 P11 */
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT4_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT6_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT7_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT8_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT9_NE_PIN_MASK     (0xF0)    /* Missing pins: P94 P95 P96 P97 */
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORTC_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORTF_NE_PIN_MASK     (0x1F)    /* Missing pins: PF0 PF1 PF2 PF3 PF4 */
    #define BSP_PRV_PORTG_NE_PIN_MASK     (0xFF)    /* Missing pins: PG0 PG1 PG2 PG3 PG4 PG5 PG6 PG7 */
    #define BSP_PRV_PORTJ_NE_PIN_MASK     (0x00)    /* Missing pins: None */
#elif BSP_PACKAGE_PINS == 100
    #define BSP_PRV_PORT0_NE_PIN_MASK     (0x0F)    /* Missing pins: P00 P01 P02 P03 */
    #define BSP_PRV_PORT1_NE_PIN_MASK     (0x03)    /* Missing pins: P10 P11 */
    #define BSP_PRV_PORT2_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT3_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT4_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORT5_NE_PIN_MASK     (0x40)    /* Missing pins: P56 */
    #define BSP_PRV_PORT6_NE_PIN_MASK     (0xFF)    /* Missing pins: P60 P61 P62 P63 P64 P65 P66 P67 */
    #define BSP_PRV_PORT7_NE_PIN_MASK     (0xFF)    /* Missing pins: P70 P71 P72 P73 P74 P75 P76 P77 */
    #define BSP_PRV_PORT8_NE_PIN_MASK     (0xCF)    /* Missing pins: P80 P81 P82 P83 P86 P87 */
    #define BSP_PRV_PORT9_NE_PIN_MASK     (0xFF)    /* Missing pins: P90 P91 P92 P93 P94 P95 P96 P97 */
    #define BSP_PRV_PORTA_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORTB_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORTC_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORTD_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORTE_NE_PIN_MASK     (0x00)    /* Missing pins: None */
    #define BSP_PRV_PORTF_NE_PIN_MASK     (0x3F)    /* Missing pins: PF0 PF1 PF2 PF3 PF4 PF5 */
    #define BSP_PRV_PORTG_NE_PIN_MASK     (0xFF)    /* Missing pins: PG0 PG1 PG2 PG3 PG4 PG5 PG6 PG7 */
    #define BSP_PRV_PORTJ_NE_PIN_MASK     (0x20)    /* Missing pins: PJ5 */
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

    /* Set PORTJ.PDR */
    PORTJ.PDR.BYTE |= BSP_PRV_PORTJ_NE_PIN_MASK;
} /* End of function bsp_non_existent_port_init() */

