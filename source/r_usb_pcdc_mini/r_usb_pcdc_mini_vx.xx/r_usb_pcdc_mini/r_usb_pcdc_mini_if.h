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
 * Copyright (C) 2014(2019) Renesas Electronics Corporation. All rights reserved.
 ******************************************************************************/
/******************************************************************************
 * File Name    : r_usb_pcdc_mini_if.h
 * Description  : Interface file for USB PCDC API
 ******************************************************************************/
/*******************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.09.2014 1.00    First Release
 *         : 30.11.2018 1.10    Supporting Smart Configurator
 *         : 31.05.2019 1.11    Added support for GNUC and ICCRX.
 ******************************************************************************/

/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "r_usb_pcdc_mini_config.h"

#ifndef USB_PCDC_MINI_IF_H
#define USB_PCDC_MINI_IF_H

/*******************************************************************************
 Macro definitions
 ******************************************************************************/

/* CDC Class Requests IDs*/
#define USB_PCDC_SET_LINE_CODING            (0x2000u)
#define USB_PCDC_GET_LINE_CODING            (0x2100u)
#define USB_PCDC_SET_CONTROL_LINE_STATE     (0x2200u)

/* setup packet table size (uint16_t * 5) */
#define     USB_CDC_SETUP_TBL_BSIZE         (10u)

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/

R_BSP_PRAGMA_UNPACK
typedef struct
{
    union {
        uint16_t    WORD;
        /* CPU bit order rigth */
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_RIGHT_8(
            uint16_t b_rx_carrier :1,   /* DCD signal */
            uint16_t b_tx_carrier :1,   /* DSR signal */
            uint16_t b_break      :1,   /* State of break detection mechanism of teh device */
            uint16_t b_ring_signal:1,   /* State of ring signal detection of the device */
            uint16_t b_framing    :1,   /* Framing error */
            uint16_t b_parity     :1,   /* Parity error */
            uint16_t b_over_run   :1,   /* Over Run error */
            uint16_t rsv          :9    /* reserve */
        )BIT;
    };
} usb_serial_state_bitmap_t;
R_BSP_PRAGMA_PACKOPTION

/* Class Notification Serial State */
typedef union
{
    uint32_t                    word;
    usb_serial_state_bitmap_t   bit;
} usb_sci_serialstate_t;        /*USB_SCI_SerialState_t; */

typedef struct
{
    uint32_t dw_dte_rate;       /* Data terminal rate, in bits per second */
    uint8_t  b_char_format;     /* Stop bits */
    uint8_t  b_parity_type;     /* Parity */
    uint8_t  b_data_bits;       /* Data bits */
    uint8_t  rsv;               /* reserve */
} usb_pcdc_linecoding_t;        /* USB_PCDC_LineCoding_t; */

R_BSP_PRAGMA_UNPACK
typedef struct
{
    union {
        uint16_t    WORD;
        /* CPU bit order rigth */
        R_BSP_ATTRIB_STRUCT_BIT_ORDER_RIGHT_3(
            uint16_t b_dtr :1,          /* DTR */
            uint16_t b_rts :1,          /* RTS */
            uint16_t rsv   :14          /* reserve */
        )BIT;
    };
} usb_pcdc_ctrllinestate_t;     /* USB_PCDC_ControlLineState_t; */
R_BSP_PRAGMA_PACKOPTION

/*******************************************************************************
 Exported global variables
 ******************************************************************************/

/*******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/

#endif /* USB_PCDC_MINI_IF_H */
/******************************************************************************
End Of File
 ******************************************************************************/
