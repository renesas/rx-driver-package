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
 * Copyright (C) 2018(2019) Renesas Electronics Corporation. All rights reserved.
  ******************************************************************************/
/*******************************************************************************
* File Name    : r_usb_dma.c
* Description  : Setting code of DMA
*******************************************************************************/
/*******************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 30.11.2018 1.00 First release
 *         : 31.05.2019 1.11    Added support for GNUC and ICCRX.
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes>, "Project Includes"
*******************************************************************************/
#include "r_usb_basic_mini_if.h"
#include "r_usb_typedef.h"
#include "r_usb_extern.h"
#include "r_usb_bitdefine.h"
#include "r_usb_reg_access.h"            /* Definition of the USB register access macro */

#if USB_CFG_DTC == USB_CFG_ENABLE
#include "r_dtc_rx_if.h"
#endif /* USB_CFG_DTC == USB_CFG_ENABLE */

#if USB_CFG_DMA == USB_CFG_ENABLE
#include "r_dmaca_rx_if.h"
#endif /* USB_CFG_DMA == USB_CFG_ENABLE */

#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
#include "r_usb_dmac.h"

/*******************************************************************************
 Macro definitions
 ******************************************************************************/

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/*******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

uint32_t    g_usb_cstd_dma_size[USB_DMA_USE_CH_MAX];     /* DMA0 and DMA1 buffer size */
uint16_t    g_usb_cstd_dma_fifo[USB_DMA_USE_CH_MAX];     /* DMA0 and DMA1 FIFO buffer size */
uint16_t    g_usb_cstd_dma_pipe[USB_DMA_USE_CH_MAX];     /* DMA0 and DMA1 pipe number */
uint8_t     g_usb_cstd_dma_ch[USB_FIFO_ACCESS_NUM_MAX];  /* DMA ch no. table */
uint8_t     g_usb_dma_is_odd_size[USB_DMA_USE_CH_MAX];   /* fraction size(1-3) */
uint32_t    g_usb_dma_odd_adr[USB_DMA_USE_CH_MAX];       /* fraction data address */

uint32_t    g_usb_cstd_fifo_address[USB_FIFO_ACCESS_NUM_MAX] =
{
    (uint32_t)&USB0.CFIFO.WORD,   /* USB0 CFIFO  address */
    (uint32_t)&USB0.D0FIFO.WORD,  /* USB0 D0FIFO address */
    (uint32_t)&USB0.D1FIFO.WORD,  /* USB0 D1FIFO address */
};

/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/
#if USB_CFG_DTC == USB_CFG_ENABLE
static dtc_transfer_data_t        gs_usb_cstd_dtc_transfer_data[USB_DMA_USE_CH_MAX];

#endif  /* USB_CFG_DTC == USB_CFG_ENABLE */

static usb_dma_int_t   gs_usb_cstd_dma_int;              /* DMA Interrupt Info */



/******************************************************************************
Function Name   : usb_cstd_dma_send_start
Description     : Start transfer using DMA. If transfer size is 0, write
                : more data to buffer.
Arguments       : uint16_t pipe   : Pipe number
                : uint16_t useport: FIFO select
Return value    : none
******************************************************************************/
void usb_cstd_dma_send_start(uint16_t pipe, uint16_t useport)
{
    uint32_t    dma_size;
    uint8_t     *p_data_ptr;
    uint16_t    ch;

    if (USB_HOST == g_usb_cstd_usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
        p_data_ptr = gp_usb_hstd_data_ptr[pipe];
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

    }

    if (USB_PERI == g_usb_cstd_usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
        p_data_ptr = gp_usb_pstd_data[pipe];
#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */

    }

    ch = usb_cstd_dma_ref_ch_no(useport);

    dma_size = g_usb_cstd_dma_size[ch];


    if ((1u == dma_size) || (0u == dma_size))
    {
        if (USB_HOST == g_usb_cstd_usb_mode)
        {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
            g_usb_hstd_current_pipe = pipe;

            usb_cstd_chg_curpipe(pipe, useport, USB_NULL);

            /* Buffer to FIFO data write */
            usb_hstd_buf_to_fifo(useport);

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
        }
        else
        {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
            usb_pstd_buf_to_fifo(pipe, useport);

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */
        }
    }
    else
    {
        dma_size &= 0xfffffffe;             /* 16bit width(Even size) */
        hw_usb_clear_dreqe(useport);        /* DMA Transfer request disable */

        usb_cstd_dma_clear_ir(useport);

        if (dma_size >= g_usb_cstd_dma_fifo[ch])
        {
            /* Fifo size block transfer */
            dma_size = (dma_size - (dma_size % g_usb_cstd_dma_fifo[ch]));
        }
        else
        {
            /* fraction size(0,1) */
            g_usb_dma_is_odd_size[ch] = g_usb_cstd_dma_size[ch] & 1ul;
            g_usb_dma_odd_adr[ch]  = (uint32_t)(p_data_ptr + dma_size);  /* fraction data address */
        }

        g_usb_cstd_dma_size[ch] = dma_size;

        usb_cstd_dma_send_setting((uint32_t)p_data_ptr, useport, dma_size);

        /* Changes the FIFO port by the pipe. */
        usb_cstd_chg_curpipe(pipe, useport, USB_FALSE);

        /* Enable Not Ready Interrupt */
        usb_cstd_nrdy_enable(pipe);

        hw_usb_set_dreqe(useport);
    }
} /* End of function usb_cstd_dma_send_start */

