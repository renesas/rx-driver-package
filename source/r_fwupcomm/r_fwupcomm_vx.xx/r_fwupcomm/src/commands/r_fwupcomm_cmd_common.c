/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
* File Name    : r_fwupcomm_cmd_common.c
* Description  : Source file for the Common command class.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           20.05.2025 1.00    Initial Release
*           11.11.2025 1.10    Added SPI communication support.
***********************************************************************************************************************/

#include "r_fwupcomm_cmd_common.h"

#if FWUPCOMM_CFG_CMD_COMMON_ENABLE == 1

/* Command table for the Common command class */
const FWUPCOMM_FAR r_fwupcomm_cmd_table_t r_fwupcomm_common_cmd_table[FWUPCOMM_CMD_COMMON_NUM_COMMANDS] =
{
    /* CMD_NAME                      VALUE CMD_DATA_MAX_SIZE      RESP_DATA_MAX_SIZE */
    { FWUPCOMM_CMD_COMMON_DATA_SEND, 0x01, FWUPCOMM_CFG_CMD_COMMON_MAX_DATA_SIZE, 0U },
    { FWUPCOMM_CMD_COMMON_DATA_RECV, 0x02, 0U, FWUPCOMM_CFG_CMD_COMMON_MAX_DATA_SIZE }
};

#endif /* FWUPCOMM_CFG_CMD_COMMON_ENABLE == 1 */
