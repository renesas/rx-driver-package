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
 * File Name    : r_usb_hmsc_driver_rtos.c
 * Description  : USB Host MSC BOT driver
 ***********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 30.06.2020 1.20    First Release
 ***********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_usb_hmsc_mini_if.h"

#if   (BSP_CFG_RTOS_USED != 0)      /* RTOS */

#include "r_usb_reg_access.h"
#include "r_usb_cstd_rtos.h"

#if defined(USB_CFG_HMSC_USE)
/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

/* functions */
static uint16_t usb_hmsc_data_act (void);
static uint16_t usb_hmsc_send_cbw (void);
static uint16_t usb_hmsc_send_data (uint8_t *buff, uint32_t size);
static uint16_t usb_hmsc_rcv_data (uint8_t *buff, uint32_t size);
static uint16_t usb_hmsc_rcv_csw (void);
static usb_er_t usb_hmsc_clear_stall (uint16_t Pipe, usb_cb_t complete);
static void     usb_hmsc_configured (usb_utr_t *ptr, uint16_t addr, uint16_t data2);
static void     usb_hmsc_detach (usb_utr_t *ptr, uint16_t addr, uint16_t data2);
static void     usb_hmsc_classinit (usb_utr_t *ptr, uint16_t data1, uint16_t data2);
static void     usb_hmsc_set_els_cbw (uint8_t *data, uint32_t trans_byte);
static void     usb_hmsc_clr_data (uint16_t len, uint8_t *buf);
static void     usb_hmsc_set_rw_cbw (uint16_t command, uint32_t secno, uint16_t seccnt, uint32_t trans_byte);
static uint16_t usb_hmsc_no_data (void);
static uint16_t usb_hmsc_data_in (uint8_t *buff, uint32_t size);
static uint16_t usb_hmsc_data_out (uint8_t *buff, uint32_t size);
static uint16_t usb_hmsc_get_string_info(uint16_t string);
static uint16_t usb_hmsc_trans_wait_tmo(uint16_t tmo);
static void     usb_hmsc_class_check (uint16_t **table);
static usb_er_t usb_hmsc_mass_storage_reset (void);
static void     usb_hmsc_trans_result(usb_utr_t *mess, uint16_t data1, uint16_t data2);

/* variables */
static uint32_t  gs_usb_hmsc_data_size;
static uint8_t  *gsp_usb_hmsc_buff;
static uint16_t  gs_usb_hmsc_process;

static uint16_t  usb_hmsc_csw_err_stall;
static usb_utr_t gs_usb_hmsc_tran_result_msg;      /* Send:usb_hmsc_req_trans_result->To:usb_hmsc_req_trans_wait_tmo */
static uint32_t  gs_usb_hmsc_tag_no;            /* CBW tag number */
static uint8_t   gs_usb_hmsc_connect_state;     /* Drive use flag */
static USB_MSC_CBW_t gs_usb_hmsc_cbw;           /* CBW headder */
static USB_MSC_CSW_t gs_usb_hmsc_csw;           /* CSW headder */


/******************************************************************************
 Exported global variables
 ******************************************************************************/

/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

static const uint16_t g_usb_hmsc_devicetpl[] =
{
    USB_CFG_TPLCNT,             /* Number of tpl table */
    0,                          /* Reserved */
    USB_CFG_TPL                 /* Vendor ID, Product ID */
};

uint8_t g_usb_hmsc_sub_class;
uint8_t g_usb_hmsc_sub_class;

/******************************************************************************
 Renesas Abstracted HMSC Driver functions
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hmsc_driver_start
 Description     : USB Host Initialize process
 Argument        : none
 Return          : none
 ******************************************************************************/
void usb_hmsc_driver_start (void)
{
    gs_usb_hmsc_connect_state = USB_HMSC_DEV_DET;

    gs_usb_hmsc_tag_no = 1;
    /* CBW Signature (Endian little:0x55534243/ big:0x43425355) */
    gs_usb_hmsc_cbw.dcbw_signature = USB_MSC_CBW_SIGNATURE;
    gs_usb_hmsc_cbw.dcbw_tag = gs_usb_hmsc_tag_no;
    gs_usb_hmsc_cbw.dcbw_dtl_lo = 0;
    gs_usb_hmsc_cbw.dcbw_dtl_ml = 0;
    gs_usb_hmsc_cbw.dcbw_dtl_mh = 0;
    gs_usb_hmsc_cbw.dcbw_dtl_hi = 0;
    gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 0;
    gs_usb_hmsc_cbw.bm_cbw_flags.BIT.reserved7 = 0;
    gs_usb_hmsc_cbw.bcbw_lun.BIT.bcbw_lun = 0;
    gs_usb_hmsc_cbw.bcbw_lun.BIT.reserved4 = 0;
    gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 0;
    gs_usb_hmsc_cbw.bcbwcb_length.BIT.reserved3 = 0;

    gs_usb_hmsc_cbw.cbwcb[0] = 0;
    gs_usb_hmsc_cbw.cbwcb[1] = 0;
    gs_usb_hmsc_cbw.cbwcb[2] = 0;
    gs_usb_hmsc_cbw.cbwcb[3] = 0;
    gs_usb_hmsc_cbw.cbwcb[4] = 0;
    gs_usb_hmsc_cbw.cbwcb[5] = 0;
    gs_usb_hmsc_cbw.cbwcb[6] = 0;
    gs_usb_hmsc_cbw.cbwcb[7] = 0;
    gs_usb_hmsc_cbw.cbwcb[8] = 0;
    gs_usb_hmsc_cbw.cbwcb[9] = 0;
    gs_usb_hmsc_cbw.cbwcb[10] = 0;
    gs_usb_hmsc_cbw.cbwcb[11] = 0;
    gs_usb_hmsc_cbw.cbwcb[12] = 0;
    gs_usb_hmsc_cbw.cbwcb[13] = 0;
    gs_usb_hmsc_cbw.cbwcb[14] = 0;
    gs_usb_hmsc_cbw.cbwcb[15] = 0;
}
/******************************************************************************
 End of function usb_hmsc_driver_start
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_set_rw_cbw
 Description     : CBW parameter initialization for the READ10/WRITE10 command
 Arguments       : uint16_t     command     : ATAPI command
                 : uint32_t     secno       : Sector number
                 : uint16_t     seccnt      : Sector count
                 : uint32_t     trans_byte  : Transfer size
 Return value    : none
 ******************************************************************************/