/******************************************************************************
Function Name   : usb_cstd_dma_rcv_start
Description     : Start transfer using DMA. If transfer size is 0, clear DMA.
Arguments       : uint16_t pipe     : Pipe number
                : uint16_t useport  : FIFO select
Return value    : none
******************************************************************************/
void usb_cstd_dma_rcv_start(uint16_t pipe, uint16_t useport)
{
    uint32_t    dma_size;
    uint16_t    mxps;
    uint8_t     *p_data_ptr;
    uint16_t    ch;
    uint16_t    trncnt;

    if (USB_HOST == g_usb_cstd_usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
        p_data_ptr = gp_usb_hstd_data_ptr[pipe];

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
    }
    if (USB_PERI == g_usb_cstd_usb_mode)
    {
#if ( (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI )
        p_data_ptr = gp_usb_pstd_data[pipe];

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */
    }

    ch = usb_cstd_dma_ref_ch_no(useport);

    dma_size = g_usb_cstd_dma_size[ch];
    /* Data size check */
    if (0u != dma_size)
    {
        hw_usb_clear_dreqe(useport);       /* DMA Transfer request disable */

        usb_cstd_dma_clear_ir(useport);

        usb_cstd_dma_rcv_setting((uint32_t)p_data_ptr, useport, dma_size);

        /* Changes the FIFO port by the pipe. */
        usb_cstd_chg_curpipe(pipe, useport, USB_FALSE);

        /* Max Packet Size */
        mxps = usb_cstd_get_maxpacket_size(pipe);

        /* Set Transaction counter */
        trncnt =  (uint16_t)(((dma_size - (uint32_t)1u)/ mxps) + (uint32_t)1u);
        usb_cstd_set_transaction(pipe, trncnt);

        /* Set BUF */
        usb_cstd_set_buf(pipe);

        /* Enable Ready Interrupt */
        hw_usb_set_brdyenb(pipe);

        /* Enable Not Ready Interrupt */
        usb_cstd_nrdy_enable(pipe);

        /* usb fifo set automatic clear mode  */
        hw_usb_clear_dclrm(useport);

        /* Set DREQ enable */
        hw_usb_set_dreqe(useport);

    }
} /* End of function usb_cstd_dma_rcv_start */


/******************************************************************************
Function Name   : usb_cstd_dfifo_end
Description     : Setup external variables used for USB data transfer; to reg-
                : ister if you want to stop the transfer of DMA.
Arguments       : uint16_t useport  : FIFO select
Return value    : none
******************************************************************************/
void usb_cstd_dfifo_end (uint16_t useport)
{
    uint16_t    pipe;
    uint16_t    channel;

    channel = usb_cstd_dma_ref_ch_no (useport);
    pipe = g_usb_cstd_dma_pipe[channel];

    if (USB_HOST == g_usb_cstd_usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
        /* received data size */
        g_usb_hstd_data_cnt[pipe] -= g_usb_cstd_dma_size[channel];

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
    }
    else
    {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
        /* received data size */
        g_usb_pstd_data_cnt[pipe] -= g_usb_cstd_dma_size[channel];

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */
    }
} /* End of function usb_cstd_dfifo_end */

/******************************************************************************
Function Name   : usb_cstd_dma_driver
Description     : USB DMA transfer complete process.
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cstd_dma_driver(void)
{
    if (gs_usb_cstd_dma_int.wp != gs_usb_cstd_dma_int.rp)
    {
        usb_cstd_dma_send_continue(gs_usb_cstd_dma_int.fifo_type[gs_usb_cstd_dma_int.rp]);

        /* Read count up */
        gs_usb_cstd_dma_int.rp = ((gs_usb_cstd_dma_int.rp+1) % USB_INT_BUFSIZE);
    }
} /* End of function usb_cstd_dma_driver */


/******************************************************************************
Function Name   : usb_cstd_dma_send_restart
Description     : Start transfer using DMA0. accsess size 32bytes.
Arguments       : uint32_t src       : transfer data pointer
                : uint32_t data_size : transfer data size
                : uint16_t pipe      : Pipe number
Return value    : none
******************************************************************************/
void usb_cstd_dma_send_restart(uint32_t src, uint32_t data_size, uint8_t pipe)
{
    uint16_t useport;

    if (USB_PIPE1 == pipe)
    {
        useport = USB_D0USE;
    }
    else
    {
        useport = USB_D1USE;
    }

    /* Changes the FIFO port by the pipe. */
    usb_cstd_chg_curpipe(pipe, useport, USB_FALSE);

    hw_usb_clear_dreqe(useport);       /* DMA Transfer request disable */

    usb_cstd_dma_clear_ir(useport);

    /* dma trans setting Divisible by FIFO buffer size  */
    usb_cstd_dma_send_setting(src, useport, data_size);

    /* Enable Not Ready Interrupt */
    usb_cstd_nrdy_enable(pipe);

    /* Set DREQ enable */
    hw_usb_set_dreqe(useport);
} /* End of function usb_cstd_dma_send_restart */


