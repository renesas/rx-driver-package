/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
* File Name    : r_fwupcomm_cmd_common.h
* Description  : Header file for the Common command class.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           20.05.2025 1.00    Initial Release
*           11.11.2025 1.10    Added SPI communication support.
***********************************************************************************************************************/

#ifndef FWUPCOMM_SRC_COMMANDS_R_FWUPCOMM_CMD_COMMON_H_
#define FWUPCOMM_SRC_COMMANDS_R_FWUPCOMM_CMD_COMMON_H_

#include "r_fwupcomm_cmd_base.h"
#include "r_fwupcomm_config.h"

#if FWUPCOMM_CFG_CMD_COMMON_ENABLE == 1

/* Enumerated type for defining commands of the Common command class */
typedef enum
{
    FWUPCOMM_CMD_COMMON_DATA_SEND = (0),    /* DATA SEND command */
    FWUPCOMM_CMD_COMMON_DATA_RECV,          /* DATA_RECV command */
    FWUPCOMM_CMD_COMMON_NUM_COMMANDS        /* Number of defined Common commands */
} r_fwupcomm_cmd_name_common_t;

/* Enumerated type for defining the command result of the Common command class */
typedef enum
{
    FWUPCOMM_CMD_COMMON_RESULT_SUCCESS = 0,    /* Command execution succeeded */
    FWUPCOMM_CMD_COMMON_RESULT_FAILED = 1      /* Command execution failed */
} r_fwupcomm_cmd_result_common_t;

/* Command table for the Common command class */
extern const FWUPCOMM_FAR r_fwupcomm_cmd_table_t r_fwupcomm_common_cmd_table[FWUPCOMM_CMD_COMMON_NUM_COMMANDS];

#if FWUPCOMM_CFG_DEVICE_PRIMARY == (0)
/**********************************************************************************************************************
* Function Name: FWUPCOMM_CFG_CMD_HANDLER_COMMON
* Description  : Process when the Common command is received.
* Arguments    : cmd  : Received command information
*                resp : Store response information
* Return Value : None
**********************************************************************************************************************/
void FWUPCOMM_CFG_CMD_HANDLER_COMMON (r_fwupcomm_cmd_info_t * cmd, r_fwupcomm_resp_info_t * resp);
#endif

#endif /* FWUPCOMM_CFG_CMD_COMMON_ENABLE == 1 */

#endif /* FWUPCOMM_SRC_COMMANDS_R_FWUPCOMM_CMD_COMMON_H_ */
