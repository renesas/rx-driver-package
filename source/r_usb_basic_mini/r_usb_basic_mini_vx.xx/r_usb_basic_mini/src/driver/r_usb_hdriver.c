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
 * Copyright (C) 2013(2020) Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_usb_hdriver.c
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
*           : 30.06.2020 1.20    Added support for RTOS.
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

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/

/* functions */
/* HCD task / MGR task */
USB_STATIC  void                usb_hstd_set_submitutr(void);
#if defined(USB_CFG_HVND_USE)
USB_STATIC  void    usb_hvnd_device_state (uint16_t data, uint16_t state);
#endif /* defined(USB_CFG_HVND_USE) */

/*******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

/* variables */
/* Host control driver variables */
uint16_t            g_usb_hstd_hcd_dcp_mxps[USB_MAXDEVADDR+1u];    /* DEVSEL & DCPMAXP (Multiple device) */
uint8_t             g_usb_hstd_devspd;                             /* Reset handshake result */
uint8_t             g_usb_hstd_cfg_descriptor[USB_CONFIGSIZE];

/* Host control driver variables */
usb_port_t          g_usb_hstd_hcd_port;
usb_tskinfo_t       *gp_usb_hstd_hcd_message;                      /* Hcd Task receive message */
usb_hcdreg_t        g_usb_hcd_driver;                              /* Device driver (registration) */
uint8_t             g_usb_hcd_remote_wakeup;
uint8_t             g_usb_hstd_ctsq;                                /* Control transfer stage management */
usb_addr_t          g_usb_hcd_devaddr;                             /* Device address */
usb_hcddevinf_t     g_usb_hcd_dev_info;                            /* port, status, config, speed, */


/******************************************************************************
Function Name   : usb_hstd_transfer_start
Description     : Request Data transfer
Argument        : usb_hutr_t *p_utr_table      : message
Return          : usb_er_t                  : USB_OK/USB_QOVR/USB_ERROR
******************************************************************************/
usb_er_t usb_hstd_transfer_start(usb_hutr_t *p_utr_table)
{
    usb_er_t    err;

    if (USB_ON == g_usb_hstd_pipe_request[p_utr_table->pipenum])
    {
        return USB_QOVR;
    }

    g_usb_hstd_current_pipe = (usb_pipe_no_t)p_utr_table->pipenum;
    /* Pipe Transfer Process check */
    if (USB_NULL != gp_usb_hstd_pipe[g_usb_hstd_current_pipe])
    {
        return USB_QOVR;
    }

    /* Get device address from pipe number */
    if (USB_PIPE0 == g_usb_hstd_current_pipe)
    {
        g_usb_hcd_devaddr    = (usb_addr_t)p_utr_table->setup[4];
    }
    else
    {
        usb_hstd_pipe_to_hcddevaddr();
        if (USB_DEVICE_0 == g_usb_hcd_devaddr)
        {
            return USB_ERROR;
        }
    }

    /* Get device speed from device address */
    if (usb_hstd_devaddr_to_speed() == USB_NOCONNECT )
    {
        return USB_ERROR;
    }

    p_utr_table->msginfo  = USB_DO_TRANSFERSTART;
    err = USB_SND_MSG(USB_HCD_MBX, (usb_msg_t*)p_utr_table);
    if (USB_OK == err)
    {
        g_usb_hstd_pipe_request[g_usb_hstd_current_pipe] = USB_ON;
    }
    return err;
}   /* End of function usb_hstd_transfer_start() */


