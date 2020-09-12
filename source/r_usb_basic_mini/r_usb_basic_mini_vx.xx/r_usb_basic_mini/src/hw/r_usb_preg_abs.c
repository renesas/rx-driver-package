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
 * Copyright (C) 2015(2019) Renesas Electronics Corporation. All rights reserved.
  ******************************************************************************/
/*******************************************************************************
 * File Name    : r_usb_preg_abs.c
 * Description  : Call USB Peripheral register access function
 *****************************************************************************/
/******************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 08.01.2014 1.00    First Release
 *         : 30.11.2018 1.10    Supporting Smart Configurator
 *         : 31.05.2019 1.11    Added support for GNUC and ICCRX.
 *         : 30.06.2019 1.12    RX23W is added.
 ******************************************************************************/
/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_basic_mini_if.h"
#include "r_usb_typedef.h"
#include "r_usb_extern.h"
#include "r_usb_reg_access.h"
#include "r_usb_bitdefine.h"

#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
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

/******************************************************************************
 Function Name   : usb_pstd_interrupt_handler
 Description     : Determine which USB interrupt occurred and report results to
                 : the usb_putr_t argument's ipp, type, and status members.
 Arguments       : usb_putr_t *p_type      : Pointer to interrupt type.
                 : usb_putr_t *p_status    : Pointer to interrupt status.
 Return value    : none
 ******************************************************************************/
