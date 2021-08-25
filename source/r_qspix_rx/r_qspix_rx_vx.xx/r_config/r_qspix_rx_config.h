/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 31.03.2021 1.00     First Release
*******************************************************************************/
/*******************************************************************************
* File Name     : r_qspix_rx_config.h
* Description   : Configures the QSPIX drivers
*******************************************************************************/
#ifndef R_QSPIX_RX_CONFIG_H
#define R_QSPIX_RX_CONFIG_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/

/*******************************************************************************
Configuration Options
*******************************************************************************/
/* This setting selects whether to include parameter checking of arguments
   of API for QSPIX FIT module or not.
    When set to "1", API includes parameter checking.
    When set to "0", API doesn't include it.
   And, when the definition of "BSP_CFG_PARAM_CHECKING_ENABLE" is "0", API doesn't include it too.
*/
#define QSPIX_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)

/* Enable the QSPIX channels to use in this build. (0) = not used. (1) = used. */
#define QSPIX_CFG_USE_CH0  (1)

/*
* Enable or Disable ROM access error interrupt
*/
#define QSPIX_CFG_ERI_INCLUDED    (1)      /* 1=included, 0=not */

/* Set interrupt priority levels for each channel present.
 * Priority is shared by all interrupt sources in a channel.
 * Values must be in the range 0 (interrupt disabled) to 15 (highest)*/
#define QSPIX_CFG_ERI_IR_PRIORITY (3)

#endif /* R_QSPIX_RX_CONFIG_H */
