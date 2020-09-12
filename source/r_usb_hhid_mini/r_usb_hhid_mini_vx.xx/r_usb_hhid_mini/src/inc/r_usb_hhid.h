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
* Copyright (C) 2014(2018) Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* File Name     : r_usb_hhid.h
* Version       : 1.10
* Description   : USB Host HID Driver API declaration
******************************************************************************/
/******************************************************************************
 * History   : DD.MM.YYYY Version Description
 *           : 01.09.2014 1.00    First Release
 *           : 01.06.2015 1.01    Added RX231.
 *           : 30.11.2018 1.10    Supporting Smart Configurator
 ******************************************************************************/

/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_basic_mini_if.h"
#include "r_usb_typedef.h"             /* Type define */
#include "r_usb_hhid_mini_if.h"
#include "r_usb_extern.h"              /* USB API public function header */

#ifndef R_USB_HHID_H
#define R_USB_HHID_H

/*******************************************************************************
 Macro definitions
 ******************************************************************************/

#define USB_HID_SHIFT2LEFT(v)       ((uint16_t)((v) << 8))  /* Report Type set */
#define USB_B_INTERFACE_PROTOCOL    (7)                     /* Interface Descriptor bInterfaceProtocol */
#define USB_MSG_HCLASS_OPEN         (10u)                   /* Host CLASS Task Command */


/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/* Enumeration Sequence */
typedef enum
{
    USB_HHID_ENUM_COMPLETE = (0),       /* Complete */
    USB_HHID_ENUM_STR_DT0_REQ,          /* String Descriptor #0 receive request */
    USB_HHID_ENUM_STR_DT0_WAIT,         /* String Descriptor #0 Receive complete */
    USB_HHID_ENUM_STR_IPRODUCT_WAIT     /* iProduct Receive complete */
} usb_hhid_enum_seq_t;


/*******************************************************************************
 Exported global variables
 ******************************************************************************/

extern uint32_t     g_usb_read_request_size[USB_MAXPIPE_NUM+1];

/*******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/

uint8_t     usb_hhid_get_hid_protocol (void);
void        usb_hhid_device_state (uint16_t data, uint16_t state);

#endif  /* R_USB_HHID_H */
/******************************************************************************
End Of File
******************************************************************************/