void usb_pstd_interrupt_handler(uint16_t * p_type, uint16_t * p_status)
{
    uint16_t intsts0;
    uint16_t intenb0;
    uint16_t ists0;
    uint16_t brdysts;
    uint16_t brdyenb;
    uint16_t bsts;
    uint16_t nrdysts;
    uint16_t nrdyenb;
    uint16_t nsts;
    uint16_t bempsts;
    uint16_t bempenb;
    uint16_t ests;

    /* Register Save */
    intsts0 = USB0.INTSTS0.WORD;
    brdysts = USB0.BRDYSTS.WORD;
    nrdysts = USB0.NRDYSTS.WORD;
    bempsts = USB0.BEMPSTS.WORD;
    intenb0 = USB0.INTENB0.WORD;
    brdyenb = USB0.BRDYENB.WORD;
    nrdyenb = USB0.NRDYENB.WORD;
    bempenb = USB0.BEMPENB.WORD;


    *p_type = USB_INT_UNKNOWN;
    *p_status = 0;

    /* Interrupt status get */
    ists0 = (uint16_t)(intsts0 & intenb0);
    bsts = (uint16_t)(brdysts & brdyenb);
    nsts = (uint16_t)(nrdysts & nrdyenb);
    ests = (uint16_t)(bempsts & bempenb);

    if ((intsts0 & (USB_VBINT | USB_RESM | USB_SOFR | USB_DVST |
                            USB_CTRT | USB_BEMP | USB_NRDY | USB_BRDY)) == 0u)
    {
        return;
    }

    /***** Processing USB bus signal *****/
    /***** Resume signal *****/
    if (USB_RESM == (ists0 & USB_RESM))
    {
        USB0.INTSTS0.WORD = (uint16_t)~USB_RESM;
        *p_type = USB_INT_RESM;
    }
    /***** Vbus change *****/
    else if (USB_VBINT == (ists0 & USB_VBINT))
    {
        /* Status clear */
        USB0.INTSTS0.WORD = (uint16_t)~USB_VBINT;
        *p_type = USB_INT_VBINT;
    }

    /***** SOFR change *****/
    else if (USB_SOFR == (ists0 & USB_SOFR))
    {
        /* SOFR Clear */
        USB0.INTSTS0.WORD = (uint16_t)~USB_SOFR;
        *p_type = USB_INT_SOFR;
    }

    /***** Processing device state *****/
    /***** DVST change *****/
    else if (USB_DVST == (ists0 & USB_DVST))
    {
        /* DVST clear */
        USB0.INTSTS0.WORD = (uint16_t)~USB_DVST;

        *p_type = USB_INT_DVST;
        *p_status = intsts0;
    }

    /***** Processing PIPE0 data *****/
    else if ((USB_BRDY == (ists0 & USB_BRDY)) && ((USB_BRDY0) == (bsts & USB_BRDY0)))
    {
        USB0.BRDYSTS.WORD = (uint16_t) ((~USB_BRDY0) & USB_BRDYSTS_MASK);

        *p_type = USB_INT_BRDY;
        *p_status = USB_BRDY0;
    }
    else if ((USB_BEMP == (ists0 & USB_BEMP)) && (USB_BEMP0 == (ests & USB_BEMP0)))
    {
        USB0.BEMPSTS.WORD = (uint16_t) ((~USB_BEMP0) & USB_BEMPSTS_MASK);
        *p_type = USB_INT_BEMP;
        *p_status = USB_BEMP0;
    }
    else if ((USB_NRDY == (ists0 & USB_NRDY)) && (USB_NRDY0 == (nsts & USB_NRDY0)))
    {
        USB0.NRDYSTS.WORD = (uint16_t) ((~USB_NRDY0) & USB_NRDYSTS_MASK);
        *p_type = USB_INT_NRDY;
        *p_status = USB_NRDY0;
    }

    /***** Processing setup transaction *****/
    else if (USB_CTRT == (ists0 & USB_CTRT))
    {
        /* CTSQ bit changes later than CTRT bit for ASSP. */
        /* CTSQ reloading */
        *p_status = hw_usb_read_intsts();

        /* USB_CTRT clear */
        USB0.INTSTS0.WORD = (uint16_t)~USB_CTRT;
        *p_type = USB_INT_CTRT;

        if (USB_CS_SQER == (uint8_t)((*p_status) & USB_CTSQ))
        {
            hw_usb_pclear_sts_valid();
            *p_type = USB_INT_UNKNOWN;
            *p_status = 0;
            return;
        }
    }

    /***** Processing PIPE1-MAX_PIPE_NO data *****/
    else if (USB_BRDY == (ists0 & USB_BRDY))
    {
        USB0.BRDYSTS.WORD = (uint16_t) ((~bsts) & USB_BRDYSTS_MASK);
        *p_type = USB_INT_BRDY;
        *p_status = bsts;
    }
    else if (USB_BEMP == (ists0 & USB_BEMP))
    {
        USB0.BEMPSTS.WORD = (uint16_t) ((~ests) & USB_BEMPSTS_MASK);
        *p_type = USB_INT_BEMP;
        *p_status = ests;
    }
    else if (USB_NRDY == (ists0 & USB_NRDY))
    {
        USB0.NRDYSTS.WORD = (uint16_t) ((~nsts) & USB_NRDYSTS_MASK);
        *p_type = USB_INT_NRDY;
        *p_status = nsts;
    }
    else
    {
        /* Non processing. */
    }
} /* End of function usb_pstd_interrupt_handler */

/******************************************************************************
 Function Name   : usb_pstd_save_request
 Description     : Save received USB command.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_pstd_save_request(void)
{
    /* Valid clear */
    hw_usb_pclear_sts_valid();

    g_usb_pstd_req_type = hw_usb_read_usbreq();
    g_usb_pstd_req_value = hw_usb_read_usbval();
    g_usb_pstd_req_index = hw_usb_read_usbindx();
    g_usb_pstd_req_length = hw_usb_read_usbleng();
} /* End of function usb_pstd_save_request */

/******************************************************************************
 Function Name   : usb_pstd_chk_configured
 Description     : Check if USB Device is in a CONFIGURED state.
 Arguments       : none
 Return value    : Configuration state (YES/NO)
 ******************************************************************************/
uint16_t usb_pstd_chk_configured(void)
{
    uint16_t buf;

    buf = hw_usb_read_intsts();

    /* Device Status - Configured check */
    if ((buf & USB_DVSQ) == USB_DS_CNFG)
    {
        /* Configured */
        return USB_TRUE;
    }
    else
    {
        /* not Configured */
        return USB_FALSE;
    }
} /* End of function usb_pstd_chk_configured */

