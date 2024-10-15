/**********************************************************************************************************************
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
* Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.    
 *********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_usb_hstdfunction_rtos.c
 * Description  : USB Host standard request related functions.
 ***********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 30.06.2020 1.20    First Release
 ***********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_basic_mini_if.h"
#include "r_usb_typedef.h"
#include "r_usb_extern.h"       /* USB register access function */
#include "r_usb_bitdefine.h"
#include "r_usb_reg_access.h"

#if (BSP_CFG_RTOS_USED != 0)    /* Use RTOS */
#include "r_usb_cstd_rtos.h"
#endif /* (BSP_CFG_RTOS_USED != 0) */

#if defined(USB_CFG_HCDC_USE)
#include "r_usb_hcdc_mini_if.h"
#include "r_usb_hcdc.h"

#endif /* defined(USB_CFG_HCDC_USE) */
#if defined(USB_CFG_HMSC_USE)
#include "r_usb_hmsc_mini_if.h"

#endif /* defined(USB_CFG_HMSC_USE) */
#if defined(USB_CFG_HHID_USE)
#include "r_usb_hhid_mini_if.h"

#endif /* defined(USB_CFG_HHID_USE) */

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
#if (BSP_CFG_RTOS_USED != 0)    /* RTOS */
/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static uint16_t gs_usb_hstd_clr_stall_setup[5];  
static uint16_t g_usb_cstd_driver_open = USB_FALSE;
static uint32_t gs_usb_hstd_data_cnt_pipe0;


/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

/******************************************************************************
 Renesas Abstracted Host Standard functions
 ******************************************************************************/

 /******************************************************************************
 Function Name   : usb_hstd_buf_to_fifo
 Description     : Set USB registers as required to write from data buffer to USB 
                 : FIFO, to have USB FIFO to write data to bus.
 Arguments       : uint16_t pipe     : Pipe no.
                 : uint16_t useport  : Port no.
 Return value    : none
 ******************************************************************************/
void usb_hstd_buf_to_fifo (uint16_t pipe, uint16_t useport)
{
    uint16_t end_flag;

    if (USB_MAX_PIPE_NO < pipe)
    {
        return; /* Error */
    }

    /* Disable Ready Interrupt */
    hw_usb_clear_brdyenb(pipe);

    /* Ignore count clear */
    g_usb_hstd_ignore_cnt[pipe] = (uint16_t) 0;

    end_flag = usb_cstd_buf_to_cfifo(pipe, useport);

    /* Check FIFO access sequence */
    switch (end_flag)
    {
        case USB_WRITING :

            /* Continue of data write */
            /* Enable Ready Interrupt */
            hw_usb_set_brdyenb(pipe);

            /* Enable Not Ready Interrupt */
            usb_cstd_nrdy_enable(pipe);

        break;

        case USB_WRITEEND :

            /* End of data write */
            /* continue */

        case USB_WRITESHRT :

            /* End of data write */
            /* Enable Empty Interrupt */
            hw_usb_set_bempenb(pipe);

            /* Enable Not Ready Interrupt */
            usb_cstd_nrdy_enable(pipe);

        break;
        case USB_FIFOERROR :

            /* FIFO access error */
            usb_hstd_forced_termination(pipe, (uint16_t) USB_DATA_ERR);

        break;
        default :
            usb_hstd_forced_termination(pipe, (uint16_t) USB_DATA_ERR);
        break;
    }
}
/******************************************************************************
 End of function usb_hstd_buf_to_fifo
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_cstd_buf_to_cfifo
 Description     : Switch PIPE, request the USB FIFO to write data, and manage 
                 : the size of written data.
 Arguments       : uint16_t     pipe        : Pipe no.
                 : uint16_t     useport     : CUSE/D0DMA/D1DMA
 Return value    : uint16_t end_flag
 ******************************************************************************/
uint16_t usb_cstd_buf_to_cfifo (uint16_t pipe, uint16_t useport)
{
    uint16_t size;
    uint16_t count;
    uint16_t buffer;
    uint16_t mxps;
    uint16_t end_flag;
    uint16_t read_pid;

    if (USB_MAX_PIPE_NO < pipe)
    {
        return USB_WRITESHRT; /* Error */
    }

    /* Changes FIFO port by the pipe. */
    if ((USB_CUSE == useport) && (USB_PIPE0 == pipe))
    {
        buffer = usb_cstd_is_set_frdy(pipe, (uint16_t) USB_CUSE, (uint16_t) USB_ISEL);
    }
    else
    {
        buffer = usb_cstd_is_set_frdy(pipe, (uint16_t) useport, USB_FALSE);
    }

    /* Check error */
    if (USB_FIFOERROR == buffer)
    {
        /* FIFO access error */
        return (USB_FIFOERROR);
    }

    /* Data buffer size */
    size = usb_cstd_get_maxpacket_size(pipe);

    /* Max Packet Size */
    mxps = usb_cstd_get_maxpacket_size(pipe);

    /* Data size check */
    if (g_usb_hstd_data_cnt[pipe] <= (uint32_t) size)
    {
        count = (uint16_t) g_usb_hstd_data_cnt[pipe];

        /* Data count check */
        if (0 == count)
        {
            /* Null Packet is end of write */
            end_flag = USB_WRITESHRT;
        }
        else if (0 != (count % mxps))
        {
            /* Short Packet is end of write */
            end_flag = USB_WRITESHRT;
        }
        else
        {
            if (USB_PIPE0 == pipe)
            {
                /* Just Send Size */
                end_flag = USB_WRITING;
            }
            else
            {
                /* Write continues */
                end_flag = USB_WRITEEND;
            }
        }
    }
    else
    {
        /* Write continues */
        end_flag = USB_WRITING;
        count = size;
    }

    read_pid = usb_cstd_get_pid(pipe);
    usb_cstd_set_nak(pipe);

    gp_usb_hstd_data_ptr[pipe] = usb_hstd_write_fifo(count, useport, gp_usb_hstd_data_ptr[pipe]);

    /* Check data count to remain */
    if (g_usb_hstd_data_cnt[pipe] < (uint32_t) size)
    {
        /* Clear data count */
        g_usb_hstd_data_cnt[pipe] = (uint32_t) 0u;
        buffer = hw_usb_read_fifoctr(useport); /* Read CFIFOCTR */

        /* Check BVAL */
        if (0u == (buffer & USB_BVAL))
        {
            /* Short Packet */
            hw_usb_set_bval(useport);
        }
    }
    else
    {
        /* Total data count - count */
        g_usb_hstd_data_cnt[pipe] -= count;
    }

    hw_usb_clear_status_bemp(pipe);
    /* USB_PID_BUF ? */
    if (USB_PID_BUF == (USB_PID & read_pid))
    {
        usb_cstd_set_buf(pipe);
    }

    /* End or Err or Continue */
    return end_flag;
}
/******************************************************************************
 End of function usb_cstd_buf_to_cfifo
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_fifo_to_buf
 Description     : Request readout from USB FIFO to buffer and process depending
                 : on status; read complete or reading.
 Arguments       : uint16_t pipe  : Pipe no.
 Return value    : none
 ******************************************************************************/
