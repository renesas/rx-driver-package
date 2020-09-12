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
#ifndef R_BLE_ABS_API_H
#define R_BLE_ABS_API_H

/***********************************************************************************************************************
* File Name    : r_ble_abs_api.h
* Description  : Abstraction API
* History      : DD.MM.YYYY Version Description           
*              : 23.08.2019 1.00    First Release
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file
 * @defgroup abs_api Abstraction API
 * @{
 * @ingroup app_lib
 * @brief Abstraction API
 * @details This provides Abstraction API.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_ble_rx23w_if.h"


/** @defgroup abs_api_macro Macros 
 *  @{
 *  @brief Macro definition
 */

/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/

/** 
 * @def BLE_ABS_ADV_PHY_LEGACY
 * @brief Non-Connectable Legacy Advertising phy setting with @ref R_BLE_ABS_StartNonConnAdv().
 */
#define BLE_ABS_ADV_PHY_LEGACY                                          (0x00)


/******************************************************************************************************************//**
 * @enum e_r_ble_abs_adv_filt_t
 * @brief  Advertising Filter Policy
 **********************************************************************************************************************/
typedef enum
{
    /**
     *  @brief  Receive a connect request from all devices.
     */
    BLE_ABS_ADV_ALLOW_CONN_ANY   = 0x00,
    /**
     *  @brief  Receive a connect request from only the devices registered in White List.
     */
    BLE_ABS_ADV_ALLOW_CONN_WLST  = 0x01,
} e_r_ble_abs_adv_filt_t;
/*@}*/

/** @defgroup abs_api_struct Structures 
 *  @{
 *  @brief Structure definition
 */
/******************************************************************************************************************//**
 * @struct st_ble_abs_gatts_cb_param_t
 * @brief  st_ble_abs_gatts_cb_param_t includes GATT Server callback function and the priority.
 **********************************************************************************************************************/
typedef struct 
{
    /**
     *  @brief GATT Server callback function to be registered.
     */
    ble_gatts_app_cb_t cb;

    /**
     *  @brief The priority of the callback function. \n
     *         Valid range is 1 <= priority <= @ref BLE_GATTS_MAX_CB. \n
     *         A lower priority number means a higher priority level.
     */
    uint8_t priority;

} st_ble_abs_gatts_cb_param_t;

/******************************************************************************************************************//**
 * @struct st_ble_abs_gatts_init_param_t
 * @brief  st_ble_abs_gatts_init_param_t includes GATT Server callback functions.
 **********************************************************************************************************************/
typedef struct 
{
    /**
     *  @brief The array of the GATT Server callback to be registered.
     */
    st_ble_abs_gatts_cb_param_t * p_cb_param;

    /**
     *  @brief The number of the callback functions provided by the p_cb_param parameter. \n
     *  Valid range is 1 <= cb_num <= @ref BLE_GATTS_MAX_CB.
     */
    uint8_t cb_num;

} st_ble_abs_gatts_init_param_t;

/******************************************************************************************************************//**
 * @struct st_ble_abs_gattc_cb_param_t
 * @brief  st_ble_abs_gattc_cb_param_t includes GATT Client callback function and the priority.
 **********************************************************************************************************************/
typedef struct 
{
    /**
     *  @brief GATT Client callback function to be registered.
     */
    ble_gattc_app_cb_t cb;

    /**
     *  @brief The priority of the callback function. \n
     *         Valid range is 1 <= priority <= @ref BLE_GATTC_MAX_CB. \n
     *         A lower priority number means a higher priority level. 
     */
    uint8_t priority;

} st_ble_abs_gattc_cb_param_t;

/******************************************************************************************************************//**
 * @struct st_ble_abs_gattc_init_param_t
 * @brief  st_ble_abs_gattc_init_param_t includes GATT Client callback functions.
 **********************************************************************************************************************/
typedef struct 
{
    /**
     *  @brief The array of the GATT Client callback to be registered.
     */
    st_ble_abs_gattc_cb_param_t * p_cb_param;

    /**
     *  @brief The number of the GATT Client callback functions used in an application. \n
        Valid range is 1 <= cb_num <= @ref BLE_GATTC_MAX_CB. \n
        The GATT Discovery Library(app_lib/discovery) registers one callback. \n
        Therefore, set the number of the callback included in the p_cb_param plus one to the cb_num field.
     */
    uint8_t cb_num;

} st_ble_abs_gattc_init_param_t;

/******************************************************************************************************************//**
 * @struct st_ble_abs_pairing_param_t
 * @brief  st_ble_abs_pairing_param_t includes the pairing parameters.
 **********************************************************************************************************************/
typedef struct 
{
    /**
     *  @brief 	IO capabilities of local device. \n
     *  Select one of the following.\n
     *  |              macro                   |   description                                                                                                                                     | 
     *  |:------------------------------------ |:------------------------------------------------------------------------------------------------------------------------------------------------- |
     *  | BLE_GAP_IOCAP_DISPLAY_ONLY(0x00)     | Output : Local device has the ability to display a 6 digit decimal number. \n Input : None                                                        | 
     *  | BLE_GAP_IOCAP_DISPLAY_YESNO(0x01)    | Output : Local device has the ability to display a 6 digit decimal number. \n Input : Local device has the ability to indicate 'yes' or 'no'      |
     *  | BLE_GAP_IOCAP_KEYBOARD_ONLY(0x02)    | Output : None. \n Input : Local device has the ability to input the number '0' - '9'.                                                             |
     *  | BLE_GAP_IOCAP_NOINPUT_NOOUTPUT(0x03) | Output : None. \n Input : None.                                                                                                                   |
     *  | BLE_GAP_IOCAP_KEYBOARD_DISPLAY(0x04) | Output : Local device has the ability to display a 6 digit decimal number. \n Input : Local device has the ability to input the number '0' - '9'. |
     */
    uint8_t iocap;

    /**
     *  @brief 	MITM protection policy.\n
     *  Select one of the following.
     *    |  macro                                 |   description                                | 
     *    |:---------------------------------------|:-------------------------------------------- |
     *    | BLE_GAP_SEC_MITM_BEST_EFFORT(0x00)     | MITM Protection not required.                | 
     *    | BLE_GAP_SEC_MITM_STRICT(0x01)          | MITM Protection required.                    | 
     */
    uint8_t mitm;

    /**
     *  @brief Determine whether to accept only Secure Connections or not.
     *    |  macro                                 |   description                                  | 
     *    |:---------------------------------------|:---------------------------------------------- |
     *    | BLE_GAP_SC_BEST_EFFORT(0x00)           | Accept Legacy pairing and Secure Connections.  | 
     *    | BLE_GAP_SC_STRICT(0x01)                | Accept only Secure Connections.                | 
     */
    uint8_t sec_conn_only;

    /**
     *  @brief Type of keys to be distributed from local device. \n
     *         The loc_key_dist field is set to a bitwise OR of the following values.
     *    |  macro                                 |   description                                  | 
     *    |:---------------------------------------|:---------------------------------------------- |
     *    | BLE_GAP_KEY_DIST_ENCKEY(0x01)          | LTK                                            |
     *    | BLE_GAP_KEY_DIST_IDKEY(0x02)           | IRK and Identity Address                       |
     *    | BLE_GAP_KEY_DIST_SIGNKEY(0x04)         | CSRK                                           |
     */
    uint8_t loc_key_dist;

    /**
     *  @brief Type of keys which local device requests a remote device to distribute.
     *    |  macro                                 |   description                                                                       | 
     *    |:---------------------------------------|:----------------------------------------------------------------------------------- |
     *    | BLE_GAP_KEY_DIST_ENCKEY(0x01)          | LTK. \n In case of Secure Connections, LTK is notified even if this bit is not set. |
     *    | BLE_GAP_KEY_DIST_IDKEY(0x02)           | IRK and Identity Address                                                            |
     *    | BLE_GAP_KEY_DIST_SIGNKEY(0x04)         | CSRK                                                                                |
     */
    uint8_t rem_key_dist;

    /**
     *  @brief The maximum LTK size(byte) to be requested to a remote device. Valid range is 0x07-0x10. \n
     *         When the LTK size of a remote device is less than max_key_size, the pairing fails. \n
     *         The LTK size of the local device is 16 bytes.
     */
    uint8_t max_key_size;

    /**
     *  @brief padding
     */
    uint8_t padding[2];

} st_ble_abs_pairing_param_t;