/******************************************************************************
 Function Name   : usb_pstd_remote_wakeup
 Description     : Set the USB peripheral to implement remote wake up.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_pstd_remote_wakeup(void)
{
    uint16_t buf;

    g_usb_pstd_remote_wakeup_state = USB_ERROR;

    /* Support remote wakeup ? */
    if (USB_TRUE == g_usb_pstd_remote_wakeup)
    {
        /* RESM interrupt disable */
        hw_usb_pclear_enb_rsme();

        /* RESM status read */
        buf = hw_usb_read_intsts();
        if ((buf & USB_RESM) != (uint16_t)0)
        {
            g_usb_pstd_remote_wakeup_state = USB_QOVR;
            /* RESM status clear */
            hw_usb_pclear_sts_resm();
        }
        else
        {
            if ((buf & USB_DS_SUSP) != (uint16_t)0)
            {
                /* Remote wakeup set */
                hw_usb_pset_wkup();
                g_usb_pstd_remote_wakeup_state = USB_OK;
            }
        }
    }
} /* End of function usb_pstd_remote_wakeup */

/******************************************************************************
 Function Name   : usb_pstd_resume_process
 Description     : Set USB registers to implement resume processing.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_pstd_resume_process(void)
{
    /* RESM status clear */
    hw_usb_pclear_sts_resm();

    /* RESM interrupt disable */
    hw_usb_pclear_enb_rsme();

} /* End of function usb_pstd_resume_process */

/******************************************************************************
 Function Name   : usb_pstd_set_stall
 Description     : Set the specified pipe's PID to STALL.
 Arguments       : uint16_t pipe   : Pipe Number
 Return value    : none
 ******************************************************************************/
void usb_pstd_set_stall(uint16_t pipe)
{
    /* PIPE control reg set */
    hw_usb_set_pid(pipe, USB_PID_STALL);
} /* End of function usb_pstd_set_stall */

/******************************************************************************
 Function Name   : usb_pstd_set_stall_pipe0
 Description     : Set pipe "0" PID to STALL.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_pstd_set_stall_pipe0(void)
{
    /* PIPE control reg set */
    hw_usb_set_pid(USB_PIPE0, USB_PID_STALL);
} /* End of function usb_pstd_set_stall_pipe0 */

/******************************************************************************
 Function Name   : usb_pstd_write_fifo
 Description     : Write specified amount of data to specified USB FIFO.
 Arguments       : uint16_t  count       : Write size.
                 : uint16_t  pipemode    : The mode of CPU/DMA(D0)/DMA(D1).
                 : uint16_t  *p_buff    : Address of buffer of data to write.
 Return value    : The incremented address of last argument (write_p).
 ******************************************************************************/
uint8_t *usb_pstd_write_fifo(uint16_t count, uint16_t pipemode, uint8_t *p_buff)
{
    uint16_t even;

    /* WAIT_LOOP */
    for (even = (uint16_t)(count >> 1); (0 != even); --even)
    {
        /* 16bit access */
        hw_usb_write_fifo16(pipemode, *((uint16_t *)p_buff));

        /* Renewal write pointer */
        p_buff += sizeof(uint16_t);
    }

    if (0u != (count & (uint16_t)0x0001u))
    {
        /* 8bit access */
        /* count == odd */
        /* Change FIFO access width */
        hw_usb_set_mbw(pipemode, USB_MBW_8);

        /* FIFO write */
        hw_usb_write_fifo8(pipemode, *p_buff);

        /* Return FIFO access width */
        hw_usb_set_mbw(pipemode, USB_MBW_16);

        /* Renewal write pointer */
        p_buff++;
    }

    return p_buff;

} /* End of function usb_pstd_write_fifo */

/******************************************************************************
 Function Name   : usb_pstd_read_fifo
 Description     : Read specified buffer size from the USB FIFO.
 Arguments       : uint16_t  count       : Read size.
                 : uint16_t  pipemode    : The mode of CPU/DMA(D0)/DMA(D1).
                 : uint16_t  *p_buff    : Address of buffer to store the read data.
 Return value    : Pointer to a buffer that contains the data to be read next.
 ******************************************************************************/
