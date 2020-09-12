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
* Copyright (C) 2013-2019 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_iwdt_rx_if.h
* Description  : Functions for using IWDT on RX devices.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*           07.06.2013 1.00    First Release
*           06.09.2013 1.10    Change enumeration name
*           19.03.2014 1.20    Removed unnecessary #includes. Added #ifdef to e_iwdt_timeout. Changed version number.
*           30.12.2014 1.30    Added support for RX113.
*           06.03.2015 1.40    Added support for RX64M & RX71M
*           02.06.2015 1.50    Added support for RX231
*           01.03.2016 1.51    Added support for RX130, RX230 and RX23T
*           30.06.2016 1.60    Added support for RX65N
*           19.12.2016 1.70    Added support for RX24U, RX24T(512KB)
*           21.07.2017 1.80    Added support for RX130_512KB, RX65N-2M
*           31.10.2017 1.81    Added the demo for RX65N, RX65N-2M
*           28.09.2018 1.90    Added support for RX66T
*           16.11.2018 1.91    Added XML document number
*           01.02.2019 2.00    Added support for RX72T, RX65N-64pin.
*                              Bug fix: R_WDT_Open(), R_IWDT_Open() invalidated if either module is in auto-start mode.
*           20.05.2019 3.00    Added support for GNUC and ICCRX.
*           28.06.2019 3.10    Added support for RX23W.
*           15.08.2019 3.20    Added support for RX72M.
*           25.11.2019 3.30    Added support for RX13T.
*                              Removed support for Generation 1 devices.
*                              Fixed to comply with GSCE Coding Standards Rev.6.00.
*           30.12.2019 3.40    Added support for RX66N, RX72N.
***********************************************************************************************************************/

#ifndef IWDT_RX_IF_H
#define IWDT_RX_IF_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"
/* Used for configuring the IWDT code */
#include "r_iwdt_rx_config.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif
/* Version Number of API. */
#define IWDT_RX_VERSION_MAJOR   (3)
#define IWDT_RX_VERSION_MINOR   (40)

#define OFS0_IWDT_DISABLED      (0x00000002)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/* API ERROR RETURN CODES */

#define IWDT_STAT_REFRESH_ERR_MASK      (0x8000)
#define IWDT_STAT_UNDERFLOW_ERR_MASK    (0x4000)
#define IWDT_STAT_ERROR_MASK            (0xC000)
#define IWDT_STAT_COUNTER_MASK          (0x3FFF)

typedef enum e_iwdt_err       // IWDT API error codes
{
    IWDT_SUCCESS=0,
    IWDT_ERR_OPEN_IGNORED,    // The module has already been Open
    IWDT_ERR_INVALID_ARG,     // Argument is not valid for parameter
    IWDT_ERR_NULL_PTR,        // Received null pointer or missing required argument
    IWDT_ERR_NOT_OPENED,      // Open function has not yet been called
    IWDT_ERR_BUSY,            // IWDT resource locked
} iwdt_err_t;


/* Open() DEFINITIONS */

typedef enum e_iwdt_timeout           // IWDT Time-Out Period
{
#if defined(BSP_MCU_RX11_ALL) || defined(BSP_MCU_RX130) || defined(BSP_MCU_RX13T) || defined(BSP_MCU_RX23_ALL) || defined(BSP_MCU_RX24U)
    IWDT_TIMEOUT_128 =0x0000u,        // 128  (cycles) 
    IWDT_TIMEOUT_512 =0x0001u,        // 512  (cycles)
    IWDT_TIMEOUT_1024=0x0002u,        // 1024 (cycles)
    IWDT_TIMEOUT_2048=0x0003u,        // 2048 (cycles)
#else /* RX64M, RX71M, RX65N, RX66T, RX66N, RX72T, RX72M, RX72N */
    IWDT_TIMEOUT_1024 =0x0000u,        // 1024 (cycles)
    IWDT_TIMEOUT_4096 =0x0001u,        // 4096 (cycles)
    IWDT_TIMEOUT_8192 =0x0002u,        // 8192 (cycles)
    IWDT_TIMEOUT_16384=0x0003u,        // 16,384 (cycles)
#endif
    IWDT_NUM_TIMEOUTS
} iwdt_timeout_t;

