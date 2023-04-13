/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_cellular_private.h
 * Description  : Configures the driver.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
#include "platform.h"
#endif

#include "r_cellular_config.h"

#ifndef CELLULAR_PRIVATE_H
#define CELLULAR_PRIVATE_H

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#define CELLULAR_PROTOCOL_TCP   (6)
#define CELLULAR_PROTOCOL_UDP   (17)
#define CELLULAR_PROTOCOL_IPV4  (4)
#define CELLULAR_PROTOCOL_IPV6  (6)

#define CELLULAR_ATC_BUFF_SIZE              (300)
#define CELLULAR_ATC_RESPONSE_BUFF_SIZE     (2048)

#define CELLULAR_MAIN_TASK_BIT              (0x01u << 1)
#define CELLULAR_RECV_TASK_BIT              (0x01u << 2)

#define CELLULAR_TIME_WAIT_TASK_START       (10000u)
#define CELLULAR_TIME_OUT_MAX_DELAY         (0xffffffffu)

#define CELLULAR_MAX_AP_NAME_LENGTH         (64)
#define CELLULAR_MAX_AP_ID_LENGTH           (32)
#define CELLULAR_MAX_AP_PASS_LENGTH         (64)
#define CELLULAR_MAX_SIM_PASS_LENGTH        (8)

#define CELLULAR_MAX_ICCID_LENGTH           (22)
#define CELLULAR_ICCID_USELESS_CHAR         (8)
#define CELLULAR_MAX_IMEI_LENGTH            (15)
#define CELLULAR_IMEI_USELESS_CHAR          (2)
#define CELLULAR_MAX_IMSI_LENGTH            (15)
#define CELLULAR_IMSI_USELESS_CHAR          (2)
#define CELLULAR_MAX_PHONENUM_LENGTH        (15)
#define CELLULAR_PHONENUM_USELESS_CHAR      (12)
#define CELLULAR_MAX_RSSI_LENGTH            (2)
#define CELLULAR_MAX_BER_LENGTH             (2)
#define CELLULAR_MAX_SVN_LENGTH             (2)
#define CELLULAR_SVN_USELESS_CHAR           (5)
#define CELLULAR_MAX_REVISION_LENGTH        (9)
#define CELLULAR_REVISION_USELESS_CHAR      (2)
#define CELLULAR_MAX_LR_SVN_LENGTH          (15)
#define CELLULAR_LR_SVN_USELESS_CHAR        (2)
#define CELLULAR_MAX_CTM_LENGTH             (20)
#define CELLULAR_CTM_USELESS_CHAR           (5)
#define CELLULAR_IPV4_ADDR_LENGTH           (15)
#define CELLULAR_IPV6_ADDR_LENGTH           (39)
#define CELLULAR_UPDATE_STATE_LENGTH        (50)
#define CELLULAR_DNS_LENGTH                 (49)
#define CELLULAR_CPIN_STATUS_LENGTH         (14)
#define CELLULAR_PDP_ADDR_LENGTH            (69)
#define CELLULAR_GET_UPDATE_STATE           "GET"

#define CELLULAR_FLG_OFF                    (0u)
#define CELLULAR_FLG_START                  (0x01u << 0)
#define CELLULAR_FLG_SHUTDOWN               (0x01u << 1)

/* Convert a macro value to a string */
#define CELLULAR_STRING_MACRO(str)          #str
/* Call CELLULAR_STRING_MACRO */
#define CELLULAR_STRING_CONVERT(arg)        (CELLULAR_STRING_MACRO(arg))

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
/* Driver Type */
#define CELLULAR_IMPLEMENT_TYPE ('A')

/* PDR port macros. */
#define CELLULAR_SET_PDR(x, y)              (CELLULAR_SET_PDR_PREPROC(x, y))
/* Set the port direction. */
#define CELLULAR_SET_PDR_PREPROC(x, y)      ((PORT ## x .PDR.BIT.B ## y))

/* PODR port macros. */
#define CELLULAR_SET_PODR(x, y)             (CELLULAR_SET_PODR_PREPROC(x, y))
/* Set the port I/O. */
#define CELLULAR_SET_PODR_PREPROC(x, y)     ((PORT ## x .PODR.BIT.B ## y))

#if CELLULAR_CFG_CTS_SW_CTRL == 1
/* PIDR port macros. */
#define CELLULAR_GET_PIDR(x, y)             (CELLULAR_GET_PIDR_PREPROC(x, y))
/* Get the port I/O. */
#define CELLULAR_GET_PIDR_PREPROC(x, y)     ((PORT ## x .PIDR.BIT.B ## y))
/* PMR port macros. */
#define CELLULAR_SET_PMR(x, y)              (CELLULAR_SET_PMR_PREPROC(x, y))
/* SET the port I/O. */
#define CELLULAR_SET_PMR_PREPROC(x, y)      ((PORT ## x .PMR.BIT.B ## y))
/* PFS port macros. */
#define CELLULAR_SET_PFS(x, y)              (CELLULAR_SET_PFS_PREPROC(x, y))
/* SET the PFS value. */
#define CELLULAR_SET_PFS_PREPROC(x, y)      ((MPC.P ## x ## y ##PFS.BYTE))
#endif

#define CELLULAR_PIN_DIRECTION_MODE_OUTPUT     (1)

#if SCI_CFG_TEI_INCLUDED == (0)
#error "The r_cellular requires TEI to be enabled, i.e., SCI_CFG_TEI_INCLUDED is set to (1).\
Please confirm r_sci_rx settings, /smc_gen/r_config/r_sci_rx_config.h."
#endif

#if BSP_CFG_RTOS_USED == (1)
/* The r_cellular requires configTICK_RATE_HZ is 1000,
 * RTOS tick interrupt cycle should be 1ms. Please configure the value. */
#elif BSP_CFG_RTOS_USED == (5)
#if TX_TIMER_TICKS_PER_SECOND != (1000)
#error "The r_cellular requires TX_TIMER_TICKS_PER_SECOND is 1000,\
RTOS tick interrupt cycle should be 1ms. Please configure the value."
#endif
#endif

#elif defined(__CCRL__) || defined(__ICCRL78__) || defined(__RL)
/* Driver Type */
#define CELLULAR_IMPLEMENT_TYPE ('B')

/* PM port macros. */
#define CELLULAR_SET_PDR(x, y)                  (CELLULAR_SET_PDR_PREPROC(x, y))
/* Set the port direction. */
#define CELLULAR_SET_PDR_PREPROC(x, y)          ((PM ## x ## _bit.no ## y))

/* P port macros. */
#define CELLULAR_SET_PODR(x, y)                 (CELLULAR_SET_PODR_PREPROC(x, y))
/* Set the port I/O. */
#define CELLULAR_SET_PODR_PREPROC(x, y)         ((P ## x ## _bit.no ## y))

#define CELLULAR_PIN_DIRECTION_MODE_OUTPUT      (0)
#endif

#endif /* CELLULAR_PRIVATE_H */
