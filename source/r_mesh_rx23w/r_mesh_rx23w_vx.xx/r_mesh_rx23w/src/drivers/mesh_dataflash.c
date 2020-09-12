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

/**
 *******************************************************************************
 * @addtogroup MESH_DATAFLASH
 * @{
 *******************************************************************************
 */

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_flash_rx_if.h"
#include "r_mesh_rx23w_if.h"
#include "mesh_dataflash.h"

/*******************************************************************************
* Macro definitions
*******************************************************************************/
#define BLK_SIZE        (FLASH_DF_BLOCK_SIZE)
#define BLK_BASE(a)     ((a) & ~(BLK_SIZE - 1))
#define BLK_OFFSET(a)   ((a) &  (BLK_SIZE - 1))
#define ADDR_TOP        (FLASH_DF_BLOCK_0 + (BLK_SIZE * MESH_CFG_DATA_FLASH_BLOCK_ID))
#define ADDR_LAST       (ADDR_TOP - 1     + (BLK_SIZE * MESH_CFG_DATA_FLASH_BLOCK_NUM))

/*******************************************************************************
* Global Variables and Private Functions declaration
*******************************************************************************/
#if DATAFLASH_EN
static uint8_t g_df_block[BLK_SIZE];
static flash_block_address_t g_df_blk_start_address;
#endif /* DATAFLASH_EN */

/*******************************************************************************
* Functions
*******************************************************************************/
#if DATAFLASH_EN
/***************************************************************************//**
* @brief Reads a block-length data from Data Flash memory
*******************************************************************************/
static flash_err_t flash_blk_read(flash_block_address_t blk_base, uint8_t *buff)
{
    flash_err_t err;

    uint8_t *paddr = (uint8_t*)blk_base;
    uint16_t len = BLK_SIZE;

    do {
        /* Read data directly */
        while (len--)
        {
            *buff++ = *paddr++;
        }
        err = FLASH_SUCCESS;
    } while(0);

    return err;
}

/***************************************************************************//**
* @brief Writes a block-length data to Data Flash memory
*******************************************************************************/
static flash_err_t flash_blk_write(flash_block_address_t blk_base, uint8_t *buff)
{
    flash_err_t err;

    do {
        /* Erase block */
        err = R_FLASH_Erase(blk_base, 1);
        if (err != FLASH_SUCCESS) break;

        /* Write data */
        err = R_FLASH_Write((uint32_t)buff, (uint32_t)blk_base, BLK_SIZE);
        if (err != FLASH_SUCCESS) break;
    } while(0);

    return err;
}
#endif /* DATAFLASH_EN */

/***************************************************************************//**
* @brief Opens Data Flash memory access
*******************************************************************************/
uint8_t mesh_dataflash_open(void)
{
#if DATAFLASH_EN
    flash_err_t err;

    do {
        err = R_FLASH_Open();
        if (err != FLASH_SUCCESS) break;

        /* Initialize start address of target block */
        g_df_blk_start_address = FLASH_DF_BLOCK_INVALID;
    } while(0);

    return (uint8_t)err;
#else /* DATAFLASH_EN */
    return FLASH_ERR_FAILURE;
#endif /* DATAFLASH_EN */
}

/***************************************************************************//**
* @brief Reads variable-length data from Data Flash memory
*******************************************************************************/
uint8_t mesh_dataflash_read(uint32_t addr, uint8_t *buff, uint16_t len)
{
#if DATAFLASH_EN
    flash_err_t err = FLASH_ERR_PARAM;
    uint8_t *paddr = (uint8_t*)addr;

    do {
        /* Check if access range is valid */
        #if 1
        if (addr < ADDR_TOP) break;
        if ((addr + len) > ADDR_LAST) break;
        #else
        if (addr < FLASH_DF_BLOCK_0) break;
        if ((addr + len) >= FLASH_DF_BLOCK_INVALID) break;
        #endif

        /* Read data directly */
        while (len--)
        {
            *buff++ = *paddr++;
        }
        err = FLASH_SUCCESS;
    } while(0);

    return (uint8_t)err;
#else /* DATAFLASH_EN */
    return FLASH_ERR_FAILURE;
#endif /* DATAFLASH_EN */
}

