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
* File Name    : r_simple_graphic_if.h
* Description  : Simple graphic driver header.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 03.10.2017 1.00    Initial Release.
***********************************************************************************************************************/

#define FONT_WIDTH 6
#define FONT_HEIGHT 8
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272

#define REFRESH_FREQUENCY 60
#define ONE_LINE_INDEX 0x200

extern uint8_t* lcd_get_frame_buffer_pointer(void);

void R_SIMPLE_GRAPHIC_Open(void);
void R_SIMPLE_GRAPHIC_Close(void);
void R_SIMPLE_GRAPHIC_PutCharacter(char character);