/******************************************************************************************************************//**
 * @struct st_ble_abs_init_param_t
 * @brief  st_ble_abs_init_param_t is the parameters for initialization.
 **********************************************************************************************************************/
typedef struct 
{
    /**
     *  @brief GAP callback function to be registered. \n
     *         Note : Host stack initialization requires to register the GAP callback function. 
     */
    ble_gap_app_cb_t gap_cb;

    /**
     *  @brief GATT Server callback functions to be registered. \n
     *         If GATT Server event notification is not needed, specify NULL to the p_gatts_cbs.
     */
    st_ble_abs_gatts_init_param_t * p_gatts_cbs;

    /**
     *  @brief GATT Client callback functions to be registered. \n
     *         If GATT Client event notification is not needed, specify NULL to the p_gattc_cbs.
     */
    st_ble_abs_gattc_init_param_t * p_gattc_cbs;

    /**
     *  @brief Vendor Specific callback function to be registered. \n
     *         If Vendor Specific event notification is not needed, specify NULL to the vs_cb.
     */
    ble_vs_app_cb_t vs_cb;

    /**
     *  @brief Pairing parameters. \n
     *         If pairing parameter configuration is not needed, specify NULL to the p_pair_param.
     */
    st_ble_abs_pairing_param_t * p_pairing_param;

} st_ble_abs_init_param_t;

/******************************************************************************************************************//**
 * @struct st_ble_abs_legacy_adv_param_t
 * @brief  st_ble_abs_legacy_adv_param_t is the parameters for legacy advertising.
 **********************************************************************************************************************/
typedef struct 
{
    /**
     *  @brief The remote device address.\n
     *  If the p_addr parameter is not NULL, Direct Connectable Advertising is performed to the remote address. \n
     *  If the p_addr parameter is NULL, Undirect Connectable Advertising is performed according to \n
     *  the advertising filter policy specified by the filter parameter.
     */
    st_ble_dev_addr_t * p_addr;

    /**
     *  @brief Advertising Data. \n
     *  If the p_adv_data is specified as NULL, Advertising Data is not included in the advertising PDU.
     */
    uint8_t * p_adv_data;

    /**
     *  @brief Scan Response Data. \n
     *  If the p_sres_data is specified as NULL, Scan Response Data is not included in the advertising PDU.
     */
    uint8_t * p_sres_data;

    /**
     *  @brief 	Advertising with the fast_adv_intv parameter continues for the period specified \n
     *  by the fast_period parameter.\n
     *  Time(ms) = fast_adv_intv * 0.625. \n
     *  If the fast_period parameter is 0, this parameter is ignored.\n
     *  Valid range is 0x00000020 - 0x00FFFFFF.
     */
    uint32_t fast_adv_intv;

    /**
     *  @brief After the elapse of the fast_period, advertising with the slow_adv_intv parameter continues \n
     *  for the period specified by the slow_period parameter.\n
     *  Time(ms) = fast_adv_intv * 0.625. \n
     *  If the fast_period parameter is 0, this parameter is ignored.\n
     *  Valid range is 0x00000020 - 0x00FFFFFF.
     */
    uint32_t slow_adv_intv;

    /**
     *  @brief The period which advertising with the fast_adv_intv parameter continues for. \n
     *  Time = duration * 10ms.\n
     *  After the elapse of the fast_period, @ref BLE_GAP_EVENT_ADV_OFF event notifies that the advertising has stopped.\n
     *  Valid range is 0x0000 - 0xFFFF. \n
     *  If the fast_period parameter is 0x0000, advertising with the fast_adv_intv parameter is not performed.
     */
    uint16_t fast_period;

    /**
     *  @brief The period which advertising with the slow_adv_intv parameter continues for. Time = duration * 10ms. \n
     *  After the elapse of the slow_period, @ref BLE_GAP_EVENT_ADV_OFF event notifies that the advertising has stopped. \n
     *  Valid range is 0x0000 - 0xFFFF. \n
     *  If the slow_period parameter is 0x0000, the advertising continues.
     */
    uint16_t slow_period;

    /**
     *  @brief Advertising data length(byte). \n
     *  Valid range is 0-31.  \n
     *  If the adv_data_length is 0, Advertising Data is not included in the advertising PDU.
     */
    uint16_t adv_data_length;

    /**
     *  @brief Scan response data length (in bytes). \n
     *  Scan Response Data(byte). \n
     *  Valid range is 0-31. \n
     *  If the sres_data_length is 0, Scan Response Data is not included in the advertising PDU.
     */
    uint16_t sres_data_length;

    /**
     *  @brief The channel map used for the advertising packet transmission. \n
     *  It is a bitwise OR of the following values.\n
     *    |  macro                    |   description   | 
     *    |:--------------------------|:--------------- |
     *    | BLE_GAP_ADV_CH_37(0x01)   | Use 37 CH.      | 
     *    | BLE_GAP_ADV_CH_38(0x02)   | Use 38 CH.      | 
     *    | BLE_GAP_ADV_CH_39(0x04)   | Use 38 CH.      | 
     *    | BLE_GAP_ADV_CH_ALL(0x07)  | Use 37 - 39 CH. | 
     */
    uint8_t adv_ch_map;

    /**
     *  @brief Advertising filter policy. \n
     *  If the p_addr parameter is NULL, the advertising is performed according to the advertising filter policy. \n
     *  If the p_addr parameter is not NULL, this parameter is ignored. \n
     *    |  macro                              |   description                                                             | 
     *    |:------------------------------------|:------------------------------------------------------------------------- |
     *    | BLE_ABS_ADV_ALLOW_CONN_ANY(0x00)    | Process scan and connection requests from all devices.                    | 
     *    | BLE_ABS_ADV_ALLOW_CONN_WLST(0x01)   | Process scan and connection requests from only devices in the White List. | 
     */
    uint8_t filter;

    /**
     *  @brief 	Own Bluetooth address type. \n Select one of the following.
     *    |  macro                              |   description                                                                                                                                                  | 
     *    |:------------------------------------|:-------------------------------------------------------------------------------------------------------------------------------------------------------------- |
     *    | BLE_GAP_ADDR_PUBLIC(0x00)           | Public Address                                                                                                                                                 | 
     *    | BLE_GAP_ADDR_RAND(0x01)             | Random Address                                                                                                                                                 | 
     *    | BLE_GAP_ADDR_RPA_ID_PUBLIC(0x02)    | Resolvable Private Address. \n If the IRK of local device has not been registered in Resolving List, public address is used.                                   | 
     *    | BLE_GAP_ADDR_RPA_ID_RANDOM(0x03)    | Resolvable Private Address. \n If the IRK of local device has not been registered in Resolving List, the random address specified by the o_addr field is used. | 
     */
    uint8_t o_addr_type;

    /**
     *  @brief 	Random address set to the advertising set, when the o_addr_type field is BLE_GAP_ADDR_RAND or BLE_GAP_ADDR_RPA_ID_RANDOM.\n
     *          When the o_addr_type field is BLE_GAP_ADDR_PUBLIC or BLE_GAP_ADDR_RPA_ID_PUBLIC, this field is ignored. 
     *  @note   If the "Balance" or "Compact" library is used, this field is ignored.
     *          Instead, set the random address (static address or non-resolvable private address) 
     *          by R_BLE_GAP_SetRandAddr() or R_BLE_VS_SetBdAddr(). 
     */
    uint8_t  o_addr[6];

    /**
     *  @brief padding
     */
    uint8_t padding[3];

} st_ble_abs_legacy_adv_param_t;


