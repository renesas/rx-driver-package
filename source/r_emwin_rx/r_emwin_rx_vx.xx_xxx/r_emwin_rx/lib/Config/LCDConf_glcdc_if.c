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
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : LCDConf_glcdc_if.c
 * Version      : 1.00
 * Description  : Display controller configuration (use RGB interface).
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
 *                                     Changed preprocessor code to activate the entire code
 *                                     by selecting the LCD interface.
 *                                     Merge the source code of r_emwin_rx_lcdif.c into this file
 *                                     and remove r_emwin_rx_lcdif.c.
 *                                     Fixed an issue where images could not be displayed correctly
 *                                     with Clut8 settings. (set_lut_entry function)
 *                                     Changed to use draw_bitmap_alpha function only in CCRX compiler
 *                                     when DRW2D is enabled (Usage restriction)
 *                                     Disable the following process to draw a circle by DRW2D (Usage restriction)
 *                                      - circle_aa
 *                                      - draw_arc_aa
 *                                      - fill_circle_aa
 *                                      - draw_circle_aa
 *         : 31.03.2023 6.32.a.1.00    Update emWin library to v6.32a.
 *                                     Added support for runtime orientation.
 *                                     Remove restrictions on the a draw_bitmap_alpha function.
  *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

#include <stdlib.h>
#include "r_emwin_rx_if.h"

#if (EMWIN_LCD_IF == LCD_IF_GLCDC)

#include "GUI.h"
#include "GUIDRV_Lin.h"
#include "LCDConf.h"

#include "r_glcdc_rx_if.h"
#include "r_glcdc_rx_pinset.h"
#include "r_dmaca_rx_if.h"
#include "r_gpio_rx_if.h"

#include "../../src/r_emwin_rx_private.h"

#if (USE_DAVE2D == 1)
#include "dave_base.h"
#include "dave_driver.h"
#endif

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/* Display driver */

#if   (EMWIN_BITS_PER_PIXEL == 32)
#if (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_0)
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_32)         /* 0   */
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_OSX_32)     /* CW  */
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_OXY_32)     /* 180 */
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_OSY_32)     /* CCW */
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_CW)
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_OSX_32)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_OXY_32)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_OSY_32)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_32)
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_CCW)
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_OSY_32)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_32)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_OSX_32)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_OXY_32)
#else
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_OXY_32)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_OSY_32)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_32)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_OSX_32)
#endif

#elif (EMWIN_BITS_PER_PIXEL == 16)
#if (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_0)
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_16)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_OSX_16)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_OXY_16)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_OSY_16)
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_CW)
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_OSX_16)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_OXY_16)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_OSY_16)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_16)
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_CCW)
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_OSY_16)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_16)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_OSX_16)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_OXY_16)
#else
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_OXY_16)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_OSY_16)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_16)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_OSX_16)
#endif

#elif (EMWIN_BITS_PER_PIXEL == 8)
#if (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_0)
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_8)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_OSX_8)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_OXY_8)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_OSY_8)
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_CW)
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_OSX_8)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_OXY_8)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_OSY_8)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_8)
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_CCW)
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_OSY_8)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_8)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_OSX_8)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_OXY_8)
#else
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_OXY_8)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_OSY_8)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_8)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_OSX_8)
#endif

#elif (EMWIN_BITS_PER_PIXEL == 4)
#if (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_0)
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_4)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_OSX_4)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_OXY_4)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_OSY_4)
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_CW)
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_OSX_4)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_OXY_4)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_OSY_4)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_4)
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_CCW)
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_OSY_4)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_4)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_OSX_4)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_OXY_4)
#else
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_OXY_4)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_OSY_4)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_4)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_OSX_4)
#endif

#elif (EMWIN_BITS_PER_PIXEL == 1)
#if (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_0)
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_1)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_OSX_1)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_OXY_1)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_OSY_1)
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_CW)
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_OSX_1)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_OXY_1)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_OSY_1)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_1)
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_CCW)
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_OSY_1)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_1)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_OSX_1)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_OXY_1)
#else
#define DISPLAY_DRIVER_0    (GUIDRV_LIN_OXY_1)
#define DISPLAY_DRIVER_1    (GUIDRV_LIN_OSY_1)
#define DISPLAY_DRIVER_2    (GUIDRV_LIN_1)
#define DISPLAY_DRIVER_3    (GUIDRV_LIN_OSX_1)
#endif
#endif

/* Color Conversion Routine */
#if   (EMWIN_BITS_PER_PIXEL == 32)
#define COLOR_CONVERSION (GUICC_M8888I)
#elif (EMWIN_BITS_PER_PIXEL == 16)
#define COLOR_CONVERSION (GUICC_M565)
#elif (EMWIN_BITS_PER_PIXEL == 8)
#define COLOR_CONVERSION (GUICC_8666)
#elif (EMWIN_BITS_PER_PIXEL == 4)
#define COLOR_CONVERSION (GUICC_16)
#elif (EMWIN_BITS_PER_PIXEL == 1)
#define COLOR_CONVERSION (GUICC_1)
#endif

/* Color format selection */
#define FORMAT_RGB_565   (GLCDC_IN_FORMAT_16BITS_RGB565)
#define FORMAT_RGB_888   (GLCDC_IN_FORMAT_32BITS_RGB888)
#define FORMAT_ARGB_1555 (GLCDC_IN_FORMAT_16BITS_ARGB1555)
#define FORMAT_ARGB_4444 (GLCDC_IN_FORMAT_16BITS_ARGB4444)
#define FORMAT_ARGB_8888 (GLCDC_IN_FORMAT_32BITS_ARGB8888)
#define FORMAT_CLUT_8    (GLCDC_IN_FORMAT_CLUT8)
#define FORMAT_CLUT_4    (GLCDC_IN_FORMAT_CLUT4)
#define FORMAT_CLUT_1    (GLCDC_IN_FORMAT_CLUT1)

/* Color conversion */
#if   (EMWIN_BITS_PER_PIXEL == 32)
#define COLOR_FORMAT     (FORMAT_ARGB_8888)
#elif (EMWIN_BITS_PER_PIXEL == 16)
#define COLOR_FORMAT     (FORMAT_RGB_565)
#elif (EMWIN_BITS_PER_PIXEL == 8)
#define COLOR_FORMAT     (FORMAT_CLUT_8)
#elif (EMWIN_BITS_PER_PIXEL == 4)
#define COLOR_FORMAT     (FORMAT_CLUT_4)
#elif (EMWIN_BITS_PER_PIXEL == 1)
#define COLOR_FORMAT     (FORMAT_CLUT_1)
#endif


/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
#if (USE_DAVE2D == 1)
typedef int DRAW_POLY_OUTLOINE (const GUI_POINT * p_src, int num_points, int thickness, int x, int y);
typedef int DRAW_ARC (int x0, int y0, int rx, int ry, I32 a0, I32 a1);
typedef int FILL_POLYGON (const GUI_POINT * p_points, int num_points, int x0, int y0);
typedef int FILL_CIRCLE (int x0, int y0, int r);
typedef int DRAW_CIRCLE (int x0, int y0, int r);
typedef int DRAW_LINE (int x0, int y0, int x1, int y1);
#endif



/* GLCDC FIT module setting check */
#if ((GLCDC_CFG_CONFIGURATION_MODE == 1) || defined(QE_DISPLAY_CONFIGURATION))

#if (EMWIN_XSIZE_PHYS != LCD_CH0_DISP_HW) || (EMWIN_XSIZE_PHYS != LCD_CH0_IN_GR2_HSIZE)
#warning "Warning!! There is a problem in the setting of the GLCDC FIT module.\n Missing the graphic layer 2 width setting."
#endif

#if (EMWIN_YSIZE_PHYS != LCD_CH0_DISP_VW) || (EMWIN_YSIZE_PHYS != LCD_CH0_IN_GR2_VSIZE)
#warning "Warning!! There is a problem in the setting of the GLCDC FIT module.\n Missing the graphic layer 2 height setting."
#endif

