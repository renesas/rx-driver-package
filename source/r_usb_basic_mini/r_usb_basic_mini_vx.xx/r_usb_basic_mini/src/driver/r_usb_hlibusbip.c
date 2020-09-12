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
* http://www.renesas.com/disclaimer *
* Copyright (C) 2013(2019) Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_usb_hlibusbip.c
* Version      : 1.01
* Description  : This is the USB host control driver code.
*******************************************************************************/
/*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 01.09.2014 1.00    First Release
*           : 01.06.2015 1.01    Added RX231.
*           : 30.11.2018 1.10    Supporting Smart Configurator
*           : 31.05.2019 1.11    Added support for GNUC and ICCRX.
*******************************************************************************/

/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_usb_basic_mini_if.h"
#include "r_usb_bitdefine.h"
#include "r_usb_typedef.h"
#include "r_usb_reg_access.h"    /* Definition of the USB register access macro */
#include "r_usb_extern.h"
#include "r_usb_dmac.h"

#if defined(USB_CFG_HCDC_USE)
#include "r_usb_hcdc_mini_config.h"

#endif /* defined(USB_CFG_HCDC_USE) */

#if defined(USB_CFG_HHID_USE)
#include "r_usb_hhid_mini_config.h"

#endif /* defined(USB_CFG_HHID_USE) */

#if defined(USB_CFG_HMSC_USE)
#include "r_usb_hmsc_mini_config.h"

#endif /* defined(USB_CFG_HMSC_USE) */

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



#if (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST

/******************************************************************************
Function Name   : usb_hstd_bchg_enable
Description     : Enable port BCHG interrupt
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_bchg_enable(void)
{
    hw_usb_hclear_enb_bchge();
    hw_usb_hset_enb_bchge();
}   /* end of function usb_hstd_bchg_enable() */


/******************************************************************************
Function Name   : usb_hstd_bchg_disable
Description     : Disable port BCHG interrupt
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_bchg_disable(void)
{
    hw_usb_hclear_sts_bchg();
    hw_usb_hclear_enb_bchge();
}   /* end of function usb_hstd_bchg_disable() */


/******************************************************************************
Function Name   : usb_hstd_set_uact
Description     : SOF start
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_set_uact(void)
{
    hw_usb_rmw_dvstctr(USB_UACT, ((USB_USBRST | USB_RESUME) | USB_UACT));
}   /* end of function usb_hstd_set_uact() */


/******************************************************************************
Function Name   : usb_hstd_ovrcr_enable
Description     : Enable OVRCR interrupt
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_ovrcr_enable(void)
{
    hw_usb_hclear_sts_ovrcr();
    hw_usb_hset_enb_ovrcre();
}   /* end of function usb_hstd_ovrcr_enable() */


/******************************************************************************
Function Name   : usb_hstd_ovrcr_disable
Description     : Disable OVRCR interrupt
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_ovrcr_disable(void)
{
    hw_usb_hclear_sts_ovrcr();
    hw_usb_hclear_enb_ovrcre();
}   /* end of function usb_hstd_ovrcr_disable() */


/******************************************************************************
Function Name   : usb_hstd_attch_clear_status
Description     : Clear port ATTCH interrupt status
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_attch_clear_status(void)
{
    hw_usb_hclear_sts_attch();
}   /* end of function usb_hstd_attch_clear_status() */


/******************************************************************************
Function Name   : usb_hstd_attch_disable
Description     : Disable ATTCH interrupt
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_attch_disable(void)
{
    usb_hstd_attch_clear_status();
    hw_usb_hclear_enb_attche();
}   /* end of function usb_hstd_attch_disable() */


/******************************************************************************
Function Name   : usb_hstd_detch_clear_status
Description     : Clear port DTCH interrupt status
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_detch_clear_status(void)
{
    hw_usb_hclear_sts_dtch();
}   /* end of function usb_hstd_detch_clear_status() */


/******************************************************************************
Function Name   : usb_hstd_detach_enable
Description     : Enable DTCH interrupt
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_detach_enable(void)
{
    usb_hstd_detch_clear_status();
    hw_usb_hset_enb_dtche();
}   /* end of function usb_hstd_detach_enable() */


