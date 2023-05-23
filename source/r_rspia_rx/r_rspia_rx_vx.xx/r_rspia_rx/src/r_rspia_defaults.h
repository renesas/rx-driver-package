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
* File Name    : r_rspia_defaults.h
* Description  : definition of default settings used for the RSPIA driver
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 31.03.2021 1.00     First Release
*                               Supported for RX671.
*         : 15.08.2022 1.30     Added RX26T support.
*                               Added macro for MRCKS bit default value: RSPIA_SPCR_MRCKS_DEF.
*                               Fixed to comply GSCE coding standard revision 6.5.0.
***********************************************************************************************************************/

#ifndef R_RSPIA_DEFS_H
#define R_RSPIA_DEFS_H

/***********************************************************************************************************************
* Default RSPIA register settings section. One set of defaults provided for all channels.
* Make sure to understand the operation of these registers if making changes.
***********************************************************************************************************************/
/* RSPIA Control Register (SPCR) */
#define RSPIA_SPCR_SPE_DEF     (0x00000000) /* 0: RSPIA operation disabled. */
#ifdef BSP_MCU_RX26T
#define RSPIA_SPCR_MRCKS_DEF   (0x00000080) /* 0: Master Receive Clock Select. */
#endif
#define RSPIA_SPCR_SPPE_DEF    (0x00000000) /* 0: No parity */
#define RSPIA_SPCR_SPOE_DEF    (0x00000000) /* 0: Even parity (ignored) */
#define RSPIA_SPCR_PTE_DEF     (0x00000000) /* 0: Disable parity self diagnostic. */
#define RSPIA_SPCR_SCKASE_DEF  (0x00000000) /* 0: Disables the RSPCK auto-stop function */
#define RSPIA_SPCR_SCKDDIS_DEF (0x00000000) /* 1: Delay between frames is not inserted in burst transfer.*/
#define RSPIA_SPCR_MODFEN_DEF  (0x00000000) /* 0: Mode fault error detection disabled. */
#define RSPIA_SPCR_SPEIE_DEF   (0x00000000) /* 0: Error Interrupt disabled. */
#define RSPIA_SPCR_SPRIE_DEF   (0x00000000) /* 0: Receive Interrupt disabled. */
#define RSPIA_SPCR_SPIIE_DEF   (0x00000000) /* 0: Disable Idle interrupt */
#define RSPIA_SPCR_RDRIS_DEF   (0x00000000) /* 0: Receive data full interrupt */
#define RSPIA_SPCR_SPTIE_DEF   (0x00000000) /* 0: Transmit Interrupt disabled */
#define RSPIA_SPCR_SPCIE_DEF   (0x00000000) /* 0: Communication End interrupt disable */
#define RSPIA_SPCR_SPMS_DEF    (0x00000000) /* 0: SPI mode operation (4-wire). Uses RSPIA generated slave select. */
#define RSPIA_SPCR_FRFS_DEF    (0x00000000) /* 0: Frame Format Motorola SPI. */
#define RSPIA_SPCR_CMMD_DEF    (0x00000000) /* CMMD[1:0] 00b = Transmit-receive Communications Mode. */
#define RSPIA_SPCR_MSTR_DEF    (0x40000000) /* 1: Master mode. */
#define RSPIA_SPCR_SYNDIS_DEF  (0x80000000) /* 1: Synchronizer disable set bit to 1. */
#ifdef BSP_MCU_RX26T
#define RSPIA_SPCR_DEF     (RSPIA_SPCR_SPE_DEF     | RSPIA_SPCR_MRCKS_DEF  | RSPIA_SPCR_SPPE_DEF   | \
                            RSPIA_SPCR_SPOE_DEF    | RSPIA_SPCR_PTE_DEF    | RSPIA_SPCR_SCKASE_DEF | \
                            RSPIA_SPCR_SCKDDIS_DEF | RSPIA_SPCR_MODFEN_DEF | RSPIA_SPCR_SPEIE_DEF  | \
                            RSPIA_SPCR_SPRIE_DEF   | RSPIA_SPCR_SPIIE_DEF  | RSPIA_SPCR_RDRIS_DEF  | \
                            RSPIA_SPCR_SPTIE_DEF   | RSPIA_SPCR_SPCIE_DEF  | RSPIA_SPCR_SPMS_DEF   | \
                            RSPIA_SPCR_FRFS_DEF    | RSPIA_SPCR_CMMD_DEF   | RSPIA_SPCR_MSTR_DEF   | \
                            RSPIA_SPCR_SYNDIS_DEF)
