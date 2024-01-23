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
#define SOCK_TBL_MAX                   (WIFI_CFG_CREATABLE_SOCKETS)            // Socket table (default:4)
#define SOCK_BUF_MAX                   (WIFI_CFG_SOCKETS_RECEIVE_BUFFER_SIZE)  // Socket buffer
#define TEMP_BUf_MAX                   (256)                                   // RX temp buffer

#define ATCMD_RESP_TIMEOUT             (1000)   // Timeout threshold for AT command response (msec)
#define DA16XXX_AT_SOCK_TX_MAX         (4096)   // DA16XXX socket transmit max length
#define DA16XXX_AT_CMD_BUF_MAX         (WIFI_CFG_AT_CMD_TX_BUFFER_SIZE)
#define DA16XXX_AT_RESP_BUF_MAX        (WIFI_CFG_AT_CMD_RX_BUFFER_SIZE)

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

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
/* UART table information */
typedef struct
{
    sci_hdl_t               sci_hdl;
    sci_cfg_t               sci_config;
    byteq_hdl_t             byteq_hdl;
    uint8_t                 recv_buf[DA16XXX_AT_RESP_BUF_MAX];
    da16xxx_resp_type_t     at_resp_type;
    da16xxx_recv_state_t    socket_recv_state;
    volatile uint8_t        tx_end_flag;
} st_uart_tbl_t;

/* Socket Timer */
typedef struct
{
    OS_TICK   threshold;        /* Timeout threshold */
    OS_TICK   tick_sta;         /* Tick of Timer start  */
} st_sock_timer_t;

/* Socket table information */
typedef struct
{
    uint8_t                 ipaddr[4];
    uint32_t                port;
    da16xxx_socket_status_t status;
    uint8_t                 ipver;
    da16xxx_socket_type_t   type;
    byteq_hdl_t             byteq_hdl;
    uint32_t                put_err_cnt;
    uint8_t                 recv_buf[SOCK_BUF_MAX];
    int32_t                 recv_len;
    st_sock_timer_t         timer_tx;
    st_sock_timer_t         timer_rx;
} st_sock_tbl_t;

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
    AT_MAX               = -99   /* Stopper                        */
} e_rslt_code_t;

typedef enum
{
    DATA_NOT_FOUND = 0,
    DATA_FOUND,
} e_atcmd_read_t;

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/
extern st_uart_tbl_t g_uart_tbl;

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: at_resp_byteq_open
 * Description  : open BYTEQ for AT command response.
 * Arguments    : none
 * Return Value : WIFI_SUCCESS
 *                WIFI_ERR_BYTEQ_OPEN
 *********************************************************************************************************************/
wifi_err_t at_resp_byteq_open (void);

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
uint32_t at_read (const char *response_fmt, ...);

/**********************************************************************************************************************
 * Function Name: at_read_wo_prefix
 * Description  : Read buffer without prefix. (example at_read_wo_prefix("%s\r\n", p_char) )
 * Arguments    : response_fmt
 * Return Value : 0     : data not found
 *                other : data found
 *********************************************************************************************************************/
uint32_t at_read_wo_prefix (const char *response_fmt, ...);

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
 * Arguments    : p_cb  - Callback function of SCI interrupts
 * Return Value : ATCMD_OK
 *                ATCMD_ERR_MODULE_COM
 *********************************************************************************************************************/
e_atcmd_err_t uart_port_open(void (* const p_cb)(void *p_args));

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

#endif /* R_WIFI_DA16XXX_PRIVATE_H */