static void usb_hmsc_set_rw_cbw (uint16_t command, uint32_t secno, uint16_t seccnt, uint32_t trans_byte)
{
    /* CBW parameter set */
    gs_usb_hmsc_cbw.dcbw_tag = gs_usb_hmsc_tag_no;
    /* bit7-bit0 set */
    gs_usb_hmsc_cbw.dcbw_dtl_lo = (uint8_t) trans_byte;
    /* bit15-bit8 set */
    gs_usb_hmsc_cbw.dcbw_dtl_ml = (uint8_t) (trans_byte >> 8);
    /* bit23-bit16 set */
    gs_usb_hmsc_cbw.dcbw_dtl_mh = (uint8_t) (trans_byte >> 16);
    /* bit31-bit24 set */
    gs_usb_hmsc_cbw.dcbw_dtl_hi = (uint8_t) (trans_byte >> 24);
    gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 0;
    gs_usb_hmsc_cbw.bm_cbw_flags.BIT.reserved7 = 0;
    gs_usb_hmsc_cbw.bcbw_lun.BIT.bcbw_lun = 0; /*** Support LUN0 ONLY ***/
    gs_usb_hmsc_cbw.bcbw_lun.BIT.reserved4 = 0;
    gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 0;
    gs_usb_hmsc_cbw.bcbwcb_length.BIT.reserved3 = 0;

    /* ATAPI_COMMAND */
    gs_usb_hmsc_cbw.cbwcb[0] = (uint8_t) command;
    /* LUN */
    gs_usb_hmsc_cbw.cbwcb[1] = 0x00;
    /* sector address */
    /* bit31-bit24 set */
    gs_usb_hmsc_cbw.cbwcb[2] = (uint8_t) (secno >> 24);
    /* bit23-bit16 set */
    gs_usb_hmsc_cbw.cbwcb[3] = (uint8_t) (secno >> 16);
    /* bit15-bit8 set */
    gs_usb_hmsc_cbw.cbwcb[4] = (uint8_t) (secno >> 8);
    /* bit7-bit0 set */
    gs_usb_hmsc_cbw.cbwcb[5] = (uint8_t) secno;
    /* Reserved */
    gs_usb_hmsc_cbw.cbwcb[6] = 0x00;
    /* Sector length */
    gs_usb_hmsc_cbw.cbwcb[7] = (uint8_t) (seccnt >> 8);
    /* Block address */
    gs_usb_hmsc_cbw.cbwcb[8] = (uint8_t) seccnt;
    /* Control data */
    gs_usb_hmsc_cbw.cbwcb[9] = 0x00;

    /* ATAPI command check */
    switch (command)
    {
        case USB_ATAPI_TEST_UNIT_READY :
        case USB_ATAPI_REQUEST_SENSE :
        case USB_ATAPI_INQUIRY :
        case USB_ATAPI_MODE_SELECT6 :
        case USB_ATAPI_MODE_SENSE6 :
        case USB_ATAPI_START_STOP_UNIT :
        case USB_ATAPI_PREVENT_ALLOW :
        case USB_ATAPI_READ_FORMAT_CAPACITY :
        case USB_ATAPI_READ_CAPACITY :
            /* Do Nothing */
        break;
            /* Initialized READ CBW TAG */
        case USB_ATAPI_READ10 :
            gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 1;
            /* 10bytes */
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 10;
        break;
            /* Initialized WRITE CBW TAG */
        case USB_ATAPI_WRITE10 :
            gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 0;
            /* 10bytes */
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 10;
        break;
        case USB_ATAPI_SEEK :
        case USB_ATAPI_WRITE_AND_VERIFY :
        case USB_ATAPI_VERIFY10 :
        case USB_ATAPI_MODE_SELECT10 :
        case USB_ATAPI_MODE_SENSE10 :
        default :
            /* Do Nothing */
        break;
    }

    if (USB_ATAPI == g_usb_hmsc_sub_class)
    {
        /* 12bytes */
        gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = USB_MSC_CBWCB_LENGTH;
    }
}
/******************************************************************************
 End of function usb_hmsc_set_rw_cbw
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_set_els_cbw
 Description     : CBW parameter initialization for other commands
 Arguments       : uint8_t*p_cbwcb     : Pointer to the CBW area
                 : uint32_t trans_byte : ATAPI command data transfer size
 Return value    : none
 ******************************************************************************/
static void usb_hmsc_set_els_cbw (uint8_t *p_cbwcb, uint32_t trans_byte)
{
    uint8_t i;

    /* CBW parameter set */
    gs_usb_hmsc_cbw.dcbw_tag = gs_usb_hmsc_tag_no;
    /* bit7-bit0 set */
    gs_usb_hmsc_cbw.dcbw_dtl_lo = (uint8_t) trans_byte;
    /* bit15-bit8 set */
    gs_usb_hmsc_cbw.dcbw_dtl_ml = (uint8_t) (trans_byte >> 8);
    /* bit23-bit16 set */
    gs_usb_hmsc_cbw.dcbw_dtl_mh = (uint8_t) (trans_byte >> 16);
    /* bit31-bit24 set */
    gs_usb_hmsc_cbw.dcbw_dtl_hi = (uint8_t) (trans_byte >> 24);
    /* Receive */
    gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 0;
    gs_usb_hmsc_cbw.bm_cbw_flags.BIT.reserved7 = 0;
    gs_usb_hmsc_cbw.bcbw_lun.BIT.bcbw_lun = 0;
    gs_usb_hmsc_cbw.bcbw_lun.BIT.reserved4 = 0;
    gs_usb_hmsc_cbw.bcbwcb_length.BIT.reserved3 = 0;

    /* WAIT_LOOP */
    for (i = 0; i < 12; i++)
    {
        gs_usb_hmsc_cbw.cbwcb[i] = p_cbwcb[i];
    }

    /* ATAPI command check */
    switch (p_cbwcb[0])
    {
        /* No data */
        case USB_ATAPI_TEST_UNIT_READY :
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 6;
        break;
            /* Receive */
        case USB_ATAPI_REQUEST_SENSE :
            gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 1;
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 6;
        break;
            /* Send */
        case USB_ATAPI_FORMAT_UNIT :
        break;
            /* Receive */
        case USB_ATAPI_INQUIRY :
            gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 1;
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 6;
        break;
        case USB_ATAPI_MODE_SELECT6 :
        case USB_ATAPI_MODE_SENSE6 :
        break;
            /* No data */
        case USB_ATAPI_START_STOP_UNIT :
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 6;
        break;
            /* No data */
        case USB_ATAPI_PREVENT_ALLOW :
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 6;
        break;
            /* Receive */
        case USB_ATAPI_READ_FORMAT_CAPACITY :
            gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 1;
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 10;
        break;
            /* Receive */
        case USB_ATAPI_READ_CAPACITY :
            gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 1;
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 10;
        break;
        case USB_ATAPI_READ10 :
        case USB_ATAPI_WRITE10 :
            /* Do Nothing */
        break;
        case USB_ATAPI_SEEK :
        case USB_ATAPI_WRITE_AND_VERIFY :
        case USB_ATAPI_VERIFY10 :
            /* Do Nothing */
        break;
            /* Send */
        case USB_ATAPI_MODE_SELECT10 :
            /* Do Nothing */
        break;
            /* Receive */
        case USB_ATAPI_MODE_SENSE10 :
            gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 1;
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 10;
        break;
        default :
            /* Do Nothing */
        break;
    }

    if (USB_ATAPI == g_usb_hmsc_sub_class)
    {
        /* 12bytes */
        gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = USB_MSC_CBWCB_LENGTH;
    }
}
/******************************************************************************
 End of function usb_hmsc_set_els_cbw
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_clr_data
 Description     : data clear
 Arguments       : uint16_t len : Clear size
                 : uint8_t *p_buf : Pointer to the area to clear
 Return value    : none
 ******************************************************************************/
