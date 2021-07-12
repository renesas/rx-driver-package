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
 * File Name    : r_usb_hdriver_rtos.c
* Description  : This is the USB host control driver code.
 ***********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 30.06.2020 1.20    First Release
 ***********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_usb_basic_mini_if.h"

#if (BSP_CFG_RTOS_USED != 0)    /* RTOS */

#include <string.h>

#include "r_usb_typedef.h"
#include "r_usb_extern.h"
#include "r_usb_bitdefine.h"
#include "r_usb_reg_access.h"
#include "r_usb_cstd_rtos.h"

#if defined(USB_CFG_HCDC_USE)
#include "r_usb_hcdc_mini_if.h"

#endif /* defined(USB_CFG_PCDC_USE) */

#if defined(USB_CFG_HHID_USE)
#include "r_usb_hhid_mini_if.h"

#endif /* defined(USB_CFG_HMSC_USE) */

#if defined(USB_CFG_HMSC_USE)
#include "r_usb_hmsc_mini_if.h"

#endif /* defined(USB_CFG_HMSC_USE) */

#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
#include "r_usb_dmac.h"
#endif  /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
#if defined(USB_CFG_HVND_USE)
static void     usb_hvnd_configured (usb_utr_t *ptr, uint16_t dev_addr, uint16_t data2);
static void     usb_hvnd_detach (usb_utr_t *ptr, uint16_t dev_addr, uint16_t data2);
static void     usb_hvnd_enumeration (uint16_t **table);
static void     usb_hvnd_pipe_info (uint8_t *table, uint16_t length);
#endif /* defined(USB_CFG_HVND_USE) */


static void usb_hstd_set_submitutr (usb_utr_t *ptr);
static void usb_hstd_set_retransfer (uint16_t pipe);

/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/
/* Device driver (registration) */
usb_hcdreg_t g_usb_hstd_device_drv;

/* Root port, status, config num, interface class, speed, */
uint8_t g_usb_hcd_remote_wakeup;

/* Control transfer stage management */
uint16_t g_usb_hstd_ctsq;

/* Manager mode */
uint16_t g_usb_hstd_mgr_mode;

/* DEVSEL & DCPMAXP (Multiple device) */
uint16_t g_usb_hstd_dcp_register;

/* Ignore count */
uint16_t g_usb_hstd_ignore_cnt[USB_MAX_PIPE_NO + 1u];

/* USB data transfer */
/* PIPEn Buffer counter */
uint32_t g_usb_hstd_data_cnt[USB_MAX_PIPE_NO + 1u];

/* PIPEn Buffer pointer(8bit) */
uint8_t *gp_usb_hstd_data_ptr[USB_MAX_PIPE_NO + 1u];

/* Message pipe */
usb_utr_t *gp_usb_hstd_pipe[USB_MAX_PIPE_NO + 1u];


#if defined(USB_CFG_HVND_USE)
/* Target Peripheral List */
static const uint16_t gs_usb_apl_devicetpl[] =
{
    USB_CFG_TPLCNT, /* Number of list */
    0,              /* Reserved */
    USB_CFG_TPL     /* Vendor ID, Product ID */
};
#endif /* defined(USB_CFG_HVND_USE) */

rtos_task_id_t  g_usb_hstd_change_device_state_task_id;

/******************************************************************************
 Renesas USB Host Driver functions
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_transfer_start_req
 Description     : Send a request for data transfer to HCD (Host Control Driver) 
                 : using the specified pipe.
 Arguments       : usb_utr_t *ptr : Pointer to usb_utr_t structure.
 Return          : usb_er_t       : USB_OK/USB_ERROR
 ******************************************************************************/
