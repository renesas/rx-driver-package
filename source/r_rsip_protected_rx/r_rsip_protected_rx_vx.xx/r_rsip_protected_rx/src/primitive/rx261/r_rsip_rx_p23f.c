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
rsip_err_t r_rsip_p23f(const uint32_t InData_UpProgram[], const uint32_t InData_UpMAC[], uint32_t MAX_CNT, uint32_t OutData_Program[], uint32_t OutData_MAC[])
{
    int32_t iLoop = 0U, jLoop = 0U, kLoop = 0U, oLoop = 0U;
    uint32_t OFS_ADR = 0U;
    (void)iLoop;
    (void)jLoop;
    (void)kLoop;
    (void)oLoop;
    (void)OFS_ADR;
    for (iLoop = 0; iLoop < MAX_CNT - 4; iLoop = iLoop + 4)
    {
        r_rsip_func100(0xc5905e76U, 0x2ef46928U, 0xf3e28899U, 0x0021f6b8U);
        RSIP.REG_0014H.WORD = 0x000003c1U;
        RSIP.REG_00D0H.WORD = 0xd922090dU;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = InData_UpProgram[iLoop + 0];
        RSIP.REG_002CH.WORD = InData_UpProgram[iLoop + 1];
        RSIP.REG_002CH.WORD = InData_UpProgram[iLoop + 2];
        RSIP.REG_002CH.WORD = InData_UpProgram[iLoop + 3];
        RSIP.REG_00D4H.WORD = 0x40000000U;
        RSIP.REG_00D0H.WORD = 0xe8008105U;
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
        OutData_Program[iLoop] = RSIP.REG_002CH.WORD;
        OutData_Program[iLoop + 1] = RSIP.REG_002CH.WORD;
        OutData_Program[iLoop + 2] = RSIP.REG_002CH.WORD;
        OutData_Program[iLoop + 3] = RSIP.REG_002CH.WORD;
        r_rsip_func101(0x6cfbd000U, 0x063767b6U, 0x84c83a1cU, 0x447ebe78U);
    }
    r_rsip_func100(0x5886d5b6U, 0x09fbd814U, 0x802a2ee8U, 0x936acc12U);
    RSIP.REG_0014H.WORD = 0x000003c1U;
    RSIP.REG_00D0H.WORD = 0xd922090dU;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = InData_UpProgram[iLoop + 0];
    RSIP.REG_002CH.WORD = InData_UpProgram[iLoop + 1];
    RSIP.REG_002CH.WORD = InData_UpProgram[iLoop + 2];
    RSIP.REG_002CH.WORD = InData_UpProgram[iLoop + 3];
    RSIP.REG_009CH.WORD = 0x80040000U;
    RSIP.REG_0000H.WORD = 0x03410011U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_009CH.WORD = 0x81040000U;
    RSIP.REG_0008H.WORD = 0x00005012U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0008H.BIT.B30)
    {
        /* waiting */
    }
    OutData_Program[iLoop] = RSIP.REG_002CH.WORD;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0008H.BIT.B30)
    {
        /* waiting */
    }
    OutData_Program[iLoop + 1] = RSIP.REG_002CH.WORD;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0008H.BIT.B30)
    {
        /* waiting */
    }
    OutData_Program[iLoop + 2] = RSIP.REG_002CH.WORD;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0008H.BIT.B30)
    {
        /* waiting */
    }
    OutData_Program[iLoop + 3] = RSIP.REG_002CH.WORD;
    RSIP.REG_0014H.WORD = 0x000000a1U;
    RSIP.REG_00D4H.WORD = 0x40000000U;
    RSIP.REG_00D0H.WORD = 0x4a108104U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
    RSIP.REG_00D0H.WORD = 0x0c100055U;
    RSIP.REG_0000H.WORD = 0x00410011U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_00D4H.WORD = 0x40000000U;
    RSIP.REG_00D0H.WORD = 0x0e108455U;
    RSIP.REG_009CH.WORD = 0x81040000U;
    RSIP.REG_0000H.WORD = 0x00490011U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_0014H.WORD = 0x000003c1U;
    RSIP.REG_00D0H.WORD = 0x0922090dU;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = InData_UpMAC[0];
    RSIP.REG_002CH.WORD = InData_UpMAC[1];
    RSIP.REG_002CH.WORD = InData_UpMAC[2];
    RSIP.REG_002CH.WORD = InData_UpMAC[3];
    RSIP.REG_00D0H.WORD = 0x9c300005U;
    RSIP.REG_0000H.WORD = 0x00410011U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    r_rsip_func100(0x60f028fcU, 0x74aade07U, 0xa0306338U, 0x7b6321d5U);
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
    r_rsip_func100(0x082ca0cdU, 0xef91f325U, 0x93a968a3U, 0x4572daf0U);
    RSIP.REG_0040H.WORD = 0x00400000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    if (1U == (RSIP.REG_0040H.BIT.B22))
    {
        r_rsip_func102(0x8cffaddeU, 0x00252503U, 0xfed098f4U, 0x65937ab7U);
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
        r_rsip_func100(0xd5416831U, 0x4c3cc22cU, 0x8eee2441U, 0x145adab2U);
        RSIP.REG_0014H.WORD = 0x000000a1U;
        RSIP.REG_00D0H.WORD = 0x0c100104U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        RSIP.REG_0008H.WORD = 0x00001012U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0008H.BIT.B30)
        {
            /* waiting */
        }
        OutData_MAC[0] = RSIP.REG_002CH.WORD;
        OutData_MAC[1] = RSIP.REG_002CH.WORD;
        OutData_MAC[2] = RSIP.REG_002CH.WORD;
        OutData_MAC[3] = RSIP.REG_002CH.WORD;
        RSIP.REG_006CH.WORD = 0x00000020U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0020H.BIT.B12)
        {
            /* waiting */
        }
        return RSIP_SUCCESS;
    }
}
RSIP_SEC_DEFAULT