void usb_hstd_fifo_to_buf (uint16_t pipe, uint16_t useport)
{
    uint16_t end_flag;

    if (USB_MAX_PIPE_NO < pipe)
    {
        return; /* Error */
    }

    /* Ignore count clear */
    g_usb_hstd_ignore_cnt[pipe] = (uint16_t) 0;
    end_flag = USB_ERROR;

    end_flag = usb_hstd_cfifo_to_buf(pipe, useport);

    /* Check FIFO access sequence */
    switch (end_flag)
    {
        case USB_READING :
            /* Continue of data read */
        break;
        case USB_READEND :
            /* End of data read */
            usb_hstd_data_end(pipe, (uint16_t) USB_DATA_OK);
        break;
        case USB_READSHRT :
            /* End of data read */
            usb_hstd_data_end(pipe, (uint16_t) USB_DATA_SHT);
        break;
        case USB_READOVER :
            /* Buffer over */
            usb_hstd_forced_termination(pipe, (uint16_t) USB_DATA_OVR);
        break;
        case USB_FIFOERROR :
            /* FIFO access error */
            usb_hstd_forced_termination(pipe, (uint16_t) USB_DATA_ERR);
        break;
        default :
            usb_hstd_forced_termination(pipe, (uint16_t) USB_DATA_ERR);
        break;
    }
}
/******************************************************************************
 End of function usb_hstd_fifo_to_buf
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_cfifo_to_buf
 Description     : Request to read data from USB FIFO, and manage the size of 
                 : the data read.
 Arguments       : uint16_t     pipe        : Pipe no.
                 : uint16_t     useport     : Pipe mode (CFIFO/D0FIFO/D1FIFO)
 Return value    : USB_READING / USB_READEND / USB_READSHRT / USB_READOVER
 ******************************************************************************/
uint16_t usb_hstd_cfifo_to_buf (uint16_t pipe, uint16_t useport)
{
    uint16_t count;
    uint16_t buffer;
    uint16_t mxps;
    uint16_t dtln;
    uint16_t end_flag;

    if (USB_MAX_PIPE_NO < pipe)
    {
        return USB_ERROR; /* Error */
    }

    /* Changes FIFO port by the pipe. */
    buffer = usb_cstd_is_set_frdy(pipe, (uint16_t) useport, USB_FALSE);
    if (USB_FIFOERROR == buffer)
    {
        /* FIFO access error */
        return (USB_FIFOERROR);
    }
    dtln = (uint16_t) (buffer & USB_DTLN);

    /* Max Packet Size */
    mxps = usb_cstd_get_maxpacket_size(pipe);

    if (g_usb_hstd_data_cnt[pipe] < dtln)        /* Buffer Over ? */
    {
        end_flag = USB_READOVER;
        usb_cstd_set_nak(pipe); /* Set NAK */
        count = (uint16_t) g_usb_hstd_data_cnt[pipe];
        g_usb_hstd_data_cnt[pipe] = dtln;
    }
    else if (g_usb_hstd_data_cnt[pipe] == dtln)           /* Just Receive Size */
    {
        /* Just Receive Size */
        count = dtln;
        end_flag = USB_READEND;
        usb_cstd_set_nak(pipe); /* Set NAK */
    }
    else                                                /* Continues Receive data */
    {
        /* Continus Receive data */
        count = dtln;
        end_flag = USB_READING;
        if ((0 == count) || (0 != (count % mxps)))
        {
            /* Null Packet receive */
            end_flag = USB_READSHRT;
            usb_cstd_set_nak(pipe); /* Select NAK */
        }
    }

    if (0 == dtln)
    {
        /* 0 length packet */
        /* Clear BVAL */
        hw_usb_set_bclr(useport);
    }
    else
    {
        gp_usb_hstd_data_ptr[pipe] = usb_hstd_read_fifo(count, useport,
                gp_usb_hstd_data_ptr[pipe]);
    }
    g_usb_hstd_data_cnt[pipe] -= count;

    /* End or Err or Continue */
    return (end_flag);
}
/******************************************************************************
 End of function usb_hstd_cfifo_to_buf
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_control_write_start
 Description     : Start data stage of Control Write transfer.
 Arguments       : uint32_t  Bsize   : Data Size
                 : uint8_t   *Table  : Data Table Address
 Return          : uint16_t          : USB_WRITEEND / USB_WRITING
                 :                   : USB_WRITESHRT / USB_FIFOERROR
 ******************************************************************************/
