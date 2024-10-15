/**********************************************************************************************************************
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
* Copyright (C) 2020(2024) Renesas Electronics Corporation. All rights reserved.    
 *********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_usb_hmanager_rtos.c
 * Description  : USB Host Control Manager
 ***********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 30.06.2020 1.20 First Release
 *         : 30.04.2024 1.30 Added support for RX261.
 ***********************************************************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_usb_basic_mini_if.h"

#if (BSP_CFG_RTOS_USED != 0)    /* RTOS */

#include <string.h>

#include "r_usb_typedef.h"
#include "r_usb_extern.h"
#include "r_usb_bitdefine.h"
#include "r_usb_reg_access.h"
#include "r_usb_cstd_rtos.h"

#if ((USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST)
/*******************************************************************************
 Macro definitions
 ******************************************************************************/

#define USB_PET_VID     (0x1a0a)    /* USB-PET Vendor ID  */
#define USB_PET_PID     (0x0200)    /* USB-PET Product ID (Use Embedded Host Test) */


/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static uint16_t gs_usb_hstd_setup_data[5];
static uint16_t gs_usb_hstd_device_descriptor[USB_DEVICESIZE / 2u];
static uint16_t gs_usb_hstd_enum_seq; /* Enumeration request */
static uint16_t gs_usb_hstd_suspend_pipe[USB_MAX_PIPE_NO + 1u];
/* Reset handshake result */
static uint16_t gs_usb_hstd_device_speed;



#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
static uint16_t         gs_usb_disp_param_set;
static usb_compliance_t gs_usb_disp_param;
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */

/* Condition compilation by the difference of the operating system */
static usb_utr_t *gsp_usb_hstd_mgr_msg;
static uint16_t gs_usb_hstd_mgr_msginfo;
static usb_cb_t gs_usb_hstd_mgr_callback;
static uint16_t usb_hstd_chk_device_class (usb_utr_t *ptr, usb_hcdreg_t *driver);
static uint16_t usb_hstd_chk_remote (void);
static void     usb_hstd_mgr_rel_mpl (usb_utr_t *ptr, uint16_t n);
static void     usb_hstd_statusresult_process(usb_utr_t *ptr);
static void     usb_hstd_mgr_submitresult_process(usb_utr_t *ptr);
static void     usb_hstd_mgr_aordetach_process(usb_utr_t *ptr);
static void     usb_hstd_device_state_ctrl (uint16_t devaddr, uint16_t msginfo);

/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/
/* Manager */
uint16_t g_usb_hstd_config_descriptor[USB_CONFIGSIZE / 2u];
usb_hcddevinf_t     g_usb_hcd_dev_info;                            /* port, status, config, speed, */
usb_hcdreg_t        g_usb_hcd_driver;                              /* Device driver (registration) */

uint8_t g_usb_hstd_class_data[CLSDATASIZE];
uint16_t g_usb_hstd_setup_data[5];

/******************************************************************************
 Function Name   : usb_hstd_mgr_rel_mpl
 Description     : Release a memory block.
 Argument        : uint16_t n    : Error no.
 Return          : none
 ******************************************************************************/
static void usb_hstd_mgr_rel_mpl (usb_utr_t *ptr, uint16_t n)
{
    rtos_release_fixed_memory(&g_rtos_usb_mpf_id, (void *)gsp_usb_hstd_mgr_msg);
}
/******************************************************************************
 End of function usb_hstd_mgr_rel_mpl
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_mgr_chgdevst_cb
 Description     : Call the callback function specified by the argument given to
 : the API usb_hstd_change_device_state.
 Arguments       : usb_utr_t    *ptr        : Pointer to usb_utr_t structure.
 Return          : none
 ******************************************************************************/
static void usb_hstd_mgr_chgdevst_cb (usb_utr_t *ptr)
{
    if (0 != gs_usb_hstd_mgr_msginfo)
    {
        (*gs_usb_hstd_mgr_callback)(ptr, USB_NULL, gs_usb_hstd_mgr_msginfo);
        gs_usb_hstd_mgr_msginfo = 0;
    }
}

/******************************************************************************
 End of function usb_hstd_mgr_chgdevst_cb
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_enumeration
 Description     : Execute enumeration on the connected USB device.
 Arguments       : usb_utr_t    *ptr        : Pointer to usb_utr_t structure.
 Return          : uint16_t                 : Enumeration status.
 ******************************************************************************/
