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
 * Copyright (C) 2015(2024) Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_usb_cdataio.c
 * Description  : USB Host and Peripheral low level data I/O code
 ***********************************************************************************************************************/
/**********************************************************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 01.09.2014 1.00    First Release
*           : 01.06.2015 1.01    Added RX231.
*           : 29.12.2015 1.02    Minor Update.
*           : 30.11.2018 1.10    Supporting Smart Configurator
*           : 31.05.2019 1.11    Added support for GNUC and ICCRX.
*           : 30.06.2020 1.20    Added support for RTOS.
*           : 30.04.2024 1.30    Added support for RX261.
  ***********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_usb_basic_mini_if.h"
#include "r_usb_bitdefine.h"
#include "r_usb_typedef.h"
#include "r_usb_reg_access.h"
#include "r_usb_extern.h"

#if defined(USB_CFG_HCDC_USE)
#include "r_usb_hcdc_mini_config.h"
#endif /* defined(USB_CFG_HCDC_USE) */

#if defined(USB_CFG_PCDC_USE)
#include "r_usb_pcdc_mini_if.h"
#endif /* defined(USB_CFG_PCDC_USE) */

#if defined(USB_CFG_PMSC_USE)
#include "r_usb_pmsc_mini_if.h"
#endif /* defined(USB_CFG_PMSC_USE) */

#if defined(USB_CFG_PHID_USE)
#include "r_usb_phid_mini_if.h"
#endif /* defined(USB_CFG_PHID_USE) */

#if defined(USB_CFG_HHID_USE)
#include "r_usb_hhid_mini_if.h"
#endif /* defined(USB_CFG_HHID_USE) */

#if defined(USB_CFG_HMSC_USE)
#include "r_usb_hmsc_mini_if.h"
#include "r_usb_hmsc.h"
#endif /* defined(USB_CFG_HMSC_USE) */

#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
#include "r_usb_dmac.h"
#endif  /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */

/******************************************************************************
 Macro definitions
 ******************************************************************************/
/* setup packet table size (uint16_t * 5) */
#define     USB_CDC_SETUP_TBL_BSIZE             (10u)

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
/* variables */
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
USB_STATIC usb_ctrl_trans_t     gs_usb_cstd_ctrl_request;

/* Pipe number of USB Host transfer.(Read pipe/Write pipe) */
static const uint8_t g_usb_hstd_pipe_no_tbl[] =
{
    /* READ pipe */         /* WRITE pipe */
    /* IN pipe */           /* OUT pipe */
  #if defined(USB_CFG_HCDC_USE)
    USB_CFG_HCDC_BULK_IN,   USB_CFG_HCDC_BULK_OUT,    /* HCDC(CDC Data class) */
  #else   /* defined(USB_CFG_HCDC_USE) */
    USB_NULL,               USB_NULL,
  #endif
  #if defined(USB_CFG_HCDC_USE)
    USB_CFG_HCDC_INT_IN,    USB_NULL,                  /* HCDCC(CDC Control class) */
  #else
    USB_NULL,               USB_NULL,
  #endif  /* defined(USB_CFG_HCDC_USE) */

  #if defined(USB_CFG_HHID_USE)
    USB_CFG_HHID_INT_IN,    USB_CFG_HHID_INT_OUT,      /* HHID */
  #else   /* defined(USB_CFG_HHID_USE) */
    USB_NULL,               USB_NULL,
  #endif  /* defined(USB_CFG_HHID_USE) */
};

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/
/* variables */
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST) && (BSP_CFG_RTOS_USED == 0)
uint16_t    g_usb_hstd_pipe_request[USB_MAX_PIPE_NO + 1u];
#endif /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST && (BSP_CFG_RTOS_USED == 0) */

#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
/* USB data transfer */
uint32_t    g_usb_pstd_data_cnt[USB_MAX_PIPE_NO + 1u];  /* PIPEn Buffer counter */
uint8_t     *gp_usb_pstd_data[USB_MAX_PIPE_NO + 1u];    /* PIPEn Buffer pointer(8bit) */
usb_putr_t  *gp_usb_pstd_pipe[USB_MAX_PIPE_NO + 1u];    /* Message pipe */

