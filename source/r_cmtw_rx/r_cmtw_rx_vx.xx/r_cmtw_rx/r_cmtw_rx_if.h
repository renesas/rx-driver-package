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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_cmtw_rx_if.h
* Description  : This module creates a timer tick using a CMTW channel based on an input by the user.
*              : The module also supports output compare and input capture features to generate
*              : waveforms and detect external events.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description               
*         : 29.08.2014 1.00    First GSCE Release.
*         : 02.03.2015 1.00    Added support for RX71M
*         : 15.03.2016 1.20    Added support for RX65N
*         : 21.07.2017 1.30    Added support for RX65N-2M
*         : 31.10.2017 1.31    Added the demo for RX65N, RX65N-2M
*         : 16.11.2018 1.32    Added XML document number
*         : 01.02.2019 1.40    Added support for RX65N-64pin
*         : 20.05.2019 2.00    Added support for GNUC and ICCRX.
*         : 15.08.2019 2.10    Added support for RX72M
*         : 30.12.2019 2.20    Added support RX66N, RX72N.
***********************************************************************************************************************/
#ifndef CMTW_RX_IF_H
#define CMTW_RX_IF_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Fixed-size integer typedefs. */
#include <stdint.h>
#include "platform.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

/* Version Number of API. */
#define CMTW_RX_VERSION_MAJOR       (02)
#define CMTW_RX_VERSION_MINOR       (20)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Channel numbers */
typedef enum
{
    CMTW_CHANNEL_0 = 0,
    CMTW_CHANNEL_1,
    CMTW_CHANNEL_MAX,
} cmtw_channel_t;

/* Time base */
typedef enum
{
    CMTW_TIME_NSEC = 0,
    CMTW_TIME_USEC,
    CMTW_TIME_MSEC,
    CMTW_TIME_SEC,
    CMTW_TIME_MAX,
} cmtw_time_unit_t;

/* PCLK divisor */
typedef enum
{
    CMTW_CLK_DIV_8 = 0,         // PCLK/8
    CMTW_CLK_DIV_32,            // PCLK/32
    CMTW_CLK_DIV_128,           // PCLK/128
    CMTW_CLK_DIV_512,           // PCLK/512
    CMTW_CLK_DIV_MAX,
} cmtw_clock_divisor_t;

/* Counter clearing source */
typedef enum
{
    CMTW_CLR_CMT        = 0,
    CMTW_CLR_DISABLED   = 1,
    CMTW_CLR_IC0        = 4,
    CMTW_CLR_IC1        = 5,
    CMTW_CLR_OC0        = 6,
    CMTW_CLR_OC1        = 7,
    CMTW_CLR_MAX,
} cmtw_clear_source_t;

/* Actions to take */
typedef enum
{
    CMTW_ACTION_NONE        = 0x00,     // Do nothing with this timer
    CMTW_ACTION_TIMER       = 0x01,     // Run timer only no interrupt
    CMTW_ACTION_INTERRUPT   = 0x02,     // Generate interrupt request
    CMTW_ACTION_CALLBACK    = 0x04,     // Generate interrupt request and execute user-defined callback
    CMTW_ACTION_ONESHOT     = 0x08,     // Generate interrupt, act only once, and turn off
} cmtw_actions_t;

/* Output pin states  */
typedef enum
{
    CMTW_OUTPUT_RETAIN      = 0,        // Do not change the pin state
    CMTW_OUTPUT_LO_TOGGLE,              // Output low initially and toggle
    CMTW_OUTPUT_HI_TOGGLE,              // OUtput hi  initially and toggle
    CMTW_OUTPUT_MAX,
} cmtw_output_states_t;

/* Input pin edges to capture */
typedef enum
{
    CMTW_EDGE_RISING        = 0,        // Capture rising edge
    CMTW_EDGE_FALLING,                  // Capture falling edge
    CMTW_EDGE_ANY,                      // Capture both rising and falling edges
    CMTW_EDGE_MAX,
} cmtw_edge_states_t;

/* Control function command codes. */
typedef enum
{
    CMTW_CMD_START,                     // Activate clocking
    CMTW_CMD_RESUME,                    // Same as start
    CMTW_CMD_STOP,                      // Pause clocking
    CMTW_CMD_RESTART,                   // Zero the counter then activate clocking
    CMTW_CMD_MAX,                       // Not a valid command.
} cmtw_cmd_t;

/* Open function CM settings */
typedef struct
{
    uint32_t                time;
    cmtw_actions_t          actions;
} cmtw_cm_settings_t;

/* Open function OC settings */
typedef struct
{
    uint32_t                time;
    cmtw_actions_t          actions;
    cmtw_output_states_t    output;
} cmtw_oc_settings_t;

/* Open function IC settings */
typedef struct
{
    cmtw_actions_t          actions;
    cmtw_edge_states_t      edge;
} cmtw_ic_settings_t;

/* Open function channel settings */
typedef struct
{
    cmtw_time_unit_t            time_unit;
    cmtw_clock_divisor_t        clock_divisor;
    cmtw_clear_source_t         clear_source;
    cmtw_cm_settings_t          cm_timer;
    cmtw_oc_settings_t          oc_timer_0;
    cmtw_oc_settings_t          oc_timer_1;
    cmtw_ic_settings_t          ic_timer_0;
    cmtw_ic_settings_t          ic_timer_1;
} cmtw_channel_settings_t;

/* CMTW function return codes */
typedef enum
{
    CMTW_SUCCESS = 0,
    CMTW_ERR_BAD_CHAN,              // Invalid channel number
    CMTW_ERR_CH_NOT_ENABLED,        // Channel is disabled by user configuration
    CMTW_ERR_CH_NOT_OPENED,         // Channel not yet opened
    CMTW_ERR_CH_NOT_CLOSED,         // Channel still open from previous open
    CMTW_ERR_CH_NOT_RUNNIG,         // Channel open and received stop command
    CMTW_ERR_CH_NOT_STOPPED,        // Channel running and received start command
    CMTW_ERR_UNKNOWN_CMD,           // Control command is not recognized
    CMTW_ERR_INVALID_ARG,           // Argument is not valid for parameter
    CMTW_ERR_NULL_PTR,              // Received null pointer; missing required argument
    CMTW_ERR_LOCK,                  // The lock procedure failed
    CMTW_ERR_OUT_OF_RANGE,          // Calculated count value is not in range
} cmtw_err_t;

typedef enum
{
    CMTW_EVENT_CM = 0,
    CMTW_EVENT_IC0,
    CMTW_EVENT_IC1,
    CMTW_EVENT_OC0,
    CMTW_EVENT_OC1,
} cmtw_event_t;

typedef struct
{
    cmtw_channel_t      channel;
    cmtw_event_t        event;
    uint32_t            count;
} cmtw_callback_data_t;

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
cmtw_err_t  R_CMTW_Open(cmtw_channel_t          channel,
                        cmtw_channel_settings_t *pconfig,
                        void                    (* const pcallback)(void *pdata));

cmtw_err_t  R_CMTW_Control(cmtw_channel_t   channel,
                           cmtw_cmd_t       cmd);

cmtw_err_t  R_CMTW_Close(cmtw_channel_t channel);

uint32_t    R_CMTW_GetVersion(void);

#endif  /* CMTW_RX_IF_H */
