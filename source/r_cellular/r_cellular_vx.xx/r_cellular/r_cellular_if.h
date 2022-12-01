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
 * Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_cellular_if.h
 * Description  : Configures the driver.
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

#include "r_cellular_config.h"
#include "r_cellular_private.h"
#include "ryz014_private.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#if (defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__) && defined(__LIT)) \
    || (defined(__CCRL__) || defined(__ICCRL78__) || defined(__RL))
/* Converts IP address to 32 bits in little endian. */
#define CELLULAR_IP_ADDER_CONVERT(IPv4_0, IPv4_1, IPv4_2, IPv4_3)\
                                ( ( ( ( uint32_t ) ( IPv4_3 ) ) << 24UL ) |    \
                                ( ( ( uint32_t ) ( IPv4_2 ) ) << 16UL ) |    \
                                ( ( ( uint32_t ) ( IPv4_1 ) ) << 8UL ) |    \
                                ( ( uint32_t ) ( IPv4_0 ) ) )
#else
/* Convert IP address to 32 bits in big endian. */
#define CELLULAR_IP_ADDER_CONVERT(IPv4_0, IPv4_1, IPv4_2, IPv4_3)\
                                ( ( ( ( uint32_t ) ( IPv4_0 ) ) << 24UL ) |    \
                                ( ( ( uint32_t ) ( IPv4_1 ) ) << 16UL ) |    \
                                ( ( ( uint32_t ) ( IPv4_2 ) ) << 8UL ) |    \
                                ( ( uint32_t ) ( IPv4_3 ) ) )
#endif

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/
typedef enum
{
    CELLULAR_SUCCESS            = 0,            // Successful
    CELLULAR_ERR_PARAMETER      = -1,           // Argument is abnormal
    CELLULAR_ERR_ALREADY_OPEN   = -2,           // Already initialized
    CELLULAR_ERR_NOT_OPEN       = -3,           // Not initialized
    CELLULAR_ERR_SERIAL_OPEN    = -4,           // Serial initialization failed
    CELLULAR_ERR_MODULE_COM     = -5,           // Communication with module failed
    CELLULAR_ERR_MODULE_TIMEOUT = -6,           // Communication with module timed out
    CELLULAR_ERR_NOT_CONNECT    = -7,           // Not connected to access point
    CELLULAR_ERR_ALREADY_CONNECT = -8,          // Already connect to access point
    CELLULAR_ERR_ALREADY_SOCKET_CONNECT = -9,   // Already socket connect
    CELLULAR_ERR_SOCKET_NOT_READY   = -10,      // Socket is not ready for use
    CELLULAR_ERR_SOCKET_CREATE_LIMIT = -11,     // Socket creation limit exceeded
    CELLULAR_ERR_BYTEQ_OPEN         = -12,      // Failed to initialize the byte queue
    CELLULAR_ERR_SEMAPHORE_INIT     = -13,      // Failed to initialize the semaphore
    CELLULAR_ERR_EVENT_GROUP_INIT   = -14,      // Failed to initialize the event group
    CELLULAR_ERR_CREATE_TASK        = -15,      // Failed to create task
    CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING = -16, // Other AT commands are running
    CELLULAR_ERR_OTHER_API_RUNNING  = -17,      // Other API are running
    CELLULAR_ERR_MEMORY_ALLOCATION  = -18,      // Failed to allocate memory
    CELLULAR_ERR_IRQ_OPEN           = -19,      // IRQ initialization failed
    CELLULAR_ERR_SIM_NOT_SUPPORT_IPV6   = -20,  // The SIM card does not support IPv6
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
    CELLULAR_MODULE_OPERATING_LEVEL5,        // Reserved
    CELLULAR_MODULE_OPERATING_RESET,         // Module Reset
} e_cellular_module_status_t;

typedef enum
{
    CELLULAR_DISABLE_NETWORK_RESULT_CODE = 0,   // Disable automatic notification of network connection status
    CELLULAR_ENABLE_NETWORK_RESULT_CODE_LEVEL1, // Enable network registration unsolicited result code
    CELLULAR_ENABLE_NETWORK_RESULT_CODE_LEVEL2, // Enable network registration and location information unsolicited result code
    CELLULAR_ENABLE_NETWORK_RESULT_CODE_LEVEL3, // Enable network registration, location information and EMM cause value information unsolicited result code
    CELLULAR_ENABLE_NETWORK_RESULT_CODE_LEVEL4, // For a UE that wants to apply PSM, enable network registration and location information unsolicited result code
    CELLULAR_ENABLE_NETWORK_RESULT_CODE_LEVEL5, // For a UE that wants to apply PSM, enable network registration, location information and EMM cause value information unsolicited result code
} e_cellular_network_result_t;

