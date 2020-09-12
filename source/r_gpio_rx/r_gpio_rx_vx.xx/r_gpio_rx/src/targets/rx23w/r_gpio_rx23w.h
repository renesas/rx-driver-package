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
* File Name    : r_gpio_rx23w.h
* Description  : Specifics for the r_gpio_rx driver for the RX23W.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 28.06.2019 1.00    First Release
***********************************************************************************************************************/
#ifndef GPIO_RX23W
#define GPIO_RX23W

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"
#if defined(BSP_MCU_RX23W)  /* Prevents the compiler from finding multiple definitions of constant in this file. */

/* Configuration for this package. */
#include "r_gpio_rx_config.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* General information about number of ports and pins on this device. */
#define GPIO_INFO_NUM_PORTS                 (19)

#if (BSP_PACKAGE_PINS == 85)
    #define GPIO_INFO_NUM_PINS              (44)
#elif (BSP_PACKAGE_PINS == 56)
    #define GPIO_INFO_NUM_PINS              (30)
#else
    #error "r_gpio_rx does not have information about this RX23W package. Please update r_gpio_rx23w.h"
#endif

/* Base registers used for offsets on output data registers. */
#define     GPIO_PRV_BASE_ADDR_OUTPUT           ((uint8_t volatile *)&PORT0.PODR.BYTE)
/* Base registers used for offsets on input data registers. */
#define     GPIO_PRV_BASE_ADDR_INPUT            ((uint8_t volatile *)&PORT0.PIDR.BYTE)
/* Base registers used for offsets on direction registers. */
#define     GPIO_PRV_BASE_ADDR_DIRECTION        ((uint8_t volatile *)&PORT0.PDR.BYTE)
/* Base registers used for offsets on mode registers. */
#define     GPIO_PRV_BASE_ADDR_MODE             ((uint8_t volatile *)&PORT0.PMR.BYTE)
/* Base registers used for offsets on output type registers. */
#define     GPIO_PRV_BASE_ADDR_OUT_TYPE         (((uint8_t volatile *)&PORT2.ODR0.BYTE)-4)
/* Base registers used for offsets on pull-up registers. */
#define     GPIO_PRV_BASE_ADDR_PULL_UP          ((uint8_t volatile *)&PORT0.PCR.BYTE)
/* Base registers used for offsets on drive capacity control registers. */
#define     GPIO_PRV_BASE_ADDR_DSCR             (((uint8_t volatile *)&PORT1.DSCR.BYTE)-1)

#define GPIO_DSCR_IS_SUPPORTED    /* High-drive is supported for the RX23W */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
#if (BSP_PACKAGE_PINS == 85)
/* This enumerator has each available GPIO port on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_0 = 0x0000,
    GPIO_PORT_1 = 0x0100,
    GPIO_PORT_2 = 0x0200,
    GPIO_PORT_3 = 0x0300,
    GPIO_PORT_4 = 0x0400,
    GPIO_PORT_B = 0x0B00,
    GPIO_PORT_C = 0x0C00,
    GPIO_PORT_D = 0x0D00,
    GPIO_PORT_E = 0x0E00,
    GPIO_PORT_J = 0x1200,
} gpio_port_t;

/* This enumerator has a bit mask for each available GPIO pin for the given port on this MCU. */
typedef enum
{
    GPIO_PORT0_PIN_MASK = 0xA8,    /* Available pins: P03, P05, P07            */
    GPIO_PORT1_PIN_MASK = 0xF0,    /* Available pins: P14 to P17               */
    GPIO_PORT2_PIN_MASK = 0xE6,    /* Available pins: P21, P22, P25 to P27     */
    GPIO_PORT3_PIN_MASK = 0xE3,    /* Available pins: P30, P31, P35 to P37     */
    GPIO_PORT4_PIN_MASK = 0xFF,    /* Available pins: P40 to P47               */
    GPIO_PORTB_PIN_MASK = 0xAB,    /* Available pins: PB0, PB1, PB3, PB5, PB7  */
    GPIO_PORTC_PIN_MASK = 0xFD,    /* Available pins: PC0, PC2 to PC7          */
    GPIO_PORTD_PIN_MASK = 0x08,    /* Available pins: PD3                      */
    GPIO_PORTE_PIN_MASK = 0x1F,    /* Available pins: PE0 to PE4               */
    GPIO_PORTJ_PIN_MASK = 0x08,    /* Available pins: PJ3                      */
} gpio_pin_bit_mask_t;

