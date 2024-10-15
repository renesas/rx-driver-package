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
 * Copyright (C) 2020(2024) Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_usb_hreg_abs.c
 * Description  : Call USB Host register access function 
 ***********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 30.06.2020 1.20 First Release
 *         : 30.04.2024 1.30 Added support for RX261.
 ***********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_basic_mini_if.h"
#include "r_usb_typedef.h"
#include "r_usb_extern.h"
#include "r_usb_bitdefine.h"
#include "r_usb_reg_access.h"
#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
#include "r_usb_cstd_rtos.h"
#endif /* (BSP_CFG_RTOS_USED != 0) */


#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
#if (BSP_CFG_RTOS_USED != 0)    /* RTOS */
/******************************************************************************
 Function Name   : usb_hstd_check_interrupt_source
 Description     : Analyzes which USB interrupt is generated
 Arguments       : usb_utr_t *ptr    : Pointer to usb_utr_t structure.
 Return          : none
 ******************************************************************************/
void usb_hstd_check_interrupt_source (usb_utr_t *ptr)
{
    uint16_t intsts0;
    uint16_t intenb0;
    uint16_t ists0;

    uint16_t intsts1;
    uint16_t intenb1;
    uint16_t ists1;

    uint16_t brdysts;
    uint16_t brdyenb;
    uint16_t bsts;

    uint16_t nrdysts;
    uint16_t nrdyenb;
    uint16_t nsts;

    uint16_t bempsts;
    uint16_t bempenb;
    uint16_t ests;
    uint16_t stat;
    uint16_t key;

    intsts0 = USB0.INTSTS0.WORD;
    intsts1 = USB0.INTSTS1.WORD;
    brdysts = USB0.BRDYSTS.WORD;
    nrdysts = USB0.NRDYSTS.WORD;
    bempsts = USB0.BEMPSTS.WORD;
    intenb0 = USB0.INTENB0.WORD;
    intenb1 = USB0.INTENB1.WORD;
    brdyenb = USB0.BRDYENB.WORD;
    nrdyenb = USB0.NRDYENB.WORD;
    bempenb = USB0.BEMPENB.WORD;

    /* Interrupt Status Get */
    key  = USB_INT_UNKNOWN;
    stat = 0;

    ists0 = (uint16_t) (intsts0 & intenb0);
    ists1 = (uint16_t) (intsts1 & intenb1);

    /*  ists2 = (uint16_t)(intsts2 & intenb2);*/
    bsts = (uint16_t) (brdysts & brdyenb);
    nsts = (uint16_t) (nrdysts & nrdyenb);
    ests = (uint16_t) (bempsts & bempenb);

    /***** Processing Setup transaction *****/
    if (USB_SACK == (ists1 & USB_SACK))
    {
        /***** Setup ACK *****/
        /* SACK Clear */
        USB0.INTSTS1.WORD = (uint16_t) ((~USB_SACK) & USB_INTSTS1_MASK);

        /* Setup Ignore,Setup Acknowledge disable */
        USB0.INTENB1.WORD &= (uint16_t) ~(USB_SIGNE | USB_SACKE);
        key = USB_INT_SACK;
    }
    else if (USB_SIGN == (ists1 & USB_SIGN))
    {
        /***** Setup Ignore *****/
        /* SIGN Clear */
        USB0.INTSTS1.WORD = (uint16_t) ((~USB_SIGN) & USB_INTSTS1_MASK);

        /* Setup Ignore,Setup Acknowledge disable */
        USB0.INTENB1.WORD &= (uint16_t) ~((USB_SIGNE) | USB_SACKE);
        key = USB_INT_SIGN;
    }

    /***** Processing PIPE0-MAX_PIPE_NO data *****/
    else if (USB_BRDY == (ists0 & USB_BRDY)) /***** EP0-7 BRDY *****/
    {
        stat = bsts;
        USB0.BRDYSTS.WORD = (uint16_t) ((~stat) & USB_BRDYSTS_MASK);
        key = USB_INT_BRDY;
    }
    else if (USB_BEMP == (ists0 & USB_BEMP)) /***** EP0-7 BEMP *****/
    {
        stat = ests;
        USB0.BEMPSTS.WORD = (uint16_t) ((~stat) & USB_BEMPSTS_MASK);
        key = USB_INT_BEMP;
    }
    else if (USB_NRDY == (ists0 & USB_NRDY)) /***** EP0-7 NRDY *****/
    {
        stat = nsts;
        USB0.NRDYSTS.WORD = (uint16_t) ((~stat) & USB_NRDYSTS_MASK);
        key = USB_INT_NRDY;
    }

    /***** Processing rootport0 *****/
    else if (USB_OVRCR == (ists1 & USB_OVRCR)) /***** OVER CURRENT *****/
    {
        /* OVRCR Clear */
        USB0.INTSTS1.WORD = (uint16_t) ((~USB_OVRCR) & USB_INTSTS1_MASK);
        key = USB_INT_OVRCR0;
    }
    else if (USB_ATTCH == (ists1 & USB_ATTCH)) /***** ATTCH INT *****/
    {
        /* DTCH  interrupt disable */
        usb_hstd_bus_int_disable();
        key = USB_INT_ATTCH0;
    }
    else if (USB_EOFERR == (ists1 & USB_EOFERR)) /***** EOFERR INT *****/
    {
        /* EOFERR Clear */
        USB0.INTSTS1.WORD = (uint16_t) ((~USB_EOFERR) & USB_INTSTS1_MASK);
        key = USB_INT_EOFERR0;
    }
    else if (USB_BCHG == (ists1 & USB_BCHG)) /***** BCHG INT *****/
    {
        /* BCHG  interrupt disable */
        usb_hstd_bchg_disable();
        key = USB_INT_BCHG0;
    }
    else if (USB_DTCH == (ists1 & USB_DTCH)) /***** DETACH *****/
    {
        /* DTCH  interrupt disable */
        usb_hstd_bus_int_disable();
        key = USB_INT_DTCH0;
    }
#if USB_CFG_BC == USB_CFG_ENABLE
    else if (USB_PDDETINT == (ists1 & USB_PDDETINT)) /***** PDDETINT INT *****/
    {
        /* PDDETINT  interrupt disable */
        USB0.INTSTS1.WORD = (uint16_t) ((~USB_PDDETINT) & USB_INTSTS1_MASK);
        key = USB_INT_PDDETINT0;
    }
#endif  /* USB_CFG_BC == USB_CFG_ENABLE */
    /***** Processing VBUS/SOF *****/
    else if (USB_VBINT == (ists0 & USB_VBINT)) /***** VBUS change *****/
    {
        /* Status Clear */
        USB0.INTSTS0.WORD = (uint16_t) ~USB_VBINT;
        key = USB_INT_VBINT;
    }
    else if (USB_SOFR == (ists0 & USB_SOFR)) /***** SOFR change *****/
    {
        /* SOFR Clear */
        USB0.INTSTS0.WORD = (uint16_t) ~USB_SOFR;
        key = USB_INT_SOFR;
    }

    else
    {
        /* no processing */
    }
    ptr->status  = stat;
    ptr->keyword = key;
}
/******************************************************************************
 End of function usb_hstd_check_interrupt_source
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_interrupt_handler
 Description     : USB interrupt routine. Analyze which USB interrupt occurred 
                 : and send message to HCD task.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_hstd_interrupt_handler (void)
{
    usb_utr_t *ptr;

    rtos_err_t ret;

    /* Initial pointer */
    ret = rtos_get_fixed_memory_isr(&g_rtos_usb_mpf_id, (void **)&ptr);
    if (RTOS_ERROR == ret)
    {
        return; /* Error */
    }
    ptr->msginfo = USB_MSG_HCD_INT;

    /* Host Function */
    /* Host Interrupt handler */
    usb_hstd_check_interrupt_source(ptr);
    ptr->msghead = (usb_mh_t) USB_NULL;

    /* Send message */
    rtos_send_mailbox_isr (&g_rtos_usb_hcd_mbx_id, (void *)ptr);
}
/******************************************************************************
 End of function usb_hstd_interrupt_handler
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_chk_clk
 Description     : Checks SOF sending setting when USB Device is detached or suspended
                 : , BCHG interrupt enable setting and clock stop processing
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_hstd_chk_clk (void)
{

    if ((USB_DETACHED == g_usb_hstd_mgr_mode) || (USB_SUSPENDED == g_usb_hstd_mgr_mode))
    {
        usb_cpu_delay_1us((uint16_t) 1); /* Wait 640ns */

        /* Enable port BCHG interrupt */
        usb_hstd_bchg_enable();
    }

} /* End of function usb_hstd_chk_clk */
/******************************************************************************
 End of function usb_hstd_chk_clk
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_detach_process
 Description     : Handles the require processing when USB device is detached
                 : (Data transfer forcibly termination processing to the connected USB Device,
                 : the clock supply stop setting and the USB interrupt dissable setteing etc)
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_hstd_detach_process (void)
{
    uint8_t  connect_inf;
    uint16_t i;
    uint16_t addr;

    /* ATTCH interrupt disable */
    usb_hstd_attch_disable();

    /* DTCH  interrupt disable */
    usb_hstd_detach_disable();
    usb_hstd_bchg_disable();

    addr = (uint16_t) (1 << USB_DEVADDRBIT);
    if (USB_DEVADD_NOCONNECT != usb_hstd_devaddr_to_speed(addr))
    {
        if (USB_IDLEST != g_usb_hstd_ctsq)
        {
            /* Control Read/Write End */
            usb_hstd_control_end((uint16_t) USB_DATA_ERR);
        }
        /* WAIT_LOOP */
        for (i = USB_MIN_PIPE_NO; i <= USB_MAX_PIPE_NO; i++)
        {
            /* Not control transfer */
            /* Agreement device address */
            if (usb_hstd_pipe_to_devsel(i) == addr)
            {
                /* PID=BUF ? */
                if (USB_PID_BUF == usb_cstd_get_pid(i))
                {
                    /* End of data transfer (IN/OUT) */
                    usb_hstd_forced_termination(i, (uint16_t) USB_DATA_STOP);
                }
                usb_cstd_clr_pipe_config(i);
            }
        }
        usb_hstd_set_devaddx_register(addr, USB_NOCONNECT);
    }

    /* Decide USB Line state (ATTACH) */
    connect_inf = usb_hstd_check_attach();
    if ((USB_ATTACHF == connect_inf) || (USB_ATTACHL == connect_inf))
    {
        usb_hstd_attach_control(connect_inf);
    }
    else
    {
        usb_hstd_detach_control();
    }
}
/******************************************************************************
 End of function usb_hstd_detach_process
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_read_lnst
 Description     : Reads LNST register two times, checks whether these values
                 : are equal and returns the value of DVSTCTR register that correspond to
                 : the port specified by 2nd argument.
 Arguments       : uint16_t *buf     : Pointer to the buffer to store DVSTCTR register
 Return value    : none
 Note            : Please change for your SYSTEM
 ******************************************************************************/
