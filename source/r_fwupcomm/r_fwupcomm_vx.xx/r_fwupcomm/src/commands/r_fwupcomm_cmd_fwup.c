/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
* File Name    : r_fwupcomm_cmd_fwup.c
* Description  : Source file for the FWUP command class.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           20.05.2025 1.00    Initial Release
*           11.11.2025 1.10    Added SPI communication support.
***********************************************************************************************************************/

#include "r_fwupcomm_cmd_fwup.h"

#if FWUPCOMM_CFG_CMD_FWUP_ENABLE == 1

/* Command table for the FWUP command class */
const FWUPCOMM_FAR r_fwupcomm_cmd_table_t r_fwupcomm_fwup_cmd_table[FWUPCOMM_CMD_FWUP_NUM_COMMANDS] =
{
    /* CMD_NAME                  VALUE CMD_DATA_MAX_SIZE               RESP_DATA_MAX_SIZE */
    { FWUPCOMM_CMD_FWUP_START,   0x01, FWUPCOMM_CFG_CMD_FWUP_START_DATA_SIZE,          0U },
    { FWUPCOMM_CMD_FWUP_WRITE,   0x02, FWUPCOMM_CFG_CMD_FWUP_WRITE_FW_BLOCK_SIZE,      4U },
    { FWUPCOMM_CMD_FWUP_INSTALL, 0x03, 0U,                                             0U },
    { FWUPCOMM_CMD_FWUP_CANCEL,  0x04, 0U,                                             0U },
    { FWUPCOMM_CMD_FWUP_VERSION, 0xF0, 0U,                                             4U }
};

#endif /* FWUPCOMM_CFG_CMD_FWUP_ENABLE == 1 */
