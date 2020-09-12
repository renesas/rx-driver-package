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
 * Copyright (C) 2014(2018) Renesas Electronics Corporation. All rights reserved.
 ******************************************************************************/
/*******************************************************************************
* File Name    : r_usb_hmsc_mini_if.h
* Version      : 1.10
* Description  : Interface file for USB host MSC
*******************************************************************************/
/*******************************************************************************
 * History   : DD.MM.YYYY Version Description
 *           : 01.09.2014 1.00    First Release
 *           : 01.06.2015 1.01    Added RX231.
 *           : 30.11.2018 1.10    Supporting Smart Configurator
 *******************************************************************************/

/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_hmsc.h"
#include "r_usb_hmsc_mini_config.h"

#ifndef R_USB_HMSC_MINI_IF_H
#define R_USB_HMSC_MINI_IF_H

/*******************************************************************************
 Macro definitions
 ******************************************************************************/

/* Version Number of API. */
#define RX_USB_HMSC_API_VERSION_MAJOR     (1)
#define RX_USB_HMSC_API_VERSION_MINOR     (0)

#define USB_HMSC_DEV_DET    (0x00)      /* detached device */
#define USB_HMSC_DEV_ATT    (0x01)      /* attached device */
#define USB_HMSC_DEV_ENU    (0x02)      /* Device enumeration */

/* subClass code */
#define USB_ATAPI_MMC5      (0x02)     /* ATAPI for CD-ROMs.  SFF-8020i */
#define USB_ATAPI           (0x05)     /* ATAPI for Floppies. SFF-8070i */
#define USB_SCSI            (0x06)     /* SCSI transparent command set */

/* Protocol code */
#define USB_BOTP            (0x50)     /* BOT:Bulk Only Transport */

#define USB_TOTALEP         (0x02)     /* Total EP Num */

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

typedef enum
{
    /*--- SFF-8070i command define ---*/
    USB_ATAPI_TEST_UNIT_READY       = (0x00u),
    USB_ATAPI_REQUEST_SENSE         = (0x03u),
    USB_ATAPI_FORMAT_UNIT           = (0x04u),
    USB_ATAPI_INQUIRY               = (0x12u),
    USB_ATAPI_MODE_SELECT6          = (0x15u),
    USB_ATAPI_MODE_SENSE6           = (0x1Au),
    USB_ATAPI_START_STOP_UNIT       = (0x1Bu),
    USB_ATAPI_PREVENT_ALLOW         = (0x1Eu),
    USB_ATAPI_READ_FORMAT_CAPACITY  = (0x23u),
    USB_ATAPI_READ_CAPACITY         = (0x25u),
    USB_ATAPI_READ10                = (0x28u),
    USB_ATAPI_WRITE10               = (0x2Au),
    USB_ATAPI_SEEK                  = (0x2Bu),
    USB_ATAPI_WRITE_AND_VERIFY      = (0x2Eu),
    USB_ATAPI_VERIFY10              = (0x2Fu),
    USB_ATAPI_MODE_SELECT10         = (0x55u),
    USB_ATAPI_MODE_SENSE10          = (0x5Au),
}usb_atapi_t;

typedef enum
{
    USB_CSW_SUCCESS = (0x00u),
    USB_CSW_FAIL    = (0x01u),
    USB_CSW_PHASE   = (0x02u),
}usb_csw_result_t;

/*******************************************************************************
 Exported global variables
 ******************************************************************************/

/*******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/

usb_err_t   R_USB_HmscStrgCmd (uint8_t *p_buf, uint16_t command);

uint16_t    R_usb_hmsc_StrgReadSector (uint16_t side, uint8_t *p_buff
                , uint32_t secno, uint16_t seccnt, uint32_t trans_byte);
uint16_t    R_usb_hmsc_StrgWriteSector (uint16_t side, uint8_t *p_buff
                , uint32_t secno, uint16_t seccnt, uint32_t trans_byte);


#endif  /* R_USB_HMSC_MINI_IF_H */
/******************************************************************************
End of file
******************************************************************************/