const uint8_t g_usb_pstd_pipe_no_tbl[] =
{
    /* OUT pipe */          /* IN pipe */
  #if defined(USB_CFG_PCDC_USE)
    USB_CFG_PCDC_BULK_OUT,  USB_CFG_PCDC_BULK_IN,   /* USB_PCDC */
    USB_NULL,               USB_CFG_PCDC_INT_IN,    /* USB_PCDCC */
    USB_CFG_PCDC_BULK_OUT2, USB_CFG_PCDC_BULK_IN2,  /* USB_PCDC2 */
    USB_NULL,               USB_CFG_PCDC_INT_IN2,   /* USB_PCDCC2 */
  #else
    USB_NULL,               USB_NULL,
    USB_NULL,               USB_NULL,
    USB_NULL,               USB_NULL,
    USB_NULL,               USB_NULL,
  #endif

  #if defined(USB_CFG_PHID_USE)
    USB_CFG_PHID_INT_OUT,   USB_CFG_PHID_INT_IN,    /* USB_PHID */
    USB_CFG_PHID_INT_OUT2,  USB_CFG_PHID_INT_IN2,    /* USB_PHID2 */
  #else
    USB_NULL,               USB_NULL,
    USB_NULL,               USB_NULL,
  #endif
};


#if defined(USB_CFG_PCDC_USE)

/* Abstract Control Model Notification - SerialState */
uint8_t g_usb_pcdc_serial_state_tbl[USB_CDC_SETUP_TBL_BSIZE] =
{
    0xA1,       /* bmRequestType */
    0x20,       /* bNotification:SERIAL_STATE */
    0x00, 0x00, /* wValue:Zero */
    0x00, 0x00, /* wIndex:Interface */
    0x02, 0x00, /* wLength:2 */
    0x00, 0x00, /* Data:UART State bitmap */
};

#endif /* defined(USB_CFG_PCDC_USE) */

#endif /* ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI) */

usb_pipe_table_t    g_usb_cstd_pipe_tbl[USB_MAXPIPE_NUM+1];

/* Callback function of USB Read/USB Write */
void (*gp_usb_cstd_callback[]) (usb_utr_t *, uint16_t, uint16_t) =
{
    /* PCDC, PCDCC */
  #if defined(USB_CFG_PCDC_USE)
        usb_pcdc_read_complete, usb_pcdc_write_complete, /* USB_PCDC  (0) */
        USB_NULL, usb_pcdc_write_complete, /* USB_PCDCC (1) */
        usb_pcdc_read_complete, usb_pcdc_write_complete, /* USB_PCDC  (2) */
        USB_NULL, usb_pcdc_write_complete, /* USB_PCDCC (3) */
  #else
        USB_NULL, USB_NULL, /* USB_PCDC  (0) */
        USB_NULL, USB_NULL, /* USB_PCDCC (1) */
        USB_NULL, USB_NULL, /* USB_PCDC2  (2) */
        USB_NULL, USB_NULL, /* USB_PCDCC2 (3) */
  #endif
        /* PHID */
  #if defined(USB_CFG_PHID_USE)
        usb_phid_read_complete, usb_phid_write_complete, /* USB_PHID (4) */
        usb_phid_read_complete, usb_phid_write_complete, /* USB_PHID (5) */
  #else
        USB_NULL, USB_NULL, /* USB_PHID (4) */
        USB_NULL, USB_NULL, /* USB_PHID (5) */
  #endif
        /* PVNDR */
        USB_NULL, USB_NULL, /* USB_PVND  (6) */

        USB_NULL, USB_NULL, /* USB_PCDC_PHID(dummy 7) */
        USB_NULL, USB_NULL, /* USB_PCDC_PMSC(dummy 8) */
        USB_NULL, USB_NULL, /* USB_PHID_PMSC(dummy 9) */

        /* HCDC, HCDCC */
#if defined(USB_CFG_HCDC_USE) && (BSP_CFG_RTOS_USED != 0)
        usb_hcdc_read_complete, usb_hcdc_write_complete, /* USB_HCDC  (10) */
        usb_hcdc_read_complete, USB_NULL, /* USB_HCDCC (11) */
#else
        USB_NULL, USB_NULL, /* USB_HCDC  (10) */
        USB_NULL, USB_NULL, /* USB_HCDCC (11) */
#endif
        /* HHID */
#if defined(USB_CFG_HHID_USE) && (BSP_CFG_RTOS_USED != 0)
        usb_hhid_read_complete, usb_hhid_write_complete, /* USB_HHID  (12) */
#else
        USB_NULL, USB_NULL, /* USB_HHID  (12) */
#endif
        /* HVNDR */
        USB_NULL, USB_NULL, /* USB_HVND  (13) */

        /* HMSC */
        USB_NULL, USB_NULL, /* USB_HMSC  (14) */
        /* PMSC */
        USB_NULL, USB_NULL, /* USB_PMSC  (15) */
}; /* const void (gp_usb_cstd_callback[])(usb_utr_t *, uint16_t, uint16_t) */

