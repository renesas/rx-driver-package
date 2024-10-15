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
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_qe_display_lcdc.c
 * Version      : 1.10
 * Description  : GLCDC adjustment related processing.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version   Description
 *         : 19.07.2024 1.00      First Release
 *         : 20.09.2024 1.10      Fixed an issue where the pointer increment value was incorrect
 *         :                      in the r_set_display_param_output function.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "r_qe_display_private.h"


/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

/* == Static variables ============================================================================================= */

/* GLCDC Setting structure */
static volatile glcdc_cfg_t s_display_cfg;           /* For adjustment  */
static volatile glcdc_cfg_t s_display_backup_cfg;    /* For backup      */

/* GLCDC color lookup table */
static uint32_t s_display_clut1[256];
static uint32_t s_display_clut2[256];

/* GLCDC gamma table */
static volatile gamma_correction_t s_display_gamma_table_r;         /* Red (for adjustment)   */
static volatile gamma_correction_t s_display_gamma_backup_table_r;  /* Red (for backup)       */
static volatile gamma_correction_t s_display_gamma_table_g;         /* Green (for adjustment) */
static volatile gamma_correction_t s_display_gamma_backup_table_g;  /* Green (for backup)     */
static volatile gamma_correction_t s_display_gamma_table_b;         /* Blue (for adjustment)  */
static volatile gamma_correction_t s_display_gamma_backup_table_b;  /* Blue (for backup)      */

/* GLCDC initialized determination flag */
static bool s_display_init_f = false;


/* == Static functions ============================================================================================== */

static void r_set_display_param_timing (uint8_t * p_data);
static void r_set_display_param_output (uint8_t * p_data);
static void r_set_display_param_layer (uint8_t * p_data, glcdc_frame_layer_t layer);
static void r_set_display_param_procedure (uint8_t * p_data);
static void r_set_display_param_dither (uint8_t * p_data);
static void r_set_display_param_brightness (uint8_t * p_data);
static void r_set_display_param_contrast (uint8_t * p_data);
static void r_set_display_param_gammma (uint8_t * p_data);
static void r_set_display_params (uint8_t command);

static int32_t r_get_display_pixel_size (glcdc_in_format_t format);
static void r_display_print_message (uint8_t response);
static uint8_t r_display_check_err_response(glcdc_err_t glcdc_ret);
static void r_display_read_clut (uint32_t * p_clut_tbl, glcdc_frame_layer_t layer);
static bool r_display_check_glcdc_reflection (void);
static void r_display_backup_setting (void);

static float r_display_modf_qe (float x, float * ip);
static float r_display_floorf_qe (float x);
static void  r_display_draw_32bit_pixel (uint32_t color, glcdc_input_cfg_t * p_config);
static void  r_display_draw_16bit_pixel (uint16_t color, glcdc_input_cfg_t * p_config);
static void  r_display_draw_8bit_pixel (uint8_t color, glcdc_input_cfg_t * p_config);
static void  r_display_draw_4bit_pixel (uint8_t color, glcdc_input_cfg_t * p_config);
static void  r_display_draw_1bit_pixel (uint8_t color, glcdc_input_cfg_t * p_config);
static void  r_display_draw_red_frame (glcdc_input_cfg_t * p_config);
static uint32_t r_display_hsv2argb (float h, float s, float v, float a, glcdc_in_format_t format);
static inline void r_display_put_pixel (glcdc_input_cfg_t * p_config, uint32_t x, uint32_t y, uint32_t color);
static void  r_display_draw_test_pattern (glcdc_input_cfg_t * p_config, uint8_t alpha, uint32_t * p_clut, bool grayscale);



/* Function Name: r_display_glcdc_init */
/*******************************************************************************************************************//**
 * @brief Initialize GLCDC.
 * @param  None
 * @retval QE_DISPLAY_SUCCESS         Processing has been completed successfully.
 * @retval QE_DISPLAY_ERR_LCD_CONFIG  LCD setting error.
 **********************************************************************************************************************/