/***************************************************************************//**
* @brief Writes variable-length data to Data Flash memory
*******************************************************************************/
uint8_t mesh_dataflash_write(uint32_t addr, uint8_t *buff, uint16_t len)
{
#if DATAFLASH_EN
    flash_err_t err = FLASH_ERR_PARAM;
    uint32_t pos, offset;

    do {
        /* Check if access range is valid */
        #if 1
        if (addr < ADDR_TOP) break;
        if ((addr + len) > ADDR_LAST) break;
        #else
        if (addr < FLASH_DF_BLOCK_0) break;
        if ((addr + len) >= FLASH_DF_BLOCK_INVALID) break;
        #endif

        /* Check if target block is changed */
        if ((g_df_blk_start_address != FLASH_DF_BLOCK_INVALID) &&
            (g_df_blk_start_address != BLK_BASE(addr)))
        {
            /* Write data to the target block */
            err = flash_blk_write(g_df_blk_start_address, g_df_block);
            if (err != FLASH_SUCCESS) break;
            g_df_blk_start_address = FLASH_DF_BLOCK_INVALID;
        }

        if (g_df_blk_start_address == FLASH_DF_BLOCK_INVALID)
        {
            /* Determine target block */
            g_df_blk_start_address = (flash_block_address_t)BLK_BASE(addr);

            /* Read data from the target block */
            err = flash_blk_read(g_df_blk_start_address, g_df_block);
            if (err != FLASH_SUCCESS) break;
        }

        for (pos = addr; pos < (addr + len); pos++)
        {
            offset = BLK_OFFSET(pos);

            /* Check if first byte of the block will be updated */
            if ((offset == 0UL) && (g_df_blk_start_address == FLASH_DF_BLOCK_INVALID))
            {
                /* Determine target block */
                g_df_blk_start_address = (flash_block_address_t)BLK_BASE(pos);

                /* Read data from the target block */
                err = flash_blk_read(g_df_blk_start_address, g_df_block);
                if (err != FLASH_SUCCESS) break;
            }

            /* Update data */
            g_df_block[offset] = *buff++;

            /* Check if is last byte of the block was updated */
            if (offset == (BLK_SIZE - 1))
            {
                /* Write data to the target block */
                err = flash_blk_write(g_df_blk_start_address, g_df_block);
                g_df_blk_start_address = FLASH_DF_BLOCK_INVALID;
                if (err != FLASH_SUCCESS) break;
            }
        }
    }while(0);

    return (uint8_t)err;
#else /* DATAFLASH_EN */
    return FLASH_ERR_FAILURE;
#endif /* DATAFLASH_EN */
}

/***************************************************************************//**
* @brief Erases Data Flash memory
*******************************************************************************/
uint8_t mesh_dataflash_erase_all(void)
{
#if DATAFLASH_EN
    flash_err_t err = FLASH_ERR_PARAM;

    do {
        if (g_df_blk_start_address != FLASH_DF_BLOCK_INVALID) break;

        /* Erase all blocks used by Mesh Stack */
        err = R_FLASH_Erase((flash_block_address_t)ADDR_TOP, MESH_CFG_DATA_FLASH_BLOCK_NUM);
    } while(0);

    return (uint8_t)err;
#else /* DATAFLASH_EN */
    return FLASH_ERR_FAILURE;
#endif /* DATAFLASH_EN */
}

/***************************************************************************//**
* @brief Closes Data Flash memory access
*******************************************************************************/
void mesh_dataflash_close(void)
{
    #if DATAFLASH_EN
    do {
        /* Check if there is data to be written */
        #if 1
        if (g_df_blk_start_address < ADDR_TOP) break;
        if (g_df_blk_start_address > ADDR_LAST) break;
        #else
        if (g_df_blk_start_address < FLASH_DF_BLOCK_0) break;
        if (g_df_blk_start_address >= FLASH_DF_BLOCK_INVALID) break;
        #endif
        if ((g_df_blk_start_address & (BLK_SIZE - 1)) != 0) break;

        flash_blk_write(g_df_blk_start_address, g_df_block);
    } while(0);
    g_df_blk_start_address = FLASH_DF_BLOCK_INVALID;

    R_FLASH_Close();
    #endif /* DATAFLASH_EN */
}

/// @} module
