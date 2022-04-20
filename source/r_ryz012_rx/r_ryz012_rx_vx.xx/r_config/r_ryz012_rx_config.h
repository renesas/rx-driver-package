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
 * Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_ryz012_rx_config.h
 * Description  : SX ULPGN WiFi driver Configuration.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef R_RYZ012_RX_CONFIG_H_
#define R_RYZ012_RX_CONFIG_H_

#define BLE_CFG_LIBRARY_TYPE                        (0)
#define BLE_CFG_RYZ012_DEVICE                       (1)
#define BLE_ABS_CFG_RF_CONNECTION_MAXIMUM           (1)
#define BLE_ABS_CFG_RF_CONNECTION_DATA_MAXIMUM      (251)
#define BLE_ABS_CFG_RF_ADVERTISING_DATA_MAXIMUM     (1650)
#define BLE_ABS_CFG_RF_ADVERTISING_SET_MAXIMUM      (1)
#define BLE_ABS_CFG_RF_SYNC_SET_MAXIMUM             (1)
#define BLE_ABS_CFG_ENABLE_SECURE_DATA              (0)
#define BLE_ABS_CFG_SECURE_DATA_DATAFLASH_BLOCK     (0)
#define BLE_ABS_CFG_NUMBER_BONDING                  (1)
#define BLE_ABS_CFG_EVENT_NOTIFY_CONNECTION_START   (1)
#define BLE_ABS_CFG_EVENT_NOTIFY_CONNECTION_CLOSE   (1)
#define BLE_ABS_CFG_EVENT_NOTIFY_ADVERTISING_START  (1)
#define BLE_ABS_CFG_EVENT_NOTIFY_ADVERTISING_CLOSE  (1)
#define BLE_ABS_CFG_EVENT_NOTIFY_SCANNING_START     (0)
#define BLE_ABS_CFG_EVENT_NOTIFY_SCANNING_CLOSE     (0)
#define BLE_ABS_CFG_EVENT_NOTIFY_INITIATING_START   (0)
#define BLE_ABS_CFG_EVENT_NOTIFY_INITIATING_CLOSE   (0)
#define BLE_ABS_CFG_EVENT_NOTIFY_DEEP_SLEEP_START   (0)
#define BLE_ABS_CFG_EVENT_NOTIFY_DEEP_SLEEP_WAKEUP  (0)
#define BLE_ABS_CFG_RF_CLVAL                        (6)
#define BLE_ABS_CFG_RF_DCDC_CONVERTER_ENABLE        (0)
#define BLE_ABS_CFG_RF_EXT32K_EN                    (0)
#define BLE_ABS_CFG_RF_MCU_CLKOUT_PORT              (0)
#define BLE_ABS_CFG_RF_MCU_CLKOUT_FREQ              (0)
#define BLE_ABS_CFG_RF_SCA                          (0)
#define BLE_ABS_CFG_RF_MAX_TX_POW                   (0)
#define BLE_ABS_CFG_RF_DEF_TX_POW                   (0)
#define BLE_ABS_CFG_RF_CLKOUT_EN                    (0)
#define BLE_ABS_CFG_RF_DEEP_SLEEP_EN                (1)
#define BLE_ABS_CFG_MCU_MAIN_CLK_KHZ                (8000)
#define BLE_ABS_CFG_DEV_DATA_CF_BLOCK               (255)
#define BLE_ABS_CFG_DEV_DATA_DF_BLOCK               (-1)
#define BLE_ABS_CFG_GATT_MTU_SIZE                   (247)
#define BLE_ABS_CFG_TIMER_NUMBER_OF_SLOT            (10)
#define BLE_ABS_CFG_PARAM_CHECKING_ENABLE           (0)
#define BLE_CFG_RF_CONN_MAX                         (1)
#define BLE_ABS_CFG_TRANSMIT_POWER_INDEX            (34)
#define BLE_CFG_SCI_CHANNEL                         (6)
#define BLE_CFG_RESET_PORT                          F
#define BLE_CFG_RESET_PIN                           5
#define BLE_CFG_SCI_MODE_PORT                       G
#define BLE_CFG_SCI_MODE_PIN                        4

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

#endif /* R_RYZ012_RX_CONFIG_H_ */
