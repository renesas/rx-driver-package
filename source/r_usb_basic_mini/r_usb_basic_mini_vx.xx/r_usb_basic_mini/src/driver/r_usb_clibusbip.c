/**********************************************************************************************************************
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
 * Copyright (C) 2015(2020) Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_usb_clibusbip.c
 * Description  : USB IP Host and Peripheral low level library
 *********************************************************************************************************************/
/**********************************************************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 01.09.2014 1.00    First Release
*           : 01.06.2015 1.01    Added RX231.
*           : 29.12.2015 1.02    Minor Update.
*           : 30.11.2018 1.10    Supporting Smart Configurator
*           : 30.06.2020 1.20    Added support for RTOS.
 ***********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_usb_basic_mini_if.h"
#include "r_usb_bitdefine.h"
#include "r_usb_typedef.h"
#include "r_usb_reg_access.h"
#include "r_usb_extern.h"
#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
#include "r_usb_cstd_rtos.h"
#endif /* (BSP_CFG_RTOS_USED != 0) */


/******************************************************************************
 Macro definitions
 *****************************************************************************/

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 *****************************************************************************/
#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
static uint16_t     g_rtos_msg_pipe[USB_MAXPIPE_NUM + 1];

#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
static uint16_t     g_rtos_msg_count_pcd_sub            = 0;
#endif /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
static uint16_t     g_rtos_msg_count_hcd_sub            = 0;
#endif /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
#endif /* BSP_CFG_RTOS_USED != 0 */

/******************************************************************************
 Function Name   : usb_cstd_nrdy_enable
 Description     : Enable NRDY interrupt of the specified pipe.
 Arguments       : uint16_t pipe  : Pipe number.
 Return value    : none
 ******************************************************************************/
void usb_cstd_nrdy_enable (uint16_t pipe)
{
    if (USB_MAX_PIPE_NO < pipe)
    {
        return; /* Error */
    }

    /* Enable NRDY */
    hw_usb_set_nrdyenb(pipe);
}
/******************************************************************************
 End of function usb_cstd_nrdy_enable
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_cstd_get_pid
 Description     : Fetch specified pipe's PID.
 Arguments       : uint16_t pipe  : Pipe number.
 Return value    : uint16_t PID-bit status
 ******************************************************************************/
uint16_t usb_cstd_get_pid (uint16_t pipe)
{
    uint16_t buf;

    if (USB_MAX_PIPE_NO < pipe)
    {
        return USB_NULL; /* Error */
    }

    /* PIPE control reg read */
    buf = hw_usb_read_pipectr(pipe);
    return (uint16_t) (buf & USB_PID);
}
/******************************************************************************
 End of function usb_cstd_get_pid
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_cstd_get_pipe_dir
 Description     : Get PIPE DIR
 Arguments       : uint16_t pipe  : Pipe number.
 Return value    : uint16_t pipe direction.
 ******************************************************************************/
uint16_t usb_cstd_get_pipe_dir (uint16_t pipe)
{
    uint16_t buffer;

    if (USB_MAX_PIPE_NO < pipe)
    {
        return USB_NULL; /* Error */
    }

    /* Pipe select */
    hw_usb_write_pipesel(pipe);

    /* Read Pipe direction */
    buffer = hw_usb_read_pipecfg();
    return (uint16_t) (buffer & USB_DIRFIELD);
}
/******************************************************************************
 End of function usb_cstd_get_pipe_dir
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_cstd_get_pipe_type
 Description     : Fetch and return PIPE TYPE.
 Arguments       : uint16_t pipe  : Pipe number.
 Return value    : uint16_t pipe type
 ******************************************************************************/
uint16_t usb_cstd_get_pipe_type (uint16_t pipe)
{
    uint16_t buffer;

    if (USB_MAX_PIPE_NO < pipe)
    {
        return USB_NULL; /* Error */
    }

    /* Pipe select */
    hw_usb_write_pipesel(pipe);

    /* Read Pipe direction */
    buffer = hw_usb_read_pipecfg();
    return (uint16_t) (buffer & USB_TYPFIELD);
}
/******************************************************************************
 End of function usb_cstd_get_pipe_type
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_cstd_do_aclrm
 Description     : Set the ACLRM-bit (Auto Buffer Clear Mode) of the specified 
                 : pipe.
 Arguments       : uint16_t pipe  : Pipe number.
 Return value    : none
 ******************************************************************************/
