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
* File Name    : r_rspia_rx_private.h
* Description  : Private definitions for r_rspia_rx.c driver on RX devices.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 31.03.2021 1.00     First Release
*                               Supported for RX671.
***********************************************************************************************************************/

#ifndef RSPIA_PRIVATE_H
#define RSPIA_PRIVATE_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Fixed width integer support. */
/* Access to peripherals and board defines. */
#include "platform.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* --- Definitions of data ---- */
#define RSPIA_BYTE_DATA     (0x01)
#define RSPIA_WORD_DATA     (0x02)
#define RSPIA_LONG_DATA     (0x04)

#define RSPIA_CLK_DIV_BASE  (512)   /* Maximum divider FOR N = 0 */
#define RSPIA_CLK_MAX_DIV   (4096)  /* Maximum RSPIA CLK divider */
#define RSPIA_CLK_MIN_DIV   (2)     /* Minimum RSPIA CLK divider */

#define RSPIA_FIFO_FRAME_SIZE   (4)

/* Bit mask definitions of RSPIA register settings options. These are for registers that contain bit fields
 * to control individual options. Do not change. */
/* RSPIA Control Register (SPCR) */
#define RSPIA_SPCR_SPE     (0x00000001) /* 0: RSPIA operation disabled.   1: RSPIA operation enabled. */
#define RSPIA_SPCR_RCKS    (0x00000000) /* 0: MRIOCLK (Adjust with Analog Delay (SPTAR.RTFA[2:0])). */
                                        /* 1: MRCLK (Adjust with Digital Delay (SPRTAR.ARST)). */
#define RSPIA_SPCR_SPPE    (0x00000100) /* 0: No parity.    1: Adds parity bit. */
#define RSPIA_SPCR_SPOE    (0x00000200) /* 0: Even parity.  1: Odd parity. */
#define RSPIA_SPCR_PTE     (0x00000800) /* 0: Disable parity self diagnostic.   1: Enable parity self diagnostic. */
#define RSPIA_SPCR_SCKASE  (0x00001000) /* 0: Disables the RSPCK auto-stop function.    1: Enables the RSPCK auto-stop function */
#define RSPIA_SPCR_SCKDDIS (0x00002000) /* 0: Disables the RSPCK auto-stop function.    1: Enables the RSPCK auto-stop function */
#define RSPIA_SPCR_MODFEN  (0x00004000) /* 0: Mode fault error detection disabled.    1: Detection enabled. */
#define RSPIA_SPCR_SPEIE   (0x00010000) /* 0: Error Interrupt disabled.    1: Error Interrupt enabled.*/
#define RSPIA_SPCR_SPRIE   (0x00020000) /* 0: Receive Interrupt disabled.     1: Receive Interrupt Enabled. */
#define RSPIA_SPCR_SPIIE   (0x00040000) /* 0: Disable Idle interrupt.    1: Enable Idle interrupt */
#define RSPIA_SPCR_RDDIS   (0x00080000) /* 0: Select Receive data full interrupt.    1: Select Error interrupt */
#define RSPIA_SPCR_SPTIE   (0x00100000) /* 0: Transmit Interrupt disabled    1: Transmit Interrupt Enabled */
#define RSPIA_SPCR_SPCIE   (0x00200000) /* 0: Communication End interrupt disabled    1: Communication End interrupt Enabled */
#define RSPIA_SPCR_SPMS    (0x01000000) /* 0: SPI (four-wire). Uses internal slave select. */
                                        /* 1: Clock synchronous operation (three-wire). Uses external slave select. */
#define RSPIA_SPCR_FRFS    (0x02000000) /* 0: Motorola SPI.     1: TI SSP */
#define RSPIA_SPCR_MSTR    (0x40000000) /* 0: Slave mode.       1: Master mode. */
#define RSPIA_SPCR_SYNDIS  (0x80000000) /* Set this bit to 1. */
#define RSPIA_SPCR_CMMD_TX_RX_MASK (0x30000000) /* 00b: Mask use enable Transmit-receive */
#define RSPIA_SPCR_CMMD_TX_MASK    (0x10000000) /* 01b: Mask use enable Transmit only */
#define RSPIA_SPCR_CMMD_RX_MASK    (0x20000000) /* 10b: Mask use enable Receive only */

