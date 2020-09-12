/******************************************************************************
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
 * File Name    : r_usb_basic_mini_if.h
 * Description  : User macro define file
 *                This file is the macrodefinition header file which a user can operate.
******************************************************************************/
/******************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 08.01.2014 1.00 First Release
 *         : 30.11.2018 1.10 Supporting Smart Configurator
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#include "platform.h"
#include "r_usb_basic_define.h"
#include "r_usb_basic_mini_config.h"

#ifndef R_USB_BASIC_MINI_IF_H
#define R_USB_BASIC_MINI_IF_H

/******************************************************************************
 Macro definitions
 ******************************************************************************/
/* USB Request Type Register */
#define     USB_BREQUEST                        (0xFF00u)   /* b15-8 */

#define     USB_STRING_SIZE                     (127u)

/* USB Standard request */
#define     USB_REQUEST_CLEAR_FEATURE           (0x01u)
#define     USB_REQUEST_SET_FEATURE             (0x03u)
#define     USB_REQUEST_SET_ADDRESS             (0x05u)
#define     USB_REQUEST_GET_DESCRIPTOR          (0x06u)
#define     USB_REQUEST_SET_CONFIGURATION       (0x09u)
#define     USB_GET_STATUS                      (0x0000u)
#define     USB_CLEAR_FEATURE                   (0x0100u)
#define     USB_SET_FEATURE                     (0x0300u)
#define     USB_SET_ADDRESS                     (0x0500u)
#define     USB_GET_DESCRIPTOR                  (0x0600u)
#define     USB_SET_DESCRIPTOR                  (0x0700u)
#define     USB_GET_CONFIGURATION               (0x0800u)
#define     USB_SET_CONFIGURATION               (0x0900u)
#define     USB_GET_INTERFACE                   (0x0A00u)
#define     USB_SET_INTERFACE                   (0x0B00u)
#define     USB_SYNCH_FRAME                     (0x0C00u)

/* USB_BMREQUESTTYPEDIR 0x0080u(b7) */
#define USB_HOST_TO_DEV                         (0u)
#define USB_DEV_TO_HOST                         (1u)

/* USB_BMREQUESTTYPETYPE    0x0060u(b6-5) */
#define USB_REQUEST_TYPE_STANDARD               (0x0000u)
#define USB_REQUEST_TYPE_CLASS                  (0x0020u)
#define USB_REQUEST_TYPE_VENDOR                 (0x0040u)
#define USB_STANDARD                            (0u)
#define USB_CLASS                               (1)
#define USB_VENDOR                              (2)

/* USB_BMREQUESTTYPERECIP   0x001Fu(b4-0) */
#define USB_DEVICE                              (0x0000u)
#define USB_INTERFACE                           (0x0001u)
#define USB_ENDPOINT                            (0x0002u)
#define USB_OTHER                               (0x0003u)

#define USB_NULL                                (0x0u)

/* USB pipe number */
#define USB_PIPE0                               (0x0u)
#define USB_PIPE1                               (0x1u)
#define USB_PIPE2                               (0x2u)
#define USB_PIPE3                               (0x3u)
#define USB_PIPE4                               (0x4u)
#define USB_PIPE5                               (0x5u)
#define USB_PIPE6                               (0x6u)
#define USB_PIPE7                               (0x7u)
#define USB_PIPE8                               (0x8u)
#define USB_PIPE9                               (0x9u)

#define USB_EP0                                 (0x0u)
#define USB_EP1                                 (0x1u)
#define USB_EP2                                 (0x2u)
#define USB_EP3                                 (0x3u)
#define USB_EP4                                 (0x4u)
#define USB_EP5                                 (0x5u)
#define USB_EP6                                 (0x6u)
#define USB_EP7                                 (0x7u)
#define USB_EP8                                 (0x8u)
#define USB_EP9                                 (0x9u)
#define USB_EP10                                (0xAu)
#define USB_EP11                                (0xBu)
#define USB_EP12                                (0xCu)
#define USB_EP13                                (0xDu)
#define USB_EP14                                (0xEu)
#define USB_EP15                                (0xFu)

/* Descriptor type  Define */
#define USB_DT_DEVICE                           (0x01u)   /* Configuration Descriptor */
#define USB_DT_CONFIGURATION                    (0x02u)   /* Configuration Descriptor */
#define USB_DT_STRING                           (0x03u)   /* Configuration Descriptor */
#define USB_DT_INTERFACE                        (0x04u)   /* Interface Descriptor */
#define USB_DT_ENDPOINT                         (0x05u)   /* Endpoint Descriptor */
#define USB_DT_DEVICE_QUALIFIER                 (0x06u)   /* Device Qualifier Descriptor */
#define USB_DT_OTHER_SPEED_CONF                 (0x07u)   /* Other Speed Configuration Descriptor */
#define USB_DT_INTERFACE_POWER                  (0x08u)   /* Interface Power Descriptor */
#define USB_DT_OTGDESCRIPTOR                    (0x09u)   /* OTG Descriptor */
#define USB_DT_HUBDESCRIPTOR                    (0x29u)   /* HUB descriptor */

