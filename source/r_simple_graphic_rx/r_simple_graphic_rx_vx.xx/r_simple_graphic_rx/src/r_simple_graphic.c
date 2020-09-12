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
* File Name    : r_simple_graphic.c
* Description  : Simple graphic driver.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 03.10.2017 1.00    Initial Release.
***********************************************************************************************************************/

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

#include <string.h>
#include <ctype.h>

#include "platform.h"
#include "r_cmt_rx_if.h"

#include "r_simple_graphic_if.h"
#include "r_ascii.h"

static uint8_t current_buffer[SCREEN_HEIGHT/FONT_HEIGHT][SCREEN_WIDTH/FONT_WIDTH];
static uint8_t previous_buffer[SCREEN_HEIGHT/FONT_HEIGHT][SCREEN_WIDTH/FONT_WIDTH];
static uint8_t *frame_buffer;
static uint32_t current_x, current_y;
static uint32_t cmt_channel;

static void simple_graphic_cmt_callback(void *p_args);
static void simple_graphic_put_character(uint32_t x, uint32_t y, uint8_t character);

void R_SIMPLE_GRAPHIC_Open(void)
{
	frame_buffer = (uint8_t *)lcd_get_frame_buffer_pointer();

	memset(current_buffer, 0, sizeof(current_buffer));
	memset(previous_buffer, 0, sizeof(previous_buffer));

	current_x = 0;
	current_y = 0;

    /* start display refreshing */
    R_CMT_CreatePeriodic(REFRESH_FREQUENCY, simple_graphic_cmt_callback, &cmt_channel);
}

void R_SIMPLE_GRAPHIC_Close(void)
{
	R_CMT_Stop(cmt_channel);
}

void R_SIMPLE_GRAPHIC_PutCharacter(char character)
{
	if(isprint(character))
	{
		current_buffer[current_y][current_x] = character;
		current_x++;
	}
	if(current_x == (SCREEN_WIDTH/FONT_WIDTH))
	{
		current_x = 0;
		current_y++;
	}
	if(character == 0x0d) /* CR */
	{
		current_x = 0;
	}
	if(character == 0x0a) /* LF */
	{
		current_y++;
	}
	if(current_y == (SCREEN_HEIGHT/FONT_HEIGHT))
	{
		current_y--;
		memcpy(current_buffer[0], current_buffer[1], sizeof(current_buffer[0]) * ((SCREEN_HEIGHT/FONT_HEIGHT) - 1));
		memset(current_buffer[(SCREEN_HEIGHT/FONT_HEIGHT) - 1], 0, sizeof(current_buffer[0]));
	}
}

static void simple_graphic_put_character(uint32_t x, uint32_t y, uint8_t character)
{
	uint32_t i, j;
    const uint8_t mask_table[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
    uint8_t dot;

	for(i = 0; i < FONT_WIDTH; i++)
	{
		for(j = 0; j < FONT_HEIGHT; j++)
		{
			if(g_ascii_table[character - 0x20][i] & mask_table[j])
			{
				dot = 0xff;	/* white */
			}
			else
			{
				dot = 0x00; /* black */
			}
			frame_buffer[(x * FONT_WIDTH) + (y * FONT_HEIGHT * ONE_LINE_INDEX) + i + (j * ONE_LINE_INDEX)] = dot;
		}
	}
}

static void simple_graphic_cmt_callback(void *p_args)
{
	uint32_t i, j;

	while(1)
	{
		if(!memcmp(current_buffer, previous_buffer, sizeof(current_buffer)))
		{
			break;
		}
		else
		{
			 for(i = 0; i < SCREEN_HEIGHT/FONT_HEIGHT; i++)
			 {
				 for(j = 0; j < SCREEN_WIDTH/FONT_WIDTH; j++)
				 {
					 if(current_buffer[i][j] != previous_buffer[i][j])
					 {
						 if(isprint(current_buffer[i][j]))
						 {
							 simple_graphic_put_character(j, i, current_buffer[i][j]);
						 }
						 else
						 {
							 simple_graphic_put_character(j, i, ' ');
						 }
						 previous_buffer[i][j] = current_buffer[i][j];
					 }
				 }
			 }
		}
	}
}

