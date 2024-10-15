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

void r_rsip_p50u(const uint32_t InData_Text[], uint32_t OutData_Text[], uint32_t MAX_CNT)
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
        r_rsip_func100(0x450c8010U, 0x9f7ddf05U, 0xc17ddf91U, 0x82e852b4U);
        RSIP.REG_0014H.WORD = 0x00020061U;
        RSIP.REG_00D4H.WORD = 0x40000000U;
        RSIP.REG_00D0H.WORD = 0x0a008106U;
        RSIP.REG_0008H.WORD = 0x000c1000U;
    }
    else if (0x00000001U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        RSIP.REG_0014H.WORD = 0x000003c1U;
        r_rsip_func100(0xb388a91aU, 0x6aa64e8dU, 0x19a2adfaU, 0x02d1293fU);
        RSIP.REG_0014H.WORD = 0x00020061U;
        RSIP.REG_00D4H.WORD = 0x40000000U;
        RSIP.REG_00D0H.WORD = 0x0a00810eU;
        RSIP.REG_0008H.WORD = 0x000c1000U;
    }
    else if (0x00000002U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        RSIP.REG_0014H.WORD = 0x000003c1U;
        r_rsip_func100(0x22b31657U, 0x308aed02U, 0x8d5592ccU, 0x76818293U);
        RSIP.REG_0014H.WORD = 0x00020061U;
        RSIP.REG_00D4H.WORD = 0x40000000U;
        RSIP.REG_00D0H.WORD = 0x0e008506U;
        RSIP.REG_0008H.WORD = 0x000c1000U;
    }
    else if (0x00000003U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        RSIP.REG_0014H.WORD = 0x000003c1U;
        r_rsip_func100(0xb8a04535U, 0x18b680f3U, 0x0a8d76ffU, 0x89483894U);
        RSIP.REG_0014H.WORD = 0x00020061U;
        RSIP.REG_00D4H.WORD = 0x40000000U;
        RSIP.REG_00D0H.WORD = 0x0900890eU;
        RSIP.REG_0008H.WORD = 0x000c1000U;
    }
    else if (0x00000004U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        RSIP.REG_0014H.WORD = 0x000003c1U;
        r_rsip_func100(0x6567a71eU, 0x33a00982U, 0xd1245ccdU, 0xc67f3c62U);
        RSIP.REG_0014H.WORD = 0x00020061U;
        RSIP.REG_00D4H.WORD = 0x40000000U;
        RSIP.REG_00D0H.WORD = 0x07008d06U;
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
        r_rsip_func101(0x8c7285c6U, 0xce56044eU, 0xdf5b9171U, 0xaf639e8cU);
    }
    else if (0x00000001U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        r_rsip_func206();
        r_rsip_func101(0x21637f85U, 0x657496fdU, 0xb9c70807U, 0x05b2c991U);
    }
    else if (0x00000002U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        r_rsip_func206();
        r_rsip_func101(0x9f7e0d38U, 0x0133a903U, 0x167eddedU, 0xd4c791aaU);
    }
    else if (0x00000003U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        r_rsip_func206();
        r_rsip_func101(0xa9180496U, 0x49a4005eU, 0xe332307bU, 0x3f7dfa4bU);
    }
    else if (0x00000004U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        r_rsip_func206();
        r_rsip_func101(0xcc0b421aU, 0x515dc845U, 0xc8c8dff0U, 0xbf684c90U);
    }
}