static void usb_hmsc_clr_data (uint16_t len, uint8_t *p_buf)
{
    uint16_t i;

    /* WAIT_LOOP */
    for (i = 0; i < len; ++i)
    {
        *p_buf++ = 0x00;
    }
}
/******************************************************************************
 End of function usb_hmsc_clr_data
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_no_data
 Description     : HMSC No data
 Arguments       : none
 Return value    : uint16_t : 
 ******************************************************************************/
static uint16_t usb_hmsc_no_data (void)
{
    uint16_t hmsc_retval;

    gs_usb_hmsc_process = USB_MSG_HMSC_NO_DATA;
    hmsc_retval = usb_hmsc_data_act();

    return (hmsc_retval);
}
/******************************************************************************
 End of function usb_hmsc_no_data
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_data_in
 Description     : HMSC Data In
 Arguments       : uint8_t      *p_buff   : Pointer to the buffer area
                 : uint32_t     size    : Data size
 Return value    : uint16_t : 
 ******************************************************************************/
static uint16_t usb_hmsc_data_in (uint8_t *p_buff, uint32_t size)
{
    uint16_t hmsc_retval;

    gsp_usb_hmsc_buff = p_buff;
    gs_usb_hmsc_data_size = size;
    gs_usb_hmsc_process = USB_MSG_HMSC_DATA_IN;
    hmsc_retval = usb_hmsc_data_act();

    return (hmsc_retval);
}
/******************************************************************************
 End of function usb_hmsc_data_in
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_data_out
 Description     : HMSC Data Out
 Arguments       : uint8_t      *p_buff   : Pointer to the buffer area
                 : uint32_t     size    : Data size
 Return value    : uint16_t : 
 ******************************************************************************/
static uint16_t usb_hmsc_data_out (uint8_t *p_buff, uint32_t size)
{
    uint16_t hmsc_retval;

    gsp_usb_hmsc_buff = p_buff;
    gs_usb_hmsc_data_size = size;
    gs_usb_hmsc_process = USB_MSG_HMSC_DATA_OUT;
    hmsc_retval = usb_hmsc_data_act();

    return (hmsc_retval);
}
/******************************************************************************
 End of function usb_hmsc_data_out
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_data_act
 Description     : Send Data request
 Arguments       : none
 Return value    : uint16_t : 
 ******************************************************************************/
static uint16_t usb_hmsc_data_act (void)
{
    uint16_t hmsc_retval;
    uint16_t err = USB_OFF;

    /* CBW */
    hmsc_retval = usb_hmsc_send_cbw();
    if (USB_HMSC_OK == hmsc_retval)
    {
        if (USB_MSG_HMSC_DATA_IN == gs_usb_hmsc_process) /* DataIn */
        {
            hmsc_retval = usb_hmsc_rcv_data(gsp_usb_hmsc_buff, gs_usb_hmsc_data_size);
            if (USB_HMSC_OK != hmsc_retval)
            {
                err = USB_ON;
            }
        }
        else if (USB_MSG_HMSC_DATA_OUT == gs_usb_hmsc_process) /* DataOut */
        {
            hmsc_retval = usb_hmsc_send_data(gsp_usb_hmsc_buff, gs_usb_hmsc_data_size);
            if (USB_HMSC_OK != hmsc_retval)
            {
                err = USB_ON;
            }
        }
        else { /* None */ }

        /* CSW */
        hmsc_retval = usb_hmsc_rcv_csw();
        if (USB_ON == err)
        {
            hmsc_retval = USB_HMSC_CSW_ERR;
        }
        else
        {
            if (USB_HMSC_CSW_ERR == hmsc_retval)
            {
                usb_cpu_delay_xms((uint16_t)200);
            }
            else if (USB_HMSC_STALL == hmsc_retval)
            {
                usb_hmsc_csw_err_stall = USB_ON;
                usb_hmsc_clear_stall(USB_CFG_HMSC_BULK_IN, class_trans_result);
                hmsc_retval = usb_hmsc_rcv_csw();
            }
            else if (USB_MSC_CSW_PHASE_ERR == hmsc_retval)
            {
                /* Reset */
                usb_hmsc_mass_storage_reset();
                /* Clear STALL for OUT pipe */
                usb_hmsc_clear_stall(USB_CFG_HMSC_BULK_OUT, class_trans_result);
                /* Clear STALL for IN pipe */
                usb_hmsc_clear_stall(USB_CFG_HMSC_BULK_IN, class_trans_result);
            }
            else
            {
                /* USB_HMSC_SUBMIT_ERR/USB_HMSC_OK */
                /* Non process */
            }
        }
    }

    return (hmsc_retval);
}
/******************************************************************************
 End of function usb_hmsc_data_act
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hmsc_send_cbw
 Description     : Send CBW
 Arguments       : none
 Return value    : uint16_t                 : Error Code
 ******************************************************************************/
static uint16_t usb_hmsc_send_cbw (void)
{
    usb_er_t err;
    uint16_t     ret = USB_HMSC_CBW_ERR;
    uint16_t    hmsc_retval;
    usb_utr_t   tran_data;

    /* Set CBW TAG usb_hmsc_CbwTagCount()*/
    gs_usb_hmsc_tag_no++;
    if (0UL == gs_usb_hmsc_tag_no)
    {
        gs_usb_hmsc_tag_no = 1UL;
    }

    /* pipe number */
    tran_data.keyword = USB_CFG_HMSC_BULK_OUT;

    /* Transfer data address */
    tran_data.p_tranadr = (void*) &gs_usb_hmsc_cbw;

    /* Transfer data length */
    tran_data.tranlen = (uint32_t) USB_MSC_CBWLENGTH;

    /* Call Back Function Info */
    tran_data.complete = &usb_hmsc_trans_result;

    /* Not control transfer */
    tran_data.p_setup = 0;

    err = usb_hstd_transfer_start(&tran_data);
    if (USB_OK == err)
    {
        hmsc_retval = usb_hmsc_trans_wait_tmo((uint16_t)3000);
        if (USB_DATA_NONE == hmsc_retval)
        {
            /* Send CBW */
            ret = USB_HMSC_OK;
        }
        else if (USB_DATA_STALL == hmsc_retval)
        {
            /* Stall */
            /* Reset */
            usb_hmsc_mass_storage_reset();
            /* Clear Stall for OUT pipe */
            usb_hmsc_clear_stall(USB_CFG_HMSC_BULK_OUT, class_trans_result);
            /* Clear Stall for IN pipe */
            usb_hmsc_clear_stall(USB_CFG_HMSC_BULK_IN, class_trans_result);
        }
        else if (USB_DATA_TMO == hmsc_retval)
        {
            /* Timeout */
            usb_hstd_transfer_end(USB_CFG_HMSC_BULK_OUT, hmsc_retval);
        }
        else
        {
            /* Non process */
        }
    }
    else
    {
        ret = USB_HMSC_SUBMIT_ERR;
    }

    return ret;
}
/******************************************************************************
 End of function usb_hmsc_send_cbw
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_rcv_data
 Description     : Receive Data request
 Arguments       : uint8_t      *p_buff     : Pointer to the area to store the data
                 : uint32_t     size        : Receive Data Size
 Return value    : uint16_t                 : Error Code
 ******************************************************************************/
