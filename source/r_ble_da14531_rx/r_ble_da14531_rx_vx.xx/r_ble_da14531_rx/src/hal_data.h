/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : hal_data.h
 * Description  :
 *********************************************************************************************************************/
/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <string.h>
#include "platform.h"
#include "rm_ble_abs.h"
#include "rm_ble_abs_api.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef HAL_DATA_H_
#define HAL_DATA_H_

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/
/** Access the BLE_ABS instance using these structures when calling API functions directly (::p_api is not used). */
extern ble_abs_instance_ctrl_t g_ble_abs0_ctrl;
extern ble_abs_cfg_t g_ble_abs0_cfg;

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

#ifndef gap_cb
void gap_cb(uint16_t type, ble_status_t result, st_ble_evt_data_t *p_data);
#endif

#ifndef vs_cb
void vs_cb(uint16_t type, ble_status_t result, st_ble_vs_evt_data_t *p_data);
#endif

#ifndef gs_abs_gatts_cb_param
extern ble_abs_gatt_server_callback_set_t gs_abs_gatts_cb_param[];
#else
ble_abs_gatt_server_callback_set_t gs_abs_gatts_cb_param[];
#endif

#ifndef gs_abs_gattc_cb_param
extern ble_abs_gatt_client_callback_set_t gs_abs_gattc_cb_param[];
#else
ble_abs_gatt_client_callback_set_t gs_abs_gattc_cb_param[];
#endif

#endif /* HAL_DATA_H_ */
