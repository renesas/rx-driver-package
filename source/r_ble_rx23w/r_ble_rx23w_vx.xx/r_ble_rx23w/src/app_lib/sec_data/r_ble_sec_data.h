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
#ifndef R_BLE_SECD_DATA_H
#define R_BLE_SECD_DATA_H

#include "r_ble_rx23w_if.h"

/*******************************************************************************************************************//**
 * @file
 * @defgroup secd Security Data Management
 * @{
 * @ingroup app_lib
 * @brief Security Data Management
 * @details This provides security data management function
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 23.08.2019 1.00    First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

#ifdef USE_EXTERNAL_CONTROLLER
#undef BLE_CFG_EN_SEC_DATA
#define BLE_CFG_EN_SEC_DATA    (0)
#else /* USE_EXTERNAL_CONTROLLER */
#include "r_ble_pf_config_private.h"
#endif /* USE_EXTERNAL_CONTROLLER */

/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/
/** @defgroup secd_macro Macros 
 *  @{
 *  @brief Macro definition
 */

/** 
 * @def BLE_ABS_ADV_PHY_LEGACY
 * @brief Non-Connectable Legacy Advertising.
 */
#define BLE_SECD_MGC_NUM_SIZE                 (4)
#define BLE_SECD_MGC_NUM                      (0x12345678)

#define BLE_SECD_BLOCK_BASE                   BLE_SECD_DATA_DF_ADDR
#define BLE_SECD_BLOCK_SIZE                   (1024)

#define BLE_SECD_BOND_NUM_OFFSET              BLE_SECD_MGC_NUM_SIZE
#define BLE_SECD_OUT_BOND_OFFSET              (5)
#define BLE_SECD_SEC_INFO_OFFSET              (8)
#define BLE_SECD_SEC_KEYS_INFO_OFFSET         (12)
#define BLE_SECD_SEC_KEYS_OFFSET              (20)
#define BLE_SECD_SEC_REM_OFFSET               (48)

#define BLE_SECD_REM_BOND_SIZE                (88)
#define BLE_SECD_LOC_AREA_SIZE                (40)
#define BLE_SECD_LOC_INFO_SIZE                (1)
#define BLE_SECD_BD_ADDR_SIZE                 (7)
#define BLE_SECD_MGN_DATA_SIZE                (8)
#define BLE_SECD_REM_KEY_ATTR_SIZE            (6)
#define BLE_SECD_REM_KEYS_SIZE                (65)
#define BLE_SECD_REM_KEYS_INFO_SIZE           (4)


#define BLE_SECD_BASE_ADDR                    (BLE_SECD_BLOCK_BASE)
#define BLE_SECD_ADDR_MGN_DATA                (BLE_SECD_BASE_ADDR)
#define BLE_SECD_SEC_BOND_NUM                 (BLE_SECD_ADDR_MGN_DATA + BLE_SECD_MGC_NUM_SIZE)
#define BLE_SECD_ADDR_LOC_AREA                (BLE_SECD_BASE_ADDR + BLE_SECD_MGN_DATA_SIZE)
#define BLE_SECD_ADDR_LOC_IRK                 (BLE_SECD_ADDR_LOC_AREA)
#define BLE_SECD_ADDR_LOC_CSRK                (BLE_SECD_ADDR_LOC_IRK + BLE_GAP_IRK_SIZE)
#define BLE_SECD_ADDR_LOC_IDADDR              (BLE_SECD_ADDR_LOC_CSRK + BLE_GAP_CSRK_SIZE)
#define BLE_SECD_ADDR_LOC_INFO                (BLE_SECD_ADDR_LOC_IDADDR + BLE_SECD_BD_ADDR_SIZE)
#define BLE_SECD_ADDR_REM_START               (BLE_SECD_ADDR_LOC_INFO + BLE_SECD_LOC_INFO_SIZE)
#define BLE_SECD_MAX_SIZE                     (BLE_SECD_SEC_REM_OFFSET + BLE_SECD_REM_BOND_SIZE * BLE_CFG_NUM_BOND)

/*@}*/



/* ========================================= Security Data API Declarations ========================================= */
/** @defgroup secd_func Functions
 *  @{
 *  @brief Function definition
 */

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************************************************//**
 * @fn ble_status_t R_BLE_SECD_WriteLocInfo(st_ble_dev_addr_t * p_lc_id_addr, uint8_t * p_lc_irk, uint8_t * p_lc_csrk)
 * @brief   Write Local device Identity Address, IRK and/or CSRK in DataFlash
 * @details Write Local device Identity Address, IRK and/or CSRK in DataFlash
 * @param[in] p_lc_id_addr Local device Identity Address
 * @param[in] p_lc_irk Local IRK
 * @param[in] p_lc_csrk Local CSRK
 * @retval  BLE_SUCCESS(0x0000) Success 
 * @retval  BLE_ERR_INVALID_PTR(0x0001) IRK(p_lc_id_addr or p_lc_irk) or CSRK(p_lc_csrk) is specified and as NULL. 
 * @retval  BLE_ERR_INVALID_OPERATION(0x0009) Write to Data Flash is failed.
 **********************************************************************************************************************/
