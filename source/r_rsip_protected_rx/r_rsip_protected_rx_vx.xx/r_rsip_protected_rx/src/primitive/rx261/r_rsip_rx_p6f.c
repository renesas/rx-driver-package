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

rsip_err_t r_rsip_p6f_r1(uint32_t InData_LC[], const uint32_t InData_Cmd[], const uint32_t InData_IV[], const uint32_t InData_InstData[], uint32_t INST_DATA_SIZE, uint32_t OutData_KeyIndex[])
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
    RSIP.REG_0070H.WORD = 0x006f0001U;
    RSIP.REG_004CH.WORD = 0x00000000U;
    r_rsip_func048(InData_LC);
    RSIP.REG_00A0H.WORD = 0x00010000U;
    RSIP.REG_0094H.WORD = 0x0000b4c0U;
    RSIP.REG_0094H.WORD = 0x0000001cU;
    RSIP.REG_00B0H.WORD = 0x00000892U;
    RSIP.REG_009CH.WORD = 0x80810006U;
    RSIP.REG_0000H.WORD = 0x00030005U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_0000H.WORD = 0x03430005U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    r_rsip_func049(InData_Cmd);
    RSIP.REG_0094H.WORD = 0x00000842U;
    RSIP.REG_0094H.WORD = 0x3420a800U;
    RSIP.REG_0094H.WORD = 0x00000009U;
    RSIP.REG_0094H.WORD = 0x1000b440U;
    RSIP.REG_0094H.WORD = 0x3CC003A0U;
    RSIP.REG_0094H.WORD = 0x000034a4U;
    RSIP.REG_0094H.WORD = 0x38008880U;
    RSIP.REG_0094H.WORD = 0x00000023U;
    RSIP.REG_0094H.WORD = 0x1000b480U;
    RSIP.REG_0094H.WORD = 0x00000005U;
    RSIP.REG_0094H.WORD = 0x38008880U;
    RSIP.REG_0094H.WORD = 0x00000022U;
    RSIP.REG_0094H.WORD = 0x1000b480U;
    RSIP.REG_0094H.WORD = 0x00000005U;
    r_rsip_func056();
    r_rsip_func100(0x44e76c0dU, 0x4dcb638dU, 0xa25155c0U, 0xce3dcbabU);
    RSIP.REG_0040H.WORD = 0x00400000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    if (1U == (RSIP.REG_0040H.BIT.B22))
    {
        r_rsip_func102(0x60b5b1a9U, 0x939acca0U, 0x73158356U, 0xacaa7c18U);
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
        RSIP.REG_0094H.WORD = 0x00003485U;
        RSIP.REG_0014H.WORD = 0x000001a1U;
        RSIP.REG_00D0H.WORD = 0x07330c04U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        RSIP.REG_00D0H.WORD = 0x07330d04U;
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
        RSIP.REG_0014H.WORD = 0x000001a1U;
        RSIP.REG_00D0H.WORD = 0x07330d04U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        RSIP.REG_00D0H.WORD = 0x06330104U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        RSIP.REG_00B0H.WORD = 0x00001404U;
        RSIP.REG_0000H.WORD = 0x00c10021U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_0014H.WORD = 0x000000c7U;
        RSIP.REG_009CH.WORD = 0x800100c0U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = S_INST2[0];
        RSIP.REG_0024H.WORD = 0x00000000U;
        RSIP.REG_0014H.WORD = 0x000000a7U;
        RSIP.REG_009CH.WORD = 0x800100e0U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x0000006fU);
        RSIP.REG_0024H.WORD = 0x00000000U;
        r_rsip_func101(0x76a89f68U, 0x0327087eU, 0x77569f8dU, 0x315a7882U);
        r_rsip_func043();
        RSIP.REG_0094H.WORD = 0x0000b4c0U;
        RSIP.REG_0094H.WORD = 0x000000ffU;
        RSIP.REG_0014H.WORD = 0x000000a7U;
        RSIP.REG_009CH.WORD = 0x800100e0U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x0000006fU);
        RSIP.REG_0024H.WORD = 0x00000000U;
        r_rsip_func101(0xe7864dd7U, 0xf6a40cd3U, 0x216760c1U, 0xe0e3046aU);
        r_rsip_func044();
        r_rsip_func100(0xe7fa3f24U, 0xa29e37f8U, 0x52808e52U, 0x84a6466bU);
        RSIP.REG_0014H.WORD = 0x000007c1U;
        RSIP.REG_00D4H.WORD = 0x40000100U;
        RSIP.REG_00D0H.WORD = 0xf7009d07U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = S_INST2[1];
        RSIP.REG_002CH.WORD = S_INST2[2];
        RSIP.REG_002CH.WORD = S_INST2[3];
        RSIP.REG_002CH.WORD = S_INST2[4];
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = S_INST2[5];
        RSIP.REG_002CH.WORD = S_INST2[6];
        RSIP.REG_002CH.WORD = S_INST2[7];
        RSIP.REG_002CH.WORD = S_INST2[8];
        RSIP.REG_00D0H.WORD = 0x080000a5U;
        RSIP.REG_0000H.WORD = 0x00410011U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        r_rsip_func100(0x923ad285U, 0x74ee3535U, 0x95d23a87U, 0x9f23e2c3U);
        RSIP.REG_00D0H.WORD = 0x080000b5U;
        RSIP.REG_0000H.WORD = 0x00410011U;
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
        RSIP.REG_002CH.WORD = S_INST2[9];
        RSIP.REG_002CH.WORD = S_INST2[10];
        RSIP.REG_002CH.WORD = S_INST2[11];
        RSIP.REG_002CH.WORD = S_INST2[12];
        RSIP.REG_00D0H.WORD = 0x8c100005U;
        RSIP.REG_0000H.WORD = 0x00410011U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_0014H.WORD = 0x000000a7U;
        RSIP.REG_009CH.WORD = 0x800100e0U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x0000006fU);
        RSIP.REG_0024H.WORD = 0x00000000U;
        r_rsip_func101(0x069bbf84U, 0x5884b388U, 0x0902a026U, 0xf47080d2U);
        r_rsip_func057_r1(InData_IV, InData_InstData, OutData_KeyIndex, INST_DATA_SIZE);
        r_rsip_func100(0x37f3d8c9U, 0x82aa7f4aU, 0x22f365f8U, 0x0bb0f2f4U);
        RSIP.REG_00B0H.WORD = 0x00001804U;
        RSIP.REG_00D0H.WORD = 0x080000b5U;
        RSIP.REG_0000H.WORD = 0x00430011U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_00D0H.WORD = 0x08000075U;
        RSIP.REG_0000H.WORD = 0x00430011U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        r_rsip_func100(0x0dc79058U, 0x214341a5U, 0x796e33c7U, 0x1987500cU);
        RSIP.REG_0040H.WORD = 0x00400000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (1U == (RSIP.REG_0040H.BIT.B22))
        {
            r_rsip_func102(0xf74c2cceU, 0xe469c971U, 0xb21cd41fU, 0x15fa7bddU);
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
            r_rsip_func100(0x2b6abbcaU, 0xabc002bcU, 0x512f8709U, 0xe656864fU);
            RSIP.REG_009CH.WORD = 0x81010000U;
            RSIP.REG_0008H.WORD = 0x00005006U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0008H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex[0] = RSIP.REG_002CH.WORD;
            r_rsip_func102(0xdd6e34e7U, 0x48fd378eU, 0x642fe628U, 0xfcd968caU);
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
