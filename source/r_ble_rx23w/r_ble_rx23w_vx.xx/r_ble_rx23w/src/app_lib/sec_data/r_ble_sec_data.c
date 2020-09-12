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
 * File Name    : r_ble_sec_data.c
 * Description  : Functions for BLE Security Data Management. 
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * History      : DD.MM.YYYY Version  Description
 *              : 23.08.2019 1.00     First Release
 **********************************************************************************************************************/

/***********************************************************************************************************************
  Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "r_ble_sec_data.h"

#if (BLE_CFG_EN_SEC_DATA == 1) && (BLE_CFG_HCI_MODE_EN == 0)
#ifndef USE_EXTERNAL_CONTROLLER
#include "r_flash_rx_if.h"
#include "r_flash_rx_config.h"
#endif /* USE_EXTERNAL_CONTROLLER */

/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/
#define BLE_SECD_UPD_BN_ADD                                             (0x00)
#define BLE_SECD_UPD_BN_ADD_OVERWR                                      (0x01)
#define BLE_SECD_UPD_BN_DEL                                             (0x02)
#define BLE_SECD_UPD_BN_ALL_DEL                                         (0x03)

/***********************************************************************************************************************
 Local Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Private global variables and functions
 **********************************************************************************************************************/
static st_ble_gap_key_ex_param_t gs_key_ex_param;
static st_ble_gap_key_dist_t gs_key_dist;

static const st_ble_dev_addr_t invalid_rem_addr = {
        .addr = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .type = 0xFF
    };

static ble_status_t find_entry(st_ble_dev_addr_t * p_dev_addr, int32_t * p_entry, uint8_t * p_sec_data);
static void find_oldest_entry(int32_t * p_entry);
static ble_status_t update_bond_num(int32_t entry, int32_t op_code, uint8_t * p_alloc_bond_num, uint8_t * p_sec_data);
static void update_bond_order(int32_t entry, uint8_t * p_sec_data, uint8_t bond_order);
static ble_status_t is_valid_entry(int32_t entry);
static ble_status_t read_bond_info(uint8_t * p_out_bond_num, uint8_t ** pp_sec_data, 
                                   st_ble_gap_bond_info_t * p_bond_info);
static void release_bond_info_buf(uint8_t * p_sec_data);

#ifdef USE_EXTERNAL_CONTROLLER
static uint8_t r_dflash_read(uint32_t addr, uint8_t *buff, uint16_t len);
static uint8_t r_dflash_write(uint32_t addr, uint8_t *buff, uint16_t len);
#else /* USE_EXTERNAL_CONTROLLER */
extern uint8_t r_dflash_read(uint32_t addr, uint8_t *buff, uint16_t len);
extern uint8_t r_dflash_write(uint32_t addr, uint8_t *buff, uint16_t len);
#endif /* USE_EXTERNAL_CONTROLLER */


/***********************************************************************************************************************
 Exported global functions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Function Name: R_BLE_SECD_WriteLocInfo
 * Description  : Write Local device Identity Address, IRK and/or CSRK in DataFlash
 * Arguments    : st_ble_dev_addr_t * p_lc_id_addr            ; Local device Identity Address
 *              : uint8_t * p_lc_irk                          ; Local IRK
 *              : uint8_t * p_lc_csrk                         ; Local CSRK
 * Return Value : BLE_SUCCESS(0x0000)                         ; Success 
 *                BLE_ERR_INVALID_PTR(0x0001)                 ; IRK(p_lc_id_addr or p_lc_irk) or CSRK(p_lc_csrk) 
 *                                                            ; is specified and as NULL. 
 *                BLE_ERR_INVALID_OPERATION(0x0009)           ; Write to DataFlash is failed.
 **********************************************************************************************************************/
