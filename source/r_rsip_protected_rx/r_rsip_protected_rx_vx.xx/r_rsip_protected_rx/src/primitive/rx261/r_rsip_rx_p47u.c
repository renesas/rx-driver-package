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

void r_rsip_p47u(const uint32_t InData_Text[], uint32_t OutData_Text[], uint32_t MAX_CNT)
{
    int32_t iLoop = 0U, jLoop = 0U, kLoop = 0U, oLoop = 0U;
    uint32_t OFS_ADR = 0U;
    (void)iLoop;
    (void)jLoop;
    (void)kLoop;
    (void)oLoop;
    (void)OFS_ADR;
    if (0x00000000U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        RSIP.REG_0014H.WORD = 0x000003c1U;
        r_rsip_func100(0xe7395012U, 0xe4c2e15aU, 0x54e7329eU, 0x979d93ebU);
        RSIP.REG_0014H.WORD = 0x00020061U;
        RSIP.REG_00D0H.WORD = 0x0a000106U;
        RSIP.REG_0008H.WORD = 0x000c1000U;
    }
    else if (0x00000001U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        RSIP.REG_0014H.WORD = 0x000003c1U;
        r_rsip_func100(0xc6f1f317U, 0x9a723c5dU, 0xa3e77bebU, 0xdd588e79U);
        RSIP.REG_0014H.WORD = 0x00020061U;
        RSIP.REG_00D0H.WORD = 0x0a00010eU;
        RSIP.REG_0008H.WORD = 0x000c1000U;
    }
    else if (0x00000002U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        RSIP.REG_0014H.WORD = 0x000003c1U;
        r_rsip_func100(0x6409a52bU, 0xb18ed938U, 0x76d9ae6fU, 0xa07155f1U);
        RSIP.REG_0014H.WORD = 0x00020061U;
        RSIP.REG_00D0H.WORD = 0x0e000506U;
        RSIP.REG_0008H.WORD = 0x000c1000U;
    }
    else if (0x00000003U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        RSIP.REG_0014H.WORD = 0x000003c1U;
        r_rsip_func100(0x7ddf29f7U, 0x53723d2fU, 0xe3b91596U, 0x3b9d259bU);
        RSIP.REG_0014H.WORD = 0x00020061U;
        RSIP.REG_00D0H.WORD = 0x0900090eU;
        RSIP.REG_0008H.WORD = 0x000c1000U;
    }
    else if (0x00000004U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        RSIP.REG_0014H.WORD = 0x000003c1U;
        r_rsip_func100(0xa1561cbeU, 0x3b3b6f3bU, 0x4b618c81U, 0xabfca094U);
        RSIP.REG_0014H.WORD = 0x00020061U;
        RSIP.REG_00D0H.WORD = 0x07000d06U;
        RSIP.REG_0008H.WORD = 0x000c1000U;
    }
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = InData_Text[0];
    RSIP.REG_002CH.WORD = InData_Text[1];
    RSIP.REG_002CH.WORD = InData_Text[2];
    RSIP.REG_002CH.WORD = InData_Text[3];
    for (iLoop = 4; iLoop < MAX_CNT ; iLoop = iLoop + 4)
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
    OutData_Text[MAX_CNT - 4] = RSIP.REG_002CH.WORD;
    OutData_Text[MAX_CNT - 3] = RSIP.REG_002CH.WORD;
    OutData_Text[MAX_CNT - 2] = RSIP.REG_002CH.WORD;
    OutData_Text[MAX_CNT - 1] = RSIP.REG_002CH.WORD;
    if (0x00000000U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        r_rsip_func206();
        r_rsip_func101(0x155f0e73U, 0x18d3e70dU, 0x2ef99dc4U, 0x07440c2bU);
    }
    else if (0x00000001U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        r_rsip_func206();
        r_rsip_func101(0x98befb50U, 0x88381f07U, 0x3133d34cU, 0xc90387cdU);
    }
    else if (0x00000002U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        r_rsip_func206();
        r_rsip_func101(0x4525a534U, 0xa02ac4deU, 0xca29ec2eU, 0xb5edf9f0U);
    }
    else if (0x00000003U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        r_rsip_func206();
        r_rsip_func101(0x44ddf30aU, 0xd0e7e1e0U, 0x39ce44d3U, 0x3e6ccacdU);
    }
    else if (0x00000004U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        r_rsip_func206();
        r_rsip_func101(0x2c8d65d7U, 0xb7583053U, 0x5e1dc7b1U, 0x91d2642dU);
    }
}
