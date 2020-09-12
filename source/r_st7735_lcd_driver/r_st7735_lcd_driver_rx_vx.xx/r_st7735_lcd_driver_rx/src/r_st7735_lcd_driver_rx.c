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
/**********************************************************************************************************************
* File Name    : r_st7735_lcd_driver_rx.c
* Description  : Functions for ST7735 LCD driver.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 24.12.2016 1.00    Initial Release.
*         : 12.04.2017 1.01    Clean up source file
*         : 09.30.2017 1.03    Clean up source file
***********************************************************************************************************************/

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

#include <string.h>
#include "platform.h"
#include "r_cmt_rx_if.h"
#include "r_sci_rx_if.h"
#include "r_gpio_rx_if.h"
#include "r_st7735_lcd_driver_rx_if.h"
#include "r_st7735_lcd_driver_rx_private.h"
#include "r_st7735_lcd_driver_rx_config.h"

#include "r_ascii.h"
#include "r_original_character.h"

#if SECURE_BOOT
#pragma section SECURE_BOOT
#endif

static sci_cfg_t sci_config = {0};
static sci_hdl_t sci_handle = {0};
static uint8_t curx = 0;
static uint8_t cury = 0;

static uint8_t current_frame_buffer[SCREEN_HEIGHT][SCREEN_WIDTH / 8];
static uint8_t previous_frame_buffer[SCREEN_HEIGHT][SCREEN_WIDTH / 8];
static uint16_t communication_buffer[SCREEN_WIDTH] = {0};
static uint16_t character_color[MAX_LINES][CHAR_PER_LINE] = {0};
static uint16_t back_color[MAX_LINES][CHAR_PER_LINE] = {0};