ble_status_t R_BLE_SECD_WriteLocInfo(st_ble_dev_addr_t * p_lc_id_addr, uint8_t * p_lc_irk, uint8_t * p_lc_csrk);

/******************************************************************************************************************//**
 * @fn ble_status_t R_BLE_SECD_ReadLocInfo(st_ble_dev_addr_t * p_lc_id_addr, uint8_t * p_lc_irk, uint8_t * p_lc_csrk)
 * @brief   Read Local device Identity Address and IRK and/or CSRK in DataFlash.
 * @details Read Local device Identity Address and IRK and/or CSRK in DataFlash.
 * @param[in] p_lc_id_addr Local device Identity Address
 * @param[in] p_lc_irk Local IRK
 * @param[in] p_lc_csrk Local CSRK
 * @retval  BLE_SUCCESS(0x0000) Success 
 * @retval  BLE_ERR_INVALID_PTR(0x0001) IRK(p_lc_id_addr or p_lc_irk) or CSRK(p_lc_csrk) is specified and as NULL. 
 * @retval  BLE_ERR_INVALID_OPERATION(0x0009) Read to Data Flash is failed.
 **********************************************************************************************************************/
ble_status_t R_BLE_SECD_ReadLocInfo(st_ble_dev_addr_t * p_lc_id_addr, uint8_t * p_lc_irk, uint8_t * p_lc_csrk);

/******************************************************************************************************************//**
 * @fn void R_BLE_SECD_DelLocInfo(void)
 * @brief   Delete Local Keys.
 * @details Delete Local Keys.
 * @param   none
 * @retval  none
 **********************************************************************************************************************/
void R_BLE_SECD_DelLocInfo(void);

/******************************************************************************************************************//**
 * @fn void R_BLE_SECD_RecvRemKeys(st_ble_dev_addr_t * p_addr, st_ble_gap_key_ex_param_t * p_keys)
 * @brief   Receive remote keys for write in DataFlash.
 * @details Receive remote keys for write in DataFlash.
 * @param[in] p_addr Remote device address
 * @param[in] p_keys Keys from remote device
 * @retval  none
 **********************************************************************************************************************/
void R_BLE_SECD_RecvRemKeys(st_ble_dev_addr_t * p_addr, st_ble_gap_key_ex_param_t * p_keys);


/******************************************************************************************************************//**
 * @fn ble_status_t R_BLE_SECD_WriteRemKeys(st_ble_dev_addr_t * p_addr, st_ble_gap_auth_info_t * p_keyinfo)
 * @brief   Write Remote Keys in DataFlash.
 * @details Write Remote Keys in DataFlash.
 * @param[in] p_addr Remote device address
 * @param[in] p_keyinfo Key info from remote device
 * @retval  BLE_SUCCESS(0x0000) Success 
 * @retval  BLE_ERR_INVALID_PTR(0x0001) p_addr or p_keyinfo is specified as NULL. 
 * @retval  BLE_ERR_INVALID_OPERATION(0x0009) Write to Data Flash is failed.
 **********************************************************************************************************************/
ble_status_t R_BLE_SECD_WriteRemKeys(st_ble_dev_addr_t * p_addr, st_ble_gap_auth_info_t * p_keyinfo);

/******************************************************************************************************************//**
 * @fn ble_status_t R_BLE_SECD_Init(void)
 * @brief   Set bonding information in DataFlash to Host Stack.
 * @details Set bonding information in DataFlash to Host Stack.
 * @param   none
 * @retval  BLE_SUCCESS(0x0000) Success 
 * @retval  BLE_ERR_INVALID_PTR(0x0001) p_addr or p_keys is specified as NULL. 
 * @retval  BLE_ERR_INVALID_ARG(0x0003) Number of bonding information is out of range.
 * @retval  BLE_ERR_INVALID_OPERATION(0x0009) Flash Open failure.
 **********************************************************************************************************************/
ble_status_t R_BLE_SECD_Init(void);

/******************************************************************************************************************//**
 * @fn ble_status_t R_BLE_SECD_DelRemKeys(st_ble_dev_addr_t * p_dev_addr)
 * @brief   Delete Remote Keys.
 * @details Delete Remote Keys.
 * @param   p_addr Remote device address. IF p_addr is specified as NULL, all remote keys are deleted.
 * @retval  none
 **********************************************************************************************************************/
void R_BLE_SECD_DelRemKeys(st_ble_dev_addr_t * p_addr);


/*@}*/

#ifdef __cplusplus
};
#endif

#endif /* R_BLE_SECD_DATA_H */
/*@}*/