void usb_hstd_read_lnst (uint16_t *p_buf)
{
    /* WAIT_LOOP */
    do
    {
        p_buf[0] = hw_usb_read_syssts();

        /* 30ms wait */
        usb_cpu_delay_xms((uint16_t) 30);
        p_buf[1] = hw_usb_read_syssts();
        if ((p_buf[0] & USB_LNST) == (p_buf[1] & USB_LNST))
        {
            /* 20ms wait */
            usb_cpu_delay_xms((uint16_t) 20);
            p_buf[1] = hw_usb_read_syssts();
        }
    }
    while ((p_buf[0] & USB_LNST) != (p_buf[1] & USB_LNST) );
    p_buf[1] = hw_usb_read_dvstctr();
}
/******************************************************************************
 End of function usb_hstd_read_lnst
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_attach_process
 Description     : Interrupt disable setting when USB Device is attached and
                 : handles the required interrupt disable setting etc when USB device
                 : is attached.
 Arguments       : none
 Return value    : none
 Note            : Please change for your SYSTEM
 ******************************************************************************/
void usb_hstd_attach_process (void)
{
    uint16_t connect_inf;

    /* ATTCH interrupt disable */
    usb_hstd_attch_disable();

    /* DTCH  interrupt disable */
    usb_hstd_detach_disable();
    usb_hstd_bchg_disable();

    /* Decide USB Line state (ATTACH) */
    connect_inf = usb_hstd_check_attach();
    if (USB_DETACH == connect_inf)
    {
        usb_hstd_detach_control();
    }
    else
    {
        usb_hstd_attach_control(connect_inf);
    }
}
/******************************************************************************
 End of function usb_hstd_attach_process
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_bus_reset
 Description     : Setting USB register when BUS Reset
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_hstd_bus_reset (void)
{
    uint16_t buf;
    uint16_t i;

    /* USBRST=1, UACT=0 */
    hw_usb_rmw_dvstctr(USB_USBRST, (USB_USBRST | USB_UACT));

    /* Wait 50ms */
    usb_cpu_delay_xms((uint16_t) 50);

    /* USBRST=0, RESUME=0, UACT=1 */
    usb_hstd_set_uact();

    /* Wait 10ms or more (USB reset recovery) */
    usb_cpu_delay_xms((uint16_t) 20);
    /* WAIT_LOOP */
    for (i = 0, buf = USB_HSPROC; (i < 3) && (USB_HSPROC == buf); ++i)
    {
        /* DeviceStateControlRegister - ResetHandshakeStatusCheck */
        buf = hw_usb_read_dvstctr();
        buf = (uint16_t) (buf & USB_RHST);
        if (USB_HSPROC == buf)
        {
            /* Wait */
            usb_cpu_delay_xms((uint16_t) 10);
        }
    }

    /* 30msec->100msec Support Tera Byte Size Storage Device */
    usb_cpu_delay_xms((uint16_t) 100);
}
/******************************************************************************
 End of function usb_hstd_bus_reset
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_resume_process
 Description     : Setting USB register when RESUME signal is detected
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_hstd_resume_process (void)
{
    uint16_t buf;

    usb_hstd_bchg_disable();

    buf = hw_usb_read_dvstctr();

    /* Reset handshake status get */

    if (buf & USB_RESUME)
    {
        /* RWUPE=0 */
        hw_usb_clear_dvstctr(USB_RWUPE);
    }
    else
    {
        /* RESUME=1, RWUPE=0 */
        hw_usb_rmw_dvstctr(USB_RESUME, (USB_RESUME | USB_RWUPE));
    }

    /* Wait */
    usb_cpu_delay_xms((uint16_t) 20);

    /* USBRST=0, RESUME=0, UACT=1 */
    usb_hstd_set_uact();

    /* Wait */
    usb_cpu_delay_xms((uint16_t) 5);
}
/******************************************************************************
 End of function usb_hstd_resume_process
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hstd_write_fifo
 Description     : Write specified amount of data to specified USB FIFO. 
 Arguments       : uint16_t     count      : Write size
                 : uint16_t     pipemode   : The mode of CPU/DMA(D0)/DMA(D1).
                 : uint16_t     *write_p   : Address of buffer of data to write.
 Return value    : The incremented address of last argument (write_p).
 ******************************************************************************/
