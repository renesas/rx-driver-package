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
* Copyright (C) 2017(2019) Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
 * File Name    : r_usb_hcdc_mini_if.h
 * Version      : 1.10
 * Description  : Interface file for USB host CDC
 *******************************************************************************/
/*******************************************************************************
 * History   : DD.MM.YYYY Version Description
 *           : 01.09.2014 1.00    First Release
 *           : 01.06.2015 1.01    Added RX231.
 *           : 30.11.2018 1.10    Supporting Smart Configurator
 *           : 31.05.2019 1.11    Added support for GNUC and ICCRX.
 *******************************************************************************/

/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_hcdc_mini_config.h"          /* CDC class define */

#ifndef USB_HCDC_IF_H
#define USB_HCDC_IF_H

/*******************************************************************************
 Macro definitions
 ******************************************************************************/

/* Serial State message Length */
#define  USB_HCDC_SERIAL_STATE_MSG_LEN          (10)

/* CDC Class Requests IDs*/
#define     USB_CDC_SEND_ENCAPSULATED_COMMAND   (0x0000)
#define     USB_CDC_GET_ENACAPSULATED_RESPONSE  (0x0100)
#define     USB_CDC_SET_COMM_FEATURE            (0x0200)
#define     USB_CDC_GET_COMM_FEATURE            (0x0300)
#define     USB_CDC_CLR_COMM_FEATURE            (0x0400)
#define     USB_CDC_SET_LINE_CODING             (0x2000)
#define     USB_CDC_GET_LINE_CODING             (0x2100)
#define     USB_CDC_SET_CONTROL_LINE_STATE      (0x2200)
#define     USB_CDC_SEND_BREAK                  (0x2300)
#define     USB_CDC_REQUEST_NONE                (0xffff)

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

typedef enum
{
    USB_HCDC_DATA_BIT_7 = (7u),     /* Line Coding 7Data bits */
    USB_HCDC_DATA_BIT_8 = (8u)      /* Line Coding 8Data bits */
} usb_hcdc_data_bit_t;

typedef enum
{
    USB_HCDC_STOP_BIT_1  = (0u),    /* Line Coding 1Stop bits */
    USB_HCDC_STOP_BIT_15 = (1u),    /* Line Coding 1.5Stop bits */
    USB_HCDC_STOP_BIT_2  = (2u)     /* Line Coding 2Stop bits */
} usb_hcdc_stop_bit_t;

typedef enum
{
    USB_HCDC_PARITY_BIT_NONE = (0u), /* Line Coding None Parity */
    USB_HCDC_PARITY_BIT_ODD  = (1u), /* Line Coding Odd Parity */
    USB_HCDC_PARITY_BIT_EVEN = (2u)  /* Line Coding Even Parity */
} usb_hcdc_parity_bit_t;

typedef enum
{
#if USB_CFG_ENDIAN == USB_CFG_BIG
    USB_HCDC_SPEED_1200   = (0xb0040000u),  /* Line Coding 1200bps */
    USB_HCDC_SPEED_2400   = (0x60090000u),  /* Line Coding 2400bps */
    USB_HCDC_SPEED_4800   = (0xc0120000u),  /* Line Coding 4800bps */
    USB_HCDC_SPEED_9600   = (0x80250000u),  /* Line Coding 9600bps */
    USB_HCDC_SPEED_14400  = (0x40380000u),  /* Line Coding 14400bps */
    USB_HCDC_SPEED_19200  = (0x004b0000u),  /* Line Coding 19200bps */
    USB_HCDC_SPEED_38400  = (0x00960000u),  /* Line Coding 38400bps */
    USB_HCDC_SPEED_57600  = (0x00e10000u),  /* Line Coding 57600bps */
    USB_HCDC_SPEED_115200 = (0x00c20100u)   /* Line Coding 115200bps */
#else /* USB_CFG_ENDIAN == USB_CFG_BIG */
    USB_HCDC_SPEED_1200   = (1200u),        /* Line Coding 1200bps */
    USB_HCDC_SPEED_2400   = (2400u),        /* Line Coding 2400bps */
    USB_HCDC_SPEED_4800   = (4800u),        /* Line Coding 4800bps */
    USB_HCDC_SPEED_9600   = (9600u),        /* Line Coding 9600bps */
    USB_HCDC_SPEED_14400  = (14400u),       /* Line Coding 14400bps */
    USB_HCDC_SPEED_19200  = (19200u),       /* Line Coding 19200bps */
    USB_HCDC_SPEED_38400  = (38400u),       /* Line Coding 38400bps */
    USB_HCDC_SPEED_57600  = (57600u),       /* Line Coding 57600bps */
    USB_HCDC_SPEED_115200 = (115200u)       /* Line Coding 115200bps */
#endif /* USB_CFG_ENDIAN == USB_CFG_BIG */
} usb_hcdc_line_speed_t;

typedef enum
{
    USB_HCDC_ABSTRACT_STATE  = (0x01u),
    USB_HCDC_COUNTRY_SETTING = (0x02u),
} usb_hcdc_feature_selctor_t;


typedef struct
{
    uint32_t dwdte_rate;    /* Data terminal rate, in bits per second */
    uint8_t  bchar_format;  /* Stop bits */
    uint8_t  bparity_type;  /* Parity */
    uint8_t  bdata_bits;    /* Data bits */
    uint8_t  rsv;           /* Reserve */
} usb_hcdc_linecoding_t;


R_BSP_PRAGMA_UNPACK
typedef struct
{
    union {
        uint16_t    WORD;
        /* CPU bit order rigth */
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_RIGHT_8(
            uint16_t brx_carrier  :1, /* DCD signal */
            uint16_t btx_carrier  :1, /* DSR signal */
            uint16_t bbreak       :1, /* State of break detection mechanism of the device */
            uint16_t bring_signal :1, /* State of ring signal detection of the device */
            uint16_t bframing     :1, /* Framing error */
            uint16_t bparity      :1, /* Parity error */
            uint16_t bover_run    :1, /* Over Run error */
            uint16_t rsv          :9  /* Reserve */
        )BIT;
    };
} usb_hcdc_serialstate_t;
R_BSP_PRAGMA_PACKOPTION

/*******************************************************************************
 Exported global variables
 ******************************************************************************/

/*******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/

#endif /* USB_HCDC_IF_H */
/******************************************************************************
End of file
 ******************************************************************************/
