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
* Copyright (C) 2014(2015-2020) Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_tfat_drv_if_usb.c
* Description  : TFAT driver Interface for USB.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 01.12.2014 1.00     First Release
*              : 22.06.2015 1.02     Added support MCU RX231.
*              : 01.04.2016 1.03     Updated the xml file.
*              : 14.12.2018 1.05     Supporting USB driver for RTOS
*              : 08.08.2019 2.00     Added support for FreeRTOS and 
*                                    Renesas uITRON (RI600V4).
*                                    Added support for GNUC and ICCRX.
*              : 10.09.2020 2.20     Added support for the format function.
*              : 15.12.2023 2.40     Fixed to comply with GSCE Coding Standards
*                                    Rev.6.5.0.
*              : 30.09.2024 2.60     Changed the comment of API functions to
*                                    the Doxygen style.
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_tfat_drv_if_dev.h"
#include "r_tfat_driver_rx_config.h"

#if (TFAT_USB_DRIVE_NUM > 0)

#include "r_usb_basic_if.h"
#include "ff.h"                  /* TFAT define */
#include "diskio.h"              /* TFAT define */
#include "r_usb_hmsc_if.h"       /* version info. */

/* version info. */
#ifndef USB_VERSION_MAJOR
#include "r_usb_if.h" /* version info. */
#endif
#ifndef RX_USB_HMSC_API_VERSION_MAJOR
#include "r_usb_hmsc.h" /* version info. */
#endif

#ifdef BSP_MCU_RX62N
#define TFAT_DRV_USB_API_TYPE2
//#define TFAT_DRV_USB_API_TYPE3  /* for USB v2.30 */
#else
#if (RX_USB_HMSC_API_VERSION_MAJOR == 1) && (RX_USB_HMSC_API_VERSION_MINOR == 00)
#define TFAT_DRV_USB_API_TYPE1
#elif (USB_VERSION_MAJOR == 1) && (USB_VERSION_MINOR <= 19) && (RX_USB_HMSC_API_VERSION_MAJOR == 1) && \
    (RX_USB_HMSC_API_VERSION_MINOR <= 10)
#define TFAT_DRV_USB_API_TYPE2
#else
#define TFAT_DRV_USB_API_TYPE3
#endif
#endif

#if defined(TFAT_DRV_USB_API_TYPE1) || defined(TFAT_DRV_USB_API_TYPE2)
#include "r_usb_hatapi_define.h"    /* Peripheral ATAPI Device extern */
#include "r_usb_hmsc_extern.h"
#include "r_usb_hmsc_api.h"
#include "r_usb_hmsc_config.h"
#else


#include "r_usb_typedef.h"
#include "r_usb_extern.h"
#include "r_usb_hmsc.h"
#endif

#ifdef TFAT_DRV_USB_API_TYPE1

#define usb_hmsc_smp_drive2_addr    usb_hmsc_SmpDrive2Addr

#define R_USB_HmscStrgReadSector    R_usb_hmsc_StrgReadSector
#define USB_FALSE   USB_DETACHED
#define R_USB_HmscStrgWriteSector   R_usb_hmsc_StrgWriteSector
//#define USB_TRCV_MSG              R_USB_TRCV_MSG
#define USB_REL_BL                  R_USB_REL_BLK
#define usb_cstd_check_schedule     R_usb_cstd_CheckSchedule
#define usb_hstd_hcd_task           R_usb_hstd_HcdTask
#define usb_hstd_mgr_task           R_usb_hstd_MgrTask
#define usb_hhub_task               R_usb_hhub_Task
#define R_USB_HmscTask              usb_hmsc_Task
#define usb_cstd_scheduler          R_usb_cstd_Scheduler

typedef USB_UTR_t usb_utr_t;
typedef USB_MH_t usb_mh_t;
typedef USB_MSG_t usb_msg_t;
typedef USB_VP_INT usb_vp_int_t;

#elif defined(TFAT_DRV_USB_API_TYPE2)

#define usb_hmsc_smp_drive2_addr    usb_hmsc_SmpDrive2Addr