uint8_t *usb_hstd_write_fifo (uint16_t count, uint16_t pipemode, uint8_t *write_p)
{
    uint16_t even;

    /* WAIT_LOOP */
    for (even = (uint16_t) (count >> 1); (0 != even); --even)
    {
        /* 16bit access */
        hw_usb_write_fifo16(pipemode, *((uint16_t *) write_p));

        /* Renewal write pointer */
        write_p += sizeof(uint16_t);
    }

    if ((count & (uint16_t) 0x0001u) != 0u)
    {
        /* 8bit access */
        /* count == odd */
        /* Change FIFO access width */
        hw_usb_set_mbw(pipemode, USB_MBW_8);

        /* FIFO write */
        hw_usb_write_fifo8(pipemode, *write_p);

        /* Return FIFO access width */
        hw_usb_set_mbw(pipemode, USB_MBW_16);

        /* Renewal write pointer */
        write_p++;
    }
    return write_p;

}
/******************************************************************************
 End of function usb_hstd_write_fifo
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_read_fifo
 Description     : Read specified buffer size from the USB FIFO.
 Arguments       : uint16_t     count       : Read size
                 : uint16_t     pipemode    : The mode of CPU/DMA(D0)/DMA(D1).
                 : uint16_t     *read_p     : Address of buffer to store the read data
 Return value    : Pointer to a buffer that contains the data to be read next.
 ******************************************************************************/