/******************************************************************************************************************//**
 * @struct st_ble_abs_ext_adv_param_t
 * @brief  st_ble_abs_ext_adv_param_t is the parameters for extended advertising.
 **********************************************************************************************************************/
typedef struct 
{
    /**
     *  @brief The remote device address. \n
     *  If the p_addr parameter is not NULL, Direct Connectable Advertising is performed to the remote address. \n
     *  If the p_addr parameter is NULL, Undirect Connectable Advertising is performed \n
     *  according to the advertising filter policy specified by the filter parameter.
     */
    st_ble_dev_addr_t * p_addr;

    /**
     *  @brief Advertising data. If p_adv_data is specified as NULL, advertising data is not set.
     */
    uint8_t * p_adv_data;

    /**
     *  @brief Advertising with the fast_adv_intv parameter continues for \n 
     *  the period specified by the fast_period parameter. \n
     *  Time(ms) = fast_adv_intv * 0.625. \n
     *  If the fast_period parameter is 0, this parameter is ignored. \n
     *  Valid range is 0x00000020 - 0x00FFFFFF.
     */
    uint32_t fast_adv_intv;

    /**
     *  @brief After the elapse of the fast_period, advertising with the slow_adv_intv parameter \n 
     *  continues for the period specified by the slow_period parameter. \n
     *  Time(ms) = fast_adv_intv * 0.625. \n
     *  If the fast_period parameter is 0, this parameter is ignored. \n
     *  Valid range is 0x00000020 - 0x00FFFFFF.
     */
    uint32_t slow_adv_intv;

    /**
     *  @brief The period which advertising with the fast_adv_intv parameter continues for. \n
     *  Time = duration * 10ms. \n
     *  After the elapse of the fast_period, @ref BLE_GAP_EVENT_ADV_OFF event notifies that the advertising has stopped. \n
     *  Valid range is 0x0000 - 0xFFFF. \n
     *  If the fast_period parameter is 0x0000, the fast_intv parameter is ignored. 
     */
    uint16_t fast_period;

    /**
     *  @brief The period which advertising with the slow_adv_intv parameter continues for. \n
     *  Time = duration * 10ms. \n
     *  After the elapse of the slow_period, @ref BLE_GAP_EVENT_ADV_OFF event notifies that the advertising has stopped. \n
     *  Valid range is 0x0000 - 0xFFFF. \n
     *  If the slow_period parameter is 0x0000, the advertising continues.
     */
    uint16_t slow_period;

    /**
     *  @brief Advertising data length (in bytes). \n
     *  Valid range is 0-229. \n
     *  If the adv_data_length is 0, Advertising Data is not included in the advertising PDU.
     */
    uint16_t adv_data_length;

    /**
     *  @brief The channel map used for the advertising packet transmission. \n
     *  It is a bitwise OR of the following values.
     *    |  macro                    |   description   | 
     *    |:--------------------------|:--------------- |
     *    | BLE_GAP_ADV_CH_37(0x01)   | Use 37 CH.      | 
     *    | BLE_GAP_ADV_CH_38(0x02)   | Use 38 CH.      | 
     *    | BLE_GAP_ADV_CH_39(0x04)   | Use 38 CH.      | 
     *    | BLE_GAP_ADV_CH_ALL(0x07)  | Use 37 - 39 CH. | 
     */
    uint8_t adv_ch_map;

    /**
     *  @brief Advertising filter policy. \n
     *  If the p_addr parameter is NULL, the advertising is performed according to the advertising filter policy. \n
     *  If the p_addr parameter is not NULL, this parameter is ignored. \n
     *    |  macro                              |   description                                                             | 
     *    |:------------------------------------|:------------------------------------------------------------------------- |
     *    | BLE_ABS_ADV_ALLOW_CONN_ANY(0x00)    | Process scan and connection requests from all devices.                    | 
     *    | BLE_ABS_ADV_ALLOW_CONN_WLST(0x01)   | Process scan and connection requests from only devices in the White List. | 
     */
    uint8_t filter;

    /**
     *  @brief 	Own Bluetooth address type. Select one of the following. \n
     *    |  macro                              |   description                                                                                                                                                  | 
     *    |:------------------------------------|:-------------------------------------------------------------------------------------------------------------------------------------------------------------- |
     *    | BLE_GAP_ADDR_PUBLIC(0x00)           | Public Address                                                                                                                                                 | 
     *    | BLE_GAP_ADDR_RAND(0x01)             | Random Address                                                                                                                                                 | 
     *    | BLE_GAP_ADDR_RPA_ID_PUBLIC(0x02)    | Resolvable Private Address. \n If the IRK of local device has not been registered in Resolving List, public address is used.                                   | 
     *    | BLE_GAP_ADDR_RPA_ID_RANDOM(0x03)    | Resolvable Private Address. \n If the IRK of local device has not been registered in Resolving List, the random address specified by the o_addr field is used. | 
     */
    uint8_t o_addr_type;

    /**
     *  @brief Primary ADV PHY. \n
     *  In this parameter, only 1M PHY and Coded PHY can be specified, and 2M PHY cannot be specified. \n
     *    |  macro                   |   description                                                                                                                                        | 
     *    |:-------------------------|:---------------------------------------------------------------------------------------------------------------------------------------------------- |
     *    | BLE_GAP_ADV_PHY_1M(0x01) | Use 1M PHY as Primary Advertising PHY. \n When the adv_prop_type field is Legacy Advertising PDU type, this field shall be set to BLE_GAP_ADV_PHY_1M.| 
     *    | BLE_GAP_ADV_PHY_CD(0x03) | Use Coded PHY as Primary Advertising PHY. \n Coding scheme is configured by @ref R_BLE_VS_SetCodingScheme().                                         | 
     * 
     */
    uint8_t adv_phy;

    /**
     *  @brief 	Secondary ADV Phy. Select one of the following.
     *    |  macro                     |   description                                                                                                       | 
     *    |:---------------------------|:------------------------------------------------------------------------------------------------------------------- |
     *    | BLE_GAP_ADV_PHY_1M(0x01)   | Use 1M PHY as Secondary Advertising PHY.                                                                            | 
     *    | BLE_GAP_ADV_PHY_2M(0x02)   | Use 2M PHY as Secondary Advertising PHY.                                                                            | 
     *    | BLE_GAP_ADV_PHY_CD(0x03)   | Use Coded PHY(S=8) as Secondary Advertising PHY. \n Coding scheme is configured by @ref R_BLE_VS_SetCodingScheme(). |
     */
    uint8_t sec_adv_phy;

    /**
     *  @brief 	Random address set to the advertising set, when the o_addr_type field is BLE_GAP_ADDR_RAND or BLE_GAP_ADDR_RPA_ID_RANDOM.\n
     *          When the o_addr_type field is BLE_GAP_ADDR_PUBLIC or BLE_GAP_ADDR_RPA_ID_PUBLIC, this field is ignored. 
     *  @note   If the "Balance" or "Compact" library is used, this field is ignored.
     *          Instead, set the random address (static address or non-resolvable private address) 
     *          by R_BLE_GAP_SetRandAddr() or R_BLE_VS_SetBdAddr(). 
     */
    uint8_t  o_addr[6];

    /**
     *  @brief padding
     */
    uint8_t padding[3];

} st_ble_abs_ext_adv_param_t;

