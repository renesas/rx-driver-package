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

rsip_err_t r_rsip_pf4(const uint32_t InData_CurveType[], const uint32_t InData_DomainParam[], uint32_t OutData_PubKeyIndex[], uint32_t OutData_PrivKeyIndex[])
{
    int32_t iLoop = 0U, jLoop = 0U, kLoop = 0U, oLoop = 0U;
    uint32_t OFS_ADR = 0U;
    (void)iLoop;
    (void)jLoop;
    (void)kLoop;
    (void)oLoop;
    (void)OFS_ADR;
    if (0x0U != (RSIP.REG_006CH.WORD & 0x17U))
    {
        return RSIP_ERR_RESOURCE_CONFLICT;
    }
    RSIP.REG_0070H.WORD = 0x00f40001U;
    RSIP.REG_004CH.WORD = 0x00000000U;
    RSIP.REG_0014H.WORD = 0x000000c7U;
    RSIP.REG_009CH.WORD = 0x80010000U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = InData_CurveType[0];
    RSIP.REG_0024H.WORD = 0x00000000U;
    RSIP.REG_0094H.WORD = 0x3420a800U;
    RSIP.REG_0094H.WORD = 0x00000003U;
    RSIP.REG_0094H.WORD = 0x2000b400U;
    RSIP.REG_0094H.WORD = 0x00000002U;
    r_rsip_func070(InData_DomainParam);
    RSIP.REG_0094H.WORD = 0x000034c0U;
    RSIP.REG_00A0H.WORD = 0x20010000U;
    RSIP.REG_00B0H.WORD = 0x00002428U;
    r_rsip_func100(0x53ce95c2U, 0x92c1053aU, 0xae85bb5bU, 0x673bcdffU);
    r_rsip_func103();
    r_rsip_func100(0x6a0f3bf9U, 0xfa72ef9fU, 0x90cfcb3eU, 0x97206a0cU);
    RSIP.REG_0014H.WORD = 0x000000a1U;
    RSIP.REG_00D0H.WORD = 0x0c200104U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
    RSIP.REG_0000H.WORD = 0x00c10009U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_0000H.WORD = 0x00010009U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    r_rsip_func103();
    r_rsip_func100(0x841942d6U, 0x0bd57853U, 0xefbbfcbcU, 0xde707c97U);
    RSIP.REG_0014H.WORD = 0x000000a1U;
    RSIP.REG_00D0H.WORD = 0x0c200104U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
    RSIP.REG_0000H.WORD = 0x00c10011U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    r_rsip_func103();
    RSIP.REG_0014H.WORD = 0x000000a1U;
    RSIP.REG_00D0H.WORD = 0x0c200104U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
    RSIP.REG_0000H.WORD = 0x00c10011U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_00B0H.WORD = 0x0000140fU;
    RSIP.REG_0000H.WORD = 0x00c0001dU;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_0014H.WORD = 0x000000a5U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x00000001U);
    RSIP.REG_00B4H.WORD = 0x000f000aU;
    RSIP.REG_00B8H.WORD = 0x00140000U;
    RSIP.REG_00A4H.WORD = 0x0404000aU;
    RSIP.REG_00A0H.WORD = 0x20010001U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_00A8H.BIT.B0)
    {
        /* waiting */
    }
    RSIP.REG_00ACH.WORD = 0x00000001U;
    RSIP.REG_00B0H.WORD = 0x0000143cU;
    RSIP.REG_0000H.WORD = 0x00c0001dU;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_0014H.WORD = 0x000000a5U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x00000002U);
    RSIP.REG_00B0H.WORD = 0x00001419U;
    RSIP.REG_0000H.WORD = 0x00c00021U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_0094H.WORD = 0x00000821U;
    RSIP.REG_0094H.WORD = 0x00000863U;
    RSIP.REG_0094H.WORD = 0x000008a5U;
    for (iLoop = 0; iLoop < 64; iLoop = iLoop + 1)
    {
        RSIP.REG_00B4H.WORD = 0x00140027U;
        RSIP.REG_00B8H.WORD = 0x001e0000U;
        RSIP.REG_00A4H.WORD = 0x0404000aU;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_00B0H.WORD = 0x0000081aU;
        RSIP.REG_009CH.WORD = 0x80020000U;
        RSIP.REG_0000H.WORD = 0x03430009U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_0094H.WORD = 0x00000884U;
        RSIP.REG_0094H.WORD = 0x38000c21U;
        RSIP.REG_0094H.WORD = 0x2000d080U;
        RSIP.REG_0094H.WORD = 0x38000c63U;
        RSIP.REG_0094H.WORD = 0x1000d081U;
        RSIP.REG_0094H.WORD = 0x38008880U;
        RSIP.REG_0094H.WORD = 0x00000003U;
        RSIP.REG_009CH.WORD = 0x00000080U;
        RSIP.REG_0040H.WORD = 0x00260000U;
        r_rsip_func100(0x61732713U, 0x9b3317fcU, 0x5dbb959bU, 0xcc6f0d77U);
        RSIP.REG_0040H.WORD = 0x00400000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (1U == (RSIP.REG_0040H.BIT.B22))
        {
            RSIP.REG_00B4H.WORD = 0x0019001eU;
            RSIP.REG_00B8H.WORD = 0x004b0000U;
            RSIP.REG_00A4H.WORD = 0x04040009U;
            RSIP.REG_00A0H.WORD = 0x20010001U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_00A8H.BIT.B0)
            {
                /* waiting */
            }
            RSIP.REG_00ACH.WORD = 0x00000001U;
            r_rsip_func101(0x62f533b0U, 0xd7bfd47aU, 0x45507e15U, 0x8f7fea96U);
        }
        else
        {
            RSIP.REG_00B4H.WORD = 0x0019001eU;
            RSIP.REG_00B8H.WORD = 0x00270000U;
            RSIP.REG_00A4H.WORD = 0x04040009U;
            RSIP.REG_00A0H.WORD = 0x20010001U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_00A8H.BIT.B0)
            {
                /* waiting */
            }
            RSIP.REG_00ACH.WORD = 0x00000001U;
            r_rsip_func101(0x09d9d80eU, 0x3ff613ebU, 0xc2686674U, 0x8e266e6fU);
        }
        RSIP.REG_00B4H.WORD = 0x003c002bU;
        RSIP.REG_00B8H.WORD = 0x001e0000U;
        RSIP.REG_00A4H.WORD = 0x04040007U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_00B4H.WORD = 0x003c0027U;
        RSIP.REG_00B8H.WORD = 0x00230000U;
        RSIP.REG_00A4H.WORD = 0x04040007U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_00B0H.WORD = 0x0000081fU;
        RSIP.REG_009CH.WORD = 0x80020040U;
        RSIP.REG_0000H.WORD = 0x03430009U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_00B4H.WORD = 0x0019001eU;
        RSIP.REG_00B8H.WORD = 0x002b0000U;
        RSIP.REG_00A4H.WORD = 0x04040009U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_00B4H.WORD = 0x001a0023U;
        RSIP.REG_00B8H.WORD = 0x00270000U;
        RSIP.REG_00A4H.WORD = 0x04040009U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_0094H.WORD = 0x00002ca0U;
        r_rsip_func101(0xc949c84bU, 0x0a017027U, 0x6df61decU, 0x8aa72fbdU);
    }
    RSIP.REG_0094H.WORD = 0x380088a0U;
    RSIP.REG_0094H.WORD = 0x00000040U;
    RSIP.REG_009CH.WORD = 0x00000080U;
    RSIP.REG_0040H.WORD = 0x00260000U;
    RSIP.REG_0040H.WORD = 0x00402000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    RSIP.REG_00B4H.WORD = 0x00140027U;
    RSIP.REG_00B8H.WORD = 0x001e0000U;
    RSIP.REG_00A4H.WORD = 0x0404000aU;
    RSIP.REG_00A0H.WORD = 0x20010001U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_00A8H.BIT.B0)
    {
        /* waiting */
    }
    RSIP.REG_00ACH.WORD = 0x00000001U;
    RSIP.REG_00B0H.WORD = 0x0000081aU;
    RSIP.REG_009CH.WORD = 0x80020000U;
    RSIP.REG_0000H.WORD = 0x03430009U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_0094H.WORD = 0x00000884U;
    RSIP.REG_0094H.WORD = 0x38000c21U;
    RSIP.REG_0094H.WORD = 0x2000d080U;
    RSIP.REG_0094H.WORD = 0x38000c63U;
    RSIP.REG_0094H.WORD = 0x1000d081U;
    RSIP.REG_0094H.WORD = 0x38008880U;
    RSIP.REG_0094H.WORD = 0x00000003U;
    RSIP.REG_009CH.WORD = 0x00000080U;
    RSIP.REG_0040H.WORD = 0x00260000U;
    r_rsip_func100(0xad0d166cU, 0x6790687aU, 0x6c7ba571U, 0xd1106df8U);
    RSIP.REG_0040H.WORD = 0x00400000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    if (1U == (RSIP.REG_0040H.BIT.B22))
    {
        RSIP.REG_00B4H.WORD = 0x0019001eU;
        RSIP.REG_00B8H.WORD = 0x004b0000U;
        RSIP.REG_00A4H.WORD = 0x04040009U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        r_rsip_func101(0x1cfb1e80U, 0xd7bbbb0fU, 0x2abda6bbU, 0x1dec4b47U);
    }
    else
    {
        RSIP.REG_00B4H.WORD = 0x0019001eU;
        RSIP.REG_00B8H.WORD = 0x00270000U;
        RSIP.REG_00A4H.WORD = 0x04040009U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        r_rsip_func101(0x96219509U, 0x86a67af6U, 0x63c3bd6bU, 0x0b0f375cU);
    }
    RSIP.REG_00B4H.WORD = 0x000f0027U;
    RSIP.REG_00B8H.WORD = 0x00320000U;
    RSIP.REG_00A4H.WORD = 0x04040009U;
    RSIP.REG_00A0H.WORD = 0x20010001U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_00A8H.BIT.B0)
    {
        /* waiting */
    }
    RSIP.REG_00ACH.WORD = 0x00000001U;
    RSIP.REG_0094H.WORD = 0x00003406U;
    r_rsip_func071(InData_DomainParam);
    RSIP.REG_0094H.WORD = 0x00003420U;
    RSIP.REG_0014H.WORD = 0x000000a7U;
    RSIP.REG_009CH.WORD = 0x800100e0U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x000000f4U);
    RSIP.REG_0024H.WORD = 0x00000000U;
    r_rsip_func101(0x636aa1a0U, 0xfa79ccdcU, 0xb9a82513U, 0xea217cc9U);
    r_rsip_func088();
    r_rsip_func100(0x68089f0cU, 0x23c5fd93U, 0xf5250e62U, 0x8e7ec2c8U);
    RSIP.REG_0040H.WORD = 0x00400000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    if (1U == (RSIP.REG_0040H.BIT.B22))
    {
        r_rsip_func102(0xe7ff197fU, 0x635bc016U, 0xd902e748U, 0x891db66aU);
        RSIP.REG_006CH.WORD = 0x00000040U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0020H.BIT.B12)
        {
            /* waiting */
        }
        return RSIP_ERR_FAIL;
    }
    else
    {
        r_rsip_func100(0xb179e806U, 0xcc48b7fcU, 0x0df747d8U, 0xc9a23722U);
        r_rsip_func103();
        RSIP.REG_0014H.WORD = 0x000000a1U;
        RSIP.REG_00D0H.WORD = 0x0c200104U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        RSIP.REG_009CH.WORD = 0x80010000U;
        RSIP.REG_0000H.WORD = 0x03410005U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_0000H.WORD = 0x0001000dU;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_0094H.WORD = 0x000034c0U;
        RSIP.REG_0014H.WORD = 0x000000a7U;
        RSIP.REG_009CH.WORD = 0x800100e0U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x0000f401U);
        RSIP.REG_0024H.WORD = 0x00000000U;
        r_rsip_func101(0xebbb7385U, 0xa916d166U, 0x1b7f86d4U, 0x9b786406U);
        r_rsip_func043();
        r_rsip_func074();
        RSIP.REG_0014H.WORD = 0x000000a7U;
        RSIP.REG_009CH.WORD = 0x800100e0U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x0000f401U);
        RSIP.REG_0024H.WORD = 0x00000000U;
        r_rsip_func101(0xe3fc641cU, 0xd2e61152U, 0x131cb0b9U, 0x397111c8U);
        r_rsip_func044();
        r_rsip_func100(0xfbb9b279U, 0x02fa4c8eU, 0xff8a80caU, 0x1632ac65U);
        RSIP.REG_00B0H.WORD = 0x00001832U;
        RSIP.REG_00D4H.WORD = 0x40000100U;
        RSIP.REG_00D0H.WORD = 0xe7009d07U;
        RSIP.REG_0000H.WORD = 0x00430021U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_0008H.WORD = 0x00001022U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0008H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PrivKeyIndex[1] = RSIP.REG_002CH.WORD;
        OutData_PrivKeyIndex[2] = RSIP.REG_002CH.WORD;
        OutData_PrivKeyIndex[3] = RSIP.REG_002CH.WORD;
        OutData_PrivKeyIndex[4] = RSIP.REG_002CH.WORD;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0008H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PrivKeyIndex[5] = RSIP.REG_002CH.WORD;
        OutData_PrivKeyIndex[6] = RSIP.REG_002CH.WORD;
        OutData_PrivKeyIndex[7] = RSIP.REG_002CH.WORD;
        OutData_PrivKeyIndex[8] = RSIP.REG_002CH.WORD;
        r_rsip_func100(0x2ce7f7eeU, 0x1bd4fc6eU, 0x8f2562fdU, 0x6d40f37eU);
        RSIP.REG_0014H.WORD = 0x000000a1U;
        RSIP.REG_00D0H.WORD = 0x0c000104U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        RSIP.REG_00D4H.WORD = 0x40000000U;
        RSIP.REG_00D0H.WORD = 0x09108105U;
        RSIP.REG_0000H.WORD = 0x00410011U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_0008H.WORD = 0x00001012U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0008H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PrivKeyIndex[9] = RSIP.REG_002CH.WORD;
        OutData_PrivKeyIndex[10] = RSIP.REG_002CH.WORD;
        OutData_PrivKeyIndex[11] = RSIP.REG_002CH.WORD;
        OutData_PrivKeyIndex[12] = RSIP.REG_002CH.WORD;
        r_rsip_func100(0x0a1fe73aU, 0x1be67c2aU, 0x2e52006cU, 0xb533ca55U);
        RSIP.REG_009CH.WORD = 0x81010000U;
        RSIP.REG_0008H.WORD = 0x00005006U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0008H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PrivKeyIndex[0] = RSIP.REG_002CH.WORD;
        r_rsip_func100(0x80e7bb19U, 0xcefc7d06U, 0xf5fd4403U, 0xe91ac49eU);
        r_rsip_func103();
        RSIP.REG_0014H.WORD = 0x000000a1U;
        RSIP.REG_00D0H.WORD = 0x0c200104U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        RSIP.REG_009CH.WORD = 0x80010000U;
        RSIP.REG_0000H.WORD = 0x03410005U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_0000H.WORD = 0x0001000dU;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_0094H.WORD = 0x000034c0U;
        RSIP.REG_0014H.WORD = 0x000000a7U;
        RSIP.REG_009CH.WORD = 0x800100e0U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x0000f402U);
        RSIP.REG_0024H.WORD = 0x00000000U;
        r_rsip_func101(0x4cecddc9U, 0xe73f5c3aU, 0xf7360ac2U, 0x159f193bU);
        r_rsip_func043();
        r_rsip_func075();
        RSIP.REG_0014H.WORD = 0x000000a7U;
        RSIP.REG_009CH.WORD = 0x800100e0U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x0000f402U);
        RSIP.REG_0024H.WORD = 0x00000000U;
        r_rsip_func101(0xcfec5514U, 0x8d1d6f4eU, 0xb3665a84U, 0x714b3786U);
        r_rsip_func044();
        RSIP.REG_0014H.WORD = 0x000000a1U;
        RSIP.REG_00D0H.WORD = 0x08000044U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        RSIP.REG_0014H.WORD = 0x000000a1U;
        RSIP.REG_00D0H.WORD = 0x08000054U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        r_rsip_func100(0x536e5264U, 0xa73a6192U, 0xef71b511U, 0x142a8480U);
        RSIP.REG_00D4H.WORD = 0x40000100U;
        RSIP.REG_00D0H.WORD = 0xe8008107U;
        RSIP.REG_00B0H.WORD = 0x00001828U;
        RSIP.REG_0000H.WORD = 0x00430021U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_0008H.WORD = 0x00001022U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0008H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[1] = RSIP.REG_002CH.WORD;
        OutData_PubKeyIndex[2] = RSIP.REG_002CH.WORD;
        OutData_PubKeyIndex[3] = RSIP.REG_002CH.WORD;
        OutData_PubKeyIndex[4] = RSIP.REG_002CH.WORD;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0008H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[5] = RSIP.REG_002CH.WORD;
        OutData_PubKeyIndex[6] = RSIP.REG_002CH.WORD;
        OutData_PubKeyIndex[7] = RSIP.REG_002CH.WORD;
        OutData_PubKeyIndex[8] = RSIP.REG_002CH.WORD;
        r_rsip_func100(0x70362d72U, 0x60b1f2eeU, 0x747cf428U, 0xed221c78U);
        RSIP.REG_00D4H.WORD = 0x40000100U;
        RSIP.REG_00D0H.WORD = 0xe8008107U;
        RSIP.REG_00B0H.WORD = 0x0000182dU;
        RSIP.REG_0000H.WORD = 0x00430021U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_0008H.WORD = 0x00001022U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0008H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[9] = RSIP.REG_002CH.WORD;
        OutData_PubKeyIndex[10] = RSIP.REG_002CH.WORD;
        OutData_PubKeyIndex[11] = RSIP.REG_002CH.WORD;
        OutData_PubKeyIndex[12] = RSIP.REG_002CH.WORD;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0008H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[13] = RSIP.REG_002CH.WORD;
        OutData_PubKeyIndex[14] = RSIP.REG_002CH.WORD;
        OutData_PubKeyIndex[15] = RSIP.REG_002CH.WORD;
        OutData_PubKeyIndex[16] = RSIP.REG_002CH.WORD;
        r_rsip_func100(0x8d0489a7U, 0x2c1a56dbU, 0x61e8cddbU, 0x52e7cf75U);
        RSIP.REG_0014H.WORD = 0x000000a1U;
        RSIP.REG_00D0H.WORD = 0x0c000104U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        RSIP.REG_00D4H.WORD = 0x40000000U;
        RSIP.REG_00D0H.WORD = 0x09108105U;
        RSIP.REG_0000H.WORD = 0x00410011U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_0008H.WORD = 0x00001012U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0008H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[17] = RSIP.REG_002CH.WORD;
        OutData_PubKeyIndex[18] = RSIP.REG_002CH.WORD;
        OutData_PubKeyIndex[19] = RSIP.REG_002CH.WORD;
        OutData_PubKeyIndex[20] = RSIP.REG_002CH.WORD;
        r_rsip_func100(0x5e83270fU, 0xdf6738fdU, 0x84538307U, 0x055f5aeeU);
        RSIP.REG_009CH.WORD = 0x81010000U;
        RSIP.REG_0008H.WORD = 0x00005006U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0008H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[0] = RSIP.REG_002CH.WORD;
        r_rsip_func102(0x90180423U, 0x4264e7bdU, 0x45e21646U, 0xd2a7a123U);
        RSIP.REG_006CH.WORD = 0x00000040U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0020H.BIT.B12)
        {
            /* waiting */
        }
        return RSIP_SUCCESS;
    }
}