uint8_t *usb_hstd_read_fifo (uint16_t count, uint16_t pipemode, uint8_t *read_p)
{
    uint16_t even;
    uint32_t odd_byte_data_temp;

    /* WAIT_LOOP */
    for (even = (uint16_t) (count >> 1); (0 != even); --even)
    {
        /* 16bit FIFO access */
        *(uint16_t *) read_p = hw_usb_read_fifo16(pipemode);

        /* Renewal read pointer */
        read_p += sizeof(uint16_t);
    }
    if ((count & (uint16_t) 0x0001) != 0)
    {
        /* 16bit FIFO access */
        odd_byte_data_temp = hw_usb_read_fifo16(pipemode);

        /* Condition compilation by the difference of the little endian */
#if USB_CFG_ENDIAN == USB_CFG_LITTLE
        *read_p = (uint8_t) (odd_byte_data_temp & 0x00ff);
#else   /* USB_CFG_ENDIAN == USB_CFG_LITTLE */
        *read_p = (uint8_t) (odd_byte_data_temp >> 8);
#endif  /* USB_CFG_ENDIAN == USB_CFG_LITTLE */

        /* Renewal read pointer */
        read_p += sizeof(uint8_t);
    }
    return read_p;
}
/******************************************************************************
 End of function usb_hstd_read_fifo
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_forced_termination
 Description     : Terminate data transmission and reception.
 Arguments       : uint16_t     pipe    : Pipe Number
                 : uint16_t     status  : Transfer status type
 Return value    : none
 Note            : In the case of timeout status, it does not call back.
 ******************************************************************************/