usb_er_t usb_hstd_transfer_start_req (usb_utr_t *ptr)
{
    usb_er_t err;
    uint16_t        pipenum;
    uint16_t        devsel;
    rtos_err_t      ret;
    rtos_task_id_t  task_id;
    usb_utr_t       *p_tran_data;

    pipenum = ptr->keyword;

    if (USB_PIPE0 == pipenum)
    {
        devsel = (uint16_t) (ptr->p_setup[4] << USB_DEVADDRBIT);
    }
    else
    {
        /* Get device address from pipe number */
        devsel = usb_hstd_pipe_to_devsel(pipenum);
    }
    if ((USB_DEVICE_0 == devsel) && (USB_PIPE0 != pipenum))
    {
        return USB_ERROR;
    }

    ptr->msghead = (usb_mh_t) USB_NULL;
    ptr->msginfo = USB_MSG_HCD_SUBMITUTR;

    if (USB_MAX_PIPE_NO < pipenum)
    {
        return USB_ERROR;
    }

    ret = rtos_get_fixed_memory(&g_rtos_usb_mpf_id, (void **)&p_tran_data, RTOS_ZERO);
    if (RTOS_ERROR == ret)
    {
        return USB_ERROR;
    }
    *p_tran_data = *ptr;
    if ((0 != ptr->p_setup) && (USB_PIPE0 == pipenum))
    {
        p_tran_data->setup_data[0] = ptr->p_setup[0];
        p_tran_data->setup_data[1] = ptr->p_setup[1];
        p_tran_data->setup_data[2] = ptr->p_setup[2];
        p_tran_data->setup_data[3] = ptr->p_setup[3];
        p_tran_data->setup_data[4] = ptr->p_setup[4];
        p_tran_data->p_setup = (uint16_t *)&p_tran_data->setup_data;
    }
    rtos_get_task_id(&task_id);

    p_tran_data->task_id = task_id;

    /* Send message */
    err = rtos_send_mailbox (&g_rtos_usb_hcd_mbx_id, (void *)p_tran_data);
    if (RTOS_SUCCESS != err)
    {
        rtos_release_fixed_memory(&g_rtos_usb_mpf_id, (void *)p_tran_data);
    }

    return err;
}
/******************************************************************************
 End of function usb_hstd_transfer_start_req
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_device_resume
 Description     : Send request for RESUME signal output to USB device to MGR task.
 Argument        : uint16_t  devaddr    : Device Address
 Return          : none
 ******************************************************************************/
void usb_hstd_device_resume (uint16_t devaddr)
{
    usb_hstd_mgr_snd_mbx((uint16_t) USB_MSG_HCD_RESUME, devaddr, (uint16_t) 0u);
}
/******************************************************************************
 End of function usb_hstd_device_resume
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_hcd_snd_mbx
 Description     : Send specified message to HCD (Host Control Driver) task.
 Arguments       : uint16_t  msginfo : Message info..
                 : uint16_t  dat     : Pipe no.
                 : uint16_t  *adr    : Address.
                 : usb_cb_t  callback: Callback function pointer.
 Return          : usb_er_t          : USB_OK etc.
 ******************************************************************************/
usb_er_t usb_hstd_hcd_snd_mbx (uint16_t msginfo, uint16_t dat, uint16_t *adr, usb_cb_t callback)
{
    usb_mh_t p_blf;
    rtos_err_t err;
    usb_utr_t *hp;

    /* Get memory pool blk */
    err = rtos_get_fixed_memory(&g_rtos_usb_mpf_id, (void **)&p_blf, RTOS_ZERO);
    if (RTOS_SUCCESS == err)
    {
        hp = (usb_utr_t *) p_blf;
        hp->msghead = (usb_mh_t) USB_NULL;
        hp->msginfo = msginfo;
        hp->keyword = dat;
        hp->p_tranadr = adr;
        hp->complete = callback;

        /* Send message */
        err = rtos_send_mailbox (&g_rtos_usb_hcd_mbx_id, (void *)p_blf);
        if (RTOS_SUCCESS != err)
        {
            rtos_release_fixed_memory(&g_rtos_usb_mpf_id, (void *)p_blf);
        }
    }
    else
    {
        /* Error */
        /* WAIT_LOOP */
        while (1)
        {
            /* error */
        }
    }
    return err;
}
/******************************************************************************
 End of function usb_hstd_hcd_snd_mbx
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_mgr_snd_mbx
 Description     : Send the message to MGR(Manager) task
 Arguments       : uint16_t msginfo  : Message info.
                 : uint16_t dat      : Port no.
                 : uint16_t res      : Result
 Return          : none
 ******************************************************************************/
void usb_hstd_mgr_snd_mbx (uint16_t msginfo, uint16_t dat, uint16_t res)
{
    usb_mh_t p_blf;
    rtos_err_t err;
    usb_utr_t *mp;

    /* Get memory pool blk */
    err = rtos_get_fixed_memory(&g_rtos_usb_mpf_id, (void **)&p_blf, RTOS_ZERO);
    if (RTOS_SUCCESS == err)
    {
        mp = (usb_utr_t *) p_blf;
        mp->msghead = (usb_mh_t) USB_NULL;
        mp->msginfo = msginfo;
        mp->keyword = dat;
        mp->result = res;

        /* Send message */
        err = rtos_send_mailbox (&g_rtos_usb_mgr_mbx_id, (void *)p_blf);
        if (RTOS_SUCCESS != err)
        {
            rtos_release_fixed_memory(&g_rtos_usb_mpf_id, (void *)p_blf);
        }
    }
    else
    {
        /* Error */
        /* WAIT_LOOP */
        while (1)
        {
            /* error */
        }
    }
}
/******************************************************************************
 End of function usb_hstd_mgr_snd_mbx
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_hcd_rel_mpl
 Description     : Release the secured memory block.
 Arguments       : usb_utr_t *ptr    : Pointer to usb_utr_t structure.
                 : uint16_t n        : Error no.
 Return          : none
 ******************************************************************************/
