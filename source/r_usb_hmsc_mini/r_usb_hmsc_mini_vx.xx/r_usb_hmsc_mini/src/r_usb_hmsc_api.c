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
* File Name    : r_usb_hmsc_api.c
* Description  : USB Peripheral Sample Code
*******************************************************************************/
/*******************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 01.09.2014 1.00    First Release
 *         : 01.06.2015 1.01    Added RX231.
 *         : 29.12.2015 1.02    Minor Update.
 *         : 30.11.2018 1.10    Supporting Smart Configurator
 ******************************************************************************/

/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_hmsc.h"

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
Function Name   : R_USB_HmscStrgCmd
Description     : Processing for MassStorage(ATAPI) command.
Arguments       : uint8_t       *p_buf  : Pointer to the buffer area to store the transfer data
                : uint16_t      command : ATAPI command
Return value    : usb_err_t error code  : USB_SUCCESS,USB_ERR_NG etc.
******************************************************************************/
usb_err_t   R_USB_HmscStrgCmd (uint8_t *p_buf, uint16_t command)
{
    usb_info_t  info;
    uint16_t    ret;

    R_USB_GetInformation (&info);
    if (USB_STS_CONFIGURED != info.status)
    {
        return USB_ERR_NG;
    }

    ret = usb_hmsc_strg_user_command (command, p_buf, usb_hmsc_strg_cmd_complete);
#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
    if (USB_PAR == ret)
    {
        return USB_ERR_PARA;
    }
#endif /* #if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */

    if (USB_OK != ret)
    {
        return USB_ERR_NG;
    }

    return USB_SUCCESS;
}   /* End of function R_USB_HmscStrgCmd */


/******************************************************************************
 End  Of File
 ******************************************************************************/
