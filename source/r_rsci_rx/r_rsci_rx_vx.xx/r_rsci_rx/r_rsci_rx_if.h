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
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_rsci_rx_if.h
* Description  : Functions for using RSCI on RX devices.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           31.03.2021 1.00    Initial Release
*                              Supported for RX671.
*           13.09.2021 1.10    Added the demo for RX671.
*           03.12.2021 2.00    Updated new features in Asynchronous mode
*                              and added support for Manchester mode.
*           31.03.2022 2.10    Supported for RX660.
*           29.07.2022 2.20    Updated demo projects.
*           15.08.2022 2.30    Supported for RX26T.
*                              Fixed to comply with GSCE Coding Standards Rev.6.5.0.
***********************************************************************************************************************/

#ifndef RSCI_IF_H
#define RSCI_IF_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"
#include "r_rsci_rx_config.h"  /* RSCI config definitions */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#if ((R_BSP_VERSION_MAJOR == 6) && (R_BSP_VERSION_MINOR < 10) || (R_BSP_VERSION_MAJOR < 6))
    #error "This module must use BSP module of Rev.6.10 or higher. Please use the BSP module of Rev.6.10 or higher."
#endif

/* Version Number of API. */
#define RSCI_VERSION_MAJOR  (2)
#define RSCI_VERSION_MINOR  (30)

#define RSCI_CLK_INT         (0x00U) /* use internal clock for baud generation */
#define RSCI_CLK_EXT8X       (0x03U) /* use external clock 8x baud rate (ASYNC) */
#define RSCI_CLK_EXT16X      (0x02U) /* use external clock 16x baud rate (ASYNC) */
#define RSCI_DATA_7BIT       (0x03U)
#define RSCI_DATA_8BIT       (0x02U)
#define RSCI_ODD_PARITY      (0x01U)
#define RSCI_EVEN_PARITY     (0x00U)
#define RSCI_NONE_PARITY     (0x02U)
#define RSCI_STOPBITS_2      (0x01U)
#define RSCI_STOPBITS_1      (0x00U)
#define RSCI_STARTBITS_3     (0x01U)
#define RSCI_STARTBITS_1     (0x00U)

/*****************************************************************************
Typedef definitions
******************************************************************************/
typedef enum e_rsci_ch       // RSCI channel numbers
{
    RSCI_CH8=8,
    RSCI_CH9,
    RSCI_CH10,
    RSCI_CH11,
    RSCI_NUM_CH=12
} rsci_ch_t;


typedef enum e_rsci_mode     // RSCI operational modes
{
    RSCI_MODE_OFF=0,         // channel not in use
    RSCI_MODE_ASYNC,         // Asynchronous
    RSCI_MODE_SSPI,          // Simple SPI
    RSCI_MODE_SYNC,          // Synchronous
    RSCI_MODE_MANC,          // Manchester
    RSCI_MODE_MAX=5          // End of modes currently supported
} rsci_mode_t;


typedef enum e_rsci_err      /* RSCI API error codes */
{
    RSCI_SUCCESS=0,
    RSCI_ERR_BAD_CHAN,       // non-existent channel number
    RSCI_ERR_OMITTED_CHAN,   // RSCI_CHx_INCLUDED is 0 in config.h
    RSCI_ERR_CH_NOT_CLOSED,  // chan still running in another mode
    RSCI_ERR_BAD_MODE,       // unsupported or incorrect mode for channel
    RSCI_ERR_INVALID_ARG,    // argument is not one of the predefined values
    RSCI_ERR_NULL_PTR,       // received null ptr; missing required argument
    RSCI_ERR_XCVR_BUSY,      // cannot start data transfer; transceiver busy

    /* Asynchronous/Manchester mode only */
    RSCI_ERR_QUEUE_UNAVAILABLE,  // can't open tx or rx queue or both
    RSCI_ERR_INSUFFICIENT_SPACE, // not enough space in transmit queue
    RSCI_ERR_INSUFFICIENT_DATA,  // not enough data in receive queue

    /* Synchronous/SSPI modes only */
    RSCI_ERR_XFER_NOT_DONE   // data transfer still in progress
    

} rsci_err_t;


/* CHANNEL CONTROL BLOCK HANDLE */

typedef struct st_rsci_ch_ctrl * rsci_hdl_t;


/* RSCI_OPEN() ARGUMENT DEFINITIONS (do NOT change values) */

typedef enum e_rsci_spi_mode
{
    RSCI_SPI_MODE_OFF = 4,   /* channel is in synchronous mode */
    RSCI_SPI_MODE_0 = 0x00,  /* SCR3 Register CPHA=0, CPOL=0; Mode 0: 00 */
    RSCI_SPI_MODE_1 = 0x01,  /* SCR3 Register CPHA=1, CPOL=0; Mode 1: 01 */
    RSCI_SPI_MODE_2 = 0x02,  /* SCR3 Register CPHA=0, CPOL=1; Mode 2: 10 */
    RSCI_SPI_MODE_3 = 0x03   /* SCR3 Register CPHA=1, CPOL=1; Mode 3: 11 */
} rsci_spi_mode_t;