qe_display_err_t r_display_glcdc_init(void)
{
    qe_display_err_t display_ret = QE_DISPLAY_SUCCESS;
    glcdc_err_t      glcdc_ret;

    s_display_gamma_table_r = g_glcdc_gamma_table_r;
    s_display_gamma_table_g = g_glcdc_gamma_table_g;
    s_display_gamma_table_b = g_glcdc_gamma_table_b;

    /* R_GLCDC_Open function release stop state of GLCDC. */
    glcdc_ret = R_GLCDC_Open((glcdc_cfg_t*) &s_display_cfg);

    if (GLCDC_SUCCESS == glcdc_ret)
    {
        /* Draw a test image to the framebuffer when graphics 1 is enabled */
        if (NULL != s_display_cfg.input[GLCDC_FRAME_LAYER_1].p_base)
        {
            r_display_draw_test_pattern((glcdc_input_cfg_t*) &s_display_cfg.input[0], 255, s_display_clut1, false);

            if (GLCDC_IN_FORMAT_CLUT8 <= s_display_cfg.input[GLCDC_FRAME_LAYER_1].format)
            {
                s_display_cfg.clut[GLCDC_FRAME_LAYER_1].enable = true;

                /* When enable is false, setting values of structure members under clut are ignored. */
                s_display_cfg.clut[GLCDC_FRAME_LAYER_1].p_base = (uint32_t*) s_display_clut1;
                s_display_cfg.clut[GLCDC_FRAME_LAYER_1].start  = 0;
                s_display_cfg.clut[GLCDC_FRAME_LAYER_1].size   = 256;

                R_GLCDC_ClutUpdate_NoReflect(GLCDC_FRAME_LAYER_1, (glcdc_clut_cfg_t *) &s_display_cfg.clut[GLCDC_FRAME_LAYER_1]);
            }

        }

        /* Draw a test image to the framebuffer when graphics 2 is enabled */
        if (NULL != s_display_cfg.input[GLCDC_FRAME_LAYER_2].p_base)
        {
            r_display_draw_test_pattern((glcdc_input_cfg_t*) &s_display_cfg.input[1], 255, s_display_clut2, false);

            if (GLCDC_IN_FORMAT_CLUT8 <= s_display_cfg.input[GLCDC_FRAME_LAYER_2].format)
            {
                s_display_cfg.clut[GLCDC_FRAME_LAYER_2].enable = true;

                /* When enable is false, setting values of structure members under clut are ignored. */
                s_display_cfg.clut[GLCDC_FRAME_LAYER_2].p_base = (uint32_t*) s_display_clut2;
                s_display_cfg.clut[GLCDC_FRAME_LAYER_2].start  = 0;
                s_display_cfg.clut[GLCDC_FRAME_LAYER_2].size   = 256;

                R_GLCDC_ClutUpdate_NoReflect(GLCDC_FRAME_LAYER_2, (glcdc_clut_cfg_t*) &s_display_cfg.clut[GLCDC_FRAME_LAYER_2]);
            }

        }

        /* Function select of multiplex pins */
        R_GLCDC_PinSet();

#if (LCD_CH0_USE_RESET_PIN == 1)
        /* Set DISP signal on */
        R_GPIO_PinDirectionSet(LCD_CH0_RESET_PIN, GPIO_DIRECTION_OUTPUT); /* Port direction: output */
        R_GPIO_PinWrite(LCD_CH0_RESET_PIN, LCD_CH0_RESET_PIN_ACTIVE_LEVEL); /* DISP on */
#endif

        R_GLCDC_Control(GLCDC_CMD_START_DISPLAY, NULL);

#if (LCD_CH0_USE_BACKLIGHT_PIN == 1)
        /* Switch backlight on, no PWM */
        R_GPIO_PinDirectionSet(LCD_CH0_BACKLIGHT_PIN, GPIO_DIRECTION_OUTPUT);
        R_GPIO_PinWrite(LCD_CH0_BACKLIGHT_PIN, LCD_CH0_BACKLIGHT_PIN_ACTIVE_LEVEL);
#endif

        /* Initialize flag set */
        s_display_init_f = true;

    }
    else if (GLCDC_ERR_INVALID_MODE == glcdc_ret) /* GLCDC is already opened (by emWin or Aeropoint) */
    {
        /* Setting initial values to adjustment structure variables */

        /* Output timing */
        s_display_cfg.output.htiming.front_porch = LCD_CH0_W_HFP;
        s_display_cfg.output.htiming.back_porch  = LCD_CH0_W_HBP;
        s_display_cfg.output.htiming.display_cyc = LCD_CH0_DISP_HW;
        s_display_cfg.output.htiming.sync_width  = LCD_CH0_W_HSYNC;

        s_display_cfg.output.vtiming.front_porch = LCD_CH0_W_VFP;
        s_display_cfg.output.vtiming.back_porch  = LCD_CH0_W_VBP;
        s_display_cfg.output.vtiming.display_cyc = LCD_CH0_DISP_VW;
        s_display_cfg.output.vtiming.sync_width  = LCD_CH0_W_VSYNC;

        /* Output format */
        s_display_cfg.output.format = LCD_CH0_OUT_FORMAT;

        /* Tcon polarity*/
        s_display_cfg.output.data_enable_polarity = LCD_CH0_TCON_POL_DE;
        s_display_cfg.output.hsync_polarity       = LCD_CH0_TCON_POL_HSYNC;
        s_display_cfg.output.vsync_polarity       = LCD_CH0_TCON_POL_VSYNC;

        /* Sync signal edge */
        s_display_cfg.output.sync_edge = LCD_CH0_OUT_EDGE;

        /* Output tcon pin */
        s_display_cfg.output.tcon_hsync = LCD_CH0_TCON_PIN_HSYNC;
        s_display_cfg.output.tcon_vsync = LCD_CH0_TCON_PIN_VSYNC;
        s_display_cfg.output.tcon_de    = LCD_CH0_TCON_PIN_DE;

        /* Endian */
        s_display_cfg.output.endian = LCD_CH0_OUT_ENDIAN;

        /* Color order */
        s_display_cfg.output.color_order = LCD_CH0_OUT_COLOR_ORDER;

        /* Output clock */
        s_display_cfg.output.clksrc          = GLCDC_CLK_SRC_INTERNAL;
        s_display_cfg.output.clock_div_ratio = LCD_CH0_OUT_CLK_DIV_RATIO;

        /* Back ground color */
        s_display_cfg.output.bg_color.argb = LCD_CH0_OUT_BG_COLOR;

        /* Correction circuit sequence */
        s_display_cfg.output.correction_proc_order = IMGC_OUTCTL_CALIB_ROUTE;

        /* Brightness */
        s_display_cfg.output.brightness.enable = IMGC_BRIGHT_OUTCTL_ACTIVE;
        s_display_cfg.output.brightness.r      = IMGC_BRIGHT_OUTCTL_OFFSET_R;
        s_display_cfg.output.brightness.g      = IMGC_BRIGHT_OUTCTL_OFFSET_G;
        s_display_cfg.output.brightness.b      = IMGC_BRIGHT_OUTCTL_OFFSET_B;

        /* Contrast */
        s_display_cfg.output.contrast.enable = IMGC_CONTRAST_OUTCTL_ACTIVE;
        s_display_cfg.output.contrast.r      = IMGC_CONTRAST_OUTCTL_GAIN_R;
        s_display_cfg.output.contrast.g      = IMGC_CONTRAST_OUTCTL_GAIN_G;
        s_display_cfg.output.contrast.b      = IMGC_CONTRAST_OUTCTL_GAIN_B;

        /* Gamma */
        s_display_cfg.output.gamma.enable = IMGC_GAMMA_ACTIVE;
        s_display_cfg.output.gamma.p_r    = (gamma_correction_t*) &s_display_gamma_table_r;
        s_display_cfg.output.gamma.p_g    = (gamma_correction_t*) &s_display_gamma_table_g;
        s_display_cfg.output.gamma.p_b    = (gamma_correction_t*) &s_display_gamma_table_b;

        /* Dithering */
        s_display_cfg.output.dithering.dithering_on        = IMGC_DITHER_ACTIVE;
        s_display_cfg.output.dithering.dithering_mode      = IMGC_DITHER_MODE;
        s_display_cfg.output.dithering.dithering_pattern_a = IMGC_DITHER_2X2_PA;
        s_display_cfg.output.dithering.dithering_pattern_b = IMGC_DITHER_2X2_PB;
        s_display_cfg.output.dithering.dithering_pattern_c = IMGC_DITHER_2X2_PC;
        s_display_cfg.output.dithering.dithering_pattern_d = IMGC_DITHER_2X2_PD;


        /* ---- Graphic layer 1 setting ---- */
        /* Image format */
        s_display_cfg.input[GLCDC_FRAME_LAYER_1].p_base        = (uint32_t*) LCD_CH0_IN_GR1_PBASE;
        s_display_cfg.input[GLCDC_FRAME_LAYER_1].hsize         = LCD_CH0_IN_GR1_HSIZE;
        s_display_cfg.input[GLCDC_FRAME_LAYER_1].vsize         = LCD_CH0_IN_GR1_VSIZE;
        s_display_cfg.input[GLCDC_FRAME_LAYER_1].offset        = LCD_CH0_IN_GR1_LINEOFFSET;
        s_display_cfg.input[GLCDC_FRAME_LAYER_1].format        = LCD_CH0_IN_GR1_FORMAT;
        s_display_cfg.input[GLCDC_FRAME_LAYER_1].frame_edge    = LCD_CH0_IN_GR1_FRAME_EDGE;
        s_display_cfg.input[GLCDC_FRAME_LAYER_1].coordinate.x  = LCD_CH0_IN_GR1_COORD_X;
        s_display_cfg.input[GLCDC_FRAME_LAYER_1].coordinate.y  = LCD_CH0_IN_GR1_COORD_Y;
        s_display_cfg.input[GLCDC_FRAME_LAYER_1].bg_color.argb = LCD_CH0_IN_GR1_BG_COLOR;

        /* Blending */
        s_display_cfg.blend[GLCDC_FRAME_LAYER_1].visible       = LCD_CH0_BLEND_GR1_VISIBLE;
        s_display_cfg.blend[GLCDC_FRAME_LAYER_1].blend_control = LCD_CH0_BLEND_GR1_BLEND_CONTROL;

        /* When blend_control is GLCDC_BLEND_CONTROL_NONE,
         * setting values of following blend structure members are ignored. */
        s_display_cfg.blend[GLCDC_FRAME_LAYER_1].frame_edge         = LCD_CH0_BLEND_GR1_FRAME_EDGE;
        s_display_cfg.blend[GLCDC_FRAME_LAYER_1].fixed_blend_value  = LCD_CH0_BLEND_GR1_FIXED_BLEND_VALUE;
        s_display_cfg.blend[GLCDC_FRAME_LAYER_1].fade_speed         = LCD_CH0_BLEND_GR1_FADE_SPEED;
        s_display_cfg.blend[GLCDC_FRAME_LAYER_1].start_coordinate.x = LCD_CH0_BLEND_GR1_START_COORD_X;
        s_display_cfg.blend[GLCDC_FRAME_LAYER_1].start_coordinate.y = LCD_CH0_BLEND_GR1_START_COORD_Y;
        s_display_cfg.blend[GLCDC_FRAME_LAYER_1].end_coordinate.x   = LCD_CH0_BLEND_GR1_END_COORD_X;
        s_display_cfg.blend[GLCDC_FRAME_LAYER_1].end_coordinate.y   = LCD_CH0_BLEND_GR1_END_COORD_Y;

        /* Chromakey */
        s_display_cfg.chromakey[GLCDC_FRAME_LAYER_1].enable = LCD_CH0_CHROMAKEY_GR1_ENABLE;

        /* When enable is false, setting values of structure members under chromakey are ignored. */
        s_display_cfg.chromakey[GLCDC_FRAME_LAYER_1].before.argb = LCD_CH0_CHROMAKEY_GR1_BEFORE_ARGB;
        s_display_cfg.chromakey[GLCDC_FRAME_LAYER_1].after.argb  = LCD_CH0_CHROMAKEY_GR1_AFTER_ARGB;

        /* Color look-up table */
        if (GLCDC_IN_FORMAT_CLUT8 <= s_display_cfg.input[GLCDC_FRAME_LAYER_1].format)
        {
            r_display_read_clut(s_display_clut1, GLCDC_FRAME_LAYER_1);

            s_display_cfg.clut[GLCDC_FRAME_LAYER_1].enable = true;

            /* When enable is false, setting values of structure members under clut are ignored. */
            s_display_cfg.clut[GLCDC_FRAME_LAYER_1].p_base = (uint32_t*) s_display_clut1;
            s_display_cfg.clut[GLCDC_FRAME_LAYER_1].start  = 0;
            s_display_cfg.clut[GLCDC_FRAME_LAYER_1].size   = 256;
        }
        else
        {
            s_display_cfg.clut[GLCDC_FRAME_LAYER_1].enable = LCD_CH0_CLUT_GR1_ENABLE;

            /* When enable is false, setting values of structure members under clut are ignored. */
            s_display_cfg.clut[GLCDC_FRAME_LAYER_1].p_base = (uint32_t*) LCD_CH0_CLUT_GR1_PBASE;
            s_display_cfg.clut[GLCDC_FRAME_LAYER_1].start  = LCD_CH0_CLUT_GR1_START;
            s_display_cfg.clut[GLCDC_FRAME_LAYER_1].size   = LCD_CH0_CLUT_GR1_SIZE;
        }

        /* ---- Graphic layer 2 setting ---- */
        /* Image format */
        s_display_cfg.input[GLCDC_FRAME_LAYER_2].p_base        = (uint32_t*) LCD_CH0_IN_GR2_PBASE;
        s_display_cfg.input[GLCDC_FRAME_LAYER_2].hsize         = LCD_CH0_IN_GR2_HSIZE;
        s_display_cfg.input[GLCDC_FRAME_LAYER_2].vsize         = LCD_CH0_IN_GR2_VSIZE;
        s_display_cfg.input[GLCDC_FRAME_LAYER_2].offset        = LCD_CH0_IN_GR2_LINEOFFSET;
        s_display_cfg.input[GLCDC_FRAME_LAYER_2].format        = LCD_CH0_IN_GR2_FORMAT;
        s_display_cfg.input[GLCDC_FRAME_LAYER_2].frame_edge    = LCD_CH0_IN_GR2_FRAME_EDGE;
        s_display_cfg.input[GLCDC_FRAME_LAYER_2].coordinate.x  = LCD_CH0_IN_GR2_COORD_X;
        s_display_cfg.input[GLCDC_FRAME_LAYER_2].coordinate.y  = LCD_CH0_IN_GR2_COORD_Y;
        s_display_cfg.input[GLCDC_FRAME_LAYER_2].bg_color.argb = LCD_CH0_IN_GR2_BG_COLOR;

        /* Blending */
        s_display_cfg.blend[GLCDC_FRAME_LAYER_2].visible       = LCD_CH0_BLEND_GR2_VISIBLE;
        s_display_cfg.blend[GLCDC_FRAME_LAYER_2].blend_control = LCD_CH0_BLEND_GR2_BLEND_CONTROL;

        /* When blend_control is GLCDC_BLEND_CONTROL_NONE,
         * setting values of following blend structure members are ignored. */
        s_display_cfg.blend[GLCDC_FRAME_LAYER_2].frame_edge         = LCD_CH0_BLEND_GR2_FRAME_EDGE;
        s_display_cfg.blend[GLCDC_FRAME_LAYER_2].fixed_blend_value  = LCD_CH0_BLEND_GR2_FIXED_BLEND_VALUE;
        s_display_cfg.blend[GLCDC_FRAME_LAYER_2].fade_speed         = LCD_CH0_BLEND_GR2_FADE_SPEED;
        s_display_cfg.blend[GLCDC_FRAME_LAYER_2].start_coordinate.x = LCD_CH0_BLEND_GR2_START_COORD_X;
        s_display_cfg.blend[GLCDC_FRAME_LAYER_2].start_coordinate.y = LCD_CH0_BLEND_GR2_START_COORD_Y;
        s_display_cfg.blend[GLCDC_FRAME_LAYER_2].end_coordinate.x   = LCD_CH0_BLEND_GR2_END_COORD_X;
        s_display_cfg.blend[GLCDC_FRAME_LAYER_2].end_coordinate.y   = LCD_CH0_BLEND_GR2_END_COORD_Y;

        /* Chromakey */
        s_display_cfg.chromakey[GLCDC_FRAME_LAYER_2].enable = LCD_CH0_CHROMAKEY_GR2_ENABLE;

        /* When enable is false, setting values of structure members under chromakey are ignored. */
        s_display_cfg.chromakey[GLCDC_FRAME_LAYER_2].before.argb = LCD_CH0_CHROMAKEY_GR2_BEFORE_ARGB;
        s_display_cfg.chromakey[GLCDC_FRAME_LAYER_2].after.argb  = LCD_CH0_CHROMAKEY_GR2_AFTER_ARGB;

        /* Color look-up table */
        if (GLCDC_IN_FORMAT_CLUT8 <= s_display_cfg.input[GLCDC_FRAME_LAYER_2].format)
        {
            r_display_read_clut(s_display_clut2, GLCDC_FRAME_LAYER_2);

            s_display_cfg.clut[GLCDC_FRAME_LAYER_2].enable = true;

            /* When enable is false, setting values of structure members under clut are ignored. */
            s_display_cfg.clut[GLCDC_FRAME_LAYER_2].p_base = (uint32_t*) s_display_clut2;
            s_display_cfg.clut[GLCDC_FRAME_LAYER_2].start  = 0;
            s_display_cfg.clut[GLCDC_FRAME_LAYER_2].size   = 256;
        }
        else
        {
            s_display_cfg.clut[GLCDC_FRAME_LAYER_2].enable = LCD_CH0_CLUT_GR2_ENABLE;

            /* When enable is false, setting values of structure members under clut are ignored. */
            s_display_cfg.clut[GLCDC_FRAME_LAYER_2].p_base = (uint32_t*) LCD_CH0_CLUT_GR2_PBASE;
            s_display_cfg.clut[GLCDC_FRAME_LAYER_2].start  = LCD_CH0_CLUT_GR2_START;
            s_display_cfg.clut[GLCDC_FRAME_LAYER_2].size   = LCD_CH0_CLUT_GR2_SIZE;
        }

        /* Detection */
        s_display_cfg.detection.vpos_detect  = LCD_CH0_DETECT_VPOS;
        s_display_cfg.detection.gr1uf_detect = LCD_CH0_DETECT_GR1UF;
        s_display_cfg.detection.gr2uf_detect = LCD_CH0_DETECT_GR2UF;

        /* Interrupt */
        s_display_cfg.interrupt.vpos_enable  = LCD_CH0_INTERRUPT_VPOS_ENABLE;
        s_display_cfg.interrupt.gr1uf_enable = LCD_CH0_INTERRUPT_GR1UF_ENABLE;
        s_display_cfg.interrupt.gr2uf_enable = LCD_CH0_INTERRUPT_GR2UF_ENABLE;

        /* Call back function */
        s_display_cfg.p_callback = GLCDC_PRV_PCALLBACK;

    }
    else
    {
        r_debug_print("Adjustment Error: Could not initialize GLCDC!!\n");
        display_ret = QE_DISPLAY_ERR_LCD_CONFIG;
    }

    return display_ret;
}
/**********************************************************************************************************************
 * End of function r_display_glcdc_init
 *********************************************************************************************************************/

