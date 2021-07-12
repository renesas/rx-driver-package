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
* Copyright (C) 2019-2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_mesh_rx23w_if.h"
#include "r_ble_pf_config_private.h"
#include "mesh_resources.h"
#include "mesh_dataflash.h"

#if (MESH_STORAGE_SIZE > BLE_CFG_MESH_DATA_DF_SIZE)
#error "(MESH_STORAGE_SIZE > BLE_CFG_MESH_DATA_DF_SIZE): Required Storage Size exceeds the Data Flash Size Available."
#endif

/*******************************************************************************
* Global Variables and Private Functions declaration
*******************************************************************************/
static union
{
    UINT32      dummy;                      /* dummy for 4byte boundary */
    UINT8       pool[MESH_MEMPOOL_SIZE];    /* declaration of the area */
} gs_mesh_mempool;

#if DATAFLASH_EN
static const MS_STORAGE_PL gs_mesh_storage =
{
    /* Information on the Dataflash available for Mesh */
    .base_address = _BLE_DF_ADDR(MESH_CFG_DATA_FLASH_BLOCK_ID),
    .size         = BLE_CFG_MESH_DATA_DF_SIZE,

    /* APIs to access Dataflash */
    .open  = mesh_dataflash_open,
    .close = mesh_dataflash_close,
    .read  = mesh_dataflash_read,
    .write = mesh_dataflash_write,
};
#endif /* DATAFLASH_EN */

/*******************************************************************************
* Functions
*******************************************************************************/
/***************************************************************************//**
* @brief Initializes Mesh Stack RAM sections
*******************************************************************************/
void mesh_section_init(void)
{
    memcpy(__sectop("MESH_R"),   __sectop("MESH_D"),   __secsize("MESH_D"));
    memcpy(__sectop("MESH_R_2"), __sectop("MESH_D_2"), __secsize("MESH_D_2"));
    memcpy(__sectop("MESH_R_1"), __sectop("MESH_D_1"), __secsize("MESH_D_1"));
    memset(__sectop("MESH_B"),   0x00, __secsize("MESH_B"));
    memset(__sectop("MESH_B_2"), 0x00, __secsize("MESH_B_2"));
    memset(__sectop("MESH_B_1"), 0x00, __secsize("MESH_B_1"));
}

/***************************************************************************//**
* @brief Initializes Mesh Stack memory pool
*******************************************************************************/
void mesh_mempool_init(void)
{
    MS_mempool_init_pl(&gs_mesh_mempool, MESH_MEMPOOL_SIZE);
}

/***************************************************************************//**
* @brief Initializes Mesh Stack storage
*******************************************************************************/
void mesh_storage_init(void)
{
    #if DATAFLASH_EN
    MS_storage_init_pl(&gs_mesh_storage);
    #endif /* DATAFLASH_EN */
}
