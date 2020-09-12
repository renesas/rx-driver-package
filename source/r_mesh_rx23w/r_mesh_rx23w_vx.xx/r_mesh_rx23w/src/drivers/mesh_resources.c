/**
 *  \file mesh_mempool.c
 */

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_mesh_rx23w_if.h"
#include "r_ble_pf_config_private.h"
#include "mesh_resources.h"
#include "mesh_dataflash.h"

#if (MEMPOOL_SIZE > BLE_CFG_MESH_DATA_DF_SIZE)
#error "(MEMPOOL_SIZE > BLE_CFG_MESH_DATA_DF_SIZE): Required Storage Size exceeds the Data Flash Size Available."
#endif

/*******************************************************************************
* Global Variables and Private Functions declaration
*******************************************************************************/
static union
{
    UINT32      dummy;                  /* dummy for 4byte boundary */
    UINT8       pool[MEMPOOL_SIZE];     /* declaration of the area */
} gs_mesh_mempool;

#if DATAFLASH_EN
static const storage_t gs_mesh_storage =
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
    mempool_init_pl(&gs_mesh_mempool, MEMPOOL_SIZE);
}

/***************************************************************************//**
* @brief Initializes Mesh Stack storage
*******************************************************************************/
void mesh_storage_init(void)
{
    #if DATAFLASH_EN
    storage_init_pl(&gs_mesh_storage);
    #endif /* DATAFLASH_EN */
}
