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
 * File Name    : r_ble_da14531_config.h
 * Description  : DA14531 BLE driver Configuration.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef R_BLE_DA14531_CONFIG_H_
#define R_BLE_DA14531_CONFIG_H_

#define BLE_ABS_CFG_PARAM_CHECKING_ENABLE           (BSP_CFG_PARAM_CHECKING_ENABLE)
#define RM_BLE_ABS_GTL_TRANSPORT_INTERFACE_UART     1
#define BLE_CFG_SCI_CHANNEL                         (6)
#define BLE_CFG_SCI_INTERRUPT_LEVEL                 (4)
#define BLE_CFG_SCK_PORT                            0
#define BLE_CFG_SCK_PIN                             2
#define BLE_CFG_RESET_PORT                          5
#define BLE_CFG_RESET_PIN                           5

#define BLE_ABS_CFG_NUMBER_BONDING                  (1)
#define BLE_ABS_CFG_TIMER_NUMBER_OF_SLOT            (10)
#define BLE_ABS_CFG_GATT_MTU_SIZE                   (247)
#define BLE_ABS_CFG_RF_CONNECTION_MAXIMUM           (1)
#define BLE_CFG_RF_CONN_MAX                         (1)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

#endif /* R_BLE_DA14531_CONFIG_H_ */