/******************************************************************************
Function Name   : usb_hstd_set_submitutr
Description     : USB data transfer/control transfer start
Argument        : none
Return          : none
******************************************************************************/
USB_STATIC void usb_hstd_set_submitutr(void)
{
    uint8_t connect_inf;

    g_usb_hstd_current_pipe                   = (usb_pipe_no_t)gp_usb_hstd_hcd_message->keyword;
    gp_usb_hstd_pipe[g_usb_hstd_current_pipe] = (usb_hutr_t*)gp_usb_hstd_hcd_message;

    g_usb_hstd_pipe_request[g_usb_hstd_current_pipe] = USB_OFF;

    /* Get device address from pipe number */
    if (USB_PIPE0 == g_usb_hstd_current_pipe)
    {
        g_usb_hcd_devaddr    = (usb_addr_t)gp_usb_hstd_pipe[g_usb_hstd_current_pipe]->setup[4];

        /* Get device speed from device address */
        connect_inf = usb_hstd_devaddr_to_speed();
        if (USB_NOCONNECT == connect_inf)
        {
            /* Control Read/Write End */
            usb_hstd_control_end(USB_DATA_DTCH);
            return;
        }

        /* Control transfer idle stage ? */
        if (USB_IDLEST == g_usb_hstd_ctsq)
        {
            usb_hstd_setup_start();
        }
        else
        {
            /* Control Read/Write End */
            usb_hstd_control_end(USB_DATA_ERR);
        }
    }
    else
    {
        usb_hstd_pipe_to_hcddevaddr();
        if (USB_DEVICE_0 == g_usb_hcd_devaddr)
        {
            /* End of data transfer (IN/OUT) */
            usb_hstd_forced_termination(USB_DATA_ERR);
            return;
        }

        /* Get device speed from device address */
        connect_inf = usb_hstd_devaddr_to_speed();
        if (USB_NOCONNECT == connect_inf)
        {
            /* End of data transfer (IN/OUT) */
            usb_hstd_forced_termination(USB_DATA_DTCH);
            return;
        }

        /* Data transfer */
        if (usb_cstd_get_pipe_dir(g_usb_hstd_current_pipe) == USB_DIR_H_IN )
        {
            usb_hstd_receive_start();
        }
        else
        {
            usb_hstd_send_start();
        }
    }
}
/******************************************************************************
 End of function usb_hstd_set_retransfer
 ******************************************************************************/


/******************************************************************************
Function Name   : usb_hstd_hcd_task
Description     : USB Host Control Driver Task
Argument        : none
Return          : none
******************************************************************************/
void usb_hstd_hcd_task(void)
{
    usb_tskinfo_t   *p_mess;
    usb_er_t        err;

    err = R_USB_RCV_MSG(USB_HCD_MBX, (usb_msg_t**)&p_mess);

    /* Message check */
    if (USB_OK != err)
    {
        return;
    }

    gp_usb_hstd_hcd_message   = (usb_tskinfo_t*)p_mess;
    g_usb_hstd_hcd_port       = (usb_port_t)gp_usb_hstd_hcd_message->keyword;
    g_usb_hstd_current_pipe   = (usb_pipe_no_t)gp_usb_hstd_hcd_message->keyword;

    /* Branch Hcd Task receive Message Command */
    switch (gp_usb_hstd_hcd_message->msginfo)
    {
        /* USB interrupt */
        case USB_INTERRUPT:                 /* USB interrupt */
            usb_hstd_interrupt_process();
        break;

        /* USB transfer */
        case USB_DO_TRANSFERSTART:          /* Transfer start */
            usb_hstd_set_submitutr();
        break;
        /* USB Transfer End Stop */
        case USB_DO_TRANSFER_STP:
            usb_hstd_transferend_check(USB_DATA_STOP);
        break;
        /* USB state control */
        case USB_HCD_USBRESET:          /* USB bus reset */
            usb_hstd_bus_reset();
            usb_hstd_status_result(USB_HCD_USBRESET);
        break;
        case USB_HCD_SUSPEND:           /* Suspend device */
            usb_hstd_suspend_process();
            usb_hstd_status_result(USB_HCD_SUSPEND);
        break;
        case USB_HCD_RESUME:            /* USB resume */
            usb_hstd_resume_process();
            usb_hstd_status_result(USB_HCD_RESUME);
        break;
        case USB_HCD_VBON:              /* USB VBUS control ON */
            usb_hstd_vbus_control_on();
#if USB_CFG_BC == USB_CFG_DISABLE
            usb_cpu_delay_xms((uint16_t)100u);
#endif  /* USB_CFG_BC == USB_CFG_ENABLE */
            usb_hstd_status_result(USB_HCD_VBON);
        break;
        case USB_HCD_VBOFF:             /* USB VBUS control OFF */
            usb_hstd_vbus_control_off();
#if USB_CFG_BC == USB_CFG_DISABLE
            usb_cpu_delay_xms((uint16_t)100);
#endif  /* USB_CFG_BC == USB_CFG_ENABLE */
            usb_hstd_status_result(USB_HCD_VBOFF);
        break;
        case USB_HCD_CLEAR_STALL:
            usb_cstd_clr_stall(g_usb_hstd_current_pipe);
            hw_usb_set_sqclr (g_usb_hstd_current_pipe);
            usb_hstd_status_result(USB_HCD_CLEAR_STALL);
        break;

        default:
            USB_DEBUG_HOOK(USB_DEBUG_HOOK_HOST | USB_DEBUG_HOOK_CODE3);
        break;
    }
}
/******************************************************************************
 End of function usb_hstd_hcd_task
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hstd_driver_registration
 Description     : Class driver registration
 Argument        : usb_hcdreg_t *p_registinfo    : class driver structure
 Return          : none
******************************************************************************/
void usb_hstd_driver_registration(usb_hcdreg_t *p_registinfo)
{
    usb_hcdreg_t    *p_driver;

    /* Driver registration area */
    usb_hstd_driver_reg_init();
    p_driver = &g_usb_hcd_driver;
    p_driver->ifclass     = p_registinfo->ifclass;
    p_driver->classcheck  = p_registinfo->classcheck;
    p_driver->statediagram= p_registinfo->statediagram;
}
/******************************************************************************
 End of function usb_hstd_driver_registration
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_driver_release
 Description     : Class driver release
 Argument        : uint8_t devclass          : interface class
 Return          : none
******************************************************************************/
void usb_hstd_driver_release(uint8_t devclass)
{
    usb_hcdreg_t    *p_driver;

    if (USB_IFCLS_NOT == devclass)
    {
        /* Device driver number */
        usb_hstd_driver_reg_init();
    }
    else
    {
        p_driver = &g_usb_hcd_driver;
        /* Driver release */
        if (p_driver->ifclass == devclass )
        {
            p_driver->rootport    = USB_NOPORT;
            p_driver->devaddr     = USB_NODEVICE;
            p_driver->devstate    = USB_STS_DETACH;
            p_driver->ifclass     = USB_IFCLS_NOT;
            return;
        }
    }
}
/******************************************************************************
 End of function usb_hstd_driver_release
 ******************************************************************************/


