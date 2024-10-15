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

rsip_err_t r_rsip_p32f(const uint32_t InData_Text[], const uint32_t InData_DataT[], const uint32_t InData_DataALen[], const uint32_t InData_TextLen[], const uint32_t InData_DataTLen[], uint32_t OutData_Text[])
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
    RSIP.REG_0014H.WORD = 0x000000c7U;
    RSIP.REG_009CH.WORD = 0x800100a0U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = InData_DataTLen[0];
    RSIP.REG_0024H.WORD = 0x00000000U;
    RSIP.REG_0094H.WORD = 0x380088a0U;
    RSIP.REG_0094H.WORD = 0x00000000U;
    RSIP.REG_009CH.WORD = 0x00000080U;
    RSIP.REG_0040H.WORD = 0x00260000U;
    RSIP.REG_0094H.WORD = 0x0000b420U;
    RSIP.REG_0094H.WORD = 0x00000010U;
    RSIP.REG_0094H.WORD = 0x34202825U;
    RSIP.REG_009CH.WORD = 0x00000080U;
    RSIP.REG_0040H.WORD = 0x00260000U;
    r_rsip_func100(0x347b03d5U, 0x7d978dd5U, 0xb7012a6fU, 0xa72152dfU);
    RSIP.REG_0040H.WORD = 0x00400000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    if (1U == (RSIP.REG_0040H.BIT.B22))
    {
        r_rsip_func102(0x6a267795U, 0xd2f641eeU, 0xacba8c05U, 0x84d4efbcU);
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
        r_rsip_func100(0x96bfa80eU, 0x4845862aU, 0xe52cfdbcU, 0xcbb2a8ecU);
        RSIP.REG_0040H.WORD = 0x00400000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (1U == (RSIP.REG_0040H.BIT.B22))
        {
            r_rsip_func100(0x21bfa286U, 0x5ca0b8f6U, 0xeb2ac462U, 0x22f144f3U);
            RSIP.REG_0014H.WORD = 0x000003c1U;
            RSIP.REG_00D4H.WORD = 0x00008020U;
            RSIP.REG_00D0H.WORD = 0x20008d05U;
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
            RSIP.REG_0094H.WORD = 0x00000821U;
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
            r_rsip_func101(0x42c4b6afU, 0x3d2f4060U, 0xe4c5d9a5U, 0xb582b506U);
        }
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
            RSIP.REG_0094H.WORD = 0x3c002825U;
            RSIP.REG_0094H.WORD = 0x12003c81U;
            RSIP.REG_0094H.WORD = 0x00002c20U;
        }
        RSIP.REG_0014H.WORD = 0x000003c1U;
        RSIP.REG_00D0H.WORD = 0x08000055U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = InData_DataT[0];
        RSIP.REG_002CH.WORD = InData_DataT[1];
        RSIP.REG_002CH.WORD = InData_DataT[2];
        RSIP.REG_002CH.WORD = InData_DataT[3];
        RSIP.REG_00D0H.WORD = 0x9c100005U;
        RSIP.REG_0094H.WORD = 0x00000821U;
        RSIP.REG_009CH.WORD = 0x81840001U;
        RSIP.REG_0000H.WORD = 0x00490011U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        r_rsip_func100(0xb486454dU, 0x5f5bc3b7U, 0xf5bfe8f5U, 0x348e84b4U);
        RSIP.REG_0040H.WORD = 0x00400000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (1U == (RSIP.REG_0040H.BIT.B22))
        {
            r_rsip_func102(0x15fe43ceU, 0xd042295bU, 0xa4c4cfe4U, 0x7fdbe24fU);
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
            r_rsip_func102(0x4f786b2fU, 0xadbf8731U, 0xf9568d75U, 0xeff92dd4U);
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
