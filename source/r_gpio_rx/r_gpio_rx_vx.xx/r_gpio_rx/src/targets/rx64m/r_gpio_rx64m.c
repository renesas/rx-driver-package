/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_gpio_rx64m.c
* Description  : Data for r_gpio_rx driver specific to RX64M.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 22.05.2014 1.00    First Release
*         : 02.04.2015 1.01    Removed P35 and P57 from g_gpio_pull_up_support array
*         :                    Added g_gpio_dscr_support array
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"

#if defined(BSP_MCU_RX64M)

/* Public interface header file for this package. */
#include "r_gpio_rx_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/* These arrays hold which pins have extra functionality. For example, not all pins have the option of enabling
 * open-drain N-channel output instead of the default CMOS output. Each entry in the array corresponds to a port.
 * Each bit in each entry corresponds to a pin on that port. If bit 3 of array entry [4] was set to 1 then that would
 * mean that PORT 4 PIN 3 supported the feature that array represented.
 *
 * These arrays are only used when GPIO_CFG_PARAM_CHECKING_ENABLE is set to 1 (checking enabled). If you know that
 * your code does not need to check the pins then you can set this macro to 0 and save a little execution time
 * and ROM space.
 *
 * Note: These arrays are defined for the largest package part. For smaller packages where some pins do not exist,
 *       pin checking is filtered by the enumerated port_pin list for that package as defined in r_gpio_rx64m.h.
 */
#if (GPIO_CFG_PARAM_CHECKING_ENABLE == 1)
const uint8_t g_gpio_open_drain_n_support[GPIO_INFO_NUM_PORTS] =
{
    0xAF,    // P00 to P03, P05, P07
    0xFF,    // P10 to P17
    0xFF,    // P20 to P27
    0xDF,    // P30 to P34, P36, P37
    0xFF,    // P40 to P47
    0x7F,    // P50 to P56
    0xFF,    // P60 to P67
    0xFF,    // P70 to P77
    0xCF,    // P80 to P83, P86, P87
    0xFF,    // P90 to P97
    0xFF,    // PA0 to PA7
    0xFF,    // PB0 to PB7
    0xFF,    // PC0 to PC7
    0xFF,    // PD0 to PD7
    0xFF,    // PE0 to PE7
    0x3F,    // PF0 to PF5
    0xFF,    // PG0 to PG7
    0x00,    // PORTH
    0x28,    // PJ3, PJ5
};

const uint8_t g_gpio_open_drain_p_support[GPIO_INFO_NUM_PORTS] =
{
    0x00,     //PORT0
    0x00,     //PORT1
    0x00,     //PORT2
    0x00,     //PORT3
    0x00,     //PORT4
    0x00,     //PORT5
    0x00,     //PORT6
    0x00,     //PORT7
    0x00,     //PORT8
    0x00,     //PORT9
    0x00,     //PORTA
    0x00,     //PORTB
    0x00,     //PORTC
    0x00,     //PORTD
    0x02,     //PORTE
    0x00,     //PORTF
    0x00,     //PORTG
    0x00,     //PORTH
    0x00      //PORTJ
};

const uint8_t g_gpio_pull_up_support[GPIO_INFO_NUM_PORTS] =
{
    0xAF,    // P00 to P03, P05, P07
    0xFF,    // P10 to P17
    0xFF,    // P20 to P27
    0xDF,    // P30 to P34, P36, P37
    0xFF,    // P40 to P47
    0x7F,    // P50 to P56
    0xFF,    // P60 to P67
    0xFF,    // P70 to P77
    0xCF,    // P80 to P83, P86, P87
    0xFF,    // P90 to P97
    0xFF,    // PA0 to PA7
    0xFF,    // PB0 to PB7
    0xFF,    // PC0 to PC7
    0xFF,    // PD0 to PD7
    0xFF,    // PE0 to PE7
    0x3F,    // PF0 to PF5
    0xFF,    // PG0 to PG7
    0x00,    // PORTH
    0x28,    // PJ3, PJ5
};

const uint8_t g_gpio_dscr_support[GPIO_INFO_NUM_PORTS] =
{
    0x07,    // P0: P00 to P02
    0x00,    // P1: -
    0x80,    // P2: P27
    0x00,    // P3: -
    0x00,    // P4: -
    0x47,    // P5: P50 to P52,P56
    0x00,    // P6: -
    0x00,    // P7: -
    0x00,    // P8: -
    0xFF,    // P9: P90 to P97
    0xFF,    // PA: PA0 to PA7
    0xFF,    // PB: PB0 to PB7
    0xFF,    // PC: PC0 to PC7
    0xFF,    // PD: PD0 to PD7
    0xFF,    // PE: PE0 to PE7
    0x00,    // PF: -
    0x03,    // PG: PG0,PG1
    0x00,    // PH: -
    0x00,    // PJ: -
};

#endif

#endif