/* Function Name: r_display_glcdc_reinit */
/*******************************************************************************************************************//**
 * @brief Re-initialize GLCDC.
 * @retval QE_DISPLAY_SUCCESS         Processing has been completed successfully.
 * @retval QE_DISPLAY_ERR_LCD_OTHER   Other LCD errors.
 **********************************************************************************************************************/
qe_display_err_t r_display_glcdc_reinit(void)
{
    qe_display_err_t display_ret = QE_DISPLAY_SUCCESS;
    glcdc_err_t      ret;
    uint8_t          command;

    /* Restoring from backup data */
    s_display_cfg = s_display_backup_cfg;
    s_display_gamma_table_r = s_display_gamma_backup_table_r;
    s_display_gamma_table_g = s_display_gamma_backup_table_g;
    s_display_gamma_table_b = s_display_gamma_backup_table_b;

    /* get current command */
    command = r_get_uart_command();

    switch (command)
    {
        case QE_DISPLAY_COMMAND_ALL:
        case QE_DISPLAY_COMMAND_TIMING:
        case QE_DISPLAY_COMMAND_OUTCTL:
        case QE_DISPLAY_COMMAND_PROCIDURE:
        case QE_DISPLAY_COMMAND_DITHER:

            /* Disabling GLCDC configuration settings mode */
            r_glcdc_config_mode_setting(false);

            /* R_GLCDC_Open function release stop state of GLCDC. */
            ret = R_GLCDC_Open((glcdc_cfg_t*) &s_display_cfg);

            if (GLCDC_SUCCESS == ret)
            {

                /* Start displaying graphics */
                ret = R_GLCDC_Control(GLCDC_CMD_START_DISPLAY, NULL);
                if (GLCDC_SUCCESS != ret)
                {
                    display_ret = QE_DISPLAY_ERR_LCD_OTHER;
                    r_debug_print("Adjustment Error: Could not restart GLCDC!! Please system reset!!\n");
                    break;
                }

                /* Waiting for GLCDC register settings to be reflected */
                /* WAIT_LOOP */
                while (true == r_display_check_glcdc_reflection())
                {
                    R_BSP_NOP();
                }
            }
            else
            {
                display_ret = QE_DISPLAY_ERR_LCD_OTHER;
                r_debug_print("Adjustment Error: Could not restart GLCDC!! Please system reset!!\n");
            }

            break;

        default:
            R_BSP_NOP();
    }

    return display_ret;
}
/**********************************************************************************************************************
 * End of function r_display_glcdc_reinit
 *********************************************************************************************************************/


/* Function Name: r_display_exec_ajustment */
/*******************************************************************************************************************//**
 * @brief Execution GLCDC adjustment.
 * @param None
 * @retval None
 **********************************************************************************************************************/
void r_display_exec_ajustment(void)
{
    glcdc_runtime_cfg_t runtime_cfg;
    glcdc_correction_t correction_cfg;
    glcdc_err_t ret;
    uint8_t response_data;
    uint8_t command;
    uint8_t status;


    if (QE_DISPLAY_STS_EXEC == r_get_uart_status())
    {
        /* Backup setting */
        r_display_backup_setting();

        /* Get Command */
        command = r_get_uart_command();

        /* Set Parameters */
        r_set_display_params(command);

        switch (command)
        {
            case QE_DISPLAY_COMMAND_ALL:
            case QE_DISPLAY_COMMAND_TIMING:
            case QE_DISPLAY_COMMAND_OUTCTL:
            case QE_DISPLAY_COMMAND_PROCIDURE:
            case QE_DISPLAY_COMMAND_DITHER:

                /* Stop displaying graphics */
                ret = R_GLCDC_Control(GLCDC_CMD_STOP_DISPLAY, NULL);
                if (GLCDC_SUCCESS != ret)
                {
                    response_data = QE_DISPLAY_RES_NACK;
                    status        = QE_DISPLAY_STS_LCD_REINIT;
                    break;
                }

                /* Close GLCDC */
                ret = R_GLCDC_Close();
                if (GLCDC_SUCCESS != ret)
                {
                    response_data = QE_DISPLAY_RES_NACK;
                    status        = QE_DISPLAY_STS_LCD_REINIT;
                    break;
                }

                /* Disabling GLCDC configuration settings mode */
                r_glcdc_config_mode_setting(false);

                /* R_GLCDC_Open function release stop state of GLCDC. */
                ret = R_GLCDC_Open((glcdc_cfg_t*) &s_display_cfg);

                if (GLCDC_SUCCESS == ret)
                {

                    if (true == s_display_init_f)
                    {
                        /* Draw a test image to the framebuffer when graphics 1 is enabled */
                        if (NULL != s_display_cfg.input[GLCDC_FRAME_LAYER_1].p_base)
                        {
                            r_display_draw_test_pattern((glcdc_input_cfg_t*) &s_display_cfg.input[0], 255, s_display_clut1, false);

                            if (GLCDC_IN_FORMAT_CLUT8 <= s_display_cfg.input[GLCDC_FRAME_LAYER_1].format)
                            {
                                s_display_cfg.clut[GLCDC_FRAME_LAYER_1].enable = true;

                                /* When enable is false, setting values of structure members under clut are ignored. */
                                s_display_cfg.clut[GLCDC_FRAME_LAYER_1].p_base = (uint32_t*) s_display_clut1;
                                s_display_cfg.clut[GLCDC_FRAME_LAYER_1].start  = 0;
                                s_display_cfg.clut[GLCDC_FRAME_LAYER_1].size   = 256;

                                R_GLCDC_ClutUpdate_NoReflect(GLCDC_FRAME_LAYER_1, (glcdc_clut_cfg_t *)&s_display_cfg.clut[GLCDC_FRAME_LAYER_1]);
                            }

                        }

                        /* Draw a test image to the framebuffer when graphics 2 is enabled */
                        if (NULL != s_display_cfg.input[GLCDC_FRAME_LAYER_2].p_base)
                        {
                            r_display_draw_test_pattern((glcdc_input_cfg_t*) &s_display_cfg.input[1], 255, s_display_clut2, false);

                            if (GLCDC_IN_FORMAT_CLUT8 <= s_display_cfg.input[GLCDC_FRAME_LAYER_2].format)
                            {
                                s_display_cfg.clut[GLCDC_FRAME_LAYER_2].enable = true;

                                /* When enable is false, setting values of structure members under clut are ignored. */
                                s_display_cfg.clut[GLCDC_FRAME_LAYER_2].p_base = (uint32_t*) s_display_clut2;
                                s_display_cfg.clut[GLCDC_FRAME_LAYER_2].start  = 0;
                                s_display_cfg.clut[GLCDC_FRAME_LAYER_2].size   = 256;

                                R_GLCDC_ClutUpdate_NoReflect(GLCDC_FRAME_LAYER_2, (glcdc_clut_cfg_t*) &s_display_cfg.clut[GLCDC_FRAME_LAYER_2]);
                            }

                        }

                    }

                    /* Start displaying graphics */
                    ret = R_GLCDC_Control(GLCDC_CMD_START_DISPLAY, NULL);
                    if (GLCDC_SUCCESS != ret)
                    {
                        response_data = QE_DISPLAY_RES_NACK;
                        status        = QE_DISPLAY_STS_LCD_REINIT;
                        break;
                    }

                    /* Waiting for GLCDC register settings to be reflected */
                    /* WAIT_LOOP */
                    while (true == r_display_check_glcdc_reflection())
                    {
                        R_BSP_NOP();
                    }

                    response_data = QE_DISPLAY_RES_SUCCES_SETTING;
                    status        = QE_DISPLAY_STS_IDLE;
                    r_set_uart_command(QE_DISPLAY_COMMAND_NULL);
                }
                else
                {
                    response_data = r_display_check_err_response(ret);
                    status        = QE_DISPLAY_STS_LCD_REINIT;
                }

                break;

            case QE_DISPLAY_COMMAND_LAYER1:

                if (NULL != s_display_cfg.input[GLCDC_FRAME_LAYER_1].p_base)
                {
                    runtime_cfg.blend     = s_display_cfg.blend[GLCDC_FRAME_LAYER_1];
                    runtime_cfg.input     = s_display_cfg.input[GLCDC_FRAME_LAYER_1];
                    runtime_cfg.chromakey = s_display_cfg.chromakey[GLCDC_FRAME_LAYER_1];

                    ret = R_GLCDC_LayerChange(GLCDC_FRAME_LAYER_1, &runtime_cfg);

                    if (GLCDC_SUCCESS == ret)
                    {
                        if (true == s_display_init_f)
                        {
                            /* Draw a test image to the framebuffer when graphics 1 is enabled */
                            if (NULL != s_display_cfg.input[GLCDC_FRAME_LAYER_1].p_base)
                            {
                                r_display_draw_test_pattern((glcdc_input_cfg_t*) &s_display_cfg.input[0], 255, s_display_clut1, false);

                                if (GLCDC_IN_FORMAT_CLUT8 <= s_display_cfg.input[GLCDC_FRAME_LAYER_1].format)
                                {
                                    s_display_cfg.clut[GLCDC_FRAME_LAYER_1].enable = true;

                                    /* When enable is false, setting values of structure members under clut are ignored. */
                                    s_display_cfg.clut[GLCDC_FRAME_LAYER_1].p_base = (uint32_t*) s_display_clut1;
                                    s_display_cfg.clut[GLCDC_FRAME_LAYER_1].start  = 0;
                                    s_display_cfg.clut[GLCDC_FRAME_LAYER_1].size   = 256;

                                    R_GLCDC_ClutUpdate_NoReflect(GLCDC_FRAME_LAYER_1, (glcdc_clut_cfg_t *)&s_display_cfg.clut[GLCDC_FRAME_LAYER_1]);
                                }
                            }
                        }

                        /* Waiting for GLCDC register settings to be reflected */
                        /* WAIT_LOOP */
                        while (true == r_display_check_glcdc_reflection())
                        {
                            R_BSP_NOP();
                        }

                        response_data = QE_DISPLAY_RES_SUCCES_SETTING;
                        status        = QE_DISPLAY_STS_IDLE;
                        r_set_uart_command(QE_DISPLAY_COMMAND_NULL);
                    }
                    else
                    {
                        response_data = r_display_check_err_response(ret);
                        status        = QE_DISPLAY_STS_LCD_REINIT;
                    }
                }
                else
                {
                    response_data = QE_DISPLAY_RES_SUCCES_SETTING;
                    status        = QE_DISPLAY_STS_IDLE;
                    r_set_uart_command(QE_DISPLAY_COMMAND_NULL);
                }

                break;

            case QE_DISPLAY_COMMAND_LAYER2:

                if (NULL != s_display_cfg.input[GLCDC_FRAME_LAYER_2].p_base)
                {
                    runtime_cfg.blend     = s_display_cfg.blend[GLCDC_FRAME_LAYER_2];
                    runtime_cfg.input     = s_display_cfg.input[GLCDC_FRAME_LAYER_2];
                    runtime_cfg.chromakey = s_display_cfg.chromakey[GLCDC_FRAME_LAYER_2];

                    ret = R_GLCDC_LayerChange(GLCDC_FRAME_LAYER_2, &runtime_cfg);

                    if (GLCDC_SUCCESS == ret)
                    {
                        if (true == s_display_init_f)
                        {
                            /* Draw a test image to the framebuffer when graphics 2 is enabled */
                            if (NULL != s_display_cfg.input[GLCDC_FRAME_LAYER_2].p_base)
                            {
                                r_display_draw_test_pattern((glcdc_input_cfg_t*) &s_display_cfg.input[1], 255, s_display_clut2, false);

                                if (GLCDC_IN_FORMAT_CLUT8 <= s_display_cfg.input[GLCDC_FRAME_LAYER_2].format)
                                {
                                    s_display_cfg.clut[GLCDC_FRAME_LAYER_2].enable = true;

                                    /* When enable is false, setting values of structure members under clut are ignored. */
                                    s_display_cfg.clut[GLCDC_FRAME_LAYER_2].p_base = (uint32_t*) s_display_clut2;
                                    s_display_cfg.clut[GLCDC_FRAME_LAYER_2].start  = 0;
                                    s_display_cfg.clut[GLCDC_FRAME_LAYER_2].size   = 256;

                                    R_GLCDC_ClutUpdate_NoReflect(GLCDC_FRAME_LAYER_2, (glcdc_clut_cfg_t*) &s_display_cfg.clut[GLCDC_FRAME_LAYER_2]);
                                }
                            }
                        }

                        /* Waiting for GLCDC register settings to be reflected */
                        /* WAIT_LOOP */
                        while (true == r_display_check_glcdc_reflection())
                        {
                            R_BSP_NOP();
                        }

                        response_data = QE_DISPLAY_RES_SUCCES_SETTING;
                        status        = QE_DISPLAY_STS_IDLE;
                        r_set_uart_command(QE_DISPLAY_COMMAND_NULL);
                    }
                    else
                    {
                        response_data = r_display_check_err_response(ret);
                        status        = QE_DISPLAY_STS_LCD_REINIT;
                    }
                }
                else
                {
                    response_data = QE_DISPLAY_RES_SUCCES_SETTING;
                    status        = QE_DISPLAY_STS_IDLE;
                    r_set_uart_command(QE_DISPLAY_COMMAND_NULL);
                }

                break;

            case QE_DISPLAY_COMMAND_BRIGHTNESS:

                correction_cfg.brightness = s_display_cfg.output.brightness;

                ret = R_GLCDC_ColorCorrection(GLCDC_CORRECTION_CMD_BRIGHTNESS, &correction_cfg.brightness);

                if (GLCDC_SUCCESS == ret)
                {
                    /* Waiting for GLCDC register settings to be reflected */
                    /* WAIT_LOOP */
                    while (true == r_display_check_glcdc_reflection())
                    {
                        R_BSP_NOP();
                    }

                    response_data = QE_DISPLAY_RES_SUCCES_SETTING;
                    status        = QE_DISPLAY_STS_IDLE;
                    r_set_uart_command(QE_DISPLAY_COMMAND_NULL);
                }
                else
                {
                    response_data = r_display_check_err_response(ret);
                    status        = QE_DISPLAY_STS_LCD_REINIT;
                }

                break;

            case QE_DISPLAY_COMMAND_CONTRAST:

                correction_cfg.contrast = s_display_cfg.output.contrast;

                ret = R_GLCDC_ColorCorrection(GLCDC_CORRECTION_CMD_CONTRAST, &correction_cfg.contrast);

                if (GLCDC_SUCCESS == ret)
                {
                    /* Waiting for GLCDC register settings to be reflected */
                    /* WAIT_LOOP */
                    while (true == r_display_check_glcdc_reflection())
                    {
                        R_BSP_NOP();
                    }

                    response_data = QE_DISPLAY_RES_SUCCES_SETTING;
                    status        = QE_DISPLAY_STS_IDLE;
                    r_set_uart_command(QE_DISPLAY_COMMAND_NULL);
                }
                else
                {
                    response_data = r_display_check_err_response(ret);
                    status        = QE_DISPLAY_STS_LCD_REINIT;
                }

                break;

            case QE_DISPLAY_COMMAND_GAMMA:

                correction_cfg.gamma = s_display_cfg.output.gamma;

                ret = R_GLCDC_ColorCorrection(GLCDC_CORRECTION_CMD_GAMMA, &correction_cfg.gamma);

                if (GLCDC_SUCCESS == ret)
                {
                    /* Waiting for GLCDC register settings to be reflected */
                    /* WAIT_LOOP */
                    while (true == r_display_check_glcdc_reflection())
                    {
                        R_BSP_NOP();
                    }

                    response_data = QE_DISPLAY_RES_SUCCES_SETTING;
                    status        = QE_DISPLAY_STS_IDLE;
                    r_set_uart_command(QE_DISPLAY_COMMAND_NULL);
                }
                else
                {
                    response_data = r_display_check_err_response(ret);
                    status        = QE_DISPLAY_STS_LCD_REINIT;
                }

                break;

            default: /* Errors that are not normally reached */

                response_data = QE_DISPLAY_RES_NACK;
                status        = QE_DISPLAY_STS_IDLE;
                r_set_uart_command(QE_DISPLAY_COMMAND_NULL);
        }

        /* There needs to be a interval between the response that data reception is complete
         * and the response that adjustment is complete. */
        R_BSP_SoftwareDelay(50, BSP_DELAY_MILLISECS);

        r_uart_send_response(response_data);
        r_set_uart_status(status);

        r_display_print_message(response_data);
    }

}
/**********************************************************************************************************************
 * End of function r_display_exec_ajustment
 *********************************************************************************************************************/


