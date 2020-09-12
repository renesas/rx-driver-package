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
* Copyright (C) 2015-2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_simple_glcdc_config_rx.c
* Description  : RX Family Envision Kit Display simple initial settings.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 28.12.2019 1.01    First Release
***********************************************************************************************************************/

#include <stdio.h>
#include <string.h>

#include "platform.h"
#include "r_glcdc_rx_if.h"
#include "r_simple_glcdc_config_rx_if.h"
#include "r_glcdc_rx_pinset.h"

// Color depth
//
#define BITS_PER_PIXEL 8  // Allowed values: 1, 4, 8, 16, 32

//
// Buffer size and stride
//
#define BYTES_PER_LINE   ((BITS_PER_PIXEL * XSIZE_PHYS) / 8)
#define LINE_OFFSET      (((BYTES_PER_LINE + 63) / 64) * 64)
#define VXSIZE_PHYS      ((LINE_OFFSET * 8) / BITS_PER_PIXEL)
#define BYTES_PER_BUFFER (LINE_OFFSET * YSIZE_PHYS)

#pragma section _FRAME_BUFFER
uint8_t frame_buffer[(XSIZE_PHYS + XSIZE_OUTOF_DISPLAY) * YSIZE_PHYS * BITS_PER_PIXEL / 8];
#pragma section

typedef uint32_t LCD_COLOR;


typedef struct {
  LCD_COLOR Color;
  uint8_t Pos;
} LCD_X_SETLUTENTRY_INFO;

static void _VSYNC_ISR(void * p);
static void _SetLUTEntry(LCD_COLOR Color, uint8_t Pos);
static void _DisplayOnOff(int OnOff);

const uint32_t lcduteconf[256] =
{
		0xFF000000, 0xFF330000, 0xFF660000, 0xFF990000,
		0xFFCC0000, 0xFFFF0000, 0xFF003300, 0xFF333300,
		0xFF663300, 0xFF993300, 0xFFCC3300, 0xFFFF3300,
		0xFF006600, 0xFF336600, 0xFF666600, 0xFF996600,
		0xFFCC6600, 0xFFFF6600, 0xFF009900, 0xFF339900,
		0xFF669900, 0xFF999900, 0xFFCC9900, 0xFFFF9900,
		0xFF00CC00, 0xFF33CC00, 0xFF66CC00, 0xFF99CC00,
		0xFFCCCC00, 0xFFFFCC00, 0xFF00FF00, 0xFF33FF00,
		0xFF66FF00, 0xFF99FF00, 0xFFCCFF00, 0xFFFFFF00,
		0xFF000033, 0xFF330033, 0xFF660033, 0xFF990033,
		0xFFCC0033, 0xFFFF0033, 0xFF003333, 0xFF333333,
		0xFF663333, 0xFF993333, 0xFFCC3333, 0xFFFF3333,
		0xFF006633, 0xFF336633, 0xFF666633, 0xFF996633,
		0xFFCC6633, 0xFFFF6633, 0xFF009933, 0xFF339933,
		0xFF669933, 0xFF999933, 0xFFCC9933, 0xFFFF9933,
		0xFF00CC33, 0xFF33CC33, 0xFF66CC33, 0xFF99CC33,
		0xFFCCCC33, 0xFFFFCC33, 0xFF00FF33, 0xFF33FF33,
		0xFF66FF33, 0xFF99FF33, 0xFFCCFF33, 0xFFFFFF33,
		0xFF000066, 0xFF330066, 0xFF660066, 0xFF990066,
		0xFFCC0066, 0xFFFF0066, 0xFF003366, 0xFF333366,
		0xFF663366, 0xFF993366, 0xFFCC3366, 0xFFFF3366,
		0xFF006666, 0xFF336666, 0xFF666666, 0xFF996666,
		0xFFCC6666, 0xFFFF6666, 0xFF009966, 0xFF339966,
		0xFF669966, 0xFF999966, 0xFFCC9966, 0xFFFF9966,
		0xFF00CC66, 0xFF33CC66, 0xFF66CC66, 0xFF99CC66,
		0xFFCCCC66, 0xFFFFCC66, 0xFF00FF66, 0xFF33FF66,
		0xFF66FF66, 0xFF99FF66, 0xFFCCFF66, 0xFFFFFF66,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0xFF000000, 0xFF111111, 0xFF222222, 0xFF333333,
		0xFF444444, 0xFF555555, 0xFF666666, 0xFF777777,
		0xFF888888, 0xFF999999, 0xFFAAAAAA, 0xFFBBBBBB,
		0xFFCCCCCC, 0xFFDDDDDD, 0xFFEEEEEE, 0xFFFFFFFF,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0xFF000099, 0xFF330099, 0xFF660099, 0xFF990099,
		0xFFCC0099, 0xFFFF0099, 0xFF003399, 0xFF333399,
		0xFF663399, 0xFF993399, 0xFFCC3399, 0xFFFF3399,
		0xFF006699, 0xFF336699, 0xFF666699, 0xFF996699,
		0xFFCC6699, 0xFFFF6699, 0xFF009999, 0xFF339999,
		0xFF669999, 0xFF999999, 0xFFCC9999, 0xFFFF9999,
		0xFF00CC99, 0xFF33CC99, 0xFF66CC99, 0xFF99CC99,
		0xFFCCCC99, 0xFFFFCC99, 0xFF00FF99, 0xFF33FF99,
		0xFF66FF99, 0xFF99FF99, 0xFFCCFF99, 0xFFFFFF99,
		0xFF0000CC, 0xFF3300CC, 0xFF6600CC, 0xFF9900CC,
		0xFFCC00CC, 0xFFFF00CC, 0xFF0033CC, 0xFF3333CC,
		0xFF6633CC, 0xFF9933CC, 0xFFCC33CC, 0xFFFF33CC,
		0xFF0066CC, 0xFF3366CC, 0xFF6666CC, 0xFF9966CC,
		0xFFCC66CC, 0xFFFF66CC, 0xFF0099CC, 0xFF3399CC,
		0xFF6699CC, 0xFF9999CC, 0xFFCC99CC, 0xFFFF99CC,
		0xFF00CCCC, 0xFF33CCCC, 0xFF66CCCC, 0xFF99CCCC,
		0xFFCCCCCC, 0xFFFFCCCC, 0xFF00FFCC, 0xFF33FFCC,
		0xFF66FFCC, 0xFF99FFCC, 0xFFCCFFCC, 0xFFFFFFCC,
		0xFF0000FF, 0xFF3300FF, 0xFF6600FF, 0xFF9900FF,
		0xFFCC00FF, 0xFFFF00FF, 0xFF0033FF, 0xFF3333FF,
		0xFF6633FF, 0xFF9933FF, 0xFFCC33FF, 0xFFFF33FF,
		0xFF0066FF, 0xFF3366FF, 0xFF6666FF, 0xFF9966FF,
		0xFFCC66FF, 0xFFFF66FF, 0xFF0099FF, 0xFF3399FF,
		0xFF6699FF, 0xFF9999FF, 0xFFCC99FF, 0xFFFF99FF,
		0xFF00CCFF, 0xFF33CCFF, 0xFF66CCFF, 0xFF99CCFF,
		0xFFCCCCFF, 0xFFFFCCFF, 0xFF00FFFF, 0xFF33FFFF,
		0xFF66FFFF, 0xFF99FFFF, 0xFFCCFFFF, 0xFFFFFFFF,
};

