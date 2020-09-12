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
* http://www.renesas.com/disclaimer *
* Copyright (C) 2014(2019) Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
 * File Name     : r_usb_hcdc_driver.c
 * Version       : 1.10
 * Description   : This is the USB host CDC class driver function code.
 *******************************************************************************/
/*******************************************************************************
 * History   : DD.MM.YYYY Version Description
 *           : 01.09.2014 1.00    First Release
 *           : 01.06.2015 1.01    Added RX231.
 *           : 29.12.2015 1.02    Minor Update.
 *           : 30.11.2018 1.10    Supporting Smart Configurator
 *           : 31.05.2019 1.11    Added support for GNUC and ICCRX.
 *******************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_usb_hcdc.h"

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
 * Function Name: usb_hcdc_pipe_info
 * Description  : Host Pipe Information check and EP Table Set
 * Arguments    : uint8_t *p_table      : Check Start Descriptor address
                : uint16_t length       : Configuration Descriptor Length
 * Return Value : uint16_t              : USB_OK / USB_ERROR
 ******************************************************************************/
uint16_t usb_hcdc_pipe_info (uint8_t *p_table, uint16_t length)
{
    uint16_t        ofdsc;
    uint16_t        retval;
    int16_t         in_pipe;
    int16_t         out_pipe;

    uint8_t         pipe_no;

    /* Check Endpoint Descriptor */
    ofdsc = p_table[0];

    /* Pipe initial */
    in_pipe     = USB_NOPORT;
    out_pipe    = USB_NOPORT;

    /* WAIT_LOOP */
    while (ofdsc < length )
    {
        if (USB_DT_ENDPOINT == p_table[ofdsc + 1])
        {
            /* Endpoint Descriptor */
            pipe_no = usb_cstd_pipe_table_set (USB_HCDC, &p_table[ofdsc]);
            if (USB_CFG_HCDC_BULK_OUT == pipe_no)
            {
                out_pipe = USB_YES;
            }
            else if (USB_CFG_HCDC_BULK_IN == pipe_no)
            {
                in_pipe = USB_YES;
            }
            else
            {
                /* Do Nothing */
            }
        }
        ofdsc +=  p_table[ofdsc];
    }

    if ((USB_NOPORT != in_pipe) && (USB_NOPORT != out_pipe))
    {
        retval = USB_OK;
    }
    else
    {
        retval = USB_ERROR;
    }

    return retval;
}   /* eof usb_hcdc_pipe_info() */


/******************************************************************************
 * Function Name: cdc_registration
 * Description  : Registration of peripheral Communications Devices Driver
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void cdc_registration (void)
{
    usb_hcdreg_t    driver;

    /* Driver registration */
    driver.ifclass      = USB_CFG_HCDC_IFCLS;            /* CDC Communications Interface class */
    driver.classcheck   = &usb_hstd_class_check;
    driver.statediagram = &usb_hcdc_device_state;
    usb_hstd_driver_registration (&driver);

} /* eof cdc_registration() */

/******************************************************************************
 * Function Name: usb_hcdc_device_state
 * Description  : Open / Close
 * Arguments    : uint16_t data          : Device address
                : uint16_t state            : Device state
 * Return Value : none
 ******************************************************************************/
void usb_hcdc_device_state (uint16_t data, uint16_t state)
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
            ctrl.type = USB_HCDC;
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

}   /* eof usb_hcdc_device_state() */


/******************************************************************************
End Of File
 ******************************************************************************/
