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
 * Copyright (C) 2016(2019) Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
* File Name    : r_usb_usbif_api.c
* Description  : USB Host and Peripheral Driver API code. 
*                HCD(Host Control Driver) PCD (Peripheral Control Driver)
 ******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 30.09.2015 1.00 First Release
*         : 30.09.2017 1.22 Update Argument Checking
*         : 30.11.2018 1.10    Supporting Smart Configurator
*         : 31.05.2019 1.11    Added support for GNUC and ICCRX.
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <string.h>

#include "r_usb_basic_mini_if.h"
#include "r_usb_bitdefine.h"
#include "r_usb_reg_access.h"
#include "r_usb_typedef.h"
#include "r_usb_extern.h"

#if defined(USB_CFG_HCDC_USE)
#include "r_usb_hcdc_mini_if.h"
#endif /* defined(USB_CFG_HCDC_USE) */

#if defined(USB_CFG_HHID_USE)
#include "r_usb_hhid_mini_if.h"
#endif /* defined(USB_CFG_HHID_USE) */

#if defined(USB_CFG_HMSC_USE)
#include "r_usb_hmsc_mini_if.h"
#endif /* defined(USB_CFG_HMSC_USE) */


#if defined(USB_CFG_PHID_USE)
#include "r_usb_phid_mini_if.h"
#endif /* defined(USB_CFG_PHID_USE) */

#if defined(USB_CFG_PCDC_USE)
#include "r_usb_pcdc_mini_if.h"
#endif /* defined(USB_CFG_PCDC_USE) */
/*******************************************************************************
 Macro definitions
 ******************************************************************************/

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/
 
/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/
 
/*******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/
uint32_t        g_usb_read_request_size[USB_MAXPIPE_NUM + 1];
usb_event_t     g_usb_cstd_event;
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
usb_hutr_t       g_usb_hstd_data[USB_MAXPIPE_NUM + 1];
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
usb_putr_t       g_usb_pstd_data[USB_MAXPIPE_NUM + 1];

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */

uint16_t        g_usb_cstd_usb_mode;
uint16_t        g_usb_cstd_open_class;
uint16_t        g_usb_hstd_is_susp_resm;

/*****************************************************************************
* Function Name: R_USB_GetVersion
* Description  : Returns the version of this module. The version number is
*                encoded such that the top two bytes are the major version
*                number and the bottom two bytes are the minor version number.
* Arguments    : none
* Return Value : version number
******************************************************************************/
uint32_t R_USB_GetVersion (void)
{
    uint32_t version = 0;

    version = (USB_VERSION_MAJOR << 16) | USB_VERSION_MINOR;

    return version;
} /* End of function R_USB_GetVersion */


/******************************************************************************
Function Name   : R_USB_GetEvent
Description     : Get event.
Arguments       : usb_ctrl_t *p_ctrl: control structure for USB API.
Return value    : event code.
******************************************************************************/
usb_status_t R_USB_GetEvent (usb_ctrl_t *p_ctrl)
{
    usb_status_t    event = USB_STS_NONE;

    usb_cstd_usb_task();
    if (g_usb_cstd_event.write_pointer != g_usb_cstd_event.read_pointer)
    {
        *p_ctrl = g_usb_cstd_event.ctrl[g_usb_cstd_event.read_pointer];
        event = g_usb_cstd_event.code[g_usb_cstd_event.read_pointer];
        g_usb_cstd_event.read_pointer++;
        if (g_usb_cstd_event.read_pointer >= USB_EVENT_MAX)
        {
            g_usb_cstd_event.read_pointer = 0;
        }
    }
    return event;
}  /* End of function R_USB_GetEvent() */