void usb_hstd_hcd_rel_mpl (usb_utr_t *ptr, uint16_t n)
{
    /* Memory Pool Release */
    rtos_release_fixed_memory(&g_rtos_usb_mpf_id, (void *)ptr);
}
/******************************************************************************
 End of function usb_hstd_hcd_rel_mpl
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_suspend
 Description     : Request suspend for USB device.
 Arguments       : usb_utr_t *ptr    : Pointer to usb_utr_t structure.
 Return          : none
 ******************************************************************************/
void usb_hstd_suspend (usb_utr_t *ptr)
{
    /* Control transfer idle stage ? */
    if (USB_IDLEST == g_usb_hstd_ctsq)
    {
        /* USB suspend process */
        usb_hstd_suspend_process();
        usb_hstd_chk_clk(); /* Check clock */
        (ptr->complete)(ptr, USB_NULL, ptr->msginfo);
    }
    else
    {
        /* 1ms wait */
        usb_cpu_delay_xms((uint16_t) 1);
        /* Change device state request */
        usb_hstd_hcd_snd_mbx(ptr->msginfo, USB_NULL, (uint16_t*) 0, &usb_hstd_status_result);
    }
}
/******************************************************************************
 End of function usb_hstd_suspend
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_set_submitutr
 Description     : Submit utr: Get the device address via the specified pipe num-
                 : ber and do a USB transfer.
 Arguments       : usb_utr_t *ptr : Pointer to usb_utr_t structure.
 Return          : none
 ******************************************************************************/
static void usb_hstd_set_submitutr (usb_utr_t *ptr)
{
    uint16_t pipenum;
    uint16_t devsel;
    uint16_t connect_inf;
    uint16_t end_flag;
    usb_utr_t *pp;

    pipenum = ptr->keyword;

    /* Get device address from pipe number */
    if (USB_PIPE0 == pipenum)
    {
        devsel = (uint16_t) (ptr->p_setup[4] << USB_DEVADDRBIT);
    }
    else
    {
        /* Get device address from pipe number */
        devsel = usb_hstd_pipe_to_devsel(pipenum);
    }

    /* Get device speed from device address */
    connect_inf = usb_hstd_devaddr_to_speed(devsel);
    if (USB_DEVADD_NOCONNECT == connect_inf)
    {
        usb_rtos_delete_msg_submbx (ptr, USB_HOST);
        return;
    }

    /* Pipe Transfer Process check */
    if (USB_NULL != gp_usb_hstd_pipe[pipenum])
    {
        usb_rtos_send_msg_to_submbx (ptr, pipenum, USB_HOST);
        return;
    }

    gp_usb_hstd_pipe[pipenum] = ptr;

    if ((USB_DEVICE_0 == devsel) && (USB_PIPE0 != pipenum))
    {
        usb_hstd_forced_termination(pipenum, (uint16_t) USB_DATA_ERR);
        return;
    }

    /* Control Transfer */
    if (USB_PIPE0 == pipenum)
    {
        /* Control transfer idle stage ? */
        if (USB_IDLEST == g_usb_hstd_ctsq)
        {
            usb_hstd_setup_start();
        }

        /* Control Read Data */
        else if (USB_DATARDCNT == g_usb_hstd_ctsq)
        {
            pp = gp_usb_hstd_pipe[USB_PIPE0];
            usb_hstd_control_read_start(pp->tranlen, (uint8_t*) pp->p_tranadr); /* Control read start */
        }

        /* Control Write Data */
        else if (USB_DATAWRCNT == g_usb_hstd_ctsq)
        {
            pp = gp_usb_hstd_pipe[USB_PIPE0];

            /* Control write start */
            end_flag = usb_hstd_control_write_start(pp->tranlen, (uint8_t*) pp->p_tranadr);
            if (USB_FIFOERROR == end_flag)
            {
                usb_hstd_control_end((uint16_t) USB_DATA_ERR); /* Control Read/Write End */
            }
        }
        else
        {
            /* Control Read/Write End */
            usb_hstd_control_end((uint16_t) USB_DATA_ERR);
        }
    }
    else
    {
        usb_hstd_set_retransfer(pipenum); /* Data Transfer */
    }
}
/******************************************************************************
 End of function usb_hstd_set_submitutr
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_set_retransfer
 Description     : Start IN/OUT transfer based on the specified pipe.
 Arguments       : uint16_t  pipe : Pipe number
 Return          : none
 ******************************************************************************/
