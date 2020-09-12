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
 * Copyright (C) 2014(2019) Renesas Electronics Corporation. All rights reserved.
 ******************************************************************************/
/*******************************************************************************
* File Name    : r_usb_hmsc_driver.c
* Description  : USB Host MSC BOT driver
*******************************************************************************/
/*******************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 01.09.2014 1.00    First Release
 *         : 01.06.2015 1.01    Added RX231.
 *         : 31.05.2019 1.11    Added support for GNUC and ICCRX.
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
uint8_t     g_usb_hmsc_sub_class;
uint8_t     g_usb_hmsc_data[512];

extern uint16_t     g_usb_hmsc_strg_process;

/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/

/* CBW headder */
static USB_MSC_CBW_t       gs_usb_hmsc_cbw;
/* CSW headder */
static USB_MSC_CSW_t       gs_usb_hmsc_csw;

static uint32_t   gs_usb_hmsc_data_residue;
/* CBW tag number */
static uint32_t    gs_usb_hmsc_tag_no;
static uint16_t    gs_usb_hmsc_connect_state;

static uint32_t    gs_usb_hmsc_data_size;
static uint8_t    *gsp_usb_hmsc_buff;
static uint16_t    gs_usb_hmsc_process;
static uint16_t    gs_usb_hmsc_data_seq      = USB_SEQ_0;
static uint16_t    gs_usb_hmsc_phase_err_seq = USB_SEQ_0;
static uint16_t    gs_usb_hmsc_csw_loop      = USB_NO;

/* Send data transfer message */
static usb_hutr_t  gs_usb_hmsc_send_data;
/* Receive data transfer message */
static usb_hutr_t  gs_usb_hmsc_receive_data;
/* Control data transfer message */
static usb_hutr_t  gs_usb_hmsc_class_request;

static uint32_t    gs_usb_hmsc_cmd_data_size;

static void        usb_hmsc_command_result (uint16_t result);
static void        usb_hmsc_device_state (uint16_t data, uint16_t state);
static void        usb_hmsc_drive_complete (usb_tskinfo_t   *p_mess);
static usb_er_t    usb_hmsc_clear_stall ( uint16_t Pipe, usb_cbinfo_t complete);
static void        usb_hmsc_check_result (usb_hutr_t *p_mess);
static void        usb_hmsc_specified_path (usb_tskinfo_t *p_mess);
static void        usb_hmsc_check_result_stall_pipe_cb (uint16_t pipe, uint16_t state);
static uint16_t    usb_hmsc_data_in (uint8_t *p_buff, uint32_t size);
static uint16_t    usb_hmsc_data_out (uint8_t *p_buff, uint32_t size);
static uint16_t    usb_hmsc_rcv_csw (void);
static uint16_t    usb_hmsc_rcv_data (uint8_t *p_buff, uint32_t size);
static uint16_t    usb_hmsc_no_data (void);
static uint16_t    usb_hmsc_send_cbw (void);
static uint16_t    usb_hmsc_send_data (uint8_t *p_buff, uint32_t size);
static void        usb_hmsc_clr_data (uint16_t len, uint8_t *p_buf);
static void        usb_hmsc_set_els_cbw (uint8_t *p_data, uint32_t trans_byte);
static void        usb_hmsc_set_rw_cbw (uint16_t command, uint32_t secno, uint16_t seccnt, uint32_t trans_byte);
static uint16_t    usb_hmsc_rcv_csw_check (uint16_t hmsc_retval);
static void        usb_hmsc_data_act (usb_tskinfo_t *p_mess);
static void        usb_hmsc_phase_err (void);
static void        usb_hmsc_driver_start (void);
static uint16_t    usb_hmsc_set_device_status (uint16_t data);
static usb_er_t    usb_hmsc_mass_storage_reset (usb_hcb_t complete);


/******************************************************************************
Function Name   : usb_hmsc_process
Description     : USB HMSC Task
Arguments       : usb_tskinfo_t *p_mess : 
Return value    : none
******************************************************************************/
void usb_hmsc_process (usb_tskinfo_t *p_mess)
{
    switch (p_mess->msginfo)
    {
        case USB_MSG_HMSC_NO_DATA:
        case USB_MSG_HMSC_DATA_IN:
        case USB_MSG_HMSC_DATA_OUT:
            usb_hmsc_data_act (p_mess);
        break;

        case USB_MSG_HMSC_CBW_ERR:
        case USB_MSG_HMSC_CSW_PHASE_ERR:
            usb_hmsc_phase_err ();
        break;

        default:
            /* Do Nothing */
        break;
    }
}   /* eof usb_hmsc_process() */


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
Function Name   : msc_registration
Description     : Registration of peripheral Communications Devices Driver
Arguments       : none
Return value    : none
******************************************************************************/
void msc_registration (void)
{
    usb_hcdreg_t    driver;

    /* Driver registration */
    driver.ifclass      = USB_IFCLS_MAS;            /* Mass Storage class */
    driver.classcheck   = &usb_hstd_class_check;
    driver.statediagram = &usb_hmsc_device_state;
    usb_hstd_driver_registration (&driver);

} /* eof msc_registration() */

/******************************************************************************
Function Name   : usb_hmsc_device_state
Description     : Open / Close
Arguments       : uint16_t data             : Device address/BC type
                : uint16_t state            : Device state
Return value    : none
******************************************************************************/
static void usb_hmsc_device_state(uint16_t data, uint16_t state)
{
    usb_ctrl_t  ctrl;
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
    usb_compliance_t disp_param;
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */

    switch (state)
    {
        case USB_STS_DETACH:
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
            disp_param.status = USB_CT_DETACH;
            disp_param.pid    = USB_NULL;
            disp_param.vid    = USB_NULL;
            usb_compliance_disp ((void *)&disp_param);

#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
            usb_hmsc_strg_drive_close();
            usb_cstd_pipe_reg_clear();
            usb_cstd_pipe_table_clear();


            usb_cstd_set_event(USB_STS_DETACH, &ctrl);
        break;

        case USB_STS_ATTACH:
#if USB_CFG_BC == USB_CFG_ENABLE
            if (USB_BC_STATE_CDP == data)
            {
                usb_cstd_set_event(USB_STS_BC, &ctrl);
            }

#endif /* USB_CFG_BC == USB_CFG_ENABLE */


#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
            disp_param.status = USB_CT_ATTACH;
            disp_param.pid    = USB_NULL;
            disp_param.vid    = USB_NULL;
                usb_compliance_disp ((void *)&disp_param);

#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
        break;

        case USB_STS_DEFAULT:
            usb_hmsc_driver_start();
        break;

        case USB_STS_CONFIGURED:
            usb_hmsc_set_device_status(USB_HMSC_DEV_ATT);
            usb_cstd_pipe_reg_set();

            /* Storage drive search. */
            usb_hmsc_strg_drive_search (usb_hmsc_drive_complete);
        break;

        case USB_STS_SUSPEND:
        break;

        case USB_STS_RESUME:
            usb_cstd_set_event(USB_STS_RESUME, &ctrl);
        break;

        case USB_STS_OVERCURRENT:
            usb_cstd_set_event (USB_STS_OVERCURRENT, &ctrl);
        break;

        default:
            /* Do Nothing */
        break;
    }

}   /* eof usb_hmsc_device_state() */

