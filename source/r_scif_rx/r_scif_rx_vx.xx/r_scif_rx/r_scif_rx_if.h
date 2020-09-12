/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
* File Name    : r_scif_rx_if.h
* Description  : Interface file for the SCIF driver.
 *********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           31.08.2014 1.00    Initial Release
*           03.03.2015 1.10    Added support for RX71M
*           16.03.2017 1.20    Modified code to prevent extra clocks from being sent in SYNC mode when operating 
*                               at high bit rates.
*           07.12.2018 1.21    Changed Minor version to '21' for update of xml file.
*           01.04.2019 1.22    Changed Minor version to 1.22.
*           11.11.2019 2.00    Changed Version to 2.00.
*                              Added support for GNUC and ICCRX.
*                              Fixed coding style.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_scif_rx_config.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef SCIF_IF_H
#define SCIF_IF_H

/* Version Number of API. */
#define SCIF_VERSION_MAJOR  (2)
#define SCIF_VERSION_MINOR  (0)

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/
typedef enum e_scif_ch      /* SCIF channel numbers */
{
    SCIF_CH8 = 8,
    SCIF_CH9 = 9,
    SCIF_CH10 = 10,
    SCIF_CH11 = 11,
    SCIF_NUM_CH = 4
} scif_ch_t;


typedef enum e_scif_mode    /* SCIF operational modes */
{
    SCIF_MODE_OFF=0,         /* channel not in use */
    SCIF_MODE_ASYNC,         /* Asynchronous */
    SCIF_MODE_SYNC,          /* Synchronous */
    SCIF_MODE_END_ENUM
} scif_mode_t;


typedef enum e_scif_err      /* SCI API error codes */
{
    SCIF_SUCCESS=0,
    SCIF_ERR_BAD_CHAN,       /* non-existent channel number */
    SCIF_ERR_OMITTED_CHAN,   /* SCI_CHx_INCLUDED is 0 in config.h */
    SCIF_ERR_CH_NOT_CLOSED,  /* chan still running in another mode */
    SCIF_ERR_BAD_MODE,       /* unsupported or incorrect mode for channel */
    SCIF_ERR_INVALID_ARG,    /* argument is not one of the predefined values */
    SCIF_ERR_NULL_PTR,       /* received null ptr; missing required argument */
    SCIF_ERR_BUSY,           /* already handling 2 requests */
    SCIF_ERR_IN_PROGRESS
} scif_err_t;


/* CHANNEL CONTROL BLOCK HANDLE */

typedef struct st_scif_ch_ctrl * scif_hdl_t;


/* SCI_OPEN() ARGUMENT DEFINITIONS (do NOT change values) */

typedef enum e_scif_clk
{
    SCIF_CLK_INT    = 0x00,  /* use internal clock for baud generation */
    SCIF_CLK_EXT8X  = 0x03,  /* use external clock 8x baud rate (ASYNC) */
    SCIF_CLK_EXT16X = 0x02   /* use external clock 16x baud rate (ASYNC) */
} scif_clk_t;

typedef enum e_scif_size
{
    SCIF_DATA_7BIT = 0x40,
    SCIF_DATA_8BIT = 0x00
} scif_size_t;

typedef enum e_scif_parity_en
{
    SCIF_PARITY_ON  = 0x20,
    SCIF_PARITY_OFF = 0x00
} scif_parity_en_t;

typedef enum e_parity_t
{
    SCIF_ODD_PARITY  = 0x10,
    SCIF_EVEN_PARITY = 0x00
} scif_parity_t;

typedef enum e_scif_stop_t
{
    SCIF_STOPBITS_2 = 0x08,
    SCIF_STOPBITS_1 = 0x00
} scif_stop_t;

/* Open() p_cfg structure when mode=SCI_MODE_ASYNC */
typedef struct st_scif_uart
{
    uint32_t          baud_rate;        /* ie 9600, 19200, 115200 */
    scif_clk_t        clk_src;
    scif_size_t       data_size;
    scif_parity_en_t  parity_en;
    scif_parity_t     parity_type;
    scif_stop_t       stop_bits;
    uint8_t           txif_priority;    /* txif INT priority; 1=low, 15=high */
    uint8_t           rxif_priority;    /* rxif INT priority; 1=low, 15=high */
    uint8_t           group_priority;   /* teif, erif, brif, drif INT priority; 
                                           must be higher priority than rx_priority */
} scif_uart_t;