static uint16_t usb_hstd_enumeration (usb_utr_t *ptr)
{
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
    uint16_t vendor_id;
    uint16_t product_id;
#endif  /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */

    usb_hcdreg_t *driver;
    uint16_t enume_mode; /* Enumeration mode (device state) */
    uint8_t *descriptor_table;
    uint16_t pipenum, devsel;

    /* Attach Detect Mode */
    enume_mode = USB_NONDEVICE;

    pipenum = gsp_usb_hstd_mgr_msg->keyword;

    /* Agreement device address */
    devsel = usb_hstd_pipe_to_devsel(pipenum);

    /* Manager Mode Change */
    if (USB_CTRL_END == gsp_usb_hstd_mgr_msg->result)
    {
        enume_mode = USB_DEVICEENUMERATION;
        switch (gs_usb_hstd_enum_seq)
        {
            /* Receive Device Descriptor */
            case 0 :
                usb_hstd_enum_set_address ((uint16_t) USB_DEVICE_0, USB_DEVICEADDR);
            break;

                /* Set Address */
            case 1 :
                descriptor_table = (uint8_t*) gs_usb_hstd_device_descriptor;
                devsel = (uint16_t) (USB_DEVICEADDR << USB_DEVADDRBIT);

                /* Set device speed */
                usb_hstd_set_devaddx_register(devsel, gs_usb_hstd_device_speed);
                g_usb_hstd_dcp_register =
                        (uint16_t) ((uint16_t) (descriptor_table[7] & USB_MAXPFIELD) | devsel);
                usb_hstd_enum_get_descriptor(USB_DEVICEADDR, 2);
            break;

            /* Receive Device Descriptor(18) */
            case 2 :
                usb_hstd_enum_get_descriptor(USB_DEVICEADDR, 3);
            break;

            /* Receive Configuration Descriptor(9) */
            case 3 :
                usb_hstd_enum_get_descriptor(USB_DEVICEADDR, 4);
            break;

            /* Receive Configuration Descriptor(xx) */
            case 4 :
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
                descriptor_table = (uint8_t*)gs_usb_hstd_device_descriptor;

                /* If 'vendor_id' and 'product_id' value is defined by PET, */
                /*    system works for compliance test mode. */
                /* Values ??defined by PET is the following. */
                /* vendor_id : 0x1A0A  */
                /* product_id : 0x0101 - 0x0108 , 0x0200 */

                vendor_id = (uint16_t)(descriptor_table[USB_DEV_ID_VENDOR_L]
                        + ((uint16_t)descriptor_table[USB_DEV_ID_VENDOR_H] << 8));

                if (0x1A0A == vendor_id)
                {
                    product_id = (uint16_t)(descriptor_table[USB_DEV_ID_PRODUCT_L]
                            + ((uint16_t)descriptor_table[USB_DEV_ID_PRODUCT_H] << 8));

                    descriptor_table = (uint8_t*)g_usb_hstd_config_descriptor;

                    if (0x0200 == product_id)
                    {
                        descriptor_table = (uint8_t*) g_usb_hstd_config_descriptor;
                        /* Device state */
                        usb_hstd_enum_set_configuration (USB_DEVICEADDR, (uint16_t) (descriptor_table[5]));
                        gs_usb_hstd_enum_seq++;
                        break;
                    }
                }

                /* Driver open */
                gs_usb_disp_param_set   = USB_OFF;
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */

                driver = &g_usb_hstd_device_drv;
                if (USB_DETACHED == driver->devstate)
                {
                    uint16_t retval;
                    retval = usb_hstd_chk_device_class(ptr, driver);
                    if (USB_OK == retval)
                    {
                        driver->devaddr = USB_DEVICEADDR;
                    }
                }

#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
                if (USB_ON == gs_usb_disp_param_set)
                {
                    usb_compliance_disp ((void *)&gs_usb_disp_param);
                }
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
                usb_hstd_enum_get_descriptor ((uint16_t) USB_DEVICEADDR, 5);
            break;

            /* Class Check Result */
            case 5 :
                descriptor_table = (uint8_t*) g_usb_hstd_config_descriptor;

                /* Device state */
                usb_hstd_enum_set_configuration (USB_DEVICEADDR, (uint16_t) (descriptor_table[5]));
            break;

            /* Set Configuration */
            case 6 :
                /* Device enumeration function */
                driver = &g_usb_hstd_device_drv;
                if (USB_DEVICEADDR == driver->devaddr)
                {
                    /* Device state */
                    g_usb_hcd_dev_info.state = USB_CONFIGURED;

                    /* Device speed */
                    g_usb_hcd_dev_info.speed = gs_usb_hstd_device_speed;

                    /* Device state */
                    driver->devstate = USB_CONFIGURED;
                    /* Call Back */
                    (*driver->devconfig)(ptr, USB_DEVICEADDR, (uint16_t) USB_NO_ARG);
                }
                enume_mode = USB_COMPLETEPIPESET;
            break;

            default :
            break;
        }
        gs_usb_hstd_enum_seq++;
    }
    return (enume_mode);
}
/******************************************************************************
 End of function usb_hstd_enumeration
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_chk_device_class
 Description     : Interface class search
 Arguments       : usb_utr_t    *ptr        : Pointer to usb_utr_t structure.
                 : usb_hcdreg_t *driver     : Class driver
 Return          : uint16_t                 : USB_OK / USB_ERROR
 ******************************************************************************/
