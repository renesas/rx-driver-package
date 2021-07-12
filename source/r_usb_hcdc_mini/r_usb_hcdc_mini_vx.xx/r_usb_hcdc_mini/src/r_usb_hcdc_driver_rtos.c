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
 * File Name    : r_usb_hcdc_driver_rtos.c
 * Description  : USB Host CDC Driver
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
#include "r_usb_hcdc.h"

#if (BSP_CFG_RTOS_USED != 0)    /* RTOS */

#include "r_usb_bitdefine.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/
#define     USB_NOPIPE                      ((uint8_t)0xFF)

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static const uint16_t g_usb_hcdc_device_tpl[] =
{
    USB_CFG_TPLCNT, /* Number of tpl table */
    0, /* Reserved */
    USB_CFG_TPL /* Vendor ID, Product ID */
};

static uint16_t usb_hcdc_get_string_info(uint16_t string);
static void usb_hcdc_init (usb_utr_t *ptr, uint16_t data1, uint16_t data2);
static void usb_hcdc_class_check (uint16_t **table);
static void usb_hcdc_configured (usb_utr_t *ptr, uint16_t devadr, uint16_t data2);
static void usb_hcdc_detach (usb_utr_t *ptr, uint16_t devadr, uint16_t data2);

/******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

/******************************************************************************
 Renesas USB Host CDC Driver functions
 ******************************************************************************/

/******************************************************************************
 * Function Name: usb_hcdc_pipe_info
 * Description  : Host Pipe Information check and EP Table Set
 * Arguments    : uint8_t *p_table      : Check Start Descriptor address
                : uint16_t     length  : Configuration Descriptor Length
 * Return Value : uint16_t              : USB_OK / USB_ERROR
 ******************************************************************************/
uint16_t usb_hcdc_pipe_info (uint8_t *p_table, uint16_t length)
{
    uint16_t ofdsc;
    uint16_t pipe_no;
    uint16_t in_pipe;
    uint16_t out_pipe;
    uint16_t status_pipe;
    uint16_t ret = USB_ERROR;

     /* Check Endpoint Descriptor */
    ofdsc = p_table[0];

    /* Pipe initial */
    in_pipe = USB_NOPIPE;
    out_pipe = USB_NOPIPE;
    status_pipe = USB_NOPIPE;

    /* WAIT_LOOP */
    while (ofdsc < length)
    {
        /* Endpoint Descriptor */
        if (USB_DT_ENDPOINT == p_table[ofdsc + USB_EP_B_DESCRIPTORTYPE])
        {
            /* Transfer Type */
            if (USB_EP_BULK == (USB_EP_TRNSMASK & p_table[ofdsc + USB_EP_B_ATTRIBUTES]))
            {
                /* Check if transfer direction is IN */
                if (USB_EP_IN == (USB_EP_DIRMASK & p_table[ofdsc + USB_EP_B_ENDPOINTADDRESS]))
                {
                    out_pipe = USB_TRUE;
                }
                else
                {
                    in_pipe = USB_TRUE;
                }
            }
            else if (USB_EP_INT == (USB_EP_TRNSMASK & p_table[ofdsc + USB_EP_B_ATTRIBUTES]))
            {
                status_pipe = USB_TRUE;
            }
            else
            {
                break;
            }

            /* EP Table pipe Information set */
            pipe_no = usb_cstd_pipe_table_set (USB_HCDC, &p_table[ofdsc]);
            if ( USB_NULL == pipe_no)
            {
                break;
            }

            if (((USB_NOPIPE != in_pipe) && (USB_NOPIPE != out_pipe)) && (USB_NOPIPE != status_pipe))
            {
                ret = USB_OK;
                break;
            }
        }
        ofdsc += p_table[ofdsc];
    }
    return ret;
}
/******************************************************************************
 End of function usb_hcdc_pipe_info
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hcdc_get_string_info
 Description     : Get string descriptors.
 Argument        : uint16_t string
 Return value    : none
 ******************************************************************************/
