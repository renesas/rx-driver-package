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
* Copyright (C) 2016(2024) Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_usb_usbif_api.c
* Description  : USB Host and Peripheral Driver API code. HCD(Host Control Driver) PCD (Peripheral Control Driver)
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 30.09.2015 1.00 First Release
*         : 30.09.2017 1.22 Update Argument Checking
*         : 30.11.2018 1.10 Supporting Smart Configurator
*         : 31.05.2019 1.11 Added support for GNUC and ICCRX.
*         : 30.06.2020 1.20 Added support for RTOS.
*         : 30.04.2024 1.30 Added support for RX261.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <string.h>

#include "r_usb_basic_mini_if.h"
#include "r_usb_bitdefine.h"
#include "r_usb_reg_access.h"
#include "r_usb_typedef.h"
#include "r_usb_extern.h"
#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
#include "r_usb_cstd_rtos.h"
#endif /* (BSP_CFG_RTOS_USED != 0) */

#if USB_CFG_DTC == USB_CFG_ENABLE
#include "r_dtc_rx_if.h"
#endif /* USB_CFG_DTC == USB_CFG_ENABLE */

#if USB_CFG_DMA == USB_CFG_ENABLE
#include "r_dmaca_rx_if.h"
#endif /* USB_CFG_DMA == USB_CFG_ENABLE */

#if (BSP_CFG_RTOS_USED == 4)        /* Renesas RI600V4 & RI600PX */
#include "r_usb_cstd_rtos.h"
#endif /* (BSP_CFG_RTOS_USED == 4) */

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
 
/***********************************************************************************************************************
 Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
#if (BSP_CFG_RTOS_USED != 0)    /* Non-OS */
    usb_ctrl_t      g_usb_cstd_event[USB_EVENT_MAX];
    usb_callback_t  *g_usb_apl_callback;
    rtos_task_id_t  g_usb_default_apl_task_id;
#else /*(BSP_CFG_RTOS_USED != 0)*/
    usb_event_t     g_usb_cstd_event;
    uint16_t        g_usb_hstd_is_susp_resm;
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
usb_hutr_t          g_usb_hstd_data[USB_MAXPIPE_NUM + 1];
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
#endif /*(BSP_CFG_RTOS_USED != 0)*/

#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
usb_putr_t       g_usb_pstd_data[USB_MAXPIPE_NUM + 1];
uint16_t         g_usb_pstd_speed;

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */

uint16_t        g_usb_cstd_usb_mode;
uint16_t        g_usb_cstd_open_class;

/***********************************************************************************************************************
 Private global variables and functions
***********************************************************************************************************************/
static uint8_t  is_init = USB_NO;

#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
static uint8_t  gs_usb_suspend_ing = USB_NO;
#endif /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

static uint8_t  gs_usb_resume_ing = USB_NO;
#endif /*(BSP_CFG_RTOS_USED != 0)*/

/***********************************************************************************************************************
* Function Name: R_USB_GetVersion
 *******************************************************************************************************************//**
 * @brief Return API version number
 * @retval Version number
 * @details The version number of the USB driver is returned.
 */
uint32_t R_USB_GetVersion (void)
{
    uint32_t version = 0;

    version = (USB_VERSION_MAJOR << 16) | USB_VERSION_MINOR;

    return version;
}
/***********************************************************************************************************************
 End of function R_USB_GetVersion
***********************************************************************************************************************/

#if (BSP_CFG_RTOS_USED != 0)    /* Use RTOS */
/***********************************************************************************************************************
 * Function Name   : R_USB_Callback
 *******************************************************************************************************************//**
 * @brief Register a callback function to be called upon completion of a USB-related event. (RTOS only)
 * @param[in]  p_callback  Pointer to the callback function
 * @details This function registers a callback function to be called when a USB-related event has completed.
 * When a USB-related event has completed, the USB driver will call the callback function that has been registered 
 * using this API.
 * @note This API is not supported when using RX100/RX200 series MCU.
 */
void    R_USB_Callback(usb_callback_t *p_callback)
{
    g_usb_apl_callback = p_callback;
} /* End of function R_USB_Callback() */

#else   /* (BSP_CFG_RTOS_USED != 0)*/
/***********************************************************************************************************************
 * Function Name   : R_USB_GetEvent
 *******************************************************************************************************************//**
 * @brief Get completed USB-related events (Non-OS only)
 * @param[in]    p_ctrl  Pointer to usb_ctrl_t structure area
 * @retval       Value of completed USB-related events
 * @details This function obtains completed USB-related events.
 * In USB host mode, the device address value of the USB device that completed 
 * an event is specified in the usb_ctrl_t structure member (address) specified 
 * by the event's argument. In USB peripheral mode, USB_NULL is specified in member (address). 
 */
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
#endif  /* (BSP_CFG_RTOS_USED != 0)*/

/***********************************************************************************************************************
 * Function Name   : R_USB_Open
 *******************************************************************************************************************//**
 * @brief Power on the USB module and initialize the USB driver. 
 * (This is a function to be used first when using the USB module.)
 * @param[in]  ctrl              Pointer to usb_ctrl_t structure area
 * @param[in]  cfg               Pointer to usb_cfg_t structure area
 * @retval     USB_SUCCESS       Success
 * @retval     USB_ERR_PARA      Parameter error
 * @retval     USB_ERR_BUSY      Specified USB module now in use
 * @details This function applies power to the USB module specified in the argument (p_ctrl).
 */
