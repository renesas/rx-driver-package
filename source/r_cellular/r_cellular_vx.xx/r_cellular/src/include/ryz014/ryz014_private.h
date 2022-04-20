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
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : ryz014_private.h
 * Description  : Internal configuration settings for RYZ014A driver.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : xx.xx.xxxx 1.00     First Release
 *         : 02.09.2021 1.01     Fixed reset timing
 *         : 21.10.2021 1.02     Support for Azure RTOS
 *                               Support for GCC for Renesas GNURX Toolchain
 *         : 15.11.2021 1.03     Improved receiving behavior, removed socket buffers
 *         : 24.01.2022 1.04     R_CELLULAR_SetPSM and R_CELLULAR_SetEDRX have been added as new APIs
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_cellular_private.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#ifndef RYZ014_PRIVATE_H
#define RYZ014_PRIVATE_H

#define CELLULAR_MAX_ARG_COUNT          (8)         /* Maximum number of arguments to an AT command */

#define CELLULAR_START_SOCKET_NUMBER    (1)         /* Start socket number */
#define CELLULAR_PACKET_DATA_SIZE       (0)         /* Packet data size (0~1500,0=auto) */
#define CELLULAR_EX_TIMEOUT             (60)        /* Exchange timeout (1sec,0~65535,0=no_limit) */
#define CELLULAR_CONNECT_TIMEOUT        (200)       /* connection timeout (0.1sec,10~1200,0=no_limit) */
#define CELLULAR_TX_TIMEOUT             (10)        /* Transmit timeout (0.1sec,0~255,0=no_limit) */
#define CELLULAR_BAUDRATE               (921600)    /* Baudrate */
#define CELLULAR_CREATABLE_SOCKETS      (6)         /* Number of sockets to create (1~6) */
#define CELLULAR_MAX_TX_SIZE            (1500)      /* Data processing size per transmission (1~1500) */
#define CELLULAR_MAX_RX_SIZE            (1500)      /* Data processing size per reception (1~1500) */

#define CELLULAR_PACKET_DATA_SIZE_LIMIT (1500)      /* Packet data size limit */
#define CELLULAR_CONNECT_TIMEOUT_LIMIT_L    (10)    /* Minimum connection time */
#define CELLULAR_CONNECT_TIMEOUT_LIMIT_H    (1200)  /* Connection time limit  */
#define CELLULAR_TX_SIZE_LIMIT_L        (1)         /* Minimum Sending size */
#define CELLULAR_TX_SIZE_LIMIT_H        (1500)      /* Sending size limit */
#define CELLULAR_RX_SIZE_LIMIT_L        (1)         /* Minimum Receive size */
#define CELLULAR_RX_SIZE_LIMIT_H        (1500)      /* Receive size limit */
#define CELLULAR_YEAR_LIMIT             (99)        /* Year limit */
#define CELLULAR_MONTH_LIMIT            (12)        /* Month limit */
#define CELLULAR_DAY_LIMIT              (31)        /* Day limit */
#define CELLULAR_HOUR_LIMIT             (23)        /* Hour limit */
#define CELLULAR_MIN_LIMIT              (59)        /* Minite limit */
#define CELLULAR_SEC_LIMIT              (59)        /* Second limit */
#define CELLULAR_TIMEZONE_LIMIT_H       (96)        /* Time Zone limit */
#define CELLULAR_TIMEZONE_LIMIT_L       (-96)       /* Minimum Time Zone */

#define CELLULAR_AP_CONNECT_RETRY_WAIT  (1000)      /* Retry interval when connecting to AP */
#define CELLULAR_CFUN_WAIT              (2000)      /* Wait time when executing CFUN command */
#define CELLULAR_RESET_WAIT             (5000)      /* Wait time when executing RESET command */
#define CELLULAR_COMMAND_TIMEOUT        (10000)     /* AT command response wait time */
#define CELLULAR_MAX_HOSTNAME_LENGTH    (253)       /* Maximum Host name length */
#define CELLULAR_SOCKETCONNECT_DELAY    (5000)      /* Socket Connect Response delay time (millisecond) */
#if (CELLULAR_IMPLEMENT_TYPE == 'B')
#define CELLULAR_SECURITY_PROFILE_ID_H  (6)         /* Maximum Security Profile ID Number  */
#define CELLULAR_SECURITY_PROFILE_ID_L  (1)         /* Minimum Security Profile ID Number  */
#define CELLULAR_MAX_NVM_CERTIFICATE_INDEX  (19)    /* Maximum Certificate Index Number  */
#define CELLULAR_NVM_CERTIFICATE_SIZE_H (16384)     /* Maximum Certificate File Size */
#define CELLULAR_NVM_CERTIFICATE_SIZE_L (1)         /* Minimum Certificate File Size */
#endif

