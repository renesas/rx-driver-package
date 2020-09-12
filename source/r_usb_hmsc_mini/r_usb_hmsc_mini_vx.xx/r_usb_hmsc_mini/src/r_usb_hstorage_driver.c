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
* File Name    : r_usb_hstorage_driver.c
* Description  : USB Host Storage Driver
*******************************************************************************/
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


/* Condition compilation by the difference of the File system */
#ifdef USB_TFAT_USE_PP
  #include "r_tfat_lib.h"               /* TFAT library */
#endif /* USB_TFAT_USE_PP */

/*******************************************************************************
 Macro definitions
 ******************************************************************************/

#define USB_STRG_SECTSIZE               (512u)  /* 512 bytes per sector */
#define USB_STRG_SECTTOP                (0)     /* Media Top sector */
#define USB_STRG_SECTCNT1               (1)     /* sector count = 1 */
#define USB_STRG_MBR_READ_RETRY_MAX     (10)    /* Max Retry count for Read of Master Boot Record */

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/*******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

uint16_t        g_usb_hmsc_strg_process                   = USB_NULL;
uint16_t        g_usb_hmsc_root_devaddr;

/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/

static void     usb_hmsc_strg_check_result(usb_hutr_t *p_mess);
static void     usb_hmsc_strg_specified_path(usb_tskinfo_t *p_mess);
static void     usb_hmsc_strg_drive_search_act(usb_tskinfo_t *p_mess);

static usb_hmsc_cb_t    gs_usb_hmsc_command_result_cb;
static uint16_t         gs_usb_hmsc_drive_search_seq            = USB_SEQ_0;
static uint16_t         gs_usb_hmsc_read_mbr_retry   = USB_SEQ_0;

/******************************************************************************
 * Function Name: usb_hmsc_strg_drive_task
 * Description  : Storage drive task
 * Arguments    : none
 * Return Value : none
******************************************************************************/
void usb_hmsc_strg_drive_task (void)
{
    usb_tskinfo_t   *p_mess;
    usb_er_t        err;                    /* Error code */

    /* receive message */
    err = R_USB_RCV_MSG (USB_HSTRG_MBX, (usb_msg_t**)&p_mess);
    if (USB_OK != err)
    {
        return;
    }

    switch (p_mess->msginfo)
    {
        case USB_MSG_HMSC_STRG_DRIVE_SEARCH:
            /* Drive search */
            usb_hmsc_strg_drive_search_act (p_mess);
        break;

        case USB_MSG_HMSC_STRG_DRIVE_SEARCH_END :
            g_usb_hmsc_strg_process = USB_NULL;
            /* Chack Callback function */
            if (USB_NULL != gs_usb_hmsc_command_result_cb)
            {
                (gs_usb_hmsc_command_result_cb)(p_mess);
            }
        break;

        case USB_MSG_HMSC_STRG_USER_COMMAND:
            /* Chack Callback function */
            if (USB_NULL != gs_usb_hmsc_command_result_cb)
            {
                (gs_usb_hmsc_command_result_cb)(p_mess);
            }
        break;
        default:
            /* Do Nothing */
        break;
    }

    /* Release Message block(Message box ID) */
    R_USB_REL_BLK(p_mess->flag);
}   /* eof usb_hmsc_strg_drive_task() */

/******************************************************************************
 * Function Name: usb_hmsc_strg_drive_search_act
 * Description  : Storage drive search
 * Arguments    : usb_tskinfo_t *p_mess   : Message
 * Return Value : none
 ******************************************************************************/