/******************************************************************************
Function Name   : usb_hmsc_set_rw_cbw
Description     : CBW parameter initialization for the READ10/WRITE10 command
Arguments       : uint16_t command : 
                : uint32_t secno : 
                : uint16_t seccnt : 
                : uint32_t trans_byte : 
Return value    : none
******************************************************************************/
static void usb_hmsc_set_rw_cbw(uint16_t command, uint32_t secno, uint16_t seccnt, uint32_t trans_byte)
{
    /* CBW parameter set */
    gs_usb_hmsc_cbw.dcbw_tag = gs_usb_hmsc_tag_no;
    /* bit7-bit0 set */
    gs_usb_hmsc_cbw.dcbw_dtl_lo = (uint8_t)trans_byte;
    /* bit15-bit8 set */
    gs_usb_hmsc_cbw.dcbw_dtl_ml = (uint8_t)(trans_byte >> 8);
    /* bit23-bit16 set */
    gs_usb_hmsc_cbw.dcbw_dtl_mh = (uint8_t)(trans_byte >> 16);
    /* bit31-bit24 set */
    gs_usb_hmsc_cbw.dcbw_dtl_hi = (uint8_t)(trans_byte >> 24);
    gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 0;
    gs_usb_hmsc_cbw.bm_cbw_flags.BIT.reserved7 = 0;
    gs_usb_hmsc_cbw.bcbw_lun.BIT.bcbw_lun = 0;
    gs_usb_hmsc_cbw.bcbw_lun.BIT.reserved4 = 0;
    gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 0;
    gs_usb_hmsc_cbw.bcbwcb_length.BIT.reserved3 = 0;

    /* ATAPI_COMMAND */
    gs_usb_hmsc_cbw.cbwcb[0] = (uint8_t)command;
    /* LUN */
    gs_usb_hmsc_cbw.cbwcb[1] = 0x00;
    /* sector address */
    /* bit31-bit24 set */
    gs_usb_hmsc_cbw.cbwcb[2] = (uint8_t)(secno >> 24);
    /* bit23-bit16 set */
    gs_usb_hmsc_cbw.cbwcb[3] = (uint8_t)(secno >> 16);
    /* bit15-bit8 set */
    gs_usb_hmsc_cbw.cbwcb[4] = (uint8_t)(secno >> 8);
    /* bit7-bit0 set */
    gs_usb_hmsc_cbw.cbwcb[5] = (uint8_t)secno;
    /* Reserved */
    gs_usb_hmsc_cbw.cbwcb[6] = 0x00;
    /* Sector length */
    gs_usb_hmsc_cbw.cbwcb[7] = (uint8_t)(seccnt >> 8);
    /* Block address */
    gs_usb_hmsc_cbw.cbwcb[8] = (uint8_t)seccnt;
    /* Control data */
    gs_usb_hmsc_cbw.cbwcb[9] = 0x00;

    /* ATAPI command check */
    switch (command)
    {
        case USB_ATAPI_TEST_UNIT_READY:
        case USB_ATAPI_REQUEST_SENSE:
        case USB_ATAPI_INQUIRY:
        case USB_ATAPI_MODE_SELECT6:
        case USB_ATAPI_MODE_SENSE6:
        case USB_ATAPI_START_STOP_UNIT:
        case USB_ATAPI_PREVENT_ALLOW:
        case USB_ATAPI_READ_FORMAT_CAPACITY:
        case USB_ATAPI_READ_CAPACITY:
            /* Do Nothing */
        break;
        /* Initialized READ CBW TAG */
        case USB_ATAPI_READ10:
            gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 1;
            /* 10bytes */
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 10;
        break;
        /* Initialized WRITE CBW TAG */
        case USB_ATAPI_WRITE10:
            gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 0;
            /* 10bytes */
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 10;
        break;
        case USB_ATAPI_SEEK:
        case USB_ATAPI_WRITE_AND_VERIFY:
        case USB_ATAPI_VERIFY10:
        case USB_ATAPI_MODE_SELECT10:
        case USB_ATAPI_MODE_SENSE10:
        default:
            /* Do Nothing */
        break;
    }

    if (USB_ATAPI == g_usb_hmsc_sub_class)
    {
        /* 12bytes */
        gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = USB_MSC_CBWCB_LENGTH;
    }
}   /* eof usb_hmsc_set_rw_cbw() */

