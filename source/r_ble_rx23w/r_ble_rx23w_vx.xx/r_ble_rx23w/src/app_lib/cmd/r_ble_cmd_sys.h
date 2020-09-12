/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file
 * @defgroup cmd_sys System Command Library
 * @{
 * @ingroup cli
 * @brief GAP Commands
 * @details This library provides CLI for System. And exposes APIs used in the commands.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 23.08.2019 1.00    First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_ble_rx23w_if.h"
#include "cli/r_ble_cli.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#ifndef R_BLE_CMD_SYS_H
#define R_BLE_CMD_SYS_H

/** @defgroup cmd_sys_val Variables
 *  @{
 *  @brief Variable definition
 */
/** @brief SYS command definition. */
extern const st_ble_cli_cmd_t g_sys_cmd;

/** @brief ble reset command callback. */
extern ble_event_cb_t g_ble_host_reset_cb;

/** @brief ble command definition. */
extern const st_ble_cli_cmd_t g_ble_cmd;

/*******************************************************************************************************************//**
 * @brief Register a callback for "ble reset" command.
 * @details
 * This function registers a callback which initializes the upper layer after the "ble reset" command.
 * @param[in] reset_cb A callback to be registered. 
 * @retval  none
***********************************************************************************************************************/
void R_BLE_CMD_SetResetCb(ble_event_cb_t reset_cb);

/*@}*/

#endif /* R_BLE_CMD_SYS_H */
/*@}*/
