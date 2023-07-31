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
* Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_gpio_rx23e-b.h
* Description  : Specifics for the r_gpio_rx driver for the RX23E-B.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           29.05.2023 1.00    First Release
***********************************************************************************************************************/
#ifndef GPIO_RX23E_B
#define GPIO_RX23E_B

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"
#if defined(BSP_MCU_RX23E_B)  /* Prevents the compiler from finding multiple definitions of constant in this file */

/* Configuration for this package. */
#include "r_gpio_rx_config.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* General information about number of ports and pins on this device. */
#define GPIO_INFO_NUM_PORTS                 (15)

#if (BSP_PACKAGE_PINS == 100)
    #define GPIO_INFO_NUM_PINS              (59)
#elif (BSP_PACKAGE_PINS == 80)
    #define GPIO_INFO_NUM_PINS              (44)
#elif (BSP_PACKAGE_PINS == 64)
#if (BSP_CFG_MCU_PART_VERSION == 0x14) || (BSP_CFG_MCU_PART_VERSION == 0x15)
    #define GPIO_INFO_NUM_PINS              (31)
#else
    #define GPIO_INFO_NUM_PINS              (28)
#endif
#elif (BSP_PACKAGE_PINS == 48)
    #define GPIO_INFO_NUM_PINS              (18)
#elif (BSP_PACKAGE_PINS == 40)
#if (BSP_CFG_MCU_PART_VERSION == 0x14) || (BSP_CFG_MCU_PART_VERSION == 0x15)
    #define GPIO_INFO_NUM_PINS              (14)
#else
    #define GPIO_INFO_NUM_PINS              (15)
#endif
#else
    #error "r_gpio_rx does not have information about this RX23E-B package. Please update r_gpio_rx23e-b.h"
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
#define     GPIO_PRV_BASE_ADDR_OUT_TYPE         (((uint8_t volatile *)&PORT1.ODR0.BYTE) - 2)
/* Base registers used for offsets on pull-up registers. */
#define     GPIO_PRV_BASE_ADDR_PULL_UP          (((uint8_t volatile *)&PORT1.PCR.BYTE)  - 1)
/* Base registers used for offsets on drive capacity control registers. */
#define     GPIO_PRV_BASE_ADDR_DSCR             (((uint8_t volatile *)&PORT1.DSCR.BYTE) - 1)

#define GPIO_DSCR_IS_SUPPORTED    /* High-drive is supported for the RX23E-B  */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
#if (BSP_PACKAGE_PINS == 100)
/* This enumerator has each available GPIO port on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_1 = 0x0100,
    GPIO_PORT_2 = 0x0200,
    GPIO_PORT_3 = 0x0300,
    GPIO_PORT_5 = 0x0500,
    GPIO_PORT_6 = 0x0600,
    GPIO_PORT_7 = 0x0700,
    GPIO_PORT_A = 0x0A00,
    GPIO_PORT_B = 0x0B00,
    GPIO_PORT_C = 0x0C00,
    GPIO_PORT_D = 0x0D00,
    GPIO_PORT_E = 0x0E00,
} gpio_port_t;

/* This enumerator has a bit mask for each available GPIO pin for the given port on this MCU. */
typedef enum
{
    GPIO_PORT1_PIN_MASK = 0xFC,    /* Available pins: P12 to P17            */
    GPIO_PORT2_PIN_MASK = 0xFF,    /* Available pins: P20 to P27            */
    GPIO_PORT3_PIN_MASK = 0xE3,    /* Available pins: P30, P31, P35 to P37  */
    GPIO_PORT5_PIN_MASK = 0x30,    /* Available pins: P54, P55              */
    GPIO_PORT6_PIN_MASK = 0xFF,    /* Available pins: P60 to P67            */
    GPIO_PORT7_PIN_MASK = 0x1F,    /* Available pins: P70 to P74            */
    GPIO_PORTA_PIN_MASK = 0x1F,    /* Available pins: PA0 to PA4            */
    GPIO_PORTB_PIN_MASK = 0x03,    /* Available pins: PB0, PB1              */
    GPIO_PORTC_PIN_MASK = 0xFF,    /* Available pins: PC0 to PC7            */
    GPIO_PORTD_PIN_MASK = 0x1F,    /* Available pins: PD0 to PD4            */
    GPIO_PORTE_PIN_MASK = 0x1F,    /* Available pins: PE0 to PE4            */
} gpio_pin_bit_mask_t;