usb_err_t R_USB_Open (usb_ctrl_t *p_ctrl, usb_cfg_t *p_cfg)
{
    usb_err_t       err;
#if USB_CFG_DTC == USB_CFG_ENABLE
    dtc_err_t   ret;

#endif  /* USB_CFG_DTC == USB_CFG_ENABLE */

#if USB_CFG_DMA == USB_CFG_ENABLE
    dmaca_return_t ret;

#endif  /* USB_CFG_DMA == USB_CFG_ENABLE */

#if (BSP_CFG_RTOS_USED > 0)
    usb_rtos_configuration();
#endif /* (BSP_CFG_RTOS_USED > 1) */

#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
    /* Argument Checking */

    if ((USB_NULL == p_ctrl) || (USB_NULL == p_cfg))
    {
        return USB_ERR_PARA;
    }

    switch (p_ctrl->type)
    {
        case USB_PCDC:
        case USB_PMSC:
        case USB_PCDC_PHID:
        case USB_PCDC_PMSC:
        case USB_PHID_PMSC:
            if (USB_PERI != p_cfg->usb_mode)
            {
                return USB_ERR_PARA;
            }
            if (USB_NULL == p_cfg->p_usb_reg)
            {
                return USB_ERR_PARA;
            }
            if (USB_FS != p_cfg->usb_speed)
            {
                return USB_ERR_PARA;
            }
        break;

        case USB_PHID:
        case USB_PVND:
            if (USB_PERI != p_cfg->usb_mode)
            {
                return USB_ERR_PARA;
            }
            if (USB_NULL == p_cfg->p_usb_reg)
            {
                return USB_ERR_PARA;
            }
            if ((USB_FS != p_cfg->usb_speed) && (USB_LS != p_cfg->usb_speed))
            {
                return USB_ERR_PARA;
            }
        break;

        /* Host */
        case USB_HCDC:
        case USB_HHID:
        case USB_HVND:
        case USB_HMSC:
            if (USB_HOST != p_cfg->usb_mode)
            {
                return USB_ERR_PARA;
            }
            if (USB_FS != p_cfg->usb_speed)
            {
                return USB_ERR_PARA;
            }
        break;

        default:
            return USB_ERR_PARA;
        break;
    }
#endif /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */

    if (USB_YES == is_init)
    {
        return USB_ERR_BUSY;
    }

    g_usb_cstd_open_class = 0;
    memset((void *)&g_usb_cstd_event, 0, sizeof(usb_event_t));
    is_init = USB_YES;

#if (BSP_CFG_RTOS_USED == 0)
    g_usb_hstd_is_susp_resm = 0;
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
    memset((void *)&g_usb_hstd_data, 0, ((USB_MAXPIPE_NUM + 1) * sizeof(usb_hutr_t)));
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
#endif /* BSP_CFG_RTOS_USED == 0 */

#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
    memset((void *)&g_usb_pstd_data, 0, ((USB_MAXPIPE_NUM + 1) * sizeof(usb_putr_t)));
#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */

    err = usb_module_start();                           /* Start USB module */
    if (USB_SUCCESS != err)
    {
        return USB_ERR_BUSY;
    }

#if USB_CFG_DTC == USB_CFG_ENABLE
    ret = R_DTC_Open();
    if (DTC_SUCCESS != ret)
    {
#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
        return USB_ERR_PARA;
#else /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */
        return USB_ERR_NG;
#endif/* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */
    }

#endif  /* USB_CFG_DTC == USB_CFG_ENABLE */

#if USB_CFG_DMA == USB_CFG_ENABLE
    R_DMACA_Init();

#if USB_CFG_NOUSE != USB_CFG_USB0_DMA_TX
    ret = R_DMACA_Open(USB_CFG_USB0_DMA_TX);
    if (DMACA_SUCCESS != ret)
    {
#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
        return USB_ERR_PARA;
#else /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */
        return USB_ERR_NG;
#endif/* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */
    }
#endif  /* USB_CFG_NOUSE != USB_CFG_USB0_DMA_TX */

#if USB_CFG_NOUSE != USB_CFG_USB0_DMA_RX
    ret = R_DMACA_Open(USB_CFG_USB0_DMA_RX);
    if (DMACA_SUCCESS != ret)
    {
#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
        return USB_ERR_PARA;
#else /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */
        return USB_ERR_NG;
#endif/* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */
    }
#endif  /* USB_CFG_NOUSE != USB_CFG_USB0_DMA_RX */

#endif  /* USB_CFG_DMA == USB_CFG_ENABLE */

    usb_cpu_usbint_init();                              /* Initialized USB interrupt */

    if (USB_HOST == p_cfg->usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
        g_usb_cstd_usb_mode = USB_HOST;
        /* HCD driver open & registration */
        usb_hstd_driver_init();                         /* USB driver initialization */
        hw_usb_hmodule_init ();                         /* Setting USB relation register  */

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
        g_usb_cstd_usb_mode = USB_PERI;
        g_usb_pstd_speed = p_cfg->usb_speed;

        usb_pstd_driver_init(p_cfg);            /* USB driver initialization */
        hw_usb_pmodule_init();                      /* Setting USB relation register  */
        if (USB_ATTACH == usb_pstd_chk_vbsts())
        {
            usb_cpu_delay_xms((uint16_t)10);
            if (USB_LS  == g_usb_pstd_speed)
            {
                hw_usb_pset_dmrpu();
            }
            else
            {
                hw_usb_pset_dprpu();
            }
        }
#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */
    }

    if (USB_SUCCESS == err)
    {
#if (BSP_CFG_RTOS_USED != 0)
        rtos_get_task_id(&g_usb_default_apl_task_id);
#endif /* BSP_CFG_RTOS_USED != 0 */

        if (USB_PCDC_PHID == p_ctrl->type)
        {
            g_usb_cstd_open_class |= (1 << USB_PCDC);      /* Set USB Open device class */
            g_usb_cstd_open_class |= (1 << USB_PCDCC);     /* Set USB Open device class */
            g_usb_cstd_open_class |= (1 << USB_PHID);      /* Set USB Open device class */
        }
        if (USB_PCDC_PMSC == p_ctrl->type)
        {
            g_usb_cstd_open_class |= (1 << USB_PCDC);      /* Set USB Open device class */
            g_usb_cstd_open_class |= (1 << USB_PCDCC);     /* Set USB Open device class */
            g_usb_cstd_open_class |= (1 << USB_PMSC);      /* Set USB Open device class */
        }
        if (USB_PHID_PMSC == p_ctrl->type)
        {
            g_usb_cstd_open_class |= (1 << USB_PHID);      /* Set USB Open device class */
            g_usb_cstd_open_class |= (1 << USB_PMSC);      /* Set USB Open device class */
        }
        else
        {
            g_usb_cstd_open_class |= (1 << p_ctrl->type);    /* Set USB Open device class */
            if (USB_PCDC == p_ctrl->type)
            {
                g_usb_cstd_open_class |= (1 << USB_PCDCC);     /* Set USB Open device class */
#if defined(USB_CFG_PCDC_2COM_USE)
                g_usb_cstd_open_class |= (1 << USB_PCDC2);     /* Set USB Open device class */
                g_usb_cstd_open_class |= (1 << USB_PCDCC2);    /* Set USB Open device class */
#endif /* defined(USB_CFG_PCDC_2COM_USE) */
            }
            if (USB_HCDC == p_ctrl->type)
            {
                g_usb_cstd_open_class |= (1 << USB_HCDCC);     /* Set USB Open device class */
            }
            if (USB_PHID == p_ctrl->type)
            {
                g_usb_cstd_open_class |= (1 << USB_PHID2);     /* Set USB Open device class */
            }
        }
    }

    return err;
}
/***********************************************************************************************************************
 End of function R_USB_Open
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name   : R_USB_Close
 *******************************************************************************************************************//**
 * @brief Power off USB module.
 * @param[in]  p_ctrl              Pointer to usb_ctrl_t structure area
 * @retval     USB_SUCCESS         Success
 * @retval     USB_ERR_PARA        Parameter error
 * @retval     USB_ERR_NOT_OPEN    USB module is not open.
 * @details This function applies power to the USB module specified in the argument (p_ctrl).
 * @note The argument (p_ctrl) is not supported when using RX100/RX200 series MCU.
 */