uint16_t     g_usb_cstd_bemp_skip[USB_MAX_PIPE_NO + 1u];

/******************************************************************************
 Function Name   : usb_cstd_debug_hook
 Description     : Debug hook
 Arguments       : uint16_t error_code          : error code
 Return value    : none
 ******************************************************************************/
void usb_cstd_debug_hook (uint16_t error_code)
{
    /* WAIT_LOOP */
    while (1)
    {
        /* Non */
    }
} /* End of function usb_cstd_debug_hook() */

/******************************************************************************
 Function Name   : usb_cstd_ctrl_receive
 Description     : Receive process for Control transfer
 Arguments       : usb_ctrl_t *p_ctrl: Control structure for USB API.
                 : uint8_t *p_buf    : transfer data address
                 : uint32_t size     : transfer length
 Return value    : usb_er_t          : USB_SUCCESS(USB_OK) / USB_ERROR.
 ******************************************************************************/
usb_er_t usb_cstd_ctrl_receive (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size)
{
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
    usb_er_t err;
#if (BSP_CFG_RTOS_USED != 0)                /* Use RTOS */
    usb_utr_t   tran_data;
    usb_utr_t   *p_tran_data;
#else  /* BSP_CFG_RTOS_USED != 0 */
    usb_sutr_t   *p_tran_data;
#endif /* BSP_CFG_RTOS_USED != 0 */

    if (USB_HOST == g_usb_cstd_usb_mode)
    {
#if (BSP_CFG_RTOS_USED != 0)                 /* Use RTOS */
        p_tran_data = (usb_utr_t *)&tran_data;
        p_tran_data->read_req_len = size;   /* Save Read Request Length */
        p_tran_data->keyword = USB_PIPE0;   /* Pipe No */
        p_tran_data->p_setup =
                (uint16_t *) &gs_usb_cstd_ctrl_request;
#else  /* BSP_CFG_RTOS_USED != 0 */
        p_tran_data = (usb_sutr_t *)&g_usb_hstd_data[USB_PIPE0];
        p_tran_data->read_req_len = size;
        p_tran_data->pipenum = USB_PIPE0; /* Pipe No */
        gs_usb_cstd_ctrl_request.address = USB_DEVICEADDR;
        p_tran_data->setup =
                (uint16_t *) &gs_usb_cstd_ctrl_request;
#endif /* BSP_CFG_RTOS_USED != 0 */
        p_tran_data->p_tranadr = p_buf; /* Data address */
        p_tran_data->tranlen = size; /* Data Size */

        /* Callback function */
        p_tran_data->complete = usb_hclass_request_complete;
        gs_usb_cstd_ctrl_request.setup = p_ctrl->setup;

        /* Setup message address set */
        err = usb_hstd_transfer_start(p_tran_data);
        return err;
    }
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
    if (USB_PERI == g_usb_cstd_usb_mode)
    {
        if (USB_ON == g_usb_pstd_pipe0_request)
        {
            return USB_QOVR;
        }
        g_usb_pstd_std_request = USB_YES;
        usb_pstd_ctrl_write(size, p_buf);
    }
#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */
    return USB_SUCCESS;
} /* End of function usb_cstd_ctrl_receive() */

/******************************************************************************
 Function Name   : usb_cstd_ctrl_send
 Description     : Send process for Control transfer
 Arguments       : usb_ctrl_t *p_ctrl: Control structure for USB API.
                 : uint8_t *p_buf    : transfer data address
                 : uint32_t size     : transfer length
 Return value    : usb_er_t          : USB_SUCCESS(USB_OK) / USB_ERROR.
 ******************************************************************************/
