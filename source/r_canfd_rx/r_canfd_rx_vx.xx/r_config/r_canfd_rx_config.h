/* Generated configuration header file - do not edit */
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
* File Name    : r_canfd_rx_config.h
* Description  : Configures RX CANFD FIT code.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 22.11.2021 1.00    Initial Release
***********************************************************************************************************************/
#ifndef CANFD_CONFIG_HEADER_FILE
#define CANFD_CONFIG_HEADER_FILE

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/

/* Selects whether to include parameter checking in the code.
   BSP_CFG_PARAM_CHECKING_ENABLE = Default (BSP).
   (0) = Disabled
   (1) = Enabled
 */
#define CANFD_CFG_PARAM_CHECKING_ENABLE   (BSP_CFG_PARAM_CHECKING_ENABLE)

/* Number of acceptance filter list rules dedicated to Channel 0.
   Any value (0~32)
   Default value is 32.
 */
#define CANFD_CFG_AFL_CH0_RULE_NUM   (32)

/* Select whether to enter the protocol exception handling state when a RES bit is sampled recessive as defined in ISO 11898-1.
   (0) = Enabled (ISO 11898-1) (default)
   (R_CANFD_GFDCFG_PXEDIS_Msk) = Disabled
 */
#define CANFD_CFG_FD_PROTOCOL_EXCEPTION   (0)


/* Select which errors should trigger an interrupt.
    0x3 (default)
    R_CANFD_GCR_DEIE_Msk | 0x3
    R_CANFD_GCR_MLIE_Msk | 0x3
    R_CANFD_GCR_POIE_Msk | 0x3
    R_CANFD_GCR_DEIE_Msk | R_CANFD_GCR_MLIE_Msk | 0x3
    R_CANFD_GCR_DEIE_Msk | R_CANFD_GCR_POIE_Msk | 0x3
    R_CANFD_GCR_MLIE_Msk | R_CANFD_GCR_POIE_Msk | 0x3
    R_CANFD_GCR_DEIE_Msk | R_CANFD_GCR_MLIE_Msk | R_CANFD_GCR_POIE_Msk | 0x3
 */
#define CANFD_CFG_GLOBAL_ERR_SOURCES    (0x3)

/* Select how messages should be prioritized for transmission. In either case, lower numbers indicate higher priority.
   (0) = Message ID
   (R_CANFD_GCFG_TPRI_Msk) = Buffer Number  (default)
 */
#define CANFD_CFG_TX_PRIORITY           ((R_CANFD_GCFG_TPRI_Msk))

/* When enabled received messages will be rejected if their DLC field is less than the value configured in the associated AFL rule. 
   If 'DLC Replacement Enable' is selected and a message passes the DLC check the DLC field is set to the value in the associated AFL rule 
   and any excess data is discarded.
  (0) = Disabled (default)
  (R_CANFD_GCFG_DCE_Msk) = Enabled
  (R_CANFD_GCFG_DCE_Msk | R_CANFD_GCFG_DRE_Msk) = DLC Replacement Enable 
 */
#define CANFD_CFG_DLC_CHECK             (0)

/* Configure whether received messages larger than the destination buffer should be truncated or rejected.
   (0) = Reject  (default)
   (R_CANFD_GCFG_TPRI_Msk) = Truncate  
 */
#define CANFD_CFG_FD_OVERFLOW           (0)

/* Configure the CANFD Clock source to be either PLL (default) or crystal direct.
   (0) = PLL  (default)
   (1) = Crystal direct  
 */
#define CANFD_CFG_CANFDCLK_SOURCE           (0)

/* Number of message buffers available for reception.\n\nAs there is no interrupt for message buffer reception it is recommended to use RX FIFOs instead. 
   Set this value to 0 to disable RX Message Buffers.
   Any value (0~32)
   Default value is 0.
 */
#define CANFD_CFG_RXMB_NUMBER           (0)

/* Payload size for all RX Message Buffers.
    (0) = 8 bytes  (default)
    (1) = 12 bytes
    (2) = 16 bytes
    (3) = 20 bytes
    (4) = 24 bytes
    (5) = 32 bytes
    (6) = 48 bytes
    (7) = 64 bytes
 */
#define CANFD_CFG_RXMB_SIZE             (0)

/* This interrupt is fired for each of the error sources selected below.
   Any value (0)~(15)
   Default value is (12).
 */
#define CANFD_CFG_GLOBAL_ERR_IPL        (12)

