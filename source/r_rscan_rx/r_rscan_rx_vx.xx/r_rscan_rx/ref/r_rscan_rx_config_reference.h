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
 * Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_rscan_rx_config_reference.h
 * Description  : Configures the RSCAN driver module for the RX231/RX23W/RSKRX23E-A MCU
 ***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 22.03.2015 1.00    Initial Release
***********************************************************************************************************************/
#ifndef RSCAN_CONFIG_HEADER_FILE
#define RSCAN_CONFIG_HEADER_FILE


/***********************************************************************************************************************
 Configuration Options
 ***********************************************************************************************************************/
/*
 * SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING
 * Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting
 * Setting to 1 includes parameter checking; 0 compiles out parameter checking
 */
#define CAN_CFG_PARAM_CHECKING_ENABLE   (BSP_CFG_PARAM_CHECKING_ENABLE)

/*
 * Setting for CAN clock source.
 * 0 = PCLKB/2
 * 1 = CANMCLK = EXTAL pin              // RSKRX231,RSKRX23W
 */
#define CAN_CFG_CLOCK_SOURCE            (1)

/*
 * CAN Interrupt Priority
 */
#define CAN_CFG_INT_PRIORITY            (5)         /* 0-31 */

/*
 * Main Callback Interrupt Sources
 * 0 = disabled                                                                 GetStatusMask() Type
 * 1 = enabled                                      Callback Event              to identify interrupt source
 */
#define CAN_CFG_INT_RXFIFO_THRESHOLD        (1)     /* CAN_EVT_RXFIFO_THRESHOLD CAN_STAT_FIFO_THRESHOLD */
#define CAN_CFG_INT_DLC_ERR                 (0)     /* CAN_EVT_GLOBAL_ERR       CAN_STAT_GLOBAL_ERR */
#define CAN_CFG_INT_HIST_FIFO_OVFL          (1)     /* CAN_EVT_GLOBAL_ERR       CAN_STAT_GLOBAL_ERR, CAN_STAT_FIFO_OVFL */
/* for TX and RX FIFOs */
#define CAN_CFG_INT_FIFO_OVFL               (1)     /* CAN_EVT_GLOBAL_ERR       CAN_STAT_GLOBAL_ERR, CAN_STAT_FIFO_OVFL */

/*
 * Channel Callback Interrupt Sources
 * 0 = disabled                                                                 GetStatusMask() Type
 * 1 = enabled                                      Callback Event              to identify interrupt source
 */
#define CAN_CFG_INT_TXFIFO_THRESHOLD        (1)     /* CAN_EVT_TRANSMIT         CAN_STAT_FIFO_THRESHOLD */
#define CAN_CFG_INT_HIST_FIFO_THRESHOLD     (1)     /* CAN_EVT_TRANSMIT         CAN_STAT_FIFO_THRESHOLD */
#define CAN_CFG_INT_MBX_TX_COMPLETE         (0)     /* CAN_EVT_TRANSMIT         CAN_STAT_CH_TXMBX_SENT */
#define CAN_CFG_INT_MBX_TX_ABORTED          (0)     /* CAN_EVT_TRANSMIT         CAN_STAT_CH_TXMBX_ABORTED */
#define CAN_CFG_INT_BUS_ERROR               (0)     /* CAN_EVT_CHANNEL_ERR      CAN_STAT_CH_ERROR */
#define CAN_CFG_INT_ERR_WARNING             (0)     /* CAN_EVT_CHANNEL_ERR      CAN_STAT_CH_ERROR */
#define CAN_CFG_INT_ERR_PASSIVE             (1)     /* CAN_EVT_CHANNEL_ERR      CAN_STAT_CH_ERROR */
#define CAN_CFG_INT_BUS_OFF_ENTRY           (1)     /* CAN_EVT_CHANNEL_ERR      CAN_STAT_CH_ERROR */
#define CAN_CFG_INT_BUS_OFF_RECOVERY        (1)     /* CAN_EVT_CHANNEL_ERR      CAN_STAT_CH_ERROR */
#define CAN_CFG_INT_OVERLOAD_FRAME_TX       (0)     /* CAN_EVT_CHANNEL_ERR      CAN_STAT_CH_ERROR */
#define CAN_CFG_INT_BUS_LOCK                (0)     /* CAN_EVT_CHANNEL_ERR      CAN_STAT_CH_ERROR */
#define CAN_CFG_INT_ARB_LOST                (0)     /* CAN_EVT_CHANNEL_ERR      CAN_STAT_CH_ERROR */


#endif /* RSCAN_CONFIG_HEADER_FILE */
