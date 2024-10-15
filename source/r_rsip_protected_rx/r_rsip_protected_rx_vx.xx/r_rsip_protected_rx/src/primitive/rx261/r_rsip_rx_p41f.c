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

rsip_err_t r_rsip_p41f(const uint32_t InData_Cmd[], const uint32_t InData_Text[], const uint32_t InData_DataT[], const uint32_t InData_DataTLen[], uint32_t OutData_DataT[])
{
    int32_t iLoop = 0U, jLoop = 0U, kLoop = 0U, oLoop = 0U;
    uint32_t OFS_ADR = 0U;
    (void)iLoop;
    (void)jLoop;
    (void)kLoop;
    (void)oLoop;
    (void)OFS_ADR;
    if ((InData_Cmd[0] == 0x00000000U) || (InData_Cmd[0] == 0x00000002U))
    {
        RSIP.REG_0014H.WORD = 0x000000a1U;
        RSIP.REG_00D0H.WORD = 0x4a000104U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        r_rsip_func101(0x254e1287U, 0x7de72bcbU, 0x4508266fU, 0x19e5cde6U);
    }
    else
    {
        RSIP.REG_0014H.WORD = 0x000000a1U;
        RSIP.REG_00D0H.WORD = 0x5a000104U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        r_rsip_func101(0xd72aa1f3U, 0x8b226e1dU, 0x59196f8fU, 0x34c63288U);
    }
    RSIP.REG_00D0H.WORD = 0x0c000045U;
    RSIP.REG_0000H.WORD = 0x00410011U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    if ((InData_Cmd[0] == 0x00000000U) || (InData_Cmd[0] == 0x00000001U))
    {
        r_rsip_func100(0x73bbcc2bU, 0x0f303517U, 0xb29c90ddU, 0x56bd2875U);
        RSIP.REG_0014H.WORD = 0x000003c1U;
        RSIP.REG_00D0H.WORD = 0x0e000505U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = InData_Text[0];
        RSIP.REG_002CH.WORD = InData_Text[1];
        RSIP.REG_002CH.WORD = InData_Text[2];
        RSIP.REG_002CH.WORD = InData_Text[3];
        RSIP.REG_0008H.WORD = 0x00001012U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0008H.BIT.B30)
        {
            /* waiting */
        }
        OutData_DataT[0] = RSIP.REG_002CH.WORD;
        OutData_DataT[1] = RSIP.REG_002CH.WORD;
        OutData_DataT[2] = RSIP.REG_002CH.WORD;
        OutData_DataT[3] = RSIP.REG_002CH.WORD;
        r_rsip_func102(0xea2085b9U, 0x24f5ae3cU, 0xab5f0afdU, 0x465d17e7U);
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
        RSIP.REG_009CH.WORD = 0x80010040U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = InData_DataTLen[0];
        RSIP.REG_0024H.WORD = 0x00000000U;
        RSIP.REG_0094H.WORD = 0x3420a840U;
        RSIP.REG_0094H.WORD = 0x00000010U;
        RSIP.REG_009CH.WORD = 0x00000080U;
        RSIP.REG_0040H.WORD = 0x00260000U;
        RSIP.REG_0094H.WORD = 0x0000b460U;
        RSIP.REG_0094H.WORD = 0x00000080U;
        RSIP.REG_0094H.WORD = 0x34202862U;
        RSIP.REG_009CH.WORD = 0x00000080U;
        RSIP.REG_0040H.WORD = 0x00260000U;
        r_rsip_func100(0x1b02e3caU, 0xa9559570U, 0xc3172986U, 0x0f58576eU);
        RSIP.REG_0040H.WORD = 0x00400000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (1U == (RSIP.REG_0040H.BIT.B22))
        {
            r_rsip_func102(0x832afc3aU, 0x10ca1daaU, 0xacd9ba64U, 0xb71399faU);
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
            RSIP.REG_0014H.WORD = 0x000003c1U;
            RSIP.REG_00D0H.WORD = 0x0e000505U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = InData_Text[0];
            RSIP.REG_002CH.WORD = InData_Text[1];
            RSIP.REG_002CH.WORD = InData_Text[2];
            RSIP.REG_002CH.WORD = InData_Text[3];
            RSIP.REG_0094H.WORD = 0x00000821U;
            RSIP.REG_009CH.WORD = 0x80840001U;
            RSIP.REG_0000H.WORD = 0x03410011U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            RSIP.REG_0094H.WORD = 0x000034e2U;
            RSIP.REG_0094H.WORD = 0x000568e7U;
            RSIP.REG_0094H.WORD = 0x00026ce7U;
            RSIP.REG_0094H.WORD = 0x00003827U;
            RSIP.REG_0094H.WORD = 0x0000b4c0U;
            RSIP.REG_0094H.WORD = 0x00000020U;
            RSIP.REG_0094H.WORD = 0x00003402U;
            RSIP.REG_0094H.WORD = 0x00008c00U;
            RSIP.REG_0094H.WORD = 0x0000001fU;
            RSIP.REG_0094H.WORD = 0x000028c0U;
            RSIP.REG_0094H.WORD = 0x00008cc0U;
            RSIP.REG_0094H.WORD = 0x0000001fU;
            RSIP.REG_0094H.WORD = 0x00004406U;
            RSIP.REG_0094H.WORD = 0x00007421U;
            RSIP.REG_0094H.WORD = 0x00007821U;
            RSIP.REG_0094H.WORD = 0x00003c27U;
            RSIP.REG_0094H.WORD = 0x000034c2U;
            RSIP.REG_0094H.WORD = 0x0000a4c0U;
            RSIP.REG_0094H.WORD = 0x0000001fU;
            RSIP.REG_0094H.WORD = 0x000568c6U;
            RSIP.REG_0094H.WORD = 0x000034e6U;
            RSIP.REG_0094H.WORD = 0x00026ce7U;
            RSIP.REG_0094H.WORD = 0x00000821U;
            for (iLoop = 0; iLoop < 4; iLoop = iLoop + 1)
            {
                RSIP.REG_0094H.WORD = 0x3420a8e0U;
                RSIP.REG_0094H.WORD = 0x0000000dU;
                RSIP.REG_0094H.WORD = 0x10003c27U;
                RSIP.REG_0094H.WORD = 0x1000a4e0U;
                RSIP.REG_0094H.WORD = 0x00000004U;
            }
            RSIP.REG_00D0H.WORD = 0x08000045U;
            RSIP.REG_009CH.WORD = 0x81840001U;
            RSIP.REG_0000H.WORD = 0x00490011U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            RSIP.REG_0014H.WORD = 0x000003c1U;
            RSIP.REG_00D0H.WORD = 0x9c000005U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = InData_DataT[0];
            RSIP.REG_002CH.WORD = InData_DataT[1];
            RSIP.REG_002CH.WORD = InData_DataT[2];
            RSIP.REG_002CH.WORD = InData_DataT[3];
            r_rsip_func100(0xda798ea1U, 0x75737e26U, 0xb69be184U, 0x4de65543U);
            RSIP.REG_0040H.WORD = 0x00400000U;
            RSIP.REG_0024H.WORD = 0x00000000U;
            if (1U == (RSIP.REG_0040H.BIT.B22))
            {
                r_rsip_func102(0x31cff823U, 0x4f108910U, 0x8e241385U, 0xa2fb0583U);
                RSIP.REG_006CH.WORD = 0x00000040U;
                /* WAIT_LOOP */
                while (0U != RSIP.REG_0020H.BIT.B12)
                {
                    /* waiting */
                }
                return RSIP_ERR_AUTHENTICATION;
            }
            else
            {
                r_rsip_func102(0xcbccad2eU, 0x9340c6bfU, 0x7262b00bU, 0x813dfe79U);
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