usb_err_t R_USB_Close (void)
{
    usb_err_t   ret_code;
#if (BSP_CFG_RTOS_USED == 0)   /* nonOS */
    uint16_t    i;

#endif /* BSP_CFG_RTOS_USED == 0 nonOS */

#if USB_CFG_DTC == USB_CFG_ENABLE
    dtc_err_t   ret;

#endif  /* USB_CFG_DTC == USB_CFG_ENABLE */

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
    uint8_t     devclass = USB_IFCLS_NOT;
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */

#if (BSP_CFG_RTOS_USED > 0)
    usb_rtos_unconfiguration();
#endif /* #if (BSP_CFG_RTOS_USED > 0) */

  #if (BSP_CFG_RTOS_USED == 0)   /* nonOS */
    /* USB Event Dummy Read */
    for (i =0; i <10; i++)
    {
        usb_cstd_usb_task();
    }
  #endif /* BSP_CFG_RTOS_USED == 0   nonOS */

#if USB_CFG_DTC == USB_CFG_ENABLE
    ret = R_DTC_Close();
    if (DTC_SUCCESS != ret)
    {
#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
        return USB_ERR_PARA;
#else /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */
        return USB_ERR_NG;
#endif/* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */
    }

#endif  /* USB_CFG_DTC == USB_CFG_ENABLE */

#if USB_CFG_DMA == USB_CFG_ENABLE

#if USB_CFG_NOUSE != USB_CFG_USB0_DMA_TX
    R_DMACA_Close(USB_CFG_USB0_DMA_TX);
#endif  /* USB_CFG_NOUSE != USB_CFG_USB0_DMA_TX */

#if USB_CFG_NOUSE != USB_CFG_USB0_DMA_RX
    R_DMACA_Close(USB_CFG_USB0_DMA_RX);
#endif  /* USB_CFG_NOUSE != USB_CFG_USB0_DMA_RX */

#endif  /* USB_CFG_DMA == USB_CFG_ENABLE */

    ret_code = usb_module_stop();
    if (USB_SUCCESS == ret_code)
    {
        is_init = USB_NO;

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

  #if (BSP_CFG_RTOS_USED == 0)   /* nonOS */
    /* USB Event Dummy Read */
    for (i =0; i <10; i++)
    {
        usb_cstd_usb_task();
    }
  #endif /* BSP_CFG_RTOS_USED == 0    nonOS */

    return ret_code;
}
/***********************************************************************************************************************
 End of function R_USB_Close
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name   : R_USB_Read
 *******************************************************************************************************************//**
 * @brief  USB data read request.
 * @param[in]  p_ctrl            Pointer to usb_ctrl_t structure area
 * @param[out] p_buf             Pointer to area that stores read data
 * @param[in]  size              Read request size
 * @retval     USB_SUCCESS       Successfully completed (Data read request completed)
 * @retval     USB_ERR_PARA      Parameter error
 * @retval     USB_ERR_BUSY      Data receive request already in process for USB device with same device address
 * @retval     USB_ERR_NG        Other error
 * @details Requests USB data read.The read data is stored in the area specified by argument (p_buf)
 */
usb_err_t R_USB_Read (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size)
{
    usb_info_t  info;
    usb_er_t    err;
    usb_err_t   result = USB_ERR_NG;

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
    if (USB_STS_CONFIGURED == info.status)
    {
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
            result = USB_SUCCESS;
    }
    else if (USB_QOVR == err)
    {
            result = USB_ERR_BUSY;
    }
    else
    {
        /* Do Nothing */
    }
    }
    return result;
}
/***********************************************************************************************************************
 End of function R_USB_Read
***********************************************************************************************************************/