/* Selects whether to include parameter checking in the code.
   BSP_CFG_PARAM_CHECKING_ENABLE = Default (BSP).
   Any value (0)~(15)
   Default value is (12).
 */
#define CANFD_CFG_RX_FIFO_IPL           (12)


/* Set the interrupt threshold value for RX FIFO 0. This setting is only applicable when the Interrupt Mode is set to 'At Threshold Value'.
    (0U) = 1/8 full  
    (1U) = 1/4 full
    (2U) = 3/8 full
    (3U) = 1/2 full  (default)
    (4U) = 5/8 full
    (5U) = 3/4 full
    (6U) = 7/8 full
    (7U) = full
 */
#define CANFD_CFG_RXFIFO0_INT_THRESHOLD ((3U))

/* Select the number of stages for RX FIFO 0.
    (1) = 4 stages
    (2) = 8 stages
    (3) = 16 stages  (default)
    (4) = 32 stages
    (5) = 48 stages
 */
#define CANFD_CFG_RXFIFO0_DEPTH         (3)

/* Select the message payload size for RX FIFO 0.
    (0) = 8 bytes
    (1) = 12 bytes
    (2) = 16 bytes
    (3) = 20 bytes
    (4) = 24 bytes
    (5) = 32 bytes
    (6) = 48 bytes
    (7) = 64 bytes  (default)
 */
#define CANFD_CFG_RXFIFO0_PAYLOAD       (7)

/* Set the interrupt mode for RX FIFO 0. Threshold mode will only fire an interrupt each time an incoming message crosses the threshold value set below.
   (0) = Disabled
   (R_CANFD_RFCR_RFIE_Msk) = At Threshold Value
   (R_CANFD_RFCR_RFIE_Msk | R_CANFD_RFCR_RFIM_Msk) = Every Frame  (default)
 */
#define CANFD_CFG_RXFIFO0_INT_MODE      ((R_CANFD_RFCR_RFIE_Msk | R_CANFD_RFCR_RFIM_Msk))

/* Enable or disable RX FIFO 0.
   (0) = Disabled
   (1) = Enabled (default)
 */
#define CANFD_CFG_RXFIFO0_ENABLE        (1)


/* Set the interrupt threshold value for RX FIFO 1. This setting is only applicable when the Interrupt Mode is set to 'At Threshold Value'.
    (0U) = 1/8 full  
    (1U) = 1/4 full
    (2U) = 3/8 full
    (3U) = 1/2 full  (default)
    (4U) = 5/8 full
    (5U) = 3/4 full
    (6U) = 7/8 full
    (7U) = full
 */
#define CANFD_CFG_RXFIFO1_INT_THRESHOLD ((3U))

/* Select the number of stages for RX FIFO 1.
    (1) = 4 stages
    (2) = 8 stages
    (3) = 16 stages  (default)
    (4) = 32 stages
    (5) = 48 stages
 */
#define CANFD_CFG_RXFIFO1_DEPTH         (3)

/* Select the message payload size for RX FIFO 1.
    (0) = 8 bytes
    (1) = 12 bytes
    (2) = 16 bytes
    (3) = 20 bytes
    (4) = 24 bytes
    (5) = 32 bytes
    (6) = 48 bytes
    (7) = 64 bytes  (default)
 */
#define CANFD_CFG_RXFIFO1_PAYLOAD       (7)

/* Set the interrupt mode for RX FIFO 1. Threshold mode will only fire an interrupt each time an incoming message crosses the threshold value set below.
   (0) = Disabled
   (R_CANFD_RFCR_RFIE_Msk) = At Threshold Value
   (R_CANFD_RFCR_RFIE_Msk | R_CANFD_RFCR_RFIM_Msk) = Every Frame  (default)
 */
#define CANFD_CFG_RXFIFO1_INT_MODE      ((R_CANFD_RFCR_RFIE_Msk | R_CANFD_RFCR_RFIM_Msk))

/* Enable or disable RX FIFO 0.
   (0) = Disabled  (default)
   (1) = Enabled 
 */
#define CANFD_CFG_RXFIFO1_ENABLE        (0)

/* Select TX0 Message buffers should trigger an interrupt when transmission is complete.
   Disabled = 0ULL  (default)
   Enabled = (1ULL << 0)
 */
#define CANFD0_EXTENDED_CFG_TXMB0_TXI_ENABLE (0ULL)

/* Select TX1 Message buffers should trigger an interrupt when transmission is complete.
   Disabled = 0ULL  (default)
   Enabled = (1ULL << 1)
 */
