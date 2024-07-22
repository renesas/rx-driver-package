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
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_wifi_da16xxx_private.h
 * Description  : Private functions definition for DA16XXX.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

#include "r_wifi_da16xxx_if.h"
#include "r_wifi_da16xxx_os_wrap.h"
#include "r_sci_rx_if.h"
#include "r_byteq_if.h"
#include "r_sci_rx_pinset.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef R_WIFI_DA16XXX_PRIVATE_H
#define R_WIFI_DA16XXX_PRIVATE_H

#if !defined(WIFI_CFG_SCI_CHANNEL)
#error "Error! Need to define WIFI_CFG_SCI_CHANNEL in r_wifi_da16xxx_config.h"
#endif

#if SCI_CFG_TEI_INCLUDED != 1
#error "Error! Need to set SCI_CFG_TEI_INCLUDED is '1' in r_sci_rx_config.h"
#endif

/* Configuration */
#define TEMP_BUF_MAX                   (256)                                   // RX temp buffer
#define ATCMD_RESP_TIMEOUT             (1000)   // Timeout threshold for AT command response (msec)
#define DA16XXX_AT_SOCK_TX_MAX         (4096)   // DA16XXX socket transmit max length
#define DA16XXX_AT_CMD_BUF_MAX         (WIFI_CFG_AT_CMD_TX_BUFFER_SIZE)
#define DA16XXX_AT_RESP_BUF_MAX        (WIFI_CFG_AT_CMD_RX_BUFFER_SIZE)

/* Minimum string size for getting local time string */
#define DA16XXX_LOCAL_TIME_STR_SIZE             (25)
#define DA16XXX_HOURS_IN_SECONDS                (3600)

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
/* Reset port pin macros.  */
#define WIFI_RESET_DDR(x, y)            (WIFI_RESET_DDR_PREPROC(x, y))
#define WIFI_RESET_DDR_PREPROC(x, y)    ((PORT ## x .PDR.BIT.B ## y))
#define WIFI_RESET_DR(x, y)             (WIFI_RESET_DR_PREPROC(x, y))
#define WIFI_RESET_DR_PREPROC(x, y)     ((PORT ## x .PODR.BIT.B ## y))

/* RTS port pin macros.  */
#define WIFI_RTS_PDR(x, y)              (WIFI_RTS_PDR_PREPROC(x, y))
#define WIFI_RTS_PDR_PREPROC(x, y)      ((PORT ## x .PDR.BIT.B ## y))
#define WIFI_RTS_PODR(x, y)             (WIFI_RTS_PODR_PREPROC(x, y))
#define WIFI_RTS_PODR_PREPROC(x, y)     ((PORT ## x .PODR.BIT.B ## y))

/* CTS port pin macros. */
#define WIFI_CTS_PIDR(x, y)             (WIFI_CTS_PIDR_PREPROC(x, y))
#define WIFI_CTS_PIDR_PREPROC(x, y)     ((PORT ## x .PIDR.BIT.B ## y))

/* PMR port macros. */
#define WIFI_SET_PMR(x, y)              (WIFI_SET_PMR_PREPROC(x, y))
#define WIFI_SET_PMR_PREPROC(x, y)      ((PORT ## x .PMR.BIT.B ## y))

/* PFS port macros. */
#define WIFI_SET_PFS(x, y)              (WIFI_SET_PFS_PREPROC(x, y))
#define WIFI_SET_PFS_PREPROC(x, y)      ((MPC.P ## x ## y ##PFS.BYTE))
#endif

/* Debug mode */
#if WIFI_CFG_USE_FREERTOS_LOGGING == 0

