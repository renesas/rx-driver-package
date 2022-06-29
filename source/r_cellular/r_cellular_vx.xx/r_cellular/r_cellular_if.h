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
 * File Name    : r_cellular_if.h
 * Version      : 1.04
 * Description  : Configures the driver.
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

#ifndef CELLULAR_IF_H
#define CELLULAR_IF_H

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "r_bsp_config.h"
#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
#include "r_sci_rx_if.h"
#include "r_sci_rx_pinset.h"
#include "r_irq_rx_if.h"
#elif defined(__CCRL__) || defined(__ICCRL78__) || defined(__RL)
#include "r_sci_rl_if.h"
#endif

#include "r_cellular_config.h"
#include "r_cellular_private.h"
#include "ryz014_private.h"

#if BSP_CFG_RTOS_USED == (1)
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "event_groups.h"
#elif BSP_CFG_RTOS_USED == (5)
#include <tx_api.h>
#include <tx_mutex.h>
#include <tx_thread.h>
#include <tx_semaphore.h>
#endif

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/
typedef enum
{
    CELLULAR_SUCCESS            = 0,            // Successful
    CELLULAR_ERR_PARAMETER      = -1,           // Argument is abnormal
    CELLULAR_ERR_ALREADY_OPEN   = -2,           // Already initialized
    CELLULAR_ERR_NOT_OPEN       = -3,           // Not initialized
    CELLULAR_ERR_SERIAL_OPEN    = -4,           // Serial initialization failed.
    CELLULAR_ERR_MODULE_COM     = -5,           // Communication with module failed
    CELLULAR_ERR_NOT_CONNECT    = -6,           // Not connected to access point
    CELLULAR_ERR_ALREADY_CONNECT = -7,          // Already connect to access point
    CELLULAR_ERR_ALREADY_SOCKET_CONNECT = -8,   // Already socket connect
    CELLULAR_ERR_SOCKET_NOT_READY = -9,         // Socket is not ready for use
    CELLULAR_ERR_SOCKET_CREATE_LIMIT = -10,     // Socket creation limit exceeded
    CELLULAR_ERR_BYTEQ_OPEN     = -11,          // Failed to initialize the byte queue
    CELLULAR_ERR_SEMAPHORE_INIT = -12,          // Failed to initialize the semaphore
    CELLULAR_ERR_EVENT_GROUP_INIT = -13,        // Failed to initialize the event group
    CELLULAR_ERR_CREATE_TASK    = -14,          // Failed to create task
    CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING = -15, // Other AT commands are running
    CELLULAR_ERR_MEMORY_ALLOCATION  = -16,      // Failed to allocate memory
} e_cellular_err_t;

typedef enum
{
    CELLULAR_SYSTEM_CLOSE = 0,  // Open function unexecuted
    CELLULAR_SYSTEM_OPEN,       // Open function already executed
    CELLULAR_SYSTEM_CONNECT,    // Connecting to AP
} e_cellular_system_status_t;

typedef enum
{
    CELLULAR_MODULE_OPERATING_LEVEL0 = 0,    // Default
    CELLULAR_MODULE_OPERATING_LEVEL1,        // Full functional use possible
    CELLULAR_MODULE_OPERATING_LEVEL2,        // Transmission function disabled
    CELLULAR_MODULE_OPERATING_LEVEL3,        // Reception function disabled
    CELLULAR_MODULE_OPERATING_LEVEL4,        // In-flight mode
} e_cellular_module_status_t;

typedef enum
{
    CELLULAR_SOCKET_STATUS_CLOSED = 0,  // Socket uncreated
    CELLULAR_SOCKET_STATUS_SOCKET,      // Socket creation
    CELLULAR_SOCKET_STATUS_CONNECTED,   // Socket connected
    CELLULAR_SOCKET_STATUS_MAX,         // End of enumeration
} e_cellular_socket_status_t;