/* Interface class Define */
#define USB_IFCLS_NOT                           (0x00u)   /* Un corresponding Class */
#define USB_IFCLS_AUD                           (0x01u)   /* Audio Class */
#define USB_IFCLS_CDC                           (0x02u)   /* CDC Class */
#define USB_IFCLS_CDCC                          (0x02u)   /* CDC-Control Class */
#define USB_IFCLS_HID                           (0x03u)   /* HID Class */
#define USB_IFCLS_PHY                           (0x05u)   /* Physical Class */
#define USB_IFCLS_IMG                           (0x06u)   /* Image Class */
#define USB_IFCLS_PRN                           (0x07u)   /* Printer Class */
#define USB_IFCLS_MAS                           (0x08u)   /* Mass Storage Class */
#define USB_IFCLS_HUB                           (0x09u)   /* HUB Class */
#define USB_IFCLS_CDCD                          (0x0Au)   /* CDC-Data Class */
#define USB_IFCLS_CHIP                          (0x0Bu)   /* Chip/Smart Card Class */
#define USB_IFCLS_CNT                           (0x0Cu)   /* Content-Security Class */
#define USB_IFCLS_VID                           (0x0Du)   /* Video Class */
#define USB_IFCLS_DIAG                          (0xDCu)   /* Diagnostic Device */
#define USB_IFCLS_WIRE                          (0xE0u)   /* Wireless Controller */
#define USB_IFCLS_APL                           (0xFEu)   /* Application-Specific */
#define USB_IFCLS_VEN                           (0xFFu)   /* Vendor-Specific Class */

/* Endpoint Descriptor  Define */
#define USB_EP_IN                               (0x80u)   /* In Endpoint */
#define USB_EP_OUT                              (0x00u)   /* Out Endpoint */
#define USB_EP_ISO                              (0x01u)   /* Isochronous Transfer */
#define USB_EP_BULK                             (0x02u)   /* Bulk Transfer */
#define USB_EP_INT                              (0x03u)   /* Interrupt Transfer */

/* Configuration descriptor bit define */
#define USB_CF_RESERVED                         (0x80u)   /* Reserved(set to 1) */
#define USB_CF_SELFP                            (0x40u)   /* Self Powered */
#define USB_CF_BUSP                             (0x00u)   /* Bus Powered */
#define USB_CF_RWUPON                           (0x20u)   /* Remote Wake up ON */
#define USB_CF_RWUPOFF                          (0x00u)   /* Remote Wake up OFF */

 /* Descriptor length Define */
#define USB_DD_BLENGTH                          (18u)     /* Device Descriptor Length */
#define USB_CD_BLENGTH                          (9u)      /* Configuration Descriptor Length */
#define USB_ID_BLENGTH                          (9u)      /* Interface Descriptor Length */
#define USB_ED_BLENGTH                          (7u)      /* Endpoint Descriptor Length */

/* Host CLASS Task Command */
#define USB_MSG_HCLASS_OPEN                     (10u)   /* Host enumeration sequence message */

#define USB_SEQ_0                               (0x00u)
#define USB_SEQ_1                               (0x01u)
#define USB_SEQ_2                               (0x02u)
#define USB_SEQ_3                               (0x03u)
#define USB_SEQ_4                               (0x04u)
#define USB_SEQ_5                               (0x05u)
#define USB_SEQ_6                               (0x06u)
#define USB_SEQ_7                               (0x07u)
#define USB_SEQ_8                               (0x08u)
#define USB_SEQ_9                               (0x09u)
#define USB_SEQ_10                              (0x0au)


/*****************************************************************************
 Typedef definitions
 ******************************************************************************/
typedef enum usb_err
{
    USB_SUCCESS = 0,            /* Successful */
    USB_ERR_PARA,               /* Invalid parameter */
    USB_ERR_BUSY,               /* API is busy now */
    USB_ERR_NOT_OPEN,           /* Device is not opened */
    USB_ERR_NOT_SUSPEND,        /* Device is not the suspend state */
    USB_ERR_OVER,               /* Received data size over */
    USB_ERR_SHORT,              /* Received data size short */
    USB_ERR_NG,                 /* Operation failed */
} usb_err_t;

typedef struct usb_descriptor
{
    uint8_t *p_device;            /* Pointer to the device descriptor */
    uint8_t *p_config_f;          /* Pointer to the configuration descriptor for Full-speed */
    uint8_t **pp_string;           /* Pointer to the string descriptor table */
    uint8_t num_string;           /* Num entry String Descriptor */
} usb_descriptor_t;

typedef struct usb_cfg
{
    uint8_t             usb_mode;   /* USB_HOST/USB_PERI */
    usb_descriptor_t    *p_usb_reg; /* Pointer to the usb_decriptor_t structure area */
} usb_cfg_t;

typedef struct usb_setup
{
    uint16_t        type;       /* USB standard/class request type */
    uint16_t        value;      /* Request value */
    uint16_t        index;      /* Request index */
    uint16_t        length;     /* Request length */
} usb_setup_t;