static uint16_t usb_hstd_chk_device_class (usb_utr_t *ptr, usb_hcdreg_t *driver)
{
    uint8_t *descriptor_table;
    uint16_t total_length1;
    uint16_t total_length2;
    uint16_t result;
    uint16_t *table[4];
    uint16_t vendor_id;
    uint16_t product_id;
    uint16_t id_check;
    uint16_t i;

    descriptor_table = (uint8_t*) gs_usb_hstd_device_descriptor;

    vendor_id  = (uint16_t)(descriptor_table[USB_DEV_ID_VENDOR_L]
               + ((uint16_t)descriptor_table[USB_DEV_ID_VENDOR_H]  << 8));
    product_id = (uint16_t)(descriptor_table[USB_DEV_ID_PRODUCT_L]
               + ((uint16_t)descriptor_table[USB_DEV_ID_PRODUCT_H] << 8));
    /* Check PET connect */
    if ((USB_PET_VID == vendor_id) && (USB_PET_PID == product_id))
    {
        return USB_OK;
    }
    else
    {
        id_check = USB_ERROR;
        /* WAIT_LOOP */
        for (i = 0; i < driver->p_tpl[0]; i++)
        {
            if ((USB_NOVENDOR == driver->p_tpl[(i * 2) + 2]) || (driver->p_tpl[(i * 2) + 2] == vendor_id))
            {
                if ((USB_NOPRODUCT == driver->p_tpl[(i * 2) + 3]) || (driver->p_tpl[(i * 2) + 3] == product_id))
                {
                    id_check = USB_OK;
    #if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
                    gs_usb_disp_param.status    = USB_CT_TPL;
                    gs_usb_disp_param.pid       = product_id;
                    gs_usb_disp_param.vid       = vendor_id;
                    gs_usb_disp_param_set       = USB_ON;
    #endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
                }
            }
        }
    }

    if (USB_ERROR == id_check)
    {
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
        if (USB_IFCLS_HUB == descriptor_table[4])
        {
            gs_usb_disp_param.status    = USB_CT_HUB;
            gs_usb_disp_param.pid       = product_id;
            gs_usb_disp_param.vid       = vendor_id;
            gs_usb_disp_param_set       = USB_ON;
        }
        else
        {
            gs_usb_disp_param.status = USB_CT_NOTTPL;
            gs_usb_disp_param.pid    = product_id;
            gs_usb_disp_param.vid    = vendor_id;
            gs_usb_disp_param_set    = USB_ON;
        }
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */

        return USB_ERROR;
    }

    descriptor_table = (uint8_t*) g_usb_hstd_config_descriptor;
    total_length1 = 0;
    total_length2 = (uint16_t) (descriptor_table[USB_DEV_W_TOTAL_LENGTH_L]
            + ((uint16_t) descriptor_table[USB_DEV_W_TOTAL_LENGTH_H] << 8));

    if (total_length2 > USB_CONFIGSIZE)
    {
        total_length2 = USB_CONFIGSIZE;
    }

    /* Search within configuration total-length */
    /* WAIT_LOOP */
    while (total_length1 < total_length2)
    {
        if (USB_DT_CONFIGURATION == descriptor_table[total_length1 + 1])
        {
            /* Configuration Descriptor ? */
            table[1] = (uint16_t*) &descriptor_table[total_length1];
        }
        else if (USB_DT_INTERFACE == descriptor_table[total_length1 + 1])
        {
            /* Interface Descriptor ? */
            if ((uint16_t) descriptor_table[total_length1 + 5] == driver->ifclass)
            {
                result = USB_ERROR;
                table[0] = (uint16_t*) &gs_usb_hstd_device_descriptor;
                table[2] = (uint16_t*) &descriptor_table[total_length1];
                table[3] = &result;
                (*driver->classcheck)((uint16_t**) &table);

                /* Interface Class */
                g_usb_hcd_driver.ifclass = descriptor_table[total_length1 + 5];
                return result;
            }
        }
        else
        {
            /* Non process */
        }
        total_length1 += descriptor_table[total_length1];
        if (0 == descriptor_table[total_length1])
        {
            break;
        }
    }
    return USB_ERROR;
}
/******************************************************************************
 End of function usb_hstd_chk_device_class
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_submit_result
 Description     : Callback after completion of a standard request.
 Arguments       : usb_utr_t    *ptr    : Pointer to usb_utr_t structure
                 : uint16_t     data1   : Not used
                 : uint16_t     data2t  : Not used
 Return          : none
 ******************************************************************************/
void usb_hstd_submit_result (usb_utr_t *ptr, uint16_t data1, uint16_t data2)
{
    usb_hstd_mgr_snd_mbx((uint16_t) USB_MSG_MGR_SUBMITRESULT, ptr->keyword, ptr->status);
}
/******************************************************************************
 End of function usb_hstd_submit_result
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_enum_get_descriptor
 Description     : Send GetDescriptor to the connected USB device.
 Arguments       : uint16_t     addr        : Device Address
                 : uint16_t     cnt_value   : Enumeration sequence
 Return          : none
 ******************************************************************************/
void usb_hstd_enum_get_descriptor (uint16_t addr, uint16_t cnt_value)
{
    uint8_t *data_table;
    usb_utr_t tran_data;

    gs_usb_hstd_setup_data[0] = USB_GET_DESCRIPTOR | USB_DEV_TO_HOST | USB_REQUEST_TYPE_STANDARD | USB_DEVICE;
    gs_usb_hstd_setup_data[2] = (uint16_t) 0x0000;
    gs_usb_hstd_setup_data[4] = addr;
    switch (cnt_value)
    {
        case 0 :
        /* continue */
        case 1 :
        /* continue */
        case 2 :
        /* continue */
        case 5 :
            gs_usb_hstd_setup_data[1] = (uint16_t) USB_DEV_DESCRIPTOR;
            gs_usb_hstd_setup_data[3] = USB_DEVICESIZE;
            tran_data.p_tranadr = gs_usb_hstd_device_descriptor;
        break;
        case 3 :
            gs_usb_hstd_setup_data[1] = (uint16_t) USB_CONF_DESCRIPTOR;
            gs_usb_hstd_setup_data[3] = (uint16_t) 0x0009;
            tran_data.p_tranadr = g_usb_hstd_config_descriptor;
        break;
        case 4 :
            gs_usb_hstd_setup_data[1] = (uint16_t) USB_CONF_DESCRIPTOR;
            data_table = (uint8_t*) g_usb_hstd_config_descriptor;
            gs_usb_hstd_setup_data[3] = (uint16_t) (((uint16_t) data_table[3] << 8) + (uint16_t) data_table[2]);
            if (gs_usb_hstd_setup_data[3] > USB_CONFIGSIZE)
            {
                gs_usb_hstd_setup_data[3] = USB_CONFIGSIZE;
            }
            tran_data.p_tranadr = g_usb_hstd_config_descriptor;
        break;
        default :
            return;
        break;
    }
    tran_data.keyword  = (uint16_t) USB_PIPE0;
    tran_data.tranlen  = (uint32_t) gs_usb_hstd_setup_data[3];
    tran_data.p_setup  = gs_usb_hstd_setup_data;
    tran_data.status   = USB_DATA_NONE;
    tran_data.complete = (usb_cb_t) &usb_hstd_submit_result;

    usb_hstd_transfer_start_req(&tran_data);
}
/******************************************************************************
 End of function usb_hstd_enum_get_descriptor
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_enum_set_address
 Description     : Send SetAddress to the connected USB device.
 Arguments       : uint16_t     addr    : Device Address.
                 : uint16_t     setaddr : New address.
 Return          : none
 ******************************************************************************/