usb_er_t usb_cstd_ctrl_send (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size)
{
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
    usb_er_t err;
#if (BSP_CFG_RTOS_USED != 0)                /* Use RTOS */
    usb_utr_t   tran_data;
    usb_utr_t   *p_tran_data;
#else  /* BSP_CFG_RTOS_USED != 0 */
    usb_sutr_t   *p_tran_data;
#endif /* BSP_CFG_RTOS_USED != 0 */

    if (USB_HOST == g_usb_cstd_usb_mode)
    {
#if (BSP_CFG_RTOS_USED != 0)                /* Use RTOS */
        p_tran_data = (usb_utr_t *)&tran_data;
        p_tran_data->read_req_len = size;   /* Save Read Request Length */
        p_tran_data->keyword = USB_PIPE0;   /* Pipe No */
        p_tran_data->p_setup =
                (uint16_t *) &gs_usb_cstd_ctrl_request;
#else  /* (BSP_CFG_RTOS_USED != 0) */
        p_tran_data = (usb_sutr_t *)&g_usb_hstd_data[USB_PIPE0];
        p_tran_data->read_req_len = size;
        p_tran_data->pipenum = USB_PIPE0; /* Pipe No */
        gs_usb_cstd_ctrl_request.address = USB_DEVICEADDR;
        p_tran_data->setup = (uint16_t *) &gs_usb_cstd_ctrl_request;
#endif /* (BSP_CFG_RTOS_USED != 0) */

        p_tran_data->p_tranadr = p_buf; /* Data address */
        p_tran_data->tranlen = size; /* Data Size */

        /* Callback function */
        p_tran_data->complete = usb_hclass_request_complete;
        gs_usb_cstd_ctrl_request.setup = p_ctrl->setup;

        /* Setup message address set */
        err = usb_hstd_transfer_start(p_tran_data);
        return err;
    }
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
    usb_ctrl_t ctrl;

    if (USB_PERI == g_usb_cstd_usb_mode)
    {
        if ((USB_NULL == p_buf) && (USB_NULL == size))
        {
            if (USB_ACK == p_ctrl->status)
            {
                usb_cstd_set_buf((uint16_t) USB_PIPE0);   /* Set BUF */
            }
            else /* USB_STALL */
            {
                usb_pstd_set_stall_pipe0();
            }

            ctrl.setup  = p_ctrl->setup;                            /* Save setup data. */
            ctrl.size   = 0;
            ctrl.status = USB_ACK;
            ctrl.type   = USB_REQUEST;
            usb_cstd_set_event(USB_STS_REQUEST_COMPLETE, &ctrl);
        }
        else
        {
            if (USB_ON == g_usb_pstd_pipe0_request)
            {
                return USB_QOVR;
            }
            g_usb_pstd_std_request = USB_YES;
            usb_pstd_ctrl_read(size, p_buf);
        }
    }
#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */
    return USB_SUCCESS;
} /* End of function usb_cstd_ctrl_send() */

/******************************************************************************
 Function Name   : usb_cstd_ctrl_stop
 Description     : Stop of USB Control transfer.
 Arguments       : none
 Return value    : usb_er_t          : USB_OK / USB_ERROR.
 ******************************************************************************/
usb_er_t usb_cstd_ctrl_stop (void)
{
    usb_er_t err = USB_SUCCESS;

    if (USB_HOST == g_usb_cstd_usb_mode)
    {
        /* ( (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST ) */
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
#if (BSP_CFG_RTOS_USED != 0)                /* Use RTOS */
        err = usb_hstd_transfer_end(USB_PIPE0, (uint16_t) USB_DATA_STOP);
#else  /* BSP_CFG_RTOS_USED != 0 */
        usb_hstd_control_end((uint16_t)USB_DATA_STOP);
#endif /* BSP_CFG_RTOS_USED != 0 */
#endif
    }
    else
    {
        /* Peripheral only */
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
        err = usb_pstd_transfer_end(USB_PIPE0);
#endif
    }
    return err;
} /* End of function usb_cstd_ctrl_stop() */

/******************************************************************************
 Function Name   : usb_cstd_data_read
 Description     : USB data read processing
 Arguments       : usb_ctrl_t *p_ctrl: Control structure for USB API.
                 : uint8_t *p_buf    : Transfer data address
                 : uint32_t size     : Transfer length
 Return value    : usb_er_t          : USB_OK / USB_ERROR.
 ******************************************************************************/