static void usb_hstd_set_retransfer (uint16_t pipe)
{
    if (USB_MAX_PIPE_NO < pipe)
    {
        return; /* Error */
    }

    /* Data Transfer */
    if (usb_cstd_get_pipe_dir(pipe) == USB_DIR_H_IN)
    {
        /* IN Transfer */
        usb_hstd_receive_start(pipe);
    }
    else
    {
        /* OUT Transfer */
        usb_hstd_send_start(pipe);
    }
}
/******************************************************************************
 End of function usb_hstd_set_retransfer
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_clr_stall
 Description     : Clear Stall
 Arguments       : uint16_t pipe        : Pipe number.
                 : usb_cb_t complete    : Callback function
 Return value    : uint16_t             : Error info.
 ******************************************************************************/
usb_er_t usb_hstd_clr_stall (uint16_t pipe, usb_cb_t complete)
{
    usb_er_t err;
    uint8_t dir_ep;

    if (USB_MAX_PIPE_NO < pipe)
    {
        return USB_ERROR; /* Error */
    }

    dir_ep = usb_hstd_pipe_to_epadr(pipe);

    err = usb_hstd_clear_feature((uint16_t) dir_ep, complete);
    if (USB_OK == err)
    {
        usb_cstd_clr_stall(pipe);
        hw_usb_set_sqclr(pipe); /* SQCLR */
    }
    return err;
}
/******************************************************************************
 End of function usb_hstd_clr_stall
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_hcd_task
 Description     : USB Host Control Driver Task.
 Argument        : usb_vp_int_t stacd  : Task Start Code.
 Return          : none
 ******************************************************************************/