uint16_t usb_hstd_control_write_start (uint32_t Bsize, uint8_t *Table)
{
    uint16_t end_flag;

    /* PID=NAK & clear STALL */
    usb_cstd_clr_stall((uint16_t) USB_PIPE0);
    g_usb_hstd_data_cnt[USB_PIPE0] = Bsize; /* Transfer size set */
    gp_usb_hstd_data_ptr[USB_PIPE0] = Table; /* Transfer address set */

    /* DCP Configuration Register  (0x5C) */
    hw_usb_write_dcpcfg(USB_DIRFIELD);
    hw_usb_set_sqset(USB_PIPE0); /* SQSET=1, PID=NAK */
    hw_usb_clear_status_bemp(USB_PIPE0);
    /* Ignore count clear */
    g_usb_hstd_ignore_cnt[USB_PIPE0] = (uint16_t) 0;
    /* Host Control sequence */
    end_flag = usb_cstd_buf_to_cfifo(USB_PIPE0, USB_CUSE);

    switch (end_flag)
    {
        /* End of data write */
        case USB_WRITESHRT :
            /* Next stage is Control write status stage */
            g_usb_hstd_ctsq = USB_STATUSWR;
            /* Enable Empty Interrupt */
            hw_usb_set_bempenb((uint16_t) USB_PIPE0);
            /* Enable Not Ready Interrupt */
            usb_cstd_nrdy_enable((uint16_t) USB_PIPE0);
            /* Set BUF */
            usb_cstd_set_buf((uint16_t) USB_PIPE0);
        break;

        /* End of data write (not null) */
        case USB_WRITEEND :
        /* continue */
        /* Continue of data write */
        case USB_WRITING :
            if (USB_SETUPWR == g_usb_hstd_ctsq)
            {
                /* Next stage is Control read data stage */
                /* Next stage is Control write data stage */
                g_usb_hstd_ctsq = USB_DATAWR;
            }
            else
            {
                /* Next stage is Control read data stage */
                g_usb_hstd_ctsq = USB_DATAWRCNT;
            }
            /* Enable Empty Interrupt */
            hw_usb_set_bempenb((uint16_t) USB_PIPE0);
            /* Enable Not Ready Interrupt */
            usb_cstd_nrdy_enable((uint16_t) USB_PIPE0);
            /* Set BUF */
            usb_cstd_set_buf((uint16_t) USB_PIPE0);
        break;
        /* FIFO access error */
        case USB_FIFOERROR :
        /* continue */
        break;
        default :
            /* no processing */
        break;
    }

    /* End or Err or Continue */
    return (end_flag);
}
/******************************************************************************
 End of function usb_hstd_control_write_start
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_control_read_start
 Description     : Start data stage of Control Read transfer.
 Arguments       : uint32_t Bsize    : Data Size
                 : uint8_t  *Table   : Data Table Address
 Return          : none
 ******************************************************************************/
void usb_hstd_control_read_start (uint32_t Bsize, uint8_t *Table)
{
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
    g_usb_hstd_response_counter = 0;

    hw_usb_clear_sts_sofr();
    hw_usb_set_intenb(USB_SOFE);
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
    /* PID=NAK & clear STALL */
    usb_cstd_clr_stall((uint16_t) USB_PIPE0);
    /* Transfer size set */
    g_usb_hstd_data_cnt[USB_PIPE0] = Bsize;
    /* Transfer address set */
    gp_usb_hstd_data_ptr[USB_PIPE0] = Table;
    hw_usb_write_dcpcfg(USB_SHTNAKFIELD);
    hw_usb_hwrite_dcpctr(USB_SQSET); /* SQSET=1, PID=NAK */

    /* Next stage is Control read data stage */
    g_usb_hstd_ctsq = USB_DATARD;

    /* Ignore count clear */
    g_usb_hstd_ignore_cnt[USB_PIPE0] = (uint16_t) 0;
    /* Interrupt enable */
    hw_usb_set_brdyenb((uint16_t) USB_PIPE0); /* Enable Ready Interrupt */
    usb_cstd_nrdy_enable((uint16_t) USB_PIPE0); /* Enable Not Ready Interrupt */
    usb_cstd_set_buf((uint16_t) USB_PIPE0); /* Set BUF */
}
/******************************************************************************
 End of function usb_hstd_control_read_start
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_status_start
 Description     : Start status stage of Control Command.
 Arguments       : none
 Return          : none
 ******************************************************************************/
void usb_hstd_status_start (void)
{
    uint16_t end_flag;
    uint8_t buf1[16];

    /* Interrupt Disable */
    /* BEMP0 Disable */
    hw_usb_clear_bempenb((uint16_t) USB_PIPE0);
    /* BRDY0 Disable */
    hw_usb_clear_brdyenb((uint16_t) USB_PIPE0);
    /* Transfer size set */
    gp_usb_hstd_pipe[USB_PIPE0]->tranlen = g_usb_hstd_data_cnt[USB_PIPE0];
    /* Save Data stage transfer size */
    gs_usb_hstd_data_cnt_pipe0 = g_usb_hstd_data_cnt[USB_PIPE0];

    /* Branch  by the Control transfer stage management */
    switch (g_usb_hstd_ctsq)
    {
        case USB_DATARD :                   /* Control Read Data */
        case USB_DATARDCNT :                /* Control Read Data continue */
            /* Control read Status */
            g_usb_hstd_ctsq = USB_DATARD;
            /* Control write start */
            end_flag = usb_hstd_control_write_start((uint32_t) 0, (uint8_t*) &buf1);
            if (USB_FIFOERROR == end_flag)
            {
                /* Control Read/Write End */
                usb_hstd_control_end((uint16_t) USB_DATA_ERR); /* Control Read/Write End */
            }
            else
            {
                /* Next stage is Control read status stage */
                g_usb_hstd_ctsq = USB_STATUSRD;
            }
        break;
            /* Control Write Data */
        case USB_STATUSWR :                  /* Control Write Data */
            /* continue */
        case USB_SETUPNDC :                  /* NoData Control */
            /* Next stage is Control write status stage */
            usb_hstd_control_read_start((uint32_t) 0, (uint8_t*) &buf1);
            g_usb_hstd_ctsq = USB_STATUSWR;
        break;
        default :
            return;
        break;
    }
}
/******************************************************************************
 End of function usb_hstd_status_start
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_control_end
 Description     : Call the user registered callback function that notifies 
                 : completion of a control transfer.
 Arguments       : uint16_t  status : Transfer status
 Return          : none
 ******************************************************************************/
