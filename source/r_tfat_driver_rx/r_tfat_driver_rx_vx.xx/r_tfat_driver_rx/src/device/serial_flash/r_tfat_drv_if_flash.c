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
* File Name    : r_tfat_drv_if_flash.c
* Description  : TFAT driver Interface for FLASH memory.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 10.06.2020 1.00     First Release
*              : 10.09.2020 2.20     Revised the processing of the flash_spi_disk_ioctl function. The value of 
*                                    GET_BLOCK_SIZE is fixed to 16.
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <string.h>
#include "r_tfat_drv_if_dev.h"
#include "r_tfat_driver_rx_config.h"

#if (TFAT_SERIAL_FLASH_DRIVE_NUM > 0)
#include "ff.h"              /* TFAT define */
#include "diskio.h"              /* TFAT define */

#include "r_flash_spi_if.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define FLASH_WRITE_PROTECT_OPEN        (1)
#define FLASH_WRITE_PROTECT_CLOSE       (0)
#define FLASH_WRITE_PROTECT_STATUS      (0x3c)

/* Sector Erase busy timeout 200*1ms = 0.2s  */                                 /** SET **/
#define FLASH_SE_BUSY_WAIT              (200)
/* Page Program time out 3*1ms = 3ms */                                         /** SET **/
#define FLASH_PP_BUSY_WAIT              (3)
/* Write Register time out 40*1ms = 40ms */                                     /** SET **/
#define FLASH_WR_BUSY_WAIT              (40)
#define FLASH_BUSY                      (0x01)
#define FLASH_NO_BUSY                   (0x00)

#define FLASH_SECTOR_SIZE               (4096)
#define BIT_PER_BYTE_SHIFT              (8)

#define TIMER_MAX_CH                    (8)
#define TIMER_CH_FLG                    (0)
#define TIMER_CH_COUNT                  (1)
#define TIMER_CH_MAX_COUNT              (2)

#define FLASH_OK                        (0)
#define FLASH_ERROR                     (-1)
/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
*******************************************************************************/
static uint32_t    gs_timer_cnt_out[TIMER_MAX_CH][TIMER_CH_MAX_COUNT + 1];

static flash_spi_status_t flash_spi_busy_check (uint8_t driver, flash_spi_poll_mode_t mode, uint32_t loop_cnt);
static int8_t flash_spi_wait_time (uint32_t channel, uint32_t time);
static int8_t flash_spi_start_timer (uint32_t channel, uint32_t msec);
static int8_t flash_spi_check_timer (uint32_t channel);
static int8_t flash_spi_end_timer (uint32_t channel);

/******************************************************************************
* Function Name : flash_spi_disk_initialize
* Description   : This function initializes the memory medium
*               :    for file operations
* Arguments     :  uint8_t  drive        : Physical drive number
* Return value  : Status of the memory medium
******************************************************************************/
DSTATUS flash_spi_disk_initialize(uint8_t drive)
{
    uint8_t                 flash_status;       // Flash status

    flash_spi_status_t      ret = FLASH_SPI_SUCCESS;

    /* Read flash status */
    ret = R_FLASH_SPI_Read_Status(drive, &flash_status);
    if (FLASH_SPI_SUCCESS > ret)
    {
        return STA_NOINIT;
    }

    if (0 != (flash_status & FLASH_BUSY))
    {
        return STA_NOINIT;                      // Flash is busy
    }

    return RES_OK;
} /* End of function flash_spi_disk_initialize() */

/******************************************************************************
* Function Name : flash_spi_disk_read
* Description   : This function reads data from the specified location
*               :    of the memory medium
* Arguments     : uint8_t drive          : Physical drive number
*               : uint8_t* buffer        : Pointer to the read data buffer
*               : uint32_t sector_number : uint32_t SectorNumber
*               : uint32_t sector_count  : Number of sectors to read
* Return value  : Result of function execution
******************************************************************************/
DRESULT flash_spi_disk_read (
    uint8_t drive,               /* Physical drive number            */
    uint8_t* buffer,             /* Pointer to the read data buffer  */
    uint32_t sector_number,      /* Start sector number              */
    uint32_t sector_count        /* Number of sectors to read        */
)
{
    flash_spi_info_t        Flash_Info_R;
    flash_spi_status_t      ret = FLASH_SPI_SUCCESS;

    /* Set flash read arguments */
    Flash_Info_R.addr       = (uint32_t)(sector_number * FLASH_SECTOR_SIZE);
    Flash_Info_R.cnt        = (uint32_t)(sector_count * FLASH_SECTOR_SIZE);
    Flash_Info_R.p_data     = buffer;
    Flash_Info_R.op_mode    = FLASH_SPI_SINGLE;

    /* Read flash data */
    ret = R_FLASH_SPI_Read_Data(drive, &Flash_Info_R);
    if (FLASH_SPI_SUCCESS > ret)
    {
        return RES_ERROR;
    }

    return RES_OK;
} /* End of function flash_spi_disk_read() */

