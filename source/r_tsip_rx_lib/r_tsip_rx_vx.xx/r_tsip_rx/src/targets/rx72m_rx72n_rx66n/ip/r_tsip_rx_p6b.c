/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED  AND RENESAS MAKES NO WARRANTIES REGARDING
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
 * Copyright (C) 2015-2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 27.06.2015 1.00     First Release
 *         : 15.05.2017 1.01     Add AES-CMAC
 *         : 30.09.2017 1.03     Add Init/Update/Final API and SHA, RSA
 *         : 28.02.2018 1.04     Change Init/Update/Final API for RX231, add TLS function and 
 *         :                     return values change FIT rules.
 *         : 30.04.2018 1.05     Add TDES, MD5 and RSAES-PKCS1-v1_5 API
 *         : 28.09.2018 1.06     Add RSA Key Generation, AES, TDES, RSA Key update features, RX66T support
 *         : 28.12.2018 1.07     Add RX72T support
 *         : 30.09.2019 1.08     Added support for GCC and IAR compiler, ECC API, RX23W and RX72M
 *         : 31.03.2020 1.09     Added support for AES-CCM, HMAC key generation, ECDH, Key Wrap API, RX66N and RX72N
 *         : 30.06.2020 1.10     Added support for ARC4, ECC(P-384) API
 *         : 30.09.2020 1.11     Added support for DH, ECDHE P-512r1 API, and generalization of KDF.
 *         :                     Added support for Key wrap API with TSIP-Lite.
 *         : 30.06.2021 1.12     Added support for RX23W chip version D
 *         : 31.08.2021 1.13     Added support for RX671
 *         : 22.10.2021 1.14     Added support for TLS1.3
 *         : 31.03.2022 1.15     Added support for TLS1.3(RX72M_RX72N_RX66N)
 *         : 15.09.2022 1.16     Added support for RSA 3k/4k and updated support for TLS1.3
 *         : 20.01.2023 1.17     Added support for TLS1.3 server
 *         : 24.05.2023 1.18     Added support for RX26T
 *         : 30.11.2023 1.19     Update example of Secure Bootloader / Firmware Update
 *         : 28.02.2024 1.20     Applied software workaround of AES-CCM decryption
 *         : 28.06.2024 1.21     Added support for TLS1.2 server
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "../r_tsip_rx_private.h"

#if (TSIP_AES_128_KEY_WRAP == 1) || (TSIP_AES_256_KEY_WRAP == 1)
/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Imported global variables and functions (from other files)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private global variables and functions
 *********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_TSIP_AESKeyUnwrapSub
