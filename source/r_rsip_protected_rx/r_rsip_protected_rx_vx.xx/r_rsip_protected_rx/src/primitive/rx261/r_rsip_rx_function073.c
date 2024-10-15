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

void r_rsip_func073(const uint32_t ARG1[])
{
    int32_t iLoop = 0U, jLoop = 0U, kLoop = 0U, oLoop = 0U;
    uint32_t OFS_ADR = 0U;
    (void)iLoop;
    (void)jLoop;
    (void)kLoop;
    (void)oLoop;
    (void)OFS_ADR;
    r_rsip_func070(ARG1);
    RSIP.REG_00A0H.WORD = 0x20010000U;
    RSIP.REG_00B8H.WORD = 0x0000000aU;
    RSIP.REG_00A4H.WORD = 0x04040010U;
    RSIP.REG_00A0H.WORD = 0x20010001U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_00A8H.BIT.B0)
    {
        /* waiting */
    }
    RSIP.REG_00ACH.WORD = 0x00000001U;
    RSIP.REG_00B0H.WORD = 0x0000141eU;
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
    RSIP.REG_00B4H.WORD = 0x001e000aU;
    RSIP.REG_00B8H.WORD = 0x00140000U;
    RSIP.REG_00A4H.WORD = 0x0404000aU;
    RSIP.REG_00A0H.WORD = 0x20010001U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_00A8H.BIT.B0)
    {
        /* waiting */
    }
    RSIP.REG_00ACH.WORD = 0x00000001U;
    RSIP.REG_00B4H.WORD = 0x00690014U;
    RSIP.REG_00B8H.WORD = 0x00190000U;
    RSIP.REG_00A4H.WORD = 0x0404000aU;
    RSIP.REG_00A0H.WORD = 0x20010001U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_00A8H.BIT.B0)
    {
        /* waiting */
    }
    RSIP.REG_00ACH.WORD = 0x00000001U;
    RSIP.REG_0040H.WORD = 0x00210000U;
    RSIP.REG_00B4H.WORD = 0x001e0069U;
    RSIP.REG_00B8H.WORD = 0x00190000U;
    RSIP.REG_00A4H.WORD = 0x0404000aU;
    RSIP.REG_00A0H.WORD = 0x20010001U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_00A8H.BIT.B0)
    {
        /* waiting */
    }
    RSIP.REG_00ACH.WORD = 0x00000001U;
    RSIP.REG_0040H.WORD = 0x00210000U;
    RSIP.REG_00B4H.WORD = 0x006e0014U;
    RSIP.REG_00B8H.WORD = 0x00190000U;
    RSIP.REG_00A4H.WORD = 0x0404000aU;
    RSIP.REG_00A0H.WORD = 0x20010001U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_00A8H.BIT.B0)
    {
        /* waiting */
    }
    RSIP.REG_00ACH.WORD = 0x00000001U;
    RSIP.REG_0040H.WORD = 0x00210000U;
    RSIP.REG_00B4H.WORD = 0x001e006eU;
    RSIP.REG_00B8H.WORD = 0x00190000U;
    RSIP.REG_00A4H.WORD = 0x0404000aU;
    RSIP.REG_00A0H.WORD = 0x20010001U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_00A8H.BIT.B0)
    {
        /* waiting */
    }
    RSIP.REG_00ACH.WORD = 0x00000001U;
    RSIP.REG_0040H.WORD = 0x00210000U;
    r_rsip_func100(0x7ac0587dU, 0x6075522aU, 0xef89dda1U, 0xd94e952fU);
    RSIP.REG_0040H.WORD = 0x00400000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    if (1U == (RSIP.REG_0040H.BIT.B22))
    {
        RSIP.REG_0094H.WORD = 0x00000800U;
        r_rsip_func101(0xe8db3caaU, 0x19354e64U, 0x18abc944U, 0x8e5fb642U);
    }
    else
    {
        r_rsip_func100(0x5b936a00U, 0xaac05c83U, 0xcd004503U, 0x20a393d3U);
        RSIP.REG_00B4H.WORD = 0x001e0014U;
        RSIP.REG_00B8H.WORD = 0x00190000U;
        RSIP.REG_00A4H.WORD = 0x0404000aU;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_00B4H.WORD = 0x0019006eU;
        RSIP.REG_00B8H.WORD = 0x000f000aU;
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
        RSIP.REG_00B4H.WORD = 0x005a000fU;
        RSIP.REG_00B8H.WORD = 0x0014000aU;
        RSIP.REG_00A4H.WORD = 0x04040002U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_00B0H.WORD = 0x00001423U;
        RSIP.REG_0000H.WORD = 0x00c00021U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_00B4H.WORD = 0x00230072U;
        RSIP.REG_00B8H.WORD = 0x000f0000U;
        RSIP.REG_00A4H.WORD = 0x04040009U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_00B4H.WORD = 0x0014000fU;
        RSIP.REG_00B8H.WORD = 0x000f000aU;
        RSIP.REG_00A4H.WORD = 0x04040002U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_00B4H.WORD = 0x00140069U;
        RSIP.REG_00B8H.WORD = 0x0032000aU;
        RSIP.REG_00A4H.WORD = 0x04040002U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_00B8H.WORD = 0x00000005U;
        RSIP.REG_00A4H.WORD = 0x04040010U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_00B4H.WORD = 0x00370076U;
        RSIP.REG_00B8H.WORD = 0x00190005U;
        RSIP.REG_00A4H.WORD = 0x04040002U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_00B4H.WORD = 0x0037007aU;
        RSIP.REG_00B8H.WORD = 0x001e0005U;
        RSIP.REG_00A4H.WORD = 0x04040002U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
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
        RSIP.REG_002CH.WORD = change_endian_long(0x00000001U);
        RSIP.REG_00B4H.WORD = 0x00140037U;
        RSIP.REG_00B8H.WORD = 0x00230005U;
        RSIP.REG_00A4H.WORD = 0x04040002U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_00B0H.WORD = 0x00001414U;
        RSIP.REG_0000H.WORD = 0x00c00021U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_00B4H.WORD = 0x0014005fU;
        RSIP.REG_00B8H.WORD = 0x00280000U;
        RSIP.REG_00A4H.WORD = 0x04040009U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        r_rsip_func100(0x88306ba5U, 0x19dfde0aU, 0xa5c35c80U, 0x96c7180dU);
        RSIP.REG_00B4H.WORD = 0x00140005U;
        RSIP.REG_00B8H.WORD = 0x002d0000U;
        RSIP.REG_00A4H.WORD = 0x04040009U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_00B4H.WORD = 0x00320023U;
        RSIP.REG_00B8H.WORD = 0x0055002dU;
        RSIP.REG_00A4H.WORD = 0x04040015U;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_0040H.WORD = 0x00000d00U;
        RSIP.REG_00B0H.WORD = 0x0000141eU;
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
        RSIP.REG_00B4H.WORD = 0x001e0055U;
        RSIP.REG_00B8H.WORD = 0x00190000U;
        RSIP.REG_00A4H.WORD = 0x0404000aU;
        RSIP.REG_00A0H.WORD = 0x20010001U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_00A8H.BIT.B0)
        {
            /* waiting */
        }
        RSIP.REG_00ACH.WORD = 0x00000001U;
        RSIP.REG_0040H.WORD = 0x00210000U;
        r_rsip_func100(0x25dd7497U, 0xe42ebf80U, 0x9b55396cU, 0x45665312U);
        RSIP.REG_0040H.WORD = 0x00400000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (1U == (RSIP.REG_0040H.BIT.B22))
        {
            RSIP.REG_0094H.WORD = 0x00000800U;
            r_rsip_func101(0x24fcbdf4U, 0x7dc7f16eU, 0x89561bc2U, 0x6fe01d80U);
        }
        else
        {
            RSIP.REG_00B4H.WORD = 0x001e000fU;
            RSIP.REG_00B8H.WORD = 0x00190000U;
            RSIP.REG_00A4H.WORD = 0x0404000aU;
            RSIP.REG_00A0H.WORD = 0x20010001U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_00A8H.BIT.B0)
            {
                /* waiting */
            }
            RSIP.REG_00ACH.WORD = 0x00000001U;
            RSIP.REG_0040H.WORD = 0x00210000U;
            r_rsip_func100(0x1796f6bbU, 0xa8735d89U, 0xce1c017cU, 0x589fda00U);
            RSIP.REG_0040H.WORD = 0x00400000U;
            RSIP.REG_0024H.WORD = 0x00000000U;
            if (1U == (RSIP.REG_0040H.BIT.B22))
            {
                r_rsip_func100(0xebda1abbU, 0x33d2eb13U, 0xa5a8c537U, 0xd4bc3602U);
                RSIP.REG_00B4H.WORD = 0x0014004bU;
                RSIP.REG_00B8H.WORD = 0x00190000U;
                RSIP.REG_00A4H.WORD = 0x04040009U;
                RSIP.REG_00A0H.WORD = 0x20010001U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_00A8H.BIT.B0)
                {
                    /* waiting */
                }
                RSIP.REG_00ACH.WORD = 0x00000001U;
                RSIP.REG_00B4H.WORD = 0x00140050U;
                RSIP.REG_00B8H.WORD = 0x001e0000U;
                RSIP.REG_00A4H.WORD = 0x04040009U;
                RSIP.REG_00A0H.WORD = 0x20010001U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_00A8H.BIT.B0)
                {
                    /* waiting */
                }
                RSIP.REG_00ACH.WORD = 0x00000001U;
                RSIP.REG_00B4H.WORD = 0x00140055U;
                RSIP.REG_00B8H.WORD = 0x00230000U;
                RSIP.REG_00A4H.WORD = 0x04040009U;
                RSIP.REG_00A0H.WORD = 0x20010001U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_00A8H.BIT.B0)
                {
                    /* waiting */
                }
                RSIP.REG_00ACH.WORD = 0x00000001U;
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
                RSIP.REG_002CH.WORD = change_endian_long(0x00000001U);
                RSIP.REG_00B4H.WORD = 0x00140023U;
                RSIP.REG_00B8H.WORD = 0x00280005U;
                RSIP.REG_00A4H.WORD = 0x04040002U;
                RSIP.REG_00A0H.WORD = 0x20010001U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_00A8H.BIT.B0)
                {
                    /* waiting */
                }
                RSIP.REG_00ACH.WORD = 0x00000001U;
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
                RSIP.REG_00B4H.WORD = 0x00140005U;
                RSIP.REG_00B8H.WORD = 0x000f0000U;
                RSIP.REG_00A4H.WORD = 0x0404000aU;
                RSIP.REG_00A0H.WORD = 0x20010001U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_00A8H.BIT.B0)
                {
                    /* waiting */
                }
                RSIP.REG_00ACH.WORD = 0x00000001U;
                RSIP.REG_00B4H.WORD = 0x000f0028U;
                RSIP.REG_00B8H.WORD = 0x00140005U;
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
                RSIP.REG_00B4H.WORD = 0x00140019U;
                RSIP.REG_00B8H.WORD = 0x00280005U;
                RSIP.REG_00A4H.WORD = 0x04040002U;
                RSIP.REG_00A0H.WORD = 0x20010001U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_00A8H.BIT.B0)
                {
                    /* waiting */
                }
                RSIP.REG_00ACH.WORD = 0x00000001U;
                RSIP.REG_00B4H.WORD = 0x00690028U;
                RSIP.REG_00B8H.WORD = 0x00190000U;
                RSIP.REG_00A4H.WORD = 0x0404000aU;
                RSIP.REG_00A0H.WORD = 0x20010001U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_00A8H.BIT.B0)
                {
                    /* waiting */
                }
                RSIP.REG_00ACH.WORD = 0x00000001U;
                RSIP.REG_0040H.WORD = 0x00210000U;
                RSIP.REG_00B4H.WORD = 0x00280069U;
                RSIP.REG_00B8H.WORD = 0x00190000U;
                RSIP.REG_00A4H.WORD = 0x0404000aU;
                RSIP.REG_00A0H.WORD = 0x20010001U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_00A8H.BIT.B0)
                {
                    /* waiting */
                }
                RSIP.REG_00ACH.WORD = 0x00000001U;
                RSIP.REG_0040H.WORD = 0x00210000U;
                r_rsip_func100(0xfd33e751U, 0xeacb0be0U, 0x6706b548U, 0xbaef3a3bU);
                RSIP.REG_0040H.WORD = 0x00400000U;
                RSIP.REG_0024H.WORD = 0x00000000U;
                if (1U == (RSIP.REG_0040H.BIT.B22))
                {
                    RSIP.REG_0094H.WORD = 0x00000800U;
                    r_rsip_func101(0x91e3fa64U, 0x81c09dd0U, 0xc6eefbecU, 0x4f274420U);
                }
                else
                {
                    RSIP.REG_0094H.WORD = 0x0000b400U;
                    RSIP.REG_0094H.WORD = 0x3070de88U;
                    r_rsip_func101(0x65a86970U, 0x8cad262bU, 0xb727bc05U, 0x448a65dfU);
                }
            }
            else
            {
                r_rsip_func100(0x270d820cU, 0x53ecd1ebU, 0x57a9776cU, 0x7dca9d49U);
                RSIP.REG_00B4H.WORD = 0x0014000fU;
                RSIP.REG_00B8H.WORD = 0x00320000U;
                RSIP.REG_00A4H.WORD = 0x04040009U;
                RSIP.REG_00A0H.WORD = 0x20010001U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_00A8H.BIT.B0)
                {
                    /* waiting */
                }
                RSIP.REG_00ACH.WORD = 0x00000001U;
                RSIP.REG_00B0H.WORD = 0x00000415U;
                RSIP.REG_0000H.WORD = 0x00c00009U;
                /* WAIT_LOOP */
                while (0U != RSIP.REG_0004H.BIT.B30)
                {
                    /* waiting */
                }
                RSIP.REG_0040H.WORD = 0x00001800U;
                RSIP.REG_00B0H.WORD = 0x0000041aU;
                RSIP.REG_0000H.WORD = 0x00c00009U;
                /* WAIT_LOOP */
                while (0U != RSIP.REG_0004H.BIT.B30)
                {
                    /* waiting */
                }
                RSIP.REG_0040H.WORD = 0x00001800U;
                r_rsip_func071(ARG1);
                RSIP.REG_00B4H.WORD = 0x00320023U;
                RSIP.REG_00B8H.WORD = 0x0046002dU;
                RSIP.REG_00A4H.WORD = 0x04040015U;
                RSIP.REG_00A0H.WORD = 0x20010001U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_00A8H.BIT.B0)
                {
                    /* waiting */
                }
                RSIP.REG_00ACH.WORD = 0x00000001U;
                RSIP.REG_0040H.WORD = 0x00000d00U;
                RSIP.REG_00B0H.WORD = 0x0000141eU;
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
                RSIP.REG_00B4H.WORD = 0x001e0046U;
                RSIP.REG_00B8H.WORD = 0x00190000U;
                RSIP.REG_00A4H.WORD = 0x0404000aU;
                RSIP.REG_00A0H.WORD = 0x20010001U;
                /* WAIT_LOOP */
                while (1U != RSIP.REG_00A8H.BIT.B0)
                {
                    /* waiting */
                }
                RSIP.REG_00ACH.WORD = 0x00000001U;
                RSIP.REG_0040H.WORD = 0x00210000U;
                r_rsip_func100(0xdfc3066dU, 0x0706291eU, 0xed82cd6dU, 0x41e85891U);
                RSIP.REG_0040H.WORD = 0x00400000U;
                RSIP.REG_0024H.WORD = 0x00000000U;
                if (1U == (RSIP.REG_0040H.BIT.B22))
                {
                    RSIP.REG_0094H.WORD = 0x00000800U;
                    r_rsip_func101(0xf8551ebcU, 0x06feb63aU, 0xb7f0aa31U, 0x0bebd31bU);
                }
                else
                {
                    RSIP.REG_00B4H.WORD = 0x00550046U;
                    RSIP.REG_00B8H.WORD = 0x00230005U;
                    RSIP.REG_00A4H.WORD = 0x04040013U;
                    RSIP.REG_00A0H.WORD = 0x20010001U;
                    /* WAIT_LOOP */
                    while (1U != RSIP.REG_00A8H.BIT.B0)
                    {
                        /* waiting */
                    }
                    RSIP.REG_00ACH.WORD = 0x00000001U;
                    RSIP.REG_0040H.WORD = 0x00000d00U;
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
                    RSIP.REG_002CH.WORD = change_endian_long(0x00000001U);
                    RSIP.REG_00B4H.WORD = 0x00140023U;
                    RSIP.REG_00B8H.WORD = 0x002d0000U;
                    RSIP.REG_00A4H.WORD = 0x0404000aU;
                    RSIP.REG_00A0H.WORD = 0x20010001U;
                    /* WAIT_LOOP */
                    while (1U != RSIP.REG_00A8H.BIT.B0)
                    {
                        /* waiting */
                    }
                    RSIP.REG_00ACH.WORD = 0x00000001U;
                    RSIP.REG_0040H.WORD = 0x00210000U;
                    r_rsip_func100(0x173d3dadU, 0x753a34a9U, 0x731cd2b7U, 0x416fd120U);
                    RSIP.REG_0040H.WORD = 0x00400000U;
                    RSIP.REG_0024H.WORD = 0x00000000U;
                    if (1U == (RSIP.REG_0040H.BIT.B22))
                    {
                        RSIP.REG_0094H.WORD = 0x00000800U;
                        r_rsip_func101(0xad3b5147U, 0xfc8518c3U, 0xb79ee941U, 0x6e76d047U);
                    }
                    else
                    {
                        r_rsip_func100(0x3262e9fdU, 0xb533b4e8U, 0xb400208aU, 0x1918628dU);
                        RSIP.REG_00B4H.WORD = 0x00140023U;
                        RSIP.REG_00B8H.WORD = 0x00280005U;
                        RSIP.REG_00A4H.WORD = 0x04040002U;
                        RSIP.REG_00A0H.WORD = 0x20010001U;
                        /* WAIT_LOOP */
                        while (1U != RSIP.REG_00A8H.BIT.B0)
                        {
                            /* waiting */
                        }
                        RSIP.REG_00ACH.WORD = 0x00000001U;
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
                        RSIP.REG_00B4H.WORD = 0x00140005U;
                        RSIP.REG_00B8H.WORD = 0x000f0000U;
                        RSIP.REG_00A4H.WORD = 0x0404000aU;
                        RSIP.REG_00A0H.WORD = 0x20010001U;
                        /* WAIT_LOOP */
                        while (1U != RSIP.REG_00A8H.BIT.B0)
                        {
                            /* waiting */
                        }
                        RSIP.REG_00ACH.WORD = 0x00000001U;
                        RSIP.REG_00B4H.WORD = 0x000f0028U;
                        RSIP.REG_00B8H.WORD = 0x00140005U;
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
                        RSIP.REG_00B4H.WORD = 0x00140019U;
                        RSIP.REG_00B8H.WORD = 0x00280005U;
                        RSIP.REG_00A4H.WORD = 0x04040002U;
                        RSIP.REG_00A0H.WORD = 0x20010001U;
                        /* WAIT_LOOP */
                        while (1U != RSIP.REG_00A8H.BIT.B0)
                        {
                            /* waiting */
                        }
                        RSIP.REG_00ACH.WORD = 0x00000001U;
                        RSIP.REG_00B4H.WORD = 0x00690028U;
                        RSIP.REG_00B8H.WORD = 0x00190000U;
                        RSIP.REG_00A4H.WORD = 0x0404000aU;
                        RSIP.REG_00A0H.WORD = 0x20010001U;
                        /* WAIT_LOOP */
                        while (1U != RSIP.REG_00A8H.BIT.B0)
                        {
                            /* waiting */
                        }
                        RSIP.REG_00ACH.WORD = 0x00000001U;
                        RSIP.REG_0040H.WORD = 0x00210000U;
                        RSIP.REG_00B4H.WORD = 0x00280069U;
                        RSIP.REG_00B8H.WORD = 0x00190000U;
                        RSIP.REG_00A4H.WORD = 0x0404000aU;
                        RSIP.REG_00A0H.WORD = 0x20010001U;
                        /* WAIT_LOOP */
                        while (1U != RSIP.REG_00A8H.BIT.B0)
                        {
                            /* waiting */
                        }
                        RSIP.REG_00ACH.WORD = 0x00000001U;
                        RSIP.REG_0040H.WORD = 0x00210000U;
                        r_rsip_func100(0x8222ff43U, 0xdb7cf239U, 0x6a0fb8bdU, 0x3ad1ec65U);
                        RSIP.REG_0040H.WORD = 0x00400000U;
                        RSIP.REG_0024H.WORD = 0x00000000U;
                        if (1U == (RSIP.REG_0040H.BIT.B22))
                        {
                            RSIP.REG_0094H.WORD = 0x00000800U;
                            r_rsip_func101(0xf38426faU, 0xecd7107cU, 0xb90779b1U, 0x9166bf88U);
                        }
                        else
                        {
                            RSIP.REG_0094H.WORD = 0x0000b400U;
                            RSIP.REG_0094H.WORD = 0x3070de88U;
                            r_rsip_func101(0xc0adb13dU, 0xe9aa6125U, 0x0d469baeU, 0x388b8355U);
                        }
                    }
                }
            }
        }
    }
    RSIP.REG_0094H.WORD = 0x38008800U;
    RSIP.REG_0094H.WORD = 0x3070de88U;
    RSIP.REG_009CH.WORD = 0x00000080U;
    RSIP.REG_0040H.WORD = 0x00A60000U;
    RSIP.REG_0094H.WORD = 0x00007c07U;
    RSIP.REG_0040H.WORD = 0x00602000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
}
