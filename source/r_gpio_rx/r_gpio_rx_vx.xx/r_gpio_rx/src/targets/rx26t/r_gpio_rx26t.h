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
* File Name    : r_gpio_rx26t.h
* Description  : Specifics for the r_gpio_rx driver for the RX26T.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 07.04.2023 1.00    First Release
***********************************************************************************************************************/
#ifndef GPIO_RX26T
#define GPIO_RX26T

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"
#if defined(BSP_MCU_RX26T)  /* Prevents the compiler from finding multiple definitions of constant in this file */

/* Configuration for this package. */
#include "r_gpio_rx_config.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* General information about number of ports and pins on this device. */
#define GPIO_INFO_NUM_PORTS                 (23)

#if (BSP_PACKAGE_PINS == 100)
    #define GPIO_INFO_NUM_PINS              (83)
#elif (BSP_PACKAGE_PINS == 80)
    #define GPIO_INFO_NUM_PINS              (63)
#elif (BSP_PACKAGE_PINS == 64)
    #define GPIO_INFO_NUM_PINS              (50)
#elif (BSP_PACKAGE_PINS == 48)
    #define GPIO_INFO_NUM_PINS              (38)
#else
    #error "r_gpio_rx does not have information about this RX26T package. Please update r_gpio_rx26t.h"
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
#define     GPIO_PRV_BASE_ADDR_OUT_TYPE         ((uint8_t volatile *)&PORT0.ODR0.BYTE)
/* Base registers used for offsets on pull-up registers. */
#define     GPIO_PRV_BASE_ADDR_PULL_UP          ((uint8_t volatile *)&PORT0.PCR.BYTE)
/* Base registers used for offsets on drive capacity control registers. */
#define     GPIO_PRV_BASE_ADDR_DSCR             ((uint8_t volatile *)&PORT0.DSCR.BYTE)
/* Base registers used for offsets on drive capacity control registers 2. (Large current high-drive) */
#define     GPIO_PRV_BASE_ADDR_DSCR2            ((uint8_t volatile *)&PORT7.DSCR2.BYTE-7)

#define GPIO_DSCR_IS_SUPPORTED    /* High-drive is supported for the RX26T                              */
#define GPIO_DSCR2_IS_SUPPORTED   /* Large current output, high-drive output is supported for the RX26T */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
#if (BSP_PACKAGE_PINS == 100)
/* This enumerator has each available GPIO port on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_0 = 0x0000,
    GPIO_PORT_1 = 0x0100,
    GPIO_PORT_2 = 0x0200,
    GPIO_PORT_3 = 0x0300,
    GPIO_PORT_4 = 0x0400,
    GPIO_PORT_5 = 0x0500,
    GPIO_PORT_6 = 0x0600,
    GPIO_PORT_7 = 0x0700,
    GPIO_PORT_8 = 0x0800,
    GPIO_PORT_9 = 0x0900,
    GPIO_PORT_A = 0x0A00,
    GPIO_PORT_B = 0x0B00,
    GPIO_PORT_D = 0x0D00,
    GPIO_PORT_E = 0x0E00,
    GPIO_PORT_N = 0x1600,
} gpio_port_t;

/* This enumerator has a bit mask for each available GPIO pin for the given port on this MCU. */
typedef enum
{
    GPIO_PORT0_PIN_MASK = 0x03,    /* Available pins: P00, P01             */
    GPIO_PORT1_PIN_MASK = 0x03,    /* Available pins: P10, P11             */
    GPIO_PORT2_PIN_MASK = 0x9F,    /* Available pins: P20 to P24, P27      */
    GPIO_PORT3_PIN_MASK = 0xCF,    /* Available pins: P30 to P33, P36, P37 */
    GPIO_PORT4_PIN_MASK = 0xFF,    /* Available pins: P40 to P47           */
    GPIO_PORT5_PIN_MASK = 0x3F,    /* Available pins: P50 to P55           */
    GPIO_PORT6_PIN_MASK = 0x3F,    /* Available pins: P60 to P65           */
    GPIO_PORT7_PIN_MASK = 0x7F,    /* Available pins: P70 to P76           */
    GPIO_PORT8_PIN_MASK = 0x07,    /* Available pins: P80 to P82           */
    GPIO_PORT9_PIN_MASK = 0x7F,    /* Available pins: P90 to P96           */
    GPIO_PORTA_PIN_MASK = 0x3F,    /* Available pins: PA0 to PA5           */
    GPIO_PORTB_PIN_MASK = 0xFF,    /* Available pins: PB0 to PB7           */
    GPIO_PORTD_PIN_MASK = 0xFF,    /* Available pins: PD0 to PD7           */
    GPIO_PORTE_PIN_MASK = 0x3F,    /* Available pins: PE0 to PE5           */
    GPIO_PORTN_PIN_MASK = 0xC0,    /* Available pins: PN6, PN7             */
} gpio_pin_bit_mask_t;