/******************************************************************************
Function Name   : R_USB_Open
Description     : Start of USB Driver.
Arguments       : usb_ctrl_t    *p_ctrl : Pointer to usb_ctrl_t structure
                : usb_cfg_t     *p_cfg    : Pointer to usb_cfg_t structure
Return value    : usb_err_t error code  : USB_SUCCESS,USB_ERR_NG etc.
******************************************************************************/
usb_err_t R_USB_Open (usb_ctrl_t *p_ctrl, usb_cfg_t *p_cfg)
{
    usb_err_t       err;
    static uint8_t  is_init = USB_NO;

#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
    /* Argument Checking */

    if ((USB_NULL == p_ctrl) || (USB_NULL == p_cfg))
    {
        return USB_ERR_PARA;
    }

#endif /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */

    switch (p_ctrl->type)
    {
        case USB_PCDC:
        case USB_PHID:
        case USB_PVND:
        case USB_PMSC:
#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
            if (USB_PERI != p_cfg->usb_mode)
            {
                return USB_ERR_PARA;
            }
#endif/* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */
            g_usb_cstd_usb_mode = USB_PERI;
        break;

        /* Host */
        case USB_HCDC:
        case USB_HHID:
        case USB_HVND:
        case USB_HMSC:
#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
            if (USB_HOST != p_cfg->usb_mode)
            {
                return USB_ERR_PARA;
            }
#endif/* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */
            g_usb_cstd_usb_mode = USB_HOST;
        break;

        default:
#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
            return USB_ERR_PARA;
#endif/* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */
        break;
    }

    if (USB_NO == is_init)
    {
        g_usb_cstd_open_class = 0;
        memset((void *)&g_usb_cstd_event, 0, sizeof(usb_event_t));
        is_init = USB_YES;
    }
    g_usb_hstd_is_susp_resm = 0;
    memset((void *)&g_usb_read_request_size, 0, ((USB_MAXPIPE_NUM + 1) * 4));

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
    memset((void *)&g_usb_hstd_data, 0, ((USB_MAXPIPE_NUM + 1) * sizeof(usb_hutr_t)));
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
    memset((void *)&g_usb_pstd_data, 0, ((USB_MAXPIPE_NUM + 1) * sizeof(usb_putr_t)));
#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */

    err = usb_module_start();                           /* Start USB module */
    if (USB_SUCCESS != err)
    {
        return USB_ERR_BUSY;
    }

    usb_cpu_usbint_init();                              /* Initialized USB interrupt */

    if (USB_HOST == g_usb_cstd_usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
        /* HCD driver open & registration */
        usb_hstd_driver_init();                             /* HCD task, MGR task open */
        hw_usb_hmodule_init ();                         /* Setting USB relation register  */
#if defined(USB_CFG_HHID_USE)
        if (USB_HHID == p_ctrl->type)
        {
            hw_usb_hset_trnensel();
        }
#endif  /* defined(USB_CFG_HHID_USE) */

#if USB_CFG_TYPEC == USB_CFG_DISABLE
        usb_hstd_vbus_control_on();
  #if USB_CFG_BC == USB_CFG_DISABLE
        usb_cpu_delay_xms((uint16_t)100);               /* 100ms wait */

  #endif /*  USB_CFG_BC == USB_CFG_DISABLE */
#else   /*  USB_CFG_TYPEC == USB_CFG_DISABLE */
        usb_hstd_vbus_control_off();

#endif /*  USB_CFG_TYPEC == USB_CFG_DISABLE */

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
    }
    else
    {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)

#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
        if (USB_NULL == p_cfg->p_usb_reg)
        {
            return USB_ERR_PARA;
        }

#endif  /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */
        usb_pstd_driver_init(p_cfg);            /* USB driver initialization */
        hw_usb_pmodule_init();                      /* Setting USB relation register  */
        if (USB_ATTACH == usb_pstd_chk_vbsts())
        {
            usb_cpu_delay_xms((uint16_t)10);
            hw_usb_pset_dprpu();
        }

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */
    }

    g_usb_cstd_open_class |= (1 << p_ctrl->type);      /* Set USB Open device class */
    if (USB_PCDC == p_ctrl->type)
    {
        g_usb_cstd_open_class |= (1 << USB_PCDCC);   /* Set USB Open device class */
    }
    if (USB_HCDC == p_ctrl->type)
    {
        g_usb_cstd_open_class |= (1 << USB_HCDCC);   /* Set USB Open device class */
    }

    return err;
} /* End of function R_USB_Open */

/******************************************************************************
Function Name   : R_USB_Close
Description     : End of USB Driver.
Arguments       : none
Return value    : usb_err_t error code  : USB_SUCCESS,USB_ERR_NG etc.
******************************************************************************/
usb_err_t R_USB_Close (void)
{
    usb_err_t   ret_code;
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
    uint8_t     devclass = USB_IFCLS_NOT;
#endif

    ret_code = usb_module_stop();

    if (USB_SUCCESS == ret_code)
    {
        if (USB_HOST == g_usb_cstd_usb_mode)
        {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
            if (((g_usb_cstd_open_class & (1 << USB_HCDC)) == (1 << USB_HCDC)) ||
                ((g_usb_cstd_open_class & (1 << USB_HCDCC)) == (1 << USB_HCDCC)))
            {
                devclass = (uint8_t)USB_IFCLS_CDC;
            }
            else if ((g_usb_cstd_open_class & (1 << USB_HHID)) == (1 << USB_HHID))
            {
                devclass = (uint8_t)USB_IFCLS_HID;
            }
            else if ((g_usb_cstd_open_class & (1 << USB_HMSC)) == (1 << USB_HMSC))
            {
                devclass = (uint8_t)USB_IFCLS_MAS;
            }
            else if ((g_usb_cstd_open_class & (1 << USB_HVND)) == (1 << USB_HVND))
            {
                devclass = (uint8_t)USB_IFCLS_VEN;
            }
            else
            {
                /**/
            }

            if (USB_IFCLS_NOT != devclass)
            {
                usb_hstd_driver_release(devclass);
            }
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
        }
        else
        {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)

            usb_pstd_driver_release();   /* Clear the information registered in the structure usb_pcdreg_t. */

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */
        }
        usb_cstd_pipe_table_clear();
    }
    else
    {
        ret_code = USB_ERR_NOT_OPEN;
    }
    /* Clear USB Open device class */
    g_usb_cstd_open_class  = 0;

    return ret_code;
} /* End of function R_USB_Close */


