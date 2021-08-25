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
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
* THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
************************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : rm_touch_qe_config.h
 * Description  : Configures the QE Touch API module.
 ***********************************************************************************************************************/
#ifndef TOUCH_CONFIG_HEADER_FILE
#define TOUCH_CONFIG_HEADER_FILE


/***********************************************************************************************************************
 Configuration Options
 ***********************************************************************************************************************/
#define TOUCH_CFG_PARAM_CHECKING_ENABLE   (BSP_CFG_PARAM_CHECKING_ENABLE)

#define TOUCH_CFG_UART_MONITOR_SUPPORT    (0)

#define TOUCH_CFG_UART_TUNING_SUPPORT     (1)

#define TOUCH_CFG_UART_NUMBER             (5)

#define TOUCH_CFG_UART_BAUDRATE           (115200)

#define TOUCH_CFG_UART_PRIORITY           (15)

#ifndef QE_TOUCH_CONFIGURATION
#define TOUCH_CFG_NUM_BUTTONS             (1)
#define TOUCH_CFG_NUM_SLIDERS             (0)
#define TOUCH_CFG_NUM_WHEELS              (0)
#if (BSP_FEATURE_CTSU_VERSION == 2)
#define TOUCH_CFG_PAD_ENABLE             (0)
#endif
#endif
#endif /* TOUCH_CONFIG_HEADER_FILE */