*******************************************************************************************************************/ /**
* @details       RX72M AES-128/256 Key unwrap
* @param[in]     InData_KeyType
* @param[in]     InData_Cmd
* @param[in]     InData_KeyIndex
* @param[in]     InData_WrappedKeyType
* @param[in]     InData_Text
* @param[out]    OutData_KeyIndex
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_AESKeyUnwrapSub(uint32_t *InData_KeyType, uint32_t *InData_Cmd, uint32_t *InData_KeyIndex, uint32_t KEY_INDEX_SIZE, uint32_t *InData_WrappedKeyType, uint32_t *InData_Text, uint32_t WRAPPED_KEY_SIZE, uint32_t *OutData_KeyIndex)
{
    int32_t iLoop = 0u, jLoop = 0u, kLoop = 0u, oLoop1 = 0u, oLoop2 = 0u, iLoop2 = 0u;
    uint32_t KEY_ADR = 0u, OFS_ADR = 0u;
    (void)iLoop;
    (void)jLoop;
    (void)kLoop;
    (void)iLoop2;
    (void)oLoop1;
    (void)oLoop2;
    (void)KEY_ADR;
    (void)OFS_ADR;
    #if TSIP_MULTI_THREADING == 1
    TSIP_MULTI_THREADING_LOCK_FUNCTION();
    #endif /* TSIP_MULTI_THREADING == 1 */
    if (0x0u != (TSIP.REG_1B4H.WORD & 0x1du))
    {
        #if TSIP_MULTI_THREADING == 1
        TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
        #endif /* TSIP_MULTI_THREADING == 1 */
        return TSIP_ERR_RESOURCE_CONFLICT;
    }
    TSIP.REG_84H.WORD = 0x00006b01u;
    TSIP.REG_108H.WORD = 0x00000000u;
    TSIP.REG_104H.WORD = 0x00000268u;
    TSIP.REG_E0H.WORD = 0x80030100u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Cmd[0];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_WrappedKeyType[0];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyType[0];
    TSIP.REG_ECH.WORD = 0x00000bffu;
    TSIP.REG_ECH.WORD = 0x00003409u;
    TSIP.REG_ECH.WORD = 0x3420a900u;
    TSIP.REG_ECH.WORD = 0x00000002u;
    TSIP.REG_ECH.WORD = 0x1000d3e0u;
    TSIP.REG_ECH.WORD = 0x38008940u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00030020u;
    TSIP.REG_ECH.WORD = 0x3420a800u;
    TSIP.REG_ECH.WORD = 0x00000003u;
    TSIP.REG_ECH.WORD = 0x1000d3e1u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x38008940u;
    TSIP.REG_ECH.WORD = 0x00000002u;
    TSIP.REG_ECH.WORD = 0x00030020u;
    TSIP.REG_ECH.WORD = 0x38008800u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x1000d3e1u;
    TSIP.REG_ECH.WORD = 0x38008800u;
    TSIP.REG_ECH.WORD = 0x00000002u;
    TSIP.REG_ECH.WORD = 0x1000d3e1u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x38008be0u;
    TSIP.REG_ECH.WORD = 0x00000003u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x73d38333u), change_endian_long(0x11167b9bu), change_endian_long(0x0b712275u), change_endian_long(0x88374124u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0x63915399u), change_endian_long(0x47251b34u), change_endian_long(0xb2989e87u), change_endian_long(0x3cf4e47cu));
        TSIP.REG_1B4H.WORD = 0x00000040u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_18H.BIT.B12)
        {
            /* waiting */
        }
        #if TSIP_MULTI_THREADING == 1
        TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
        #endif /* TSIP_MULTI_THREADING == 1 */
        return TSIP_ERR_FAIL;
    }
    else
    {
        TSIP.REG_ECH.WORD = 0x38000d08u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0x570720d8u), change_endian_long(0xdf0c2d3cu), change_endian_long(0x7331d445u), change_endian_long(0x0c162b03u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_ECH.WORD = 0x38000d4au;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            RX72M_RX72N_RX66N_func100(change_endian_long(0xbba2f485u), change_endian_long(0x69899f0fu), change_endian_long(0x36cb41aeu), change_endian_long(0x179f6e2fu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_A4H.WORD = 0x200c3b0du;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_KeyIndex[0];
                TSIP.REG_100H.WORD = InData_KeyIndex[1];
                TSIP.REG_100H.WORD = InData_KeyIndex[2];
                TSIP.REG_100H.WORD = InData_KeyIndex[3];
                TSIP.REG_A4H.WORD = 0x000c0b0cu;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x4e2279dbu);
                RX72M_RX72N_RX66N_func101(change_endian_long(0xb57604b5u), change_endian_long(0x06eb54f1u), change_endian_long(0x6b04a845u), change_endian_long(0x55327b45u));
            }
            else
            {
                TSIP.REG_A4H.WORD = 0x000c2b0du;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_KeyIndex[0];
                TSIP.REG_100H.WORD = InData_KeyIndex[1];
                TSIP.REG_100H.WORD = InData_KeyIndex[2];
                TSIP.REG_100H.WORD = InData_KeyIndex[3];
                TSIP.REG_A4H.WORD = 0x000c0b0cu;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x2a46c04bu);
                RX72M_RX72N_RX66N_func101(change_endian_long(0x2903180eu), change_endian_long(0x2fe0977cu), change_endian_long(0x3c21ff13u), change_endian_long(0xa6b1b67bu));
            }
            RX72M_RX72N_RX66N_func100(change_endian_long(0x7b450bacu), change_endian_long(0x9f6b48c4u), change_endian_long(0x4aab3765u), change_endian_long(0x8470f503u));
            TSIP.REG_A4H.WORD = 0x02f8073du;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[4];
            TSIP.REG_100H.WORD = InData_KeyIndex[5];
            TSIP.REG_100H.WORD = InData_KeyIndex[6];
            TSIP.REG_100H.WORD = InData_KeyIndex[7];
            TSIP.REG_A4H.WORD = 0x000007bdu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[8];
            TSIP.REG_100H.WORD = InData_KeyIndex[9];
            TSIP.REG_100H.WORD = InData_KeyIndex[10];
            TSIP.REG_100H.WORD = InData_KeyIndex[11];
            TSIP.REG_B0H.WORD = 0x00000000u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x69cd66ceu), change_endian_long(0xbbe4d1a2u), change_endian_long(0xa4bfb9bdu), change_endian_long(0xaacf85d0u));
        }
        else
        {
            TSIP.REG_ECH.WORD = 0x38000d4au;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            RX72M_RX72N_RX66N_func100(change_endian_long(0x1d4cf32au), change_endian_long(0xb2e2620eu), change_endian_long(0x2aa05087u), change_endian_long(0x78ed6557u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_A4H.WORD = 0x200c3b0du;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_KeyIndex[0];
                TSIP.REG_100H.WORD = InData_KeyIndex[1];
                TSIP.REG_100H.WORD = InData_KeyIndex[2];
                TSIP.REG_100H.WORD = InData_KeyIndex[3];
                TSIP.REG_A4H.WORD = 0x000c0b0cu;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x6e78d475u);
                RX72M_RX72N_RX66N_func101(change_endian_long(0x526ba5dcu), change_endian_long(0x5b2f872cu), change_endian_long(0x4e5b53beu), change_endian_long(0x8d37f29bu));
            }
            else
            {
                TSIP.REG_A4H.WORD = 0x000c2b0du;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_KeyIndex[0];
                TSIP.REG_100H.WORD = InData_KeyIndex[1];
                TSIP.REG_100H.WORD = InData_KeyIndex[2];
                TSIP.REG_100H.WORD = InData_KeyIndex[3];
                TSIP.REG_A4H.WORD = 0x000c0b0cu;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x85d04999u);
                RX72M_RX72N_RX66N_func101(change_endian_long(0x8b7d967cu), change_endian_long(0x1f5429bfu), change_endian_long(0xa4109aabu), change_endian_long(0x5b8fd950u));
            }
            RX72M_RX72N_RX66N_func100(change_endian_long(0x4e144b84u), change_endian_long(0x14053079u), change_endian_long(0x4ca8aaf0u), change_endian_long(0x1008c84au));
            TSIP.REG_A4H.WORD = 0x02f8073du;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[4];
            TSIP.REG_100H.WORD = InData_KeyIndex[5];
            TSIP.REG_100H.WORD = InData_KeyIndex[6];
            TSIP.REG_100H.WORD = InData_KeyIndex[7];
            RX72M_RX72N_RX66N_func100(change_endian_long(0x616b289eu), change_endian_long(0x150ccd91u), change_endian_long(0xad7d1d49u), change_endian_long(0xbf2f2fa6u));
            TSIP.REG_A4H.WORD = 0x00f9073du;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[8];
            TSIP.REG_100H.WORD = InData_KeyIndex[9];
            TSIP.REG_100H.WORD = InData_KeyIndex[10];
            TSIP.REG_100H.WORD = InData_KeyIndex[11];
            TSIP.REG_A4H.WORD = 0x000007bdu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[12];
            TSIP.REG_100H.WORD = InData_KeyIndex[13];
            TSIP.REG_100H.WORD = InData_KeyIndex[14];
            TSIP.REG_100H.WORD = InData_KeyIndex[15];
            TSIP.REG_B0H.WORD = 0x40000000u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x7c3ff602u), change_endian_long(0xcfff0fb8u), change_endian_long(0x6feaf63bu), change_endian_long(0x752a6b89u));
        }
        TSIP.REG_A4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00001113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0x7bbec4eau), change_endian_long(0x3591d6a8u), change_endian_long(0xec85451eu), change_endian_long(0x09d43b87u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func102(change_endian_long(0xcfbb153eu), change_endian_long(0x3313ff78u), change_endian_long(0x4a1975b2u), change_endian_long(0x7705af44u));
            TSIP.REG_1B4H.WORD = 0x00000040u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_18H.BIT.B12)
            {
                /* waiting */
            }
            #if TSIP_MULTI_THREADING == 1
            TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
            #endif /* TSIP_MULTI_THREADING == 1 */
            return TSIP_ERR_KEY_SET;
        }
        else
        {
            RX72M_RX72N_RX66N_func030();
            TSIP.REG_ECH.WORD = 0x00002ca0u;
            TSIP.REG_ECH.WORD = 0x00003500u;
            TSIP.REG_104H.WORD = 0x00000168u;
            TSIP.REG_E0H.WORD = 0x80020000u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_Text[0];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_Text[1];
            TSIP.REG_ECH.WORD = 0x0000b7e0u;
            TSIP.REG_ECH.WORD = 0x00000008u;
            TSIP.REG_ECH.WORD = 0x00000bdeu;
            TSIP.REG_ECH.WORD = 0x00002fc0u;
            TSIP.REG_ECH.WORD = 0x0000b780u;
            TSIP.REG_ECH.WORD = 0x00000008u;
            for(iLoop = 2; iLoop < WRAPPED_KEY_SIZE; iLoop = iLoop + 2)
            {
                TSIP.REG_104H.WORD = 0x00000168u;
                TSIP.REG_E0H.WORD = 0x8082001fu;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_Text[iLoop + 0];
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_Text[iLoop+1 + 0];
                TSIP.REG_ECH.WORD = 0x000027fcu;
                TSIP.REG_ECH.WORD = 0x00002fc0u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0x82e8dceau), change_endian_long(0xace60a0eu), change_endian_long(0x9e991be4u), change_endian_long(0x612e50e9u));
            }
            TSIP.REG_ECH.WORD = 0x38000bc5u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_1CH.WORD = 0x00402000u;
            TSIP.REG_ECH.WORD = 0x0000377fu;
            TSIP.REG_ECH.WORD = 0x0000ab60u;
            TSIP.REG_ECH.WORD = 0x00000008u;
            TSIP.REG_ECH.WORD = 0x00003785u;
            TSIP.REG_ECH.WORD = 0x00003380u;
            TSIP.REG_ECH.WORD = 0x0000349cu;
            TSIP.REG_ECH.WORD = 0x00026c84u;
            TSIP.REG_ECH.WORD = 0x00016f9cu;
            TSIP.REG_ECH.WORD = 0x0000249cu;
            TSIP.REG_ECH.WORD = 0x00000bdeu;
            TSIP.REG_ECH.WORD = 0x0000b720u;
            TSIP.REG_ECH.WORD = 0x00000004u;
            TSIP.REG_ECH.WORD = 0x0000b740u;
            TSIP.REG_ECH.WORD = 0x0000000cu;
            TSIP.REG_ECH.WORD = 0x00003785u;
            TSIP.REG_ECH.WORD = 0x00003380u;
            for(jLoop = 5; jLoop >= 0; jLoop = jLoop - 1)
            {
                TSIP.REG_ECH.WORD = 0x000037fbu;
                TSIP.REG_ECH.WORD = 0x00000bbdu;
                for(iLoop = (WRAPPED_KEY_SIZE/2)-1; iLoop >= 1; iLoop = iLoop - 1)
                {
                    TSIP.REG_ECH.WORD = 0x00000824u;
                    TSIP.REG_A4H.WORD = 0x0000ca85u;
                    TSIP.REG_E0H.WORD = 0x81020000u;
                    TSIP.REG_00H.WORD = 0x0000180bu;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_E0H.WORD = 0x8182001fu;
                    TSIP.REG_00H.WORD = 0x0000180bu;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_E0H.WORD = 0x80040000u;
                    TSIP.REG_00H.WORD = 0x00008113u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_ECH.WORD = 0x00003c5fu;
                    TSIP.REG_ECH.WORD = 0x000027f9u;
                    TSIP.REG_ECH.WORD = 0x00003c7fu;
                    TSIP.REG_ECH.WORD = 0x00002bfau;
                    TSIP.REG_ECH.WORD = 0x00003080u;
                    TSIP.REG_ECH.WORD = 0x00002fa0u;
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x9871fce0u), change_endian_long(0x9a9a0942u), change_endian_long(0xe0262515u), change_endian_long(0x2bb13b49u));
                }
                TSIP.REG_ECH.WORD = 0x38000bbcu;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_1CH.WORD = 0x00402000u;
                TSIP.REG_ECH.WORD = 0x00002fc0u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0x570a7394u), change_endian_long(0x39e5645eu), change_endian_long(0x06e0adcfu), change_endian_long(0x0da67fbeu));
            }
            TSIP.REG_ECH.WORD = 0x38008bc0u;
            TSIP.REG_ECH.WORD = 0x00000006u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_1CH.WORD = 0x00402000u;
            TSIP.REG_ECH.WORD = 0x38008800u;
            TSIP.REG_ECH.WORD = 0xa6a6a6a6u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            TSIP.REG_ECH.WORD = 0x38008820u;
            TSIP.REG_ECH.WORD = 0xa6a6a6a6u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            RX72M_RX72N_RX66N_func100(change_endian_long(0xcad17faeu), change_endian_long(0x73f247eeu), change_endian_long(0xfb404d2du), change_endian_long(0x593a8720u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX72M_RX72N_RX66N_func102(change_endian_long(0xe26af934u), change_endian_long(0x2f8a2128u), change_endian_long(0xd84a1fa0u), change_endian_long(0xfcf65baeu));
                TSIP.REG_1B4H.WORD = 0x00000040u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_18H.BIT.B12)
                {
                    /* waiting */
                }
                #if TSIP_MULTI_THREADING == 1
                TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
                #endif /* TSIP_MULTI_THREADING == 1 */
                return TSIP_ERR_FAIL;
            }
            else
            {
                RX72M_RX72N_RX66N_func100(change_endian_long(0x3412f10du), change_endian_long(0xdd2203e4u), change_endian_long(0xe288b1aau), change_endian_long(0x7bc70441u));
                TSIP.REG_ECH.WORD = 0x00003408u;
                RX72M_RX72N_RX66N_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80040000u;
                TSIP.REG_00H.WORD = 0x00008213u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_A4H.WORD = 0x200c3b0du;
                TSIP.REG_E0H.WORD = 0x81040000u;
                TSIP.REG_00H.WORD = 0x00001813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_A4H.WORD = 0x000c0b0cu;
                TSIP.REG_E0H.WORD = 0x810100c0u;
                TSIP.REG_00H.WORD = 0x00001807u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x0000b7e0u;
                TSIP.REG_ECH.WORD = 0x00000008u;
                TSIP.REG_104H.WORD = 0x00000051u;
                TSIP.REG_A4H.WORD = 0x00040804u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_104H.WORD = 0x00000051u;
                TSIP.REG_A4H.WORD = 0x00050804u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_ECH.WORD = 0x00000bdeu;
                TSIP.REG_ECH.WORD = 0x0000b760u;
                TSIP.REG_ECH.WORD = 0x00000004u;
                TSIP.REG_ECH.WORD = 0x0000b780u;
                TSIP.REG_ECH.WORD = 0x00000010u;
                TSIP.REG_ECH.WORD = 0x0000a8e0u;
                TSIP.REG_ECH.WORD = 0x00000008u;
                for(iLoop = 0; iLoop < KEY_INDEX_SIZE-8; iLoop = iLoop + 4)
                {
                    TSIP.REG_ECH.WORD = 0x000008c6u;
                    TSIP.REG_ECH.WORD = 0x34202bc7u;
                    TSIP.REG_ECH.WORD = 0x2000d0c0u;
                    TSIP.REG_ECH.WORD = 0x00007c06u;
                    TSIP.REG_1CH.WORD = 0x00602000u;
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x7a38f69eu), change_endian_long(0x8e09b65du), change_endian_long(0x5b33582eu), change_endian_long(0x65628d0du));
                    TSIP.REG_A4H.WORD = 0x00e007bdu;
                    TSIP.REG_E0H.WORD = 0x8184001fu;
                    TSIP.REG_00H.WORD = 0x00001813u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_04H.WORD = 0x00000112u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex[iLoop+4 + 0] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex[iLoop+4 + 1] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex[iLoop+4 + 2] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex[iLoop+4 + 3] = TSIP.REG_100H.WORD;
                    TSIP.REG_ECH.WORD = 0x000027fcu;
                    TSIP.REG_ECH.WORD = 0x000027dbu;
                    RX72M_RX72N_RX66N_func101(change_endian_long(0xb0450f61u), change_endian_long(0x64a580b6u), change_endian_long(0x3f37b1dcu), change_endian_long(0xcfcda800u));
                }
                TSIP.REG_ECH.WORD = 0x38000bc7u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_1CH.WORD = 0x00402000u;
                RX72M_RX72N_RX66N_func100(change_endian_long(0x5aa8802au), change_endian_long(0xb0d79e8au), change_endian_long(0xf5126337u), change_endian_long(0x09fae2e6u));
                TSIP.REG_104H.WORD = 0x00000051u;
                TSIP.REG_A4H.WORD = 0x00000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_A4H.WORD = 0x000009cdu;
                TSIP.REG_00H.WORD = 0x00001113u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_04H.WORD = 0x00000112u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_KeyIndex[iLoop+4 + 0] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[iLoop+4 + 1] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[iLoop+4 + 2] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[iLoop+4 + 3] = TSIP.REG_100H.WORD;
                RX72M_RX72N_RX66N_func100(change_endian_long(0x364e7015u), change_endian_long(0xcdddee01u), change_endian_long(0xe9639ca0u), change_endian_long(0x0bd11b56u));
                TSIP.REG_E0H.WORD = 0x81040000u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_KeyIndex[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_KeyIndex[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_KeyIndex[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_KeyIndex[3] = TSIP.REG_100H.WORD;
                RX72M_RX72N_RX66N_func102(change_endian_long(0x7b13681du), change_endian_long(0xe1e380a0u), change_endian_long(0xcba38447u), change_endian_long(0x71b74c28u));
                TSIP.REG_1B4H.WORD = 0x00000040u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_18H.BIT.B12)
                {
                    /* waiting */
                }
                #if TSIP_MULTI_THREADING == 1
                TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
                #endif /* TSIP_MULTI_THREADING == 1 */
                return TSIP_SUCCESS;
            }
        }
    }
}
/**********************************************************************************************************************
 End of function ./input_dir/RX72M/RX72M_p6b_r1.prc
 *********************************************************************************************************************/
#endif /* #if (TSIP_AES_128_KEY_WRAP == 1) || (TSIP_AES_256_KEY_WRAP == 1) */
