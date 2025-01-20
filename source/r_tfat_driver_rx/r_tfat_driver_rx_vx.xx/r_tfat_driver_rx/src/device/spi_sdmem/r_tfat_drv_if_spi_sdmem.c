/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_tfat_drv_if_spi_sdmem.c
* Description  : TFAT driver Interface for SPI mode SD memory card.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 08.05.2024 1.00     First Release
*              : 30.09.2024 2.60     Changed the comment of API functions to
*                                    the Doxygen style.
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <string.h>
#include "r_tfat_drv_if_dev.h"
#include "r_tfat_driver_rx_config.h"

#if (TFAT_SPI_SDMEM_DRIVE_NUM > 0)
#include "ff.h"              /* TFAT define */
#include "diskio.h"          /* TFAT define */

#include "r_sdc_spi_rx_if.h"
#include "src/r_sdc_spi_private.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define SPI_SDMEM_PRV_CSD_SHIT_7                  (7)
#define SPI_SDMEM_PRV_CSD_ERASE_BLOCK_MASK        (0x0000007f)

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
*******************************************************************************/


/**********************************************************************************************************************
* Function Name: spi_sdmem_disk_initialize
*******************************************************************************************************************//**
* @brief This function initializes the disk drive.
*
* @param[in] drive  Specifies the initialize drive number.
*
* @retval TFAT_RES_OK  Normal termination.
*
* @retval Others       DSTATUS status of the disk after function execution.
*
* @details This function does not execute the SPI mode SD memory card driver initialize. Please implement SPI mode
* SD memory card initialize code in user code.
* @note None
*/
DSTATUS spi_sdmem_disk_initialize(uint8_t drive)
{
    return RES_OK;
}


/**********************************************************************************************************************
* Function Name: spi_sdmem_disk_read
*******************************************************************************************************************//**
* @brief This function reads the data from disk.
*
* @param[in]  drive  Specifies the physical drive number.
*
* @param[out] buffer Pointer to the read buffer to store the read data. A buffer of the size equal to the
* number of bytes to be read is required.
*
* @param[in]  sector_number  Specifies the start sector number in logical block address (LBA).
*
* @param[in]  sector_count   Specifies number of sectors to read. The value can be 1 to 255.
*
* @retval DRESULT  Result of the function execution.
*
* @details Read data from the SPI mode SD memory by block.
* @note None
*/
DRESULT spi_sdmem_disk_read (
    uint8_t drive,               /* Physical drive number            */
    uint8_t* buffer,             /* Pointer to the read data buffer  */
    uint32_t sector_number,      /* Start sector number              */
    uint32_t sector_count        /* Number of sectors to read        */
)
{
    sdc_spi_access_t   spi_sdmem_Access;
    sdc_spi_status_t   res = SDC_SPI_SUCCESS;

    /* parameter check */
    if ((NULL == buffer) || (0 == sector_count))
    {
        return RES_ERROR;
    }

    spi_sdmem_Access.p_buff = (uint8_t *)buffer;
    spi_sdmem_Access.lbn    = sector_number;
    spi_sdmem_Access.cnt    = sector_count;

    /* TRANS mode : Software */
    res = R_SDC_SPI_Read(drive, &spi_sdmem_Access);
    if ( SDC_SPI_SUCCESS != res )
    {
        return RES_ERROR;
    }

    return RES_OK;
}


/**********************************************************************************************************************
* Function Name: spi_sdmem_disk_write
*******************************************************************************************************************//**
* @brief This function writes the data to the disk.
*
* @param[in] drive  Specifies the physical drive number.
*
* @param[in] buffer Pointer to the data to be written.
*
* @param[in] sector_number  Specifies the start sector number in logical block address (LBA).
*
* @param[in] sector_count   Specifies number of sectors to read. The value can be 1 to 255.
*
* @retval DRESULT  Result of the function execution.
*
* @details Write the data to the SPI mode SD memory by block.
* @note None
*/
DRESULT spi_sdmem_disk_write (
    uint8_t drive,                /* Physical drive number           */
    const uint8_t* buffer,        /* Pointer to the write data       */
    uint32_t sector_number,       /* Sector number to write          */
    uint32_t sector_count         /* Number of sectors to write      */
)
{

    sdc_spi_access_t   spi_sdmem_Access;
    sdc_spi_status_t   res;

    /* parameter check */
    if ((NULL == buffer) || (0 == sector_count))
    {
        return RES_ERROR;
    }

    spi_sdmem_Access.p_buff     = (uint8_t *)buffer;
    spi_sdmem_Access.lbn        = sector_number;
    spi_sdmem_Access.cnt        = sector_count;
    spi_sdmem_Access.write_mode = SDC_SPI_WRITE_OVERWRITE;

    /* TRANS mode : Software */
    res = R_SDC_SPI_Write(drive, &spi_sdmem_Access);
    if ( SDC_SPI_SUCCESS != res )
    {
        return RES_ERROR;
    }

    return RES_OK;
}