/******************************************************************************
Function Name   : usb_cstd_dma_send_continue
Description     : Set end of DMA transfer. Set to restart DMA trans-
                : fer according to data size of remaining functions to be pro-
                : cessed.
Arguments       : uint16_t useport   : FIFO select
Return value    : none
******************************************************************************/
void usb_cstd_dma_send_continue(uint16_t useport)
{
    uint8_t     *p_src_adr;
    uint16_t    pipe;
    uint32_t    *p_data_cnt;
    uint8_t     *p_data_ptr;
    uint16_t    channel;
    uint16_t    dma_size;
    bool        cpu_write = false;

    channel = usb_cstd_dma_ref_ch_no(useport);
    pipe = g_usb_cstd_dma_pipe[channel];

    if (USB_HOST == g_usb_cstd_usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
        p_data_cnt = &g_usb_hstd_data_cnt[pipe];
        p_data_ptr = gp_usb_hstd_data_ptr[pipe];

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
    }
    if (USB_PERI == g_usb_cstd_usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
    p_data_cnt = &g_usb_pstd_data_cnt[pipe];
    p_data_ptr = gp_usb_pstd_data[pipe];
#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */
    }

    /* trans data smaller than Buffer size */
    /*  equal all data transfer end  */
    if ((*p_data_cnt) < g_usb_cstd_dma_fifo[channel])
    {
        if (g_usb_dma_is_odd_size[channel] > 0)  /* fraction size(0,1) */
        {
            cpu_write = true;           /* Set flag for CPU FIFO Write */
        }
        else
        {
            /* FIFO buffer empty flag clear */
            hw_usb_clear_status_bemp(pipe);

            /* bval control for transfer enable fifo 2 usb control */
            hw_usb_set_bval(useport);

            /* FIFO bufer empty interrupt enable */
            hw_usb_set_bempenb(pipe);
        }
    }
    else
    {
        /* update remaining transfer data size */
        (*p_data_cnt) -= g_usb_cstd_dma_size[channel];

        /* check transfer remaining data */
        if (0u == (*p_data_cnt))
        {
            /* FIFO buffer empty flag clear */
            hw_usb_clear_status_bemp(pipe);

            /* check FIFO_EMPTY / INBUF bit */
            if ((hw_usb_read_pipectr(pipe) & USB_INBUFM) != USB_INBUFM)
            {
                /* DMA transfer function end. call callback function */
                if (USB_HOST == g_usb_cstd_usb_mode)
                {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
                    usb_hstd_data_end(USB_DATA_OK);

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
                }
                if (USB_PERI == g_usb_cstd_usb_mode)
                {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
                    usb_pstd_data_end(pipe, (uint16_t)USB_DATA_NONE);

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */
                }

            }
            else
            {
                /* FIFO bufer empty interrupt enable */
                hw_usb_set_bempenb(pipe);
            }
        }
        else
        {
            /* dma trans remaining data */
            /* DMA source address update */
            p_src_adr = (uint8_t *)((uint32_t)p_data_ptr + g_usb_cstd_dma_size[channel]);

            /* DMA Transfer size update */
            g_usb_cstd_dma_size[channel] = *p_data_cnt;

            dma_size = g_usb_cstd_dma_size[channel];

            g_usb_dma_is_odd_size[channel] = g_usb_cstd_dma_size[channel] & 1ul; /* fraction size(0,1) */
            dma_size &= 0xfffffffe;                                                     /* 16bit width(Even size) */

            g_usb_dma_odd_adr[channel]  = (uint32_t)(p_src_adr + dma_size);   /* fraction data address */

            if (0 != dma_size)
            {
                g_usb_cstd_dma_size[channel] = dma_size;

                /* DMA0 1byte trans */
                usb_cstd_dma_send_restart((uint32_t)p_src_adr, dma_size, pipe);
            }
            else
            {
                cpu_write = true;           /* Set flag for CPU FIFO Write */
            }
        }
    }

    if (true == cpu_write)
    {
        if (USB_HOST == g_usb_cstd_usb_mode)
        {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
            g_usb_hstd_current_pipe = pipe;
            g_usb_hstd_data_cnt[pipe] = (uint32_t)g_usb_dma_is_odd_size[channel];   /* fraction size(1-3) */
            gp_usb_hstd_data_ptr[pipe] = (uint8_t *)g_usb_dma_odd_adr[channel];   /* fraction data address */
            usb_hstd_buf_to_fifo(useport);

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
        }
        else
        {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
            g_usb_pstd_data_cnt[pipe] = (uint32_t)g_usb_dma_is_odd_size[channel];    /* fraction size(1-3) */
            gp_usb_pstd_data[pipe] = (uint8_t *)g_usb_dma_odd_adr[channel];      /* fraction data address */
            usb_pstd_buf_to_fifo(pipe, useport);

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */
        }
        g_usb_dma_is_odd_size[channel] = 0;
    }
} /* End of function usb_cstd_dma_send_continue */


