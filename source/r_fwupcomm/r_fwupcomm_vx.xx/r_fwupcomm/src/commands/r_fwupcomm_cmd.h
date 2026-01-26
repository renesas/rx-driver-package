/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
 * File Name    : r_fwupcomm_cmd.h
 * Description  : Header file for the command class.
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           20.05.2025 1.00    Initial Release
 *           11.11.2025 1.10    Added SPI communication support.
 ***********************************************************************************************************************/

#ifndef FWUPCOMM_SRC_R_FWUPCOMM_CMD_H_
#define FWUPCOMM_SRC_R_FWUPCOMM_CMD_H_

#include "r_fwupcomm_cmd_base.h"
#include "r_fwupcomm_cmd_common.h"
#include "r_fwupcomm_cmd_fwup.h"

/* Number of defined command classes */
#define FWUPCOMM_CMD_NUM_CLASS    (FWUPCOMM_CFG_CMD_COMMON_ENABLE + FWUPCOMM_CFG_CMD_FWUP_ENABLE)

/* Enumerated type for defining the command classes */
typedef enum
{
#if FWUPCOMM_CFG_CMD_COMMON_ENABLE == 1
    FWUPCOMM_CMD_CLS_COMMON = (0),    /* Common command */
#endif /* FWUPCOMM_CFG_CMD_COMMON_ENABLE == 1 */
#if FWUPCOMM_CFG_CMD_FWUP_ENABLE == 1
    FWUPCOMM_CMD_CLS_FWUP = (1),      /* FWUP command */
#endif /* FWUPCOMM_CFG_CMD_FWUP_ENABLE == 1 */
} r_fwupcomm_cmd_class_t;

/* Structure for defining the command definitions table */
typedef struct
{
    const FWUPCOMM_FAR r_fwupcomm_cmd_table_t * table;    /* Pointer to the command table for the command class */
    uint8_t                        num_cmd;  /* Number of commands in the command class */
} r_fwupcomm_cmd_def_table_t;

/* Command definition table of each command class. */
extern const FWUPCOMM_FAR r_fwupcomm_cmd_def_table_t r_fwupcomm_cmd_def_table_list[FWUPCOMM_CMD_NUM_CLASS];

/* Command handler of each command class. */
extern const R_FWUPCOMM_CmdHandler_t FWUPCOMM_FAR r_fwupcomm_cmd_handler_list[FWUPCOMM_CMD_NUM_CLASS];

#endif /* FWUPCOMM_SRC_R_FWUPCOMM_CMD_H_ */
