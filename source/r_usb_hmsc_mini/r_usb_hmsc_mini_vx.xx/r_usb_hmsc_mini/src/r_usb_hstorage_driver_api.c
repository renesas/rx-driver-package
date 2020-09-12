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
/******************************************************************************
 * File Name    : r_usb_hstorage_driver_api.c
 * Description  : USB Host Storage Driver API
 ******************************************************************************/
/******************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.09.2014 1.00    First Release
 *         : 01.06.2015 1.01    Added RX231.
 *         : 30.11.2018 1.10    Supporting Smart Configurator
 ******************************************************************************/

/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_hmsc_mini_if.h"

/*******************************************************************************
 Macro definitions
 ******************************************************************************/

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/*******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/


/******************************************************************************
 * Function Name: usb_hmsc_strg_read_sector
 * Description  : Releases drive
 * Arguments    : uint16_t side        : Side
                : uint8_t  *p_buff     : Buffer address
                : uint32_t secno       : Sector number
                : uint16_t seccnt      : Sector count
                : uint32_t trans_byte  : Trans byte
 * Return Value : uint16_t             : [DONE/ERROR]
 ******************************************************************************/
uint16_t usb_hmsc_strg_read_sector (uint16_t side, uint8_t *p_buff
    , uint32_t secno, uint16_t seccnt, uint32_t trans_byte)
{
    uint16_t    result;

    /* Device Status */
    if (USB_HMSC_DEV_ATT != usb_hmsc_get_device_status())
    {
        return (USB_ERROR);
    }

    result = usb_hmsc_read10 (p_buff, secno, seccnt, trans_byte);

    return (result);
}   /* eof usb_hmsc_strg_read_sector () */

/******************************************************************************
 * Function Name: usb_hmsc_strg_write_sector
 * Description  : Writes sector information
 * Arguments    : uint16_t side        : Side
                : uint8_t  *p_buff     : Buffer address
                : uint32_t secno       : Sector number
                : uint16_t seccnt      : Sector count
                : uint32_t trans_byte  : Trans byte
 * Return Value : uint16_t             : [DONE/ERROR]
 ******************************************************************************/
uint16_t usb_hmsc_strg_write_sector (uint16_t side, uint8_t *p_buff
    , uint32_t secno, uint16_t seccnt, uint32_t trans_byte)
{
    uint16_t    result;

    /* Device Status */
    if (USB_HMSC_DEV_ATT != usb_hmsc_get_device_status())
    {
        return (USB_ERROR);
    }

    result = usb_hmsc_write10 (p_buff, secno, seccnt, trans_byte);
    return (result);
}   /* eof usb_hmsc_strg_write_sector () */

/******************************************************************************
End  Of File
 ******************************************************************************/
