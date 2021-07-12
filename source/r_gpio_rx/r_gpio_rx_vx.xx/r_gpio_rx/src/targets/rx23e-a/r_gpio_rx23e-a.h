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
* Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_gpio_rx23e-a.h
* Description  : Specifics for the r_gpio_rx driver for the RX23E-A.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 31.03.2020 1.00    First Release
***********************************************************************************************************************/
#ifndef GPIO_RX23E_A
#define GPIO_RX23E_A

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"
#if defined(BSP_MCU_RX23E_A)  /* Prevents the compiler from finding multiple definitions of constant in this file. */

/* Configuration for this package. */
#include "r_gpio_rx_config.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* General information about number of ports and pins on this device. */
#define GPIO_INFO_NUM_PORTS                 (18)

#if (BSP_PACKAGE_PINS == 48)
    #define GPIO_INFO_NUM_PINS              (21)
#elif (BSP_PACKAGE_PINS == 40)
    #define GPIO_INFO_NUM_PINS              (17)
#else
    #error "r_gpio_rx does not have information about this RX23E-A package. Please update r_gpio_rx23e-a.h"
#endif

/* Base registers used for offsets on output data registers. */
#define     GPIO_PRV_BASE_ADDR_OUTPUT           (((uint8_t volatile *)&PORT1.PODR.BYTE) - 1)
/* Base registers used for offsets on input data registers. */
#define     GPIO_PRV_BASE_ADDR_INPUT            (((uint8_t volatile *)&PORT1.PIDR.BYTE) - 1)
/* Base registers used for offsets on direction registers. */
#define     GPIO_PRV_BASE_ADDR_DIRECTION        (((uint8_t volatile *)&PORT1.PDR.BYTE)  - 1)
/* Base registers used for offsets on mode registers. */
#define     GPIO_PRV_BASE_ADDR_MODE             (((uint8_t volatile *)&PORT1.PMR.BYTE)  - 1)
/* Base registers used for offsets on output type registers. */
#define     GPIO_PRV_BASE_ADDR_OUT_TYPE         (((uint8_t volatile *)&PORT3.ODR0.BYTE) - 6)
/* Base registers used for offsets on pull-up registers. */
#define     GPIO_PRV_BASE_ADDR_PULL_UP          (((uint8_t volatile *)&PORT1.PCR.BYTE)  - 1)
/* Base registers used for offsets on drive capacity control registers. */
#define     GPIO_PRV_BASE_ADDR_DSCR             (((uint8_t volatile *)&PORT1.DSCR.BYTE) - 1)

#define GPIO_DSCR_IS_SUPPORTED    /* High-drive is supported for the RX13T */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
#if (BSP_PACKAGE_PINS == 48)
/* This enumerator has each available GPIO port on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_1 = 0x0100,
    GPIO_PORT_2 = 0x0200,
    GPIO_PORT_3 = 0x0300,
    GPIO_PORT_B = 0x0B00,
    GPIO_PORT_C = 0x0C00,
    GPIO_PORT_H = 0x1100,
} gpio_port_t;

/* This enumerator has a bit mask for each available GPIO pin for the given port on this MCU. */
typedef enum
{
    GPIO_PORT1_PIN_MASK = 0xF0,    /* Available pins: P14 to P17               */
    GPIO_PORT2_PIN_MASK = 0xC0,    /* Available pins: P26 to P27               */
    GPIO_PORT3_PIN_MASK = 0xE3,    /* Available pins: P30 to P31, P35 to P37   */
    GPIO_PORTB_PIN_MASK = 0x03,    /* Available pins: PB0 to PB1               */
    GPIO_PORTC_PIN_MASK = 0xF0,    /* Available pins: PC4 to PC7               */
    GPIO_PORTH_PIN_MASK = 0x0F,    /* Available pins: PH0 to PH3               */
} gpio_pin_bit_mask_t;

