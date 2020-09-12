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
 *****************************************************************************/
/******************************************************************************
 * File Name    : r_usb_pstdfunction.c
 * Description  : USB Peripheral standard function code
 ******************************************************************************/
/*******************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 08.01.2014 1.00 First Release
 *         : 30.11.2018 1.10    Supporting Smart Configurator
 *         : 31.05.2019 1.11    Added support for GNUC and ICCRX.
*******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include <string.h>
#include "r_usb_basic_mini_if.h"
#include "r_usb_typedef.h"
#include "r_usb_extern.h"
#include "r_usb_bitdefine.h"
#include "r_usb_reg_access.h"

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
 Function Name   : usb_pstd_set_feature_function
 Description     : Process a SET_FEATURE request.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_pstd_set_feature_function (void)
{
    /* Request error */
    usb_pstd_set_stall_pipe0();
} /* End of function usb_pstd_set_feature_function() */

/******************************************************************************
 Function Name   : usb_pstd_chk_vbsts
 Description     : Return the VBUS status.
 Arguments       : none
 Return          : uint16_t connection status(ATTACH/DETACH)
 ******************************************************************************/
uint16_t usb_pstd_chk_vbsts (void)
{
    uint16_t buf1;
    uint16_t buf2;
    uint16_t buf3;
    uint16_t connect_info;

    /* VBUS chattering cut */
    /* WAIT_LOOP */
    do
    {
        buf1 = hw_usb_read_intsts();
        usb_cpu_delay_1us((uint16_t) 10);
        buf2 = hw_usb_read_intsts();
        usb_cpu_delay_1us((uint16_t) 10);
        buf3 = hw_usb_read_intsts();
    } while (((buf1 & USB_VBSTS) != (buf2 & USB_VBSTS)) || ((buf2 & USB_VBSTS) != (buf3 & USB_VBSTS)));

    /* VBUS status judge */
    if ((buf1 & USB_VBSTS) != (uint16_t) 0)
    {
        /* Attach */
        connect_info = USB_ATTACH;
    }
    else
    {
        /* Detach */
        connect_info = USB_DETACH;
    }
    return connect_info;
} /* End of function usb_pstd_chk_vbsts() */


/******************************************************************************
 Function Name   : usb_pstd_busreset_function
 Description     : Processing for USB bus reset detection.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_pstd_busreset_function (void)
{
    /* Non processing. */
} /* End of function usb_pstd_busreset_function() */

/******************************************************************************
 Function Name   : usb_pstd_suspend_function
 Description     : Processing for suspend signal detection.
 Arguments       : none
 Return value    : none
 ******************************************************************************/
void usb_pstd_suspend_function (void)
{
    /* Non processing. */
} /* End of function usb_pstd_suspend_function() */

/******************************************************************************
 Function Name   : usb_pstd_driver_init
 Description     : 
 Arguments       : usb_ctrl_t *p_ctrl
                 : usb_cfg_t  *p_cfg
 Return value    : none
 ******************************************************************************/
void usb_pstd_driver_init (usb_cfg_t *p_cfg)
{
    uint16_t i;

    /* WAIT_LOOP */
    for (i = 0; i < USB_ALT_NO; i++)
    {
        g_usb_pstd_alt_no[i] = 0;
    }

    g_usb_pstd_req_type = USB_NULL;
    g_usb_pstd_req_value = USB_NULL;
    g_usb_pstd_req_index = USB_NULL;
    g_usb_pstd_req_length = USB_NULL;
    g_usb_pstd_pipe0_request = USB_OFF;
    g_usb_pstd_std_request = USB_NO;
    g_usb_pstd_is_connected = USB_FALSE;
    memset((void *)&g_usb_pstd_req_reg, 0, sizeof(usb_setup_t));
    g_usb_pstd_stall_cb = USB_NULL;
    memset((void *)&g_usb_pstd_driver, 0, sizeof(usb_pcdreg_t));

    /* WAIT_LOOP */
    for (i = 0; i < (USB_MAX_PIPE_NO + 1); i++)
    {
        gp_usb_pstd_data[i] = USB_NULL;
        g_usb_pstd_data_cnt[i] = 0;
    }

    /* WAIT_LOOP */
    for (i = 0; i < USB_EVENT_MAX; i++)
    {
        g_usb_cstd_event.code[i] = USB_STS_NONE;
    }

    /* WAIT_LOOP */
    for (i = USB_PIPE0; i <= USB_MAX_PIPE_NO; i++)
    {
        g_usb_pstd_is_stall_pipe[i] = USB_FALSE;
        gp_usb_pstd_pipe[i] = (usb_putr_t *) USB_NULL;
    }

    g_usb_pstd_config_no = 0; /* Configuration number */
    g_usb_pstd_remote_wakeup = USB_FALSE; /* Remote wake up enable flag */

    usb_pstd_registration(p_cfg);

} /* End of function usb_pstd_driver_init() */

/******************************************************************************
Function Name   : usb_pstd_get_open_class
Description     : Get Open USB Device Class.
Arguments       : none
Return value    : Device class(USB_PCDC/USB_PHID/USB_PMSC/USB_PVND)
******************************************************************************/
uint8_t usb_pstd_get_open_class (void)
{
    uint8_t ret;

    if (0 != ((1 << USB_PHID) & g_usb_cstd_open_class))
    {
        ret = USB_PHID;
    }
    else if (0 != ((1 << USB_PCDC) & g_usb_cstd_open_class))
    {
        ret = USB_PCDC;
    }
    else if (0 != ((1 << USB_PMSC) & g_usb_cstd_open_class))
    {
        ret = USB_PMSC;
    }
    else if (0 != ((1 << USB_PVND) & g_usb_cstd_open_class))
    {
        ret = USB_PVND;
    }
    else
    {
        ret = USB_NULL;     /* Error */
    }

    return ret;
}   /* End of function usb_pstd_get_open_class() */

#endif  /* (USB_CFG_MODE & USB_CFG_PERI) == USB_CFG_REPI */

/******************************************************************************
 End  Of File
 ******************************************************************************/