typedef enum
{
    CELLULAR_TX_END_FLAG_OFF = 0,   // TX end flag off
    CELLULAR_TX_END_FLAG_ON,        // TX end flag on
} e_cellular_tx_end_flg_t;

typedef enum
{
    CELLULAR_RECEIVE_FLAG_OFF = 0,  // Receive flag off
    CELLULAR_RECEIVE_FLAG_ON,       // Receive flag off
} e_cellular_receive_flg_t;

typedef enum
{
    CELLULAR_ATC_OK                 = 0,    // Successful
    CELLULAR_ATC_ERR_PARAMETER      = -1,   // Argument is abnormal
    CELLULAR_ATC_ERR_MODULE_COM     = -2,   // Communication with module failed
    CELLULAR_ATC_ERR_COMPARE        = -3,   // The return value from the module is disappointing.
    CELLULAR_ATC_ERR_TIMEOUT        = -4,   // Time out
} e_cellular_err_atc_t;

typedef enum
{
    CELLULAR_NOT_TIMEOUT    = 0,    // No timeout.
    CELLULAR_TIMEOUT        = -1,   // Timeout
} e_cellular_timeout_check_t;

typedef enum
{
    CELLULAR_SEMAPHORE_SUCCESS  = 0,    // Semaphore processing succeeded
    CELLULAR_SEMAPHORE_ERR_GIVE = -1,   // Semaphore release failed
    CELLULAR_SEMAPHORE_ERR_TAKE = -2,   // Semaphore acquisition failed.
} e_cellular_err_semaphore_t;

typedef enum
{
    CELLULAR_TIMEOUT_NOT_OVERFLOW = 0,  // There is no overflow in the timeout setting
    CELLULAR_TIMEOUT_OVERFLOW,          // Overflow in timeout setting
} e_cellular_timeout_ovf_check_t;

typedef enum
{
    CELLULAR_ATC_RESPONCE_CONFIRMED = 0,    // AT command response confirmed
    CELLULAR_ATC_RESPONCE_UNCONFIRMED,      // AT command response Unconfirmed
} e_cellular_atc_res_check_t;

typedef enum
{
    CELLULAR_EDRX_MODE_INVALID        = 0,  // Disable the edrx function
    CELLULAR_EDRX_MODE_ACTIVE         = 1,  // Enable the edrx function
    CELLULAR_EDRX_MODE_ACTIVE_RESULT  = 2,  // Activate the edrx function and return the results
    CELLULAR_EDRX_MODE_INIT           = 3,  // Initialize and disable the edrx function
} e_cellular_edrx_mode_t;

typedef enum
{
    CELLULAR_EDRX_CYCLE_5_SEC = 0,  // edrx cycle(5.12sec)
    CELLULAR_EDRX_CYCLE_10_SEC,     // edrx cycle(10.24sec)
    CELLULAR_EDRX_CYCLE_20_SEC,     // edrx cycle(20.48sec)
    CELLULAR_EDRX_CYCLE_40_SEC,     // edrx cycle(40.96sec)
    CELLULAR_EDRX_CYCLE_81_SEC,     // edrx cycle(81.92sec)
    CELLULAR_EDRX_CYCLE_163_SEC,    // edrx cycle(163.84sec)
    CELLULAR_EDRX_CYCLE_327_SEC,    // edrx cycle(327.68sec)
    CELLULAR_EDRX_CYCLE_655_SEC,    // edrx cycle(655.36sec)
    CELLULAR_EDRX_CYCLE_1310_SEC,   // edrx cycle(1,310.72sec)
    CELLULAR_EDRX_CYCLE_2621_SEC,   // edrx cycle(2,621.44sec)
} e_cellular_edrx_cycle_t;