/* This enumerator has each available GPIO pin on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_1_PIN_2 = 0x0102,
    GPIO_PORT_1_PIN_3 = 0x0103,
    GPIO_PORT_1_PIN_4 = 0x0104,
    GPIO_PORT_1_PIN_5 = 0x0105,
    GPIO_PORT_1_PIN_6 = 0x0106,
    GPIO_PORT_1_PIN_7 = 0x0107,
    GPIO_PORT_2_PIN_0 = 0x0200,
    GPIO_PORT_2_PIN_1 = 0x0201,
    GPIO_PORT_2_PIN_2 = 0x0202,
    GPIO_PORT_2_PIN_3 = 0x0203,
    GPIO_PORT_2_PIN_4 = 0x0204,
    GPIO_PORT_2_PIN_5 = 0x0205,
    GPIO_PORT_2_PIN_6 = 0x0206,
    GPIO_PORT_2_PIN_7 = 0x0207,
    GPIO_PORT_3_PIN_0 = 0x0300,
    GPIO_PORT_3_PIN_1 = 0x0301,
    GPIO_PORT_3_PIN_5 = 0x0305,
    GPIO_PORT_3_PIN_6 = 0x0306,
    GPIO_PORT_3_PIN_7 = 0x0307,
    GPIO_PORT_5_PIN_4 = 0x0504,
    GPIO_PORT_5_PIN_5 = 0x0505,
    GPIO_PORT_6_PIN_0 = 0x0600,
    GPIO_PORT_6_PIN_1 = 0x0601,
    GPIO_PORT_6_PIN_2 = 0x0602,
    GPIO_PORT_6_PIN_3 = 0x0603,
    GPIO_PORT_6_PIN_4 = 0x0604,
    GPIO_PORT_6_PIN_5 = 0x0605,
    GPIO_PORT_6_PIN_6 = 0x0606,
    GPIO_PORT_6_PIN_7 = 0x0607,
    GPIO_PORT_7_PIN_0 = 0x0700,
    GPIO_PORT_7_PIN_1 = 0x0701,
    GPIO_PORT_7_PIN_2 = 0x0702,
    GPIO_PORT_7_PIN_3 = 0x0703,
    GPIO_PORT_7_PIN_4 = 0x0704,
    GPIO_PORT_A_PIN_0 = 0x0A00,
    GPIO_PORT_A_PIN_1 = 0x0A01,
    GPIO_PORT_A_PIN_2 = 0x0A02,
    GPIO_PORT_A_PIN_3 = 0x0A03,
    GPIO_PORT_A_PIN_4 = 0x0A04,
    GPIO_PORT_B_PIN_0 = 0x0B00,
    GPIO_PORT_B_PIN_1 = 0x0B01,
    GPIO_PORT_C_PIN_0 = 0x0C00,
    GPIO_PORT_C_PIN_1 = 0x0C01,
    GPIO_PORT_C_PIN_2 = 0x0C02,
    GPIO_PORT_C_PIN_3 = 0x0C03,
    GPIO_PORT_C_PIN_4 = 0x0C04,
    GPIO_PORT_C_PIN_5 = 0x0C05,
    GPIO_PORT_C_PIN_6 = 0x0C06,
    GPIO_PORT_C_PIN_7 = 0x0C07,
    GPIO_PORT_D_PIN_0 = 0x0D00,
    GPIO_PORT_D_PIN_1 = 0x0D01,
    GPIO_PORT_D_PIN_2 = 0x0D02,
    GPIO_PORT_D_PIN_3 = 0x0D03,
    GPIO_PORT_D_PIN_4 = 0x0D04,
    GPIO_PORT_E_PIN_0 = 0x0E00,
    GPIO_PORT_E_PIN_1 = 0x0E01,
    GPIO_PORT_E_PIN_2 = 0x0E02,
    GPIO_PORT_E_PIN_3 = 0x0E03,
    GPIO_PORT_E_PIN_4 = 0x0E04,
} gpio_port_pin_t;

#elif (BSP_PACKAGE_PINS == 80)
/* This enumerator has each available GPIO port on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_1 = 0x0100,
    GPIO_PORT_2 = 0x0200,
    GPIO_PORT_3 = 0x0300,
    GPIO_PORT_5 = 0x0500,
    GPIO_PORT_6 = 0x0600,
    GPIO_PORT_A = 0x0A00,
    GPIO_PORT_B = 0x0B00,
    GPIO_PORT_C = 0x0C00,
    GPIO_PORT_E = 0x0E00,
} gpio_port_t;

/* This enumerator has a bit mask for each available GPIO pin for the given port on this MCU. */
typedef enum
{
    GPIO_PORT1_PIN_MASK = 0xF0,    /* Available pins: P14 to P17            */
    GPIO_PORT2_PIN_MASK = 0xFC,    /* Available pins: P22 to P27            */
    GPIO_PORT3_PIN_MASK = 0xE3,    /* Available pins: P30, P31, P35 to P37  */
    GPIO_PORT5_PIN_MASK = 0x30,    /* Available pins: P54, P55              */
    GPIO_PORT6_PIN_MASK = 0xFF,    /* Available pins: P60 to P67            */
    GPIO_PORTA_PIN_MASK = 0x1F,    /* Available pins: PA0 to PA4            */
    GPIO_PORTB_PIN_MASK = 0x03,    /* Available pins: PB0, PB1              */
    GPIO_PORTC_PIN_MASK = 0xFB,    /* Available pins: PC0, PC1, PC3 to PC7  */
    GPIO_PORTE_PIN_MASK = 0x1F,    /* Available pins: PE0 to PE4            */
} gpio_pin_bit_mask_t;