void usb_hstd_hcd_task (usb_vp_int_t stacd)
{
    usb_utr_t *ptr;
    usb_er_t err;
    uint16_t rootport;
    uint16_t pipenum;
    uint16_t msginfo;
    uint16_t connect_inf;
    rtos_err_t ret;

    /* WAIT_LOOP */
    while (1)
    {
        /* Receive message */
        ret = rtos_receive_mailbox (&g_rtos_usb_hcd_mbx_id, (void **)&ptr, 10000);
        if (RTOS_SUCCESS == ret)
        {
            rootport = ptr->keyword;
            pipenum = ptr->keyword;

            /* Branch Hcd Task receive Message Command */
            msginfo = ptr->msginfo;
            switch (msginfo)
            {
                case USB_MSG_HCD_INT :
                    /* USB INT */
                    usb_hstd_interrupt_process(ptr);
                    rtos_release_fixed_memory(&g_rtos_usb_mpf_id, (void **)ptr);
                break;

                case USB_MSG_HCD_SUBMITUTR :
                    /* USB Submit utr */
                    usb_hstd_set_submitutr(ptr);
                break;

                case USB_MSG_HCD_USBRESET :
                    /* USB bus reset */
                    usb_hstd_bus_reset();
                    /* Check current port speed */
                    connect_inf = usb_cstd_port_speed();
                    /* Callback */
                    (ptr->complete)(ptr, USB_NULL, connect_inf);
                    /* Release Memory Block */
                    usb_hstd_hcd_rel_mpl(ptr, msginfo);
                break;

                case USB_MSG_HCD_REMOTE :
                    /* Suspend device */
                    g_usb_hcd_remote_wakeup = USB_YES;
                    usb_hstd_suspend(ptr);
                    /* CallBack */
                    (ptr->complete)(ptr, USB_NULL, USB_MSG_HCD_REMOTE);

                    /* Release Memory Block */
                    usb_hstd_hcd_rel_mpl(ptr, msginfo);
                break;

                case USB_MSG_HCD_RESUME :
                    usb_hstd_resume_process(); /* USB resume */
                    (ptr->complete)(ptr, rootport, USB_MSG_HCD_RESUME); /* Callback */
                    usb_hstd_hcd_rel_mpl(ptr, msginfo); /* Release Memory Block */
                break;

                case USB_MSG_HCD_CLR_STALL :
                    err = usb_hstd_clr_stall(pipenum, (usb_cb_t) &class_trans_result);
                    (ptr->complete)(ptr, (uint16_t)err, USB_MSG_HCD_CLR_STALL);
                    /* Release Memory Block */
                    usb_hstd_hcd_rel_mpl(ptr, msginfo);
                break;

                case USB_MSG_HCD_CLRSEQBIT :
                    hw_usb_set_sqclr(pipenum); /* SQCLR */
                    (ptr->complete)(ptr, (uint16_t) USB_NO_ARG, (uint16_t) USB_MSG_HCD_CLRSEQBIT); /* Callback */
                    usb_hstd_hcd_rel_mpl(ptr, msginfo);
                break;

                case USB_MSG_HCD_SETSEQBIT :
                    hw_usb_set_sqset(pipenum); /* SQSET */
                    (ptr->complete)(ptr, (uint16_t) USB_NO_ARG, (uint16_t) USB_MSG_HCD_SETSEQBIT); /* Callback */
                    usb_hstd_hcd_rel_mpl(ptr, msginfo); /* Release Memory Block */
                break;

                case USB_MSG_HCD_TRANSEND1 :
                    /* Pipe Transfer Process check */
                    if (USB_NULL != gp_usb_hstd_pipe[pipenum])
                    {
                        /* Control Transfer stop */
                        if (USB_PIPE0 == pipenum)
                        {
                            /* Control Read/Write End */
                            usb_hstd_control_end((uint16_t) USB_DATA_TMO);
                        }
                        else
                        {
                            /* Transfer stop */
                            usb_hstd_forced_termination(pipenum, (uint16_t) USB_DATA_TMO);
                        }
                    }

                    /* Release Memory Block */
                    usb_hstd_hcd_rel_mpl(ptr, msginfo);
                break;

                case USB_MSG_HCD_TRANSEND2 :
                    /* Pipe Transfer Process check */
                    if (USB_NULL != gp_usb_hstd_pipe[pipenum])
                    {
                        /* Control Transfer stop */
                        if (USB_PIPE0 == pipenum)
                        {
                            /* Control Read/Write End */
                            usb_hstd_control_end((uint16_t) USB_DATA_STOP);
                        }
                        else
                        {
                            /* Transfer stop */
                            usb_hstd_forced_termination(pipenum, (uint16_t) USB_DATA_STOP);
                        }
                    }

                    /* Release Memory Block */
                    usb_hstd_hcd_rel_mpl(ptr, msginfo);
                break;

                default :
                break;
            }
        }
    } /* End of while(1) */
}
/******************************************************************************
 End of function usb_hstd_hcd_task
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_transfer_end
 Description     : Request HCD to force termination of data transfer.
 Arguments       : uint16_t pipe        : Pipe number
                 : uint16_t status      : Data transfer status
 Return          : usb_err_t error code : USB_OK etc
 ******************************************************************************/
usb_er_t usb_hstd_transfer_end (uint16_t pipe, uint16_t status)
{
    uint16_t msg;
    usb_er_t err;

    if (USB_NULL == gp_usb_hstd_pipe[pipe])
    {
        return USB_QOVR;
    }

    if (USB_DATA_TMO == status)
    {
        msg = USB_MSG_HCD_TRANSEND1;
    }
    else
    {
        msg = USB_MSG_HCD_TRANSEND2;
    }

    err = usb_hstd_hcd_snd_mbx(msg, pipe, (uint16_t*) 0, &usb_hstd_dummy_function);
    return err;
}
/******************************************************************************
 End of function usb_hstd_transfer_end
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_driver_registration
 Description     : The HDCD information registered in the class driver structure
                 : is registered in HCD.
 Arguments       : usb_hcdreg_t *callback   : Pointer to usb_hcdreg_t structure
 Return          : none
 ******************************************************************************/