typedef enum
{
    CELLULAR_REG_STATS_VALUE0 = 0,  // Not registered
    CELLULAR_REG_STATS_VALUE1,      // Registered, home network
    CELLULAR_REG_STATS_VALUE2,      // Not registered, but MT is currently trying to attach or searching
    CELLULAR_REG_STATS_VALUE3,      // Registration denied
    CELLULAR_REG_STATS_VALUE4,      // Unknown
    CELLULAR_REG_STATS_VALUE5,      // Registered, roaming
    CELLULAR_REG_STATS_VALUE6,      // Registered for "SMS only", home network
    CELLULAR_REG_STATS_VALUE7,      // Registered for "SMS only", roaming
    CELLULAR_REG_STATS_VALUE8,      // Attached for emergency bearer services only
    CELLULAR_REG_STATS_VALUE9,      // Registered for "CSFB not preferred", home network
    CELLULAR_REG_STATS_VALUE10,     // Registered for "CSFB not preferred", roaming
    CELLULAR_REG_STATS_VALUE80,     // Registered, temporary connection lost
} e_cellular_reg_stat_t;

typedef enum
{
    CELLULAR_ACCESS_TEC0 = 0,       // GSM
    CELLULAR_ACCESS_TEC1,           // GSM Compact
    CELLULAR_ACCESS_TEC2,           // UTRAN
    CELLULAR_ACCESS_TEC3,           // GSM w/EGPRS
    CELLULAR_ACCESS_TEC4,           // UTRAN w/HSDPA
    CELLULAR_ACCESS_TEC5,           // UTRAN w/HSUPA
    CELLULAR_ACCESS_TEC6,           // UTRAN w/HSDPA and HSUPA
    CELLULAR_ACCESS_TEC7,           // E-UTRAN
} e_cellular_access_tec_t;

typedef enum
{
    CELLULAR_INFO_TYPE0 = 0,    // Report information for the serving cell only
    CELLULAR_INFO_TYPE1 = 1,    // Report information for the intra-frequency cells only
    CELLULAR_INFO_TYPE2 = 2,    // Report information for the intra-frequency cells only
    CELLULAR_INFO_TYPE7 = 7,    // Report information for all cells
    CELLULAR_INFO_TYPE9 = 9,    // Report information for the serving cell only with RSRP/CINR on main antenna.
} e_cellular_info_type_t;

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
    CELLULAR_ATC_RESPONSE_CONFIRMED = 0,    // AT command response confirmed
    CELLULAR_ATC_RESPONSE_UNCONFIRMED,      // AT command response Unconfirmed
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
    CELLULAR_PTW_CYCLE_1_SEC = 0,   // PTW(1.28sec)
    CELLULAR_PTW_CYCLE_2_SEC,       // PTW(2.56sec)
    CELLULAR_PTW_CYCLE_4_SEC,       // PTW(3.84sec)
    CELLULAR_PTW_CYCLE_5_SEC,       // PTW(5.12sec)
    CELLULAR_PTW_CYCLE_6_SEC,       // PTW(6.40sec)
    CELLULAR_PTW_CYCLE_7_SEC,       // PTW(7.68sec)
    CELLULAR_PTW_CYCLE_9_SEC,       // PTW(8.96sec)
    CELLULAR_PTW_CYCLE_10_SEC,      // PTW(10.24sec)
    CELLULAR_PTW_CYCLE_11_SEC,      // PTW(11.52sec)
    CELLULAR_PTW_CYCLE_13_SEC,      // PTW(12.80sec)
    CELLULAR_PTW_CYCLE_14_SEC,      // PTW(14.08sec)
    CELLULAR_PTW_CYCLE_15_SEC,      // PTW(15.36sec)
    CELLULAR_PTW_CYCLE_16_SEC,      // PTW(16.64sec)
    CELLULAR_PTW_CYCLE_18_SEC,      // PTW(17.92sec)
    CELLULAR_PTW_CYCLE_19_SEC,      // PTW(19.20sec)
    CELLULAR_PTW_CYCLE_20_SEC,      // PTW(20.48sec)
} e_cellular_ptw_cycle_t;

