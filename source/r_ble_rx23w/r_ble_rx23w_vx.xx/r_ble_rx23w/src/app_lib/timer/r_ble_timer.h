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
/*******************************************************************************************************************//**
 * @file
 * @defgroup timer Timer Library
 * @{
 * @ingroup app_lib
 * @brief This library provides APIs for Software Timer.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 23.08.2019 1.00    First Release
***********************************************************************************************************************/

#include "r_ble_rx23w_if.h"

#ifndef R_BLE_TIMER_H
#define R_BLE_TIMER_H


/** @defgroup timer_macro Macros 
 *  @{
 *  @brief Macro definition
 */
/** The maximum number of timer slot. */
#define BLE_TIMER_NUM_OF_SLOT (10)

/** The invalid timer handle. */
#define BLE_TIMER_INVALID_HDL (0xFF)

/** The timer type. */
typedef enum {
    BLE_TIMER_ONE_SHOT, /**< One shot timer type  */
    BLE_TIMER_PERIODIC  /**< Periodic timer type */
} e_ble_timer_type_t;
/*@}*/

/** @defgroup timer_callback Callbacks
 *  @{
 *  @brief Callback definition
 */
/** The timer callback invoked when the timer expired. */
typedef void (*ble_timer_cb_t)(uint32_t timer_hdl);
/*@}*/

/** @defgroup timer_func Functions
 *  @{
 *  @brief Function definition
 */
/******************************************************************************************************************//**
 * @brief Initialize the timer library.
 * @details This function should be called before calling any other timer library function.
 **********************************************************************************************************************/
void R_BLE_TIMER_Init(void);

/******************************************************************************************************************//**
 * @brief Terminate the timer library.
 * @details This function should be called after deleting all the created timers.
 **********************************************************************************************************************/
void R_BLE_TIMER_Terminate(void);

/******************************************************************************************************************//**
 * @brief     Create a timer.
 * @details   The timer created is in inactive (stopped), to start the timer, call @ref R_BLE_TIMER_Start.
 *            After this function return SUCCESS, one timer slot is occupied.
 *            The maximum number of timer slot is specified with BLE_TIMER_NUM_OF_SLOT.
 * @param[out] p_timer_hdl output location for the timer handle.
 * @param[in]  timeout_ms The timer timeout in milli seconds.
 * @param[in]  type The timer type (@ref e_ble_timer_type_t).
 * @param[in]  cb The callback invoked when the timer is expired.
 * @retval    BLE_SUCCESS Success 
 * @retval    BLE_ERR_INVALID_ARG Some of arguments are invalid.
 * @retval    BLE_ERR_LIMIT_EXCEEDED No timer slot is available.
 **********************************************************************************************************************/
ble_status_t R_BLE_TIMER_Create(uint32_t *p_timer_hdl, uint32_t timeout_ms, uint8_t type, ble_timer_cb_t cb);

/******************************************************************************************************************//**
 * @brief Delete the timer.
 * @details If the timer is active, the timer is stopped and deleted.
 *          After this function returns SUCCESS, the timer slot is freed.
 * @param[in,out] timer_hdl Timer handle identifying the timer to be deleted and initialize with BLE_TIMER_INVALID_HDL.
 * @retval BLE_SUCCESS Success
 * @retval BLE_ERR_INVALID_HDL The timer handle is invalid.
 **********************************************************************************************************************/
ble_status_t R_BLE_TIMER_Delete(uint32_t *p_timer_hdl);

/******************************************************************************************************************//**
 * @brief Start the timer.
 * @details After starting, the timer will be expired at the timeout specified in
 *          @ref R_BLE_TIMER_Create or @ref R_BLE_TIMER_UpdateTimeout
 * @param[in] timer_hdl Timer handle identifying the timer to be deleted.
 * @retval BLE_SUCCESS Success
 * @retval BLE_ERR_INVALID_HDL The timer handle is invalid.
 **********************************************************************************************************************/
ble_status_t R_BLE_TIMER_Start(uint32_t timer_hdl);

/******************************************************************************************************************//**
 * @brief   Stop the timer.
 * @details When the timer is stopped, the remaining time is discarded.
 *          The timer can be started by calling @ref R_BLE_TIMER_Start.
 * @param[in] timer_hdl Timer handle identifying the timer to be deleted.
 * @retval BLE_SUCCESS Success
 * @retval  BLE_ERR_INVALID_HDL The timer handle is invalid.
 **********************************************************************************************************************/
ble_status_t R_BLE_TIMER_Stop(uint32_t timer_hdl);

/******************************************************************************************************************//**
 * @brief Update timeout of the timer.
 * @details If the timer is active, the timer stop and started with new timeout.
 * @param[in] timer_hdl Timer handle identifying the timer to be deleted.
 * @param[in] timeout_ms The timeout to update in milli seconds.
 * @retval BLE_SUCCESS Success
 * @retval BLE_ERR_INVALID_HDL The timer handle is invalid.
 **********************************************************************************************************************/
ble_status_t R_BLE_TIMER_UpdateTimeout(uint32_t timer_hdl, uint32_t timeout_ms);

/******************************************************************************************************************//**
 * @brief Check whether the timer is active.
 * @retval true the timer is active (started).
 * @retval  false the timer is inactive (stopped or not created).
 **********************************************************************************************************************/
bool R_BLE_TIMER_IsActive(void);
/*@}*/

#endif /* R_BLE_TIMER_H */

/** @} */