#define R_USB_HmscStrgReadSector    R_usb_hmsc_StrgReadSector
#define USB_FALSE   USB_DETACHED
#define R_USB_HmscStrgWriteSector   R_usb_hmsc_StrgWriteSector
//#define USB_TRCV_MSG                R_USB_TRCV_MSG
#define USB_REL_BL                  R_USB_REL_BLK
#define usb_cstd_check_schedule     R_usb_cstd_CheckSchedule
#define usb_hstd_hcd_task           R_usb_hstd_HcdTask
#define usb_hstd_mgr_task           R_usb_hstd_MgrTask
#define usb_hhub_task               R_usb_hhub_Task
#define R_USB_HmscTask              usb_hmsc_Task
#define usb_cstd_scheduler          R_usb_cstd_Scheduler

typedef USB_UTR_t usb_utr_t;
typedef USB_MH_t usb_mh_t;
typedef USB_MSG_t usb_msg_t;
typedef USB_VP_INT usb_vp_int_t;

#else /* TFAT_DRV_USB_API_TYPE3 */
extern void usb_hmsc_smp_drive2_addr (uint16_t side, usb_utr_t *devadr);
#endif

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
static void R_usb_hmsc_WaitLoop (void);
#endif /* (BSP_CFG_RTOS_USED == 0) */
static uint16_t g_usb_hmsc_tfatSecSize = 512;

#ifdef TFAT_DRV_USB_API_TYPE1
usb_utr_t tfat_ptr;
#endif


/**********************************************************************************************************************
* Function Name: usb_disk_initialize
*******************************************************************************************************************//**
* @brief This function initializes the disk drive.
*
* @param[in] pdrv  Specifies the initialize drive number.
*
* @retval TFAT_RES_OK  Normal termination.
*
* @retval Others       DSTATUS status of the disk after function execution.
*
* @details This API does not call USB driver initialize function because of USB driver limitation
* (1 time call is only accepted). Please call USB driver initialize function in user program.
* @note None
*/
DSTATUS usb_disk_initialize (uint8_t pdrv)
{
    return RES_OK;
}


/**********************************************************************************************************************
* Function Name: usb_disk_read
*******************************************************************************************************************//**
* @brief This function reads the data from disk.
*
* @param[in]  pdrv    Specifies the physical drive number.
*
* @param[out] buff    Pointer to the read buffer to store the read data. A buffer of the size equal to the
* number of bytes to be read is required.
*
* @param[in]  sector  Specifies the start sector number in logical block address (LBA).
*
* @param[in]  count   Specifies number of sectors to read. The value can be 1 to 255.
*
* @retval DRESULT  Result of the function execution.
*
* @details This function reads the data from disk drive. The position of read data is specified using
* this function argument.
* @note None
*/
DRESULT usb_disk_read (
    uint8_t pdrv,               /* Physical drive number            */
    uint8_t* buff,              /* Pointer to the read data buffer  */
    uint32_t sector,            /* Start sector number              */
    uint32_t count              /* Number of sectors to read        */
)
{
#if (BSP_CFG_RTOS_USED == 0)
    uint16_t res[10];
    usb_utr_t *mess;
#endif /* (BSP_CFG_RTOS_USED == 0) */
    uint16_t err;
    uint32_t tran_byte;
#ifndef TFAT_DRV_USB_API_TYPE1
    usb_utr_t tfat_ptr;
#endif

    /* set transfer length */
    tran_byte = (uint32_t) count * g_usb_hmsc_tfatSecSize;

#ifdef TFAT_DRV_USB_API_TYPE1
    usb_hmsc_smp_drive2_addr(pdrv);             /* Drive no. -> USB IP no. and IO Reg Base address */
#else
    usb_hmsc_smp_drive2_addr(pdrv, &tfat_ptr);  /* Drive no. -> USB IP no. and IO Reg Base address */
#endif
    /* Check Detach */
#if defined(TFAT_DRV_USB_API_TYPE1) || defined(TFAT_DRV_USB_API_TYPE2)
    R_usb_hstd_DeviceInformation(&tfat_ptr, tfat_ptr.keyword, (uint16_t *)res);         /* Get device connect state */
    if ( USB_FALSE == res[1] )
#else
    if (USB_FALSE == R_USB_HmscGetDevSts(pdrv))
#endif
    {
        return RES_ERROR;
    }

    /* read function */
    err = R_USB_HmscStrgReadSector(&tfat_ptr, (uint16_t) pdrv, buff, sector, (uint16_t) count, tran_byte);
    if ( USB_OK != err)
    {
        return RES_ERROR;
    }

#if (BSP_CFG_RTOS_USED == 0)
    /* Wait USB read sequence(READ10) */
    do
    {
        /* Check Detach */
#if defined(TFAT_DRV_USB_API_TYPE1) || defined(TFAT_DRV_USB_API_TYPE2)
        R_usb_hstd_DeviceInformation(&tfat_ptr, tfat_ptr.keyword, (uint16_t *)res); /* Get device connect state */
#else
        res[1] = R_USB_HmscGetDevSts(pdrv);
#endif
        R_usb_hmsc_WaitLoop(); /* Task Schedule */
        err = USB_TRCV_MSG(USB_HSTRG_MBX, (usb_msg_t** ) & mess, (uint16_t )0); /* Receive read complete msg */
    }
    while ((err != USB_OK) && (res[1] != USB_FALSE)); /* WAIT_LOOP */

    if (err == USB_OK)
    { /* Complete R_USB_HmscStrgReadSector() */
        err = mess->result; /* Set result for R_USB_HmscStrgReadSector() */
        USB_REL_BLK(USB_HSTRG_MPL, (usb_mh_t )mess);
    }
    else
    { /* Device detach */
        R_usb_hmsc_WaitLoop(); /* Task Schedule */
        err = USB_TRCV_MSG(USB_HSTRG_MBX, (usb_msg_t** ) & mess, (uint16_t )0); /* Receive read complete msg */
        if ( USB_OK == err)
        {
            USB_REL_BLK(USB_HSTRG_MPL, (usb_mh_t )mess);
        }
        err = USB_ERROR;
    }

    if (err != USB_OK)
    {
        return RES_ERROR;
    }
#endif /* (BSP_CFG_RTOS_USED == 0) */

    return RES_OK;
}


