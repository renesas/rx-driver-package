/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
* File Name    : r_fwupcomm_if.h
* Description  : Interface header file for the fwupcomm module.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           20.05.2025 1.00    Initial Release
*           11.11.2025 1.10    Added SPI communication support.
***********************************************************************************************************************/

#ifndef FWUPCOMM_IF_H_
#define FWUPCOMM_IF_H_

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "r_smc_entry.h"
#include "r_fwupcomm_cmd.h"
#include "r_fwupcomm_config.h"

#if BSP_CFG_RTOS_USED == 1  /* FreeRTOS is used. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#endif /* BSP_CFG_RTOS_USED == 1 */

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
/* Version Number of API. */
#define FWUPCOMM_VERSION_MAJOR                  (1)
#define FWUPCOMM_VERSION_MINOR                  (10)

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/* Enumeration of the return values of the API functions */
typedef enum
{
    FWUPCOMM_SUCCESS = 0,
    FWUPCOMM_ERR_INVALID_PTR,        /* The pointer passed as an argument was NULL. */
    FWUPCOMM_ERR_INVALID_ARG,        /* The parameter passed as an argument was invalid. */
    FWUPCOMM_ERR_NOT_OPEN,           /* The module has not been opened. */
    FWUPCOMM_ERR_ALREADY_OPEN,       /* The module has already been initialized. */
    FWUPCOMM_ERR_INVALID_CMD,        /* An invalid command was received. */
    FWUPCOMM_ERR_INVALID_RESP,       /* The received response was invalid. */
    FWUPCOMM_ERR_RECV_RESP_TIMEOUT,  /* A timeout occurred before a response was received. */
    FWUPCOMM_ERR_NO_CMD,             /* No command was received. */
    FWUPCOMM_ERR_CH_ALREADY_OPEN,    /* The communications channel has already been opened. */
    FWUPCOMM_ERR_CH_SEND,            /* Sending of data in the communications channel failed. */
    FWUPCOMM_ERR_CH_SEND_BUSY,       /* The communications channel was busy so sending of data failed. */
    FWUPCOMM_ERR_CH_RECV,            /* Receiving of data from the communications channel failed. */
    FWUPCOMM_ERR_CH_RECV_NO_DATA,    /* The communications channel does not have enough received data. */
} fwupcomm_err_t;

/* Declaration of the structure used for the handler of the module */
typedef struct r_fwupcomm_hdl r_fwupcomm_hdl_t;

/**********************************************************************************************************************
 * Function Name:  r_fwupcomm_timer_cb_t
 * Description  : Declaration of the callback function to be called when the timer expires.
 * Argument     : hdl: Handler of the module
 * Return Value : None
 *********************************************************************************************************************/
typedef void (* r_fwupcomm_timer_cb_t)(r_fwupcomm_hdl_t * hdl);

/**********************************************************************************************************************
 * Function Name:  r_fwupcomm_start_timer_t
 * Description  : Declaration of the function to start counting by a timer.
 * Arguments    : hdl       : Handler of the module
 *              : timeout_ms: Timeout time for the timer
 *              : callback  : Callback function to be called when the timer expires
 * Return Value : None
 *********************************************************************************************************************/
typedef void (* r_fwupcomm_start_timer_t)(r_fwupcomm_hdl_t * hdl,
                                            uint32_t timeout_ms,
                                            r_fwupcomm_timer_cb_t callback);

/**********************************************************************************************************************
 * Function Name:  r_fwupcomm_stop_timer_t
 * Description  : Declaration of the function to stop counting by a timer.
 * Return Value : None
 *********************************************************************************************************************/
typedef void (* r_fwupcomm_stop_timer_t)(void);

/* Structure used for registering a timer interface */
typedef struct r_fwupcomm_timer
{
    r_fwupcomm_start_timer_t start;  /* Pointer to the function to start counting by a timer */
    r_fwupcomm_stop_timer_t stop;    /* Pointer to the function to stop counting by a timer */
} r_fwupcomm_timer_t;

typedef struct r_fwupcomm_cmd_buf
{
    uint8_t * data;    /* Pointer to the packet buffer */
    uint16_t  size;    /* Size of the packet buffer */
} r_fwupcomm_packet_buf_t;

/* Structure used for the handler of the module */
struct r_fwupcomm_hdl
{
    uint8_t                 open_flag;            /* Flag indicating whether the module is open or not */
    uint8_t                 timer_timeout_flag;   /* Flag indicating whether the timer has timed out or not */
    r_fwupcomm_timer_t      timer;                /* Timer interface */
    r_fwupcomm_packet_buf_t send_buf;             /* Buffer for sending packet */
    r_fwupcomm_packet_buf_t recv_buf;             /* Buffer for receiving packet */
#if BSP_CFG_RTOS_USED == 1  /* FreeRTOS is used. */
    QueueHandle_t cmd_queue;                      /* Queue for sending command instructions */
    TaskHandle_t task_hdl;                        /* Handler for task to be notified after communications */
#endif /* BSP_CFG_RTOS_USED == 1 */
};

