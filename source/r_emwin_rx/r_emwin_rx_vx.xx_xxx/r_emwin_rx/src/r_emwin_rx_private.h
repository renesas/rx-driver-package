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
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
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
 *         : 31.08.2022 6.26.c.1.00    Update emWin library to v6.26c.
 *                                     Moved EMWIN_DISPLAY_DRIVER from r_emwin_rx_config.h to this file. 
 *                                     Since it can be determined by the interface whether it is LiN or FlexColor, 
 *                                     it is changed to handle it internally.
 *                                     Added touch driver IC settings.
 *         : 31.03.2023 6.32.a.1.00    Update emWin library to v6.32a.
 *                                     Fixed preprocessing with value of BSP_CFG_RTOS_USED.
 *         : 31.01.2024 6.34.g.1.00    Update emWin library to v6.34g.
 *                                     Added BUFFER_ADJUST_OFFSET definition.
 *                                     Fixed NUM_COLORS definition.
 *********************************************************************************************************************/
#ifndef EMWIN_RX_PRIVATE_H
#define EMWIN_RX_PRIVATE_H

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

/* Display Driver */
#define DISP_DRV_GUIDRV_LIN           (0)
#define DISP_DRV_GUIDRV_FLEXCOLOR     (1)
#define DISP_DRV_GUIDRV_OTHER         (99)

#if (EMWIN_LCD_IF == LCD_IF_GLCDC)
#define EMWIN_DISPLAY_DRIVER          (DISP_DRV_GUIDRV_LIN)
#elif ((EMWIN_LCD_IF == LCD_IF_RSPI) || (EMWIN_LCD_IF == LCD_IF_SCI_SPI))
#define EMWIN_DISPLAY_DRIVER          (DISP_DRV_GUIDRV_FLEXCOLOR)
#elif (EMWIN_LCD_IF == LCD_IF_OTHER)
#define EMWIN_DISPLAY_DRIVER          (DISP_DRV_GUIDRV_OTHER)
#endif


/* Buffer size and stride */
#define BYTES_PER_LINE       ((EMWIN_BITS_PER_PIXEL * EMWIN_XSIZE_PHYS) / 8)
#define LINE_OFFSET          (((BYTES_PER_LINE + 63) / 64) * 64)
#define VXSIZE_PHYS          ((LINE_OFFSET * 8) / EMWIN_BITS_PER_PIXEL)
#define BYTES_PER_BUFFER     (LINE_OFFSET * EMWIN_YSIZE_PHYS)
#define BUFFER_ADJUST_OFFSET (0x40)


/* Color number */
#if (EMWIN_BITS_PER_PIXEL < 16)
#define NUM_COLORS (1 << EMWIN_BITS_PER_PIXEL)
#endif


/* Touch driver */
#define TOUCH_DRV_IC_FT5306    (0)
#define TOUCH_DRV_IC_XPT2046   (1)
#define TOUCH_DRV_IC_OTHER     (99)

/* Touch Driver IC (SPI Interface) */
/* Note:
 * Preparations for adding touch drivers in the future version. */
#define EMWIN_TOUCH_DRIVER_IC  (TOUCH_DRV_IC_XPT2046)

/* Buffer for passing touch data to emWin */
#define TOUCH_DATA_BUFFER_SIZE (GUI_PID_BUFFER_SIZE * 6 + 3)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
#if (BSP_CFG_RTOS_USED == 0) /* Non-OS */
typedef void * cb_timer_arg_t;
#elif (BSP_CFG_RTOS_USED == 1) /* FreeRTOS */
typedef TimerHandle_t cb_timer_arg_t;
#else
#warning "Warning!! It is necessary to implement callback functuon for timer."
#endif

/* Callback function of timer */
typedef void emwin_rx_cb_timer (cb_timer_arg_t arg);

/** Return value */
typedef enum e_emwin_rx_err
{
    EMWIN_RX_SUCCESS = 0u,  /* Success. */
    EMWIN_RX_FAIL,          /* Fail.    */
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