/* This enumerator has each available GPIO pin on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_0_PIN_0 = 0x0000,
    GPIO_PORT_0_PIN_1 = 0x0001,
    GPIO_PORT_1_PIN_0 = 0x0100,
    GPIO_PORT_1_PIN_1 = 0x0101,
    GPIO_PORT_2_PIN_0 = 0x0200,
    GPIO_PORT_2_PIN_1 = 0x0201,
    GPIO_PORT_2_PIN_2 = 0x0202,
    GPIO_PORT_2_PIN_3 = 0x0203,
    GPIO_PORT_2_PIN_4 = 0x0204,
    GPIO_PORT_2_PIN_7 = 0x0207,
    GPIO_PORT_3_PIN_0 = 0x0300,
    GPIO_PORT_3_PIN_1 = 0x0301,
    GPIO_PORT_3_PIN_2 = 0x0302,
    GPIO_PORT_3_PIN_3 = 0x0303,
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
    GPIO_PORT_5_PIN_0 = 0x0500,
    GPIO_PORT_5_PIN_1 = 0x0501,
    GPIO_PORT_5_PIN_2 = 0x0502,
    GPIO_PORT_5_PIN_3 = 0x0503,
    GPIO_PORT_5_PIN_4 = 0x0504,
    GPIO_PORT_5_PIN_5 = 0x0505,
    GPIO_PORT_6_PIN_0 = 0x0600,
    GPIO_PORT_6_PIN_1 = 0x0601,
    GPIO_PORT_6_PIN_2 = 0x0602,
    GPIO_PORT_6_PIN_3 = 0x0603,
    GPIO_PORT_6_PIN_4 = 0x0604,
    GPIO_PORT_6_PIN_5 = 0x0605,
    GPIO_PORT_7_PIN_0 = 0x0700,
    GPIO_PORT_7_PIN_1 = 0x0701,
    GPIO_PORT_7_PIN_2 = 0x0702,
    GPIO_PORT_7_PIN_3 = 0x0703,
    GPIO_PORT_7_PIN_4 = 0x0704,
    GPIO_PORT_7_PIN_5 = 0x0705,
    GPIO_PORT_7_PIN_6 = 0x0706,
    GPIO_PORT_8_PIN_0 = 0x0800,
    GPIO_PORT_8_PIN_1 = 0x0801,
    GPIO_PORT_8_PIN_2 = 0x0802,
    GPIO_PORT_9_PIN_0 = 0x0900,
    GPIO_PORT_9_PIN_1 = 0x0901,
    GPIO_PORT_9_PIN_2 = 0x0902,
    GPIO_PORT_9_PIN_3 = 0x0903,
    GPIO_PORT_9_PIN_4 = 0x0904,
    GPIO_PORT_9_PIN_5 = 0x0905,
    GPIO_PORT_9_PIN_6 = 0x0906,
    GPIO_PORT_A_PIN_0 = 0x0A00,
    GPIO_PORT_A_PIN_1 = 0x0A01,
    GPIO_PORT_A_PIN_2 = 0x0A02,
    GPIO_PORT_A_PIN_3 = 0x0A03,
    GPIO_PORT_A_PIN_4 = 0x0A04,
    GPIO_PORT_A_PIN_5 = 0x0A05,
    GPIO_PORT_B_PIN_0 = 0x0B00,
    GPIO_PORT_B_PIN_1 = 0x0B01,
    GPIO_PORT_B_PIN_2 = 0x0B02,
    GPIO_PORT_B_PIN_3 = 0x0B03,
    GPIO_PORT_B_PIN_4 = 0x0B04,
    GPIO_PORT_B_PIN_5 = 0x0B05,
    GPIO_PORT_B_PIN_6 = 0x0B06,
    GPIO_PORT_B_PIN_7 = 0x0B07,
    GPIO_PORT_D_PIN_0 = 0x0D00,
    GPIO_PORT_D_PIN_1 = 0x0D01,
    GPIO_PORT_D_PIN_2 = 0x0D02,
    GPIO_PORT_D_PIN_3 = 0x0D03,
    GPIO_PORT_D_PIN_4 = 0x0D04,
    GPIO_PORT_D_PIN_5 = 0x0D05,
    GPIO_PORT_D_PIN_6 = 0x0D06,
    GPIO_PORT_D_PIN_7 = 0x0D07,
    GPIO_PORT_E_PIN_0 = 0x0E00,
    GPIO_PORT_E_PIN_1 = 0x0E01,
    GPIO_PORT_E_PIN_2 = 0x0E02,
    GPIO_PORT_E_PIN_3 = 0x0E03,
    GPIO_PORT_E_PIN_4 = 0x0E04,
    GPIO_PORT_E_PIN_5 = 0x0E05,
    GPIO_PORT_N_PIN_6 = 0x1606,
    GPIO_PORT_N_PIN_7 = 0x1607,
} gpio_port_pin_t;

#elif (BSP_PACKAGE_PINS == 80)
/* This enumerator has each available GPIO port on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_0 = 0x0000,
    GPIO_PORT_1 = 0x0100,
    GPIO_PORT_2 = 0x0200,
    GPIO_PORT_3 = 0x0300,
    GPIO_PORT_4 = 0x0400,
    GPIO_PORT_5 = 0x0500,
    GPIO_PORT_6 = 0x0600,
    GPIO_PORT_7 = 0x0700,
    GPIO_PORT_9 = 0x0900,
    GPIO_PORT_A = 0x0A00,
    GPIO_PORT_B = 0x0B00,
    GPIO_PORT_D = 0x0D00,
    GPIO_PORT_E = 0x0E00,
    GPIO_PORT_N = 0x1600,
} gpio_port_t;

/* This enumerator has a bit mask for each available GPIO pin for the given port on this MCU. */
typedef enum
{
    GPIO_PORT0_PIN_MASK = 0x03,    /* Available pins: P00, P01           */
    GPIO_PORT1_PIN_MASK = 0x03,    /* Available pins: P10, P11           */
    GPIO_PORT2_PIN_MASK = 0x87,    /* Available pins: P20 to P22, P27    */
    GPIO_PORT3_PIN_MASK = 0xC3,    /* Available pins: P30, P31, P36, P37 */
    GPIO_PORT4_PIN_MASK = 0xFF,    /* Available pins: P40 to P47         */
    GPIO_PORT5_PIN_MASK = 0x3F,    /* Available pins: P50 to P55         */
    GPIO_PORT6_PIN_MASK = 0x31,    /* Available pins: P60, P64, P65      */
    GPIO_PORT7_PIN_MASK = 0x7F,    /* Available pins: P70 to P76         */
    GPIO_PORT9_PIN_MASK = 0x7F,    /* Available pins: P90 to P96         */
    GPIO_PORTA_PIN_MASK = 0x28,    /* Available pins: PA3, PA5           */
    GPIO_PORTB_PIN_MASK = 0x7F,    /* Available pins: PB0 to PB6         */
    GPIO_PORTD_PIN_MASK = 0xFC,    /* Available pins: PD2 to PD7         */
    GPIO_PORTE_PIN_MASK = 0x1C,    /* Available pins: PE2 to PE4         */
    GPIO_PORTN_PIN_MASK = 0xC0,    /* Available pins: PN6, PN7           */
} gpio_pin_bit_mask_t;