void R_SIMPLE_GLCDC_CONFIG_Open(void)
{
	  glcdc_cfg_t glcdc_cfg;
	  glcdc_err_t ret;
	  LCD_X_SETLUTENTRY_INFO colorcnf;
	  uint32_t i;

	  R_GLCDC_PinSet();

	  //
	  // Set DISP signal
	  //
	  DISP_SIGNAL_PORT_DIRECTION  = 1;  // Port direction: output
	  DISP_SIGNAL_PORT_OUTPUT_DATA = 1;  // DISP on
	  //
	  // Switch backlight on, no PWM
	  //
	  BACKLIGHT_SIGNAL_PORT_DIRECTION  = 1;
	  BACKLIGHT_SIGNAL_PORT_OUTPUT_DATA = 1;
	  //
	  // Set the BGEN.SWRST bit to 1 to release the GLCDC from a software reset
	  //
	  // R_GLCDC_Open function release the GLCDC from a software reset.
	  //
	  // Set the frequency of the LCD_CLK and PXCLK to suit the format and set the PANELCLK.CLKEN bit to 1
	  //
	  glcdc_cfg.output.clksrc = GLCDC_CLK_SRC_INTERNAL;   			  // Select PLL clock
	  glcdc_cfg.output.clock_div_ratio = GLCDC_PANEL_CLK_DIVISOR_24;  // 240 / 24 = 10 MHz
	  																  // No frequency division
	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  // Enable LCD_CLK output
	  //
	  // Definition of Background Screen
	  //
	  										   	   	   	   	   	   	  // Horizontal cycle (whole control screen)
	  	  	  	  	  	  	  	  	  	  	   	   	   	   	   	   	  // Vertical cycle (whole control screen)
	  glcdc_cfg.output.htiming.front_porch = 5;         			  // Horizontal Synchronization Signal Assertion Position
	  glcdc_cfg.output.vtiming.front_porch = 8;         			  // Vertical Synchronization Assertion Position
	  glcdc_cfg.output.htiming.back_porch = 40;                        // Horizontal Active Pixel Start Position (min. 6 pixels)
	  glcdc_cfg.output.vtiming.back_porch = 8;
	  glcdc_cfg.output.htiming.display_cyc = XSIZE_PHYS;              // Horizontal Active Pixel Width
	  glcdc_cfg.output.vtiming.display_cyc = YSIZE_PHYS;              // Vertical Active Display Width
	  glcdc_cfg.output.htiming.sync_width = 1;        				  // Vertical Active Display Start Position (min. 3 lines)
	  glcdc_cfg.output.vtiming.sync_width = 1;

	  //
	  // Graphic 1 configuration
	  //
	  glcdc_cfg.input[GLCDC_FRAME_LAYER_1].p_base = NULL;			  // Disable Graphics 1

	  //
	  // Graphic 2 configuration
	  //
	  													  	  	  	  // Enable reading of the frame buffer
	  glcdc_cfg.input[GLCDC_FRAME_LAYER_2].p_base = frame_buffer;   // Specify the start address of the frame buffer
	  glcdc_cfg.input[GLCDC_FRAME_LAYER_2].offset = LINE_OFFSET;      // Offset value from the end address of the line to the start address of the next line
	  																  // Single Line Data Transfer Count
	  																  // Single Frame Line Count
	  glcdc_cfg.input[GLCDC_FRAME_LAYER_2].format = GLCDC_IN_FORMAT_CLUT8;     // Frame Buffer
	  glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].visible = true;
	  glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].blend_control = GLCDC_BLEND_CONTROL_NONE;	// Display Screen Control (current graphics)
	  glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].frame_edge = false;        // Rectangular Alpha Blending Area Frame Display Control
	  glcdc_cfg.input[GLCDC_FRAME_LAYER_2].frame_edge = false;        // Graphics Area Frame Display Control
	  																  // Alpha Blending Control (Per-pixel alpha blending)
	  glcdc_cfg.input[GLCDC_FRAME_LAYER_2].coordinate.y = 0;          // Graphics Area Vertical Start Position
	  glcdc_cfg.input[GLCDC_FRAME_LAYER_2].vsize = YSIZE_PHYS;        // Graphics Area Vertical Width
	  glcdc_cfg.input[GLCDC_FRAME_LAYER_2].coordinate.x = 0;          // Graphics Area Horizontal Start Position
	  glcdc_cfg.input[GLCDC_FRAME_LAYER_2].hsize = XSIZE_PHYS;        // Graphics Area Horizontal Width
	  glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].start_coordinate.x = 0;    // Rectangular Alpha Blending Area Vertical Start Position
	  glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].end_coordinate.x= YSIZE_PHYS; // Rectangular Alpha Blending Area Vertical Width
	  glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].start_coordinate.y = 0;    // Rectangular Alpha Blending Area Horizontal Start Position
	  glcdc_cfg.blend[GLCDC_FRAME_LAYER_2].end_coordinate.y= XSIZE_PHYS; // Rectangular Alpha Blending Area Horizontal Width
	  																  // Graphic 2 Register Value Reflection Enable
	  //
	  // Timing configuration
	  //
	  //   Horizontal Synchronization Signal Reference Timing Offset (not support)
	  //   Signal Reference Timing Select (not support)
	  //   STVB Signal Assertion Timing
	  //   STVB Signal Pulse Width
	  //   STHB Signal Pulse Width
	  glcdc_cfg.output.tcon_vsync = GLCDC_TCON_PIN_0;           // TCON0 Output Signal Select STVA (VSYNC)
	  glcdc_cfg.output.tcon_hsync = GLCDC_TCON_PIN_2;           // TCON2 Output Signal Select STHA (HSYNC)
	  glcdc_cfg.output.tcon_de    = GLCDC_TCON_PIN_3;           // TCON3 Output Signal Select DE (DEN)
	  glcdc_cfg.output.data_enable_polarity = GLCDC_SIGNAL_POLARITY_HIACTIVE;
	  glcdc_cfg.output.hsync_polarity = GLCDC_SIGNAL_POLARITY_LOACTIVE;
	  glcdc_cfg.output.vsync_polarity = GLCDC_SIGNAL_POLARITY_LOACTIVE;
	  glcdc_cfg.output.sync_edge = GLCDC_SIGNAL_SYNC_EDGE_RISING;
	  //
	  // Output interface
	  //
	  //   Serial RGB Data Output Delay Control (0 cycles) (not support)
	  //   Serial RGB Scan Direction Select (forward) (not support)
	  //   Pixel Clock Division Control (no division)
	  glcdc_cfg.output.format = GLCDC_OUT_FORMAT_16BITS_RGB565; // Output Data Format Select (RGB565)
	  glcdc_cfg.output.color_order = GLCDC_COLOR_ORDER_RGB; ///**/GLCDC_COLOR_ORDER_BGR;  	// Pixel Order Control (B-G-R)
	  glcdc_cfg.output.endian = GLCDC_ENDIAN_LITTLE;  			// Bit Endian Control (Little endian)
	  //
	  // Brightness Adjustment
	  //
	  glcdc_cfg.output.brightness.b = _BRIGHTNESS;  // B
	  glcdc_cfg.output.brightness.g = _BRIGHTNESS;  // G
	  glcdc_cfg.output.brightness.r = _BRIGHTNESS;  // R
	  //
	  // Contrast Adjustment Value
	  //
	  glcdc_cfg.output.contrast.b = _CONTRAST;  // B
	  glcdc_cfg.output.contrast.g = _CONTRAST;  // G
	  glcdc_cfg.output.contrast.r = _CONTRAST;  // R
	  //
	  // Disable Gamma
	  //
	  glcdc_cfg.output.gamma.enable = false;
	  //
	  // Disable Chromakey
	  //
	  glcdc_cfg.chromakey[GLCDC_FRAME_LAYER_2].enable = false;
	  //
	  // Disable Dithering
	  //
	  glcdc_cfg.output.dithering.dithering_on = false;
	  //
	  // CLUT Adjustment Value
	  //
	  glcdc_cfg.clut[GLCDC_FRAME_LAYER_2].enable = false;
	  //
	  // Enable VPOS ISR
	  //
	  //   Detecting Scanline Setting
	  glcdc_cfg.detection.vpos_detect = true;		         	// Enable detection of specified line notification in graphic 2
	  glcdc_cfg.interrupt.vpos_enable = true;		           	// Enable VPOS interrupt request
	  //   Interrupt Priority Level (r_glcdc_rx_config.h)
	  //   Interrupt Request Enable
	  //   Clears the STMON.VPOS flag
	  //   VPOS (line detection)
	  glcdc_cfg.detection.gr1uf_detect = false;
	  glcdc_cfg.detection.gr2uf_detect = false;
	  glcdc_cfg.interrupt.gr1uf_enable = false;
	  glcdc_cfg.interrupt.gr2uf_enable = false;
	  //
	  // Set function to be called on VSYNC
	  //
	  glcdc_cfg.p_callback = (void (*)(glcdc_callback_args_t *))_VSYNC_ISR;
	  //
	  // Register Reflection
	  //
	  ret = R_GLCDC_Open(&glcdc_cfg);
	  if (ret != GLCDC_SUCCESS) {
		  while(1);
	  }

		for(i =0; i<256; i++)
		{
		    LCD_X_SETLUTENTRY_INFO * p;
			colorcnf.Color = lcduteconf[i];
			colorcnf.Pos = i;
			p = (LCD_X_SETLUTENTRY_INFO *)&colorcnf;
			_SetLUTEntry(p->Color, p->Pos);
		}

	  memset(frame_buffer, 0, sizeof(frame_buffer));
	  _DisplayOnOff(1);
}

