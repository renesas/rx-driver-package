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
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_usb_hstorage_driver_rtos.c
 * Description  : USB Host Storage Driver
 ***********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 30.06.2020 1.00    First Release
 ***********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_hmsc_mini_if.h"
#if   (BSP_CFG_RTOS_USED != 0)      /* RTOS */

#include <string.h>

#if   (BSP_CFG_RTOS_USED == 1)      /* FreeRTOS */
#include "FreeRTOS.h"
#endif  /* BSP_CFG_RTOS_USED == 1 */

#if defined(USB_CFG_HMSC_USE)

/******************************************************************************
Macro definitions
******************************************************************************/
#define RETRY_COUNT (10)

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
uint8_t g_usb_hmsc_data[512];

/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

/******************************************************************************
 Renesas Abstracted Peripheral Driver functions
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hmsc_error_process
 Description     : Error processing when CSW error is retuned by MSC device.
 Arguments       : uint16_t     err_code    : Error code
 Return value    : uint16_t                 : Error code
 ******************************************************************************/
uint16_t usb_hmsc_error_process (uint16_t err_code)
{
    uint16_t    retval;
    static uint8_t g_usb_hmsc_rs_data[USB_HMSC_REQUEST_SENSE_SIZE]; /* Request Sense Data Buffer */

    retval = err_code;

    if (USB_HMSC_CSW_ERR == err_code)
    {
        retval = usb_hmsc_request_sense(g_usb_hmsc_rs_data);
    }
    
    return retval;
}
/******************************************************************************
 End of function usb_hmsc_error_processing
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_strg_drive_search
 Description     : Searches drive SndMsg
 Arguments       : none
 Return value    : uint16_t                 : Status
 ******************************************************************************/
uint16_t usb_hmsc_strg_drive_search (void)
{
    uint16_t hmsc_retval;
    uint16_t count = 0;

    /* Get MAX_LUN */
    usb_hmsc_get_max_unit();

    usb_cpu_delay_xms(100);

    /* Inquiry */
    hmsc_retval = usb_hmsc_inquiry((uint8_t*) &g_usb_hmsc_data);
    if (USB_HMSC_OK != hmsc_retval)
    {
        hmsc_retval = usb_hmsc_error_process(hmsc_retval);
        if (USB_HMSC_OK != hmsc_retval)
        {
            return USB_ERROR;
        }
    }

    /* Read Capacity */
    hmsc_retval = usb_hmsc_read_capacity((uint8_t*) &g_usb_hmsc_data);
    if (USB_HMSC_OK != hmsc_retval)
    {
        hmsc_retval = usb_hmsc_error_process(hmsc_retval);
        if (USB_HMSC_OK != hmsc_retval)
        {
            return USB_ERROR;
        }

        /* WAIT_LOOP */
        while (1)
        {
            /* TestUnitReady */
            hmsc_retval = usb_hmsc_test_unit();

            if (USB_HMSC_OK == hmsc_retval)
            {
                /* Read Capacity (Retry) */
                hmsc_retval = usb_hmsc_read_capacity((uint8_t*) &g_usb_hmsc_data);
                break;
            }
            else
            {
                hmsc_retval = usb_hmsc_error_process(hmsc_retval);
                if (USB_HMSC_OK != hmsc_retval)
                {
                    return USB_ERROR;
                }
            }
        }
    }

    /* Don't delete the following processing!! */
    /* (This is necessary for the specific USB memory.) */
    /* Read10 secno = 0, seccnt = 1 */
    /* WAIT_LOOP */
    for (count = 0;; count++)
    {
        if (RETRY_COUNT != count)
        {
            hmsc_retval =  usb_hmsc_read10 ((uint8_t*)&g_usb_hmsc_data, 0, 1, USB_HMSC_STRG_SECTSIZE);
            if (USB_HMSC_OK == hmsc_retval)
            {
                break;
            }
        }
        else
        {
            hmsc_retval = usb_hmsc_error_process(hmsc_retval);
            if (USB_HMSC_OK != hmsc_retval)
            {
                return USB_ERROR;
            }
        }
    }

    return USB_OK;
}
/******************************************************************************
 End of function usb_hmsc_strg_drive_search
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_strg_drive_open
 Description     : Releases drive
 Arguments       : none
 Return value    : uint16_t                 : [OK/ERROR]
 ******************************************************************************/
