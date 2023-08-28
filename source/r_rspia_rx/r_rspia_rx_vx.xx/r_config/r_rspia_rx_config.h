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
* File Name     : r_rspia_rx_config.c
* Description   : Configures the RSPIA drivers
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 31.03.2021 1.00     First Release
*                               Supported for RX671.
*         : 30.06.2023 1.40     Supported for RX26T-256K.
***********************************************************************************************************************/
#ifndef RSPIA_CONFIG_HEADER_FILE
#define RSPIA_CONFIG_HEADER_FILE

#if ((BSP_MCU_RX26T == 1) && ((BSP_CFG_MCU_PART_MEMORY_SIZE == 0x8) || (BSP_CFG_MCU_PART_MEMORY_SIZE == 0xA)))
#error  "Error - RX26T Product with 48 Kbytes RAM not support RSPIA Module!"
#endif

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/
/* Checking of arguments passed to RSPIA API functions can be enable or disabled.
 * Disabling argument checking is provided for systems that absolutely require faster and smaller code.
 * By default the module is configured to use the setting of the system-wide BSP_CFG_PARAM_CHECKING_ENABLE macro.
 * This can be overridden for the local module by redefining RSPIA_CFG_PARAM_CHECKING_ENABLE.
 * To control parameter checking locally, set RSPIA_CFG_PARAM_CHECKING_ENABLE to 1 to enable it,
 * otherwise set to 0 skip checking.
 */
#define RSPIA_CFG_PARAM_CHECKING_ENABLE  (BSP_CFG_PARAM_CHECKING_ENABLE)

#define RSPIA_CFG_REQUIRE_LOCK (1)

/* Fixed data that is transmitted during receive-only operations. Change as needed. */
#define RSPIA_CFG_DUMMY_TX_DATA (0xFFFFFFFF)

/* Enable the RSPIA channels to use in this build. (0) = not used. (1) = used. */
#define RSPIA_CFG_USE_CH0  (1)

/* Set interrupt priority levels for each channel present.
 * Priority is shared by all interrupt sources in a channel.
 * Values must be in the range 0 (interrupt disabled) to 15 (highest)*/
#define RSPIA_CFG_CH0_IR_PRIORITY (3)

/* SET TX FIFO THRESHOLD; (RSPIA supported MCU ONLY) 0 lowest, 3 highest */
/* Set the same value for TX FIFO THRESHOLD and RX FIFO THRESHOLD. */
#define RSPIA_CFG_CH0_TX_FIFO_THRESH (2)

/* SET RX FIFO THRESHOLD; (RSPIA supported MCU ONLY) 0 lowest, 3 highest */
#define RSPIA_CFG_CH0_RX_FIFO_THRESH (2)

/**********************************************************************************************************************/
#endif /* RSPIA_CONFIG_HEADER_FILE */
