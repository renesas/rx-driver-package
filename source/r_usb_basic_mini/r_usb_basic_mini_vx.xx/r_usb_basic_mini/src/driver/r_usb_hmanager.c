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
 * Copyright (C) 2014(2020) Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_usb_hmanager.c
* Description  : This is the USB host control driver code.
 ***********************************************************************************************************************/
/**********************************************************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 01.09.2014 1.00    First Release
*           : 01.06.2015 1.01    Added RX231.
*           : 30.11.2018 1.10    Supporting Smart Configurator
*           : 31.05.2019 1.11    Added support for GNUC and ICCRX.
*           : 30.06.2019 1.12    RX23W is added.
*           : 31.05.2019 1.11    Added support for GNUC and ICCRX.
 ***********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_usb_basic_mini_if.h"

#if (BSP_CFG_RTOS_USED == 0)    /* Non-OS */

#include "r_usb_bitdefine.h"
#include "r_usb_typedef.h"
#include "r_usb_reg_access.h"            /* Definition of the USB register access macro */
#include "r_usb_extern.h"


#if defined(USB_CFG_HMSC_USE)
#include "r_usb_hmsc_mini_if.h"
#endif /* defined(USB_CFG_HMSC_USE) */

#if defined(USB_CFG_HHID_USE)
#include "r_usb_hhid_mini_if.h"
#endif /* defined(USB_CFG_HHID_USE) */

#if (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST
/*******************************************************************************
 Macro definitions
 ******************************************************************************/
#define USB_PET_VID     (0x1a0a)    /* USB-PET Vendor ID  */
#define USB_PET_PID     (0x0200)    /* USB-PET Product ID (Use Embedded Host Test) */

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/
USB_STATIC  usb_cbinfo_t    gs_usb_mgr_callback;                  /* API callback function */


/* Host manager sequnce function table */
USB_STATIC  const uint8_t   gs_usb_hstd_seq_tbl0[20] =
{
/* When the sequence processing starts newly by the sequence unprocessing now. */
/*  msginfo CONTINUE    DETACH      ATTACH      OVERCU      REMOTE      SUSPEN */
            USB_NOSEQE0,USB_DETACH0,USB_ATTACH0,USB_OVERCU0,USB_REMOTE0,USB_SUSPEN0,
/*          RESUME      PORT_E      PORT_D      CLRSTL */
            USB_RESUME0,USB_PORT_E0,USB_PORT_D0,USB_CLRSTL0,
/*Global suspend(Select port) */
            USB_SUSPEN2,USB_RESUME2,
};


USB_STATIC  const uint8_t   gs_usb_hstd_seq_tbl1[11] =
{
/* When the sequence processing is continuas. */
/*  process CONTINUE    Detach      Attach      OverCurrent Remote      Suspend */
            USB_NOSEQE0,USB_NOSEQE0,USB_ENUMER1,USB_NOSEQE0,USB_REMOTE1,USB_SUSPEN1,
/*          Resume      PortEnable  PortDisable StallClear */
            USB_RESUME1,USB_PORT_E1,USB_PORT_D1,USB_CLRSTL1,
};


USB_STATIC  const uint8_t   gs_usb_hstd_seq_tbl2[11] =
{
/* When the detach request is generated while processing the sequence. */
/*  process CONTINUE    Detach      Attach      Current     Remote      Suspend */
            USB_NOSEQE0,USB_NOSEQE1,USB_DETACH2,USB_DETACH1,USB_DETACH1,USB_DETACH1,
/*          Resume      PortEnable  PortDisable StallClear */
            USB_DETACH1,USB_DETACH1,USB_NOSEQE0,USB_DETACH1,
};

/* functions */
/* HCD task / MGR task */
USB_STATIC  void                usb_hstd_no_sequence(void);
USB_STATIC  void                usb_hstd_wait_sequence(void);
USB_STATIC  void                usb_hstd_attach_start(void);
USB_STATIC  void                usb_hstd_enumeration_sequence(void);
USB_STATIC  void                usb_hstd_global_suspend(void);
USB_STATIC  void                usb_hstd_suspend_start(void);
USB_STATIC  void                usb_hstd_suspend_sequence(void);
USB_STATIC  void                usb_hstd_global_resume(void);
USB_STATIC  void                usb_hstd_resume_start(void);
USB_STATIC  void                usb_hstd_resume_sequence(void);
USB_STATIC  void                usb_hstd_remote_start(void);
USB_STATIC  void                usb_hstd_remote_sequence(void);
USB_STATIC  void                usb_hstd_detach_start(void);
USB_STATIC  void                usb_hstd_enumeration_detach(void);
USB_STATIC  void                usb_hstd_overcurrent_start(void);
USB_STATIC  void                usb_hstd_portenable_start(void);
USB_STATIC  void                usb_hstd_portenable_sequence(void);
USB_STATIC  void                usb_hstd_portdisable_start(void);
USB_STATIC  void                usb_hstd_portdisable_sequence(void);
USB_STATIC  void                usb_hstd_clearstall_start(void);
USB_STATIC  void                usb_hstd_clearstall_sequence(void);
USB_STATIC  void                usb_hstd_mgr_wait(uint16_t time);
USB_STATIC  void                usb_hstd_sequence_clear(void);
USB_STATIC  void                usb_hstd_sequence_end(void);
USB_STATIC  uint8_t             usb_hstd_sequence_port(void);
USB_STATIC  uint8_t             usb_hstd_sequence_addr(void);
USB_STATIC  uint8_t             usb_hstd_sequence_pipe(void);
USB_STATIC  uint8_t             usb_hstd_enumeration(void);

