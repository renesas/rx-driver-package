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

rsip_err_t r_rsip_p90_r1(const uint32_t InData_Cmd[], const uint32_t InData_KeyIndex[], const uint32_t InData_WrappedKeyType[], const uint32_t InData_Text[], uint32_t KEY_INDEX_SIZE, uint32_t OutData_KeyIndex[], uint32_t WRAPPED_KEY_SIZE)
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
    RSIP.REG_0070H.WORD = 0x00900001U;
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
    r_rsip_func100(0xd58808a2U, 0x5747f04aU, 0x10731983U, 0x50763d38U);
    RSIP.REG_0040H.WORD = 0x00400000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    if (1U == (RSIP.REG_0040H.BIT.B22))
    {
        r_rsip_func102(0x5792506fU, 0x6ede2010U, 0xa4a79be8U, 0x1d7e2f05U);
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
        RSIP.REG_002CH.WORD = change_endian_long(0x00009001U);
        RSIP.REG_0024H.WORD = 0x00000000U;
        r_rsip_func101(0x2b110264U, 0xc1ff751fU, 0x8eb51cadU, 0x51d5cc3bU);
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
        RSIP.REG_002CH.WORD = change_endian_long(0x00009001U);
        RSIP.REG_0024H.WORD = 0x00000000U;
        r_rsip_func101(0x9f1b8f7eU, 0xe6f26780U, 0x6dfb04ddU, 0xcf49e659U);
        r_rsip_func044();
        RSIP.REG_0094H.WORD = 0x38000c84U;
        RSIP.REG_009CH.WORD = 0x00000080U;
        RSIP.REG_0040H.WORD = 0x00260000U;
        r_rsip_func100(0xe81fa99aU, 0x4000bb8bU, 0x9551bdbeU, 0xfcee3ee4U);
        RSIP.REG_0040H.WORD = 0x00400000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (1U == (RSIP.REG_0040H.BIT.B22))
        {
            r_rsip_func100(0x1b70853aU, 0xdbd980fbU, 0x89f2a844U, 0x88f1a293U);
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
            RSIP.REG_00D0H.WORD = 0x9c100005U;
            RSIP.REG_0000H.WORD = 0x00410011U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
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
            r_rsip_func101(0xb6698e60U, 0xc4366661U, 0x2a3cc6adU, 0xe950ee7fU);
        }
        else
        {
            r_rsip_func100(0x88ea23bdU, 0xfd0eb5e2U, 0xf2b16303U, 0x214f1793U);
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
            RSIP.REG_00D0H.WORD = 0x08000085U;
            RSIP.REG_00B0H.WORD = 0x00001804U;
            RSIP.REG_0000H.WORD = 0x00430011U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            r_rsip_func100(0xa823a031U, 0x299d139aU, 0x1dc8bbdaU, 0xc696f7d6U);
            RSIP.REG_00D0H.WORD = 0x08000095U;
            RSIP.REG_0000H.WORD = 0x00430011U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            RSIP.REG_00D4H.WORD = 0x40000000U;
            r_rsip_func101(0xb84bb4abU, 0xd10bd443U, 0x90ca1dc5U, 0x7dfc2a64U);
        }
        r_rsip_func100(0x1a4aec35U, 0x43be825fU, 0x8653c34bU, 0x03ba1e95U);
        RSIP.REG_0040H.WORD = 0x00400000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (1U == (RSIP.REG_0040H.BIT.B22))
        {
            r_rsip_func102(0xdd712eb7U, 0x97bd3082U, 0x037348aeU, 0x793efdf0U);
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
            RSIP.REG_0094H.WORD = 0x00002ca0U;
            RSIP.REG_00D0H.WORD = 0x08000044U;
            RSIP.REG_009CH.WORD = 0x810100c0U;
            RSIP.REG_0000H.WORD = 0x00490005U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            RSIP.REG_00D0H.WORD = 0x08000054U;
            RSIP.REG_009CH.WORD = 0x810100e0U;
            RSIP.REG_0000H.WORD = 0x00490005U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            RSIP.REG_0014H.WORD = 0x000001c7U;
            RSIP.REG_009CH.WORD = 0x80020000U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = InData_Text[0];
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = InData_Text[1];
            RSIP.REG_0024H.WORD = 0x00000000U;
            RSIP.REG_0094H.WORD = 0x00000884U;
            RSIP.REG_0094H.WORD = 0x00000863U;
            RSIP.REG_0094H.WORD = 0x00002c60U;
            for (iLoop = 2; iLoop < WRAPPED_KEY_SIZE; iLoop = iLoop + 2)
            {
                RSIP.REG_0014H.WORD = 0x000001c7U;
                RSIP.REG_009CH.WORD = 0x80820004U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_0014H.BIT.B31)
                {
                    /* waiting */
                }
                RSIP.REG_002CH.WORD = InData_Text[iLoop + 0];
                /* WAIT_LOOP */
                while (1U != RSIP.REG_0014H.BIT.B31)
                {
                    /* waiting */
                }
                RSIP.REG_002CH.WORD = InData_Text[iLoop + 1];
                RSIP.REG_0024H.WORD = 0x00000000U;
                RSIP.REG_0094H.WORD = 0x0000a480U;
                RSIP.REG_0094H.WORD = 0x00000008U;
                RSIP.REG_0094H.WORD = 0x00002c60U;
                r_rsip_func101(0x63acef25U, 0x92afb7e6U, 0x139cef65U, 0x5495a53aU);
            }
            RSIP.REG_0094H.WORD = 0x38000865U;
            RSIP.REG_009CH.WORD = 0x00000080U;
            RSIP.REG_0040H.WORD = 0x00260000U;
            RSIP.REG_0040H.WORD = 0x00402000U;
            RSIP.REG_0024H.WORD = 0x00000000U;
            RSIP.REG_0094H.WORD = 0x0000a880U;
            RSIP.REG_0094H.WORD = 0x00000008U;
            RSIP.REG_00D0H.WORD = 0x08000065U;
            RSIP.REG_009CH.WORD = 0x81020080U;
            RSIP.REG_0000H.WORD = 0x00490009U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            RSIP.REG_0000H.WORD = 0x00400009U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            RSIP.REG_0094H.WORD = 0x000034c5U;
            RSIP.REG_0094H.WORD = 0x000030c0U;
            RSIP.REG_0094H.WORD = 0x00003486U;
            RSIP.REG_0094H.WORD = 0x00026c84U;
            RSIP.REG_0094H.WORD = 0x00016cc6U;
            RSIP.REG_0094H.WORD = 0x00002486U;
            RSIP.REG_0094H.WORD = 0x000008a5U;
            for (jLoop = 5; jLoop >= 0; jLoop = jLoop - 1)
            {
                RSIP.REG_0014H.WORD = 0x000000a1U;
                RSIP.REG_00D0H.WORD = 0x0c200104U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_0014H.BIT.B31)
                {
                    /* waiting */
                }
                RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
                RSIP.REG_009CH.WORD = 0x800100e0U;
                RSIP.REG_0000H.WORD = 0x03410005U;
                /* WAIT_LOOP */
                while (0U != RSIP.REG_0004H.BIT.B30)
                {
                    /* waiting */
                }
                RSIP.REG_0040H.WORD = 0x00001800U;
                RSIP.REG_009CH.WORD = 0x800100c0U;
                RSIP.REG_0000H.WORD = 0x03410005U;
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
                RSIP.REG_0094H.WORD = 0x000030c0U;
                for (iLoop = (WRAPPED_KEY_SIZE / 2) - 1; iLoop >= 1; iLoop = iLoop - 1)
                {
                    RSIP.REG_0094H.WORD = 0x00000824U;
                    RSIP.REG_00D0H.WORD = 0x0a00810dU;
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
                    RSIP.REG_0094H.WORD = 0x00003c47U;
                    RSIP.REG_0094H.WORD = 0x0000a4e0U;
                    RSIP.REG_0094H.WORD = 0x00000004U;
                    RSIP.REG_0094H.WORD = 0x00003c67U;
                    RSIP.REG_0094H.WORD = 0x0000a8e0U;
                    RSIP.REG_0094H.WORD = 0x0000000cU;
                    RSIP.REG_0094H.WORD = 0x00003080U;
                    RSIP.REG_0094H.WORD = 0x000030c0U;
                    r_rsip_func101(0xb8d009d6U, 0xe4168bc6U, 0x469765b8U, 0x3e784ff7U);
                }
                RSIP.REG_0094H.WORD = 0x38000cc6U;
                RSIP.REG_009CH.WORD = 0x00000080U;
                RSIP.REG_0040H.WORD = 0x00260000U;
                RSIP.REG_0040H.WORD = 0x00402000U;
                RSIP.REG_0024H.WORD = 0x00000000U;
                RSIP.REG_0094H.WORD = 0x00002ca0U;
                r_rsip_func101(0x7af311caU, 0x0ed9eb5aU, 0x5ef2e555U, 0xde2ec602U);
            }
            RSIP.REG_0094H.WORD = 0x380088a0U;
            RSIP.REG_0094H.WORD = 0x00000006U;
            RSIP.REG_009CH.WORD = 0x00000080U;
            RSIP.REG_0040H.WORD = 0x00260000U;
            RSIP.REG_0040H.WORD = 0x00402000U;
            RSIP.REG_0024H.WORD = 0x00000000U;
            RSIP.REG_0094H.WORD = 0x38008800U;
            RSIP.REG_0094H.WORD = 0xa6a6a6a6U;
            RSIP.REG_009CH.WORD = 0x00000080U;
            RSIP.REG_0040H.WORD = 0x00A60000U;
            RSIP.REG_0094H.WORD = 0x38008820U;
            RSIP.REG_0094H.WORD = 0xa6a6a6a6U;
            RSIP.REG_009CH.WORD = 0x00000080U;
            RSIP.REG_0040H.WORD = 0x00A60000U;
            r_rsip_func100(0xac7373ceU, 0x944b5933U, 0xd72ee511U, 0xf9b2d9ffU);
            RSIP.REG_0040H.WORD = 0x00400000U;
            RSIP.REG_0024H.WORD = 0x00000000U;
            if (1U == (RSIP.REG_0040H.BIT.B22))
            {
                r_rsip_func102(0x52832c4cU, 0xe0344aa8U, 0x77596d97U, 0xaab2f46fU);
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
                r_rsip_func100(0x3b654dc6U, 0xf5f0ffebU, 0x99e331b0U, 0xa6f7e413U);
                RSIP.REG_0094H.WORD = 0x00000842U;
                RSIP.REG_009CH.WORD = 0x81880002U;
                RSIP.REG_00B0H.WORD = 0x00001404U;
                RSIP.REG_0000H.WORD = 0x00c90021U;
                /* WAIT_LOOP */
                while (0U != RSIP.REG_0004H.BIT.B30)
                {
                    /* waiting */
                }
                RSIP.REG_0040H.WORD = 0x00001800U;
                RSIP.REG_0014H.WORD = 0x000000a1U;
                RSIP.REG_00D0H.WORD = 0x0c100104U;
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
                RSIP.REG_0094H.WORD = 0x0000a820U;
                RSIP.REG_0094H.WORD = 0x00000005U;
                RSIP.REG_0014H.WORD = 0x000000a1U;
                RSIP.REG_00D0H.WORD = 0x0c000104U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_0014H.BIT.B31)
                {
                    /* waiting */
                }
                RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
                RSIP.REG_009CH.WORD = 0x80010080U;
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
                RSIP.REG_002CH.WORD = change_endian_long(0x00009002U);
                RSIP.REG_0024H.WORD = 0x00000000U;
                r_rsip_func101(0x9e4193adU, 0xca8cd07cU, 0x1fae62f7U, 0x7a920456U);
                r_rsip_func043();
                RSIP.REG_0094H.WORD = 0x000034c4U;
                RSIP.REG_0014H.WORD = 0x000000a7U;
                RSIP.REG_009CH.WORD = 0x800100e0U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_0014H.BIT.B31)
                {
                    /* waiting */
                }
                RSIP.REG_002CH.WORD = change_endian_long(0x00009002U);
                RSIP.REG_0024H.WORD = 0x00000000U;
                r_rsip_func101(0xd1f6cce8U, 0x0c694b73U, 0xd80b896eU, 0x56ebb9f1U);
                r_rsip_func044();
                RSIP.REG_0094H.WORD = 0x00000842U;
                RSIP.REG_00B0H.WORD = 0x00001804U;
                RSIP.REG_009CH.WORD = 0x80880002U;
                RSIP.REG_0000H.WORD = 0x03430021U;
                /* WAIT_LOOP */
                while (0U != RSIP.REG_0004H.BIT.B30)
                {
                    /* waiting */
                }
                RSIP.REG_0040H.WORD = 0x00001800U;
                RSIP.REG_0094H.WORD = 0x000008e7U;
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
                RSIP.REG_0094H.WORD = 0x00000884U;
                for (iLoop = 0; iLoop < KEY_INDEX_SIZE - 5; iLoop = iLoop + 4)
                {
                    RSIP.REG_0094H.WORD = 0x000008c6U;
                    RSIP.REG_0094H.WORD = 0x34202881U;
                    RSIP.REG_0094H.WORD = 0x2000d0c0U;
                    RSIP.REG_0094H.WORD = 0x00007c06U;
                    RSIP.REG_0040H.WORD = 0x00602000U;
                    RSIP.REG_0024H.WORD = 0x00000000U;
                    r_rsip_func100(0xcb532fbeU, 0xfd8a8a58U, 0xb2696512U, 0xf5310dfcU);
                    RSIP.REG_00D4H.WORD = 0x40000000U;
                    RSIP.REG_00D0H.WORD = 0xe7008d05U;
                    RSIP.REG_009CH.WORD = 0x81840007U;
                    RSIP.REG_0000H.WORD = 0x00490011U;
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
                    OutData_KeyIndex[iLoop + 1] = RSIP.REG_002CH.WORD;
                    OutData_KeyIndex[iLoop + 2] = RSIP.REG_002CH.WORD;
                    OutData_KeyIndex[iLoop + 3] = RSIP.REG_002CH.WORD;
                    OutData_KeyIndex[iLoop + 4] = RSIP.REG_002CH.WORD;
                    RSIP.REG_0094H.WORD = 0x0000a4e0U;
                    RSIP.REG_0094H.WORD = 0x00000010U;
                    RSIP.REG_0094H.WORD = 0x0000a480U;
                    RSIP.REG_0094H.WORD = 0x00000004U;
                    r_rsip_func101(0xdbf9dbcaU, 0xa8462e40U, 0x3522bbcdU, 0xaf4279f4U);
                }
                RSIP.REG_0094H.WORD = 0x38000881U;
                RSIP.REG_009CH.WORD = 0x00000080U;
                RSIP.REG_0040H.WORD = 0x00260000U;
                RSIP.REG_0040H.WORD = 0x00402000U;
                RSIP.REG_0024H.WORD = 0x00000000U;
                r_rsip_func100(0x536d226cU, 0x50ac8dfbU, 0x7a65b504U, 0x52ac92b2U);
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
                OutData_KeyIndex[iLoop + 1] = RSIP.REG_002CH.WORD;
                OutData_KeyIndex[iLoop + 2] = RSIP.REG_002CH.WORD;
                OutData_KeyIndex[iLoop + 3] = RSIP.REG_002CH.WORD;
                OutData_KeyIndex[iLoop + 4] = RSIP.REG_002CH.WORD;
                r_rsip_func100(0x7babeee6U, 0xbb5a4046U, 0xdac7a8c3U, 0x396d5f1fU);
                RSIP.REG_009CH.WORD = 0x81010000U;
                RSIP.REG_0008H.WORD = 0x00005006U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_0008H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_KeyIndex[0] = RSIP.REG_002CH.WORD;
                r_rsip_func102(0x77e0c42fU, 0xbb52a08cU, 0x33896697U, 0xc2b5f1a8U);
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