typedef struct usb_ctrl
{
    uint8_t         pipe;       /* USB pipe number */
    uint8_t         type;       /* USB device class etc */
    uint8_t         status;     /* USB device state etc */
    uint32_t        size;       /* Read data size */
    usb_setup_t     setup;      /* usb_setup_t structure area */
} usb_ctrl_t;

typedef struct usb_pipe
{
    uint8_t         ep;         /* Endpoint number */
    uint8_t         type;       /* Transfer type (USB_BULK/USB_INT etc) */
    uint16_t        mxps;       /* Max packet size */
} usb_pipe_t;

typedef struct usb_info
{
    uint8_t type;               /* USB device class type */
    uint8_t speed;              /* USB speed */
    uint8_t status;             /* USB device status */
    uint8_t port;               /* Battery charging information */
} usb_info_t;

typedef enum usb_ct_status
{
    USB_CT_ATTACH,            /* Device Attach Detection */
    USB_CT_DETACH,            /* Device Detach Detection */
    USB_CT_TPL,               /* TPL device connect */
    USB_CT_NOTTPL,            /* Not TPL device connect */
    USB_CT_HUB,               /* USB Hub connect */
    USB_CT_OVRC,              /* Over current */
    USB_CT_NORES,             /* Response Time out for Control Read Transfer */
    USB_CT_SETUP_ERR,         /* Setup Transaction Error */
} usb_ct_status_t;

typedef struct usb_compliance
{
    usb_ct_status_t     status;         /* USB Status */
    uint16_t            vid;            /* Vendor ID  */
    uint16_t            pid;            /* Product ID */
} usb_compliance_t;


/* USB speed type */
typedef enum usb_speed
{
    USB_NOT_CONNECT = 0, USB_LS, USB_FS, USB_HS,
} usb_speed_t;

/* USB request result */
typedef enum usb_setup_status
{
    USB_ACK = 0, USB_STALL,
} usb_setup_status_t;

/* USB driver status */
typedef enum usb_status
{
    USB_STS_ATTACH = 0,
    USB_STS_DEFAULT,
    USB_STS_ADDRESS,
    USB_STS_CONFIGURED,
    USB_STS_SUSPEND,
    USB_STS_RESUME,
    USB_STS_DETACH,
    USB_STS_REQUEST,
    USB_STS_REQUEST_COMPLETE,
    USB_STS_READ_COMPLETE,
    USB_STS_WRITE_COMPLETE,
    USB_STS_BC,
    USB_STS_OVERCURRENT,
    USB_STS_NOT_SUPPORT,
    USB_STS_NONE,
    USB_STS_MSC_CMD_COMPLETE,
} usb_status_t;

/* USB class type */
typedef enum usb_class
{
    USB_PCDC = 0, USB_PCDCC, USB_PHID, USB_PVND,

    USB_HCDC, USB_HCDCC, USB_HHID, USB_HVND,

    USB_HMSC, USB_PMSC, USB_REQUEST
} usb_class_t;

/* USB battery charging type */
typedef enum usb_bcport
{
    USB_SDP = 0, USB_CDP, USB_DCP,
} usb_bcport_t;

/* USB status */
typedef enum usb_onoff
{
    USB_OFF = 0, USB_ON,
} usb_onoff_t;

/* USB read / write type */
typedef enum usb_rw
{
    USB_READ = 0, USB_WRITE,
} usb_rw_t;

/* USB transfer type */
typedef enum usb_transfer
{
    USB_BULK = 0, USB_INT, USB_ISO,
} usb_transfer_t;

/******************************************************************************
 Export global functions
 ******************************************************************************/
extern usb_err_t       R_USB_Open (usb_ctrl_t *p_ctrl, usb_cfg_t *p_cfg);
extern usb_err_t       R_USB_Close (void);
extern usb_err_t       R_USB_Read (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size);
extern usb_err_t       R_USB_Write (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size);
extern usb_err_t       R_USB_Stop (usb_ctrl_t *p_ctrl, uint16_t type);
extern usb_err_t       R_USB_Suspend (void);
extern usb_err_t       R_USB_Resume (void);
extern usb_err_t       R_USB_VbusSetting (uint16_t state);
extern usb_err_t       R_USB_PullUp(uint8_t state);
extern usb_err_t       R_USB_GetInformation (usb_info_t *p_info);
extern usb_err_t       R_USB_PipeRead (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size);
extern usb_err_t       R_USB_PipeWrite (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size);
extern usb_err_t       R_USB_PipeStop (usb_ctrl_t *p_ctrl);
extern usb_err_t       R_USB_GetUsePipe (uint16_t *p_pipe);
extern usb_err_t       R_USB_GetPipeInfo (usb_ctrl_t *p_ctrl, usb_pipe_t *p_info);
extern usb_status_t    R_USB_GetEvent (usb_ctrl_t *p_ctrl);
extern uint32_t        R_USB_GetVersion (void);

#endif /* R_USB_BASIC_MINI_IF_H */