/* Processing function table */
USB_STATIC  const mgrfunc_t gs_usb_mgr_func_tbl[] =
{
    usb_hstd_no_sequence,               /* No sequence processing. */
    usb_hstd_wait_sequence,             /* The message is preserved in the wait queue table. */
    usb_hstd_attach_start,              /* Start of attach sequence */
    usb_hstd_enumeration_sequence,      /* Enumeration sequence */
    usb_hstd_global_suspend,            /* Start of global suspend(select port) */
    usb_hstd_suspend_start,             /* Start of selective suspend(select device) */
    usb_hstd_suspend_sequence,          /* Suspend sequence */
    usb_hstd_global_resume,             /* Start of global resume(select port) */
    usb_hstd_resume_start,              /* Start of selective resume(select device) */
    usb_hstd_resume_sequence,           /* Resume sequence */
    usb_hstd_remote_start,              /* Start of remote wakeup sequence */
    usb_hstd_remote_sequence,           /* Remote wakeup sequence */
    usb_hstd_detach_start,              /* Start of detach processing */
    usb_hstd_detach_start,              /* Start of detach processing in sequence continuance */
    usb_hstd_enumeration_detach,        /* Start of detach processing in enumeration sequence */
    usb_hstd_overcurrent_start,         /* Start of over current processing */
    usb_hstd_portenable_start,          /* Start of port enable sequence */
    usb_hstd_portenable_sequence,       /* Port enable sequence */
    usb_hstd_portdisable_start,         /* Start of port disable sequence */
    usb_hstd_portdisable_sequence,      /* Port disable sequence */
    usb_hstd_clearstall_start,          /* Start of STALL clear sequence */
    usb_hstd_clearstall_sequence,       /* STALL clear sequence */
};

#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
USB_STATIC uint8_t usb_hstd_get_ifclass( uint8_t class );
#endif

/*******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

/* variables */
/* Host manager variables */
usb_tskinfo_t       *gp_usb_mgr_msg;                               /* Mgr Task receive message */
usb_hutr_t          g_usb_mgr_ctrl_msg;
usb_port_t          g_usb_mgr_port;

/* Host manager variables */
usb_addr_t          g_usb_mgr_devaddr;                             /* Device address */
uint8_t             g_usb_mgr_seq;
uint8_t             g_usb_mgr_seq_mode;
usb_pipe_no_t       g_usb_mgr_pipe;                                /* Pipe number */



/******************************************************************************
Function Name   : usb_hstd_mgr_task
Description     : Manager Task
Argument        : none
Return          : none
******************************************************************************/
void usb_hstd_mgr_task(void)
{
    usb_tskinfo_t   *p_mess;
    uint8_t         fncno;

    /* Receive message */
    if (R_USB_RCV_MSG(USB_MGR_MBX, (usb_msg_t**)&p_mess) != USB_OK)
    {
        return;
    }

    gp_usb_mgr_msg    = (usb_tskinfo_t*)p_mess;
    if ((gp_usb_mgr_msg->msginfo < USB_MGR_CONTINUE) || (gp_usb_mgr_msg->msginfo > USB_MGR_NOSEQUENCE))
    {
        R_USB_REL_BLK(gp_usb_mgr_msg->flag);
        return;
    }

    if (USB_MGR_NOSEQUENCE == g_usb_mgr_seq_mode)
    {
        /* When the sequence processing starts newly by the sequence unprocessing now. */
        fncno = gs_usb_hstd_seq_tbl0[(gp_usb_mgr_msg->msginfo - USB_MGR_CONTINUE)];
    }
    else if (USB_RTP_DETACH == gp_usb_mgr_msg->msginfo)
    {
        /* When the detach request is generated while processing the sequence. */
        if (g_usb_mgr_port == gp_usb_mgr_msg->keyword)
        {
            fncno = gs_usb_hstd_seq_tbl2[g_usb_mgr_seq_mode];
        }
        else
        {
            fncno = USB_NOSEQE1;
        }
    }
    else if (USB_MGR_CONTINUE == gp_usb_mgr_msg->msginfo)
    {
        /* When the sequence processing is continuas. */
        fncno = gs_usb_hstd_seq_tbl1[g_usb_mgr_seq_mode];
    }
    else
    {
        /* Because the sequence is being processed, the request is reserved. */
        fncno = USB_NOSEQE1;
    }

    /* Sequence processing */
    (*gs_usb_mgr_func_tbl[fncno])();

}   /* End of function usb_hstd_mgr_task() */


/******************************************************************************
Function Name   : usb_hstd_no_sequence
Description     : No sequence processing
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_no_sequence(void)
{
    R_USB_REL_BLK(gp_usb_mgr_msg->flag);
}   /* End of function usb_hstd_no_sequence() */