/* Function Name: r_set_display_param_timing */
/*******************************************************************************************************************//**
 * @brief Set timing parameter.
 * @param[in] p_data  Pointer to receive data.
 * @retval None
 **********************************************************************************************************************/
static void r_set_display_param_timing(uint8_t * p_data)
{
    uint16_t temp;

    /* Output clock */
    s_display_cfg.output.clock_div_ratio = (glcdc_panel_clk_div_t) *p_data;
    p_data++;

    /* Output timing */
    temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
    s_display_cfg.output.vtiming.sync_width = temp;
    p_data += 2;

    temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
    s_display_cfg.output.vtiming.back_porch = temp;
    p_data += 2;

    temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
    s_display_cfg.output.vtiming.front_porch = temp;
    p_data += 2;

    temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
    s_display_cfg.output.htiming.sync_width = temp;
    p_data += 2;

    temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
    s_display_cfg.output.htiming.back_porch = temp;
    p_data += 2;

    temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
    s_display_cfg.output.htiming.front_porch = temp;

}
/**********************************************************************************************************************
 * End of function r_set_display_param_timing
 *********************************************************************************************************************/


/* Function Name: r_set_display_param_output */
/*******************************************************************************************************************//**
 * @brief Set output parameter.
 * @param[in] p_data  Pointer to receive data.
 * @retval None
 **********************************************************************************************************************/
static void r_set_display_param_output(uint8_t * p_data)
{
    uint32_t temp;

    s_display_cfg.output.tcon_vsync = (glcdc_tcon_pin_t) *p_data;
    p_data++;

    s_display_cfg.output.vsync_polarity = (glcdc_signal_polarity_t) *p_data;
    p_data++;

    s_display_cfg.output.tcon_hsync = (glcdc_tcon_pin_t) *p_data;
    p_data++;

    s_display_cfg.output.hsync_polarity = (glcdc_signal_polarity_t) *p_data;
    p_data++;

    s_display_cfg.output.tcon_de = (glcdc_tcon_pin_t) *p_data;
    p_data++;

    s_display_cfg.output.data_enable_polarity = (glcdc_signal_polarity_t) *p_data;
    p_data++;

    /* Output format */
    s_display_cfg.output.format = (glcdc_out_format_t) *p_data;
    p_data++;

    /* Sync signal edge */
    s_display_cfg.output.sync_edge = (glcdc_sync_edge_t) *p_data;
    p_data++;

    /* Back ground color */
    temp = (uint32_t) (((*(p_data + 2)) << 16) | ((*(p_data + 1)) << 8) | (*p_data));
    s_display_cfg.output.bg_color.argb = temp;
    p_data += 4;

    /* Endian */
    s_display_cfg.output.endian = (glcdc_endian_t) *p_data;
    p_data++;

    /* Color order */
    s_display_cfg.output.color_order = (glcdc_color_order_t) *p_data;

}
/**********************************************************************************************************************
 * End of function r_set_display_param_output
 *********************************************************************************************************************/


/* Function Name: r_set_display_param_layer */
/*******************************************************************************************************************//**
 * @brief Set adjusting parameter.
 * @param[in] p_data  Pointer to receive data.
 * @param[in] layer   Target layer num.
 * @retval None
 **********************************************************************************************************************/
static void r_set_display_param_layer(uint8_t * p_data, glcdc_frame_layer_t layer)
{
    uint16_t temp;
    int32_t  line_offset;
    int32_t  bits_per_pixel;

    temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
    s_display_cfg.input[layer].vsize = temp;
    p_data += 2;

    temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
    s_display_cfg.input[layer].hsize = temp;
    p_data += 2;

    temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
    s_display_cfg.input[layer].coordinate.x = temp;
    p_data += 2;

    temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
    s_display_cfg.input[layer].coordinate.y = temp;
    p_data += 2;

    s_display_cfg.input[layer].format = (glcdc_in_format_t) *p_data;

    bits_per_pixel = r_get_display_pixel_size(s_display_cfg.input[layer].format);
    line_offset    = (int32_t)(((((s_display_cfg.input[layer].hsize * bits_per_pixel) / 8) + 63) / 64) * 64);

    s_display_cfg.input[layer].offset = line_offset;

}
/**********************************************************************************************************************
 * End of function r_set_display_param_layer
 *********************************************************************************************************************/


/* Function Name: r_set_display_param_procedure */
/*******************************************************************************************************************//**
 * @brief Set procedure parameter.
 * @param[in] p_data  Pointer to receive data.
 * @retval None
 **********************************************************************************************************************/
static void r_set_display_param_procedure(uint8_t * p_data)
{
    /* Correction circuit sequence */
    s_display_cfg.output.correction_proc_order = (glcdc_correction_proc_order_t) *p_data;

}
/**********************************************************************************************************************
 * End of function r_set_display_param_procedure
 *********************************************************************************************************************/


/* Function Name: r_set_display_param_dither */
/*******************************************************************************************************************//**
 * @brief Set dithering parameter.
 * @param[in] p_data  Pointer to receive data.
 * @retval None
 **********************************************************************************************************************/
static void r_set_display_param_dither(uint8_t * p_data)
{
    /* Dithering */
    s_display_cfg.output.dithering.dithering_mode = (glcdc_dithering_mode_t) *p_data;
    p_data++;

    s_display_cfg.output.dithering.dithering_pattern_a = (glcdc_dithering_pattern_t) *p_data;
    p_data++;

    s_display_cfg.output.dithering.dithering_pattern_b = (glcdc_dithering_pattern_t) *p_data;
    p_data++;

    s_display_cfg.output.dithering.dithering_pattern_c = (glcdc_dithering_pattern_t) *p_data;
    p_data++;

    s_display_cfg.output.dithering.dithering_pattern_d = (glcdc_dithering_pattern_t) *p_data;
}
/**********************************************************************************************************************
 * End of function r_set_display_param_dither
 *********************************************************************************************************************/


/* Function Name: r_set_display_param_brightness */
/*******************************************************************************************************************//**
 * @brief Set brightness parameter.
 * @param[in] p_data  Pointer to receive data.
 * @retval None
 **********************************************************************************************************************/
static void r_set_display_param_brightness(uint8_t * p_data)
{
    uint16_t temp;

    /* Brightness */
    temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
    s_display_cfg.output.brightness.r = temp;
    p_data += 2;

    temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
    s_display_cfg.output.brightness.g = temp;
    p_data += 2;

    temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
    s_display_cfg.output.brightness.b = temp;

}
/**********************************************************************************************************************
 * End of function r_set_display_param_brightness
 *********************************************************************************************************************/