void usb_hstd_control_end (uint16_t status)
{
    /* Interrupt Disable */
    hw_usb_clear_bempenb((uint16_t) USB_PIPE0); /* BEMP0 Disable */
    hw_usb_clear_brdyenb((uint16_t) USB_PIPE0); /* BRDY0 Disable */
    hw_usb_clear_nrdyenb((uint16_t) USB_PIPE0); /* NRDY0 Disable */

    usb_cstd_clr_stall((uint16_t) USB_PIPE0); /* PID=NAK & clear STALL */
    hw_usb_set_mbw(USB_CUSE, USB_MBW_16);

    /* SUREQ=1, SQCLR=1, PID=NAK */
    hw_usb_hwrite_dcpctr((uint16_t) (USB_SUREQCLR | USB_SQCLR));

    /* CFIFO buffer clear */
    usb_cstd_chg_curpipe((uint16_t) USB_PIPE0, (uint16_t) USB_CUSE, USB_FALSE);
    hw_usb_set_bclr(USB_CUSE); /* Clear BVAL */
    usb_cstd_chg_curpipe((uint16_t) USB_PIPE0, (uint16_t) USB_CUSE, (uint16_t) USB_ISEL);
    hw_usb_set_bclr(USB_CUSE); /* Clear BVAL */

    /* Host Control sequence */
    if ((USB_CTRL_READING != status) && (USB_CTRL_WRITING != status))
    {
        /* Next stage is idle */
        g_usb_hstd_ctsq = USB_IDLEST;
    }

    gp_usb_hstd_pipe[USB_PIPE0]->status = status;

    /* Callback */
    if ( USB_NULL != gp_usb_hstd_pipe[USB_PIPE0])
    {
        if ( USB_NULL != (gp_usb_hstd_pipe[USB_PIPE0]->complete))
        {
            /* Process Done Callback */
            (gp_usb_hstd_pipe[USB_PIPE0]->complete)(gp_usb_hstd_pipe[USB_PIPE0], USB_NULL, USB_NULL);
        }
    }
    rtos_release_fixed_memory(&g_rtos_usb_mpf_id, (void *)gp_usb_hstd_pipe[USB_PIPE0]);
    gp_usb_hstd_pipe[USB_PIPE0] = (usb_utr_t*) USB_NULL;

    usb_rtos_resend_msg_to_submbx (USB_PIPE0, USB_HOST);

#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
    hw_usb_clear_enb_sofe();
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */

}
/******************************************************************************
 End of function usb_hstd_control_end
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_setup_start
 Description     : Start control transfer setup stage. (Set global function re-
                 : quired to start control transfer, and USB register).
 Arguments       : none
 Return          : none
 ******************************************************************************/
void usb_hstd_setup_start (void)
{
    uint16_t dir;
    uint16_t setup_req;
    uint16_t setup_val;
    uint16_t setup_indx;
    uint16_t setup_leng;
    uint16_t *p_setup;

    p_setup = gp_usb_hstd_pipe[USB_PIPE0]->p_setup;

    setup_req = *p_setup++; /* Set Request data */
    setup_val = *p_setup++; /* Set wValue data */
    setup_indx = *p_setup++; /* Set wIndex data */
    setup_leng = *p_setup++; /* Set wLength data */

    /* Max Packet Size + Device Number select */
    hw_usb_write_dcpmxps(g_usb_hstd_dcp_register);

    /* Transfer Length check */
    /* Check Last flag */
    if (gp_usb_hstd_pipe[USB_PIPE0]->tranlen < setup_leng)
    {
        setup_leng = (uint16_t) gp_usb_hstd_pipe[USB_PIPE0]->tranlen;
    }
    if (setup_leng < gp_usb_hstd_pipe[USB_PIPE0]->tranlen)
    {
        gp_usb_hstd_pipe[USB_PIPE0]->tranlen = (uint32_t) setup_leng;
    }

    /* Control sequence setting */
    dir = (uint16_t) (setup_req & USB_BMREQUESTTYPEDIR);

    /* Check wLength field */
    if (0 == setup_leng)
    {
        /* Check direction field */
        if (0 == dir)
        {
            /* Next stage is NoData control status stage */
            g_usb_hstd_ctsq = USB_SETUPNDC;
        }
        else
        {
            /* Error */
            g_usb_hstd_ctsq = USB_IDLEST;
        }
    }
    else
    {
        /* Check Dir field */
        if (0 == dir)
        {
            /* Next stage is Control Write data stage */
            g_usb_hstd_ctsq = USB_SETUPWR;
        }
        else
        {
            /* Next stage is Control read data stage */
            g_usb_hstd_ctsq = USB_SETUPRD;
        }
    }

    /* Control transfer idle stage? */
    if (USB_IDLEST == g_usb_hstd_ctsq)
    {
        /* Control Read/Write End */
        usb_hstd_control_end((uint16_t) USB_DATA_STOP);
    }
    else
    {
        /* SETUP request set */
        /* Set Request data */
        hw_usb_hwrite_usbreq(setup_req);
        hw_usb_hset_usbval(setup_val);
        hw_usb_hset_usbindx(setup_indx);
        hw_usb_hset_usbleng(setup_leng);

        /* Ignore count clear */
        g_usb_hstd_ignore_cnt[USB_PIPE0] = (uint16_t) 0;

        hw_usb_hclear_sts_sign();
        hw_usb_hclear_sts_sack();
        hw_usb_hset_enb_signe();
        hw_usb_hset_enb_sacke();
        hw_usb_hset_sureq();
    }
}
/******************************************************************************
 End of function usb_hstd_setup_start
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_brdy_pipe
 Description     : BRDY Interrupt
 Arguments       : uint16_t bitsts  : BRDYSTS Reg & BRDYENB Reg
 Return value    : none
 ******************************************************************************/
void usb_hstd_brdy_pipe (uint16_t bitsts)
{
    /* When operating by the host function, usb_hstd_brdy_pipe() is executed without fail because */
    /* only one BRDY message is issued even when the demand of PIPE0 and PIPEx has been generated at the same time. */
    if (USB_BRDY0 == (bitsts & USB_BRDY0))
    {
        /* Branch  by the Control transfer stage management */
        if (USB_DATARD == g_usb_hstd_ctsq)
        {
            switch (usb_hstd_cfifo_to_buf(USB_PIPE0, USB_CUSE))
            {
                /* End of data read */
                case USB_READEND :

                    /* continue */
                    /* End of data read */
                case USB_READSHRT :
                    usb_hstd_status_start();
                break;
                case USB_READING : /* Continue of data read */
                break;
                case USB_READOVER : /* FIFO access error */
                    usb_hstd_control_end((uint16_t) USB_DATA_OVR); /* Control Read/Write End */
                break;

                case USB_FIFOERROR : /* FIFO access error */
                    usb_hstd_control_end((uint16_t) USB_DATA_ERR); /* Control Read/Write End */
                break;
                default :
                break;
            }
        }
        else if (USB_DATARDCNT == g_usb_hstd_ctsq)
        {
            /* Data stage of Control read transfer */
            switch (usb_hstd_cfifo_to_buf(USB_PIPE0, USB_CUSE))
            {

                case USB_READEND : /* End of data read */
                    usb_hstd_control_end((uint16_t) USB_CTRL_READING); /* Control Read/Write End */
                break;
                case USB_READSHRT : /* End of data read */
                    usb_hstd_status_start(); /* Control Read/Write Status */
                break;
                case USB_READING : /* Continue of data read */
                break;
                case USB_READOVER : /* FIFO access error */
                    usb_hstd_control_end((uint16_t) USB_DATA_OVR); /* Control Read/Write End */
                break;
                case USB_FIFOERROR : /* FIFO access error */
                    usb_hstd_control_end((uint16_t) USB_DATA_ERR); /* Control Read/Write End */
                break;
                default :
                break;
            }
        }
        else if (USB_STATUSWR == g_usb_hstd_ctsq)
        {
            /* Status stage of Control write (NoData control) transfer */
            usb_hstd_control_end((uint16_t) USB_CTRL_END); /* Control Read/Write End */
        }
        else
        {
            /* no processing */
        }
    }

    /* BRDY interrupt */
    usb_hstd_brdy_pipe_process(bitsts);
}
/******************************************************************************
 End of function usb_hstd_brdy_pipe
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_nrdy_pipe
 Description     : NRDY Interrupt
 Arguments       : uint16_t bitsts  : NRDYSTS Reg & NRDYENB Reg
 Return value    : none
 ******************************************************************************/