/******************************************************************************
Function Name   : usb_hstd_wait_sequence
Description     : The message is preserved in the wait queue table
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_wait_sequence(void)
{
    usb_hstd_mgr_wait(50);
}   /* End of function usb_hstd_wait_sequence() */


/******************************************************************************
    Attach sequence functions
******************************************************************************/
/******************************************************************************
Function Name   : usb_hstd_attach_start
Description     : Start of attach sequence
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_attach_start(void)
{
    usb_hcdreg_t    *p_driver;

    usb_hstd_sequence_port();
    /* The callback is done to all registered drivers.  */
    p_driver  = &g_usb_hcd_driver;
    if (USB_NODEVICE == p_driver->devaddr)
    {
        g_usb_hcd_dev_info.state    = USB_STS_ATTACH;
#if USB_CFG_BC == USB_CFG_ENABLE
        (*p_driver->statediagram)( g_usb_hstd_bc.state, USB_STS_ATTACH);
#else /* USB_CFG_BC == USB_CFG_ENABLE */
        (*p_driver->statediagram)(g_usb_mgr_port, USB_STS_ATTACH);
#endif /* USB_CFG_BC == USB_CFG_ENABLE */
    }
    usb_hstd_mgr_wait((uint16_t)USB_TATTDB);
    return;
}   /* End of function usb_hstd_attach_start() */


/******************************************************************************
    Enumeration sequence functions
******************************************************************************/
/******************************************************************************
Function Name   : usb_hstd_enumeration_sequence
Description     : Enumeration sequence
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_enumeration_sequence(void)
{
    usb_hcdreg_t    *p_driver;
    uint8_t         enume_mode;

    switch (g_usb_mgr_seq)
    {
        case    0:
            g_usb_hcd_dev_info.state    = USB_STS_DEFAULT;
            if ((USB_DEVICEADDR == g_usb_mgr_devaddr) || ((USB_DEVICEADDR+1) == g_usb_mgr_devaddr))
            {
                usb_hstd_status_change(USB_HCD_USBRESET, (usb_strct_t)USB_NULL);
            }
        break;
        case    USB_1:
            /* The callback is done to all registered drivers.  */
            p_driver  = &g_usb_hcd_driver;
            (*p_driver->statediagram)(g_usb_mgr_port, USB_STS_DEFAULT);
            g_usb_hstd_hcd_port         = g_usb_mgr_port;
            g_usb_hstd_devspd           = usb_cstd_port_speed();
            g_usb_hcd_devaddr           = 0;
            if (USB_NOCONNECT == g_usb_hstd_devspd)
            {
                usb_hstd_set_devaddx_register(USB_NOCONNECT);
            }
            else
            {
                usb_hstd_set_devaddx_register(g_usb_hstd_devspd);
            }
            g_usb_hstd_hcd_dcp_mxps[0] = (uint16_t)(USB_DEFPACKET);

            if (usb_hstd_devaddr_to_speed() == USB_LSCONNECT)
            {
                hw_usb_hset_trnensel();
            }
            else
            {
                hw_usb_hclear_trnensel();
            }
    
            usb_hstd_get_descriptor(0);
            R_USB_REL_BLK(gp_usb_mgr_msg->flag);
        break;
        case    USB_7:
            switch (gp_usb_mgr_msg->keyword)
            {
                case    USB_NO:             /* The class driver responds when it is not possible to operate. */
                    /* The pre-register is released and the class driver is retrieved again */
                    p_driver = &g_usb_hcd_driver;
                    p_driver->rootport    = USB_NOPORT;
                    p_driver->devaddr     = USB_NODEVICE;
                    g_usb_mgr_seq   = USB_6;
                    usb_hstd_enumeration();
                break;
                case    USB_YES:            /* The class driver responds when it is possible to operate. */
                    usb_hstd_set_configuration();
                break;
                case    USB_ERROR:          /* The class driver is not found.  */
                /* continue */
                default:
                break;
            }
            R_USB_REL_BLK(gp_usb_mgr_msg->flag);
        break;
        default:
            if (USB_CTRL_END == g_usb_mgr_ctrl_msg.status)
            {
                enume_mode = usb_hstd_enumeration();
                switch (enume_mode)
                {
                    case USB_NONDEVICE:         /* Detach detection or enumeration error */
                        usb_hstd_sequence_clear();
                    break;
                    case USB_COMPLETEPIPESET:   /* Enumeration complete */
                        g_usb_mgr_devaddr    = USB_DEVICE_0;     /* DEVICE_0is not connected */
                        usb_hstd_sequence_clear();
                    break;
                    default:                    /* Enumeration sequence */
                    break;
                }
            }
            else
            {
                /* Enumeration error */
                usb_hstd_sequence_clear();
            }
        break;
    }
}   /* End of function usb_hstd_enumeration_sequence() */


/******************************************************************************
    Suspend sequence functions
******************************************************************************/