void usb_cstd_do_aclrm (uint16_t pipe)
{
    if (USB_MAX_PIPE_NO < pipe)
    {
        return; /* Error */
    }

    /* Control ACLRM */
    hw_usb_set_aclrm(pipe);
    hw_usb_clear_aclrm(pipe);
}
/******************************************************************************
 End of function usb_cstd_do_aclrm
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_cstd_set_buf
 Description     : Set PID (packet ID) of the specified pipe to BUF.
 Arguments       : uint16_t pipe  : Pipe number.
 Return value    : none
 ******************************************************************************/
void usb_cstd_set_buf (uint16_t pipe)
{
    if (USB_MAX_PIPE_NO < pipe)
    {
        return; /* Error */
    }

    /* PIPE control reg set */
    hw_usb_set_pid(pipe, USB_PID_BUF);
}
/******************************************************************************
 End of function usb_cstd_set_buf
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_cstd_clr_stall
 Description     : Set up to NAK the specified pipe, and clear the STALL-bit set
                 : to the PID of the specified pipe.
 Arguments       : uint16_t pipe  : Pipe number.
 Return value    : none
 Note            : PID is set to NAK.
 ******************************************************************************/
void usb_cstd_clr_stall (uint16_t pipe)
{
    if (USB_MAX_PIPE_NO < pipe)
    {
        return; /* Error */
    }

    /* Set NAK */
    usb_cstd_set_nak(pipe);

    /* Clear STALL */
    hw_usb_clear_pid(pipe, USB_PID_STALL);
}
/******************************************************************************
 End of function usb_cstd_clr_stall
 ******************************************************************************/
 
/******************************************************************************
 Function Name   : usb_cstd_port_speed
 Description     : Get USB-speed of the specified port.
 Arguments       : none
 Return value    : uint16_t  : FSCONNECT : Full-Speed
                 :           : LSCONNECT : Low-Speed
                 :           : NOCONNECT : not connect
******************************************************************************/
uint16_t usb_cstd_port_speed(void)
{
    uint16_t    buf;
    uint16_t conn_inf = USB_NOCONNECT;

    /* Reset handshake status get */
    buf = hw_usb_read_dvstctr();
    buf = (uint16_t) (buf & USB_RHST);

    /* Check RHST */
    if (USB_FSMODE == buf)
    {
        conn_inf = USB_FSCONNECT;
    }
    if (USB_LSMODE == buf)
    {
        conn_inf = USB_LSCONNECT;
    }

    return  (conn_inf);
}
/******************************************************************************
 End of function usb_cstd_port_speed
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_cstd_set_event
 Description     : Set event.
 Arguments       : usb_status_t event  : event code.
                 : usb_ctrl_t *p_ctrl  :control structure for USB API.
 Return value    : none
 ******************************************************************************/