/* This enumerator has each available GPIO pin on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_1_PIN_4 = 0x0104,
    GPIO_PORT_1_PIN_5 = 0x0105,
    GPIO_PORT_1_PIN_6 = 0x0106,
    GPIO_PORT_1_PIN_7 = 0x0107,
    GPIO_PORT_2_PIN_2 = 0x0202,
    GPIO_PORT_2_PIN_3 = 0x0203,
    GPIO_PORT_2_PIN_4 = 0x0204,
    GPIO_PORT_2_PIN_5 = 0x0205,
    GPIO_PORT_2_PIN_6 = 0x0206,
    GPIO_PORT_2_PIN_7 = 0x0207,
    GPIO_PORT_3_PIN_0 = 0x0300,
    GPIO_PORT_3_PIN_1 = 0x0301,
    GPIO_PORT_3_PIN_5 = 0x0305,
    GPIO_PORT_3_PIN_6 = 0x0306,
    GPIO_PORT_3_PIN_7 = 0x0307,
    GPIO_PORT_5_PIN_4 = 0x0504,
    GPIO_PORT_5_PIN_5 = 0x0505,
    GPIO_PORT_6_PIN_0 = 0x0600,
    GPIO_PORT_6_PIN_1 = 0x0601,
    GPIO_PORT_6_PIN_2 = 0x0602,
    GPIO_PORT_6_PIN_3 = 0x0603,
    GPIO_PORT_6_PIN_4 = 0x0604,
    GPIO_PORT_6_PIN_5 = 0x0605,
    GPIO_PORT_6_PIN_6 = 0x0606,
    GPIO_PORT_6_PIN_7 = 0x0607,
    GPIO_PORT_A_PIN_0 = 0x0A00,
    GPIO_PORT_A_PIN_1 = 0x0A01,
    GPIO_PORT_A_PIN_2 = 0x0A02,
    GPIO_PORT_A_PIN_3 = 0x0A03,
    GPIO_PORT_A_PIN_4 = 0x0A04,
    GPIO_PORT_B_PIN_0 = 0x0B00,
    GPIO_PORT_B_PIN_1 = 0x0B01,
    GPIO_PORT_C_PIN_0 = 0x0C00,
    GPIO_PORT_C_PIN_1 = 0x0C01,
    GPIO_PORT_C_PIN_3 = 0x0C03,
    GPIO_PORT_C_PIN_4 = 0x0C04,
    GPIO_PORT_C_PIN_5 = 0x0C05,
    GPIO_PORT_C_PIN_6 = 0x0C06,
    GPIO_PORT_C_PIN_7 = 0x0C07,
    GPIO_PORT_E_PIN_0 = 0x0E00,
    GPIO_PORT_E_PIN_1 = 0x0E01,
    GPIO_PORT_E_PIN_2 = 0x0E02,
    GPIO_PORT_E_PIN_3 = 0x0E03,
    GPIO_PORT_E_PIN_4 = 0x0E04,
} gpio_port_pin_t;

#elif (BSP_PACKAGE_PINS == 64)
/* This enumerator has each available GPIO port on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_1 = 0x0100,
    GPIO_PORT_2 = 0x0200,
    GPIO_PORT_3 = 0x0300,
    GPIO_PORT_5 = 0x0500,
    GPIO_PORT_6 = 0x0600,
    GPIO_PORT_B = 0x0B00,
    GPIO_PORT_C = 0x0C00,
    GPIO_PORT_E = 0x0E00,
} gpio_port_t;

/* This enumerator has a bit mask for each available GPIO pin for the given port on this MCU. */
typedef enum
{
    GPIO_PORT1_PIN_MASK = 0xF0,    /* Available pins: P14 to P17            */
    GPIO_PORT2_PIN_MASK = 0xC0,    /* Available pins: P26, P27              */
    GPIO_PORT3_PIN_MASK = 0xE3,    /* Available pins: P30, P31, P35 to P37  */
    GPIO_PORT5_PIN_MASK = 0x30,    /* Available pins: P54, P55              */
    GPIO_PORT6_PIN_MASK = 0xF0,    /* Available pins: P64 to P67            */
    GPIO_PORTB_PIN_MASK = 0x03,    /* Available pins: PB0, PB1              */
    GPIO_PORTC_PIN_MASK = 0xFB,    /* Available pins: PC0, PC1, PC3 to PC7  */
#if (BSP_CFG_MCU_PART_VERSION == 0x14) || (BSP_CFG_MCU_PART_VERSION == 0x15) 
    GPIO_PORTE_PIN_MASK = 0x1F,    /* Available pins: PE0 to PE4            */
#else
    GPIO_PORTE_PIN_MASK = 0x03,    /* Available pins: PE0, PE1              */
#endif
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
    GPIO_PORT_5_PIN_4 = 0x0504,
    GPIO_PORT_5_PIN_5 = 0x0505,
    GPIO_PORT_6_PIN_4 = 0x0604,
    GPIO_PORT_6_PIN_5 = 0x0605,
    GPIO_PORT_6_PIN_6 = 0x0606,
    GPIO_PORT_6_PIN_7 = 0x0607,
    GPIO_PORT_B_PIN_0 = 0x0B00,
    GPIO_PORT_B_PIN_1 = 0x0B01,
    GPIO_PORT_C_PIN_0 = 0x0C00,
    GPIO_PORT_C_PIN_1 = 0x0C01,
    GPIO_PORT_C_PIN_3 = 0x0C03,
    GPIO_PORT_C_PIN_4 = 0x0C04,
    GPIO_PORT_C_PIN_5 = 0x0C05,
    GPIO_PORT_C_PIN_6 = 0x0C06,
    GPIO_PORT_C_PIN_7 = 0x0C07,
    GPIO_PORT_E_PIN_0 = 0x0E00,
    GPIO_PORT_E_PIN_1 = 0x0E01,
#if (BSP_CFG_MCU_PART_VERSION == 0x14) || (BSP_CFG_MCU_PART_VERSION == 0x15)
    GPIO_PORT_E_PIN_2 = 0x0E02,
    GPIO_PORT_E_PIN_3 = 0x0E03,
    GPIO_PORT_E_PIN_4 = 0x0E04,
#endif  
} gpio_port_pin_t;