static uint16_t usb_hmsc_rcv_data (uint8_t *p_buff, uint32_t size)
{
    usb_er_t err;
    uint16_t hmsc_retval;
    uint16_t ret = USB_HMSC_DAT_RD_ERR;
    usb_utr_t   tran_data;

    /* pipe number */
    tran_data.keyword = USB_CFG_HMSC_BULK_IN;

    /* Transfer data address */
    tran_data.p_tranadr = (void*) p_buff;
    /* Transfer data length */
    tran_data.tranlen = size;

    /* Call Back Function Info */
    tran_data.complete = &usb_hmsc_trans_result;

    /* Not control transfer */
    tran_data.p_setup = USB_NULL;

    err = usb_hstd_transfer_start(&tran_data);
    if (USB_OK == err)
    {
        hmsc_retval = usb_hmsc_trans_wait_tmo((uint16_t)3000);
        if ((USB_DATA_SHT == hmsc_retval) || (USB_DATA_OK == hmsc_retval))
        {
            ret = USB_HMSC_OK;
        }
        else if (USB_DATA_STALL == hmsc_retval)
        {
            usb_hmsc_clear_stall(USB_CFG_HMSC_BULK_IN, class_trans_result);
            ret = USB_HMSC_OK;
        }
        else if (USB_DATA_TMO == hmsc_retval)
        {
            /* Timeout */
            usb_hstd_transfer_end(USB_CFG_HMSC_BULK_IN, (uint16_t)USB_DATA_TMO);
        }
        else
        {
            /* Non process */
        }
    }
    else
    {
        ret = USB_HMSC_SUBMIT_ERR;
    }

    return ret;
}
/******************************************************************************
 End of function usb_hmsc_rcv_data
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_send_data
 Description     : Send Pipe Data
 Arguments       : uint8_t      *p_buff     : Pointer to the area to store the data
                 : uint32_t     size        : Receive Data Size
 Return value    : uint16_t                : Error Code
 ******************************************************************************/
static uint16_t usb_hmsc_send_data (uint8_t *p_buff, uint32_t size)
{
    usb_er_t    err;
    uint16_t    hmsc_retval;
    uint16_t    ret = USB_HMSC_DAT_WR_ERR;
    usb_utr_t   tran_data;

    /* pipe number */
    tran_data.keyword = USB_CFG_HMSC_BULK_OUT;

    /* Transfer data address */
    tran_data.p_tranadr = (void*) p_buff;

    /* Transfer data length */
    tran_data.tranlen = size;

    /* Call Back Function Info */
    tran_data.complete = &usb_hmsc_trans_result;

    /* Not control transfer */
    tran_data.p_setup = 0;

    err = usb_hstd_transfer_start(&tran_data);
    if (USB_OK == err)
    {
        hmsc_retval = usb_hmsc_trans_wait_tmo((uint16_t)3000);
        if (USB_DATA_NONE == hmsc_retval)
        {
            ret = USB_HMSC_OK;
        }
        else if (USB_DATA_STALL == hmsc_retval)
        {
            usb_hmsc_clear_stall(USB_CFG_HMSC_BULK_OUT, class_trans_result);
            ret = USB_HMSC_STALL;
        }
        else if (USB_DATA_TMO == hmsc_retval)
        {
            usb_hstd_transfer_end(USB_CFG_HMSC_BULK_OUT, (uint16_t)USB_DATA_TMO);
        }
        else
        {
            /* Non process */
        }
    }
    else
    {
        ret = USB_HMSC_SUBMIT_ERR;
    }

    return ret;
}
/******************************************************************************
 End of function usb_hmsc_send_data
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_rcv_csw
 Description     : Receive CSW
 Arguments       : none
 Return value    : uint16_t                  : Error Code
 ******************************************************************************/
static uint16_t usb_hmsc_rcv_csw (void)
{   
    usb_er_t    err;
    uint16_t    hmsc_retval;
    uint32_t    request_size;
    usb_utr_t   tran_data;
    uint16_t    ret = USB_HMSC_CSW_ERR;

    /* pipe number */
    tran_data.keyword = USB_CFG_HMSC_BULK_IN;

    /* Transfer data address */
    tran_data.p_tranadr = (void*) &gs_usb_hmsc_csw;

    /* Transfer data length */
    tran_data.tranlen = (uint32_t) USB_MSC_CSW_LENGTH;

    /* Call Back Function Info */
    tran_data.complete = &usb_hmsc_trans_result;
    tran_data.p_setup = 0;

    /* Not control transfer */

    err = usb_hstd_transfer_start(&tran_data);
    if (USB_OK == err)
    {
        hmsc_retval = usb_hmsc_trans_wait_tmo((uint16_t)3000);
        if ((USB_DATA_SHT == hmsc_retval) || (USB_DATA_OK == hmsc_retval))
        {
            /* bit7-bit0 set */
            request_size = gs_usb_hmsc_cbw.dcbw_dtl_lo;
            /* bit15-bit8 set */
            request_size |= (gs_usb_hmsc_cbw.dcbw_dtl_ml << 8);
            /* bit23-bit16 set */
            request_size |= (gs_usb_hmsc_cbw.dcbw_dtl_mh << 16);
            /* bit31-bit24 set */
            request_size |= (gs_usb_hmsc_cbw.dcbw_dtl_hi << 24);

            /* CSW Check */
            if (USB_MSC_CSW_SIGNATURE == gs_usb_hmsc_csw.dcsw_signature)
            {
                if (gs_usb_hmsc_csw.dcsw_tag == gs_usb_hmsc_cbw.dcbw_tag)
                {
                    if (USB_MSC_CSW_OK == gs_usb_hmsc_csw.dcsw_status)
                    {
                        ret = USB_HMSC_OK;
                    }
                    else if (USB_MSC_CSW_PHASE_ERR == gs_usb_hmsc_csw.dcsw_status)
                    {
                        if (USB_ON == usb_hmsc_csw_err_stall)
                        {
                            usb_hmsc_csw_err_stall = USB_OFF;

                            /* Reset */
                            usb_hmsc_mass_storage_reset();

                            /* Clear STALL for OUT pipe */
                            usb_hmsc_clear_stall(USB_CFG_HMSC_BULK_OUT, class_trans_result);

                            /* Clear STALL for IN pipe */
                            usb_hmsc_clear_stall(USB_CFG_HMSC_BULK_IN, class_trans_result);
                        }
                        ret = USB_HMSC_CSW_PHASE_ERR;
                    }
                    else
                    {
                        /* Non process */
                    }
                }
            }
        }
        else if (USB_DATA_STALL == hmsc_retval)
        {
            if (USB_ON == usb_hmsc_csw_err_stall)
            {
                usb_hmsc_csw_err_stall = USB_OFF;

                /* Reset */
                usb_hmsc_mass_storage_reset();

                /* Clear STALL for OUT pipe */
                usb_hmsc_clear_stall(USB_CFG_HMSC_BULK_OUT, class_trans_result);

                /* Clear STALL for IN pipe */
                usb_hmsc_clear_stall(USB_CFG_HMSC_BULK_IN, class_trans_result);
                ret = USB_HMSC_CSW_PHASE_ERR;
            }
            else
            {
                ret = USB_HMSC_STALL;
            }
        }
        else if (USB_DATA_TMO == hmsc_retval)
        {
            usb_hstd_transfer_end(USB_CFG_HMSC_BULK_IN, (uint16_t)USB_DATA_TMO);
        }
        else
        {
            /* Do Nothing */
        }
    }
    else
    {
        ret = USB_HMSC_SUBMIT_ERR;
    }

    return ret;
}
/******************************************************************************
 End of function usb_hmsc_rcv_csw
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_clear_stall
 Description     : Clear Stall
 Arguments       : uint16_t     pipe        : Pipe number
                 : usb_cb_t     complete    : Callback function
 Return value    : uint16_t
 ******************************************************************************/