void usb_hstd_nrdy_pipe (uint16_t bitsts)
{
    uint16_t buffer;

    /* When operating by the host function, usb_hstd_nrdy_pipe() is executed without fail because */
    /* only one NRDY message is issued even when the demand of PIPE0 and PIPEx has been generated at the same time. */
    if (USB_NRDY0 == (bitsts & USB_NRDY0))
    {
        /* Get Pipe PID from pipe number */
        buffer = usb_cstd_get_pid((uint16_t) USB_PIPE0);

        /* STALL ? */
        if (USB_PID_STALL == (buffer & USB_PID_STALL))
        {
            /* PIPE0 STALL call back */
            usb_hstd_control_end((uint16_t) USB_DATA_STALL);
        }
        else
        {
            /* Ignore count */
            g_usb_hstd_ignore_cnt[USB_PIPE0]++;

            /* Pipe error check */
            if (USB_PIPEERROR == g_usb_hstd_ignore_cnt[USB_PIPE0])
            {
                /* Control Data Stage Device Ignore X 3 call back */
                usb_hstd_control_end((uint16_t) USB_DATA_ERR);
            }
            else
            {
                /* Control Data Stage Retry */
                /* 5ms wait */
                usb_cpu_delay_xms((uint16_t) 5);

                /* PIPE0 Send IN or OUT token */
                usb_cstd_set_buf((uint16_t) USB_PIPE0);
            }
        }
    }

    /* Nrdy Pipe interrupt */
    usb_hstd_nrdy_pipe_process(bitsts);
}
/******************************************************************************
 End of function usb_hstd_nrdy_pipe
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_bemp_pipe
 Description     : BEMP interrupt
 Arguments       : uint16_t bitsts  : BEMPSTS Reg & BEMPENB Reg
 Return value    : none
 ******************************************************************************/
void usb_hstd_bemp_pipe (uint16_t bitsts)
{
    uint16_t buffer;

    /* When operating by the host function, usb_hstd_bemp_pipe_process() is executed without fail because */
    /* only one BEMP message is issued even when the demand of PIPE0 and PIPEx has been generated at the same time. */
    if (USB_BEMP0 == (bitsts & USB_BEMP0))
    {
        /* Get Pipe PID from pipe number */
        buffer = usb_cstd_get_pid((uint16_t) USB_PIPE0);
        /* MAX packet size error ? */
        if (USB_PID_STALL == (buffer & USB_PID_STALL))
        {
            usb_hstd_control_end((uint16_t) USB_DATA_STALL); /* PIPE0 STALL call back */
        }
        else
        {
            /* Branch by the Control transfer stage management */
            if (USB_DATAWR == g_usb_hstd_ctsq)
            {
                buffer = usb_cstd_buf_to_cfifo(USB_PIPE0, USB_CUSE);
                if (USB_WRITESHRT == buffer) /* Buffer to CFIFO data write */
                {
                    /* End of data write */
                    g_usb_hstd_ctsq = USB_STATUSWR; /* Next stage is Control write status stage */
                    hw_usb_set_bempenb((uint16_t) USB_PIPE0); /* Enable Empty Interrupt */
                    usb_cstd_nrdy_enable((uint16_t) USB_PIPE0); /* Enable Not Ready Interrupt */
                }
                else if ((USB_WRITING == buffer) && (USB_WRITEEND == buffer))  /* Buffer to CFIFO data write */
                {
                    /* Continue of data write */
                    hw_usb_set_bempenb((uint16_t) USB_PIPE0); /* Enable Empty Interrupt */
                    usb_cstd_nrdy_enable((uint16_t) USB_PIPE0); /* Enable Not Ready Interrupt */
                }
                else if (USB_FIFOERROR == buffer) /* Buffer to CFIFO data write */
                {
                    /* FIFO access error */
                    usb_hstd_control_end((uint16_t) USB_DATA_ERR); /* Control Read/Write End */
                }
                else
                {
                    /* no processing */
                }
            }
            else if (USB_DATAWRCNT == g_usb_hstd_ctsq)
            {
                /* Next stage to Control write data */
                /* Buffer to CFIFO data write */
                buffer = usb_cstd_buf_to_cfifo(USB_PIPE0, USB_CUSE);
                if (USB_WRITESHRT == buffer) /* Buffer to CFIFO data write */
                {
                    /* End of data write */
                    g_usb_hstd_ctsq = USB_STATUSWR; /* Next stage is Control write status stage */
                    hw_usb_set_bempenb((uint16_t) USB_PIPE0); /* Enable Empty Interrupt */
                    usb_cstd_nrdy_enable((uint16_t) USB_PIPE0); /* Enable Not Ready Interrupt */
                }
                else if (USB_WRITEEND == buffer)  /* Buffer to CFIFO data write */
                {
                    /* End of data write (not null) */
                    usb_hstd_control_end((uint16_t) USB_CTRL_WRITING); /* Control Read/Write End */
                }
                else if (USB_WRITING == buffer)   /* Buffer to CFIFO data write */
                {
                    /* Continue of data write */
                    hw_usb_set_bempenb((uint16_t) USB_PIPE0); /* Enable Empty Interrupt */
                    usb_cstd_nrdy_enable((uint16_t) USB_PIPE0); /* Enable Not Ready Interrupt */
                }
                else if (USB_FIFOERROR == buffer) /* Buffer to CFIFO data write */
                {
                    usb_hstd_control_end((uint16_t) USB_DATA_ERR); /* Control Read/Write End */
                }
                else
                {
                    /* no processing */
                }
            }
            else if (USB_STATUSWR == g_usb_hstd_ctsq)
            {
                /* End of data stage (Control write) */
                usb_hstd_status_start();
            }
            else if (USB_STATUSRD == g_usb_hstd_ctsq)
            {
                /* Status stage of Control read transfer */
                /* Control Read/Write End */
                usb_hstd_control_end((uint16_t) USB_CTRL_END);
            }
            else
            {
                /* no processing */
            }
        }
    }
    usb_hstd_bemp_pipe_process(bitsts); /* BEMP interrupt */
}
/******************************************************************************
 End of function usb_hstd_bemp_pipe
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_devaddr_to_speed
 Description     : Get USB speed set in USB register based on the specified USB 
                 : Device address.
 Arguments       : uint16_t addr        : device address
 Return value    : uint16_t             : device speed
 Note            : Use also to a connection check is possible
 ******************************************************************************/