void usb_hstd_enum_set_address (uint16_t addr, uint16_t setaddr)
{
    usb_utr_t tran_data;

    gs_usb_hstd_setup_data[0] = USB_SET_ADDRESS | USB_HOST_TO_DEV | USB_REQUEST_TYPE_STANDARD | USB_DEVICE;
    gs_usb_hstd_setup_data[1] = setaddr;
    gs_usb_hstd_setup_data[2] = (uint16_t) 0x0000;
    gs_usb_hstd_setup_data[3] = (uint16_t) 0x0000;  /* wLength */
    gs_usb_hstd_setup_data[4] = addr;
    tran_data.keyword = (uint16_t) USB_PIPE0;
    tran_data.p_tranadr = USB_NULL;
    tran_data.tranlen = 0;
    tran_data.p_setup = gs_usb_hstd_setup_data;
    tran_data.status = USB_DATA_NONE;
    tran_data.complete = (usb_cb_t) &usb_hstd_submit_result;

    usb_hstd_transfer_start_req(&tran_data);
}
/******************************************************************************
 End of function usb_hstd_enum_set_address
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_enum_set_configuration
 Description     : Send SetConfiguration to the connected USB device.
 Arguments       : uint16_t     addr    : Device Address.
                 : uint16_t     confnum : Configuration number.
 Return          : none
 ******************************************************************************/
void usb_hstd_enum_set_configuration (uint16_t addr, uint16_t confnum)
{
    usb_utr_t tran_data;

    gs_usb_hstd_setup_data[0] = USB_SET_CONFIGURATION | USB_HOST_TO_DEV | USB_REQUEST_TYPE_STANDARD | USB_DEVICE;
    gs_usb_hstd_setup_data[1] = confnum;
    gs_usb_hstd_setup_data[2] = (uint16_t) 0x0000;
    gs_usb_hstd_setup_data[3] = (uint16_t) 0x0000;  /* wLength */
    gs_usb_hstd_setup_data[4] = addr;
    tran_data.keyword = (uint16_t) USB_PIPE0;
    tran_data.p_tranadr = USB_NULL;
    tran_data.tranlen = 0;
    tran_data.p_setup = gs_usb_hstd_setup_data;
    tran_data.status = USB_DATA_NONE;
    tran_data.complete = (usb_cb_t) &usb_hstd_submit_result;

    usb_hstd_transfer_start_req(&tran_data);
}
/******************************************************************************
 End of function usb_hstd_enum_set_configuration
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_enum_dummy_request
 Description     : Dummy function.
 Arguments       : uint16_t     addr        : Device Address
                 : uint16_t     cnt_value   : Enumeration Sequence
 Return          : none
 ******************************************************************************/
void usb_hstd_enum_dummy_request (uint16_t addr, uint16_t cnt_value)
{
    /* Non */
}
/******************************************************************************
 End of function usb_hstd_enum_dummy_request
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_mgr_suspend
 Description     : Suspend request
 Arguments       : usb_utr_t    *ptr    : Pointer to usb_utr_t structure
                 : uint16_t     info    : Info for release of memory block.
 Return          : none
 ******************************************************************************/
void usb_hstd_mgr_suspend (usb_utr_t *ptr, uint16_t info)
{
    uint16_t devaddr, devsel;
    uint16_t j;
    uint16_t ret;

    devaddr = gsp_usb_hstd_mgr_msg->keyword;
    devsel  = (uint16_t) (devaddr << USB_DEVADDRBIT);

    /* Get root port number from device addr */
    if (USB_DEVADD_NOCONNECT != usb_hstd_devaddr_to_speed(devsel))
    {
        /* PIPE suspend */
        /* WAIT_LOOP */
        for (j = USB_MIN_PIPE_NO; j <= USB_MAX_PIPE_NO; j++)
        {
            /* Agreement device address */
            if (usb_hstd_pipe_to_devsel(j) == devsel)
            {
                /* PID=BUF ? */
                if (USB_PID_BUF == usb_cstd_get_pid(j))
                {
                    usb_cstd_set_nak(j);
                    gs_usb_hstd_suspend_pipe[j] = USB_SUSPENDED;
                }
            }
        }

        /* Get Configuration Descriptor */
        usb_hstd_get_config_desc(devaddr, (uint16_t)0x09);

        /* Check Remote Wake-up */
        if (USB_TRUE == usb_hstd_chk_remote())
        {
            /* Set Feature */
            ret = usb_hstd_set_feature(devaddr, (uint16_t)0xFF);
            if (USB_OK == ret)
            {
                usb_hstd_device_state_ctrl(devaddr, (uint16_t) USB_MSG_HCD_REMOTE);
                g_usb_hstd_mgr_mode = USB_SUSPENDED;
            }
        }
        else
        {
            usb_hstd_device_state_ctrl(devaddr, (uint16_t) USB_MSG_HCD_REMOTE);
            g_usb_hstd_mgr_mode = USB_SUSPENDED;
        }
    }

    usb_hstd_mgr_rel_mpl(ptr, info);
}
/******************************************************************************
 End of function usb_hstd_mgr_suspend
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_device_state_ctrl
 Description     : Setup a call to the function usb_hstd_change_device_state to re-
                 : quest the connected USB Device to change status.
 Arguments       : uint16_t     devaddr : Device address.
                 : uint16_t     msginfo : Request type.
 Return          : none
 ******************************************************************************/