/* Open() p_cfg structure when mode=RSCI_MODE_ASYNC */
typedef struct st_rsci_uart
{
    uint32_t    baud_rate;      // ie 9600, 19200, 115200
    uint8_t     clk_src;        // use RSCI_CLK_INT/EXT8X/EXT16X
    uint8_t     data_size;      // use RSCI_DATA_nBIT
    uint8_t     parity_type;    // use RSCI_ODD/EVEN/NONE_PARITY
    uint8_t     stop_bits;      // use RSCI_STOPBITS_1/2
    bool        msb_first;
    bool        invert_data;
    uint8_t     int_priority;   // interrupt priority; 1=low, 15=high
} rsci_uart_t;

/* Open() p_cfg structure when mode = RSCI_MODE_SYNC or RSCI_MODE_SSPI */
typedef struct st_rsci_sync_sspi
{
    rsci_spi_mode_t  spi_mode;       // clock polarity and phase; unused for sync
    uint32_t         bit_rate;       // ie 1000000 for 1Mbps
    bool             msb_first;
    bool             invert_data;
    uint8_t          int_priority;   // interrupt priority; 1=low, 15=high
} rsci_sync_sspi_t;

/* Open() p_cfg structure when mode=RSCI_MODE_MANC */
typedef struct st_rsci_manc
{
    uint32_t    baud_rate;      // ie 9600, 19200, 115200
    uint8_t     data_size;      // use RSCI_DATA_nBIT
    uint8_t     parity_type;    // use RSCI_ODD/EVEN/NONE_PARITY
    uint8_t     start_bits;     // use RSCI_STARTBITS_1/3
    uint8_t     stop_bits;      // use RSCI_STOPBITS_1/2
    bool        msb_first;
    bool        invert_data;
    uint8_t     int_priority;   // interrupt priority; 1=low, 15=high
} rsci_manc_t;

typedef union
{
    rsci_uart_t      async;
    rsci_sync_sspi_t sync;
    rsci_sync_sspi_t sspi;
    rsci_manc_t      manc;
} rsci_cfg_t;


/* CALLBACK FUNCTION ARGUMENT DEFINITIONS */

typedef enum e_rsci_cb_evt   // callback function events
{
    /* Async/Manc Events */
    RSCI_EVT_TEI,            // TEI interrupt occurred; transmitter is idle
    RSCI_EVT_RX_CHAR,        // received a character; already placed in queue
    RSCI_EVT_RXBUF_OVFL,     // rx queue is full; can't save anymore data
    RSCI_EVT_FRAMING_ERR,    // receiver hardware framing error
    RSCI_EVT_PARITY_ERR,     // receiver hardware parity error

    /* Async Events */
    RSCI_EVT_RX_CHAR_MATCH,  // received a matched character; already placed in queue

    /* SSPI/Sync Events */
    RSCI_EVT_XFER_DONE,      // transfer completed
    RSCI_EVT_XFER_ABORTED,   // transfer aborted

    /* Manc Events */
    RSCI_EVT_MANCHESTER_CODE_ERR,   // receiver hardware manchester code error
    RSCI_EVT_RECEIVE_SYNC_ERR,      // receiver hardware receive sync error
    RSCI_EVT_START_BIT_ERR,         // receiver hardware start bit error
    RSCI_EVT_PREFACE_ERR,           // receiver hardware preface error

    /* Common Events */
    RSCI_EVT_OVFL_ERR        // receiver hardware overrun error

} rsci_cb_evt_t;

typedef struct st_rsci_cb_args // callback arguments
{
    rsci_hdl_t       hdl;
    rsci_cb_evt_t    event;
    uint8_t         byte;   // byte read when error occurred (unused for TEI, XFER_DONE)
    uint8_t         num;    // Number of bytes were stored to queue (used only async(FIFO))
} rsci_cb_args_t;


/* RSCI_CONTROL() ARGUMENT DEFINITIONS */

