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

rsip_err_t r_rsip_p44f(const uint32_t InData_Cmd[], const uint32_t InData_Text[], const uint32_t InData_DataT[], const uint32_t InData_DataTLen[], uint32_t OutData_DataT[])
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
        RSIP.REG_00D4H.WORD = 0x40000000U;
        RSIP.REG_00D0H.WORD = 0x4a008104U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        r_rsip_func101(0xd7274411U, 0xe61bf65dU, 0xf246c74aU, 0x752ea90cU);
    }
    else
    {
        RSIP.REG_0014H.WORD = 0x000000a1U;
        RSIP.REG_00D4H.WORD = 0x40000000U;
        RSIP.REG_00D0H.WORD = 0x5a008104U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        r_rsip_func101(0xfb05ffacU, 0x4957c37cU, 0x90737539U, 0x54a7ac36U);
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
        r_rsip_func100(0xde6b629aU, 0x0a78e741U, 0xda448098U, 0x2ce31265U);
        RSIP.REG_0014H.WORD = 0x000003c1U;
        RSIP.REG_00D4H.WORD = 0x40000000U;
        RSIP.REG_00D0H.WORD = 0x0e008505U;
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
        r_rsip_func102(0x56c82f54U, 0x4845b64dU, 0x7114cc71U, 0x5a330b14U);
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
        r_rsip_func100(0x0952f879U, 0x5acad953U, 0x50bc9075U, 0xf108ca66U);
        RSIP.REG_0040H.WORD = 0x00400000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (1U == (RSIP.REG_0040H.BIT.B22))
        {
            r_rsip_func102(0x55a20fffU, 0x53da0e29U, 0xfc80fce2U, 0x47cb75e4U);
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
            RSIP.REG_00D4H.WORD = 0x40000000U;
            RSIP.REG_00D0H.WORD = 0x0e008505U;
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
            r_rsip_func100(0x75494b09U, 0xceb83151U, 0xbcfd56a8U, 0x13d4d261U);
            RSIP.REG_0040H.WORD = 0x00400000U;
            RSIP.REG_0024H.WORD = 0x00000000U;
            if (1U == (RSIP.REG_0040H.BIT.B22))
            {
                r_rsip_func102(0xce758207U, 0xb1117162U, 0xc170b859U, 0xe093da41U);
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
                r_rsip_func102(0xd20993d0U, 0x6a715befU, 0x78152676U, 0x54797cf5U);
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
