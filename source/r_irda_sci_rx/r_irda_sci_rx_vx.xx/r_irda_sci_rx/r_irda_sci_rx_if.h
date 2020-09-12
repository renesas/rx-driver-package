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
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 01.12.2014 1.00     First Release
*              : 27.01.2015 1.01     Chage symbol name of Arguments 
*                                    None change proceedures.
*******************************************************************************/
/*******************************************************************************
* System Name  : interface file for irda fit module.
* File Name    : r_irda_sci_rx_if.h
* Version      : 1.01
* Device       : RX
* Abstract     : include file for using irda fit module.
* Tool-Chain   : Renesas RXC Toolchain v2.02.00
* OS           : not use
* H/W Platform : RSKRX113
* Description  : Functions for using IRDA on RX devices. 
* Limitation   : none
*******************************************************************************/
#ifndef IRDA_SCI_IF_H
#define IRDA_SCI_IF_H

/*******************************************************************************
  Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* Fixed width integer support. */
#include <stdint.h>
/* bool support */
#include <stdbool.h>

/* Used for configuring the IrDA code */
#include "r_irda_sci_rx_config.h"

/*******************************************************************************
  Macro definitions
*******************************************************************************/
/* Version Number of API. */
#define IRDA_SCI_VERSION_MAJOR  (1)
#define IRDA_SCI_VERSION_MINOR  (01)

/*******************************************************************************
  Typedef definitions
*******************************************************************************/
/* SCI channel numbers */
typedef enum e_irda_sci_ch
{
    IRDA_SCI_CH0=0,
    IRDA_SCI_CH1,
    IRDA_SCI_CH2,
    IRDA_SCI_CH3,
    IRDA_SCI_CH4,
    IRDA_SCI_CH5,
    IRDA_SCI_CH6,
    IRDA_SCI_CH7,
    IRDA_SCI_CH8,
    IRDA_SCI_CH9,
    IRDA_SCI_CH10,
    IRDA_SCI_CH11,
    IRDA_SCI_CH12,
    IRDA_SCI_NUM_CH
} irda_sci_ch_t;

/* API error codes */
typedef enum e_irda_sci_err
{
    IRDA_SCI_SUCCESS=0,
    IRDA_SCI_ERR_LOCK_FUNC,          /* Hardware Lock                                */
    IRDA_SCI_ERR_BAD_CHAN,           /* non-existent channel number                  */
    IRDA_SCI_ERR_OMITTED_CHAN,       /* IRDA_SCI_CHx_INCLUDED is 0 in config.h       */
    IRDA_SCI_ERR_CH_NOT_CLOSED,      /* chan still running                           */
    IRDA_SCI_ERR_INVALID_ARG,        /* argument is not one of the predefined values */
    IRDA_SCI_ERR_NULL_PTR,           /* received null ptr; missing required argument */
    IRDA_SCI_ERR_QUEUE_UNAVAILABLE,  /* can't open tx or rx queue or both            */
    IRDA_SCI_ERR_INSUFFICIENT_SPACE, /* not enough space in transmit queue           */
    IRDA_SCI_ERR_INSUFFICIENT_DATA,  /* not enough data in receive queue             */
} irda_sci_err_t;

/*----- R_IRDA_SCI_Open() ARGUMENT DEFINITIONS -----*/

/*----- Channel control block handle -----*/
typedef struct st_irda_sci_ch_ctrl * irda_sci_hdl_t;

/* IrDA Output Pulse Width Select Data (do NOT change values) */
#define IRDA_SCI_OUT_WIDTH_3_16  (0x00U)
#define IRDA_SCI_OUT_WIDTH_2     (0x01U)
#define IRDA_SCI_OUT_WIDTH_4     (0x02U)
#define IRDA_SCI_OUT_WIDTH_8     (0x03U)
#define IRDA_SCI_OUT_WIDTH_16    (0x04U)
#define IRDA_SCI_OUT_WIDTH_32    (0x05U)
#define IRDA_SCI_OUT_WIDTH_64    (0x06U)
#define IRDA_SCI_OUT_WIDTH_128   (0x07U)

/* R_IRDA_SCI_Open() p_cfg structure */
typedef struct st_irda_sci
{
    uint32_t    baud_rate;      /* ie 9600, 19200, 115200                     */
    uint8_t     clk_out_width;  /* IrDA Output Pulse Width                    */
    uint8_t     int_priority;   /* txi, tei, rxi INT priority; 1=low, 15=high */
} irda_sci_t;

/* handle for channel (ptr to chan control block) */
typedef irda_sci_t irda_control;

/*----- R_IRDA_SCI_Control() ARGUMENT DEFINITIONS -----*/
/* commands */
typedef enum e_irda_sci_cmd
{
    IRDA_SCI_CMD_TX_Q_FLUSH,               /* flush transmit queue                     */
    IRDA_SCI_CMD_RX_Q_FLUSH,               /* flush receive queue                      */
    IRDA_SCI_CMD_TX_Q_BYTES_FREE,          /* get count of unused transmit queue bytes */
    IRDA_SCI_CMD_RX_Q_BYTES_AVAIL_TO_READ, /* get num bytes ready for reading          */

} irda_sci_cmd_t;

/* IRDA_SCI_CMD_TX_Q_BYTES_FREE and IRDA_SCI_CMD_RX_Q_BYTES_AVAIL_TO_READ take a pointer to a uint16_t for p_args */

/*----- CALLBACK FUNCTION ARGUMENT DEFINITIONS -----*/

/* callback function events */
typedef enum e_irda_sci_cb_event
{
    IRDA_SCI_EVT_TEI,            /* TEI interrupt occurred; transmitter is idle   */
    IRDA_SCI_EVT_RX_CHAR,        /* received a character; already placed in queue */
    IRDA_SCI_EVT_RXBUF_OVFL,     /* rx queue is full; can't save anymore data     */
    IRDA_SCI_EVT_FRAMING_ERR,    /* receiver hardware framing error               */
    IRDA_SCI_EVT_OVFL_ERR        /* receiver hardware overrun error               */
} irda_sci_cb_event_t;

/* callback arguments */
typedef struct st_irda_sci_cb_args
{
    irda_sci_hdl_t       hdl;       /* handle for channel (ptr to chan control block) */
    irda_sci_cb_event_t  event;     /* Factors that the callback function called */
    uint8_t              byte;      /* byte read when error occurred (unused for TEI, XFER_DONE) */
} irda_sci_cb_args_t;

/*******************************************************************************
  Public Functions
*******************************************************************************/
extern irda_sci_err_t R_IRDA_SCI_Open       (uint8_t const          chan,
                                            irda_sci_t * const      p_cfg,
                                            void                    (* const p_callback)(void *p_args),
                                            irda_sci_hdl_t * const  p_hdl);

extern irda_sci_err_t R_IRDA_SCI_Send       (irda_sci_hdl_t const   hdl,
                                            uint8_t                 *p_src,
                                            uint16_t const          length);

extern irda_sci_err_t R_IRDA_SCI_Receive    (irda_sci_hdl_t const   hdl,
                                            uint8_t                 *p_dst,
                                            uint16_t const          length);

extern irda_sci_err_t R_IRDA_SCI_Control    (irda_sci_hdl_t const   hdl,
                                            irda_sci_cmd_t const    cmd,
                                            void                    *p_args);

extern irda_sci_err_t R_IRDA_SCI_Close      (irda_sci_hdl_t const   hdl);
extern uint32_t       R_IRDA_SCI_GetVersion (void);

#endif /* IRDA_SCI_IF_H */

/* End of File */
