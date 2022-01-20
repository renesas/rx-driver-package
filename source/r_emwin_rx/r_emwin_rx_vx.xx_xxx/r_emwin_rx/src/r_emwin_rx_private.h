/**********************************************************************************************************************
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
 *
 * Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_emwin_rx_private.h
 * Version      : 1.00
 * Description  : Header file of emWin FIT module internal functions.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version        Description
 *         : 31.07.2020 6.14.a.1.00    First Release
 *         : 04.09.2020 6.14.a.1.10    Update to adjust r_emwin_rx_config.h file.
 *         : 11.12.2020 6.14.g.1.20    Update to adjust emWin v6.14g. Modify multi-touch and timer function.
 *                                     Adjust GCC and IAR compilers.
 *         : 31.03.2021 6.14.g.1.30    Update to adjust the spec of Smart Configurator and QE for Display.
 *         : 29.12.2021 6.20.  1.00    Update emWin library to v6.22.
 *                                     Adjust configuration option with Smart Configurator.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "GUI.h"
#include "platform.h"
#include "r_emwin_rx_if.h"
#include "r_emwin_rx_config.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef EMWIN_RX_PRIVATE_H
#define EMWIN_RX_PRIVATE_H

/* Buffer size and stride */
#define BYTES_PER_LINE   ((EMWIN_BITS_PER_PIXEL * EMWIN_XSIZE_PHYS) / 8)
#define LINE_OFFSET      (((BYTES_PER_LINE + 63) / 64) * 64)
#define VXSIZE_PHYS      ((LINE_OFFSET * 8) / EMWIN_BITS_PER_PIXEL)
#define BYTES_PER_BUFFER (LINE_OFFSET * EMWIN_YSIZE_PHYS)

/* Color number */
#if ((USE_DAVE2D == 0) && (EMWIN_BITS_PER_PIXEL < 16))
#define NUM_COLORS (1 << EMWIN_BITS_PER_PIXEL)
#else
#define NUM_COLORS  (256)
#endif

#define TOUCH_DATA_BUFFER_SIZE (GUI_PID_BUFFER_SIZE * 6 + 3)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
#if (BSP_CFG_RTOS_USED == 0) /* Non-OS */
typedef void * cb_timer_arg_t;
#elif (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
typedef TimerHandle_t cb_timer_arg_t;
#elif (BSP_CFG_RTOS_USED == 2) /* SEGGER embOS */
#warning "Warning!! It is necessary to implement callback functuon for timer."
#elif (BSP_CFG_RTOS_USED == 3) /* Micrium MicroC/OS */
#warning "Warning!! It is necessary to implement callback functuon for timer."
#elif (BSP_CFG_RTOS_USED == 4) /* Renesas RI600V4 & RI600PX */
#warning "Warning!! It is necessary to implement callback functuon for timer."
#elif (BSP_CFG_RTOS_USED == 5) /* Azure RTOS */
#warning "Warning!! It is necessary to implement callback functuon for timer."
#else
#warning "Warning!! It is necessary to implement callback functuon for timer."
#endif

/* Callback function of timer */
typedef void emwin_rx_cb_timer (cb_timer_arg_t arg);

/** Return value */
typedef enum e_emwin_rx_err
{
    EMWIN_RX_SUCCESS = 0u,  // Success.
    EMWIN_RX_FAIL,          // Fail.
} e_emwin_rx_err_t;

typedef enum
{
    EMWIN_RX_LCD_SWITCH_ON = 0u,
    EMWIN_RX_LCD_SWITCH_OFF,
} e_emwin_rx_lcd_switch_t;

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_emwin_rx_lcd_open
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
e_emwin_rx_err_t r_emwin_rx_lcd_open (void);

/**********************************************************************************************************************
 * Function Name: r_emwin_rx_lcd_switch_buffer
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
e_emwin_rx_err_t r_emwin_rx_lcd_switch_buffer (uint32_t * p_addr);

/**********************************************************************************************************************
 * Function Name: r_emwin_rx_lcd_clut_update
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
e_emwin_rx_err_t r_emwin_rx_lcd_clut_update (uint32_t * p_clut);

/**********************************************************************************************************************
 * Function Name: r_emwin_rx_lcd_switch
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
e_emwin_rx_err_t r_emwin_rx_lcd_switch (e_emwin_rx_lcd_switch_t lcd_switch);

/**********************************************************************************************************************
 * Function Name: r_emwin_rx_pid_open
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
e_emwin_rx_err_t r_emwin_rx_pid_open (void);

/**********************************************************************************************************************
 * Function Name: r_emwin_rx_pidconf_cb
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
e_emwin_rx_err_t r_emwin_rx_pidconf_cb (uint8_t * p_addr, uint32_t size);

/**********************************************************************************************************************
 * Function Name: r_emwin_rx_guix_timer_create
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
e_emwin_rx_err_t r_emwin_rx_guix_timer_create (uint32_t period_ms, emwin_rx_cb_timer * pf_callback_timer);

#endif /* EMWIN_RX_PRIVATE_H */