/******************************************************************************
* Function Name : flash_spi_disk_write
* Description   : This function writes data to a specified location
*               :    of the memory medium
* Arguments     : uint8_t drive          : Physical drive number
*               : const uint8_t* buffer  : Pointer to the write data
*               : uint32_t sector_number : Sector number to write
*               : uint32_t sector_count  : Number of sectors to write
* Return value  : Result of function execution
******************************************************************************/
DRESULT flash_spi_disk_write (
    uint8_t drive,                /* Physical drive number           */
    const uint8_t* buffer,        /* Pointer to the write data       */
    uint32_t sector_number,       /* Sector number to write          */
    uint32_t sector_count         /* Number of sectors to write      */
)
{
    uint32_t                address;            //write start address
    uint32_t                sector_cnt;         //sector count
    uint8_t                 *p_buf = (uint8_t *)buffer;    // data buffer;
    uint8_t                 flash_status;       // flash status
    
    flash_spi_info_t        Flash_Info_W;
    flash_spi_erase_info_t  Flash_Info_E;
    flash_spi_status_t      ret = FLASH_SPI_SUCCESS;

    /* Disable the write PROTECT */
    ret = R_FLASH_SPI_Set_Write_Protect(drive, FLASH_WRITE_PROTECT_CLOSE);
    if (FLASH_SPI_SUCCESS > ret)
    {
        return RES_ERROR;
    }

    /* Disable the write PROTECT completed */
    ret = flash_spi_busy_check(drive, FLASH_SPI_MODE_REG_WRITE_POLL, FLASH_WR_BUSY_WAIT);
    if (FLASH_SPI_SUCCESS != ret)
    {
        return RES_ERROR;
    }

    ret = R_FLASH_SPI_Read_Status(drive, &flash_status);
    if (FLASH_SPI_SUCCESS > ret)
    {
        return RES_ERROR;
    }

    if (0 != (flash_status & FLASH_WRITE_PROTECT_STATUS))   // Write protect is disable
    {
        return RES_WRPRT;
    }

    sector_cnt          = sector_count;                     // Write sector count
    address             = (uint32_t)(sector_number * FLASH_SECTOR_SIZE);    // Write address

    /* WAIT_LOOP */
    while (sector_cnt--)
    {
        Flash_Info_E.addr   = address;
        Flash_Info_E.mode   = FLASH_SPI_MODE_S_ERASE;

        /* erase sector */
        ret = R_FLASH_SPI_Erase(drive, &Flash_Info_E);
        if (FLASH_SPI_SUCCESS > ret)
        {
            return RES_ERROR;
        }

        ret = flash_spi_busy_check(drive, FLASH_SPI_MODE_ERASE_POLL, FLASH_SE_BUSY_WAIT);
        if (FLASH_SPI_SUCCESS != ret)
        {
            return RES_ERROR;
        }
        
        /* Set write arguments */
        Flash_Info_W.addr       = address;
        Flash_Info_W.cnt        = FLASH_SECTOR_SIZE;
        Flash_Info_W.p_data     = p_buf;
        Flash_Info_W.op_mode    = FLASH_SPI_SINGLE;

        do
        {
            /* Write page */
            ret = R_FLASH_SPI_Write_Data_Page(drive, &Flash_Info_W);
            if (FLASH_SPI_SUCCESS > ret)
            {
                return RES_ERROR;
            }

            ret = flash_spi_busy_check(drive, FLASH_SPI_MODE_PROG_POLL, FLASH_PP_BUSY_WAIT);
            if (FLASH_SPI_SUCCESS > ret)
            {
                return RES_ERROR;
            }
        }
        while (0 != Flash_Info_W.cnt); /* WAIT_LOOP */
        
        address += FLASH_SECTOR_SIZE;           // Update write address
    }

    /* Enable write PROTECT */
    ret = R_FLASH_SPI_Set_Write_Protect(drive, FLASH_WRITE_PROTECT_OPEN);
    if (FLASH_SPI_SUCCESS > ret)
    {
        return RES_ERROR;
    }
    
    ret = flash_spi_busy_check(drive, FLASH_SPI_MODE_REG_WRITE_POLL, FLASH_WR_BUSY_WAIT);
    if (FLASH_SPI_SUCCESS != ret)
    {
        return RES_ERROR;
    }

    return RES_OK;
} /* End of function flash_spi_disk_write() */

