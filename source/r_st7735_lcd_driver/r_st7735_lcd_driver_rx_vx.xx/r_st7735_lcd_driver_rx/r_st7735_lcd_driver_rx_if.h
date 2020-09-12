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
* Copyright (C) 2016-2107 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_st7735_lcd_driver_rx_if.c
* Description  : Functions for ST7735 LCD driver.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 24.12.2016 1.00    Initial Release.
*         : 12.04.2017 1.01    Clean up source file
*         : 09.30.2017 1.03    Clean up source file
***********************************************************************************************************************/

#ifndef ST7735_LCD_DRIVER_RX_H
#define ST7735_LCD_DRIVER_RX_H

#define SCREEN_WIDTH  (128)
#define SCREEN_HEIGHT (128)

/* Automatic calculation of parameters */
#define FONT_WIDTH                (6)   /* including a space */
#define FONT_HEIGHT               (8)   /* including 1 pixel space */
#define MAX_LINES                 (SCREEN_HEIGHT / FONT_HEIGHT)
#define CHAR_PER_LINE             (SCREEN_WIDTH / FONT_WIDTH)

/***********************************************************************************************************************
  DISPLAY color DEFINITIONS (16 bits) R5G6B5 format
  Only Primary & secondary colors are defined here. Other colors can be created using RGB values.
***********************************************************************************************************************/
#define COLOR_BLACK       (0x0000)
#define COLOR_RED         (0xf800)
#define COLOR_GREEN       (0x07E0)
#define COLOR_BLUE        (0x001F)
#define COLOR_YELLOW      (0xFFE0)
#define COLOR_CYAN        (0x07FF)
#define COLOR_MAGENTA     (0xF81F)
#define COLOR_WHITE       (0xFFFF)
#define COLOR_RENESAS     (0x2953)

/* Initializes the LCD */
void R_ST7735_LcdOpen(void);
void R_ST7735_LcdClose(void);
void R_ST7735_LcdPrintString(char *string);
void R_ST7735_LcdPrintCharAtSpecifiedPosition(char character, uint32_t x, uint32_t y);
void R_ST7735_LcdSetColorToSpecifiedPosition(uint16_t color_code, uint32_t x, uint32_t y);

void R_ST7735_LcdPrintStringAtSpecifiedPosition(char *string, uint32_t x, uint32_t y);
void R_ST7735_LcdDisplaySetCurrentPisution(uint32_t x, uint32_t y);
void R_ST7735_LcdPrintOriginalCharacter(uint8_t original_char, uint32_t x, uint32_t y);
void R_ST7735_LcdSetBackColorToSpecifiedPosition(uint16_t color_code, uint32_t x, uint32_t y);

#endif /* ST7735_LCD_DRIVER_RX_H */
