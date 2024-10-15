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
 * Copyright (C) 2014(2024) Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name     : r_usb_hhid_driver.c
* Description   : This is the USB host HID class driver function code.
 ***********************************************************************************************************************/
/**********************************************************************************************************************
 * History   : DD.MM.YYYY Version Description
 *           : 01.09.2014 1.00    First Release
 *           : 01.06.2015 1.01    Added RX231.
 *           : 29.12.2015 1.02    Minor Update.
 *           : 30.11.2018 1.10    Supporting Smart Configurator
 *           : 31.05.2019 1.11    Added support for GNUC and ICCRX.
 *           : 30.06.2020 1.20    Added support for RTOS.
 * vv        : 30.04.2024 1.30    Added support for RX261.
 ***********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_hhid.h"

#if defined(USB_CFG_HHID_USE)

#if (BSP_CFG_RTOS_USED == 0)    /* Non-OS */
/*******************************************************************************
 Macro definitions
 ******************************************************************************/
#define USB_B_INTERFACE_PROTOCOL            (7)                 /* Interface Descriptor bInterfaceProtocol */

/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/
/* functions */
static void        usb_hhid_device_state (uint16_t data, uint16_t state);

/******************************************************************************
Renesas Host HID Driver functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_hhid_pipe_info
Description     : pipe information table set
Arguments       : uint8_t *p_table       : Descriptor store address
                : uint16_t length       : Descriptor total lenght
Return value    : uint16_t              : Error info(USB_OK / USB_ERROR)
******************************************************************************/
uint16_t usb_hhid_pipe_info (uint8_t *p_table, uint16_t length)
{
    /* Offset for Descriptor Top Address */
    uint16_t ofdsc;
    uint8_t  pipe_no;
    uint8_t  protocol;
    uint8_t  num_endpoint;
    uint16_t detect_in_pipe = USB_OFF;
    uint16_t detect_out_pipe = USB_ON;

    protocol = gp_usb_hstd_interface_tbl[USB_IF_B_INTERFACEPROTOCOL];
    num_endpoint = gp_usb_hstd_interface_tbl[USB_IF_B_NUMENDPOINTS];

    if (USB_HID_OTHER == protocol)
    {
        if (2 == num_endpoint)
        {
            detect_out_pipe = USB_OFF;
        }
    }

    /* Descriptor Address set */
    ofdsc = p_table[0];

    /* Loop for Endpoint Descriptor search */
    /* WAIT_LOOP */
    while (ofdsc < length)
    {
        /* Endpoint Descriptor */
        if (USB_DT_ENDPOINT == p_table[ofdsc + USB_EP_B_DESCRIPTORTYPE])
        {
            /* Check pipe information */
            pipe_no = usb_cstd_pipe_table_set (USB_HHID, &p_table[ofdsc]);
            if (USB_NULL == pipe_no)
            {
                return USB_ERROR;
            }

            if (USB_CFG_HHID_INT_IN == pipe_no)
            {
                detect_in_pipe = USB_ON;
            }
            else if (USB_CFG_HHID_INT_OUT == pipe_no)
            {
                detect_out_pipe = USB_ON;
            }
            else
            {
                /* error */
            }

            if ((USB_ON == detect_in_pipe) && (USB_ON == detect_out_pipe))
            {
                    return USB_OK;
            }
        }
        ofdsc += p_table[ofdsc];/* Next descriptor point set */
    }
    return USB_ERROR;
}
/******************************************************************************
 End of function usb_hhid_pipe_info
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hhid_registration
 Description     : registration of Host HID driver.
 Arguments       : none
 Return value    : none
******************************************************************************/
void usb_hhid_registration(void)
{
    usb_hcdreg_t    driver;

    /* Driver registration */
    driver.ifclass          = USB_IFCLS_HID;            /* HID class */
    driver.classcheck       = &usb_hstd_class_check;
    driver.statediagram     = &usb_hhid_device_state;
    usb_hstd_driver_registration (&driver);
}
/******************************************************************************
 End of function usb_hhid_registration
 ******************************************************************************/

/******************************************************************************
Function Name   : usb_hhid_device_state
Description     : Open / Close
Arguments       : uint16_t data          : Device address
                : uint16_t state            : Device state
Return value    : none
******************************************************************************/
void usb_hhid_device_state(uint16_t data, uint16_t state)
{
    usb_ctrl_t  ctrl;
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
    usb_compliance_t disp_param;
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */

    switch (state)
    {
        case USB_STS_DETACH:
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
            disp_param.status = USB_CT_DETACH;
            disp_param.pid    = USB_NULL;
            disp_param.vid    = USB_NULL;
            /* Notification detach */
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
            /* Notification Attach */
            usb_compliance_disp ((void *)&disp_param);

#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
        break;

        case USB_STS_DEFAULT:
        break;

        case USB_STS_CONFIGURED:
            usb_cstd_pipe_reg_set ();
            ctrl.type = USB_HHID;
            usb_cstd_set_event (USB_STS_CONFIGURED, &ctrl);
        break;

        case USB_STS_SUSPEND:
            /* Do Nothing */
        break;

        case USB_STS_RESUME:
            usb_cstd_set_event (USB_STS_RESUME, &ctrl);
        break;

        case USB_STS_OVERCURRENT:
            usb_cstd_set_event (USB_STS_OVERCURRENT, &ctrl);
        break;

        default:
            /* Do Nothing */
        break;
    }

}   /* eof usb_hhid_device_state() */

/******************************************************************************
Function Name   : usb_hhid_get_hid_protocol
Description     : Get HID Protocol
Arguments       : none
Return value    : Hid Protocol
******************************************************************************/
uint8_t usb_hhid_get_hid_protocol(void)
{
    /* table[USB_B_INTERFACE_PROTOCOL]:Interface Descriptor-bInterfaceProtocol */
    return gp_usb_hstd_interface_tbl[USB_B_INTERFACE_PROTOCOL];
}
/******************************************************************************
 End of function usb_hhid_get_hid_protocol
 ******************************************************************************/

#endif /*(BSP_CFG_RTOS_USED == 0)*/

#endif /* defined(USB_CFG_HHID_USE) */

/******************************************************************************
End Of File
******************************************************************************/