/******************************************************************************
* Function Name : flash_spi_disk_ioctl
* Description   : This function is used to execute memory operations
*               :    other than read\write
* Arguments     : uint8_t drive   : Drive number
*               : uint8_t command : Control command code
*               : void*   buffer  : Data transfer buffer
* Return value  : Result of function execution
******************************************************************************/
DRESULT flash_spi_disk_ioctl (
    uint8_t drive,                /* Drive number             */
    uint8_t command,              /* Control command code     */
    void* buffer                  /* Data transfer buffer     */
)
{
    uint8_t             cmd = command;
    flash_spi_status_t      ret = FLASH_SPI_SUCCESS;
#if FF_FS_READONLY == 0
    uint8_t                 flash_status;       // flash status
#endif
#if FF_USE_MKFS == 1
    uint32_t                flash_size;
    flash_spi_mem_info_t Flash_MemInfo;
#endif

    if ((NULL == buffer) && (CTRL_SYNC != cmd))
    {
        return RES_PARERR;
    }

    switch (cmd)
    {
        case CTRL_SYNC:
#if FF_FS_READONLY == 0
            /* Read FLASH status */
            ret = R_FLASH_SPI_Read_Status(drive, &flash_status);
            if (FLASH_SPI_SUCCESS > ret)
            {
                return RES_ERROR;
            }
            
            if (0 != (flash_status & FLASH_BUSY))
            {
                return RES_NOTRDY;             // Flash is busy
            }
            else
            {
                R_BSP_NOP();          // Flash isn't busy
            }
#endif
        break;
        
        case GET_SECTOR_COUNT:
#if FF_USE_MKFS == 1
            /* Get Flash memory information */
            ret = R_FLASH_SPI_GetMemoryInfo(drive, &Flash_MemInfo);
            if (FLASH_SPI_SUCCESS != ret)
            {
                return RES_ERROR;
            }
            
            flash_size = (Flash_MemInfo.mem_size / FLASH_SECTOR_SIZE);
            ((uint32_t *)buffer)[0] = flash_size;       // block size is 64KB, sector size is 4KB
#endif
        break;
        
        case GET_SECTOR_SIZE:
#if FF_MAX_SS != FF_MIN_SS
            flash_size = FLASH_SECTOR_SIZE;
            ((uint32_t *)buffer)[0] = flash_size;       // Return the sector size of flash
#endif
        break;
        
        case GET_BLOCK_SIZE:
#if FF_USE_MKFS == 1
            ((uint32_t *)buffer)[0] = 16;               // block size is 64KB, sector size is 4KB
#endif
        break;
        
        case CTRL_TRIM:
            
        break;
        
        default:
            return RES_PARERR;
        break;
    }

    return RES_OK;
} /* End of function flash_spi_disk_ioctl() */

/******************************************************************************
* Function Name : flash_spi_disk_status
* Description   : This function is used to retrieve the current status
*               :    of the disk
* Arguments     : uint8_t drive : Physical drive number
* Return value  : Status of the disk
******************************************************************************/
DSTATUS flash_spi_disk_status (
    uint8_t drive                  /* Physical drive number    */
)
{
    uint8_t                 flash_status;           // flash status
    flash_spi_status_t      ret = FLASH_SPI_SUCCESS;

    ret = R_FLASH_SPI_Read_Status(drive, &flash_status);
    if (FLASH_SPI_SUCCESS != ret)
    {
        return STA_NODISK;
    }

    if (0 != (flash_status & FLASH_BUSY))
    {
        return STA_NOINIT;                          //Flash is busy
    }
    else
    {
        R_BSP_NOP();
    }

    return RES_OK;
} /* End of function flash_spi_disk_status() */

/******************************************************************************
* Function Name : flash_spi_1ms_interval
* Description   : This function is used to initialize the 1ms timer
*               :    of the disk
* Arguments     : None
* Return value  : None
******************************************************************************/
void flash_spi_1ms_interval (
     void
)
{
    uint32_t channel = 0;

    /* WAIT_LOOP */
    for (channel = 0; channel < TIMER_MAX_CH; channel++)
    {
        if (0 != gs_timer_cnt_out[channel][TIMER_CH_FLG])
        {
            gs_timer_cnt_out[channel][TIMER_CH_COUNT]++;
        }
    }

    return;
} /* End of function flash_spi_1ms_interval() */