/******************************************************************************
Function Name   : usb_hstd_enumeration
Description     : USB Enumeration
Argument        : none
Return          : uint16_t                  : enumeration status
******************************************************************************/
USB_STATIC  uint8_t usb_hstd_enumeration(void)
{
    usb_hcdreg_t    *p_driver;
    uint8_t         *p_table[3];
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
    uint16_t vendor_id;
    uint16_t product_id;
    usb_compliance_t disp_param;
    uint8_t         ifclass;
#endif /* end USB_CFG_COMPLIANCE == USB_CFG_ENABLE */

    switch (g_usb_mgr_seq)
    {
        case USB_2:                 /* Receive Device Descriptor */
            /* Set device speed */
            g_usb_hstd_hcd_port    = g_usb_mgr_port;
            g_usb_hcd_devaddr      = g_usb_mgr_devaddr;
            usb_hstd_set_devaddx_register(g_usb_hstd_devspd);
            g_usb_hstd_hcd_dcp_mxps[g_usb_mgr_devaddr]
                = (uint16_t)((uint16_t)(g_usb_hstd_device_descriptor[USB_DEV_B_MAX_PACKET_SIZE_0] & USB_MAXPFIELD) |
                             (USB_ADDR2DEVSEL(g_usb_mgr_devaddr)));
            usb_hstd_set_asddress();
            g_usb_hcd_dev_info.state    = USB_STS_ADDRESS;
        break;
        case USB_3:             /* Set Address */
        /* continue */
        case USB_4:             /* Receive Device Descriptor(18) */
        /* continue */
        case USB_5:             /* Receive Configuration Descriptor(9) */
            if (usb_hstd_get_descriptor(g_usb_mgr_seq) != USB_OK)
            {
                return (USB_NONDEVICE);
            }
        break;
        case USB_6:             /* Receive Configuration Descriptor(xx) */
            /* Device enumeration function */
            p_driver = &g_usb_hcd_driver;

#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
            vendor_id = (uint16_t)(g_usb_hstd_device_descriptor[USB_DEV_ID_VENDOR_L]
                    + ((uint16_t)g_usb_hstd_device_descriptor[USB_DEV_ID_VENDOR_H] << 8));
            product_id = (uint16_t)(g_usb_hstd_device_descriptor[USB_DEV_ID_PRODUCT_L]
                    + ((uint16_t)g_usb_hstd_device_descriptor[USB_DEV_ID_PRODUCT_H] << 8));

            /* Check PET connect */
            if ((USB_PET_VID == vendor_id) && (USB_PET_PID == product_id))
            {
                usb_hstd_set_configuration();
                g_usb_mgr_seq = USB_7;
                return (USB_DEVICEENUMERATION);
            }
            else
            {
                ifclass = usb_hstd_get_ifclass( p_driver->ifclass );
            }

            if ((USB_STS_DETACH == p_driver->devstate) && ( USB_YES == ifclass ) )
            {
#else /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
            if (USB_STS_DETACH == p_driver->devstate)
            {
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
                /* To judge the detach while the class driver is operating, the pre-register is executed. */
                p_driver->rootport    = g_usb_mgr_port;
                p_driver->devaddr     = g_usb_mgr_devaddr;
                p_table[0]    = g_usb_hstd_device_descriptor;
                p_table[1]    = g_usb_hstd_cfg_descriptor;
                p_table[2]    = (uint8_t*)&g_usb_mgr_devaddr;

                g_usb_hcd_dev_info.port     = g_usb_mgr_port;
                g_usb_hcd_dev_info.speed    = g_usb_hstd_devspd;

                (*p_driver->classcheck)(p_table);
                return (USB_DEVICEENUMERATION);
                /* In this function, check device class of enumeration flow move to class */
                /* "usb_hstd_return_enu_mgr()" is used to return */
            }
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
            ifclass = usb_hstd_get_ifclass( USB_IFCLS_HUB );
            if (USB_YES == ifclass )
            {
                disp_param.status = USB_CT_HUB;
                disp_param.pid    = product_id;
                disp_param.vid    = vendor_id;
            }
            else
            {
                disp_param.status = USB_CT_NOTTPL;
                disp_param.pid    = product_id;
                disp_param.vid    = vendor_id;
            }
            usb_compliance_disp ((void *)&disp_param);

#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
            usb_hstd_return_enu_mgr(USB_ERROR);
            usb_hstd_connect_err_event_set();           /* Set event USB_STS_NOT_SUPPORT */
        break;
        case USB_8:             /* Set Configuration */
            /* Device enumeration function */
            p_driver = &g_usb_hcd_driver;
            if (g_usb_mgr_devaddr == p_driver->devaddr )
            {
                g_usb_hcd_dev_info.port     = g_usb_mgr_port;
                g_usb_hcd_dev_info.state    = USB_STS_CONFIGURED;
                g_usb_hcd_dev_info.config   = g_usb_hstd_cfg_descriptor[USB_DEV_B_CONFIGURATION_VALUE];
                g_usb_hcd_dev_info.speed    = g_usb_hstd_devspd;
                /* Device state */
                p_driver->devstate                        = USB_STS_CONFIGURED;
                (*p_driver->statediagram)(p_driver->devaddr, USB_STS_CONFIGURED);
                return (USB_COMPLETEPIPESET);
            }
            return (USB_COMPLETEPIPESET);
        break;
        default:
                /* Non process */
        break;
    }
    return (USB_DEVICEENUMERATION);
}   /* End of function usb_hstd_enumeration() */


/******************************************************************************
Function Name   : usb_hstd_global_suspend
Description     : Start of global suspend(select port)
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_global_suspend(void)
{
    if (g_usb_hcd_dev_info.port == gp_usb_mgr_msg->keyword)
    {
        if (USB_STS_CONFIGURED == g_usb_hcd_dev_info.state)
        {
            g_usb_hcd_dev_info.state    = USB_DO_GLOBAL_SUSPEND;
            USB_GET_SND(USB_MGR_MBX, USB_DO_SELECTIVE_SUSPEND, gp_usb_mgr_msg->complete, USB_DEVICEADDR);
        }
    }
    R_USB_REL_BLK(gp_usb_mgr_msg->flag);
}   /* End of function usb_hstd_global_suspend() */

/******************************************************************************
Function Name   : usb_hstd_suspend_start
Description     : Start of selective suspend(select device)
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_suspend_start(void)
{
    if (usb_hstd_sequence_addr() == USB_CONNECT)
    {
        if (USB_STS_CONFIGURED == g_usb_hcd_dev_info.state)
        {
            g_usb_hcd_dev_info.state    = USB_DO_SELECTIVE_SUSPEND;
            usb_hstd_get_descriptor(USB_4);
            R_USB_REL_BLK(gp_usb_mgr_msg->flag);
        }
        else if (USB_DO_GLOBAL_SUSPEND == g_usb_hcd_dev_info.state)
        {
            usb_hstd_get_descriptor(USB_4);
            R_USB_REL_BLK(gp_usb_mgr_msg->flag);
        }
        else
        {
            usb_hstd_sequence_end();
        }
    }
}   /* End of function usb_hstd_suspend_start() */

/******************************************************************************
Function Name   : usb_hstd_suspend_sequence
Description     : Suspend sequence
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_suspend_sequence(void)
{
    switch (g_usb_mgr_seq)
    {
        case USB_1:
            /* Because it is that the Control forwarding is sure to end correctly, */
            /* GetDescriptor omits the normal termination of control forwarding judgment. */
            if (0 != (g_usb_hstd_cfg_descriptor[USB_DEV_BM_ATTRIBUTES] & USB_CF_RWUPON))
            {
                usb_hstd_set_feature((uint16_t)0xFF);
            }
            else
            {
                g_usb_mgr_seq++;
                g_usb_hcd_remote_wakeup = USB_NO;
                usb_hstd_status_change(USB_HCD_SUSPEND, (usb_strct_t)USB_NULL);
            }
        break;
        case USB_2:
            /* Because it is that the Control forwarding is sure to end correctly, */
            /* SetFeature(enable wakeup) omits the normal termination of control forwarding judgment. */
            g_usb_hcd_remote_wakeup = USB_YES;
            usb_hstd_status_change(USB_HCD_SUSPEND, (usb_strct_t)USB_NULL);
        break;
        case USB_3:
            usb_hstd_update_devstate(USB_STS_SUSPEND);
            if (USB_DO_SELECTIVE_SUSPEND == g_usb_hcd_dev_info.state)
            {
                g_usb_hcd_dev_info.state    = USB_STS_SUSPEND;
            }
            if (USB_DO_GLOBAL_SUSPEND == g_usb_hcd_dev_info.state)
            {
                g_usb_hcd_dev_info.state    = USB_STS_SUSPEND;
            }
            if (USB_NULL != gs_usb_mgr_callback)
            {
                (*gs_usb_mgr_callback)( g_usb_mgr_devaddr, USB_STS_SUSPEND );
            }
            usb_hstd_sequence_end();
        break;
        default:
            /* no processing */
            usb_hstd_sequence_end();
        break;
    }
}   /* End of function usb_hstd_suspend_sequence() */