void usb_hstd_device_state_ctrl (uint16_t devaddr, uint16_t msginfo)
{
    if (USB_DEVICEADDR == devaddr)
    {
        usb_hstd_hcd_snd_mbx(msginfo, USB_NULL, (uint16_t*)0, &usb_hstd_status_result);
    }
}
/******************************************************************************
 End of function usb_hstd_device_state_ctrl
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_mgr_reset
 Description     : Request HCD (Host Control Driver) to do a USB bus reset.
 Arguments       : usb_utr_t    *ptr    : Pointer to usb_utr_t structure
                 : uint16_t     devaddr : Device address.
 Return          : none
 ******************************************************************************/
void usb_hstd_mgr_reset (usb_utr_t *ptr, uint16_t addr)
{
    usb_hstd_device_state_ctrl(addr, (uint16_t) USB_MSG_HCD_USBRESET);
    if (USB_DEVICEADDR == addr)
    {
        g_usb_hstd_mgr_mode = USB_DEFAULT;
    }
}
/******************************************************************************
 End of function usb_hstd_mgr_reset
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_mgr_resume
 Description     : Request HCD (Host Control Device) to send RESUME signal.
 Arguments       : usb_utr_t    *ptr : Pointer to usb_utr_t structure
                 : uint16_t     info : Information.
 Return          : none
 ******************************************************************************/
void usb_hstd_mgr_resume (usb_utr_t *ptr, uint16_t info)
{
    uint16_t rootport;
    uint16_t devaddr;
    uint16_t devsel;
    uint16_t j;
    uint16_t ret;
    usb_hcdreg_t *driver;

    devaddr = gsp_usb_hstd_mgr_msg->keyword;
    devsel = (uint16_t) (devaddr << USB_DEVADDRBIT);
    rootport = ptr->keyword;    // debug saeki 20200420

    /* Get root port number from device addr */
    if (USB_DEVADD_NOCONNECT != usb_hstd_devaddr_to_speed(devsel))
    {
        /* Device resume */
        usb_hstd_device_state_ctrl(devaddr, gsp_usb_hstd_mgr_msg->msginfo);

        /* Wait resume signal */
        usb_cpu_delay_xms((uint16_t)100);

        /* Get Configuration Descriptor */
        usb_hstd_get_config_desc(devaddr, (uint16_t) 0x09);

        /* Check Remote wake-up */
        if (USB_TRUE == usb_hstd_chk_remote())
        {
            ret = usb_hstd_clear_feature((uint16_t) 0xFF, (usb_cb_t) &class_trans_result);
            if (USB_OK == ret)
            {
                g_usb_hstd_mgr_mode = USB_CONFIGURED;
            }
        }
        else
        {
            g_usb_hstd_mgr_mode = USB_CONFIGURED;
        }
    }

    if (USB_CONFIGURED == g_usb_hstd_mgr_mode)
    {
        /* PIPE resume */
        /* WAIT_LOOP */
        for (j = USB_MIN_PIPE_NO; j <= USB_MAX_PIPE_NO; j++)
        {
            /* Agreement device address */
            if (usb_hstd_pipe_to_devsel(j) == devsel)
            {
                if (USB_SUSPENDED == gs_usb_hstd_suspend_pipe[j])
                {
                    usb_cstd_set_buf(j);
                    gs_usb_hstd_suspend_pipe[j] = USB_OK;
                }
            }
        }

        driver = &g_usb_hstd_device_drv;
        if ((rootport + USB_DEVICEADDR) == driver->devaddr)
        {
            (*driver->devresume)(ptr, driver->devaddr, (uint16_t) USB_NO_ARG);

            /* Device state */
            g_usb_hcd_dev_info.state = USB_CONFIGURED;

            if (USB_DO_GLOBAL_RESUME == gs_usb_hstd_mgr_msginfo)
            {
                usb_hstd_mgr_chgdevst_cb(ptr);
            }

            /* Device state */
            driver->devstate = USB_CONFIGURED;
        }
    }

    usb_hstd_mgr_rel_mpl(ptr, info);
}
/******************************************************************************
 End of function usb_hstd_mgr_resume
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_chk_remote
 Description     : check remote
 Arguments       : none
 Return value    : uint16_t                 : error info
 ******************************************************************************/
static uint16_t usb_hstd_chk_remote (void)
{
    if ((uint8_t) 0 != (g_usb_hstd_class_data[7] & USB_CF_RWUPON))
    {
        return USB_TRUE;
    }
    return USB_FALSE;
}
/******************************************************************************
 End of function usb_hstd_chk_remote
 ******************************************************************************/