typedef enum
{
    CELLULAR_PTW_CYCLE_NONE = 0,    // PTW(PTW not used)
    CELLULAR_PTW_CYCLE_1_SEC,       // PTW(1sec)
    CELLULAR_PTW_CYCLE_2_SEC,       // PTW(2sec)
    CELLULAR_PTW_CYCLE_3_SEC,       // PTW(3sec)
    CELLULAR_PTW_CYCLE_4_SEC,       // PTW(4sec)
    CELLULAR_PTW_CYCLE_5_SEC,       // PTW(5sec)
    CELLULAR_PTW_CYCLE_6_SEC,       // PTW(6sec)
    CELLULAR_PTW_CYCLE_7_SEC,       // PTW(7sec)
    CELLULAR_PTW_CYCLE_8_SEC,       // PTW(8sec)
    CELLULAR_PTW_CYCLE_9_SEC,       // PTW(9sec)
    CELLULAR_PTW_CYCLE_10_SEC,      // PTW(10sec)
    CELLULAR_PTW_CYCLE_12_SEC,      // PTW(12sec)
    CELLULAR_PTW_CYCLE_14_SEC,      // PTW(14sec)
    CELLULAR_PTW_CYCLE_16_SEC,      // PTW(16sec)
    CELLULAR_PTW_CYCLE_18_SEC,      // PTW(18sec)
    CELLULAR_PTW_CYCLE_20_SEC,      // PTW(20sec)
} e_cellular_ptw_cycle_t;

typedef enum
{
    CELLULAR_PSM_MODE_INVALID        = 0,   // Disable the PSM function
    CELLULAR_PSM_MODE_ACTIVE         = 1,   // Enable the PSM function
    CELLULAR_PSM_MODE_INIT           = 2,   // Initialize and disable the PSM function
} e_cellular_psm_mode_t;

typedef enum
{
    CELLULAR_TAU_CYCLE_10_MIN = 0,  // TAU cycle(10min)
    CELLULAR_TAU_CYCLE_1_HOUR,		// TAU cycle(1hour)
    CELLULAR_TAU_CYCLE_10_HOUR,     // TAU cycle(10hour)
    CELLULAR_TAU_CYCLE_2_SEC,       // TAU cycle(2sec)
    CELLULAR_TAU_CYCLE_30_SEC,      // TAU cycle(30sec)
    CELLULAR_TAU_CYCLE_1_MIN,       // TAU cycle(1min)
    CELLULAR_TAU_CYCLE_320_HOUR,    // TAU cycle(320hour)
    CELLULAR_TAU_CYCLE_NONE,        // TAU cycle(Timer is deactivated)
} e_cellular_tau_cycle_t;

typedef enum
{
    CELLULAR_ACTIVE_CYCLE_2_SEC = 0,    // Active time(2sec)
    CELLULAR_ACTIVE_CYCLE_1_MIN,		// Active time(1min)
    CELLULAR_ACTIVE_CYCLE_6_MIN,        // Active time(6min)
    CELLULAR_ACTIVE_CYCLE_NONE,         // Active time(Timer is deactivated)
} e_cellular_active_cycle_t;