/******************************************************************************
    Resume sequence functions
******************************************************************************/
/******************************************************************************
Function Name   : usb_hstd_global_resume
Description     : Start of global resume(select port)
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_global_resume(void)
{
    if (g_usb_hcd_dev_info.port == gp_usb_mgr_msg->keyword)
    {
        if (USB_STS_SUSPEND == g_usb_hcd_dev_info.state)
        {
            g_usb_hcd_dev_info.state    = USB_DO_GLOBAL_RESUME;
            USB_GET_SND(USB_MGR_MBX, USB_DO_SELECTIVE_RESUME, gp_usb_mgr_msg->complete, USB_DEVICEADDR);
        }
    }
    R_USB_REL_BLK(gp_usb_mgr_msg->flag);
}   /* End of function usb_hstd_global_resume() */

/******************************************************************************
Function Name   : usb_hstd_resume_start
Description     : Start of selective resume(select device)
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_resume_start(void)
{
    if (usb_hstd_sequence_addr() == USB_CONNECT)
    {
        if (USB_STS_SUSPEND == g_usb_hcd_dev_info.state)
        {
            usb_hstd_status_change(USB_HCD_RESUME, (usb_strct_t)USB_NULL);
        }
        else if (USB_DO_GLOBAL_RESUME == g_usb_hcd_dev_info.state)
        {
            if ((USB_DEVICEADDR == g_usb_mgr_devaddr) || ((USB_DEVICEADDR+1) == g_usb_mgr_devaddr))
            {
                usb_hstd_status_change(USB_HCD_RESUME, (usb_strct_t)USB_NULL);
            }
        }
        else
        {
            usb_hstd_sequence_end();
        }
    }

    R_USB_REL_BLK( gp_usb_mgr_msg->flag );
}   /* End of function usb_hstd_resume_start() */