ble_status_t R_BLE_SECD_WriteLocInfo(st_ble_dev_addr_t * p_lc_id_addr, uint8_t * p_lc_irk, uint8_t * p_lc_csrk)
{
    ble_status_t retval = BLE_SUCCESS;
    uint32_t local_tmp_data[(BLE_SECD_LOC_AREA_SIZE + BLE_SECD_MGN_DATA_SIZE + 3) / 4];

    if(((NULL == p_lc_id_addr) || (NULL == p_lc_irk)) && (NULL == p_lc_csrk))
    {
        return BLE_ERR_INVALID_PTR;
    }

    retval = r_dflash_read(BLE_SECD_BASE_ADDR, 
                           (uint8_t *)local_tmp_data, 
                           BLE_SECD_LOC_AREA_SIZE + BLE_SECD_MGN_DATA_SIZE);
    if(BLE_SUCCESS != retval)
    {
        return BLE_ERR_INVALID_OPERATION;
    }

    if(NULL != p_lc_irk)
    {
        memcpy((uint8_t *)local_tmp_data + BLE_SECD_MGN_DATA_SIZE, p_lc_irk, BLE_GAP_IRK_SIZE);
        memcpy((uint8_t *)local_tmp_data + BLE_SECD_MGN_DATA_SIZE + BLE_GAP_IRK_SIZE + BLE_GAP_CSRK_SIZE, 
               p_lc_id_addr,
               BLE_SECD_BD_ADDR_SIZE);
    }

    if(NULL != p_lc_csrk)
    {
        memcpy((uint8_t *)local_tmp_data + BLE_SECD_MGN_DATA_SIZE + BLE_GAP_IRK_SIZE, p_lc_csrk, BLE_GAP_CSRK_SIZE);
    }

    local_tmp_data[0] = BLE_SECD_MGC_NUM;

    retval = r_dflash_write(BLE_SECD_BASE_ADDR, 
                            (uint8_t *)local_tmp_data, 
                            BLE_SECD_LOC_AREA_SIZE + BLE_SECD_MGN_DATA_SIZE);
    if(BLE_SUCCESS != retval)
    {
        return BLE_ERR_INVALID_OPERATION;
    }

    return retval;
} /* End of function R_BLE_SECD_WriteLocInfo() */

/***********************************************************************************************************************
 * Function Name: R_BLE_SECD_ReadLocInfo
 * Description  : Read Local device Identity Address and IRK and/or CSRK in DataFlash.
 * Arguments    : st_ble_dev_addr_t * p_lc_id_addr            ; Local device Identity Address
 *              : uint8_t * p_lc_irk                          ; Local IRK
 *              : uint8_t * p_lc_csrk                         ; Local CSRK
 * Return Value : BLE_SUCCESS(0x0000)                         ; Success 
 *                BLE_ERR_INVALID_PTR(0x0001)                 ; IRK(p_lc_id_addr or p_lc_irk) or CSRK(p_lc_csrk) 
 *                                                            ; is specified and as NULL. 
 *                BLE_ERR_INVALID_OPERATION(0x0009)           ; Read to DataFlash is failed.
 *                BLE_ERR_NOT_FOUND(0x000D)                   ; IRK and Identity Address not found.
 **********************************************************************************************************************/
ble_status_t R_BLE_SECD_ReadLocInfo(st_ble_dev_addr_t * p_lc_id_addr, uint8_t * p_lc_irk, uint8_t * p_lc_csrk)
{
    ble_status_t retval;
    uint8_t * p_loc_area;
    uint32_t mgc_num;

    if(((NULL == p_lc_id_addr) || (NULL == p_lc_irk)) && (NULL == p_lc_csrk))
    {
        return BLE_ERR_INVALID_PTR;
    }

    p_loc_area = malloc(BLE_SECD_MGN_DATA_SIZE + BLE_SECD_LOC_AREA_SIZE);
    if(NULL == p_loc_area)
    {
        return BLE_ERR_MEM_ALLOC_FAILED;
    }

    retval = r_dflash_read(BLE_SECD_ADDR_MGN_DATA, p_loc_area, BLE_SECD_MGN_DATA_SIZE + BLE_SECD_LOC_AREA_SIZE);
    if(BLE_SUCCESS != retval)
    {
        free(p_loc_area);
        return BLE_ERR_INVALID_OPERATION;
    }

    memcpy(&mgc_num, p_loc_area, BLE_SECD_MGC_NUM_SIZE);
    if(BLE_SECD_MGC_NUM != mgc_num)
    {
        free(p_loc_area);
        return BLE_ERR_NOT_FOUND;
    }

    if((NULL != p_lc_irk) && (NULL != p_lc_id_addr))
    {
        memcpy(p_lc_irk, &p_loc_area[BLE_SECD_MGN_DATA_SIZE], BLE_GAP_IRK_SIZE);
        memcpy(p_lc_id_addr, &p_loc_area[BLE_SECD_MGN_DATA_SIZE + BLE_GAP_IRK_SIZE + BLE_GAP_CSRK_SIZE], 
               BLE_SECD_BD_ADDR_SIZE);
    }

    if(NULL != p_lc_csrk)
    {
        memcpy(p_lc_csrk, &p_loc_area[BLE_SECD_MGN_DATA_SIZE + BLE_GAP_IRK_SIZE], BLE_GAP_CSRK_SIZE);
    }

    free(p_loc_area);

    return BLE_SUCCESS;
} /* End of function R_BLE_SECD_ReadLocInfo() */

