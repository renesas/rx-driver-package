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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_gpio_rx23w.c
* Description  : Data for r_gpio_rx driver specific to RX23W.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 28.06.2019 1.00    First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"

#if defined(BSP_MCU_RX23W)

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
 *       pin checking is filtered by the enumerated port_pin list for that package as defined in r_gpio_rx23w.h.
 */

#if (GPIO_CFG_PARAM_CHECKING_ENABLE == 1)
const uint8_t g_gpio_open_drain_n_support[GPIO_INFO_NUM_PORTS] =
{
    0x00,     //PORT0   None
    0xF0,     //PORT1   P14 to P17
    0xE6,     //PORT2   P21, P22, P25 to P27
    0xC3,     //PORT3   P30, P31, P36, P37
    0x00,     //PORT4   None
    0x00,     //PORT5   None
    0x00,     //PORT6   None
    0x00,     //PORT7   None
    0x00,     //PORT8   None
    0x00,     //PORT9   None
    0x00,     //PORTA   None
    0xAB,     //PORTB   PB0, PB1, PB3, PB5, PB7
    0xFD,     //PORTC   PC0, PC2 to PC7
    0x00,     //PORTD   None
    0x1F,     //PORTE   PE0 to PE4
    0x00,     //PORTF   None
    0x00,     //PORTG   None
    0x00,     //PORTH   None
    0x08,     //PORTJ   PJ3
};

const uint8_t g_gpio_open_drain_p_support[GPIO_INFO_NUM_PORTS] =
{
    0x00,     //PORT0   None
    0x00,     //PORT1   None
    0x00,     //PORT2   None
    0x00,     //PORT3   None
    0x00,     //PORT4   None
    0x00,     //PORT5   None
    0x00,     //PORT6   None
    0x00,     //PORT7   None
    0x00,     //PORT8   None
    0x00,     //PORT9   None
    0x00,     //PORTA   None
    0x00,     //PORTB   None
    0x00,     //PORTC   None
    0x00,     //PORTD   None
    0x02,     //PORTE   PE1
    0x00,     //PORTF   None
    0x00,     //PORTG   None
    0x00,     //PORTH   None
    0x00,     //PORTJ   None
};

const uint8_t g_gpio_pull_up_support[GPIO_INFO_NUM_PORTS] =
{
    0xA8,     //PORT0   P03, P05, P07
    0xF0,     //PORT1   P14 to P17
    0xE6,     //PORT2   P21, P22, P25 to P27
    0xC3,     //PORT3   P30, P31, P36, P37
    0xFF,     //PORT4   P40 to P47
    0x00,     //PORT5   None
    0x00,     //PORT6   None
    0x00,     //PORT7   None
    0x00,     //PORT8   None
    0x00,     //PORT9   None
    0x00,     //PORTA   None
    0xAB,     //PORTB   PB0, PB1, PB3, PB5, PB7
    0xFD,     //PORTC   PC0, PC2 to PC7
    0x08,     //PORTD   PD3
    0x1F,     //PORTE   PE0 to PE4
    0x00,     //PORTF   None
    0x00,     //PORTG   None
    0x00,     //PORTH   None
    0x08,     //PORTJ   PJ3
};

const uint8_t g_gpio_dscr_support[GPIO_INFO_NUM_PORTS] =
{
    0x00,    // PORT0   None
    0xF0,    // PORT1   P14 to P17
    0xE6,    // PORT2   P21, P22, P25 to P27
    0x03,    // PORT3   P30, P31
    0x00,    // PORT4   None
    0x00,    // PORT5   None
    0x00,    // PORT6   None
    0x00,    // PORT7   None
    0x00,    // PORT8   None
    0x00,    // PORT9   None
    0x00,    // PORTA   None
    0xAB,    // PORTB   PB0, PB1, PB3, PB5, PB7
    0xFD,    // PORTC   PC0, PC2 to PC7
    0x08,    // PORTD   PD3
    0x1F,    // PORTE   PE0 to PE4
    0x00,    // PORTF   None
    0x00,    // PORTG   None
    0x00,    // PORTH   None
    0x08,    // PORTJ   PJ3
};

#endif

#endif