static usb_er_t usb_hmsc_clear_stall (uint16_t pipe, usb_cb_t complete)
{
    usb_er_t err;

    err = usb_hstd_clr_stall(pipe, complete);
    if (USB_OK == err)
    {

        /* Clear STALL pipe 0 */
        usb_cstd_clr_stall((uint16_t)pipe);

        /* SQCLR */
        usb_hstd_change_device_state(usb_hstd_dummy_function, USB_DO_CLR_SQTGL, (uint16_t)pipe);
    }

    return err;
}
/******************************************************************************
 End of function usb_hmsc_clear_stall
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hmsc_read10
 Description     : Read10
 Arguments       : uint8_t  *p_buff       : Buffer address
                 : uint32_t secno       : Sector number
                 : uint16_t seccnt      : Sector count
                 : uint32_t trans_byte  : Trans byte
 Return value    : uint16_t : 
 ******************************************************************************/
uint16_t usb_hmsc_read10 (uint8_t *p_buff, uint32_t secno, uint16_t seccnt,
        uint32_t trans_byte)
{
    uint16_t hmsc_retval;

    /* set CBW parameter */
    usb_hmsc_set_rw_cbw((uint16_t) USB_ATAPI_READ10, secno, seccnt, trans_byte);

    /* Data IN */
    hmsc_retval = usb_hmsc_data_in(p_buff, trans_byte);
    return (hmsc_retval);
}
/******************************************************************************
 End of function usb_hmsc_read10
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_write10
 Description     : Write10
 Arguments       : uint8_t *p_buff       : Buffer address
                 : uint32_t secno      : Sector number
                 : uint16_t seccnt     : Sector count
                 : uint32_t trans_byte : Trans byte
 Return value    : uint16_t : 
 ******************************************************************************/
uint16_t usb_hmsc_write10 (uint8_t *p_buff, uint32_t secno, uint16_t seccnt,
        uint32_t trans_byte)
{
    uint16_t hmsc_retval;

    /* set CBW parameter */
    usb_hmsc_set_rw_cbw((uint16_t) USB_ATAPI_WRITE10, secno, seccnt, trans_byte);

    /* Data OUT */
    hmsc_retval = usb_hmsc_data_out(p_buff, trans_byte);
    return (hmsc_retval);
}
/******************************************************************************
 End of function usb_hmsc_write10
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hmsc_test_unit
 Description     : TestUnit
 Arguments       : none
 Return value    : uint16_t : 
 ******************************************************************************/