/******************************************************************************
Function Name   : R_USB_Read
Description     : USB Data Receive process.
Arguments       : usb_ctrl_t    *p_ctrl     : Pointer to usb_ctrl_t structure
                : uint8_t       *p_buf        : Pointer to the data buffer areatransfer data address
                : uint32_t      size        : Read size
Return value    : usb_err_t error code      : USB_SUCCESS,USB_ERR_NG etc.
******************************************************************************/
usb_err_t R_USB_Read (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size)
{
    usb_info_t  info;
    usb_er_t    err;

#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
    if (((USB_NULL == p_ctrl) || (USB_NULL == p_buf)) || (USB_NULL == size))
    {
        return USB_ERR_PARA;
    }

    /* Argument Checking */
    if ((USB_PCDCC == p_ctrl->type) || (USB_PVND == p_ctrl->type) || (USB_HVND == p_ctrl->type)
        || (USB_HMSC == p_ctrl->type) || (USB_PMSC == p_ctrl->type))
    {
        return USB_ERR_PARA;
    }

    if (USB_REQUEST != p_ctrl->type)
    {
        if (USB_NULL == (g_usb_cstd_open_class & (1 << p_ctrl->type)))      /* Check USB Open device class */
        {
            return USB_ERR_PARA;
        }
    }

#endif  /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */

    R_USB_GetInformation(&info);
    if (USB_STS_CONFIGURED != info.status)
    {
        return USB_ERR_NG;
    }

    if (USB_REQUEST == p_ctrl->type)
    {
        err = usb_cstd_ctrl_receive(p_ctrl, p_buf, size);
    }
    else
    {
        err = usb_cstd_data_read(p_ctrl, p_buf, size);
    }

    if (USB_OK == err)
    {
        return USB_SUCCESS;
    }
    else if (USB_QOVR == err)
    {
        return USB_ERR_BUSY;
    }
    else
    {
        /* Do Nothing */
    }

    return USB_ERR_NG;
} /* End of function R_USB_Read */


/******************************************************************************
Function Name   : R_USB_Write
Description     : USB Data send process.
Arguments       : usb_ctrl_t    *p_ctrl     : Pointer to usb_ctrl_t structure
                : uint8_t       *p_buf        : Pointer to the data buffer areatransfer data address
                : uint32_t      size        : Write size
Return value    : usb_err_t error code      : USB_SUCCESS,USB_ERR_NG etc.
******************************************************************************/
usb_err_t R_USB_Write (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size)
{
    usb_info_t  info;
    usb_er_t    err;

#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
    if ((USB_NULL == p_ctrl) || ((USB_NULL == p_buf) && (0 != size)))
    {
        return USB_ERR_PARA;
    }

    /* Argument Checking */
    if ((USB_HCDCC == p_ctrl->type) || (USB_PVND == p_ctrl->type) || (USB_HVND == p_ctrl->type)
        || (USB_HMSC == p_ctrl->type) || (USB_PMSC == p_ctrl->type))
    {
        return USB_ERR_PARA;
    }

    if (USB_REQUEST != p_ctrl->type)
    {
        if (USB_NULL == (g_usb_cstd_open_class & (1 << p_ctrl->type)))      /* Check USB Open device class */
        {
            return USB_ERR_PARA;
        }
    }

#endif  /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */

    R_USB_GetInformation(&info);
    if (USB_STS_CONFIGURED != info.status)
    {
        return USB_ERR_NG;
    }

    if (USB_REQUEST == p_ctrl->type)
    {
        err = usb_cstd_ctrl_send(p_ctrl, p_buf, size);
    }
    else
    {
        err = usb_cstd_data_write(p_ctrl, p_buf, size);
    }

    if (USB_OK == err)
    {
        return USB_SUCCESS;
    }
    else if (USB_QOVR == err)
    {
        return USB_ERR_BUSY;
    }
    else
    {
        /* error */
        /* Do Nothing */
    }

    return USB_ERR_NG;
} /* End of function R_USB_Write */


/******************************************************************************
Function Name   : R_USB_Stop
Description     : USB Transfer stop
Arguments       : usb_ctrl_t    *p_ctrl : Pointer to usb_ctrl_t structure
                : uint16_t      type    : Read(0)/Write(1)
Return value    : usb_err_t error code  : USB_SUCCESS,USB_ERR_NG etc.
******************************************************************************/
usb_err_t    R_USB_Stop (usb_ctrl_t *p_ctrl, uint16_t type)
{
    usb_info_t  info;
    usb_er_t    err;

#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
    if (USB_NULL == p_ctrl)
    {
        return USB_ERR_PARA;
    }

    if ((USB_WRITE != type) && (USB_READ != type))
    {
        return USB_ERR_PARA;
    }

    /* Argument Checking */
    if ((USB_PVND == p_ctrl->type) || (USB_HVND == p_ctrl->type)
        || (USB_HMSC == p_ctrl->type) || (USB_PMSC == p_ctrl->type))
    {
        return USB_ERR_PARA;
    }
    
    if (((USB_HCDCC == p_ctrl->type) && (USB_WRITE == type))   ||
        ((USB_PCDCC == p_ctrl->type) && (USB_READ == type)))
    {
        return USB_ERR_PARA;
    }

#endif  /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */

    R_USB_GetInformation(&info);
    if (USB_STS_CONFIGURED != info.status)
    {
        return USB_ERR_NG;
    }

    if (USB_REQUEST == p_ctrl->type)
    {
        err = usb_cstd_ctrl_stop();
    }
    else
    {
        err = usb_cstd_data_stop(p_ctrl, type);
    }

    if (USB_OK == err)
    {
        return USB_SUCCESS;
    }
    else
    {
        /* error */
        /* Do Nothing */
    }

    return USB_ERR_NG;
} /* End of function R_USB_Stop */


