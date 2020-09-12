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
#define VBATT_CFG_PARAM_CHECKING_ENABLE (1)


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


/* Interrupt priority levels of VBATT pin voltage drop detection maskable interrupt.
   Set value must be in the range 1 (lowest) to 15 (highest).
   This setting becomes valid when VBATT_CFG_DETECT_FUNCTION is "VBATT_DTCT_ENABLE_INT_ENABLE".
*/
#define VBATT_CFG_INT_PRIORITY          (5)


#endif /* R_VBATT_RX_CONFIG_H */