/* Function Name: r_set_display_param_contrast */
/*******************************************************************************************************************//**
 * @brief Set contrast parameter.
 * @param[in] p_data  Pointer to receive data.
 * @retval None
 **********************************************************************************************************************/
static void r_set_display_param_contrast(uint8_t * p_data)
{
    /* Contrast */
    s_display_cfg.output.contrast.r = *p_data;
    p_data++;

    s_display_cfg.output.contrast.g = *p_data;
    p_data++;

    s_display_cfg.output.contrast.b = *p_data;

}
/**********************************************************************************************************************
 * End of function r_set_display_param_contrast
 *********************************************************************************************************************/


/* Function Name: r_set_display_param_gammma */
/*******************************************************************************************************************//**
 * @brief Set gammma parameter.
 * @param[in] p_data  Pointer to receive data.
 * @retval None
 **********************************************************************************************************************/
static void r_set_display_param_gammma(uint8_t * p_data)
{
    uint16_t temp;
    volatile uint8_t i;

    /* WAIT_LOOP */
    for (i = 0; i < 15; i++)
    {
        temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
        s_display_gamma_table_r.threshold[i] = temp;
        p_data += 2;
    }

    /* WAIT_LOOP */
    for (i = 0; i < 16; i++)
    {
        temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
        s_display_gamma_table_r.gain[i] = temp;
        p_data += 2;
    }

    /* WAIT_LOOP */
    for (i = 0; i < 15; i++)
    {
        temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
        s_display_gamma_table_g.threshold[i] = temp;
        p_data += 2;
    }

    /* WAIT_LOOP */
    for (i = 0; i < 16; i++)
    {
        temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
        s_display_gamma_table_g.gain[i] = temp;
        p_data += 2;
    }

    /* WAIT_LOOP */
    for (i = 0; i < 15; i++)
    {
        temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
        s_display_gamma_table_b.threshold[i] = temp;
        p_data += 2;
    }

    /* WAIT_LOOP */
    for (i = 0; i < 16; i++)
    {
        temp = (uint16_t) (((*(p_data + 1)) << 8) | (*p_data));
        s_display_gamma_table_b.gain[i] = temp;
        p_data += 2;
    }

    s_display_cfg.output.gamma.p_r = (gamma_correction_t*) &s_display_gamma_table_r;
    s_display_cfg.output.gamma.p_g = (gamma_correction_t*) &s_display_gamma_table_g;
    s_display_cfg.output.gamma.p_b = (gamma_correction_t*) &s_display_gamma_table_b;

}
/**********************************************************************************************************************
 * End of function r_set_display_param_gammma
 *********************************************************************************************************************/


/* Function Name: r_set_display_params */
/*******************************************************************************************************************//**
 * @brief Set glcdc parameter.
 * @param[in] command  Receive command.
 * @retval None
 **********************************************************************************************************************/
static void r_set_display_params(uint8_t command)
{
    uint8_t * p_buff;

    p_buff = r_get_uart_buffer_pointer();

    switch (command)
    {
        case QE_DISPLAY_COMMAND_ALL:
            r_set_display_param_timing(p_buff);
            p_buff += QE_DISPLAY_SIZE_REV_COMMAND_TIMING;
            r_set_display_param_output(p_buff);
            p_buff += QE_DISPLAY_SIZE_REV_COMMAND_OUTCTL;
            r_set_display_param_layer(p_buff, GLCDC_FRAME_LAYER_1);
            p_buff += QE_DISPLAY_SIZE_REV_COMMAND_LAYER1;
            r_set_display_param_layer(p_buff, GLCDC_FRAME_LAYER_2);
            p_buff += QE_DISPLAY_SIZE_REV_COMMAND_LAYER2;
            r_set_display_param_procedure(p_buff);
            p_buff += QE_DISPLAY_SIZE_REV_COMMAND_PROC_ORDER;
            r_set_display_param_dither(p_buff);
            p_buff += QE_DISPLAY_SIZE_REV_COMMAND_DITHER;
            r_set_display_param_brightness(p_buff);
            p_buff += QE_DISPLAY_SIZE_REV_COMMAND_BRIGHTNESS;
            r_set_display_param_contrast(p_buff);
            p_buff += QE_DISPLAY_SIZE_REV_COMMAND_CONTRAST;
            r_set_display_param_gammma(p_buff);
            break;

        case QE_DISPLAY_COMMAND_TIMING:
            r_set_display_param_timing(p_buff);
            break;

        case QE_DISPLAY_COMMAND_OUTCTL:
            r_set_display_param_output(p_buff);
            break;

        case QE_DISPLAY_COMMAND_LAYER1:
            r_set_display_param_layer(p_buff, GLCDC_FRAME_LAYER_1);
            break;

        case QE_DISPLAY_COMMAND_LAYER2:
            r_set_display_param_layer(p_buff, GLCDC_FRAME_LAYER_2);
            break;

        case QE_DISPLAY_COMMAND_PROCIDURE:
            r_set_display_param_procedure(p_buff);
            break;

        case QE_DISPLAY_COMMAND_DITHER:
            r_set_display_param_dither(p_buff);
            break;

        case QE_DISPLAY_COMMAND_BRIGHTNESS:
            r_set_display_param_brightness(p_buff);
            break;

        case QE_DISPLAY_COMMAND_CONTRAST:
            r_set_display_param_contrast(p_buff);
            break;

        case QE_DISPLAY_COMMAND_GAMMA:
            r_set_display_param_gammma(p_buff);
            break;

        default:
            R_BSP_NOP();
    }

}
/**********************************************************************************************************************
 * End of function r_set_display_params
 *********************************************************************************************************************/


/* Function Name: r_display_print_message */
/*******************************************************************************************************************//**
 * @brief Adjustment error message output processing.
 * @param[in] uint8_t     DISPLAY FIT err return code.
 * @retval None
 **********************************************************************************************************************/
static void r_display_print_message(uint8_t response)
{

    switch (response)
    {
        case QE_DISPLAY_RES_ERR_INVALID_ARG:
            r_debug_print("Adjustment Error: Invalid parameter or disable layer!!\n");
            break;

        case QE_DISPLAY_RES_ERR_INVALID_TIMING_SETTING:
            r_debug_print("Adjustment Error: Invalid timing setting!!\n");
            break;

        case QE_DISPLAY_RES_ERR_INVALID_LAYER_SETTING:
            r_debug_print("Adjustment Error: Invalid layer setting!!\n");
            break;

        case QE_DISPLAY_RES_ERR_INVALID_ALIGNMENT:
            r_debug_print("Adjustment Error: Invalid graphic layer base address alignment!!\n");
            break;

        case QE_DISPLAY_RES_ERR_INVALID_GAMMA_SETTING:
            r_debug_print("Adjustment Error: Invalid gamma setting!!\n");
            break;

        case QE_DISPLAY_RES_ERR_INVALID_BLEND_SETTING:
            r_debug_print("Adjustment Error: Invalid blend setting!!\n");
            break;

        case QE_DISPLAY_RES_NACK:
            r_debug_print("Adjustment Error: unknown error!!\n");
            break;

        default:
            R_BSP_NOP();

    }

}
/**********************************************************************************************************************
 * End of function r_display_print_message
 *********************************************************************************************************************/


/* Function Name: r_display_check_err_response */
/*******************************************************************************************************************//**
 * @brief Adjustment error response check.
 * @param[in] glcdc_ret     GLCDC FIT API return value.
 * @retval Error code.
 **********************************************************************************************************************/
static uint8_t r_display_check_err_response(glcdc_err_t glcdc_ret)
{
    uint8_t response;

    switch (glcdc_ret)
    {
        case GLCDC_ERR_INVALID_ARG:
            response = QE_DISPLAY_RES_ERR_INVALID_ARG;
            break;

        case GLCDC_ERR_INVALID_TIMING_SETTING:
            response = QE_DISPLAY_RES_ERR_INVALID_TIMING_SETTING;
            break;

        case GLCDC_ERR_INVALID_LAYER_SETTING:
            response = QE_DISPLAY_RES_ERR_INVALID_LAYER_SETTING;
            break;

        case GLCDC_ERR_INVALID_ALIGNMENT:
            response = QE_DISPLAY_RES_ERR_INVALID_ALIGNMENT;
            break;

        case GLCDC_ERR_INVALID_GAMMA_SETTING:
            response = QE_DISPLAY_RES_ERR_INVALID_GAMMA_SETTING;
            break;

        case GLCDC_ERR_INVALID_BLEND_SETTING:
            response = QE_DISPLAY_RES_ERR_INVALID_BLEND_SETTING;
            break;

        default:
            response = QE_DISPLAY_RES_NACK;
    }

    return response;
}
/**********************************************************************************************************************
 * End of function r_display_check_err_response
 *********************************************************************************************************************/


/* Function Name: r_get_display_pixel_size */
/*******************************************************************************************************************//**
 * @brief Get pixel size.
 * @param[in] format    Color format in use.
 * @retval Pixel size.
 **********************************************************************************************************************/
static int32_t r_get_display_pixel_size(glcdc_in_format_t format)
{
    int32_t bits;

    switch (format)
    {
        case GLCDC_IN_FORMAT_32BITS_ARGB8888:
        case GLCDC_IN_FORMAT_32BITS_RGB888:

            bits = 32;

            break;

        case GLCDC_IN_FORMAT_16BITS_RGB565:
        case GLCDC_IN_FORMAT_16BITS_ARGB1555:
        case GLCDC_IN_FORMAT_16BITS_ARGB4444:

            bits = 16;

            break;

        case GLCDC_IN_FORMAT_CLUT8:

            bits = 8;

            break;

        case GLCDC_IN_FORMAT_CLUT4:

            bits = 4;

            break;

        case GLCDC_IN_FORMAT_CLUT1:

            bits = 1;

            break;

        default:

            bits = 0;
    }

    return bits;
}
/**********************************************************************************************************************
 * End of function r_get_display_pixel_size
 *********************************************************************************************************************/


/* Function Name: r_display_read_clut */
/*******************************************************************************************************************//**
 * @brief GLCDC clut read.
 * @param[in] p_clut_tbl    Pointer to the variable that stores the read CLUT.
 * @param[in] layer         CLUT layer to read.
 * @retval None
 **********************************************************************************************************************/
static void r_display_read_clut(uint32_t * p_clut_tbl, glcdc_frame_layer_t layer)
{
    uint8_t sel;
    uint32_t * clut_table;
    uint16_t i;

    /* Read CLUT of graphic 1 */
    if (GLCDC_FRAME_LAYER_1 == layer)
    {
        sel = (uint8_t) GLCDC.GR1CLUTINT.BIT.SEL;

        if (0 == sel)
        {
            clut_table = (uint32_t*) &GLCDC.GR1CLUT0[0].LONG;

            /* WAIT_LOOP */
            for (i = 0; i < 256; i++)
            {
                *(p_clut_tbl + i) = *(clut_table + i);
            }
        }
        else
        {
            clut_table = (uint32_t*) &GLCDC.GR1CLUT1[0].LONG;

            /* WAIT_LOOP */
            for (i = 0; i < 256; i++)
            {
                *(p_clut_tbl + i) = *(clut_table + i);
            }
        }
    }
    else /* Read CLUT of graphic 2 */
    {
        sel = (uint8_t) GLCDC.GR2CLUTINT.BIT.SEL;

        if (0 == sel)
        {
            clut_table = (uint32_t*) &GLCDC.GR2CLUT0[0].LONG;

            /* WAIT_LOOP */
            for (i = 0; i < 256; i++)
            {
                *(p_clut_tbl + i) = *(clut_table + i);
            }
        }
        else
        {
            clut_table = (uint32_t*) &GLCDC.GR2CLUT1[0].LONG;

            /* WAIT_LOOP */
            for (i = 0; i < 256; i++)
            {
                *(p_clut_tbl + i) = *(clut_table + i);
            }
        }
    }

}
/**********************************************************************************************************************
 * End of function r_display_read_clut
 *********************************************************************************************************************/