/******************************************************************************
Function Name   : R_USB_Suspend
Description     : USB Suspend process for USB Host.
Arguments       : none
Return value    : usb_err_t error code  : USB_SUCCESS,USB_ERR_NG etc.
******************************************************************************/
usb_err_t    R_USB_Suspend (void)
{
#if USB_CFG_MODE == USB_CFG_PERI
    return USB_ERR_NG;              /* Support Host only. */

#else /* USB_CFG_MODE == USB_CFG_PERI */

    usb_err_t   ret_code = USB_SUCCESS;
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
    usb_info_t  info;
    usb_er_t    err;

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

    if (USB_PERI == g_usb_cstd_usb_mode)
    {
        return USB_ERR_NG;              /* Support Host only. */
    }

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)

    R_USB_GetInformation(&info);
    if (USB_STS_CONFIGURED == info.status)
    {
        if (USB_NULL != (g_usb_hstd_is_susp_resm & (1 << USB_STS_SUSPEND)))
        {
            return USB_ERR_BUSY;
        }

        err = usb_hstd_change_device_state ((usb_cbinfo_t)&usb_hstd_suspend_complete, USB_DO_GLOBAL_SUSPEND, 0);
        if (USB_OK == err)
        {
            g_usb_hstd_is_susp_resm |= (1 << USB_STS_SUSPEND);
        }
        else
        {
            ret_code = USB_ERR_NG;
        }
    }
    else if (USB_STS_SUSPEND == info.status)
    {
        return USB_ERR_BUSY;
    }
    else
    {
        return USB_ERR_NG;
    }

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

    return ret_code;

#endif /* USB_CFG_MODE == USB_CFG_PERI */
} /* End of function R_USB_Suspend */


/******************************************************************************
Function Name   : R_USB_Resume
Description     : USB Resume process(Host) and Remote wakeup process(Peri).
Arguments       : none
Return value    : usb_err_t error code  : USB_SUCCESS,USB_ERR_NG etc.
******************************************************************************/
usb_err_t    R_USB_Resume (void)
{
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
    usb_er_t    err;

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
    usb_err_t   ret_code = USB_SUCCESS;
    usb_info_t  info;

    R_USB_GetInformation(&info);
    if (USB_STS_SUSPEND != info.status)
    {
        return USB_ERR_NOT_SUSPEND;
    }

    if (USB_HOST == g_usb_cstd_usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)


        if (USB_NULL != (g_usb_hstd_is_susp_resm & (1 << USB_STS_RESUME)))
        {
            return USB_ERR_BUSY;
        }

        err = usb_hstd_change_device_state ((usb_cbinfo_t)&usb_hstd_resume_complete, USB_DO_GLOBAL_RESUME, 0);
        if (USB_OK == err)
        {
            g_usb_hstd_is_susp_resm |= (1 << USB_STS_RESUME);
        }
        else
        {
            ret_code = USB_ERR_NG;
        }
#else   /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
        ret_code = USB_ERR_NG;
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
    }
    else
    {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
        /* Support remote wakeup ? */
        if (USB_TRUE == g_usb_pstd_remote_wakeup)
        {
            usb_pstd_change_device_state(USB_DO_REMOTEWAKEUP, USB_NULL, (usb_pcb_t)usb_pstd_dummy_function);
            switch (g_usb_pstd_remote_wakeup_state)
            {
                case USB_OK:
                    ret_code = USB_SUCCESS;
                break;

                case USB_QOVR:
                    ret_code = USB_ERR_NOT_SUSPEND;
                break;

                case USB_ERROR:
                default:
                    ret_code = USB_ERR_NG;
                break;
            }
        }
        else
        {
            ret_code = USB_ERR_NG;
        }
#else   /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_PERI */
        ret_code = USB_ERR_NG;
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_PERI */

    }

    return ret_code;
} /* End of function R_USB_Resume */


/******************************************************************************
Function Name   : R_USB_VbusSetting
Description     : Contol of USB VBUS.(USB Host only)
Arguments       : uint16_t      state   : HI/LOW assertion
Return value    : usb_err_t error code  : USB_SUCCESS,USB_ERR_NG etc.
******************************************************************************/
usb_err_t   R_USB_VbusSetting (uint16_t state)
{

#if (USB_CFG_MODE == USB_CFG_PERI)
    return USB_ERR_NG;
#else /* USB_CFG_MODE == USB_CFG_PERI */

    if (USB_PERI == g_usb_cstd_usb_mode) 
    {
        return USB_ERR_NG;              /* Support Host only. */
    }

    /* Argument Checking */

    if ((USB_ON != state) && (USB_OFF != state))
    {
       return USB_ERR_PARA;
    }

    if (USB_ON == state)
    {
        usb_hstd_vbus_control_on();
    }
    else
    {
        usb_hstd_vbus_control_off();
    }

    return USB_SUCCESS;
#endif /* USB_CFG_MODE == USB_CFG_PERI */
} /* End of function R_USB_VbusSetting */