/******************************************************************************
Function Name   : usb_cstd_dma_get_ir_vect
Description     : Get vector no. of USB DxFIFO
Arguments       : uint16_t  useport     : FIFO select
Return value    : Vector no.
******************************************************************************/
uint16_t    usb_cstd_dma_get_ir_vect(uint16_t use_port)
{
    uint16_t    vect;
    
    if (USB_D0USE == use_port)
    {
        vect = IR_USB0_D0FIFO0;
    }
    else
    {
        vect = IR_USB0_D1FIFO0;
    }

    return vect;
} /* End of function usb_cstd_dma_get_ir_vect */

/******************************************************************************
Function Name   : usb_cstd_dma_clear_ir
Description     : Clear Interrupt status of USB DxFIFO.
Arguments       : uint16_t  useport     : FIFO select
Return value    : none
******************************************************************************/
void    usb_cstd_dma_clear_ir(uint16_t use_port)
{
    if (USB_D0USE == use_port)
    {
        IR(USB0,D0FIFO0) = 0;
    }
    else
    {
        IR(USB0,D1FIFO0) = 0;
    }
} /* End of function usb_cstd_dma_clear_ir */


/******************************************************************************
Function Name   : usb_cstd_dma_set_ch_no
Description     : Set DMA channel no.
Arguments       : uint16_t useport  : FIFO select
                : uint8_t dma_ch_no : DMA channel no.
Return value    : none
******************************************************************************/
void        usb_cstd_dma_set_ch_no(uint16_t use_port, uint8_t dma_ch_no)
{
    g_usb_cstd_dma_ch[use_port] = dma_ch_no;  /* DMA ch no. table */
} /* End of function usb_cstd_dma_set_ch_no */

/******************************************************************************
Function Name   : usb_cstd_dma_ref_ch_no
Description     : Get DMA channel no.
Arguments       : uint16_t  useport : FIFO select
Return value    : DMA channel no.
******************************************************************************/
uint8_t     usb_cstd_dma_ref_ch_no(uint16_t use_port)
{
    return g_usb_cstd_dma_ch[use_port];  /* DMA ch no. table */
} /* End of function usb_cstd_dma_ref_ch_no */

/******************************************************************************
Function Name   : usb_cstd_dma_send_complete
Description     : Set event for DMA transfer complete of Buffer to DxFIFO.
Arguments       : uint16_t useport: FIFO select(USB_D0USE/USB_D1USE)
Return value    : none
******************************************************************************/
void usb_cstd_dma_send_complete(uint16_t use_port)
{
    usb_cstd_dma_stop(use_port);    /* Stop DMA,FIFO access */

    gs_usb_cstd_dma_int.fifo_type[gs_usb_cstd_dma_int.wp] = use_port;
    gs_usb_cstd_dma_int.wp = ((gs_usb_cstd_dma_int.wp+1) % USB_INT_BUFSIZE);

    hw_usb_clear_dreqe(use_port);       /* DMA Transfer request disable */
} /* End of function usb_cstd_dma_send_complete */

/*******************************************************************************
* Function Name: usb_cstd_d0fifo_cb
* Description  : Interrupt service routine of D0FIFO
* Arguments    : none
* Return Value : none
*******************************************************************************/
void usb_cstd_d0fifo_cb (void)
{
    usb_cstd_dma_send_complete(USB_D0USE);
} /* End of function usb_cstd_d0fifo_cb */


/*******************************************************************************
* Function Name: usb_cstd_d1fifo_cb
* Description  : Interrupt service routine of D1FIFO
* Arguments    : none
* Return Value : none
*******************************************************************************/
void usb_cstd_d1fifo_cb (void)
{
    usb_cstd_dma_send_complete(USB_D1USE);
} /* End of function usb_cstd_d1fifo_cb */

#endif  /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */


#if USB_CFG_DMA == USB_CFG_ENABLE
/******************************************************************************
Function Name   : usb_cstd_dma_rcv_setting
Description     : FIFO to Buffer data read DMA start
Arguments       : uint32_t des_addr      : Source address
                : uint16_t useport       : FIFO Access mode
                : uint32_t transfer_size : Transfer size
Return value    : void
******************************************************************************/
void usb_cstd_dma_rcv_setting(uint32_t des_addr, uint16_t useport, uint32_t transfer_size)
{
    dmaca_transfer_data_cfg_t   td_cfg;
    dmaca_stat_t                dmac_status;
    uint8_t                     dma_ch;
    uint16_t                    vect;

    dma_ch = usb_cstd_dma_ref_ch_no(useport);

    vect = usb_cstd_dma_get_ir_vect(useport);

    /* Wait for Complete of DMA transfer. */
    /* WAIT_LOOP */
    do
    {
        R_DMACA_Control(dma_ch, DMACA_CMD_STATUS_GET, &dmac_status);
    }
    while (true == dmac_status.act_stat);

    R_DMACA_Control(dma_ch, DMACA_CMD_DTIF_STATUS_CLR, &dmac_status);

/* DMA test code start */
    /* Operation - No Extended Repeat Area Function and No Offset Subtraction */
    /* Source address is fixed
    * Transfer data size is 32-bit (long word).
    * DMAC transfer mode is Repeat mode & Source side is repeat area
    * At the beginning of transfer, clear the interrupt flag of the activation source
    to 0.
    * Transfer Request source is software. */
    /* Set Transfer data configuration. */
    td_cfg.data_size            = DMACA_DATA_SIZE_WORD;
    td_cfg.transfer_mode        = DMACA_TRANSFER_MODE_BLOCK;
    td_cfg.repeat_block_side    = DMACA_REPEAT_BLOCK_DISABLE;
    td_cfg.act_source           = (dmaca_activation_source_t)vect;
    td_cfg.request_source       = DMACA_TRANSFER_REQUEST_PERIPHERAL;
    td_cfg.dtie_request         = DMACA_TRANSFER_END_INTERRUPT_DISABLE;
    td_cfg.esie_request         = DMACA_TRANSFER_ESCAPE_END_INTERRUPT_DISABLE;
    td_cfg.rptie_request        = DMACA_REPEAT_SIZE_END_INTERRUPT_DISABLE;
    td_cfg.sarie_request        = DMACA_SRC_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    td_cfg.darie_request        = DMACA_DES_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    td_cfg.src_addr_mode        = DMACA_SRC_ADDR_FIXED;
    td_cfg.src_addr_repeat_area = DMACA_SRC_ADDR_EXT_REP_AREA_NONE;
    td_cfg.des_addr_mode        = DMACA_DES_ADDR_INCR;
    td_cfg.des_addr_repeat_area = DMACA_DES_ADDR_EXT_REP_AREA_NONE;
    td_cfg.offset_value         = 0x00000000;
    td_cfg.interrupt_sel        = DMACA_CLEAR_INTERRUPT_FLAG_BEGINNING_TRANSFER;
    td_cfg.p_src_addr           = (void *)g_usb_cstd_fifo_address[useport];
    td_cfg.p_des_addr           = (void *)des_addr;
    td_cfg.transfer_count       = ((transfer_size -1) / g_usb_cstd_dma_fifo[dma_ch]) +1;
    td_cfg.block_size           = g_usb_cstd_dma_fifo[dma_ch] / 2;

    /* Call R_DMACA_Create(). */
    R_DMACA_Create(dma_ch, &td_cfg);

    R_DMACA_Int_Enable(dma_ch, USB_DMA_CH_PRI);

    /* Call R_DMACA_Control().
    Enable DMAC transfer. */
    R_DMACA_Control(dma_ch, DMACA_CMD_ENABLE, &dmac_status);
} /* End of function usb_cstd_dma_rcv_setting */