/*RSPIA Slave Select Polarity Register (SSLP) */
#define RSPIA_SSLP_SSL0P  (0x01) /* 0 = active low. 1 = active high [Motorola SPI]. 0 = active high. 1 = active low [TI SSP]. */
#define RSPIA_SSLP_SSL1P  (0x02)
#define RSPIA_SSLP_SSL2P  (0x04)
#define RSPIA_SSLP_SSL3P  (0x08)
#define RSPIA_SSLP_MASK   (0x0F)

/* RSPIA Pin Control Register (SPPCR) */
#define RSPIA_SPPCR_SPLP  (0x01) /* 0: Normal mode. 1: Loopback mode (reversed transmit data = receive). */
#define RSPIA_SPPCR_SPLP2 (0x02) /* 0: Normal mode. 1: Loopback mode (transmit data = receive data). */
#define RSPIA_SPPCR_MOIFV (0x10) /* 0: MOSI pin idles low. 1: MOSI pin idles high. */
#define RSPIA_SPPCR_MOIFE (0x20) /* 0: MOSI pin idles at final previous data. 1: MOSI pin idles at MOIFV. */
#define RSPIA_SPPCR_MASK  (0x33) /* Protect reserved bits */

/* RSPIA Status Register  (SPSR) only Read */
#define RSPIA_SPSR_RRDYF (0x0080)   /* 0: Receive data ready is not detected.     1: Receive data ready is detected. */
#define RSPIA_SPSR_OVRF  (0x0100)   /* 0: No overrun error.                       1: An overrun error occurred. */
#define RSPIA_SPSR_IDLNF (0x0200)   /* 0: RSPIA is in the idle state.             1: RSPIA is in the transfer state. */
#define RSPIA_SPSR_MODF  (0x0400)   /* 0: No mode fault error                     1: A mode fault error occurred or
                                                                                   An under run error occurs */
#define RSPIA_SPSR_PERF  (0x0800)   /* 0: No parity error.                        1: A parity error occurred. */
#define RSPIA_SPSR_UDRF  (0x1000)   /* 0: No under run error                      1: An under run error occurs */
#define RSPIA_SPSR_SPTEF (0x2000)   /* 0: The number of empty stages in the transmit FIFO ≤ the value set by the SPFCR.TTRG[1:0] bits */
                                    /* 1: The number of empty stages in the transmit FIFO > the value set by the SPFCR.TTRG[1:0] bits */
#define RSPIA_SPSR_SPCF  (0x4000)   /* 0: RSPIA is not communicating or communicating. */
                                    /* 1: RSPIA communication is completed. */
#define RSPIA_SPSR_SPRF  (0x8000)   /* 0: The number of data stored in the receive FIFO ≤ number of frames set by the SPFCR.RTRG[1:0] bits. */
                                    /* 1: The number of data stored in the receive FIFO > number of frames set by the SPFCR.RTRG[1:0] bits. */

/* RSPIA Status Clear Register (SPSCLR) only Write */
#define RSPIA_SPSCLR_RRDYFC (0x0080) /* 1: The RSPIA receive data ready flag can be cleared. */
#define RSPIA_SPSCLR_OVRFC  (0x0100) /* 1: The overrun error flag can be cleared. */
#define RSPIA_SPSCLR_MODFC  (0x0400) /* 1: The mode fault error flag can be cleared. */
#define RSPIA_SPSCLR_PERFC  (0x0800) /* 1: The parity error flag can be cleared. */
#define RSPIA_SPSCLR_UDRFC  (0x1000) /* 1: The underrun error flag can be cleared. */
#define RSPIA_SPSCLR_SPTEFC (0x2000) /* 1: The transmit buffer empty flag can be cleared. */
#define RSPIA_SPSCLR_SPCFC  (0x4000) /* 1: The communication end flag can be cleared. */
#define RSPIA_SPSCLR_SPRFC  (0x8000) /* 1: The receive buffer full flag can be cleared. */
#define RSPIA_SPSCLR_MASK   (0x1D80) /* Mask clear error flag */
#define RSPIA_SPSCLR_MODF_UDRF_MASK   (0x1400) /* Mask clear mode fault and underrun error flag */