#define RYZ014_ATC_ECHO_OFF                 "ATE0\r"
#define RYZ014_ATC_FUNCTION_LEVEL_CHECK     "AT+CFUN?\r"
#define RYZ014_ATC_FUNCTION_LEVEL           "AT+CFUN=%s\r"
#define RYZ014_ATC_PIN_LOCK_CHECK           "AT+CPIN?\r"
#define RYZ014_ATC_PIN_LOCK_RELEASE         "AT+CPIN=\"%s\"\r"
#define RYZ014_ATC_CONNECT_SOCKET           "AT+SQNSD=%s,%s,%s,\"%s.%s.%s.%s\",0,%s,1\r"
#define RYZ014_ATC_CONNECT_SOCKET_TOHOST    "AT+SQNSD=%s,%s,%s,\"%s\",0,%s,1\r"
#define RYZ014_ATC_CLOSE_SOCKET             "AT+SQNSH=%s\r"
#define RYZ014_ATC_SEND_SCOKET              "AT+SQNSSENDEXT=%s,%s\r"
#define RYZ014_ATC_RECV_SCOKET              "AT+SQNSRECV=%s,%s\r"
#define RYZ014_ATC_DNS_LOOKUP               "AT+SQNDNSLKUP=\"%s\",0\r"
#define RYZ014_ATC_AP_CONFIG                "AT+CGDCONT=1,\"IPV4V6\",\"%s\",,,,0,0,0,0,0,0,1,,0\r"
#define RYZ014_ATC_USER_CONFIG              "AT+CGAUTH=1,1,\"%s\",\"%s\"\r"
#define RYZ014_ATC_SOCKET_CONFIG_1          "AT+SQNSCFG=%s,1,%s,%s,%s,%s\r"
#define RYZ014_ATC_SOCKET_CONFIG_2          "AT+SQNSCFGEXT=%s,1,0,0\r"
#define RYZ014_ATC_CONNECT_CHECK            "AT+CGATT?\r"
#define RYZ014_ATC_SET_CONNECT_STATUS       "AT+CGATT=%s\r"
#define RYZ014_ATC_SHUTDOWN                 "AT+SQNSSHDN\r"
#define RYZ014_ATC_GET_TIME                 "AT+CCLK?\r"
#define RYZ014_ATC_SET_TIME                 "AT+CCLK=\"%s/%s/%s,%s:%s:%s%s\"\r"
#define RYZ014_ATC_RESET                    "AT^RESET\r"
#define RYZ014_ATC_CEREG_OFF                "AT+CEREG=0\r"
#define RYZ014_ATC_AUTO_CONNECT_OFF         "AT+SQNAUTOCONNECT=0\r"
#define RYZ014_ATC_SIM_ST_OFF               "AT+SQNSIMST=0\r"
#define RYZ014_ATC_GET_SERVICE_STATUS       "AT+COPS?\r"
#define RYZ014_ATC_SET_PROVIDER             "AT+COPS=%s,2,\"%s%s\"\r"
#define RYZ014_ATC_GET_PDN_STATE            "AT+CGACT?\r"
#define RYZ014_ATC_ACTIVATE_PDN             "AT+CGACT=1,%s\r"
#define RYZ014_ATC_GET_IPADDR               "AT+CGPADDR=%s\r"
#define RYZ014_ATC_GET_PSM                  "AT+CPSMS?\r"
#define RYZ014_ATC_SET_PSM                  "AT+CPSMS=%s,,,\"%s\",\"%s\"\r"
#define RYZ014_ATC_GET_EDRX                 "AT+SQNEDRX?\r"
#define RYZ014_ATC_SET_EDRX                 "AT+SQNEDRX=%s,4,\"%s\",\"%s\"\r"
#define RYZ014_ATC_GET_SIGNAL_STRENGTH      "AT+CSQ\r"
#define RYZ014_ATC_GET_SW_VERSION           "AT+CGMR\r"
#define RYZ014_ATC_GET_SERIAL_NUMBER        "AT+CGSN\r"
#define RYZ014_ATC_GET_MODULE_NAME          "AT+CGMM\r"
#define RYZ014_ATC_GET_MAKER_NAME           "AT+CGMI\r"
#define RYZ014_ATC_GET_IMSI                 "AT+CIMI\r"
#define RYZ014_ATC_SEND_COMMAND_TO_SIM      "AT+CRSM=%s,%s,%s,%s,%s,\"%s\",\"%s\"\r"
#define RYZ014_ATC_SET_PSM_CONFIG           "AT+SQNIPSCFG=%s,5000\r"
#define RYZ014_ATC_SET_RING_CONFIG          "AT+SQNRICFG=%s,3,1000\r"
#define RYZ014_ATC_SET_IND_NOTIFY           "AT+CMER=3,0,0,%s,0,0,0\r"
#define RYZ014_ATC_SET_SQN_PROVIDER         "AT+SQNCTM=\"%s\"\r"
#define RYZ014_NO_COMMAND                   "\r"
#if (CELLULAR_IMPLEMENT_TYPE == 'B')
#define RYZ014_ATC_WRITE_CERTIFICATE        "AT+SQNSNVW=\"%s\",%s,%s\r"
#define RYZ014_ATC_ERASE_CERTIFICATE        "AT+SQNSNVW=\"%s\",%s,0\r"
#define RYZ014_ATC_CONFIG_SSL_PROFILE       "AT+SQNSPCFG=%s,2,,%s,%s,%s,%s,\"\"\r"
#define RYZ014_ATC_CONFIG_SSL_SOCKET        "AT+SQNSSCFG=%s,%s,%s\r"
#endif

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/
typedef enum
{
    ATC_ECHO_OFF         = 0,
    ATC_FUNCTION_LEVEL_CHECK,
    ATC_FUNCTION_LEVEL,
    ATC_PIN_LOCK_CHECK,
    ATC_PIN_LOCK_RELEASE,
    ATC_CONNECT_SOCKET,
    ATC_CONNECT_SOCKET_TOHOST,
    ATC_CLOSE_SOCKET,
    ATC_SEND_SOCKET,
    ATC_RECV_SOCKET,
    ATC_DNS_LOOKUP,
    ATC_AP_CONFIG,
    ATC_USER_CONFIG,
    ATC_SOCKET_CONFIG_1,
    ATC_SOCKET_CONFIG_2,
    ATC_CONNECT_CHECK,
    ATC_SET_CONNECT_STATUS,
    ATC_SHUTDOWN,
    ATC_GET_TIME,
    ATC_SET_TIME,
    ATC_RESET,
    ATC_CEREG_OFF,
    ATC_AUTO_CONNECT_OFF,
    ATC_SIM_ST_OFF,
    ATC_GET_SERVICE_STATUS,
    ATC_SET_PROVIDER,
    ATC_GET_PDN_STATE,
    ATC_ACTIVATE_PDN,
    ATC_GET_IPADDR,
    ATC_GET_PSM,
    ATC_SET_PSM,
    ATC_GET_EDRXS,
    ATC_SET_EDRXS,
    ATC_GET_SIGNAL_STRENGTH,
    ATC_GET_SW_VERSION,
    ATC_GET_SERIAL_NUMBER,
    ATC_GET_MODULE_NAME,
    ATC_GET_MAKER_NAME,
    ATC_GET_IMSI,
    ATC_SEND_COMMAND_TO_SIM,
    ATC_SET_PSM_CONFIG,
    ATC_SET_RING_CONFIG,
    ATC_SET_IND_NOTIFY,
    ATC_SQNSSENDEXT_END,
#if (CELLULAR_IMPLEMENT_TYPE == 'B')
    ATC_WRITE_CERTIFICATE,
    ATC_ERASE_CERTIFICATE,
    ATC_CONFIG_SSL_PROFILE,
    ATC_CONFIG_SSL_SOCKET,
#endif
    ATC_LIST_MAX
} e_atc_list_t;

typedef enum
{
    ATC_RETURN_NONE = 0,        // No response from the module
    ATC_RETURN_OK,              // Module response is "OK"
    ATC_RETURN_ERROR,           // Module response is "ERROR"
    ATC_RETURN_OK_GO_SEND,      // Module response is ">"
    ATC_RETURN_SEND_NO_CARRIER, // Module response is "NO CARRIER"
    ATC_RETURN_CPIN_READY,      // Module response is "+CPIN: READY"
    ATC_RETURN_SIM_LOCK,        // Module response is "+CPIN: SIM PIN"
    ATC_RETURN_AP_CONNECTING,   // Module response is "CONNECT"
    ATC_RETURN_AP_NOT_CONNECT,  // Module is not connected to the access point
    ATC_RETURN_CFUN1,           // Module control level is 1
    ATC_RETURN_CFUN4,           // Module control level is 4
    ATC_RETURN_ENUM_MAX,        // Maximum enumeration value
} e_cellular_atc_return_t;

typedef enum
{
    ATC_AP_DISCONNECT = 0,  // Disconnect from AP
    ATC_AP_CONNECT,         // Connect to AP
} e_atc_ap_status_t;

#endif /* #define RYZ014_PRIVATE_H */