/******************************************************************************
Function Name   : usb_hmsc_set_els_cbw
Description     : CBW parameter initialization for other commands
Arguments       : uint8_t *p_data     : Set CBW data address.
                : uint32_t trans_byte : ATAPI command data transfer size
Return value    : none
******************************************************************************/
static void usb_hmsc_set_els_cbw(uint8_t *p_data, uint32_t trans_byte)
{
    uint8_t     i;

    /* CBW parameter set */
    gs_usb_hmsc_cbw.dcbw_tag = gs_usb_hmsc_tag_no;
    /* bit7-bit0 set */
    gs_usb_hmsc_cbw.dcbw_dtl_lo = (uint8_t)trans_byte;
    /* bit15-bit8 set */
    gs_usb_hmsc_cbw.dcbw_dtl_ml = (uint8_t)(trans_byte >> 8);
    /* bit23-bit16 set */
    gs_usb_hmsc_cbw.dcbw_dtl_mh = (uint8_t)(trans_byte >> 16);
    /* bit31-bit24 set */
    gs_usb_hmsc_cbw.dcbw_dtl_hi = (uint8_t)(trans_byte >> 24);
    /* Receive */
    gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 0;
    gs_usb_hmsc_cbw.bm_cbw_flags.BIT.reserved7 = 0;
    gs_usb_hmsc_cbw.bcbw_lun.BIT.bcbw_lun = 0;
    gs_usb_hmsc_cbw.bcbw_lun.BIT.reserved4 = 0;
    gs_usb_hmsc_cbw.bcbwcb_length.BIT.reserved3 = 0;

    /* WAIT_LOOP */
    for (i = 0; i < 12; i++)
    {
        gs_usb_hmsc_cbw.cbwcb[i] = p_data[i];
    }

    /* ATAPI command check */
    switch (p_data[0])
    {
        /* No data */
        case USB_ATAPI_TEST_UNIT_READY:
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 6;
        break;
        /* Receive */
        case USB_ATAPI_REQUEST_SENSE:
            gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 1;
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 6;
        break;
        /* Send */
        case USB_ATAPI_FORMAT_UNIT:
        break;
        /* Receive */
        case USB_ATAPI_INQUIRY:
            gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 1;
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 6;
        break;
        case USB_ATAPI_MODE_SELECT6:
        case USB_ATAPI_MODE_SENSE6:
        break;
        /* No data */
        case USB_ATAPI_START_STOP_UNIT:
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 6;
        break;
        /* No data */
        case USB_ATAPI_PREVENT_ALLOW:
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 6;
        break;
        /* Receive */
        case USB_ATAPI_READ_FORMAT_CAPACITY:
            gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 1;
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 10;
        break;
        /* Receive */
        case USB_ATAPI_READ_CAPACITY:
            gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 1;
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 10;
        break;
        case USB_ATAPI_READ10:
        case USB_ATAPI_WRITE10:
            /* Do Nothing */
        break;
        case USB_ATAPI_SEEK:
        case USB_ATAPI_WRITE_AND_VERIFY:
        case USB_ATAPI_VERIFY10:
            /* Do Nothing */
        break;
        /* Send */
        case USB_ATAPI_MODE_SELECT10:
            /* Do Nothing */
        break;
        /* Receive */
        case USB_ATAPI_MODE_SENSE10:
            gs_usb_hmsc_cbw.bm_cbw_flags.BIT.cbw_dir = 1;
            gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = 10;
        break;
        default:
            /* Do Nothing */
        break;
    }

    if (USB_ATAPI == g_usb_hmsc_sub_class)
    {
        /* 12bytes */
        gs_usb_hmsc_cbw.bcbwcb_length.BIT.bcbwcb_length = USB_MSC_CBWCB_LENGTH;
    }
}   /* eof usb_hmsc_set_els_cbw() */

/******************************************************************************
Function Name   : usb_hmsc_clr_data
Description     : data clear
Arguments       : uint16_t len   : Clear data size
                : uint8_t *p_buf : Clear data address
Return value    : none
******************************************************************************/
static void usb_hmsc_clr_data(uint16_t len, uint8_t *p_buf)
{
    uint16_t    i;

    /* WAIT_LOOP */
    for (i = 0; i < len; ++i)
    {
        *p_buf++ = 0x00;
    }
}   /* eof usb_hmsc_clr_data() */


/******************************************************************************
Function Name   : usb_hmsc_no_data
Description     : HMSC No data
Arguments       : none
Return value    : uint16_t : USB_OK
******************************************************************************/
static uint16_t usb_hmsc_no_data (void)
{
    usb_tskinfo_t mess;

    mess.keyword = 0;
    gs_usb_hmsc_process = USB_MSG_HMSC_NO_DATA;
    usb_hmsc_specified_path (&mess);
    gs_usb_hmsc_data_seq = USB_SEQ_0;

    return USB_OK; /* USB_OK; */
}   /* eof usb_hmsc_no_data() */

/******************************************************************************
Function Name   : usb_hmsc_data_in
Description     : HMSC Data In
Arguments       : uint8_t *p_buff : Receive data address
                : uint32_t size   : Receive data size
Return value    : uint16_t : USB_OK
******************************************************************************/
static uint16_t usb_hmsc_data_in(uint8_t *p_buff, uint32_t size)
{
    usb_tskinfo_t mess;

    mess.keyword = 0;
    gsp_usb_hmsc_buff = p_buff;
    gs_usb_hmsc_data_size = size;

    gs_usb_hmsc_process = USB_MSG_HMSC_DATA_IN;
    usb_hmsc_specified_path (&mess);
    gs_usb_hmsc_data_seq = USB_SEQ_0;

    return USB_OK; /* USB_OK; */
}   /* eof usb_hmsc_data_in() */

/******************************************************************************
Function Name   : usb_hmsc_data_out
Description     : HMSC Data Out
Arguments       : uint8_t *p_buff : Send data address
                : uint32_t size   : Send data size
Return value    : uint16_t : USB_OK
******************************************************************************/
static uint16_t usb_hmsc_data_out(uint8_t *p_buff, uint32_t size)
{
    usb_tskinfo_t mess;

    mess.keyword = 0;
    gsp_usb_hmsc_buff = p_buff;
    gs_usb_hmsc_data_size = size;
    gs_usb_hmsc_process = USB_MSG_HMSC_DATA_OUT;
    usb_hmsc_specified_path (&mess);
    gs_usb_hmsc_data_seq = USB_SEQ_0;

    return USB_OK; /* USB_OK; */
}   /* eof usb_hmsc_data_out() */