usb_er_t usb_cstd_data_read (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size)
{
    uint8_t  pipe;
    usb_er_t err;
#if (BSP_CFG_RTOS_USED != 0)                /* Use RTOS */
    usb_utr_t   tran_data;
    usb_utr_t   *p_tran_data;
#else  /* BSP_CFG_RTOS_USED != 0 */
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
    usb_utr_t   *p_tran_data;
#else  /* ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI) */
    usb_sutr_t   *p_tran_data;
#endif /* ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI) */
#endif /* BSP_CFG_RTOS_USED != 0 */

    pipe = usb_cstd_get_usepipe(p_ctrl, USB_READ);

    if (USB_HOST == g_usb_cstd_usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
#if (BSP_CFG_RTOS_USED != 0)                /* Use RTOS */
        p_tran_data = (usb_utr_t *)&tran_data;
        p_tran_data->read_req_len = size;   /* Save Read Request Length */
        p_tran_data->keyword = pipe;        /* Pipe No */
        p_tran_data->complete = gp_usb_cstd_callback[p_ctrl->type * 2]; /* Callback function */
#else  /* BSP_CFG_RTOS_USED != 0 */
        p_tran_data = (usb_sutr_t *)&g_usb_hstd_data[pipe];
        p_tran_data->read_req_len   = size;
        p_tran_data->pipenum   = pipe; /* Pipe No */
        p_tran_data->complete  = usb_hstd_read_complete; /* Callback function */
#endif /* BSP_CFG_RTOS_USED != 0 */
        p_tran_data->p_tranadr = p_buf; /* Data address */
        p_tran_data->tranlen   = size; /* Data Size */
        err = usb_hstd_transfer_start(p_tran_data);
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
    }
    else
    {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
#if (BSP_CFG_RTOS_USED != 0)                /* Use RTOS */
        p_tran_data = (usb_utr_t *)&tran_data;
#else  /* BSP_CFG_RTOS_USED != 0 */
        p_tran_data = (usb_utr_t *)&g_usb_pstd_data[pipe];
#endif /* BSP_CFG_RTOS_USED != 0 */
        p_tran_data->keyword   = pipe; /* Pipe No */
        p_tran_data->read_req_len   = size;
        p_tran_data->p_tranadr = p_buf; /* Data address */
        p_tran_data->tranlen   = size; /* Data Size */
        p_tran_data->complete  = (usb_cb_t)gp_usb_cstd_callback[p_ctrl->type*2]; /* Callback function */
        err = usb_pstd_transfer_start(p_tran_data);
#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */
    }
    return err;
} /* End of function usb_cstd_data_read() */

/******************************************************************************
 Function Name   : usb_cstd_data_write
 Description     : USB data write processing
 Arguments       : usb_ctrl_t *p_ctrl: Control structure for USB API.
                 : uint8_t *p_buf    : Transfer data address
                 : uint32_t size     : Transfer length
 Return value    : usb_er_t          : USB_OK / USB_ERROR.
 ******************************************************************************/
