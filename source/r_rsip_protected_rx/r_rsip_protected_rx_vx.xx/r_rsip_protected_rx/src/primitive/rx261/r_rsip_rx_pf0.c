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

rsip_err_t r_rsip_pf0(const uint32_t InData_CurveType[], const uint32_t InData_KeyIndex[], const uint32_t InData_MsgDgst[], const uint32_t InData_DomainParam[], uint32_t OutData_Signature[])
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
    RSIP.REG_0070H.WORD = 0x00f00001U;
    RSIP.REG_004CH.WORD = 0x00000000U;
    RSIP.REG_0014H.WORD = 0x000000c7U;
    RSIP.REG_009CH.WORD = 0x80010000U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = InData_CurveType[0];
    RSIP.REG_0024H.WORD = 0x00000000U;
    RSIP.REG_0094H.WORD = 0x3420a800U;
    RSIP.REG_0094H.WORD = 0x00000003U;
    RSIP.REG_0094H.WORD = 0x2000b400U;
    RSIP.REG_0094H.WORD = 0x00000002U;
    r_rsip_func070(InData_DomainParam);
    r_rsip_func100(0xd54c7d25U, 0x49f922bdU, 0x103f9dc9U, 0x9aaf722eU);
    RSIP.REG_00B0H.WORD = 0x0000140fU;
    r_rsip_func103();
    r_rsip_func100(0xed562cf4U, 0x811a01e2U, 0x5f9b3c41U, 0xeb974316U);
    RSIP.REG_0014H.WORD = 0x000000a1U;
    RSIP.REG_00D0H.WORD = 0x0c200104U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
    RSIP.REG_0000H.WORD = 0x00c10011U;
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
    RSIP.REG_0000H.WORD = 0x00c10011U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_00B0H.WORD = 0x00001428U;
    RSIP.REG_0000H.WORD = 0x00c0001dU;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_0014H.WORD = 0x000000a5U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x00000001U);
    RSIP.REG_00B0H.WORD = 0x00001419U;
    RSIP.REG_0000H.WORD = 0x00c0001dU;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_0014H.WORD = 0x000000a5U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x00000002U);
    RSIP.REG_00B4H.WORD = 0x0019000aU;
    RSIP.REG_00B8H.WORD = 0x00140000U;
    RSIP.REG_00A4H.WORD = 0x0404000aU;
    RSIP.REG_00A0H.WORD = 0x20010001U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_00A8H.BIT.B0)
    {
        /* waiting */
    }
    RSIP.REG_00ACH.WORD = 0x00000001U;
    RSIP.REG_00A0H.WORD = 0x20010000U;
    RSIP.REG_00B8H.WORD = 0x00000014U;
    RSIP.REG_00A4H.WORD = 0x04040010U;
    RSIP.REG_00A0H.WORD = 0x20010001U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_00A8H.BIT.B0)
    {
        /* waiting */
    }
    RSIP.REG_00ACH.WORD = 0x00000001U;
    RSIP.REG_00B4H.WORD = 0x0028000fU;
    RSIP.REG_00B8H.WORD = 0x002d0014U;
    RSIP.REG_00A4H.WORD = 0x04040002U;
    RSIP.REG_00A0H.WORD = 0x20010001U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_00A8H.BIT.B0)
    {
        /* waiting */
    }
    RSIP.REG_00ACH.WORD = 0x00000001U;
    RSIP.REG_00B4H.WORD = 0x0028002dU;
    RSIP.REG_00B8H.WORD = 0x00320000U;
    RSIP.REG_00A4H.WORD = 0x04040009U;
    RSIP.REG_00A0H.WORD = 0x20010001U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_00A8H.BIT.B0)
    {
        /* waiting */
    }
    RSIP.REG_00ACH.WORD = 0x00000001U;
    r_rsip_func071(InData_DomainParam);
    RSIP.REG_0094H.WORD = 0x00003420U;
    RSIP.REG_0014H.WORD = 0x000000a7U;
    RSIP.REG_009CH.WORD = 0x800100e0U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x000000f0U);
    RSIP.REG_0024H.WORD = 0x00000000U;
    r_rsip_func101(0xf698d71cU, 0x3b841503U, 0xa9bee387U, 0xf7d685a9U);
    r_rsip_func088();
    r_rsip_func100(0xda0a5cc9U, 0xdd80e215U, 0x7aa0eaf6U, 0xbe49b01eU);
    RSIP.REG_0040H.WORD = 0x00400000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    if (1U == (RSIP.REG_0040H.BIT.B22))
    {
        r_rsip_func102(0x6573e50fU, 0x99f08178U, 0x11c04025U, 0x438aaec4U);
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
        RSIP.REG_00B8H.WORD = 0x0000000aU;
        RSIP.REG_00A4H.WORD = 0x04040010U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_00B4H.WORD = 0x005a0028U;
        RSIP.REG_00B8H.WORD = 0x002d000aU;
        RSIP.REG_00A4H.WORD = 0x04040002U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_0014H.WORD = 0x000000a5U;
        RSIP.REG_00B0H.WORD = 0x0000140fU;
        RSIP.REG_0000H.WORD = 0x00c0001dU;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000001U);
        RSIP.REG_00B4H.WORD = 0x000f002dU;
        RSIP.REG_00B8H.WORD = 0x0028000aU;
        RSIP.REG_00A4H.WORD = 0x04040002U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_00B4H.WORD = 0x000f0028U;
        RSIP.REG_00B8H.WORD = 0x00140000U;
        RSIP.REG_00A4H.WORD = 0x0404000aU;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_0040H.WORD = 0x00210000U;
        r_rsip_func100(0x6ffc2212U, 0x120533fcU, 0x631bf3a0U, 0xfe8ef521U);
        RSIP.REG_0040H.WORD = 0x00400000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (1U == (RSIP.REG_0040H.BIT.B22))
        {
            r_rsip_func102(0xe05111cbU, 0x5d59fdb1U, 0x3cb4f0d1U, 0x1e23413dU);
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
            r_rsip_func100(0xd6ea8af8U, 0x251f4af8U, 0x5f03435eU, 0x15da1ee8U);
            RSIP.REG_00B0H.WORD = 0x00001414U;
            RSIP.REG_0000H.WORD = 0x00c0001dU;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            RSIP.REG_0014H.WORD = 0x000000a5U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = change_endian_long(0x00000002U);
            RSIP.REG_00B4H.WORD = 0x0014000aU;
            RSIP.REG_00B8H.WORD = 0x000f0000U;
            RSIP.REG_00A4H.WORD = 0x0404000aU;
            RSIP.REG_00A0H.WORD = 0x20010001U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_00A8H.BIT.B0)
            {
                /* waiting */
            }
            RSIP.REG_00ACH.WORD = 0x00000001U;
            RSIP.REG_00B4H.WORD = 0x000f0032U;
            RSIP.REG_00B8H.WORD = 0x0014000aU;
            RSIP.REG_00A4H.WORD = 0x04040000U;
            RSIP.REG_0008H.WORD = 0x00020000U;
            RSIP.REG_00A0H.WORD = 0x20010001U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_00A8H.BIT.B0)
            {
                /* waiting */
            }
            RSIP.REG_00ACH.WORD = 0x00000001U;
            RSIP.REG_0040H.WORD = 0x00000d00U;
            RSIP.REG_00B4H.WORD = 0x005a0014U;
            RSIP.REG_00B8H.WORD = 0x000f000aU;
            RSIP.REG_00A4H.WORD = 0x04040002U;
            RSIP.REG_00A0H.WORD = 0x20010001U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_00A8H.BIT.B0)
            {
                /* waiting */
            }
            RSIP.REG_00ACH.WORD = 0x00000001U;
            RSIP.REG_0014H.WORD = 0x000007c5U;
            RSIP.REG_00B0H.WORD = 0x00001414U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = InData_MsgDgst[0];
            RSIP.REG_002CH.WORD = InData_MsgDgst[1];
            RSIP.REG_002CH.WORD = InData_MsgDgst[2];
            RSIP.REG_002CH.WORD = InData_MsgDgst[3];
            RSIP.REG_002CH.WORD = InData_MsgDgst[4];
            RSIP.REG_002CH.WORD = InData_MsgDgst[5];
            RSIP.REG_002CH.WORD = InData_MsgDgst[6];
            RSIP.REG_002CH.WORD = InData_MsgDgst[7];
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
            RSIP.REG_002CH.WORD = change_endian_long(0x000000f0U);
            RSIP.REG_0024H.WORD = 0x00000000U;
            r_rsip_func101(0x85ebdadcU, 0xb8d1fcb6U, 0x05c89c65U, 0x9e01adedU);
            r_rsip_func043();
            r_rsip_func074();
            RSIP.REG_0014H.WORD = 0x000000a7U;
            RSIP.REG_009CH.WORD = 0x800100e0U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0014H.BIT.B31)
            {
                /* waiting */
            }
            RSIP.REG_002CH.WORD = change_endian_long(0x000000f0U);
            RSIP.REG_0024H.WORD = 0x00000000U;
            r_rsip_func101(0x635739fcU, 0xea9bc791U, 0xe33c1936U, 0x7913fec5U);
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
            RSIP.REG_00B0H.WORD = 0x0000141eU;
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
            r_rsip_func100(0x27cfad37U, 0xfca1381dU, 0x871d04fcU, 0x5e35f806U);
            RSIP.REG_0040H.WORD = 0x00400000U;
            RSIP.REG_0024H.WORD = 0x00000000U;
            if (1U == (RSIP.REG_0040H.BIT.B22))
            {
                r_rsip_func102(0xbc24e908U, 0x0f70bb52U, 0x631fafccU, 0x73658d5cU);
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
                RSIP.REG_00B4H.WORD = 0x005a001eU;
                RSIP.REG_00B8H.WORD = 0x0019000aU;
                RSIP.REG_00A4H.WORD = 0x04040002U;
                RSIP.REG_00A0H.WORD = 0x20010001U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_00A8H.BIT.B0)
                {
                    /* waiting */
                }
                RSIP.REG_00ACH.WORD = 0x00000001U;
                RSIP.REG_00B4H.WORD = 0x00280019U;
                RSIP.REG_00B8H.WORD = 0x001e000aU;
                RSIP.REG_00A4H.WORD = 0x04040002U;
                RSIP.REG_00A0H.WORD = 0x20010001U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_00A8H.BIT.B0)
                {
                    /* waiting */
                }
                RSIP.REG_00ACH.WORD = 0x00000001U;
                RSIP.REG_00B4H.WORD = 0x001e0014U;
                RSIP.REG_00B8H.WORD = 0x0019000aU;
                RSIP.REG_00A4H.WORD = 0x04040005U;
                RSIP.REG_00A0H.WORD = 0x20010001U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_00A8H.BIT.B0)
                {
                    /* waiting */
                }
                RSIP.REG_00ACH.WORD = 0x00000001U;
                RSIP.REG_00B4H.WORD = 0x0019000fU;
                RSIP.REG_00B8H.WORD = 0x001e000aU;
                RSIP.REG_00A4H.WORD = 0x04040002U;
                RSIP.REG_00A0H.WORD = 0x20010001U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_00A8H.BIT.B0)
                {
                    /* waiting */
                }
                RSIP.REG_00ACH.WORD = 0x00000001U;
                RSIP.REG_00B0H.WORD = 0x0000140fU;
                RSIP.REG_0000H.WORD = 0x00c0001dU;
                /* WAIT_LOOP */
                while (0U != RSIP.REG_0004H.BIT.B30)
                {
                    /* waiting */
                }
                RSIP.REG_0040H.WORD = 0x00001800U;
                RSIP.REG_0014H.WORD = 0x000000a5U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_0014H.BIT.B31)
                {
                    /* waiting */
                }
                RSIP.REG_002CH.WORD = change_endian_long(0x00000001U);
                RSIP.REG_00B4H.WORD = 0x000f001eU;
                RSIP.REG_00B8H.WORD = 0x00140000U;
                RSIP.REG_00A4H.WORD = 0x0404000aU;
                RSIP.REG_00A0H.WORD = 0x20010001U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_00A8H.BIT.B0)
                {
                    /* waiting */
                }
                RSIP.REG_00ACH.WORD = 0x00000001U;
                RSIP.REG_0040H.WORD = 0x00210000U;
                r_rsip_func100(0x3a4e10e1U, 0xffbfb278U, 0xfd0a7e5bU, 0x68478703U);
                RSIP.REG_0040H.WORD = 0x00400000U;
                RSIP.REG_0024H.WORD = 0x00000000U;
                if (1U == (RSIP.REG_0040H.BIT.B22))
                {
                    r_rsip_func102(0xcab598f5U, 0xf015e6f4U, 0xfb43ca20U, 0xb2d0ecc9U);
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
                    r_rsip_func100(0xcd00a4fdU, 0x44bd347dU, 0xd8906151U, 0x05476f25U);
                    RSIP.REG_00B0H.WORD = 0x00001828U;
                    RSIP.REG_0008H.WORD = 0x00006022U;
                    /* WAIT_LOOP */
                    while (1U != RSIP.REG_0008H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_Signature[0] = RSIP.REG_002CH.WORD;
                    OutData_Signature[1] = RSIP.REG_002CH.WORD;
                    OutData_Signature[2] = RSIP.REG_002CH.WORD;
                    OutData_Signature[3] = RSIP.REG_002CH.WORD;
                    OutData_Signature[4] = RSIP.REG_002CH.WORD;
                    OutData_Signature[5] = RSIP.REG_002CH.WORD;
                    OutData_Signature[6] = RSIP.REG_002CH.WORD;
                    OutData_Signature[7] = RSIP.REG_002CH.WORD;
                    r_rsip_func100(0x25f9d547U, 0x4f8a0d2fU, 0x22f4f4e2U, 0x22a744dcU);
                    RSIP.REG_00B0H.WORD = 0x0000181eU;
                    RSIP.REG_0008H.WORD = 0x00006022U;
                    /* WAIT_LOOP */
                    while (1U != RSIP.REG_0008H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_Signature[8] = RSIP.REG_002CH.WORD;
                    OutData_Signature[9] = RSIP.REG_002CH.WORD;
                    OutData_Signature[10] = RSIP.REG_002CH.WORD;
                    OutData_Signature[11] = RSIP.REG_002CH.WORD;
                    OutData_Signature[12] = RSIP.REG_002CH.WORD;
                    OutData_Signature[13] = RSIP.REG_002CH.WORD;
                    OutData_Signature[14] = RSIP.REG_002CH.WORD;
                    OutData_Signature[15] = RSIP.REG_002CH.WORD;
                    r_rsip_func102(0x4048dcd7U, 0x579d7e75U, 0x15f52448U, 0x14fdd03cU);
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
}
