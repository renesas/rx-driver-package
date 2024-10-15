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
void r_rsip_func057_r1(const uint32_t ARG1[], const uint32_t ARG2[], uint32_t ARG3[], uint32_t INST_DATA_SIZE)
{
    int32_t iLoop = 0U, jLoop = 0U, kLoop = 0U, oLoop = 0U;
    uint32_t OFS_ADR = 0U;
    (void)iLoop;
    (void)jLoop;
    (void)kLoop;
    (void)oLoop;
    (void)OFS_ADR;
    RSIP.REG_0094H.WORD = 0x00003467U;
    RSIP.REG_0014H.WORD = 0x000003c1U;
    RSIP.REG_00D0H.WORD = 0x08000065U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = ARG1[0];
    RSIP.REG_002CH.WORD = ARG1[1];
    RSIP.REG_002CH.WORD = ARG1[2];
    RSIP.REG_002CH.WORD = ARG1[3];
    RSIP.REG_0014H.WORD = 0x000000a1U;
    RSIP.REG_00D0H.WORD = 0x08000074U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
    RSIP.REG_0094H.WORD = 0x000034c0U;
    RSIP.REG_0014H.WORD = 0x000000a7U;
    RSIP.REG_009CH.WORD = 0x800100e0U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x000000ffU);
    RSIP.REG_0024H.WORD = 0x00000000U;
    r_rsip_func101(0x57a89e91U, 0x0269b41dU, 0xff3a4e6eU, 0xf66570b7U);
    r_rsip_func043();
    RSIP.REG_0094H.WORD = 0x000034c4U;
    RSIP.REG_0014H.WORD = 0x000000a7U;
    RSIP.REG_009CH.WORD = 0x800100e0U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x000000feU);
    RSIP.REG_0024H.WORD = 0x00000000U;
    r_rsip_func101(0xeebe827dU, 0x53269c98U, 0x1e0e1f2aU, 0xefc0a0c4U);
    r_rsip_func044();
    r_rsip_func100(0x35f021f5U, 0xe4e2d110U, 0xc258a170U, 0x71d32d6eU);
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
    r_rsip_func081_r1();
    RSIP.REG_0094H.WORD = 0x00007c01U;
    RSIP.REG_0040H.WORD = 0x00600000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    oLoop = 0;
    if (0x00000000U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        for (iLoop = 0; iLoop < (INST_DATA_SIZE - 4) ; iLoop = iLoop + 4)
        {
            r_rsip_func100(0x42a8ba00U, 0xc79b3fabU, 0xcd22cf6dU, 0xaf9cd3b7U);
            RSIP.REG_0014H.WORD = 0x000003c1U;
            RSIP.REG_00D0H.WORD = 0xd922090dU;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = ARG2[iLoop + 0];
            RSIP.REG_002CH.WORD = ARG2[iLoop + 1];
            RSIP.REG_002CH.WORD = ARG2[iLoop + 2];
            RSIP.REG_002CH.WORD = ARG2[iLoop + 3];
            RSIP.REG_00D4H.WORD = 0x40000000U;
            RSIP.REG_00D0H.WORD = 0xe7008d05U;
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
            ARG3[iLoop + 1] = RSIP.REG_002CH.WORD;
            ARG3[iLoop + 2] = RSIP.REG_002CH.WORD;
            ARG3[iLoop + 3] = RSIP.REG_002CH.WORD;
            ARG3[iLoop + 4] = RSIP.REG_002CH.WORD;
            r_rsip_func101(0x6c4a506fU, 0xa4b3f4feU, 0x99ac6c42U, 0x130f383dU);
        }
        oLoop = iLoop;
        r_rsip_func101(0x7358df5eU, 0xe840c6dcU, 0x775fab85U, 0xfe2c702cU);
    }
    else if (0x00000016U == (RSIP.REG_0044H.WORD & 0xffffffffU))
    {
        for (iLoop = 0; iLoop < (INST_DATA_SIZE - 4) ; iLoop = iLoop + 4)
        {
            r_rsip_func100(0x7b5d7360U, 0xfea7e26eU, 0x076e6519U, 0x2ca7019dU);
            RSIP.REG_0014H.WORD = 0x000003c1U;
            RSIP.REG_00D0H.WORD = 0xd922090dU;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = ARG2[iLoop + 0];
            RSIP.REG_002CH.WORD = ARG2[iLoop + 1];
            RSIP.REG_002CH.WORD = ARG2[iLoop + 2];
            RSIP.REG_002CH.WORD = ARG2[iLoop + 3];
            RSIP.REG_00D4H.WORD = 0x40000000U;
            RSIP.REG_00D0H.WORD = 0xe8008105U;
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
            ARG3[iLoop + 1] = RSIP.REG_002CH.WORD;
            ARG3[iLoop + 2] = RSIP.REG_002CH.WORD;
            ARG3[iLoop + 3] = RSIP.REG_002CH.WORD;
            ARG3[iLoop + 4] = RSIP.REG_002CH.WORD;
            r_rsip_func101(0xea4b8220U, 0xbc4f7475U, 0xc4ec3156U, 0x40c4e546U);
        }
        oLoop = iLoop;
        r_rsip_func101(0x0ca9309aU, 0x38b44a9aU, 0x623569c0U, 0xbcbabc2bU);
    }
    r_rsip_func100(0xae2f229fU, 0xb1856cafU, 0xa6e2cdc2U, 0x1449a0b6U);
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
    ARG3[oLoop + 1] = RSIP.REG_002CH.WORD;
    ARG3[oLoop + 2] = RSIP.REG_002CH.WORD;
    ARG3[oLoop + 3] = RSIP.REG_002CH.WORD;
    ARG3[oLoop + 4] = RSIP.REG_002CH.WORD;
    RSIP.REG_0014H.WORD = 0x000003c1U;
    RSIP.REG_00D0H.WORD = 0x0922090dU;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = ARG2[iLoop + 0];
    RSIP.REG_002CH.WORD = ARG2[iLoop + 1];
    RSIP.REG_002CH.WORD = ARG2[iLoop + 2];
    RSIP.REG_002CH.WORD = ARG2[iLoop + 3];
    RSIP.REG_00D0H.WORD = 0x9c300005U;
    RSIP.REG_0000H.WORD = 0x00410011U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_0094H.WORD = 0x000034e3U;
    RSIP.REG_0094H.WORD = 0x00007c07U;
    RSIP.REG_0040H.WORD = 0x00602000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
}
RSIP_SEC_DEFAULT
