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
 * File Name    : ryz014_private.h
 * Description  : Internal configuration settings for RYZ014A driver.
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

#define CELLULAR_RTS_DELAYTIME          (600)       /* Delay time after RTS (millisecond) */

#define CELLULAR_MAX_ARG_COUNT          (8)         /* Maximum number of arguments to an AT command */

#define CELLULAR_START_SOCKET_NUMBER    (1)         /* Start socket number */
#define CELLULAR_PACKET_DATA_SIZE       (0)         /* Packet data size (0~1500,0=auto) */
#define CELLULAR_CONNECT_TIMEOUT        (200)       /* connection timeout (0.1sec,10~1200,0=no_limit) */
#define CELLULAR_TX_TIMEOUT             (10)        /* Transmit timeout (0.1sec,0~255,0=no_limit) */
#define CELLULAR_BAUDRATE               (921600)    /* Baudrate */
#define CELLULAR_CREATABLE_SOCKETS      (6)         /* Number of sockets to create (1~6) */
#define CELLULAR_MAX_TX_SIZE            (1500)      /* Data processing size per transmission (1~1500) */
#define CELLULAR_MAX_RX_SIZE            (1500)      /* Data processing size per reception (1~1500) */

#define CELLULAR_PACKET_DATA_SIZE_LIMIT     (1500)      /* Packet data size limit */
#define CELLULAR_CONNECT_TIMEOUT_LIMIT_L    (10)        /* Minimum connection time */
#define CELLULAR_CONNECT_TIMEOUT_LIMIT_H    (1200)      /* Connection time limit  */
#define CELLULAR_TX_SIZE_LIMIT_L            (1)         /* Minimum Sending size */
#define CELLULAR_TX_SIZE_LIMIT_H            (1500)      /* Sending size limit */
#define CELLULAR_RX_SIZE_LIMIT_L            (1)         /* Minimum Receive size */
#define CELLULAR_RX_SIZE_LIMIT_H            (1500)      /* Receive size limit */
#define CELLULAR_YEAR_LIMIT                 (99)        /* Year limit */
#define CELLULAR_MONTH_LIMIT                (12)        /* Month limit */
#define CELLULAR_DAY_LIMIT                  (31)        /* Day limit */
#define CELLULAR_HOUR_LIMIT                 (23)        /* Hour limit */
#define CELLULAR_MIN_LIMIT                  (59)        /* Minute limit */
#define CELLULAR_SEC_LIMIT                  (59)        /* Second limit */
#define CELLULAR_TIMEZONE_LIMIT_H           (96)        /* Time Zone limit */
#define CELLULAR_TIMEZONE_LIMIT_L           (-96)       /* Minimum Time Zone */

#define CELLULAR_AP_CONNECT_RETRY_WAIT  (1000)      /* Retry interval when connecting to AP */
#define CELLULAR_CFUN_WAIT              (2000)      /* Wait time when executing CFUN command */
#define CELLULAR_RESET_WAIT             (5000)      /* Wait time when executing RESET command */
#define CELLULAR_COMMAND_TIMEOUT        (60000)     /* AT command response wait time */
#define CELLULAR_MAX_HOSTNAME_LENGTH    (253)       /* Maximum Host name length */
#define CELLULAR_SOCKETCONNECT_DELAY    (5000)      /* Socket Connect Response delay time (millisecond) */
#define CELLULAR_SQNRICFG_MODE          (2)         /* All URCs are indicated by activated RING0 line of the UART0 interface, whatever URC AT channel origin */
#define CELLULAR_SQNIPSCFG_MODE         (1)         /* Power saving is activated. Module power state is controlled by RTS0 line*/
#define CELLULAR_PING_REQ_MIN           (1)         /* Minimum number of ping echo requests to send */
#define CELLULAR_PING_REQ_DEFAULT       (4)         /* Default number of ping echo requests to send */
#define CELLULAR_PING_REQ_MAX           (64)        /* Maximum number of ping echo requests to send */
#define CELLULAR_PING_MES_MIN           (32)        /* Minimum length of ping echo request message */
#define CELLULAR_PING_MES_MAX           (1400)      /* Maximum length of ping echo request message */
#define CELLULAR_PING_INTER_MIN         (1)         /* Minimum Echo Interval (second) */
#define CELLULAR_PING_INTER_MAX         (600)       /* Maximum Echo Interval (second) */
#define CELLULAR_PING_TIMEOUT_MIN       (1)         /* Minimum time to wait for Echo reply (second) */
#define CELLULAR_PING_TIMEOUT_DEFAULT   (10)        /* Default time to wait for Echo reply (second) */
#define CELLULAR_PING_TIMEOUT_MAX       (60)        /* Maximum time to wait for Echo reply (second) */