/******************************************************************************
Function Name   : R_USB_PullUp
Description     : Contol of USB D+/D- line pull-up.(USB Peripheral only)
Arguments       : uint16_t      state   : USB_ON(Enable)/USB_OFF(Disable)
Return value    : usb_err_t error code  : USB_SUCCESS,USB_ERR_NG etc.
******************************************************************************/
usb_err_t   R_USB_PullUp(uint8_t state)
{

#if (USB_CFG_MODE == USB_CFG_HOST)
    return USB_ERR_NG;
#else /* USB_CFG_MODE == USB_CFG_HOST */

    if (USB_HOST == g_usb_cstd_usb_mode) 
    {
        return USB_ERR_NG;              /* Support Host only. */
    }

    /* Argument Checking */

    if ((USB_ON != state) && (USB_OFF != state))
    {
       return USB_ERR_PARA;
    }

    if (USB_ON == state)
    {
        hw_usb_pset_dprpu();
    }
    else
    {
        hw_usb_pclear_dprpu();
    }

    return USB_SUCCESS;
#endif /* USB_CFG_MODE == USB_CFG_HOST */
} /* End of function R_USB_GetPipeInfo */
/******************************************************************************
 End of function R_USB_PullUp
 ******************************************************************************/

/******************************************************************************
Function Name   : R_USB_GetInformation
Description     : Get USB Informatio.(Device class,Connect speed,Device status)
Arguments       : usb_info_t    *p_info : Pointer to usb_info_t structure
Return value    : usb_err_t error code  : USB_SUCCESS,USB_ERR_NG etc.
******************************************************************************/
usb_err_t   R_USB_GetInformation (usb_info_t *p_info)
{
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
    uint16_t    status;

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */

    if (0 == g_usb_cstd_open_class)
    {
        return USB_ERR_NG;
    }

#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE

    /* Argument Checking */
    if (USB_NULL == p_info)
    {
        return USB_ERR_PARA;
    }


#endif  /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */

    if (USB_HOST == g_usb_cstd_usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)

        /* Set USB Device class */
        switch (g_usb_hcd_driver.ifclass)
        {
            case USB_IFCLS_CDC:
                p_info->type   = USB_PCDC;
            break;
            case USB_IFCLS_HID:
                p_info->type   = USB_PHID;
            break;
            case USB_IFCLS_MAS:
                p_info->type   = USB_PMSC;
            break;
            case USB_IFCLS_VEN:
                p_info->type   = USB_PVND;
            break;
            default:
                p_info->type   = USB_NULL;
            break;
        }

        /* Set USB connect speed */
        switch(g_usb_hcd_dev_info.speed)
        {
            case USB_NOCONNECT:
                p_info->speed  = USB_NULL;
            break;
            case USB_HSCONNECT:
                p_info->speed  = USB_HS;
            break;
            case USB_FSCONNECT:
                p_info->speed  = USB_FS;
            break;
            case USB_LSCONNECT:
                p_info->speed  = USB_LS;
            break;
            default:
                p_info->speed  = USB_NULL;
            break;
        }

        /* Set USB device state */
        switch (g_usb_hcd_dev_info.state)
        {
            case USB_DO_GLOBAL_SUSPEND:
            case USB_DO_SELECTIVE_SUSPEND:
                p_info->status = USB_STS_SUSPEND;
            break;
            case USB_DO_GLOBAL_RESUME:
            case USB_DO_SELECTIVE_RESUME:
                p_info->status = USB_STS_CONFIGURED;
            break;

            default:
                p_info->status = g_usb_hcd_dev_info.state;
            break;
        }

        /* Set USB Peri BC port state */
#if USB_CFG_BC == USB_CFG_ENABLE
        if (USB_BC_STATE_CDP == g_usb_hstd_bc.state)
        {
            p_info->port   = USB_CDP;
        }
        else
        {
            p_info->port   = USB_SDP;               /* USB_SDP/USB_CDP/USB_DCP */
        }

#else   /* #if USB_CFG_BC == USB_CFG_ENABLE */
        p_info->port   = USB_SDP;  /* USB_SDP/USB_CDP/USB_DCP */

#endif  /* #if USB_CFG_BC == USB_CFG_ENABLE */
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
    }
    else
    {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)

        p_info->type   = usb_pstd_get_open_class();     /* Device class */
        switch (usb_cstd_port_speed())
        {
            case USB_NOCONNECT:
                p_info->speed  = USB_NULL;
            break;
            case USB_HSCONNECT:
                p_info->speed  = USB_HS;
            break;
            case USB_FSCONNECT:
                p_info->speed  = USB_FS;
            break;
            case USB_LSCONNECT:
                p_info->speed  = USB_LS;
            break;
            default:
                p_info->speed  = USB_NULL;
            break;
        }
        status = hw_usb_read_intsts();
        switch ((uint16_t)(status & USB_DVSQ))
        {
            case USB_DS_POWR:                           /* Power state  */
                p_info->status = USB_STS_DETACH;
            break;
            case USB_DS_DFLT:                           /* Default state  */
                p_info->status = USB_STS_DEFAULT;
            break;
            case USB_DS_ADDS:                           /* Address state  */
                p_info->status = USB_STS_ADDRESS;
            break;
            case USB_DS_CNFG:                           /* Configured state  */
                p_info->status = USB_STS_CONFIGURED;
            break;
            case USB_DS_SPD_POWR:                       /* Power suspend state */
            case USB_DS_SPD_DFLT:                       /* Default suspend state */
            case USB_DS_SPD_ADDR:                       /* Address suspend state */
            case USB_DS_SPD_CNFG:                       /* Configured Suspend state */
                p_info->status = USB_STS_SUSPEND;
            break;
            default:                                    /* Error */
            break;                                  /* p_info->status = USB_STS_ERROR; */
        }
