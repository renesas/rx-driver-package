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
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_lpt_rx130_private.h
 * Version      : 1.00
 * Description  : Functions for using even link controller
 ************************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 01.07.2016 1.00    First Release
 ***********************************************************************************************************************/
#ifndef LPT_RX130_PRIVATE_H
#define LPT_RX130_PRIVATE_H
/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_lpt_rx_config.h"

/*******************************************************************************
  Macro definitions
*******************************************************************************/
#define LPT_CONV_US         ((uint32_t)(1000)*(uint32_t)(1000)) /* Value converted to microseconds */
#define LPT_CALCSCALE_OFFSET ((uint32_t)1000)           /* OFFSET value deal with clock source change to MHz */

/* Minimum cycle */
#define LPT_MIN_PERIOD      ((uint32_t)(0x00000002))    /* Minimum cycle */
/* Maximum cycle */
#define LPT_MAX_PERIOD      ((uint32_t)(0x0000FFFF))    /* Maximum cycle (shall not overflow with uint16_t) */

/* Common register setting */
#define LPT_LPTCR1_INIT     ((uint8_t)(0x01))           /* Initial value of LPTCR1 */
#define LPT_LPTCR2_INIT     ((uint8_t)(0x01))           /* Initial value of LPTCR2 */
#define LPT_LPTPRD_INIT     ((uint16_t)(0xFFFF))        /* Initial value of LPTPRD */
#define LPT_LPCMR0_INIT     ((uint16_t)(0xFFFF))        /* Initial value of LPCMR0 */
#define LPT_LPWUCR_INIT     ((uint16_t)(0x0000))        /* Initial value of LPWUCR */

/* Low-Power Timer Clock Division Ratio Select Data (do NOT change values) */
#define LPT_DIVISION_RATIO_2    (0x01U)
#define LPT_DIVISION_RATIO_4    (0x02U)
#define LPT_DIVISION_RATIO_8    (0x03U)
#define LPT_DIVISION_RATIO_16   (0x04U)
#define LPT_DIVISION_RATIO_32   (0x05U)

#define LPT_DIVISION_RATIO_NUM  (5)     /* Number of LPT division ratio */

/*------------------------------------------------------------------------------
  Parameter check of Configuration Options
------------------------------------------------------------------------------*/
#ifdef BSP_CFG_LPT_CLOCK_SOURCE
    #define LPT_CLOCK_SOURCE    (BSP_CFG_LPT_CLOCK_SOURCE)
#else
    #define LPT_CLOCK_SOURCE    (LPT_CFG_LPT_CLOCK_SOURCE)
#endif

#if   (LPT_CLOCK_SOURCE == 0)
    #define LPT_LPTSRCCLK_HZ    (BSP_SUB_CLOCK_HZ)
    #define LPT_LPTSRCCLK_CYC   (31)                    /* 1cycle/Sub-clock(32.768kHz) */
#elif (LPT_CLOCK_SOURCE == 1)
    #define LPT_LPTSRCCLK_HZ    (15000)                 /* IWDTCLK frequency */
    #define LPT_LPTSRCCLK_CYC   (79)                    /* 1cycle/IWDTCLK(min:12.75kHz max:17.25kHz) */
#else
    #error "ERROR - LPT_CFG_LPT_CLOCK_SOURCE - Unknown lpt clock source chosen in r_lpt_rx130_private.h"
#endif

#if   (2000000 < LPT_LPTSRCCLK_HZ)
    /* lpt_regval_calc() calculation result overflow when lpt clock source 2MHz greater. */
    #error "ERROR - LPT_LPTSRCCLK_HZ - 2MHz < lpt clock frequency in r_lpt_rx130_private.h"
#endif


#endif /* LPT_RX130_PRIVATE_H */

/***********************************************************************************************************************
 * End of File
 ***********************************************************************************************************************/