static uint16_t usb_hcdc_get_string_info(uint16_t string)
{
    uint16_t retval;
    uint8_t *p_desc;
    uint16_t desc_len;

    /* String descriptor */
    retval = usb_hstd_get_string_desc(0);

    if (USB_OK == retval)
    {
        /* String descriptor */
        retval = usb_hstd_get_string_desc(string);
    }
    else /* USB_ERROR */
    {
        usb_hstd_connect_err_event_set();
    }

    /* String descriptor check */
    if (USB_OK == retval)
    {
        if (g_usb_hstd_class_data[0] < (uint8_t) ((32 * 2) + 2))
        {
            g_usb_hstd_class_data[0] = (uint8_t) (g_usb_hstd_class_data[0] / 2);
            g_usb_hstd_class_data[0] = (uint8_t) (g_usb_hstd_class_data[0] - 1);
        }
        else
        {
            g_usb_hstd_class_data[0] = (uint8_t) 32;
        }

        /* Condition compilation by the difference of useful function */
#if defined(USB_DEBUG_ON)
        for (j = (uint16_t)0; j < g_usb_hstd_class_data[0]; j++)
        {
            p_data[j] = g_usb_hstd_class_data[j * (uint16_t)2 + (uint16_t)2];
        }
        p_data[g_usb_hstd_class_data[0]] = 0;
#endif  /* defined(USB_DEBUG_ON) */
    }

    p_desc = (uint8_t *) g_usb_hstd_config_descriptor;
    desc_len = ((uint16_t) *(p_desc + 3)) << 8;
    desc_len += (uint16_t) *(p_desc + 2);

    /* Pipe Information table set */
    retval = usb_hcdc_pipe_info(p_desc, desc_len);
    if (USB_ERROR == retval)
    {
        /* Pipe Information table error */
        usb_hstd_connect_err_event_set();
    }
    else
    {
        retval = USB_OK;
    }

    return retval;
} /* End of function usb_hcdc_get_string_info() */

/******************************************************************************
 Function Name   : usb_hcdc_registration
 Description     : registration of Communications Devices class Driver
 Argument        : none
 Return          : none
 ******************************************************************************/
void usb_hcdc_registration (void)
{
    usb_hcdreg_t driver;

    /* Driver registration */
#if USB_CFG_HCDC_IFCLS == USB_CFG_CDC
    driver.ifclass = (uint16_t) USB_IFCLS_CDC;                  /* CDC Communications Interface */

#else   /* USB_CFG_HCDC_IFCLS == USB_CFG_CDC */
    driver.ifclass = (uint16_t)USB_IFCLS_VEN;                   /* CDC Communications Interface */

#endif  /* USB_CFG_HCDC_IFCLS == USB_CFG_CDC */
    driver.p_tpl      = (uint16_t*) &g_usb_hcdc_device_tpl; /* Target peripheral list */
    driver.classinit  = (usb_cb_t) &usb_hcdc_init; /* Driver init */
    driver.classcheck = (usb_cb_check_t) &usb_hcdc_class_check; /* Driver check */
    driver.devconfig  = (usb_cb_t) &usb_hcdc_configured;        /* Device configuered */
    driver.devdetach  = (usb_cb_t) &usb_hcdc_detach;            /* Device detach */
    driver.devsuspend = (usb_cb_t) &usb_hstd_dummy_function;    /* Device suspend */
    driver.devresume  = (usb_cb_t) &usb_hstd_dummy_function;    /* Device resume */

    usb_hstd_driver_registration(&driver); /* Host CDC class driver registration. */
}
/******************************************************************************
 End of function usb_hcdc_registration
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hcdc_configured
 Description     : Callback function for CDC device configuered
 Argument        : usb_utr_t *ptr   : Pointer to usb_utr_t structure.
                 : uint16_t     devadr  : Device Adrress
                 : uint16_t     data2   : Not used
 Return value    : none
 ******************************************************************************/
static void usb_hcdc_configured (usb_utr_t *ptr, uint16_t devadr, uint16_t data2)
{
    usb_ctrl_t ctrl;

    usb_cstd_pipe_reg_set(); /* Host CDC Pipe registration */
    ctrl.type = USB_HCDC;
    ctrl.p_data = (void *)g_usb_default_apl_task_id;
    usb_cstd_set_event(USB_STS_CONFIGURED, &ctrl); /* Set Event()  */
}
/******************************************************************************
 End of function usb_hcdc_configured
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hcdc_detach
 Description     : Callback function for CDC device detach
 Argument        : usb_utr_t *ptr   : Pointer to usb_utr_t structure.
                 : uint16_t     devadr  : Device Adrress
                 : uint16_t     data2   : Not used
 Return value    : none
 ******************************************************************************/
