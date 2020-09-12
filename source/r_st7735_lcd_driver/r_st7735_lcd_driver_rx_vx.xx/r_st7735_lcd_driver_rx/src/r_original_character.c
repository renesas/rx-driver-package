/*
 * r_original_character.c
 *
 *  Created on: 2017/01/30
 *      Author: a5098420
 */

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
* Copyright (C) 2015-2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_original_character.c
* Version      : 1.03
* Device(s)    : R5F52318AxFP
* Tool-Chain   : CCRX
* H/W Platform : RSKRX231
* Description  : Defines ascii font table
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 01.06.2015 1.00     First Release
*         : 12.04.2017 1.01     Clean up source file
*         : 09.30.2017 1.03     Clean up source file
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"
#include "r_original_character.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

#if SECURE_BOOT
#pragma section SECURE_BOOT
#endif

#pragma pack

const char g_original_character_table[][6] =
{
    { 0x08, 0x08, 0x49, 0x2a, 0x1c, 0x08},      /* → */
    { 0x08, 0x1c, 0x2a, 0x49, 0x08, 0x08},      /* ← */
    { 0x0c, 0x02, 0xff, 0x02, 0x0c, 0x00},      /* ↓ */
    { 0x30, 0x40, 0xff, 0x40, 0x30, 0x00},      /* ↑ */
    { 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F},      /* ■ */
    { 0x00, 0x60, 0x90, 0x90, 0x60, 0x00},      /* ○ */
    { 0x08, 0x08, 0x08, 0x08, 0x08, 0x08},      /* - */
    { 0x08, 0x08, 0xFF, 0x08, 0x08, 0x08},      /* ⊥ */
};
#pragma unpack

#if SECURE_BOOT
#pragma section
#endif

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