/******************************************************************************************************************//**
 * @struct st_ble_abs_non_conn_adv_param_t
 * @brief  st_ble_abs_non_conn_adv_param_t is the parameters for non-connectable advertising.
 **********************************************************************************************************************/
typedef struct 
{
    /**
     *  @brief The remote device address. \n
     *  If the p_addr parameter is not NULL, Direct Connectable Advertising is performed to the remote address. \n
     *  If the p_addr parameter is NULL, Undirect Connectable Advertising is performed \n
     *  according to the advertising filter policy specified by the filter parameter.
     */
    st_ble_dev_addr_t * p_addr;

    /**
     *  @brief Advertising data. If p_adv_data is specified as NULL, advertising data is not set.
     */
    uint8_t * p_adv_data;

    /**
     *  @brief Advertising with the adv_intv parameter continues for the period specified by the duration parameter.\n
     *  Time(ms) = fast_adv_intv * 0.625. \n
     *  If the duration parameter is 0x0000, the advertising with the adv_intv parameter continue. \n
     *  Valid range is 0x00000020 - 0x00FFFFFF.
     */
    uint32_t adv_intv;

    /**
     *  @brief The period which advertising with the adv_intv parameter continues for. \n
     *  Time = duration * 10ms.\n
     *  After the elapse of the duration, @ref BLE_GAP_EVENT_ADV_OFF event notifies that the advertising has stopped. \n
     *  Valid range is 0x0000 - 0xFFFF. \n
     *  If the duration parameter is 0x0000, the advertising continues.
     */
    uint16_t duration;

    /**
     *  @brief Advertising data length (in bytes).\n
     *  If the adv_phy parameter is @ref BLE_ABS_ADV_PHY_LEGACY(0x00), the valid range is 0-31. \n
     *  If the adv_phy parameter is the other values, the valid range is 0-1650. \n
     *  If the adv_data_length parameter is 0, Advertising Data is not included in the advertising PDU.
     */
    uint16_t adv_data_length;

    /**
     *  @brief The channel map used for the advertising packet transmission. \n
     *  It is a bitwise OR of the following values.
     *    |  macro                    |   description   | 
     *    |:--------------------------|:--------------- |
     *    | BLE_GAP_ADV_CH_37(0x01)   | Use 37 CH.      | 
     *    | BLE_GAP_ADV_CH_38(0x02)   | Use 38 CH.      | 
     *    | BLE_GAP_ADV_CH_39(0x04)   | Use 38 CH.      | 
     *    | BLE_GAP_ADV_CH_ALL(0x07)  | Use 37 - 39 CH. | 
     */
    uint8_t adv_ch_map;

    /**
     *  @brief 	Own Bluetooth address type. Select one of the following. \n
     *    |  macro                              |   description                                                                                                                                                  | 
     *    |:------------------------------------|:-------------------------------------------------------------------------------------------------------------------------------------------------------------- |
     *    | BLE_GAP_ADDR_PUBLIC(0x00)           | Public Address                                                                                                                                                 | 
     *    | BLE_GAP_ADDR_RAND(0x01)             | Random Address                                                                                                                                                 | 
     *    | BLE_GAP_ADDR_RPA_ID_PUBLIC(0x02)    | Resolvable Private Address. \n If the IRK of local device has not been registered in Resolving List, public address is used.                                   | 
     *    | BLE_GAP_ADDR_RPA_ID_RANDOM(0x03)    | Resolvable Private Address. \n If the IRK of local device has not been registered in Resolving List, the random address specified by the o_addr field is used. | 
     */
    uint8_t o_addr_type;

    /**
     *  @brief Primary ADV PHY. \n
     *  In this parameter, only 1M PHY and Coded PHY can be specified, and 2M PHY cannot be specified. \n
     *    |  macro                       |   description                                                                                                                                                             | 
     *    |:-----------------------------|:------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
     *    | BLE_ABS_ADV_PHY_LEGACY(0x00) | Use 1M PHY as Primary Advertising PHY for Non-Connectable Legacy Advertising. \n If Periodic Advertising is performed, this value shall not set to the adv_phy parameter. | 
     *    | BLE_GAP_ADV_PHY_1M(0x01)     | Use 1M PHY as Primary Advertising PHY. \n When the adv_prop_type field is Legacy Advertising PDU type, this field shall be set to BLE_GAP_ADV_PHY_1M.                     | 
     *    | BLE_GAP_ADV_PHY_CD(0x03)     | Use Coded PHY as Primary Advertising PHY. \n Coding scheme is configured by @ref R_BLE_VS_SetCodingScheme().                                                              | 
     */
    uint8_t adv_phy;

    /**
     *  @brief 	Secondary ADV Phy. Select one of the following.
     *    |  macro                     |   description                                                                                                       | 
     *    |:---------------------------|:------------------------------------------------------------------------------------------------------------------- |
     *    | BLE_GAP_ADV_PHY_1M(0x01)   | Use 1M PHY as Secondary Advertising PHY.                                                                            | 
     *    | BLE_GAP_ADV_PHY_2M(0x02)   | Use 2M PHY as Secondary Advertising PHY.                                                                            | 
     *    | BLE_GAP_ADV_PHY_CD(0x03)   | Use Coded PHY(S=8) as Secondary Advertising PHY. \n Coding scheme is configured by @ref R_BLE_VS_SetCodingScheme(). |
     */
    uint8_t sec_adv_phy;

    /**
     *  @brief 	Random address set to the advertising set, when the o_addr_type field is BLE_GAP_ADDR_RAND or BLE_GAP_ADDR_RPA_ID_RANDOM.\n
     *          When the o_addr_type field is BLE_GAP_ADDR_PUBLIC or BLE_GAP_ADDR_RPA_ID_PUBLIC, this field is ignored. 
     *  @note   If the "Balance" or "Compact" library is used, this field is ignored.
     *          Instead, set the random address (static address or non-resolvable private address) 
     *          by R_BLE_GAP_SetRandAddr() or R_BLE_VS_SetBdAddr(). 
     */
    uint8_t  o_addr[6];

    /**
     *  @brief padding
     */
    uint8_t padding[2];

} st_ble_abs_non_conn_adv_param_t;