uint16_t usb_hstd_devaddr_to_speed (uint16_t addr)
{
    uint16_t buffer;

    /* Get device address configuration register from device address */
    buffer = hw_usb_hread_devadd(addr);
    if (USB_ERROR != buffer)
    {
        /* Return device speed */
        return (uint16_t) (buffer & USB_USBSPD);
    }
    return USB_DEVADD_NOCONNECT;
}
/******************************************************************************
 End of function usb_hstd_devaddr_to_speed
 ******************************************************************************/
 
/******************************************************************************
 Function Name   : usb_hstd_check_attach
 Description     : Checks whether USB Device is attached or not and return USB speed
                 : of USB Device
 Arguments       : none
 Return value    : uint16_t          : connection status
                 :                   : (USB_ATTACHF/USB_ATTACHL/USB_DETACH/USB_OK)
 Note            : Please change for your SYSTEM
 ******************************************************************************/
uint16_t usb_hstd_check_attach (void)
{
    uint16_t buf[3];
    uint16_t ret = USB_OK;

    usb_hstd_read_lnst(buf);

    if (USB_FS_JSTS == (buf[0] & USB_LNST))
    {
        /* High/Full speed device */
        ret = USB_ATTACHF;
    }
    else if (USB_LS_JSTS == (buf[0] & USB_LNST))
    {
        /* Low speed device */
        ret = USB_ATTACHL;
    }
    else
    {
        ret = USB_DETACH;
    }
    return ret;
}
/******************************************************************************
 End of function usb_hstd_check_attach
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_set_feature
 Description     : Set SetFeature
 Arguments       : uint16_t     addr        : device address
                 : uint16_t     epnum       : endpoint number
 Return value    : uint16_t                 : error info
 ******************************************************************************/
uint16_t usb_hstd_set_feature (uint16_t addr, uint16_t epnum)
{
    if (0xFF == epnum)
    {
        /* SetFeature(Device) */
        g_usb_hstd_setup_data[0] = USB_SET_FEATURE | USB_HOST_TO_DEV | USB_REQUEST_TYPE_STANDARD | USB_DEVICE;
        g_usb_hstd_setup_data[1] = USB_DEV_REMOTE_WAKEUP;
        g_usb_hstd_setup_data[2] = (uint16_t) 0x0000;
    }
    else
    {
        /* SetFeature(endpoint) */
        g_usb_hstd_setup_data[0] = USB_SET_FEATURE | USB_HOST_TO_DEV | USB_REQUEST_TYPE_STANDARD | USB_ENDPOINT;
        g_usb_hstd_setup_data[1] = USB_ENDPOINT_HALT;
        g_usb_hstd_setup_data[2] = epnum;
    }
    g_usb_hstd_setup_data[3] = (uint16_t) 0x0000;
    g_usb_hstd_setup_data[4] = addr;

    return usb_hstd_do_command();
}
/******************************************************************************
 End of function usb_hstd_set_feature
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_clear_feature
 Description     : Send ClearFeature command to the connected USB device.
 Arguments       : uint16_t epnum    : Endpoint number.
                 : usb_cb_t complete : Callback function.
 Return value    : uint16_t          : Error info.
 ******************************************************************************/
usb_er_t usb_hstd_clear_feature (uint16_t epnum, usb_cb_t complete)
{
    usb_er_t ret_code;
    usb_utr_t   tran_data;

    if (0xFF == epnum)
    {
        /* ClearFeature(Device) */
        gs_usb_hstd_clr_stall_setup[0] = USB_CLEAR_FEATURE | USB_HOST_TO_DEV | USB_REQUEST_TYPE_STANDARD | USB_DEVICE;
        gs_usb_hstd_clr_stall_setup[1] = USB_DEV_REMOTE_WAKEUP;
        gs_usb_hstd_clr_stall_setup[2] = (uint16_t) 0x0000;
    }
    else
    {
        /* ClearFeature(endpoint) */
        gs_usb_hstd_clr_stall_setup[0] = USB_CLEAR_FEATURE | USB_HOST_TO_DEV | USB_REQUEST_TYPE_STANDARD | USB_ENDPOINT;
        gs_usb_hstd_clr_stall_setup[1] = USB_ENDPOINT_HALT;
        gs_usb_hstd_clr_stall_setup[2] = epnum;
    }
    gs_usb_hstd_clr_stall_setup[3] = (uint16_t) 0x0000;
    gs_usb_hstd_clr_stall_setup[4] = USB_DEVICEADDR;

    tran_data.p_tranadr = USB_NULL;
    tran_data.complete = complete;
    tran_data.tranlen = 0;
    tran_data.keyword = USB_PIPE0;
    tran_data.p_setup = gs_usb_hstd_clr_stall_setup;

    ret_code = usb_hstd_transfer_start_req(&tran_data);
    if (USB_OK == ret_code)
    {
        ret_code = class_trans_wait_tmo((uint16_t)3000);
    }
    return ret_code;
}
/******************************************************************************
 End of function usb_hstd_clear_feature
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_do_command
 Description     : command submit
 Arguments       : none
 Return value    : uint16_t                 : USB_OK
 ******************************************************************************/