#if (LINE_OFFSET != LCD_CH0_IN_GR2_LINEOFFSET)
#warning "Warning!! There is a problem in the setting of the GLCDC FIT module.\n Missing the graphic layer 2 line offset setting."
#endif

#ifndef __GNUC__
#if (LCD_CH0_IN_GR1_PBASE != NULL)
#warning "Warning!! There is a problem in the setting of the GLCDC FIT module.\n Missing the graphic layer 1 setting."
#endif
#else
    /* Please check the graphic layer 1 setting. */
#endif

#if (LCD_CH0_DETECT_VPOS != true)
#warning "Warning!! There is a problem in the setting of the GLCDC FIT module.\n Missing the line detection setting."
#endif

#if (LCD_CH0_INTERRUPT_VPOS_ENABLE != true)
#warning "Warning!! There is a problem in the setting of the GLCDC FIT module.\n Missing the line detection interrupt setting."
#endif

#if (LCD_CH0_CALLBACK_ENABLE != true)
#warning "Warning!! There is a problem in the setting of the GLCDC FIT module.\n Missing the callback function setting. "
#endif

#endif /* (GLCDC_CFG_CONFIGURATION_MODE == 1) || defined(QE_DISPLAY_CONFIGURATION) */


/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
static const uint32_t s_a_buffer_ptr[] =
{
    EMWIN_GUI_FRAME_BUFFER1,
#if (EMWIN_NUM_BUFFERS > 1)
    EMWIN_GUI_FRAME_BUFFER2,
#if (EMWIN_NUM_BUFFERS > 2)
    EMWIN_GUI_FRAME_BUFFER3,
#if (EMWIN_NUM_BUFFERS > 3)
    #warning "Warning!! It is necessary to implement frame buffer addresses."
#endif
#endif
#endif
};

static uint32_t s_write_buffer_index;

#if (EMWIN_BITS_PER_PIXEL < 16)
static uint32_t s_a_clut[NUM_COLORS];
#endif


static volatile int32_t s_pending_buffer = -1;

static glcdc_runtime_cfg_t s_runtime_cfg;
static volatile bool s_first_interrupt_flag; /* It is used for interrupt control of GLCDC module */

/* Dave2D */
#if (USE_DAVE2D == 1)
static uint32_t        s_dave_active;
static d2_device       * sp_d2_handle;
static d2_renderbuffer * sp_renderbuffer;
static int s_lcd_current_orientation;
#endif

#if (USE_DAVE2D == 1)
static void lcd_fill_rect (int32_t layer_index, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t pixel_index);
#endif

static e_emwin_rx_err_t r_emwin_rx_lcd_open(void);
static e_emwin_rx_err_t r_emwin_rx_lcd_switch_buffer(uint32_t * p_addr);
static e_emwin_rx_err_t r_emwin_rx_lcd_switch(e_emwin_rx_lcd_switch_t lcd_switch);

#if (EMWIN_BITS_PER_PIXEL < 16)
static e_emwin_rx_err_t r_emwin_rx_lcd_clut_update(uint32_t * p_clut);
#endif


/**********************************************************************************************************************
 * Function Name: switch_buffers_on_vsync
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void switch_buffers_on_vsync(int32_t index)
{
    e_emwin_rx_err_t ret;
    uint32_t *       p_addr;

    p_addr = (uint32_t *)s_a_buffer_ptr[index];
    ret    = r_emwin_rx_lcd_switch_buffer(p_addr);

    if (EMWIN_RX_SUCCESS == ret)
    {
        GUI_MULTIBUF_ConfirmEx(0, index); /* Tell emWin that buffer is used */
    }
    else
    {
        /* You should not end up here. Buffer change has failed, check "ret" to get further information
         * or add code to debug the issue. */
        R_BSP_NOP();
    }
}
/**********************************************************************************************************************
 * End of function switch_buffers_on_vsync
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: init_controller
 * Description  : Should initialize the display controller.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void init_controller(void)
{
    e_emwin_rx_err_t ret;

    /* Register Dave2D interrupt */
#if (USE_DAVE2D == 1)
    R_BSP_InterruptWrite(BSP_INT_SRC_AL1_DRW2D_DRW_IRQ, (bsp_int_cb_t)drw_int_isr);
#endif

    ret = r_emwin_rx_lcd_open();

    if (EMWIN_RX_SUCCESS != ret)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* Error */
        }
    }

#if (EMWIN_USE_DISP_SIGNAL_PIN == 1)
    /* Set DISP signal on */
    R_GPIO_PinDirectionSet(EMWIN_DISP_SIGNAL_PIN, GPIO_DIRECTION_OUTPUT); /* Port direction: output */
    R_GPIO_PinWrite(EMWIN_DISP_SIGNAL_PIN, GPIO_LEVEL_HIGH); /* DISP on */
#endif

#if (EMWIN_USE_BACKLIGHT_PIN == 1)
    /* Switch backlight on, no PWM */
    R_GPIO_PinDirectionSet(EMWIN_BACKLIGHT_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(EMWIN_BACKLIGHT_PIN, GPIO_LEVEL_HIGH);
#endif

#if (EMWIN_INIT_DMAC == 1)
    /* Init DMA */
    R_DMACA_Init();
#endif

    /* Extended Bus Master Priority Control Register */
    BSC.EBMAPCR.BIT.PR1SEL = 3;
    BSC.EBMAPCR.BIT.PR2SEL = 1;
    BSC.EBMAPCR.BIT.PR3SEL = 2;
    BSC.EBMAPCR.BIT.PR4SEL = 0;
    BSC.EBMAPCR.BIT.PR5SEL = 4;
}
/**********************************************************************************************************************
 * End of function init_controller
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: set_lut_entry
 * Description  : Should set the desired LUT entry.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void set_lut_entry(LCD_COLOR color, uint8_t pos)
{
#if (EMWIN_BITS_PER_PIXEL < 16)
    e_emwin_rx_err_t ret;

    s_a_clut[pos] = color;

    if (pos == (NUM_COLORS - 1))
    {
        ret = r_emwin_rx_lcd_clut_update(s_a_clut);
        if (EMWIN_RX_SUCCESS != ret)
        {
            /* WAIT_LOOP */
            while (1)
            {
                R_BSP_NOP(); /* Error */
            }
        }
    }
#else
    GUI_USE_PARA(color);
    GUI_USE_PARA(pos);
#endif
}
/**********************************************************************************************************************
 * End of function set_lut_entry
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: display_on_off
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void display_on_off(int32_t on_off)
{
    e_emwin_rx_lcd_switch_t lcd_switch;
#if (EMWIN_USE_BACKLIGHT_PIN == 1)
    gpio_level_t            gpio_level;
#endif

    if (on_off)
    {
        lcd_switch = EMWIN_RX_LCD_SWITCH_ON;
#if (EMWIN_USE_BACKLIGHT_PIN == 1)
        gpio_level = GPIO_LEVEL_HIGH;
#endif
    }
    else
    {
        lcd_switch = EMWIN_RX_LCD_SWITCH_OFF;
#if (EMWIN_USE_BACKLIGHT_PIN == 1)
        gpio_level = GPIO_LEVEL_LOW;
#endif
    }

    r_emwin_rx_lcd_switch(lcd_switch);

#if (EMWIN_USE_BACKLIGHT_PIN == 1)
    R_GPIO_PinWrite(EMWIN_BACKLIGHT_PIN, gpio_level);
#endif
}
/**********************************************************************************************************************
 * End of function display_on_off
 *********************************************************************************************************************/

