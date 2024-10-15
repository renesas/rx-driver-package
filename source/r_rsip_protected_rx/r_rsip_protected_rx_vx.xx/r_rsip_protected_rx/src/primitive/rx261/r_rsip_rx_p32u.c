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

void r_rsip_p32u(const uint32_t InData_Text[], uint32_t OutData_Text[], uint32_t MAX_CNT)
{
    int32_t iLoop = 0U, jLoop = 0U, kLoop = 0U, oLoop = 0U;
    uint32_t OFS_ADR = 0U;
    (void)iLoop;
    (void)jLoop;
    (void)kLoop;
    (void)oLoop;
    (void)OFS_ADR;
    r_rsip_func100(0x30d01596U, 0xf484565cU, 0x6364d511U, 0xe347e2baU);
    RSIP.REG_0014H.WORD = 0x00020061U;
    RSIP.REG_00D4H.WORD = 0x00008020U;
    RSIP.REG_00D0H.WORD = 0x20008d06U;
    RSIP.REG_0008H.WORD = 0x000c1000U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = InData_Text[0];
    RSIP.REG_002CH.WORD = InData_Text[1];
    RSIP.REG_002CH.WORD = InData_Text[2];
    RSIP.REG_002CH.WORD = InData_Text[3];
    for (iLoop = 4; iLoop < MAX_CNT; iLoop = iLoop + 4)
    {
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = InData_Text[iLoop + 0];
        RSIP.REG_002CH.WORD = InData_Text[iLoop + 1];
        RSIP.REG_002CH.WORD = InData_Text[iLoop + 2];
        RSIP.REG_002CH.WORD = InData_Text[iLoop + 3];
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0008H.BIT.B30)
        {
            /* waiting */
        }
        OutData_Text[iLoop - 4] = RSIP.REG_002CH.WORD;
        OutData_Text[iLoop - 3] = RSIP.REG_002CH.WORD;
        OutData_Text[iLoop - 2] = RSIP.REG_002CH.WORD;
        OutData_Text[iLoop - 1] = RSIP.REG_002CH.WORD;
    }
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0008H.BIT.B30)
    {
        /* waiting */
    }
    OutData_Text[iLoop - 4] = RSIP.REG_002CH.WORD;
    OutData_Text[iLoop - 3] = RSIP.REG_002CH.WORD;
    OutData_Text[iLoop - 2] = RSIP.REG_002CH.WORD;
    OutData_Text[iLoop - 1] = RSIP.REG_002CH.WORD;
    r_rsip_func206();
    r_rsip_func101(0xed822f0aU, 0xe07a8d55U, 0xe7683852U, 0x557b0c1cU);
}