uint16_t usb_hstd_do_command (void)
{
    usb_er_t retval;
    usb_utr_t   tran_data;

    tran_data.p_tranadr = (void*) g_usb_hstd_class_data;
    tran_data.complete = &class_trans_result;
    tran_data.tranlen = (uint32_t) g_usb_hstd_setup_data[3];
    tran_data.keyword = USB_PIPE0;
    tran_data.p_setup = g_usb_hstd_setup_data;

    retval = usb_hstd_transfer_start_req(&tran_data);
    if (USB_OK == retval)
    {
        retval = class_trans_wait_tmo((uint16_t)3000);
    }

    return USB_OK;
}
/******************************************************************************
 End of function usb_hstd_do_command
 ******************************************************************************/

 /******************************************************************************
 Function Name   : usb_hstd_pipe_to_epadr
 Description     : Get the associated endpoint value of the specified pipe.
 Arguments       : uint16_t pipe  : Pipe number.
 Return value    : uint8_t        : OK    : Endpoint nr + direction.
                 :                : ERROR : Error.
 ******************************************************************************/
uint8_t usb_hstd_pipe_to_epadr (uint16_t pipe)
{
    uint16_t buffer;
    uint16_t direp;

    if (USB_MAX_PIPE_NO < pipe)
    {
        return USB_NULL; /* Error */
    }

    /* Pipe select */
    hw_usb_write_pipesel(pipe);

    /* Read Pipe direction */
    buffer = hw_usb_read_pipecfg();
    direp = (uint16_t) ((((buffer & USB_DIRFIELD) ^ USB_DIRFIELD) << 3) + (buffer & USB_EPNUMFIELD));
    return (uint8_t) (direp);
}
/******************************************************************************
 End of function usb_hstd_pipe_to_epadr
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_pipe_to_devsel
 Description     : Get device address from pipe number
 Arguments       : uint16_t pipe  : Pipe number.
 Return value    : uint16_t DEVSEL-bit status
 ******************************************************************************/
uint16_t usb_hstd_pipe_to_devsel (uint16_t pipe)
{
    uint16_t devsel;
    uint16_t buffer;

    if (USB_MAX_PIPE_NO < pipe)
    {
        return USB_NULL; /* Error */
    }

    if (USB_PIPE0 == pipe)
    {
        buffer = hw_usb_read_dcpmaxp();
    }
    else
    {
        /* Pipe select */
        hw_usb_write_pipesel(pipe);
        buffer = hw_usb_read_pipemaxp();
    }

    /* Device address */
    devsel = (uint16_t) (buffer & USB_DEVSEL);

    return devsel;
}
/******************************************************************************
 End of function usb_hstd_pipe_to_devsel
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_status_notification
 Description     : Notify MGR (manager) task that attach or detach occurred.
 Arguments       : uint16_t result   : Result.
 Return          : none
 ******************************************************************************/
void usb_hstd_status_notification (uint16_t result)
{
    usb_hstd_mgr_snd_mbx((uint16_t) USB_MSG_MGR_AORDETACH, USB_NULL, result);
}
/******************************************************************************
 End of function usb_hstd_status_notification
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_status_result
 Description     : This is a callback as a result of calling 
                 : usb_hstd_change_device_state. This notifies the MGR (manager) 
                 : task that the change of USB Device status completed.
 Arguments       : usb_utr_t    *ptr    : Pointer to usb_utr_t structure.
                 : uint16_t     data1   : No use.
                 : uint16_t     result  : Result.
 Return          : none
 ******************************************************************************/
void usb_hstd_status_result (usb_utr_t *ptr, uint16_t data1, uint16_t result)
{
    usb_hstd_mgr_snd_mbx((uint16_t) USB_MSG_MGR_STATUSRESULT, USB_NULL, result);
}
/******************************************************************************
 End of function usb_hstd_status_result
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hclass_request_complete
 Description     : Class request transfer complete
 Arguments       : usb_utr_t *p_mess  : Pointer to usb_utr_t structure.
                 : uint16_t  data1  : Not used.
                 : uint16_t  data2  : Not used.
 Return          : none
 ******************************************************************************/
void usb_hclass_request_complete (usb_utr_t *p_mess, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;

    if (USB_CTRL_END == p_mess->status) /* Check usb receive status */
    {
        ctrl.status = USB_ACK;
    }
    else if (USB_DATA_STALL == p_mess->status) /* Check usb receive status */
    {
        ctrl.status = USB_STALL;
    }
    else
    {
        ctrl.status = USB_ERROR;
    }
    ctrl.type = USB_REQUEST;
    ctrl.setup.type = p_mess->p_setup[0];
    ctrl.pipe = USB_PIPE0;
    ctrl.setup.value = p_mess->p_setup[1];
    ctrl.setup.index = p_mess->p_setup[2];
    ctrl.setup.length = p_mess->p_setup[3];
    ctrl.size = ctrl.setup.length - gs_usb_hstd_data_cnt_pipe0;
    ctrl.p_data = (void *)p_mess->task_id;

    usb_cstd_set_event(USB_STS_REQUEST_COMPLETE, &ctrl); /* Set Event()  */
}
/******************************************************************************
 End of function usb_hclass_request_complete
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_suspend_complete
 Description     : usb_hstd_change_device_state callback (Suspend)
 Arguments       : usb_utr_t *ptr   : Pointer to usb_utr_t structure.
                 : uint16_t data1   : Not used.
                 : uint16_t data2   : Not used.
 Return value    : none
 ******************************************************************************/
void usb_hstd_suspend_complete (usb_utr_t *ptr, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;

    ctrl.p_data = (void *)ptr->task_id;

    usb_cstd_set_event(USB_STS_SUSPEND, &ctrl);
}
/******************************************************************************
 End of function usb_hstd_suspend_complete
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_resume_complete
 Description     : usb_hstd_change_device_state callback (Resume)
 Arguments       : usb_utr_t *ptr   : Pointer to usb_utr_t structure.
                 : uint16_t data1   : Not used.
                 : uint16_t data2   : Not used.
 Return value    : none
 ******************************************************************************/
void usb_hstd_resume_complete (usb_utr_t *ptr, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;

    ctrl.p_data = (void *)ptr->task_id;

    usb_cstd_set_event(USB_STS_RESUME, &ctrl);
}/* End of function usb_hstd_resume_complete */

/******************************************************************************
 Function Name   : usb_host_registration
 Description     : sample registration.
 Argument        : none
 Return          : none
 ******************************************************************************/
void usb_host_registration (void)
{

#if defined(USB_CFG_HCDC_USE)
    usb_hcdc_registration();

#endif /* defined(USB_CFG_PCDC_USE) */

#if defined(USB_CFG_HHID_USE)
    usb_hhid_registration();

#endif /* defined(USB_CFG_HMSC_USE) */

#if defined(USB_CFG_HMSC_USE)
    usb_hmsc_registration();

#endif /* defined(USB_CFG_HMSC_USE) */

#if defined(USB_CFG_HVND_USE)
    usb_hvnd_registration();

#endif /* defined(USB_CFG_HVND_USE) */
}
/******************************************************************************
 End of function usb_host_registration
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_get_string_desc
 Description     : Set GetDescriptor
 Arguments       : uint16_t     string      : descriptor index
 Return value    : uint16_t                 : error info
 ******************************************************************************/