/******************************************************************************
Function Name   : usb_hmsc_data_act
Description     : Receive Data request
Arguments       : usb_tskinfo_t *p_mess : Pointer to usb_tskinfo_t structure
Return value    : none
******************************************************************************/
static void usb_hmsc_data_act(usb_tskinfo_t *p_mess)
{
    uint16_t    hmsc_retval = USB_HMSC_CSW_ERR;
    uint16_t    result;
    uint32_t    size;
    uint8_t     *p_buff;

    p_buff = gsp_usb_hmsc_buff;
    size = gs_usb_hmsc_data_size;
    result = p_mess->keyword;

    switch (gs_usb_hmsc_data_seq)
    {
        case USB_SEQ_0:
            /* CBW */
            usb_hmsc_send_cbw ();
            gs_usb_hmsc_data_seq++;
        break;
        case USB_SEQ_1:
            if (USB_DATA_STALL == result)
            {
                gs_usb_hmsc_data_seq = USB_SEQ_0;
            }
            else if (USB_DATA_OK != result)
            {
                gs_usb_hmsc_data_seq = USB_SEQ_0;
                usb_hmsc_command_result (USB_HMSC_CSW_ERR);
            }
            else
            {
                /* Data */
                switch (gs_usb_hmsc_process)
                {
                    case USB_MSG_HMSC_NO_DATA :
                        gs_usb_hmsc_data_seq = USB_SEQ_3;
                        usb_hmsc_specified_path (p_mess);
                    break;
                    case USB_MSG_HMSC_DATA_OUT :
                        usb_hmsc_send_data (p_buff, size);
                        gs_usb_hmsc_data_seq++;
                    break;
                    case USB_MSG_HMSC_DATA_IN :
                        gs_usb_hmsc_cmd_data_size = 0;
                        usb_hmsc_rcv_data (p_buff, size);
                        gs_usb_hmsc_data_seq++;
                    break;
                    default :
                    break;
                }
            }
        break;

        case USB_SEQ_2:
            if (USB_MSG_HMSC_DATA_OUT == gs_usb_hmsc_process)
            {
                if (USB_DATA_STALL == result) 
                {
                    usb_hmsc_clear_stall (USB_CFG_HMSC_BULK_OUT, usb_hmsc_check_result_stall_pipe_cb);
                    gs_usb_hmsc_data_seq++;
                }
                else if (USB_DATA_OK != result) 
                {
                    usb_hmsc_command_result (USB_HMSC_CSW_ERR);
                    gs_usb_hmsc_data_seq = USB_SEQ_0;
                }
                else
                {
                    /* CSW */
                    gs_usb_hmsc_data_seq = USB_SEQ_3;
                    usb_hmsc_specified_path (p_mess);
                }
            }
            else if (USB_MSG_HMSC_DATA_IN == gs_usb_hmsc_process)
            {
                if (USB_DATA_STALL == result) 
                {
                    usb_hmsc_clear_stall (USB_CFG_HMSC_BULK_IN, usb_hmsc_check_result_stall_pipe_cb);
                    gs_usb_hmsc_data_seq++;
                }
                else if ((USB_DATA_OK != result) && (USB_DATA_SHT != result))
                {
                    usb_hmsc_command_result (USB_HMSC_CSW_ERR);
                    gs_usb_hmsc_data_seq = USB_SEQ_0;
                }
                else
                {
                    gs_usb_hmsc_cmd_data_size = g_usb_hstd_data_cnt[USB_CFG_HMSC_BULK_IN];
                    /* CSW */
                    gs_usb_hmsc_data_seq = USB_SEQ_3;
                    usb_hmsc_specified_path (p_mess);
                }
            }
            else
            {
                /* Do Nothing */
            }
        break;

        case USB_SEQ_3 : /* Check ClearStall and Send CSW */
            usb_hmsc_rcv_csw ();
            gs_usb_hmsc_data_seq++;
        break;


        case USB_SEQ_4:
            gs_usb_hmsc_data_seq = USB_SEQ_0;
            hmsc_retval = usb_hmsc_rcv_csw_check (result);
            switch (hmsc_retval)
            {
                case USB_HMSC_OK:
                    if (USB_YES == gs_usb_hmsc_csw_loop)
                    {
                        gs_usb_hmsc_csw_loop = USB_NO;
                        hmsc_retval = USB_HMSC_CSW_ERR;
                    }
                    usb_hmsc_command_result (hmsc_retval);
                break;
                case USB_HMSC_CSW_ERR:
                    if (USB_MSG_HMSC_STRG_USER_COMMAND != g_usb_hmsc_strg_process)
                    {
                        gs_usb_hmsc_csw_loop = USB_YES;
                        /* Request sense command */
                        usb_hmsc_request_sense ((uint8_t *)&g_usb_hmsc_data);
                    }
                    else
                    {
                        if (USB_YES == gs_usb_hmsc_csw_loop)
                        {
                            gs_usb_hmsc_csw_loop = USB_NO;
                        }
                        usb_hmsc_command_result (USB_HMSC_CSW_ERR);
                    }
                break;
                case USB_HMSC_STALL:
                    usb_hmsc_clear_stall (USB_CFG_HMSC_BULK_IN, usb_hmsc_check_result_stall_pipe_cb);
                    gs_usb_hmsc_data_seq = USB_SEQ_3;
                break;
                case USB_HMSC_CSW_PHASE_ERR:
                    gs_usb_hmsc_process = USB_MSG_HMSC_CSW_PHASE_ERR;
                    gs_usb_hmsc_phase_err_seq = USB_SEQ_0;
                    usb_hmsc_specified_path (p_mess);
                break;
                default:
                    /* Do Nothing */
                break;
            }
        break;
        default:
            usb_hmsc_command_result(USB_HMSC_CSW_ERR);
            gs_usb_hmsc_data_seq = USB_SEQ_0;
        break;
    }
}   /* eof usb_hmsc_data_act() */

/******************************************************************************
Function Name   : usb_hmsc_phase_err
Description     : HMSC Stall Error
Arguments       : none
Return value    : none
******************************************************************************/
static void usb_hmsc_phase_err (void)
{
    switch (gs_usb_hmsc_phase_err_seq)
    {
        case USB_SEQ_0:
            /* MassStorage Reset Class Request*/
            usb_hmsc_mass_storage_reset((usb_hcb_t)usb_hmsc_check_result);
            gs_usb_hmsc_phase_err_seq++;
        break;

        case USB_SEQ_1:
            usb_hmsc_clear_stall (USB_CFG_HMSC_BULK_OUT, usb_hmsc_check_result_stall_pipe_cb);
            gs_usb_hmsc_phase_err_seq++;
        break;
        case USB_SEQ_2:
            usb_hmsc_clear_stall (USB_CFG_HMSC_BULK_IN, usb_hmsc_check_result_stall_pipe_cb);
            gs_usb_hmsc_phase_err_seq++;
        break;
        case USB_SEQ_3:
            usb_hmsc_command_result (USB_HMSC_CSW_PHASE_ERR);
            gs_usb_hmsc_phase_err_seq = USB_SEQ_0;
        break;

        default:
            /* Do Nothing */
        break;
    }
}   /* eof usb_hmsc_phase_err() */

/******************************************************************************
Function Name   : usb_hmsc_specified_path
Description     : Next Process Selector
Arguments       : usb_tskinfo_t *p_mess : Pointer to usb_tskinfo_t structure
Return value    : none
******************************************************************************/
static void usb_hmsc_specified_path (usb_tskinfo_t *p_mess)
{
    /* Send Message for USB Host Device Class. Next phase process. */
    USB_GET_SND(USB_HCLASS_MBX, gs_usb_hmsc_process, &usb_hstd_dummy_function, (usb_strct_t)p_mess->keyword);
}   /* eof usb_hmsc_specified_path() */

/******************************************************************************
Function Name   : usb_hmsc_check_result
Description     : Hub class check result
Arguments       : usb_hutr_t *p_mess : Pointer to usb_tskinfo_t structure
Return value    : none
******************************************************************************/
static void usb_hmsc_check_result (usb_hutr_t *p_mess)
{
    /* Send Message for USB Host Device Class. Complete usb data transfer. */
    USB_GET_SND(USB_HCLASS_MBX, gs_usb_hmsc_process, &usb_hstd_dummy_function, (usb_strct_t)p_mess->status);
}   /* eof usb_hmsc_check_result() */