/******************************************************************************
Function Name   : usb_hstd_change_device_state
Description     : Change HUB down port device status
Argument        : usb_cbinfo_t complete     : callback function pointer
                : uint16_t msginfo           : Message Info
                : uint16_t keyword           : address, rootport, etc
Return          : usb_er_t                   : USB_OK etc
******************************************************************************/
usb_er_t usb_hstd_change_device_state(usb_cbinfo_t complete, usb_strct_t msginfo, usb_strct_t keyword)
{
    switch (msginfo)
    {
        case USB_DO_PORT_ENABLE:
            usb_hstd_vbus_control_on();
            return  USB_OK;
        break;
        /* continue */
        case USB_DO_PORT_DISABLE:
        /* continue */
        case USB_DO_GLOBAL_SUSPEND:
        /* continue */
        case USB_DO_GLOBAL_RESUME:
            if (USB_PORT0 == keyword)
            {
                return  USB_GET_SND(USB_MGR_MBX, msginfo, complete, keyword);
            }
        break;
        case USB_DO_CLEAR_STALL:
            if (( keyword <= USB_MAX_PIPE_NO ) && ( keyword >= USB_MIN_PIPE_NO ))
            {
                return  USB_GET_SND(USB_MGR_MBX, USB_DO_CLEAR_STALL, complete, keyword);
            }
        break;
        default:
            /* no processing */
        break;
    }
    return  USB_ERROR;
}
/******************************************************************************
 End of function usb_hstd_change_device_state
 ******************************************************************************/


#if defined(USB_CFG_HMSC_USE)
/******************************************************************************
Function Name   : usb_hstd_device_information
Description     : Return to device information
Argument        : usb_addr_t devaddr        : Device address
                : uint16_t *tbl             : Table Pointer
Return          : None
******************************************************************************/
void usb_hstd_device_information (usb_addr_t addr, uint16_t *p_table)
{
    if (0 == addr)
    {
        /* When "0" is specified for the device address, it responds as follows. */
        /* When the device is not operating the enumeration */
        /*      table[0]    = USB_NOPORT        */
        /*      table[1]    = USB_STS_DETACH    */
        /* When the device is operating the enumeration */
        /*      table[0]    = Port number       */
        /*      table[1]    = USB_STS_DEFAULT   */
        p_table[0]    = g_usb_hcd_dev_info.port;
        p_table[1]    = USB_STS_DEFAULT;
        if (USB_NOPORT == p_table[0])
        {
            p_table[1]    = USB_STS_DETACH;
        }
        return;
    }

    if (0 != g_usb_mgr_seq)
    {
/* Should respond while processing the sequence? */
        return;
    }

/* Trouble occurs when this function is called because g_usb_hcd_dev_info[] cannot correct the setting. */
    p_table[0]    = g_usb_hcd_dev_info.port;
    p_table[1]    = g_usb_hcd_dev_info.state;
    p_table[2]    = g_usb_hcd_dev_info.config;
    p_table[3]    = g_usb_hcd_dev_info.speed;

    /* The code conversion is necessary for the suspend status. */
    if (USB_STS_SUSPEND == g_usb_hcd_dev_info.state)
    {
        p_table[1]    = USB_STS_SUSPEND;
    }
}
/******************************************************************************
 End of function usb_hstd_device_information
 ******************************************************************************/
