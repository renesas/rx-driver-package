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
 * File Name    : r_emwin_rx_lcdconf.c
 * Version      : 1.00
 * Description  : Functions of LCD interface.
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
#include "platform.h"
#if (EMWIN_LCD_IF == LCD_IF_GLCDC)
#include "r_glcdc_rx_if.h"
#endif
#include "r_pinset.h"
#include "r_emwin_rx_private.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#if (EMWIN_LCD_IF == LCD_IF_GLCDC)
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
#define COLOR_FORMAT     (FORMAT_RGB_ARGB8888)
#elif (EMWIN_BITS_PER_PIXEL == 16)
#define COLOR_FORMAT     (FORMAT_RGB_565)
#elif (EMWIN_BITS_PER_PIXEL == 8)
#define COLOR_FORMAT     (FORMAT_CLUT_8)
#elif (EMWIN_BITS_PER_PIXEL == 4)
#define COLOR_FORMAT     (FORMAT_CLUT_4)
#elif (EMWIN_BITS_PER_PIXEL == 1)
#define COLOR_FORMAT     (FORMAT_CLUT_1)
#endif
#endif /* EMWIN_LCD_IF == LCD_IF_GLCDC */

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
#if (EMWIN_LCD_IF == LCD_IF_GLCDC)
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
#endif /* EMWIN_LCD_IF == LCD_IF_GLCDC */

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
static volatile int32_t s_pending_buffer = -1;

#if (EMWIN_LCD_IF == LCD_IF_GLCDC)
static glcdc_runtime_cfg_t s_runtime_cfg;
static volatile bool s_first_interrupt_flag; /* It is used for interrupt control of GLCDC module */
#endif

/**********************************************************************************************************************
 * Function Name: r_emwin_rx_lcd_open
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
e_emwin_rx_err_t r_emwin_rx_lcd_open(void)
{
    e_emwin_rx_err_t emwin_ret = EMWIN_RX_FAIL;

#if (EMWIN_LCD_IF == LCD_IF_GLCDC)
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
#else /* EMWIN_LCD_IF == LCD_IF_OTHER */
    #warning "Warning It is necessary to implement open function of LCD interface.!!"
#endif

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
e_emwin_rx_err_t r_emwin_rx_lcd_switch_buffer(uint32_t * p_addr)
{
    e_emwin_rx_err_t emwin_ret = EMWIN_RX_FAIL;

#if (EMWIN_LCD_IF == LCD_IF_GLCDC)
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
#else /* EMWIN_LCD_IF == LCD_IF_OTHER */
    #warning "Warning It is necessary to implement show-buffer function of LCD interface.!!"
#endif

    return emwin_ret;
}
/**********************************************************************************************************************
 * End of function r_emwin_rx_lcd_switch_buffer
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_emwin_rx_lcd_clut_update
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
e_emwin_rx_err_t r_emwin_rx_lcd_clut_update(uint32_t * p_clut)
{
    e_emwin_rx_err_t    emwin_ret = EMWIN_RX_FAIL;

#if (EMWIN_LCD_IF == LCD_IF_GLCDC)
    glcdc_err_t         glcdc_ret;
    glcdc_clut_cfg_t    p_clut_cfg;

    p_clut_cfg.enable = true;
    p_clut_cfg.p_base = p_clut;
    p_clut_cfg.size   = NUM_COLORS;
    p_clut_cfg.start  = 0;
#ifdef __ICCRX__
    glcdc_ret = R_GLCDC_Control(GLCDC_CMD_START_DISPLAY, (void const * const)FIT_NO_FUNC);
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
#else /* EMWIN_LCD_IF == LCD_IF_OTHER */
    #warning "Warning It is necessary to implement update-clut function of LCD interface.!!"
#endif

    return emwin_ret;
}
/**********************************************************************************************************************
 * End of function r_emwin_rx_lcd_clut_update
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: r_emwin_rx_lcd_switch
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
e_emwin_rx_err_t r_emwin_rx_lcd_switch(e_emwin_rx_lcd_switch_t lcd_switch)
{
    e_emwin_rx_err_t    emwin_ret = EMWIN_RX_FAIL;

#if (EMWIN_LCD_IF == LCD_IF_GLCDC)
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
    glcdc_ret = R_GLCDC_Control(glcdc_cmd, (void const * const)FIT_NO_FUNC);
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
#else /* EMWIN_LCD_IF == LCD_IF_OTHER */
    #warning "Warning It is necessary to implement switch function of LCD interface.!!"
#endif

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
#if (EMWIN_LCD_IF == LCD_IF_GLCDC)
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
#else /* EMWIN_LCD_IF == LCD_IF_OTHER */
    #warning "Warning It is necessary to implement ISR function of LCD interface.!!"
#endif
}
/**********************************************************************************************************************
 * End of function _VSYNC_ISR
 *********************************************************************************************************************/