#else
#define RSPIA_SPCR_DEF     (RSPIA_SPCR_SPE_DEF    | RSPIA_SPCR_SPPE_DEF   | RSPIA_SPCR_SPOE_DEF    | \
                            RSPIA_SPCR_PTE_DEF    | RSPIA_SPCR_SCKASE_DEF | RSPIA_SPCR_SCKDDIS_DEF | \
                            RSPIA_SPCR_MODFEN_DEF | RSPIA_SPCR_SPEIE_DEF  | RSPIA_SPCR_SPRIE_DEF   | \
                            RSPIA_SPCR_SPIIE_DEF  | RSPIA_SPCR_RDRIS_DEF  | RSPIA_SPCR_SPTIE_DEF   | \
                            RSPIA_SPCR_SPCIE_DEF  | RSPIA_SPCR_SPMS_DEF   | RSPIA_SPCR_FRFS_DEF    | \
                            RSPIA_SPCR_CMMD_DEF   | RSPIA_SPCR_MSTR_DEF   | RSPIA_SPCR_SYNDIS_DEF)
#endif

/* RSPIA Receive-Only Mode Control Register (SPRMCR) */
/* Only effective In Master Receive only mode */
#define RSPIA_SPRMCR_RFC_DEF   (0x00)  /* b0 to b4 RFC[4:0] 00001b to 11111b = 1 to 31 frames,
                                        * The number of received frames. */
#define RSPIA_SPRMCR_TERM_DEF  (0x00)  /* 1: Receive End. Reading value is always 0. */
#define RSPIA_SPRMCR_START_DEF (0x00)  /* 1: Receive Start. Reading value is always 0. */
#define RSPIA_SPRMCR_DEF       (RSPIA_SPRMCR_RFC_DEF | RSPIA_SPRMCR_TERM_DEF | RSPIA_SPRMCR_START_DEF)

/*RSPIA Slave Select Polarity Register (SSLP) */
#define RSPIA_SSLP_SSL0P_DEF  (0x00) /* 0 = active low. 1 = active high [Motorola SPI];
                                      * 0 = active high. 1 = active low [TI SSP]. */
#define RSPIA_SSLP_SSL1P_DEF  (0x00)
#define RSPIA_SSLP_SSL2P_DEF  (0x00)
#define RSPIA_SSLP_SSL3P_DEF  (0x00)

#define RSPIA_SSLP_DEF     (RSPIA_SSLP_SSL0P_DEF | RSPIA_SSLP_SSL1P_DEF | \
                            RSPIA_SSLP_SSL2P_DEF | RSPIA_SSLP_SSL3P_DEF)

/* RSPIA Pin Control Register (SPPCR) */
#define RSPIA_SPPCR_SPLP_DEF  (0x00) /* 0: Normal mode. 1: Loop-back mode (reversed transmit data = receive data) */
#define RSPIA_SPPCR_SPLP2_DEF (0x00) /* 0: Normal mode. 1: Loop-back mode (transmit data = receive data) */
#define RSPIA_SPPCR_MOIFV_DEF (0x00) /* 0: MOSI pin idles low. 1: MOSI pin idles high. */
#define RSPIA_SPPCR_MOIFE_DEF (0x00) /* 0: MOSI pin idles at final previous data. 1: MOSI pin idles at MOIFV. */

#define RSPIA_SPPCR_DEF    (RSPIA_SPPCR_SPLP_DEF | RSPIA_SPPCR_SPLP2_DEF | \
                            RSPIA_SPPCR_MOIFV_DEF | RSPIA_SPPCR_MOIFE_DEF)

/* RSPIA Sequence Control Register (SPSCR) */
#define RSPIA_SPSCR_DEF  (0x00) /* b0 to b2 SPSLN[2:0] 000b to 111b = SPCMD0 to SPCMD7.
                                 * However, the RSPIA in slave mode reference SPCMD0. */

/* RSPIA Data Control Register (SPDCR) */
#define RSPIA_SPDCR_BYSW_DEF   (0x0000)  /* 0: Byte swapping of SPDR data disabled */
#define RSPIA_SPDCR_SPRDTD_DEF (0x0000)  /* 0: read SPDR values from the RX buffer. */
#define RSPIA_SPDCR_SPLW_DEF   (0x0000)  /* 0: Not invert serial data */
#define RSPIA_SPDCR_SPFC_DEF   (0x0000)  /* b8 to b9 SPFC[1:0] 00b to 11 = 1 to 4 frames */

#define RSPIA_SPDCR_DEF    (RSPIA_SPDCR_BYSW_DEF | RSPIA_SPDCR_SPRDTD_DEF | \
                            RSPIA_SPDCR_SPLW_DEF | RSPIA_SPDCR_SPFC_DEF)

/* RSPIA Clock Delay Register (SPCKD) */
#define RSPIA_SPCKD_DEF   (0x00)

/* RSPIA Slave Select Negation Delay Register (SSLND) */
#define RSPIA_SSLND_DEF   (0x00)

/* RSPIA Next-Access Delay Register (SPND) */
#define RSPIA_SPND_DEF    (0x00)

#endif /* R_RSPIA_DEFS_H_ */