/******************************************************************************************************************//**
 * @struct st_ble_abs_perd_adv_param_t
 * @brief  st_ble_abs_perd_adv_param_t is the parameters for periodic advertising.
 **********************************************************************************************************************/
typedef struct 
{
    /**
     *  @brief Advertising parameters.
     */
    st_ble_abs_non_conn_adv_param_t param;

    /**
     *  @brief Periodic advertising data. If p_perd_adv_data is specified as NULL, periodic advertising data is not set.
     */
    uint8_t * p_perd_adv_data;

    /**
     *  @brief Periodic advertising interval. \n 
     *  Time(ms) = perd_intv_min * 1.25. \n 
     *  Valid range is 0x0006 - 0xFFFF.
     */
    uint16_t perd_intv;

    /**
     *  @brief Periodic advertising data length (in bytes). \n
     *  Valid range is 0 - 1650. \n
     *  If the perd_adv_data_length is 0, Periodic Advertising Data is not included in the advertising PDU.
     */
    uint16_t perd_adv_data_length;

} st_ble_abs_perd_adv_param_t;

/******************************************************************************************************************//**
 * @struct st_ble_abs_scan_phy_param_t
 * @brief  st_ble_abs_scan_phy_param_t is the phy parameters for scan.
 **********************************************************************************************************************/
typedef struct 
{
    /**
     *  @brief Fast scan interval. \n 
     *  Interval(ms) = fast_intv * 0.625. \n
     *  Valid range is 0x0004 - 0xFFFF.
     *  
     */
    uint16_t fast_intv;

    /**
     *  @brief Slow Scan interval. \n
     *  Slow Scan interval(ms) = slow_intv * 0.625. \n
     *  Valid range is 0x0004 - 0xFFFF.
     */
    uint16_t slow_intv;

    /**
     *  @brief Fast Scan window. \n
     *  Fast Scan window(ms) = fast_window * 0.625. \n
     *  Valid range is 0x0004 - 0xFFFF.
     */
    uint16_t fast_window;

    /**
     *  @brief Slow Scan window. \n
     *  Slow Scan window(ms) = slow_window * 0.625. \n
     *  Valid range is 0x0004 - 0xFFFF.
     */
    uint16_t slow_window;

    /**
     *  @brief Scan type.
     *    |  macro                       |   description   | 
     *    |:-----------------------------|:--------------- |
     *    | BLE_GAP_SCAN_PASSIVE(0x00)   | Passive Scan.   | 
     *    | BLE_GAP_SCAN_ACTIVE(0x01)    | Active Scan.    | 
     */
    uint8_t scan_type;

    /**
     *  @brief padding.
     */
    uint8_t padding[3];

} st_ble_abs_scan_phy_param_t;

/******************************************************************************************************************//**
 * @struct st_ble_abs_scan_param_t
 * @brief  st_ble_abs_scan_param_t is the parameters for scan.
 **********************************************************************************************************************/
typedef struct 
{
    /**
     *  @brief Scan parameters for receiving the advertising packets in 1M PHY. \n
     *  In case of not receiving the advertising packets in 1M PHY, this field is specified as NULL. \n
     *  p_phy_param_1M or p_phy_param_coded field shall be set to scan parameters.
     */
    st_ble_abs_scan_phy_param_t * p_phy_param_1M;

    /**
     *  @brief Scan parameters for receiving the advertising packets in Coded PHY. \n
     *  In case of not receiving the advertising packets in Coded PHY, this field is specified as NULL. \n
     *  p_phy_param_1M or p_phy_param_coded field shall be set to scan parameters.
     */
    st_ble_abs_scan_phy_param_t * p_phy_param_coded;

    /**
     *  @brief Data for Advertising Data filtering. \n
     *  The p_filter_data parameter is used for the advertising data in single advertising report. \n
     *  The advertising data composed of multiple advertising reports is not filtered by this parameter. \n
     *  If the p_filter_data parameter is specified as NULL, the filtering is not done.
     */
    uint8_t * p_filter_data;

    /**
     *  @brief The period which scan with the fast scan interval/fast scan window continues for. \n
     *  Time(ms) = fast_period * 10. \n
     *  Valid range is 0x0000 - 0xFFFF. \n
     *  If the fast_period parameter is 0x0000, scan with the fast scan interval/fast scan window is not performed. \n
     *  After the elapse of the fast_period, @ref BLE_GAP_EVENT_SCAN_TO event notifies that the scan has stopped.
     */
    uint16_t fast_period;

    /**
     *  @brief The period which scan with the slow scan interval/slow scan window continues for. \n
     *  Time = slow_period * 10ms. \n
     *  Valid range is 0x0000 - 0xFFFF. \n
     *  If the slow_period parameter is 0x0000, the scan continues. \n
     *  After the elapse of the slow_period, @ref BLE_GAP_EVENT_SCAN_TO event notifies that the scan has stopped.
     */
    uint16_t slow_period;

    /**
     *  @brief The length of the data specified by the p_filter_data parameter. \n
     *  Valid range is 0x0000-0x0010. \n
     *  If the filter_data_length parameter is 0, the filtering is not done.
     */
    uint16_t filter_data_length;

    /**
     *  @brief Scan Filter Policy. Select one of the following.
     *    |  macro                                            |   description                                                                                                                                                                                                                                                                                                                                                                                                               | 
     *    |:--------------------------------------------------|:--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
     *    | BLE_GAP_SCAN_ALLOW_ADV_ALL(0x00)                  | Accept all advertising and scan response PDUs except directed advertising PDUs not addressed to local device.                                                                                                                                                                                                                                                                                                               | 
     *    | BLE_GAP_SCAN_ALLOW_ADV_WLST(0x01)                 | Accept only advertising and scan response PDUs from remote devices whose address is registered in the White List. \n Directed advertising PDUs which are not addressed to local device is ignored.                                                                                                                                                                                                                          |
     *    | BLE_GAP_SCAN_ALLOW_ADV_EXCEPT_DIRECTED(0x02)      | Accept all advertising and scan response PDUs except directed advertising PDUs whose the target address is identity address but doesn't address local device. However directed advertising PDUs whose the target address is the local resolvable private address are accepted.                                                                                                                                              | 
     *    | BLE_GAP_SCAN_ALLOW_ADV_EXCEPT_DIRECTED_WLST(0x03) | Accept all advertising and scan response PDUs. The following are excluded.  \n \arg Advertising and scan response PDUs where the advertiser's identity address is not in the White List. \n \arg Directed advertising PDUs whose the target address is identity address but doesn't address local device. However directed advertising PDUs whose the target address is the local resolvable private address are accepted.  | 
     */
    uint8_t dev_filter;

    /**
     *  @brief Filter duplicates. \n
     *  Maximum number of filtered devices is 8. \n
     *  The 9th and subsequent devices are not filtered by this parameter.  \n
     *    |  macro                                            |   description                                            | 
     *    |:--------------------------------------------------|:-------------------------------------------------------- |
     *    | BLE_GAP_SCAN_FILT_DUPLIC_DISABLE(0x00)            | Duplicate filter disabled.                               | 
     *    | BLE_GAP_SCAN_FILT_DUPLIC_ENABLE(0x01)             | Duplicate filter enabled.                                | 
     *    | BLE_GAP_SCAN_FILT_DUPLIC_ENABLE_FOR_PERIOD(0x02)) | Duplicate filtering enabled, reset for each scan period. | 
     * 
     */
    uint8_t filter_dups;

    /**
     *  @brief The AD type of the data specified by the p_filter_data parameter.\n
     *  The AD type identifier values are defined in Bluetooth SIG Assigned Number \n
     *  (https://www.bluetooth.com/specifications/assigned-numbers).\n
     *  If this parameter is set to 0, the scanner device retrieves an AD Data having a data forward coinciding 
     *  with the data specified by the p_filter_data parameter.
     */
    uint8_t filter_ad_type;

    /**
     *  @brief Padding
     */
    uint8_t padding[3];

} st_ble_abs_scan_param_t;