/***********************************************************************************************************************
* Function Name: R_ST7735_LcdOpen
* Description  : LCD initialization
* Arguments    : none -
* Return Value : none -
***********************************************************************************************************************/
void R_ST7735_LcdOpen(void)
{
    uint32_t cmt_channel = 0;
    int8_t i = 0;
	int8_t j = 0;

    /* Start SPI communication channel to LCD Display */
    st7735_lcd_driver_init_sci();

    /* initialize Port*/
    st7735_lcd_driver_init_control_pins();

    /* initialize Standard PMOD display */
    st7735_lcd_driver_init_lcd();

    /* clear display */
    st7735_lcd_driver_clear_display();

    /* clear frame buffers */
    memset(current_frame_buffer, 0x00, sizeof(current_frame_buffer));
    memset(previous_frame_buffer, 0x00, sizeof(previous_frame_buffer));

    /* set character default color */
    for(i = 0; i < MAX_LINES; i++)
    {
        for(j = 0; j < CHAR_PER_LINE; j++)
        {
            character_color[i][j] = ST7735_LCD_DRIVER_CFG_CHARACTER_DEFAULT_COLOR;
            back_color[i][j] = ST7735_LCD_DRIVER_CFG_BACKGROUD_DEFAULT_COLOR;
        }
    }

    /* start display refreshing */
    R_CMT_CreatePeriodic(REFRESH_FREQUENCY, st7735_lcd_driver_cmt_callback, &cmt_channel);
}
/***********************************************************************************************************************
End of function R_ST7735_LcdOpen
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_ST7735_LcdClose
* Description  : End processing of LCD
* Arguments    : none -
* Return Value : none -
***********************************************************************************************************************/
void R_ST7735_LcdClose(void)
{
	R_SCI_Close(sci_handle);
}
/***********************************************************************************************************************
End of function R_ST7735_LcdClose
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_ST7735_LcdPrintString
* Description  : Display a string of arguments to the LCD
* Arguments    : string -
*                    The string to be displayed
* Return Value : none -
***********************************************************************************************************************/
void R_ST7735_LcdPrintString(char *string)
{
    int8_t i = 0;
    int8_t string_length = 0;

    string_length = strlen(string);
    for(i = 0; i < string_length; i++)
    {
        st7735_lcd_driver_putchar(*string++);
    }
}
/***********************************************************************************************************************
End of function R_ST7735_LcdPrintString
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_ST7735_LcdPrintCharAtSpecifiedPosition
* Description  : Display characters at specified coordinates
* Arguments    : character -
*                    Character to display
*                x -
*                    x position
*                y -
*                    y position
* Return Value : none -
***********************************************************************************************************************/
void R_ST7735_LcdPrintCharAtSpecifiedPosition(char character, uint32_t x, uint32_t y)
{
    R_BSP_InterruptsDisable();
    st7735_lcd_driver_display_set_current_position(x, y);
    st7735_lcd_driver_putchar(character);
    R_BSP_InterruptsEnable();
}
/***********************************************************************************************************************
End of function R_ST7735_LcdPrintCharAtSpecifiedPosition
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_ST7735_LcdSetColorToSpecifiedPosition
* Description  : Set the font color of the specified coordinates
* Arguments    : color_code -
*                    Font color
*                x -
*                    x position
*                y -
*                    y position
* Return Value : none
***********************************************************************************************************************/
void R_ST7735_LcdSetColorToSpecifiedPosition(uint16_t color_code, uint32_t x, uint32_t y)
{
    character_color[y][x] = color_code;
}
/***********************************************************************************************************************
End of function R_ST7735_LcdSetColorToSpecifiedPosition
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_ST7735_LcdPrintStringAtSpecifiedPosition
* Description  : Display a string to specify the top coordinate
* Arguments    : string -
*                    String to display
*                x -
*                    x position
*                y -
*                    y position
* Return Value : none
***********************************************************************************************************************/
void R_ST7735_LcdPrintStringAtSpecifiedPosition(char *string, uint32_t x, uint32_t y)
{
    R_BSP_InterruptsDisable();
    st7735_lcd_driver_display_set_current_position(x, y);
    R_ST7735_LcdPrintString(string);
    R_BSP_InterruptsEnable();
}
/***********************************************************************************************************************
End of function R_ST7735_LcdPrintStringAtSpecifiedPosition
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_ST7735_LcdDisplaySetCurrentPisution
* Description  : Set the coordinates of the operation position of the LCD screen
* Arguments    : x -
*                    x position
*                y -
*                    y position
* Return Value : none
***********************************************************************************************************************/
void R_ST7735_LcdDisplaySetCurrentPisution(uint32_t x, uint32_t y)
{
    st7735_lcd_driver_display_set_current_position((uint8_t)x, (uint8_t)y);
}
/***********************************************************************************************************************
End of function R_ST7735_LcdDisplaySetCurrentPisution
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_ST7735_LcdPrintOriginalCharacter
* Description  : Display arbitrary characters / symbols
* Arguments    : original_char -
*                    Original_char to display
*                x -
*                    x position
*                y -
*                    y position
* Return Value : none
***********************************************************************************************************************/
void R_ST7735_LcdPrintOriginalCharacter(uint8_t original_char, uint32_t x, uint32_t y)
{
    uint8_t * ptr = 0;
    uint8_t   i= 0;
    uint8_t   pattern = 0;
    uint8_t   position_x = 0;
    uint8_t   position_y = 0;

    /* Get pointer to start of representation of character in ASCII table */
    ptr = (uint8_t *)&g_original_character_table[original_char][0];

    /* Copy character over */
    for (i = 0; i < FONT_WIDTH; i++)
    {
        pattern = (*ptr++);
        position_x = (uint8_t)((x * FONT_WIDTH) + i);
        position_y = (uint8_t)(y * FONT_HEIGHT);
        st7735_lcd_driver_display_write_column(position_x, position_y, pattern);
    }
}
/***********************************************************************************************************************
End of function R_ST7735_LcdPrintOriginalCharacter
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: R_ST7735_LcdSetBackColorToSpecifiedPosition
* Description  : Set the back color of the specified coordinates
* Arguments    : color_code -
*                    Back color
*                x -
*                    x position
*                y -
*                    y position
* Return Value : none
***********************************************************************************************************************/
void R_ST7735_LcdSetBackColorToSpecifiedPosition(uint16_t color_code, uint32_t x, uint32_t y)
{
    back_color[y][x] = color_code;
}
/***********************************************************************************************************************
End of function R_ST7735_LcdSetBackColorToSpecifiedPosition
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: st7735_lcd_driver_init_sci_pins
* Description  : This function initialize SCI.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void st7735_lcd_driver_init_sci(void)
{
    sci_err_t sci_err;

    /* set SCI */
    sci_config.sync.spi_mode = SCI_SPI_MODE_OFF;
    sci_config.sync.bit_rate = ST7735_LCD_DRIVER_CFG_SCI_CLOCK_RATE;
    sci_config.sync.msb_first = true;
    sci_config.sync.invert_data = false;
    sci_config.sync.int_priority = ST7735_LCD_DRIVER_CFG_DISPLAY_COMMUNICATION_LINE_INTERRUPT_PRIORITY;
    sci_err = R_SCI_Open(ST7735_LCD_DRIVER_CFG_DISPLAY_COMMUNICATION_LINE_CHANNEL,
            SCI_MODE_SYNC, &sci_config, st7735_lcd_driver_sci_callback, &sci_handle);
    if(SCI_SUCCESS != sci_err)
    {
        /* nothing to do */
    }
}
/***********************************************************************************************************************
End of function st7735_lcd_driver_init_sci_pins
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: st7735_lcd_driver_init_control_pins
* Description  : This function initialize SCI.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void st7735_lcd_driver_init_control_pins(void)
{
    gpio_err_t gpio_err;

    /* Set COMMAND pin */
    R_GPIO_PinDirectionSet(ST7735_LCD_DRIVER_CFG_DATA_CMD_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_DATA_CMD_PIN, GPIO_LEVEL_LOW);
    gpio_err = R_GPIO_PinControl(ST7735_LCD_DRIVER_CFG_DATA_CMD_PIN, GPIO_CMD_OUT_CMOS);
    if(SCI_SUCCESS != gpio_err)
    {
        /* nothing to do */
    }

    /* Set ENABLE pin */
    R_GPIO_PinDirectionSet(ST7735_LCD_DRIVER_CFG_ENABLE_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_ENABLE_PIN, GPIO_LEVEL_LOW);
    gpio_err = R_GPIO_PinControl(ST7735_LCD_DRIVER_CFG_ENABLE_PIN, GPIO_CMD_OUT_CMOS);
    if(SCI_SUCCESS != gpio_err)
    {
        /* nothing to do */
    }

    /* Set RESET pin */
    R_GPIO_PinDirectionSet(ST7735_LCD_DRIVER_CFG_RESET_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_RESET_PIN, GPIO_LEVEL_LOW);
    gpio_err = R_GPIO_PinControl(ST7735_LCD_DRIVER_CFG_RESET_PIN, GPIO_CMD_OUT_CMOS);
    if(SCI_SUCCESS != gpio_err)
    {
        /* nothing to do */
    }

    /* Set BACKLIGHT pin */
    R_GPIO_PinDirectionSet(ST7735_LCD_DRIVER_CFG_BACKLIGHT_ENABLE_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_BACKLIGHT_ENABLE_PIN, GPIO_LEVEL_LOW);
    gpio_err = R_GPIO_PinControl(ST7735_LCD_DRIVER_CFG_BACKLIGHT_ENABLE_PIN, GPIO_CMD_OUT_CMOS);
    if(SCI_SUCCESS != gpio_err)
    {
        /* nothing to do */
    }
}
/***********************************************************************************************************************
End of function st7735_lcd_driver_init_control_pins
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: st7735_lcd_driver_init_lcd
* Description  : This function initialize LCD driver.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void st7735_lcd_driver_init_lcd(void)
{
    /* Preset the output ports for i/o pins prior to setting as outputs */
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_DATA_CMD_PIN, GPIO_LEVEL_HIGH);
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_ENABLE_PIN, GPIO_LEVEL_HIGH);
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_RESET_PIN, GPIO_LEVEL_HIGH);

    /* backlight off */
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_BACKLIGHT_ENABLE_PIN, GPIO_LEVEL_LOW);

    /* Wait 10 ms */
    R_BSP_SoftwareDelay(10, BSP_DELAY_MICROSECS);

    /* Clear reset line to reset display */
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_RESET_PIN, GPIO_LEVEL_LOW);

    /* Wait 10 ms */
    R_BSP_SoftwareDelay(10, BSP_DELAY_MICROSECS);

    /* Release reset */
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_RESET_PIN, GPIO_LEVEL_HIGH);

    /* software reset */
    st7735_lcd_driver_write_command(ST7735_SWRESET);
    R_BSP_SoftwareDelay(120, BSP_DELAY_MICROSECS);

    /* out of sleep mode */
    st7735_lcd_driver_write_command(ST7735_SLPOUT);
    R_BSP_SoftwareDelay(120, BSP_DELAY_MICROSECS);

    /* set color mode */
    st7735_lcd_driver_write_command(ST7735_COLMOD);

    /* 16 bit color */
    st7735_lcd_driver_write_byte(0x05);
    R_BSP_SoftwareDelay(10, BSP_DELAY_MICROSECS);

    /* memory access control (directions) */
    st7735_lcd_driver_write_command(ST7735_MADCTL);

    /* row address/col address, bottom to top refresh */
    st7735_lcd_driver_write_byte(0xC8);

    /* normal mode */
    st7735_lcd_driver_write_command(ST7735_NORON);
    R_BSP_SoftwareDelay(10, BSP_DELAY_MICROSECS);

    /* display on */
    st7735_lcd_driver_write_command(ST7735_DISPON);
    R_BSP_SoftwareDelay(120, BSP_DELAY_MICROSECS);

    /* enable backlight */
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_BACKLIGHT_ENABLE_PIN, GPIO_LEVEL_HIGH);
}
/***********************************************************************************************************************
End of function st7735_lcd_driver_init_lcd
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: st7735_lcd_driver_sci_callback
* Description  : Callback function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void st7735_lcd_driver_sci_callback(void *p_args)
{
    sci_cb_args_t *args;
    args = (sci_cb_args_t *)p_args;
	if (SCI_EVT_XFER_DONE == args->event)
    {
        nop();
    }
    else if (SCI_EVT_XFER_ABORTED == args->event)
    {
        nop();
    }
    else if (SCI_EVT_OVFL_ERR == args->event) {
		nop();
	} else {
	}
}
/***********************************************************************************************************************
End of function st7735_lcd_driver_sci_callback
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: st7735_lcd_driver_sci_send_data
* Description  : send data to st7735
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void st7735_lcd_driver_sci_send_data(uint8_t *tx_buf, uint16_t tx_num)
{
    sci_err_t sci_err;

    sci_err = R_SCI_Send(sci_handle, tx_buf, tx_num);
    if(SCI_SUCCESS != sci_err)
    {
        /* nothing to do */
    }
    else
    {
        /* wait until complete sending */
		while (R_SCI_Control(sci_handle, SCI_CMD_CHECK_XFER_DONE, NULL) != SCI_SUCCESS)
		{
			;
		}
    }
}
/***********************************************************************************************************************
End of function st7735_lcd_driver_sci_send_data
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: st7735_lcd_driver_write_command
* Description  : Encapsulation of SPI8 send, sends 1 cmd byte.
* Arguments    : cmd - Command to write
* Return Value : None
* Note         : DATA_CMD_PIN is set low for command
***********************************************************************************************************************/
static void st7735_lcd_driver_write_command(uint8_t command)
{
    /* data cmd pin low to indicate a command */
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_DATA_CMD_PIN, GPIO_LEVEL_LOW);

    /* assert chip select */
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_ENABLE_PIN, GPIO_LEVEL_LOW);

    /* Write the command */
    st7735_lcd_driver_sci_send_data(&command, 1);

    /* de-assert chip select */
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_ENABLE_PIN, GPIO_LEVEL_HIGH);

    /* data cmd pin high to signify data */
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_DATA_CMD_PIN, GPIO_LEVEL_HIGH);

}
/***********************************************************************************************************************
End of function st7735_lcd_driver_write_command
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: st7735_lcd_driver_write_byte
* Description  : Write a byte to the display
* Arguments    : byte - data to write
* Return Value : None
* Note         : DATA_CMD_PIN is set hi for data.
***********************************************************************************************************************/
static void st7735_lcd_driver_write_byte(uint8_t byte)
{
    /* data cmd pin low to indicate a command */
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_DATA_CMD_PIN, GPIO_LEVEL_HIGH);

    /* assert chip select */
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_ENABLE_PIN, GPIO_LEVEL_LOW);

    /* Write the data */
    st7735_lcd_driver_sci_send_data(&byte, 1);

    /* de-assert chip select */
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_ENABLE_PIN, GPIO_LEVEL_HIGH);

    /* data cmd pin high to signify data */
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_DATA_CMD_PIN, GPIO_LEVEL_HIGH);
}
/***********************************************************************************************************************
End of function st7735_lcd_driver_write_byte
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: st7735_lcd_driver_write_multi_byte
* Description  : Write a byte to the display
* Arguments    : data - data to write
*              : length - data length
* Return Value : None
* Note         : DATA_CMD_PIN is set hi for data.
***********************************************************************************************************************/
static void st7735_lcd_driver_write_multi_byte(uint8_t *data, uint32_t length)
{
    /* data cmd pin low to indicate a command */
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_DATA_CMD_PIN, GPIO_LEVEL_HIGH);

    /* assert chip select */
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_ENABLE_PIN, GPIO_LEVEL_LOW);

    /* Write the data */
    st7735_lcd_driver_sci_send_data(data, length);

    /* de-assert chip select */
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_ENABLE_PIN, GPIO_LEVEL_HIGH);

    /* data cmd pin high to signify data */
    R_GPIO_PinWrite(ST7735_LCD_DRIVER_CFG_DATA_CMD_PIN, GPIO_LEVEL_HIGH);
}
/***********************************************************************************************************************
End of function st7735_lcd_driver_write_multi_byte
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: st7735_lcd_driver_clear_display
* Description  : This function clears the screen
* Arguments    : uint16_t color - in R5G6B5 format
* Return Value : none
***********************************************************************************************************************/
static void st7735_lcd_driver_clear_display(void)
{
    uint8_t i;
    uint8_t j;
    uint8_t k;

    for(i = 0; i < SCREEN_HEIGHT; i++)
    {
        for(j = 0; j < (SCREEN_WIDTH / 8); j++)
        {
            for(k = 0; k < 8; k++)
            {
                communication_buffer[(j * 8) + k]
                      = st7735_lcd_driver_change_endian_short(ST7735_LCD_DRIVER_CFG_BACKGROUD_DEFAULT_COLOR);
            }
        }
    }

    /* Write to RAM */
    for(i = 0; i < SCREEN_HEIGHT; i++)
    {
        st7735_lcd_driver_set_address_window(0, i, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
        st7735_lcd_driver_write_command(ST7735_RAMWR);
        st7735_lcd_driver_write_multi_byte((uint8_t *)communication_buffer, sizeof(communication_buffer));
    }
}
/***********************************************************************************************************************
End of function st7735_lcd_driver_clear_display
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: st7735_lcd_driver_set_address_window
* Description  : Draw horz line with the specified color
* Arguments    : xs, ys start pixel position
*                ys, ye end pixel position
* Return Value : None
***********************************************************************************************************************/
static void st7735_lcd_driver_set_address_window(uint8_t xs, uint8_t ys, uint8_t xe, uint8_t ye)
{
    if (((ys <= ye) && (xs <= xe)) && ((ye < SCREEN_HEIGHT) && (xe < SCREEN_WIDTH)))
    {
        xs += LEFT_MARGIN;
        xe += LEFT_MARGIN;
        ys += TOP_MARGIN;
        ye += TOP_MARGIN;

        /* column addr set */
        st7735_lcd_driver_write_command(ST7735_CASET);
        st7735_lcd_driver_write_byte(0x00);

        /* XSTART */
        st7735_lcd_driver_write_byte(xs);
        st7735_lcd_driver_write_byte(0x00);

        /* XEND   */
        st7735_lcd_driver_write_byte(xe);

        /* row addr set */
        st7735_lcd_driver_write_command(ST7735_RASET);
        st7735_lcd_driver_write_byte(0x00);

        /* YSTART */
        st7735_lcd_driver_write_byte(ys);
        st7735_lcd_driver_write_byte(0x00);

        /* YEND   */
        st7735_lcd_driver_write_byte(ye);
    }
}
/***********************************************************************************************************************
End of function st7735_lcd_driver_set_address_window
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: st7735_lcd_driver_cmt_callback
* Description  : callback from CMT
* Arguments    :
* Return Value : None
***********************************************************************************************************************/
static void st7735_lcd_driver_cmt_callback(void *p_args)
{
    uint8_t i;
    uint8_t j;
    uint8_t k;
    const uint8_t mask_table[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

    R_BSP_InterruptsEnable();

	if (R_SCI_Control(sci_handle, SCI_CMD_CHECK_XFER_DONE, NULL) == SCI_SUCCESS)
    {
        for(i = 0; i < SCREEN_HEIGHT; i++)
        {
            if(memcmp(current_frame_buffer[i], previous_frame_buffer[i], SCREEN_WIDTH / 8))
            {
                st7735_lcd_driver_set_address_window(0, i, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
                st7735_lcd_driver_write_command(ST7735_RAMWR);
                for(j = 0; j < (SCREEN_WIDTH / 8); j++)
                {
                    for(k = 0; k < 8; k++)
                    {
                        if((current_frame_buffer[i][j] & mask_table[k]))
                        {
                            communication_buffer[(j * 8) + k]
                                                 = st7735_lcd_driver_change_endian_short(character_color[i / 8][j]);
                        }
                        else
                        {
                            communication_buffer[(j * 8) + k]
                                                 = st7735_lcd_driver_change_endian_short(back_color[i / 8][j]);
                        }
                    }
                }
                st7735_lcd_driver_write_multi_byte((uint8_t *)communication_buffer, sizeof(communication_buffer));
                memcpy(previous_frame_buffer[i], current_frame_buffer[i], SCREEN_WIDTH / 8);
            }
        }
    }
    else
    {

    }
}
/***********************************************************************************************************************
End of function st7735_lcd_driver_cmt_callback
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: st7735_lcd_driver_putchar
* Description  : Displays a character at the current cursor position.
*                Advances the cursor by 1 position.
*                Cursor wraps to next line at the end of the line.
*                Cursor wraps to Home position at the end of the display.
*
*                The following control codes are accepted:
*
*                LF increments y cursor position
*                CR resets x cursor to 0.
*                BS moves x cursor back by 1 place if possible.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void st7735_lcd_driver_putchar(uint8_t const val)
{
    uint8_t * ptr;
    uint8_t   i;
    uint8_t   pattern;
    uint8_t   x;
    uint8_t   y;
    static uint32_t linefeed_flag;

    switch (val)
    {
        /* Carriage return character */
        case CR:
            curx = 0;
            break;

        /* Line feed character */
        case LF:
        	if(!linefeed_flag)
        	{
        		st7735_lcd_driver_linefeed();
        	}
            break;

        /* Backspace character */
        case BS:
            if (curx > 0)
            {
                curx--;
            }
            break;

        /* Normal displayable characters */
        default:
            /* Ensure value is within the ASCII range */
            if ((val >= 0x20) && (val <= 0x7f))
            {
            	linefeed_flag = 0;
                /* Get pointer to start of representation of character in ASCII table */
                ptr = (uint8_t *)&g_ascii_table[val - 0x20][0];

                /* Copy character over */
                for (i = 0; i < FONT_WIDTH; i++)
                {
                    pattern = (*ptr++);
                    x = (uint8_t)((curx * FONT_WIDTH) + i);
                    y = (uint8_t)(cury * FONT_HEIGHT);
                    st7735_lcd_driver_display_write_column(x, y, pattern);
                }

                /* move cursor to next co-ordinate on LCD */
                curx++;

                /* move to next row if reached the end of line */
                if (curx >= CHAR_PER_LINE)
                {
                    curx = 0;
                    linefeed_flag = 1;
                    st7735_lcd_driver_linefeed();
                }
            }
            break;
    }
}
/***********************************************************************************************************************
End of function st7735_lcd_driver_putchar
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: st7735_lcd_driver_linefeed
* Description  : realize linefeed behavior like command line interface.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/

static void st7735_lcd_driver_linefeed(void)
{
	uint32_t i;
	static uint32_t processing_flag;

	if(!processing_flag)
	{
		processing_flag = 1;
	    cury++;
	    if (cury >= MAX_LINES)
	    {
	        cury = MAX_LINES - 1;
	        memcpy(current_frame_buffer[0], current_frame_buffer[FONT_HEIGHT], (sizeof(current_frame_buffer[0]) * FONT_HEIGHT) * (MAX_LINES - 1));
	        for(i = 0; i < CHAR_PER_LINE; i++)
	        {
	        	R_ST7735_LcdPrintCharAtSpecifiedPosition(' ', i, MAX_LINES - 1);
	        }
	    }
		processing_flag = 0;
	}
}

/***********************************************************************************************************************
End of function st7735_lcd_driver_linefeed
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: st7735_lcd_driver_display_write_pixel
* Description  : Write the color to the current cursor position
* Arguments    : x,y - pixel position
*                color in R5G6B5 format
* Return Value : None
***********************************************************************************************************************/
static void st7735_lcd_driver_display_write_pixel(uint8_t const x, uint8_t const y, uint16_t const color)
{
    const uint8_t mask_table[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

	if (ST7735_LCD_DRIVER_CFG_BACKGROUD_DEFAULT_COLOR != color)
    {
        current_frame_buffer[y][x / 8] |= mask_table[x % 8];
    }
    else
    {
        current_frame_buffer[y][x / 8] &= (~mask_table[x % 8]);
    }
}
/***********************************************************************************************************************
End of function st7735_lcd_driver_display_write_pixel
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: st7735_lcd_driver_display_write_column
* Description  : Write a character at a current cursor position
* Arguments    : x,y pixel position
*                pattern to draw 1 = font color, 0 = back color
* Return Value : None
***********************************************************************************************************************/
static void st7735_lcd_driver_display_write_column(uint8_t const x, uint8_t const y, uint8_t const pattern)
{
    uint8_t mask;
    uint8_t i;
    uint8_t y1;

    mask = 0x80;
    for (i = 0; i < FONT_HEIGHT; i++)
    {
        y1 = (uint8_t)(y + i);
        if (pattern & mask)
        {
            st7735_lcd_driver_display_write_pixel(x, y1, character_color[x / CHAR_PER_LINE][y / MAX_LINES]);
        }
        else
        {
            st7735_lcd_driver_display_write_pixel(x, y1, back_color[x / CHAR_PER_LINE][y / MAX_LINES]);
        }
        mask = (uint8_t)(mask >> 1);
    }
}
/***********************************************************************************************************************
End of function st7735_lcd_driver_display_write_column
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: st7735_lcd_driver_display_set_current_position
* Description  : Set the cursor to the specified position. the position must be
*                within the limits.
* Arguments    : uint8_t x - X position
*                        y - Y position
* Return Value : none
***********************************************************************************************************************/
static void st7735_lcd_driver_display_set_current_position(uint8_t const x, uint8_t const y)
{
    if (x < CHAR_PER_LINE)
    {
        curx = x;
    }

    if (y < MAX_LINES)
    {
        cury = y;
    }
}
/***********************************************************************************************************************
End of function st7735_lcd_driver_display_set_current_position
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: st7735_lcd_driver_change_endian_short
* Description  : Endian conversion processing
* Arguments    : data-
*                    Data
* Return Value : revw(data)
*                    Converted data
*
***********************************************************************************************************************/
static uint16_t st7735_lcd_driver_change_endian_short(uint16_t data)
{
    return revw(data);
}
/***********************************************************************************************************************
End of function st7735_lcd_driver_change_endian_short
***********************************************************************************************************************/

#if SECURE_BOOT
#pragma section
#endif
