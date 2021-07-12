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
* Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file
 * @defgroup rtos BLE Task functions for FreeRTOS
 * @{
 * @ingroup app_lib
 * @brief BLE Task functions for FreeRTOS
 * @details This functions control the BLE task on FreeRTOS.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_ble_rx23w_if.h"

#if (BSP_CFG_RTOS_USED == 1)
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#ifndef R_BLE_RTOS_H
#define R_BLE_RTOS_H

/******************************************************************************************************************//**
Exported global variables
***********************************************************************************************************************/
/** @defgroup rtos_val Variables
 *  @{
 *  @brief Variable definition
 */
/** @brief BLE task handle. */
extern TaskHandle_t g_ble_task_hdl;
/*@}*/

/** @defgroup rtos_ble_func Functions
 *  @{
 *  @brief Function definition
 */

/******************************************************************************************************************//**
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
/******************************************************************************************************************//**
 * @fn void R_BLE_RTOS_WakeTask(void)
 * @brief   Wake up the BLE Task.
 * @param   void
 * @retval  none
 **********************************************************************************************************************/
void R_BLE_RTOS_WakeTask(void);

/******************************************************************************************************************//**
 * @fn void R_BLE_RTOS_WakeTaskFromIsr(void)
 * @brief   Wake up the BLE Task from ISR.
 * @param   void
 * @retval  none
 **********************************************************************************************************************/
void R_BLE_RTOS_WakeTaskFromIsr(void);

/*@}*/

#endif /* (BSP_CFG_RTOS_USED == 1) */
#endif /* R_BLE_RTOS_H */
/*@}*/

