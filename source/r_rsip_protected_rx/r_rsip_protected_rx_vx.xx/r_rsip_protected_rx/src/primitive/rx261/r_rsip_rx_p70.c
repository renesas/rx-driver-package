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

rsip_err_t r_rsip_p70_r1(uint32_t InData_LC[], const uint32_t InData_Cmd[], const uint32_t InData_KeyIndex[], uint32_t KEY_INDEX_SIZE)
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
    RSIP.REG_0070H.WORD = 0x00700001U;
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
    RSIP.REG_0094H.WORD = 0x3CC003BFU;
    RSIP.REG_0094H.WORD = 0x38008800U;
    RSIP.REG_0094H.WORD = 0x00000006U;
    RSIP.REG_0094H.WORD = 0x1000b440U;
    RSIP.REG_0094H.WORD = 0x00000000U;
    RSIP.REG_0094H.WORD = 0x000034a4U;
    RSIP.REG_0094H.WORD = 0x38008880U;
    RSIP.REG_0094H.WORD = 0x000000ffU;
    RSIP.REG_0094H.WORD = 0x1000b480U;
    RSIP.REG_0094H.WORD = 0x00000005U;
    RSIP.REG_0094H.WORD = 0x38008880U;
    RSIP.REG_0094H.WORD = 0x00000023U;
    RSIP.REG_0094H.WORD = 0x1000b480U;
    RSIP.REG_0094H.WORD = 0x00000005U;
    RSIP.REG_0094H.WORD = 0x38008880U;
    RSIP.REG_0094H.WORD = 0x00000022U;
    RSIP.REG_0094H.WORD = 0x1000b480U;
    RSIP.REG_0094H.WORD = 0x00000005U;
    r_rsip_func056();
    r_rsip_func100(0x47389989U, 0x170146f4U, 0xd9ff7111U, 0x73c417c6U);
    RSIP.REG_0040H.WORD = 0x00400000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    if (1U == (RSIP.REG_0040H.BIT.B22))
    {
        r_rsip_func102(0x5ef294b5U, 0xcabbf507U, 0x025de376U, 0x5fcc238cU);
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
        RSIP.REG_002CH.WORD = change_endian_long(0x00000070U);
        RSIP.REG_0024H.WORD = 0x00000000U;
        r_rsip_func101(0xe9d9912bU, 0x36e14895U, 0x00fb5f82U, 0x5d7fcb33U);
        r_rsip_func043();
        RSIP.REG_0094H.WORD = 0x000034c4U;
        RSIP.REG_0014H.WORD = 0x000000a7U;
        RSIP.REG_009CH.WORD = 0x800100e0U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000070U);
        RSIP.REG_0024H.WORD = 0x00000000U;
        r_rsip_func101(0x9478e99eU, 0x919872dfU, 0xbd531b16U, 0xe621ceb6U);
        r_rsip_func044();
        r_rsip_func100(0xb62f9c69U, 0xec3bedd9U, 0xae67eafaU, 0x73137bfeU);
        RSIP.REG_0014H.WORD = 0x000000a1U;
        RSIP.REG_00D0H.WORD = 0x08000044U;
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
        if (0x00000000U == (RSIP.REG_0044H.WORD & 0xffffffffU))
        {
            RSIP.REG_0014H.WORD = 0x00020061U;
            RSIP.REG_00D4H.WORD = 0x40000000U;
            RSIP.REG_00D0H.WORD = 0xf7009c06U;
            for (iLoop = 0; iLoop < (KEY_INDEX_SIZE - 5) ; iLoop = iLoop + 4)
            {
                /* WAIT_LOOP */
                while (1U != RSIP.REG_0014H.BIT.B31)
                {
                    /* waiting */
                }
                RSIP.REG_002CH.WORD = InData_KeyIndex[iLoop + 1];
                RSIP.REG_002CH.WORD = InData_KeyIndex[iLoop + 2];
                RSIP.REG_002CH.WORD = InData_KeyIndex[iLoop + 3];
                RSIP.REG_002CH.WORD = InData_KeyIndex[iLoop + 4];
            }
            r_rsip_func205();
            r_rsip_func101(0xe768dbd8U, 0x65f2ae47U, 0x33f55832U, 0xcb5331e5U);
        }
        else if (0x00000016U == (RSIP.REG_0044H.WORD & 0xffffffffU))
        {
            RSIP.REG_0014H.WORD = 0x00020061U;
            RSIP.REG_00D4H.WORD = 0x40000000U;
            RSIP.REG_00D0H.WORD = 0xe8009006U;
            for (iLoop = 0; iLoop < (KEY_INDEX_SIZE - 5) ; iLoop = iLoop + 4)
            {
                /* WAIT_LOOP */
                while (1U != RSIP.REG_0014H.BIT.B31)
                {
                    /* waiting */
                }
                RSIP.REG_002CH.WORD = InData_KeyIndex[iLoop + 1];
                RSIP.REG_002CH.WORD = InData_KeyIndex[iLoop + 2];
                RSIP.REG_002CH.WORD = InData_KeyIndex[iLoop + 3];
                RSIP.REG_002CH.WORD = InData_KeyIndex[iLoop + 4];
            }
            r_rsip_func205();
            r_rsip_func101(0xe175519aU, 0xfb93067eU, 0x56d6201bU, 0xf5b7c3a1U);
        }
        RSIP.REG_0014H.WORD = 0x000003c1U;
        RSIP.REG_00D4H.WORD = 0x40000000U;
        RSIP.REG_00D0H.WORD = 0x07008d05U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = InData_KeyIndex[iLoop + 1];
        RSIP.REG_002CH.WORD = InData_KeyIndex[iLoop + 2];
        RSIP.REG_002CH.WORD = InData_KeyIndex[iLoop + 3];
        RSIP.REG_002CH.WORD = InData_KeyIndex[iLoop + 4];
        RSIP.REG_00D0H.WORD = 0x9c100005U;
        RSIP.REG_0000H.WORD = 0x00410011U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        r_rsip_func100(0x46e1eeb8U, 0xcd12c979U, 0x3c6b1036U, 0xd78ec079U);
        RSIP.REG_0040H.WORD = 0x00400000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (1U == (RSIP.REG_0040H.BIT.B22))
        {
            r_rsip_func102(0x0ceb6a93U, 0x94c755a6U, 0x296caaa8U, 0x8c6cf0e2U);
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
            r_rsip_func102(0x4712add4U, 0x01ea4bd1U, 0x7b838056U, 0xe806a7dfU);
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