/******************************************************************************
* Function Name : flash_spi_busy_check
* Description   : This function is used to check flash memory status
*               :    of the disk
* Arguments     : flash_spi_poll_mode_t mode          Mode of error check
*                 uint32_t loop_cnt                   loop counter(ms)
* Return value  : Result of function execution
******************************************************************************/
static flash_spi_status_t flash_spi_busy_check (
    uint8_t driver,                     /* channel number */
    flash_spi_poll_mode_t mode,         /* Poll mode */
    uint32_t loop_cnt                   /* loop count for 1ms */
)
{
    flash_spi_status_t ret = FLASH_SPI_SUCCESS_BUSY;

    do
    {
        /* FLASH is busy.
           User application can perform other processing while flash is busy. */

        ret = R_FLASH_SPI_Polling(driver, mode);
        if (FLASH_SPI_SUCCESS_BUSY != ret)
        {
            /* FLASH is ready or error. */
            break;
        }
        loop_cnt--;
        flash_spi_wait_time(driver, 1);     // 1 ms
    }
    while (0 != loop_cnt); /* WAIT_LOOP */

    return ret;
} /* End of function flash_spi_busy_check() */

/******************************************************************************
* Function Name : flash_spi_wait_time
* Description   : This function is used to wait for specified time
*               :    of the disk
* Arguments     : uint32_t channel       timer channel number
*                 uint32_t time          timeout(ms)
* Return value  : Result of function execution
******************************************************************************/
static int8_t flash_spi_wait_time (
    uint32_t channel,                    /* timer channel No. */
    uint32_t time                        /* timeout value */
)
{
    if (flash_spi_start_timer(channel, time) == FLASH_ERROR)
    {
        return FLASH_ERROR;
    }

    /* WAIT_LOOP */
    while (1)
    {
        /* Timeout? */
        if (FLASH_ERROR == flash_spi_check_timer(channel))
        {
            break;
        }
    }

    flash_spi_end_timer(channel);

    return FLASH_OK;
} /* End of function flash_spi_wait_time() */

/******************************************************************************
* Function Name : flash_spi_start_timer
* Description   : This function is used to start timer
*               :    of the disk
* Arguments     : uint32_t channel       timer channel number
*                 uint32_t msec          timeout(ms)
* Return value  : Result of function execution
******************************************************************************/
static int8_t flash_spi_start_timer (
    uint32_t channel,                    /* timer channel No. */
    uint32_t msec                        /* timeout value */
)
{
    if (TIMER_MAX_CH <= channel)
    {
        return FLASH_ERROR;
    }

    /* ---- Check argument ---- */
    if ((0 >= msec) || (msec > 0x0fffffff))
    {
        return FLASH_ERROR;
    }
    gs_timer_cnt_out[channel][TIMER_CH_FLG] = 1;
    gs_timer_cnt_out[channel][TIMER_CH_COUNT] = 0;
    gs_timer_cnt_out[channel][TIMER_CH_MAX_COUNT] = msec;

    return FLASH_OK;
} /* End of function flash_spi_start_timer() */

/******************************************************************************
* Function Name : flash_spi_check_timer
* Description   : This function is used to check timeout
*               :    of the disk
* Arguments     : uint32_t channel       timer channel number
* Return value  : Result of function execution
******************************************************************************/
static int8_t flash_spi_check_timer (
    uint32_t channel                   /* timer channel No. */
)
{
    int8_t ret = 0;

    /* ---- Check compare ---- */
    if (gs_timer_cnt_out[channel][TIMER_CH_COUNT] >= gs_timer_cnt_out[channel][TIMER_CH_MAX_COUNT])
    {
        ret = FLASH_ERROR;
    }
    else
    {
        ret = FLASH_OK;
    }

    return ret;
} /* End of function flash_spi_check_timer() */

/******************************************************************************
* Function Name : flash_spi_end_timer
* Description   : This function is used to end the timer
* Arguments     : uint32_t channel       timer channel number
* Return value  : Result of function execution
******************************************************************************/
static int8_t flash_spi_end_timer (
    uint32_t channel                   /* timer channel No. */
)
{
    gs_timer_cnt_out[channel][TIMER_CH_FLG] = 0;

    return FLASH_OK;
} /* End of function flash_spi_end_timer() */

#endif /* (TFAT_SERIAL_FLASH_DRIVE_NUM > 0) */
/******************************************************************************
End  Of File
******************************************************************************/