/***********************************************************************************************************************
 * Function Name   : R_USB_Write
 *******************************************************************************************************************//**
 * @brief  USB data write request.
 * @param[in]  p_ctrl          Pointer to usb_ctrl_t structure area
 * @param[in]  p_buf           Pointer to area that stores read data
 * @param[in]  size            Write request size
 * @retval     USB_SUCCESS     Successfully completed (Data write request completed)
 * @retval     USB_ERR_PARA    Parameter error
 * @retval     USB_ERR_BUSY    Data write request already in process for USB device with same device address
 * @retval     USB_ERR_NG      Other error
 * @details    Requests USB data write.Stores write data in area specified by argument (p_buf).
 */
usb_err_t R_USB_Write (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size)
{
    usb_info_t  info;
    usb_er_t    err;
    usb_err_t   result = USB_ERR_NG;

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
    if (USB_STS_CONFIGURED == info.status)
    {
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
            result = USB_SUCCESS;
    }
    else if (USB_QOVR == err)
    {
            result = USB_ERR_BUSY;
    }
    else
    {
        /* error */
        /* Do Nothing */
    }
    }
    return result;
}
/***********************************************************************************************************************
 End of function R_USB_Write
***********************************************************************************************************************/


/***********************************************************************************************************************
 * Function Name   : R_USB_Stop
 *******************************************************************************************************************//**
 * @brief USB data read/write stop request
 * @param[in]  p_ctrl          Pointer to usb_ctrl_t structure area
 * @param[in]  type            Receive (USB_READ) or send (USB_WRITE)
 * @retval     USB_SUCCESS     Successfully completed (stop completed)
 * @retval     USB_ERR_PARA    Parameter error
 * @retval     USB_ERR_NG      Other error
 * @details This function is used to request a data read/write transfer be terminated when a data read/write transfer 
 * is performing. To stop a data read, set USB_READ as the argument (type); to stop a data write, 
 * specify USB_WRITE as the argument (type).
 */
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
}
/***********************************************************************************************************************
 End of function R_USB_Stop
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name   : R_USB_Suspend
 *******************************************************************************************************************//**
 * @brief Suspend signal transmission
 * @param[in]  p_ctrl   Pointer to usb_ctrl_t structure area
 * @retval     USB_SUCCESS     Successfully completed
 * @retval     USB_ERR_PARA    Parameter error
 * @retval     USB_ERR_BUSY    During a suspend request to the specified USB module, or when the USB module is already 
 * in the suspended state 
 * @retval     USB_ERR_NG      Other error
 * @details This function sends a SUSPEND signal from the USB module assigned to the member (module) 
 * of the usb_crtl_t structure.
 * @note The argument (p_ctrl) is not supported when using RX100/RX200 series MCU.
 */
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
#if (BSP_CFG_RTOS_USED != 0)    /* RTOS */
        if (USB_YES == gs_usb_suspend_ing)
        {
            return USB_ERR_BUSY;
        }
        else
        {
            gs_usb_suspend_ing = USB_YES;
        }

        err = usb_hstd_change_device_state((usb_cb_t)&usb_hstd_suspend_complete,
                                           USB_DO_GLOBAL_SUSPEND, USB_DEVICEADDR);

        if (USB_OK != err)
        {
            ret_code = USB_ERR_NG;
        }

        gs_usb_suspend_ing = USB_NO;