/******************************************************************************
Function Name   : usb_hstd_detach_disable
Description     : Disable DTCH interrupt
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_detach_disable(void)
{
    usb_hstd_detch_clear_status();
    hw_usb_hclear_enb_dtche();
}   /* end of function usb_hstd_detach_disable() */

/******************************************************************************
Function Name   : usb_hstd_bus_int_disable
Description     : Disable USB Bus-interrupt
Argument        : none
Return          : none
******************************************************************************/
void usb_hstd_bus_int_disable(void)
{
    /* ATTCH, DTCH, BCHG  interrupt disable */
    usb_hstd_attch_disable();
    usb_hstd_detach_disable();
    usb_hstd_bchg_disable();
}   /* end of function usb_hstd_bus_int_disable() */

/******************************************************************************
Function Name   : usb_hstd_send_start
Description     : Send Data start
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_send_start(void)
{
    usb_hutr_t       *p_utr;
#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
    uint16_t        use_port;
#endif /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */
    usb_pipe_no_t   pipe;

    pipe = g_usb_hstd_current_pipe;
    /* Evacuation pointer */
    p_utr     = gp_usb_hstd_pipe[pipe];

    /* Set data count, data pointer */
    usb_cstd_set_nak(pipe);
    g_usb_hstd_data_cnt[pipe] = p_utr->tranlen;
    gp_usb_hstd_data_ptr[pipe] = (uint8_t*)p_utr->p_tranadr;

    /* Pipe number to FIFO port select */
    hw_usb_clear_status_bemp(pipe);
    hw_usb_clear_sts_brdy(pipe);

#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
    use_port = usb_cstd_pipe_to_fport(pipe);

    if ((USB_D0USE == use_port) || (USB_D1USE == use_port))
    {
        usb_cstd_dma_set_ch_no (use_port, USB_CFG_USB0_DMA_TX);

        /* Setting for use PIPE number */
        g_usb_cstd_dma_pipe[USB_CFG_USB0_DMA_TX] = pipe;

        /* Buffer size */
        g_usb_cstd_dma_fifo[USB_CFG_USB0_DMA_TX] = usb_cstd_get_maxpacket_size(pipe);
        /* Check data count */
        if (g_usb_hstd_data_cnt[g_usb_cstd_dma_pipe[USB_CFG_USB0_DMA_TX]] <= g_usb_cstd_dma_fifo[USB_CFG_USB0_DMA_TX])
        {
            /* Transfer data size */
            g_usb_cstd_dma_size[USB_CFG_USB0_DMA_TX] 
                                     = (uint16_t)g_usb_hstd_data_cnt[g_usb_cstd_dma_pipe[USB_CFG_USB0_DMA_TX]];
        }
        else
        {
            /* Transfer data size */
            g_usb_cstd_dma_size[USB_CFG_USB0_DMA_TX] 
                                                = g_usb_hstd_data_cnt[g_usb_cstd_dma_pipe[USB_CFG_USB0_DMA_TX]]
                                                  - (g_usb_hstd_data_cnt[g_usb_cstd_dma_pipe[USB_CFG_USB0_DMA_TX]] 
                                                     % g_usb_cstd_dma_fifo[USB_CFG_USB0_DMA_TX]);
        }

        usb_cstd_dma_send_start(pipe, use_port);

        /* Set BUF */
        usb_cstd_set_buf(pipe);
    }
    else
    {
#endif /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */
        /* Buffer to FIFO data write */
        usb_hstd_buf_to_fifo(USB_CUSE);
        usb_cstd_set_buf(pipe);
#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
    }
#endif /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */

}   /* end of function usb_hstd_send_start() */


/******************************************************************************
Function Name   : usb_hstd_receive_start
Description     : Receive Data start
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hstd_receive_start(void)
{
    usb_hutr_t      *p_utr;
    uint16_t        trncnt;
    uint16_t        mxps;
#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
    uint16_t        use_port;
#endif /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */
    usb_pipe_no_t   pipe;

    pipe = g_usb_hstd_current_pipe;

    /* Evacuation pointer */
    p_utr     = gp_usb_hstd_pipe[pipe];

    /* Set data count, data pointer */
    usb_cstd_set_nak(pipe);
    g_usb_hstd_data_cnt[pipe] = p_utr->tranlen;
    gp_usb_hstd_data_ptr[pipe] = (uint8_t*)p_utr->p_tranadr;