void R_SIMPLE_GLCDC_CONFIG_Close(void)
{
	R_GLCDC_Close();
	_DisplayOnOff(0);
}

uint8_t *lcd_get_frame_buffer_pointer(void)
{
	return frame_buffer;
}

static void _VSYNC_ISR(void * p)
{
	GLCDC.STCLR.BIT.VPOSCLR = 1;                  // Clears the STMON.VPOS flag
}

static void _DisplayOnOff(int OnOff) {
  if (OnOff) {
    R_GLCDC_Control(GLCDC_CMD_START_DISPLAY, FIT_NO_FUNC);  // Enable background generation block
    BACKLIGHT_SIGNAL_PORT_OUTPUT_DATA = 1;
  } else {
	R_GLCDC_Control(GLCDC_CMD_STOP_DISPLAY, FIT_NO_FUNC);  	// Disable background generation block
	BACKLIGHT_SIGNAL_PORT_OUTPUT_DATA = 0;
  }
}

static void _SetLUTEntry(LCD_COLOR Color, uint8_t Pos)
{
	GLCDC.GR2CLUT0[Pos].BIT.R = (Color & 0xFF0000) >> 16;
	GLCDC.GR2CLUT0[Pos].BIT.G = (Color & 0x00FF00) >> 8;
	GLCDC.GR2CLUT0[Pos].BIT.B = (Color & 0x0000FF);
}