/* This enumerator has each available GPIO pin on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_1_PIN_4 = 0x0104,
    GPIO_PORT_1_PIN_5 = 0x0105,
    GPIO_PORT_1_PIN_6 = 0x0106,
    GPIO_PORT_1_PIN_7 = 0x0107,
    GPIO_PORT_2_PIN_6 = 0x0206,
    GPIO_PORT_2_PIN_7 = 0x0207,
    GPIO_PORT_3_PIN_0 = 0x0300,
    GPIO_PORT_3_PIN_1 = 0x0301,
    GPIO_PORT_3_PIN_5 = 0x0305,
    GPIO_PORT_3_PIN_6 = 0x0306,
    GPIO_PORT_3_PIN_7 = 0x0307,
    GPIO_PORT_B_PIN_0 = 0x0B00,
    GPIO_PORT_B_PIN_1 = 0x0B01,
    GPIO_PORT_C_PIN_4 = 0x0C04,
    GPIO_PORT_C_PIN_5 = 0x0C05,
    GPIO_PORT_C_PIN_6 = 0x0C06,
    GPIO_PORT_C_PIN_7 = 0x0C07,
    GPIO_PORT_H_PIN_0 = 0x1100,
    GPIO_PORT_H_PIN_1 = 0x1101,
    GPIO_PORT_H_PIN_2 = 0x1102,
    GPIO_PORT_H_PIN_3 = 0x1103,
} gpio_port_pin_t;

#elif (BSP_PACKAGE_PINS == 40)
/* This enumerator has each available GPIO port on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
	GPIO_PORT_1 = 0x0100,
	GPIO_PORT_2 = 0x0200,
	GPIO_PORT_3 = 0x0300,
	GPIO_PORT_B = 0x0B00,
	GPIO_PORT_C = 0x0C00,
	GPIO_PORT_H = 0x1100,
} gpio_port_t;

/* This enumerator has a bit mask for each available GPIO pin for the given port on this MCU. */
typedef enum
{
    GPIO_PORT1_PIN_MASK = 0xF0,    /* Available pins: P14 to P17               */
    GPIO_PORT2_PIN_MASK = 0xC0,    /* Available pins: P26 to P27               */
    GPIO_PORT3_PIN_MASK = 0xE3,    /* Available pins: P30 to P31, P35 to P37   */
    GPIO_PORTB_PIN_MASK = 0x03,    /* Available pins: PB0 to PB1               */
    GPIO_PORTC_PIN_MASK = 0x30,    /* Available pins: PC4 to PC5               */
    GPIO_PORTH_PIN_MASK = 0x03,    /* Available pins: PH0 to PH1               */
} gpio_pin_bit_mask_t;

/* This enumerator has each available GPIO pin on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_1_PIN_4 = 0x0104,
    GPIO_PORT_1_PIN_5 = 0x0105,
    GPIO_PORT_1_PIN_6 = 0x0106,
    GPIO_PORT_1_PIN_7 = 0x0107,
    GPIO_PORT_2_PIN_6 = 0x0206,
    GPIO_PORT_2_PIN_7 = 0x0207,
    GPIO_PORT_3_PIN_0 = 0x0300,
    GPIO_PORT_3_PIN_1 = 0x0301,
    GPIO_PORT_3_PIN_5 = 0x0305,
    GPIO_PORT_3_PIN_6 = 0x0306,
    GPIO_PORT_3_PIN_7 = 0x0307,
    GPIO_PORT_B_PIN_0 = 0x0B00,
    GPIO_PORT_B_PIN_1 = 0x0B01,
    GPIO_PORT_C_PIN_4 = 0x0C04,
    GPIO_PORT_C_PIN_5 = 0x0C05,
    GPIO_PORT_H_PIN_0 = 0x1100,
    GPIO_PORT_H_PIN_1 = 0x1101,
} gpio_port_pin_t;
#endif

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

#endif /* BSP_MCU_RX23E-A */
#endif /* GPIO_RX23E-A */