#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
    use_port = usb_cstd_pipe_to_fport(pipe);

    if ((USB_D0USE == use_port) || (USB_D1USE == use_port))
    {
        usb_cstd_dma_set_ch_no (use_port, USB_CFG_USB0_DMA_RX);

        g_usb_cstd_dma_pipe[USB_CFG_USB0_DMA_RX] = pipe;              /* Setting for use PIPE number */
        g_usb_cstd_dma_fifo[USB_CFG_USB0_DMA_RX] = usb_cstd_get_maxpacket_size(pipe); /* Buffer size */

        /* Transfer data size */
        g_usb_cstd_dma_size[USB_CFG_USB0_DMA_RX] 
                              = (uint16_t)g_usb_hstd_data_cnt[g_usb_cstd_dma_pipe[USB_CFG_USB0_DMA_RX]];

        usb_cstd_dma_rcv_start(pipe, use_port);
    }
    else
    {
#endif /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */

        /* Pipe number to FIFO port select */
        usb_cstd_chg_curpipe(pipe, USB_CUSE, USB_NULL);

        if (0 != p_utr->tranlen)
        {
            /* Max Packet Size */
            mxps = usb_cstd_get_maxpacket_size(pipe);
            trncnt  = (uint16_t)(p_utr->tranlen/mxps);
            if (( p_utr->tranlen % mxps ) != 0)
            {
                trncnt++;
            }

            /* Set Transaction counter */
            usb_cstd_set_transaction(pipe, trncnt);
        }

        /* Enable Brdy/Nrdy Interrupt */
        usb_cstd_set_buf(pipe);
        hw_usb_set_brdyenb(pipe);
        usb_cstd_nrdy_enable(pipe);
#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
    }
#endif /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */
}   /* end of function usb_hstd_receive_start() */


/******************************************************************************
Function Name   : usb_hstd_data_end
Description     : End of data transfer (IN/OUT)
                : In the case of timeout status, it does not call back.
Arguments       : uint16_t status       ; Transfer status type
Return value    : none
******************************************************************************/
void usb_hstd_data_end(uint16_t status)
{
    usb_hutr_t       *p_utr;
    usb_pipe_no_t    pipe;

    pipe = g_usb_hstd_current_pipe;

    /* Evacuation pointer */
    p_utr     = gp_usb_hstd_pipe[pipe];
    gp_usb_hstd_pipe[pipe]    = (usb_hutr_t*)USB_NULL;

    /* Set NAK */
    usb_cstd_set_nak(pipe);

    /* Disable Interrupt */
    hw_usb_clear_brdyenb(pipe);
    hw_usb_clear_nrdyenb(pipe);
    hw_usb_clear_bempenb(pipe);

    /* Disable Transaction count */
    usb_cstd_clr_transaction(pipe);

#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
    if (USB_D0USE == usb_cstd_pipe_to_fport(pipe) )
    {
        /* DMA buffer clear mode clear */
        hw_usb_clear_dclrm(USB_D0USE);
        hw_usb_set_mbw(USB_D0USE, USB_MBW_16 );
    }
    if (USB_D1USE == usb_cstd_pipe_to_fport(pipe) )
    {
        /* DMA buffer clear mode clear */
        hw_usb_clear_dclrm(USB_D1USE);
        hw_usb_set_mbw(USB_D1USE, USB_MBW_16);
    }
#endif /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */

    /* Call Back */
    if (USB_NULL != p_utr)
    {
        /* Transfer information set */
        p_utr->tranlen    = g_usb_hstd_data_cnt[pipe];
        p_utr->status     = (usb_strct_t)status;
        if (USB_NULL != p_utr->complete)
        {
            (*p_utr->complete)(p_utr);
        }
    }
}   /* end of function usb_hstd_data_end() */