/**********************************************************************************************************************
* Function Name: usb_disk_write
*******************************************************************************************************************//**
* @brief This function writes the data to the disk.
*
* @param[in] pdrv    Specifies the physical drive number.
*
* @param[in] buff    Pointer to the data to be written.
*
* @param[in] sector  Specifies the start sector number in logical block address (LBA).
*
* @param[in] count   Specifies number of sectors to read. The value can be 1 to 255.
*
* @retval DRESULT  Result of the function execution.
*
* @details This function writes the data to the disk drive. The position of write data is specified using
* this function argument.
* @note None
*/
DRESULT usb_disk_write (
    uint8_t pdrv,           /* Physical drive number           */
    const uint8_t* buff,    /* Pointer to the write data       */
    uint32_t sector,        /* Sector number to write          */
    uint32_t count          /* Number of sectors to write      */
)
{
#if (BSP_CFG_RTOS_USED == 0)
    uint16_t res[10];
    usb_utr_t *mess;
#endif /* (BSP_CFG_RTOS_USED == 0) */
    uint16_t err;
    uint32_t tran_byte;
#ifndef TFAT_DRV_USB_API_TYPE1
    usb_utr_t tfat_ptr;
#endif

    /* set transfer length */
    tran_byte = (uint32_t) count * g_usb_hmsc_tfatSecSize;
#ifdef TFAT_DRV_USB_API_TYPE1
    usb_hmsc_smp_drive2_addr(pdrv);             /* Drive no. -> USB IP no. and IO Reg Base address */
#else
    usb_hmsc_smp_drive2_addr(pdrv, &tfat_ptr);  /* Drive no. -> USB IP no. and IO Reg Base address */
#endif

    /* Check Detach */
#if defined(TFAT_DRV_USB_API_TYPE1) || defined(TFAT_DRV_USB_API_TYPE2)
    R_usb_hstd_DeviceInformation(&tfat_ptr, tfat_ptr.keyword, (uint16_t *)res);         /* Get device connect state */
    if ( USB_FALSE == res[1] )
#else
    if (USB_FALSE == R_USB_HmscGetDevSts(pdrv))
#endif
    {
        return RES_ERROR;
    }

    /* write function */
    err = R_USB_HmscStrgWriteSector(&tfat_ptr, (uint16_t) pdrv, (uint8_t *) buff, sector, (uint16_t) count,
                                    tran_byte);
    if ( USB_OK != err)
    {
        return RES_ERROR;
    }

#if (BSP_CFG_RTOS_USED == 0)
    /* Wait USB write sequence(WRITE10) */
    do
    {
        /* Check Detach */
#if defined(TFAT_DRV_USB_API_TYPE1) || defined(TFAT_DRV_USB_API_TYPE2)
        R_usb_hstd_DeviceInformation(&tfat_ptr, tfat_ptr.keyword, (uint16_t *)res); /* Get device connect state */
#else
        res[1] = R_USB_HmscGetDevSts(pdrv);
#endif
        R_usb_hmsc_WaitLoop(); /* Task Schedule */
        err = USB_TRCV_MSG(USB_HSTRG_MBX, (usb_msg_t** ) & mess, (uint16_t )0); /* Receive write complete msg */
    }
    while ((err != USB_OK) && (res[1] != USB_FALSE)); /* WAIT_LOOP */

    if ( USB_OK == err)
    { /* Complete R_USB_HmscStrgWriteSector() */
        err = mess->result; /* Set result for R_USB_HmscStrgWriteSector() */
        USB_REL_BLK(USB_HSTRG_MPL, (usb_mh_t )mess);
    }
    else
    { /* Device detach */
        R_usb_hmsc_WaitLoop(); /* Task Schedule */
        err = USB_TRCV_MSG(USB_HSTRG_MBX, (usb_msg_t** ) & mess, (uint16_t )0); /* Receive write complete msg */
        if (err == USB_OK)
        {
            USB_REL_BLK(USB_HSTRG_MPL, (usb_mh_t )mess);
        }
        err = USB_ERROR;
    }

    if (err != USB_OK)
    {
        return RES_ERROR;
    }
#endif /* (BSP_CFG_RTOS_USED == 0) */

    return RES_OK;
}