#if (USE_DAVE2D == 1)
/**********************************************************************************************************************
 * Function Name: get_d2_mode
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static uint32_t get_d2_mode(void)
{
    uint32_t r;
#if   (EMWIN_BITS_PER_PIXEL == 32)
    r = d2_mode_argb8888;
#elif (EMWIN_BITS_PER_PIXEL == 16)
    r = d2_mode_rgb565;
#elif (EMWIN_BITS_PER_PIXEL == 8)
    r = d2_mode_i8;
#elif (EMWIN_BITS_PER_PIXEL == 4)
    r = d2_mode_i4;
#elif (EMWIN_BITS_PER_PIXEL == 1)
    r = d2_mode_i1;
#endif
    return r;
}
/**********************************************************************************************************************
 * End of function get_d2_mode
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: modify_coord
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void modify_coord(int32_t * p_x, int32_t * p_y)
{
    int32_t x_temp;
    int32_t y_temp;

    if (ORIENTATION_CW == s_lcd_current_orientation)
    {
        x_temp = *p_x;
        *p_x = EMWIN_XSIZE_PHYS - 1 - *p_y;
        *p_y = x_temp;
    }
    else if (ORIENTATION_180 == s_lcd_current_orientation)
    {
        *p_x = EMWIN_XSIZE_PHYS - 1 - *p_x;
        *p_y = EMWIN_YSIZE_PHYS - 1 - *p_y;
    }
    else if (ORIENTATION_CCW == s_lcd_current_orientation)
    {
        y_temp = *p_y;
        *p_y = EMWIN_YSIZE_PHYS - 1 - *p_x;
        *p_x = y_temp;
    }
    else
    {
        R_BSP_NOP();
    }

}
/**********************************************************************************************************************
 * End of function modify_coord
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: modify_rect
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void modify_rect(const GUI_RECT * p_rect_org, GUI_RECT * p_rect)
{
    int32_t temp;

    if (ORIENTATION_CW == s_lcd_current_orientation)
    {
        p_rect->x0 = EMWIN_XSIZE_PHYS - 1 - p_rect_org->y0;
        p_rect->y0 = p_rect_org->x0;
        p_rect->x1 = EMWIN_XSIZE_PHYS - 1 - p_rect_org->y1;
        p_rect->y1 = p_rect_org->x1;
        if (p_rect->x0 > p_rect->x1)
        {
            temp = p_rect->x0;
            p_rect->x0 = p_rect->x1;
            p_rect->x1 = temp;
        }

    }
    else if (ORIENTATION_180 == s_lcd_current_orientation)
    {
        p_rect->x0 = EMWIN_XSIZE_PHYS - 1 - p_rect_org->x0;
        p_rect->y0 = EMWIN_YSIZE_PHYS - 1 - p_rect_org->y0;
        p_rect->x1 = EMWIN_XSIZE_PHYS - 1 - p_rect_org->x1;
        p_rect->y1 = EMWIN_YSIZE_PHYS - 1 - p_rect_org->y1;
        if (p_rect->x0 > p_rect->x1)
        {
            temp = p_rect->x0;
            p_rect->x0 = p_rect->x1;
            p_rect->x1 = temp;
        }
        if (p_rect->y0 > p_rect->y1)
        {
            temp = p_rect->y0;
            p_rect->y0 = p_rect->y1;
            p_rect->y1 = temp;
        }
    }
    else if (ORIENTATION_CCW == s_lcd_current_orientation)
    {
        p_rect->x0 = p_rect_org->y0;
        p_rect->y0 = EMWIN_YSIZE_PHYS - 1 - p_rect_org->x0;
        p_rect->x1 = p_rect_org->y1;
        p_rect->y1 = EMWIN_YSIZE_PHYS - 1 - p_rect_org->x1;
        if (p_rect->y0 > p_rect->y1)
        {
            temp = p_rect->y0;
            p_rect->y0 = p_rect->y1;
            p_rect->y1 = temp;
        }
    }
    else
    {
        *p_rect = *p_rect_org;
    }
}
/**********************************************************************************************************************
 * End of function modify_rect
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: lcd_fill_rect_xor
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void lcd_fill_rect_xor(int32_t layer_index, int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{
    int32_t temp;

    LCD_SetDevFunc(layer_index, LCD_DEVFUNC_FILLRECT, NULL);

    /* The following is required because emWin already mirrors/swaps the coordinates.
     * Since we call the default function we have to revert the mirroring/swapping. */

    if (ORIENTATION_CW == s_lcd_current_orientation)
    {
        temp = x0;
        x0 = EMWIN_XSIZE_PHYS - 1 - x1;
        x1 = EMWIN_XSIZE_PHYS - 1 - temp;
        temp = y0;
        y0 = x0;
        x0 = temp;
        temp = y1;
        y1 = x1;
        x1 = temp;
        LCD_FillRect(x0, y0, x1, y1);

    }
    else if (ORIENTATION_180 == s_lcd_current_orientation)
    {
        x0 = EMWIN_XSIZE_PHYS - 1 - x0;
        x1 = EMWIN_XSIZE_PHYS - 1 - x1;
        y0 = EMWIN_YSIZE_PHYS - 1 - y0;
        y1 = EMWIN_YSIZE_PHYS - 1 - y1;
        if (x0 > x1)
        {
            temp = x0;
            x0 = x1;
            x1 = temp;
        }
        if (y0 > y1)
        {
            temp = y0;
            y0 = y1;
            y1 = temp;
        }
        LCD_FillRect(x0, y0, x1, y1);

    }
    else if (ORIENTATION_CCW == s_lcd_current_orientation)
    {
        temp = y1;
        y1 = (EMWIN_YSIZE_PHYS - 1 - y0);
        y0 = (EMWIN_YSIZE_PHYS - 1 - temp);
        LCD_FillRect(y0, x0, y1, x1);

    }
    else
    {
        LCD_FillRect(x0, y0, x1, y1);
    }

    LCD_SetDevFunc(layer_index, LCD_DEVFUNC_FILLRECT, (void (*)(void)) lcd_fill_rect);
}
/**********************************************************************************************************************
 * End of function lcd_fill_rect_xor
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: lcd_fill_rect
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void lcd_fill_rect(int32_t layer_index, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t pixel_index)
{
    uint32_t     mode;
    d2_color     color;
    GUI_RECT     rect;
    int32_t      x_size;
    int32_t      y_size;
    uint8_t      alpha;
    GUI_DRAWMODE draw_mode;

    draw_mode = GUI_GetDrawMode();
    if (GUI_DM_XOR == draw_mode)
    {
        lcd_fill_rect_xor(layer_index, x0, y0, x1, y1);
    }
    else
    {
        GUI_USE_PARA(layer_index);
        mode = get_d2_mode();

        /* Generate render operations */
        d2_framebuffer(sp_d2_handle, (void *)s_a_buffer_ptr[s_write_buffer_index],
                        EMWIN_XSIZE_PHYS, EMWIN_XSIZE_PHYS, EMWIN_YSIZE_PHYS, mode);
        d2_selectrenderbuffer(sp_d2_handle, sp_renderbuffer);
        color = GUI_Index2Color(pixel_index);
        d2_setcolor(sp_d2_handle, 0, color);
        alpha = GUI_GetAlpha();
        d2_setalpha(sp_d2_handle, alpha);
        modify_rect(GUI_GetClipRect(), &rect);
        d2_cliprect(sp_d2_handle, rect.x0, rect.y0, rect.x1, rect.y1);
        x_size = (x1 - x0) + 1;
        y_size = (y1 - y0) + 1;
        d2_renderbox(sp_d2_handle, x0 * 16, y0 * 16, x_size * 16, y_size * 16);

        /* Execute render operations */
        d2_executerenderbuffer(sp_d2_handle, sp_renderbuffer, 0);
        d2_flushframe(sp_d2_handle);
        alpha = 0xFF;
        d2_setalpha(sp_d2_handle, alpha);
    }
}
/**********************************************************************************************************************
 * End of function lcd_fill_rect
 *********************************************************************************************************************/