/******************************************************************************
Function Name   : usb_hstd_brdy_pipe_process
Description     : BRDY interrupt
Arguments       : uint16_t bitsts       ; BRDYSTS Register & BRDYENB Register
Return value    : none
******************************************************************************/
void usb_hstd_brdy_pipe_process(uint16_t bitsts)
{
    uint16_t    useport;
#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
    uint16_t buffer;
    uint16_t maxps;
    uint16_t set_dtc_block_cnt;
    uint16_t trans_dtc_block_cnt;
    uint16_t dma_ch;

#endif /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */
    usb_pipe_no_t   pipe;

    /* WAIT_LOOP */
    for (pipe = USB_MIN_PIPE_NO; pipe <= USB_MAX_PIPE_NO; pipe++ )
    {
        if ((bitsts & USB_BITSET(pipe)) != 0 )
        {
            g_usb_hstd_current_pipe = pipe;
            /* Interrupt check */
            hw_usb_clear_status_bemp(pipe);
            if (USB_NULL != gp_usb_hstd_pipe[pipe])
            {
                useport = usb_cstd_pipe_to_fport(pipe);
                if ((USB_D0USE == useport) || (USB_D1USE == useport))
                {
#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
                    dma_ch = usb_cstd_dma_ref_ch_no(useport);
                    maxps = g_usb_cstd_dma_fifo[dma_ch];

                    /* DMA Transfer request disable */
                    hw_usb_clear_dreqe(useport );

                    /* DMA stop */
                    usb_cstd_dma_stop(useport);

                    /* Changes the FIFO port by the pipe. */
                    usb_cstd_chg_curpipe(pipe, useport, USB_NULL);

                    buffer = hw_usb_read_fifoctr(useport);

                    set_dtc_block_cnt = (uint16_t)((g_usb_hstd_data_cnt[g_usb_cstd_dma_pipe[dma_ch]] -1)
                                                                             / g_usb_cstd_dma_fifo[dma_ch]) +1;

                    trans_dtc_block_cnt = usb_cstd_dma_get_crtb(dma_ch);
                    /* Get D0fifo Receive Data Length */
                    g_usb_cstd_dma_size[dma_ch] = buffer & USB_DTLN;
                    if (set_dtc_block_cnt > trans_dtc_block_cnt)
                    {
                        g_usb_cstd_dma_size[dma_ch] += ((set_dtc_block_cnt - (trans_dtc_block_cnt + 1)) * maxps);
                    }

                    /* Check data count */
                    if (g_usb_cstd_dma_size[dma_ch] == g_usb_hstd_data_cnt[pipe] )
                    {
                        g_usb_hstd_data_cnt[pipe] = 0;
                        /* End of data transfer */
                        usb_hstd_data_end( USB_DATA_OK);
                    }
                    else if (g_usb_cstd_dma_size[dma_ch] > g_usb_hstd_data_cnt[pipe] )
                    {
                        /* D0FIFO access DMA stop */
                        usb_cstd_dfifo_end(useport);
                        /* End of data transfer */
                        usb_hstd_data_end( USB_DATA_OVR);
                    }
                    else
                    {
                        /* D0FIFO access DMA stop */
                        usb_cstd_dfifo_end(useport);
                        /* End of data transfer */
                        usb_hstd_data_end( USB_DATA_SHT);
                    }

                    /* Set BCLR */
                    hw_usb_set_bclr(useport );

#endif /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */

                }
                else
                {
                    /* Pipe number to FIFO port select */
                    if (usb_cstd_get_pipe_dir(g_usb_hstd_current_pipe) == USB_BUF2FIFO )
                    {
                        /* Buffer to FIFO data write */
                        usb_hstd_buf_to_fifo(useport);
                    }
                    else
                    {
                        /* FIFO to Buffer data read */
                        usb_hstd_fifo_to_buf();
                    }
                }
            }
        }
    }

}   /* end of function usb_hstd_brdy_pipe_process() */