/******************************************************************************
Function Name   : usb_hstd_resume_sequence
Description     : Resume sequence
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_resume_sequence(void)
{
    switch (g_usb_mgr_seq)
    {
        case USB_1:
            usb_hstd_get_descriptor(USB_4);
            R_USB_REL_BLK(gp_usb_mgr_msg->flag);
        break;
        case USB_2:
            /* Because it is that the Control forwarding is sure to end correctly, */
            /* GetDescriptor omits the normal termination of control forwarding judgment. */
            if (0 != (g_usb_hstd_cfg_descriptor[USB_DEV_BM_ATTRIBUTES] & USB_CF_RWUPON))
            {
                usb_hstd_clear_feature((uint16_t)0xFF);
            }
            else
            {
                usb_hstd_update_devinfo(USB_STS_CONFIGURED);
                if (USB_NULL != gs_usb_mgr_callback)
                {
                    (*gs_usb_mgr_callback)( g_usb_mgr_devaddr, USB_STS_RESUME );
                }
                usb_hstd_sequence_end();
            }
        break;
        case USB_3:
            /* Because it is that the Control forwarding is sure to end correctly, */
            /* ClearFeature(disable wakeup) omits the normal termination of control forwarding judgment. */
            usb_hstd_update_devinfo(USB_STS_CONFIGURED);
            if (USB_NULL != gs_usb_mgr_callback)
            {
                (*gs_usb_mgr_callback)( g_usb_mgr_devaddr, USB_STS_RESUME );
            }
            usb_hstd_sequence_end();
        break;
        default:
            /* no processing */
            usb_hstd_sequence_end();
        break;
    }
}   /* End of function usb_hstd_resume_sequence() */


/******************************************************************************
    Remote wakeup sequence functions
******************************************************************************/
/******************************************************************************
Function Name   : usb_hstd_remote_start
Description     : Start of remote wakeup sequence
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_remote_start(void)
{
    if (usb_hstd_sequence_port() == USB_CONNECT)
    {
        if (USB_STS_SUSPEND == g_usb_hcd_dev_info.state)
        {
            usb_hstd_status_change(USB_HCD_RESUME, (usb_strct_t)USB_NULL);
        }
    }
    R_USB_REL_BLK(gp_usb_mgr_msg->flag);
}   /* End of function usb_hstd_remote_start() */

/******************************************************************************
Function Name   : usb_hstd_remote_sequence
Description     : Remote wakeup sequence
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_remote_sequence(void)
{
    usb_hcdreg_t    *p_driver;

    switch (g_usb_mgr_seq)
    {
        case USB_1:
            usb_hstd_clear_feature((uint16_t)0xFF);
            R_USB_REL_BLK(gp_usb_mgr_msg->flag);
        break;
        case USB_2:
            /* Because it is that the Control forwarding is sure to end correctly, */
            /* ClearFeature(disable wakeup) omits the normal termination of control forwarding judgment. */
            usb_hstd_update_devinfo(USB_STS_CONFIGURED);
            p_driver  = &g_usb_hcd_driver;
            if (g_usb_mgr_devaddr == p_driver->devaddr)
            {
                (*p_driver->statediagram)(g_usb_mgr_devaddr, USB_STS_RESUME);
            }
            usb_hstd_sequence_end();
        break;
        default:
            /* no processing */
            usb_hstd_sequence_end();
        break;
    }
}   /* End of function usb_hstd_remote_sequence() */


/******************************************************************************
    Detach processing functions
******************************************************************************/
/******************************************************************************
Function Name   : usb_hstd_detach_start
Description     : Start of detach processing
                  Start of detach processing in sequence continuance
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_detach_start(void)
{
    usb_hcdreg_t    *p_driver;

    usb_hstd_sequence_port();

    p_driver  = &g_usb_hcd_driver;
    if (g_usb_mgr_devaddr == p_driver->devaddr)
    {
        (*p_driver->statediagram)(g_usb_mgr_devaddr, USB_STS_DETACH);
        usb_hstd_clr_device_info();
        p_driver->rootport                = USB_NOPORT;
        p_driver->devaddr                 = USB_NODEVICE;
        p_driver->devstate                = USB_STS_DETACH;
    }
    usb_hstd_sequence_end();
}   /* End of function usb_hstd_detach_start() */

/******************************************************************************
Function Name   : usb_hstd_enumeration_detach
Description     : Start of detach processing in enumeration sequence
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_enumeration_detach(void)
{
    usb_hcdreg_t    *p_driver;

    if (USB_6 == g_usb_mgr_seq)
    {
        /* The detach request is notified while processing the class check. */
        p_driver = &g_usb_hcd_driver;
        (*p_driver->statediagram)(g_usb_mgr_devaddr, USB_STS_DETACH);
        usb_hstd_clr_device_info();
    }
    usb_hstd_detach_start();
}   /* End of function usb_hstd_enumeration_detach() */