/******************************************************************************
Function Name   : usb_hmsc_check_result_stall_pipe_cb
Description     : Hub class check result
Arguments       : uint16_t pipe  : 
                : uint16_t state : 
Return value    : none
******************************************************************************/
static void usb_hmsc_check_result_stall_pipe_cb (uint16_t pipe, uint16_t state)
{
    g_usb_hstd_current_pipe = pipe;

    /* Send Message for USB Host Device Class. Clear stall complete. */
    USB_GET_SND(USB_HCLASS_MBX, gs_usb_hmsc_process, &usb_hstd_dummy_function, state);
}   /* eof usb_hmsc_check_result_stall_pipe_cb() */

/******************************************************************************
Function Name   : usb_hmsc_command_result
Description     : Hub class check result
Arguments       : uint16_t result     : Result
Return value    : none
******************************************************************************/
static void usb_hmsc_command_result (uint16_t result)
{
    /* Send Message for StrageDriveTask. Notification ATAPI command processing complete. */
    USB_GET_SND(USB_HSTRG_MBX, g_usb_hmsc_strg_process, &usb_hstd_dummy_function, (usb_strct_t)result);
}   /* eof usb_hmsc_command_result() */

/******************************************************************************
Function Name   : usb_hmsc_send_cbw
Description     : Send CBW
Arguments       : none
Return value    : uint16_t                  : Error Code
******************************************************************************/
static uint16_t usb_hmsc_send_cbw (void)
{
    usb_er_t    err;

    /* Set CBW TAG */
    gs_usb_hmsc_tag_no++;
    if (0UL == gs_usb_hmsc_tag_no)
    {
        gs_usb_hmsc_tag_no = 1UL;
    }

    /* CBW 31Byte transfer */
    err = usb_hmsc_send_data((uint8_t *)&gs_usb_hmsc_cbw, USB_MSC_CBWLENGTH);

    return err;
}   /* eof usb_hmsc_send_cbw() */

/******************************************************************************
Function Name   : usb_hmsc_rcv_data
Description     : Receive Data request
Arguments       : uint8_t *p_buff           : Receive Data Buffer Address
                : uint32_t size             : Receive Data Size
Return value    : uint16_t                  : Error Code
******************************************************************************/
static uint16_t usb_hmsc_rcv_data (uint8_t *p_buff, uint32_t size)
{
    usb_er_t    err;

    gs_usb_hmsc_receive_data.pipenum   = USB_CFG_HMSC_BULK_IN;
    /* Transfer data address */
    gs_usb_hmsc_receive_data.p_tranadr = (void*)p_buff;
    /* Transfer data length */
    gs_usb_hmsc_receive_data.tranlen   = size;
    /* Not control transfer */
    gs_usb_hmsc_receive_data.setup     = USB_NULL;
    /* Call Back Function Info */
    gs_usb_hmsc_receive_data.complete  = (usb_hcb_t)&usb_hmsc_check_result;

    err = usb_hstd_transfer_start(&gs_usb_hmsc_receive_data);
    if (err != USB_OK) 
    {
        return USB_HMSC_SUBMIT_ERR;
    }
    return err;
}   /* eof usb_hmsc_rcv_data() */

/******************************************************************************
Function Name   : usb_hmsc_send_data
Description     : Send Pipe Data
Arguments       : uint8_t  *p_buff           : Data Info Address
                : uint32_t size             : Data Size
Return value    : uint16_t                  : Error Code(USB_OK)
******************************************************************************/
static uint16_t usb_hmsc_send_data(uint8_t *p_buff, uint32_t size)
{
    usb_er_t    err;

    gs_usb_hmsc_send_data.pipenum     = USB_CFG_HMSC_BULK_OUT;
    /* Transfer data address */
    gs_usb_hmsc_send_data.p_tranadr   = (void*)p_buff;
    /* Transfer data length */
    gs_usb_hmsc_send_data.tranlen     = size;
    /* Not control transfer */
    gs_usb_hmsc_send_data.setup       = USB_NULL;
    /* Call Back Function Info */
    gs_usb_hmsc_send_data.complete    = (usb_hcb_t)&usb_hmsc_check_result;

    err = usb_hstd_transfer_start(&gs_usb_hmsc_send_data);
    if (err != USB_OK) 
    {
        return USB_HMSC_SUBMIT_ERR;
    }
    return err;
}   /* eof usb_hmsc_send_data() */


/******************************************************************************
Function Name   : usb_hmsc_rcv_csw
Description     : Receive CSW
Arguments       : none
Return value    : uint16_t                  : Error Code
******************************************************************************/
static uint16_t usb_hmsc_rcv_csw (void)
{
    usb_er_t    err;

    /* CSW 13Byte receive request. */
    err = usb_hmsc_rcv_data((uint8_t *)&gs_usb_hmsc_csw, USB_MSC_CSW_LENGTH);

    return err;
}   /* eof usb_hmsc_rcv_csw() */

/******************************************************************************
Function Name   : usb_hmsc_rcv_csw_check
Description     : Check Receive CSW
Arguments       : uint16_t result      : Return Value
Return value    : uint16_t             : Error Code
******************************************************************************/
static uint16_t usb_hmsc_rcv_csw_check(uint16_t result)
{
    uint32_t    request_size;

    switch (result) 
    {
        case USB_DATA_SHT:
            /* Continue */
        case USB_DATA_OK:
            /* CSW Check */
            /* bit7-bit0 set */
            request_size = gs_usb_hmsc_cbw.dcbw_dtl_lo;
            /* bit15-bit8 set */
            request_size |= (gs_usb_hmsc_cbw.dcbw_dtl_ml << 8);
            /* bit23-bit16 set */
            request_size |= (gs_usb_hmsc_cbw.dcbw_dtl_mh << 16);
            /* bit31-bit24 set */
            request_size |= (gs_usb_hmsc_cbw.dcbw_dtl_hi << 24);
            gs_usb_hmsc_data_residue = request_size - gs_usb_hmsc_cmd_data_size;

            /* Check CSW singature(4byte) */
            if (gs_usb_hmsc_csw.dcsw_signature != USB_MSC_CSW_SIGNATURE)
            {
                return USB_HMSC_CSW_ERR;
            }
            if (gs_usb_hmsc_csw.dcsw_tag != gs_usb_hmsc_cbw.dcbw_tag)
            {
                return USB_HMSC_CSW_ERR;
            }
            switch (gs_usb_hmsc_csw.dcsw_status)
            {
                case USB_MSC_CSW_OK:
                    return USB_HMSC_OK;
                break;
                case USB_MSC_CSW_NG:
                    return USB_HMSC_CSW_ERR;
                break;
                case USB_MSC_CSW_PHASE_ERR:
                    return USB_HMSC_CSW_PHASE_ERR;
                break;
                default:
                break;
            }
            return USB_HMSC_CSW_ERR;
        break;
        case USB_DATA_STALL:
            /* Stall */
            return USB_HMSC_STALL;
        break;
        case USB_DATA_ERR:
            /* Do Nothing */
        break;
        case USB_DATA_OVR:
            /* Do Nothing */
        break;
        default:
            /* Do Nothing */
        break;
    }
    return USB_HMSC_CSW_ERR;
}   /* eof usb_hmsc_rcv_csw_check() */

