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
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_st7735_lcd_driver_rx_config.c
* Description  : Configuration for ST7735 LCD driver.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           24.12.2016 1.00    Initial Release.
***********************************************************************************************************************/

#ifndef ST7735_LCD_DRIVER_RX_CONFIG_H
#define ST7735_LCD_DRIVER_RX_CONFIG_H

/*******************************************************************************************************
 * NOTICE:
 *   This driver needs 7 pins (mixed SCI(SPI mode) and GPIO) control for ST7735 like below.
 *     1: SPI clock -> SCI(SPI mode)
 *     2: SPI MOSI -> SCI(SPI mode)
 *     3: SPI MISO -> SCI(SPI mode)
 *     4: SPI chip select -> GPIO
 *     5: RESET -> GPIO
 *     6: BACKLIGHT enable -> GPIO
 *     7: Command -> GPIO
 *
 *   Please set MPC register for SCI(SPI mode) out of this module.
 *******************************************************************************************************/

#include "platform.h"
#include "r_sci_rx_if.h"

/* SCI(SPI mode) clock rate */
#define ST7735_LCD_DRIVER_CFG_SCI_CLOCK_RATE			12000000 /* 12MHz */
/* DATA/COMMAND select pin */
#define ST7735_LCD_DRIVER_CFG_DATA_CMD_PIN              (GPIO_PORT_2_PIN_6)
/* Display/enable pin  (chip select) */
#define ST7735_LCD_DRIVER_CFG_ENABLE_PIN                (GPIO_PORT_C_PIN_4)
/* reset pin */
#define ST7735_LCD_DRIVER_CFG_RESET_PIN                 (GPIO_PORT_D_PIN_0)
/* backlight enable pin */
#define ST7735_LCD_DRIVER_CFG_BACKLIGHT_ENABLE_PIN      (GPIO_PORT_3_PIN_0)

/* Display communication line interrupt priority */
#define ST7735_LCD_DRIVER_CFG_DISPLAY_COMMUNICATION_LINE_INTERRUPT_PRIORITY (15) /* need higher than CMT interrupt priority */
/* Display communication line channel */
#define ST7735_LCD_DRIVER_CFG_DISPLAY_COMMUNICATION_LINE_CHANNEL (SCI_CH10)

/* CHARACTER DEFAULT COLOR */
#define ST7735_LCD_DRIVER_CFG_CHARACTER_DEFAULT_COLOR COLOR_WHITE

/* BACKGROPUD DEFAULT COLOR */
#define ST7735_LCD_DRIVER_CFG_BACKGROUD_DEFAULT_COLOR COLOR_BLACK

#endif /* ST7735_LCD_DRIVER_RX_CONFIG_H */
