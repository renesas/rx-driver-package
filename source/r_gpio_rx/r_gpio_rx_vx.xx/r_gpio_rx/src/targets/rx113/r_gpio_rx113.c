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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_gpio_rx113.c
* Description  : Data for r_gpio_rx driver specific to RX113.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 18.02.2014 1.00    First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"

#if defined(BSP_MCU_RX113)

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
 */
#if (GPIO_CFG_PARAM_CHECKING_ENABLE == 1)
const uint8_t g_gpio_open_drain_n_support[GPIO_INFO_NUM_PORTS] =
{
    0x94,     //PORT0   P02, P04, P07
    0xFF,     //PORT1   P10 to P17
    0xFF,     //PORT2   P20 to P27
    0x07,     //PORT3   P30 to P32
    0x00,     //PORT4   None
    0x4F,     //PORT5   P50 to P53, P56
    0x00,     //PORT6
    0x00,     //PORT7
    0x00,     //PORT8
    0x00,     //PORT9   None
    0xFF,     //PORTA   PA0 to PA7
    0xFF,     //PORTB   PB0 to PB7
    0xFF,     //PORTC   PC0 to PC7
    0x00,     //PORTD   None
    0xFF,     //PORTE   PE0 to PE7
    0x00,     //PORTF   None
    0x00,     //PORTG
    0x00,     //PORTH   None
    0x08,     //PORTJ   PJ3
};

const uint8_t g_gpio_open_drain_p_support[GPIO_INFO_NUM_PORTS] =
{
    0x00,     //PORT0
    0x11,     //PORT1   P10, P14
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
    0x02,     //PORTE   PE1
    0x00,     //PORTF
    0x00,     //PORTG
    0x00,     //PORTH
    0x00,     //PORTJ
};

const uint8_t g_gpio_pull_up_support[GPIO_INFO_NUM_PORTS] =
{
    0x94,     //PORT0   P02, P04, P07
    0xFF,     //PORT1   P10 to P17
    0xFF,     //PORT2   P20 to P27
    0x07,     //PORT3   P30 to P32
    0x00,     //PORT4   None
    0x7F,     //PORT5   P50 to P56
    0x00,     //PORT6
    0x00,     //PORT7
    0x00,     //PORT8
    0x00,     //PORT9   None
    0xFF,     //PORTA   PA0 to PA7
    0xFF,     //PORTB   PB0 to PB7
    0xFF,     //PORTC   PC0 to PC7
    0x1F,     //PORTD   PD0 to PD4
    0xFF,     //PORTE   PE0 to PE7
    0xC0,     //PORTF   PF6, PF7
    0x00,     //PORTG
    0x00,     //PORTH   None
    0x0F,     //PORTJ   PJ0 to PJ3
};
#endif

#endif