/******************************************************************************
Function Name   : usb_hmsc_clear_stall
Description     : Clear Stall
Arguments       : uint16_t pipe         : pipe no.
                : usb_cbinfo_t complete : call back
Return value    : USB_OK/USB_ERROR
******************************************************************************/
static usb_er_t usb_hmsc_clear_stall (uint16_t pipe, usb_cbinfo_t complete)
{
    usb_er_t    err;

    err = usb_hstd_change_device_state (complete, USB_DO_CLEAR_STALL, pipe);

    return err;
}   /* eof usb_hmsc_clear_stall () */

/******************************************************************************
Function Name   : usb_hmsc_drive_complete
Description     : Next Process Selector
Argument        : usb_tskinfo_t *p_mess : Pointer to usb_tskinfo_t structure
Return          : none
******************************************************************************/
static void usb_hmsc_drive_complete(usb_tskinfo_t *p_mess)
{
    usb_ctrl_t  ctrl;

    ctrl.type = USB_HMSC;
    usb_cstd_set_event(USB_STS_CONFIGURED, &ctrl);

}   /* eof usb_hmsc_drive_complete() */

/******************************************************************************
Function Name   : usb_hmsc_set_device_status
Description     : Sets HMSCD operation state
Arguments       : uint16_t data : 
Return value    : uint16_t : USB_OK
******************************************************************************/
static uint16_t usb_hmsc_set_device_status (uint16_t data)
{
    gs_usb_hmsc_connect_state = data;
    return USB_OK; /* USB_OK; */
}   /* eof usb_hmsc_set_device_status() */

/******************************************************************************
Function Name   : usb_hmsc_get_device_status
Description     : Responds to HMSCD operation state
Arguments       : none
Return value    : uint16_t : USB_HMSC_DEV_DET/USB_HMSC_DEV_ATT
******************************************************************************/
uint16_t usb_hmsc_get_device_status (void)
{
    return (gs_usb_hmsc_connect_state);
}   /* eof usb_hmsc_get_device_status() */

/******************************************************************************
Function Name   : usb_hmsc_read10
Description     : Read10
Arguments       : uint8_t *p_buff   : Read data store address
                : uint32_t secno  : Read start sector no.
                : uint16_t seccnt : Read sector count
                : uint32_t trans_byte : transfer size.
Return value    : uint16_t : USB_OK
******************************************************************************/
uint16_t usb_hmsc_read10 (uint8_t *p_buff, uint32_t secno, uint16_t seccnt, uint32_t trans_byte)
{
    uint16_t    hmsc_retval;

    /* set CBW parameter */
    usb_hmsc_set_rw_cbw ((uint16_t)USB_ATAPI_READ10, secno, seccnt, trans_byte);

    /* Data IN */
    hmsc_retval = usb_hmsc_data_in(p_buff, trans_byte);
    return (hmsc_retval);
}   /* eof usb_hmsc_read10() */

/******************************************************************************
Function Name   : usb_hmsc_write10
Description     : Write10
Arguments       : uint8_t *p_buff   : Write data store address
                : uint32_t secno  : Write start sector no.
                : uint16_t seccnt : Write sector count
                : uint32_t trans_byte : transfer size.
Return value    : uint16_t : USB_OK
******************************************************************************/
uint16_t usb_hmsc_write10 (uint8_t *p_buff, uint32_t secno, uint16_t seccnt, uint32_t trans_byte)
{
    uint16_t    hmsc_retval;

    /* set CBW parameter */
    usb_hmsc_set_rw_cbw ((uint16_t)USB_ATAPI_WRITE10, secno, seccnt, trans_byte);

    /* Data OUT */
    hmsc_retval = usb_hmsc_data_out(p_buff, trans_byte);
    return (hmsc_retval);
}   /* eof usb_hmsc_write10() */

