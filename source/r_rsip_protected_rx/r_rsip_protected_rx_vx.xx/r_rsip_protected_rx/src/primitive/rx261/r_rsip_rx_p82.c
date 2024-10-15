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
rsip_err_t r_rsip_p82(void)
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
    RSIP.REG_0070H.WORD = 0x00820001U;
    RSIP.REG_004CH.WORD = 0x00000000U;
    RSIP.REG_0014H.WORD = 0x000000a1U;
    RSIP.REG_00D0H.WORD = 0x0b0700c4U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x01a0d7adU);
    RSIP.REG_0014H.WORD = 0x000000a1U;
    RSIP.REG_00D0H.WORD = 0x08000074U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
    RSIP.REG_0094H.WORD = 0x3000a820U;
    RSIP.REG_0094H.WORD = 0x00000003U;
    RSIP.REG_0094H.WORD = 0x00010020U;
    RSIP.REG_0094H.WORD = 0x00000821U;
    RSIP.REG_0094H.WORD = 0x00000080U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_00E8H.BIT.B0)
    {
        /* waiting */
    }
    r_rsip_func100(0xe60147d0U, 0x7586528aU, 0xf51eb608U, 0xb5eca5d4U);
    RSIP.REG_0094H.WORD = 0x00007c01U;
    RSIP.REG_0040H.WORD = 0x00600000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    if (0x00000000U == (RSIP.REG_0044H.WORD & 0x0000ffffU))
    {
        /* WAIT_LOOP */
        while (0U != RSIP.REG_00E8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00E4H.WORD = 0x00200003U;
        r_rsip_func101(0x3419c752U, 0xb9632a78U, 0x1376b94dU, 0x5190cb4fU);
    }
    else if (0x00000001U == (RSIP.REG_0044H.WORD & 0x0000ffffU))
    {
        /* WAIT_LOOP */
        while (0U != RSIP.REG_00E8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00E4H.WORD = 0x00200001U;
        r_rsip_func101(0x66a5d30cU, 0xac72b9f6U, 0xe397c717U, 0xe2346e6aU);
    }
    else if (0x00000002U == (RSIP.REG_0044H.WORD & 0x0000ffffU))
    {
        /* WAIT_LOOP */
        while (0U != RSIP.REG_00E8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00E4H.WORD = 0x00200002U;
        r_rsip_func101(0x866db390U, 0x797451d4U, 0xd904a985U, 0xa176ddadU);
    }
    RSIP.REG_00D0H.WORD = 0x08000044U;
    RSIP.REG_009CH.WORD = 0x81010020U;
    RSIP.REG_0000H.WORD = 0x00490005U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_0094H.WORD = 0x00000800U;
    RSIP.REG_009CH.WORD = 0x80880000U;
    RSIP.REG_0000H.WORD = 0x03400021U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_0094H.WORD = 0x000008e7U;
    RSIP.REG_0094H.WORD = 0x0000b480U;
    RSIP.REG_0094H.WORD = 0xffffffffU;
    RSIP.REG_0094H.WORD = 0x0000b4c0U;
    RSIP.REG_0094H.WORD = 0x00000001U;
    RSIP.REG_00D0H.WORD = 0x0e340406U;
    for (iLoop = 0; iLoop < 32; iLoop = iLoop + 1)
    {
        RSIP.REG_009CH.WORD = 0x80010000U;
        RSIP.REG_0000H.WORD = 0x03440005U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_009CH.WORD = 0x81010000U;
        RSIP.REG_0000H.WORD = 0x00490005U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_0094H.WORD = 0x00003420U;
        for (jLoop = 0; jLoop < 8; jLoop = jLoop + 1)
        {
            RSIP.REG_0094H.WORD = 0x00003441U;
            RSIP.REG_0094H.WORD = 0x00008c40U;
            RSIP.REG_0094H.WORD = 0x0000000fU;
            RSIP.REG_0094H.WORD = 0x00016c42U;
            RSIP.REG_0094H.WORD = 0x01003862U;
            RSIP.REG_0094H.WORD = 0x00002c60U;
            RSIP.REG_0094H.WORD = 0x01003c62U;
            RSIP.REG_0094H.WORD = 0x00046821U;
        }
        RSIP.REG_0094H.WORD = 0x00003420U;
        RSIP.REG_0094H.WORD = 0x00003441U;
        RSIP.REG_0094H.WORD = 0x00008c40U;
        RSIP.REG_0094H.WORD = 0x80000000U;
        RSIP.REG_0094H.WORD = 0x38000882U;
        RSIP.REG_0094H.WORD = 0x00030020U;
        RSIP.REG_0094H.WORD = 0x00002ca0U;
        RSIP.REG_0094H.WORD = 0x342028c5U;
        RSIP.REG_0094H.WORD = 0x100034c5U;
        RSIP.REG_0094H.WORD = 0x00000060U;
        RSIP.REG_0094H.WORD = 0x0000b4a0U;
        RSIP.REG_0094H.WORD = 0x00000001U;
        RSIP.REG_0094H.WORD = 0x00000080U;
        for (jLoop = 0; jLoop < 31; jLoop = jLoop + 1)
        {
            RSIP.REG_0094H.WORD = 0x00016c21U;
            RSIP.REG_0094H.WORD = 0x00003481U;
            RSIP.REG_0094H.WORD = 0x00008c80U;
            RSIP.REG_0094H.WORD = 0x80000000U;
            RSIP.REG_0094H.WORD = 0x38000882U;
            RSIP.REG_0094H.WORD = 0x00030020U;
            RSIP.REG_0094H.WORD = 0x00002ca0U;
            RSIP.REG_0094H.WORD = 0x342028c5U;
            RSIP.REG_0094H.WORD = 0x100034c5U;
            RSIP.REG_0094H.WORD = 0x00000060U;
            RSIP.REG_0094H.WORD = 0x00003444U;
            RSIP.REG_0094H.WORD = 0x0000b4a0U;
            RSIP.REG_0094H.WORD = 0x00000001U;
            RSIP.REG_0094H.WORD = 0x00000080U;
        }
        RSIP.REG_0094H.WORD = 0x00003420U;
        for (jLoop = 0; jLoop < 32; jLoop = jLoop + 1)
        {
            RSIP.REG_0094H.WORD = 0x38008c20U;
            RSIP.REG_0094H.WORD = 0x00000001U;
            RSIP.REG_0094H.WORD = 0x00020020U;
            RSIP.REG_0094H.WORD = 0x00002ce0U;
            RSIP.REG_0094H.WORD = 0x00000060U;
            RSIP.REG_0094H.WORD = 0x0000a4e0U;
            RSIP.REG_0094H.WORD = 0x00010000U;
            RSIP.REG_0094H.WORD = 0x00000080U;
            RSIP.REG_0094H.WORD = 0x00016821U;
        }
    }
    RSIP.REG_0040H.WORD = 0x00001200U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_00C8H.BIT.B6)
    {
        /* waiting */
    }
    RSIP.REG_00D0H.WORD = 0x00000000U;
    RSIP.REG_0040H.WORD = 0x00000400U;
    RSIP.REG_0094H.WORD = 0x00000800U;
    RSIP.REG_0094H.WORD = 0x0000b420U;
    RSIP.REG_0094H.WORD = 0x00000033U;
    RSIP.REG_0094H.WORD = 0x342028c1U;
    RSIP.REG_0094H.WORD = 0x2000d011U;
    RSIP.REG_0094H.WORD = 0x0000b4a0U;
    RSIP.REG_0094H.WORD = 0x00000348U;
    RSIP.REG_0094H.WORD = 0x0000b4c0U;
    RSIP.REG_0094H.WORD = 0x000000b7U;
    RSIP.REG_0094H.WORD = 0x00003467U;
    RSIP.REG_0094H.WORD = 0x00008c60U;
    RSIP.REG_0094H.WORD = 0x0000ffffU;
    RSIP.REG_0094H.WORD = 0x34202865U;
    RSIP.REG_0094H.WORD = 0x2000d012U;
    RSIP.REG_0094H.WORD = 0x342028c3U;
    RSIP.REG_0094H.WORD = 0x2000d012U;
    RSIP.REG_0094H.WORD = 0x001068e7U;
    RSIP.REG_0094H.WORD = 0x342028e5U;
    RSIP.REG_0094H.WORD = 0x2000d013U;
    RSIP.REG_0094H.WORD = 0x342028c7U;
    RSIP.REG_0094H.WORD = 0x2000d013U;
    RSIP.REG_0094H.WORD = 0x00002467U;
    r_rsip_func100(0x371d76c7U, 0x9510dbbdU, 0xab17feb8U, 0xe103e490U);
    RSIP.REG_0094H.WORD = 0x38008860U;
    RSIP.REG_0094H.WORD = 0x00000400U;
    RSIP.REG_009CH.WORD = 0x00000080U;
    RSIP.REG_0040H.WORD = 0x00260000U;
    RSIP.REG_0040H.WORD = 0x00402000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    RSIP.REG_0008H.WORD = 0x00020000U;
    RSIP.REG_0094H.WORD = 0x0000b420U;
    RSIP.REG_0094H.WORD = 0x0000005AU;
    RSIP.REG_0094H.WORD = 0x00000842U;
    RSIP.REG_0094H.WORD = 0x00000863U;
    RSIP.REG_0094H.WORD = 0x00000884U;
    RSIP.REG_0094H.WORD = 0x0000b4a0U;
    RSIP.REG_0094H.WORD = 0x00000002U;
    for (iLoop = 0; iLoop < 16; iLoop = iLoop + 1)
    {
        RSIP.REG_0094H.WORD = 0x010038c4U;
        RSIP.REG_0094H.WORD = 0x34202826U;
        RSIP.REG_0094H.WORD = 0x10005002U;
        RSIP.REG_0094H.WORD = 0x00002466U;
        RSIP.REG_0094H.WORD = 0x00002c40U;
        RSIP.REG_0094H.WORD = 0x00002485U;
    }
    r_rsip_func100(0xc880f44eU, 0x1fc73220U, 0x1b9c3e9eU, 0x0a2caaadU);
    RSIP.REG_0094H.WORD = 0x38008860U;
    RSIP.REG_0094H.WORD = 0x00000100U;
    RSIP.REG_009CH.WORD = 0x00000080U;
    RSIP.REG_0040H.WORD = 0x00260000U;
    RSIP.REG_0040H.WORD = 0x00402000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    RSIP.REG_0008H.WORD = 0x00020000U;
    RSIP.REG_0014H.WORD = 0x000000a1U;
    RSIP.REG_00D0H.WORD = 0x0c000104U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
    RSIP.REG_009CH.WORD = 0x80010020U;
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
    RSIP.REG_0094H.WORD = 0x00002c20U;
    RSIP.REG_0094H.WORD = 0x38000c00U;
    RSIP.REG_009CH.WORD = 0x00000080U;
    RSIP.REG_0040H.WORD = 0x00A60000U;
    r_rsip_func100(0x54bac3c8U, 0x14aac977U, 0xec346248U, 0xb619831fU);
    RSIP.REG_0040H.WORD = 0x00400000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    if (1U == (RSIP.REG_0040H.BIT.B22))
    {
        r_rsip_func102(0x41ec7edeU, 0x9c00ebcfU, 0x7443d110U, 0x81602725U);
        RSIP.REG_006CH.WORD = 0x00000040U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0020H.BIT.B12)
        {
            /* waiting */
        }
        return RSIP_ERR_RETRY;
    }
    else
    {
        r_rsip_func100(0xba50928dU, 0x0cdc0163U, 0x31df3781U, 0xd64e1094U);
        RSIP.REG_0014H.WORD = 0x000000a1U;
        RSIP.REG_00D0H.WORD = 0x0c300104U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        RSIP.REG_009CH.WORD = 0x80040000U;
        RSIP.REG_0000H.WORD = 0x03410011U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_00D4H.WORD = 0x00000600U;
        RSIP.REG_00D0H.WORD = 0x0e349407U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_00E8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00E4H.WORD = 0x00200003U;
        RSIP.REG_0000H.WORD = 0x00440071U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_00D0H.WORD = 0x0e340505U;
        RSIP.REG_0000H.WORD = 0x00440011U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_00E8H.BIT.B3)
        {
            /* waiting */
        }
        RSIP.REG_00E0H.WORD = 0x00000000U;
        RSIP.REG_009CH.WORD = 0x80040080U;
        RSIP.REG_0000H.WORD = 0x03410011U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_0014H.WORD = 0x000000a1U;
        RSIP.REG_00D0H.WORD = 0x080000b4U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        r_rsip_func100(0x364b222eU, 0xc366e68eU, 0x41a24f27U, 0xee46e8acU);
        RSIP.REG_0014H.WORD = 0x000003a1U;
        RSIP.REG_00D0H.WORD = 0x08000075U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        RSIP.REG_002CH.WORD = change_endian_long(0x00000001U);
        RSIP.REG_00D4H.WORD = 0x00000100U;
        RSIP.REG_00D0H.WORD = 0x07338d07U;
        RSIP.REG_009CH.WORD = 0x81080000U;
        RSIP.REG_0000H.WORD = 0x00490021U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_00D0H.WORD = 0x080000b5U;
        RSIP.REG_0000H.WORD = 0x00410011U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        r_rsip_func100(0x00d86584U, 0x82c19787U, 0x07749e8eU, 0x5820d965U);
        RSIP.REG_00D0H.WORD = 0x08000075U;
        RSIP.REG_0000H.WORD = 0x00410011U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        r_rsip_func103();
        r_rsip_func100(0x29ba5f6cU, 0xc10e2eeeU, 0xb330d140U, 0x0e0a8d07U);
        RSIP.REG_0014H.WORD = 0x000000a1U;
        RSIP.REG_00D0H.WORD = 0x0c2000d4U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        r_rsip_func100(0x510dcfecU, 0x77923d85U, 0x74a7f74fU, 0x337b16d2U);
        r_rsip_func103();
        RSIP.REG_0014H.WORD = 0x000000a1U;
        RSIP.REG_00D0H.WORD = 0x0c200104U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        RSIP.REG_00A0H.WORD = 0x00030000U;
        RSIP.REG_0004H.WORD = 0x20000000U;
        RSIP.REG_00B0H.WORD = 0x00000401U;
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
        RSIP.REG_0004H.WORD = 0x00000000U;
        RSIP.REG_00A0H.WORD = 0x00030000U;
        RSIP.REG_00B0H.WORD = 0x000074c0U;
        RSIP.REG_0000H.WORD = 0x00c00601U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_0070H.WORD = 0x00008002U;
        RSIP.REG_0078H.WORD = 0x00000d01U;
        RSIP.REG_0070H.WORD = 0x00008001U;
        RSIP.REG_0078H.WORD = 0x00000214U;
        r_rsip_func102(0x3293c9f3U, 0x8b9cfd7aU, 0xff878c95U, 0x0c90c6e3U);
        RSIP.REG_006CH.WORD = 0x00000040U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0020H.BIT.B12)
        {
            /* waiting */
        }
        return RSIP_SUCCESS;
    }
}
RSIP_SEC_DEFAULT