/* This enumerator has each available GPIO pin on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_0_PIN_0 = 0x0000,
    GPIO_PORT_0_PIN_1 = 0x0001,
    GPIO_PORT_1_PIN_0 = 0x0100,
    GPIO_PORT_1_PIN_1 = 0x0101,
    GPIO_PORT_2_PIN_0 = 0x0200,
    GPIO_PORT_2_PIN_1 = 0x0201,
    GPIO_PORT_2_PIN_2 = 0x0202,
    GPIO_PORT_2_PIN_7 = 0x0207,
    GPIO_PORT_3_PIN_0 = 0x0300,
    GPIO_PORT_3_PIN_1 = 0x0301,
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
    GPIO_PORT_5_PIN_0 = 0x0500,
    GPIO_PORT_5_PIN_1 = 0x0501,
    GPIO_PORT_5_PIN_2 = 0x0502,
    GPIO_PORT_5_PIN_3 = 0x0503,
    GPIO_PORT_5_PIN_4 = 0x0504,
    GPIO_PORT_5_PIN_5 = 0x0505,
    GPIO_PORT_6_PIN_0 = 0x0600,
    GPIO_PORT_6_PIN_4 = 0x0604,
    GPIO_PORT_6_PIN_5 = 0x0605,
    GPIO_PORT_7_PIN_0 = 0x0700,
    GPIO_PORT_7_PIN_1 = 0x0701,
    GPIO_PORT_7_PIN_2 = 0x0702,
    GPIO_PORT_7_PIN_3 = 0x0703,
    GPIO_PORT_7_PIN_4 = 0x0704,
    GPIO_PORT_7_PIN_5 = 0x0705,
    GPIO_PORT_7_PIN_6 = 0x0706,
    GPIO_PORT_9_PIN_0 = 0x0900,
    GPIO_PORT_9_PIN_1 = 0x0901,
    GPIO_PORT_9_PIN_2 = 0x0902,
    GPIO_PORT_9_PIN_3 = 0x0903,
    GPIO_PORT_9_PIN_4 = 0x0904,
    GPIO_PORT_9_PIN_5 = 0x0905,
    GPIO_PORT_9_PIN_6 = 0x0906,
    GPIO_PORT_A_PIN_3 = 0x0A03,
    GPIO_PORT_A_PIN_5 = 0x0A05,
    GPIO_PORT_B_PIN_0 = 0x0B00,
    GPIO_PORT_B_PIN_1 = 0x0B01,
    GPIO_PORT_B_PIN_2 = 0x0B02,
    GPIO_PORT_B_PIN_3 = 0x0B03,
    GPIO_PORT_B_PIN_4 = 0x0B04,
    GPIO_PORT_B_PIN_5 = 0x0B05,
    GPIO_PORT_B_PIN_6 = 0x0B06,
    GPIO_PORT_D_PIN_2 = 0x0D02,
    GPIO_PORT_D_PIN_3 = 0x0D03,
    GPIO_PORT_D_PIN_4 = 0x0D04,
    GPIO_PORT_D_PIN_5 = 0x0D05,
    GPIO_PORT_D_PIN_6 = 0x0D06,
    GPIO_PORT_D_PIN_7 = 0x0D07,
    GPIO_PORT_E_PIN_2 = 0x0E02,
    GPIO_PORT_E_PIN_3 = 0x0E03,
    GPIO_PORT_E_PIN_4 = 0x0E04,
    GPIO_PORT_N_PIN_6 = 0x1606,
    GPIO_PORT_N_PIN_7 = 0x1607,
} gpio_port_pin_t;

#elif (BSP_PACKAGE_PINS == 64)
/* This enumerator has each available GPIO port on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_0 = 0x0000,
    GPIO_PORT_1 = 0x0100,
    GPIO_PORT_2 = 0x0200,
    GPIO_PORT_3 = 0x0300,
    GPIO_PORT_4 = 0x0400,
    GPIO_PORT_5 = 0x0500,
    GPIO_PORT_6 = 0x0600,
    GPIO_PORT_7 = 0x0700,
    GPIO_PORT_9 = 0x0900,
    GPIO_PORT_B = 0x0B00,
    GPIO_PORT_D = 0x0D00,
    GPIO_PORT_E = 0x0E00,
    GPIO_PORT_N = 0x1600,
} gpio_port_t;

/* This enumerator has a bit mask for each available GPIO pin for the given port on this MCU. */
typedef enum
{
    GPIO_PORT0_PIN_MASK = 0x03,    /* Available pins: P00, P01   */
    GPIO_PORT1_PIN_MASK = 0x02,    /* Available pins: P11        */
    GPIO_PORT2_PIN_MASK = 0x07,    /* Available pins: P20 to P22 */
    GPIO_PORT3_PIN_MASK = 0xC0,    /* Available pins: P36, P37   */
    GPIO_PORT4_PIN_MASK = 0xFF,    /* Available pins: P40 to P47 */
    GPIO_PORT5_PIN_MASK = 0x1C,    /* Available pins: P52 to P54 */
    GPIO_PORT6_PIN_MASK = 0x30,    /* Available pins: P64, P65   */
    GPIO_PORT7_PIN_MASK = 0x7F,    /* Available pins: P70 to P76 */
    GPIO_PORT9_PIN_MASK = 0x7F,    /* Available pins: P90 to P96 */
    GPIO_PORTB_PIN_MASK = 0x7F,    /* Available pins: PB0 to PB6 */
    GPIO_PORTD_PIN_MASK = 0xF8,    /* Available pins: PD3 to PD7 */
    GPIO_PORTE_PIN_MASK = 0x04,    /* Available pins: PE2        */
    GPIO_PORTN_PIN_MASK = 0xC0,    /* Available pins: PN6, PN7   */
} gpio_pin_bit_mask_t;