#elif (BSP_PACKAGE_PINS == 48)
/* This enumerator has each available GPIO port on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_1 = 0x0100,
    GPIO_PORT_2 = 0x0200,
    GPIO_PORT_3 = 0x0300,
    GPIO_PORT_5 = 0x0500,
    GPIO_PORT_6 = 0x0600,
    GPIO_PORT_B = 0x0B00,
    GPIO_PORT_C = 0x0C00,
} gpio_port_t;

/* This enumerator has a bit mask for each available GPIO pin for the given port on this MCU. */
typedef enum
{ 
    GPIO_PORT1_PIN_MASK = 0xF0,    /* Available pins: P14 to P17            */
    GPIO_PORT2_PIN_MASK = 0xC0,    /* Available pins: P26, P27              */
    GPIO_PORT3_PIN_MASK = 0xE3,    /* Available pins: P30, P31, P35 to P37  */
    GPIO_PORT5_PIN_MASK = 0x30,    /* Available pins: P54, P55              */
    GPIO_PORT6_PIN_MASK = 0xC0,    /* Available pins: P66, P67              */
    GPIO_PORTB_PIN_MASK = 0x03,    /* Available pins: PB0, PB1              */
    GPIO_PORTC_PIN_MASK = 0x80,    /* Available pins: PC7                   */
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
    GPIO_PORT_5_PIN_4 = 0x0504,
    GPIO_PORT_5_PIN_5 = 0x0505,
    GPIO_PORT_6_PIN_6 = 0x0606,
    GPIO_PORT_6_PIN_7 = 0x0607,
    GPIO_PORT_B_PIN_0 = 0x0B00,
    GPIO_PORT_B_PIN_1 = 0x0B01,
    GPIO_PORT_C_PIN_7 = 0x0C07,
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
#if (BSP_CFG_MCU_PART_VERSION != 0x14) && (BSP_CFG_MCU_PART_VERSION != 0x15)
    GPIO_PORT_E = 0x0E00,
#endif
} gpio_port_t;

