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
* Copyright (C) 2015(2016-2020) Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_tfat_drv_if_usb_mini.c
* Description  : TFAT driver Interface for USB Mini Firmware.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 21.01.2015 1.00     First Release
*              : 22.06.2015 1.02     Added support MCU RX231.
*              : 01.04.2016 1.03     Updated the xml file.
*              : 08.08.2019 2.00     Added support for GNUC and ICCRX.
*              : 16.04.2020 2.10     Added support for FreeRTOS and 
*                                    Renesas uITRON (RI600V4).
*              : 10.09.2020 2.20     Added support for the format function.
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_tfat_drv_if_dev.h"
#include "r_tfat_driver_rx_config.h"

#if (TFAT_USB_MINI_DRIVE_NUM > 0)

#include "ff.h"                  /* TFAT define */
#include "diskio.h"              /* TFAT define */

#include "r_usb_basic_mini_if.h"
#include "r_usb_hmsc_mini_if.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
*******************************************************************************/

#if (BSP_CFG_RTOS_USED == 0)
static void R_usb_mini_hmsc_WaitLoop(void);
#endif /* BSP_CFG_RTOS_USED == 0 */

static uint16_t usb_ghmsc_tfatSecSize = 512;


/******************************************************************************
* Function Name : usb_mini_disk_initialize
* Description   : This function initializes the memory medium
*               :    for file operations
* Arguments     : uint8_t  drive        : Physical drive number
* Return value  : Status of the memory medium
******************************************************************************/
DSTATUS usb_mini_disk_initialize(uint8_t pdrv)
{
    return  RES_OK;
}

/******************************************************************************
* Function Name : usb_mini_disk_read
* Description   : This function reads data from the specified location
*               :    of the memory medium
* Arguments     : uint8_t drive          : Physical drive number
*               : uint8_t* buffer        : Pointer to the read data buffer
*               : uint32_t sector_number : uint32_t SectorNumber
*               : uint32_t sector_count  : Number of sectors to read
* Return value  : Result of function execution
******************************************************************************/
DRESULT usb_mini_disk_read (
    uint8_t drive,              /* Physical drive number            */
    uint8_t* buffer,            /* Pointer to the read data buffer  */
    uint32_t sector_number,     /* Start sector number              */
    uint32_t sector_count       /* Number of sectors to read        */
)
{
#if (BSP_CFG_RTOS_USED == 0)
    usb_tskinfo_t   *mess;
    usb_tskinfo_t   *mes;
#endif /* BSP_CFG_RTOS_USED == 0 */
    uint16_t        err;
    uint32_t        tran_byte;
    usb_info_t      info;
    usb_err_t       ret;

    /* set transfer length */
    tran_byte = (uint32_t)sector_count * usb_ghmsc_tfatSecSize;

#if (BSP_CFG_RTOS_USED == 0)
    ret = R_USB_GetInformation (&info);
    if ((USB_STS_CONFIGURED != info.status) || (USB_SUCCESS != ret))
    {
        return RES_ERROR;
    }

    /* read function */
    err = R_usb_hmsc_StrgReadSector((uint16_t)drive, buffer, sector_number, (uint16_t)sector_count, tran_byte);
    if ( USB_HMSC_OK == err )
    {
        do  /* Wait for complete R_usb_hmsc_StrgReadSector() */
        {
            ret = R_USB_GetInformation (&info);                         /* Get device connect state */
            R_usb_mini_hmsc_WaitLoop();                                 /* Task Schedule */
            err = R_USB_RCV_MSG(USB_HSTRG_MBX, (usb_msg_t**)&mess);     /* Receive read complete msg */
        }
        while ((USB_E_OK != err) && ((USB_STS_CONFIGURED == info.status) && (USB_SUCCESS == ret))); /* WAIT_LOOP */

        if ( err == USB_E_OK )
        {   /* Complete R_usb_hmsc_StrgReadSector() */
            mes = (usb_tskinfo_t *)mess;
            err = mes->keyword;
            R_USB_REL_BLK(mess->flag);
        }
        else
        {   /* Device detach */
            R_usb_mini_hmsc_WaitLoop();                                 /* Task Schedule */
            err = R_USB_RCV_MSG(USB_HSTRG_MBX, (usb_msg_t**)&mess);     /* Receive read complete msg */
            if ( USB_E_OK == err )
            {
                R_USB_REL_BLK(mess->flag);
            }
            err = USB_ERROR;
        }
    }

    if ( err != USB_E_OK )
    {
        return RES_ERROR;
    }
#else  /* BSP_CFG_RTOS_USED == 0 */
    ret = R_USB_GetInformation (&info);
    if ((USB_STS_CONFIGURED != info.status) || (USB_SUCCESS != ret))
    {
        return RES_ERROR;
    }

    /* read function */
    err = R_usb_hmsc_StrgReadSector((uint16_t)drive, buffer, sector_number, (uint16_t) sector_count, tran_byte);
    if ( USB_OK != err)
    {
        return RES_ERROR;
    }
#endif /* BSP_CFG_RTOS_USED == 0 */

    return RES_OK;
}