typedef enum
{
    CELLULAR_CYCLE_MULTIPLIER_0 = 0,    // Multiplier 0
    CELLULAR_CYCLE_MULTIPLIER_1 = 1,    // Multiplier 1
    CELLULAR_CYCLE_MULTIPLIER_2 = 2,    // Multiplier 2
    CELLULAR_CYCLE_MULTIPLIER_3 = 3,    // Multiplier 3
    CELLULAR_CYCLE_MULTIPLIER_4 = 4,    // Multiplier 4
    CELLULAR_CYCLE_MULTIPLIER_5 = 5,    // Multiplier 5
    CELLULAR_CYCLE_MULTIPLIER_6 = 6,    // Multiplier 6
    CELLULAR_CYCLE_MULTIPLIER_7 = 7,    // Multiplier 7
    CELLULAR_CYCLE_MULTIPLIER_8 = 8,    // Multiplier 8
    CELLULAR_CYCLE_MULTIPLIER_9 = 9,    // Multiplier 9
    CELLULAR_CYCLE_MULTIPLIER_10 = 10,  // Multiplier 10
    CELLULAR_CYCLE_MULTIPLIER_11 = 11,  // Multiplier 11
    CELLULAR_CYCLE_MULTIPLIER_12 = 12,  // Multiplier 12
    CELLULAR_CYCLE_MULTIPLIER_13 = 13,  // Multiplier 13
    CELLULAR_CYCLE_MULTIPLIER_14 = 14,  // Multiplier 14
    CELLULAR_CYCLE_MULTIPLIER_15 = 15,  // Multiplier 15
    CELLULAR_CYCLE_MULTIPLIER_16 = 16,  // Multiplier 16
    CELLULAR_CYCLE_MULTIPLIER_17 = 17,  // Multiplier 17
    CELLULAR_CYCLE_MULTIPLIER_18 = 18,  // Multiplier 18
    CELLULAR_CYCLE_MULTIPLIER_19 = 19,  // Multiplier 19
    CELLULAR_CYCLE_MULTIPLIER_20 = 20,  // Multiplier 20
    CELLULAR_CYCLE_MULTIPLIER_21 = 21,  // Multiplier 21
    CELLULAR_CYCLE_MULTIPLIER_22 = 22,  // Multiplier 22
    CELLULAR_CYCLE_MULTIPLIER_23 = 23,  // Multiplier 23
    CELLULAR_CYCLE_MULTIPLIER_24 = 24,  // Multiplier 24
    CELLULAR_CYCLE_MULTIPLIER_25 = 25,  // Multiplier 25
    CELLULAR_CYCLE_MULTIPLIER_26 = 26,  // Multiplier 26
    CELLULAR_CYCLE_MULTIPLIER_27 = 27,  // Multiplier 27
    CELLULAR_CYCLE_MULTIPLIER_28 = 28,  // Multiplier 28
    CELLULAR_CYCLE_MULTIPLIER_29 = 29,  // Multiplier 29
    CELLULAR_CYCLE_MULTIPLIER_30 = 30,  // Multiplier 30
    CELLULAR_CYCLE_MULTIPLIER_31 = 31,  // Multiplier 31
} e_cellular_cycle_multiplier_t;

#if (CELLULAR_IMPLEMENT_TYPE == 'B')
typedef enum
{
    CELLULAR_NVM_TYPE_CERTIFICATE = 0,  //Certificate
    CELLULAR_NVM_TYPE_PRIVATEKEY        //PrivateKey
} e_cellular_nvm_type_t;

typedef enum
{
    CELLULAR_NO_CERT_VALIDATE = 0,          //certificate not validated
    CELLULAR_VALIDATE_CERT_EXPDATE = 1,     //Validate certificate chain, validity period
    CELLULAR_VALIDATE_CERT_EXPDATE_CN = 5,  //Validate certificate chain, validity period, common name
} e_cellular_cert_validate_level_t;

typedef enum
{
    CELLULAR_SSL_DEACTIVE= 0,   //Deactivate secure socket
    CELLULAR_SSL_ACTIVE = 1,    //Activate SSL/TLS on the socket
} e_cellular_ssl_active_t;
#endif

typedef struct cellular_socket_time_ctrl
{
    e_cellular_timeout_ovf_check_t timeout_overflow_flag;
    uint32_t start_time;            // Start time
    uint32_t this_time;             // Now Time
    uint32_t end_time;              // End time
    uint32_t remain_time;           // Remaining time
} st_cellular_socket_time_ctrl_t;

typedef struct cellular_module_time_ctrl
{
    e_cellular_timeout_ovf_check_t timeout_overflow_flag;
    uint32_t start_time;            // Start time
    uint32_t this_time;             // Now Time
    uint32_t end_time;              // End time
} st_cellular_module_time_ctrl_t;

typedef struct cellular_datetime
{
    uint8_t year;       // Year
    uint8_t month;      // Month
    uint8_t day;        // Day
    uint8_t hour;       // Hour
    uint8_t min;        // Minute
    uint8_t sec;        // Second
    int8_t  timezone;   // Time Zone
} st_cellular_datetime_t;

