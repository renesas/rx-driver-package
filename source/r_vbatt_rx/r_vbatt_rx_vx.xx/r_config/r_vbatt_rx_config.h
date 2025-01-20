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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 30.06.2015 1.00     First Release
*              : 31.03.2021 2.00     Added support for RX671
*              : 28.06.2024 2.30     Added support Nested Interrupt.
*******************************************************************************/
/*******************************************************************************
* File Name     : r_vbatt_rx_config.h
* Description   : Configures the Battery Backup Function drivers
*******************************************************************************/
#ifndef R_VBATT_RX_CONFIG_H
#define R_VBATT_RX_CONFIG_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* Defines for Battery Backup Function support */
#include "r_vbatt_rx_if.h"


/*******************************************************************************
Configuration Options
*******************************************************************************/
/* This setting selects whether to include parameter checking of arguments 
   of API for Battery backup function FIT module or not.
    When set to "1", API includes parameter checking.
    When set to "0", API doesn't include it.
   And, when the definition of "BSP_CFG_PARAM_CHECKING_ENABLE" is "0", API doesn't include it too.
*/
#define VBATT_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)


/* Setting to 
    VBATT pin voltage drop detection (VBATTCR.VBTLVDEN).
    VBATT pin voltage drop detection interrupt (VBTLVDICR.VBTLVDIE).
    VBATT pin voltage drop detection interrupt type (VBTLVDICR.VBTLVDISEL).
   Please choose from macro definition of the following.
    VBATT_DTCT_DISABLE            = VBATT pin voltage drop detection disabled
                                    VBATT pin voltage drop detection interrupt disabled
    VBATT_DTCT_ENABLE_INT_DISABLE = VBATT pin voltage drop detection enabled
                                    VBATT pin voltage drop detection interrupt disabled
    VBATT_DTCT_ENABLE_NMI_ENABLE  = VBATT pin voltage drop detection enabled
                                    VBATT pin voltage drop detection interrupt enabled
                                    VBATT pin voltage drop detection interrupt enable type is non-maskable interrupt
    VBATT_DTCT_ENABLE_INT_ENABLE  = VBATT pin voltage drop detection enabled
                                    VBATT pin voltage drop detection interrupt enabled
                                    VBATT pin voltage drop detection interrupt enable type is maskable interrupt
*/
#define VBATT_CFG_DETECT_FUNCTION       (VBATT_DTCT_DISABLE)


/* Setting to 
    VBATT pin voltage drop detection level (VBATTCR.VBTLVDLVL).
   Please choose from macro definition of the following.
    VBATT_DTCT_LEVEL_2_20_V  = 2.20V
    VBATT_DTCT_LEVEL_2_00_V  = 2.00V
*/
#define VBATT_CFG_DETECT_LEVEL          (VBATT_DTCT_LEVEL_2_20_V)


/* Setting to Channel 0 Input 
   Please choose from macro definition of the following.
    VBATT_TAMPER_DISABLE = The TAMPI0/RTCIC0 signal input is disabled.
    VBATT_TAMPER_ENABLE = The TAMPI0/RTCIC0 signal input is enabled.
    Note: Only RX671 can use this definition.
*/
#define VBATT_CFG_TAMPER_CH0 (VBATT_TAMPER_DISABLE)


/* Setting to Channel 1 Input 
   Please choose from macro definition of the following.
    VBATT_TAMPER_DISABLE = The TAMPI1/RTCIC1 signal input is disabled.
    VBATT_TAMPER_ENABLE = The TAMPI1/RTCIC1 signal input is enabled.
    Note: Only RX671 can use this definition.
*/
#define VBATT_CFG_TAMPER_CH1 (VBATT_TAMPER_DISABLE)


/* Setting to Channel 2 Input 
   Please choose from macro definition of the following.
    VBATT_TAMPER_DISABLE = The TAMPI2/RTCIC2 signal input is disabled.
    VBATT_TAMPER_ENABLE = The TAMPI2/RTCIC2 signal input is enabled.
    Note: Only RX671 can use this definition.
*/
#define VBATT_CFG_TAMPER_CH2 (VBATT_TAMPER_DISABLE)


