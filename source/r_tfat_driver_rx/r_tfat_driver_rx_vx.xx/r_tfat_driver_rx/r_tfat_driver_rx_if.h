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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_tfat_driver_rx_if.h
* Description  : TFAT driver Interface.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 01.12.2014 1.00     First Release
*              : 05.01.2015 1.01     Added support USB Mini Firmware.
*              : 22.06.2015 1.02     Added support MCU RX231.
*              : 01.04.2016 1.03     Added support RX family.
*              : 29.06.2018 1.04     Modified SDHI to SDMEM.
*              : 08.08.2019 2.00     Added support for FreeRTOS and 
*                                    Renesas uITRON (RI600V4).
*                                    Added support for GNUC and ICCRX.
*              : 10.06.2020 2.10     Added support MMC Firmware and 
*                                    FLASH Firmware.
*              : 15.12.2023 2.40     Fixed to comply with GSCE Coding Standards Rev.6.5.0.
*******************************************************************************/
#ifndef _R_TFAT_DRIVER_RX_IF_H_
#define _R_TFAT_DRIVER_RX_IF_H_

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include "platform.h"
#include "ff.h"                  /* TFAT define */
#include "diskio.h"              /* TFAT define */

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* used memory define */
#define TFAT_CTRL_NONE          0
#define TFAT_CTRL_USB           1
#define TFAT_CTRL_SDMEM         2
#define TFAT_CTRL_MMC           3
#define TFAT_CTRL_USB_MINI      4
#define TFAT_CTRL_SERIAL_FLASH  5

/*******************************************************************************
Typedef definitions
*******************************************************************************/
/* Drive number define ("uint8_t drive") */
typedef enum
{
    TFAT_DRIVE_NUM_0 = 0x00,
    TFAT_DRIVE_NUM_1,
    TFAT_DRIVE_NUM_2,
    TFAT_DRIVE_NUM_3,
    TFAT_DRIVE_NUM_4,
    TFAT_DRIVE_NUM_5,
    TFAT_DRIVE_NUM_6,
    TFAT_DRIVE_NUM_7,
    TFAT_DRIVE_NUM_8,
    TFAT_DRIVE_NUM_9,
    TFAT_DRIVE_NUM_MAX,
}TFAT_DRV_NUM;

/*******************************************************************************
Exported global functions (to be accessed by other files)
*******************************************************************************/
extern DRESULT drv_change_alloc(TFAT_DRV_NUM tfat_drv,
                                uint8_t dev_type,
                                uint8_t dev_drv_num);
void disk_1ms_interval (void);
#endif    /* _R_TFAT_DRIVER_RX_IF_H_ */

/*******************************************************************************
End  of file
*******************************************************************************/
