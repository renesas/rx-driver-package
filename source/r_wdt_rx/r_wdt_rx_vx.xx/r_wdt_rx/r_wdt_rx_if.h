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
* File Name    : r_wdt_rx_if.h
* Description  : Functions for using WDT on RX devices.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*           28.04.2016 1.00    First Release for RX65N
*           01.10.2017 1.10    Added support for RX65N-2MB
*           31.10.2017 1.20    Added support for RX230, RX231, RX64M, RX71M
*           28.09.2018 1.30    Added support for RX66T
*           16.11.2018 1.31    Added XML document number
*           01.02.2019 1.40    Added support for RX72T, RX65N-64pin.
*                              Bug fix: R_WDT_Open(), R_IWDT_Open() invalidated if either module is in auto-start mode.
*           20.05.2019 2.00    Added support for GNUC and ICCRX.
*           28.06.2019 2.10    Added support for RX23W
*           15.08.2019 2.20    Added support for RX72M
*           30.12.2019 2.30    Added support RX66N, RX72N.
***********************************************************************************************************************/

#ifndef WDT_RX_IF_H
#define WDT_RX_IF_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"
/* Used for configuring the WDT code */
#include "r_wdt_rx_config.h"


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

/* Version Number of API. */
#define WDT_RX_VERSION_MAJOR   (2)
#define WDT_RX_VERSION_MINOR   (30)

#define OFS0_WDT_DISABLED              (0x00020000)

#define WDT_STAT_REFRESH_ERR_MASK      (0x8000)
#define WDT_STAT_UNDERFLOW_ERR_MASK    (0x4000)
#define WDT_STAT_ERROR_MASK            (0xC000)
#define WDT_STAT_COUNTER_MASK          (0x3FFF)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/* API ERROR RETURN CODES */

typedef enum e_wdt_err       // WDT API error codes
{
    WDT_SUCCESS=0,
    WDT_ERR_OPEN_IGNORED,    // The module has already been Open
    WDT_ERR_INVALID_ARG,     // Argument is not valid for parameter
    WDT_ERR_NULL_PTR,        // Received null pointer or missing required argument
    WDT_ERR_NOT_OPENED,      // Open function has not yet been called
    WDT_ERR_BUSY,            // WDT resource locked
} wdt_err_t;


/* Open() DEFINITIONS */

typedef enum e_wdt_timeout           // WDT Time-Out Period
{

    WDT_TIMEOUT_1024 =0x0000u,        // 1024 (cycles)
    WDT_TIMEOUT_4096 =0x0001u,        // 4096 (cycles)
    WDT_TIMEOUT_8192 =0x0002u,        // 8192 (cycles)
    WDT_TIMEOUT_16384=0x0003u,        // 16,384 (cycles)
    WDT_NUM_TIMEOUTS
} wdt_timeout_t;

typedef enum e_wdt_clock_div          // WDT Clock Division Ratio
{
    WDT_CLOCK_DIV_4   =0x0010u,       // PCLK/4
    WDT_CLOCK_DIV_64  =0x0040u,       // PCLK/64
    WDT_CLOCK_DIV_128 =0x00F0u,       // PCLK/128
    WDT_CLOCK_DIV_512 =0x0060u,       // PCLK/512
    WDT_CLOCK_DIV_2048=0x0070u,       // PCLK/2048
    WDT_CLOCK_DIV_8192=0x0080u        // PCLK/8192
} wdt_clock_div_t;

typedef enum e_wdt_window_end         // Window End Position
{
    WDT_WINDOW_END_75=0x0000u,        // 75%
    WDT_WINDOW_END_50=0x0100u,        // 50%
    WDT_WINDOW_END_25=0x0200u,        // 25%
    WDT_WINDOW_END_0 =0x0300u         // 0% (window end position is not specified)
} wdt_window_end_t;

typedef enum e_wdt_window_start       // Window Start Position
{
    WDT_WINDOW_START_25 =0x0000u,     // 25%
    WDT_WINDOW_START_50 =0x1000u,     // 50%
    WDT_WINDOW_START_75 =0x2000u,     // 75%
    WDT_WINDOW_START_100=0x3000u      // 100% (window start position is not specified)
} wdt_window_start_t;

typedef enum e_wdt_timeout_control    // Signal control when Time-out and Refresh error
{
    WDT_TIMEOUT_NMI  =0x00u,          // Non-maskable interrupt request output is enabled
    WDT_TIMEOUT_RESET=0x80u           // Reset output is enabled
} wdt_timeout_control_t;

typedef struct st_wdt_config                  // WDT configuration options used in Open function
{
    wdt_timeout_t          timeout;           // Time-out period   
    wdt_clock_div_t        wdtcks_div;        // WDT clock division ratio
    wdt_window_start_t     window_start;      // Window start position
    wdt_window_end_t       window_end;        // Window end position
    wdt_timeout_control_t  timeout_control;   // Reset or NMI output when time-out
 } wdt_config_t;


 /* Control() DEFINITIONS */

typedef enum e_wdt_cmd                 // Command used in Control and GetStatus function
{
    WDT_CMD_GET_STATUS,                // Get WDT status
    WDT_CMD_REFRESH_COUNTING,          // Refresh the counter
} wdt_cmd_t;


/***********************************************************************************************************************
Public Functions
***********************************************************************************************************************/
#if ((BSP_CFG_OFS0_REG_VALUE & OFS0_WDT_DISABLED) == OFS0_WDT_DISABLED) /* Register start mode */
wdt_err_t    R_WDT_Open(void * const p_cfg);
#endif
wdt_err_t    R_WDT_Control(wdt_cmd_t const cmd, uint16_t * p_status);
uint32_t     R_WDT_GetVersion(void);

#endif /* WDT_RX_IF_H */
