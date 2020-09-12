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
 * @defgroup board Board Control Library
 * @{
 * @ingroup app_lib
 * @brief Board Control Library
 * @details This library provides APIs to control LEDs and switchs on a board.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 23.08.2019 1.00    First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_ble_rx23w_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#ifndef R_BLE_BOARD_H
#define R_BLE_BOARD_H

/** @defgroup board_macro Macros 
 *  @{
 *  @brief Macro definition
 */
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief LED number.
***********************************************************************************************************************/
typedef enum
{
    BLE_BOARD_LED1, /**< LED1 */
    BLE_BOARD_LED2, /**< LED2 */

    BLE_BOARD_LED_MAX
} e_ble_led_t;

/*******************************************************************************************************************//**
 * @brief Switch number.
***********************************************************************************************************************/
typedef enum
{
    BLE_BOARD_SW1, /**< Switch1 */
    BLE_BOARD_SW2, /**< Switch2 */

    BLE_BOARD_SW_MAX
} e_ble_sw_t;
/*@}*/

/** @defgroup board_callback Callbacks 
 *  @{
 *  @brief Callback definition
 */

/*******************************************************************************************************************//**
 * @brief The callback invoked when the switch is ON.
***********************************************************************************************************************/
typedef void (*ble_sw_cb_t)(void);
/*@}*/

/** @defgroup board_func Functions
 *  @{
 *  @brief Function definition
 */
/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @brief Initialize LED and Switch related gpio pins.
***********************************************************************************************************************/
void R_BLE_BOARD_Init(void);

/*******************************************************************************************************************//**
 * @brief Set LED state (on or off).
 * @param[in] led LED number to control
 * @param[in] onoff true for light up, false for turn off
***********************************************************************************************************************/
void R_BLE_BOARD_SetLEDState(e_ble_led_t led, bool onoff);

/*******************************************************************************************************************//**
 * @brief Toggle LED state (on, off).
 * @param[in] led LED number to control
***********************************************************************************************************************/
void R_BLE_BOARD_ToggleLEDState(e_ble_led_t led);

/*******************************************************************************************************************//**
 * @brief Register switch callback.
 * @param[in] sw Switch number to control
 * @param[in] cb The callback invoked when the switch is ON.
***********************************************************************************************************************/
void R_BLE_BOARD_RegisterSwitchCb(e_ble_sw_t sw, ble_sw_cb_t cb);
/*@}*/

#endif /* R_BLE_BOARD_H */
/*@}*/
