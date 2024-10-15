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

rsip_err_t r_rsip_p30(uint32_t InData_LC[], uint32_t OutData_KeyIndex[])
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
    RSIP.REG_0070H.WORD = 0x00300001U;
    RSIP.REG_004CH.WORD = 0x00000000U;
    r_rsip_func048(InData_LC);
    RSIP.REG_0094H.WORD = 0x38008800U;
    RSIP.REG_0094H.WORD = 0x00000000U;
    RSIP.REG_009CH.WORD = 0x00000080U;
    RSIP.REG_0040H.WORD = 0x00A60000U;
    r_rsip_func100(0xa8db3db9U, 0x2224f85fU, 0xa0358a74U, 0xa272908cU);
    RSIP.REG_0040H.WORD = 0x00400000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    if (1U == (RSIP.REG_0040H.BIT.B22))
    {
        r_rsip_func102(0xc3824262U, 0x8c06b2e7U, 0x5ccd6655U, 0x365c0d71U);
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
        RSIP.REG_0040H.WORD = 0x00b00000U;
        r_rsip_func100(0x832afa3bU, 0xf3e7b265U, 0x588ac816U, 0xd4929265U);
        RSIP.REG_0040H.WORD = 0x00400000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (1U == (RSIP.REG_0040H.BIT.B22))
        {
            r_rsip_func102(0x7042061dU, 0x4b7c7d8eU, 0x62fd0097U, 0xb961fd19U);
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
            RSIP.REG_0014H.WORD = 0x000000a7U;
            RSIP.REG_009CH.WORD = 0x800100e0U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = change_endian_long(0x00000030U);
            RSIP.REG_0024H.WORD = 0x00000000U;
            r_rsip_func101(0x9557e2a9U, 0x09f7a315U, 0x4159af2fU, 0x76957242U);
            r_rsip_func040();
            r_rsip_func100(0x8d4a9bf4U, 0x757a6a82U, 0x8bc889aeU, 0x292e5f98U);
            r_rsip_func103();
            r_rsip_func100(0xba4c11e4U, 0xbba4d596U, 0xaa99e7b6U, 0xdf4dda7fU);
            RSIP.REG_0014H.WORD = 0x000000a1U;
            RSIP.REG_00D0H.WORD = 0x0c200104U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
            RSIP.REG_009CH.WORD = 0x80080000U;
            RSIP.REG_0000H.WORD = 0x03410011U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            r_rsip_func103();
            r_rsip_func100(0x201c2209U, 0x9776e2a7U, 0xbc16b213U, 0xc13691a1U);
            RSIP.REG_0014H.WORD = 0x000000a1U;
            RSIP.REG_00D0H.WORD = 0x0c200104U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
            RSIP.REG_0000H.WORD = 0x03410011U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            RSIP.REG_00D4H.WORD = 0x40000100U;
            RSIP.REG_00D0H.WORD = 0xe7009d07U;
            RSIP.REG_009CH.WORD = 0x81080000U;
            RSIP.REG_0000H.WORD = 0x00490021U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            RSIP.REG_00D4H.WORD = 0x40000000U;
            RSIP.REG_0014H.WORD = 0x000003a1U;
            RSIP.REG_00D0H.WORD = 0x09108105U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
            RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
            RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
            RSIP.REG_002CH.WORD = change_endian_long(0x00000002U);
            RSIP.REG_0008H.WORD = 0x00001032U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0008H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex[0] = RSIP.REG_002CH.WORD;
            OutData_KeyIndex[1] = RSIP.REG_002CH.WORD;
            OutData_KeyIndex[2] = RSIP.REG_002CH.WORD;
            OutData_KeyIndex[3] = RSIP.REG_002CH.WORD;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0008H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex[4] = RSIP.REG_002CH.WORD;
            OutData_KeyIndex[5] = RSIP.REG_002CH.WORD;
            OutData_KeyIndex[6] = RSIP.REG_002CH.WORD;
            OutData_KeyIndex[7] = RSIP.REG_002CH.WORD;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0008H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex[8] = RSIP.REG_002CH.WORD;
            OutData_KeyIndex[9] = RSIP.REG_002CH.WORD;
            OutData_KeyIndex[10] = RSIP.REG_002CH.WORD;
            OutData_KeyIndex[11] = RSIP.REG_002CH.WORD;
            RSIP.REG_006CH.WORD = 0x00000020U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0020H.BIT.B12)
            {
                /* waiting */
            }
            return RSIP_SUCCESS;
        }
    }
}