void usb_cstd_set_event (usb_status_t event, usb_ctrl_t *p_ctrl)
{
#if (BSP_CFG_RTOS_USED != 0)    /* RTOS */
    static uint16_t     count = 0;

    p_ctrl->event = event;
    g_usb_cstd_event[count] = *p_ctrl;

    switch (event)
    {
        case    USB_STS_DEFAULT :
            (*g_usb_apl_callback)(&g_usb_cstd_event[count], (rtos_task_id_t)p_ctrl->p_data, USB_OFF);
        break;
        case    USB_STS_CONFIGURED :
            (*g_usb_apl_callback)(&g_usb_cstd_event[count], (rtos_task_id_t)p_ctrl->p_data, USB_OFF);
        break;
        case    USB_STS_BC :
        case    USB_STS_OVERCURRENT :
        case    USB_STS_NOT_SUPPORT :
            (*g_usb_apl_callback)(&g_usb_cstd_event[count], (rtos_task_id_t)p_ctrl->p_data, USB_OFF);
        break;
        case    USB_STS_DETACH :
            (*g_usb_apl_callback)(&g_usb_cstd_event[count], (rtos_task_id_t)p_ctrl->p_data, USB_OFF);
        break;
 
        case    USB_STS_REQUEST :
            (*g_usb_apl_callback)(&g_usb_cstd_event[count], (rtos_task_id_t)p_ctrl->p_data, USB_ON);
        break;

        case    USB_STS_SUSPEND :
        case    USB_STS_RESUME :
            if (USB_HOST == g_usb_cstd_usb_mode)
            {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
                (*g_usb_apl_callback)(&g_usb_cstd_event[count], (rtos_task_id_t)p_ctrl->p_data, USB_OFF);
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
            }
            else
            {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
                (*g_usb_apl_callback)(&g_usb_cstd_event[count], (rtos_task_id_t)USB_NULL, USB_OFF);
#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */
            }
        break;

        case    USB_STS_REQUEST_COMPLETE :
            if (USB_HOST == g_usb_cstd_usb_mode)
            {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
                (*g_usb_apl_callback)(&g_usb_cstd_event[count], (rtos_task_id_t)p_ctrl->p_data, USB_OFF);
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
            }
            else
            {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
                /* Processing for USB request has the no data stage */
                (*g_usb_apl_callback)(&g_usb_cstd_event[count], (rtos_task_id_t)USB_NULL, USB_OFF);
#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */
            }
        break;

        case    USB_STS_READ_COMPLETE :
        case    USB_STS_WRITE_COMPLETE :
#if defined(USB_CFG_HMSC_USE)       
        case    USB_STS_MSC_CMD_COMPLETE:
#endif /* defined(USB_CFG_HMSC_USE) */
            (*g_usb_apl_callback)(&g_usb_cstd_event[count], (rtos_task_id_t)p_ctrl->p_data, USB_OFF);
        break;

        default :
            /* Do Nothing */
        break;
    }
    count = ((count + 1) % USB_EVENT_MAX);
#else /* (BSP_CFG_RTOS_USED != 0) */
    g_usb_cstd_event.code[g_usb_cstd_event.write_pointer] = event;
    g_usb_cstd_event.ctrl[g_usb_cstd_event.write_pointer] = *p_ctrl;
    g_usb_cstd_event.write_pointer++;
    if (g_usb_cstd_event.write_pointer >= USB_EVENT_MAX)
    {
        g_usb_cstd_event.write_pointer = 0;
    }
#endif /*(BSP_CFG_RTOS_USED != 0)*/
} /* End of function usb_cstd_set_event() */

#if (BSP_CFG_RTOS_USED == 0)    /* Non-OS */
/******************************************************************************
 Function Name   : usb_cstd_usb_task
 Description     : USB driver main loop processing.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_cstd_usb_task (void)
{
    if (USB_HOST == g_usb_cstd_usb_mode)
    {
#if (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST

    if (USB_FLGSET == usb_hstd_scheduler() )      /* Check message for task */
    {
        usb_hstd_hcd_task();                /* USB Host driver task */
        usb_hstd_mgr_task();                /* USB Manager task */
        usb_hstd_class_task();

#if defined(USB_CFG_HMSC_USE)
        usb_hmsc_strg_drive_task();         /* HSTRG Task */

#endif /* defined(USB_CFG_HMSC_USE) */
    }

#endif /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
    }
    else
    {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
        usb_pstd_pcd_task();
#if defined(USB_CFG_PMSC_USE)
        usb_pmsc_task();
#endif /* defined(USB_CFG_PMSC_USE) */
#endif /*( (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI )*/
    }
} /* End of function usb_cstd_usb_task() */
#endif /*(BSP_CFG_RTOS_USED == 0)*/


#if (BSP_CFG_RTOS_USED != 0)         /* Use RTOS */
/******************************************************************************
 Function Name   : usb_rtos_delete_msg_submbx
 Description     : Message clear for PIPE Transfer wait que.
 Arguments       : usb_utr_t *ptr       : Pointer to usb_utr_t structure.
                 : uint16_t  pipe_no    : Pipe no.
 Return          : none
 ******************************************************************************/
