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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_gpio_rx72t.c
* Description  : Data for r_gpio_rx driver specific to RX72T.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 01.02.2019 2.50    First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"

#if defined(BSP_MCU_RX72T)

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
 *       pin checking is filtered by the enumerated port_pin list for that package as defined in r_gpio_rx72t.h.
 */
#if (GPIO_CFG_PARAM_CHECKING_ENABLE == 1)
const uint8_t g_gpio_open_drain_n_support[GPIO_INFO_NUM_PORTS] =
{
    0x03,    // P0: P00, P01
    0xFF,    // P1: P10 to P17
    0xFF,    // P2: P20 to P27
    0xFF,    // P3: P30 to P37
    0x88,    // P4: P43, P47
    0x3F,    // P5: P50 to P55
    0x3F,    // P6: P60 to P65
    0x7F,    // P7: P70 to P76
    0x07,    // P8: P80 to P82
    0x7F,    // P9: P90 to P96
    0xFF,    // PA: PA0 to PA7
    0xFF,    // PB: PB0 to PB7
    0x7F,    // PC: PC0 to PC6
    0xFF,    // PD: PD0 to PD7
    0x7B,    // PE: PE0, PE1, PE3 to PE6
    0x0F,    // PF: PF0 to PF3
    0x07,    // PG: PG0 to PG2
    0xEE,    // PH: PH1 to PH3, PH5 to PH7
    0x00,    // PJ: -
    0x07,    // PK: PK0 to PK2
};

const uint8_t g_gpio_open_drain_p_support[GPIO_INFO_NUM_PORTS] =
{
    0x00,    // P0: -
    0x00,    // P1: -
    0x00,    // P2: -
    0x00,    // P3: -
    0x00,    // P4: -
    0x00,    // P5: -
    0x00,    // P6: -
    0x00,    // P7: -
    0x00,    // P8: -
    0x00,    // P9: -
    0x00,    // PA: -
    0x00,    // PB: -
    0x00,    // PC: -
    0x00,    // PD: -
    0x00,    // PE: -
    0x00,    // PF: -
    0x00,    // PG: -
    0x00,    // PH: -
    0x00,    // PJ: -
    0x00,    // PK: -
};

const uint8_t g_gpio_pull_up_support[GPIO_INFO_NUM_PORTS] =
{
    0x03,    // P0: P00, P01
    0xFF,    // P1: P10 to P17
    0xFF,    // P2: P20 to P27
    0xFF,    // P3: P30 to P37
    0x88,    // P4: P43, P47
    0x3F,    // P5: P50 to P55
    0x3F,    // P6: P60 to P65
    0x7F,    // P7: P70 to P76
    0x07,    // P8: P80 to P82
    0x7F,    // P9: P90 to P96
    0xFF,    // PA: PA0 to PA7
    0xFF,    // PB: PB0 to PB7
    0x7F,    // PC: PC0 to PC6
    0xFF,    // PD: PD0 to PD7
    0x7B,    // PE: PE0, PE1, PE3 to PE6
    0x0F,    // PF: PF0 to PF3
    0x07,    // PG: PG0 to PG2
    0xEE,    // PH: PH1 to PH3, PH5 to PH7
    0x00,    // PJ: -
    0x07,    // PK: PK0 to PK2
};

const uint8_t g_gpio_dscr_support[GPIO_INFO_NUM_PORTS] =
{
    0x03,    // P0: P00, P01
    0xFF,    // P1: P10 to P17
    0xFF,    // P2: P20 to P27
    0x3F,    // P3: P30 to P35
    0x00,    // P4: -
    0x00,    // P5: -
    0x00,    // P6: -
    0x7F,    // P7: P70 to P76
    0x07,    // P8: P80 to P82
    0x7F,    // P9: P90 to P96
    0xFF,    // PA: PA0 to PA7
    0xFF,    // PB: PB0 to PB7
    0x7F,    // PC: PC0 to PC6
    0xFF,    // PD: PD0 to PD7
    0x7B,    // PE: PE0, PE1, PE3 to PE6
    0x0F,    // PF: PF0 to PF3
    0x07,    // PG: PG0 to PG2
    0x00,    // PH: -
    0x00,    // PJ: -
    0x07,    // PK: PK0 to PK2
};

const uint8_t g_gpio_dscr2_support[GPIO_INFO_NUM_PORTS] =
{
    0x00,    // P0: -
    0x00,    // P1: -
    0x00,    // P2: -
    0x00,    // P3: -
    0x00,    // P4: -
    0x00,    // P5: -
    0x00,    // P6: -
    0x7E,    // P7: P71 to P76
    0x02,    // P8: P81
    0x3F,    // P9: P90 to P95
    0x00,    // PA: -
    0x20,    // PB: PB5
    0x00,    // PC: -
    0x08,    // PD: PD3
    0x00,    // PE: -
    0x00,    // PF: -
    0x00,    // PG: -
    0x00,    // PH: -
    0x00,    // PJ: -
    0x00,    // PK: -
};

#endif /* GPIO_CFG_PARAM_CHECKING_ENABLE */

#endif /* BSP_MCU_RX72T */