typedef enum
{
    CELLULAR_PSM_MODE_INVALID   = 0,    // Disable the PSM function
    CELLULAR_PSM_MODE_ACTIVE    = 1,    // Enable the PSM function
    CELLULAR_PSM_MODE_INIT      = 2,    // Initialize and disable the PSM function
} e_cellular_psm_mode_t;

typedef enum
{
    CELLULAR_TAU_CYCLE_10_MIN = 0,  // TAU cycle(10min)
    CELLULAR_TAU_CYCLE_1_HOUR,      // TAU cycle(1hour)
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
    CELLULAR_ACTIVE_CYCLE_1_MIN,        // Active time(1min)
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

typedef enum
{
    CELLULAR_AUTH_TYPE_NONE = 0,    // Authentication protocol not used
    CELLULAR_AUTH_TYPE_PAP,         // Authentication protocol uses PAP
    CELLULAR_AUTH_TYPE_CHAP,        // Authentication protocol uses CHAP
    CELLULAR_AUTH_TYPE_MAX,         // End of enumeration
} e_cellular_auth_type_t;

typedef enum
{
    CELLULAR_DISABLE_AUTO_CONNECT = 0,  // Disable automatic connection to AP
    CELLULAR_ENABLE_AUTO_CONNECT,       // Enable automatic connection to AP (next start-up or restart)
} e_cellular_auto_connect_t;

typedef enum
{
    CELLULAR_PSM_DEACTIVE = 0,  //Deactivate PSM
    CELLULAR_PSM_ACTIVE = 1,    //Activate PSM
} e_cellular_psm_active_t;

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

typedef struct cellular_time_ctrl
{
    e_cellular_timeout_ovf_check_t timeout_overflow_flag;
    uint32_t start_time;    // Start time
    uint32_t this_time;     // Now Time
    uint32_t end_time;      // End time
} st_cellular_time_ctrl_t;

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

typedef struct cellular_edrx_config
{
    e_cellular_edrx_mode_t      edrx_mode;  // eDRX mode
    e_cellular_edrx_cycle_t     edrx_cycle; // eDRX cycle
    e_cellular_ptw_cycle_t      ptw_cycle;  // PTW cycle
} st_cellular_edrx_config_t;

typedef struct cellular_psm_config
{
    e_cellular_psm_mode_t           psm_mode;           // PSM mode
    e_cellular_tau_cycle_t          tau_cycle;          // TAU cycle
    e_cellular_cycle_multiplier_t   tau_multiplier;     // TAU cycle multiplier
    e_cellular_active_cycle_t       active_cycle;       // Active time
    e_cellular_cycle_multiplier_t   active_multiplier;  // Active time multiplier
} st_cellular_psm_config_t;

typedef struct cellular_iccid
{
    uint8_t iccid[CELLULAR_MAX_ICCID_LENGTH + 1];   // Card identification number
} st_cellular_iccid_t;

typedef struct cellular_imei
{
    uint8_t imei[CELLULAR_MAX_IMEI_LENGTH + 1]; // International Mobile station Equipment Identity number
} st_cellular_imei_t;

typedef struct cellular_imsi
{
    uint8_t imsi[CELLULAR_MAX_IMSI_LENGTH + 1]; // International Mobile Subscriber Identity
} st_cellular_imsi_t;

typedef struct cellular_phonenum
{
    uint8_t phonenum[CELLULAR_MAX_PHONENUM_LENGTH + 1]; // Phone Number
} st_cellular_phonenum_t;

typedef struct cellular_rssi
{
    uint8_t rssi[CELLULAR_MAX_RSSI_LENGTH + 1]; // Received signal strength indication
    uint8_t ber[CELLULAR_MAX_BER_LENGTH + 1];   // Channel bit error rate (in percent)
} st_cellular_rssi_t;

typedef struct cellular_svn
{
    uint8_t svn[CELLULAR_MAX_SVN_LENGTH + 1];           // Software Version Number
    uint8_t revision[CELLULAR_MAX_REVISION_LENGTH + 1]; // Software Revision Number
} st_cellular_svn_t;

typedef struct cellular_notice
{
    e_cellular_network_result_t level;          // Network status notification level
    e_cellular_reg_stat_t       stat;           // EPS registration status
    uint8_t                     ta_code[5];     // Tracking area code
    uint8_t                     cell_id[9];     // Cell ID
    e_cellular_access_tec_t     access_tec;     // Access Technology
    uint8_t                     active_time[9]; // Active-Time
    uint8_t                     tau[9];         // Periodic-TAU
} st_cellular_notice_t;

typedef struct cellular_ipaddr
{
    uint8_t     ipv4[CELLULAR_IPV4_ADDR_LENGTH + 1];    // IPv4 address
    uint8_t     ipv6[CELLULAR_IPV6_ADDR_LENGTH + 1];    // IPv6 address
} st_cellular_ipaddr_t;

typedef struct cellular_ping_cfg
{
    uint8_t     count;      // Number of Ping Echo Request to send (default: 4, 1-64)
    uint16_t    len;        // Length of Ping Echo Request message (default: 32, 32-1400).
    uint16_t    interval;   // Wait interval seconds between sending each Ping Echo Request (in seconds)(default: 1, 1-600)
    uint8_t     timeout;    // Time to wait for an Echo Reply (in seconds)(default: 10, 1-60).
} st_cellular_ping_cfg_t;

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
    st_cellular_time_ctrl_t timeout_ctrl;                           // Timeout management structure
    e_atc_list_t            at_command;                             // Running AT command
    e_cellular_atc_return_t atc_res;                                // AT command response
    e_cellular_atc_res_check_t  atc_flg;                            // Response status of AT command
    sci_hdl_t               sci_hdl;                                // SCI control block handle
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
    st_cellular_ipaddr_t            ip_addr;                    // Destination address
    uint32_t                        port;                       // Destination port number
    void *                          rx_semaphore;               // Semaphore handle (for receiving)
    st_cellular_time_ctrl_t         cellular_tx_timeout_ctrl;   // Transmission timeout management structure
    st_cellular_time_ctrl_t         cellular_rx_timeout_ctrl;   // Receive timeout management structure
} st_cellular_socket_ctrl_t;