/******************************************************************************
Function Name   : usb_cstd_dma_send_setting
Description     : Buffer to FIFO data write DMA start
Arguments       : uint32_t  src_adr         : Source address
                : uint16_t  useport         : FIFO Access mode
                : uint32_t  transfer_size   : Transfer size
Return value    : none
******************************************************************************/
void usb_cstd_dma_send_setting(uint32_t src_adr, uint16_t useport, uint32_t transfer_size)
{
    dmaca_transfer_data_cfg_t   td_cfg;
    dmaca_stat_t                dmac_status;
    uint16_t                    vect;
    uint8_t                     dma_ch;

    dma_ch = usb_cstd_dma_ref_ch_no(useport);

    if (USB_D0USE == useport)
    {
        R_DMACA_Int_Callback(dma_ch, (void *) usb_cstd_d0fifo_cb);
    }
    else
    {
        R_DMACA_Int_Callback(dma_ch, (void *) usb_cstd_d1fifo_cb);
    }

    vect = usb_cstd_dma_get_ir_vect(useport);

    /* Wait for Complete of DMA transfer. */
    /* WAIT_LOOP */
    do
    {
        R_DMACA_Control(dma_ch, DMACA_CMD_STATUS_GET, &dmac_status);
    }
    while (true == dmac_status.act_stat);

    R_DMACA_Control(dma_ch, DMACA_CMD_DTIF_STATUS_CLR, &dmac_status);

/* DMA test code start */
    /* Operation - No Extended Repeat Area Function and No Offset Subtraction */
    /* Source address is fixed
    * Transfer data size is 32-bit (long word).
    * DMAC transfer mode is Repeat mode & Source side is repeat area
    * At the beginning of transfer, clear the interrupt flag of the activation source
    to 0.
    * Transfer Request source is software. */
    /* Set Transfer data configuration. */
    td_cfg.data_size            = DMACA_DATA_SIZE_WORD;
    td_cfg.transfer_mode        = DMACA_TRANSFER_MODE_BLOCK;
    td_cfg.repeat_block_side    = DMACA_REPEAT_BLOCK_DISABLE;
    td_cfg.act_source           = (dmaca_activation_source_t)vect;
    td_cfg.request_source       = DMACA_TRANSFER_REQUEST_PERIPHERAL;
    td_cfg.dtie_request         = DMACA_TRANSFER_END_INTERRUPT_ENABLE;
    td_cfg.esie_request         = DMACA_TRANSFER_ESCAPE_END_INTERRUPT_DISABLE;
    td_cfg.rptie_request        = DMACA_REPEAT_SIZE_END_INTERRUPT_DISABLE;
    td_cfg.sarie_request        = DMACA_SRC_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    td_cfg.darie_request        = DMACA_DES_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    td_cfg.src_addr_mode        = DMACA_SRC_ADDR_INCR;
    td_cfg.src_addr_repeat_area = DMACA_SRC_ADDR_EXT_REP_AREA_NONE;
    td_cfg.des_addr_mode        = DMACA_DES_ADDR_FIXED;
    td_cfg.des_addr_repeat_area = DMACA_DES_ADDR_EXT_REP_AREA_NONE;
    td_cfg.offset_value         = 0x00000000;
    td_cfg.interrupt_sel        = DMACA_CLEAR_INTERRUPT_FLAG_BEGINNING_TRANSFER;
    td_cfg.p_src_addr           = (void *)src_adr;
    td_cfg.p_des_addr           = (void *)g_usb_cstd_fifo_address[useport];
    td_cfg.transfer_count       = ((transfer_size -1) / g_usb_cstd_dma_fifo[dma_ch]) +1;

    if (g_usb_cstd_dma_fifo[dma_ch] > transfer_size)
    {
        td_cfg.block_size           = transfer_size / 2;
    }
    else
    {
        td_cfg.block_size           = g_usb_cstd_dma_fifo[dma_ch] / 2;
    }

    /* Call R_DMACA_Create(). */
    R_DMACA_Create(dma_ch, &td_cfg);

    R_DMACA_Int_Enable(dma_ch, USB_DMA_CH2_PRI);

    /* Call R_DMACA_Control().
    Enable DMAC transfer. */
    R_DMACA_Control(dma_ch, DMACA_CMD_ENABLE, &dmac_status);

} /* End of function usb_cstd_dma_send_setting */


/******************************************************************************
Function Name   : usb_cstd_dma_stop
Description     : DMA stop
Arguments       : uint16_t  use_port    : FIFO select
Return value    : void
******************************************************************************/
void usb_cstd_dma_stop(uint16_t use_port)
{
    dmaca_stat_t                dmac_status;
    uint8_t                     dma_ch;

    dma_ch = usb_cstd_dma_ref_ch_no(use_port);  /* Get DMA channel no. */

    /* Wait for Complete of DMA transfer. */
    /* WAIT_LOOP */
    do
    {
        R_DMACA_Control(dma_ch, DMACA_CMD_STATUS_GET, &dmac_status);
    }
    while (true == dmac_status.act_stat);

    R_DMACA_Control(dma_ch, DMACA_CMD_DTIF_STATUS_CLR, &dmac_status); /* Clear DMA status */


} /* End of function usb_cstd_dma_stop */


/******************************************************************************
Function Name   : usb_cstd_dma_get_crtb
Description     : Get DMA Current Transaction Byte reg B(CRTB).
Arguments       : uint16_t dma_ch : DMA Channel no.
Return value    : DMA Current Transaction Byte reg B(CRTB)
******************************************************************************/
uint16_t usb_cstd_dma_get_crtb(uint16_t dma_ch)
{
    dmaca_stat_t                dmac_status;

    /* Wait for Complete of DMA transfer. */
    /* WAIT_LOOP */
    do
    {
        R_DMACA_Control(dma_ch, DMACA_CMD_STATUS_GET, &dmac_status);
    }
    while (true == dmac_status.act_stat);

    return dmac_status.transfer_count;

} /* End of function usb_cstd_dma_get_crtb */

#endif  /* USB_CFG_DMA == USB_CFG_ENABLE */