usb_er_t usb_cstd_data_write (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size)
{
    uint8_t  pipe;
    usb_er_t err;
#if (BSP_CFG_RTOS_USED != 0)                /* Use RTOS */
    usb_utr_t   tran_data;
    usb_utr_t   *p_tran_data;
#else  /* BSP_CFG_RTOS_USED != 0 */
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
    usb_utr_t   *p_tran_data;
#else  /* ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI) */
    usb_sutr_t   *p_tran_data;
#endif /* ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI) */
#endif /* BSP_CFG_RTOS_USED != 0 */

    pipe = usb_cstd_get_usepipe(p_ctrl, USB_WRITE);

    if (USB_HOST == g_usb_cstd_usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
#if (BSP_CFG_RTOS_USED != 0)                /* Use RTOS */
        p_tran_data = (usb_utr_t *)&tran_data;
        p_tran_data->keyword = pipe;        /* Pipe No */
        p_tran_data->complete = gp_usb_cstd_callback[(p_ctrl->type * 2) + 1]; /* Callback function */
#else  /* BSP_CFG_RTOS_USED != 0 */
        p_tran_data = (usb_sutr_t *)&g_usb_hstd_data[pipe];
        p_tran_data->pipenum   = pipe; /* Pipe No */
        p_tran_data->complete  = usb_hstd_write_complete; /* Callback function */
#endif /* BSP_CFG_RTOS_USED != 0 */
        p_tran_data->p_tranadr = p_buf; /* Data address */
        p_tran_data->tranlen   = size; /* Data Size */
        err = usb_hstd_transfer_start(p_tran_data);
#endif
    }
    else
    {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
#if (BSP_CFG_RTOS_USED != 0)                /* Use RTOS */
        p_tran_data = (usb_utr_t *)&tran_data;
#else  /* BSP_CFG_RTOS_USED != 0 */
        p_tran_data = (usb_utr_t *)&g_usb_pstd_data[pipe];
#endif /* BSP_CFG_RTOS_USED != 0 */
        p_tran_data->keyword = pipe; /* Pipe No */

#if defined(USB_CFG_PCDC_USE)
        if ((USB_CFG_PCDC_INT_IN != pipe) || (USB_CFG_PCDC_INT_IN2 != pipe))
        {
            p_tran_data->p_tranadr = p_buf; /* Data address */
            p_tran_data->tranlen = size; /* Data Size */
        }
        else
        {
            g_usb_pcdc_serial_state_tbl[8] = p_buf[0];
            g_usb_pcdc_serial_state_tbl[9] = p_buf[1];
            p_tran_data->p_tranadr = g_usb_pcdc_serial_state_tbl; /* Data address */
            p_tran_data->tranlen = 10; /* Data Size */
        }

#else   /* defined(USB_CFG_PCDC_USE) */
        p_tran_data->p_tranadr = p_buf; /* Data address */
        p_tran_data->tranlen = size; /* Data Size */
#endif  /* defined(USB_CFG_PCDC_USE) */
        p_tran_data->complete = (usb_cb_t)gp_usb_cstd_callback[(p_ctrl->type*2)+1]; /* Callback function */
        err = usb_pstd_transfer_start(p_tran_data);
#endif /* ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI) */
    }

    return err;
} /* End of function usb_cstd_data_write() */

/******************************************************************************
 Function Name   : usb_cstd_data_stop
 Description     : USB data transfer stop processing
 Arguments       : usb_ctrl_t *p_ctrl: control structure for USB API.
                 : uint16_t type     : Read(0)/Write(1)
 Return value    : usb_er_t          : USB_OK / USB_ERROR.
 ******************************************************************************/
usb_er_t usb_cstd_data_stop (usb_ctrl_t *p_ctrl, uint16_t type)
{
    uint8_t  pipe;
    usb_er_t err = USB_ERROR;

    pipe = usb_cstd_get_usepipe(p_ctrl, type);

    if (USB_NULL == pipe)
    {
        return USB_ERROR;
    }

    if (USB_HOST == g_usb_cstd_usb_mode)
    {
        /* Host only */
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
#if (BSP_CFG_RTOS_USED != 0)                /* Use RTOS */
        err = usb_hstd_transfer_end(pipe, (uint16_t) USB_DATA_STOP);
#else  /* BSP_CFG_RTOS_USED != 0 */
        if (USB_ON == g_usb_hstd_pipe_request[pipe])
        {
            return USB_ERROR;
        }

        g_usb_hstd_current_pipe = pipe;
        usb_hstd_forced_termination(USB_DATA_STOP);
        err = USB_OK;
#endif /* BSP_CFG_RTOS_USED != 0 */
#endif
    }
    else
    {
        /* Peripheral only */
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
        err = usb_pstd_transfer_end(pipe);
#endif
    }
    return err;
} /* End of function usb_cstd_data_stop() */

/******************************************************************************
 Function Name   : usb_cstd_get_usepipe
 Description     : Get pipe number for USB Read/USB Write
 Arguments       : usb_ctrl_t *p_ctrl: Control structure for USB API.
                 : uint8_t dir       : Read(0)/Write(1)
 Return value    : Bitmap of Use pipe
 ******************************************************************************/
