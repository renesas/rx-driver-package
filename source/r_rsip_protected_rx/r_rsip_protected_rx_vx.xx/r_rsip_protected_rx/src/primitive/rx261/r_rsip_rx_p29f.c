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

rsip_err_t r_rsip_p29f(const uint32_t InData_Text[], const uint32_t InData_TextLen[], const uint32_t InData_DataALen[], uint32_t OutData_Text[], uint32_t OutData_DataT[])
{
    int32_t iLoop = 0U, jLoop = 0U, kLoop = 0U, oLoop = 0U;
    uint32_t OFS_ADR = 0U;
    (void)iLoop;
    (void)jLoop;
    (void)kLoop;
    (void)oLoop;
    (void)OFS_ADR;
    RSIP.REG_0014H.WORD = 0x000001c7U;
    RSIP.REG_009CH.WORD = 0x80020040U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = InData_TextLen[0];
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = InData_TextLen[1];
    RSIP.REG_0024H.WORD = 0x00000000U;
    RSIP.REG_0094H.WORD = 0x0000b400U;
    RSIP.REG_0094H.WORD = 0x0000007FU;
    RSIP.REG_0094H.WORD = 0x0000b420U;
    RSIP.REG_0094H.WORD = 0xFFFFFF00U;
    RSIP.REG_0094H.WORD = 0x0c002823U;
    RSIP.REG_0094H.WORD = 0x04a02802U;
    RSIP.REG_009CH.WORD = 0x00000080U;
    RSIP.REG_0040H.WORD = 0x00260000U;
    r_rsip_func100(0x9b4a27c6U, 0xe58da5e8U, 0x005153fcU, 0x5a97a47eU);
    RSIP.REG_0040H.WORD = 0x00400000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    if (1U == (RSIP.REG_0040H.BIT.B22))
    {
        r_rsip_func102(0xc304ef06U, 0xdea003b9U, 0xec63dab7U, 0x4743e371U);
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
        RSIP.REG_0094H.WORD = 0x00003403U;
        RSIP.REG_0094H.WORD = 0x00036800U;
        RSIP.REG_0094H.WORD = 0x08008c00U;
        RSIP.REG_0094H.WORD = 0x0000000fU;
        RSIP.REG_009CH.WORD = 0x00000080U;
        RSIP.REG_0040H.WORD = 0x00A60000U;
        r_rsip_func100(0x9e5e15e4U, 0xb7063286U, 0x75037e7eU, 0x1e00b8dfU);
        RSIP.REG_0040H.WORD = 0x00400000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (1U == (RSIP.REG_0040H.BIT.B22))
        {
            r_rsip_func100(0xd66c72caU, 0xeac09030U, 0x65e4d587U, 0xc5da65dcU);
            RSIP.REG_0014H.WORD = 0x000003c1U;
            RSIP.REG_00D4H.WORD = 0x00000020U;
            RSIP.REG_00D0H.WORD = 0x07008d05U;
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
            RSIP.REG_0094H.WORD = 0x00000884U;
            for (iLoop = 0; iLoop < 16; iLoop = iLoop + 1)
            {
                RSIP.REG_0094H.WORD = 0x3c002820U;
                RSIP.REG_0094H.WORD = 0x12003c81U;
                RSIP.REG_0094H.WORD = 0x00002c20U;
            }
            RSIP.REG_00D4H.WORD = 0x00008000U;
            RSIP.REG_00D0H.WORD = 0x0e128455U;
            RSIP.REG_0094H.WORD = 0x00000821U;
            RSIP.REG_009CH.WORD = 0x81840001U;
            RSIP.REG_0000H.WORD = 0x00490011U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0004H.BIT.B30)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00001800U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_00C8H.BIT.B6)
            {
                /* waiting */
            }
            RSIP.REG_0040H.WORD = 0x00000400U;
            RSIP.REG_0040H.WORD = 0x00000500U;
            RSIP.REG_009CH.WORD = 0x81840001U;
            RSIP.REG_0008H.WORD = 0x00005012U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0008H.BIT.B30)
            {
                /* waiting */
            }
            OutData_Text[0] = RSIP.REG_002CH.WORD;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0008H.BIT.B30)
            {
                /* waiting */
            }
            OutData_Text[1] = RSIP.REG_002CH.WORD;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0008H.BIT.B30)
            {
                /* waiting */
            }
            OutData_Text[2] = RSIP.REG_002CH.WORD;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0008H.BIT.B30)
            {
                /* waiting */
            }
            OutData_Text[3] = RSIP.REG_002CH.WORD;
            r_rsip_func101(0x6e61ed49U, 0xca0b32cbU, 0x5c5661aaU, 0xf695e0cfU);
        }
        r_rsip_func100(0x9bca5bc7U, 0x99bc427fU, 0xb82d5a9bU, 0xfd30abcdU);
        RSIP.REG_0014H.WORD = 0x000001c1U;
        RSIP.REG_00D4H.WORD = 0x00008000U;
        RSIP.REG_00D0H.WORD = 0x0e128455U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = InData_DataALen[0];
        RSIP.REG_002CH.WORD = InData_DataALen[1];
        RSIP.REG_009CH.WORD = 0x81020040U;
        RSIP.REG_0000H.WORD = 0x00490009U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_00D0H.WORD = 0x08000045U;
        RSIP.REG_0094H.WORD = 0x0000b420U;
        RSIP.REG_0094H.WORD = 0x00000010U;
        RSIP.REG_009CH.WORD = 0x81840001U;
        RSIP.REG_0000H.WORD = 0x00490011U;
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
        RSIP.REG_00D4H.WORD = 0x00000020U;
        RSIP.REG_00D0H.WORD = 0x07008d05U;
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
        OutData_DataT[0] = RSIP.REG_002CH.WORD;
        OutData_DataT[1] = RSIP.REG_002CH.WORD;
        OutData_DataT[2] = RSIP.REG_002CH.WORD;
        OutData_DataT[3] = RSIP.REG_002CH.WORD;
        r_rsip_func102(0xc0cc191dU, 0x430c60d7U, 0x35243cc3U, 0xc9499f3cU);
        RSIP.REG_006CH.WORD = 0x00000040U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0020H.BIT.B12)
        {
            /* waiting */
        }
        return RSIP_SUCCESS;
    }
}
