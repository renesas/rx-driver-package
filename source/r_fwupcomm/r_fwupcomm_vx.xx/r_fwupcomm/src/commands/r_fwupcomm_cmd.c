/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
* File Name    : r_fwupcomm_cmd.c
* Description  : Source file for the command class.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           20.05.2025 1.00    Initial Release
*           11.11.2025 1.10    Added SPI communication support.
***********************************************************************************************************************/


#include "r_fwupcomm_cmd.h"

/* Command definition table of each command class. */
const FWUPCOMM_FAR r_fwupcomm_cmd_def_table_t r_fwupcomm_cmd_def_table_list[FWUPCOMM_CMD_NUM_CLASS] =
{
#if FWUPCOMM_CFG_CMD_COMMON_ENABLE == 1
    [FWUPCOMM_CMD_CLS_COMMON] = {r_fwupcomm_common_cmd_table, FWUPCOMM_CMD_COMMON_NUM_COMMANDS},
#endif /* FWUPCOMM_CFG_CMD_COMMON_ENABLE == 1 */
#if FWUPCOMM_CFG_CMD_FWUP_ENABLE == 1
    [FWUPCOMM_CMD_CLS_FWUP]   = {r_fwupcomm_fwup_cmd_table,   FWUPCOMM_CMD_FWUP_NUM_COMMANDS},
#endif /* FWUPCOMM_CFG_CMD_FWUP_ENABLE == 1 */
};

#if FWUPCOMM_CFG_DEVICE_PRIMARY == (0)
/* Command handler of each command class. */
const FWUPCOMM_FAR R_FWUPCOMM_CmdHandler_t r_fwupcomm_cmd_handler_list[FWUPCOMM_CMD_NUM_CLASS] =
{
#if FWUPCOMM_CFG_CMD_COMMON_ENABLE == 1
    [FWUPCOMM_CMD_CLS_COMMON] = FWUPCOMM_CFG_CMD_HANDLER_COMMON,
#endif /* FWUPCOMM_CFG_CMD_COMMON_ENABLE == 1 */
#if FWUPCOMM_CFG_CMD_FWUP_ENABLE == 1
    [FWUPCOMM_CMD_CLS_FWUP]   = FWUPCOMM_CFG_CMD_HANDLER_FWUP,
#endif /* FWUPCOMM_CFG_CMD_FWUP_ENABLE == 1 */
};
#endif
