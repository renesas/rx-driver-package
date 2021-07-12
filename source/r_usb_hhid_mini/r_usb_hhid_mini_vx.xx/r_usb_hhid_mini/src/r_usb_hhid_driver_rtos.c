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
 * File Name    : r_usb_hhid_driver_rtos.c
* Description   : This is the USB host HID class driver function code.
 ***********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           : 01.09.2014 1.00    First Release
 *           : 01.06.2015 1.01    Added RX231.
 *           : 29.12.2015 1.02    Minor Update.
 *           : 30.11.2018 1.10    Supporting Smart Configurator
 *           : 31.05.2019 1.11    Added support for GNUC and ICCRX.
 ***********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include <string.h>

#include "r_usb_hhid.h"

#if (BSP_CFG_RTOS_USED != 0)    /* RTOS */

#include "r_usb_bitdefine.h"
#include "r_rtos_abstract.h"
#include "r_usb_cstd_rtos.h"

/*******************************************************************************
 Macro definitions
 ******************************************************************************/
#define USB_HHID_CLSDATASIZE                (512)
#define USB_B_INTERFACE_PROTOCOL            (7)                 /* Interface Descriptor bInterfaceProtocol */

/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/
/* variables */
static uint16_t gs_usb_hhid_protocol;            /* HID Device protocol(Mouse/Keyboard) */
static uint16_t gs_usb_hhid_num_endpoint;        /* Num Endpoints */
/* Host HID TPL */
static const uint16_t gs_usb_hhid_devicetpl[] =
{
    USB_CFG_TPLCNT, /* Number of tpl table */
    0, /* Reserved */
    USB_CFG_TPL /* Vendor ID, Product ID */
};


/* functions */
static void      usb_hid_suspend_complete (usb_utr_t *ptr, uint16_t devadr);
static void      usb_hid_configured (usb_utr_t *ptr, uint16_t dev_addr, uint16_t data2);
static void      usb_hid_detach (usb_utr_t *ptr, uint16_t devadr, uint16_t data2);
static void      usb_hid_resume_complete (usb_utr_t *ptr, uint16_t devadr, uint16_t data2);
static void      usb_hhid_class_check (uint16_t **table);

static uint16_t usb_hhid_get_string_info(uint16_t string);

/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

/******************************************************************************
Renesas Host HID Driver functions
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hhid_pipe_info
 Description     : pipe information table set
 Arguments       : uint8_t *p_table      : Descriptor store address
                 : uint16_t length       : Descriptor total lenght
 Return value    : uint16_t              : Error info(USB_OK / USB_ERROR)
 ******************************************************************************/