typedef struct cellular_sci_ctrl
{
    uint32_t                baud_rate;                              // Baudrate
    uint32_t                tx_buff_size;                           // Command send buffer size
    uint8_t                 atc_buff[CELLULAR_ATC_BUFF_SIZE];       // AT command buffer
    uint32_t                rx_buff_size;                           // Command receive buffer size
    uint8_t                 receive_buff[CELLULAR_ATC_RESPONSE_BUFF_SIZE]; // receive buffer
    uint16_t                tx_process_size;                        // Size per send
    uint16_t                rx_process_size;                        // Size per receive
    uint32_t                atc_timeout;                            // AT command response timeout
    volatile e_cellular_tx_end_flg_t tx_end_flg;                    // Transmission completion flag
    sci_cb_evt_t            sci_err_flg;                            // sci error flg
    st_cellular_module_time_ctrl_t  timeout_ctrl;                   // Timeout management structure
    st_cellular_datetime_t  time;                                   // Daytime management structure
    e_atc_list_t            at_command;                             // Running AT command
    e_cellular_atc_return_t atc_res;                                // AT command response
    e_cellular_atc_res_check_t  atc_flg;                            // Response status of AT command
    sci_hdl_t               sci_hdl;                                // sci_hdl
} st_cellular_sci_ctrl_t;

typedef struct cellular_socket_ctrl
{
    uint32_t                        receive_num;                // Size received from the module
    int16_t                         receive_unprocessed_size;   // Data size possessed by the module
    volatile e_cellular_receive_flg_t receive_flg;              // Receivable Flag
    uint8_t *                       p_recv;                     // Pointer to the receive buffer
    uint32_t                        total_recv_count;           // Total received data size
    uint16_t                        packet_data_size;           // Data size per packet
    uint16_t                        exchange_timeout;           // Exchange timeout
    uint16_t                        connect_timeout;            // Socket connection timeout
    uint8_t                         send_timeout;               // Packet transmission timeout
    uint32_t                        receive_count;              // Receive byte count
    e_cellular_socket_status_t      socket_status;              // Socket Status
    uint8_t                         ipversion;                  // IP Version
    uint8_t                         protocol;                   // Protocol Type
    uint32_t                        ipaddr;                     // Destination IP address
    uint32_t                        port;                       // Destination port number
    void *                          rx_semaphore;               // Semaphore handle (for receiving)
    st_cellular_socket_time_ctrl_t  cellular_tx_timeout_ctrl;   // Transmission timeout management structure
    st_cellular_socket_time_ctrl_t  cellular_rx_timeout_ctrl;   // Receive timeout management structure
} st_cellular_socket_ctrl_t;

typedef struct cellular_ctrl
{
    uint8_t                     ap_name[64 + 1];        // AP name
    uint8_t                     ap_user_name[32 + 1];   // AP user name
    uint8_t                     ap_pass[64 + 1];        // AP Password
    uint8_t                     sim_pin_code[8 + 1];    // SIM Pin Code
    uint8_t                     ap_connect_retry;   // AP connection retry limit
    uint8_t                     dns_address[4];     // IP address obtained by DNS query
    e_cellular_system_status_t  system_state;       // AP connection status
    e_cellular_module_status_t  module_status;      // Module Status
    void *                      at_semaphore;       // Semaphore handle (for at command)
    void *                      recv_taskhandle;    // Task handle
    void *                      eventgroup;         // Event Group Handles
    st_cellular_sci_ctrl_t      sci_ctrl;           // Module communication management structure
    st_cellular_socket_ctrl_t * p_socket_ctrl;      // Pointer to the socket management structure
    uint8_t                     creatable_socket;   // Number of sockets to create
} st_cellular_ctrl_t;