/******************************************************************************
Function Name   : usb_hstd_nrdy_pipe_process
Description     : Nrdy Pipe interrupt (host only)
Arguments       : uint16_t bitsts       ; NRDYSTS Register & NRDYENB Register
Return value    : none
******************************************************************************/
void usb_hstd_nrdy_pipe_process(uint16_t bitsts)
{
/* PERI spec                                                            */
/*  transmission pipe                                                   */
/* @5       a) only NRDY                                                */
/* @1       b) NRDY+OVRN    (Isochronous)                               */
/*  Receive pipe                                                        */
/* @5       a) only NRDY                                                */
/* @1       b) NRDY+OVRN    (Isochronous)                               */
/* @2       c) only NRDY    (interval error of isochronous)             */
/* HOST spec                                                            */
/*  transmission pipe                                                   */
/* @1       a) NRDY+OVRN    (Isochronous)                               */
/* @4       b) NRDY+NAK     (Ignore)                                    */
/* @3       c) NRDY+STALL   (Recieve STALL)                             */
/*  Receive pipe                                                        */
/* @1       a) NRDY+OVRN    (Isochronous)                               */
/* @4       b) NRDY+NAK     (Ignore)                                    */
/* @2       c) NRDY         (Ignore of isochronous)                     */
/* @2       d) NRDY         (CRC error of isochrnous)                   */
/* @3       e) NRDY+STALL   (Recieve STALL)                             */

/* Condition compilation by the difference of USB function */
    uint16_t        buffer;
    usb_pipe_no_t   pipe;

    /* WAIT_LOOP */
    for (pipe = USB_MIN_PIPE_NO; pipe <= USB_MAX_PIPE_NO; pipe++ )
    {
        if (0 != (bitsts & USB_BITSET(pipe)))
        {
            /* Interrupt check */
            if (USB_NULL != gp_usb_hstd_pipe[pipe])
            {
                g_usb_hstd_current_pipe = pipe;
                buffer = usb_cstd_get_pid(pipe);
                /* STALL ? */
                if (USB_PID_STALL == (buffer & USB_PID_STALL))
                {
                    /* @3 (End of data transfer) */
                    usb_hstd_forced_termination(USB_DATA_STALL);
                }
                else
                {
                    /* @4 (Device Ignore X 3 callback (End of data transfer)) */
                    if (USB_LNST_ATTACH == usb_hstd_check_attach())
                    {
                        usb_hstd_forced_termination(USB_DATA_ERR);
                    }
                    else
                    {
                        usb_hstd_forced_termination(USB_DATA_DTCH);
                    }
                }
            }
        }
    }

}   /* end of function usb_hstd_nrdy_pipe_process() */


/******************************************************************************
Function Name   : usb_hstd_bemp_pipe_process
Description     : BEMP interrupt
Arguments       : uint16_t bitsts       ; BEMPSTS Register & BEMPENB Register
Return value    : none
******************************************************************************/
void usb_hstd_bemp_pipe_process(uint16_t bitsts)
{
    uint16_t    buffer;
#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
    uint16_t    useport;

#endif /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */
    usb_pipe_no_t   pipe;

    /* WAIT_LOOP */
    for (pipe = USB_MIN_PIPE_NO; pipe <= USB_MAX_PIPE_NO; pipe++ )
    {
        if (0 != (bitsts & USB_BITSET(pipe)))
        {
            if ((USB_NULL != gp_usb_hstd_pipe[pipe]) && (USB_ON != g_usb_cstd_bemp_skip[pipe]))
            {
                g_usb_hstd_current_pipe = pipe;
                buffer = usb_cstd_get_pid(pipe);
                /* MAX packet size error ? */
                if ((buffer & USB_PID_STALL) == USB_PID_STALL )
                {
                    usb_hstd_forced_termination(USB_DATA_STALL);
                }
                else
                {
                    /* Because interrupt pipe can read "0" at this bit, the distinction of BULK/INT is unnecessary. */
                    if (USB_INBUFM != (hw_usb_read_pipectr(pipe) & USB_INBUFM))
                    {

#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
                        useport = usb_cstd_pipe_to_fport(pipe);
                        hw_usb_clear_dreqe(useport);
                        /* DMA stop */
                        usb_cstd_dma_stop(useport);

#endif /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */

                        g_usb_cstd_bemp_skip[pipe] = USB_ON;

                        /* End of data transfer */
                        usb_hstd_data_end(USB_DATA_OK);
                    }
                }
            }
        }
    }

}   /* end of function usb_hstd_bemp_pipe_process() */

