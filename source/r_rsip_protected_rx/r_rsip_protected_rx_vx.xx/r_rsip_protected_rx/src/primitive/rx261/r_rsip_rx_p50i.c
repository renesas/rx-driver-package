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

rsip_err_t r_rsip_p50i(const uint32_t InData_Cmd[], const uint32_t InData_KeyIndex[], const uint32_t InData_IV[])
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
    RSIP.REG_0070H.WORD = 0x00500001U;
    RSIP.REG_004CH.WORD = 0x00000000U;
    RSIP.REG_0014H.WORD = 0x000000c7U;
    RSIP.REG_009CH.WORD = 0x80010080U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = InData_Cmd[0];
    RSIP.REG_0024H.WORD = 0x00000000U;
    RSIP.REG_0094H.WORD = 0x3000a880U;
    RSIP.REG_0094H.WORD = 0x00000004U;
    RSIP.REG_0094H.WORD = 0x00010020U;
    RSIP.REG_0094H.WORD = 0x0000b480U;
    RSIP.REG_0094H.WORD = 0x00000004U;
    RSIP.REG_0094H.WORD = 0x00000080U;
    RSIP.REG_0014H.WORD = 0x000000c7U;
    RSIP.REG_009CH.WORD = 0x800100c0U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = InData_KeyIndex[0];
    RSIP.REG_0024H.WORD = 0x00000000U;
    RSIP.REG_0014H.WORD = 0x000000a7U;
    RSIP.REG_009CH.WORD = 0x800100e0U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x00000050U);
    RSIP.REG_0024H.WORD = 0x00000000U;
    r_rsip_func101(0x089c5887U, 0x486f3e61U, 0x79feb25aU, 0x99eb3dacU);
    r_rsip_func043();
    RSIP.REG_0094H.WORD = 0x0000b4c0U;
    RSIP.REG_0094H.WORD = 0x00000007U;
    RSIP.REG_0014H.WORD = 0x000000a7U;
    RSIP.REG_009CH.WORD = 0x800100e0U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x00000050U);
    RSIP.REG_0024H.WORD = 0x00000000U;
    r_rsip_func101(0x3552058cU, 0xec3ea681U, 0xafa58d80U, 0x162a9bbbU);
    r_rsip_func044();
    RSIP.REG_0014H.WORD = 0x000007c1U;
    RSIP.REG_00D4H.WORD = 0x40000100U;
    RSIP.REG_00D0H.WORD = 0xf7009d07U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = InData_KeyIndex[1];
    RSIP.REG_002CH.WORD = InData_KeyIndex[2];
    RSIP.REG_002CH.WORD = InData_KeyIndex[3];
    RSIP.REG_002CH.WORD = InData_KeyIndex[4];
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = InData_KeyIndex[5];
    RSIP.REG_002CH.WORD = InData_KeyIndex[6];
    RSIP.REG_002CH.WORD = InData_KeyIndex[7];
    RSIP.REG_002CH.WORD = InData_KeyIndex[8];
    RSIP.REG_00B0H.WORD = 0x00001404U;
    RSIP.REG_0000H.WORD = 0x00c10021U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_0014H.WORD = 0x000003c1U;
    RSIP.REG_00D4H.WORD = 0x40000000U;
    RSIP.REG_00D0H.WORD = 0x07008d05U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = InData_KeyIndex[9];
    RSIP.REG_002CH.WORD = InData_KeyIndex[10];
    RSIP.REG_002CH.WORD = InData_KeyIndex[11];
    RSIP.REG_002CH.WORD = InData_KeyIndex[12];
    RSIP.REG_00D0H.WORD = 0x9c100005U;
    RSIP.REG_0000H.WORD = 0x00410011U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    r_rsip_func100(0xe0503836U, 0x0cd214a1U, 0x3080db10U, 0x8de5fcd4U);
    RSIP.REG_0040H.WORD = 0x00400000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    if (1U == (RSIP.REG_0040H.BIT.B22))
    {
        r_rsip_func102(0xf9ff4dadU, 0xc2360c68U, 0xaf642f8bU, 0x3819aa34U);
        RSIP.REG_006CH.WORD = 0x00000040U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0020H.BIT.B12)
        {
            /* waiting */
        }
        return RSIP_ERR_KEY_SET;
    }
    else
    {
        r_rsip_func100(0xa3875502U, 0xa7c39eadU, 0x653b85c5U, 0x0383680bU);
        RSIP.REG_00D0H.WORD = 0x08000085U;
        RSIP.REG_00B0H.WORD = 0x00001804U;
        RSIP.REG_0000H.WORD = 0x00430011U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        r_rsip_func100(0x2550e961U, 0xb5358c76U, 0x0f40e54dU, 0x2c8e23b3U);
        RSIP.REG_00D0H.WORD = 0x08000095U;
        RSIP.REG_0000H.WORD = 0x00430011U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        r_rsip_func100(0xa2f19fa7U, 0x2d159933U, 0xdab0ee28U, 0x018df61bU);
        RSIP.REG_0094H.WORD = 0x00007c04U;
        RSIP.REG_0040H.WORD = 0x00600000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (0x00000000U == (RSIP.REG_0044H.WORD & 0xffffffffU))
        {
            r_rsip_func101(0x2f51b935U, 0xf8001206U, 0xac0ba634U, 0x99a26f89U);
        }
        else if (0x00000001U == (RSIP.REG_0044H.WORD & 0xffffffffU))
        {
            r_rsip_func101(0x7bd99b13U, 0x32b0ccd7U, 0x073a820cU, 0xd0538713U);
        }
        else if (0x00000002U == (RSIP.REG_0044H.WORD & 0xffffffffU))
        {
            RSIP.REG_0014H.WORD = 0x000003c1U;
            RSIP.REG_00D0H.WORD = 0x08000045U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = InData_IV[0];
            RSIP.REG_002CH.WORD = InData_IV[1];
            RSIP.REG_002CH.WORD = InData_IV[2];
            RSIP.REG_002CH.WORD = InData_IV[3];
            r_rsip_func101(0xfa7b9d94U, 0xb3f547beU, 0x794fea50U, 0xcef262e7U);
        }
        else if (0x00000003U == (RSIP.REG_0044H.WORD & 0xffffffffU))
        {
            RSIP.REG_0014H.WORD = 0x000003c1U;
            RSIP.REG_00D0H.WORD = 0x08000045U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = InData_IV[0];
            RSIP.REG_002CH.WORD = InData_IV[1];
            RSIP.REG_002CH.WORD = InData_IV[2];
            RSIP.REG_002CH.WORD = InData_IV[3];
            r_rsip_func101(0xde32c157U, 0xa93d423cU, 0xbb54b1adU, 0x60bd6577U);
        }
        else if (0x00000004U == (RSIP.REG_0044H.WORD & 0xffffffffU))
        {
            RSIP.REG_0014H.WORD = 0x000003c1U;
            RSIP.REG_00D0H.WORD = 0x08000045U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = InData_IV[0];
            RSIP.REG_002CH.WORD = InData_IV[1];
            RSIP.REG_002CH.WORD = InData_IV[2];
            RSIP.REG_002CH.WORD = InData_IV[3];
            r_rsip_func101(0xb1fa6be2U, 0xa565f4e5U, 0x1dcb1327U, 0x2ba3dac3U);
        }
        else
        {
        }
        return RSIP_SUCCESS;
    }
}