/**********************************************************************************************************************
* Function Name: usb_disk_ioctl
*******************************************************************************************************************//**
* @brief This function controls the drive.
*
* @param[in] pdrv  Specifies the physical drive number.
*
* @param[in] cmd   Specifies the command code. The command code will always be 0.
*
* @param[in] buff  Pointer should always be a NULL pointer.
*
* @retval DRESULT  Result of the function execution.
*
* @details The usb_disk_ioctl function is used only by the f_sync function amongst all the TFAT FIT functions.
* Users who do not plan to use f_sync function in their applications can skip the implementation for this
* particular driver interface function.\n
* For users who wish to use f_sync function in their applications, the command CTRL_SYNC has to be implemented.\n
* For users who wish to use f_sync function in their applications, this particular driver interface function
* will have to be implemented. This driver function should consist of the code to finish off any pending write
* process. If the disk i/o module has a write back cache, the dirty sector must be flushed immediately.The f_sync
* function will perform a save operation to the unsaved data related to the file object passed as argument.
* @note None
*/
DRESULT usb_disk_ioctl (
    uint8_t pdrv,          /* Drive number             */
    uint8_t cmd,           /* Control command code     */
    void* buff             /* Data transfer buffer     */
)
{
#if FF_USE_MKFS == 1
#if (BSP_CFG_RTOS_USED == 0)
    usb_utr_t  *p_mess;
    uint16_t   res[10];
#endif /* (BSP_CFG_RTOS_USED == 0) */
    usb_utr_t  tfat_ptr;
    uint32_t   num_blocks;
    uint32_t   block_length;
    uint16_t   err;
    uint8_t    buffer[8];
#endif

    if ((NULL == buff) && (CTRL_SYNC != cmd))
    {
        return RES_PARERR;
    }

    switch (cmd)
    {
        case CTRL_SYNC:

        break;

        case GET_SECTOR_COUNT:
        case GET_BLOCK_SIZE:
#if FF_USE_MKFS
            /* Check Detach */
            if (USB_FALSE == R_USB_HmscGetDevSts(pdrv))
            {
                return RES_ERROR;
            }

            usb_hmsc_smp_drive2_addr(pdrv, &tfat_ptr);  /* Drive no. -> USB IP no. and IO Reg Base address */
            err = usb_hmsc_strg_user_command(&tfat_ptr, pdrv, USB_ATAPI_READ_CAPACITY, buffer, usb_hstd_dummy_function);
            if (USB_OK != err)
            {
                return RES_ERROR;
            }
#if (BSP_CFG_RTOS_USED == 0)
            /* Wait for complete usb_hmsc_strg_user_command() */
            do
            {
            /* Check Detach */
#if defined(TFAT_DRV_USB_API_TYPE1) || defined(TFAT_DRV_USB_API_TYPE2)
                R_usb_hstd_DeviceInformation(&tfat_ptr, tfat_ptr.keyword, (uint16_t *)res); /* Get device connect state */
#else
                res[1] = R_USB_HmscGetDevSts(pdrv);
#endif
                /*
                 * The completion judgment of usb_hmsc_strg_user_command() is the message sent by getting
                 * the memory block of the previous stage. R_usb_hmsc_WaitLoop() and R_USB_RCV_MSG()
                 * were required to receive the message.
                 */
                R_usb_hmsc_WaitLoop(); /* Task Schedule */
                err = USB_TRCV_MSG(USB_HSTRG_MBX, (usb_msg_t**) &p_mess, (uint16_t)0); /* Receive capacities complete msg */
            }
            while ((err != USB_OK) && (res[1] != USB_FALSE)); /* WAIT_LOOP */

            if (USB_OK == err)
            { /* Complete usb_hmsc_strg_user_command() */
                err = p_mess->result;
                USB_REL_BLK(USB_HSTRG_MPL, (usb_mh_t)p_mess);
            }
            else
            { /* Device detach */
                R_usb_hmsc_WaitLoop(); /* Task Schedule */
                err = USB_TRCV_MSG(USB_HSTRG_MBX, (usb_msg_t**) &p_mess, (uint16_t)0); /* Receive capacities complete msg */
                if (err == USB_OK)
                {
                    USB_REL_BLK(USB_HSTRG_MPL, (usb_mh_t)p_mess);
                }
                err = USB_ERROR;
            }
#endif /* (BSP_CFG_RTOS_USED == 0) */

            /* eight bytes of READ CAPACITY data */
            block_length = (uint32_t) buffer[7];
            block_length |= ((uint32_t) buffer[6] << 8);
            block_length |= ((uint32_t) buffer[5] << 16);
            block_length |= ((uint32_t) buffer[4] << 24);
            block_length = block_length / USB_HMSC_STRG_SECTSIZE;

            if (GET_SECTOR_COUNT == cmd)
            {
                num_blocks = (uint32_t) buffer[3];
                num_blocks |= ((uint32_t) buffer[2] << 8);
                num_blocks |= ((uint32_t) buffer[1] << 16);
                num_blocks |= ((uint32_t) buffer[0] << 24);
                num_blocks = (num_blocks + 1) * block_length;
                /* Get USB number of blocks */
                ((uint32_t *)buff)[0] = num_blocks;
            }
            else if (GET_BLOCK_SIZE == cmd)
            {
                /* Get USB block length */
                ((uint32_t *)buff)[0] = block_length;
            }
            else
            {
                return RES_ERROR;
            }

            return RES_OK;
#else
            return RES_PARERR;
#endif
        break;

        case GET_SECTOR_SIZE:
#if FF_MAX_SS == FF_MIN_SS
            return RES_PARERR;
#else
            ((uint32_t *)buff)[0] = (uint32_t)USB_HMSC_STRG_SECTSIZE;
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
* Function Name: usb_disk_status
*******************************************************************************************************************//**
* @brief This function gets the information about disk drive.
*
* @param[in] pdrv  Specifies the physical drive number.
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
DSTATUS usb_disk_status (uint8_t pdrv /* Physical drive number    */
                        )
{
    /*  Please put the code for disk_status driver interface
     function over here.  */
    /*  Please refer the application note for details.  */
    return RES_OK;
}

#if (BSP_CFG_RTOS_USED == 0)
/**********************************************************************************************************************
* Function Name: R_usb_hmsc_WaitLoop
*******************************************************************************************************************//**
* @brief This function waits for the data read/write.
*
* @details Please refer to the USB driver document for details.
*
* @note None
*/
void R_usb_hmsc_WaitLoop (void)
{
    if (usb_cstd_check_schedule() == USB_FLGSET)
    {
        usb_hstd_hcd_task((usb_vp_int_t) 0);
        usb_hstd_mgr_task((usb_vp_int_t) 0);
        usb_hhub_task((usb_vp_int_t) 0);
        R_USB_HmscTask(); /* HMSC Task */
    }
    usb_cstd_scheduler();
}
#endif /* (BSP_CFG_RTOS_USED == 0) */

#endif /* (TFAT_USB_DRIVE_NUM > 0) */
/******************************************************************************
End  of file
******************************************************************************/