void usb_hstd_forced_termination (uint16_t pipe, uint16_t status)
{
    uint16_t buffer;

    /* PID = NAK */
    /* Set NAK */
    usb_cstd_set_nak(pipe);

    /* Disable Interrupt */
    /* Disable Ready Interrupt */
    hw_usb_clear_brdyenb(pipe);

    /* Disable Not Ready Interrupt */
    hw_usb_clear_nrdyenb(pipe);

    /* Disable Empty Interrupt */
    hw_usb_clear_bempenb(pipe);

    usb_cstd_clr_transaction(pipe);

#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
    /* Clear D0FIFO-port */
    buffer = hw_usb_read_fifosel(USB_D0USE);
    if ((buffer & USB_CURPIPE) == pipe)
    {
        /* Changes the FIFO port by the pipe. */
        usb_cstd_chg_curpipe((uint16_t) USB_PIPE0, (uint16_t) USB_D0USE, USB_FALSE);
    }

    /* Clear D1FIFO-port */
    buffer = hw_usb_read_fifosel(USB_D1USE);
    if ((buffer & USB_CURPIPE) == pipe)
    {
        /* Changes the FIFO port by the pipe. */
        usb_cstd_chg_curpipe((uint16_t) USB_PIPE0, (uint16_t) USB_D1USE, USB_FALSE);
    }

#endif /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */

    /* Clear CFIFO-port */
    buffer = hw_usb_read_fifosel(USB_CUSE);
    if ((buffer & USB_CURPIPE) == pipe)
    {
        /* Changes the FIFO port by the pipe. */
        usb_cstd_chg_curpipe((uint16_t) USB_PIPE0, (uint16_t) USB_CUSE, USB_FALSE);
    }

    /* Do Aclr */
    usb_cstd_do_aclrm(pipe);

    /* Call Back */
    if (USB_NULL != gp_usb_hstd_pipe[pipe])
    {
        /* Transfer information set */
        gp_usb_hstd_pipe[pipe]->tranlen = g_usb_hstd_data_cnt[pipe];
        gp_usb_hstd_pipe[pipe]->status = status;
        if (USB_NULL != (gp_usb_hstd_pipe[pipe]->complete))
        {
            (gp_usb_hstd_pipe[pipe]->complete)(gp_usb_hstd_pipe[pipe], 0, 0);
        }

        rtos_release_fixed_memory(&g_rtos_usb_mpf_id, (void *)gp_usb_hstd_pipe[pipe]);
        gp_usb_hstd_pipe[pipe] = (usb_utr_t*) USB_NULL;
        usb_rtos_resend_msg_to_submbx (pipe, USB_HOST);
    }
}
/******************************************************************************
 End of function usb_hstd_forced_termination
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_set_devaddx_register
 Description     : Set USB speed (Full/Hi) of the connected USB Device.
 Arguments       : uint16_t addr    : device address
                 : uint16_t usbspd  : device speed
 Return value    : none
 ******************************************************************************/
