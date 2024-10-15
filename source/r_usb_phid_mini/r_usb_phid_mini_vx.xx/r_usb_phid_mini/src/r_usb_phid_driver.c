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
 * Copyright (C) 2014(2020) Renesas Electronics Corporation. All rights reserved.
 ******************************************************************************/
/******************************************************************************
 * File Name    : r_usb_phid_driver.c
 * Description  : USB HID application code
 ******************************************************************************/
/*******************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.09.2014 1.00     First Release
 *         : 01.06.2015 1.01     Added RX231.
 *         : 30.11.2018 1.10     Supporting Smart Configurator
 *         : 30.06.2020 1.20     Added support for RTOS.
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_phid_mini_if.h"
#include "r_usb_basic_mini_if.h"
#include "r_usb_typedef.h"
#include "r_usb_extern.h"
#include "r_usb_bitdefine.h"
#include "r_usb_reg_access.h"
#include "r_usb_phid.h"

#ifdef USB_CFG_PHID_USE

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
  * Function Name: usb_phid_write_complete
  * Description  : CallBack Function
  * Arguments    : usb_utr_t   *p_mess  : Pointer to usb_utr_t structure
                 : uint16_t     data1   : Not used
                 : uint16_t     data2   : Not used
 * Return Value : none
 ******************************************************************************/
void usb_phid_write_complete (usb_putr_t *p_mess, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;

    ctrl.pipe   = p_mess->keyword;                    /* Pipe number setting */
    ctrl.type   = USB_PHID;                           /* PHID class  */
    ctrl.size   = 0;

    if (USB_CFG_PHID_INT_IN == ctrl.pipe)
    {
        ctrl.type = USB_PHID;    /* Device class setting  */
    }
    else
    {
        ctrl.type = USB_PHID2;   /* Device class setting  */
    }

    if (USB_DATA_NONE == p_mess->status)
    {
        ctrl.status = USB_SUCCESS;
    }
    else
    {
        ctrl.status = USB_ERR_NG;
    }

#if (BSP_CFG_RTOS_USED != 0)                          /* RTOS */
    ctrl.p_data = (void *)p_mess->task_id;
#endif /* (BSP_CFG_RTOS_USED != 0) */
    usb_cstd_set_event(USB_STS_WRITE_COMPLETE, &ctrl);
}
/******************************************************************************
 End of function usb_phid_write_complete
 ******************************************************************************/

/******************************************************************************
  * Function Name: usb_phid_read_complete
  * Description  : CallBack Function
  * Arguments    : usb_utr_t   *p_mess  : Pointer to usb_utr_t structure
                 : uint16_t     data1   : Not used
                 : uint16_t     data2   : Not used
 * Return Value : none
 ******************************************************************************/
void usb_phid_read_complete (usb_putr_t *p_mess, uint16_t data1, uint16_t data2)
{
    usb_ctrl_t ctrl;

    /* Set Receive data length */
    ctrl.size   = p_mess->read_req_len - p_mess->tranlen;
    ctrl.pipe   = p_mess->keyword;                  /* Pipe number setting */

    if (USB_CFG_PHID_INT_OUT == ctrl.pipe)
    {
        ctrl.type = USB_PHID;    /* Device class setting  */
    }
    else
    {
        ctrl.type = USB_PHID2;   /* Device class setting  */
    }

    switch (p_mess->status)
    {
        case USB_DATA_OK :
            ctrl.status = USB_SUCCESS;
        break;
        case USB_DATA_SHT :
            ctrl.status = USB_ERR_SHORT;
        break;
        case USB_DATA_OVR :
            ctrl.status = USB_ERR_OVER;
            ctrl.size   = 0;
        break;
        case USB_DATA_ERR :
        default :
            ctrl.status = USB_ERR_NG;
            ctrl.size   = 0;
        break;
    }
#if (BSP_CFG_RTOS_USED != 0)                         /* RTOS */
    ctrl.p_data = (void *)p_mess->task_id;
#endif /* (BSP_CFG_RTOS_USED != 0) */
    usb_cstd_set_event(USB_STS_READ_COMPLETE, &ctrl);
}
/******************************************************************************
 End of function usb_phid_read_complete
 ******************************************************************************/

#endif                                 /* USB_CFG_PHID_USE */

/******************************************************************************
 End  Of File
 ******************************************************************************/