/******************************************************************************************************************//**
 * @struct st_ble_abs_conn_phy_param_t
 * @brief  st_ble_abs_conn_phy_param_t is the phy parameters for create connection.
 **********************************************************************************************************************/
typedef struct 
{
    /**
     *  @brief Connection interval. \n
     *  Time(ms) = conn_intv_min * 1.25. \n
     *  Valid range is 0x0006 - 0x0C80.
     */
    uint16_t conn_intv;

    /**
     *  @brief Slave latency. \n 
     *  Valid range is 0x0000 - 0x01F3.
     */
    uint16_t conn_latency;

    /**
     *  @brief Supervision timeout. \n
     *  Time(ms) = sup_to * 10. \n
     *  Valid range is 0x000A - 0x0C80.
     */
    uint16_t sup_to;

    /**
     *  @brief Padding
     */
    uint8_t padding[2];

} st_ble_abs_conn_phy_param_t;


/******************************************************************************************************************//**
 * @struct st_ble_abs_conn_param_t
 * @brief  st_ble_abs_conn_param_t is the parameters for create connection.
 **********************************************************************************************************************/
typedef struct 
{
    /**
     *  @brief Connection interval, slave latency, supervision timeout for 1M PHY. \n
     *  The p_conn_1M is specified as NULL, a connection request is not sent with 1M PHY.
     */
    st_ble_abs_conn_phy_param_t * p_conn_1M;

    /**
     *  @brief Connection interval, slave latency, supervision timeout for 2M PHY. \n 
     *  The p_conn_2M is specified as NULL, a connection request is not sent with 2M PHY. 
     */
    st_ble_abs_conn_phy_param_t * p_conn_2M;

    /**
     *  @brief Connection interval, slave latency, supervision timeout for Coded PHY.  \n
     *  The p_conn_coded is specified as NULL, a connection request is not sent with Coded PHY.
     */
    st_ble_abs_conn_phy_param_t * p_conn_coded;

    /**
     *  @brief Address of the device to be connected. \n 
     *  If the filter field is @ref BLE_GAP_INIT_FILT_USE_WLST(0x01), this parameter is ignored.
     */
    st_ble_dev_addr_t * p_addr;

    /**
     *  @brief The filter field specifies whether the White List is used or not, when connecting with a remote device.\n
     *    |  macro                           |   description                                                                                                                      | 
     *    |:---------------------------------|:---------------------------------------------------------------------------------------------------------------------------------- |
     *    | BLE_GAP_INIT_FILT_USE_ADDR(0x00) | White List is not used. \n The remote device to be connected is specified by the p_addr field is used.                             | 
     *    | BLE_GAP_INIT_FILT_USE_WLST(0x01) | White List is used. \n The remote device registered in White List is connected with local device. \n The p_addr field is ignored.  |
     */
    uint8_t filter;

    /**
     *  @brief The time(sec) to cancel the create connection request. \n
     *  Valid range is 0 <= conn_to <= 10. \n
     *  If the conn_to field is 0, the create connection request is not canceled. \n
     */
    uint8_t conn_to;

    /**
     *  @brief Padding
     */
    uint8_t padding[2];

} st_ble_abs_conn_param_t;
/*@}*/



/* ============================================== ABS API Declarations ============================================== */
/** @defgroup abs_api_func Functions 
 *  @{
 *  @brief Function definition
 */