typedef struct cellular_ap_cfg
{
    uint8_t                     ap_name[CELLULAR_MAX_AP_NAME_LENGTH + 1];       // AP name
    uint8_t                     ap_user_name[CELLULAR_MAX_AP_ID_LENGTH + 1];    // AP user name
    uint8_t                     ap_pass[CELLULAR_MAX_AP_PASS_LENGTH + 1];       // AP Password
    e_cellular_auth_type_t      auth_type;  // Authentication protocol type
} st_cellular_ap_cfg_t;

typedef struct cellular_ring_ctrl
{
    e_cellular_psm_active_t         psm;                // PSM status
    void *                          rts_semaphore;      // Semaphore handle
    void *                          ring_event;         // Event Group Handles
    void *                          ring_taskhandle;    // Task handle
    irq_handle_t                    ring_irqhandle;     // IRQ control block handle
    st_cellular_time_ctrl_t         timeout;            // Timeout management structure
} st_cellular_ring_ctrl_t;

typedef struct cellular_ctrl
{
    uint8_t                     running_api_count;  // Number of running APIs
    uint8_t                     ap_connect_retry;   // AP connection retry limit
    e_cellular_system_status_t  system_state;       // AP connection status
    e_cellular_module_status_t  module_status;      // Module Status
    st_cellular_ipaddr_t        pdp_addr;           // PDP Address
    void *                      at_semaphore;       // Semaphore handle (for at command)
    void *                      recv_taskhandle;    // Task handle
    void *                      eventgroup;         // Event Group Handles
    void *                      recv_data;          // Pointer to the storage location of the retrieved information.
    st_cellular_sci_ctrl_t      sci_ctrl;           // Module communication management structure
    st_cellular_ring_ctrl_t     ring_ctrl;          // Ring line management structure
    st_cellular_socket_ctrl_t * p_socket_ctrl;      // Pointer to the socket management structure
    uint8_t                     creatable_socket;   // Number of sockets to create
} st_cellular_ctrl_t;

