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

void r_rsip_func000(const uint32_t InData_PaddedMsg[], uint32_t MAX_CNT)
{
    int32_t iLoop = 0U, jLoop = 0U, kLoop = 0U, oLoop = 0U;
    uint32_t OFS_ADR = 0U;
    (void)iLoop;
    (void)jLoop;
    (void)kLoop;
    (void)oLoop;
    (void)OFS_ADR;
    RSIP.REG_0014H.WORD = 0x00020064U;
    for (iLoop = 0; iLoop < MAX_CNT; iLoop = iLoop + 16)
    {
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 0];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 1];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 2];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 3];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 4];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 5];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 6];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 7];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 8];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 9];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 10];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 11];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 12];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 13];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 14];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 15];
    }
    /* WAIT_LOOP */
    while (0U != RSIP.REG_00F8H.BIT.B2)
    {
        /* waiting */
    }
    RSIP.REG_0014H.WORD = 0x00000000U;
    RSIP.REG_0040H.WORD = 0x00001600U;
}