/******************************************************************************
    Over current processing functions
******************************************************************************/
/******************************************************************************
Function Name   : usb_hstd_overcurrent_start
Description     : Start of over current processing
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_overcurrent_start(void)
{
    usb_hcdreg_t    *p_driver;
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
    usb_compliance_t disp_param;

    disp_param.status = USB_CT_OVRC;
    disp_param.pid    = USB_NULL;
    disp_param.vid    = USB_NULL;
    usb_compliance_disp ((void *)&disp_param);
#endif

    usb_hstd_sequence_port();
    usb_hstd_clr_device_info();

    p_driver = &g_usb_hcd_driver;
    if (p_driver->rootport == g_usb_mgr_port )
    {
        (*p_driver->statediagram)(p_driver->devaddr, USB_STS_OVERCURRENT);
        g_usb_mgr_devaddr     = p_driver->devaddr;
        usb_hstd_clr_device_info();
        p_driver->rootport    = USB_NOPORT;
        p_driver->devaddr     = USB_NODEVICE;
    }
    usb_hstd_sequence_end();
}   /* End of function usb_hstd_overcurrent_start() */


/******************************************************************************
    Port enable sequence functions
******************************************************************************/
/******************************************************************************
Function Name   : usb_hstd_portenable_start
Description     : Start of port enable sequence
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_portenable_start(void)
{
    usb_hstd_sequence_port();
    usb_hstd_status_change(USB_HCD_VBON, (usb_strct_t)USB_NULL);
}   /* End of function usb_hstd_portenable_start() */


/******************************************************************************
Function Name   : usb_hstd_portenable_sequence
Description     : Port enable sequence
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_portenable_sequence(void)
{
    usb_hstd_sequence_end();
}   /* End of function usb_hstd_portenable_sequence() */


/******************************************************************************
    Port disable sequence functions
******************************************************************************/
/******************************************************************************
Function Name   : usb_hstd_portdisable_start
Description     : Start of port disable sequence
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_portdisable_start(void)
{
    usb_hstd_sequence_port();
    usb_hstd_status_change(USB_HCD_VBOFF, (usb_strct_t)USB_NULL );
}   /* End of function usb_hstd_portdisable_start() */

/******************************************************************************
Function Name   : usb_hstd_portdisable_sequence
Description     : Port disable sequence
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_portdisable_sequence(void)
{
    usb_hstd_sequence_end();
}   /* End of function usb_hstd_portdisable_sequence() */


/******************************************************************************
    Clear_Stall sequence functions
******************************************************************************/
/******************************************************************************
Function Name   : usb_hstd_clearstall_start
Description     : Start of STALL clear sequence
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_clearstall_start(void)
{
    uint16_t        dir_ep;

    if (usb_hstd_sequence_pipe() == USB_CONNECT)
    {
        dir_ep = (uint16_t)usb_hstd_pipe_to_epadr(g_usb_hstd_current_pipe);
        usb_hstd_pipe_to_hcddevaddr();
        usb_hstd_clear_feature(dir_ep);
    }
    R_USB_REL_BLK( gp_usb_mgr_msg->flag );
}   /* End of function usb_hstd_clearstall_start() */

/******************************************************************************
Function Name   : usb_hstd_clearstall_sequence
Description     : STALL clear sequence
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_clearstall_sequence(void)
{
    switch (g_usb_mgr_seq)
    {
        case USB_1:
            /* Because it is that the Control forwarding is sure to end correctly, */
            /* ClearFeature(clear stall) omits the normal termination of control forwarding judgment. */
            usb_hstd_status_change(USB_HCD_CLEAR_STALL, (usb_strct_t)g_usb_mgr_pipe);
        break;
        case USB_2:
            if (USB_NULL != gs_usb_mgr_callback)
            {
                (*gs_usb_mgr_callback)(g_usb_mgr_pipe, USB_NULL);
            }
            usb_hstd_sequence_end();
        break;
        default:
            /* no processing */
            usb_hstd_sequence_end();
        break;
    }
}   /* End of function usb_hstd_clearstall_sequence() */


/******************************************************************************
    Common wait sequence
******************************************************************************/

/******************************************************************************
Function Name   : usb_hstd_return_enu_mgr
Description     : Return to Enumeration of MGR
Argument        : uint16_t cls_result       : class check result
Return          : none
******************************************************************************/
void usb_hstd_return_enu_mgr(uint16_t cls_result)
{
    g_usb_mgr_seq++;
    USB_GET_SND(USB_MGR_MBX, USB_MGR_CONTINUE, &usb_hstd_dummy_function, (usb_strct_t)cls_result);
}   /* End of function usb_hstd_return_enu_mgr() */

/******************************************************************************
Function Name   : usb_hstd_mgr_wait
Description     : Manager sequence message wait
Argument        : wait times
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_mgr_wait(uint16_t count)
{
    usb_er_t        err;

    err = USB_WAI_MSG(USB_MGR_MBX, (usb_msg_t*)gp_usb_mgr_msg, count);
    if (USB_OK != err)
    {
        USB_DEBUG_HOOK(USB_DEBUG_HOOK_HOST | USB_DEBUG_HOOK_CODE5);
    }
}   /* End of function usb_hstd_mgr_wait() */