/* Setting to Tamper 0 Detection Interrupt Enable
   Please choose from macro definition of the following.
    VBATT_TAMPER_DETECT_INT_DISABLE = Tamper 0 detection interrupt is disabled.
    VBATT_TAMPER_DETECT_INT_ENABLE = Tamper 0 detection interrupt is enabled.
    Note: Only RX671 can use this definition.
*/
#define VBATT_CFG_TAMPER_CH0_DETECT_INT    (VBATT_TAMPER_DETECT_INT_DISABLE)


/* Setting to Tamper 0 Erase Enable
   Please choose from macro definition of the following.
    VBATT_TAMPER_ERASE_DISABLE = Backup registers are not erased in response to a tamper  event.
    VBATT_TAMPER_ERASE_ENABLE = Backup registers are erased in response to a tamper 0 event.
    Note: Only RX671 can use this definition.
*/
#define VBATT_CFG_TAMPER_CH0_ERASE         (VBATT_TAMPER_ERASE_DISABLE)


/* Setting to Time Capture Event 0 Source Select
   Please choose from macro definition of the following.
    VBATT_TAMPER_TCE_RTCIC_PIN = Input signal from the RTCIC0 pin.
    VBATT_TAMPER_TCE_TAMPER_EVENT = Tamper 0 event.
    Note: Only RX671 can use this definition.
*/
#define VBATT_CFG_TAMPER_TCE_CH0_SELECT          (VBATT_TAMPER_TCE_RTCIC_PIN)


/* Setting to Channel 0 Noise Filter Enable
   Please choose from macro definition of the following.
    VBATT_TAMPER_CHEN_NOISE_FILTER_DISABLE = Noise filter for the TAMPI0/RTCIC0 pin is disabled.
    VBATT_TAMPER_CHEN_NOISE_FILTER_ENABLE = Noise filter for the TAMPI0/RTCIC0 pin is enabled.
    Note: Only RX671 can use this definition.
*/
#define VBATT_CFG_TAMPER_CHEN_CH0_NOISE_FILTER   (VBATT_TAMPER_CHEN_NOISE_FILTER_DISABLE)


/* Setting to Channel 0 Trigger Select
   Please choose from macro definition of the following.
    VBATT_TAMPER_CHEN_FALLING_EDGE = A falling edge of the input on the TAMPI0 pin.
    VBATT_TAMPER_CHEN_RISING_EDGE = A rising edge of the input on the TAMPI0 pin.
    Note: Only RX671 can use this definition.
*/
#define VBATT_CFG_TAMPER_CHEN_CH0_EDGE           (VBATT_TAMPER_CHEN_RISING_EDGE)


/* Setting to Tamper 1 Detection Interrupt Enable
   Please choose from macro definition of the following.
    VBATT_TAMPER_DETECT_INT_DISABLE = Tamper 1 detection interrupt is disabled.
    VBATT_TAMPER_DETECT_INT_ENABLE = Tamper 1 detection interrupt is enabled.
    Note: Only RX671 can use this definition.
*/
#define VBATT_CFG_TAMPER_CH1_DETECT_INT    (VBATT_TAMPER_DETECT_INT_DISABLE)


/* Setting to Tamper 1 Erase Enable
   Please choose from macro definition of the following.
    VBATT_TAMPER_ERASE_DISABLE = Backup registers are not erased in response to a tamper  event.
    VBATT_TAMPER_ERASE_ENABLE = Backup registers are erased in response to a tamper 1 event.
    Note: Only RX671 can use this definition.
*/
#define VBATT_CFG_TAMPER_CH1_ERASE         (VBATT_TAMPER_ERASE_DISABLE)


/* Setting to Time Capture Event 1 Source Select
   Please choose from macro definition of the following.
    VBATT_TAMPER_TCE_RTCIC_PIN = Input signal from the RTCIC1 pin.
    VBATT_TAMPER_TCE_TAMPER_EVENT = Tamper 1 event.
    Note: Only RX671 can use this definition.
*/
#define VBATT_CFG_TAMPER_TCE_CH1_SELECT          (VBATT_TAMPER_TCE_RTCIC_PIN)


/* Setting to Channel 1 Noise Filter Enable
   Please choose from macro definition of the following.
    VBATT_TAMPER_CHEN_NOISE_FILTER_DISABLE = Noise filter for the TAMPI1/RTCIC1 pin is disabled.
    VBATT_TAMPER_CHEN_NOISE_FILTER_ENABLE = Noise filter for the TAMPI1/RTCIC1 pin is enabled.
    Note: Only RX671 can use this definition.
*/
#define VBATT_CFG_TAMPER_CHEN_CH1_NOISE_FILTER   (VBATT_TAMPER_CHEN_NOISE_FILTER_DISABLE)