/* This enumerator has each available GPIO pin on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_0_PIN_0 = 0x0000,
    GPIO_PORT_0_PIN_1 = 0x0001,
    GPIO_PORT_1_PIN_1 = 0x0101,
    GPIO_PORT_2_PIN_0 = 0x0200,
    GPIO_PORT_2_PIN_1 = 0x0201,
    GPIO_PORT_2_PIN_2 = 0x0202,
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
    GPIO_PORT_5_PIN_2 = 0x0502,
    GPIO_PORT_5_PIN_3 = 0x0503,
    GPIO_PORT_5_PIN_4 = 0x0504,
    GPIO_PORT_6_PIN_4 = 0x0604,
    GPIO_PORT_6_PIN_5 = 0x0605,
    GPIO_PORT_7_PIN_0 = 0x0700,
    GPIO_PORT_7_PIN_1 = 0x0701,
    GPIO_PORT_7_PIN_2 = 0x0702,
    GPIO_PORT_7_PIN_3 = 0x0703,
    GPIO_PORT_7_PIN_4 = 0x0704,
    GPIO_PORT_7_PIN_5 = 0x0705,
    GPIO_PORT_7_PIN_6 = 0x0706,
    GPIO_PORT_9_PIN_0 = 0x0900,
    GPIO_PORT_9_PIN_1 = 0x0901,
    GPIO_PORT_9_PIN_2 = 0x0902,
    GPIO_PORT_9_PIN_3 = 0x0903,
    GPIO_PORT_9_PIN_4 = 0x0904,
    GPIO_PORT_9_PIN_5 = 0x0905,
    GPIO_PORT_9_PIN_6 = 0x0906,
    GPIO_PORT_B_PIN_0 = 0x0B00,
    GPIO_PORT_B_PIN_1 = 0x0B01,
    GPIO_PORT_B_PIN_2 = 0x0B02,
    GPIO_PORT_B_PIN_3 = 0x0B03,
    GPIO_PORT_B_PIN_4 = 0x0B04,
    GPIO_PORT_B_PIN_5 = 0x0B05,
    GPIO_PORT_B_PIN_6 = 0x0B06,
    GPIO_PORT_D_PIN_3 = 0x0D03,
    GPIO_PORT_D_PIN_4 = 0x0D04,
    GPIO_PORT_D_PIN_5 = 0x0D05,
    GPIO_PORT_D_PIN_6 = 0x0D06,
    GPIO_PORT_D_PIN_7 = 0x0D07,
    GPIO_PORT_E_PIN_2 = 0x0E02,
    GPIO_PORT_N_PIN_6 = 0x1606,
    GPIO_PORT_N_PIN_7 = 0x1607,
} gpio_port_pin_t;

#elif (BSP_PACKAGE_PINS == 48)
/* This enumerator has each available GPIO port on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_0 = 0x0000,
    GPIO_PORT_1 = 0x0100,
    GPIO_PORT_2 = 0x0200,
    GPIO_PORT_3 = 0x0300,
    GPIO_PORT_4 = 0x0400,
    GPIO_PORT_5 = 0x0500,
    GPIO_PORT_6 = 0x0600,
    GPIO_PORT_7 = 0x0700,
    GPIO_PORT_9 = 0x0900,
    GPIO_PORT_B = 0x0B00,
    GPIO_PORT_D = 0x0D00,
    GPIO_PORT_E = 0x0E00,
    GPIO_PORT_N = 0x1600,
} gpio_port_t;

/* This enumerator has a bit mask for each available GPIO pin for the given port on this MCU. */
typedef enum
{ 
    GPIO_PORT0_PIN_MASK = 0x01,    /* Available pins: P00           */
    GPIO_PORT1_PIN_MASK = 0x03,    /* Available pins: P10, P11      */
    GPIO_PORT2_PIN_MASK = 0x03,    /* Available pins: P20, P21      */
    GPIO_PORT3_PIN_MASK = 0xC0,    /* Available pins: P36, P37      */
    GPIO_PORT4_PIN_MASK = 0x1F,    /* Available pins: P40 to P44    */
    GPIO_PORT5_PIN_MASK = 0x0C,    /* Available pins: P52, P53      */
    GPIO_PORT6_PIN_MASK = 0x04,    /* Available pins: P62           */
    GPIO_PORT7_PIN_MASK = 0x7E,    /* Available pins: P71 to P76    */
    GPIO_PORT9_PIN_MASK = 0x3E,    /* Available pins: P91 to P95    */
    GPIO_PORTB_PIN_MASK = 0x7F,    /* Available pins: PB0 to PB6    */
    GPIO_PORTD_PIN_MASK = 0xA8,    /* Available pins: PD3, PD5, PD7 */
    GPIO_PORTE_PIN_MASK = 0x04,    /* Available pins: PE2           */
    GPIO_PORTN_PIN_MASK = 0x40,    /* Available pins: PN6           */
} gpio_pin_bit_mask_t;