/******************************************************************************
Function Name   : usb_hmsc_test_unit
Description     : TestUnit
Arguments       : uint16_t : USB_OK
Return value    : uint16_t : USB_OK
******************************************************************************/
uint16_t usb_hmsc_test_unit (void)
{
    uint8_t     cbwcb[USB_MSC_CBWCB_LENGTH];
    uint16_t    hmsc_retval;

    /* Data clear */
    usb_hmsc_clr_data ((uint16_t)USB_MSC_CBWCB_LENGTH, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USB_ATAPI_TEST_UNIT_READY;

    /* Set CBW parameter */
    usb_hmsc_set_els_cbw ((uint8_t*)&cbwcb, (uint32_t)0);

    /* No Data */
    hmsc_retval = usb_hmsc_no_data ();
    return (hmsc_retval);
}   /* eof usb_hmsc_test_unit() */

/******************************************************************************
Function Name   : usb_hmsc_request_sense
Description     : RequestSense
Arguments       : uint8_t *p_buff : Read data store address.
Return value    : uint16_t : USB_OK
******************************************************************************/
uint16_t usb_hmsc_request_sense (uint8_t *p_buff)
{
    uint8_t     cbwcb[USB_MSC_CBWCB_LENGTH];
    uint8_t     length = 18;
    uint16_t    hmsc_retval;

    /* Data clear */
    usb_hmsc_clr_data ((uint16_t)USB_MSC_CBWCB_LENGTH, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USB_ATAPI_REQUEST_SENSE;
    /* Allocation length */
    cbwcb[4] = length;

    /* Set CBW parameter */
    usb_hmsc_set_els_cbw ((uint8_t*)&cbwcb, (uint32_t)length);

    /* Data IN */
    hmsc_retval = usb_hmsc_data_in (p_buff, (uint32_t)length);
    return (hmsc_retval);
}   /* eof usb_hmsc_request_sense() */

/******************************************************************************
Function Name   : usb_hmsc_inquiry
Description     : Inquiry
Arguments       : uint8_t *p_buff : Read data store address.
Return value    : uint16_t : USB_OK
******************************************************************************/
uint16_t usb_hmsc_inquiry (uint8_t *p_buff)
{
    uint8_t     cbwcb[USB_MSC_CBWCB_LENGTH];
    uint8_t     length = 36;
    uint16_t    hmsc_retval;

    /* Data clear */
    usb_hmsc_clr_data ((uint16_t)USB_MSC_CBWCB_LENGTH, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USB_ATAPI_INQUIRY;
    /* Allocation length */
    cbwcb[4] = length;

    /* Set CBW parameter */
    usb_hmsc_set_els_cbw ((uint8_t*)&cbwcb, (uint32_t)length);

    /* Data IN */
    hmsc_retval = usb_hmsc_data_in (p_buff, (uint32_t)length);
    return (hmsc_retval);
}   /* eof usb_hmsc_inquiry() */

/******************************************************************************
Function Name   : usb_hmsc_read_capacity
Description     : ReadCapacity
Arguments       : uint8_t *p_buff : Read data store address.
Return value    : uint16_t : USB_OK
******************************************************************************/
uint16_t usb_hmsc_read_capacity (uint8_t *p_buff)
{
    uint8_t     cbwcb[USB_MSC_CBWCB_LENGTH];
    uint8_t     length = 8;
    uint16_t    hmsc_retval;

    /* Data clear */
    usb_hmsc_clr_data ((uint16_t)USB_MSC_CBWCB_LENGTH, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USB_ATAPI_READ_CAPACITY;

    /* Set CBW parameter */
    usb_hmsc_set_els_cbw ((uint8_t*)&cbwcb, (uint32_t)length);

    /* Data IN */
    hmsc_retval = usb_hmsc_data_in (p_buff, (uint32_t)length);
    return (hmsc_retval);
}   /* eof usb_hmsc_read_capacity() */

/******************************************************************************
Function Name   : usb_hmsc_read_format_capacity
Description     : ReadFormatCapacity
Arguments       : uint8_t *p_buff : Read data store address.
Return value    : uint16_t : USB_OK
******************************************************************************/
uint16_t usb_hmsc_read_format_capacity (uint8_t *p_buff)
{
    uint8_t     cbwcb[USB_MSC_CBWCB_LENGTH];
    uint8_t     length = 0x20;
    uint16_t    hmsc_retval;

    /* Data clear */
    usb_hmsc_clr_data ((uint16_t)USB_MSC_CBWCB_LENGTH, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USB_ATAPI_READ_FORMAT_CAPACITY;
    /* Allocation length */
    cbwcb[8] = length;

    /* Set CBW parameter */
    usb_hmsc_set_els_cbw ((uint8_t*)&cbwcb, (uint32_t)length);

    /* Data IN */
    hmsc_retval = usb_hmsc_data_in (p_buff, (uint32_t)length);
    return (hmsc_retval);
}   /* eof usb_hmsc_read_format_capacity() */

/******************************************************************************
Function Name   : usb_hmsc_get_max_unit
Description     : Get Max LUN request
Argument        : usb_hcb_t complete         : CallBack Function
Return value    : usb_er_t                  : Error Code
******************************************************************************/
usb_er_t usb_hmsc_get_max_unit (usb_hcb_t complete)
{
    usb_er_t err;
    static usb_ctrl_trans_t get_max_lun_table;

    get_max_lun_table.setup.type = 0xFEA1;
    get_max_lun_table.setup.value = 0x0000;
    get_max_lun_table.setup.index = 0x0000;
    get_max_lun_table.setup.length = 0x0001;
    get_max_lun_table.address = USB_DEVICEADDR;

    g_usb_hstd_current_pipe = USB_PIPE0;

    /* Recieve MaxLUN */
    gs_usb_hmsc_class_request.pipenum   = USB_PIPE0;
    gs_usb_hmsc_class_request.p_tranadr = g_usb_hmsc_data;
    gs_usb_hmsc_class_request.tranlen   = 1;
    /* Set Get MAX UNIT setup */
    gs_usb_hmsc_class_request.setup     = (uint16_t *)&get_max_lun_table;
    gs_usb_hmsc_class_request.complete  = complete;

    err = usb_hstd_transfer_start(&gs_usb_hmsc_class_request);
    return err;
}   /* eof usb_hmsc_get_max_unit() */

/******************************************************************************
Function Name   : usb_hmsc_mass_storage_reset
Description     : Mass Strage Reset request
Argument        : usb_hcb_t complete        : Callback Funtion
Return value    : usb_er_t                 : Error Code
******************************************************************************/
static usb_er_t usb_hmsc_mass_storage_reset (usb_hcb_t complete)
{
    usb_er_t err;
    static usb_ctrl_trans_t mass_storage_reset_table;

    mass_storage_reset_table.setup.type = 0xFF21;
    mass_storage_reset_table.setup.value = 0x0000;
    mass_storage_reset_table.setup.index = 0x0000;
    mass_storage_reset_table.setup.length = 0x0000;
    mass_storage_reset_table.address = USB_DEVICEADDR;

    g_usb_hstd_current_pipe = USB_PIPE0;

    /* Set MassStorageReset */
    gs_usb_hmsc_class_request.pipenum   = USB_PIPE0;
    gs_usb_hmsc_class_request.p_tranadr = USB_NULL;
    gs_usb_hmsc_class_request.tranlen   = 0UL;
    /* Set Mass Strage Reset setup */
    gs_usb_hmsc_class_request.setup     = (uint16_t *)&mass_storage_reset_table;
    gs_usb_hmsc_class_request.complete  = complete;

    err = usb_hstd_transfer_start(&gs_usb_hmsc_class_request);
    return err;
}   /* eof usb_hmsc_mass_storage_reset() */

/******************************************************************************
Function Name   : usb_hmsc_prevent_allow
Description     : PreventAllow
Arguments       : uint8_t *p_buff : Read data store address.
Return value    : uint16_t : USB_OK
******************************************************************************/
uint16_t usb_hmsc_prevent_allow (uint8_t *p_buff)
{
    uint8_t     cbwcb[USB_MSC_CBWCB_LENGTH];
    uint16_t    hmsc_retval;

    /* Data clear */
    usb_hmsc_clr_data((uint16_t)USB_MSC_CBWCB_LENGTH, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USB_ATAPI_PREVENT_ALLOW;
    /* Reserved */
    cbwcb[4] = p_buff[0];

    /* Set CBW parameter */
    usb_hmsc_set_els_cbw((uint8_t*)&cbwcb, (uint32_t)0);

    /* No Data */
    hmsc_retval = usb_hmsc_no_data();
    return (hmsc_retval);
}   /* eof usb_hmsc_prevent_allow() */

/******************************************************************************
Function Name   : usb_hmsc_mode_sense10
Description     : ModeSense10
Arguments       : uint8_t *p_buff : [OUT]Pointer to the buffer area/[IN]p_buff[0] Page Code
Return value    : uint16_t : USB_OK
******************************************************************************/
uint16_t usb_hmsc_mode_sense10 (uint8_t *p_buff)
{
    uint8_t     cbwcb[USB_MSC_CBWCB_LENGTH];
    uint8_t     length;
    uint16_t    hmsc_retval;

    /* Data clear */
    usb_hmsc_clr_data((uint16_t)USB_MSC_CBWCB_LENGTH, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USB_ATAPI_MODE_SENSE10;

    /* Set LUN / DBD=1 */
    /* b7-b5:LUN (=0),b4-b0:Reserved */
    cbwcb[1] = 0x00;

    /* b7-b6:PageControl = Current values(=0),b5-b0:PageCode */
    cbwcb[2] = p_buff[0];

    switch (p_buff[0])    /* [IN]p_buff[0] Page Code */
    {
        case 0x01:  /* Read-Write Error Recovery Page */
        case 0x08:  /* Caching Page */
        case 0x1B:  /* Removable Block Access Capacities Page */
            length = 20;    /* 8 + 12 */
        break;
        case 0x1C:  /* Timer & Protect Page */
            length = 16;    /* 8 + 8 */
        break;
        case 0x3F:  /* Return all pages(valid only for MODE SENSE command) */
            length = 44;    /* 8 + 12 + 12 +12 */
        break;
        default:
            return USB_PAR;
        break;
    }

    /* Allocation length */
    cbwcb[7] = 0x00;    /* MSB */
    cbwcb[8] = length;  /* LSB */

    /* Set CBW parameter */
    usb_hmsc_set_els_cbw((uint8_t*)&cbwcb, (uint32_t)length);

    /* Data IN */
    hmsc_retval = usb_hmsc_data_in(p_buff, (uint32_t)length);
    return (hmsc_retval);
}   /* eof usb_hmsc_mode_sense10() */

/******************************************************************************
Function Name   : usb_hmsc_mode_select10
Description     : ModeSelect6
Arguments       : uint8_t *p_buff : Pointer to the buffer area
                :         buff[0]-[7]:Mode Parameter Header
                :         buff[8]-[n]:Page(s)
                :          buff[8] b7:Parameter Savable(PS)
                :          buff[8] b6:Reserved
                :          buff[8] b5-b0:Page Code
Return value    : uint16_t : USB_OK
******************************************************************************/
uint16_t usb_hmsc_mode_select10 (uint8_t *p_buff)
{
    uint8_t     cbwcb[USB_MSC_CBWCB_LENGTH];
    uint8_t     length;
    uint16_t    hmsc_retval;

    /* Data clear */
    usb_hmsc_clr_data((uint16_t)USB_MSC_CBWCB_LENGTH, cbwcb);

    /* Data set */
    /* Command */
    cbwcb[0] = USB_ATAPI_MODE_SELECT10;

    /* Logical Unit Number = 0/ Save Pages(SP) = 0 */
    cbwcb[1] = 0x10;

    switch ((p_buff[8] & 0x3f))
    {
        case 0x01:  /* Read-Write Error Recovery Page */
        case 0x08:  /* Caching Page */
        case 0x1B:  /* Removable Block Access Capacities Page */
            length = 20;    /* 8 + 12 */
        break;
        case 0x1C:  /* Timer & Protect Page */
            length = 16;    /* 8 + 8 */
        break;
        default:
            return USB_PAR;
        break;
    }

    /* Parameter list length */
    cbwcb[7] = 0x00;    /* MSB */
    cbwcb[8] = length;  /* LSB */

    /* Set CBW parameter */
    usb_hmsc_set_els_cbw((uint8_t*)&cbwcb, (uint32_t)length);

    /* Data OUT */
    hmsc_retval = usb_hmsc_data_out(p_buff, (uint32_t)length);
    return (hmsc_retval);
}   /* eof usb_hmsc_mode_select10() */

/******************************************************************************
Function Name   : usb_hmsc_driver_start
Description     : initialized USB_HMSC_TSK
Arguments       : none
Return value    : none
******************************************************************************/
static void usb_hmsc_driver_start (void)
{
    /* 0x00 */
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

    gs_usb_hmsc_cbw.cbwcb[0]  = 0;
    gs_usb_hmsc_cbw.cbwcb[1]  = 0;
    gs_usb_hmsc_cbw.cbwcb[2]  = 0;
    gs_usb_hmsc_cbw.cbwcb[3]  = 0;
    gs_usb_hmsc_cbw.cbwcb[4]  = 0;
    gs_usb_hmsc_cbw.cbwcb[5]  = 0;
    gs_usb_hmsc_cbw.cbwcb[6]  = 0;
    gs_usb_hmsc_cbw.cbwcb[7]  = 0;
    gs_usb_hmsc_cbw.cbwcb[8]  = 0;
    gs_usb_hmsc_cbw.cbwcb[9]  = 0;
    gs_usb_hmsc_cbw.cbwcb[10] = 0;
    gs_usb_hmsc_cbw.cbwcb[11] = 0;
    gs_usb_hmsc_cbw.cbwcb[12] = 0;
    gs_usb_hmsc_cbw.cbwcb[13] = 0;
    gs_usb_hmsc_cbw.cbwcb[14] = 0;
    gs_usb_hmsc_cbw.cbwcb[15] = 0;
}   /* eof usb_hmsc_driver_start() */


/******************************************************************************
 Function Name   : usb_hmsc_strg_cmd_complete
 Description     : ATAPI command transfer complete call-back.
 Argument        : usb_tskinfo_t   *p_mess   : Pointer to usb_tskinfo_t structure
 Return          : none
 ******************************************************************************/
void usb_hmsc_strg_cmd_complete (usb_tskinfo_t *p_mess)
{
    usb_ctrl_t ctrl;

    ctrl.size = 0;
    switch (p_mess->keyword)
    {
        case USB_HMSC_OK:
            ctrl.size = gs_usb_hmsc_data_residue;
            ctrl.status = USB_CSW_SUCCESS;
        break;

        case USB_HMSC_CSW_ERR:
            ctrl.status = USB_CSW_FAIL;
        break;

        case USB_HMSC_CSW_PHASE_ERR:
            ctrl.status = USB_CSW_PHASE;
        break;

        default:
            ctrl.status = USB_CSW_FAIL;
        break;
    }

    usb_cstd_set_event(USB_STS_MSC_CMD_COMPLETE, &ctrl); /* Set Event(USB receive complete)  */
} /* End of function usb_hmsc_strg_cmd_complete() */

/******************************************************************************
End  Of File
******************************************************************************/