typedef struct cellular_cfg
{
    uint8_t     sim_pin_code[CELLULAR_MAX_SIM_PASS_LENGTH + 1]; // SIM Pin Code
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
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_SEMAPHORE_INIT -
 *                                  Semaphore initialization failed.
 *                @retval        CELLULAR_ERR_EVENT_GROUP_INIT -
 *                                  Event group initialization failed.
 *                @retval        CELLULAR_ERR_CREATE_TASK -
 *                                  Task initialization failed.
 *                @retval        CELLULAR_ERR_MEMORY_ALLOCATION -
 *                                  Failed to allocate memory.
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
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 **************************************************************************************/
e_cellular_err_t R_CELLULAR_Close (st_cellular_ctrl_t * const p_ctrl);

/**************************************************************************
 * Function Name  @fn            R_CELLULAR_APConnect
 * Description    @details       Connect to an access point.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     p_ap_cfg -
 *                                  Pointer to AP information structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully connected to access point.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_ALREADY_CONNECT
 *                                  Already connected to AP.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 *************************************************************************/
e_cellular_err_t R_CELLULAR_APConnect (st_cellular_ctrl_t * const p_ctrl, const st_cellular_ap_cfg_t * const p_ap_cfg);

/******************************************************************************
 * Function Name  @fn            R_CELLULAR_IsConnected
 * Description    @details       Get the connection status to the access point.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained connection status.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
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
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_NOT_CONNECT -
 *                                  Not connected to access point.
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
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_NOT_CONNECT -
 *                                  Not connected to access point.
 *                @retval        CELLULAR_ERR_SOCKET_CREATE_LIMIT
 *                                  Socket creation limit exceeded.
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
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_NOT_CONNECT -
 *                                  Not connected to access point.
 *                @retval        CELLULAR_ERR_SOCKET_NOT_READY
 *                                  Socket is not ready for use.
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
 *                @param[in]     p_ip_addr -
 *                                  Pointer to destination address.
 *                @param[in]     port -
 *                                  Destination port number.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully socket connection.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                  Communication with module timed out.
 *                @retval        CELLULAR_ERR_NOT_CONNECT -
 *                                  Not connected to access point.
 *                @retval        CELLULAR_ERR_ALREADY_SOCKET_CONNECT
 *                                  Already socket connect.
 *                @retval        CELLULAR_ERR_SOCKET_NOT_READY
 *                                  Socket is not ready for use.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_ConnectSocket (st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no,
                                                const uint8_t * const p_ip_addr, const uint16_t port);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_ConnectSocketToHost
 * Description    @details       Perform socket connection.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     socket_no -
 *                                  Number to connect socket.
 *                @param[in]     p_hostname -
 *                                  Pointer to destination host name.
 *                @param[in]     port -
 *                                  Destination port number.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully socket connection.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_NOT_CONNECT -
 *                                  Not connected to access point.
 *                @retval        CELLULAR_ERR_ALREADY_SOCKET_CONNECT -
 *                                  Already socket connect.
 *                @retval        CELLULAR_ERR_SOCKET_NOT_READY
 *                                  Socket not created.
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
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_NOT_CONNECT -
 *                                  Not connected to access point.
 *                @retval        CELLULAR_ERR_SOCKET_NOT_READY
 *                                  Socket not created.
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
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_NOT_CONNECT -
 *                                  Not connected to access point.
 *                @retval        CELLULAR_ERR_SOCKET_NOT_READY
 *                                  Socket not created.
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
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_NOT_CONNECT -
 *                                  Not connected to access point.
 *                @retval        CELLULAR_ERR_SOCKET_NOT_READY
 *                                  Socket not created.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ************************************************************************************************/
int32_t R_CELLULAR_ReceiveSocket (st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no,
                                        uint8_t * const p_data, const int32_t length,
                                        const uint32_t timeout_ms);

/*********************************************************************************************************
 * Function Name  @fn            R_CELLULAR_ListeningSocket
 * Description    @details       Specifies the socket listening for TCP connections on the specified port.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     socket_no -
 *                                  Socket number to listen for TCP connections.
 *                @param[in]     ip_version -
 *                                  IP Version.
 *                @param[in]     port -
 *                                  Port number to listen for TCP connections.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully set the time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_NOT_CONNECT -
 *                                  Not connected to access point.
 *                @retval        CELLULAR_ERR_SOCKET_NOT_READY
 *                                  Socket not created.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ********************************************************************************************************/
e_cellular_err_t R_CELLULAR_ListeningSocket (st_cellular_ctrl_t * const p_ctrl, const uint8_t socket_no,
                                                const uint8_t ip_version, const uint16_t port);

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
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
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
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_GetTime (st_cellular_ctrl_t * const p_ctrl, st_cellular_datetime_t * const p_time);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_GetIMEI
 * Description    @details       Get IMEI.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[out]    p_imei -
 *                                  Pointer to store the retrieved IMEI.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_GetIMEI (st_cellular_ctrl_t * const p_ctrl, st_cellular_imei_t * const p_imei);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_GetIMSI
 * Description    @details       Get IMSI.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] p_imsi -
 *                                  Pointer to store the retrieved IMSI.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_GetIMSI (st_cellular_ctrl_t * const p_ctrl, st_cellular_imsi_t * const p_imsi);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_GetSVN
 * Description    @details       Get SVN.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] p_svn -
 *                                  Pointer to store the retrieved SVN.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_GetSVN (st_cellular_ctrl_t * const p_ctrl, st_cellular_svn_t * const p_svn);