uint8_t *usb_pstd_read_fifo(uint16_t count, uint16_t pipemode, uint8_t *p_buff)
{
    uint16_t even;
    uint32_t odd_byte_data_temp;
    
    /* WAIT_LOOP */
    for (even = (uint16_t)(count >> 1); (0 != even); --even)
    {
        /* 16bit FIFO access */
        *(uint16_t *)p_buff= hw_usb_read_fifo16(pipemode);

        /* Renewal read pointer */
        p_buff += sizeof(uint16_t);
    }
    if (0 != (count & (uint16_t)0x0001))
    {
        /* 16bit FIFO access */
        odd_byte_data_temp = hw_usb_read_fifo16(pipemode);

        /* Condition compilation by the difference of the little endian */
#if USB_CFG_ENDIAN == USB_CFG_LITTLE
        *p_buff = (uint8_t)(odd_byte_data_temp & 0x00ff);
#else   /* USB_CFG_ENDIAN == USB_CFG_LITTLE */
        *p_buff = (uint8_t)(odd_byte_data_temp >> 8);
#endif  /* USB_CFG_ENDIAN == USB_CFG_LITTLE */

        /* Renewal read pointer */
        p_buff += sizeof(uint8_t);
    }

    return p_buff;
} /* End of function usb_pstd_read_fifo */

/******************************************************************************
 Function Name   : usb_pstd_forced_termination
 Description     : Terminate data transmission and reception.
 Arguments       : uint16_t pipe     : Pipe Number
                 : uint16_t status   : Transfer status type
 Return value    : none
 Note            : In the case of timeout status, it does not call back.
 ******************************************************************************/
void usb_pstd_forced_termination(uint16_t pipe, uint16_t status)
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
    buffer = hw_usb_read_fifosel(USB_D0USE);
    if ((buffer & USB_CURPIPE) == pipe )
    {
        /* Stop DMA,FIFO access */
        usb_cstd_dma_stop(USB_D0USE);
        /* Changes the FIFO port by the pipe. */
        usb_cstd_chg_curpipe(USB_PIPE0, USB_D0USE, USB_NOUSE);
    }

    buffer = hw_usb_read_fifosel(USB_D1USE);
    if ((buffer & USB_CURPIPE) == pipe )
    {
        /* Stop DMA,FIFO access */
        usb_cstd_dma_stop(USB_D1USE);
        /* Changes the FIFO port by the pipe. */
        usb_cstd_chg_curpipe(USB_PIPE0, USB_D1USE, USB_NOUSE);
    }
#endif /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */

    buffer = hw_usb_read_fifosel(USB_CUSE);
    if ((buffer & USB_CURPIPE) == pipe )
    {
        /* FIFO buffer SPLIT transaction initialized */
        usb_cstd_chg_curpipe(USB_PIPE0, USB_CUSE, USB_NOUSE);
    }

    /* Do Aclr */
    usb_cstd_do_aclrm(pipe);

    /* Call Back */
    if (USB_NULL != gp_usb_pstd_pipe[pipe])
    {
        /* Transfer information set */
        gp_usb_pstd_pipe[pipe]->tranlen = g_usb_pstd_data_cnt[pipe];
        gp_usb_pstd_pipe[pipe]->status = status;
        gp_usb_pstd_pipe[pipe]->pipectr = hw_usb_read_pipectr(pipe);

        if (USB_NULL != (gp_usb_pstd_pipe[pipe]->complete))
        {
            (gp_usb_pstd_pipe[pipe]->complete)(gp_usb_pstd_pipe[pipe], USB_NULL, USB_NULL);
        }

        gp_usb_pstd_pipe[pipe] = (usb_putr_t *)USB_NULL;
    }
} /* End of function usb_pstd_forced_termination */

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */

/******************************************************************************
 End of file
 ******************************************************************************/