void usb_hstd_driver_registration (usb_hcdreg_t *callback)
{
    usb_hcdreg_t *p_driver;

    p_driver = &g_usb_hstd_device_drv;

    p_driver->rootport = USB_NOPORT; /* Root port */
    p_driver->devaddr = USB_NODEVICE; /* Device address */
    p_driver->devstate = USB_DETACHED; /* Device state */
    p_driver->ifclass = callback->ifclass; /* Interface Class */
    p_driver->p_tpl = callback->p_tpl; /* Target peripheral list */
    p_driver->classinit = callback->classinit; /* Driver init */
    p_driver->classcheck = callback->classcheck; /* Driver check */
    p_driver->devconfig = callback->devconfig; /* Device configured */
    p_driver->devdetach = callback->devdetach; /* Device detach */
    p_driver->devsuspend = callback->devsuspend; /* Device suspend */
    p_driver->devresume = callback->devresume; /* Device resume */
    /* Initialized device driver */
    (*p_driver->classinit)(USB_NULL, p_driver->devaddr, (uint16_t) USB_NO_ARG);
}
/******************************************************************************
 End of function usb_hstd_driver_registration
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_driver_release
 Description     : Release the Device Class Driver.
 Arguments       : uint8_t   devclass   : Interface class
 Return          : none
 ******************************************************************************/
void usb_hstd_driver_release (uint8_t devclass)
{
    usb_hcdreg_t *p_driver;

    if (USB_IFCLS_NOT != devclass)
    {
        p_driver = &g_usb_hstd_device_drv;
        if (p_driver->ifclass == devclass)
        {
            p_driver->rootport = USB_NOPORT; /* Root port */
            p_driver->devaddr = USB_NODEVICE; /* Device address */
            p_driver->devstate = USB_DETACHED; /* Device state */

            /* Interface Class : NO class */
            p_driver->ifclass = (uint16_t) USB_IFCLS_NOT;
        }
    }
}
/******************************************************************************
 End of function usb_hstd_driver_release
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_change_device_state
 Description     : Request to change the status of the connected USB Device
 Arguments       : usb_cb_t complete    : callback function pointer
                 : uint16_t msginfo     : Message Info
                 : uint16_t member      : Device address/pipe number
 Return          : usb_err_t            : USB_OK etc
 ******************************************************************************/
usb_er_t usb_hstd_change_device_state (usb_cb_t complete, uint16_t msginfo, uint16_t member)
{
    usb_mh_t            p_blf;
    usb_er_t            err = USB_SUCCESS;
    rtos_err_t          ret;
    usb_utr_t           *hp;
    rtos_task_id_t      task_id;

    switch (msginfo)
    {
        /* USB_MSG_HCD_CLR_STALL */
        case USB_DO_CLR_STALL :
            err = usb_hstd_hcd_snd_mbx(USB_MSG_HCD_CLR_STALL, member, (uint16_t*) 0, complete);
        break;

            /* USB_MSG_HCD_SQTGLBIT */
        case USB_DO_SET_SQTGL :
            err = usb_hstd_hcd_snd_mbx(USB_MSG_HCD_SETSEQBIT, member, (uint16_t*) 0, complete);
        break;

            /* USB_MSG_HCD_CLRSEQBIT */
        case USB_DO_CLR_SQTGL :
            err = usb_hstd_hcd_snd_mbx(USB_MSG_HCD_CLRSEQBIT, member, (uint16_t*) 0, complete);
        break;

        default :

            /* Get memory pool blk */
            ret = rtos_get_fixed_memory(&g_rtos_usb_mpf_id, (void **)&p_blf, RTOS_ZERO);
            if (RTOS_SUCCESS == ret)
            {
                rtos_get_task_id(&task_id);

                hp = (usb_utr_t *) p_blf;
                hp->msghead = (usb_mh_t) USB_NULL;
                hp->msginfo = msginfo;
                hp->keyword = member;
                hp->complete = complete;

                g_usb_hstd_change_device_state_task_id = task_id;

                /* Send message */
                err = rtos_send_mailbox (&g_rtos_usb_mgr_mbx_id, (void *)p_blf); 
                if (RTOS_SUCCESS != err)
                {
                    rtos_release_fixed_memory(&g_rtos_usb_mpf_id, (void *)p_blf);
                    return USB_ERR_NG;
                }
            }
            else
            {
                /* Error */
                /* WAIT_LOOP */
                while (1)
                {
                    /* error */
                }
            }
        break;
    }
    return err;
}
/******************************************************************************
 End of function usb_hstd_change_device_state
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_hcd_open
 Description     : Start HCD(Host Control Driver) task
 Arguments       : none
 Return          : usb_err_t            : USB_OK etc
 ******************************************************************************/