#if USB_CFG_BC == USB_CFG_ENABLE
        p_info->port   = g_usb_pstd_bc_detect;           /* USB_SDP/USB_CDP/USB_DCP */
#else   /* USB_CFG_BC == USB_CFG_ENABLE */
        p_info->port   = USB_SDP;                   /* USB_SDP */
#endif  /* USB_CFG_BC == USB_CFG_ENABLE */

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */
    }
    return USB_SUCCESS;
} /* End of function R_USB_GetInformation */


/******************************************************************************
Function Name   : R_USB_PipeRead
Description     : USB Data receive process for Vendor class.
Arguments       : usb_ctrl_t    *p_ctrl : Pointer to usb_ctrl_t structure
                : uint8_t       *p_buf  : Pointer to the data buffer area
                : uint32_t      size    : Read size
Return value    : usb_err_t error code  : USB_SUCCESS,USB_ERR_NG etc.
******************************************************************************/
usb_err_t  R_USB_PipeRead (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size)
{
#if !defined(USB_CFG_HVND_USE) && !defined(USB_CFG_PVND_USE)
    return USB_ERR_NG;

#else   /* !defined(USB_CFG_HVND_USE) && !defined(USB_CFG_PVND_USE) */
    usb_err_t   ret_code = USB_ERR_NG;
    usb_er_t    err;
    usb_info_t  info;

#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
    /* Argument Checking */
    if (((USB_NULL == p_ctrl) || (USB_PIPE0 == p_ctrl->pipe)) || (USB_MAXPIPE_NUM < p_ctrl->pipe))
    {
        return USB_ERR_PARA;
    }

    if ((USB_NULL == p_buf) || (USB_NULL == size))
    {
        return USB_ERR_PARA;
    }

    if (USB_NULL == (g_usb_cstd_open_class & (1 << p_ctrl->type)))           /* Check USB Open device class */
    {
        return USB_ERR_PARA;
    }

#endif  /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */

    R_USB_GetInformation(&info);
    if (USB_STS_CONFIGURED != info.status)
    {
        return USB_ERR_NG;
    }

    /* PIPE Transfer set */
    if (USB_HOST == g_usb_cstd_usb_mode)
    {
#if !defined(USB_CFG_HVND_USE)
        return USB_ERR_NG;
#else /* !defined(USB_CFG_HVND_USE) */
        g_usb_hstd_data[p_ctrl->pipe].pipenum     = p_ctrl->pipe;           /* Pipe No */
        g_usb_hstd_data[p_ctrl->pipe].p_tranadr   = p_buf;                  /* Data address */
        g_usb_hstd_data[p_ctrl->pipe].tranlen     = size;                   /* Data Size */
        g_usb_hstd_data[p_ctrl->pipe].setup       = USB_NULL;
        g_usb_hstd_data[p_ctrl->pipe].complete    = usb_hstd_read_complete;      /* Callback function */
        g_usb_read_request_size[p_ctrl->pipe] = size;
        err = usb_hstd_transfer_start(&g_usb_hstd_data[p_ctrl->pipe]);      /* USB Transfer Start */

#endif /* defined(USB_CFG_HVND_USE) */
    }
    else
    {
#if !defined(USB_CFG_PVND_USE)
        return USB_ERR_NG;
#else /* !defined(USB_CFG_PVND_USE) */
        g_usb_pstd_data[p_ctrl->pipe].keyword    = p_ctrl->pipe;            /* Pipe No */
        g_usb_pstd_data[p_ctrl->pipe].p_tranadr  = p_buf;                   /* Data address */
        g_usb_pstd_data[p_ctrl->pipe].tranlen    = size;                    /* Data Size */
        g_usb_pstd_data[p_ctrl->pipe].complete   = (usb_pcb_t)&usb_pstd_read_complete;  /* Callback function */

        g_usb_read_request_size[p_ctrl->pipe] = size;
        err = usb_pstd_transfer_start(&g_usb_pstd_data[p_ctrl->pipe]);      /* USB Transfer Start */


#endif /* defined(USB_CFG_PVND_USE) */
    }

    if (USB_OK == err)
    {
        ret_code = USB_SUCCESS;
    }
    else if (USB_QOVR == err)
    {
        ret_code = USB_ERR_BUSY;
    }
    else
    {
        ret_code = USB_ERR_NG;
    }

    return ret_code;
#endif   /* !defined(USB_CFG_HVND_USE) && !defined(USB_CFG_PVND_USE) */
} /* End of function R_USB_PipeRead */


