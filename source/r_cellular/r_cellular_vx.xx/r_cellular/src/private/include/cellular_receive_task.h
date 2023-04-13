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
 * File Name    : cellular_receive_task.h
 * Description  : Configures the driver.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_cellular_if.h"

#ifndef CELLULAR_RECEIVE_TASK_H
#define CELLULAR_RECEIVE_TASK_H

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#define CELLULAR_IDLE_PRIORITY      (6)

#define ATC_RES_BEGIN_OR_END        "\r\n"

#define ATC_RES_GO_SEND             ">"
#define ATC_RES_OK                  "OK\r\n"
#define ATC_RES_ERROR               "ERROR\r\n"
#define ATC_RES_NO_CARRIER          "NO CARRIER\r\n"
#define ATC_RES_CONNECT             "CONNECT\r\n"
#define ATC_RES_EXIT                "^EXIT:"
#define ATC_RES_DATA_RECEIVE        "+SQNSRING:"
#define ATC_RES_DATA_RECEIVE_QIRD   "+SQNSRECV:"
#define ATC_RES_READ_DNS            "+SQNDNSLKUP:"
#define ATC_RES_PIN_LOCK_STATUS     "+CPIN:"
#define ATC_RES_SOCKET_CLOSE        "+SQNSH:"
#define ATC_RES_SYSTEM_START        "+SYSSTART\r\n"
#define ATC_RES_ATTACH_STATUS       "+CGATT:"
#define ATC_RES_AP_CONNECT_CONFIG   "+CGDCONT:"
#define ATC_RES_FUNCTION_LEVEL      "+CFUN:"
#define ATC_RES_TIMEZONE            "+CTZE:"
#define ATC_RES_CREG_STATUS         "+CREG:"
#define ATC_RES_CEREG_STATUS        "+CEREG:"
#define ATC_RES_GET_TIME            "+CCLK:"
#define ATC_RES_GET_SERVICE_STATUS  "+COPS:"
#define ATC_RES_GET_PDP_STATUS      "+CGACT:"
#define ATC_RES_GET_IP_ADDR         "+CGPADDR:"
#define ATC_RES_GET_PSMS            "+CPSMS:"
#define ATC_RES_GET_EDRXS           "+SQNEDRX:"
#define ATC_RES_SET_EDRXS           "+CEDRXP:"
#define ATC_RES_GET_SIGNAL          "+CSQ:"
#define ATC_RES_COMMAND_SEND_SIM    "+CRSM:"
#define ATC_RES_TIMEZONE_INFO       "+CTZV:"
#define ATC_RES_IND_INFO            "+CIEV:"
#define ATC_RES_SVN                 "+CGSN:"
#define ATC_RES_GET_PHONE_NUM       "+CNUM:"
#define ATC_RES_GET_ICCID           "+SQNCCID:"
#define ATC_RES_PING                "+PING:"
#define ATC_RES_GET_CELLINFO        "+SQNMONI:"
#define ATC_RES_GET_AUTOCONNECT     "+SQNAUTOCONNECT:"
#define ATC_RES_GET_CTM             "+SQNCTM:"
#define ATC_RES_SMCWRX              "+SMCWRX:"
#define ATC_RES_SMCWTX              "+SMCWTX:"
#define ATC_RES_SHUTDOWN            "+SHUTDOWN"
#define ATC_RES_FIRMUPGRADE         "+SQNSUPGRADE:"
#define ATC_RES_GET_CERTIFICATE     "+SQNSNVR:"

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/
typedef enum
{
    CELLULAR_RES_GO_SEND = 0,           // Request for Data Transmission
    CELLULAR_RES_OK,                    // Response is OK
    CELLULAR_RES_ERROR,                 // Response is ERROR
    CELLULAR_RES_NO_CARRIER,            // Response is NO CARRIER
    CELLULAR_RES_CONNECT,               // Response is CONNECT
    CELLULAR_RES_EXIT,                  // Exit error detected (module is automatically restarted)
    CELLULAR_RES_DATA_RECEIVE,          // Data receipt notification
    CELLULAR_RES_DATA_RECEIVE_QIRD,     // Receipt data delivery request
    CELLULAR_RES_READ_DNS,              // DNS query results
    CELLULAR_RES_PIN_LOCK_STATUS,       // Check SIM lock status
    CELLULAR_RES_SOCKET_CLOSE,          // Socket Disconnection Notification
    CELLULAR_RES_SYSTEM_START,          // Module activation notification
    CELLULAR_RES_ATTACH_STATUS,         // Connection status to access point
    CELLULAR_GET_AP_CONNECT_CONFIG,     // Get AP connect config
    CELLULAR_RES_FUNCTION_LEVEL,        // Module control level
    CELLULAR_RES_TIMEZONE,              // Time zone notification
    CELLULAR_RES_CREG_STATUS,           // Connected base station information
    CELLULAR_RES_CEREG_STATUS,          // Connected base station information
    CELLULAR_RES_GET_TIME,              // Time acquisition process
    CELLULAR_GET_SERVICE_STATUS,        // Get service status
    CELLULAR_GET_PDP_STATUS,            // Get PDP status
    CELLULAR_GET_IP_ADDR,               // Get the IP address in use
    CELLULAR_GET_PSMS,                  // Get the power save mode setting
    CELLULAR_GET_EDRXS,                 // Get the low power mode setting
    CELLULAR_SET_EDRXS,                 // Setting the low power mode
    CELLULAR_GET_SIGNAL_QUALITY,        // Get signal strength
    CELLULAR_RES_TO_COMMAND_SEND_SIM,   // SIM access results
    CELLULAR_RES_TIMEZONE_INFO,         // Get time zone information
    CELLULAR_RES_IND_INFO,              // Get indicator notifications
    CELLULAR_GET_SVN,                   // Get software version number
    CELLULAR_GET_PHONE_NUMBER,          // Get phone number
    CELLULAR_GET_ICCID,                 // Get ICCID
    CELLULAR_PING,                      // Perform PING
    CELLULAR_GET_CELLINFO,              // Get cell information
    CELLULAR_GET_AUTOCONNECT,           // Get auto-connect settings
    CELLULAR_GET_CTM,                   // Get Conformance Test Mode
    CELLULAR_SMCWRX,                    // SMCWRX Command results
    CELLULAR_SMCWTX,                    // SMCWTX Command results
    CELLULAR_SHUTDOWN,                  // Module shutdown notification
    CELLULAR_FIRMUPGRADE,               // Firm Upgrade Status Notification
    CELLULAR_GET_CERTIFICATE,           // Get Certificate
    CELLULAR_RES_MAX,                   // End of analysis result processing

    CELLULAR_RES_PUT_CHAR,              // Received data storage process
    CELLULAR_RES_CHECK,                 // Receipt confirmation process
    CELLULAR_RES_NONE,                  // No process
} e_atc_return_code_t;

typedef enum
{
    JOB_STATUS_NONE= 0,                 // AT command response being analyzed
    JOB_STATUS_FIRST_CHAR_CHECK,        // First character check
    JOB_STATUS_COLON_CHECK,             // Confirm receipt of colon
    JOB_STATUS_MAX,                     // End of enum
} e_job_status_t;

typedef struct cellular_receive
{
    uint8_t             data;           // Data received from module
    uint8_t             overflow_flag;  // Receive buffer overflow flag
    e_atc_return_code_t job_no;         // Execution process number
    e_job_status_t      job_status;     // Execution process confirmation number
    uint8_t             socket_no;      // Socket number
    uint32_t            recv_count;     // Received bytes
    uint32_t            tmp_recvcnt;    // Processing start point
} st_cellular_receive_t;

#if CELLULAR_CFG_URC_CHARGET_ENABLED == 1
extern void CELLULAR_CFG_URC_CHARGET_FUNCTION (void * p_args);
#endif

#endif /* CELLULAR_RECEIVE_TASK_H */