/* Structure used as an argument of the Open function during initialization */
typedef struct r_fwupcomm_cfg
{
    r_fwupcomm_timer_t timer;    /* Timer interface */
} r_fwupcomm_cfg_t;


/**********************************************************************************************************************
 * Function Name: R_FWUPCOMM_Open
 * Description  : Opens a communications channel for use by or within this module. This function must be executed 
 *                before other API functions are used.
 * Arguments    : hdl: Handler of the module
 *              : cfg: Structure variable with information required for initializing modules
 * Return Value : FWUPCOMM_SUCCESS            : The channel was successfully initialized.
 *              : FWUPCOMM_ERR_INVALID_PTR    : The pointer passed as an argument was NULL.
 *              : FWUPCOMM_ERR_ALREADY_OPEN   : The module has already been initialized.
 *              : FWUPCOMM_ERR_CH_ALREADY_OPEN: The communications channel has already been opened.
 *              : FWUPCOMM_ERR_NOT_OPEN       : Initializing the communication channel failed.
 *********************************************************************************************************************/
fwupcomm_err_t R_FWUPCOMM_Open (r_fwupcomm_hdl_t * hdl, void * cfg);

/**********************************************************************************************************************
 * Function Name: R_FWUPCOMM_Close
 * Description  : Closes a communications channel for use by or within this module.
 * Argument     : hdl: Handler of the module
 * Return Value : FWUPCOMM_SUCCESS            : Closing was successful.
 *              : FWUPCOMM_ERR_NOT_OPEN       : The module has not been opened.
 *              : FWUPCOMM_ERR_INVALID_PTR    : The pointer passed as an argument is NULL.
 *********************************************************************************************************************/
fwupcomm_err_t R_FWUPCOMM_Close (r_fwupcomm_hdl_t * hdl);

/**********************************************************************************************************************
 * Function Name: R_FWUPCOMM_CmdSend
 * Description  : Sends a command for the secondary MCU and receives a corresponding response.
 * Arguments    : hdl      : Handler of the module
 *              : cmd_instr: Structure variable with information on the command to be sent 
 *                           and the destination for storing the response
 * Return Value : FWUPCOMM_SUCCESS:               The command was successfully completed.
 *              : FWUPCOMM_ERR_NOT_OPEN:          The module has not been opened.
 *              : FWUPCOMM_ERR_INVALID_PTR:       The pointer passed as an argument was NULL.
 *              : FWUPCOMM_ERR_INVALID_ARG:       The parameter passed as an argument was invalid.
 *              : FWUPCOMM_ERR_CH_SEND:           Sending of data in the communications channel failed.
 *              : FWUPCOMM_ERR_CH_RECV:           Receiving of data from the communications channel failed.
 *              : FWUPCOMM_ERR_RECV_RESP_TIMEOUT: A timeout occurred before a response was received.
 *********************************************************************************************************************/
fwupcomm_err_t R_FWUPCOMM_CmdSend (r_fwupcomm_hdl_t * hdl, r_fwupcomm_cmd_instr_t * cmd_instr);

/**********************************************************************************************************************
 * Function Name: R_FWUPCOMM_ProcessCmdLoop
 * Description  : Receives a command from the primary MCU, runs the corresponding handler, 
 *                and sends the result of executing the command. Periodically execute this function 
 *                in the secondary MCU while it is waiting for commands.
 * Argument     : hdl: Handler of the module
 * Return Value : FWUPCOMM_SUCCESS:         The channel was successfully initialized.
 *              : FWUPCOMM_ERR_NOT_OPEN:    The module has not been opened.
 *              : FWUPCOMM_ERR_INVALID_PTR: The pointer passed as an argument was NULL.
 *              : FWUPCOMM_ERR_INVALID_ARG: The parameter passed as an argument was invalid.
 *              : FWUPCOMM_ERR_NO_CMD:      No command was received.
 *              : FWUPCOMM_ERR_INVALID_CMD: An invalid command was received.
 *              : FWUPCOMM_ERR_CH_SEND:     Sending of data in the communications channel failed.
 *              : FWUPCOMM_ERR_CH_RECV:     Receiving of data from the communications channel failed.
 *********************************************************************************************************************/
fwupcomm_err_t R_FWUPCOMM_ProcessCmdLoop (r_fwupcomm_hdl_t * hdl);


#endif /* FWUPCOMM_IF_H_ */