/******************************************************************************
* Function Name : usb_mini_disk_write
* Description   : This function writes data to a specified location
*               :    of the memory medium
* Arguments     : uint8_t drive          : Physical drive number
*               : const uint8_t* buffer  : Pointer to the write data
*               : uint32_t sector_number : Sector number to write
*               : uint32_t sector_count  : Number of sectors to write
* Return value  : Result of function execution
******************************************************************************/
DRESULT usb_mini_disk_write (
    uint8_t drive,               /* Physical drive number           */
    const uint8_t* buffer,       /* Pointer to the write data       */
    uint32_t sector_number,      /* Sector number to write          */
    uint32_t sector_count        /* Number of sectors to write      */
)
{
#if (BSP_CFG_RTOS_USED == 0)
    usb_tskinfo_t   *mess;
    usb_tskinfo_t   *mes;
#endif /* BSP_CFG_RTOS_USED == 0 */
    uint16_t        err;
    uint32_t        tran_byte;
    usb_info_t      info;
    usb_err_t       ret;

    /* set transfer length */
    tran_byte = (uint32_t)sector_count * usb_ghmsc_tfatSecSize;

#if (BSP_CFG_RTOS_USED == 0)
    ret = R_USB_GetInformation (&info);
    if ((USB_STS_CONFIGURED != info.status) || (USB_SUCCESS != ret))
    {
        return RES_ERROR;
    }

    /* write function */
    err = R_USB_HmscStrgWriteSector((uint16_t)drive, (uint8_t *)buffer
                                     , sector_number, (uint16_t)sector_count, tran_byte);
    if ( USB_HMSC_OK == err )
    {
        do  /* Wait for complete R_USB_HmscStrgWriteSector() */
        {
            ret = R_USB_GetInformation (&info);                         /* Get device connect state */
            R_usb_mini_hmsc_WaitLoop();                                 /* Task Schedule */
            err = R_USB_RCV_MSG(USB_HSTRG_MBX, (usb_msg_t**)&mess);     /* Receive write complete msg */
        }
        while ((USB_E_OK != err) && ((USB_STS_CONFIGURED == info.status) && (USB_SUCCESS == ret))); /* WAIT_LOOP */

        if ( err == USB_E_OK )
        {   /* Complete R_usb_hmsc_StrgReadSector() */
            mes = (usb_tskinfo_t *)mess;
            err = mes->keyword;
            R_USB_REL_BLK(mess->flag);
        }
        else
        {   /* Device detach */
            R_usb_mini_hmsc_WaitLoop();                                 /* Task Schedule */
            err = R_USB_RCV_MSG(USB_HSTRG_MBX, (usb_msg_t**)&mess);     /* Receive read complete msg */
            if ( USB_E_OK == err )
            {
                R_USB_REL_BLK(mess->flag);
            }
            err = USB_ERROR;
        }
    }

    if ( err != USB_E_OK )
    {
        return RES_ERROR;
    }
#else  /* BSP_CFG_RTOS_USED == 0 */
    ret = R_USB_GetInformation (&info);
    if ((USB_STS_CONFIGURED != info.status) || (USB_SUCCESS != ret))
    {
        return RES_ERROR;
    }

    /* write function */
    err = R_USB_HmscStrgWriteSector((uint16_t)drive, (uint8_t *)buffer, sector_number, (uint16_t) sector_count,
                                    tran_byte);
    if ( USB_OK != err)
    {
        return RES_ERROR;
    }

#endif /* BSP_CFG_RTOS_USED == 0 */
    return RES_OK;
}