/* Condition compilation by the difference of IP */
/******************************************************************************
 Function Name   : usb_hstd_get_config_desc
 Description     : Set GetConfigurationDescriptor
 Arguments       : uint16_t     addr        : device address
                 : uint16_t     length      : descriptor length
                 : usb_cb_t     complete    : callback function
 Return value    : uint16_t                 : error info
 ******************************************************************************/
uint16_t usb_hstd_get_config_desc (uint16_t addr, uint16_t length)
{
    uint16_t i;

    /* Get Configuration Descriptor */
    g_usb_hstd_setup_data[0] = USB_GET_DESCRIPTOR | USB_DEV_TO_HOST | USB_REQUEST_TYPE_STANDARD | USB_DEVICE;
    g_usb_hstd_setup_data[1] = USB_CONF_DESCRIPTOR;
    g_usb_hstd_setup_data[2] = (uint16_t) 0x0000;
    g_usb_hstd_setup_data[3] = length;
    if (g_usb_hstd_setup_data[3] > CLSDATASIZE)
    {
        g_usb_hstd_setup_data[3] = (uint16_t) CLSDATASIZE;
    }
    g_usb_hstd_setup_data[4] = addr;

    /* WAIT_LOOP */
    for (i = 0; i < g_usb_hstd_setup_data[3]; i++)
    {
        g_usb_hstd_class_data[i] = (uint8_t) 0xFF;
    }
    return usb_hstd_do_command();
}
/******************************************************************************
 End of function usb_hstd_get_config_desc
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hstd_mgr_task
 Description     : The host manager (MGR) task.
 Argument        : usb_vp_int_t stacd          : Task Start Code
 Return          : none
 ******************************************************************************/
void usb_hstd_mgr_task (usb_vp_int_t stacd)
{
    usb_utr_t *ptr;
    usb_hcdreg_t *driver;
    uint16_t rootport;
    uint16_t msginfo;
    usb_ctrl_t ctrl;
    rtos_err_t ret;
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
    usb_compliance_t disp_param;
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */

    /* WAIT_LOOP */
    while(1)
    {
        /* Receive message */
        ret = rtos_receive_mailbox (&g_rtos_usb_mgr_mbx_id, (void **)&ptr, (rtos_time_t)10000);
        if (RTOS_SUCCESS == ret)
        {
            gsp_usb_hstd_mgr_msg = ptr;
            rootport = ptr->keyword;

            /* Detach is all device */
            msginfo = ptr->msginfo;
            switch (ptr->msginfo)
            {
                /* USB-bus control (change device state) */
                case USB_MSG_MGR_STATUSRESULT :
                    ptr->task_id = g_usb_hstd_change_device_state_task_id;
                    usb_hstd_statusresult_process(ptr);
                    usb_hstd_mgr_rel_mpl(ptr, msginfo);
                break;
                case USB_MSG_MGR_SUBMITRESULT :
                    usb_hstd_mgr_submitresult_process(ptr);
                    usb_hstd_mgr_rel_mpl(ptr, msginfo);
                break;
                case USB_MSG_MGR_AORDETACH :
                    usb_hstd_mgr_aordetach_process(ptr);
                break;
                case USB_MSG_MGR_OVERCURRENT :
#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
                    ctrl.p_data =  (void *)g_usb_default_apl_task_id;
#endif /* (BSP_CFG_RTOS_USED != 0) */
                    usb_cstd_set_event(USB_STS_OVERCURRENT, &ctrl);  /* Set Event()  */
                    usb_hstd_vbus_control_off();
                    g_usb_hstd_mgr_mode = USB_DEFAULT;
                    driver = &g_usb_hstd_device_drv;
                    if (driver->rootport == rootport)
                    {
                        /* Root port */
                        g_usb_hcd_dev_info.port = USB_NOPORT;
                        /* Device state */
                        g_usb_hcd_dev_info.state = USB_DETACHED;
                        /* Interface Class : NO class */
                        g_usb_hcd_driver.ifclass = (uint16_t) USB_IFCLS_NOT;
                        /* No connect */
                        g_usb_hcd_dev_info.speed = (uint16_t) USB_DEVADD_NOCONNECT;
                        /* Root port */
                        driver->rootport = USB_NOPORT;
                        /* Device address */
                        driver->devaddr = USB_NODEVICE;
                        /* Device state */
                        driver->devstate = USB_DETACHED;
                    }
                    usb_hstd_mgr_rel_mpl(ptr, msginfo);
                break;
                /* USB_MSG_HCD_SUSPEND */
                case USB_DO_GLOBAL_SUSPEND :
                    ptr->msginfo = USB_MSG_HCD_REMOTE;
                    gs_usb_hstd_mgr_callback = ptr->complete;
                    gs_usb_hstd_mgr_msginfo = msginfo;
                    usb_hstd_mgr_suspend(ptr, msginfo);
                break;
                /* USB_MSG_HCD_RESUME */
                case USB_DO_GLOBAL_RESUME :
                    ptr->msginfo = USB_MSG_HCD_RESUME;
                    gs_usb_hstd_mgr_callback = ptr->complete;
                    gs_usb_hstd_mgr_msginfo = msginfo;
                    usb_hstd_mgr_resume(ptr, msginfo);
                break;
                /* USB_MSG_HCD_RESUME */
                case USB_MSG_HCD_RESUME :
                    gs_usb_hstd_mgr_msginfo = msginfo;
                    usb_hstd_mgr_resume(ptr, msginfo);
                break;
                default :
                    usb_hstd_mgr_rel_mpl(ptr, msginfo);
                break;
            }
        }
    } /* End of while(1) */
}
/******************************************************************************
 End of function usb_hstd_mgr_task
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hstd_statusresult_process
 Description     : USB-bus control (change device state)
 Arguments       : usb_utr_t    *ptr : Pointer to usb_utr_t structure
 Return          : none
 ******************************************************************************/