/***********************************************************************************************************************
 * Function Name: R_BLE_SECD_DelLocInfo
 * Description  : Delete Local Keys.
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
void R_BLE_SECD_DelLocInfo(void)
{
    uint8_t invalid_data[BLE_SECD_LOC_AREA_SIZE];

    memset(invalid_data, 0xFF, BLE_SECD_LOC_AREA_SIZE);
    r_dflash_write(BLE_SECD_ADDR_LOC_IRK, invalid_data, BLE_SECD_LOC_AREA_SIZE);

    return;
} /* End of function R_BLE_SECD_DelLocInfo() */


/***********************************************************************************************************************
 * Function Name: R_BLE_SECD_RecvRemKeys
 * Description  : Receive remote keys for write in DataFlash.
 * Arguments    : st_ble_dev_addr_t * p_addr                  ; Remote device address
 *              : st_ble_gap_key_ex_param_t * p_keys          ; Keys from remote device
 * Return Value : none
 **********************************************************************************************************************/
void R_BLE_SECD_RecvRemKeys(st_ble_dev_addr_t * p_addr, st_ble_gap_key_ex_param_t * p_keys)
{
    if((NULL == p_addr) || (NULL == p_keys))
    {
        return;
    }

    (void)p_addr;

    /* key_ex_parma */
    memcpy((uint8_t *)&gs_key_ex_param, (uint8_t *)p_keys, BLE_SECD_REM_KEY_ATTR_SIZE);

    /* keys */
    memcpy((uint8_t *)&gs_key_dist, (uint8_t *)p_keys->p_keys_info, BLE_SECD_REM_KEYS_SIZE);

    return;
} /* End of function R_BLE_SECD_RecvRemKeys() */

/***********************************************************************************************************************
 * Function Name: R_BLE_SECD_WriteRemKeys
 * Description  : Write Remote Keys in DataFlash.
 * Arguments    : st_ble_dev_addr_t * p_addr                  ; Remote device address
 *              : st_ble_gap_auth_info_t * p_keyinfo          ; Key information
 * Return Value : BLE_SUCCESS(0x0000)                         ; Success 
 *                BLE_ERR_INVALID_PTR(0x0001)                 ; p_addr or p_keyinfo is specified as NULL. 
 *                BLE_ERR_INVALID_OPERATION(0x0009)           ; Write to DataFlash is failed.
 *                BLE_ERR_MEM_ALLOC_FAILED(0x000C)            ; Memory allocation is failed.
 **********************************************************************************************************************/