static void usb_hcdc_detach (usb_utr_t *ptr, uint16_t devadr, uint16_t data2)
{
    usb_ctrl_t ctrl;

    usb_cstd_pipe_table_clear();
    ctrl.p_data = (void *)g_usb_default_apl_task_id;
    usb_cstd_set_event(USB_STS_DETACH, &ctrl); /* Set Event()  */
}
/******************************************************************************
 End of function usb_hcdc_detach
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hcdc_class_check
 Description     : class check.
 Arguments       : uint16_t     **table : Pointer to data table.
 Return          : none
 ******************************************************************************/
static void usb_hcdc_class_check (uint16_t **table)
{
    uint16_t retval;
    uint8_t *p_device_table;

    p_device_table = (uint8_t*) (table[0]); /* Set device descriptor address */

    *table[3] = USB_OK; /* Set class check done  */

    /* Get String Descriptors */
    retval = usb_hcdc_get_string_info(p_device_table[USB_DEV_I_PRODUCT]);
    if (USB_OK != retval)
    {
        *table[3] = USB_ERROR;
        return;
    }
}
/******************************************************************************
 End of function usb_hcdc_class_check()
 ******************************************************************************/


/******************************************************************************
 Function Name   : usb_hcdc_read_complete
 Description     : CallBack Function
 Argument        : usb_utr_t    *mess   : Pointer to usb_utr_t structure.
                 : uint16_t     data1   : Not used
                 : uint16_t     data2   : Not used
 Return          : none
 ******************************************************************************/
void usb_hcdc_read_complete (usb_utr_t *mess, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;

    ctrl.pipe = mess->keyword; /* Pipe number setting */
    if (USB_CFG_HCDC_BULK_IN == ctrl.pipe)
    {
        ctrl.type = USB_HCDC; /* CDC Data class  */
    }

    else /* USB_CFG_HCDC_INT_IN */
    {
        ctrl.type = USB_HCDCC; /* CDC Control class  */
    }
    ctrl.size = mess->read_req_len - mess->tranlen;                 /* Set Receive data length */

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
        ctrl.size   = 0;                                            /* Set Receive data length */
    }
    else
    {
        /* USB_DATA_ERR */
            ctrl.status = USB_ERR_NG;
        ctrl.size   = 0;                                            /* Set Receive data length */
    }
    ctrl.p_data = (void *)mess->task_id;

    usb_cstd_set_event(USB_STS_READ_COMPLETE, &ctrl); /* Set Event()  */
}
/******************************************************************************
 End of function usb_hcdc_read_complete
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hcdc_write_complete
 Description     : CallBack Function
 Argument        : usb_utr_t    *mess   : Pointer to usb_utr_t structure.
                 : uint16_t     data1   : Not used
                 : uint16_t     data2   : Not used
 Return          : none
 ******************************************************************************/
void usb_hcdc_write_complete (usb_utr_t *mess, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;

    ctrl.pipe = mess->keyword;  /* Pipe number setting */
    ctrl.type = USB_HCDC;       /* CDC Data class  */
    ctrl.size   = 0;            /* Set data length 0   */

    if (USB_DATA_NONE == mess->status)
    {
        ctrl.status = USB_SUCCESS;
    }
    else
    {
        ctrl.status = USB_ERR_NG;
    }
    ctrl.p_data = (void *)mess->task_id;

    usb_cstd_set_event(USB_STS_WRITE_COMPLETE, &ctrl); /* Set Event()  */
}
/******************************************************************************
 End of function usb_hcdc_write_complete
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hcdc_driver_start
 Description     : USB host Initialize process
 Arguments       : none
 Return          : none
 ******************************************************************************/
void usb_hcdc_driver_start (void)
{
}
/******************************************************************************
 End of function usb_hcdc_driver_start()
 ******************************************************************************/

/******************************************************************************
 Function Name   : usb_hcdc_init
 Description     : Initialize of Host CDC driver.
                 : uint16_t data1, uint16_t data2)
 Arguments       : uint16_t     data1   : not use
                 : uint16_t     data2   : not use
 Return value    : none
 ******************************************************************************/
static void usb_hcdc_init (usb_utr_t *ptr, uint16_t data1, uint16_t data2)
{
}
/******************************************************************************
 End of function usb_hcdc_init
 ******************************************************************************/

#endif /*(BSP_CFG_RTOS_USED != 0)*/
/******************************************************************************
 End  Of File
 ******************************************************************************/