/* Function Name: r_display_check_glcdc_reflection */
/*******************************************************************************************************************//**
 * @brief GLCDC register setting reflection check.
 * @param None
 * @retval true   Register update is underway.
 * @retval false  Register update is not underway.
 **********************************************************************************************************************/
static bool r_display_check_glcdc_reflection(void)
{
    bool flag;

    flag  = (bool) GLCDC.BGEN.BIT.VEN;
    flag |= (bool) GLCDC.GR1VEN.BIT.VEN;
    flag |= (bool) GLCDC.GR2VEN.BIT.VEN;
    flag |= (bool) GLCDC.OUTVEN.BIT.VEN;
    flag |= (bool) GLCDC.GAMGVEN.BIT.VEN;
    flag |= (bool) GLCDC.GAMBVEN.BIT.VEN;
    flag |= (bool) GLCDC.GAMRVEN.BIT.VEN;

    return flag;
}
/**********************************************************************************************************************
 * End of function r_display_check_glcdc_reflection
 *********************************************************************************************************************/


/* Function Name: r_display_backup_setting */
/*******************************************************************************************************************//**
 * @brief Back up the settings that were working before adjustment.
 * @param None
 * @retval None
 **********************************************************************************************************************/
static void r_display_backup_setting(void)
{
    if (NULL != s_display_cfg.input[GLCDC_FRAME_LAYER_1].p_base)
    {
        /* Backup the frame buffer address */
        s_display_cfg.input[GLCDC_FRAME_LAYER_1].p_base = (uint32_t*) GLCDC.GR1FLM2;

        /* Backup the clut data */
        if (GLCDC_IN_FORMAT_CLUT8 <= s_display_cfg.input[GLCDC_FRAME_LAYER_1].format)
        {
            r_display_read_clut(s_display_clut1, GLCDC_FRAME_LAYER_1);
        }

    }

    if (NULL != s_display_cfg.input[GLCDC_FRAME_LAYER_2].p_base)
    {
        /* Backup the frame buffer address */
        s_display_cfg.input[GLCDC_FRAME_LAYER_2].p_base = (uint32_t*) GLCDC.GR2FLM2;

        /* Backup the clut data */
        if (GLCDC_IN_FORMAT_CLUT8 <= s_display_cfg.input[GLCDC_FRAME_LAYER_2].format)
        {
            r_display_read_clut(s_display_clut2, GLCDC_FRAME_LAYER_2);
        }

    }

    /* Backup the cfg */
    s_display_backup_cfg = s_display_cfg;

    /* Backup the gammma table */
    s_display_gamma_backup_table_r = s_display_gamma_table_r;
    s_display_gamma_backup_table_g = s_display_gamma_table_g;
    s_display_gamma_backup_table_b = s_display_gamma_table_b;

}
/**********************************************************************************************************************
 * End of function r_display_backup_setting
 *********************************************************************************************************************/


/* Function Name: r_display_modf_qe */
/*******************************************************************************************************************//**
 * @brief Split into integer and decimal parts.
 * @param[in]  x        Float value to split.
 * @param[in]  * ip     Pointer to store the split integer part.
 * @retval     Split decimal part.
 **********************************************************************************************************************/
static float r_display_modf_qe(float x, float * ip)
{
    float absx    = x >= 0 ? x : -x;
    uint32_t absi = (uint32_t) absx;
    float absf    = absx - (float) absi;

    if (x < 0)
    {
        *ip = (float) -absi;
        return -absf;
    }
    else
    {
        *ip = (float) absi;
        return absf;
    }
}
/**********************************************************************************************************************
 End of function r_display_modf_qe
 *********************************************************************************************************************/


/* Function Name: r_display_floorf_qe */
/*******************************************************************************************************************//**
 * @brief Truncation after decimal point.
 * @param[in]  x       Float value rounded down to the decimal point.
 * @retval     Integer value
 **********************************************************************************************************************/
static float r_display_floorf_qe(float x)
{
    if (x < 0)
    {
        if (r_display_modf_qe(-x, &x) >= FLT_EPSILON)
        {
            x++;
        }
        return x;
    }
    else
    {
        r_display_modf_qe(x, &x);
        return x;
    }
}
/**********************************************************************************************************************
 End of function r_display_floorf_qe
 *********************************************************************************************************************/


/* Function Name: r_display_draw_32bit_pixel */
/*******************************************************************************************************************//**
 * @brief Draws the frame in the specified color of 32bit/pixel data.
 * @param[in]  color        32bit/pixel Color data
 * @param[in]  p_config     Pointer to display configuration struct
 * @retval     None
 **********************************************************************************************************************/
static void r_display_draw_32bit_pixel(uint32_t color, glcdc_input_cfg_t * p_config)
{
    uint32_t * p_pixel_addr;
    volatile uint32_t i;
    volatile uint32_t j;

    int32_t  line_offset  = p_config->offset;
    uint16_t image_width  = p_config->hsize;
    uint16_t image_height = p_config->vsize;

    /* Get the top address of the frame buffer */
    p_pixel_addr = (uint32_t*) p_config->p_base;

    /* Top line , WAIT_LOOP */
    for (i = 0; i < (line_offset / 4); i++)
    {
        *p_pixel_addr++ = color;
    }

    /* WAIT_LOOP */
    for (i = 0; i < (image_height - 2); i++)
    {
        for (j = 0; j < (line_offset / 4); j++)
        {
            /* Edge pixel */
            if ((0 == j) || ((image_width - 1) == j))
            {
                *p_pixel_addr++ = color;
            }
            else
            {
                p_pixel_addr++;
            }
        }
    }

    /* End line , WAIT_LOOP */
    for (i = 0; i < (line_offset / 4); i++)
    {
        *p_pixel_addr++ = color;
    }
}
/**********************************************************************************************************************
 End of function r_display_draw_32bit_pixel
 *********************************************************************************************************************/


/* Function Name: r_display_draw_16bit_pixel */
/*******************************************************************************************************************//**
 * @brief Draws the frame in the specified color of 16bit/pixel data.
 * @param[in]  color        16bit/pixel Color data
 * @param[in]  p_config     Pointer to display configuration struct
 * @retval     None
 **********************************************************************************************************************/
static void r_display_draw_16bit_pixel(uint16_t color, glcdc_input_cfg_t * p_config)
{
    uint16_t * p_pixel_addr;
    volatile uint32_t i;
    volatile uint32_t j;

    int32_t  line_offset  = p_config->offset;
    uint16_t image_width  = p_config->hsize;
    uint16_t image_height = p_config->vsize;

    /* Get the top address of the frame buffer */
    p_pixel_addr = (uint16_t*) p_config->p_base;

    /* Top line , WAIT_LOOP */
    for (i = 0; i < (line_offset / 2); i++)
    {
        *p_pixel_addr++ = color;
    }

    /* WAIT_LOOP */
    for (i = 0; i < (image_height - 2); i++)
    {
        for (j = 0; j < (line_offset / 2); j++)
        {
#ifdef __BIG
            /*  Edge pixel (big endian) */
            if ((1 == j) || ((image_width - 2) == j))
            {
                *p_pixel_addr++ = color;
            }
#else
            /*  Edge pixel (little endian) */
            if ((0 == j) || ((image_width - 1) == j))
            {
                *p_pixel_addr++ = color;
            }
#endif
            else
            {
                p_pixel_addr++;
            }
        }
    }

    /* End line , WAIT_LOOP */
    for (i = 0; i < (line_offset / 2); i++)
    {
        *p_pixel_addr++ = color;
    }
}
/**********************************************************************************************************************
 End of function r_display_draw_16bit_pixel
 *********************************************************************************************************************/


/* Function Name: r_display_draw_8bit_pixel */
/*******************************************************************************************************************//**
 * @brief Draws the frame in the specified color of 8bit/pixel data.
 * @param[in]  color        8bit/pixel Color data
 * @param[in]  p_config     Pointer to display configuration struct
 * @retval     None
 **********************************************************************************************************************/
static void r_display_draw_8bit_pixel(uint8_t color, glcdc_input_cfg_t * p_config)
{
    uint8_t * p_pixel_addr;
    volatile uint32_t i;

    int32_t  line_offset  = p_config->offset;
    uint16_t image_width  = p_config->hsize;
    uint16_t image_height = p_config->vsize;

    /* Get the top address of the frame buffer */
    p_pixel_addr = (uint8_t*) p_config->p_base;

    /* Top line , WAIT_LOOP */
    for (i = 0; i < line_offset; i++)
    {
        *p_pixel_addr++ = color;
    }

    /* WAIT_LOOP */
    for (i = 0; i < (image_height - 2); i++)
    {
#ifdef __BIG
        /*  Edge pixel (big endian) */
        *(p_pixel_addr + 3) = color;

        switch((image_width) % 4)
        {
            case 0:
                *(p_pixel_addr + (image_width - 1) - 3) = color;
                break;
            case 2:
                *(p_pixel_addr + (image_width - 1) + 1) = color;
                break;
        }

#else
        /*  Edge pixel (little endian) */
        *p_pixel_addr = color;
        *(p_pixel_addr + (image_width - 1)) = color;
#endif /* End of #ifdef _BIG */
        p_pixel_addr += (sizeof(uint8_t) * line_offset);
    }

    /* End line , WAIT_LOOP */
    for (i = 0; i < line_offset; i++)
    {
        *p_pixel_addr++ = color;
    }
}
/**********************************************************************************************************************
 End of function r_display_draw_8bit_pixel
 *********************************************************************************************************************/


/* Function Name: r_display_draw_4bit_pixel */
/*******************************************************************************************************************//**
 * @brief Draws the frame in the specified color of 4bit/pixel data.
 * @param[in]  color        4bit/pixel Color data
 * @param[in]  p_config     Pointer to display configuration struct
 * @retval     None
 **********************************************************************************************************************/