static void usb_hmsc_strg_drive_search_act(usb_tskinfo_t *p_mess)
{
    switch (gs_usb_hmsc_drive_search_seq)
    {
        case USB_SEQ_0:
            /* Unit number set */
            g_usb_hmsc_root_devaddr = USB_DEVICEADDR;   /* Use TFAT RX Driver */
            gs_usb_hmsc_command_result_cb = (usb_hmsc_cb_t)p_mess->complete;

            g_usb_hmsc_strg_process = USB_MSG_HMSC_STRG_DRIVE_SEARCH;
            /* Get Max LUN */
            usb_hmsc_get_max_unit (usb_hmsc_strg_check_result);
            gs_usb_hmsc_drive_search_seq++;
        break;

        case USB_SEQ_1:
            /* Check connection */
            /* usb_cpu_delay_xms(100); */ /* 2018.05.09 Delete */

            /* Inquiry */
            usb_hmsc_inquiry ((uint8_t*)&g_usb_hmsc_data);
            gs_usb_hmsc_drive_search_seq++;
        break;

        case USB_SEQ_2:
            /* Read Format Capacity */
            usb_hmsc_read_format_capacity ((uint8_t*)&g_usb_hmsc_data);
            gs_usb_hmsc_drive_search_seq++;
        break;

        case USB_SEQ_3:
            /* Read Capacity */
            usb_hmsc_read_capacity ((uint8_t*)&g_usb_hmsc_data);
            gs_usb_hmsc_drive_search_seq++;
        break;

        case USB_SEQ_4:
            if (USB_HMSC_OK != p_mess->keyword)
            {
                /* TestUnitReady */
                usb_hmsc_test_unit();
                gs_usb_hmsc_drive_search_seq++;
            }
            else
            {
                /* Pass TestUnitReady  */
                gs_usb_hmsc_drive_search_seq = USB_SEQ_6;
                usb_hmsc_strg_specified_path (p_mess);
            };
        break;

        case USB_SEQ_5:
            if (USB_HMSC_OK != p_mess->keyword)
            {
                /* TestUnitReady */
                usb_hmsc_test_unit ();
            }
            else
            {
                /* Read Capacity */
                usb_hmsc_read_capacity ((uint8_t*)&g_usb_hmsc_data);
                gs_usb_hmsc_read_mbr_retry = 0;
                gs_usb_hmsc_drive_search_seq++;
            };
        break;

        case USB_SEQ_6:
            /* Read10 */
            usb_hmsc_read10 ((uint8_t*)&g_usb_hmsc_data, USB_STRG_SECTTOP, USB_STRG_SECTCNT1, USB_STRG_SECTSIZE);
            gs_usb_hmsc_drive_search_seq++;
        break;

        case USB_SEQ_7:
            if (USB_HMSC_OK == p_mess->keyword)
            {
                gs_usb_hmsc_drive_search_seq = USB_SEQ_0;
                g_usb_hmsc_strg_process = USB_MSG_HMSC_STRG_DRIVE_SEARCH_END;
            }
            else
            {
                gs_usb_hmsc_drive_search_seq = USB_SEQ_6;
                gs_usb_hmsc_read_mbr_retry++;   /* Update Retry count */
                if (USB_STRG_MBR_READ_RETRY_MAX == gs_usb_hmsc_read_mbr_retry)
                {
                    /* Retry end. */
                    gs_usb_hmsc_drive_search_seq = USB_SEQ_0;
                    g_usb_hmsc_strg_process = USB_MSG_HMSC_STRG_DRIVE_SEARCH_END;
                    usb_hstd_connect_err_event_set();           /* Set event USB_STS_NOT_SUPPORT */
                }
            }
            usb_hmsc_strg_specified_path (p_mess);
        break;

        default:
            gs_usb_hmsc_drive_search_seq = USB_SEQ_0;
            g_usb_hmsc_strg_process = USB_NULL;
            usb_hstd_connect_err_event_set();           /* Set event USB_STS_NOT_SUPPORT */
            usb_hmsc_strg_specified_path ((usb_tskinfo_t *)p_mess);
        break;
    }
}   /* eof usb_hmsc_strg_drive_search_act() */


/******************************************************************************
 * Function Name: usb_hmsc_strg_specified_path
 * Description  : Next Process Selector
 * Arguments    : usb_tskinfo_t *p_mess  : Message
 * Return Value : none
******************************************************************************/
static void usb_hmsc_strg_specified_path(usb_tskinfo_t *p_mess)
{
    /* Message send to StrageDriveTask. Next phase process */
    USB_GET_SND(USB_HSTRG_MBX, g_usb_hmsc_strg_process, &usb_hstd_dummy_function, (usb_strct_t)p_mess->keyword);
}   /* eof usb_hmsc_strg_specified_path() */