void usb_rtos_delete_msg_submbx (usb_utr_t *p_ptr, uint16_t usb_mode)
{
    usb_utr_t   *mess;
    uint16_t    i;
    uint16_t    pipe;

    pipe = p_ptr->keyword;

    if (0 == g_rtos_msg_pipe[pipe])
    {
        return;
    }

    if (USB_HOST == usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
        /* WAIT_LOOP */
        for (i = 0; i != g_rtos_msg_count_hcd_sub; i++)
        {
            rtos_receive_mailbox(&g_rtos_usb_hcd_sub_mbx_id, (void **)&mess, RTOS_ZERO);
            if (pipe == mess->keyword)
            {
            	rtos_release_fixed_memory (&g_rtos_usb_mpf_id, (void *)mess);
            }
            else
            {
                rtos_send_mailbox (&g_rtos_usb_hcd_sub_mbx_id, (void *)mess);
            }
        }
        g_rtos_msg_count_hcd_sub -= g_rtos_msg_pipe[pipe];
#endif /* ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST) */
    }
    else
    {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
        /* WAIT_LOOP */
        for (i = 0; i != g_rtos_msg_count_pcd_sub; i++)
        {
            rtos_receive_mailbox (&g_rtos_usb_pcd_sub_mbx_id, (void **)&mess, RTOS_ZERO);
            if (pipe == mess->keyword)
            {
                rtos_release_fixed_memory (&g_rtos_usb_mpf_id, (void *)mess);
            }
            else
            {
                rtos_send_mailbox (&g_rtos_usb_pcd_sub_mbx_id, (void *)mess);
            }
        }
        g_rtos_msg_count_pcd_sub -= g_rtos_msg_pipe[pipe];
#endif /* ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST) */
    }
    g_rtos_msg_pipe[pipe] = 0;
}
/******************************************************************************
 End of function usb_rtos_delete_msg_submbx
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_rtos_resend_msg_to_submbx
 Description     : Get PIPE Transfer wait que and Message send to HCD/PCD
 Argument        : uint16_t  pipe_no    : Pipe no.
 Return          : none
 ******************************************************************************/
void usb_rtos_resend_msg_to_submbx (uint16_t pipe, uint16_t usb_mode)
{
    usb_utr_t   *mess;

    if (USB_MAXPIPE_NUM < pipe)
    {
        return;
    }

    if (0 == g_rtos_msg_pipe[pipe])
    {
        return;
    }

    if (USB_HOST == usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
        /* WAIT_LOOP */
        while(1)
        {
            rtos_receive_mailbox (&g_rtos_usb_hcd_sub_mbx_id, (void **)&mess, RTOS_ZERO);
            if (mess->keyword == pipe)
            {
                g_rtos_msg_pipe[pipe]--;
                g_rtos_msg_count_hcd_sub--;
                rtos_send_mailbox (&g_rtos_usb_hcd_mbx_id,(void *)mess);
                break;
            }
            else
            {
                rtos_send_mailbox (&g_rtos_usb_hcd_sub_mbx_id, (void *)mess);
            }
        }
#endif /* ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST) */
    }
    else
    {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
        /* WAIT_LOOP */
        while(1)
        {
            rtos_receive_mailbox (&g_rtos_usb_pcd_sub_mbx_id, (void **)&mess, RTOS_ZERO);
            if (mess->keyword == pipe)
            {
                g_rtos_msg_pipe[pipe]--;
                g_rtos_msg_count_pcd_sub--;
                rtos_send_mailbox (&g_rtos_usb_pcd_mbx_id,(void *)mess);
                break;
            }
            else
            {
                rtos_send_mailbox (&g_rtos_usb_pcd_sub_mbx_id, (void *)mess);
            }
        }
#endif /* ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI) */
    }
}
/******************************************************************************
 End of function usb_rtos_resend_msg_to_submbx
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_rtos_send_msg_to_submbx
 Description     : Message foward to PIPE Transfer wait que.
 Arguments       : usb_utr_t *ptr       : Pointer to usb_utr_t structure.
                 : uint16_t  pipe_no    : Pipe no.
 Return          : none
 ******************************************************************************/
void usb_rtos_send_msg_to_submbx (usb_utr_t *p_ptr, uint16_t pipe_no, uint16_t usb_mode)
{
    if (USB_MAXPIPE_NUM < pipe_no)
    {
        return;
    }

    g_rtos_msg_pipe[pipe_no]++;
    if (USB_HOST == usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
        g_rtos_msg_count_hcd_sub++;
        rtos_send_mailbox (&g_rtos_usb_hcd_sub_mbx_id, (void *)p_ptr);
#endif  /* ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST) */
    }
    else
    {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
        g_rtos_msg_count_pcd_sub++;
        rtos_send_mailbox (&g_rtos_usb_pcd_sub_mbx_id, (void *)p_ptr);
#endif /* ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI) */
    }
}
/******************************************************************************
 End of function usb_rtos_send_msg_to_submbx
 ******************************************************************************/
#endif /*(BSP_CFG_RTOS_USED != 0)*/

/******************************************************************************
 End  Of File
 ******************************************************************************/