#if 0 /* Usage restriction */
/**********************************************************************************************************************
 * Function Name: circle_aa
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static int32_t circle_aa(int32_t x0, int32_t y0, int32_t r, int32_t w)
{
    uint32_t mode;
    int32_t  ret;
    GUI_RECT rect;
    uint32_t color;

    mode = get_d2_mode();

    /* Generate render operations */
    d2_framebuffer(sp_d2_handle, (void *)s_a_buffer_ptr[s_write_buffer_index],
                    EMWIN_XSIZE_PHYS, EMWIN_XSIZE_PHYS, EMWIN_YSIZE_PHYS, mode);
    d2_selectrenderbuffer(sp_d2_handle, sp_renderbuffer);
    color = GUI_Index2Color(GUI_GetColorIndex());
    d2_setcolor(sp_d2_handle, 0, color);
    modify_rect(GUI__GetClipRect(), &rect);
    d2_cliprect(sp_d2_handle, rect.x0, rect.y0, rect.x1, rect.y1);
    ret = d2_rendercircle(sp_d2_handle, x0 * 16, y0 * 16, r * 16, w * 16);

    /* Execute render operations */
    d2_executerenderbuffer(sp_d2_handle, sp_renderbuffer, 0);
    d2_flushframe(sp_d2_handle);

    return ret;
}
/**********************************************************************************************************************
 * End of function circle_aa
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: fill_circle_aa
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static int32_t fill_circle_aa(int32_t x0, int32_t y0, int32_t r)
{
    modify_coord(&x0, &y0);
    return (circle_aa(x0, y0, r, 0));
}
/**********************************************************************************************************************
 * End of function fill_circle_aa
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: draw_circle_aa
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static int32_t draw_circle_aa(int32_t x0, int32_t y0, int32_t r)
{
    modify_coord(&x0, &y0);
    return (circle_aa(x0, y0, r, GUI_GetPenSize()));
}
/**********************************************************************************************************************
 * End of function draw_circle_aa
 *********************************************************************************************************************/
#endif

