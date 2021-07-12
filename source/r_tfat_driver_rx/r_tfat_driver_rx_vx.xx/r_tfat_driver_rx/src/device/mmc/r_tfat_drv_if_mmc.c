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
* Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_tfat_drv_if_mmc.c
* Description  : TFAT driver Interface for MMC memory card or eMMC.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 10.06.2020 1.00     First Release
*              : 10.09.2020 2.20     Revised the processing of the mmcif_disk_ioctl function. The value of 
*                                    GET_BLOCK_SIZE is increased by one.
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <string.h>
#include "r_tfat_drv_if_dev.h"
#include "r_tfat_driver_rx_config.h"

#if (TFAT_MMC_DRIVE_NUM > 0)
#include "ff.h"              /* TFAT define */
#include "diskio.h"              /* TFAT define */

#include "r_mmcif_rx_if.h"
#include "src/r_mmcif_rx_private.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define MMC_PRV_CSD_SHIT_7                  (7)
#define MMC_PRV_CSD_ERASE_BLOCK_MASK        (0x0000007f)
/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
*******************************************************************************/

/******************************************************************************
* Function Name : mmcif_disk_initialize
* Description   : This function initializes the memory medium
*               :    for file operations
* Arguments     :  uint8_t  drive        : Physical drive number
* Return value  : Status of the memory medium
******************************************************************************/
DSTATUS mmcif_disk_initialize(uint8_t drive)
{
    return RES_OK;
}/* End of function mmcif_disk_initialize() */

/******************************************************************************
* Function Name : mmcif_disk_read
* Description   : This function reads data from the specified location
*               :    of the memory medium
* Arguments     : uint8_t drive          : Physical drive number
*               : uint8_t* buffer        : Pointer to the read data buffer
*               : uint32_t sector_number : uint32_t SectorNumber
*               : uint32_t sector_count  : Number of sectors to read
* Return value  : Result of function execution
******************************************************************************/
DRESULT mmcif_disk_read (
    uint8_t drive,               /* Physical drive number            */
    uint8_t* buffer,             /* Pointer to the read data buffer  */
    uint32_t sector_number,      /* Start sector number              */
    uint32_t sector_count        /* Number of sectors to read        */
)
{
    mmc_access_t        mmc_Access;
    mmc_status_t        res = MMC_SUCCESS;

    /* parameter check */
    if ((NULL == buffer) || (0 == sector_count))
    {
        return RES_ERROR;
    }

    mmc_Access.p_buff       = (uint8_t *)buffer;
    mmc_Access.lbn          = sector_number;
    mmc_Access.cnt          = sector_count;
    mmc_Access.rw_mode      = MMC_PRE_DEF;

    /* TRANS mode : Software */
    res = R_MMCIF_Read_Memory_Software_Trans(drive, &mmc_Access);
    if ( MMC_SUCCESS != res )
    {
        return RES_ERROR;
    }

    return RES_OK;
}/* End of function mmcif_disk_read() */

/******************************************************************************
* Function Name : mmcif_disk_write
* Description   : This function writes data to a specified location
*               :    of the memory medium
* Arguments     : uint8_t drive          : Physical drive number
*               : const uint8_t* buffer  : Pointer to the write data
*               : uint32_t sector_number : Sector number to write
*               : uint32_t sector_count  : Number of sectors to write
* Return value  : Result of function execution
******************************************************************************/
DRESULT mmcif_disk_write (
    uint8_t drive,                /* Physical drive number           */
    const uint8_t* buffer,        /* Pointer to the write data       */
    uint32_t sector_number,       /* Sector number to write          */
    uint32_t sector_count         /* Number of sectors to write      */
)
{

    mmc_access_t        mmc_Access;
    mmc_status_t        res = MMC_SUCCESS;

    /* parameter check */
    if ((NULL == buffer) || (0 == sector_count))
    {
        return RES_ERROR;
    }

    mmc_Access.p_buff     = (uint8_t *)buffer;
    mmc_Access.lbn        = sector_number;
    mmc_Access.cnt        = sector_count;
    mmc_Access.rw_mode    = MMC_PRE_DEF;

    /* TRANS mode : Software */
    res = R_MMCIF_Write_Memory_Software_Trans(drive, &mmc_Access);
    if ( MMC_SUCCESS != res )
    {
        return RES_ERROR;
    }

    return RES_OK;
}/* End of function mmcif_disk_write() */

/******************************************************************************
* Function Name : mmcif_disk_ioctl
* Description   : This function is used to execute memory operations
*               :    other than read\write
* Arguments     : uint8_t drive   : Drive number
*               : uint8_t command : Control command code
*               : void*   buffer  : Data transfer buffer
* Return value  : Result of function execution
******************************************************************************/
DRESULT mmcif_disk_ioctl (
    uint8_t drive,                /* Drive number             */
    uint8_t command,              /* Control command code     */
    void* buffer                  /* Data transfer buffer     */
)
{
    uint8_t             cmd = command;
#if FF_USE_MKFS == 1
    mmc_card_status_t   mmc_CardStatus;
    mmc_card_reg_t      mmc_CardInfo;
    mmc_status_t        ret;
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
#if FF_USE_MKFS == 1
            /* Get MMC device status */
            ret = R_MMCIF_Get_CardStatus(drive, &mmc_CardStatus);
            if (MMC_SUCCESS != ret)
            {
                return RES_ERROR;
            }
            
            ((uint32_t *)buffer)[0] = (mmc_CardStatus.card_sector_size);
#else
            return RES_PARERR;
#endif
        break;
        
        case GET_SECTOR_SIZE:
#if FF_MAX_SS == FF_MIN_SS
            return RES_PARERR;
#else
            ((uint32_t *)buffer)[0] = (uint32_t)MMC_TRANS_BLOCK_SIZE;
#endif
        break;
        
        case GET_BLOCK_SIZE:
#if FF_USE_MKFS == 1
            /* Get MMC device information */
            ret = R_MMCIF_Get_CardInfo(drive, &mmc_CardInfo);
            if (MMC_SUCCESS != ret)
            {
                return RES_ERROR;
            }
            
            /* Erase sector size are [45:39] bits of CSD register */
            ((uint32_t *)buffer)[0]  = ((mmc_CardInfo.csd[1]>>MMC_PRV_CSD_SHIT_7) & MMC_PRV_CSD_ERASE_BLOCK_MASK) + 1;
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
}/* End of function mmcif_disk_ioctl() */

/******************************************************************************
* Function Name : mmcif_disk_status
* Description   : This function is used to retrieve the current status
*               :    of the disk
* Arguments     : uint8_t drive : Physical drive number
* Return value  : Status of the disk
******************************************************************************/
DSTATUS mmcif_disk_status (
    uint8_t drive                  /* Physical drive number    */
)
{

    /*  Please put the code for disk_status driver interface
         function over here.  */
    /*  Please refer the application note for details.  */
    return RES_OK;
}/* End of function mmcif_disk_status() */
#endif /* (TFAT_MMC_DRIVE_NUM > 0) */
/******************************************************************************
End  Of File
******************************************************************************/
