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
 * Copyright (C) 2014(2020) Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name     : r_media_driver_api.c
 * Description   : This is the API interface between the application and 
 *                 block media device drivers.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 08.01.2014 1.00 First Release
 *         : 26.12.2014 1.10 RX71M is added
 *         : 30.09.2015 1.11 RX63N/RX631 is added.
 *         : 30.09.2016 1.20 RX65N/RX651 is added.
 *         : 30.09.2017 1.22 Delete LUN List.
 *         : 31.03.2018 1.23 Supporting Smart Configurator
 *         : 01.03.2020 1.30 RX72N/RX66N is added and uITRON is supported.
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
#include <stdint.h>
#include <stddef.h>

#include "r_usb_basic_if.h"
#include "r_usb_typedef.h"
#include "r_usb_extern.h"
#include "r_usb_bitdefine.h"
#include "r_usb_reg_access.h"

#include "r_usb_media_driver_if.h"
#include "r_usb_patapi.h"

/***********************************************************************************************************************
Private global variables and functions
 ***********************************************************************************************************************/
/* 
 Media driver list
 This array is defined to hold a set of pointers to media_driver_t type structures. 
 This provides a means to manage multiple media device drivers in the same system. 
 An individual media driver is accessed by means of indexing into this array by use of the logical unit number (lun)
 parameter of the media driver API function call.
 The maximum number of drivers loaded at any given time is limited by the user configurable value MAX_NUM_LUNS.
 This list may contain duplicate pointers to the same driver in order to support multiple media devices of the same type.
 */
static usb_media_driver_t * g_p_mediadriverlist = USB_NULL;


/***********************************************************************************************************************
 * Function Name: R_USB_media_initialize
 * Description  : This function initializes data structures and variables that 
 *                are used by the media device to support its operation for the 
 *                first time.
 * Arguments    : usb_media_driver_t * p_media_driver - 
 *                    initializes data structures
 * Return value : result -
 *                    USB_MEDIA_RET_OK:      Success
 *                    USB_MEDIA_RET_PARERR:  Invalid parameter error.
 *                    USB_MEDIA_RET_OP_FAIL: Any other failures
 ***********************************************************************************************************************/
bool R_USB_media_initialize (usb_media_driver_t * p_media_driver)
{
    bool result;

    if (USB_NULL != p_media_driver)
    {
        g_p_mediadriverlist = p_media_driver;
        result = true;
    }
    else
    {
        result = false;
    }

    return result;
} /* End of function R_USB_media_initialize() */


/***********************************************************************************************************************
 * Function Name: R_USB_media_open
 * Description  : This function 
 * Arguments    : none
 * Return value : result -
 *                    USB_MEDIA_RET_OK:      Success
 *                    USB_MEDIA_RET_PARERR:  Invalid parameter error.
 *                    USB_MEDIA_RET_OP_FAIL: Any other failures
 ***********************************************************************************************************************/
usb_media_ret_t R_USB_media_open(void)
{
    usb_media_ret_t result;

    result = g_p_mediadriverlist->pf_media_open();

    return result;
} /* End of function R_USB_media_open() */

/***********************************************************************************************************************
 * Function Name: R_USB_media_close
 * Description  : This function 
 * Arguments    : none
 * Return value : result -
 *                    USB_MEDIA_RET_OK:      Success
 *                    USB_MEDIA_RET_PARERR:  Invalid parameter error.
 *                    USB_MEDIA_RET_OP_FAIL: Any other failures
 ***********************************************************************************************************************/
usb_media_ret_t R_USB_media_close(void)
{
    usb_media_ret_t result;

    result = g_p_mediadriverlist->pf_media_close();

    return result;
} /* End of function R_USB_media_close() */

/***********************************************************************************************************************
 * Function Name: R_USB_media_read
 * Description  : This function reads data from a specified location of the 
 *                storage medium
 * Arguments    : p_wbuffer -
 *                    Pointer to the read data buffer.
 *              : start_block -
 *                    Start block number.
 *              : block_count -
 *                    Number of blocks to read.
 * Return value : result -
 *                    USB_MEDIA_RET_OK:      Success
 *                    USB_MEDIA_RET_PARERR:  Invalid parameter error
 *                    USB_MEDIA_RET_RWERR:   Read/Write Error
 *                    USB_MEDIA_RET_NOTRDY:  Not Ready 
 *                    USB_MEDIA_RET_OP_FAIL: Any other failures
 ***********************************************************************************************************************/
usb_media_ret_t R_USB_media_read(uint8_t* p_rbuffer, uint32_t start_block, uint8_t block_count)
{
    usb_media_ret_t result;

    result = g_p_mediadriverlist->pf_media_read(p_rbuffer, start_block, block_count);
    return result;
} /* End of function R_USB_media_read() */

/***********************************************************************************************************************
 * Function Name: R_USB_media_write
 * Description  : This function writes data to a specified location of the 
 *                storage medium
 * Arguments    : p_wbuffer -
 *                    Pointer to the write data buffer.
 *              : start_block -
 *                    Start block number.
 *              : block_count -
 *                    Number of blocks to write.
 * Return value : result -
 *                    USB_MEDIA_RET_OK:      Success
 *                    USB_MEDIA_RET_PARERR:  Invalid parameter error
 *                    USB_MEDIA_RET_RWERR:   Read/Write Error
 *                    USB_MEDIA_RET_WRPRT:   Write Protected 
 *                    USB_MEDIA_RET_NOTRDY:  Not Ready 
 *                    USB_MEDIA_RET_OP_FAIL: Any other failures
 ***********************************************************************************************************************/
usb_media_ret_t R_USB_media_write(uint8_t* p_wbuffer, uint32_t start_block, uint8_t block_count)
{
    usb_media_ret_t result;

    result = g_p_mediadriverlist->pf_media_write(p_wbuffer, start_block, block_count);
    return result;
} /* End of function R_USB_media_write() */

/***********************************************************************************************************************
 * Function Name: R_USB_media_ioctl
 * Description  : This function provides a generalized means to pass special 
 *                command and control instructions to the media driver and for
 *                the driver to return information.
 * Arguments    : ioctl_cmd -
 *                    Control command code.
 *              : ioctl_data -
 *                    Data transfer buffer
 * Return value : result -
 *                   USB_MEDIA_RET_OK:       Success
 *                   USB_MEDIA_RET_PARERR:   Invalid parameter error
 *                   USB_MEDIA_RET_NOTRDY:   Not Ready 
 *                   USB_MEDIA_RET_OP_FAIL:  Any other failures
 ***********************************************************************************************************************/
usb_media_ret_t R_USB_media_ioctl(usb_ioctl_cmd_t ioctl_cmd, void * ioctl_data)
{
    usb_media_ret_t result;

    result = g_p_mediadriverlist->pf_media_ctrl(ioctl_cmd, ioctl_data);

    return result;
} /* End of function R_USB_media_ioctl() */

/***********************************************************************************************************************
 End Of File
 ***********************************************************************************************************************/
