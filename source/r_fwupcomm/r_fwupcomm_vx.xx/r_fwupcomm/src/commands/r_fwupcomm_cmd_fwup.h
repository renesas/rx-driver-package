/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
* File Name    : r_fwupcomm_cmd_fwup.h
* Description  : Header file for the FWUP command class.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           20.05.2025 1.00    Initial Release
*           11.11.2025 1.10    Added SPI communication support.
***********************************************************************************************************************/

#ifndef FWUPCOMM_SRC_COMMANDS_R_FWUPCOMM_CMD_FWUP_H_
#define FWUPCOMM_SRC_COMMANDS_R_FWUPCOMM_CMD_FWUP_H_

#include "r_fwupcomm_cmd_base.h"
#include "r_fwupcomm_config.h"

#if FWUPCOMM_CFG_CMD_FWUP_ENABLE == 1

/* Enumerated type for defining commands of the FWUP command class */
typedef enum
{
    FWUPCOMM_CMD_FWUP_START = (0),    /* START command */
    FWUPCOMM_CMD_FWUP_WRITE,          /* WRITE command */
    FWUPCOMM_CMD_FWUP_INSTALL,        /* INSTALL command */
    FWUPCOMM_CMD_FWUP_CANCEL,         /* CANCEL command */
    FWUPCOMM_CMD_FWUP_VERSION,        /* VERSION command */
    FWUPCOMM_CMD_FWUP_NUM_COMMANDS    /* Number of defined FWUP commands */
} r_fwupcomm_cmd_name_fwup_t;

/* Enumerated type for defining the command result of the FWUP command class */
typedef enum
{
    FWUPCOMM_CMD_FWUP_RESULT_SUCCESS   = (0),    /* Command execution succeeded */
    FWUPCOMM_CMD_FWUP_RESULT_VERIFY_OK = (1),    /* Signature verification succeeded */
    FWUPCOMM_CMD_FWUP_RESULT_FAILED    = (2)     /* Command execution failed */
} r_fwupcomm_cmd_result_fwup_t;

/* Command table for the FWUP command class */
extern const FWUPCOMM_FAR r_fwupcomm_cmd_table_t r_fwupcomm_fwup_cmd_table[FWUPCOMM_CMD_FWUP_NUM_COMMANDS];

#if FWUPCOMM_CFG_DEVICE_PRIMARY == (0)
/**********************************************************************************************************************
* Function Name: FWUPCOMM_CFG_CMD_HANDLER_FWUP
* Description  : Process when the FWUP command is received.
* Arguments    : cmd  : Received command information.
*                resp : Store response information.
* Return Value : none
**********************************************************************************************************************/
void FWUPCOMM_CFG_CMD_HANDLER_FWUP (r_fwupcomm_cmd_info_t * cmd, r_fwupcomm_resp_info_t * resp);
#endif

#endif /* FWUPCOMM_CFG_CMD_FWUP_ENABLE == 1 */

#endif /* FWUPCOMM_SRC_COMMANDS_R_FWUPCOMM_CMD_FWUP_H_ */