/* This enumerator has each available GPIO pin on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_0_PIN_3 = 0x0003,
    GPIO_PORT_0_PIN_5 = 0x0005,
    GPIO_PORT_0_PIN_7 = 0x0007,
    GPIO_PORT_1_PIN_4 = 0x0104,
    GPIO_PORT_1_PIN_5 = 0x0105,
    GPIO_PORT_1_PIN_6 = 0x0106,
    GPIO_PORT_1_PIN_7 = 0x0107,
    GPIO_PORT_2_PIN_1 = 0x0201,
    GPIO_PORT_2_PIN_2 = 0x0202,
    GPIO_PORT_2_PIN_5 = 0x0205,
    GPIO_PORT_2_PIN_6 = 0x0206,
    GPIO_PORT_2_PIN_7 = 0x0207,
    GPIO_PORT_3_PIN_0 = 0x0300,
    GPIO_PORT_3_PIN_1 = 0x0301,
    GPIO_PORT_3_PIN_5 = 0x0305,
    GPIO_PORT_3_PIN_6 = 0x0306,
    GPIO_PORT_3_PIN_7 = 0x0307,
    GPIO_PORT_4_PIN_0 = 0x0400,
    GPIO_PORT_4_PIN_1 = 0x0401,
    GPIO_PORT_4_PIN_2 = 0x0402,
    GPIO_PORT_4_PIN_3 = 0x0403,
    GPIO_PORT_4_PIN_4 = 0x0404,
    GPIO_PORT_4_PIN_5 = 0x0405,
    GPIO_PORT_4_PIN_6 = 0x0406,
    GPIO_PORT_4_PIN_7 = 0x0407,
    GPIO_PORT_B_PIN_0 = 0x0B00,
    GPIO_PORT_B_PIN_1 = 0x0B01,
    GPIO_PORT_B_PIN_3 = 0x0B03,
    GPIO_PORT_B_PIN_5 = 0x0B05,
    GPIO_PORT_B_PIN_7 = 0x0B07,
    GPIO_PORT_C_PIN_0 = 0x0C00,
    GPIO_PORT_C_PIN_2 = 0x0C02,
    GPIO_PORT_C_PIN_3 = 0x0C03,
    GPIO_PORT_C_PIN_4 = 0x0C04,
    GPIO_PORT_C_PIN_5 = 0x0C05,
    GPIO_PORT_C_PIN_6 = 0x0C06,
    GPIO_PORT_C_PIN_7 = 0x0C07,
    GPIO_PORT_D_PIN_3 = 0x0D03,
    GPIO_PORT_E_PIN_0 = 0x0E00,
    GPIO_PORT_E_PIN_1 = 0x0E01,
    GPIO_PORT_E_PIN_2 = 0x0E02,
    GPIO_PORT_E_PIN_3 = 0x0E03,
    GPIO_PORT_E_PIN_4 = 0x0E04,
    GPIO_PORT_J_PIN_3 = 0x1203,
} gpio_port_pin_t;

#elif (BSP_PACKAGE_PINS == 56)
/* This enumerator has each available GPIO port on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_0 = 0x0000,
    GPIO_PORT_1 = 0x0100,
    GPIO_PORT_2 = 0x0200,
    GPIO_PORT_3 = 0x0300,
    GPIO_PORT_4 = 0x0400,
    GPIO_PORT_B = 0x0B00,
    GPIO_PORT_C = 0x0C00,
    GPIO_PORT_D = 0x0D00,
    GPIO_PORT_E = 0x0E00,
} gpio_port_t;

/* This enumerator has a bit mask for each available GPIO pin for the given port on this MCU. */
typedef enum
{
    GPIO_PORT0_PIN_MASK = 0x20,    /* Available pins: P05                       */
    GPIO_PORT1_PIN_MASK = 0xF0,    /* Available pins: P14 to P17                */
    GPIO_PORT2_PIN_MASK = 0xC0,    /* Available pins: P26, P27                  */
    GPIO_PORT3_PIN_MASK = 0xE3,    /* Available pins: P30, P31, P35 to P37      */
    GPIO_PORT4_PIN_MASK = 0xE2,    /* Available pins: P41, P45 to P47           */
    GPIO_PORTB_PIN_MASK = 0x83,    /* Available pins: PB0, PB1, PB7             */
    GPIO_PORTC_PIN_MASK = 0xFD,    /* Available pins: PC0, PC2 to PC7           */
    GPIO_PORTD_PIN_MASK = 0x08,    /* Available pins: PD3                       */
    GPIO_PORTE_PIN_MASK = 0x1C,    /* Available pins: PE2 to PE4                */
} gpio_pin_bit_mask_t;

/* This enumerator has each available GPIO pin on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_0_PIN_5 = 0x0005,
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
    GPIO_PORT_4_PIN_1 = 0x0401,
    GPIO_PORT_4_PIN_5 = 0x0405,
    GPIO_PORT_4_PIN_6 = 0x0406,
    GPIO_PORT_4_PIN_7 = 0x0407,
    GPIO_PORT_B_PIN_0 = 0x0B00,
    GPIO_PORT_B_PIN_1 = 0x0B01,
    GPIO_PORT_B_PIN_7 = 0x0B07,
    GPIO_PORT_C_PIN_0 = 0x0C00,
    GPIO_PORT_C_PIN_2 = 0x0C02,
    GPIO_PORT_C_PIN_3 = 0x0C03,
    GPIO_PORT_C_PIN_4 = 0x0C04,
    GPIO_PORT_C_PIN_5 = 0x0C05,
    GPIO_PORT_C_PIN_6 = 0x0C06,
    GPIO_PORT_C_PIN_7 = 0x0C07,
    GPIO_PORT_D_PIN_3 = 0x0D03,
    GPIO_PORT_E_PIN_2 = 0x0E02,
    GPIO_PORT_E_PIN_3 = 0x0E03,
    GPIO_PORT_E_PIN_4 = 0x0E04,
} gpio_port_pin_t;
#endif

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

#endif /* BSP_MCU_RX23W */
#endif /* GPIO_RX23W */
