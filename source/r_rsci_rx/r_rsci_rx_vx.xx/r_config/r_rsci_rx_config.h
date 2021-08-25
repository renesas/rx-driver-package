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
* File Name     : r_rsci_rx_config.h
* Description   : Configures the RSCI driver
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           31.03.2021 1.00    Initial Release
*                              Supported for RX671.
***********************************************************************************************************************/
#ifndef RSCI_CONFIG_H
#define RSCI_CONFIG_H

#include "platform.h"

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/

/* SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING */
/* Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting */
/* Setting to 1 includes parameter checking; 0 compiles out parameter checking */
#define RSCI_CFG_PARAM_CHECKING_ENABLE   (BSP_CFG_PARAM_CHECKING_ENABLE)

/* SPECIFY WHETHER TO INCLUDE CODE FOR DIFFERENT RSCI MODES */
/* Setting an equate to 1 includes code specific to that mode. */
#define RSCI_CFG_ASYNC_INCLUDED  (1)
#define RSCI_CFG_SYNC_INCLUDED   (0)
#define RSCI_CFG_SSPI_INCLUDED   (0)

/* SPECIFY BYTE VALUE TO TRANSMIT WHILE CLOCKING IN DATA IN SSPI MODES */
#define RSCI_CFG_DUMMY_TX_BYTE   (0xFF)

/* SPECIFY CHANNELS TO INCLUDE SOFTWARE SUPPORT FOR 1=included, 0=not */
/*
 * NOTE: If using ASYNC mode, adjust BYTEQ_CFG_MAX_CTRL_BLKS in r_byteq_config.h
 * to provide 2 queues per channel (static mode only).
 * RX MCU supported channels
 *
 * CH#   671
 * ---   ---
 * CH10   X
 * CH11   X

*/

#define RSCI_CFG_CH10_INCLUDED   (0)
#define RSCI_CFG_CH11_INCLUDED   (0)

/* SPECIFY ASYNC MODE TX QUEUE BUFFER SIZES (will not allocate if chan not enabled */
#define RSCI_CFG_CH10_TX_BUFSIZ  (80)
#define RSCI_CFG_CH11_TX_BUFSIZ  (80)

/* SPECIFY ASYNC MODE RX QUEUE BUFFER SIZES (will not allocate if chan not enabled */
#define RSCI_CFG_CH10_RX_BUFSIZ  (80)
#define RSCI_CFG_CH11_RX_BUFSIZ  (80)

/* 
* ENABLE TRANSMIT END INTERRUPT (ASYNCHRONOUS)
* This interrupt only occurs when the last bit of the last byte of data 
* has been sent and the transmitter has become idle. The interrupt calls
* the user's callback function specified in R_RSCI_Open() and passes it an
* RSCI_EVT_TEI event. A typical use of this feature is to disable an external
* transceiver to save power. It would then be up to the user's code to 
* re-enable the transceiver before sending again. Not including this feature
* reduces code space used by the interrupt. Note that this equate is only
* for including the TEI code. The interrupt itself must be enabled using an
* R_RSCI_Control(hdl, RSCI_CMD_EN_TEI, NULL) call.
*/
#define RSCI_CFG_TEI_INCLUDED    (0)      /* 1=included, 0=not */

/* 
* SET GROUPAL0 (ERI,TEI) INTERRUPT PRIORITY; RX671
* This sets the priority level for receiver overrun, framing, and parity errors
* as well as TEI interrupts for all RSCI channels.
*/
#define RSCI_CFG_ERI_TEI_PRIORITY (3)     /* (RX671 ONLY) 1 lowest, 15 highest */

/* ENABLE TX/RX FIFO; (RSCIi supported MCU ONLY) 1=included, 0=not */
#define RSCI_CFG_CH10_FIFO_INCLUDED  (0)
#define RSCI_CFG_CH11_FIFO_INCLUDED  (0)

/* SET TX FIFO THRESHOLD; (RSCIi supported MCU ONLY) 0 lowest, 31 highest */
/* TX FIFO THRESHOLD is invalid in Clock Synchronous Mode and Simple SPI Mode. */
/* Set the same value for TX FIFO THRESHOLD and RX FIFO THRESHOLD in Clock Synchronous Mode and Simple SPI Mode. */
#define RSCI_CFG_CH10_TX_FIFO_THRESH (16)
#define RSCI_CFG_CH11_TX_FIFO_THRESH (16)

/* SET RX FIFO THRESHOLD; (RSCIi supported MCU ONLY) 1 lowest, 31 highest */
#define RSCI_CFG_CH10_RX_FIFO_THRESH (16)
#define RSCI_CFG_CH11_RX_FIFO_THRESH (16)

/* ENABLE Received Data match function (RSCIj and RSCIi supported MCU RX671 ONLY) 1=included, 0=not */
#define RSCI_CFG_CH10_DATA_MATCH_INCLUDED (0)
#define RSCI_CFG_CH11_DATA_MATCH_INCLUDED (0)

/* Set enable/ disable transmit signal transition timing adjust feature for each channel*/
#define RSCI_CFG_CH10_TX_SIGNAL_TRANSITION_TIMING_INCLUDED  (0)
#define RSCI_CFG_CH11_TX_SIGNAL_TRANSITION_TIMING_INCLUDED  (0)

/* Set enable/ disable receive data sampling timing adjust feature for each channel*/
#define RSCI_CFG_CH10_RX_DATA_SAMPLING_TIMING_INCLUDED  (0)
#define RSCI_CFG_CH11_RX_DATA_SAMPLING_TIMING_INCLUDED  (0)

#endif /* RSCI_CONFIG_H */