/******************************************************************************
Function Name   : usb_cstd_pipe_table_set
Description     : Set pipe table
Arguments       : uint16_t class     : USB Device class(USB_HVND/USB_HCDC/USB_HHID/USB_HMSC/USB_HUB)
                : uint8_t *descriptor: Address for Endpoint descriptor
Return value    : Pipe no (USB_PIPE1->USB_PIPE9:OK, 0:Error)
******************************************************************************/
uint8_t         usb_cstd_pipe_table_set (uint16_t usb_class, uint8_t *p_descriptor)
{
    uint8_t     pipe_no;
    uint16_t    pipe_cfg;
    uint16_t    pipe_maxp;
    uint16_t    pipe_peri = 0;

    /* Check Endpoint descriptor */
    if (USB_DT_ENDPOINT != p_descriptor[USB_DEV_B_DESCRIPTOR_TYPE])
    {
        return 0;   /* Error */
    }

    /* set pipe configuration value */
    switch ((uint16_t)(p_descriptor[USB_EP_B_ATTRIBUTES] & USB_EP_TRNSMASK))
    {
        /* Bulk Endpoint */
        case USB_EP_BULK:
            /* Set pipe configuration table */
            if (USB_EP_IN == (p_descriptor[USB_EP_B_ENDPOINTADDRESS] & USB_EP_DIRMASK))
            {
                /* IN(receive) */
                pipe_cfg    = (uint16_t)(USB_TYPFIELD_BULK | USB_CFG_DBLB | USB_SHTNAKON | USB_DIR_H_IN);
                pipe_no     = usb_hstd_get_pipe_no (usb_class, USB_EP_BULK, USB_PIPE_DIR_IN);
            }
            else 
            {
                /* OUT(send) */
                pipe_cfg    = (uint16_t)(USB_TYPFIELD_BULK | USB_CFG_DBLB | USB_DIR_H_OUT);
                pipe_no     = usb_hstd_get_pipe_no (usb_class, USB_EP_BULK, USB_PIPE_DIR_OUT);
            }
        break;

        /* Interrupt Endpoint */
        case USB_EP_INT:
            /* Set pipe configuration table */
            if (USB_EP_IN == (p_descriptor[USB_EP_B_ENDPOINTADDRESS] & USB_EP_DIRMASK))
            {
                /* IN(receive) */
                pipe_cfg    = (uint16_t)(USB_TYPFIELD_INT | USB_DIR_H_IN);
                pipe_no     = usb_hstd_get_pipe_no (usb_class, USB_EP_INT, USB_PIPE_DIR_IN);
            }
            else 
            {
                /* OUT(send) */
                pipe_cfg    = (uint16_t)(USB_TYPFIELD_INT | USB_DIR_H_OUT);
                pipe_no     = usb_hstd_get_pipe_no (usb_class, USB_EP_INT, USB_PIPE_DIR_OUT);
            }

            /* set interval counter */
            /* Get value for Interval Error Detection Interval  */
            pipe_peri = usb_hstd_get_pipe_peri_value (p_descriptor[USB_EP_B_INTERVAL]);
        break;

        default:
            return 0;   /* Error */
        break;
    }

    /* Check Pipe no. */
    if (USB_NULL != pipe_no)
    {
        /* Endpoint number set */
        pipe_cfg    |= (uint16_t)(p_descriptor[USB_EP_B_ENDPOINTADDRESS] & USB_EP_NUMMASK);

        /* set max packet size */
        pipe_maxp   =  (uint16_t)p_descriptor[USB_EP_B_MAXPACKETSIZE_L] | (USB_DEVICEADDR << USB_DEVADDRBIT);

        if (USB_TRUE != g_usb_cstd_pipe_tbl[pipe_no].use_flag)
        {
            g_usb_cstd_pipe_tbl[pipe_no].use_flag  = USB_TRUE;
            g_usb_cstd_pipe_tbl[pipe_no].pipe_cfg  = pipe_cfg;
            g_usb_cstd_pipe_tbl[pipe_no].pipe_maxp = pipe_maxp;
            g_usb_cstd_pipe_tbl[pipe_no].pipe_peri = pipe_peri;
        }
        else
        {
            pipe_no = 0;    /* Error */
        }
    }
    return (pipe_no);
} /* End of function usb_cstd_pipe_table_set() */


