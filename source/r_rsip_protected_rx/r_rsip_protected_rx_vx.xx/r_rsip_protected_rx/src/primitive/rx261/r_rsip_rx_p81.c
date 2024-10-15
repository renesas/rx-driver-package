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
rsip_err_t r_rsip_p81(void)
{
    int32_t iLoop = 0U, jLoop = 0U, kLoop = 0U, oLoop = 0U;
    uint32_t OFS_ADR = 0U;
    (void)iLoop;
    (void)jLoop;
    (void)kLoop;
    (void)oLoop;
    (void)OFS_ADR;
    RSIP.REG_008CH.WORD = 0x00000001U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_008CH.BIT.B1)
    {
        /* waiting */
    }
    if (0x0009F7C3U != (RSIP.REG_0090H.WORD & 0xFFFFFFFFU))
    {
        return RSIP_ERR_FAIL;
    }
    else
    {
        RSIP.REG_000CH.WORD = 0x38c60eedU;
        RSIP.REG_0024H.WORD = 0x00000000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        RSIP.REG_0048H.WORD = 0x00000000U;
        RSIP.REG_0008H.WORD = 0x00000001U;
        RSIP.REG_0010H.WORD = 0x00001401U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (0x0U != (RSIP.REG_006CH.WORD & 0x17U))
        {
            return RSIP_ERR_RESOURCE_CONFLICT;
        }
        RSIP.REG_00C0H.WORD = 0x00000001U;
        RSIP.REG_00E0H.WORD = 0x00000001U;
        RSIP.REG_00F0H.WORD = 0x00000001U;
        RSIP.REG_0070H.WORD = 0x00818001U;
        RSIP.REG_0078H.WORD = 0x00000d00U;
        RSIP.REG_00C4H.WORD = 0x00008002U;
        RSIP.REG_0014H.WORD = 0x000003a1U;
        r_rsip_func101(change_endian_long(0x5270f37cU), change_endian_long(0xbf2be020U), change_endian_long(0x46e2055dU), change_endian_long(0xadd4e6b8U));
        RSIP.REG_00C4H.WORD = 0x00000000U;
        RSIP.REG_0014H.WORD = 0x000003a1U;
        RSIP.REG_00D0H.WORD = 0x0a0700f5U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = 0x17bb05e0U;
        RSIP.REG_002CH.WORD = 0xd5ce4a63U;
        RSIP.REG_002CH.WORD = 0x184fb60aU;
        RSIP.REG_002CH.WORD = 0x27996c70U;
        r_rsip_func101(change_endian_long(0x73480c17U), change_endian_long(0xcfcc7dffU), change_endian_long(0xc32e1111U), change_endian_long(0x00ba8bd2U));
        r_rsip_func100(change_endian_long(0x11121160U), change_endian_long(0xba2b9f28U), change_endian_long(0x552cc303U), change_endian_long(0x5e0aa2c8U));
        RSIP.REG_0008H.WORD = 0x00020000U;
        if (0U == (RSIP.REG_0020H.BIT.B13))
        {
            RSIP.REG_006CH.WORD = 0x00000020U;
            return RSIP_ERR_FAIL;
        }
        else
        {
            RSIP.REG_0038H.WORD = 0x000000F1U;
            RSIP.REG_0078H.WORD = 0x00000220U;
            r_rsip_func102(change_endian_long(0x7b0b8ac1U), change_endian_long(0x16fffecfU), change_endian_long(0xfc1af76fU), change_endian_long(0x70d1397aU));
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
RSIP_SEC_DEFAULT