/******************************************************************************
* Function Name : usb_mini_disk_ioctl
* Description   : This function is used to execute memory operations
*               :    other than read\write
* Arguments     : uint8_t drive   : Drive number
*               : uint8_t command : Control command code
*               : void*   buffer  : Data transfer buffer
* Return value  : Result of function execution
******************************************************************************/
DRESULT usb_mini_disk_ioctl (
    uint8_t drive,               /* Drive number             */
    uint8_t command,             /* Control command code     */
    void* buffer                 /* Data transfer buffer     */
)
{
#if FF_USE_MKFS == 1
#if (BSP_CFG_RTOS_USED == 0)
    usb_tskinfo_t   *p_mess;
    usb_tskinfo_t   *p_mes;
#endif /* BSP_CFG_RTOS_USED == 0 */
    usb_info_t      info;
    usb_err_t       ret;
    uint32_t        num_blocks;
    uint32_t        block_length;
    uint16_t        err;
    uint8_t         buff[8];
#endif

    if ((NULL == buffer) && (CTRL_SYNC != command))
    {
        return RES_PARERR;
    }

    switch (command)
    {
        case CTRL_SYNC:

        break;

        case GET_SECTOR_COUNT:
        case GET_BLOCK_SIZE:
#if FF_USE_MKFS
            ret = R_USB_GetInformation (&info);
            if ((USB_STS_CONFIGURED != info.status) || (USB_SUCCESS != ret))
            {
                return RES_ERROR;
            }

#if (BSP_CFG_RTOS_USED == 0)
            /* Get format capacities */
            err = usb_hmsc_strg_user_command(USB_ATAPI_READ_CAPACITY, buff, (usb_hmsc_cb_t)usb_hstd_dummy_function);
            if (USB_OK != err)
            {
                return RES_ERROR;
            }

            do  /* Wait for complete usb_hmsc_strg_user_command() */
            {
                ret = R_USB_GetInformation (&info);                         /* Get device connect state */
                /*
                 * The completion judgment of usb_hmsc_strg_user_command() is the message sent by getting
                 * the memory block of the previous stage. R_usb_mini_hmsc_WaitLoop() and R_USB_RCV_MSG()
                 * were required to receive the message.
                 */
                R_usb_mini_hmsc_WaitLoop();                                 /* Task Schedule */
                err = R_USB_RCV_MSG(USB_HSTRG_MBX, (usb_msg_t**)&p_mess);   /* Receive capacities complete msg */
            }
            while ((USB_E_OK != err) && ((USB_STS_CONFIGURED == info.status) && (USB_SUCCESS == ret))); /* WAIT_LOOP */

            if (USB_E_OK == err)
            {   /* Complete usb_hmsc_strg_user_command() */
                p_mes = (usb_tskinfo_t *)p_mess;
                err = p_mes->keyword;
                R_USB_REL_BLK(p_mess->flag);
            }
            else
            {   /* Device detach */
                R_usb_mini_hmsc_WaitLoop();                                 /* Task Schedule */
                err = R_USB_RCV_MSG(USB_HSTRG_MBX, (usb_msg_t**)&p_mess);   /* Receive capacities complete msg */
                if (USB_E_OK == err)
                {
                    R_USB_REL_BLK(p_mess->flag);
                }
                err = USB_ERROR;
            }
#else /* (BSP_CFG_RTOS_USED == 0) */
            err = usb_hmsc_strg_user_command(USB_ATAPI_READ_CAPACITY, buff);
            if (USB_OK != err)
            {
                return RES_ERROR;
            }
#endif /* BSP_CFG_RTOS_USED == 0 */

            /* eight bytes of READ CAPACITY data */
            block_length = (uint32_t) buff[7];
            block_length |= ((uint32_t) buff[6] << 8);
            block_length |= ((uint32_t) buff[5] << 16);
            block_length |= ((uint32_t) buff[4] << 24);
            block_length = block_length / USB_HMSC_STRG_SECTSIZE;

            if (GET_SECTOR_COUNT == command)
            {
                num_blocks = (uint32_t) buff[3];
                num_blocks |= ((uint32_t) buff[2] << 8);
                num_blocks |= ((uint32_t) buff[1] << 16);
                num_blocks |= ((uint32_t) buff[0] << 24);
                num_blocks = (num_blocks + 1) * block_length;
                /* Get USB mini number of blocks */
                ((uint32_t *)buffer)[0] = num_blocks;
            }
            else if (GET_BLOCK_SIZE == command)
            {
                /* Get USB mini block length */
                ((uint32_t *)buffer)[0] = block_length;
            }
            else
            {
                return RES_ERROR;
            }
#else
        return RES_PARERR;
#endif
        break;

        case GET_SECTOR_SIZE:
#if FF_MAX_SS == FF_MIN_SS
            return RES_PARERR;
#else
            ((uint32_t *)buffer)[0] = (uint32_t)USB_HMSC_STRG_SECTSIZE;
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

/******************************************************************************
* Function Name : usb_mini_disk_status
* Description   : This function is used to retrieve the current status
*               :    of the disk
* Arguments     : uint8_t drive : Physical drive number
* Return value  : Status of the disk
******************************************************************************/
DSTATUS usb_mini_disk_status (
    uint8_t drive                 /* Physical drive number    */
)
{

    /*  Please put the code for disk_status driver interface
         function over here.  */
    /*  Please refer the application note for details.  */
    return RES_OK;
}

#if (BSP_CFG_RTOS_USED == 0)
/******************************************************************************
Function Name   : R_usb_mini_hmsc_WaitLoop
Description     : Hmsc wait loop function
Arguments       : none
Return value    : none
******************************************************************************/
void R_usb_mini_hmsc_WaitLoop(void)
{
    if ( R_usb_cstd_Scheduler() == USB_FLGSET )
    {
        R_usb_hstd_HcdTask();
        R_usb_hstd_MgrTask();
        R_usb_hmsc_Task();
    }
}
#endif /* BSP_CFG_RTOS_USED == 0 */

#endif // (TFAT_USB_MINI_DRIVE_NUM > 0)
/******************************************************************************
End  of file
******************************************************************************/
