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
/***********************************************************************************************************************
* File Name     : r_scif_rx_config.h
* Description   : Configures the SCI FIFO driver
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           31.08.2014 1.00    Initial Release
*           11.11.2019 2.00    Added support for GNUC and ICCRX.
*                              Fixed coding style.
***********************************************************************************************************************/
#ifndef SCIF_CONFIG_H
#define SCIF_CONFIG_H

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/

/* SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING */
/* Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting
   Setting to 1 includes parameter checking; 0 compiles out parameter checking */
#define SCIF_CFG_PARAM_CHECKING_ENABLE   BSP_CFG_PARAM_CHECKING_ENABLE

/* SPECIFY WHETHER TO INCLUDE CODE FOR DIFFERENT SCI MODES */
/* Setting an equate to 1 includes code specific to that mode. */
#define SCIF_CFG_ASYNC_INCLUDED  (1)
#define SCIF_CFG_SYNC_INCLUDED   (0)

/* SPECIFY BYTE VALUE TO TRANSMIT WHILE CLOCKING IN DATA IN SYNC MODE */
#define SCIF_CFG_DUMMY_TX_BYTE   (0xFF)

/* SPECIFY CHANNELS TO INCLUDE SOFTWARE SUPPORT FOR 1=included, 0=not */
#define SCIF_CFG_CH8_INCLUDED   (0)
#define SCIF_CFG_CH9_INCLUDED   (1)
#define SCIF_CFG_CH10_INCLUDED  (0)
#define SCIF_CFG_CH11_INCLUDED  (0)

/* SPECIFY TX FIFO THRESHOLD */
/* The TX FIFO threshold indicates the number of bytes remaining in the FIFO before
 * more data should be loaded. The lower the threshold the fewer TXIF interrupts are
 * required to send multiple FIFOs of data. */
#define SCIF_CFG_CH8_TX_FIFO_THRESHOLD  (8)
#define SCIF_CFG_CH9_TX_FIFO_THRESHOLD  (8)
#define SCIF_CFG_CH10_TX_FIFO_THRESHOLD (8)
#define SCIF_CFG_CH11_TX_FIFO_THRESHOLD (8)

/* SPECIFY RX FIFO THRESHOLD (should match TX FIFO for Sync) */
/* The RX FIFO threshold indicates the number of bytes available in the FIFO before
 * more data should be read. The higher the threshold the fewer RXIF interrupts are
 * required to receive multiple FIFOs of data. */
#define SCIF_CFG_CH8_RX_FIFO_THRESHOLD  (8)
#define SCIF_CFG_CH9_RX_FIFO_THRESHOLD  (8)
#define SCIF_CFG_CH10_RX_FIFO_THRESHOLD (8)
#define SCIF_CFG_CH11_RX_FIFO_THRESHOLD (8)


#endif /* SCIF_CONFIG_H */