uint16_t usb_hhid_pipe_info (uint8_t *p_table, uint16_t length)
{
    /* Offset for Descriptor Top Address */
    uint16_t ofdsc;
    uint16_t pipe_no;
    uint16_t detect_in_pipe = USB_OFF;
    uint16_t detect_out_pipe = USB_ON;

    if (USB_HID_OTHER == gs_usb_hhid_protocol)
    {
        if (2 == gs_usb_hhid_num_endpoint)
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
            pipe_no = usb_cstd_pipe_table_set (USB_HHID,&p_table[ofdsc]);
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
void usb_hhid_registration (void)
{
    usb_hcdreg_t driver;

    driver.ifclass    = (uint16_t)       USB_IFCLS_HID;                     /* Interface class : HID */
    driver.p_tpl      = (uint16_t*)      &gs_usb_hhid_devicetpl;            /* Target peripheral list */
    driver.classinit  = (usb_cb_t)       &usb_hstd_dummy_function;          /* Driver init */
    driver.classcheck = (usb_cb_check_t) &usb_hhid_class_check;             /* Driver check */
    driver.devconfig  = (usb_cb_t)       &usb_hid_configured;               /* Device configuered */
    driver.devdetach  = (usb_cb_t)       &usb_hid_detach;                   /* Device detach */
    driver.devsuspend = (usb_cb_t)       &usb_hid_suspend_complete;         /* Device suspend */
    driver.devresume  = (usb_cb_t)       &usb_hid_resume_complete;          /* Device resume */

    usb_hstd_driver_registration(&driver);                                  /* Host HID class driver registration. */
}
/******************************************************************************
 End of function usb_hhid_registration
 ******************************************************************************/

/******************************************************************************
 Callback functions
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hid_configured
 Description     : Callback function for HID device configuered
 Arguments       : usb_utr_t    *ptr    : Pointer to usb_utr_t structure.
                 : uint16_t     devadr  : Device Adrress
                 : uint16_t     data2   : Not use
 Return value    : none
 ******************************************************************************/
static void usb_hid_configured (usb_utr_t *ptr, uint16_t devadr, uint16_t data2)
{
    usb_ctrl_t ctrl;

    usb_cstd_pipe_reg_set(); /* Host CDC Pipe registration */
    ctrl.type = USB_HHID;
    ctrl.p_data = (void *)g_usb_default_apl_task_id;
    usb_cstd_set_event(USB_STS_CONFIGURED, &ctrl); /* Set Event()  */
}
/******************************************************************************
 End of function usb_hid_configured
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hid_detach
 Description     : Callback function for HID device detach
 Arguments       : usb_utr_t    *ptr    : Pointer to usb_utr_t structure.
                 : uint16_t     devadr  : Device Adrress
                 : uint16_t     data2   : Not use
 Return value    : none
 ******************************************************************************/
static void usb_hid_detach (usb_utr_t *ptr, uint16_t devadr, uint16_t data2)
{
    usb_ctrl_t ctrl;

    usb_cstd_pipe_reg_clear();
    usb_cstd_pipe_table_clear();
    ctrl.type = USB_HHID;
    ctrl.p_data = (void *)g_usb_default_apl_task_id;
    usb_cstd_set_event(USB_STS_DETACH, &ctrl); /* Set Event()  */
}
/******************************************************************************
 End of function usb_hid_detach
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hid_suspend_complete
 Description     : Suspended state process
 Arguments       : usb_utr_t    *ptr        : Pointer to usb_utr_t structure.
 Arguments       : uint16_t     devadr      : Device Adrress
 Return value    : none
 ******************************************************************************/
void usb_hid_suspend_complete (usb_utr_t *ptr, uint16_t devadr)
{
    usb_ctrl_t ctrl;

    ctrl.type = USB_HHID;
    ctrl.p_data = (void *)ptr->task_id;
    usb_cstd_set_event(USB_STS_SUSPEND, &ctrl);
}
/******************************************************************************
 End of function usb_hid_suspend_complete
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hid_resume_complete
 Description     : Resume complete callback.
 Arguments       : usb_utr_t    *ptr    : Pointer to usb_utr_t structure.
                 : uint16_t     devadr  : Device Adrress
                 : uint16_t     data2   : Not use
 Return value    : none
 ******************************************************************************/
static void usb_hid_resume_complete (usb_utr_t *ptr, uint16_t devadr, uint16_t data2)
{
    usb_ctrl_t ctrl;

    ctrl.type = USB_HHID;
    ctrl.p_data = (void *)ptr->task_id;
    usb_cstd_set_event(USB_STS_RESUME, &ctrl);
}
/******************************************************************************
 End of function usb_hid_resume_complete
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hhid_read_complete
 Description     : data receive complete process.
 Arguments       : usb_utr_t    *mess   : Pointer to usb_utr_t structure.
                 : uint16_t     data1   : Not used
                 : uint16_t     data2   : Not used
 Return value    : none
 ******************************************************************************/
void usb_hhid_read_complete (usb_utr_t *mess, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;

    ctrl.pipe    = mess->keyword;                               /* Pipe number setting */
    ctrl.type    = USB_HHID;                                    /* hid Data class  */
    ctrl.size    = mess->read_req_len - mess->tranlen;

    if (USB_DATA_OK == mess->status)
    {
        ctrl.status = USB_SUCCESS;
    }
    else if (USB_DATA_SHT == mess->status)
    {
        ctrl.status = USB_ERR_SHORT;
    }
    else if (USB_DATA_OVR == mess->status)
    {
        ctrl.status = USB_ERR_OVER;
        ctrl.size    = 0;
    }
    else
    {
        /* USB_DATA_ERR */
        ctrl.status = USB_ERR_NG;
        ctrl.size    = 0;
    }
    ctrl.p_data = (void *)mess->task_id;

    usb_cstd_set_event(USB_STS_READ_COMPLETE, &ctrl);                /* Set Event()  */
}
/******************************************************************************
 End of function usb_hhid_read_complete
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hhid_write_complete
 Description     : data transfer complete process.
 Arguments       : usb_utr_t    *mess   : Pointer to usb_utr_t structure.
                 : uint16_t     data1   : Not used
                 : uint16_t     data2   : Not used
 Return value    : none
 ******************************************************************************/
void usb_hhid_write_complete (usb_utr_t *mess, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;

    ctrl.pipe    = mess->keyword;                               /* Pipe number setting */
    ctrl.type    = USB_HHID;                                    /* hid Data class  */
    ctrl.size    = 0;

    if (USB_DATA_NONE == mess->status)                          /* USB_CFG_PHID_INT_OUT */
    {
        ctrl.status = USB_SUCCESS;
    }
    else
    {
        ctrl.status = USB_ERR_NG;
    }
    ctrl.p_data = (void *)mess->task_id;

    usb_cstd_set_event(USB_STS_WRITE_COMPLETE, &ctrl);               /* Set Event()  */
}
/******************************************************************************
 End of function usb_hhid_write_complete
 ******************************************************************************/

/******************************************************************************
 Initialization functions
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hhid_driver_start
 Description     : USB Host Initialize process
 Arguments       : none
 Return          : none
 ******************************************************************************/
void usb_hhid_driver_start (void)
{
    gs_usb_hhid_protocol = 0;
    gs_usb_hhid_num_endpoint = 0;
}
/******************************************************************************
 End of function usb_hhid_driver_start()
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hhid_get_hid_protocol
 Description     : Get HID Protocol
 Arguments       : none
 Return          : Hid Protocol
 ******************************************************************************/
uint8_t usb_hhid_get_hid_protocol (void)
{
    /* table[7]:Interface Descriptor-bInterfaceProtocol */
    return gs_usb_hhid_protocol;
}
/******************************************************************************
 End of function usb_hhid_get_hid_protocol
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hhid_class_check
 Description     : Driver check.
 Arguments       : uint16_t     **table     : Pointer to HID class information table
 Return value    : none
 ******************************************************************************/
static void usb_hhid_class_check (uint16_t **table)
{
    uint16_t    retval;
    uint8_t     *p_device_table;
    uint8_t     *p_interface_table;   /* Interface Descriptor Table */

    p_device_table              = (uint8_t*) ((uint16_t*) table[0]);            /* Device Descriptor Table */
    p_interface_table           = (uint8_t*) ((uint16_t*) table[2]);          /* Interface Descriptor Table */
    gs_usb_hhid_protocol        = p_interface_table[USB_B_INTERFACE_PROTOCOL];  /* Interface Protocol */
    gs_usb_hhid_num_endpoint    = p_interface_table[USB_IF_B_NUMENDPOINTS];     /* Num Endpoints */

    /* Get String Descriptors */
    retval = usb_hhid_get_string_info (p_device_table[USB_DEV_I_PRODUCT]);
    if (USB_OK == retval)
    {
        /* Descriptor check result */
        *table[3] = USB_OK;
    }
    else
    {
        *table[3] = USB_ERROR;
    }
}
/******************************************************************************
 End of function usb_hhid_class_check
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hhid_get_string_info
 Description     : Get string descriptors.
 Argument        : uint16_t string
 Return value    : none
 ******************************************************************************/
static uint16_t usb_hhid_get_string_info(uint16_t string)
{
    uint16_t retval;
    uint8_t *p_desc;
    uint16_t desc_len;

    /* Send GetDescriptor(String) */
    retval = usb_hstd_get_string_desc((uint16_t)0);
    if (USB_OK != retval)
    {
        return USB_ERROR;
    }

    /* Send GetDescriptor(String) */
    retval = usb_hstd_get_string_desc((uint16_t)string);
    if (USB_OK != retval)
    {
        return USB_ERROR;
    }

    p_desc = (uint8_t *)&g_usb_hstd_config_descriptor;

    desc_len = ((uint16_t) *(p_desc + 3)) << 8;
    desc_len += (uint16_t) *(p_desc + 2);

    /* pipe information table set */
    retval = usb_hhid_pipe_info(p_desc, desc_len);
    if (USB_ERROR == retval)
    {
        /* Enumeration class check error */
        retval = USB_ERROR;
        usb_hstd_connect_err_event_set();
    }
    else
    {
        /* Enumeration class check OK */
        retval = USB_OK;
    }

    return retval;
}
/******************************************************************************
 End of function usb_hhid_get_string_info
 ******************************************************************************/

#endif /*(BSP_CFG_RTOS_USED != 0)*/
/******************************************************************************
 End  Of File
 ******************************************************************************/