static void usb_hstd_statusresult_process(usb_utr_t *ptr)
{
    usb_hcdreg_t *driver;

    switch (g_usb_hstd_mgr_mode)
    {
        /* End of reset signal */
        case USB_DEFAULT :
            gs_usb_hstd_device_speed = ptr->result;
            /* Set device speed */
            usb_hstd_set_devaddx_register((uint16_t) USB_DEVICE_0, gs_usb_hstd_device_speed);
            g_usb_hstd_dcp_register = (uint16_t) (USB_DEFPACKET + USB_DEVICE_0);
            gs_usb_hstd_enum_seq = 0;
            if (USB_FSCONNECT == gs_usb_hstd_device_speed)
            {
                /* Full Speed Device Connect */
                hw_usb_hclear_trnensel();
                usb_hstd_enum_get_descriptor ((uint16_t) USB_DEVICE_0, (uint16_t) 0);
            }
            else if (USB_LSCONNECT == gs_usb_hstd_device_speed)
            {
                /* Low Speed Device Connect */
                hw_usb_hset_trnensel();
                usb_hstd_ls_connect_function();
            }
            else
            {
                g_usb_hstd_mgr_mode = USB_DETACHED;
            }
        break;
        /* End of resume signal */
        case USB_CONFIGURED :
            /* This Resume Sorce is moved to usb_hResuCont() by nonOS */
            driver = &g_usb_hstd_device_drv;
            /* rootport : ptr->keyword */
            if ((ptr->keyword + USB_DEVICEADDR) == driver->devaddr)
            {
                (*driver->devresume)(ptr, driver->devaddr, (uint16_t) USB_NO_ARG);
                if (USB_DO_GLOBAL_RESUME == gs_usb_hstd_mgr_msginfo)
                {
                    usb_hstd_mgr_chgdevst_cb(ptr);
                }
                /* Device state */
                g_usb_hcd_dev_info.state = USB_CONFIGURED;
                /* Device state */
                driver->devstate = USB_CONFIGURED;
            }
        break;
        /* Start of suspended state */
        case USB_SUSPENDED :
            driver = &g_usb_hstd_device_drv;
            if (USB_DEVICEADDR == driver->devaddr)
            {
                (*driver->devsuspend)(ptr, driver->devaddr, (uint16_t) USB_NO_ARG);
                if (USB_DO_GLOBAL_SUSPEND == gs_usb_hstd_mgr_msginfo)
                {
                    usb_hstd_mgr_chgdevst_cb(ptr);
                }
                /* Device state */
                g_usb_hcd_dev_info.state = USB_SUSPENDED;
                /* Device state */
                driver->devstate = USB_SUSPENDED;
            }
        break;
        /* Continue of resume signal */
        case USB_RESUME_PROCESS :
                /* No process */
        break;
        case USB_DETACHED :
            if (USB_PORT_DISABLE == gs_usb_hstd_mgr_msginfo)
            {
                usb_hstd_mgr_chgdevst_cb(ptr);
            }
        break;
        default :
                /* No process */
        break;
    }
}
/******************************************************************************
 End of function usb_hstd_statusresult_process
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hstd_mgr_submitresult_process
 Description     : completion of a standard request.
 Arguments       : usb_utr_t    *ptr : Pointer to usb_utr_t structure
 Return          : none
 ******************************************************************************/
static void usb_hstd_mgr_submitresult_process(usb_utr_t *ptr)
{
    uint16_t pipenum;
    uint16_t enume_mode; /* Enumeration mode (device state) */
    uint16_t connect_speed;

    pipenum = ptr->keyword;

    /* Agreement device address */
    usb_hstd_pipe_to_devsel(pipenum);

    /* Get root port number from device addr */
    if (USB_DEFAULT == g_usb_hstd_mgr_mode)
    {
        /* Enumeration */
        /* Peripheral Device Speed support check */
        connect_speed = usb_cstd_port_speed();
        if (USB_NOCONNECT != connect_speed)
        {
            enume_mode = usb_hstd_enumeration(ptr);
            if ((USB_NONDEVICE ==enume_mode) || (USB_NOTTPL ==enume_mode))
            {
                /* Detach Mode */
                g_usb_hstd_mgr_mode = USB_DETACHED;
            }
            else if (USB_COMPLETEPIPESET ==enume_mode)
            {
                g_usb_hstd_mgr_mode = USB_CONFIGURED;
            }
            else
            {
                /* No process */
            }
        }
    }
}
/******************************************************************************
 End of function usb_hstd_mgr_submitresult_process
 ******************************************************************************/



/******************************************************************************
 Function Name   : usb_hstd_mgr_aordetach_process
 Description     : attach or detach
 Arguments       : usb_utr_t    *ptr : Pointer to usb_utr_t structure
 Return          : none
 ******************************************************************************/