#define CELLULAR_MAX_NVM_CERTIFICATE_INDEX  (19)    /* Maximum Certificate Index Number  */
#define CELLULAR_SECURITY_PROFILE_ID_H      (6)     /* Maximum Security Profile ID Number  */
#define CELLULAR_SECURITY_PROFILE_ID_L      (1)     /* Minimum Security Profile ID Number  */
#define CELLULAR_NVM_CERTIFICATE_SIZE_H     (16384) /* Maximum Certificate File Size */
#define CELLULAR_NVM_CERTIFICATE_SIZE_L     (1)     /* Minimum Certificate File Size */

#define RYZ014_ATC_ECHO_OFF                 "ATE0\r"
#define RYZ014_ATC_FUNCTION_LEVEL_CHECK     "AT+CFUN?\r"
#define RYZ014_ATC_FUNCTION_LEVEL           "AT+CFUN=%s\r"
#define RYZ014_ATC_PIN_LOCK_CHECK           "AT+CPIN?\r"
#define RYZ014_ATC_PIN_LOCK_RELEASE         "AT+CPIN=\"%s\"\r"
#define RYZ014_ATC_CONNECT_SOCKET           "AT+SQNSD=%s,%s,%s,\"%s\",0,%s,1\r"
#define RYZ014_ATC_CLOSE_SOCKET             "AT+SQNSH=%s\r"
#define RYZ014_ATC_SEND_SCOKET              "AT+SQNSSENDEXT=%s,%s\r"
#define RYZ014_ATC_RECV_SCOKET              "AT+SQNSRECV=%s,%s\r"
#define RYZ014_ATC_DNS_LOOKUP               "AT+SQNDNSLKUP=\"%s\",%s\r"
#define RYZ014_ATC_AP_CONFIG                "AT+CGDCONT=1,\"IPV4V6\",\"%s\",,,,0,0,0,0,0,0,1,,0\r"
#define RYZ014_ATC_PRIVATE_AP_CONFIG        "AT+CGDCONT=%s,\"IPV4V6\",\"%s\"\r"
#define RYZ014_ATC_AP_CONFIG_CHECK          "AT+CGDCONT?\r"
#define RYZ014_ATC_USER_CONFIG              "AT+CGAUTH=1,%s,\"%s\",\"%s\"\r"
#define RYZ014_ATC_CLEAR_CONFIG             "AT+CGAUTH=1,0\r"
#define RYZ014_ATC_SOCKET_CONFIG_1          "AT+SQNSCFG=%s,1,%s,%s,%s,%s\r"
#define RYZ014_ATC_SOCKET_CONFIG_2          "AT+SQNSCFGEXT=%s,1,0,0\r"
#define RYZ014_ATC_LISTENING_SOCKET         "AT+SQNSL=%s,%s,%s,0\r"
#define RYZ014_ATC_CONNECT_CHECK            "AT+CGATT?\r"
#define RYZ014_ATC_SET_CONNECT_STATUS       "AT+CGATT=%s\r"
#define RYZ014_ATC_SHUTDOWN                 "AT+SQNSSHDN\r"
#define RYZ014_ATC_GET_TIME                 "AT+CCLK?\r"
#define RYZ014_ATC_SET_TIME                 "AT+CCLK=\"%s/%s/%s,%s:%s:%s%s\"\r"
#define RYZ014_ATC_RESET                    "AT^RESET\r"
#define RYZ014_ATC_SET_NOTICE_LEVEL         "AT+CEREG=%s\r"
#define RYZ014_ATC_GET_NOTICE_LEVEL         "AT+CEREG?\r"
#define RYZ014_ATC_AUTO_CONNECT             "AT+SQNAUTOCONNECT=%s\r"
#define RYZ014_ATC_AUTO_CONNECT_CHECK       "AT+SQNAUTOCONNECT?\r"
#define RYZ014_ATC_SIM_ST_OFF               "AT+SQNSIMST=0\r"
#define RYZ014_ATC_GET_SERVICE_STATUS       "AT+COPS?\r"
#define RYZ014_ATC_SET_PROVIDER             "AT+COPS=%s,2,\"%s%s\"\r"
#define RYZ014_ATC_GET_PDN_STATE            "AT+CGACT?\r"
#define RYZ014_ATC_ACTIVATE_PDN             "AT+CGACT=1,%s\r"
#define RYZ014_ATC_GET_IPADDR               "AT+CGPADDR=1\r"
#define RYZ014_ATC_GET_PSM                  "AT+CPSMS?\r"
#define RYZ014_ATC_SET_PSM                  "AT+CPSMS=%s,,,\"%s\",\"%s\"\r"
#define RYZ014_ATC_GET_EDRX                 "AT+SQNEDRX?\r"
#define RYZ014_ATC_SET_EDRX                 "AT+SQNEDRX=%s,4,\"%s\",\"%s\"\r"
#define RYZ014_ATC_GET_SIGNAL_STRENGTH      "AT+CSQ\r"
#define RYZ014_ATC_GET_SW_REVISION          "AT+CGMR\r"
#define RYZ014_ATC_GET_SERIAL_NUMBER        "AT+CGSN\r"
#define RYZ014_ATC_GET_SVN                  "AT+CGSN=3\r"
#define RYZ014_ATC_GET_MODULE_NAME          "AT+CGMM\r"
#define RYZ014_ATC_GET_MAKER_NAME           "AT+CGMI\r"
#define RYZ014_ATC_GET_IMSI                 "AT+CIMI\r"
#define RYZ014_ATC_SEND_COMMAND_TO_SIM      "AT+CRSM=%s,%s,%s,%s,%s,\"%s\",\"%s\"\r"
#define RYZ014_ATC_SET_INTER_CONFIG         "AT+SQNIPSCFG=%s,%s\r"
#define RYZ014_ATC_SET_RING_CONFIG          "AT+SQNRICFG=%s,3,%s\r"
#define RYZ014_ATC_SET_PSM_CONFIG           "AT+SQNPSCFG=%s\r"
#define RYZ014_ATC_SET_IND_NOTIFY           "AT+CMER=3,0,0,%s,0,0,0\r"
#define RYZ014_ATC_GET_PHONE_NUM            "AT+CNUM\r"
#define RYZ014_ATC_GET_ICCID                "AT+SQNCCID?\r"
#define RYZ014_ATC_PING                     "AT+PING=\"%s\",%s,%s,%s,%s\r"
#define RYZ014_ATC_GET_CELLINFO             "AT+SQNMONI=%s\r"
#define RYZ014_ATC_SET_CTM                  "AT+SQNCTM=\"%s\"\r"
#define RYZ014_ATC_GET_CTM                  "AT+SQNCTM?\r"
#define RYZ014_ATC_SET_BAND                 "AT+SQNBANDSEL=0,\"%s\",\"%s\"\r"
#define RYZ014_ATC_FACTORYRESET             "AT+SQNSFACTORYRESET\r"
#define RYZ014_ATC_SMCWRX                   "AT+SMCWRX=%s\r"
#define RYZ014_ATC_SMCWTX                   "AT+SMCWTX=%s,%s,%s\r"
#define RYZ014_ATC_CGPIAF                   "AT+CGPIAF=1,0,1,0\r"
#define RYZ014_ATC_CEER                     "AT+CEER\r"
#define RYZ014_ATC_FIRMUPGRADE              "AT+SQNSUPGRADE=\"%s\",%s,5,%s\r"
#define RYZ014_ATC_FIRMUPGRADE_SSL          "AT+SQNSUPGRADE=\"%s\",%s,5,%s,%s\r"
#define RYZ014_ATC_FIRMUPGRADE_CHECK        "AT+SQNSUPGRADE?\r"
#define RYZ014_ATC_GET_LR_SVN               "ATI1\r"
#define RYZ014_ATC_WRITE_CERTIFICATE        "AT+SQNSNVW=\"%s\",%s,%s\r"
#define RYZ014_ATC_ERASE_CERTIFICATE        "AT+SQNSNVW=\"%s\",%s,0\r"
#define RYZ014_ATC_GET_CERTIFICATE          "AT+SQNSNVR=\"%s\",%s\r"
#define RYZ014_ATC_CONFIG_SSL_PROFILE       "AT+SQNSPCFG=%s,2,,%s,%s,%s,%s,\"\"\r"
#if (CELLULAR_IMPLEMENT_TYPE == 'B')
#define RYZ014_ATC_CONFIG_SSL_SOCKET        "AT+SQNSSCFG=%s,%s,%s\r"
#endif
#define RYZ014_NO_COMMAND                   "\r"

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
    ATC_CLOSE_SOCKET,
    ATC_SEND_SOCKET,
    ATC_RECV_SOCKET,
    ATC_DNS_LOOKUP,
    ATC_AP_CONFIG,
    ATC_PRIVATE_AP_CONFIG,
    ATC_AP_CONFIG_CHECK,
    ATC_USER_CONFIG,
    ATC_CLEAR_CONFIG,
    ATC_SOCKET_CONFIG_1,
    ATC_SOCKET_CONFIG_2,
    ATC_LISTENING_SOCKET,
    ATC_CONNECT_CHECK,
    ATC_SET_CONNECT_STATUS,
    ATC_SHUTDOWN,
    ATC_GET_TIME,
    ATC_SET_TIME,
    ATC_RESET,
    ATC_SET_NOTICE_LEVEL,
    ATC_GET_NOTICE_LEVEL,
    ATC_AUTO_CONNECT,
    ATC_AUTO_CONNECT_CHECK,
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
    ATC_GET_SW_REVISION,
    ATC_GET_SERIAL_NUMBER,
    ATC_GET_SVN,
    ATC_GET_MODULE_NAME,
    ATC_GET_MAKER_NAME,
    ATC_GET_IMSI,
    ATC_SEND_COMMAND_TO_SIM,
    ATC_SET_INTER_CONFIG,
    ATC_SET_RING_CONFIG,
    ATC_SET_PSM_CONFIG,
    ATC_SET_IND_NOTIFY,
    ATC_GET_PHONE_NUM,
    ATC_GET_ICCID,
    ATC_PING,
    ATC_GET_CELLINFO,
    ATC_SET_CTM,
    ATC_GET_CTM,
    ATC_SET_BAND,
    ATC_FACTORYRESET,
    ATC_SMCWRX,
    ATC_SMCWTX,
    ATC_IPV6_CONFIG,
    ATC_CONNECT_LOSS_INFO,
    ATC_FIRMUPGRADE,
    ATC_FIRMUPGRADE_SSL,
    ATC_FIRMUPGRADE_CHECK,
    ATC_GET_LR_SVN,
    ATC_WRITE_CERTIFICATE,
    ATC_ERASE_CERTIFICATE,
    ATC_GET_CERTIFICATE,
    ATC_CONFIG_SSL_PROFILE,
#if (CELLULAR_IMPLEMENT_TYPE == 'B')
    ATC_CONFIG_SSL_SOCKET,
#endif
    ATC_SQNSSENDEXT_END,
    ATC_LIST_MAX
} e_atc_list_t;

typedef enum
{
    ATC_RETURN_NONE = 0,        // No response from the module
    ATC_RETURN_OK,              // Module response is "OK"
    ATC_RETURN_ERROR,           // Module response is "ERROR"
    ATC_RETURN_OK_GO_SEND,      // Module response is ">"
    ATC_RETURN_SEND_NO_CARRIER, // Module response is "NO CARRIER"
    ATC_RETURN_AP_CONNECTING,   // Module response is "CONNECT"
    ATC_RETURN_ENUM_MAX,        // Maximum enumeration value
} e_cellular_atc_return_t;

typedef enum
{
    ATC_AP_DISCONNECT = 0,  // Disconnect from AP
    ATC_AP_CONNECT,         // Connect to AP
} e_atc_ap_status_t;

#endif /* #define RYZ014_PRIVATE_H */