#else   /* (BSP_CFG_RTOS_USED != 0) */
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
#endif  /* (BSP_CFG_RTOS_USED != 0) */
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
}
/***********************************************************************************************************************
 End of function R_USB_Suspend
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name   : R_USB_Resume
 *******************************************************************************************************************//**
 * @brief Resume signal transmission
 * @param[in]  p_ctrl              Pointer to usb_ctrl_t structure area
 * @retval     USB_SUCCESS         Successfully completed
 * @retval     USB_ERR_PARA        Parameter error
 * @retval     USB_ERR_BUSY        During a resume request processing
 * @retval     USB_ERR_NOT_SUSPEND USB device is not in the SUSPEND state
 * @retval     USB_ERR_NG          USB device is not in a state that can request the remote wakeup (USB peripheral mode only)
 * @details This function sends a RESUME signal from the USB module assigned to the member (module) of 
 * the usb_ctrl_t structure.
 * @note The argument (p_ctrl) is not supported when using RX100/RX200 series MCU.
 */
usb_err_t    R_USB_Resume (void)
{
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
    usb_er_t    err;

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
    usb_err_t   ret_code = USB_SUCCESS;
    usb_info_t  info;

#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
  #if !defined(USB_CFG_PHID_USE)
    if (USB_PERI == g_usb_cstd_usb_mode)
    {
        return USB_ERR_PARA;
    }
  #endif /* !defined(USB_CFG_PHID_USE) */
#endif  /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */

    R_USB_GetInformation(&info);
    if (USB_STS_SUSPEND != info.status)
    {
        return USB_ERR_NOT_SUSPEND;
    }

    if (USB_HOST == g_usb_cstd_usb_mode)
    {
#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)


#if (BSP_CFG_RTOS_USED != 0)    /* RTOS */
        if (USB_YES == gs_usb_resume_ing)
        {
            return USB_ERR_BUSY;
        }
        else
        {
            gs_usb_resume_ing = USB_YES;
        }

        err = usb_hstd_change_device_state((usb_cb_t)&usb_hstd_resume_complete,
                                         USB_DO_GLOBAL_RESUME, USB_DEVICEADDR);
        if (USB_OK != err)
        {
            ret_code = USB_ERR_NG;
        }

        gs_usb_resume_ing = USB_NO;
#else   /* (BSP_CFG_RTOS_USED != 0) */

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

#endif  /* (BSP_CFG_RTOS_USED != 0) */

#else   /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
        ret_code = USB_ERR_NG;
#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
    }
    else
    {
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)

#if (BSP_CFG_RTOS_USED != 0)    /* RTOS */
        if (USB_YES == gs_usb_resume_ing)
        {
            return USB_ERR_BUSY;
        }
        else
        {
            gs_usb_resume_ing = USB_YES;
        }
        /* Support remote wakeup ? */
        if (USB_TRUE == g_usb_pstd_remote_wakeup)
        {
            usb_pstd_change_device_state(USB_DO_REMOTEWAKEUP, USB_NULL, (usb_cb_t)usb_pstd_dummy_function);
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
        gs_usb_resume_ing = USB_NO;

#else   /* (BSP_CFG_RTOS_USED != 0) */
        /* Support remote wakeup ? */
        if (USB_TRUE == g_usb_pstd_remote_wakeup)
        {
            usb_pstd_change_device_state(USB_DO_REMOTEWAKEUP, USB_NULL, (usb_cb_t)usb_pstd_dummy_function);
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

#endif  /* (BSP_CFG_RTOS_USED != 0) */

#else   /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */
        ret_code = USB_ERR_NG;
#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */
    }

    return ret_code;
}
/***********************************************************************************************************************
 End of function R_USB_Resume
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name   : R_USB_VbusSetting
 *******************************************************************************************************************//**
 * @brief VBUS Supply Start/Stop Specification
 * @param[in]  p_ctrl          Pointer to usb_ctrl_t structure area
 * @param[in]  state           VBUS supply start/stop specification
 * @retval     USB_SUCCESS     Successful completion (VBUS supply start/stop completed)
 * @retval     USB_ERR_PARA    Parameter error
 * @retval     USB_ERR_NG      Other Error
 * @details Specifies starting or stopping the VBUS supply.
 * @note The argument (p_ctrl) is not supported when using RX100/RX200 series MCU.
 */
usb_err_t   R_USB_VbusSetting (uint16_t state)
{

#if (USB_CFG_MODE == USB_CFG_PERI)
    return USB_ERR_NG;
#else /* USB_CFG_MODE == USB_CFG_PERI */

    if (USB_PERI == g_usb_cstd_usb_mode) 
    {
        return USB_ERR_NG;              /* Support Host only. */
    }

#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE

    if ((USB_ON != state) && (USB_OFF != state))
    {
       return USB_ERR_PARA;
    }

#endif  /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */

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
}
/***********************************************************************************************************************
 End of function R_USB_VbusSetting
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name   : R_USB_GetInformation
 *******************************************************************************************************************//**
 * @brief Get USB device information
 * @param[in]  p_info          Pointer to usb_info_t structure area
 * @retval     USB_SUCCESS     Successful completion
 * @retval     USB_ERR_PARA    Parameter error
 * @retval     USB_ERR_NG      Other Error
 * @details This function gets the USB device information.
 * @note The argument (p_ctrl) is not supported when using RX100/RX200 series MCU.
 */
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
#if (BSP_CFG_RTOS_USED != 0)                            /* Use RTOS */
            case USB_POWERED:                           /* Power state  */
                p_info->status = USB_STS_POWERED;
                break;
            case USB_DEFAULT:                           /* Default state  */
                p_info->status = USB_STS_DEFAULT;
                break;
            case USB_ADDRESS:                           /* Address state  */
                p_info->status = USB_STS_ADDRESS;
                break;
            case USB_CONFIGURED:                        /* Configured state  */
                p_info->status = USB_STS_CONFIGURED;
                break;
            case USB_SUSPENDED:                         /* Suspend state */
                p_info->status = USB_STS_SUSPEND;
                break;
            case USB_DETACHED:                          /* Disconnect(VBUSon) state */
                p_info->status = USB_STS_DETACH;
                break;
            default:                                    /* Error */
                p_info->status = USB_NULL;
                break;
#else /* (BSP_CFG_RTOS_USED != 0) */
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
#endif /* (BSP_CFG_RTOS_USED != 0) */
        }

        /* Set USB Peri BC port state */