/**********************************************************************************************************************
* Function Name: spi_sdmem_disk_ioctl
*******************************************************************************************************************//**
* @brief This function controls the drive.
*
* @param[in] drive   Specifies the physical drive number.
*
* @param[in] command Specifies the command code. The command code will always be 0.
*
* @param[in] buffer  Pointer should always be a NULL pointer.
*
* @retval DRESULT  Result of the function execution.
*
* @details The spi_sdmem_disk_ioctl function is used only by the f_sync function amongst all the TFAT FIT
* functions. Users who do not plan to use f_sync function in their applications can skip the implementation
* for this particular driver interface function.\n
* For users who wish to use f_sync function in their applications, the command CTRL_SYNC has to be implemented.\n
* For users who wish to use f_sync function in their applications, this particular driver interface function
* will have to be implemented. This driver function should consist of the code to finish off any pending write
* process. If the disk i/o module has a write back cache, the dirty sector must be flushed immediately.The f_sync
* function will perform a save operation to the unsaved data related to the file object passed as argument.
* @note None
*/
DRESULT spi_sdmem_disk_ioctl (
    uint8_t drive,                /* Drive number             */
    uint8_t command,              /* Control command code     */
    void* buffer                  /* Data transfer buffer     */
)
{
    uint8_t                cmd = command;
#if FF_USE_MKFS == 1
    sdc_spi_card_status_t   spi_sdmem_cardstatus;
    sdc_spi_card_reg_t      spi_sdmem_cardinfo;
    sdc_spi_status_t        ret;
#endif

    if ((NULL == buffer) && (CTRL_SYNC != cmd))
    {
        return RES_PARERR;
    }

    switch (cmd)
    {
        case CTRL_SYNC:

        break;

        case GET_SECTOR_COUNT:
#if FF_USE_MKFS
            /* Get SD card sector count */
            ret = R_SDC_SPI_GetCardStatus(drive, &spi_sdmem_cardstatus);
            if (SDC_SPI_SUCCESS != ret)
            {
                return RES_ERROR;
            }

            ((uint32_t *)buffer)[0] = spi_sdmem_cardstatus.card_sector_size;
#else
            return RES_PARERR;
#endif
        break;

        case GET_SECTOR_SIZE:
#if FF_MAX_SS == FF_MIN_SS
            return RES_PARERR;
#else
            ((uint32_t *)buffer)[0] = (uint32_t)SDC_SPI_TRANS_BLOCK_SIZE;
#endif
        break;

        case GET_BLOCK_SIZE:
#if FF_USE_MKFS
            /* Get SD card block size */
            ret = R_SDC_SPI_GetCardInfo(drive, &spi_sdmem_cardinfo);
            if (SDC_SPI_SUCCESS != ret)
            {
                return RES_ERROR;
            }

            /* Erase sector size are [45:39] bits of CSD register */
            ((uint32_t *)buffer)[0]  = ((spi_sdmem_cardinfo.csd[1]>>SPI_SDMEM_PRV_CSD_SHIT_7) & SPI_SDMEM_PRV_CSD_ERASE_BLOCK_MASK) + 1;
#else
            return RES_PARERR;
#endif
        break;

        case CTRL_TRIM:

        break;

        default:
            return RES_PARERR;
        break;
    }
    return RES_OK;
}

/**********************************************************************************************************************
* Function Name: spi_sdmem_disk_status
*******************************************************************************************************************//**
* @brief This function gets the disk drive status.
*
* @param[in] drive  Specifies the physical drive number.
*
* @retval TFAT_RES_OK  Normal termination.
*
* @retval Others       DSTATUS status of the disk after function execution.
*
* @details This function should consist of the code that checks the disk and returns the current disk status.
* The disk status can have any of the three values, see Section 2.10 in the application note for details.
* The disk status can be returned by updating the return value with the macros related to disk status.
* @note None
*/
DSTATUS spi_sdmem_disk_status (
    uint8_t drive                  /* Physical drive number    */
)
{

    /*  Please put the code for disk_status driver interface
         function over here.  */
    /*  Please refer the application note for details.  */
    return RES_OK;
}
#endif /* (TFAT_SPI_SDMEM_DRIVE_NUM > 0) */
/******************************************************************************
End  Of File
******************************************************************************/