void usb_hstd_set_devaddx_register (uint16_t addr, uint16_t usbspd)
{
    uint16_t    data;

    if (USB_DEVICE_0 == addr)
    {
        hw_usb_write_dcpmxps((uint16_t) (USB_DEFPACKET + USB_DEVICE_0));
    }

    if (USB_FSCONNECT == usbspd)
    {
        data    = (uint16_t)(USB_FULLSPEED);
    }
    else if (USB_LSCONNECT == usbspd)
    {
        data    = (uint16_t)(USB_LOWSPEED);
    }
    else                                      /* else if (usbspd == USB_NOCONNECT) */
    {
        data    = (uint16_t)(USB_NOTUSED);    /* RTPT is set to port0 when unused. */
    }

    hw_usb_hset_usbspd(addr, data);
}
/******************************************************************************
 End of function usb_hstd_set_devaddx_register
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_nrdy_endprocess
 Description     : NRDY interrupt processing. (Forced termination of data trans-
                 : mission and reception of specified pipe.)
 Arguments       : uint16_t     pipe        : Pipe No
 Return value    : none
 Note            : none
 ******************************************************************************/
void usb_hstd_nrdy_endprocess (uint16_t pipe)
{
    uint16_t buffer;

    /*
     Host Function
     */
    buffer = usb_cstd_get_pid(pipe);

    /* STALL ? */
    if (USB_PID_STALL == (buffer & USB_PID_STALL))
    {
        /* @4 */
        /* End of data transfer */
        usb_hstd_forced_termination(pipe, USB_DATA_STALL);
    }
    else
    {
        /* Wait for About 60ns */
        buffer = hw_usb_read_syssts();

        /* @3 */
        g_usb_hstd_ignore_cnt[pipe]++;

        if (USB_PIPEERROR == g_usb_hstd_ignore_cnt[pipe])
        {
            /* Data Device Ignore X 3 call back */
            /* End of data transfer */
            usb_hstd_forced_termination(pipe, USB_DATA_ERR);
        }
        else
        {
            /* 5ms wait */
            usb_cpu_delay_xms(5);

            /* PIPEx Data Retry */
            usb_cstd_set_buf(pipe);
        }
    }
}
/******************************************************************************
 End of function usb_hstd_nrdy_endprocess
 ******************************************************************************/

#endif /*(BSP_CFG_RTOS_USED != 0)*/
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
/******************************************************************************
 End of file
 ******************************************************************************/