#if USB_CFG_BC == USB_CFG_ENABLE
        if (USB_BC_STATE_CDP == g_usb_hstd_bc.state)
        {
            p_info->port   = USB_CDP;
        }
        else
        {
            p_info->port   = USB_SDP;                   /* USB_SDP/USB_CDP/USB_DCP */
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
            break;                                      /* p_info->status = USB_STS_ERROR; */
        }
#if USB_CFG_BC == USB_CFG_ENABLE
        p_info->port   = g_usb_pstd_bc_detect;          /* USB_SDP/USB_CDP/USB_DCP */
#else   /* USB_CFG_BC == USB_CFG_ENABLE */
        p_info->port   = USB_SDP;                       /* USB_SDP */
#endif  /* USB_CFG_BC == USB_CFG_ENABLE */

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI */
    }
    return USB_SUCCESS;
}
/***********************************************************************************************************************
 End of function R_USB_GetInformation
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name   : R_USB_PipeRead
 *******************************************************************************************************************//**
 * @brief Request data read via specified pipe
 * @param[in]  p_ctrl          Pointer to usb_ctrl_t structure area
 * @param[out] p_buf           Pointer to area that stores data
 * @param[in]  size            Read request size
 * @retval     USB_SUCCESS     Successful completion
 * @retval     USB_ERR_PARA    Parameter error
 * @retval     USB_ERR_BUSY    Specifed pipe now handling data receive/send request
 * @retval     USB_ERR_NG      Other Error
 * @details This function requests a data read via the pipe specified in the argument. 
 * The read data is stored in the area specified in the argument (p_buf).
 */