usb_err_t usb_hstd_hcd_open (void)
{
    uint16_t i;

    memset((void *)&g_usb_hstd_device_drv, 0, sizeof(usb_hcdreg_t));
    g_usb_hstd_dcp_register = 0;

    /* WAIT_LOOP */
    for (i =0; i< (USB_MAX_PIPE_NO + 1); i++)
    {
        g_usb_hstd_data_cnt[i] = 0;
        gp_usb_hstd_data_ptr[i] = 0;
        g_usb_hstd_ignore_cnt[i] = 0;
    }

    /* Control transfer stage management */
    g_usb_hstd_ctsq = USB_IDLEST;

    g_usb_hcd_remote_wakeup = USB_NO;

    /* WAIT_LOOP */
    for (i = USB_PIPE0; i <= USB_MAX_PIPE_NO; i++)
    {
        gp_usb_hstd_pipe[i] = (usb_utr_t*) USB_NULL;
    }

#if USB_CFG_BC == USB_CFG_ENABLE
    g_usb_hstd_bc.state = USB_BC_STATE_INIT;
#endif  /* USB_CFG_BC == USB_CFG_ENABLE */

    return USB_SUCCESS;
}
/******************************************************************************
 End of function usb_hstd_hcd_open
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_transfer_start
 Description     : Send a request for data transfer to HCD (Host Control Driver) 
                 : using the specified pipe.
 Arguments       : usb_utr_t *ptr : Pointer to usb_utr_t structure.
 Return          : usb_er_t       : USB_OK/USB_QOVR/USB_ERROR
 ******************************************************************************/
usb_er_t usb_hstd_transfer_start (usb_utr_t *ptr)
{
    usb_er_t        err;

    /* Check enumeration  */
    if (USB_PIPE0 == ptr->keyword)
    {
        if (USB_DEFAULT == g_usb_hstd_mgr_mode)
        {
            return USB_QOVR;
        }
    }

    err = usb_hstd_transfer_start_req( ptr );

    return err;
}
/******************************************************************************
 End of function usb_hstd_transfer_start
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_connect_err_event_set
 Description     : Set event for "USB_STS_NOT_SUPPORT"
 Arguments       : none
 Return          : none
 ******************************************************************************/
void usb_hstd_connect_err_event_set(void)
{
    usb_ctrl_t ctrl;

#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
    ctrl.p_data =  (void *)g_usb_default_apl_task_id;
#endif /* (BSP_CFG_RTOS_USED != 0) */
    usb_cstd_set_event(USB_STS_NOT_SUPPORT, &ctrl);          /* set event()  */
}
/******************************************************************************
 End of function usb_hstd_connect_err_event_set
 ******************************************************************************/

#if defined(USB_CFG_HVND_USE)
/******************************************************************************
 Function Name   : usb_hvnd_registration
 Description     : Host vendor class registration.
 Argument        : none
 Return          : none
 ******************************************************************************/
void usb_hvnd_registration (void)
{
    usb_hcdreg_t driver;

    /* Driver registration */

    /* Interface Class */
    driver.ifclass = (uint16_t) USB_IFCLS_VEN;

    /* Target peripheral list */
    driver.p_tpl = (uint16_t*) &gs_usb_apl_devicetpl;

    /* Driver init */
    driver.classinit = &usb_hstd_dummy_function;

    /* Driver check */
    driver.classcheck = &usb_hvnd_enumeration;

    /* Device configured */
    driver.devconfig = &usb_hvnd_configured;

    /* Device detach */
    driver.devdetach = &usb_hvnd_detach;

    /* Device suspend */
    driver.devsuspend = &usb_hstd_dummy_function;

    /* Device resume */
    driver.devresume = &usb_hstd_dummy_function;

    usb_hstd_driver_registration(&driver);
}
/******************************************************************************
 End of function usb_hvnd_registration
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hvnd_pipe_info
 Description     : Host pipe information check. Set EP table.
 Arguments       : uint8_t *p_table      : Descriptor start address.
                 : uint16_t length       : Configuration Descriptor Length
 Return value    : none
 ******************************************************************************/
void usb_hvnd_pipe_info (uint8_t *p_table, uint16_t length)
{
    uint16_t ofdsc;

    /* Check Endpoint Descriptor */
    ofdsc = p_table[0];

    /* WAIT_LOOP */
    while (ofdsc < length)
    {
        /* Search within Interface */
        if (USB_DT_ENDPOINT == p_table[ofdsc + 1])
        {
            /* Endpoint Descriptor */
            usb_cstd_pipe_table_set (USB_HVND, &p_table[ofdsc]);
        }
        ofdsc += p_table[ofdsc];
    }
}
/******************************************************************************
 End of function usb_hvnd_pipe_info
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hvnd_enumeration
 Description     : Vendor class example enumeration execution by host.
 Argument        : uint16_t **pp_table  : Pointer to data table.
 Return          : none
 ******************************************************************************/