/* Open() p_cfg structure when mode = SCI_MODE_SYNC */
typedef struct st_scif_sync
{
    uint32_t        bit_rate;       /* ie 1000000 for 1Mbps */
    bool            msb_first;
    uint8_t         int_priority;   /* interrupt priority; 1=lo, 15=hi */
} scif_sync_t;

typedef union
{
    scif_uart_t     async;
    scif_sync_t     sync;
} scif_cfg_t;


/* CALLBACK FUNCTION ARGUMENT DEFINITIONS */

typedef enum e_scif_cb_evt      /* callback function events */
{
    /* Async Events */
    SCIF_EVT_TX_DONE,           /* Send() requests processed; last bit transmitted */
    SCIF_EVT_RX_DONE,           /* Receive() request processed; some or no data may be in RX FIFO */
    SCIF_EVT_RX_BREAK,          /* received BREAK condition */
    SCIF_EVT_RX_OVERFLOW,       /* receiver FIFO overrun error */
    SCIF_EVT_RX_FRAMING_ERR,    /* received framing error */
    SCIF_EVT_RX_PARITY_ERR,     /* received parity error */

    /* Sync Events */
    SCIF_EVT_XCV_DONE,          /* All request processed */
    SCIF_EVT_XCV_ABORTED        /* transfer aborted; FIFOs reset */
} scif_cb_evt_t;

typedef struct st_scif_cb_args  /* callback arguments */
{
    scif_hdl_t      hdl;
    scif_cb_evt_t   event;
} scif_cb_args_t;


/* SCI_CONTROL() ARGUMENT DEFINITIONS */

/* commands */
typedef enum e_scif_cmd
{
    /* Both modes */
    SCIF_CMD_CHANGE_BAUD,       /* change baud/bit rate */

    /* Async commands */
    SCIF_CMD_EN_FLOW_CTRL,       /* enable CTS/RTS flow control */
    SCIF_CMD_EN_NOISE_CANCEL,    /* enable noise cancellation */
    SCIF_CMD_EN_MSB_FIRST,       /* send/receive MSB first */
    SCIF_CMD_GENERATE_BREAK,     /* generate break condition; resets FIFOs */
    SCIF_CMD_TX_BYTES_REMAINING, /* number total bytes yet to transmit */
    SCIF_CMD_RX_BYTES_PENDING,   /* number bytes yet to receive */
    SCIF_CMD_CHECK_TX_DONE,      /* see if tx requests complete; SCIF_SUCCESS if yes */
    SCIF_CMD_CHECK_RX_DONE,      /* see if rx request complete; SCIF_SUCCESS if yes */
    SCIF_CMD_RESET_TX,           /* abort tx requests; flush transmit FIFO */
    SCIF_CMD_RESET_RX,           /* abort rx requests; flush receive FIFO */

    /* Sync commands */
    SCIF_CMD_CHECK_XCV_DONE,     /* see if Send, Receive, or SendReive reuests
                                    are done; SCIF_SUCCESS if yes */
    SCIF_CMD_RESET_XCV           /* abort transfer requests; flush FIFOs */
} scif_cmd_t;

/* SCIF_CMD_CHANGE_BAUD/CHANGE_BITRATE take a ptr to this structure for *p_args */
typedef struct st_scif_baud
{
    uint32_t    pclk;       /* peripheral clock speed; e.g. 120000000 is 120MHz */
    uint32_t    rate;       /* e.g. 9600, 19200, 115200 */
} scif_baud_t;

/* SCIF_CMD_TX_BYTES_REMAINING and SCIF_CMD_RX_BYTES_PENDING take a pointer */
/* to a uint16_t for p_args */



/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
scif_err_t R_SCIF_Open(uint8_t chan, scif_mode_t const mode, scif_cfg_t * const p_cfg,
                        void (* const p_callback)(void *p_args), scif_hdl_t * const  p_hdl);
scif_err_t R_SCIF_Send(scif_hdl_t const hdl, uint8_t *p_src, uint16_t const length);
scif_err_t R_SCIF_SendReceive(scif_hdl_t const hdl, uint8_t *p_src, uint8_t *p_dst, uint16_t const length);
scif_err_t R_SCIF_Receive(scif_hdl_t const hdl, uint8_t *p_dst,  uint16_t const length);
scif_err_t R_SCIF_Control(scif_hdl_t const hdl, scif_cmd_t const cmd, void *p_args);
scif_err_t R_SCIF_Close(scif_hdl_t const hdl);
uint32_t R_SCIF_GetVersion(void);

#endif /* SCIF_IF_H */

