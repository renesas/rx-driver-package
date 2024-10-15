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

rsip_err_t r_rsip_pdcf(const uint32_t InData_Cmd[], const uint32_t InData_MAC[], const uint32_t InData_length[], uint32_t OutData_MAC[])
{
    int32_t iLoop = 0U, jLoop = 0U, kLoop = 0U, oLoop = 0U;
    uint32_t OFS_ADR = 0U;
    (void)iLoop;
    (void)jLoop;
    (void)kLoop;
    (void)oLoop;
    (void)OFS_ADR;
    RSIP.REG_00F4H.WORD = 0x00000100U;
    RSIP.REG_00F4H.WORD = 0x00000020U;
    RSIP.REG_00B0H.WORD = 0x00001404U;
    RSIP.REG_0000H.WORD = 0x00c50021U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_00F4H.WORD = 0x00000010U;
    RSIP.REG_0014H.WORD = 0x000007a4U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0xc1059ed8U);
    RSIP.REG_002CH.WORD = change_endian_long(0x367cd507U);
    RSIP.REG_002CH.WORD = change_endian_long(0x3070dd17U);
    RSIP.REG_002CH.WORD = change_endian_long(0xf70e5939U);
    RSIP.REG_002CH.WORD = change_endian_long(0xffc00b31U);
    RSIP.REG_002CH.WORD = change_endian_long(0x68581511U);
    RSIP.REG_002CH.WORD = change_endian_long(0x64f98fa7U);
    RSIP.REG_002CH.WORD = change_endian_long(0xbefa4fa4U);
    RSIP.REG_00F4H.WORD = 0x00000011U;
    r_rsip_func002();
    RSIP.REG_00B0H.WORD = 0x00001804U;
    RSIP.REG_0000H.WORD = 0x0143001dU;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_0000H.WORD = 0x00030005U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_0014H.WORD = 0x000000a4U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x80000000U);
    RSIP.REG_0000H.WORD = 0x01400019U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_0014H.WORD = 0x000001a4U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
    RSIP.REG_002CH.WORD = change_endian_long(0x000002E0U);
    /* WAIT_LOOP */
    while (0U != RSIP.REG_00F8H.BIT.B2)
    {
        /* waiting */
    }
    RSIP.REG_00F4H.WORD = 0x00000100U;
    RSIP.REG_0040H.WORD = 0x00001600U;
    RSIP.REG_0014H.WORD = 0x000000c7U;
    RSIP.REG_009CH.WORD = 0x80010000U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = InData_Cmd[0];
    RSIP.REG_0024H.WORD = 0x00000000U;
    RSIP.REG_0094H.WORD = 0x38000c00U;
    RSIP.REG_009CH.WORD = 0x00000080U;
    RSIP.REG_0040H.WORD = 0x00260000U;
    r_rsip_func100(0xe7a5852bU, 0xb189ca9cU, 0x6c5c8183U, 0xa95c3dd2U);
    RSIP.REG_0040H.WORD = 0x00400000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    if (1U == (RSIP.REG_0040H.BIT.B22))
    {
        r_rsip_func100(0x82ed60f7U, 0x05d626ebU, 0x112dd549U, 0x40c0ecb3U);
        RSIP.REG_00F4H.WORD = 0x00000020U;
        RSIP.REG_0008H.WORD = 0x0000401eU;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0008H.BIT.B30)
        {
            /* waiting */
        }
        OutData_MAC[0] = RSIP.REG_002CH.WORD;
        OutData_MAC[1] = RSIP.REG_002CH.WORD;
        OutData_MAC[2] = RSIP.REG_002CH.WORD;
        OutData_MAC[3] = RSIP.REG_002CH.WORD;
        OutData_MAC[4] = RSIP.REG_002CH.WORD;
        OutData_MAC[5] = RSIP.REG_002CH.WORD;
        OutData_MAC[6] = RSIP.REG_002CH.WORD;
        RSIP.REG_0000H.WORD = 0x00050005U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        r_rsip_func102(0xf0f74e81U, 0x0af69c2bU, 0xbcc85f4dU, 0x58d59ae4U);
        RSIP.REG_006CH.WORD = 0x00000040U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0020H.BIT.B12)
        {
            /* waiting */
        }
        return RSIP_SUCCESS;
    }
    else
    {
        RSIP.REG_0014H.WORD = 0x000000c7U;
        RSIP.REG_009CH.WORD = 0x80010020U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = InData_length[0];
        RSIP.REG_0024H.WORD = 0x00000000U;
        RSIP.REG_0094H.WORD = 0x3420a820U;
        RSIP.REG_0094H.WORD = 0x00000004U;
        RSIP.REG_009CH.WORD = 0x00000080U;
        RSIP.REG_0040H.WORD = 0x00260000U;
        RSIP.REG_0094H.WORD = 0x3420a820U;
        RSIP.REG_0094H.WORD = 0x0000001dU;
        RSIP.REG_009CH.WORD = 0x00000080U;
        RSIP.REG_0040H.WORD = 0x00A60000U;
        r_rsip_func100(0x8e83bdcdU, 0x836dd51bU, 0xb9098e65U, 0x9a636c11U);
        RSIP.REG_0040H.WORD = 0x00400000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (1U == (RSIP.REG_0040H.BIT.B22))
        {
            r_rsip_func102(0x0507c3f5U, 0x579438c2U, 0x065567d6U, 0xde3ee273U);
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
            RSIP.REG_0094H.WORD = 0x00000842U;
            RSIP.REG_009CH.WORD = 0x80870002U;
            RSIP.REG_00F4H.WORD = 0x00000020U;
            RSIP.REG_0000H.WORD = 0x0345001dU;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            RSIP.REG_0000H.WORD = 0x00050005U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            RSIP.REG_0094H.WORD = 0x00000800U;
            for (iLoop = 0; iLoop < 32; iLoop = iLoop + 1)
            {
                RSIP.REG_0094H.WORD = 0x3c002841U;
                RSIP.REG_0094H.WORD = 0x12003c02U;
                RSIP.REG_0094H.WORD = 0x00002c40U;
            }
            RSIP.REG_00D0H.WORD = 0x08000045U;
            RSIP.REG_0094H.WORD = 0x00000842U;
            RSIP.REG_009CH.WORD = 0x81880002U;
            RSIP.REG_0000H.WORD = 0x00490011U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            RSIP.REG_00D0H.WORD = 0x08000055U;
            RSIP.REG_0000H.WORD = 0x00490011U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            RSIP.REG_0014H.WORD = 0x000006c1U;
            RSIP.REG_00D0H.WORD = 0x9c000005U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = InData_MAC[0];
            RSIP.REG_002CH.WORD = InData_MAC[1];
            RSIP.REG_002CH.WORD = InData_MAC[2];
            RSIP.REG_002CH.WORD = InData_MAC[3];
            RSIP.REG_00D0H.WORD = 0x9c100005U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = InData_MAC[4];
            RSIP.REG_002CH.WORD = InData_MAC[5];
            RSIP.REG_002CH.WORD = InData_MAC[6];
            RSIP.REG_0014H.WORD = 0x000000a1U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
            r_rsip_func100(0x5c158977U, 0x7071248fU, 0x31cfe35cU, 0xdbfd69b7U);
            RSIP.REG_0040H.WORD = 0x00400000U;
            RSIP.REG_0024H.WORD = 0x00000000U;
            if (1U == (RSIP.REG_0040H.BIT.B22))
            {
                r_rsip_func102(0xcfe3223fU, 0xb62eef33U, 0x95a9fd63U, 0xbc0d89deU);
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
                r_rsip_func102(0xdcfc0787U, 0x8788740aU, 0x7e303098U, 0xf3857d05U);
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