static void usb_hstd_mgr_aordetach_process(usb_utr_t *ptr)
{
#if USB_CFG_BC == USB_CFG_ENABLE
    usb_ctrl_t ctrl;
#endif /* USB_CFG_BC == USB_CFG_ENABLE */
    usb_hcdreg_t *driver;
    uint16_t msginfo;
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
    usb_compliance_t disp_param;
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */

    msginfo = ptr->msginfo;
    if (USB_DETACH == ptr->result)
    {
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
        disp_param.status = USB_CT_DETACH;
        disp_param.pid    = USB_NULL;
        disp_param.vid    = USB_NULL;
        usb_compliance_disp ((void *)&disp_param);
#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
        g_usb_hstd_mgr_mode = USB_DETACHED;
        gs_usb_hstd_device_speed = USB_DEVADD_NOCONNECT;
        driver = &g_usb_hstd_device_drv;
        if (USB_DEVICEADDR == driver->devaddr)
        {
#if defined(USB_CFG_HHID_USE)
#endif  /* defined(USB_CFG_HHID_USE) */
            (*driver->devdetach)(ptr, driver->devaddr, (uint16_t) USB_NO_ARG);
            /* Root port */
            g_usb_hcd_dev_info.port = USB_NOPORT;
            /* Device state */
            g_usb_hcd_dev_info.state = USB_DETACHED;
            /* Interface Class : NO class */
            g_usb_hcd_driver.ifclass = (uint16_t) USB_IFCLS_NOT;
            /* No connect */
            g_usb_hcd_dev_info.speed = (uint16_t) USB_DEVADD_NOCONNECT;
            /* Root port */
            driver->rootport = USB_NOPORT;
            /* Device address */
            driver->devaddr = USB_NODEVICE;
            /* Device state */
            driver->devstate = USB_DETACHED;
        }
    }
    else if ((USB_ATTACHL == ptr->result) || (USB_ATTACHF == ptr->result))
    {
#if USB_CFG_COMPLIANCE == USB_CFG_ENABLE
        disp_param.status = USB_CT_ATTACH;
        disp_param.pid    = USB_NULL;
        disp_param.vid    = USB_NULL;
        usb_compliance_disp ((void *)&disp_param);

#endif /* USB_CFG_COMPLIANCE == USB_CFG_ENABLE */
        if (USB_DETACHED == g_usb_hstd_mgr_mode)
        {
            g_usb_hstd_mgr_mode = USB_DEFAULT;
#if USB_CFG_BC == USB_CFG_ENABLE
            /* Call Back */
            /*USB_BC_ATTACH(ptr, USB_DEVICEADDR, (uint16_t)g_usb_hstd_bc.state); */
            if (USB_BC_STATE_CDP == g_usb_hstd_bc.state)
            {
#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
                ctrl.p_data =  (void *)g_usb_default_apl_task_id;
#endif /* (BSP_CFG_RTOS_USED != 0) */
                usb_cstd_set_event(USB_STS_BC, &ctrl);               /* Set Event()  */
            }
#endif /* USB_CFG_BC == USB_CFG_ENABLE */
            usb_cpu_delay_xms((uint16_t) 100);                      /* Don't remove. USB Spec: 7.1.7.3 */
            usb_hstd_mgr_reset(ptr, USB_DEVICEADDR);
        }
    }
    else
    {
        /* No process */
    }
    usb_hstd_mgr_rel_mpl(ptr, msginfo);
}
/******************************************************************************
 End of function usb_hstd_mgr_aordetach_process
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hstd_mgr_open
 Description     : Initialize global variable that contains registration status
                 : of HDCD.
 Arguments       : none
 Return          : none
 ******************************************************************************/
usb_er_t usb_hstd_mgr_open (void)
{
    usb_hcdreg_t *driver;
    uint16_t i;

    /* WAIT_LOOP */
    for (i =0; i< (USB_MAX_PIPE_NO + 1); i++)
    {
        gs_usb_hstd_suspend_pipe[i] = 0;
    }

    memset((void *)&gs_usb_hstd_setup_data, 0, (5*2));
    memset(g_usb_hstd_class_data, 0, CLSDATASIZE);
    memset(gs_usb_hstd_device_descriptor, 0, USB_DEVICESIZE);
    memset(g_usb_hstd_config_descriptor, 0, USB_CONFIGSIZE);
    memset((void *)&g_usb_hstd_setup_data, 0, (5*2));

    gsp_usb_hstd_mgr_msg = 0;
    gs_usb_hstd_enum_seq = 0;
    gs_usb_hstd_mgr_msginfo = 0;
    gs_usb_hstd_mgr_callback = 0;

    /* Manager Mode */
    g_usb_hstd_mgr_mode = USB_DETACHED;
    gs_usb_hstd_device_speed = USB_DEVADD_NOCONNECT;

    driver = &g_usb_hstd_device_drv;

    driver->rootport = USB_NOPORT; /* Root port */
    driver->devaddr = USB_NODEVICE; /* Device address */
    driver->devstate = USB_DETACHED; /* Device state */
    driver->ifclass = (uint16_t) USB_IFCLS_NOT; /* Interface Class : NO class */
    g_usb_hcd_driver.ifclass = (uint16_t) USB_IFCLS_NOT; /* Interface Class : NO class */
    g_usb_hcd_dev_info.port = USB_NOPORT; /* Root port */
    g_usb_hcd_dev_info.state = USB_DETACHED; /* Device state */
    g_usb_hcd_dev_info.speed = (uint16_t) USB_DEVADD_NOCONNECT; /* No connect */

    return USB_OK;
}
/******************************************************************************
 End of function usb_hstd_mgr_open
 ******************************************************************************/

#endif  /* (USB_CFG_MODE & USB_CFG_HOST) == USB_CFG_HOST */
#endif /*(BSP_CFG_RTOS_USED != 0)*/
/******************************************************************************
 End  Of File
 ******************************************************************************/