ble_status_t R_BLE_SECD_WriteRemKeys(st_ble_dev_addr_t * p_addr, st_ble_gap_auth_info_t * p_keyinfo)
{
    ble_status_t retval = BLE_SUCCESS;
    int32_t entry;
    int32_t op_code = BLE_SECD_UPD_BN_ADD;
    uint32_t start_addr;
    uint8_t bond_num;
    uint8_t * p_sec_data;

    if((NULL == p_addr) || (NULL == p_keyinfo))
    {
        return BLE_ERR_INVALID_PTR;
    }

    /* mem alloc */
    p_sec_data = malloc(BLE_SECD_MAX_SIZE);
    if(NULL == p_sec_data)
    {
        return BLE_ERR_MEM_ALLOC_FAILED;
    }

    /* read rem area */
    retval = r_dflash_read(BLE_SECD_BASE_ADDR, p_sec_data, BLE_SECD_MAX_SIZE);
    if(BLE_SUCCESS != retval)
    {
        free(p_sec_data);
        return BLE_ERR_INVALID_OPERATION;
    }

    /* find entry with p_addr */
    retval = find_entry(p_addr, &entry, p_sec_data);
    if(BLE_SUCCESS != retval)
    {
        /* find empty entry */
        retval = find_entry(NULL, &entry, p_sec_data);
        if(BLE_SUCCESS != retval)
        {
            /* find oldest entry */
            find_oldest_entry(&entry);

            /** found the entry for overwrite */
            op_code = BLE_SECD_UPD_BN_ADD_OVERWR;
        }
    }

    start_addr = BLE_SECD_ADDR_REM_START + entry * BLE_SECD_REM_BOND_SIZE;

    /* addr */
    memcpy(&p_sec_data[BLE_SECD_SEC_REM_OFFSET + entry * BLE_SECD_REM_BOND_SIZE], 
           (uint8_t *)p_addr, BLE_SECD_BD_ADDR_SIZE);

    /* ex_key_param */
    gs_key_ex_param.p_keys_info = (st_ble_gap_key_dist_t *)(start_addr + BLE_SECD_SEC_KEYS_OFFSET);
    memcpy(&p_sec_data[BLE_SECD_SEC_REM_OFFSET + entry * BLE_SECD_REM_BOND_SIZE + BLE_SECD_SEC_KEYS_INFO_OFFSET], 
           (uint8_t *)&gs_key_ex_param, BLE_SECD_REM_KEY_ATTR_SIZE);

    /* keys */
    memcpy(&p_sec_data[BLE_SECD_SEC_REM_OFFSET + entry * BLE_SECD_REM_BOND_SIZE + BLE_SECD_SEC_KEYS_OFFSET], 
           (uint8_t *)&gs_key_dist, BLE_SECD_REM_KEYS_SIZE);

    /* keyinfo */
    memcpy(&p_sec_data[BLE_SECD_SEC_REM_OFFSET + entry * BLE_SECD_REM_BOND_SIZE + BLE_SECD_SEC_INFO_OFFSET], 
           (uint8_t *)p_keyinfo, BLE_SECD_REM_KEYS_INFO_SIZE);

    /** update bond order and the number of bonds. */
    update_bond_num(entry, op_code, &bond_num, p_sec_data);

    p_sec_data[BLE_SECD_SEC_REM_OFFSET + entry * BLE_SECD_REM_BOND_SIZE + BLE_SECD_BD_ADDR_SIZE] = bond_num;

    /* write to DataFlash */
    retval = r_dflash_write(BLE_SECD_BASE_ADDR, (uint8_t *)p_sec_data, BLE_SECD_MAX_SIZE);
    free(p_sec_data);
    
    return retval;
} /* End of function R_BLE_SECD_WriteRemKeys() */


/***********************************************************************************************************************
 * Function Name: R_BLE_SECD_Init
 * Description  : Set bonding information in DataFlash to Host Stack.
 * Arguments    : none
 * Return Value : BLE_SUCCESS(0x0000)                         ; Success 
 *                BLE_ERR_INVALID_PTR(0x0001)                 ; p_addr or p_keys is specified as NULL. 
 *                BLE_ERR_INVALID_ARG(0x0003)                 ; Number of bonding information is out of range.
 **********************************************************************************************************************/
ble_status_t R_BLE_SECD_Init(void)
{
    uint8_t in_bond_num;
    uint8_t out_bond_num;
    ble_status_t retval = BLE_SUCCESS;
    uint8_t * p_sec_data;
    st_ble_gap_bond_info_t bond_info[BLE_CFG_NUM_BOND];

    /** Read bonding information from DataFlash. */
    retval = read_bond_info(&in_bond_num, &p_sec_data, bond_info);
    if((BLE_SUCCESS != retval) && (BLE_ERR_NOT_FOUND != retval))
    {
        return retval;
    }

    /** No bonding information is written in DataFlash. */
    if(0 == in_bond_num)
    {
        return BLE_SUCCESS;
    }

    /** Set bonding information in DataFlash to Host Stack. */
    retval = R_BLE_GAP_SetBondInfo(bond_info, in_bond_num, &out_bond_num);

    /** bonding info buffer release */
    release_bond_info_buf(p_sec_data);

    return retval;
} /* End of function R_BLE_SECD_Init() */