/******************************************************************************
Function Name   : R_USB_PipeWrite
Description     : USB Data send process for Vendor class.
Arguments       : usb_ctrl_t    *p_ctrl : Pointer to usb_ctrl_t structure
                : uint8_t       *p_buf  : Pointer to the data buffer area
                : uint32_t      size    : Write size
Return value    : usb_err_t error code  : USB_SUCCESS,USB_ERR_NG etc.
******************************************************************************/
usb_err_t  R_USB_PipeWrite (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size)
{
#if !defined(USB_CFG_HVND_USE) && !defined(USB_CFG_PVND_USE)
    return USB_ERR_NG;

#else
    usb_err_t   ret_code = USB_SUCCESS;
    usb_er_t    err;
    usb_info_t  info;
    
#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
        /* Argument Checking */
    if (((USB_NULL == p_ctrl) || (USB_PIPE0 == p_ctrl->pipe)) || (USB_MAXPIPE_NUM < p_ctrl->pipe))
    {
        return USB_ERR_PARA;
    }

    if ((USB_NULL == p_buf) && (USB_NULL != size))
    {
        return USB_ERR_PARA;
    }

    if (USB_NULL == (g_usb_cstd_open_class & (1 << p_ctrl->type)))           /* Check USB Open device class */
    {
        return USB_ERR_PARA;
    }

#endif  /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */

    R_USB_GetInformation(&info);
    if (USB_STS_CONFIGURED != info.status)
    {
        return USB_ERR_NG;
    }

    /* PIPE Transfer set */
    if (USB_HOST == g_usb_cstd_usb_mode)
    {
#if !defined(USB_CFG_HVND_USE)
        return USB_ERR_NG;

#else /* !defined(USB_CFG_HVND_USE) */
        g_usb_hstd_data[p_ctrl->pipe].pipenum     = p_ctrl->pipe;           /* Pipe No */
        g_usb_hstd_data[p_ctrl->pipe].p_tranadr   = p_buf;                  /* Data address */
        g_usb_hstd_data[p_ctrl->pipe].tranlen     = size;                   /* Data Size */
        g_usb_hstd_data[p_ctrl->pipe].setup       = USB_NULL;
        g_usb_hstd_data[p_ctrl->pipe].complete    = usb_hstd_write_complete;     /* Callback function */

        err = usb_hstd_transfer_start(&g_usb_hstd_data[p_ctrl->pipe]);      /* USB Transfer Start */

#endif  /* !defined(USB_CFG_HVND_USE) */
    }
    else
    {
#if !defined(USB_CFG_PVND_USE)
        return USB_ERR_NG;

#else /* !defined(USB_CFG_PVND_USE) */
        g_usb_pstd_data[p_ctrl->pipe].keyword    = p_ctrl->pipe;            /* Pipe No */
        g_usb_pstd_data[p_ctrl->pipe].p_tranadr  = p_buf;                   /* Data address */
        g_usb_pstd_data[p_ctrl->pipe].tranlen    = size;                    /* Data Size */
        g_usb_pstd_data[p_ctrl->pipe].complete   = (usb_pcb_t)&usb_pstd_write_complete; /* Callback function */

        err = usb_pstd_transfer_start(&g_usb_pstd_data[p_ctrl->pipe]);      /* USB Transfer Start */
#endif  /* defined(USB_CFG_PVND_USE) */
    }

    if (USB_OK == err)
    {
        ret_code = USB_SUCCESS;
    }
    else if (USB_QOVR == err)
    {
        ret_code = USB_ERR_BUSY;
    }
    else
    {
        ret_code = USB_ERR_NG;
    }

    return ret_code;
#endif /* #if !defined(USB_CFG_HVND_USE) && !defined(USB_CFG_PVND_USE) */

} /* End of function R_USB_PipeWrite */


/******************************************************************************
Function Name   : R_USB_PipeStop
Description     : USB transfer stop process for Vendor class.
Arguments       : usb_ctrl_t    *p_ctrl : Pointer to usb_ctrl_t structure
Return value    : usb_err_t error code  : USB_SUCCESS,USB_ERR_NG etc.
******************************************************************************/
usb_err_t    R_USB_PipeStop (usb_ctrl_t *p_ctrl)
{
#if !defined(USB_CFG_HVND_USE) && !defined(USB_CFG_PVND_USE)
    return USB_ERR_NG;
#else

#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
    usb_er_t    err;
#endif /* ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI) */
    usb_err_t   ret_code = USB_ERR_NG;
    usb_info_t  info;

#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
    /* Argument Checking */
    if ((USB_NULL == p_ctrl) || ((USB_PIPE0 == p_ctrl->pipe) || (USB_MAXPIPE_NUM < p_ctrl->pipe)))
    {
        return USB_ERR_PARA;
    }

    if (USB_NULL == (g_usb_cstd_open_class & (1 << p_ctrl->type)))      /* Check USB Open device class */
    {
        return USB_ERR_PARA;
    }

#endif  /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */

    R_USB_GetInformation(&info);
    if (USB_STS_CONFIGURED != info.status)
    {
        return USB_ERR_NG;
    }

    if (USB_HOST == g_usb_cstd_usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
        g_usb_hstd_current_pipe = p_ctrl->pipe;
        usb_hstd_forced_termination(USB_DATA_STOP);
        ret_code = USB_SUCCESS;

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
    }
    else
    {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
        err = usb_pstd_transfer_end(p_ctrl->pipe);
        if (USB_OK == err)
        {
            ret_code = USB_SUCCESS;
        }

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */
    }

    return ret_code;
#endif /* !defined(USB_CFG_HVND_USE) && !defined(USB_CFG_PVND_USE) */
} /* End of function R_USB_PipeStop */