usb_err_t  R_USB_PipeRead (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size)
{
#if !defined(USB_CFG_HVND_USE) && !defined(USB_CFG_PVND_USE)
    return USB_ERR_NG;

#else   /* !defined(USB_CFG_HVND_USE) && !defined(USB_CFG_PVND_USE) */
    usb_err_t   ret_code = USB_ERR_NG;
#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
    usb_utr_t   tran_data;
    usb_utr_t   *p_tran_data;
#else  /* BSP_CFG_RTOS_USED != 0 */
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
    usb_utr_t   *p_tran_data;
#else  /* ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI) */
    usb_sutr_t   *p_tran_data;
#endif /* ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI) */
#endif /* BSP_CFG_RTOS_USED != 0 */

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

#endif  /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */

    R_USB_GetInformation(&info);
    if (USB_STS_CONFIGURED == info.status)
    {
        /* PIPE Transfer set */
        if (USB_HOST == g_usb_cstd_usb_mode)
        {
    #if !defined(USB_CFG_HVND_USE)
            return USB_ERR_NG;
    #else /* !defined(USB_CFG_HVND_USE) */

    #if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
            p_tran_data = (usb_utr_t *)&tran_data;
                p_tran_data->keyword     = p_ctrl->pipe;            /* Pipe No */
            p_tran_data->p_setup     = USB_NULL;
            p_tran_data->complete    = usb_hvnd_read_complete;      /* Callback function */
    #else  /* BSP_CFG_RTOS_USED != 0 */
            p_tran_data = (usb_sutr_t *)&g_usb_hstd_data[p_ctrl->pipe];
            p_tran_data->pipenum     = p_ctrl->pipe;                /* Pipe No */
            p_tran_data->setup       = USB_NULL;
            p_tran_data->complete    = usb_hstd_read_complete;      /* Callback function */
    #endif /* BSP_CFG_RTOS_USED != 0 */
            p_tran_data->p_tranadr   = p_buf;                       /* Data address */
            p_tran_data->tranlen     = size;                        /* Data Size */
            p_tran_data->read_req_len = size;                       /* Data Size */
            err = usb_hstd_transfer_start(p_tran_data);             /* USB Transfer Start */
    #endif /* defined(USB_CFG_HVND_USE) */
        }
        else
        {
    #if !defined(USB_CFG_PVND_USE)
            return USB_ERR_NG;
    #else /* !defined(USB_CFG_PVND_USE) */
    #if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
            p_tran_data = (usb_utr_t *)&tran_data;
    #else  /* BSP_CFG_RTOS_USED != 0 */
            p_tran_data = (usb_utr_t *)&g_usb_pstd_data[p_ctrl->pipe];
    #endif /* BSP_CFG_RTOS_USED != 0 */
            p_tran_data->keyword    = p_ctrl->pipe;             /* Pipe No */
            p_tran_data->p_tranadr  = p_buf;                    /* Data address */
            p_tran_data->tranlen    = size;                     /* Data Size */
            p_tran_data->complete   = (usb_cb_t)&usb_pstd_read_complete;  /* Callback function */
            p_tran_data->read_req_len = size;                   /* Data Size */
            err = usb_pstd_transfer_start(p_tran_data);         /* USB Transfer Start */
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
        }
    }

    return ret_code;
#endif   /* !defined(USB_CFG_HVND_USE) && !defined(USB_CFG_PVND_USE) */
}
/***********************************************************************************************************************
 End of function R_USB_PipeRead
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name   : R_USB_PipeWrite
 *******************************************************************************************************************//**
 * @brief Request data write to specified pipe
 * @param[in]  p_ctrl          Pointer to usb_ctrl_t structure area
 * @param[in]  p_buf           Pointer to area that write stores data
 * @param[in]  size            Write request size
 * @retval     USB_SUCCESS     Successful completion
 * @retval     USB_ERR_PARA    Parameter error
 * @retval     USB_ERR_BUSY    Specifed pipe now handling data receive/send request
 * @retval     USB_ERR_NG      Other Error
 * @details This function requests a data write. The write data is stored in the area specified in the argument (p_buf).
 */
usb_err_t  R_USB_PipeWrite (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size)
{
#if !defined(USB_CFG_HVND_USE) && !defined(USB_CFG_PVND_USE)
    return USB_ERR_NG;

#else   /* !defined(USB_CFG_HVND_USE) && !defined(USB_CFG_PVND_USE) */

    usb_err_t   ret_code = USB_ERR_NG;
    usb_er_t    err;
    usb_info_t  info;
#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
    usb_utr_t   tran_data;
    usb_utr_t   *p_tran_data;
#else  /* BSP_CFG_RTOS_USED != 0 */
#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI)
    usb_utr_t   *p_tran_data;
#else  /* ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI) */
    usb_sutr_t   *p_tran_data;
#endif /* ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI) */
#endif /* BSP_CFG_RTOS_USED != 0 */

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

#endif  /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */

    R_USB_GetInformation(&info);
    if (USB_STS_CONFIGURED == info.status)
    {
        /* PIPE Transfer set */
        if (USB_HOST == g_usb_cstd_usb_mode)
        {
    #if !defined(USB_CFG_HVND_USE)
            return USB_ERR_NG;

    #else /* !defined(USB_CFG_HVND_USE) */
    #if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
            p_tran_data = (usb_utr_t *)&tran_data;
            p_tran_data->keyword     = p_ctrl->pipe;                /* Pipe No */
            p_tran_data->p_setup     = USB_NULL;
            p_tran_data->complete    = usb_hvnd_write_complete;     /* Callback function */
    #else  /* BSP_CFG_RTOS_USED != 0 */
            p_tran_data = (usb_sutr_t *)&g_usb_hstd_data[p_ctrl->pipe];
            p_tran_data->pipenum     = p_ctrl->pipe;                /* Pipe No */
            p_tran_data->setup       = USB_NULL;
            p_tran_data->complete    = usb_hstd_write_complete;     /* Callback function */
    #endif /* BSP_CFG_RTOS_USED != 0 */
            p_tran_data->p_tranadr   = p_buf;                       /* Data address */
            p_tran_data->tranlen     = size;                        /* Data Size */
            err = usb_hstd_transfer_start(p_tran_data);             /* USB Transfer Start */
    #endif  /* !defined(USB_CFG_HVND_USE) */
        }
        else
        {
    #if !defined(USB_CFG_PVND_USE)
            return USB_ERR_NG;

    #else /* !defined(USB_CFG_PVND_USE) */
    #if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
            p_tran_data = (usb_utr_t *)&tran_data;
    #else  /* BSP_CFG_RTOS_USED != 0 */
            p_tran_data = (usb_utr_t *)&g_usb_pstd_data[p_ctrl->pipe];
    #endif /* BSP_CFG_RTOS_USED != 0 */
            p_tran_data->keyword    = p_ctrl->pipe;             /* Pipe No */
            p_tran_data->p_tranadr  = p_buf;                    /* Data address */
            p_tran_data->tranlen    = size;                     /* Data Size */
            p_tran_data->complete   = (usb_cb_t)&usb_pstd_write_complete; /* Callback function */

            err = usb_pstd_transfer_start(p_tran_data);         /* USB Transfer Start */
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
        }
    }

    return ret_code;
#endif /* #if !defined(USB_CFG_HVND_USE) && !defined(USB_CFG_PVND_USE) */

}
/***********************************************************************************************************************
 End of function R_USB_PipeWrite
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name   : R_USB_PipeStop
 *******************************************************************************************************************//**
 * @brief Stop data read/write via specified pipe
 * @param[in]  p_ctrl          Pointer to usb_ctrl_t structure area
 * @retval     USB_SUCCESS     Successful completed (stop request completed)
 * @retval     USB_ERR_PARA    Parameter error
 * @retval     USB_ERR_NG      Other Error
 * @details This function is used to terminate a data read/write operation.
 */
