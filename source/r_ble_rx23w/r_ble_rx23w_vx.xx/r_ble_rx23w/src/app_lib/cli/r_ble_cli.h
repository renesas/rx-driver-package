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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name: r_ble_cli.h
 * Description : Command Line Interface Library.
 * History : DD.MM.YYYY Version Description           
 *         : 23.08.2019 1.00    First Release
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file
 * @defgroup cli Command Line Interface Library
 * @{
 * @ingroup app_lib
 * @brief Command Line Interface Library.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <stdint.h>
#include <string.h>
#include "r_ble_rx23w_if.h"
#include "console/r_ble_console.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#ifndef R_BLE_CLI_H
#define R_BLE_CLI_H

/** @defgroup cli_macro Macros 
 *  @{
 *  @brief Macro definition
 */

/*******************************************************************************************************************//**
 * @brief Maximum characters in a line
 **********************************************************************************************************************/
#define BLE_CLI_LINE_LEN          (80)

/*******************************************************************************************************************//**
 * @brief Number of history command.
 **********************************************************************************************************************/
#define BLE_CLI_NUM_OF_HISTORY    (8)

/*******************************************************************************************************************//**
 * @brief Number of sections in a line 
 **********************************************************************************************************************/
#define BLE_CLI_NUM_OF_COMPLEMENT (10)
/*@}*/

/** @defgroup cli_callback Callbacks
 *  @{
 *  @brief Callback definition
 */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Command execution handler.
 **********************************************************************************************************************/
typedef void (*ble_cli_exec_cb_t)(int argc, char *p_argv[]);

/*******************************************************************************************************************//**
 * @brief Command abort handler.
 **********************************************************************************************************************/
typedef void (*ble_cli_abort_cb_t)(void);
/*@}*/

/** @defgroup cli_struct Structures 
 *  @{
 *  @brief Structure definition
 */

/*******************************************************************************************************************//**
 * @brief Command parameters.
 **********************************************************************************************************************/
typedef struct st_ble_cli_cmd_t
{
    const char                            *p_name;      /**< Command name. */
    const char                            *p_help;      /**< Command help message. */
    const struct st_ble_cli_cmd_t * const *p_cmds;      /**< Sub commands. */
    uint16_t                              num_of_cmds;  /**< Number of sub commands */
    ble_cli_exec_cb_t                     exec;         /**< Command execution handler. */
    ble_cli_abort_cb_t                    abort;        /**< Command abort handler. */
} st_ble_cli_cmd_t;
/*@}*/

/** @defgroup cli_func Functions
 *  @{
 *  @brief Function definition
 */

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Initialize CLI library.
 * @return BLE_SUCCESS, otherwise an error code
 **********************************************************************************************************************/
ble_status_t R_BLE_CLI_Init(void);

/*******************************************************************************************************************//**
 * @brief Terminate CLI library.
 * @return BLE_SUCCESS, otherwise an error code
 **********************************************************************************************************************/
ble_status_t R_BLE_CLI_Terminate(void);

/*******************************************************************************************************************//**
 * @brief Register top level commands.
 * @param[in] p_cmds      Commands
 * @param[in] num_of_cmds Number of commands
 * @return BLE_SUCCESS, otherwise an error code
 **********************************************************************************************************************/
ble_status_t R_BLE_CLI_RegisterCmds(const st_ble_cli_cmd_t * const p_cmds[], uint16_t num_of_cmds);

/*******************************************************************************************************************//**
 * @brief Process input characters from the user.
 * @details This function process the input characters.
 **********************************************************************************************************************/
void R_BLE_CLI_Process(void);

/*******************************************************************************************************************//**
 * @brief Print with format like printf.
 * @details This function print with format.
 **********************************************************************************************************************/
#define R_BLE_CLI_Printf console_putsf

/*******************************************************************************************************************//**
 * @brief Inform the completion of abortable command.
 **********************************************************************************************************************/
void R_BLE_CLI_SetCmdComp(void);
/*@}*/

#endif /* R_BLE_CLI_H */

/*@}*/