uint16_t usb_hstd_get_string_desc (uint16_t string)
{
    uint16_t i;

    if (0 == string)
    {
        g_usb_hstd_setup_data[2] = (uint16_t) 0x0000;
        g_usb_hstd_setup_data[3] = (uint16_t) 0x0004;
    }
    else
    {
        /* Set LanguageID */
        g_usb_hstd_setup_data[2] = (uint16_t) (g_usb_hstd_class_data[2]);
        g_usb_hstd_setup_data[2] |= (uint16_t) ((uint16_t) (g_usb_hstd_class_data[3]) << 8);
        g_usb_hstd_setup_data[3] = (uint16_t) CLSDATASIZE;
    }
    g_usb_hstd_setup_data[0] = USB_GET_DESCRIPTOR | USB_DEV_TO_HOST | USB_REQUEST_TYPE_STANDARD | USB_DEVICE;
    g_usb_hstd_setup_data[1] = (uint16_t) (USB_STRING_DESCRIPTOR + string);
    g_usb_hstd_setup_data[4] = USB_DEVICEADDR;

    /* WAIT_LOOP */
    for (i = 0; i < g_usb_hstd_setup_data[3]; i++)
    {
        g_usb_hstd_class_data[i] = (uint8_t) 0xFF;
    }
    return usb_hstd_do_command();
}
/******************************************************************************
 End of function usb_hstd_get_string_desc
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_dummy_function
 Description     : Dummy function
 Arguments       : usb_utr_t *ptr   : Pointer to usb_utr_t structure.
                 : uint16_t data1    : Not used.
                 : uint16_t data2    : Not used.
 Return value    : none
 ******************************************************************************/
void usb_hstd_dummy_function (usb_utr_t *ptr, uint16_t data1, uint16_t data2)
{
    /* None */
}
/******************************************************************************
 End of function usb_hstd_dummy_function
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_ls_connect_function
 Description     : Low-speed device connect.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_hstd_ls_connect_function (void)
{
    usb_hstd_enum_get_descriptor((uint16_t) USB_DEVICE_0, (uint16_t) 0);
}
/******************************************************************************
 End of function usb_hstd_ls_connect_function
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_ovrcr0function
 Description     : Set USB registers as required due to an OVRCR (over-current)
                 : interrupt, and notify the MGR (manager) task about this.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_hstd_ovrcr0function (void)
{
    /* OVRCR interrupt disable */
    usb_hstd_ovrcr_disable();
    /* Notification over current */
    usb_hstd_mgr_snd_mbx((uint16_t) USB_MSG_MGR_OVERCURRENT, USB_NULL, (uint16_t) 0u);
}
/******************************************************************************
 End of function usb_hstd_ovrcr0function
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_driver_init
 Description     : USB Host driver initialization
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_hstd_driver_init (void)
{
    uint16_t i;

    if (USB_FALSE == g_usb_cstd_driver_open)
    {
        /* WAIT_LOOP */
        for (i = 0; i < USB_EVENT_MAX; i++)
        {
            g_usb_cstd_event[i].event = USB_STS_NONE;
        }

        g_usb_cstd_driver_open = USB_TRUE;
    }

    usb_hstd_mgr_open(); /* Manager open */
    usb_hstd_hcd_open(); /* Hcd open */
#if defined(USB_CFG_HCDC_USE) || defined(USB_CFG_HHID_USE) || defined(USB_CFG_HMSC_USE) || defined(USB_CFG_HVND_USE)
    usb_class_driver_start(); /* Init host class driver task. */
    usb_host_registration(); /* Class Registration */

#endif  /* defined(USB_CFG_HCDC_USE)||defined(USB_CFG_HHID_USE)||defined(USB_CFG_HMSC_USE)||defined(USB_CFG_HVND_USE) */
} /* End of function usb_hstd_driver_init() */

/******************************************************************************
 Function Name   : usb_class_driver_start
 Description     : Init host class driver task.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_class_driver_start (void)
{
#if defined(USB_CFG_HCDC_USE)
    usb_hcdc_driver_start();

#endif /* defined(USB_CFG_HCDC_USE) */
#if defined(USB_CFG_HMSC_USE)
    usb_hmsc_driver_start();
    usb_hmsc_storage_driver_start();

#endif /* defined(USB_CFG_HMSC_USE) */
#if defined(USB_CFG_HHID_USE)
    usb_hhid_driver_start();

#endif /* defined(USB_CFG_HHID_USE) */

} /* End of function usb_class_driver_start() */

/******************************************************************************
 Function Name   : class_trans_result
 Description     : Send a message to notify the result of the class request.
 Argument        : usb_utr_t *ptr   : USB system internal structure. Selects channel.
                   uint16_t data1
                   uint16_t data2
 Return value    : none
 ******************************************************************************/
void class_trans_result(usb_utr_t *ptr, uint16_t data1, uint16_t data2)
{
    /* Send an internal message */
    rtos_send_mailbox (&g_rtos_usb_cls_mbx_id, (void *)ptr);
} /* End of function class_trans_result() */

/******************************************************************************
 Function Name   : class_trans_wait_tmo
 Description     : Receive the result of the class request with a timeout.
 Argument        : uint16_t tmo     : Time-out value.
 Return value    : USB_OK/USB_ERROR
 ******************************************************************************/
uint16_t class_trans_wait_tmo(uint16_t tmo)
{
    usb_utr_t   *mess;
    rtos_err_t  err;
    uint16_t    ret = USB_ERROR;

    /* Receive message with time out */
    err = rtos_receive_mailbox (&g_rtos_usb_cls_mbx_id, (void **)&mess, (rtos_time_t)tmo);
    if (RTOS_SUCCESS == err)
    {
        /* Check the status of transfer */
        if (USB_CTRL_END == mess->status)
        {
            ret = USB_OK;
        }
    }

    return ret;

} /* End of function class_trans_wait_tmo() */

#endif /*(BSP_CFG_RTOS_USED != 0)*/
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
/******************************************************************************
 End  Of File
 ******************************************************************************/