/**********************************************************************************************************************
 * Function Name: draw_line_aa
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static int32_t draw_line_aa(int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{
    uint32_t mode;
    int32_t  ret;
    uint32_t color;
    GUI_RECT rect;

    mode = get_d2_mode();
    modify_coord(&x0, &y0);
    modify_coord(&x1, &y1);

    /* Generate render operations */
    d2_framebuffer(sp_d2_handle, (void *)s_a_buffer_ptr[s_write_buffer_index],
                    EMWIN_XSIZE_PHYS, EMWIN_XSIZE_PHYS, EMWIN_YSIZE_PHYS, mode);
    d2_selectrenderbuffer(sp_d2_handle, sp_renderbuffer);
    color = GUI_Index2Color(GUI_GetColorIndex());
    d2_setcolor(sp_d2_handle, 0, color);
    modify_rect(GUI__GetClipRect(), &rect);
    d2_cliprect(sp_d2_handle, rect.x0, rect.y0, rect.x1, rect.y1);
    ret = d2_renderline(sp_d2_handle, x0 * 16, y0 * 16, x1 * 16, y1 * 16, GUI_GetPenSize() * 16, d2_le_exclude_none);

    /* Execute render operations */
    d2_executerenderbuffer(sp_d2_handle, sp_renderbuffer, 0);
    d2_flushframe(sp_d2_handle);

    return ret;
}
/**********************************************************************************************************************
 * End of function draw_line_aa
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: draw_memdev_alpha
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void draw_memdev_alpha(void * p_dst, const void * p_src, int32_t x_size, int32_t y_size,
                                int32_t bytes_per_line_dst, int32_t bytes_per_line_src)
{
    uint32_t pitch_src;
    uint32_t pitch_dst;
    uint32_t mode;

    pitch_dst = bytes_per_line_dst / 4;
    pitch_src = bytes_per_line_src / 4;
    mode      = get_d2_mode();

    /* Set address of destination memory device as frame buffer to be used */
    d2_framebuffer(sp_d2_handle, p_dst, pitch_dst, pitch_dst, y_size, d2_mode_argb8888);

    /* Generate render operations */
    d2_selectrenderbuffer(sp_d2_handle, sp_renderbuffer);
    d2_setblitsrc(sp_d2_handle, (void *)p_src, pitch_src, x_size, y_size, d2_mode_argb8888);
    d2_blitcopy(sp_d2_handle, x_size, y_size, 0, 0, x_size * 16, y_size * 16, 0, 0, d2_bf_usealpha);

    /* Execute render operations */
    d2_executerenderbuffer(sp_d2_handle, sp_renderbuffer, 0);
    d2_flushframe(sp_d2_handle);

    /* Restore frame buffer */
    d2_framebuffer(sp_d2_handle, (void *)s_a_buffer_ptr[s_write_buffer_index], EMWIN_XSIZE_PHYS, EMWIN_XSIZE_PHYS, EMWIN_YSIZE_PHYS, mode);
}
/**********************************************************************************************************************
 * End of function draw_memdev_alpha
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: draw_bitmap_alpha
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void draw_bitmap_alpha(int32_t layer_index, int32_t x, int32_t y, const void * p, int32_t x_size, int32_t y_size,
                                int32_t bytes_per_line)
{
    uint32_t pitch;
    uint32_t mode;

    pitch = bytes_per_line / 4;
    mode  = get_d2_mode();

    /* Generate render operations */
    d2_framebuffer(sp_d2_handle, (void *)s_a_buffer_ptr[s_write_buffer_index],
                    EMWIN_XSIZE_PHYS, EMWIN_XSIZE_PHYS, EMWIN_YSIZE_PHYS, mode);
    d2_selectrenderbuffer(sp_d2_handle, sp_renderbuffer);
    d2_setblitsrc(sp_d2_handle, (void *)p, pitch, x_size, y_size, d2_mode_argb8888);
    d2_blitcopy(sp_d2_handle, x_size, y_size, 0, 0, x_size * 16, y_size * 16, x * 16, y * 16, d2_bf_usealpha);

    /* Execute render operations */
    d2_executerenderbuffer(sp_d2_handle, sp_renderbuffer, 0);
    d2_flushframe(sp_d2_handle);
}
/**********************************************************************************************************************
 * End of function draw_bitmap_alpha
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: draw_bitmap_16bpp
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void draw_bitmap_16bpp(int32_t layer_index, int32_t x, int32_t y, const void * p, int32_t x_size, int32_t y_size,
                                int32_t bytes_per_line)
{
    uint32_t mode;
    uint32_t mode_src;

    GUI_USE_PARA(layer_index);
    GUI_USE_PARA(bytes_per_line);
    mode     = get_d2_mode();
    mode_src = d2_mode_rgb565;

    /* Generate render operations */
    d2_framebuffer(sp_d2_handle, (void *)s_a_buffer_ptr[s_write_buffer_index],
                    EMWIN_XSIZE_PHYS, EMWIN_XSIZE_PHYS, EMWIN_YSIZE_PHYS, mode);
    d2_selectrenderbuffer(sp_d2_handle, sp_renderbuffer);
    d2_setblitsrc(sp_d2_handle, (void *)p, bytes_per_line / 2, x_size, y_size, mode_src);
    d2_blitcopy(sp_d2_handle, x_size, y_size, 0, 0, x_size * 16, y_size * 16, x * 16, y * 16, 0);

    /* Execute render operations */
    d2_executerenderbuffer(sp_d2_handle, sp_renderbuffer, 0);
    d2_flushframe(sp_d2_handle);
}
/**********************************************************************************************************************
 * End of function draw_bitmap_16bpp
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: fill_polygon_aa
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static int32_t fill_polygon_aa(const GUI_POINT * p_points, int32_t num_points, int32_t x0, int32_t y0)
{
    uint32_t   mode;
    d2_point * p_data;
    d2_point * p_data_i;
    int32_t    i;
    int32_t    ret;
    GUI_RECT   rect;
    uint32_t   color;

    mode   = get_d2_mode();
    p_data = malloc(sizeof(d2_point) * num_points * 2);
    ret    = 1;
    modify_coord(&x0, &y0);
    if (p_data)
    {
        p_data_i = p_data;

        /* WAIT_LOOP */
        for (i = 0; i < num_points; i++ )
        {
            modify_coord((int32_t *)&p_points->x, (int32_t *)&p_points->y);
            *p_data_i++ = (d2_point)(p_points->x + x0) * 16;
            *p_data_i++ = (d2_point)(p_points->y + y0) * 16;
            p_points++;
        }

        /* Generate render operations */
        d2_framebuffer(sp_d2_handle, (void *)s_a_buffer_ptr[s_write_buffer_index],
                        EMWIN_XSIZE_PHYS, EMWIN_XSIZE_PHYS, EMWIN_YSIZE_PHYS, mode);
        d2_selectrenderbuffer(sp_d2_handle, sp_renderbuffer);
        color = GUI_Index2Color(GUI_GetColorIndex());
        d2_setcolor(sp_d2_handle, 0, color);
        modify_rect(GUI__GetClipRect(), &rect);
        d2_cliprect(sp_d2_handle, rect.x0, rect.y0, rect.x1, rect.y1);
        ret = d2_renderpolygon(sp_d2_handle, p_data, num_points, d2_le_closed);

        /* Execute render operations */
        d2_executerenderbuffer(sp_d2_handle, sp_renderbuffer, 0);
        d2_flushframe(sp_d2_handle);
        free(p_data);
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function fill_polygon_aa
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: draw_poly_outline_aa
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static int32_t draw_poly_outline_aa(const GUI_POINT * p_points, int32_t num_points, int32_t Thickness,
                                    int32_t x, int32_t y)
{
    uint32_t   mode;
    d2_point * p_data;
    d2_point * p_data_i;
    int32_t    i;
    int32_t    ret;
    GUI_RECT   rect;
    uint32_t   color;

    mode   = get_d2_mode();
    p_data = malloc(sizeof(d2_point) * num_points * 2);
    ret    = 1;
    modify_coord(&x, &y);
    if (p_data)
    {
        p_data_i = p_data;

        /* WAIT_LOOP */
        for (i = 0; i < num_points; i++ )
        {
            modify_coord((int32_t *)&p_points->x, (int32_t *)&p_points->y);
            *p_data_i++ = (d2_point)(p_points->x + x) * 16;
            *p_data_i++ = (d2_point)(p_points->y + y) * 16;
            p_points++;
        }

        /* Generate render operations */
        d2_framebuffer(sp_d2_handle, (void *)s_a_buffer_ptr[s_write_buffer_index],
                        EMWIN_XSIZE_PHYS, EMWIN_XSIZE_PHYS, EMWIN_YSIZE_PHYS, mode);
        d2_selectrenderbuffer(sp_d2_handle, sp_renderbuffer);
        color = GUI_Index2Color(GUI_GetColorIndex());
        d2_setcolor(sp_d2_handle, 0, color);
        modify_rect(GUI__GetClipRect(), &rect);
        d2_cliprect(sp_d2_handle, rect.x0, rect.y0, rect.x1, rect.y1);
        d2_selectrendermode(sp_d2_handle, d2_rm_outline);
        ret = d2_renderpolyline(sp_d2_handle, p_data, num_points, Thickness * 16, d2_le_closed);
        d2_selectrendermode(sp_d2_handle, d2_rm_solid);

        /* Execute render operations */
        d2_executerenderbuffer(sp_d2_handle, sp_renderbuffer, 0);
        d2_flushframe(sp_d2_handle);
        free(p_data);
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function draw_poly_outline_aa
 *********************************************************************************************************************/

#if 0 /* Usage restriction */
/**********************************************************************************************************************
 * Function Name: draw_arc_aa
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static int32_t draw_arc_aa(int32_t x0, int32_t y0, int32_t rx, int32_t ry, int32_t a0, int32_t a1)
{
    uint32_t mode;
    int32_t  ret;
    int32_t  nx0;
    int32_t  ny0;
    int32_t  nx1;
    int32_t  ny1;
    GUI_RECT rect;
    uint32_t color;
    uint32_t flag;

    GUI_USE_PARA(ry);

    /* If both angles are equal (e.g. 0 and 0 or 180 and 180) nothing has to be done */
    if (a0 == a1)
    {
        return 0; /* Nothing to do, no angle - no arc */
    }
    if (a1 < a0)
    {
        return 0; /* Nothing to do, emWin doesn't support this one */
    }

    /* If the angles not equal, but meet at the same position
     * we don't draw an arc but a circle instead. */
    if (a1 > (a0 + 360000))
    {
        ret = draw_circle_aa(x0, y0, rx); /* a1 meets a0 after one round so we have a circle */

        return ret;
    }
    if ((a0 % 360000) == (a1 % 360000))
    {
        ret = draw_circle_aa(x0, y0, rx); /* Both angles are at the same position but not equal, so we have a circle */

        return ret;
    }

    mode = get_d2_mode();
    modify_coord(&x0, &y0);
    nx0  = -GUI__SinHQ(a0);
    ny0  = -GUI__CosHQ(a0);
    nx1  = GUI__SinHQ(a1);
    ny1  = GUI__CosHQ(a1);

    /* If the difference between both is larger than 180 degrees we must use the concave flag */
    if (((a1 - a0) % 360000) <= 180000)
    {
        flag = 0;
    }
    else
    {
        flag = d2_wf_concave;
    }

    /* Generate render operations */
    d2_framebuffer(sp_d2_handle, (void *)s_a_buffer_ptr[s_write_buffer_index],
                    EMWIN_XSIZE_PHYS, EMWIN_XSIZE_PHYS, EMWIN_YSIZE_PHYS, mode);
    d2_selectrenderbuffer(sp_d2_handle, sp_renderbuffer);
    color = GUI_Index2Color(GUI_GetColorIndex());
    d2_setcolor(sp_d2_handle, 0, color);
    modify_rect(GUI__GetClipRect(), &rect);
    d2_cliprect(sp_d2_handle, rect.x0, rect.y0, rect.x1, rect.y1);
    ret = d2_renderwedge(sp_d2_handle, x0 * 16, y0 * 16, rx * 16, GUI_GetPenSize() * 16, nx0, ny0, nx1, ny1, flag);

    /* Execute render operations */
    d2_executerenderbuffer(sp_d2_handle, sp_renderbuffer, 0);
    d2_flushframe(sp_d2_handle);

    return ret;
}
/**********************************************************************************************************************
 * End of function draw_arc_aa
 *********************************************************************************************************************/
#endif

/**********************************************************************************************************************
 * Function Name: R_EMWIN_GetD2
 * Description  : Get Dave2D handle.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
d2_device * R_EMWIN_GetD2(void)
{
    return sp_d2_handle;
}
/**********************************************************************************************************************
 * End of function R_EMWIN_GetD2
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_EMWIN_EnableDave2D
 * Description  : Enable Dave2D functions.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void R_EMWIN_EnableDave2D(void)
{
    if (ORIENTATION_0 == s_lcd_current_orientation)
    {
        /* Only available in ORIENTATION_0 */
        GUI_SetFuncDrawAlpha         ((GUI_DRAWMEMDEV_FUNC*) draw_memdev_alpha, (GUI_DRAWBITMAP_FUNC*) draw_bitmap_alpha);
        GUI_SetFuncDrawM565          ((GUI_DRAWMEMDEV_FUNC*) draw_memdev_alpha, (GUI_DRAWBITMAP_FUNC*) draw_bitmap_alpha);
        LCD_SetDevFunc               (0, LCD_DEVFUNC_DRAWBMP_16BPP, (void (*)(void)) draw_bitmap_16bpp);
        GUI_AA_SetFuncDrawPolyOutline((DRAW_POLY_OUTLOINE*) draw_poly_outline_aa);
/*      GUI_AA_SetFuncDrawArc    ((DRAW_ARC *)draw_arc_aa); *//* Usage restriction */
        GUI_AA_SetFuncFillPolygon    ((FILL_POLYGON*) fill_polygon_aa);
    }
    else
    {
        GUI_SetFuncDrawAlpha         (NULL, NULL);
        GUI_SetFuncDrawM565          (NULL, NULL);
        LCD_SetDevFunc               (0, LCD_DEVFUNC_DRAWBMP_16BPP, (void (*)(void)) NULL);
        GUI_AA_SetFuncDrawPolyOutline(NULL);
        GUI_AA_SetFuncDrawArc        (NULL);
        GUI_AA_SetFuncFillPolygon    (NULL);
    }

    /* Usable in any orientation */
