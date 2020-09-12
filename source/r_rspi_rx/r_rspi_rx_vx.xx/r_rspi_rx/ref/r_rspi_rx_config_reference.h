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
* Copyright (C) 2013(2014-2018) Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name     : r_rspi_rx_config.c
* Description   : Configures the RSPI drivers
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 25.10.2013 1.00     First Release
*         : 31.07.2017 1.70     Supported RX65N-2MB and RX130N-512KB.
*         : 20.09.2018 1.80     Supported RX66T.
*         : 20.12.2018 2.00     Added double buffer and dmadtc transfer mode.
***********************************************************************************************************************/
#ifndef RSPI_CONFIG_HEADER_FILE
#define RSPI_CONFIG_HEADER_FILE

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/
/* Checking of arguments passed to RSPI API functions can be enable or disabled.
 * Disabling argument checking is provided for systems that absolutely require faster and smaller code.
 * By default the module is configured to use the setting of the system-wide BSP_CFG_PARAM_CHECKING_ENABLE macro.
 * This can be overridden for the local module by redefining RSPI_CFG_PARAM_CHECKING_ENABLE.
 * To control parameter checking locally, set RSPI_CFG_PARAM_CHECKING_ENABLE to 1 to enable it,
 * otherwise set to 0 skip checking.
 */
#define RSPI_CFG_PARAM_CHECKING_ENABLE  (BSP_CFG_PARAM_CHECKING_ENABLE)

/* If this is defined, then High Speed Read Operation is valid.
   RSPI can speed up the consecutive reception with the function of receive
   buffer. If the MCU does not have RSPCK Auto-Stop Function, this module
   disables interrupts during the High Speed Read Operation. */
#define RSPI_CFG_HIGH_SPEED_READ (1)

#define RSPI_CFG_REQUIRE_LOCK (1)

/* Fixed data that is transmitted during receive-only operations. Change as needed. */
#define RSPI_CFG_DUMMY_TXDATA (0xFFFFFFFF)

/* Enable the RSPI channels to use in this build. (0) = not used. (1) = used. */
#define RSPI_CFG_USE_CHAN0  (1)
#define RSPI_CFG_USE_CHAN1  (0)
#define RSPI_CFG_USE_CHAN2  (0)

/* Set interrupt priority levels for each channel present.
 * Priority is shared by all interrupt sources in a channel.
 * Values must be in the range 0 (interrupt disabled) to 15 (highest)*/
#define RSPI_CFG_IR_PRIORITY_CHAN0 (3)
#define RSPI_CFG_IR_PRIORITY_CHAN1 (3)
#define RSPI_CFG_IR_PRIORITY_CHAN2 (3)

/******************************************************************************
ENABLE DEBUGGING INFORMATION
******************************************************************************/
/* Stores error log information using LONGQ driver. */
#define RSPI_CFG_LONGQ_ENABLE  (0)


/* When  reading the RSPI received data register for data frame bit lengths
 * that are not 8, 16, or 32-bits the unused upper bit will contain residual
 * values from the transmit data. As a convenience, these unused upper bits
 * may be optionally masked off (cleared to 0) by the driver when the data
 * is transferred to the user-data buffer.
 * Since this takes additional processing time in the data transfer interrupt handler
 * it will cause a reduction in performance for the highest bit rate settings.
 * This is not needed for 8, 16, or 32-bit transfers. So only enable this option
 * when using data frame bit lengths that are not 8, 16, or 32-bits. */
#define RSPI_CFG_MASK_UNUSED_BITS (0)

/* For RX63 group MCUs the RSPI error interrupt is a group interrupt shared with the SCI peripheral
 * So this is disabled by default for RX63 group to prevent conflict with SCI module. If not using the SCI module
 * This may be enabled by setting RSPI_CFG_USE_RX63_ERROR_INTERRUPT to (1).
 */
#define RSPI_CFG_USE_RX63_ERROR_INTERRUPT (0)
/**********************************************************************************************************************/
#endif /* RSPI_CONFIG_HEADER_FILE */