uint16_t usb_hmsc_strg_drive_open (void)
{
    uint16_t retval;
    uint8_t *p_desc;
    uint16_t desc_len;

    p_desc = (uint8_t *) g_usb_hstd_config_descriptor;
    desc_len = ((uint16_t) *(p_desc + 3)) << 8;
    desc_len += (uint16_t) *(p_desc + 2);
    /* Pipe Information table set */
    retval = usb_hmsc_pipe_info(p_desc, desc_len);
    if (USB_ERROR != retval)
    {
        usb_cstd_pipe_reg_set();
        usb_hmsc_set_device_status(USB_HMSC_DEV_ATT);
    }

    return retval;
}
/******************************************************************************
 End of function usb_hmsc_strg_drive_open
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_strg_drive_close
 Description     : Releases drive
 Arguments       : none
 Return value    : uint16_t             : USB_OK
 ******************************************************************************/
uint16_t usb_hmsc_strg_drive_close (void)
{
    usb_hmsc_set_device_status(USB_HMSC_DEV_DET);

    return (USB_OK);
}
/******************************************************************************
 End of function usb_hmsc_strg_drive_close
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_strg_user_command
 Description     : USB Mass Storage Command
 Arguments       : uint16_t     command     : ATAPI command
                 : uint8_t      *p_buff     : Buffer address
 Return value    : uint16_t                 : [DONE/ERROR]
 ******************************************************************************/
uint16_t usb_hmsc_strg_user_command (uint16_t command, uint8_t *p_buff)
{
    uint16_t result;

    /* Device Status */
    if (USB_HMSC_DEV_ATT != usb_hmsc_get_device_status())
    {
        return (USB_ERROR);
    }

    switch (command)
    {
        case USB_ATAPI_TEST_UNIT_READY:             /* Test unit */
            result = usb_hmsc_test_unit();
            /* The result of the user command should be reported before returning this function */
        break;
        case USB_ATAPI_REQUEST_SENSE:               /*Request sense */
            result = usb_hmsc_request_sense(p_buff);
            /* The result of the user command should be reported before returning this function */
        break;
        case USB_ATAPI_INQUIRY:                     /* Inquiry */
            result = usb_hmsc_inquiry(p_buff);
            /* The result of the user command should be reported before returning this function */
        break;
        case USB_ATAPI_PREVENT_ALLOW:               /* Prevent allow */
            result = usb_hmsc_prevent_allow(p_buff);
            /* The result of the user command should be reported before returning this function */
        break;
        case USB_ATAPI_READ_FORMAT_CAPACITY:        /* Read format capacity */
            result = usb_hmsc_read_format_capacity(p_buff);
            /* The result of the user command should be reported before returning this function */
        break;
        case USB_ATAPI_READ_CAPACITY:               /* Read capacity */
            result = usb_hmsc_read_capacity(p_buff);
            /* The result of the user command should be reported before returning this function */
        break;
        case USB_ATAPI_MODE_SELECT10:                /* Mode select10 */
            result = usb_hmsc_mode_select10(p_buff);
            /* The result of the user command should be reported before returning this function */
        break;
        case USB_ATAPI_MODE_SENSE10:                /* Mode sense10 */
            result = usb_hmsc_mode_sense10(p_buff);
            /* The result of the user command should be reported before returning this function */
        break;

        /*
         USB_ATAPI_FORMAT_UNIT:
         USB_ATAPI_MODE_SELECT6:
         USB_ATAPI_MODE_SENSE6:
         USB_ATAPI_START_STOP_UNIT:
         USB_ATAPI_READ10:
         USB_ATAPI_WRITE10:
         USB_ATAPI_SEEK:
         USB_ATAPI_WRITE_AND_VERIFY:
         USB_ATAPI_VERIFY10:
        */
        default :
            return (USB_PAR);
        break;
    }

    if (USB_HMSC_OK == result)
    {
        result = USB_OK; /* USB_OK; */
    }
    else if (USB_PAR == result)
    {
        result = USB_PAR;
    }
    else
    {
        result = USB_ERROR;
    }

    return result;
}
/******************************************************************************
 End of function usb_hmsc_strg_user_command
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_storage_driver_start
 Description     : USB Host Initialize process
 Arguments       : none
 Return          : none
 ******************************************************************************/
void usb_hmsc_storage_driver_start (void)
{
    memset((void *)&g_usb_hmsc_data, 0, 512);
}
/******************************************************************************
 End of function usb_hmsc_storage_driver_start
 ******************************************************************************/
#endif /* defined(USB_CFG_HMSC_USE) */

#endif   /* BSP_CFG_RTOS_USED != 0 */
/******************************************************************************
 End  Of File
 ******************************************************************************/