/******************************************************************************
Function Name   : usb_hstd_get_pipe_no
Description     : Get PIPE No.
Arguments       : uint16_t class     : USB Device class(USB_HVND/USB_HCDC/USB_HHID/USB_HMSC/USB_HUB)
                : uint8_t  type      : Transfer Type.(USB_EP_BULK/USB_EP_INT)
                : uint8_t  dir       : (USB_PIPE_DIR_IN/USB_PIPE_DIR_OUT)
Return value    : Pipe no (USB_PIPE1->USB_PIPE9:OK, USB_NULL:Error)
******************************************************************************/
uint8_t usb_hstd_get_pipe_no (uint16_t usb_class, uint8_t type, uint8_t dir)
{
    uint8_t     pipe_no = USB_NULL;
#if defined(USB_CFG_HVND_USE)
    uint16_t    pipe;
#endif /* defined(USB_CFG_HVND_USE) */

    switch (usb_class)
    {
        case USB_HVND:
#if defined(USB_CFG_HVND_USE)
            if (USB_EP_BULK == type)
            {   /* BULK PIPE Loop */
                /* WAIT_LOOP */
                for (pipe = USB_BULK_PIPE_START; pipe < (USB_BULK_PIPE_END +1); pipe++)
                {
                    if (USB_FALSE == g_usb_cstd_pipe_tbl[pipe].use_flag)
                    {   /* Check Free pipe */
                        pipe_no = pipe; /* Set Free pipe */
                        break;
                    }
                }
            }
            if (USB_EP_INT == type)
            {   /* Interrupt PIPE Loop */
                /* WAIT_LOOP */
                for (pipe = USB_INT_PIPE_START; pipe < (USB_INT_PIPE_END +1); pipe++)
                {
                    if (USB_FALSE == g_usb_cstd_pipe_tbl[pipe].use_flag)
                    {   /* Check Free pipe */
                        pipe_no = pipe; /* Set Free pipe */
                        break;
                    }
                }
            }

#endif /* defined(USB_CFG_HVND_USE) */
        break;

        case USB_HCDC:
#if defined(USB_CFG_HCDC_USE)
            if (USB_EP_BULK == type)
            {
                if (USB_PIPE_DIR_IN == dir)
                {
                    pipe_no     = USB_CFG_HCDC_BULK_IN;
                }
                else
                {
                    pipe_no     = USB_CFG_HCDC_BULK_OUT;
                }
            }
            if (USB_EP_INT == type)
            {
                if (USB_PIPE_DIR_IN == dir)
                {
                    pipe_no     = USB_CFG_HCDC_INT_IN;
                }
            }
#endif /* defined(USB_CFG_HCDC_USE) */
        break;

        case USB_HHID:
#if defined(USB_CFG_HHID_USE)
            if (USB_EP_INT == type)
            {
                if (USB_PIPE_DIR_IN == dir)
                {
                    pipe_no     = USB_CFG_HHID_INT_IN;
                }
                else
                {   /* Check root port device1 or Hub downport device1 */
                    pipe_no     = USB_CFG_HHID_INT_OUT;
                }
            }
#endif /* defined(USB_CFG_HHID_USE) */
        break;

        case USB_HMSC:
#if defined(USB_CFG_HMSC_USE)
            if (USB_EP_BULK == type)
            {
                if (USB_PIPE_DIR_IN == dir)
                {
                    pipe_no     = USB_CFG_HMSC_BULK_IN;
                }
                else
                {
                    pipe_no     = USB_CFG_HMSC_BULK_OUT;
                }
            }
#endif /* defined(USB_CFG_HMSC_USE) */
        break;

        default:
            return USB_NULL;
        break;
    }

    return pipe_no;
} /* end of function usb_hstd_get_pipe_no() */

/******************************************************************************
Function Name   : usb_hstd_get_pipe_peri_value
Description     : Get value to be set in PIPEPERI
Arguments       : uint8_t   binterval : bInterval for ENDPOINT Descriptor.
Return value    : Value for set PIPEPERI
******************************************************************************/
uint16_t usb_hstd_get_pipe_peri_value (uint8_t binterval)
{
    uint16_t    pipe_peri = USB_NULL;
    uint16_t    work1;
    uint16_t    work2;

    /* set interval counter */
    if (0 != binterval)
    {
        /* The time of the FS/LS interrupt forwarding of the interval is specified by the unit of ms. */
        /* It is necessary to calculate to specify USB-IP by the n-th power of two. */
        /* The NAK rate of the control and the bulk transfer doesn't correspond. */
        work1 = binterval;
        work2 = 0;
        /* WAIT_LOOP */
        for ( ; 0 != work1; work2++ )
        {
            work1 = (uint16_t)(work1 >> 1);
        }
        if (0 != work2)
        {
            /* Interval time */
            pipe_peri |= (uint16_t)(work2 - 1);
        }
    }

    return pipe_peri;
} /* end of function usb_hstd_get_pipe_peri_value() */


#endif /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
/******************************************************************************
End of file
******************************************************************************/