typedef enum e_iwdt_clock_div           // IWDT Clock Division Ratio
{
    IWDT_CLOCK_DIV_1  =0x0000u,       // IWDTCLK/1
    IWDT_CLOCK_DIV_16 =0x0020u,       // IWDTCLK/16
    IWDT_CLOCK_DIV_32 =0x0030u,       // IWDTCLK/32
    IWDT_CLOCK_DIV_64 =0x0040u,       // IWDTCLK/64
    IWDT_CLOCK_DIV_128=0x00F0u,       // IWDTCLK/128
    IWDT_CLOCK_DIV_256=0x0050u        // IWDTCLK/256
} iwdt_clock_div_t;

typedef enum e_iwdt_window_end           // Window End Position
{
    IWDT_WINDOW_END_75=0x0000u,       // 75%
    IWDT_WINDOW_END_50=0x0100u,       // 50%
    IWDT_WINDOW_END_25=0x0200u,       // 25%
    IWDT_WINDOW_END_0 =0x0300u        // 0% (window end position is not specified)
} iwdt_window_end_t;

typedef enum e_iwdt_window_start         // Window Start Position
{
    IWDT_WINDOW_START_25 =0x0000u,    // 25%
    IWDT_WINDOW_START_50 =0x1000u,    // 50%
    IWDT_WINDOW_START_75 =0x2000u,    // 75%
    IWDT_WINDOW_START_100=0x3000u     // 100% (window start position is not specified)
} iwdt_window_start_t;

typedef enum e_iwdt_timeout_control   // Signal control when Time-out and Refresh error
{
    IWDT_TIMEOUT_NMI  =0x00u,         // Non-maskable interrupt request output is enabled
    IWDT_TIMEOUT_RESET=0x80u          // Reset output is enabled
} iwdt_timeout_control_t;

typedef enum e_iwdt_count_stop        // Sleep mode count stop
{
    IWDT_COUNT_STOP_DISABLE=0x00u,    // 0: Count stop is disabled 
    IWDT_COUNT_STOP_ENABLE =0x80u     // 1: Count stop is enabled
} iwdt_count_stop_t;

typedef struct st_iwdt_config                 // IWDT configuration options used in Open function
{
    iwdt_timeout_t          timeout;          // Time-out period   
    iwdt_clock_div_t        iwdtclk_div;      // IWDT clock division ratio
    iwdt_window_start_t     window_start;     // Window start position
    iwdt_window_end_t       window_end;       // Window end position
    iwdt_timeout_control_t  timeout_control;  // Reset or NMI output when time-out  
    iwdt_count_stop_t       count_stop_enable;// Sleep mode count stop function 
 } iwdt_config_t;


 /* Control() DEFINITIONS */

typedef enum e_iwdt_cmd                 // Command used in Control and GetStatus function
{
    IWDT_CMD_GET_STATUS,                // Get IWDT status
    IWDT_CMD_REFRESH_COUNTING           // Refresh the counter
} iwdt_cmd_t;

/***********************************************************************************************************************
Public Functions
***********************************************************************************************************************/
#if ((BSP_CFG_OFS0_REG_VALUE & OFS0_IWDT_DISABLED) == OFS0_IWDT_DISABLED) /* Register start mode */
/* Initializes the IWDT counter options by initializing the associated registers */
iwdt_err_t    R_IWDT_Open(void * const p_cfg);
#endif

/* Getting the IWDT status and refreshing the down-counter of IWDT */
iwdt_err_t    R_IWDT_Control(iwdt_cmd_t const cmd, uint16_t * p_status);

/* Returns the version number of the module. */
uint32_t      R_IWDT_GetVersion(void);

#endif /* IWDT_RX_IF_H */