/*  GUI_AA_SetFuncFillCircle     ((FILL_CIRCLE *)fill_circle_aa); *//* Usage restriction */
/*  GUI_AA_SetFuncDrawCircle     ((DRAW_CIRCLE *)draw_circle_aa); *//* Usage restriction */
    GUI_AA_SetFuncDrawLine       ((DRAW_LINE *)draw_line_aa);
    LCD_SetDevFunc               (0, LCD_DEVFUNC_FILLRECT, (void(*)(void))lcd_fill_rect);
    GUI_AlphaEnableFillRectHW    (1);
    s_dave_active = 1;
}
/**********************************************************************************************************************
 * End of function R_EMWIN_EnableDave2D
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_EMWIN_DisableDave2D
 * Description  : Disable Dave2D functions.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void R_EMWIN_DisableDave2D(void)
{
    /* Only available in ORIENTATION_0 */
    GUI_SetFuncDrawAlpha         (NULL, NULL);
    GUI_SetFuncDrawM565          (NULL, NULL);
    LCD_SetDevFunc               (0, LCD_DEVFUNC_DRAWBMP_16BPP, (void(*)(void))NULL);
    GUI_AA_SetFuncDrawPolyOutline(NULL);
    GUI_AA_SetFuncDrawArc        (NULL);
    GUI_AA_SetFuncFillPolygon    (NULL);

    /* Usable in any orientation */
    GUI_AA_SetFuncFillCircle     (NULL);
    GUI_AA_SetFuncDrawCircle     (NULL);
    GUI_AA_SetFuncDrawLine       (NULL);
    LCD_SetDevFunc               (0, LCD_DEVFUNC_FILLRECT, (void(*)(void))NULL);
    GUI_AlphaEnableFillRectHW    (0);
    s_dave_active = 0;
}
/**********************************************************************************************************************
 * End of function R_EMWIN_DisableDave2D
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_EMWIN_GetDaveActive
 * Description  : Get status of Dave2D functions.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
uint32_t R_EMWIN_GetDaveActive(void)
{
    return s_dave_active;
}
/**********************************************************************************************************************
 * End of function R_EMWIN_GetDaveActive
 *********************************************************************************************************************/
#endif /* USE_DAVE2D == 1 */

/**********************************************************************************************************************
 * Function Name: copy_buffer
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void copy_buffer(int32_t layer_index, int32_t index_src, int32_t index_dst)
{
    dmaca_return_t            ret;
    dmaca_transfer_data_cfg_t td_cfg;
    dmaca_stat_t              dmac_status;

    GUI_USE_PARA(layer_index);
    ret = R_DMACA_Open(EMWIN_DMAC_NUMBER);
    if (DMACA_SUCCESS != ret)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* Error */
        }
    }

    td_cfg.transfer_mode        = DMACA_TRANSFER_MODE_BLOCK;
    td_cfg.repeat_block_side    = DMACA_REPEAT_BLOCK_DISABLE;
    td_cfg.data_size            = DMACA_DATA_SIZE_LWORD;
    td_cfg.act_source           = (dmaca_activation_source_t)0;
    td_cfg.request_source       = DMACA_TRANSFER_REQUEST_SOFTWARE;
    td_cfg.dtie_request         = DMACA_TRANSFER_END_INTERRUPT_DISABLE;
    td_cfg.esie_request         = DMACA_TRANSFER_ESCAPE_END_INTERRUPT_DISABLE;
    td_cfg.rptie_request        = DMACA_REPEAT_SIZE_END_INTERRUPT_DISABLE;
    td_cfg.sarie_request        = DMACA_SRC_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    td_cfg.darie_request        = DMACA_DES_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    td_cfg.src_addr_mode        = DMACA_SRC_ADDR_INCR;
    td_cfg.src_addr_repeat_area = DMACA_SRC_ADDR_EXT_REP_AREA_NONE;
    td_cfg.des_addr_mode        = DMACA_DES_ADDR_INCR;
    td_cfg.des_addr_repeat_area = DMACA_DES_ADDR_EXT_REP_AREA_NONE;
    td_cfg.offset_value         = LINE_OFFSET;
    td_cfg.interrupt_sel        = DMACA_CLEAR_INTERRUPT_FLAG_BEGINNING_TRANSFER;
    td_cfg.p_src_addr           = (void *)s_a_buffer_ptr[index_src];
    td_cfg.p_des_addr           = (void *)s_a_buffer_ptr[index_dst];
    td_cfg.transfer_count       = EMWIN_YSIZE_PHYS;
    td_cfg.block_size           = LINE_OFFSET >> 2;

    ret = R_DMACA_Create(EMWIN_DMAC_NUMBER, &td_cfg);
    if (DMACA_SUCCESS != ret)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* Error */
        }
    }

    ret = R_DMACA_Control(EMWIN_DMAC_NUMBER, DMACA_CMD_ENABLE, &dmac_status);
    if (DMACA_SUCCESS != ret)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* Error */
        }
    }

    ret = R_DMACA_Control(EMWIN_DMAC_NUMBER, DMACA_CMD_SOFT_REQ_NOT_CLR_REQ, &dmac_status);
    if (DMACA_SUCCESS != ret)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* Error */
        }
    }

    do
    {
        ret = R_DMACA_Control(EMWIN_DMAC_NUMBER, DMACA_CMD_STATUS_GET, &dmac_status);
        if (DMACA_SUCCESS != ret)
        {
            /* WAIT_LOOP */
            while (1)
            {
                R_BSP_NOP(); /* Error */
            }
        }
    } while (0 == (dmac_status.dtif_stat)); /* WAIT_LOOP */

    ret = R_DMACA_Close(EMWIN_DMAC_NUMBER);
    if ((DMACA_SUCCESS != ret) && (DMACA_SUCCESS_OTHER_CH_BUSY != ret))
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* Error */
        }
    }

    s_write_buffer_index = index_dst;
}
/**********************************************************************************************************************
 * End of function copy_buffer
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_EMWIN_GetBufferAddr
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void * R_EMWIN_GetBufferAddr(void)
{
    return (void *)s_a_buffer_ptr[s_write_buffer_index];
}
/**********************************************************************************************************************
 * End of function R_EMWIN_GetBufferAddr
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_emwin_rx_lcd_open
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static e_emwin_rx_err_t r_emwin_rx_lcd_open(void)
{
    e_emwin_rx_err_t emwin_ret = EMWIN_RX_FAIL;

    glcdc_cfg_t     glcdc_cfg;
    glcdc_err_t     glcdc_ret;

#if (!defined(QE_DISPLAY_CONFIGURATION) && (GLCDC_CFG_CONFIGURATION_MODE == 0))
    /* Configuration example for RX72N envision kit. */

    /* Set the frequency of the LCD_CLK and PXCLK to suit the format and set the PANELCLK.CLKEN bit to 1 */
    glcdc_cfg.output.clksrc          = GLCDC_CLK_SRC_INTERNAL;              /* Select PLL clock */
    glcdc_cfg.output.clock_div_ratio = GLCDC_PANEL_CLK_DIVISOR_24; /* 240 / 24 = 10 MHz */

    /* Definition of Background Screen */
    glcdc_cfg.output.htiming.front_porch = 5;
    glcdc_cfg.output.vtiming.front_porch = 8;
    glcdc_cfg.output.htiming.back_porch  = 40;
    glcdc_cfg.output.vtiming.back_porch  = 8;
    glcdc_cfg.output.htiming.display_cyc = EMWIN_XSIZE_PHYS;
    glcdc_cfg.output.vtiming.display_cyc = EMWIN_YSIZE_PHYS;
    glcdc_cfg.output.htiming.sync_width  = 1;
    glcdc_cfg.output.vtiming.sync_width  = 1;

    /* Graphic 1 configuration */
    glcdc_cfg.input[GLCDC_FRAME_LAYER_1].p_base = NULL;

    /* Graphic 2 configuration */
    glcdc_cfg.input[GLCDC_FRAME_LAYER_2].p_base             = (uint32_t *)EMWIN_GUI_FRAME_BUFFER1;
    glcdc_cfg.input[GLCDC_FRAME_LAYER_2].offset             = LINE_OFFSET;
    glcdc_cfg.input[GLCDC_FRAME_LAYER_2].format             = COLOR_FORMAT;
    glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].visible            = true;
    glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].blend_control      = GLCDC_BLEND_CONTROL_NONE;
    glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].frame_edge         = false;
    glcdc_cfg.input[GLCDC_FRAME_LAYER_2].frame_edge         = false;
    glcdc_cfg.input[GLCDC_FRAME_LAYER_2].coordinate.y       = 0;
    glcdc_cfg.input[GLCDC_FRAME_LAYER_2].vsize              = EMWIN_YSIZE_PHYS;
    glcdc_cfg.input[GLCDC_FRAME_LAYER_2].coordinate.x       = 0;
    glcdc_cfg.input[GLCDC_FRAME_LAYER_2].hsize              = EMWIN_XSIZE_PHYS;
    glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].start_coordinate.x = 0;
    glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].end_coordinate.x   = EMWIN_YSIZE_PHYS;
    glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].start_coordinate.y = 0;
    glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].end_coordinate.y   = EMWIN_XSIZE_PHYS;

    /* Timing configuration */
    glcdc_cfg.output.tcon_vsync           = GLCDC_TCON_PIN_0;
    glcdc_cfg.output.tcon_hsync           = GLCDC_TCON_PIN_2;
    glcdc_cfg.output.tcon_de              = GLCDC_TCON_PIN_3;
    glcdc_cfg.output.data_enable_polarity = GLCDC_SIGNAL_POLARITY_HIACTIVE;
    glcdc_cfg.output.hsync_polarity       = GLCDC_SIGNAL_POLARITY_LOACTIVE;
    glcdc_cfg.output.vsync_polarity       = GLCDC_SIGNAL_POLARITY_LOACTIVE;
    glcdc_cfg.output.sync_edge            = GLCDC_SIGNAL_SYNC_EDGE_RISING;

    /* Output interface */
    glcdc_cfg.output.format      = GLCDC_OUT_FORMAT_16BITS_RGB565;
    glcdc_cfg.output.color_order = GLCDC_COLOR_ORDER_RGB;
    glcdc_cfg.output.endian      = GLCDC_ENDIAN_LITTLE;

    /* Brightness Adjustment */
    glcdc_cfg.output.brightness.b = 0x200;
    glcdc_cfg.output.brightness.g = 0x200;
    glcdc_cfg.output.brightness.r = 0x200;

    /* Contrast Adjustment Value */
    glcdc_cfg.output.contrast.b = 0x80;
    glcdc_cfg.output.contrast.g = 0x80;
    glcdc_cfg.output.contrast.r = 0x80;

    /* Disable Gamma */
    glcdc_cfg.output.gamma.enable = false;

    /* Disable Chromakey */
    glcdc_cfg.chromakey[GLCDC_FRAME_LAYER_2].enable = false;

    /* Disable Dithering */
    glcdc_cfg.output.dithering.dithering_on = false;

    /* CLUT Adjustment Value */
    glcdc_cfg.clut[GLCDC_FRAME_LAYER_2].enable = false;

    /* Enable VPOS ISR */
    glcdc_cfg.detection.vpos_detect  = true;
    glcdc_cfg.interrupt.vpos_enable  = true;
    glcdc_cfg.detection.gr1uf_detect = false;
    glcdc_cfg.detection.gr2uf_detect = false;
    glcdc_cfg.interrupt.gr1uf_enable = false;
    glcdc_cfg.interrupt.gr2uf_enable = false;

    /* Set function to be called on VSYNC */
    glcdc_cfg.p_callback = (void (*)(void *))_VSYNC_ISR;

