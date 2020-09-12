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
 ******************************************************************************/
/******************************************************************************
 * File Name    : r_usb_phid_mini_if.h
 * Description  : Interface file for USB Peripheral HID class API for RX
 ******************************************************************************/
/******************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.09.2014 1.00    First Release
 *         : 01.06.2015 1.01    Added RX231.
 *         : 30.11.2018 1.10    Supporting Smart Configurator
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

/* User specific options for Flash API */
#include "r_usb_phid_mini_config.h"

#ifndef USB_PHID_MINI_IF_H
#define USB_PHID_MINI_IF_H

/******************************************************************************
 Macro definitions
 ******************************************************************************/

#define USB_PHID_ECHO_MODE          (0)           /*  Peripheral HID Application echo mode */
#define USB_PHID_KEYBOARD_MODE      (1)           /* Peripheral HID Application keyboard mode */
#define USB_PHID_MOUSE_MODE         (2)           /* Peripheral HID Application mouse mode */

/*----- HID Mouse Class Request Defines --------*/
/* HID Class Request codes */
#define USB_GET_REPORT                  (0x0100u)
#define USB_GET_IDLE                    (0x0200u)
#define USB_GET_PROTOCOL                (0x0300u)
#define USB_SET_IDLE                    (0x0A00u)
#define USB_SET_REPORT                  (0x0900u)
#define USB_SET_PROTOCOL                (0x0B00u)

/* PHID reception message type definition */
#define USB_GET_REPORT_DESCRIPTOR       (0x2200u)
#define USB_GET_HID_DESCRIPTOR          (0x2100u)

/* ----- DESCRIPTOR Types ----- */
#define USB_DT_TYPE_GETREPORT           ((uint8_t)0x01)   /* Report Type Input */
#define USB_DT_TYPE_SETREPORT           ((uint8_t)0x02)   /* Report Type Output */
#define USB_DT_TYPE_HIDDESCRIPTOR       ((uint8_t)0x21)   /* HID descriptor type */
#define USB_DT_TYPE_RPTDESCRIPTOR       ((uint8_t)0x22)   /* Report descriptor type */

/* ----- Subclass Codes ----- */
#define USB_IFSUB_NOBOOT                ((uint8_t)0x00)   /* No Subclass */
#define USB_IFSUB_BOOT                  ((uint8_t)0x01)   /* Boot Interface Subclass */

/* ----- Protocol Codes ----- */
#define USB_IFPRO_NONE                  ((uint8_t)0x00)   /* Demonstration */
#define USB_IFPRO_KBD                   ((uint8_t)0x01)   /* Keyboard */
#define USB_IFPRO_MSE                   ((uint8_t)0x02)   /* Mouse */

/* Peripheral HID Max Packet Size */
#define USB_PHID_MXPS_KBD               (8u)
#define USB_PHID_MXPS_MSE               (3u)
#define USB_PHID_MXPS_ECHO              (64u)

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

/*******************************************************************************
 Exported global variables
 ******************************************************************************/

/*******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/

#endif /* USB_PHID_MINI_IF_H */
/******************************************************************************
 End  Of File
 ******************************************************************************/