/******************************************************************************
 * Function Name: usb_hmsc_strg_check_result
 * Description  : Hub class check result
 * Arguments    : usb_tskinfo_t *p_mess    : Message
 * Return Value : none
******************************************************************************/
static void usb_hmsc_strg_check_result(usb_hutr_t *p_mess)
{
    /* Message send to StrageDriveTask. Complete Get MAX UNIT. */
    USB_GET_SND(USB_HSTRG_MBX, g_usb_hmsc_strg_process, &usb_hstd_dummy_function, (usb_strct_t)p_mess->status);
}   /* eof usb_hmsc_strg_check_result() */


/******************************************************************************
 * Function Name: usb_hmsc_strg_drive_search
 * Description  : Searches drive SndMsg
 * Arguments    : usb_hcb_t complete    : Callback function
 * Return Value : uint16_t             : Status
******************************************************************************/
uint16_t usb_hmsc_strg_drive_search (usb_hmsc_cb_t complete)
{
    gs_usb_hmsc_drive_search_seq = USB_SEQ_0;

    /* Message send to StrageDriveTask. Request Strage Drive Serch */
    return USB_GET_SND (USB_HSTRG_MBX, USB_MSG_HMSC_STRG_DRIVE_SEARCH, complete, USB_NULL);
}   /* eof usb_hmsc_strg_drive_search() */


/******************************************************************************
 * Function Name: usb_hmsc_strg_drive_close
 * Description  : Releases drive
 * Arguments    : none
 * Return Value : uint16_t         : [DONE/ERROR]
******************************************************************************/
uint16_t usb_hmsc_strg_drive_close (void)
{
    /* Device Status */
    if (USB_HMSC_DEV_ATT != usb_hmsc_get_device_status())
    {
        return (USB_ERROR);
    }

    return (USB_OK);
}   /* eof usb_hmsc_strg_drive_close() */


/******************************************************************************
 * Function Name: usb_hmsc_strg_user_command
 * Description  : USB Mass Storage Command
 * Arguments    : uint16_t command     : Command
                : uint8_t  *p_buff     : Buffer address
                : usb_hcb_t complete   : callback info
 * Return Value : uint16_t             : [DONE/ERROR]
******************************************************************************/
uint16_t usb_hmsc_strg_user_command(uint16_t command, uint8_t *p_buff, usb_hmsc_cb_t complete)
{
    uint16_t    result;

    /* Device Status */
    if (USB_HMSC_DEV_ATT != usb_hmsc_get_device_status())
    {
        return (USB_ERROR);
    }

    g_usb_hmsc_strg_process = USB_MSG_HMSC_STRG_USER_COMMAND;
    gs_usb_hmsc_command_result_cb = complete;

    switch (command)
    {
        case USB_ATAPI_TEST_UNIT_READY:             /* Test unit */
            result = usb_hmsc_test_unit();
        break;
        case USB_ATAPI_REQUEST_SENSE:               /*Request sense */
            result = usb_hmsc_request_sense(p_buff);
        break;
        case USB_ATAPI_INQUIRY:                     /* Inquiry */
            result = usb_hmsc_inquiry(p_buff);
        break;
        case USB_ATAPI_PREVENT_ALLOW:               /* Prevent allow */
            result = usb_hmsc_prevent_allow(p_buff);
        break;
        case USB_ATAPI_READ_FORMAT_CAPACITY:        /* Read format capacity */
            result = usb_hmsc_read_format_capacity(p_buff);
        break;
        case USB_ATAPI_READ_CAPACITY:               /* Read capacity */
            result = usb_hmsc_read_capacity(p_buff);
        break;
        case USB_ATAPI_MODE_SELECT10:                /* Mode select6 */
            result = usb_hmsc_mode_select10(p_buff);
        break;
        case USB_ATAPI_MODE_SENSE10:                /* Mode sense10 */
            result = usb_hmsc_mode_sense10(p_buff);
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
        default:
            return (USB_PAR);
        break;
    }
    if (USB_HMSC_OK == result)
    {
        result = USB_OK; /* USB_OK; */
    }
    else if (USB_PAR == result)
    {
        return (USB_PAR);
    }
    else
    {
        result = USB_ERROR;
    }

    return result;
}   /* eof usb_hmsc_strg_user_command() */


/******************************************************************************
End  Of File
******************************************************************************/