usb_err_t    R_USB_PipeStop (usb_ctrl_t *p_ctrl)
{
#if !defined(USB_CFG_HVND_USE) && !defined(USB_CFG_PVND_USE)
    return USB_ERR_NG;
#else

#if ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI) || (BSP_CFG_RTOS_USED != 0)
    usb_er_t    err = USB_ERR_NG;
#endif /* ((USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_PERI) || (BSP_CFG_RTOS_USED != 0) */
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
#if (BSP_CFG_RTOS_USED != 0)                            /* Use RTOS */
        err = usb_hstd_transfer_end(p_ctrl->pipe, (uint16_t)USB_DATA_STOP);
        if (USB_OK == err)
        {
            ret_code = USB_SUCCESS;
        }
#else /* (BSP_CFG_RTOS_USED != 0) */
        g_usb_hstd_current_pipe = p_ctrl->pipe;
        usb_hstd_forced_termination(USB_DATA_STOP);
        ret_code = USB_SUCCESS;
#endif /* (BSP_CFG_RTOS_USED != 0) */

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
}
/***********************************************************************************************************************
 End of function R_USB_PipeStop
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name   : R_USB_GetUsePipe
 *******************************************************************************************************************//**
 * @brief Get used pipe number from bit map
 * @param[in]  p_ctrl          Pointer to usb_ctrl_t structure area
 * @param[out] p_pipe          Pointer to area that stores the selected pipe number (bit map information)
 * @retval     USB_SUCCESS     Successful completed (stop request completed)
 * @retval     USB_ERR_PARA    Parameter error
 * @retval     USB_ERR_NG      Other Error
 * @details Get the selected pipe number (number of the pipe that has completed initalization) via bit map information. 
 * The bit map information is stored in the area specified in argument (p_pipe). 
 * Based on the information (module member and address member) assigned to the usb_ctrl_t structure, 
 * obtains the PIPE information of that USB device.
 * @note The argument (p_ctrl) is not supported when using RX100/RX200 series MCU.
 */
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
}
/***********************************************************************************************************************
 End of function R_USB_GetUsePipe
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name   : R_USB_GetPipeInfo
 *******************************************************************************************************************//**
 * @brief Get pipe information for specified pipe
 * @param[in]  p_ctrl          Pointer to usb_ctrl_t structure area
 * @param[out] p_info          Pointer to usb_pipe_t structure area
 * @retval     USB_SUCCESS     Successful completed (stop request completed)
 * @retval     USB_ERR_PARA    Parameter error
 * @retval     USB_ERR_NG      Other Error
 * @details This function gets the following pipe information regarding the pipe specified 
 * in the argument (p_ctrl) member (pipe): endpoint number, transfer type, transfer direction and maximum packet size. 
 * The obtained pipe information is stored in the area specified in the argument (p_info).
 */
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
        p_info->ep = usb_hstd_pipe_to_epadr(p_ctrl->pipe);
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
        p_info->mxps = usb_cstd_get_maxpacket_size(p_ctrl->pipe);     /* Set Max packet size */
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
}
/***********************************************************************************************************************
 End of function R_USB_GetPipeInfo
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name   : R_USB_PullUp
 *******************************************************************************************************************//**
 * @brief Pull-up enable/disable setting of D+/D- line
 * @param[in]  state   Pull-up enable/disable setting
 * @retval     USB_SUCCESS     Successful completion (Pull-up enable/disable setting completed)
 * @retval     USB_ERR_PARA    Parameter error
 * @retval     USB_ERR_NG      Other Error
 * @details This API enables or disables pull-up of D+/D- line. 
 */
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

    if (USB_LS  == g_usb_pstd_speed)
    {
        if (USB_ON == state)
        {
            hw_usb_pset_dmrpu();
        }
        else
        {
            hw_usb_pclear_dmrpu();
        }
    }
    else
    {
        if (USB_ON == state)
        {
            hw_usb_pset_dprpu();
        }
        else
        {
            hw_usb_pclear_dprpu();
        }
    }


    return USB_SUCCESS;
#endif /* USB_CFG_MODE == USB_CFG_HOST */
}
/***********************************************************************************************************************
 End of function R_USB_PullUp
***********************************************************************************************************************/

/***********************************************************************************************************************
End  Of File
***********************************************************************************************************************/