/***********************************************************************************************************************
 * Function Name: R_BLE_SECD_DelRemKeys
 * Description  : Delete Remote Keys.
 * Arguments    : st_ble_dev_addr_t * p_dev_addr              ; Remote device address. IF p_addr is specified as NULL, 
 *                                                            ; all remote keys are deleted.
 **********************************************************************************************************************/
void R_BLE_SECD_DelRemKeys(st_ble_dev_addr_t * p_dev_addr)
{
    uint8_t * p_sec_data;
    ble_status_t retval;

    p_sec_data = malloc(BLE_SECD_MAX_SIZE);
    retval = r_dflash_read(BLE_SECD_BASE_ADDR, p_sec_data, BLE_SECD_MAX_SIZE);
    if(BLE_SUCCESS != retval)
    {
        free(p_sec_data);
        return;
    }

    if(NULL != p_dev_addr)
    {
        int32_t entry;
        int32_t del_target;
        uint8_t bond_num;

        retval = find_entry(p_dev_addr, &entry, p_sec_data);
        if(BLE_SUCCESS == retval)
        {
            update_bond_num(entry, BLE_SECD_UPD_BN_DEL, NULL, p_sec_data);
            bond_num = p_sec_data[BLE_SECD_MGC_NUM_SIZE];
            if((0xFF == bond_num) || (entry == bond_num))
            {
                del_target = entry;
            }
            else
            {
                /* swap remote bond area */
                memcpy(&p_sec_data[BLE_SECD_SEC_REM_OFFSET + entry * BLE_SECD_REM_BOND_SIZE], 
                       &p_sec_data[BLE_SECD_SEC_REM_OFFSET + bond_num * BLE_SECD_REM_BOND_SIZE], 
                       BLE_SECD_REM_BOND_SIZE);
                del_target = (int32_t)bond_num;
            }

            memset(&p_sec_data[BLE_SECD_SEC_REM_OFFSET + del_target * BLE_SECD_REM_BOND_SIZE], 0xFF, 
                   BLE_SECD_REM_BOND_SIZE);
        }
    }
    else
    {
        memset(&p_sec_data[BLE_SECD_SEC_REM_OFFSET], 0xFF, BLE_SECD_REM_BOND_SIZE * BLE_CFG_NUM_BOND);
        update_bond_num(0, BLE_SECD_UPD_BN_ALL_DEL, NULL, p_sec_data);
    }

    r_dflash_write((uint32_t)BLE_SECD_BASE_ADDR, p_sec_data, BLE_SECD_MAX_SIZE);
    free(p_sec_data);

    return;
} /* End of function R_BLE_SECD_DelRemKeys() */


/************************************************
 *   static function definitions                *
 ***********************************************/
/***********************************************************************************************************************
 * Function Name: find_entry
 * Description  : Find entry.
 * Arguments    : st_ble_dev_addr_t * p_dev_addr              ; Remote device address
 *              : int32_t * p_entry                           ; Entry
 *              : uint8_t * p_sec_data                        ; Security Data Area
 * Return Value : BLE_SUCCESS(0x0000)                         ; Success 
 *                BLE_ERR_NOT_FOUND(0x000D)                   ; Empty entry is not found.
  *********************************************************************************************************************/
static ble_status_t find_entry(st_ble_dev_addr_t * p_dev_addr, int32_t * p_entry, uint8_t * p_sec_data)
{
    st_ble_dev_addr_t * p_addr;
    int32_t i;
    uint32_t mgc_num = BLE_SECD_MGC_NUM;

    /* check magic number */
    if((NULL != p_dev_addr) && (0 != memcmp(p_sec_data, (uint8_t *)&mgc_num, BLE_SECD_MGC_NUM_SIZE)))
    {
        return BLE_ERR_NOT_FOUND;
    }

    p_addr = (NULL == p_dev_addr) ? (st_ble_dev_addr_t *)&invalid_rem_addr : p_dev_addr;

    for(i=0; i<BLE_CFG_NUM_BOND; i++)
    {
        if(0 == memcmp(&p_sec_data[BLE_SECD_SEC_REM_OFFSET + i * BLE_SECD_REM_BOND_SIZE], 
                       p_addr, BLE_SECD_BD_ADDR_SIZE))
        {
            *p_entry = i;
            return BLE_SUCCESS;
        }
    }

    return BLE_ERR_NOT_FOUND;
} /* End of function find_entry() */