#endif /*defined(USB_CFG_HMSC_USE)*/

#if defined(USB_CFG_HVND_USE)
/******************************************************************************
 * Function Name: usb_hvnd_device_state
 * Description  : Open / Close
 * Arguments    : uint16_t data          : Device address
                : uint16_t state            : Device state
 * Return Value : none
 ******************************************************************************/
USB_STATIC void usb_hvnd_device_state (uint16_t data, uint16_t state)
{
    usb_ctrl_t  ctrl;
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
    usb_compliance_t disp_param;
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */

    switch( state )
    {
        case USB_STS_DETACH:
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
            disp_param.status = USB_CT_DETACH;
            disp_param.pid    = USB_NULL;
            disp_param.vid    = USB_NULL;
            usb_compliance_disp ((void *)&disp_param);

#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
            usb_cstd_pipe_reg_clear ();
            usb_cstd_pipe_table_clear ();

            usb_cstd_set_event (USB_STS_DETACH, &ctrl);
        break;

        case USB_STS_ATTACH:
#if USB_CFG_BC == USB_CFG_ENABLE
            if (USB_BC_STATE_CDP == data)
            {
                usb_cstd_set_event (USB_STS_BC, &ctrl);
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
        break;

        case USB_STS_CONFIGURED:
            usb_cstd_pipe_reg_set ();
            ctrl.type = USB_HVND;
            usb_cstd_set_event (USB_STS_CONFIGURED, &ctrl);
        break;

        case USB_STS_SUSPEND:
        break;

        case USB_STS_RESUME:
            usb_cstd_set_event (USB_STS_RESUME, &ctrl);
        break;

        case USB_STS_OVERCURRENT:
            usb_cstd_set_event (USB_STS_OVERCURRENT, &ctrl);
        break;

        default:
        break;
    }

}   /* eof usb_hvnd_device_state() */

/******************************************************************************
 * Function Name: usb_hvnd_registration
 * Description  : Registration of peripheral Communications Devices Driver
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void usb_hvnd_registration (void)
{
    usb_hcdreg_t    driver;

    /* Driver registration */
    driver.ifclass      = USB_IFCLS_VEN;            /* Vendor-Specific Class */
    driver.classcheck   = &usb_hstd_class_check;
    driver.statediagram = &usb_hvnd_device_state;
    usb_hstd_driver_registration (&driver);

}
/******************************************************************************
 End of function usb_hvnd_registration
 ******************************************************************************/

/******************************************************************************
 * Function Name: usb_hvnd_pipe_info
 * Description  : Host Pipe Information check and EP Table Set
 * Arguments    : uint8_t *p_table      : Check Start Descriptor address
                : uint16_t length       : Configuration Descriptor Length
 * Return Value : uint16_t              : USB_OK / USB_ERROR
 ******************************************************************************/
uint16_t usb_hvnd_pipe_info (uint8_t *p_table, uint16_t length)
{
    uint16_t        ofdsc;

    /* Check Endpoint Descriptor */
    ofdsc = p_table[0];

    /* WAIT_LOOP */
    while (ofdsc < length )
    {
        /* Search within Interface */
        if (USB_DT_ENDPOINT == p_table[ofdsc + 1])
        {
            /* Endpoint Descriptor */
            usb_cstd_pipe_table_set (USB_HVND, &p_table[ofdsc]);
        }
        ofdsc +=  p_table[ofdsc];
    }

    return USB_OK;
}
/******************************************************************************
 End of function usb_hvnd_pipe_info
 ******************************************************************************/



#endif /* defined(USB_CFG_HVND_USE) */

#endif /*(BSP_CFG_RTOS_USED == 0)*/
#endif /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
/******************************************************************************
 End  Of File
******************************************************************************/
