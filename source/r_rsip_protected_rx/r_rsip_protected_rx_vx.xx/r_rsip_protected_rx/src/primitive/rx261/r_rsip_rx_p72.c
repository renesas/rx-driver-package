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

rsip_err_t r_rsip_p72(const uint32_t InData_InitVal[], const uint32_t InData_PaddedMsg[], const uint32_t MAX_CNT, uint32_t OutData_MsgDigest[])
{
    int32_t iLoop = 0U, jLoop = 0U, kLoop = 0U, oLoop = 0U;
    uint32_t OFS_ADR = 0U;
    (void)iLoop;
    (void)jLoop;
    (void)kLoop;
    (void)oLoop;
    (void)OFS_ADR;
    if (0x0U != (RSIP.REG_0068H.WORD & 0x16U))
    {
        return RSIP_ERR_RESOURCE_CONFLICT;
    }
    RSIP.REG_0070H.WORD = 0x00720001U;
    RSIP.REG_004CH.WORD = 0x00000000U;
    RSIP.REG_00F4H.WORD = 0x00000010U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_00F8H.BIT.B0)
    {
        /* waiting */
    }
    RSIP.REG_0014H.WORD = 0x000007c4U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = InData_InitVal[0];
    RSIP.REG_002CH.WORD = InData_InitVal[1];
    RSIP.REG_002CH.WORD = InData_InitVal[2];
    RSIP.REG_002CH.WORD = InData_InitVal[3];
    RSIP.REG_002CH.WORD = InData_InitVal[4];
    RSIP.REG_002CH.WORD = InData_InitVal[5];
    RSIP.REG_002CH.WORD = InData_InitVal[6];
    RSIP.REG_002CH.WORD = InData_InitVal[7];
    RSIP.REG_00F4H.WORD = 0x00000011U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_00F8H.BIT.B0)
    {
        /* waiting */
    }
    RSIP.REG_0014H.WORD = 0x00000064U;
    for (iLoop = 0; iLoop < MAX_CNT ; iLoop = iLoop + 16)
    {
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 0];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 1];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 2];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 3];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 4];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 5];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 6];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 7];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 8];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 9];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 10];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 11];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 12];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 13];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 14];
        RSIP.REG_002CH.WORD = InData_PaddedMsg[iLoop + 15];
    }
    /* WAIT_LOOP */
    while (0U != RSIP.REG_00F8H.BIT.B2)
    {
        /* waiting */
    }
    RSIP.REG_0014H.WORD = 0x00000000U;
    RSIP.REG_00F4H.WORD = 0x00000100U;
    RSIP.REG_00F4H.WORD = 0x00000020U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_00F8H.BIT.B1)
    {
        /* waiting */
    }
    r_rsip_func100(0x72b5d012U, 0x1adbaf00U, 0x1ba4a8faU, 0x1df67646U);
    RSIP.REG_0008H.WORD = 0x00004022U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0008H.BIT.B30)
    {
        /* waiting */
    }
    OutData_MsgDigest[0] = RSIP.REG_002CH.WORD;
    OutData_MsgDigest[1] = RSIP.REG_002CH.WORD;
    OutData_MsgDigest[2] = RSIP.REG_002CH.WORD;
    OutData_MsgDigest[3] = RSIP.REG_002CH.WORD;
    OutData_MsgDigest[4] = RSIP.REG_002CH.WORD;
    OutData_MsgDigest[5] = RSIP.REG_002CH.WORD;
    OutData_MsgDigest[6] = RSIP.REG_002CH.WORD;
    OutData_MsgDigest[7] = RSIP.REG_002CH.WORD;
    r_rsip_func102(0xcc0ef20eU, 0x27fbb21cU, 0x6f5c36ffU, 0x5482252bU);
    RSIP.REG_0068H.WORD = 0x00000040U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0020H.BIT.B12)
    {
        /* waiting */
    }
    return RSIP_SUCCESS;
}