/* Setting to Channel 1 Trigger Select
   Please choose from macro definition of the following.
    VBATT_TAMPER_CHEN_FALLING_EDGE = A falling edge of the input on the TAMPI1 pin.
    VBATT_TAMPER_CHEN_RISING_EDGE = A rising edge of the input on the TAMPI1 pin.
    Note: Only RX671 can use this definition.
*/
#define VBATT_CFG_TAMPER_CHEN_CH1_EDGE           (VBATT_TAMPER_CHEN_RISING_EDGE)


/* Setting to Tamper 2 Detection Interrupt Enable
   Please choose from macro definition of the following.
    VBATT_TAMPER_DETECT_INT_DISABLE = Tamper 2 detection interrupt is disabled.
    VBATT_TAMPER_DETECT_INT_ENABLE = Tamper 2 detection interrupt is enabled.
    Note: Only RX671 can use this definition.
*/
#define VBATT_CFG_TAMPER_CH2_DETECT_INT    (VBATT_TAMPER_DETECT_INT_DISABLE)


/* Setting to Tamper 2 Erase Enable
   Please choose from macro definition of the following.
    VBATT_TAMPER_ERASE_DISABLE = Backup registers are not erased in response to a tamper  event.
    VBATT_TAMPER_ERASE_ENABLE = Backup registers are erased in response to a tamper 2 event.
    Note: Only RX671 can use this definition.
*/
#define VBATT_CFG_TAMPER_CH2_ERASE         (VBATT_TAMPER_ERASE_DISABLE)


/* Setting to Time Capture Event 2 Source Select
   Please choose from macro definition of the following.
    VBATT_TAMPER_TCE_RTCIC_PIN = Input signal from the RTCIC2 pin.
    VBATT_TAMPER_TCE_TAMPER_EVENT = Tamper 2 event.
    Note: Only RX671 can use this definition.
*/
#define VBATT_CFG_TAMPER_TCE_CH2_SELECT          (VBATT_TAMPER_TCE_RTCIC_PIN)


/* Setting to Channel 2 Noise Filter Enable
   Please choose from macro definition of the following.
    VBATT_TAMPER_CHEN_NOISE_FILTER_DISABLE = Noise filter for the TAMPI2/RTCIC2 pin is disabled.
    VBATT_TAMPER_CHEN_NOISE_FILTER_ENABLE = Noise filter for the TAMPI2/RTCIC2 pin is enabled.
    Note: Only RX671 can use this definition.
*/
#define VBATT_CFG_TAMPER_CHEN_CH2_NOISE_FILTER   (VBATT_TAMPER_CHEN_NOISE_FILTER_DISABLE)


/* Setting to Channel 2 Trigger Select
   Please choose from macro definition of the following.
    VBATT_TAMPER_CHEN_FALLING_EDGE = A falling edge of the input on the TAMPI2 pin.
    VBATT_TAMPER_CHEN_RISING_EDGE = A rising edge of the input on the TAMPI2 pin.
    Note: Only RX671 can use this definition.
*/
#define VBATT_CFG_TAMPER_CHEN_CH2_EDGE           (VBATT_TAMPER_CHEN_RISING_EDGE)


/* Interrupt priority levels of VBATT pin voltage drop detection maskable interrupt.
   Set value must be in the range 1 (lowest) to 15 (highest).
   This setting becomes valid when VBATT_CFG_DETECT_FUNCTION is "VBATT_DTCT_ENABLE_INT_ENABLE".
   This setting becomes valid when VBATT_CFG_TAMPER_CH0_DETECT_INT, VBATT_CFG_TAMPER_CH1_DETECT_INT or 
   VBATT_CFG_TAMPER_CH2_DETECT_INT is "TAMPER_DETECT_INT_ENABLE".
*/
#define VBATT_CFG_INT_PRIORITY          (5)

/*
 * Enable Nested Interrupt
 *  0 : Disable nested interrupt.
 *  1 : Enable nested interrupt.
*/
#define VBATT_CFG_EN_NESTED_INT   (0)

#endif /* R_VBATT_RX_CONFIG_H */