#endif /* (!defined(QE_DISPLAY_CONFIGURATION) && (GLCDC_CFG_CONFIGURATION_MODE == 0)) */

    /* Initialize a first time interrupt flag
     *   Unintended specified line notification from graphic 2 and graphic 1, 2 underflow is detected only
     *   for first time after release GLCDC software reset.
     *   This variable is a flag to skip the first time interrupt processing.
     *   Refer to Graphic LCD Controller (GLCDC) section of User's Manual: Hardware for details. */
    s_first_interrupt_flag = false;

    /* R_GLCDC_Open function release stop state of GLCDC. */
    glcdc_ret = R_GLCDC_Open(&glcdc_cfg);

    if (GLCDC_SUCCESS == glcdc_ret)
    {
        s_runtime_cfg.blend     = glcdc_cfg.blend[GLCDC_FRAME_LAYER_2];
        s_runtime_cfg.input     = glcdc_cfg.input[GLCDC_FRAME_LAYER_2];
        s_runtime_cfg.chromakey = glcdc_cfg.chromakey[GLCDC_FRAME_LAYER_2];

        /* Function select of multiplex pins (Display B) */
        R_GLCDC_PinSet();

        emwin_ret = EMWIN_RX_SUCCESS;
    }
    else
    {
        emwin_ret = EMWIN_RX_FAIL;
    }

    return emwin_ret;
}
/**********************************************************************************************************************
 * End of function r_emwin_rx_lcd_open
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_emwin_rx_lcd_switch_buffer
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static e_emwin_rx_err_t r_emwin_rx_lcd_switch_buffer(uint32_t * p_addr)
{
    e_emwin_rx_err_t emwin_ret = EMWIN_RX_FAIL;

    glcdc_err_t      glcdc_ret;

    s_pending_buffer = 1;

    /* WAIT_LOOP */
    while (1 == s_pending_buffer)
    {
        /* Wait until s_pending_buffer is cleared by ISR */
        R_BSP_NOP();
    }
    s_runtime_cfg.input.p_base = p_addr; /* Specify the start address of the frame buffer */

    /* Graphic 2 Register Value Reflection Enable */
    glcdc_ret = R_GLCDC_LayerChange(GLCDC_FRAME_LAYER_2, &s_runtime_cfg);

    if (GLCDC_SUCCESS == glcdc_ret)
    {
        emwin_ret = EMWIN_RX_SUCCESS;
    }
    else
    {
        emwin_ret = EMWIN_RX_FAIL;
    }

    return emwin_ret;
}
/**********************************************************************************************************************
 * End of function r_emwin_rx_lcd_switch_buffer
 *********************************************************************************************************************/

#if (EMWIN_BITS_PER_PIXEL < 16)
/**********************************************************************************************************************
 * Function Name: r_emwin_rx_lcd_clut_update
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static e_emwin_rx_err_t r_emwin_rx_lcd_clut_update(uint32_t * p_clut)
{
    e_emwin_rx_err_t    emwin_ret = EMWIN_RX_FAIL;

    glcdc_err_t         glcdc_ret;
    glcdc_clut_cfg_t    p_clut_cfg;

    p_clut_cfg.enable = true;
    p_clut_cfg.p_base = p_clut;
    p_clut_cfg.size   = NUM_COLORS;
    p_clut_cfg.start  = 0;
#ifdef __ICCRX__
    glcdc_ret = R_GLCDC_Control(GLCDC_CMD_START_DISPLAY, (void *)FIT_NO_FUNC);
#else
    glcdc_ret = R_GLCDC_Control(GLCDC_CMD_START_DISPLAY, FIT_NO_FUNC);
#endif
    GUI_Delay(100);
    if (GLCDC_SUCCESS == glcdc_ret)
    {
        glcdc_ret = R_GLCDC_ClutUpdate(GLCDC_FRAME_LAYER_2, &p_clut_cfg);
    }

    if (GLCDC_SUCCESS == glcdc_ret)
    {
        emwin_ret = EMWIN_RX_SUCCESS;
    }
    else
    {
        emwin_ret = EMWIN_RX_FAIL;
    }

    return emwin_ret;
}
/**********************************************************************************************************************
 * End of function r_emwin_rx_lcd_clut_update
 *********************************************************************************************************************/
#endif

/**********************************************************************************************************************
 * Function Name: r_emwin_rx_lcd_switch
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static e_emwin_rx_err_t r_emwin_rx_lcd_switch(e_emwin_rx_lcd_switch_t lcd_switch)
{
    e_emwin_rx_err_t    emwin_ret = EMWIN_RX_FAIL;

    glcdc_err_t         glcdc_ret;
    glcdc_control_cmd_t glcdc_cmd;

    if (EMWIN_RX_LCD_SWITCH_ON == lcd_switch)
    {
        glcdc_cmd = GLCDC_CMD_START_DISPLAY;
    }
    else /* EMWIN_RX_LCD_SWITCH_OFF == lcd_switch */
    {
        glcdc_cmd = GLCDC_CMD_STOP_DISPLAY;
    }