#if WIFI_CFG_DEBUG_LOG == 4
/* All log level messages will logged */
#define WIFI_LOG_ERROR(message) printf_log_error message
#define WIFI_LOG_WARN(message)  printf_log_warn message
#define WIFI_LOG_INFO(message)  printf_log_info message
#define WIFI_LOG_DEBUG(message) printf_log_debug message
#elif WIFI_CFG_DEBUG_LOG == 3
/* Only INFO, WARNING and ERROR messages will be logged */
#define WIFI_LOG_ERROR(message) printf_log_error message
#define WIFI_LOG_WARN(message)  printf_log_warn message
#define WIFI_LOG_INFO(message)  printf_log_info message
#define WIFI_LOG_DEBUG(message)
#elif WIFI_CFG_DEBUG_LOG == 2
/* Only WARNING and ERROR messages will be logged */
#define WIFI_LOG_ERROR(message) printf_log_error message
#define WIFI_LOG_WARN(message)  printf_log_warn message
#define WIFI_LOG_INFO(message)
#define WIFI_LOG_DEBUG(message)
#elif WIFI_CFG_DEBUG_LOG == 1
/* Only ERROR messages will be logged */
#define WIFI_LOG_ERROR(message) printf_log_error message
#define WIFI_LOG_WARN(message)
#define WIFI_LOG_INFO(message)
#define WIFI_LOG_DEBUG(message)
#elif WIFI_CFG_DEBUG_LOG == 0
#define WIFI_LOG_ERROR(message)
#define WIFI_LOG_WARN(message)
#define WIFI_LOG_INFO(message)
#define WIFI_LOG_DEBUG(message)
#endif /* WIFI_CFG_DEBUG_LOG */

#else

#if BSP_CFG_RTOS_USED != 1
#error "Error - BSP_CFG_RTOS_USED is not 1(FreeRTOS)!"
#endif /* BSP_CFG_RTOS_USED */

#define WIFI_LOG_ERROR(message) LogError(message)
#define WIFI_LOG_WARN(message)  LogWarn(message)
#define WIFI_LOG_INFO(message)  LogInfo(message)
#define WIFI_LOG_DEBUG(message) LogDebug(message)
#endif /* WIFI_CFG_USE_FREERTOS_LOGGING */

/* Convert a macro value to a string */
#define WIFI_STRING_MACRO(str)          #str
/* Call WIFI_STRING_MACRO */
#define WIFI_STRING_CONVERT(arg)        (WIFI_STRING_MACRO(arg))

#define DA16XXX_MAX_CERT_SIZE               (2048) // Maximum length of certificate supported by DA16XXX.
#define DA16XXX_MAX_ALPN                    (3)    // Maximum number of ALPNs supported by DA16XXX.
#define DA16XXX_MAX_SNI_LEN                 (64)   // Maximum length of SNI supported by DA16XXX.
#define DA16XXX_CERT_START                  "\x1B"
#define DA16XXX_CERT_END                    "\x03"

#define DA16XXX_MQTT_TLS_CIPHER_SUITE_MAX   (17)   // Maximum number of TLS cipher suites supported by DA16XXX.
#define DA16XXX_MQTT_MAX_TOPIC_LEN          (64)   // Maximum total length for topics supported by DA16XXX.
#define DA16XXX_MQTT_MAX_PUBMSG_LEN         (2048) // Maximum total length for message supported by DA16XXX.
#define DA16XXX_MQTT_MAX_PUBTOPICMSG_LEN    (2063) // Maximum total length for message + topic supported by DA16XXX.
#define DA16XXX_MQTT_SUBTOPIC_MAX_CNT       (4)    // Maximum number of subscription topics allowed.

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
/* Common Timer */
typedef struct
{
    OS_TICK   threshold;        /* Timeout threshold */
    OS_TICK   tick_sta;         /* Tick of Timer start  */
} st_wifi_timer;

/* MQTT module status */
typedef enum
{
    DA16XXX_MQTT_CLOSE = 0,           /* Close MQTT Client service */
    DA16XXX_MQTT_OPEN,                /* Open MQTT Client service  */
    DA16XXX_MQTT_CONNECTED,           /* Connected MQTT broker     */
    DA16XXX_MQTT_DISCONNECTED,        /* Disconnected MQTT broker  */
} e_mqtt_module_status_t;

