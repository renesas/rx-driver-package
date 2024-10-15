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

rsip_err_t r_rsip_p76f(const uint32_t InData_Cmd[], const uint32_t InData_MAC[], const uint32_t InData_length[], uint32_t OutData_MAC[])
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
    RSIP.REG_002CH.WORD = change_endian_long(0x6a09e667U);
    RSIP.REG_002CH.WORD = change_endian_long(0xbb67ae85U);
    RSIP.REG_002CH.WORD = change_endian_long(0x3c6ef372U);
    RSIP.REG_002CH.WORD = change_endian_long(0xa54ff53aU);
    RSIP.REG_002CH.WORD = change_endian_long(0x510e527fU);
    RSIP.REG_002CH.WORD = change_endian_long(0x9b05688cU);
    RSIP.REG_002CH.WORD = change_endian_long(0x1f83d9abU);
    RSIP.REG_002CH.WORD = change_endian_long(0x5be0cd19U);
    RSIP.REG_00F4H.WORD = 0x00000011U;
    r_rsip_func002();
    RSIP.REG_00B0H.WORD = 0x00001804U;
    RSIP.REG_0000H.WORD = 0x01430021U;
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
    RSIP.REG_0000H.WORD = 0x01400015U;
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
    RSIP.REG_002CH.WORD = change_endian_long(0x00000300U);
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
    r_rsip_func100(0x96d0cfa2U, 0xa103d4e0U, 0x3e1ab66dU, 0x5f33ee61U);
    RSIP.REG_0040H.WORD = 0x00400000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    if (1U == (RSIP.REG_0040H.BIT.B22))
    {
        r_rsip_func100(0x01503775U, 0x0db33891U, 0xc1fcda9fU, 0xf71df4bdU);
        RSIP.REG_00F4H.WORD = 0x00000020U;
        RSIP.REG_0008H.WORD = 0x00004022U;
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
        OutData_MAC[7] = RSIP.REG_002CH.WORD;
        r_rsip_func102(0xa7ee937bU, 0x53fe03edU, 0xc5656d76U, 0x0d081682U);
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
        RSIP.REG_0094H.WORD = 0x00000021U;
        RSIP.REG_009CH.WORD = 0x00000080U;
        RSIP.REG_0040H.WORD = 0x00A60000U;
        r_rsip_func100(0xd91e8af3U, 0x34f638c5U, 0x23451567U, 0x38fddb26U);
        RSIP.REG_0040H.WORD = 0x00400000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (1U == (RSIP.REG_0040H.BIT.B22))
        {
            r_rsip_func102(0x4283c60fU, 0xa40a3c5cU, 0x3371425eU, 0x0613a9bdU);
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
            RSIP.REG_009CH.WORD = 0x80880002U;
            RSIP.REG_00F4H.WORD = 0x00000020U;
            RSIP.REG_0000H.WORD = 0x03450021U;
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
            RSIP.REG_0014H.WORD = 0x000007c1U;
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
            RSIP.REG_002CH.WORD = InData_MAC[7];
            r_rsip_func100(0x4de820b7U, 0x2ba2666bU, 0x538d5cbaU, 0xb6bf0c07U);
            RSIP.REG_0040H.WORD = 0x00400000U;
            RSIP.REG_0024H.WORD = 0x00000000U;
            if (1U == (RSIP.REG_0040H.BIT.B22))
            {
                r_rsip_func102(0xe8ee2f93U, 0xe954ea9bU, 0x8b08c4d7U, 0x1cd906caU);
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
                r_rsip_func102(0x3a22fe55U, 0x1ba058c8U, 0x3a4c5bbdU, 0x6f6c4526U);
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
