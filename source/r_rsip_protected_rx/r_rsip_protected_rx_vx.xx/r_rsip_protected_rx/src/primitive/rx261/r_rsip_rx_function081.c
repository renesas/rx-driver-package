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
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 15.10.2024 1.00     First Release.
 *********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "./r_rsip_primitive.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
RSIP_SEC_P_SECURE_BOOT
void r_rsip_func081_r1(void)
{
    int32_t iLoop = 0U, jLoop = 0U, kLoop = 0U, oLoop = 0U;
    uint32_t OFS_ADR = 0U;
    (void)iLoop;
    (void)jLoop;
    (void)kLoop;
    (void)oLoop;
    (void)OFS_ADR;
    RSIP.REG_0094H.WORD = 0x00003424U;
    RSIP.REG_0094H.WORD = 0x3420a880U;
    RSIP.REG_0094H.WORD = 0x0000000aU;
    RSIP.REG_0094H.WORD = 0x10000821U;
    RSIP.REG_0094H.WORD = 0x0000b4e0U;
    RSIP.REG_0094H.WORD = 0x0000000dU;
    RSIP.REG_0094H.WORD = 0x342028e4U;
    RSIP.REG_0094H.WORD = 0x10000821U;
    RSIP.REG_0094H.WORD = 0x38008880U;
    RSIP.REG_0094H.WORD = 0x00000016U;
    RSIP.REG_0094H.WORD = 0x1000b420U;
    RSIP.REG_0094H.WORD = 0x00000016U;
    RSIP.REG_0094H.WORD = 0x38008880U;
    RSIP.REG_0094H.WORD = 0x0000001cU;
    RSIP.REG_0094H.WORD = 0x1000b420U;
    RSIP.REG_0094H.WORD = 0x00000016U;
    RSIP.REG_0094H.WORD = 0x38008880U;
    RSIP.REG_0094H.WORD = 0x00000022U;
    RSIP.REG_0094H.WORD = 0x1000b420U;
    RSIP.REG_0094H.WORD = 0x00000016U;
}
RSIP_SEC_DEFAULT