typedef struct cellular_cfg
{
    uint8_t     ap_name[64 + 1];        // AP name
    uint8_t     ap_user_name[32 + 1];   // AP user name
    uint8_t     ap_pass[64 + 1];        // AP Password
    uint8_t     sim_pin_code[8 + 1];    // SIM Pin Code
    uint32_t    baud_rate;              // Baudrate
    uint8_t     ap_gatt_retry_count;    // AP connection retry limit
    uint32_t    sci_timeout;            // Communication timeout with module
    uint16_t    tx_process_size;        // Size per send(1~1500)
    uint16_t    rx_process_size;        // Size per receive(1~1500)
    uint16_t    packet_data_size;       // Data size per packet(0~1500)
    uint16_t    exchange_timeout;       // Exchange timeout(0~65535)
    uint16_t    connect_timeout;        // Socket connection timeout(0,10~1200)
    uint8_t     send_timeout;           // Packet transmission timeout(0~255). Set fixed value(10).
    uint8_t     creatable_socket;       // Number of sockets to create(1~6)
} st_cellular_cfg_t;

/*****************************************************************************
 * Public Functions
 *****************************************************************************/
/*************************************************************************************************
 * Function Name  @fn            R_CELLULAR_Open
 * Description    @details       Establish a communication state with the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     p_cfg -
 *                                  Pointer to the configuration structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully established a communication state with the module
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_ALREADY_OPEN -
 *                                  Already initialized.
 *                @retval        CELLULAR_ERR_SERIAL_OPEN -
 *                                  Serial initialization failed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ************************************************************************************************/
e_cellular_err_t R_CELLULAR_Open (st_cellular_ctrl_t * const p_ctrl, const st_cellular_cfg_t * const p_cfg);

/***************************************************************************************
 * Function Name  @fn            R_CELLULAR_Close
 * Description    @details       Terminate communication with the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully disconnected communication with module.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 **************************************************************************************/
e_cellular_err_t R_CELLULAR_Close (st_cellular_ctrl_t * const p_ctrl);

/**************************************************************************
 * Function Name  @fn            R_CELLULAR_APConnect
 * Description    @details       Connect to an access point.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully connected to access point.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 *************************************************************************/
e_cellular_err_t R_CELLULAR_APConnect (st_cellular_ctrl_t * const p_ctrl);

/******************************************************************************
 * Function Name  @fn            R_CELLULAR_IsConnected
 * Description    @details       Get the connection status to the access point.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained connection status.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_CONNECT -
 *                                  Not connected to access point.
 *****************************************************************************/
e_cellular_err_t R_CELLULAR_IsConnected (st_cellular_ctrl_t * const p_ctrl);

/*******************************************************************************
 * Function Name  @fn            R_CELLULAR_Disconnect
 * Description    @details       Disconnect from the access point.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully disconnected from access point.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ******************************************************************************/
e_cellular_err_t R_CELLULAR_Disconnect (st_cellular_ctrl_t * const p_ctrl);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_CreateSocket
 * Description    @details       Perform socket connection.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     protocol_type -
 *                                  Protocol Type.
 *                @param[in]     ip_version -
 *                                  IP Version.
 * Return Value   @retval        1 to 6
 *                                  Successfully created a socket.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_SOCKET_NUM -
 *                                  No available socket.
 *                @retval        CELLULAR_ERR_NOT_CONNECT -
 *                                  Not connected to access point.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
