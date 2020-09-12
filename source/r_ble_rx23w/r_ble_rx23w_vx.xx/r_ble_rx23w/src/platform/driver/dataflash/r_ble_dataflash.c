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
* File Name    : r_ble_dataflash.c
* Description  : The Data Flash driver for the BLE module.
* History      : DD.MM.YYYY Version Description
*              : 23.08.2019 1.00    First Release
***********************************************************************************************************************/

/**
 ****************************************************************************************
 * @addtogroup R_BLE_DATAFLASH
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "r_ble_rx23w_if.h"
#include "r_ble_rx23w_config.h"

#if (BLE_CFG_DEV_DATA_DF_BLOCK >= 0) || \
    ( (BLE_CFG_EN_SEC_DATA != 0) && (BLE_CFG_SECD_DATA_DF_BLOCK >= 0) )
#define ENABLE_BLE_DF_DRIVER    /* Enable/disable the data flash driver for BLE */
#endif

#ifdef ENABLE_BLE_DF_DRIVER
#include "r_flash_rx_if.h"
#endif /* ENABLE_BLE_DF_DRIVER */


#ifndef DISBALE_BLE_SECTION
#if defined(__CCRX__)
#pragma section P BLE_P
#pragma section C BLE_C
#pragma section D BLE_D
#pragma section B BLE_B
#endif /* defined(__CCRX__) */
#endif /* DISBALE_BLE_SECTION */

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

#ifdef ENABLE_HCI_MODE
#define DF_DATA_SIZE_MAX    (256)   /* max: FLASH_DF_BLOCK_SIZE */
#else /* ENABLE_HCI_MODE */
#define DF_DATA_SIZE_MAX    (1024)   /* max: FLASH_DF_BLOCK_SIZE */
#endif /* ENABLE_HCI_MODE */
#ifdef ENABLE_BLE_DF_DRIVER
/*
 * EXTERNAL FUNCTIONS/VARIABLE
 ****************************************************************************************
 */

/*
 * GLOBAL/STATIC VARIABLE
 ****************************************************************************************
 */
BLE_SECTION_B(1) uint8_t g_df_block[DF_DATA_SIZE_MAX];


/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */


/****************************************************************************************
* @brief Close FIT flash module.
* @param None
* @return None
*****************************************************************************************/
BLE_SECTION_P static void flash_close(void)
{
    /* E2 DataFlash Access disable */
#if ( ((FLASH_RX_VERSION_MAJOR<<16U)|(FLASH_RX_VERSION_MINOR))>=((3U<<16U)|(30U)) )
    R_FLASH_Close();
#else /* (((FLASH_RX_VERSION_MAJOR<<16U)|(FLASH_RX_VERSION_MINOR))>=0x3001E) */
    extern  flash_states_t g_flash_state;
    FLASH.DFLCTL.BIT.DFLEN = 0;
    g_flash_state = FLASH_UNINITIALIZED;
#endif /* (((FLASH_RX_VERSION_MAJOR<<16U)|(FLASH_RX_VERSION_MINOR))>=0x3001E) */
}

/****************************************************************************************
* @brief Read the value of the specified data flash
* @param[in] uint32_t addr - Address of the data flash to be read
* @param[out] uint8_t *buff - Copy destination buffer pointer of read data
* @param[in] uint16_t len  - Read length
* @return uint8_t 0=Success, Other=Error code
*****************************************************************************************/
BLE_SECTION_P uint8_t r_dflash_read(uint32_t addr, uint8_t *buff, uint16_t len)
{
    flash_err_t err = FLASH_ERR_PARAM;
    uint8_t *paddr = (uint8_t*)addr;

    do {
        if( (addr) < FLASH_DF_BLOCK_0 ) break;
        if( (addr + len) >= FLASH_DF_BLOCK_INVALID ) break;

        /* Open driver */
        err = R_FLASH_Open();
        if (err != FLASH_SUCCESS) break;

        /* Directly read data */
        while( len-- )
        {
            *buff++ = *paddr++;
        }

    } while(0);

    flash_close();

    return (uint8_t)err;

}

/****************************************************************************************
* @brief Write data flash and Read data that has already been written and overwrite the value.
* @param[in] uint32_t addr - Address of the data flash to be write
* @param[in] uint8_t *buff - Copy source buffer pointer of read data
* @param[in] uint16_t len  - Write length
* @return uint8_t 0=Success, Other=Error code
*****************************************************************************************/
BLE_SECTION_P uint8_t r_dflash_write(uint32_t addr, uint8_t *buff, uint16_t len)
{
    flash_err_t err = FLASH_ERR_PARAM;
    uint8_t *block_addr = (uint8_t*)(addr & ~0x3FFUL);
    uint16_t offset = (uint16_t)(addr & 0x3FFUL);
    uint16_t ii;

    do{
        if( (addr) < FLASH_DF_BLOCK_0 ) break;
        if( (addr + len) >= FLASH_DF_BLOCK_INVALID ) break;

        /* Open driver */
        err = R_FLASH_Open();
        if (err != FLASH_SUCCESS) break;

        /* set write data */
        for( ii = 0; ii < DF_DATA_SIZE_MAX; ii++ )
        {
            if( (ii >= offset) && (ii < (offset + len)) )
            {
                g_df_block[ii] = *buff++;
            }
            else
            {
                g_df_block[ii] = block_addr[ii];
            }
        }

        /* Erase data block */
        err = R_FLASH_Erase((flash_block_address_t)(uint32_t)block_addr, 1);
        if (err != FLASH_SUCCESS) break;

        /* Write data flash */
        err = R_FLASH_Write((uint32_t)g_df_block, (uint32_t)block_addr, DF_DATA_SIZE_MAX);
        if (err != FLASH_SUCCESS) break;

    }while(0);

    flash_close();

    return (uint8_t)err;
}
#else /* ENABLE_BLE_DF_DRIVER */
BLE_SECTION_P uint8_t r_dflash_read(uint32_t addr, uint8_t *buff, uint16_t len)
{
    (void)(addr);
    (void)(buff);
    (void)(len);

    return (uint8_t)(0xFF);
}
BLE_SECTION_P uint8_t r_dflash_write(uint32_t addr, uint8_t *buff, uint16_t len)
{
    (void)(addr);
    (void)(buff);
    (void)(len);

    return (uint8_t)(0xFF);
}
#endif /* ENABLE_BLE_DF_DRIVER */

#ifndef DISBALE_BLE_SECTION
#if defined(__CCRX__)
#pragma section
#endif /* defined(__CCRX__) */
#endif /* DISBALE_BLE_SECTION */

/// @} module