/* WIFI FIT module status */
typedef enum
{
    MODULE_DISCONNECTED = 0,    /* Disconnected WIFI module */
    MODULE_CONNECTED,           /* Connected WIFI module    */
    MODULE_ACCESSPOINT,         /* Connected access point   */
} e_wifi_module_status_t;

/* return code for common functions */
typedef enum
{
    ATCMD_OK             = 0,   /* OK      */
    ATCMD_ERR_MODULE_COM = -1,  /* cannot communicate module */
    ATCMD_ERR_TIMEOUT    = -2   /* communicate module timeout */
} e_atcmd_err_t;

/* UART HW flow control */
typedef enum
{
    RTS_OFF   = 0,              /* RTS = ON         */
    RTS_ON                      /* RTS = OFF        */
} e_flow_ctrl_t;

/* Result code */
typedef enum
{
    AT_OK                = 0,    /* OK                             */
    AT_UNKNOWN_CMD       = -1,   /* Unknown command                */
    AT_INSUF_PARAMS      = -2,   /* Insufficent parameter          */
    AT_TOO_MANY_PARAMS   = -3,   /* Too many parameters            */
    AT_INVALID_PARAM     = -4,   /* Wrong parameter value          */
    AT_UNSUPPORTED_FUN   = -5,   /* Unsupported function           */
    AT_NOT_CONNECTED_AP  = -6,   /* Not connected to an AP         */
    AT_NO_RESULT         = -7,   /* No result                      */
    AT_RESP_BUF_OVERFLOW = -8,   /* Response buffer overflow       */
    AT_FUNC_NOT_CONFIG   = -9,   /* Function is not configured     */
    AT_CMD_TIMEOUT       = -10,  /* Command timeout                */
    AT_NVRAM_WR_FAIL     = -11,  /* NVRAM write failure            */
    AT_RETEN_MEM_WR_FAIL = -12,  /* Retention memory write failure */
    AT_INTERNAL_TIMEOUT  = -97,  /* Internal timeout               */
    AT_INTERNAL_ERROR    = -98,  /* Internal error                 */
    AT_UNKNOWN           = -999  /* Undefined error                */
} e_rslt_code_t;

typedef enum
{
    DATA_NOT_FOUND = 0,
    DATA_FOUND,
} e_atcmd_read_t;

typedef enum
{
    DA16XXX_HTTP_CLOSE = 0,
    DA16XXX_HTTP_OPEN,
} da16xxx_http_status_t;

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: at_send_raw
 * Description  : Send raw AT commands on UART.
 * Arguments    : data
 *                length
 * Return Value : ATCMD_OK
 *                ATCMD_ERR_MODULE_COM
 *                ATCMD_ERR_TIMEOUT
 *********************************************************************************************************************/
e_atcmd_err_t at_send_raw (uint8_t *data, uint16_t const length);

/**********************************************************************************************************************
 * Function Name: at_send
 * Description  : Send AT commands on UART.
 * Arguments    : cmd
 * Return Value : ATCMD_OK
 *                ATCMD_ERR_MODULE_COM
 *                ATCMD_ERR_TIMEOUT
 *********************************************************************************************************************/
e_atcmd_err_t at_send (const char *cmd, ...);

/**********************************************************************************************************************
 * Function Name: at_recv
 * Description  : Receive response and return RESULT CODE.
 * Arguments    : none
 * Return Value : AT_OK
 *                AT_UNKNOWN_CMD
 *                AT_INSUF_PARAMS
 *                AT_TOO_MANY_PARAMS
 *                AT_INVALID_PARAM
 *                AT_UNSUPPORTED_FUN
 *                AT_NOT_CONNECTED_AP
 *                AT_NO_RESULT
 *                AT_RESP_BUF_OVERFLOW
 *                AT_FUNC_NOT_CONFIG
 *                AT_CMD_TIMEOUT
 *                AT_NVRAM_WR_FAIL
 *                AT_RETEN_MEM_WR_FAIL
 *                AT_INTERNAL_TIMEOUT
 *                AT_INTERNAL_ERROR
 *********************************************************************************************************************/