uint16_t usb_hmsc_test_unit (void)
{
    uint8_t cbwcb[USB_MSC_CBWCB_LENGTH];
    uint16_t hmsc_retval;

    /* Data clear */
    usb_hmsc_clr_data((uint16_t)USB_MSC_CBWCB_LENGTH, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USB_ATAPI_TEST_UNIT_READY;

    /* Set CBW parameter */
    usb_hmsc_set_els_cbw((uint8_t*) &cbwcb, (uint32_t) 0);

    /* No Data */
    hmsc_retval = usb_hmsc_no_data();
    return (hmsc_retval);
}
/******************************************************************************
 End of function usb_hmsc_test_unit
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hmsc_request_sense
 Description     : RequestSense
 Arguments       : uint8_t      *p_buff   : Pointer to the buffer area
 Return value    : uint16_t : 
 ******************************************************************************/
uint16_t usb_hmsc_request_sense (uint8_t *p_buff)
{
    uint8_t cbwcb[USB_MSC_CBWCB_LENGTH];
    uint8_t length = 18;
    uint16_t hmsc_retval;

    /* Data clear */
    usb_hmsc_clr_data((uint16_t)USB_MSC_CBWCB_LENGTH, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USB_ATAPI_REQUEST_SENSE;

    /* Allocation length */
    cbwcb[4] = length;

    /* Set CBW parameter */
    usb_hmsc_set_els_cbw((uint8_t*) &cbwcb, (uint32_t) length);

    /* Data IN */
    hmsc_retval = usb_hmsc_data_in(p_buff, (uint32_t) length);
    return (hmsc_retval);
}
/******************************************************************************
 End of function usb_hmsc_request_sense
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_inquiry
 Description     : Inquiry
 Arguments       : uint8_t      *p_buff   : Pointer to the buffer area
 Return value    : uint16_t : 
 ******************************************************************************/
uint16_t usb_hmsc_inquiry (uint8_t *p_buff)
{
    uint8_t cbwcb[USB_MSC_CBWCB_LENGTH];
    uint8_t length = 36;
    uint16_t hmsc_retval;

    /* Data clear */
    usb_hmsc_clr_data((uint16_t)USB_MSC_CBWCB_LENGTH, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USB_ATAPI_INQUIRY;

    /* Allocation length */
    cbwcb[4] = length;

    /* Set CBW parameter */
    usb_hmsc_set_els_cbw((uint8_t*) &cbwcb, (uint32_t) length);

    /* Data IN */
    hmsc_retval = usb_hmsc_data_in(p_buff, (uint32_t) length);
    return (hmsc_retval);
}
/******************************************************************************
 End of function usb_hmsc_inquiry
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_read_capacity
 Description     : ReadCapacity
 Arguments       : uint8_t      *p_buff   : Pointer to the buffer area
 Return value    : uint16_t : 
 ******************************************************************************/
uint16_t usb_hmsc_read_capacity (uint8_t *p_buff)
{
    uint8_t cbwcb[USB_MSC_CBWCB_LENGTH];
    uint8_t length = 8;
    uint16_t hmsc_retval;

    /* Data clear */
    usb_hmsc_clr_data((uint16_t)USB_MSC_CBWCB_LENGTH, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USB_ATAPI_READ_CAPACITY;

    /* Set CBW parameter */
    usb_hmsc_set_els_cbw((uint8_t*) &cbwcb, (uint32_t) length);

    /* Data IN */
    hmsc_retval = usb_hmsc_data_in(p_buff, (uint32_t) length);
    return (hmsc_retval);
}
/******************************************************************************
 End of function usb_hmsc_read_capacity
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_read_format_capacity
 Description     : ReadFormatCapacity
 Arguments       : uint8_t      *p_buff   : Pointer to the buffer area
 Return value    : uint16_t : 
 ******************************************************************************/
uint16_t usb_hmsc_read_format_capacity (uint8_t *p_buff)
{
    uint8_t cbwcb[USB_MSC_CBWCB_LENGTH];
    uint8_t length = 0x20;
    uint16_t hmsc_retval;

    /* Data clear */
    usb_hmsc_clr_data((uint16_t) USB_MSC_CBWCB_LENGTH, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USB_ATAPI_READ_FORMAT_CAPACITY;

    /* Allocation length */
    cbwcb[8] = length;

    /* Set CBW parameter */
    usb_hmsc_set_els_cbw((uint8_t*) &cbwcb, (uint32_t) length);

    /* Data IN */
    hmsc_retval = usb_hmsc_data_in(p_buff, (uint32_t) length);
    return (hmsc_retval);
}
/******************************************************************************
 End of function usb_hmsc_read_format_capacity
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_get_max_unit
 Description     : Get Max LUN request
 Arguments       : none
 Return value    : usb_er_t                  : Error Code
 ******************************************************************************/
usb_er_t usb_hmsc_get_max_unit (void)
{
    static usb_ctrl_trans_t get_max_lun_table;
    uint16_t    hmsc_retval;
    usb_utr_t   tran_data;
    usb_er_t    ret = USB_ERROR;

    get_max_lun_table.setup.type = 0xFEA1;
    get_max_lun_table.setup.value = 0x0000;
    get_max_lun_table.setup.index = 0x0000;
    get_max_lun_table.setup.length = 0x0001;
    get_max_lun_table.address = USB_DEVICEADDR;

    /* Recieve MaxLUN */
    tran_data.keyword = USB_PIPE0;
    tran_data.p_tranadr = g_usb_hmsc_data;
    tran_data.tranlen = get_max_lun_table.setup.length;
    tran_data.p_setup = (uint16_t *) &get_max_lun_table;
    tran_data.complete = usb_hmsc_trans_result;
    usb_hstd_transfer_start_req(&tran_data);
    hmsc_retval = usb_hmsc_trans_wait_tmo((uint16_t)3000);
    if (USB_DATA_TMO == hmsc_retval)
    {
        usb_hstd_transfer_end(USB_PIPE0, hmsc_retval);
    }
    else if (USB_CTRL_END == hmsc_retval)
    {
        ret = USB_OK;
    }
    else
    {
        /* Non process */
    }
    return ret;
}
/******************************************************************************
 End of function usb_hmsc_get_max_unit
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_mass_storage_reset
 Description     : Mass Strage Reset request
 Arguments       : none
 Return value    : usb_er_t                 : Error Code
 ******************************************************************************/
static usb_er_t usb_hmsc_mass_storage_reset (void)
{
    uint16_t    hmsc_retval;
    usb_utr_t   tran_data;
    usb_er_t    ret = USB_ERROR;

    static usb_ctrl_trans_t mass_storage_reset_table;

    mass_storage_reset_table.setup.type = 0xFF21;
    mass_storage_reset_table.setup.value = 0x0000;
    mass_storage_reset_table.setup.index = 0x0000;
    mass_storage_reset_table.setup.length = 0x0000;
    mass_storage_reset_table.address = USB_DEVICEADDR;

    /* Set MassStorageReset */
    tran_data.keyword = USB_PIPE0;
    tran_data.p_tranadr = USB_NULL;
    tran_data.tranlen = mass_storage_reset_table.setup.length;
    tran_data.p_setup = (uint16_t *) &mass_storage_reset_table;
    tran_data.complete = usb_hmsc_trans_result;
    usb_hstd_transfer_start(&tran_data);
    hmsc_retval = usb_hmsc_trans_wait_tmo((uint16_t)3000);
    if (USB_DATA_TMO == hmsc_retval)
    {
        usb_hstd_transfer_end(USB_PIPE0, hmsc_retval);
    }
    else if (USB_CTRL_END == hmsc_retval)
    {
        ret = USB_OK;
    }
    else
    {
        /* Non process */
    }
    return ret;
}
/******************************************************************************
 End of function usb_hmsc_mass_storage_reset
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_prevent_allow
 Description     : PreventAllow
 Arguments       : uint8_t      *p_buff   : Pointer to the buffer area
 Return value    : uint16_t : 
 ******************************************************************************/
uint16_t usb_hmsc_prevent_allow (uint8_t *p_buff)
{
    uint8_t cbwcb[USB_MSC_CBWCB_LENGTH];
    uint16_t hmsc_retval;

    /* Data clear */
    usb_hmsc_clr_data((uint16_t) USB_MSC_CBWCB_LENGTH, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USB_ATAPI_PREVENT_ALLOW;

    /* Reserved */
    cbwcb[4] = p_buff[0];

    /* Set CBW parameter */
    usb_hmsc_set_els_cbw((uint8_t*) &cbwcb, (uint32_t) 0);

    /* No Data */
    hmsc_retval = usb_hmsc_no_data();
    return (hmsc_retval);
}
/******************************************************************************
 End of function usb_hmsc_prevent_allow
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_mode_sense10
 Description     : ModeSense10
 Arguments       : uint8_t      *p_buff   : [OUT]Pointer to the buffer area/[IN]p_buff[0] Page Code
 Return value    : uint16_t : 
 ******************************************************************************/
uint16_t usb_hmsc_mode_sense10 (uint8_t *p_buff)
{
    uint8_t cbwcb[USB_MSC_CBWCB_LENGTH];
    uint8_t length;
    uint16_t hmsc_retval;

    /* Data clear */
    usb_hmsc_clr_data((uint16_t) USB_MSC_CBWCB_LENGTH, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USB_ATAPI_MODE_SENSE10;

    /* Set LUN / DBD=1 */
    /* b7-b5:LUN (=0),b4-b0:Reserved */
    cbwcb[1] = 0x00;

    /* b7-b6:PageControl = Current values(=0),b5-b0:PageCode */
    cbwcb[2] = p_buff[0];

    /* [IN]p_buff[0] Page Code */
    if ((0x01 == p_buff[0]) || (0x08 == p_buff[0]) || (0x1b == p_buff[0]))
    {
        /* 0x01:Read-Write Error Recovery Page */
        /* 0x08:Caching Page */
        /* 0x1b:Removable Block Access Capacities Page */
        length = 20;    /* 8 + 12 */
    }
    else if (0x1c == p_buff[0]) 
    {
        /* Timer & Protect Page */
        length = 16;    /* 8 + 8 */
    }
    else if (0x3f == p_buff[0]) 
    {
        /* Return all pages(valid only for MODE SENSE command) */
        length = 44;    /* 8 + 12 + 12 +12 */
    }
    else
    {
        return USB_PAR;
    }

    /* Allocation length */
    cbwcb[7] = 0x00;    /* MSB */
    cbwcb[8] = length;  /* LSB */

    /* Set CBW parameter */
    usb_hmsc_set_els_cbw((uint8_t*) &cbwcb, (uint32_t) length);

    /* Data IN */
    hmsc_retval = usb_hmsc_data_in(p_buff, (uint32_t) length);
    return (hmsc_retval);
}
/******************************************************************************
 End of function usb_hmsc_mode_sense10
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hmsc_mode_select10
 Description     : ModeSelect6
 Arguments       : uint8_t      *p_buff   : Pointer to the buffer area
                 :              buff[0]-[7]:Mode Parameter Header
                 :              buff[8]-[n]:Page(s)
                 :               buff[8] b7:Parameter Savable(PS)
                 :               buff[8] b6:Reserved
                 :               buff[8] b5-b0:Page Code
 Return value    : uint16_t : 
 ******************************************************************************/
uint16_t usb_hmsc_mode_select10 (uint8_t *p_buff)
{
    uint8_t cbwcb[USB_MSC_CBWCB_LENGTH];
    uint8_t length = 18;
    uint16_t hmsc_retval;

    /* Data clear */
    usb_hmsc_clr_data((uint16_t) USB_MSC_CBWCB_LENGTH, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USB_ATAPI_MODE_SELECT10;

    /* Logical Unit Number = 0/ Save Pages(SP) = 0 */
    cbwcb[1] = 0x10;

    if ((0x01 == (p_buff[8] & 0x3f)) || (0x08 == (p_buff[8] & 0x3f)) || (0x1b == (p_buff[8] & 0x3f)))
    {
        /* 0x01:Read-Write Error Recovery Page */
        /* 0x08:Caching Page */
        /* 0x1b:Removable Block Access Capacities Page */
        length = 20;    /* 8 + 12 */
    }
    else if (0x1c == (p_buff[8] & 0x3f)) 
    {
        length = 16;    /* 8 + 8 */
    }
    else
    {
        return USB_PAR;
    }

    /* Parameter list length */
    cbwcb[7] = 0x00;    /* MSB */
    cbwcb[8] = length;  /* LSB */

    /* Set CBW parameter */
    usb_hmsc_set_els_cbw((uint8_t*) &cbwcb, (uint32_t) length);

    /* Data OUT */
    hmsc_retval = usb_hmsc_data_out(p_buff, (uint32_t) length);
    return (hmsc_retval);
}
/******************************************************************************
 End of function usb_hmsc_mode_select10
 ******************************************************************************/

/******************************************************************************
Function Name   : usb_hmsc_set_device_status
Description     : Sets HMSCD operation state
Arguments       : uint16_t data : 
Return value    : uint16_t : USB_OK
******************************************************************************/
uint16_t usb_hmsc_set_device_status (uint16_t data)
{
    gs_usb_hmsc_connect_state = data;
    return USB_OK; /* USB_OK; */
}   /* eof usb_hmsc_set_device_status() */

/******************************************************************************
 Function Name   : usb_hmsc_get_device_status
 Description     : Responds to HMSCD operation state
 Arugments       : none
 Return value    : uint16_t :
 ******************************************************************************/
uint16_t usb_hmsc_get_device_status (void)
{
    return (gs_usb_hmsc_connect_state);
}
/******************************************************************************
 End of function usb_hmsc_get_device_status
 ******************************************************************************/

/******************************************************************************
Function Name   : usb_hmsc_pipe_info
Description     : Pipe Information
Arguments       : uint8_t *p_table : 
                : uint16_t msgnum : 
                : uint16_t length : 
Return value    : uint16_t : USB_OK / USB_ERROR
******************************************************************************/
uint16_t usb_hmsc_pipe_info(uint8_t *p_table, uint16_t length)
{
    uint16_t    ofdsc;
    uint16_t    retval;
    int16_t     in_pipe;
    int16_t     out_pipe;
    uint8_t     pipe_no;

    /* Check Endpoint Descriptor */
    ofdsc = p_table[0];
    /* Pipe initial */
    in_pipe     = USB_NOPORT;
    out_pipe    = USB_NOPORT;

    /* WAIT_LOOP */
    while (ofdsc < length)
    {
        if (USB_DT_ENDPOINT == p_table[ofdsc + 1])
        {
            /* Endpoint Descriptor */
            pipe_no = usb_cstd_pipe_table_set (USB_HMSC, &p_table[ofdsc]);
            if (USB_CFG_HMSC_BULK_OUT == pipe_no)
            {
                out_pipe = USB_YES;
            }
            else if (USB_CFG_HMSC_BULK_IN == pipe_no)
            {
                in_pipe = USB_YES;
            }
            else
            {
                /* Do Nothing */
            }
        }
        ofdsc +=  p_table[ofdsc];
    }

    if ((in_pipe != USB_NOPORT) && (out_pipe != USB_NOPORT))
    {
        retval = USB_OK; /* USB_OK; */
    }
    else
    {
        retval = USB_ERROR;
    }

    return retval;
}   /* eof usb_hmsc_pipe_info() */

/******************************************************************************
 Function Name   : usb_hmsc_registration
 Description     : registration of Host msc driver.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_hmsc_registration (void)
{
    usb_hcdreg_t driver;

    /* Driver registration */
    driver.ifclass    = (uint16_t) USB_IFCLS_MAS;           /* Use Interface class for MSC. */
    driver.p_tpl      = (uint16_t*) &g_usb_hmsc_devicetpl;  /* Target peripheral list. */
    driver.classinit  = &usb_hmsc_classinit;                /* Driver init. */
    driver.classcheck = &usb_hmsc_class_check;              /* Driver check. */
    driver.devconfig  = &usb_hmsc_configured;               /* Callback when device is configured. */
    driver.devdetach  = &usb_hmsc_detach;                   /* Callback when device is detached. */
    driver.devsuspend = &usb_hstd_dummy_function;           /* Callback when device is suspended. */
    driver.devresume  = &usb_hstd_dummy_function;           /* Callback when device is resumed. */

    usb_hstd_driver_registration(&driver); /* Host MSC class driver registration. */
}
/******************************************************************************
 End of function usb_hmsc_registration
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hmsc_configured
 Description     : Callback function for MSC device configured
 Arguments       : usb_utr_t    *ptr    : Pointer to usb_utr_t structure
                 : uint16_t     devadr  : Device Address
                 : uint16_t     data2   : Not used
 Return value    : none
 ******************************************************************************/
static void usb_hmsc_configured (usb_utr_t *ptr, uint16_t devadr, uint16_t data2)
{
    uint16_t ret;
    usb_ctrl_t  ctrl;

    ret = usb_hmsc_strg_drive_open();
    if (USB_ERROR == ret)
    {
        usb_hstd_connect_err_event_set();
    }

    rtos_get_semaphore (&g_rtos_usb_hmsc_sem_id, RTOS_FOREVER);

    ret = usb_hmsc_strg_drive_search();
    if (USB_OK == ret)
    {
        ctrl.p_data = (void *)g_usb_default_apl_task_id;
        usb_cstd_set_event(USB_STS_CONFIGURED, &ctrl); /* Set Event()  */
    }

    rtos_release_semaphore (&g_rtos_usb_hmsc_sem_id);
}
/******************************************************************************
 End of function usb_hmsc_configured
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_detach
 Description     : Callback function for MSC device detach
 Arguments       : usb_utr_t    *ptr    : Pointer to usb_utr_t structure
                 : uint16_t     addr    : Device Address
                 : uint16_t     data2   : Not used
 Return value    : none
 ******************************************************************************/
static void usb_hmsc_detach (usb_utr_t *ptr, uint16_t addr, uint16_t data2)
{
    usb_ctrl_t  ctrl;

    usb_cstd_pipe_reg_clear();
    usb_cstd_pipe_table_clear();
    ctrl.p_data = (void *)g_usb_default_apl_task_id;
    usb_cstd_set_event(USB_STS_DETACH, &ctrl); /* Set Event()  */
}
/******************************************************************************
 End of function usb_hmsc_detach
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hmsc_classinit
 Description     : Host msc driver init
 Arguments       : usb_utr_t *ptr    : Pointer to usb_utr_t structure.
                 : uint16_t data1    : Not used.
                 : uint16_t data2    : Not used.
 Return value    : none
 ******************************************************************************/
static void usb_hmsc_classinit (usb_utr_t *ptr, uint16_t data1, uint16_t data2)
{
    usb_hmsc_csw_err_stall = USB_OFF;

    usb_hmsc_strg_drive_close ();
}
/******************************************************************************
 End of function usb_hmsc_classinit
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hmsc_class_check
 Description     : check connected device
 Arguments       : uint16_t **table     : Pointer to the class information table
 Return value    : none
 ******************************************************************************/
static void usb_hmsc_class_check (uint16_t **table)
{
    uint16_t retval;
    uint8_t *p_device_table;
    uint8_t *interface_table;

    interface_table = (uint8_t*) (table[2]);
    *table[3] = USB_ERROR;

    /* Check Interface Descriptor (device-class) */
    if (USB_IFCLS_MAS != interface_table[USB_IF_B_INTERFACECLASS])
    {
        return;
    }

    /* Check Interface Descriptor (subclass) */
    g_usb_hmsc_sub_class = interface_table[USB_IF_B_INTERFACESUBCLASS];
    if ((USB_ATAPI != g_usb_hmsc_sub_class) && (USB_SCSI != g_usb_hmsc_sub_class)
         && (USB_ATAPI_MMC5 != g_usb_hmsc_sub_class))
    {
        g_usb_hmsc_sub_class = USB_NULL;    /* sub class error */
        return;
    }

    /* Check Interface Descriptor (protocol) */
    if (USB_BOTP != interface_table[USB_IF_B_INTERFACEPROTOCOL])
    {
        return;
    }

    /* Check Endpoint number */
    if (interface_table[USB_IF_B_NUMENDPOINTS] < 2)
    {
        return;
    }

    /* Get String Descriptors */
    p_device_table = (uint8_t*) (table[0]);
    retval = usb_hmsc_get_string_info (p_device_table[USB_DEV_I_PRODUCT]);
    if (USB_OK != retval)
    {
        return;
    }
    *table[3] = USB_OK;
}
/******************************************************************************
 End of function usb_hmsc_class_check
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hmsc_get_string_info
 Description     : Get string descriptors.
 Argument        : uint16_t string
 Return value    : none
 ******************************************************************************/
static uint16_t usb_hmsc_get_string_info(uint16_t string)
{
    uint16_t retval;

    /* Send GetDescriptor(String) */
    retval = usb_hstd_get_string_desc((uint16_t)0);
    if (USB_OK != retval)
    {
        return USB_ERROR;
    }

    /* Send GetDescriptor(String) */
    retval = usb_hstd_get_string_desc((uint16_t)string);
    if (USB_OK != retval)
    {
        return USB_ERROR;
    }

    return USB_OK;
} /* End of function usb_hmsc_get_string_info() */


/******************************************************************************
 Function Name   : usb_hmsc_trans_result
 Description     : Send a message to notify the result of the hmsc class request.
 Argument        : usb_utr_t *ptr   : USB system internal structure. Selects channel.
                   uint16_t data1
                   uint16_t data2
 Return value    : none
 ******************************************************************************/
void usb_hmsc_trans_result(usb_utr_t *mess, uint16_t data1, uint16_t data2)
{
    gs_usb_hmsc_tran_result_msg = *mess;

    /* Send a message to HMSC mailbox */
    rtos_send_mailbox (&g_rtos_usb_hmsc_mbx_id, (void *)&gs_usb_hmsc_tran_result_msg);
} /* End of function usb_hmsc_trans_result() */

/******************************************************************************
 Function Name   : usb_hmsc_trans_wait_tmo
 Description     : Receive the result of the hmsc class request with a timeout.
 Argument        : usb_utr_t *ptr   : USB system internal structure. Selects channel.
                   uint16_t tmo     : Time-out value.
 Return value    : USB_OK/USB_ERROR
 ******************************************************************************/
static uint16_t usb_hmsc_trans_wait_tmo(uint16_t tmo)
{
    usb_utr_t *mess;
    rtos_err_t err;

    /* Receive message from HMSC mailbox with time-out */
    err = rtos_receive_mailbox (&g_rtos_usb_hmsc_mbx_id, (void **)&mess, (rtos_time_t)tmo );
    if (RTOS_SUCCESS != err)
    {
        return USB_ERROR;
    }

    return (mess->status);
} /* End of function usb_hmsc_trans_wait_tmo() */


#endif /* defined(USB_CFG_HMSC_USE) */

#endif   /* BSP_CFG_RTOS_USED != 0 */
/******************************************************************************
 End  Of File
 ******************************************************************************/