/***********************************************************************************************************************
 * Function Name: find_oldest_entry
 * Description  : Find the oldest entry.
 * Arguments    : int32_t * p_entry                           ; Entry
 * Return Value : none
  *********************************************************************************************************************/
static void find_oldest_entry(int32_t * p_entry)
{
    uint8_t bond_order;
    int32_t out_bond;

    r_dflash_read(BLE_SECD_ADDR_MGN_DATA + BLE_SECD_OUT_BOND_OFFSET, &bond_order, 1);
    out_bond = (int32_t)bond_order;
    * p_entry = out_bond;

    return;
} /* End of function find_oldest_entry() */

/***********************************************************************************************************************
 * Function Name: update_bond_num
 * Description  : Update Bond Number
 * Arguments    : int32_t entry                               ; Entry
 *              : int32_t op_code                             ; Update Operation Code
 *              : uint8_t * p_alloc_bond_num                  ; Bonding Number
 *              : uint8_t * p_sec_data                        ; Security Data Area
 * Return Value : BLE_SUCCESS(0x0000)                         ; Success 
 *                BLE_ERR_INVALID_OPERATION(0x0009)           ; Magic Numver read or write failuire.
  *********************************************************************************************************************/
static ble_status_t update_bond_num(int32_t entry, int32_t op_code, uint8_t * p_alloc_bond_num, uint8_t * p_sec_data)
{
    ble_status_t retval = BLE_ERR_INVALID_OPERATION;
    uint8_t bond_num;
    uint8_t bond_order;

    bond_num = p_sec_data[BLE_SECD_MGC_NUM_SIZE];

    switch(op_code)
    {
        case BLE_SECD_UPD_BN_ADD :
        case BLE_SECD_UPD_BN_ADD_OVERWR :
            /* update bond_num */
            if(0xFF == bond_num)
            {
                bond_num = 1;
                p_sec_data[BLE_SECD_OUT_BOND_OFFSET] = 0;
                uint32_t mgc_num = BLE_SECD_MGC_NUM;
                memcpy(p_sec_data, (uint8_t *)&mgc_num, BLE_SECD_MGC_NUM_SIZE);
            }
            else if(BLE_CFG_NUM_BOND <= bond_num)
            {
                bond_num = BLE_CFG_NUM_BOND;
            }
            else
            {
                bond_num++;
            }

            p_sec_data[BLE_SECD_MGC_NUM_SIZE] = bond_num;

            /* update bond order */
            if(BLE_SECD_UPD_BN_ADD_OVERWR == op_code)
            {
                update_bond_order(entry, p_sec_data, 1);
            }

            * p_alloc_bond_num = bond_num;
            retval = BLE_SUCCESS;
        break;
        default : /* BLE_SECD_UPD_BN_DEL & BLE_SECD_UPD_BN_ALL_DEL */
            if(BLE_CFG_NUM_BOND >= bond_num)
            {
                /* update bond_num */
                if((BLE_SECD_UPD_BN_ALL_DEL == op_code) || (0 == bond_num - 1))
                {
                    bond_num = 0xFF;
                    p_sec_data[BLE_SECD_OUT_BOND_OFFSET] = 0xFF;
                }
                else
                {
                    bond_num = (uint8_t)(bond_num - 1);
                }
                
                p_sec_data[BLE_SECD_MGC_NUM_SIZE] = bond_num;

                if(0xFF != bond_num)
                {
                    bond_order = p_sec_data[BLE_SECD_ADDR_REM_START + 
                                            entry * BLE_SECD_REM_BOND_SIZE + BLE_SECD_BD_ADDR_SIZE];
                    update_bond_order(entry, p_sec_data, bond_order);
                }

                retval = BLE_SUCCESS;
            }
        break;
    }

    return retval;
} /* End of function update_bond_num() */