#define CANFD0_EXTENDED_CFG_TXMB1_TXI_ENABLE (0ULL)

/* Select TX2 Message buffers should trigger an interrupt when transmission is complete.
   Disabled = 0ULL  (default)
   Enabled = (1ULL << 2)
 */
#define CANFD0_EXTENDED_CFG_TXMB2_TXI_ENABLE (0ULL)

/* Select TX3 Message buffers should trigger an interrupt when transmission is complete.
   Disabled = 0ULL  (default)
   Enabled = (1ULL << 3)
 */
#define CANFD0_EXTENDED_CFG_TXMB3_TXI_ENABLE (0ULL)

/* Select Error Warning interrupt sources to enable.
   Disabled = 0ULL  (default)
   Enabled = R_CANFD_CHCR_EWIE_Msk
 */
#define CANFD0_EXTENDED_CFG_WARNING_ERROR_INTERRUPTS (0U)

/* Select error passive interrupt sources to enable.
   Disabled = 0U  (default)
   Enabled = R_CANFD_CHCR_EPIE_Msk
 */
#define CANFD0_EXTENDED_CFG_PASSING_ERROR_INTERRUPTS (0U)

/* Select which channel bus-Off Entry error interrupt sources to enable.
   Disabled = 0U  (default)
   Enabled = R_CANFD_CHCR_BOEIE_Msk
 */
#define CANFD0_EXTENDED_CFG_BUS_OFF_ENTRY_ERROR_INTERRUPTS (0U)

/* Select channel bus-Off Recovery error interrupt sources to enable.
   Disabled = 0U  (default)
   Enabled = R_CANFD_CHCR_BORIE_Msk
 */
#define CANFD0_EXTENDED_CFG_BUS_OFF_RECOVERY_ERROR_INTERRUPTS (0U)

/* Select channel overload error interrupt sources to enable.
   Disabled = 0U  (default)
   Enabled = R_CANFD_CHCR_OLIE_Msk
 */
#define CANFD0_EXTENDED_CFG_OVERLOAD_ERROR_INTERRUPTS (0U)

/* This interrupt is fired for each of the error sources selected below.
   Any value (0)~(15)
   Default value is (12).
 */
#define CANFD0_CFG_IPL (12)

/***********************************************************************************************************************
Configuration Manual Bitrate (default settings: Nominal Bit Rate = 1Mbit; Data Bit Rate = 5Mbit; DLL = 40MHz)
***********************************************************************************************************************/
/* Specify clock divisor for nominal bitrate.
   Any value (1~1024)
   Default value is (1).
 */
#define CANFD0_BIT_TIMING_CFG_BRP (1)

/* Select the Time Segment 1 value. Check module usage notes for how to calculate this value.
   Any value (2~256)
   Default value is (29).
 */
#define CANFD0_BIT_TIMING_CFG_TSEG1 (29)

/* Select the Time Segment 2 value. Check module usage notes for how to calculate this value.
   Any value (2~128)
   Default value is (10).
 */
#define CANFD0_BIT_TIMING_CFG_TSEG2 (10)

/* Select the Synchronization Jump Width value. Check module usage notes for how to calculate this value.
   Any value (1~128)
   Default value is (4).
 */
#define CANFD0_BIT_TIMING_CFG_SJW (4)

/* Specify clock divisor for data bitrate.
   Any value (1~1024)
   Default value is (1).
 */
#define CANFD0_DATA_TIMING_CFG_BRP (1)

/* Select the Time Segment 1 value. Check module usage notes for how to calculate this value.
   Any value (2~32)
   Default value is (5).
 */
#define CANFD0_DATA_TIMING_CFG_TSEG1 (5)

/* Select the Time Segment 2 value. Check module usage notes for how to calculate this value.
   Any value (2~16)
   Default value is (2).
 */
#define CANFD0_DATA_TIMING_CFG_TSEG2 (2)

/* Select the Synchronization Jump Width value. Check module usage notes for how to calculate this value.
   Any value (1~16)
   Default value is (1).
 */
#define CANFD0_DATA_TIMING_CFG_SJW (1)

/* When enabled the CANFD module will automatically compensate for any transceiver
   or bus delay between transmitted and received bits.
   When manually supplying bit timing values with delay compensation enabled be
   sure the data prescaler is 2 or smaller for correct operation.
   (0) = Disabled
   (1) = Enabled (default)
   Default value is (1).
 */
#define CANFD0_EXTENDED_CFG_DELAY_COMPENSATION (1)

#endif /* CANFD_CONFIG_HEADER_FILE */
