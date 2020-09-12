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
 * Copyright (C) 2013(2018) Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
* File Name    : r_usb_hdriverapi.c
* Version      : 1.01
* Description  : This is the USB host control driver API function code.
*******************************************************************************/
/*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 01.09.2014 1.00    First Release
*           : 01.06.2015 1.01    Added RX231.
*           : 30.11.2018 1.10    Supporting Smart Configurator
*******************************************************************************/

/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_usb_basic_mini_if.h"
#include "r_usb_bitdefine.h"
#include "r_usb_typedef.h"
#include "r_usb_extern.h"

#if (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST


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
Function Name   : usb_hstd_driver_init
Description     : Usb Driver Open
Argument        : none
Return          : none
******************************************************************************/
void usb_hstd_driver_init(void)
{
    /* Initialized Manager Mode */
    g_usb_mgr_seq   = 0;
    g_usb_mgr_seq_mode    = USB_MGR_NOSEQUENCE;

    /* Initialized driver registration area */
    usb_hstd_driver_reg_init();

    /* Initial device address & driver number */
    g_usb_hcd_devaddr    = (usb_addr_t)0;

    /* Initialized device information area */
    usb_hstd_clr_device_info();

    /* Control transfer stage management */
    g_usb_hcd_ctsq               = USB_IDLEST;
    g_usb_hcd_remote_wakeup  = USB_NO;

    usb_cstd_init_library();                    /* Initialized global variables */

#if USB_CFG_BC == USB_CFG_ENABLE
    g_usb_hstd_bc.state = USB_BC_STATE_INIT;
#endif

    usb_hstd_class_driver_start ();
    usb_hstd_host_registration ();

}   /* End of function usb_hstd_driver_init() */


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

}   /* End of function usb_hstd_driver_registration() */

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

}   /* End of function usb_hstd_driver_release() */


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
        case USB_DO_INITHWFUNCTION:
            usb_hstd_hw_start();                        /* Begining the clock supply of USB-IP*/
            usb_hstd_sw_reset();                        /* USB-IP initialized */
            return  USB_OK;
        break;
        case USB_DO_SETHWFUNCTION:
            /* USB interrupt message initialize, select HW function */
            usb_hstd_set_hw_function();
            return  USB_OK;
        break;
        default:
            /* no processing */
        break;
    }
    return  USB_ERROR;
}   /* End of function usb_hstd_change_device_state() */


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
}   /* End of function usb_hstd_device_information() */
#endif /*defined(USB_CFG_HMSC_USE)*/

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


#endif /* #if (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
/******************************************************************************
End of file
******************************************************************************/