uint8_t usb_cstd_get_usepipe (usb_ctrl_t *p_ctrl, uint8_t dir)
{
    uint8_t pipe = USB_NULL;
    uint8_t idx;

    if (USB_HOST == g_usb_cstd_usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
        /* Host */
        idx = ((p_ctrl->type - USB_HCDC) * 2) + dir;
        pipe = g_usb_hstd_pipe_no_tbl[idx];
#endif
    }
    else
    {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
        /* Peripheral */
        idx = (p_ctrl->type * 2) + dir;
        pipe = g_usb_pstd_pipe_no_tbl[idx];
#endif
    }
    return pipe;
} /* End of function usb_cstd_get_usepipe() */

/******************************************************************************
 Function Name   : usb_cstd_pipe_to_fport
 Description     : Get port No. from the specified pipe No. by argument
 Arguments       : uint16_t pipe  : Pipe number.
 Return value    : uint16_t       : FIFO port selector.
 ******************************************************************************/
uint16_t usb_cstd_pipe_to_fport(uint16_t pipe)
{
    uint16_t fifo_mode = USB_CUSE;

    if (USB_MAX_PIPE_NO < pipe)
    {
        return USB_NULL; /* Error */
    }

#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))

    if (USB_PIPE1 == pipe)
    {
        fifo_mode = USB_D0USE;
    }
    if (USB_PIPE2 == pipe)
    {
        fifo_mode = USB_D1USE;
    }

#endif  /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */

    return fifo_mode;
}
/******************************************************************************
 End of function usb_cstd_pipe_to_fport
 ******************************************************************************/

/******************************************************************************
Function Name   : usb_cstd_pipe_table_clear
Description     : Clear pipe table.
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cstd_pipe_table_clear (void)
{
    uint8_t pipe_no;

    /* Search use pipe block */
    /* WAIT_LOOP */
    for (pipe_no = USB_MIN_PIPE_NO; pipe_no < (USB_MAX_PIPE_NO +1); pipe_no++)
    {   /* Check use block */
        if (USB_TRUE == g_usb_cstd_pipe_tbl[pipe_no].use_flag)
        {   /* Clear use block */
            g_usb_cstd_pipe_tbl[pipe_no].use_flag = USB_FALSE;
            g_usb_cstd_pipe_tbl[pipe_no].pipe_cfg = USB_NULL;
            g_usb_cstd_pipe_tbl[pipe_no].pipe_maxp = USB_NULL;
            g_usb_cstd_pipe_tbl[pipe_no].pipe_peri = USB_NULL;
        }
    }

} /* End of function usb_cstd_pipe_table_clear() */

/******************************************************************************
Function Name   : usb_cstd_pipe_reg_set
Description     : Set USB Pipe registers. (Use Pipe All)
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cstd_pipe_reg_set (void)
{
    uint8_t pipe_no;

    /* Current FIFO port Clear */
    usb_cstd_chg_curpipe((uint16_t) USB_PIPE0, (uint16_t) USB_CUSE, USB_FALSE);
    usb_cstd_chg_curpipe((uint16_t) USB_PIPE0, (uint16_t) USB_D0USE, USB_FALSE);
    usb_cstd_chg_curpipe((uint16_t) USB_PIPE0, (uint16_t) USB_D1USE, USB_FALSE);

    /* Search use pipe block */
    /* WAIT_LOOP */
    for (pipe_no = USB_MIN_PIPE_NO; pipe_no < (USB_MAX_PIPE_NO +1); pipe_no++)
    {   /* Check use block */
        if (USB_TRUE == g_usb_cstd_pipe_tbl[pipe_no].use_flag)
        {
            /* Initialization of registers associated with specified pipe. */
            usb_cstd_set_pipe_config (pipe_no);
        }
    }
} /* End of function usb_cstd_pipe_reg_set() */

/******************************************************************************
Function Name   : usb_cstd_pipe_reg_clear
Description     : Clear Pipe registers. (Use Pipe All)
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cstd_pipe_reg_clear (void)
{
    uint8_t pipe_no;

    /* Search use pipe block */
    /* WAIT_LOOP */
    for (pipe_no = USB_MIN_PIPE_NO; pipe_no < (USB_MAX_PIPE_NO +1); pipe_no++)
    {   /* Check use block */
        if (USB_TRUE == g_usb_cstd_pipe_tbl[pipe_no].use_flag)
        {   /* Clear specified pipe configuration register. */
            usb_cstd_clr_pipe_config(pipe_no);
        }
    }
} /* End of function usb_cstd_pipe_reg_clear() */


/******************************************************************************
 End  Of File
******************************************************************************/