void usb_hvnd_enumeration (uint16_t **table)
{
    uint8_t *pdesc;
    uint16_t total_len;

    *table[3] = USB_OK;

    /* Pipe Information table set */
    pdesc = (uint8_t *) g_usb_hstd_config_descriptor;

    total_len = ((uint16_t) *(pdesc + 3)) << 8;
    total_len |= (uint16_t) *(pdesc + 2);

    /* Pipe Information table set */
    usb_hvnd_pipe_info(pdesc, total_len);
}
/******************************************************************************
 End of function usb_hvnd_enumeration
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hvnd_configured
 Description     : Host vendor application initialization.
 Argument        : uint16_t dev_addr : Device address.
                 : uint16_t data2    : Not used.
 Return          : none
 ******************************************************************************/
void usb_hvnd_configured (usb_utr_t *ptr, uint16_t dev_addr, uint16_t data2)
{
    usb_ctrl_t ctrl;

    ctrl.type = USB_HVND;
    usb_cstd_pipe_reg_set();
#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
    ctrl.p_data = (void *)g_usb_default_apl_task_id;
#endif /* (BSP_CFG_RTOS_USED != 0) */
    usb_cstd_set_event(USB_STS_CONFIGURED, &ctrl);
}
/******************************************************************************
 End of function usb_hvnd_configured
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hvnd_detach
 Description     : Close host vendor application.
 Argument        : uint16_t data1            : Not Use
                 : uint16_t data2            : Not Use
 Return          : none
 ******************************************************************************/
void usb_hvnd_detach (usb_utr_t *ptr, uint16_t dev_addr, uint16_t data2)
{
    usb_ctrl_t ctrl;

    usb_cstd_pipe_reg_clear ();
    usb_cstd_pipe_table_clear ();

#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
    ctrl.p_data = (void *)g_usb_default_apl_task_id;
#endif /* (BSP_CFG_RTOS_USED != 0) */
    usb_cstd_set_event(USB_STS_DETACH, &ctrl);
}
/******************************************************************************
 End of function usb_hvnd_detach
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hvnd_read_complete
 Description     : Notify application task of data reception completion.
 : (Callback function at completion of INT data reception.)
 Arguments       :
 Return value    : none
 ******************************************************************************/
void usb_hvnd_read_complete (usb_utr_t *ptr, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;

    ctrl.pipe = ptr->keyword;   /* Pipe number setting */
    ctrl.type = USB_HVND;       /* Vendor class  */

    ctrl.size = ptr->read_req_len - ptr->tranlen;
    ctrl.p_data = (void *)ptr->task_id;

    switch (ptr->status)
    {
        case USB_DATA_OK :
            ctrl.status = USB_SUCCESS;
        break;
        case USB_DATA_SHT :
            ctrl.status = USB_ERR_SHORT;
        break;
        case USB_DATA_OVR :
            ctrl.status = USB_ERR_OVER;
            ctrl.size = 0;
        break;
        case USB_DATA_ERR :
        default :
            ctrl.status = USB_ERR_NG;
            ctrl.size = 0;
        break;
    }

    usb_cstd_set_event(USB_STS_READ_COMPLETE, &ctrl); /* Set Event()  */
}
/******************************************************************************
 End of function usb_hvnd_read_complete()
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hvnd_write_complete
 Description     : Notify application task of data transmission completion.
 : (Callback function at completion of INT data reception.)
 Arguments       :
 Return value    : none
 ******************************************************************************/
void usb_hvnd_write_complete (usb_utr_t *ptr, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;

    ctrl.pipe = ptr->keyword;   /* Pipe number setting */
    ctrl.type = USB_HVND;       /* Vendor class  */
    ctrl.p_data = (void *)ptr->task_id;
    ctrl.size = 0;

    if (USB_DATA_NONE == ptr->status)
    {
        ctrl.status = USB_SUCCESS;
    }
    else
    {
        ctrl.status = USB_ERR_NG;
    }

    usb_cstd_set_event(USB_STS_WRITE_COMPLETE, &ctrl); /* Set Event()  */
}
/******************************************************************************
 End of function usb_hvnd_write_complete()
 ******************************************************************************/

#endif /* defined(USB_CFG_HVND_USE) */

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
#endif /*(BSP_CFG_RTOS_USED != 0)*/
/******************************************************************************
 End  Of File
 ******************************************************************************/