/*******************************************************************************
 * Function Name  @fn            R_CELLULAR_GetPhonenum
 * Description    @details       Get Phone Number.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] p_phonenum -
 *                                  Pointer to store the retrieved Phone number.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ******************************************************************************/
e_cellular_err_t R_CELLULAR_GetPhonenum (st_cellular_ctrl_t * const p_ctrl, st_cellular_phonenum_t * const p_phonenum);

/*******************************************************************************
 * Function Name  @fn            R_CELLULAR_GetICCID
 * Description    @details       Get ICCID.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] p_iccid -
 *                                  Pointer to store the retrieved ICCID.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ******************************************************************************/
e_cellular_err_t R_CELLULAR_GetICCID (st_cellular_ctrl_t * const p_ctrl, st_cellular_iccid_t * const p_iccid);

/*******************************************************************************
 * Function Name  @fn            R_CELLULAR_GetRSSI
 * Description    @details       Get RSSI.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] p_rssi -
 *                                  Pointer to store the retrieved RSSI.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ******************************************************************************/
e_cellular_err_t R_CELLULAR_GetRSSI (st_cellular_ctrl_t * const p_ctrl, st_cellular_rssi_t * const p_rssi);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_DnsQuery
 * Description    @details       Execute DNS query.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     p_domain_name -
 *                                  Pointer to domain name.
 *                @param[in]     ip_version -
 *                                  Protocol Type.
 *                @param[in/out] p_addr -
 *                                  Pointer to store IP address.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully DNS query.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_NOT_CONNECT -
 *                                  Not connected to access point.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_DnsQuery (st_cellular_ctrl_t * const p_ctrl, const uint8_t * const p_domain_name,
                                        const uint8_t ip_version, st_cellular_ipaddr_t * const p_addr);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_SetEDRX
 * Description    @details       Set eDRX.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     p_config -
 *                                  Pointer to the configuration structure.
 *                @param[in/out] p_result -
 *                                  Pointer to the result structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully set up eDRX.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_SetEDRX (st_cellular_ctrl_t * const p_ctrl,
                                        const st_cellular_edrx_config_t * const p_config,
                                        st_cellular_edrx_config_t * const p_result);

/************************************************************************
 * Function Name  @fn            R_CELLULAR_SetPSM
 * Description    @details       Execute DNS query.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     p_config -
 *                                  Pointer to the configuration structure.
 *                @param[in/out] p_result -
 *                                  Pointer to the result structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully set up PSM.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_SetPSM (st_cellular_ctrl_t * const p_ctrl,
                                        const st_cellular_psm_config_t * const p_config,
                                        st_cellular_psm_config_t * const p_result);

/*************************************************************************************************
 * Function Name  @fn            R_CELLULAR_Ping
 * Description    @details       Perform Ping.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     p_hostname -
 *                                  Pointer to destination host name.
 *                @param[in]     p_cfg -
 *                                  Pointer to the configuration structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_NOT_CONNECT -
 *                                  Not connected to access point.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 *                @retval        CELLULAR_ERR_MEMORY_ALLOCATION -
 *                                  Failed to create the memory pool.
 ************************************************************************************************/