/***********************************************************************************************************************
 * Function Name: update_bond_order
 * Description  : Check entry validation
 * Arguments    : int32_t entry                               ; Entry
 *              : uint8_t * p_sec_data                        ; Security Data Area
 *              : uint8_t bond_order                          ; Bonding Information order
 * Return Value : none
  *********************************************************************************************************************/
static void update_bond_order(int32_t entry, uint8_t * p_sec_data, uint8_t bond_order)
{
    ble_status_t retval = BLE_ERR_INVALID_OPERATION;
    int32_t i;

    for(i=0; i<BLE_CFG_NUM_BOND; i++)
    {
        if(entry != i)
        {
            retval = is_valid_entry(i);
            if(BLE_SUCCESS != retval)
            {
                continue;
            }

            uint8_t tmp_order;
            tmp_order = p_sec_data[BLE_SECD_SEC_REM_OFFSET + i * BLE_SECD_REM_BOND_SIZE + BLE_SECD_BD_ADDR_SIZE];

            if(bond_order < tmp_order)
            {
                tmp_order = (uint8_t)(tmp_order - 1);
                p_sec_data[BLE_SECD_SEC_REM_OFFSET + i * BLE_SECD_REM_BOND_SIZE + BLE_SECD_BD_ADDR_SIZE] = tmp_order;
                if(1 == tmp_order)
                {
                    p_sec_data[BLE_SECD_OUT_BOND_OFFSET] = (uint8_t)i;
                }
            }
        }
    }

    return;
} /* End of function update_bond_order() */

/***********************************************************************************************************************
 * Function Name: is_valid_entry
 * Description  : Check entry validation
 * Arguments    : int32_t entry                               ; Entry
 * Return Value : BLE_SUCCESS(0x0000)                         ; Success valid entry.
 *                BLE_ERR_INVALID_OPERATION(0x0009)           ; Read BD_ADDR is failed.
 *                BLE_ERR_NOT_FOUND(0x000D)                   ; invalid entry
  *********************************************************************************************************************/
static ble_status_t is_valid_entry(int32_t entry)
{
    ble_status_t retval;
    uint8_t bd_addr[BLE_SECD_BD_ADDR_SIZE];
    uint8_t invalid_bd_addr[BLE_SECD_BD_ADDR_SIZE];
    uint32_t start_addr;

    start_addr = BLE_SECD_ADDR_REM_START + entry * BLE_SECD_REM_BOND_SIZE;
    retval = r_dflash_read(start_addr, bd_addr, BLE_SECD_BD_ADDR_SIZE);
    if(BLE_SUCCESS != retval)
    {
        return BLE_ERR_INVALID_OPERATION;
    }

    memset(invalid_bd_addr, 0xFF, BLE_SECD_BD_ADDR_SIZE);
    if(0 == memcmp(bd_addr, invalid_bd_addr, BLE_SECD_BD_ADDR_SIZE))
    {
        return BLE_ERR_NOT_FOUND;
    }

    return BLE_SUCCESS;
} /* End of function is_valid_entry() */

/***********************************************************************************************************************
 * Function Name: read_bond_info
 * Description  : Read Remote bonding information in DataFlash.
 * Arguments    : uint8_t * p_out_bond_num               ; The number of remote devices whose keys stored in DataFlash.
 *              : uint8_t ** pp_sec_data                 ; The allocate area which in security data is stored.
 *              : st_ble_gap_bond_info_t * p_bond_info   ; The security data to be set by R_BLE_GAP_SetBondInfo().
 * Return Value : BLE_SUCCESS(0x0000)                    ; Success 
 *                BLE_ERR_MEM_ALLOC_FAILED(0x000C)       ; Memory allocation is failed. 
 *                BLE_ERR_NOT_FOUND(0x000D)              ; No security data is stored in DataFlash.
 **********************************************************************************************************************/
