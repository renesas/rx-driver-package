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
* Copyright (C) 2016-2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_st7735_lcd_driver_rx_private.c
* Description  : Functions for ST7735 LCD driver.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 24.12.2016 1.00    Initial Release.
*         : 12.04.2017 1.01    Clean up source file
*         : 09.30.2017 1.03    Clean up source file
***********************************************************************************************************************/

#ifndef ST7735_LCD_DRIVER_RX_PRIVATE_H
#define ST7735_LCD_DRIVER_RX_PRIVATE_H

#include "r_st7735_lcd_driver_rx_config.h"


#define LEFT_MARGIN (2)             /* Allow 2 pixel margin on the left and the top */
#define TOP_MARGIN  (3)
#define CR (0x0d)
#define LF (0x0a)
#define BS (0x08)

#define REFRESH_FREQUENCY (60)  /* 60 Hz */

/* Command for ST7735 */
#define ST7735_NOP      (0x0)
#define ST7735_SWRESET  (0x01)
#define ST7735_SLPIN    (0x10)
#define ST7735_SLPOUT   (0x11)
#define ST7735_PTLON    (0x12)
#define ST7735_NORON    (0x13)
#define ST7735_INVOFF   (0x20)
#define ST7735_INVON    (0x21)
#define ST7735_DISPON   (0x29)
#define ST7735_CASET    (0x2A)
#define ST7735_RASET    (0x2B)
#define ST7735_RAMWR    (0x2C)
#define ST7735_COLMOD   (0x3A)
#define ST7735_MADCTL   (0x36)
#define ST7735_FRMCTR1  (0xB1)
#define ST7735_INVCTR   (0xB4)
#define ST7735_DISSET5  (0xB6)
#define ST7735_PWCTR1   (0xC0)
#define ST7735_PWCTR2   (0xC1)
#define ST7735_PWCTR3   (0xC2)
#define ST7735_VMCTR1   (0xC5)
#define ST7735_PWCTR6   (0xFC)
#define ST7735_GMCTRP1  (0xE0)
#define ST7735_GMCTRN1  (0xE1)

static void st7735_lcd_driver_init_sci(void);
static void st7735_lcd_driver_init_control_pins(void);
static void st7735_lcd_driver_init_lcd(void);
static void st7735_lcd_driver_sci_callback(void *p_args);
static void st7735_lcd_driver_sci_send_data(uint8_t * const tx_buf, uint16_t tx_num);
static void st7735_lcd_driver_write_command(uint8_t cmd);
static void st7735_lcd_driver_write_byte(uint8_t byte);
static void st7735_lcd_driver_write_multi_byte(uint8_t *data, uint32_t length);
static void st7735_lcd_driver_clear_display(void);
static void st7735_lcd_driver_set_address_window(uint8_t xs, uint8_t ys, uint8_t xe, uint8_t ye);
static void st7735_lcd_driver_cmt_callback(void *p_args);
static void st7735_lcd_driver_putchar(uint8_t const val);
static void st7735_lcd_driver_linefeed(void);
static void st7735_lcd_driver_display_write_pixel(uint8_t const x, uint8_t const y, uint16_t const color);
static void st7735_lcd_driver_display_write_column (uint8_t const x, uint8_t const y, uint8_t const pattern);
static void st7735_lcd_driver_display_set_current_position(uint8_t const x, uint8_t const y);
static uint16_t st7735_lcd_driver_change_endian_short(uint16_t data);

#endif /* ST7735_LCD_DRIVER_RX_PRIVATE_H */