/* commands */
typedef enum e_rsci_cmd
{
    /* All modes */
    RSCI_CMD_CHANGE_BAUD,              /* change baud/bit rate */
#if ((RSCI_CFG_CH10_FIFO_INCLUDED) || (RSCI_CFG_CH11_FIFO_INCLUDED))
    RSCI_CMD_CHANGE_TX_FIFO_THRESH,    /* change TX FIFO threshold */
    RSCI_CMD_CHANGE_RX_FIFO_THRESH,    /* change RX FIFO threshold */
#endif
    RSCI_CMD_SET_RXI_PRIORITY,         /* change RXI priority level */
    RSCI_CMD_SET_TXI_PRIORITY,         /* change TXI priority level */
    RSCI_CMD_XFER_LSB_FIRST,           /* start from LSB bit when sending */
    RSCI_CMD_XFER_MSB_FIRST,           /* start from MSB bit when sending */
    RSCI_CMD_INVERT_DATA,              /* logic level of send/receive data is invert */

    /* Async commands */
    RSCI_CMD_EN_NOISE_CANCEL,          /* enable noise cancellation */
    RSCI_CMD_OUTPUT_BAUD_CLK,          /* output baud clock on the SCK pin */
    RSCI_CMD_START_BIT_EDGE,           /* detect start bit as falling edge of RXDn pin
                                         (default detect as low level on RXDn pin) */
    RSCI_CMD_GENERATE_BREAK,           /* generate break condition */
    RSCI_CMD_COMPARE_RECEIVED_DATA,    /* Compare received data with comparison data */

    /* Async/Manc commands */
    RSCI_CMD_TX_Q_FLUSH,               /* flush transmit queue */
    RSCI_CMD_RX_Q_FLUSH,               /* flush receive queue */
    RSCI_CMD_TX_Q_BYTES_FREE,          /* get count of unused transmit queue bytes */
    RSCI_CMD_RX_Q_BYTES_AVAIL_TO_READ, /* get num bytes ready for reading */

    /* Async/Sync/Manc commands */
    RSCI_CMD_EN_CTS_IN,                /* enable CTS input (default RTS output) */

    /* SSPI/Sync commands */
    RSCI_CMD_CHECK_XFER_DONE,          /* see if send, rcv, or both are done; RSCI_SUCCESS if yes */
    RSCI_CMD_ABORT_XFER,

    /* SSPI commands */
    RSCI_CMD_CHANGE_SPI_MODE,          /* change clock polarity and phase in SSPI mode */

    /*Sampling/transition timing adjust commands*/
    RSCI_CMD_RX_SAMPLING_ENABLE,
    RSCI_CMD_RX_SAMPLING_DISABLE,
    RSCI_CMD_TX_TRANSITION_TIMING_ENABLE,
    RSCI_CMD_TX_TRANSITION_TIMING_DISABLE,
    RSCI_CMD_SAMPLING_TIMING_ADJUST,
    RSCI_CMD_TRANSITION_TIMING_ADJUST,

    /* Manchester commands */
    RSCI_CMD_START_BIT_PATTERN_LOW_TO_HIGH, /* start bit is a low to high */
    RSCI_CMD_START_BIT_PATTERN_HIGH_TO_LOW, /* start bit is a high to low */
    RSCI_CMD_EN_SYNC,                       /* enable sync, start bit pattern set with SYNC bit in TDR */
    RSCI_CMD_SET_TRASMIT_PREFACE_LENGTH,    /* set preface length in transmit data */
    RSCI_CMD_SET_RECEIVE_PREFACE_LENGTH,    /* set preface length in received frames */
} rsci_cmd_t;

/* RSCI_CMD_CHANGE_BAUD/CHANGE_BITRATE take a ptr to this structure for *p_args */
typedef struct st_rsci_baud
{
    uint32_t    pclk;       // peripheral clock speed; e.g. 24000000 is 24MHz
    uint32_t    rate;       // e.g. 9600, 19200, 115200
} rsci_baud_t;

/* RSCI_CMD_TX_Q_BYTES_FREE and RSCI_CMD_RX_Q_BYTES_AVAIL_TO_READ take a pointer
   to a uint16_t for p_args */

/* RSCI_CMD_SET_RXI_PRIORITY and RSCI_CMD_SET_TXI_PRIORITY take a pointer to a
   uint8_t for p_args */

/* RSCI_CMD_CHANGE_SPI_MODE takes a pointer to an sci_spi_mode_t for p_args */

/*****************************************************************************
Public Functions
******************************************************************************/
rsci_err_t R_RSCI_Open (uint8_t const      chan,
                        rsci_mode_t const   mode,
                        rsci_cfg_t * const  p_cfg,
                        void               (* const p_callback)(void *p_args),
                        rsci_hdl_t * const  p_hdl);

rsci_err_t R_RSCI_Send (rsci_hdl_t const    hdl,
                        uint8_t            *p_src,
                        uint16_t const     length);
                    
rsci_err_t R_RSCI_SendReceive (rsci_hdl_t const hdl,        /* SSPI/SYNC only */
                            uint8_t         *p_src,
                            uint8_t         *p_dst,
                            uint16_t const  length);

rsci_err_t R_RSCI_Receive (rsci_hdl_t const hdl,
                        uint8_t         *p_dst,
                        uint16_t const  length);

rsci_err_t R_RSCI_Control (rsci_hdl_t const     hdl,
                        rsci_cmd_t const     cmd,
                        void                *p_args);

rsci_err_t R_RSCI_Close (rsci_hdl_t const hdl);

uint32_t  R_RSCI_GetVersion (void);

                                    
#endif /* RSCI_IF_H */