static ble_status_t read_bond_info(uint8_t * p_out_bond_num, uint8_t ** pp_sec_data, 
                                   st_ble_gap_bond_info_t * p_bond_info)
{
    int32_t i;
    uint32_t start_addr;
    uint8_t * p_bonds;
    uint32_t magic_num;

    * pp_sec_data = malloc(BLE_SECD_MAX_SIZE);
    p_bonds = * pp_sec_data;
    if(NULL == p_bonds)
    {
        return BLE_ERR_MEM_ALLOC_FAILED;
    }

    r_dflash_read((uint32_t)BLE_SECD_BASE_ADDR, p_bonds, BLE_SECD_MAX_SIZE);

    /** check magic number and bond number */
    *p_out_bond_num = 0;
    memcpy((uint8_t *)&magic_num, p_bonds, BLE_SECD_MGC_NUM_SIZE);
    if((BLE_SECD_MGC_NUM != magic_num) || (0xFF == p_bonds[BLE_SECD_BOND_NUM_OFFSET]) || 
       (0x00 == p_bonds[BLE_SECD_BOND_NUM_OFFSET]))
    {
        free(p_bonds);
        return BLE_ERR_NOT_FOUND;
    }

    if(BLE_CFG_NUM_BOND < p_bonds[BLE_SECD_BOND_NUM_OFFSET])
    {
        free(p_bonds);
        return BLE_ERR_LIMIT_EXCEEDED;
    }

    for(i=0; i<p_bonds[BLE_SECD_BOND_NUM_OFFSET]; i++)
    {
        start_addr = BLE_SECD_SEC_REM_OFFSET + i * BLE_SECD_REM_BOND_SIZE;
        p_bond_info[i].p_addr = (st_ble_dev_addr_t *)(p_bonds + start_addr);
        p_bond_info[i].p_auth_info = (st_ble_gap_auth_info_t *)(p_bonds + start_addr + BLE_SECD_SEC_INFO_OFFSET);
        p_bond_info[i].p_keys = (st_ble_gap_key_ex_param_t *)(p_bonds + start_addr + BLE_SECD_SEC_KEYS_INFO_OFFSET);
        p_bond_info[i].p_keys->p_keys_info = (st_ble_gap_key_dist_t *)(p_bonds + start_addr + BLE_SECD_SEC_KEYS_OFFSET);
        (*p_out_bond_num)++;
    }

    return BLE_SUCCESS;
} /* End of function read_bond_info() */

/***********************************************************************************************************************
 * Function Name: release_bond_info_buf
 * Description  : Release bonding information buffer.
 * Arguments    : uint8_t * p_sec_data                    ; Security Data stored in RAM.
 * Return Value : none
 **********************************************************************************************************************/
static void release_bond_info_buf(uint8_t * p_sec_data)
{
    free(p_sec_data);
} /* End of function release_bond_info_buf() */


#else  /* (BLE_CFG_EN_SEC_DATA == 1) && (BLE_CFG_HCI_MODE_EN == 0) */

ble_status_t R_BLE_SECD_WriteLocInfo(st_ble_dev_addr_t * p_lc_id_addr, uint8_t * p_lc_irk, uint8_t * p_lc_csrk)
{
    (void)p_lc_id_addr;
    (void)p_lc_irk;
    (void)p_lc_csrk;
    return BLE_ERR_UNSUPPORTED;
}

ble_status_t R_BLE_SECD_ReadLocInfo(st_ble_dev_addr_t * p_lc_id_addr, uint8_t * p_lc_irk, uint8_t * p_lc_csrk)
{
    (void)p_lc_id_addr;
    (void)p_lc_irk;
    (void)p_lc_csrk;
    return BLE_ERR_UNSUPPORTED;
}

void R_BLE_SECD_DelLocInfo(void)
{
    return;
}

void R_BLE_SECD_RecvRemKeys(st_ble_dev_addr_t * p_addr, 
                            st_ble_gap_key_ex_param_t * p_keys)
{
    (void)p_addr;
    (void)p_keys;
    return;
}

ble_status_t R_BLE_SECD_WriteRemKeys(st_ble_dev_addr_t * p_addr, 
                                     st_ble_gap_auth_info_t * p_keyinfo)
{
    (void)p_addr;
    (void)p_keyinfo;
    return BLE_ERR_UNSUPPORTED;
}

ble_status_t R_BLE_SECD_Init(void)
{
    return BLE_ERR_UNSUPPORTED;
}

void R_BLE_SECD_DelRemKeys(st_ble_dev_addr_t * p_addr)
{
    (void)p_addr;
    return;
}

#endif /* (BLE_CFG_EN_SEC_DATA == 1) && (BLE_CFG_HCI_MODE_EN == 0) */

