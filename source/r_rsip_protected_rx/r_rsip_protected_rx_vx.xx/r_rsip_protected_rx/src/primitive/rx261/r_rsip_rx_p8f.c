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

rsip_err_t r_rsip_p8f_r1(const uint32_t InData_Cmd[], const uint32_t InData_KeyIndex[], const uint32_t InData_WrappedKeyType[], const uint32_t InData_WrappedKeyIndex[], uint32_t KEY_INDEX_SIZE, uint32_t OutData_Text[], uint32_t WRAPPED_KEY_SIZE)
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
    RSIP.REG_0070H.WORD = 0x008f0001U;
    RSIP.REG_004CH.WORD = 0x00000000U;
    RSIP.REG_0014H.WORD = 0x000001c7U;
    RSIP.REG_009CH.WORD = 0x800200c0U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = InData_Cmd[0];
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = InData_WrappedKeyType[0];
    RSIP.REG_0024H.WORD = 0x00000000U;
    RSIP.REG_0094H.WORD = 0x00003407U;
    RSIP.REG_0094H.WORD = 0x3420a8c0U;
    RSIP.REG_0094H.WORD = 0x00000002U;
    RSIP.REG_009CH.WORD = 0x00000080U;
    RSIP.REG_0040H.WORD = 0x00A60000U;
    RSIP.REG_0094H.WORD = 0x3420a800U;
    RSIP.REG_0094H.WORD = 0x00000003U;
    RSIP.REG_009CH.WORD = 0x00000080U;
    RSIP.REG_0040H.WORD = 0x00A60000U;
    RSIP.REG_0094H.WORD = 0x38008800U;
    RSIP.REG_0094H.WORD = 0x00000001U;
    RSIP.REG_009CH.WORD = 0x00000080U;
    RSIP.REG_0040H.WORD = 0x00260000U;
    r_rsip_func100(0x0ab8e613U, 0x14132937U, 0xf807ec55U, 0x3d971297U);
    RSIP.REG_0040H.WORD = 0x00400000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    if (1U == (RSIP.REG_0040H.BIT.B22))
    {
        r_rsip_func102(0x67f9974eU, 0x869787d5U, 0xe1f39251U, 0x5ee8465dU);
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
        RSIP.REG_0094H.WORD = 0x00003486U;
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
        RSIP.REG_002CH.WORD = change_endian_long(0x00008f01U);
        RSIP.REG_0024H.WORD = 0x00000000U;
        r_rsip_func101(0xd5fbb95fU, 0x93eb3bd7U, 0x0fedc866U, 0xf6294be6U);
        r_rsip_func043();
        RSIP.REG_0094H.WORD = 0x38000c84U;
        RSIP.REG_0094H.WORD = 0x1000b4c0U;
        RSIP.REG_0094H.WORD = 0x00000005U;
        RSIP.REG_0094H.WORD = 0x2000b4c0U;
        RSIP.REG_0094H.WORD = 0x00000007U;
        RSIP.REG_0014H.WORD = 0x000000a7U;
        RSIP.REG_009CH.WORD = 0x800100e0U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00008f01U);
        RSIP.REG_0024H.WORD = 0x00000000U;
        r_rsip_func101(0x88b1c3dcU, 0x2c9be808U, 0xf1c4024aU, 0xfc9128caU);
        r_rsip_func044();
        RSIP.REG_0094H.WORD = 0x38000c84U;
        RSIP.REG_009CH.WORD = 0x00000080U;
        RSIP.REG_0040H.WORD = 0x00260000U;
        r_rsip_func100(0xa524f703U, 0x82ebf7cbU, 0x1c01e66cU, 0x489b23deU);
        RSIP.REG_0040H.WORD = 0x00400000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (1U == (RSIP.REG_0040H.BIT.B22))
        {
            RSIP.REG_0014H.WORD = 0x000003c1U;
            RSIP.REG_00D4H.WORD = 0x40000000U;
            RSIP.REG_00D0H.WORD = 0xf7009d05U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = InData_KeyIndex[1];
            RSIP.REG_002CH.WORD = InData_KeyIndex[2];
            RSIP.REG_002CH.WORD = InData_KeyIndex[3];
            RSIP.REG_002CH.WORD = InData_KeyIndex[4];
            RSIP.REG_00B0H.WORD = 0x00000401U;
            RSIP.REG_0000H.WORD = 0x00c10009U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            RSIP.REG_00B0H.WORD = 0x00000402U;
            RSIP.REG_0000H.WORD = 0x00c10009U;
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
            RSIP.REG_002CH.WORD = InData_KeyIndex[5];
            RSIP.REG_002CH.WORD = InData_KeyIndex[6];
            RSIP.REG_002CH.WORD = InData_KeyIndex[7];
            RSIP.REG_002CH.WORD = InData_KeyIndex[8];
            r_rsip_func101(0xc2cf82b4U, 0xb00ebb8bU, 0x1c454b85U, 0x0b18ef93U);
        }
        else
        {
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
            r_rsip_func101(0xe8546c1cU, 0x7d22d95eU, 0xdb23610dU, 0xd816fe8dU);
        }
        RSIP.REG_00D0H.WORD = 0x9c100005U;
        RSIP.REG_0000H.WORD = 0x00410011U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        r_rsip_func100(0x1ebc2c69U, 0x50598e1bU, 0x01d07ea5U, 0x1a28187eU);
        RSIP.REG_0040H.WORD = 0x00400000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (1U == (RSIP.REG_0040H.BIT.B22))
        {
            r_rsip_func102(0xcf7be50dU, 0x11c04312U, 0x3b895c38U, 0xe49c7e05U);
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
            r_rsip_func030();
            RSIP.REG_0094H.WORD = 0x00003427U;
            RSIP.REG_0094H.WORD = 0x0000b460U;
            RSIP.REG_0094H.WORD = 0x00000010U;
            RSIP.REG_0094H.WORD = 0x00003ca3U;
            RSIP.REG_0094H.WORD = 0x00003466U;
            RSIP.REG_0014H.WORD = 0x000000c7U;
            RSIP.REG_009CH.WORD = 0x800100c0U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = InData_WrappedKeyIndex[0];
            RSIP.REG_0024H.WORD = 0x00000000U;
            RSIP.REG_0014H.WORD = 0x000000a7U;
            RSIP.REG_009CH.WORD = 0x800100e0U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = change_endian_long(0x00008f02U);
            RSIP.REG_0024H.WORD = 0x00000000U;
            r_rsip_func101(0x9b2c352fU, 0xafa796c7U, 0x27849e9cU, 0x102fe940U);
            r_rsip_func043();
            RSIP.REG_0094H.WORD = 0x000034c3U;
            RSIP.REG_0014H.WORD = 0x000000a7U;
            RSIP.REG_009CH.WORD = 0x800100e0U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = change_endian_long(0x00008f02U);
            RSIP.REG_0024H.WORD = 0x00000000U;
            r_rsip_func101(0x5d14308bU, 0x9503116aU, 0xd63deaf8U, 0xcc598af8U);
            r_rsip_func044();
            RSIP.REG_0094H.WORD = 0x000034e1U;
            RSIP.REG_0094H.WORD = 0x0000b460U;
            RSIP.REG_0094H.WORD = 0x00000010U;
            RSIP.REG_0094H.WORD = 0x000038a3U;
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
            RSIP.REG_0094H.WORD = 0x00000863U;
            RSIP.REG_0094H.WORD = 0x0000b420U;
            RSIP.REG_0094H.WORD = 0x00000010U;
            RSIP.REG_0094H.WORD = 0x00000842U;
            RSIP.REG_0094H.WORD = 0x0000b400U;
            RSIP.REG_0094H.WORD = 0x00000004U;
            RSIP.REG_0094H.WORD = 0x0000a8e0U;
            RSIP.REG_0094H.WORD = 0x00000005U;
            for (iLoop = 0; iLoop < KEY_INDEX_SIZE - 5; iLoop = iLoop + 4)
            {
                RSIP.REG_0014H.WORD = 0x000003c1U;
                RSIP.REG_00D4H.WORD = 0x40000000U;
                RSIP.REG_00D0H.WORD = 0xf7008d05U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_0014H.BIT.B31)
                {
                    /* waiting */
                }
                RSIP.REG_002CH.WORD = InData_WrappedKeyIndex[iLoop + 1];
                RSIP.REG_002CH.WORD = InData_WrappedKeyIndex[iLoop + 2];
                RSIP.REG_002CH.WORD = InData_WrappedKeyIndex[iLoop + 3];
                RSIP.REG_002CH.WORD = InData_WrappedKeyIndex[iLoop + 4];
                RSIP.REG_009CH.WORD = 0x80840003U;
                RSIP.REG_0000H.WORD = 0x03410011U;
                /* WAIT_LOOP */
                while (0U != RSIP.REG_0004H.BIT.B30)
                {
                    /* waiting */
                }
                RSIP.REG_0040H.WORD = 0x00001800U;
                RSIP.REG_0094H.WORD = 0x00002461U;
                RSIP.REG_0094H.WORD = 0x00002440U;
                r_rsip_func101(0x945158cbU, 0xccf3d8e7U, 0xc6a78342U, 0x6d930705U);
            }
            RSIP.REG_0094H.WORD = 0x38000847U;
            RSIP.REG_009CH.WORD = 0x00000080U;
            RSIP.REG_0040H.WORD = 0x00260000U;
            RSIP.REG_0040H.WORD = 0x00402000U;
            RSIP.REG_0024H.WORD = 0x00000000U;
            RSIP.REG_0014H.WORD = 0x000003c1U;
            RSIP.REG_00D4H.WORD = 0x40000000U;
            RSIP.REG_00D0H.WORD = 0x07008d05U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = InData_WrappedKeyIndex[iLoop + 1];
            RSIP.REG_002CH.WORD = InData_WrappedKeyIndex[iLoop + 2];
            RSIP.REG_002CH.WORD = InData_WrappedKeyIndex[iLoop + 3];
            RSIP.REG_002CH.WORD = InData_WrappedKeyIndex[iLoop + 4];
            RSIP.REG_00D0H.WORD = 0x9c100005U;
            RSIP.REG_0000H.WORD = 0x00410011U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            r_rsip_func100(0x1c5ba8e7U, 0x1ef22841U, 0x1de894acU, 0xa78a5845U);
            RSIP.REG_0040H.WORD = 0x00400000U;
            RSIP.REG_0024H.WORD = 0x00000000U;
            if (1U == (RSIP.REG_0040H.BIT.B22))
            {
                r_rsip_func102(0xd57033dbU, 0x957fc02fU, 0xef0eda9fU, 0x090533b7U);
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
                RSIP.REG_0094H.WORD = 0x38000c84U;
                RSIP.REG_009CH.WORD = 0x00000080U;
                RSIP.REG_0040H.WORD = 0x00260000U;
                r_rsip_func100(0x1d81d2d6U, 0x8c99eeffU, 0xd6222753U, 0x6a1b8177U);
                RSIP.REG_0040H.WORD = 0x00400000U;
                RSIP.REG_0024H.WORD = 0x00000000U;
                if (1U == (RSIP.REG_0040H.BIT.B22))
                {
                    r_rsip_func100(0x8515dd84U, 0xa97b4355U, 0xf94a0e17U, 0xb9b2437aU);
                    RSIP.REG_00D0H.WORD = 0x08000085U;
                    RSIP.REG_00B0H.WORD = 0x00000801U;
                    RSIP.REG_0000H.WORD = 0x00430009U;
                    /* WAIT_LOOP */
                    while (0U != RSIP.REG_0004H.BIT.B30)
                    {
                        /* waiting */
                    }
                    RSIP.REG_0040H.WORD = 0x00001800U;
                    RSIP.REG_00B0H.WORD = 0x00000802U;
                    RSIP.REG_0000H.WORD = 0x00430009U;
                    /* WAIT_LOOP */
                    while (0U != RSIP.REG_0004H.BIT.B30)
                    {
                        /* waiting */
                    }
                    RSIP.REG_0040H.WORD = 0x00001800U;
                    RSIP.REG_00D4H.WORD = 0x00000000U;
                    r_rsip_func101(0x3b108a6cU, 0xc367dc32U, 0xb89e1f8cU, 0x2b8aa5fdU);
                }
                else
                {
                    r_rsip_func100(0x68b6d8f7U, 0xe5173f79U, 0xda046887U, 0xb39aceb9U);
                    RSIP.REG_00D0H.WORD = 0x08000085U;
                    RSIP.REG_00B0H.WORD = 0x00001804U;
                    RSIP.REG_0000H.WORD = 0x00430011U;
                    /* WAIT_LOOP */
                    while (0U != RSIP.REG_0004H.BIT.B30)
                    {
                        /* waiting */
                    }
                    RSIP.REG_0040H.WORD = 0x00001800U;
                    r_rsip_func100(0x01f95b16U, 0x6fdd7d18U, 0x85c12678U, 0x99ba84f3U);
                    RSIP.REG_00D0H.WORD = 0x08000095U;
                    RSIP.REG_0000H.WORD = 0x00430011U;
                    /* WAIT_LOOP */
                    while (0U != RSIP.REG_0004H.BIT.B30)
                    {
                        /* waiting */
                    }
                    RSIP.REG_0040H.WORD = 0x00001800U;
                    RSIP.REG_00D4H.WORD = 0x40000000U;
                    r_rsip_func101(0x3461edaaU, 0x695f0f4cU, 0x94863225U, 0xb38ddd4eU);
                }
                RSIP.REG_0094H.WORD = 0x0000b400U;
                RSIP.REG_0094H.WORD = 0xa6a6a6a6U;
                RSIP.REG_0094H.WORD = 0x00003420U;
                RSIP.REG_0094H.WORD = 0x0000b480U;
                RSIP.REG_0094H.WORD = 0x00000001U;
                RSIP.REG_0094H.WORD = 0x000008c6U;
                RSIP.REG_0094H.WORD = 0x00036ca5U;
                for (jLoop = 0; jLoop <= 5; jLoop = jLoop + 1)
                {
                    RSIP.REG_0094H.WORD = 0x000008e7U;
                    for (iLoop = 1; iLoop <= (WRAPPED_KEY_SIZE - 2) / 2; iLoop = iLoop + 1)
                    {
                        RSIP.REG_00D0H.WORD = 0x0a008105U;
                        RSIP.REG_009CH.WORD = 0x81020000U;
                        RSIP.REG_0000H.WORD = 0x00490009U;
                        /* WAIT_LOOP */
                        while (0U != RSIP.REG_0004H.BIT.B30)
                        {
                            /* waiting */
                        }
                        RSIP.REG_0040H.WORD = 0x00001800U;
                        RSIP.REG_009CH.WORD = 0x81820007U;
                        RSIP.REG_0000H.WORD = 0x00490009U;
                        /* WAIT_LOOP */
                        while (0U != RSIP.REG_0004H.BIT.B30)
                        {
                            /* waiting */
                        }
                        RSIP.REG_0040H.WORD = 0x00001800U;
                        RSIP.REG_009CH.WORD = 0x80040000U;
                        RSIP.REG_0000H.WORD = 0x03410011U;
                        /* WAIT_LOOP */
                        while (0U != RSIP.REG_0004H.BIT.B30)
                        {
                            /* waiting */
                        }
                        RSIP.REG_0040H.WORD = 0x00001800U;
                        RSIP.REG_0094H.WORD = 0x00000824U;
                        RSIP.REG_0094H.WORD = 0x00003c47U;
                        RSIP.REG_0094H.WORD = 0x0000a4e0U;
                        RSIP.REG_0094H.WORD = 0x00000004U;
                        RSIP.REG_0094H.WORD = 0x00003c67U;
                        RSIP.REG_0094H.WORD = 0x0000a4e0U;
                        RSIP.REG_0094H.WORD = 0x00000004U;
                        RSIP.REG_0094H.WORD = 0x00002c80U;
                        r_rsip_func101(0xf48fba6dU, 0x83a7965aU, 0x6834a7e3U, 0xb3babba9U);
                    }
                    RSIP.REG_0094H.WORD = 0x380008e5U;
                    RSIP.REG_009CH.WORD = 0x00000080U;
                    RSIP.REG_0040H.WORD = 0x00260000U;
                    RSIP.REG_0040H.WORD = 0x00402000U;
                    RSIP.REG_0024H.WORD = 0x00000000U;
                    RSIP.REG_0094H.WORD = 0x00002cc0U;
                    r_rsip_func101(0x975474c4U, 0xb0b53da6U, 0xe94eadaaU, 0x80166f51U);
                }
                RSIP.REG_0094H.WORD = 0x380088c0U;
                RSIP.REG_0094H.WORD = 0x00000006U;
                RSIP.REG_009CH.WORD = 0x00000080U;
                RSIP.REG_0040H.WORD = 0x00260000U;
                RSIP.REG_0040H.WORD = 0x00402000U;
                RSIP.REG_0024H.WORD = 0x00000000U;
                r_rsip_func100(0x34fd5452U, 0x5fff9552U, 0xa7ee922aU, 0xd14ac723U);
                RSIP.REG_009CH.WORD = 0x81020000U;
                RSIP.REG_0008H.WORD = 0x0000500aU;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_0008H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_Text[0] = RSIP.REG_002CH.WORD;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_0008H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_Text[1] = RSIP.REG_002CH.WORD;
                RSIP.REG_0094H.WORD = 0x000008e7U;
                RSIP.REG_0094H.WORD = 0x00000863U;
                RSIP.REG_0094H.WORD = 0x000368a5U;
                for (iLoop = 2; iLoop < WRAPPED_KEY_SIZE; iLoop = iLoop + 2)
                {
                    RSIP.REG_0094H.WORD = 0x000008c6U;
                    RSIP.REG_0094H.WORD = 0x34202865U;
                    RSIP.REG_0094H.WORD = 0x2000d0c0U;
                    RSIP.REG_0094H.WORD = 0x00007c06U;
                    RSIP.REG_0040H.WORD = 0x00602000U;
                    RSIP.REG_0024H.WORD = 0x00000000U;
                    r_rsip_func100(0x4b27618eU, 0x5b6747ebU, 0x2e38e209U, 0x53f72620U);
                    RSIP.REG_009CH.WORD = 0x81820007U;
                    RSIP.REG_0008H.WORD = 0x0000500aU;
                    /* WAIT_LOOP */
                    while (1U != RSIP.REG_0008H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_Text[iLoop] = RSIP.REG_002CH.WORD;
                    /* WAIT_LOOP */
                    while (1U != RSIP.REG_0008H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_Text[iLoop + 1] = RSIP.REG_002CH.WORD;
                    RSIP.REG_0094H.WORD = 0x0000a4e0U;
                    RSIP.REG_0094H.WORD = 0x00000008U;
                    RSIP.REG_0094H.WORD = 0x00002c60U;
                    r_rsip_func101(0xf6c807cfU, 0x4be3671fU, 0x46e53277U, 0x65ea7cb4U);
                }
                RSIP.REG_0094H.WORD = 0x38000865U;
                RSIP.REG_009CH.WORD = 0x00000080U;
                RSIP.REG_0040H.WORD = 0x00260000U;
                RSIP.REG_0040H.WORD = 0x00402000U;
                RSIP.REG_0024H.WORD = 0x00000000U;
                r_rsip_func102(0xddb960f6U, 0xa749eb56U, 0xa6799cc4U, 0x8e614f5bU);
                RSIP.REG_006CH.WORD = 0x00000040U;
                /* WAIT_LOOP */
                while (0U != RSIP.REG_0020H.BIT.B12)
                {
                    /* waiting */
                }
                return RSIP_SUCCESS;
            }
        }
    }
}