static void r_display_draw_4bit_pixel(uint8_t color, glcdc_input_cfg_t * p_config)
{
    uint8_t * p_pixel_addr;
    volatile uint32_t i;
    uint8_t color_upper_4bit      = (color & 0x0F) << 4;
    uint8_t color_mask_upper_4bit = 0x0F;
    uint8_t color_lower_4bit      = color & 0x0F;
    uint8_t color_mask_lower_4bit = 0xF0;

    int32_t  line_offset  = p_config->offset;
    uint16_t image_width  = p_config->hsize;
    uint16_t image_height = p_config->vsize;

    /* Get the top address of the frame buffer */
    p_pixel_addr = (uint8_t*) p_config->p_base;

    /* Top line , WAIT_LOOP */
    for (i = 0; i < line_offset; i++)
    {
        *p_pixel_addr++ = (color_upper_4bit | color_lower_4bit);
    }

    /* WAIT_LOOP */
    for (i = 0; i < (image_height - 2); i++)
    {
#ifdef __BIG
        /*  Edge pixel (big endian) */
        *(p_pixel_addr + 3) = (*(p_pixel_addr + 3) & color_mask_lower_4bit) | color_lower_4bit;

        switch((image_width/2) % 4)
        {
            case 0:
                *(p_pixel_addr + ((image_width/2) - 1) - 3) =
                        (*(p_pixel_addr + ((image_width/2) - 1) - 3) & color_mask_upper_4bit) | color_upper_4bit;
                break;
            case 1:
                *(p_pixel_addr + ((image_width/2) - 1) + 3) =
                        (*(p_pixel_addr + ((image_width/2) - 1) + 3) & color_mask_upper_4bit) | color_upper_4bit;
                break;
            case 2:
                *(p_pixel_addr + ((image_width/2) - 1) + 1) =
                        (*(p_pixel_addr + ((image_width/2) - 1) + 1) & color_mask_upper_4bit) | color_upper_4bit;
                break;
            case 3:
                *(p_pixel_addr + ((image_width/2) - 1) - 1) =
                        (*(p_pixel_addr + ((image_width/2) - 1) - 1) & color_mask_upper_4bit) | color_upper_4bit;
                break;
        }

#else
        /*  Edge pixel (little endian) */
        *p_pixel_addr = ( *p_pixel_addr & color_mask_lower_4bit) | color_lower_4bit;
        *(p_pixel_addr + ((image_width / 2) - 1)) = ( *(p_pixel_addr + ((image_width / 2) - 1)) & color_mask_upper_4bit)
                | color_upper_4bit;
#endif
        p_pixel_addr += (sizeof(uint8_t) * line_offset);

    }

    /* End line , WAIT_LOOP */
    for (i = 0; i < line_offset; i++)
    {
        *p_pixel_addr++ = (color_upper_4bit | color_lower_4bit);
    }
}
/**********************************************************************************************************************
 End of function r_display_draw_4bit_pixel
 *********************************************************************************************************************/


/* Function Name: r_display_draw_1bit_pixel */
/*******************************************************************************************************************//**
 * @brief Draws the frame in the specified color of 1bit/pixel data.
 * @param[in]  color        1bit/pixel Color data
 * @param[in]  p_config     Pointer to display configuration struct
 * @retval     None
 **********************************************************************************************************************/
static void r_display_draw_1bit_pixel(uint8_t color, glcdc_input_cfg_t * p_config)
{
    uint8_t * p_pixel_addr;
    volatile uint32_t i;
    uint8_t color_data     = (color > 0) ? 0x1 : 0x0;
    uint8_t color_byte_all = (color > 0) ? 0xFF : 0x00;

    int32_t  line_offset  = p_config->offset;
    uint16_t image_width  = p_config->hsize;
    uint16_t image_height = p_config->vsize;

    /* Get the top address of the frame buffer */
    p_pixel_addr = (uint8_t*) p_config->p_base;

    /* Top line , WAIT_LOOP */
    for (i = 0; i < line_offset; i++)
    {
        *p_pixel_addr++ = color_byte_all;
    }

    /* WAIT_LOOP */
    for (i = 0; i < (image_height - 2); i++)
    {
        uint8_t target_byte = (uint8_t) ((image_width + 7) / 8);
        uint8_t color_shift_data = (uint8_t) (color_data << ((image_width + 7) % 8));
#ifdef __BIG
        /*  Edge pixel (big endian) */
        *(p_pixel_addr + 3) = *(p_pixel_addr + 3) | color_data;

        switch(target_byte % 4)
        {
            case 0:
                *(p_pixel_addr + (target_byte - 1) - 3) =
                        *(p_pixel_addr + (target_byte - 1) - 3) | color_shift_data;
                break;
            case 1:
                *(p_pixel_addr + (target_byte - 1) + 3) =
                        *(p_pixel_addr + (target_byte - 1) + 3) | color_shift_data;
                break;
            case 2:
                *(p_pixel_addr + (target_byte - 1) + 1) =
                        *(p_pixel_addr + (target_byte - 1) + 1) | color_shift_data;
                break;
            case 3:
                *(p_pixel_addr + (target_byte - 1) - 1) =
                        *(p_pixel_addr + (target_byte - 1) - 1) | color_shift_data;
                break;
        }

#else
        /*  Edge pixel (little endian) */
        *p_pixel_addr = (*p_pixel_addr) | color_data;
        *(p_pixel_addr + (target_byte - 1)) = *(p_pixel_addr + (target_byte - 1)) | color_shift_data;
#endif
        p_pixel_addr += (sizeof(uint8_t) * line_offset);

    }

    /* End line , WAIT_LOOP */
    for (i = 0; i < line_offset; i++)
    {
        *p_pixel_addr++ = color_byte_all;
    }
}
/**********************************************************************************************************************
 End of function r_display_draw_1bit_pixel
 *********************************************************************************************************************/

/* Function Name: r_display_draw_red_frame */
/*******************************************************************************************************************//**
 * @brief Draw Red frame.
 * @param[in]  p_config     Pointer to display configuration struct
 * @retval     None
 **********************************************************************************************************************/
static void r_display_draw_red_frame(glcdc_input_cfg_t * p_config)
{
    if ((GLCDC_IN_FORMAT_32BITS_ARGB8888 == p_config->format) || (GLCDC_IN_FORMAT_32BITS_RGB888 == p_config->format))
    {
        r_display_draw_32bit_pixel(PIXEL_COLOR_RED_ARGB8888, p_config);
    }
    else if (GLCDC_IN_FORMAT_16BITS_RGB565 == p_config->format)
    {
        r_display_draw_16bit_pixel(PIXEL_COLOR_RED_RGB565, p_config);
    }
    else if (GLCDC_IN_FORMAT_16BITS_ARGB1555 == p_config->format)
    {
        r_display_draw_16bit_pixel(PIXEL_COLOR_RED_ARGB1555, p_config);
    }
    else if (GLCDC_IN_FORMAT_16BITS_ARGB4444 == p_config->format)
    {
        r_display_draw_16bit_pixel(PIXEL_COLOR_RED_ARGB4444, p_config);
    }
    else if (GLCDC_IN_FORMAT_CLUT8 == p_config->format)
    {
        r_display_draw_8bit_pixel(PIXEL_COLOR_RED_CLUT8, p_config);
    }
    else if (GLCDC_IN_FORMAT_CLUT4 == p_config->format)
    {
        r_display_draw_4bit_pixel(PIXEL_COLOR_RED_CLUT4, p_config);
    }
    else if (GLCDC_IN_FORMAT_CLUT1 == p_config->format)
    {
        r_display_draw_1bit_pixel(PIXEL_COLOR_WHITE_CLUT1, p_config);
    }
    else
    {
        /* Do Nothing */
    }
}
/**********************************************************************************************************************
 End of function r_display_draw_red_frame
 *********************************************************************************************************************/

/* Function Name: r_display_hsv2argb */
/*******************************************************************************************************************//**
 * @brief  Converts float HSV (0.0-1.0) to RGB888
 * @param[in]    h  hue
 * @param[in]    s  saturation
 * @param[in]    v  value
 * @param[in]    a  alpha
 * @param[in]    format  desired color format
 * @retval  Resulting pixel color (may need to be cast to size)
 **********************************************************************************************************************/
static uint32_t r_display_hsv2argb(float h, float s, float v, float a, glcdc_in_format_t format)
{
    uint32_t r;
    uint32_t g;
    uint32_t b;
    uint32_t alpha;

    if (s <= 0.0F)                     /* "<=" instead of "==" to avoid pedantic GCC warning */
    {
        /* monochrome */
        r = (uint16_t) (v * 0xFF);     /* NOLINT(readability-magic-numbers) */
        b = (uint16_t) (v * 0xFF);     /* NOLINT(readability-magic-numbers) */
        g = (uint16_t) (v * 0xFF);     /* NOLINT(readability-magic-numbers) */
    }
    else
    {
        if (h >= 1.0F)
        {
            h = h - r_display_floorf_qe(h);
        }

        if (s > 1.0F)
        {
            s = s - r_display_floorf_qe(s);
        }

        if (v > 1.0F)
        {
            v = v - r_display_floorf_qe(v);
        }

        h *= 6.0F;                     // NOLINT(readability-magic-numbers)

        if (0 == (int) h)
        {
            r = (uint32_t) (0xFF * v);                           // NOLINT(readability-magic-numbers)
            g = (uint32_t) (0xFF * v * (1.0F - s * (1.0F - h))); // NOLINT(readability-magic-numbers)
            b = (uint32_t) (0xFF * v * (1.0F - s));              // NOLINT(readability-magic-numbers)
        }
        else if (1 == (int) h)
        {
            r = (uint32_t) (0xFF * v * (1.0F - s * (h - 1.0F))); // NOLINT(readability-magic-numbers)
            g = (uint32_t) (0xFF * v);                           // NOLINT(readability-magic-numbers)
            b = (uint32_t) (0xFF * v * (1.0F - s));              // NOLINT(readability-magic-numbers)
        }
        else if (2 == (int) h)
        {
            r = (uint32_t) (0xFF * v * (1.0F - s));                       // NOLINT(readability-magic-numbers)
            g = (uint32_t) (0xFF * (v));                                  // NOLINT(readability-magic-numbers)
            b = (uint32_t) (0xFF * v * (1.0F - s * (1.0F - (h - 2.0F)))); // NOLINT(readability-magic-numbers)
        }
        else if (3 == (int) h)
        {
            r = (uint32_t) (0xFF * v * (1.0F - s));              // NOLINT(readability-magic-numbers)
            g = (uint32_t) (0xFF * v * (1.0F - s * (h - 3.0F))); // NOLINT(readability-magic-numbers)
            b = (uint32_t) (0xFF * (v));                         // NOLINT(readability-magic-numbers)
        }
        else if (4 == (int) h)
        {
            r = (uint32_t) (0xFF * v * (1.0F - s * (1.0F - (h - 4.0F)))); // NOLINT(readability-magic-numbers)
            g = (uint32_t) (0xFF * v * (1.0F - s));                       // NOLINT(readability-magic-numbers)
            b = (uint32_t) (0xFF * (v));                                  // NOLINT(readability-magic-numbers)
        }
        else if (5 == (int) h)
        {
            r = (uint32_t) (0xFF * (v));                         // NOLINT(readability-magic-numbers)
            g = (uint32_t) (0xFF * v * (1.0F - s));              // NOLINT(readability-magic-numbers)
            b = (uint32_t) (0xFF * v * (1.0F - s * (h - 5.0F))); // NOLINT(readability-magic-numbers)
        }
        else
        {
            r = 0xFF;              // NOLINT(readability-magic-numbers)
            g = 0xFF;              // NOLINT(readability-magic-numbers)
            b = 0xFF;              // NOLINT(readability-magic-numbers)
        }
    }

    if (a > 1.0F)
    {
        a = a - r_display_floorf_qe(a);
    }

    alpha = (uint32_t) (a * 0xFFU);

    if ((GLCDC_IN_FORMAT_32BITS_ARGB8888 == format) || (GLCDC_IN_FORMAT_32BITS_RGB888 == format))
    {
        return (alpha << 24) + (r << 16) + (g << 8) + b;
    }
    else if (GLCDC_IN_FORMAT_16BITS_ARGB4444 == format)
    {
        return ((alpha >> 4) << 12) + ((r >> 4) << 8) + ((g >> 4) << 4) + (b >> 4);
    }
    else if (GLCDC_IN_FORMAT_16BITS_ARGB1555 == format)
    {
        return ((alpha >> 7) << 15) + ((r >> 3) << 10) + ((g >> 3) << 5) + (b >> 3);
    }
    else if (GLCDC_IN_FORMAT_16BITS_RGB565 == format)
    {
        return ((r >> 3) << 11) + ((g >> 2) << 5) + (b >> 3);
    }
    else if (GLCDC_IN_FORMAT_CLUT8 == format)
    {
        return ((r >> 5) << 5) + ((g >> 5) << 2) + (b >> 6); /* RGB332 */
    }
    else if (GLCDC_IN_FORMAT_CLUT4 == format)
    {
        return ((r >> 7) << 3) + ((g >> 6) << 1) + (b >> 7); /* RGB121 */
    }
    else if (GLCDC_IN_FORMAT_CLUT1 == format)
    {
        return (r + g + b) > ((0xFFU / 2) * 3) ? 1 : 0;
    }
    else
    {
        return 0x80808080;         // NOLINT(readability-magic-numbers)
    }
}
/**********************************************************************************************************************
 End of function r_display_hsv2argb
 *********************************************************************************************************************/