e_rslt_code_t at_recv (void);

/**********************************************************************************************************************
 * Function Name: at_exec
 * Description  : Send AT commands and Receive response on UART.
 *                Recommend using for AT basic command.
 * Arguments    : cmd
 * Return Value : Same as at_recv() function.
 *********************************************************************************************************************/
e_rslt_code_t at_exec (const char *cmd, ...);

/**********************************************************************************************************************
 * Function Name: at_exec_wo_mutex
 * Description  : Send AT commands and Receive response on UART.
 *                Recommend using for AT extended command.
 * Arguments    : cmd
 * Return Value : Same as at_recv() function.
 *********************************************************************************************************************/
e_rslt_code_t at_exec_wo_mutex (const char *cmd, ...);

/**********************************************************************************************************************
 * Function Name: at_read
 * Description  : Read buffer with prefix. (example at_read("abcd = %s\r\n", p_char) )
 * Arguments    : response_fmt
 * Return Value : 0     : data not found
 *                other : data found
 *********************************************************************************************************************/
e_atcmd_read_t at_read (const char *response_fmt, ...);

/**********************************************************************************************************************
 * Function Name: at_read_wo_prefix
 * Description  : Read buffer without prefix. (example at_read_wo_prefix("%s\r\n", p_char) )
 * Arguments    : response_fmt
 * Return Value : 0     : data not found
 *                other : data found
 *********************************************************************************************************************/
e_atcmd_read_t at_read_wo_prefix (const char *response_fmt, ...);

/**********************************************************************************************************************
 * Function Name: at_move_to_next_line
 * Description  : Move pointer to next line of response buffer.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void at_move_to_next_line (void);

/**********************************************************************************************************************
 * Function Name: at_move_to_first_line
 * Description  : Set pointer to first line of response buffer.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void at_move_to_first_line (void);

/**********************************************************************************************************************
 * Function Name: at_get_current_line
 * Description  : Get pointer on current line of response buffer.
 * Arguments    : None
 * Return Value : Pointer of current line
 *********************************************************************************************************************/
uint8_t * at_get_current_line (void);

/**********************************************************************************************************************
 * Function Name: at_set_timeout
 * Description  : Set timeout threshold for AT command response (msec).
 * Arguments    : timeout_ms
 * Return Value : None
 *********************************************************************************************************************/
void at_set_timeout (uint32_t timeout_ms);

/**********************************************************************************************************************
 * Function Name: post_err_event
 * Description  : Post error event to user callback function.
 * Arguments    : err_event - error event
 *                sock_idx  - socket index
 * Return Value : None
 *********************************************************************************************************************/
void post_err_event (wifi_err_event_enum_t err_event, uint32_t sock_idx);

/**********************************************************************************************************************
 * Function Name: flow_ctrl_init
 * Description  : Initialise hardware flow.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void flow_ctrl_init (void);

/**********************************************************************************************************************
 * Function Name: flow_ctrl_set
 * Description  : Control RTS pin.
 * Arguments    : flow - Control direction
 * Return Value : None
 *********************************************************************************************************************/
void flow_ctrl_set (e_flow_ctrl_t flow);

/**********************************************************************************************************************
 * Function Name: uart_port_open
 * Description  : Open serial port.
 * Arguments    : None
 * Return Value : ATCMD_OK
 *                ATCMD_ERR_MODULE_COM
 *********************************************************************************************************************/
e_atcmd_err_t uart_port_open(void);

/**********************************************************************************************************************
 * Function Name: uart_port_close
 * Description  : Close serial port.
 * Arguments    : none
 * Return Value : none
 *********************************************************************************************************************/