e_cellular_err_t R_CELLULAR_Ping (st_cellular_ctrl_t * const p_ctrl, const uint8_t * const p_hostname,
                                    const st_cellular_ping_cfg_t * const p_cfg);

/*************************************************************************************************
 * Function Name  @fn            R_CELLULAR_GetAPConnectState
 * Description    @details       Get AP connection status.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     level -
 *                                  Result notification level.
 *                @param[in/out] p_result -
 *                                  Pointer to structure for storing results.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ************************************************************************************************/
e_cellular_err_t R_CELLULAR_GetAPConnectState (st_cellular_ctrl_t * const p_ctrl,
                                                const e_cellular_network_result_t level,
                                                st_cellular_notice_t * const p_result);

/*************************************************************************************************
 * Function Name  @fn            R_CELLULAR_GetCellInfo
 * Description    @details       Get cell information.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     type -
 *                                  Notification information type.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ************************************************************************************************/
e_cellular_err_t R_CELLULAR_GetCellInfo (st_cellular_ctrl_t * const p_ctrl, const e_cellular_info_type_t type);

/*************************************************************************************************
 * Function Name  @fn            R_CELLULAR_AutoConnectConfig
 * Description    @details       Enable/disable automatic connection to APs.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     type -
 *                                  Enable/disable.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ************************************************************************************************/
e_cellular_err_t R_CELLULAR_AutoConnectConfig (st_cellular_ctrl_t * const p_ctrl, e_cellular_auto_connect_t const type);

/*************************************************************************************************
 * Function Name  @fn            R_CELLULAR_SetBand
 * Description    @details       Set the band settings.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     p_band -
 *                                  Band to be set.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ************************************************************************************************/
e_cellular_err_t R_CELLULAR_SetBand (st_cellular_ctrl_t * const p_ctrl, const uint8_t * const p_band);

/*************************************************************************************************
 * Function Name  @fn            R_CELLULAR_GetPDPAddress
 * Description    @details       Get PDP address.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in/out] p_addr -
 *                                  Pointer to array storing PDP addresses.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_NOT_CONNECT -
 *                                  Not connected to access point.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ************************************************************************************************/
e_cellular_err_t R_CELLULAR_GetPDPAddress (st_cellular_ctrl_t * const p_ctrl, st_cellular_ipaddr_t * const p_addr);

/*************************************************************************************************
 * Function Name  @fn            R_CELLULAR_SoftwareReset
 * Description    @details       Reset module by AT command.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ************************************************************************************************/
e_cellular_err_t R_CELLULAR_SoftwareReset (st_cellular_ctrl_t * const p_ctrl);

/*************************************************************************************************
 * Function Name  @fn            R_CELLULAR_HardwareReset
 * Description    @details       Reset the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ************************************************************************************************/
e_cellular_err_t R_CELLULAR_HardwareReset (st_cellular_ctrl_t * const p_ctrl);

/*************************************************************************************************
 * Function Name  @fn            R_CELLULAR_FactoryReset
 * Description    @details       Restore factory conditions.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ************************************************************************************************/
e_cellular_err_t R_CELLULAR_FactoryReset (st_cellular_ctrl_t * const p_ctrl);

/*************************************************************************************************
 * Function Name  @fn            R_CELLULAR_RTS_Ctrl
 * Description    @details       Restore factory conditions.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     lowhigh -
 *                                  Port output state.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully obtained time.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_NOT_OPEN -
 *                                  R_CELLULAR_Open function is not executed.
 ************************************************************************************************/
e_cellular_err_t R_CELLULAR_RTS_Ctrl (st_cellular_ctrl_t * const p_ctrl, const uint8_t lowhigh);

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
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
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
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
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
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
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
 *                @retval        CELLULAR_ERR_MODULE_TIMEOUT
 *                                      Communication with module timed out.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ***********************************************************************/
e_cellular_err_t R_CELLULAR_ConfigSSLSocket (st_cellular_ctrl_t * const p_ctrl,
                                    const uint8_t socket_no,
                                    const e_cellular_ssl_active_t active,
                                    const uint8_t security_profile_id);
#endif /* (CELLULAR_IMPLEMENT_TYPE == 'B') */

#endif /* CELLULAR_IF_H */