/* Function Name: r_display_put_pixel */
/*******************************************************************************************************************//**
 * @brief Utility function to draw a pixel on a frame buffer
 * @param[in]  p_config     Pointer to display configuration struct
 * @param[in]  x            X-coordinate
 * @param[in]  y            Y-coordinate
 * @param[in]  color        Pixel color value
 * @retval     None
 **********************************************************************************************************************/
static inline void r_display_put_pixel(glcdc_input_cfg_t * p_config, uint32_t x, uint32_t y, uint32_t color)
{
    uint32_t fg_hpix;
    uint8_t * fb = (uint8_t*) p_config->p_base;

    if ((GLCDC_IN_FORMAT_32BITS_ARGB8888 == p_config->format) || (GLCDC_IN_FORMAT_32BITS_RGB888 == p_config->format))
    {
        fg_hpix = p_config->offset / 4;
        *(((uint32_t*) fb) + x + (y * fg_hpix)) = color;

        return;
    }
    else if ((GLCDC_IN_FORMAT_16BITS_ARGB4444 == p_config->format)
            || (GLCDC_IN_FORMAT_16BITS_ARGB1555 == p_config->format)
            || (GLCDC_IN_FORMAT_16BITS_RGB565 == p_config->format))
    {
        fg_hpix = p_config->offset / 2;
#ifdef __BIG
        if ((x % 2) == 0)
        {
            *(((uint16_t *) fb) + (x + 1) + (y * fg_hpix)) = (uint16_t) color;
        }
        else
        {
            *(((uint16_t *) fb) + (x - 1) + (y * fg_hpix)) = (uint16_t) color;
        }
#else
        *(((uint16_t*) fb) + x + (y * fg_hpix)) = (uint16_t) color;
#endif
        return;
    }
    else if (GLCDC_IN_FORMAT_CLUT8 == p_config->format)
    {
        fg_hpix = p_config->offset;
#ifdef __BIG
        switch (x % 4)
        {
            case 0:
                *(fb + (x + 3) + (y * fg_hpix)) = (uint8_t) color;
                break;
            case 1:
                *(fb + (x + 1) + (y * fg_hpix)) = (uint8_t) color;
                break;
            case 2:
                *(fb + (x - 1) + (y * fg_hpix)) = (uint8_t) color;
                break;
            case 3:
                *(fb + (x - 3) + (y * fg_hpix)) = (uint8_t) color;
                break;
        }
#else
        *(fb + x + (y * fg_hpix)) = (uint8_t) color;
#endif

        return;
    }
    else if (GLCDC_IN_FORMAT_CLUT4 == p_config->format)
    {
        fg_hpix = p_config->offset;
        uint8_t color_4bit;
        uint8_t color_4bit_mask;
        uint8_t * ptr_clut4;
#ifdef __BIG
        switch ((x / 2) % 4)
        {
            case 0:
                ptr_clut4 = (fb + ((x / 2) + 3) + (y * (fg_hpix)));
                break;
            case 1:
                ptr_clut4 = (fb + ((x / 2) + 1) + (y * (fg_hpix)));
                break;
            case 2:
                ptr_clut4 = (fb + ((x / 2) - 1) + (y * (fg_hpix)));
                break;
            case 3:
                ptr_clut4 = (fb + ((x / 2) - 3) + (y * (fg_hpix)));
                break;
        }
#else
        ptr_clut4 = (fb + (x / 2) + (y * (fg_hpix)));
#endif

        if (x % 2 == 0)
        {
            color_4bit = (uint8_t) color;
            color_4bit_mask = 0xF0; // NOLINT(readability-magic-numbers)
        }
        else
        {
            color_4bit = (uint8_t) (color << 4);
            color_4bit_mask = 0x0F;
        }

        *ptr_clut4 &= color_4bit_mask;
        *ptr_clut4 |= color_4bit;

        return;
    }
    else if (GLCDC_IN_FORMAT_CLUT1 == p_config->format)
    {
        fg_hpix = p_config->offset;
        uint8_t color_1bit = (uint8_t) (color << (x % 8));
        uint8_t color_1bit_mask = (uint8_t) ~(1 << (x % 8));
        uint8_t * ptr_clut1;
#ifdef __BIG
        switch ((x / 8) % 4)
        {
            case 0:
                ptr_clut1 = (fb + ((x / 8) + 3) + (y * (fg_hpix)));
                break;
            case 1:
                ptr_clut1 = (fb + ((x / 8) + 1) + (y * (fg_hpix)));
                break;
            case 2:
                ptr_clut1 = (fb + ((x / 8) - 1) + (y * (fg_hpix)));
                break;
            case 3:
                ptr_clut1 = (fb + ((x / 8) - 3) + (y * (fg_hpix)));
                break;
        }
#else
        ptr_clut1 = (fb + (x / 8) + (y * (fg_hpix)));
#endif
        (*ptr_clut1) &= color_1bit_mask;
        (*ptr_clut1) |= color_1bit;

        return;
    }
    else
    {
        return;
    }
}
/**********************************************************************************************************************
 End of function r_display_put_pixel
 *********************************************************************************************************************/

/* Function Name: r_display_draw_test_pattern */
/*******************************************************************************************************************//**
 * @brief Utility function to draw a test pattern on a frame buffer
 * @param[in]  p_config     Pointer to display configuration struct
 * @param[in]  alpha        Alpha level to draw entire frame buffer at
 * @param[in]  clut         Pointer to color look-up table
 * @param[in]  grayscale    False to draw color pattern, true to draw grayscale gradient
 * @retval     None
 **********************************************************************************************************************/
static void r_display_draw_test_pattern(glcdc_input_cfg_t * p_config, uint8_t alpha, uint32_t * p_clut, bool grayscale)
{
    uint32_t y = 0;
    uint32_t fg_hpix = p_config->hsize;
    uint32_t fg_vpix = p_config->vsize;
    float alpha_f = (float) alpha / (float) 0xFFU;

    if (grayscale)
    {
        /* WAIT_LOOP */
        for (; y < fg_vpix; y++)
        {
            for (uint32_t x = 0; x < fg_hpix; x++)
            {
                r_display_put_pixel(
                        p_config,
                        x,
                        y,
                        r_display_hsv2argb(
                                0.0F,
                                0.0F,
                                (float) x / (float) fg_hpix,
                                alpha_f,
                                p_config->format));
            }
        }
    }
    else
    {
        /* draw color bars , WAIT_LOOP */
        for (; y < (fg_vpix / 8); y++)
        {
            for (uint32_t x = 0; x < fg_hpix; x++)
            {
                r_display_put_pixel(
                        p_config,
                        x,
                        y,
                        r_display_hsv2argb(
                                0.0F,
                                0.0F,
                                (float) x / (float) fg_hpix,
                                alpha_f,
                                p_config->format));
            }
        }

        /* WAIT_LOOP */
        for (; y < ((fg_vpix / 8) * 2); y++)
        {
            for (uint32_t x = 0; x < fg_hpix; x++)
            {
                r_display_put_pixel(
                        p_config,
                        x,
                        y,
                        r_display_hsv2argb(
                                0.0F,
                                1.0F,
                                (float) x / (float) fg_hpix,
                                alpha_f,
                                p_config->format));
            }
        }

        /* WAIT_LOOP */
        for (; y < ((fg_vpix / 8) * 3); y++)
        {
            for (uint32_t x = 0; x < fg_hpix; x++)
            {
                r_display_put_pixel(
                        p_config,
                        x,
                        y,
                        r_display_hsv2argb(
                                1.0F / 3.0F,
                                1.0F,
                                (float) x / (float) fg_hpix,
                                alpha_f,
                                p_config->format));
            }
        }

        /* WAIT_LOOP */
        for (; y < ((fg_vpix / 8) * 4); y++)
        {
            for (uint32_t x = 0; x < fg_hpix; x++)
            {
                r_display_put_pixel(
                        p_config,
                        x,
                        y,
                        r_display_hsv2argb(
                                2.0F / 3.0F,
                                1.0F,
                                (float) x / (float) fg_hpix,
                                alpha_f,
                                p_config->format));
            }
        }

        uint32_t y0 = y;

        /* WAIT_LOOP */
        for (; y < ((fg_vpix / 8) * 6); y++)
        {
            for (uint32_t x = 0; x < fg_hpix; x++)
            {
                r_display_put_pixel(
                        p_config,
                        x,
                        y,
                        r_display_hsv2argb(
                                (float) x / (float) fg_hpix,
                                1.0F,
                                (float) (y - y0) / (float) (fg_vpix / 4),
                                alpha_f,
                                p_config->format));
            }
        }

        y0 = y;

        /* WAIT_LOOP */
        for (; y < fg_vpix; y++)
        {
            for (uint32_t x = 0; x < fg_hpix; x++)
            {
                r_display_put_pixel(p_config,
                        x,
                        y,
                        r_display_hsv2argb(
                                (float) x / (float) fg_hpix,
                                ((float) (fg_vpix / 4) - (float) (y - y0)) / (float) (fg_vpix / 4),
                                1.0F,
                                alpha_f,
                                p_config->format));
            }
        }

        /* draw red frame */
        r_display_draw_red_frame(p_config);
    }

    if ((GLCDC_IN_FORMAT_CLUT8 <= p_config->format) || (GLCDC_IN_FORMAT_16BITS_ARGB1555 == p_config->format))
    {
        switch (p_config->format)
        {
            case GLCDC_IN_FORMAT_CLUT8:

                /* WAIT_LOOP */
                for (uint32_t i = 0; i < (0xFFU + 1); i++)
                {
                    *p_clut++ = (0xFFU << 24)
                            + (((i >> 5) * 0xFFU / 0x7U) << 16)
                            + ((((i >> 2) & 0x7U) * 0xFFU / 0x7U) << 8)
                            + ((i & 0x3U) * 0xFFU / 0x3U);
                }

                break;

            case GLCDC_IN_FORMAT_CLUT4:

                /* WAIT_LOOP */
                for (uint32_t i = 0; i < 0x10U; i++)
                {
                    *p_clut++ = (0xFFU << 24)
                            + (((i >> 3) * 0xFFU) << 16)
                            + ((((i >> 1) & 0x3U) * 0xFFU / 0x3U) << 8)
                            + ((i & 0x1U) * 0xFFU);
                }

                break;

            case GLCDC_IN_FORMAT_CLUT1:

                *p_clut++ = 0xFF000000;   /* NOLINT(readability-magic-numbers) */
                *p_clut   = 0xFFFFFFFF;   /* NOLINT(readability-magic-numbers) */
                break;

            case GLCDC_IN_FORMAT_16BITS_ARGB1555:

                /* Alpha refers to CLUT[0].A (when Alpha is 0) and CLUT[0x80].A (when Alpha is 1). */
                *p_clut          = 0x00000000;
                *(p_clut + 0x80) = 0xFF000000;
                break;

            default:
                break;
        }
    }
}
/**********************************************************************************************************************
 End of function r_display_draw_test_pattern
 *********************************************************************************************************************/

/* End of File */