/******************************************************************************
Function Name   : R_USB_GetUsePipe
Description     : Get Information for Use pipe.
Arguments       : uint16_t      *p_pipe : Pointer to the area to store the used pipe bitmap
Return value    : usb_err_t error code  : USB_SUCCESS,USB_ERR_NG etc.
******************************************************************************/
usb_err_t    R_USB_GetUsePipe (uint16_t *p_pipe)
{
    usb_info_t  info;
    uint16_t    pipe_no;

#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
    if (USB_NULL == p_pipe)
    {
        return USB_ERR_PARA;
    }

#endif /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */

    R_USB_GetInformation(&info);
    if (USB_STS_CONFIGURED != info.status)
    {
        return USB_ERR_NG;
    }

    /* Get PIPE Number from Endpoint address */
    *p_pipe = ((uint16_t)1 << USB_PIPE0);
    /* WAIT_LOOP */
    for (pipe_no = USB_MIN_PIPE_NO; pipe_no < (USB_MAX_PIPE_NO +1); pipe_no++)
    {
        if (USB_TRUE == g_usb_cstd_pipe_tbl[pipe_no].use_flag)
        {
             (*p_pipe) |= ((uint16_t)1 << pipe_no);
        }
    }

    return USB_SUCCESS;
} /* End of function R_USB_GetUsePipe */


/******************************************************************************
Function Name   : R_USB_GetPipeInfo
Description     : Get Infomation for pipe setting.(DIR,Transfer Type,MAXPS)
Arguments       : usb_ctrl_t    *p_ctrl : Pointer to usb_ctrl_t structure
                : usb_pipe_t    *p_info : Pointer to the area to store the pipe information
Return value    : usb_err_t error code  : USB_SUCCESS,USB_ERR_NG etc.
******************************************************************************/
usb_err_t    R_USB_GetPipeInfo (usb_ctrl_t *p_ctrl, usb_pipe_t *p_info)
{
    usb_info_t  info;
    uint16_t    pipe_type;

#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
    /* Argument Checking */
    if (((USB_NULL == p_ctrl) || (USB_NULL == p_info)) || ((0 == p_ctrl->pipe) || (USB_MAXPIPE_NUM < p_ctrl->pipe)))
    {
        return USB_ERR_PARA;
    }

#endif  /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */


    R_USB_GetInformation(&info);
    if (USB_STS_CONFIGURED != info.status)
    {
        return USB_ERR_NG;
    }

    if (USB_HOST == g_usb_cstd_usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)

        g_usb_hstd_current_pipe = p_ctrl->pipe;

        p_info->ep = usb_hstd_pipe_to_epadr();
        pipe_type = usb_cstd_get_pipe_type(p_ctrl->pipe);

        switch (pipe_type)
        {
            case USB_TYPFIELD_ISO: 
                p_info->type = USB_ISO;                              /* Set Isochronous */
            break;            
            case USB_TYPFIELD_BULK:
                p_info->type = USB_BULK;                             /* Set Bulk */
            break;            
            case USB_TYPFIELD_INT:
                p_info->type = USB_INT;                              /* Set Interrupt */
            break;
            default:
                return USB_ERR_NG;
            break;
        }
        p_info->mxps = usb_cstd_get_maxpacket_size(p_ctrl->pipe);
        return USB_SUCCESS;

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
    }
    else
    {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
        /* Get PIPE Number from Endpoint address */
        p_info->ep   = (uint8_t)(g_usb_cstd_pipe_tbl[p_ctrl->pipe].pipe_cfg & USB_EPNUMFIELD);  /* Set EP num. */
        if (USB_DIR_P_IN == (g_usb_cstd_pipe_tbl[p_ctrl->pipe].pipe_cfg & USB_DIRFIELD))        /* Check dir */
        {
            p_info->ep |= USB_EP_DIR_IN;                            /* Set DIR IN */
        }
        pipe_type = usb_cstd_get_pipe_type(p_ctrl->pipe);
        switch (pipe_type)
        {
            case USB_TYPFIELD_ISO:
                p_info->type = USB_ISO;                                 /* Set Isochronous */
            break;
            
            case USB_TYPFIELD_BULK:
                p_info->type = USB_BULK;                                /* Set Bulk */
            break;
            
            case USB_TYPFIELD_INT:
                p_info->type = USB_INT;                                 /* Set Interrupt */
            break;
            
            default:
                return USB_ERR_NG;
            break;
        }
        p_info->mxps = usb_cstd_get_maxpacket_size(p_ctrl->pipe); /* Set Max packet size */
        return USB_SUCCESS;

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */
    }
    return USB_ERR_NG;
} /* End of function R_USB_GetPipeInfo */

/******************************************************************************
End  Of File
******************************************************************************/
