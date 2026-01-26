/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
 * File Name    : r_fwupcomm_cmd_base.h
 * Description  : Base of command header file for the fwupcomm module.
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           20.05.2025 1.00    Initial Release
 *           11.11.2025 1.10    Added SPI communication support.
 ***********************************************************************************************************************/

#ifndef SMC_GEN_R_FWUPCOMM_SRC_COMMANDS_R_FWUPCOMM_CMD_BASE_H_
#define SMC_GEN_R_FWUPCOMM_SRC_COMMANDS_R_FWUPCOMM_CMD_BASE_H_

#include <stdint.h>
#include "r_smc_entry.h"

#if defined(__RX)
    /* __far */
    #define FWUPCOMM_FAR
#else
    /* __far */
    #define FWUPCOMM_FAR        __far
#endif /* defined(__RX) */

/* Structure for specifying command */
typedef struct r_fwupcomm_cmd_table
{
    uint8_t  type;                /* Value indicating this command (enumerator) */
    uint8_t  value;               /* Actual value used for communications by this command */
    uint16_t cmd_data_max_size;   /* Maximum size of the command data of this command */
    uint16_t resp_data_max_size;  /* Maximum size of the response data of this command */
} r_fwupcomm_cmd_table_t;


/* Structure for specifying command option */
typedef struct r_fwupcomm_cmd_option
{
    uint8_t no_resp;    /* Whether to return a response */
} r_fwupcomm_cmd_option_t;

/* Structure for specifying command information */
typedef struct r_fwupcomm_cmd_info
{
    uint8_t      device_address;        /* Address of the destination device for a command */
    uint8_t      class;                 /* Command class */
    r_fwupcomm_cmd_option_t option;     /* Command option */
    uint8_t      name;                  /* Command name */
    uint8_t      arg;                   /* Command argument */
    uint16_t     data_size;             /* Command data size */
    const void * data;                  /* Pointer to command data */
} r_fwupcomm_cmd_info_t;

/* Structure for storing response information */
typedef struct r_fwupcomm_resp_info
{
    int8_t   result;        /* Command result */
    void *   data;          /* Pointer to the destination for storing response data */
    uint16_t data_size;     /* Size of the destination for storing response data*/
} r_fwupcomm_resp_info_t;

/* Structure used as an argument of the CmdSend function when a command is to be sent */
typedef struct r_fwupcomm_cmd_instr
{
    uint16_t               timeout_ms;    /* Timeout time from sending the command to receiving the response */
#if BSP_CFG_RTOS_USED == 1  /* FreeRTOS is used. */
    TaskHandle_t           notify_task;
#endif /* BSP_CFG_RTOS_USED == 1 */
    r_fwupcomm_cmd_info_t  cmd;           /* Command information */
    r_fwupcomm_resp_info_t resp;          /* Destination for storing response information */
} r_fwupcomm_cmd_instr_t;

/**********************************************************************************************************************
 * Function Name:  R_FWUPCOMM_CmdHandler_t
 * Description  : Definition of a handler function to be executed when a command is received
 * Arguments    : cmd  : Received command information
 *              : resp : Store response information
 * Return Value : None
 *********************************************************************************************************************/
typedef void (* R_FWUPCOMM_CmdHandler_t)(r_fwupcomm_cmd_info_t * cmd,
                                            r_fwupcomm_resp_info_t * resp);


#endif /* SMC_GEN_R_FWUPCOMM_SRC_COMMANDS_R_FWUPCOMM_CMD_BASE_H_ */