int32_t R_CELLULAR_CreateSocket (st_cellular_ctrl_t * const p_ctrl,
                                    const uint8_t protocol_type,
                                    const uint8_t ip_version);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_CloseSocket
 * Description    @details       Close the socket.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     socket_no -
 *                                  Number to close socket.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully closed the socket.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_SOCKET_NUM -
 *                                  No available socket.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_CloseSocket (st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_ConnectSocket
 * Description    @details       Perform socket connection.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     socket_no -
 *                                  Number to close socket.
 *                @param[in]     ip_address -
 *                                  Destination IP address.
 *                @param[in]     port -
 *                                  Destination port number.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully socket connection.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_SOCKET_NUM -
 *                                  No available socket.
 *                @retval        CELLULAR_ERR_NOT_CONNECT -
 *                                  Not connected to access point.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_ConnectSocket (st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no,
                                                const uint32_t ip_address, const uint16_t port);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_ConnectSocketToHost
 * Description    @details       Perform socket connection.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     socket_no -
 *                                  Number to connect socket.
 *                @param[in]     p_hostname -
 *                                  Destination Host Name.
 *                @param[in]     port -
 *                                  Destination port number.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully socket connection.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_SOCKET_NUM -
 *                                  No available socket.
 *                @retval        CELLULAR_ERR_NOT_CONNECT -
 *                                  Not connected to access point.
 *                @retval        CELLULAR_ERR_ALREADY_SOCKET_CONNECT -
 *                                  Already socket connect.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_ConnectSocketToHost (st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no,
                                                const uint8_t * p_hostname, const uint16_t port);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_ShutdownSocket
 * Description    @details       Disconnect the socket connection.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     socket_no -
 *                                  Number to close socket.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully disconnected the socket connection.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_SOCKET_NUM -
 *                                  No available socket.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_ShutdownSocket (st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no);

/*************************************************************************************************
 * Function Name  @fn            R_CELLULAR_SendSocket
 * Description    @details       Sending data to a socket.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     socket_no -
 *                                  Socket number for sending data.
 *                @param[in]     p_data -
 *                                  Pointer to send data.
 *                @param[in]     length -
 *                                  Send data size.
 *                @param[in]     timeout_ms -
 *                                  timeout millisecond.
 * Return Value   @retval        More than 1 -
 *                                  Successfully sent data.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_SOCKET_NUM -
 *                                  No available socket.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ************************************************************************************************/
int32_t R_CELLULAR_SendSocket (st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no,
                                    const uint8_t * const p_data, const int32_t length,
                                    const uint32_t timeout_ms);

/*************************************************************************************************
 * Function Name  @fn            R_CELLULAR_ReceiveSocket
 * Description    @details       Reading data from a socket.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     socket_no -
 *                                  Socket number for reading data.
 *                @param[in]     p_data -
 *                                  Pointer to store the read data.
 *                @param[in]     length -
 *                                  Read data size.
 *                @param[in]     timeout_ms -
 *                                  timeout millisecond.
 * Return Value   @retval        More than 1 -
 *                                  Successfully loaded data.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_SOCKET_NUM -
 *                                  No available socket.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ************************************************************************************************/
int32_t R_CELLULAR_ReceiveSocket (st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no,
                                        uint8_t * const p_data, const int32_t length,
                                        const uint32_t timeout_ms);

/*************************************************************************************************
 * Function Name  @fn            R_CELLULAR_SetTime
 * Description    @details       Set time to module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     p_time -
 *                                  Pointer to set time.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully set the time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ************************************************************************************************/
e_cellular_err_t R_CELLULAR_SetTime (st_cellular_ctrl_t * const p_ctrl, const st_cellular_datetime_t * const p_time);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_GetTime
 * Description    @details       Get the time in the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[out]    p_time -
 *                                  Pointer to store the retrieved time.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_GetTime (st_cellular_ctrl_t * const p_ctrl, st_cellular_datetime_t * const p_time);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_DnsQuery
 * Description    @details       Execute DNS query.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     p_domain_name -
 *                                  Pointer to domain name.
 *                @param[out]    p_ip_address -
 *                                  Pointer to store IP address.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully DNS query.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_DnsQuery (st_cellular_ctrl_t * const p_ctrl, const uint8_t * const p_domain_name,
                                            uint32_t * const p_ip_address);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_SetEDRX
 * Description    @details       Set eDRX.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     mode -
 *                                  Value of the mode to set.
 *                @param[in]     edrx -
 *                                  Value of eDRX setting.
 *                @param[in]     ptw -
 *                                  Value of PTW setting.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully set up eDRX.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_SetEDRX (st_cellular_ctrl_t * const p_ctrl, const e_cellular_edrx_mode_t mode,
                                        const e_cellular_edrx_cycle_t edrx, const e_cellular_ptw_cycle_t ptw);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_SetPSM
 * Description    @details       Execute DNS query.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     mode -
 *                                  Value of the mode to set.
 *                @param[in]     tau -
 *                                  Value of TAU setting.
 *                @param[in]     tau_multiplier -
 *                                  Set the TAU multiplier.
 *                @param[in]     active -
 *                                  Value of active time setting.
 *                @param[in]     active_multiplier -
 *                                  Set the active time multiplier.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully set up PSM.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_SetPSM (st_cellular_ctrl_t * const p_ctrl, const e_cellular_psm_mode_t mode,
                    const e_cellular_tau_cycle_t tau, const e_cellular_cycle_multiplier_t tau_multiplier,
                    const e_cellular_active_cycle_t active, const e_cellular_cycle_multiplier_t active_multiplier);

#if (CELLULAR_IMPLEMENT_TYPE == 'B')
/************************************************************************
 * Function Name  @fn            R_CELLULAR_WriteCertificate
 * Description    @details       Write certificate or private key to non-volatile memory of the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     data_type -
 *                                  Information to write.
 *                @param[in]     index -
 *                                  Destination index.
 *                @param[in]     p_data -
 *                                  Pointer to the data to write.
 *                @param[in]     size -
 *                                  Size of the data to be written.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully write certificate.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_WriteCertificate (st_cellular_ctrl_t * const p_ctrl, const e_cellular_nvm_type_t data_type,
                                                    const uint8_t index, const uint8_t * p_data,
                                                    const uint32_t size);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_EraseCertificate
 * Description    @details       Erase certificate or private key to non-volatile memory of the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     data_type -
 *                                  Information to write.
 *                @param[in]     index -
 *                                  Destination index.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully erase certificate.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_EraseCertificate (st_cellular_ctrl_t * const p_ctrl, const e_cellular_nvm_type_t data_type,
                                                    const uint8_t index);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_ConfigSSLProfile
 * Description    @details       Configurate security profile.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     security_profile_id -
 *                                  Security Profile identifier
 *                @param[in]     cert_valid_level -
 *                                  Server certificate validation
 *                @param[in]     ca_certificate_id -
 *                                  Certificate ID of a trusted certification authority
 *                                    by written to non-volatile memory.
 *                @param[in]     client_certificate_id -
 *                                  Client certificate ID by written to non-volatile memory.
 *                @param[in]     client_privatekey_id -
 *                                  Privatekey ID by written to non-volatile memory.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully configurate.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_ConfigSSLProfile (st_cellular_ctrl_t * const p_ctrl,
                                    const uint8_t security_profile_id,
                                    const e_cellular_cert_validate_level_t cert_valid_level,
                                    const uint8_t ca_certificate_id,
                                    const uint8_t client_certificate_id,
                                    const uint8_t client_privatekey_id);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_ConfigSSLSocket
 * Description    @details       Execute DNS query.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     socket_no -
 *                                  Socket number to configurate.
 *                @param[in]     active -
 *                                  Pointer to store IP address.
 *                @param[in]     security_profile_id -
 *                                  Link to Security Profile identifier.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully configurate.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_SOCKET_NOT_READY -
 *                                  Socket is not ready for use.
 *                @retval        CELLULAR_ERR_ALREADY_SOCKET_CONNECT -
 *                                  Already socket connect.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_ConfigSSLSocket (st_cellular_ctrl_t * const p_ctrl,
                                    const uint8_t socket_no,
                                    const e_cellular_ssl_active_t active,
                                    const uint8_t security_profile_id);
#endif /* (CELLULAR_IMPLEMENT_TYPE == 'B') */

#endif /* CELLULAR_IF_H */