#ifdef __cplusplus
extern "C" {
#endif



/******************************************************************************************************************//**
 * @fn ble_status_t R_BLE_ABS_Init(st_ble_abs_init_param_t * p_init_param)
 * @brief   Initialize the Host Stack. 
 * @details Host stack is initialized with this function. Before using All the R_BLE APIs, 
 *          it's necessary to call this function. A callback functions are registered with this function. 
 *          In order to receive the GAP, GATT, Vendor specific event, it's necessary to register a callback function.
 *          The result of this API call is notified in @ref BLE_GAP_EVENT_STACK_ON event.
 * @param[in] p_init_param A callback functions registered with this function.
 * @retval  BLE_SUCCESS(0x0000) Success 
 * @retval  BLE_ERR_INVALID_PTR(0x0001) gap_cb is specified as NULL. 
 * @retval  BLE_ERR_INVALID_ARG(0x0003) The pairing parameter is out of range. 
 * @retval  BLE_ERR_INVALID_STATE(0x0008) The reason for this error is as follows:
 *          - Host Stack was already initialized.
 *          - The task for host stack is not running.  
 * @retval  BLE_ERR_CONTEXT_FULL(0x000B) Callback function has already registered.
 * @retval  BLE_ERR_MEM_ALLOC_FAILED(0x000C) Insufficient memory is needed to generate this function. 
 * @details 
 *          <b>Message Sequence Chart</b>\n
 *          \arg @ref subsection_msc_abs_init
 **********************************************************************************************************************/
ble_status_t R_BLE_ABS_Init(st_ble_abs_init_param_t * p_init_param);

/******************************************************************************************************************//**
 * @fn void R_BLE_ABS_Reset(ble_event_cb_t init_cb)
 * @brief   Reset BLE.
 * @details BLE is reset with this function. The process is carried out in the following order.
 *          - R_BLE_Close() 
 *          - R_BLE_GAP_Terminate()
 *          - R_BLE_Open()
 *          - R_BLE_SetEvent()
 * 
 *          The init_cb callback initializes the others (Host Stack, timer, etc...).
 * @param[in] init_cb  callback function.
 * @retval  none
 * @details 
 *          <b>Message Sequence Chart</b>\n
 *          \arg @ref subsection_msc_abs_reset
 **********************************************************************************************************************/
void R_BLE_ABS_Reset(ble_event_cb_t init_cb);

/******************************************************************************************************************//**
 * @fn ble_status_t R_BLE_ABS_StartLegacyAdv(st_ble_abs_legacy_adv_param_t * p_adv_param)
 * @brief   Start Legacy Connectable Advertising.
 * @details Start Legacy Advertising after setting advertising parameters, advertising data and scan response data. \n
 *          If this function with the "all features" library is used, set 4 to BLE_CFG_RF_ADV_SET_MAX. \n
 *          Advertising configuration for this function is as follows.
 *          \arg The legacy advertising uses the advertising set whose advertising handle is 0.
 *          \arg The advertising type is connectable and scannable(ADV_IND).
 *          \arg The address type of local device is Public Identity Address or
 *          RPA(If the resolving list contains no matching entry, use the public address.).
 *          \arg Scan request event( @ref BLE_GAP_EVENT_SCAN_REQ_RECV ) is not notified.
 *           
 *          The relationship between fast_intv, fast_period, slow_intv and slow_period field 
 *          in p_adv_param parameter is shown in @ref Figure 1.1.
 *          @image html "abs_adv_period_en.svg" "Figure 1.1" width=1000px
 * @param[in] p_adv_param Advertising parameters for Legacy Advertising.
 * @retval  BLE_SUCCESS(0x0000) Success 
 * @retval  BLE_ERR_INVALID_PTR(0x0001) p_adv_param is specified as NULL. 
 * @retval  BLE_ERR_INVALID_ARG(0x0003) The advertising parameter is out of range. 
 * @retval  BLE_ERR_INVALID_STATE(0x0008) Host stack hasn't been initialized. 
 * @retval  BLE_ERR_MEM_ALLOC_FAILED(0x000C) Insufficient memory is needed to generate this function. 
 * @details 
 *          <b>Message Sequence Chart</b>\n
 *          \arg @ref subsection_msc_abs_legacy_adv
 **********************************************************************************************************************/
ble_status_t R_BLE_ABS_StartLegacyAdv(st_ble_abs_legacy_adv_param_t * p_adv_param);

/******************************************************************************************************************//**
 * @fn ble_status_t R_BLE_ABS_StartExtAdv(st_ble_abs_ext_adv_param_t * p_adv_param)
 * @brief   Start Extended Connectable Advertising.
 * @details Start Extended Advertising after setting advertising parameters, advertising data. \n
 *          If this function with the "all features" library is used, set 4 to BLE_CFG_RF_ADV_SET_MAX. \n
 *          Advertising configuration for this function is as follows.
 *          \arg The extended advertising uses the advertising set whose advertising handle is 1.
 *          \arg The advertising type is connectable and non-scannable.
 *          \arg The address type of local device is Public Identity Address or 
 *          RPA(If the resolving list contains no matching entry, use the public address.).
 *          \arg Scan request event( @ref BLE_GAP_EVENT_SCAN_REQ_RECV ) is not notified.
 *          \arg Secondary Advertising Max Skip is 0.
 *          The relationship between fast_intv, fast_period, 
 *          slow_intv and slow_period field in p_adv_param parameter is shown in @ref Figure 1.1
 * @param[in] p_adv_param Advertising parameters for Extended Advertising.
 * @retval  BLE_SUCCESS(0x0000) Success 
 * @retval  BLE_ERR_INVALID_PTR(0x0001) p_adv_param is specified as NULL. 
 * @retval  BLE_ERR_INVALID_ARG(0x0003) The advertising parameter is out of range. 
 * @retval  BLE_ERR_INVALID_STATE(0x0008) Host stack hasn't been initialized. 
 * @retval  BLE_ERR_MEM_ALLOC_FAILED(0x000C) Insufficient memory is needed to generate this function. 
 * @details 
 *          <b>Message Sequence Chart</b>\n
 *          \arg @ref subsection_msc_abs_ext_adv
 **********************************************************************************************************************/
ble_status_t R_BLE_ABS_StartExtAdv(st_ble_abs_ext_adv_param_t * p_adv_param);

/******************************************************************************************************************//**
 * @fn ble_status_t R_BLE_ABS_StartNonConnAdv(st_ble_abs_non_conn_adv_param_t * p_adv_param)
 * @brief   Start Non-Connectable Advertising.
 * @details Start Non-Connectable Advertising after setting advertising parameters, advertising data. \n
 *          If this function with the "all features" library is used, set 4 to BLE_CFG_RF_ADV_SET_MAX. \n
 *          Advertising configuration for this function is as follows.
 *           \arg The non-connectable advertising uses the advertising set whose advertising handle is 2.
 *          \arg The advertising type is non-connectable and non-scannable.
 *          \arg The address type of local device is Public Identity Address or 
 *          RPA(If the resolving list contains no matching entry, use the public address.).
 *          \arg Scan request event( @ref BLE_GAP_EVENT_SCAN_REQ_RECV ) is not notified.
 *          \arg Secondary Advertising Max Skip is 0.
 * @param[in] p_adv_param Advertising parameters for Non-Connectable Advertising.
 * @retval  BLE_SUCCESS(0x0000) Success 
 * @retval  BLE_ERR_INVALID_PTR(0x0001) p_adv_param is specified as NULL. 
 * @retval  BLE_ERR_INVALID_ARG(0x0003) The advertising parameter is out of range. 
 * @retval  BLE_ERR_INVALID_STATE(0x0008) Host stack hasn't been initialized. 
 * @retval  BLE_ERR_MEM_ALLOC_FAILED(0x000C) Insufficient memory is needed to generate this function. 
 * @details 
 *          <b>Message Sequence Chart</b>\n
 *          \arg @ref subsection_msc_abs_non_conn_adv
 **********************************************************************************************************************/
ble_status_t R_BLE_ABS_StartNonConnAdv(st_ble_abs_non_conn_adv_param_t * p_adv_param);

/******************************************************************************************************************//**
 * @fn ble_status_t R_BLE_ABS_StartPerdAdv(st_ble_abs_perd_adv_param_t * p_adv_param)
 * @brief   Start Periodic Advertising.
 * @details Start Periodic Advertising after setting advertising parameters, periodic advertising parameters, 
 *          advertising data and periodic advertising data. \n
 *          If this function with the "all features" library is used, set 4 to BLE_CFG_RF_ADV_SET_MAX. \n
 *          Advertising configuration for this function is as follows.
 *          \arg The periodic advertising uses the advertising set whose advertising handle is 3.
 *          \arg The advertising type is non-connectable and non-scannable.
 *          \arg The address type of local device is Public Identity Address or 
 *          RPA(If the resolving list contains no matching entry, use the public address.).
 *          \arg Scan request event( @ref BLE_GAP_EVENT_SCAN_REQ_RECV ) is not notified.
 *          \arg Secondary Advertising Max Skip is 0.
 * @param[in] p_adv_param Advertising parameters for Periodic Advertising.
 * @retval  BLE_SUCCESS(0x0000) Success 
 * @retval  BLE_ERR_INVALID_PTR(0x0001) p_adv_param is specified as NULL. 
 * @retval  BLE_ERR_INVALID_ARG(0x0003) The advertising parameter is out of range. 
 * @retval  BLE_ERR_INVALID_STATE(0x0008) Host stack hasn't been initialized. 
 * @retval  BLE_ERR_MEM_ALLOC_FAILED(0x000C) Insufficient memory is needed to generate this function. 
 * @details 
 *          <b>Message Sequence Chart</b>\n
 *          \arg @ref subsection_msc_abs_perd_adv
 **********************************************************************************************************************/
ble_status_t R_BLE_ABS_StartPerdAdv(st_ble_abs_perd_adv_param_t * p_adv_param);

/******************************************************************************************************************//**
 * @fn ble_status_t R_BLE_ABS_StartScan(st_ble_abs_scan_param_t * p_scan_param)
 * @brief   Start scanning.
 * @details Start scanning after setting scan parameters.
 *          Scanning configuration for this function is as follows.
 *          \arg The scanner address type is Public Identity Address.
 *          \arg The type of the procedure is Observation Procedure( @ref BLE_GAP_SC_PROC_OBS(0x00)).
 * 
 *          The result of this API call is notified in BLE_GAP_EVENT_SCAN_ON event. \n
 *          Advertising report is notified in BLE_GAP_EVENT_ADV_REPT_IND event. \n
 *          The relationship between fast_intv, fast_period, slow_intv and slow_period field 
 *          in p_scan_param parameter is shown in @ref Figure 1.2. \n
 *          After the fast scanning with fast_intv/fast_window for "fast_period", 
 *          the slow scanning with slow_intv/slow_window performs for "slow_period". \n
 *          The term specified by "fast_period", "slow_period" has been finished, 
 *          and @ref BLE_GAP_EVENT_SCAN_TO event is notified.
 *          If fast_period is 0, only the slow scanning with slow_intv/slow_window is performed. \n
 *          If slow_period is 0, the slow scanning with slow_intv/slow_window continues.
 * 
 *          @image html "abs_api_scan_period_en.svg" "Figure 1.2" width=1000px
 * @param   p_scan_param Scan parameters.
 * @retval  BLE_SUCCESS(0x0000) Success 
 * @retval  BLE_ERR_INVALID_PTR(0x0001) p_scan_param is specified as NULL. 
 * @retval  BLE_ERR_INVALID_ARG(0x0003) The scan parameter is out of range. 
 * @retval  BLE_ERR_INVALID_STATE(0x0008) Host stack hasn't been initialized. 
 * @retval  BLE_ERR_MEM_ALLOC_FAILED(0x000C) Insufficient memory is needed to generate this function. 
 * @details 
 *          <b>Message Sequence Chart</b>\n
 *          \arg @ref subsection_msc_abs_scan
 **********************************************************************************************************************/
ble_status_t R_BLE_ABS_StartScan(st_ble_abs_scan_param_t * p_scan_param);

/******************************************************************************************************************//**
 * @fn ble_status_t R_BLE_ABS_CreateConn(st_ble_abs_conn_param_t * p_conn_param)
 * @brief   Request create connection.
 * @details The configuration for the request for creating connection is as follows.\n
 *          \arg The initiator address type is Public Identity Address.
 *          \arg The scan interval is 60ms and the scan window is 30ms in case of 1M PHY or 2M PHY.
 *          \arg The scan interval is 180ms and the scan window is 90ms in case of coded PHY.
 *          \arg The Minimum CE Length and the Maximum CE Length are 0xFFFF.
 * 
 *          When the request for a connection has been received by the Controller, 
 *          @ref BLE_GAP_EVENT_CREATE_CONN_COMP event is notified.
 *          When a link has been established, @ref BLE_GAP_EVENT_CONN_IND event is notified.
 * @param   p_conn_param Create connection parameters.
 * @retval  BLE_SUCCESS(0x0000) Success 
 * @retval  BLE_ERR_INVALID_PTR(0x0001) p_conn_param is specified as NULL. 
 * @retval  BLE_ERR_INVALID_ARG(0x0003) The create connection parameter is out of range. 
 * @retval  BLE_ERR_INVALID_STATE(0x0008) Host stack hasn't been initialized. 
 * @retval  BLE_ERR_MEM_ALLOC_FAILED(0x000C) Insufficient memory is needed to generate this function. 
 * @details 
 *          <b>Message Sequence Chart</b>\n
 *          \arg @ref subsection_msc_abs_conn
 **********************************************************************************************************************/
ble_status_t R_BLE_ABS_CreateConn(st_ble_abs_conn_param_t * p_conn_param);

/******************************************************************************************************************//**
 * @fn ble_status_t R_BLE_ABS_SetLocPrivacy(uint8_t * p_lc_irk, uint8_t privacy_mode)
 * @brief   Configure local device privacy.
 * @details Generate a IRK, add it to the resolving list, set privacy mode and enable RPA function.
 *          Register vendor specific callback function, if IRK is generated by this function.
 *          After configuring local device privacy, 
 *          @ref BLE_GAP_ADDR_RPA_ID_PUBLIC is specified as own device address in 
 *          the advertising/scan/create connection API.
 * @param   p_lc_irk IRK to be registered in the resolving list. 
 *          If p_lc_irk is specified as NULL, the new IRK is generated.
 * @param   privacy_mode privacy mode. \n
 *          The following value is set as the privacy mode. \n
 *          |  macro                       |   description         |
 *          |:-----------------------------|:--------------------- |
 *          | BLE_GAP_NET_PRIV_MODE(0x00)  | Network Privacy Mode  |
 *          | BLE_GAP_DEV_PRIV_MODE(0x01)  | Device Privacy Mode   |
 * @retval  BLE_SUCCESS(0x0000) Success 
 * @retval  BLE_ERR_INVALID_ARG(0x0003) The privacy_mode parameter is out of range. 
 * @retval  BLE_ERR_INVALID_STATE(0x0008) Host stack hasn't been initialized or 
 *                                        configuring the resolving list or privacy mode.
 * @retval  BLE_ERR_MEM_ALLOC_FAILED(0x000C) Insufficient memory is needed to generate this function. 
 * @details 
 *          <b>Message Sequence Chart</b>\n
 *          \arg @ref subsection_msc_abs_priv
 **********************************************************************************************************************/
ble_status_t R_BLE_ABS_SetLocPrivacy(uint8_t * p_lc_irk, uint8_t privacy_mode);

/******************************************************************************************************************//**
 * @fn ble_status_t R_BLE_ABS_StartAuth(uint16_t conn_hdl)
 * @brief   Start pairing or encryption.
 * @details Start pairing or encryption. If pairing has been done, start encryption.
 *          The pairing parameters are configured by @ref R_BLE_ABS_Init() or @ref R_BLE_GAP_SetPairingParams().
 *          If the pairing parameters are configure by @ref R_BLE_ABS_Init(), 
 *          - bonding policy is that bonding information is stored.
 *          - Key press notification is not supported.
 *          
 * @param   conn_hdl Connection handle identifying the remote device 
 *                   which local device starts pairing or encryption with.
 * @retval  BLE_SUCCESS(0x0000) Success 
 * @retval  BLE_ERR_INVALID_STATE(0x0008) Host stack hasn't been initialized or 
 *                                        while generating OOB data, this function was called.
 * @retval  BLE_ERR_MEM_ALLOC_FAILED(0x000C) Insufficient memory is needed to generate this function. 
 * @retval  BLE_ERR_INVALID_HDL(0x000E) The remote device specified by conn_hdl is not found. 
 * @details 
 *          <b>Message Sequence Chart</b>\n
 *          \arg @ref subsection_msc_abs_auth_master
 *          \arg @ref subsection_msc_abs_auth_slave
 **********************************************************************************************************************/
ble_status_t R_BLE_ABS_StartAuth(uint16_t conn_hdl);

#ifdef __cplusplus
};
#endif

/*@}*/

#endif /* R_BLE_ABS_API_H */

/*@}*/