/* This enumerator has each available GPIO pin on this MCU. This list will change depending on the MCU chosen. */
typedef enum
{
    GPIO_PORT_0_PIN_0 = 0x0000,
    GPIO_PORT_1_PIN_0 = 0x0100,
    GPIO_PORT_1_PIN_1 = 0x0101,
    GPIO_PORT_2_PIN_0 = 0x0200,
    GPIO_PORT_2_PIN_1 = 0x0201,
    GPIO_PORT_3_PIN_6 = 0x0306,
    GPIO_PORT_3_PIN_7 = 0x0307,
    GPIO_PORT_4_PIN_0 = 0x0400,
    GPIO_PORT_4_PIN_1 = 0x0401,
    GPIO_PORT_4_PIN_2 = 0x0402,
    GPIO_PORT_4_PIN_3 = 0x0403,
    GPIO_PORT_4_PIN_4 = 0x0404,
    GPIO_PORT_5_PIN_2 = 0x0502,
    GPIO_PORT_5_PIN_3 = 0x0503,
    GPIO_PORT_6_PIN_2 = 0x0602,
    GPIO_PORT_7_PIN_1 = 0x0701,
    GPIO_PORT_7_PIN_2 = 0x0702,
    GPIO_PORT_7_PIN_3 = 0x0703,
    GPIO_PORT_7_PIN_4 = 0x0704,
    GPIO_PORT_7_PIN_5 = 0x0705,
    GPIO_PORT_7_PIN_6 = 0x0706,
    GPIO_PORT_9_PIN_1 = 0x0901,
    GPIO_PORT_9_PIN_2 = 0x0902,
    GPIO_PORT_9_PIN_3 = 0x0903,
    GPIO_PORT_9_PIN_4 = 0x0904,
    GPIO_PORT_9_PIN_5 = 0x0905,
    GPIO_PORT_B_PIN_0 = 0x0B00,
    GPIO_PORT_B_PIN_1 = 0x0B01,
    GPIO_PORT_B_PIN_2 = 0x0B02,
    GPIO_PORT_B_PIN_3 = 0x0B03,
    GPIO_PORT_B_PIN_4 = 0x0B04,
    GPIO_PORT_B_PIN_5 = 0x0B05,
    GPIO_PORT_B_PIN_6 = 0x0B06,
    GPIO_PORT_D_PIN_3 = 0x0D03,
    GPIO_PORT_D_PIN_5 = 0x0D05,
    GPIO_PORT_D_PIN_7 = 0x0D07,
    GPIO_PORT_E_PIN_2 = 0x0E02,
    GPIO_PORT_N_PIN_6 = 0x1606,
} gpio_port_pin_t;
#endif

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

#endif /* BSP_MCU_RX26T */
#endif /* GPIO_RX26T */
