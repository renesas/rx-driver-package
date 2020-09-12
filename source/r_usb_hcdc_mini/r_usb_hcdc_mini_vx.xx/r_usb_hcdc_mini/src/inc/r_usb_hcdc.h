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
/******************************************************************************
 * File Name     : r_usb_hcdc.h
 * Description   : USB Host CDC class definition
 *******************************************************************************/
/******************************************************************************
 * History   : DD.MM.YYYY Version Description
 *           : 01.09.2014 1.00    First Release
 *           : 01.06.2015 1.01    Added RX231.
 *           : 30.11.2018 1.10    Supporting Smart Configurator
 *           : 31.05.2019 1.11    Added support for GNUC and ICCRX.
 ******************************************************************************/

/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_basic_mini_if.h"
#include "r_usb_typedef.h"              /* Type define */
#include "r_usb_hcdc_mini_if.h"
#include "r_usb_extern.h"               /* USB API public function header */

#ifndef R_USB_HCDC_H
#define R_USB_HCDC_H

/*******************************************************************************
 Macro definitions
 ******************************************************************************/
/* Communications Devices Class Object */

/* Setup packet table size (uint16_t * 5) */
#define  USB_HCDC_SETUP_TBL_SIZE         (5)

/* Class Request format member position */
/* Setup packet table Request position          (uint16_t * 4) */
#define  USB_HCDC_SETUP_POS_REQ          (0)
/* Setup packet table Value position            (uint16_t * 4) */
#define  USB_HCDC_SETUP_POS_VAL          (1)
/* Setup packet table Index position            (uint16_t * 4) */
#define  USB_HCDC_SETUP_POS_INDX         (2)
/* Setup packet table Length position           (uint16_t * 4) */
#define  USB_HCDC_SETUP_POS_LENG         (3)
/* Setup packet table device address position   (uint16_t * 4) */
#define  USB_HCDC_SETUP_POS_DEVADR       (4)
/* Line Coding Structure Length */
#define  USB_HCDC_LINE_CODING_STR_LEN    (7)
/* CommFeature Structure Length */
#define  USB_HCDC_COMM_FEATURE_STR_LEN   (2)

/* CDC Transfer data size */
#define  USB_STRING_SIZE                 (127u)

#define  USB_HCDC_EP_OFFSET_DATA         (0)       /* Ep Table Bulk Pipe offset */
#define  USB_HCDC_EP_OFFSET_CTL          (2)       /* Ep Table Int Pipe offset */
#define  USB_HCDC_EP_SEQ_REVERCE         (0x8000)  /* Ep Table Sequence Reverce */
#define  USB_HCDC_EP_SEQ_MASK            (0x7fff)  /* Ep Table Sequence Reverce Mask */

#define USERVENDOR   (USB_NOVENDOR)    /* Vendor ID nocheck */
#define USERPRODUCT  (USB_NOPRODUCT)   /* Product ID nocheck */

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/
/* Abstract Control Model Requests */
typedef enum
{
    /* Request Code define */
    USB_HCDC_SEND_ENCAPSULATED_COMMAND  = (0x00u),
    USB_HCDC_GET_ENACAPSULATED_RESPONSE = (0x01u),
    USB_HCDC_SET_COMM_FEATURE           = (0x02u),
    USB_HCDC_GET_COMM_FEATURE           = (0x03u),
    USB_HCDC_CLR_COMM_FEATURE           = (0x04u),
    USB_HCDC_SET_LINE_CODING            = (0x20u),
    USB_HCDC_GET_LINE_CODING            = (0x21u),
    USB_HCDC_SET_CONTROL_LINE_STATE     = (0x22u),
    USB_HCDC_SEND_BREAK                 = (0x23u)
} usb_hcdc_abs_req_t;

typedef struct
{
    uint16_t    bdtr : 1;       /* DTR */
    uint16_t    brts : 1;       /* RTS */
    uint16_t    rsv2 : 6;       /* Reserve */
    uint16_t    rsv1 : 8;       /* Reserve */
} usb_hcdc_controllinestate_t;

typedef struct
{
    uint8_t     *p_data;        /* Protocol dependent data */
    uint16_t    wlength;        /* Amount of data, in bytes, associated with this recipient. */
} usb_hcdc_encapsulated_t;

typedef struct
{
    uint16_t    wtime_ms;       /* Duration of Break */
} usb_hcdc_breakduration_t;

typedef struct
{
/* CPU bit order (BIT_LITTLE:little, BIT_BIG:big) */
    uint16_t    bis  : 1;       /* Idle Setting */
    uint16_t    bdms : 1;       /* Data Multiplexed State */
    uint16_t    rsv2 : 6;       /* Reserve */
    uint16_t    rsv1 : 8;       /* Reserve */
} usb_hcdc_abstractstate_t;

typedef struct
{
    uint16_t    country_code;
} usb_hcdc_countrysetting_t;

typedef struct
{
    union {
        uint16_t                        data;
        usb_hcdc_abstractstate_t     abstractstate;
        usb_hcdc_countrysetting_t    countrysetting;
    } status;
    uint16_t    wvalue;             /* Feature selector code. */
} usb_hcdc_commfeature_t;

typedef union
{
    usb_hcdc_linecoding_t        *linecoding;
    usb_hcdc_controllinestate_t  controllinestate;
    usb_hcdc_encapsulated_t      encapsulated;
    usb_hcdc_breakduration_t     breakduration;
    usb_hcdc_commfeature_t       *commfeature;
} usb_hcdc_classrequestparm_t;    /* Class Request Parameter */

typedef struct
{
    uint16_t                        devadr;
    usb_hcdc_abs_req_t            brequestcode;
    usb_hcdc_classrequestparm_t   parm;
    usb_hcb_t                       complete;   /* Call Back Function Info */
} usb_hcdc_classrequest_utr_t;


/* SerialState CB */
typedef void (*USB_HCDC_SERIAL_ST_CB_t)(uint16_t result, usb_hcdc_serialstate_t serial_state); 

/*******************************************************************************
 Exported global variables
 ******************************************************************************/

extern uint32_t     g_usb_read_request_size[USB_MAXPIPE_NUM+1];

/*******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/

extern void         usb_hcdc_dummy_function (void);

#endif  /* R_USB_HCDC_H */
/******************************************************************************
End Of File
 ******************************************************************************/