/******************************************************************************
Function Name   : usb_hstd_sequence_clear
Description     : Sequence clear
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_sequence_clear(void)
{
    g_usb_mgr_seq_mode    = USB_MGR_NOSEQUENCE;
    g_usb_mgr_seq   = 0;
}   /* End of function usb_hstd_sequence_clear() */


/******************************************************************************
Function Name   : usb_hstd_sequence_end
Description     : Sequence end
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_sequence_end(void)
{
    usb_hstd_sequence_clear();
    R_USB_REL_BLK(gp_usb_mgr_msg->flag);
}   /* End of function usb_hstd_sequence_end() */


/******************************************************************************
Function Name   : usb_hstd_sequence_port
Description     : Sequence port start
Argument        : none
Return          : USB_CONNECT/USB_NOCONNECT
******************************************************************************/
USB_STATIC uint8_t usb_hstd_sequence_port(void)
{
    g_usb_mgr_port           = (usb_port_t)gp_usb_mgr_msg->keyword;
    gs_usb_mgr_callback      = gp_usb_mgr_msg->complete;
    g_usb_mgr_seq_mode       = (gp_usb_mgr_msg->msginfo - USB_MGR_CONTINUE);
    g_usb_mgr_seq            = 0;
    g_usb_mgr_devaddr        = (usb_addr_t)(g_usb_mgr_port + USB_DEVICEADDR);
    gp_usb_mgr_msg->msginfo  = USB_MGR_CONTINUE;
    if (g_usb_hcd_dev_info.port == g_usb_mgr_port)
    {
        return USB_CONNECT;
    }
    return USB_NOCONNECT;
}   /* End of function usb_hstd_sequence_port() */


/******************************************************************************
Function Name   : usb_hstd_sequence_addr
Description     : Sequence device address start
Argument        : none
Return          : USB_CONNECT/USB_NOCONNECT
******************************************************************************/
USB_STATIC uint8_t usb_hstd_sequence_addr(void)
{
    g_usb_mgr_devaddr     = (usb_addr_t)gp_usb_mgr_msg->keyword;
    gs_usb_mgr_callback   = gp_usb_mgr_msg->complete;
    g_usb_mgr_seq_mode    = (gp_usb_mgr_msg->msginfo - USB_MGR_CONTINUE);
    g_usb_mgr_seq         = 0;
    g_usb_hcd_devaddr     = g_usb_mgr_devaddr;
    g_usb_mgr_port        = usb_hstd_devaddr_to_port();
    g_usb_hstd_hcd_port   = g_usb_mgr_port;
    if (g_usb_hcd_dev_info.port == g_usb_mgr_port)
    {
        return USB_CONNECT;
    }
    usb_hstd_sequence_end();
    return USB_NOCONNECT;
}   /* End of function usb_hstd_sequence_addr() */

/******************************************************************************
Function Name   : usb_hstd_sequence_pipe
Description     : Sequence pipe start
Argument        : none
Return          : USB_CONNECT/USB_NOCONNECT
******************************************************************************/
USB_STATIC uint8_t usb_hstd_sequence_pipe(void)
{
    g_usb_hstd_current_pipe  = (usb_port_t)gp_usb_mgr_msg->keyword;
    g_usb_mgr_pipe           = g_usb_hstd_current_pipe;
    gs_usb_mgr_callback      = gp_usb_mgr_msg->complete;
    g_usb_mgr_seq_mode       = (gp_usb_mgr_msg->msginfo - USB_MGR_CONTINUE);
    g_usb_mgr_seq            = 0;
    g_usb_mgr_devaddr        = usb_hstd_pipe_to_addr();
    gp_usb_mgr_msg->msginfo  = USB_MGR_CONTINUE;
    if (USB_DEVICE_0 != g_usb_mgr_devaddr)
    {
        return USB_CONNECT;
    }
    return USB_NOCONNECT;
}   /* End of function usb_hstd_sequence_pipe() */

#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
/******************************************************************************
Function Name   : usb_hstd_get_ifclass
Description     : 
Arguments       : none
Return value    : uint8_t                  : class
Note            : 
******************************************************************************/
USB_STATIC uint8_t usb_hstd_get_ifclass( uint8_t class )
{
    uint16_t    len;
    uint16_t    index;
    
    len = g_usb_hstd_cfg_descriptor[2] | ( g_usb_hstd_cfg_descriptor[3] << 8 );
    index = g_usb_hstd_cfg_descriptor[0];
    /* WAIT_LOOP */
    while (index < len )
    {
        if (0 == g_usb_hstd_cfg_descriptor[index])
        {
            return USB_NO;
        }

        if (USB_DT_INTERFACE == g_usb_hstd_cfg_descriptor[index+1])
        {
            if (g_usb_hstd_cfg_descriptor[index+5] == class )
            {
                return USB_YES;
            }
        }
        index += g_usb_hstd_cfg_descriptor[index];
    }
    return USB_NO;
} /* End of function usb_hstd_get_ifclass() */
#endif /* end USB_CFG_COMPLIANCE == USB_CFG_ENABLE */


#endif /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
#endif /*(BSP_CFG_RTOS_USED == 0)*/
/******************************************************************************
 End  Of File
******************************************************************************/