#ifdef __ICCRX__
    glcdc_ret = R_GLCDC_Control(glcdc_cmd, (void *)FIT_NO_FUNC);
#else
    glcdc_ret = R_GLCDC_Control(glcdc_cmd, FIT_NO_FUNC);
#endif
    if (GLCDC_SUCCESS == glcdc_ret)
    {
        emwin_ret = EMWIN_RX_SUCCESS;
    }
    else
    {
        emwin_ret = EMWIN_RX_FAIL;
    }

    return emwin_ret;
}
/**********************************************************************************************************************
 * End of function r_emwin_rx_lcd_switch
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: _VSYNC_ISR
 * Description  : Callback function of LCD interface.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void _VSYNC_ISR(void * p)
{
    if (false == s_first_interrupt_flag)
    {
        s_first_interrupt_flag = true;

        /* Do nothing */
    }
    else
    {
        glcdc_callback_args_t * p_decode;

        /* Casting glcdc_callback_args_t pointer is used to handle input. */
        p_decode = (glcdc_callback_args_t*) p;

        if (GLCDC_EVENT_GR1_UNDERFLOW == p_decode->event)
        {
            R_BSP_NOP(); /* GR1 underflow , if necessary */
        }
        else if (GLCDC_EVENT_GR2_UNDERFLOW == p_decode->event)
        {
            R_BSP_NOP(); /* GR2 underflow , if necessary */
        }
        else /* GLCDC_EVENT_LINE_DETECTION */
        {
            /* Sets a flag to be able to notice the interrupt */
            s_pending_buffer = 0;
        }
    }
}
/**********************************************************************************************************************
 * End of function _VSYNC_ISR
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: lcd_driver_setting
 * Description  : Called during configuration changes such as display rotation.
 * Arguments    : .
 * Return Value : .
 * Note         : This call back function is called from the emWin library.
 *********************************************************************************************************************/
void lcd_driver_setting(GUI_DEVICE * p_device, int rotateindex, int layerindex)
{
    int32_t swap_xy_ex;

    GUI_USE_PARA(p_device);
    GUI_USE_PARA(layerindex);

    /* Display driver configuration */
    swap_xy_ex = LCD_GetSwapXYEx(0);
    if (swap_xy_ex)
    {
        LCD_SetSizeEx(0, EMWIN_YSIZE_PHYS, EMWIN_XSIZE_PHYS);
        LCD_SetVSizeEx(0, EMWIN_YSIZE_PHYS, EMWIN_XSIZE_PHYS);
    }
    else
    {
        LCD_SetSizeEx(0, EMWIN_XSIZE_PHYS, EMWIN_YSIZE_PHYS);
        LCD_SetVSizeEx(0, VXSIZE_PHYS, EMWIN_YSIZE_PHYS);
    }
    LCD_SetBufferPtrEx(0, (void**) s_a_buffer_ptr);

    /* Set function pointers */
    LCD_SetDevFunc(0, LCD_DEVFUNC_COPYBUFFER, (void (*)(void)) copy_buffer);

#if (USE_DAVE2D == 1)
    /* Change current orientation */
    s_lcd_current_orientation = (rotateindex + EMWIN_DISPLAY_ORIENTATION) % 4;
#endif

}
/**********************************************************************************************************************
 * End of function lcd_driver_setting
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: LCD_X_Config
 * Description  : Called during the initialization process in order to set up the display driver configuration.
 * Arguments    : .
 * Return Value : .
 * Note         : This function is called from the emWin library.
 *********************************************************************************************************************/
void LCD_X_Config(void)
{
    GUI_DEVICE * p_device;
#if (USE_DAVE2D == 1)
    d2_s32  d2_ret;
#endif

#if (EMWIN_NUM_BUFFERS > 1)
    GUI_MULTIBUF_ConfigEx(0, EMWIN_NUM_BUFFERS);
#endif

    /* Set display driver and color conversion for 1st layer */
    p_device = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_0, COLOR_CONVERSION, 0, 0);


#if (USE_DAVE2D == 1)
    /* Initialize Dave2D */
    sp_d2_handle = d2_opendevice(0);
    if (NULL == sp_d2_handle)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* Error */
        }
    }

    d2_ret = d2_inithw(sp_d2_handle, 0);
    if (D2_OK != d2_ret)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* Error */
        }
    }

    sp_renderbuffer = d2_newrenderbuffer(sp_d2_handle, 20, 20);
    if (NULL == sp_renderbuffer)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* Error */
        }
    }
#endif

    /* Display size and frame buffer setting */
    lcd_driver_setting(p_device,0,0);

#if (EMWIN_USE_RUNTIME_ORIENTATION == 1)
    /* Create additional drivers and add them to the display rotation module */
    LCD_ROTATE_AddDriverExOrientation(DISPLAY_DRIVER_1, 0, GUI_ROTATION_CW);
    LCD_ROTATE_AddDriverExOrientation(DISPLAY_DRIVER_2, 0, GUI_ROTATION_180);
    LCD_ROTATE_AddDriverExOrientation(DISPLAY_DRIVER_3, 0, GUI_ROTATION_CCW);

    /* Set call back function */
    LCD_ROTATE_SetCallback(lcd_driver_setting, 0);
#endif
}
/**********************************************************************************************************************
 * End of function LCD_X_Config
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: LCD_X_DisplayDriver
 * Description  : This function is called by the display driver for several purposes.
 *                To support the according task the routine needs to be adapted to
 *                the display controller. Please note that the commands marked with
 *                'optional' are not cogently required and should only be adapted if
 *                the display controller supports these features.
 * Arguments    : layer_index - Index of layer to be configured
 *                cmd         - Please refer to the details in the switch statement below
 *                p_data      - Pointer to a LCD_X_DATA structure
 * Return Value : < -1 - Error
 *                  -1 - Command not handled
 *                   0 - Ok
 * Note         : This function is called from the emWin library.
 *********************************************************************************************************************/
int LCD_X_DisplayDriver(unsigned layer_index, unsigned cmd, void * p_data)
{
    int32_t r = 0;
    static bool init_f = false;

    GUI_USE_PARA(layer_index);
    switch (cmd)
    {
        /* Required */
        case LCD_X_INITCONTROLLER:
        {
            /* Called during the initialization process in order to set up the
             * display controller and put it into operation. If the display
             * controller is not initialized by any external routine this needs
             * to be adapted by the customer... */
            if(false == init_f)
            {
                init_controller();
                init_f = true;
            }

#if (USE_DAVE2D == 1)
            R_EMWIN_EnableDave2D();
#endif

            break;
        }

        /* Required for setting a lookup table entry which is passed in the 'Pos' and 'Color' element of p. */
        case LCD_X_SETLUTENTRY:
        {
            LCD_X_SETLUTENTRY_INFO * p;

            p = (LCD_X_SETLUTENTRY_INFO *)p_data;
            set_lut_entry(p->Color, p->Pos);
            break;
        }

        /* Required if multiple buffers are used. The 'Index' element of p contains the buffer index. */
        case LCD_X_SHOWBUFFER:
        {
            LCD_X_SHOWBUFFER_INFO * p;

            p = (LCD_X_SHOWBUFFER_INFO *)p_data;
            switch_buffers_on_vsync(p->Index);
            break;
        }

        /* Required if the display controller should support switching on and off. */
        case LCD_X_ON:
        {
            display_on_off(1);
            break;
        }

        /* Required if the display controller should support switching on and off. */
        case LCD_X_OFF:
        {
            display_on_off(0);
            break;
        }

        default:
        {
            r = -1;
        }
    }

    return r;
}
/**********************************************************************************************************************
 * End of function LCD_X_DisplayDriver
 *********************************************************************************************************************/
#endif /* (EMWIN_LCD_IF == LCD_IF_GLCDC) */