/* RSPIA Data Control Register (SPDCR) masks*/
#define RSPIA_SPDCR_BYSW   (0x0001) /* 0: Byte swapping of SPDR data disabled */
#define RSPIA_SPDCR_SPRDTD (0x0008) /* 0: read SPDR values from RX buffer. 1: values read from transmit buffer*/
#define RSPIA_SPDCR_DINV   (0x0010) /* 0: Not invert serial data. 1: Invert serial data. */
#define RSPIA_SPDCR_SPFC   (0x0300) /* 0: Transfer n+1 frames at a time */
#define RSPIA_SPDCR_MASK   (0x0319)

/* RSPIA Clock Delay Register (SPCKD) */
#define RSPIA_SPCKD_MASK (0x07) /* n+1 RSPCK */

/* RSPIA Slave Select Negation Delay Register (SSLND) */
#define RSPIA_SSLND_MASK (0x07) /* n+1 RSPCK */

/* RSPIA Next-Access Delay Register (SPND) */
#define RSPIA_SPND_MASK  (0x07) /* n+1 RSPCK + 2 PCLK */

/* RSPIA Sequence Control Register (SPSCR) */
#define RSPIA_SPSCR_MASK  (0x07) /* n+1 Sequence Length */

/* RSPIA FIFO Clear Register (SPFCLR) */
#define RSPIA_SPFCLR_MASK (0x01)

/* Bit masks for RSPIA Command Registers 0 to 7 (SPCMD0 to SPCMD7). */
#define RSPIA_SPCMD_CPHA   (0x00000001) /* 0: Data sampling on odd edge, data variation on even edge. */
                                        /* 1: Data variation on odd edge, data sampling on even edge. */
#define RSPIA_SPCMD_CPOL   (0x00000002) /* 0: RSPCK is low when idle. 1: RSPCK is high when idle. */
#define RSPIA_SPCMD_BRDV   (0x0000000C) /* Base bit-rate divisor. Div 2^N.  2-bits  */
#define RSPIA_SPCMD_SSLKP  (0x00000080) /* 0: SSL signal negated at end of transfer. 1: Keep SSL level at end until next. */
#define RSPIA_SPCMD_LSBF   (0x00001000) /* 0: MSB first. 1: LSB first. */
#define RSPIA_SPCMD_SPNDEN (0x00002000) /* 0: A next-access delay of 1 RSPCK + 2 PCLK. */
                                        /* 1: delay = the setting of the RSPIA next access delay register (SPND)*/
#define RSPIA_SPCMD_SLNDEN (0x00004000) /* 0: An SSL negation delay of 1 RSPCK. */
                                        /* 1: Delay = setting of RSPIA slave select negation delay register (SSLND)*/
#define RSPIA_SPCMD_SCKDEN (0x00008000) /* 0: An RSPCK delay of 1 RSPCK. */
                                        /* 1: Delay is equal to the setting of the RSPIA clock delay register (SPCKD). */
#define RSPIA_SPCMD_SPB    (0x001F0000) /* b16 to b20 SPB[4:0] bit mask */
#define RSPIA_SPCMD_SSLA   (0x07000000) /* n = Slave select 0-3 when SPI mode. */

#ifndef NULL
 #define NULL   (_NULL)
#endif /* NULL */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef struct rspia_cfg_block_s
{
    uint8_t chan;                           /* Number of the currently channel. */
    uint8_t current_slave;                  /* Number of the currently assigned slave. */
    bool rspia_chan_opened;                 /* This variable determines whether the peripheral has already been initialized. */
    void   (*p_callback)(void *p_cbdat);    /* pointer to user callback function. */
} rspia_cfg_block_t;

#endif /* RSPIA_PRIVATE_H */