void uart_port_close(void);

/**********************************************************************************************************************
 * Function Name: printf_log_error
 * Description  : Print error log without freeRTOS logging module.
 * Arguments    : format - format message
 * Return Value : None
 *********************************************************************************************************************/
void printf_log_error (const char * format, ...);

/**********************************************************************************************************************
 * Function Name: printf_log_warn
 * Description  : Print warning log without freeRTOS logging module.
 * Arguments    : format - format message
 * Return Value : None
 *********************************************************************************************************************/
void printf_log_warn (const char * format, ...);

/**********************************************************************************************************************
 * Function Name: printf_log_info
 * Description  : Print information log without freeRTOS logging module.
 * Arguments    : format - format message
 * Return Value : None
 *********************************************************************************************************************/
void printf_log_info (const char * format, ...);

/**********************************************************************************************************************
 * Function Name: printf_log_debug
 * Description  : Print debug (AT command) log without freeRTOS logging module.
 * Arguments    : format - format message
 * Return Value : None
 *********************************************************************************************************************/
void printf_log_debug (const char * format, ...);

/**********************************************************************************************************************
 * Function Name: da16xxx_handle_incoming_common_data
 * Description  : Handles incoming common data.
 * Arguments    : type
 *                state
 *                data
 * Return Value : none
 *********************************************************************************************************************/
void da16xxx_handle_incoming_common_data(wifi_resp_type_t *type, wifi_recv_state_t *state, uint8_t data);

#if WIFI_CFG_TCP_SUPPORT == 1
/**********************************************************************************************************************
 * Function Name: da16xxx_handle_incoming_socket_data
 * Description  : Handles incoming socket data.
 * Arguments    : type
 *                state
 *                data
 * Return Value : none
 *********************************************************************************************************************/
void da16xxx_handle_incoming_socket_data(wifi_resp_type_t *type, wifi_recv_state_t *state, uint8_t data);
#endif /* WIFI_CFG_TCP_SUPPORT */

#if WIFI_CFG_TLS_SUPPORT == 1
/**********************************************************************************************************************
 * Function Name: da16xxx_handle_incoming_secure_socket_data
 * Description  : Handles incoming TLS socket data.
 * Arguments    : type
 *                state
 *                data
 * Return Value : none
 *********************************************************************************************************************/
void da16xxx_handle_incoming_secure_socket_data(wifi_resp_type_t *type, wifi_recv_state_t *state, uint8_t data);
#endif /* WIFI_CFG_TLS_SUPPORT */

#if WIFI_CFG_MQTT_SUPPORT == 1
/**********************************************************************************************************************
 * Function Name: da16xxx_handle_incoming_mqtt_data
 * Description  : Handles incoming MQTT data.
 * Arguments    : type
 *                state
 *                data
 * Return Value : none
 *********************************************************************************************************************/
void da16xxx_handle_incoming_mqtt_data(wifi_resp_type_t *type, wifi_recv_state_t *state, uint8_t data);
#endif /* WIFI_CFG_MQTT_SUPPORT */

#if WIFI_CFG_HTTP_SUPPORT == 1
/**********************************************************************************************************************
 * Function Name: da16xxx_handle_incoming_http_data
 * Description  : Handles incoming HTTP data.
 * Arguments    : type
 *                state
 *                data
 * Return Value : none
 *********************************************************************************************************************/
void da16xxx_handle_incoming_http_data(wifi_resp_type_t *type, wifi_recv_state_t *state, uint8_t data);
#endif /* WIFI_CFG_HTTP_SUPPORT */

/**********************************************************************************************************************
 * Function Name: uart_port_set_baudrate
 * Description  : Set baud rate for serial port.
 * Arguments    : rate_num
 * Return Value : none
 *********************************************************************************************************************/
void uart_port_set_baudrate (uint32_t rate_num);

#endif /* R_WIFI_DA16XXX_PRIVATE_H */
