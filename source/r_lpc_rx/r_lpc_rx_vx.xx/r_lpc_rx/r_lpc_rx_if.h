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
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_lpc_rx_if.h
 * Description  : The LPC module configures the MCU for the different operating and low power modes.
 *                This file is the LPC module's interface header file and should be included by the application that
 *                intends to use the API.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description           
 *         : 01.10.2016 1.00    First Release
 *         : 01.04.2019 1.41    Changed Minor version to 1.41.
 *         : 01.07.2019 1.42    Changed Minor version to 1.42.
 *         : 14.11.2019 2.00    Changed version to 2.00.
 ***********************************************************************************************************************/

#ifndef R_LPC_RX_IF_H
    #define R_LPC_RX_IF_H

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
    #include <stdint.h>

/* Includes board and MCU related header files. */
    #include "platform.h"
    #include "r_lpc_rx_config.h"
    #include "r_lpc_rx_platform.h"

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

/* Version Number of API. */
    #define LPC_RX_VERSION_MAJOR           (2)
    #define LPC_RX_VERSION_MINOR           (0)

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/
/*  LPC API Error codes */
typedef enum lpc_err
{
    LPC_SUCCESS = 0,
    LPC_ERR_OSC_STOP_ENABLED,     // Software Standby Mode cannot be entered if Oscillation Stop is enabled
    LPC_ERR_CLOCK_EXCEEDED,       // If clock speed exceeds the limit of current operating mode
    LPC_ERR_ILL_MAIN_CLK_FREQ,    // If Main Oscillator is more than limit and is chosen as the Sleep return clock
    LPC_ERR_ILL_CLOCK_SOURCE,     // Return Clock Switch is only supported if clock on entering Sleep is allowed clock
    LPC_ERR_P_E_MODE,             // The OPCCR register cannot be rewritten while the flash memory is being programmed or erased (P/E).
    LPC_ERR_DEEP_SLEEP_STATUS,    // The condition to go to a deep sleep mode isn't met.
    LPC_ERR_ILL_PARAM,            // Illegal argument not specified in enum
    LPC_ERR_ILLEGAL               // Illegal setting
} lpc_err_t;

typedef void (*lpc_callback_set_t) (void *pdata); /* Callback function type */
 
/***********************************************************************************************************************
 Exported global variables
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/
uint32_t R_LPC_GetVersion (void);
lpc_err_t R_LPC_OperatingModeSet (lpc_operating_mode_t e_mode);
lpc_err_t R_LPC_LowPowerModeConfigure (lpc_low_power_mode_t e_mode);
lpc_err_t R_LPC_LowPowerModeActivate (void (*pcallback) (void* pdata));
lpc_err_t R_LPC_ReturnClockSwitch (lpc_clock_switch_t e_clock_source, bool enable);

#endif /* R_LPC_RX_IF_H */