#if USB_CFG_DTC == USB_CFG_ENABLE
/******************************************************************************
Function Name   : usb_cstd_dma_rcv_setting
Description     : FIFO to Buffer data read DMA start
Arguments       : uint32_t des_addr      : Source address
                : uint16_t useport       : FIFO Access mode
                : uint32_t transfer_size : Transfer size
Return value    : void
******************************************************************************/
void usb_cstd_dma_rcv_setting(uint32_t des_addr, uint16_t useport, uint32_t transfer_size)
{
    dtc_transfer_data_cfg_t     td_cfg;
    dtc_stat_t                  dmac_status;
    dtc_cmd_arg_t               args;
    uint8_t                     dma_ch;
    uint16_t                    vect;

    /* DTC Transfer enable
    b0    DTCST    DTC module start bit
    b7-b1 Reserved 0
    */
    R_DTC_Control(DTC_CMD_DTC_START, USB_NULL, 0);

    /* DTC control register (Transfer Information Read No Skip)
    b2-b0 Reserved 0
    b3    Reserved 0
    b4    RRS      DTC Transfer Information Read Skip enable bit
    b7-b5 Reserved 0
    */
    R_DTC_Control(DTC_CMD_DATA_READ_SKIP_DISABLE, USB_NULL, 0);

    /* DTC control register (Transfer Information Read No Skip)
    b2-b0 Reserved 0
    b3    Reserved 0
    b4    RRS      DTC Transfer Information Read Skip enable bit
    b7-b5 Reserved 0
    */
    R_DTC_Control(DTC_CMD_DATA_READ_SKIP_ENABLE, USB_NULL, 0);

    dma_ch = usb_cstd_dma_ref_ch_no(useport);

    vect = usb_cstd_dma_get_ir_vect(useport);

    /* Wait for Complete of DMA transfer. */
    /* WAIT_LOOP */
    do
    {
        R_DTC_Control(DTC_CMD_STATUS_GET, &dmac_status, &args);
    }
    while (true == dmac_status.in_progress);

/* DMA test code start */
    /* Operation - No Extended Repeat Area Function and No Offset Subtraction */
    /* Source address is fixed
    * Transfer data size is 32-bit (long word).
    * DMAC transfer mode is Repeat mode & Source side is repeat area
    * At the beginning of transfer, clear the interrupt flag of the activation source
    to 0.
    * Transfer Request source is software. */
    /* Set Transfer data configuration. */
    td_cfg.data_size             = DTC_DATA_SIZE_WORD;
    td_cfg.transfer_mode         = DTC_TRANSFER_MODE_BLOCK;
    td_cfg.repeat_block_side     = DTC_REPEAT_BLOCK_SOURCE;
    td_cfg.src_addr_mode         = DTC_SRC_ADDR_FIXED;
    td_cfg.dest_addr_mode        = DTC_DES_ADDR_INCR;
    td_cfg.source_addr           = g_usb_cstd_fifo_address[useport];
    td_cfg.dest_addr             = des_addr;
    td_cfg.transfer_count        = ((transfer_size -1) / g_usb_cstd_dma_fifo[dma_ch]) + 1;
    td_cfg.block_size            = g_usb_cstd_dma_fifo[dma_ch]  / 2;

    td_cfg.chain_transfer_enable = DTC_CHAIN_TRANSFER_DISABLE;
    td_cfg.chain_transfer_mode   = DTC_CHAIN_TRANSFER_CONTINUOUSLY;
    td_cfg.response_interrupt    = DTC_INTERRUPT_AFTER_ALL_COMPLETE;

    /* Call R_DTC_Create(). */
    R_DTC_Create((dtc_activation_source_t)vect, &gs_usb_cstd_dtc_transfer_data[dma_ch], &td_cfg, 0);

    if (USB_D0USE == useport)
    {
        /* DTC start enable register (USB0 D0FIFO transfer enable)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        DTCE(USB0, D0FIFO0) = 1;
    }
    else
    {
        /* DTC start enable register (USBA D0FIFO transfer enable)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        DTCE(USB0, D1FIFO0) = 1;
    }
} /* End of function usb_cstd_dma_rcv_setting */

/******************************************************************************
Function Name   : usb_cstd_dma_send_setting
Description     : Buffer to FIFO data write DMA start
Arguments       : uint32_t  src_adr         : Source address
                : uint16_t  useport         : FIFO Access mode
                : uint32_t  transfer_size   : Transfer size
Return value    : none
******************************************************************************/
void usb_cstd_dma_send_setting(uint32_t src_adr, uint16_t useport, uint32_t transfer_size)
{
    dtc_transfer_data_cfg_t     td_cfg;
    dtc_stat_t                  dmac_status;
    dtc_cmd_arg_t               args;
    uint8_t                     dma_ch;
    uint16_t                    vect;

    /* DTC Transfer enable
    b0    DTCST    DTC module start bit
    b7-b1 Reserved 0
    */
    R_DTC_Control(DTC_CMD_DTC_START, USB_NULL, 0);

    /* DTC control register (Transfer Information Read No Skip)
    b2-b0 Reserved 0
    b3    Reserved 0
    b4    RRS      DTC Transfer Information Read Skip enable bit
    b7-b5 Reserved 0
    */
    R_DTC_Control(DTC_CMD_DATA_READ_SKIP_DISABLE, USB_NULL, 0);

    /* DTC control register (Transfer Information Read No Skip)
    b2-b0 Reserved 0
    b3    Reserved 0
    b4    RRS      DTC Transfer Information Read Skip enable bit
    b7-b5 Reserved 0
    */
    R_DTC_Control(DTC_CMD_DATA_READ_SKIP_ENABLE, USB_NULL, 0);

    dma_ch = usb_cstd_dma_ref_ch_no(useport);

    vect = usb_cstd_dma_get_ir_vect(useport);

    /* Wait for Complete of DMA transfer. */
    /* WAIT_LOOP */
    do
    {
        R_DTC_Control(DTC_CMD_STATUS_GET, &dmac_status, &args);
    }
    while (true == dmac_status.in_progress);

/* DMA test code start */
    /* Operation - No Extended Repeat Area Function and No Offset Subtraction */
    /* Source address is fixed
    * Transfer data size is 32-bit (long word).
    * DMAC transfer mode is Repeat mode & Source side is repeat area
    * At the beginning of transfer, clear the interrupt flag of the activation source
    to 0.
    * Transfer Request source is software. */
    /* Set Transfer data configuration. */
    td_cfg.data_size             = DTC_DATA_SIZE_WORD;
    td_cfg.transfer_mode         = DTC_TRANSFER_MODE_BLOCK;
    td_cfg.repeat_block_side     = DTC_REPEAT_BLOCK_DESTINATION;
    td_cfg.src_addr_mode         = DTC_SRC_ADDR_INCR;
    td_cfg.dest_addr_mode        = DTC_DES_ADDR_FIXED;
    td_cfg.source_addr           = src_adr;
    td_cfg.dest_addr             = g_usb_cstd_fifo_address[useport];
    td_cfg.transfer_count        = ((transfer_size -1) / g_usb_cstd_dma_fifo[dma_ch]) +1;
    if (g_usb_cstd_dma_fifo[dma_ch] > transfer_size)
    {
        td_cfg.block_size            = transfer_size / 2;
    }
    else
    {
        td_cfg.block_size            = g_usb_cstd_dma_fifo[dma_ch] / 2;
    }

    td_cfg.chain_transfer_enable = DTC_CHAIN_TRANSFER_DISABLE;
    td_cfg.chain_transfer_mode   = DTC_CHAIN_TRANSFER_CONTINUOUSLY;
    td_cfg.response_interrupt    = DTC_INTERRUPT_AFTER_ALL_COMPLETE;

    /* Call R_DTC_Create(). */
    R_DTC_Create((dtc_activation_source_t)vect, &gs_usb_cstd_dtc_transfer_data[dma_ch], &td_cfg, 0);

    if (USB_D0USE == useport)
    {
        /* DTC start enable register (USB0 D0FIFO transfer enable)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        DTCE (USB0, D0FIFO0) = 1;
        IPR (USB0, D0FIFO0) = USB_CFG_INTERRUPT_PRIORITY;                   /* D0FIFO0 Enable */
    }
    else
    {
        /* DTC start enable register (USBA D0FIFO transfer enable)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        DTCE (USB0, D1FIFO0) = 1;
        IPR (USB0, D1FIFO0) = USB_CFG_INTERRUPT_PRIORITY;                   /* D0FIFO0 Enable */
    }
} /* End of function usb_cstd_dma_send_setting */


/******************************************************************************
Function Name   : usb_cstd_dma_stop
Description     : DMA stop
Arguments       : uint16_t  use_port    : FIFO select
Return value    : void
******************************************************************************/
void usb_cstd_dma_stop(uint16_t use_port)
{
    dtc_stat_t      stat;
    dtc_cmd_arg_t   args;

    /* Wait for Complete of DTC transfer. */
    /* WAIT_LOOP */
    do
    {
        R_DTC_Control(DTC_CMD_STATUS_GET, &stat, &args);
    }
    while (true == stat.in_progress);

    
    /* DTC start enable register (USB0 D0FIFO transfer disable)
    b0    DTCE     DTC start enable bit
    b7-b1 Reserved 0
    */
    if (USB_D0USE == use_port)
    {
        DTCE(USB0, D0FIFO0) = 0;
        IPR(USB0, D0FIFO0) = 0u;                   /* D0FIFO0 Disable */
    }
    else
    {
        DTCE(USB0, D1FIFO0) = 0;
        IPR(USB0, D1FIFO0) = 0u;                   /* D0FIFO0 Disable */
    }
        
    usb_cstd_dma_clear_ir(use_port);

} /* End of function usb_cstd_dma_stop */


/******************************************************************************
Function Name   : usb_cstd_dma_get_crtb
Description     : Get DMA Current Transaction Byte reg B(CRTB).
Arguments       : uint16_t dma_ch : DMA Channel no.
Return value    : DMA Current Transaction Byte reg B(CRTB)
******************************************************************************/
uint16_t usb_cstd_dma_get_crtb(uint16_t dma_ch)
{
    uint16_t        value;
    dtc_stat_t      dmac_status;
    dtc_cmd_arg_t   args;

    /* Wait for Complete of DMA transfer. */
    /* WAIT_LOOP */
    do
    {
        R_DTC_Control(DTC_CMD_STATUS_GET, &dmac_status, &args);
    }
    while (true == dmac_status.in_progress);

    /* Read DTC transfer count (CRB) */
    value = (uint16_t)(gs_usb_cstd_dtc_transfer_data[dma_ch].lw4 & 0xffff);

    return value;

} /* End of function usb_cstd_dma_get_crtb */

#endif  /* USB_CFG_DTC == USB_CFG_ENABLE */


/******************************************************************************
End of file
******************************************************************************/