/* This enumerator has a bit mask for each available GPIO pin for the given port on this MCU. */
typedef enum
{ 
    GPIO_PORT1_PIN_MASK = 0xF0,    /* Available pins: P14 to P17            */
    GPIO_PORT2_PIN_MASK = 0xC0,    /* Available pins: P26, P27              */
    GPIO_PORT3_PIN_MASK = 0xE3,    /* Available pins: P30, P31, P35 to P37  */
    GPIO_PORTB_PIN_MASK = 0x03,    /* Available pins: PB0, PB1              */
    GPIO_PORTC_PIN_MASK = 0x80,    /* Available pins: PC7                   */
#if (BSP_CFG_MCU_PART_VERSION != 0x14) && (BSP_CFG_MCU_PART_VERSION != 0x15)
    GPIO_PORTE_PIN_MASK = 0x01,    /* Available pins: PE0                   */
#endif
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
    GPIO_PORT_C_PIN_7 = 0x0C07,
#if (BSP_CFG_MCU_PART_VERSION != 0x14) && (BSP_CFG_MCU_PART_VERSION != 0x15)
    GPIO_PORT_E_PIN_0 = 0x0E00,
#endif
} gpio_port_pin_t;
#endif
/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

#endif /* definedBSP_MCU_RX23E_B */

#endif /* GPIO_RX23E_B */
