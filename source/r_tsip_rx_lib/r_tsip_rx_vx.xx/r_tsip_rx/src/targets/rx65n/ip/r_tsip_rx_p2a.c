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

#if TSIP_PRV_USE_RSA_1024 == 1
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
* Function Name: R_TSIP_GenerateRsa1024RandomKeyIndexSub
*******************************************************************************************************************/ /**
* @details       RX65NHU RSA-1024 Key Generation
* @param[out]    OutData_PubKeyIndex
* @param[out]    OutData_PrivKeyIndex
* @retval        TSIP_SUCCESS
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRsa1024RandomKeyIndexSub(uint32_t MAX_CNT, uint32_t *OutData_PubKeyIndex, uint32_t *OutData_PrivKeyIndex)
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
    if (0x0u != (TSIP.REG_1BCH.WORD & 0x1fu))
    {
        #if TSIP_MULTI_THREADING == 1
        TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
        #endif /* TSIP_MULTI_THREADING == 1 */
        return TSIP_ERR_RESOURCE_CONFLICT;
    }
    TSIP.REG_84H.WORD = 0x00002a01u;
    TSIP.REG_108H.WORD = 0x00000000u;
    TSIP.REG_C4H.WORD = 0x200e1a0du;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_RAM[16+0 + 0];
    TSIP.REG_100H.WORD = S_RAM[16+0 + 1];
    TSIP.REG_100H.WORD = S_RAM[16+0 + 2];
    TSIP.REG_100H.WORD = S_RAM[16+0 + 3];
    RX65NHU_func100(change_endian_long(0x9f3cb56bu), change_endian_long(0x14bd5e59u), change_endian_long(0xf8aef41au), change_endian_long(0x094b9c42u));
    RX65NHU_func103();
    RX65NHU_func100(change_endian_long(0x5393ce31u), change_endian_long(0xdf0d6040u), change_endian_long(0xdffae5feu), change_endian_long(0xe2ad0a87u));
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x01080c84u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_ECH.WORD = 0x00000a31u;
    for(kLoop = 0; kLoop < MAX_CNT; kLoop = kLoop + 1)
    {
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_00H.WORD = 0x00003043u;
        TSIP.REG_2CH.WORD = 0x00000012u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00002f57u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00030005u);
        TSIP.REG_100H.WORD = change_endian_long(0x0007000bu);
        TSIP.REG_100H.WORD = change_endian_long(0x000d0011u);
        TSIP.REG_100H.WORD = change_endian_long(0x00130017u);
        TSIP.REG_100H.WORD = change_endian_long(0x001d001fu);
        TSIP.REG_100H.WORD = change_endian_long(0x00250029u);
        TSIP.REG_100H.WORD = change_endian_long(0x002b002fu);
        TSIP.REG_100H.WORD = change_endian_long(0x003b003du);
        TSIP.REG_100H.WORD = change_endian_long(0x00430047u);
        TSIP.REG_100H.WORD = change_endian_long(0x0049004fu);
        TSIP.REG_100H.WORD = change_endian_long(0x00530059u);
        TSIP.REG_100H.WORD = change_endian_long(0x00610065u);
        TSIP.REG_100H.WORD = change_endian_long(0x0067006bu);
        TSIP.REG_100H.WORD = change_endian_long(0x006d0071u);
        TSIP.REG_100H.WORD = change_endian_long(0x007f0083u);
        TSIP.REG_100H.WORD = change_endian_long(0x0089008bu);
        TSIP.REG_100H.WORD = change_endian_long(0x00950097u);
        TSIP.REG_100H.WORD = change_endian_long(0x009d00a3u);
        TSIP.REG_100H.WORD = change_endian_long(0x00a700adu);
        TSIP.REG_100H.WORD = change_endian_long(0x00b300b5u);
        TSIP.REG_100H.WORD = change_endian_long(0x00bf00c1u);
        TSIP.REG_100H.WORD = change_endian_long(0x00c500c7u);
        TSIP.REG_100H.WORD = change_endian_long(0x00d300dfu);
        TSIP.REG_100H.WORD = change_endian_long(0x00e300e5u);
        TSIP.REG_100H.WORD = change_endian_long(0x00e900efu);
        TSIP.REG_100H.WORD = change_endian_long(0x00f100fbu);
        TSIP.REG_100H.WORD = change_endian_long(0x01010107u);
        TSIP.REG_100H.WORD = change_endian_long(0x010d010fu);
        TSIP.REG_100H.WORD = change_endian_long(0x01150119u);
        TSIP.REG_100H.WORD = change_endian_long(0x011b0125u);
        TSIP.REG_100H.WORD = change_endian_long(0x01330137u);
        TSIP.REG_100H.WORD = change_endian_long(0x0139013du);
        TSIP.REG_100H.WORD = change_endian_long(0x014b0151u);
        TSIP.REG_100H.WORD = change_endian_long(0x015b015du);
        TSIP.REG_100H.WORD = change_endian_long(0x01610167u);
        TSIP.REG_100H.WORD = change_endian_long(0x016f0175u);
        TSIP.REG_100H.WORD = change_endian_long(0x017b017fu);
        TSIP.REG_100H.WORD = change_endian_long(0x0185018du);
        TSIP.REG_100H.WORD = change_endian_long(0x01910199u);
        TSIP.REG_100H.WORD = change_endian_long(0x01a301a5u);
        TSIP.REG_100H.WORD = change_endian_long(0x01af01b1u);
        TSIP.REG_100H.WORD = change_endian_long(0x01b701bbu);
        TSIP.REG_100H.WORD = change_endian_long(0x01c101c9u);
        TSIP.REG_100H.WORD = change_endian_long(0x01cd01cfu);
        TSIP.REG_100H.WORD = change_endian_long(0x01d301dfu);
        TSIP.REG_100H.WORD = change_endian_long(0x01e701ebu);
        TSIP.REG_100H.WORD = change_endian_long(0x01f301f7u);
        TSIP.REG_100H.WORD = change_endian_long(0x01fd0000u);
        TSIP.REG_ECH.WORD = 0x000008c6u;
        TSIP.REG_00H.WORD = 0x00000343u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_E0H.WORD = 0x80b00006u;
        TSIP.REG_00H.WORD = 0x000083c3u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x008f0001u;
        RX65NHU_func100(change_endian_long(0xb4613069u), change_endian_long(0x6ba3cc60u), change_endian_long(0x44a48756u), change_endian_long(0x8b05f3fbu));
        RX65NHU_func103();
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
        TSIP.REG_ECH.WORD = 0x0000d01fu;
        TSIP.REG_E0H.WORD = 0x81040000u;
        TSIP.REG_00H.WORD = 0x00003813u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        for(iLoop = 0; iLoop < 8; iLoop = iLoop + 4)
        {
            RX65NHU_func100(change_endian_long(0xb2ab5184u), change_endian_long(0x13b81163u), change_endian_long(0x38a2a2f7u), change_endian_long(0xb339b103u));
            RX65NHU_func103();
            TSIP.REG_104H.WORD = 0x00000052u;
            TSIP.REG_C4H.WORD = 0x01000c84u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_00H.WORD = 0x00003213u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX65NHU_func101(change_endian_long(0xa42e6b80u), change_endian_long(0x1b89af19u), change_endian_long(0xcff06708u), change_endian_long(0x9d8bb19bu));
        }
        RX65NHU_func100(change_endian_long(0xb2ab5184u), change_endian_long(0x13b81163u), change_endian_long(0x38a2a2f7u), change_endian_long(0xb339b103u));
        RX65NHU_func103();
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
        TSIP.REG_ECH.WORD = 0x0000d060u;
        TSIP.REG_E0H.WORD = 0x81040000u;
        TSIP.REG_00H.WORD = 0x00003813u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_2CH.WORD = 0x00000011u;
        TSIP.REG_104H.WORD = 0x00000f57u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xB51EB851u);
        TSIP.REG_100H.WORD = change_endian_long(0xEB851EB8u);
        TSIP.REG_100H.WORD = change_endian_long(0x51EB851Eu);
        TSIP.REG_100H.WORD = change_endian_long(0xB851EB85u);
        TSIP.REG_100H.WORD = change_endian_long(0x1EB851EBu);
        TSIP.REG_100H.WORD = change_endian_long(0x851EB851u);
        TSIP.REG_100H.WORD = change_endian_long(0xEB851EB8u);
        TSIP.REG_100H.WORD = change_endian_long(0x51EB851Eu);
        TSIP.REG_100H.WORD = change_endian_long(0xB851EB85u);
        TSIP.REG_100H.WORD = change_endian_long(0x1EB851EBu);
        TSIP.REG_100H.WORD = change_endian_long(0x851EB851u);
        TSIP.REG_100H.WORD = change_endian_long(0xEB851EB8u);
        TSIP.REG_100H.WORD = change_endian_long(0x51EB851Eu);
        TSIP.REG_100H.WORD = change_endian_long(0xB851EB85u);
        TSIP.REG_100H.WORD = change_endian_long(0x1EB851EBu);
        TSIP.REG_100H.WORD = change_endian_long(0x851EB80Eu);
        TSIP.REG_24H.WORD = 0x000011c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00001591u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00210000u;
        RX65NHU_func100(change_endian_long(0xe61ce3d9u), change_endian_long(0x62f16887u), change_endian_long(0x006d90c3u), change_endian_long(0x66affdc6u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func101(change_endian_long(0x3fcccf37u), change_endian_long(0x6a4da876u), change_endian_long(0x529c37d1u), change_endian_long(0xb4561308u));
            continue;
        }
        else
        {
            RX65NHU_func101(change_endian_long(0xe85b208au), change_endian_long(0x6d7cd80cu), change_endian_long(0x34199054u), change_endian_long(0x3abb29bcu));
        }
        TSIP.REG_ECH.WORD = 0x38008a20u;
        TSIP.REG_ECH.WORD = 0x00000001u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX65NHU_func100(change_endian_long(0x78fd6472u), change_endian_long(0xce62af9au), change_endian_long(0x8cca50b2u), change_endian_long(0x4b228a5au));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_24H.WORD = 0x00001dc0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x00001191u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00210000u;
            RX65NHU_func100(change_endian_long(0xadf5633au), change_endian_long(0xf75d329cu), change_endian_long(0x9fcc297bu), change_endian_long(0x35a6fa01u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_24H.WORD = 0x000011c0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00001d91u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                RX65NHU_func101(change_endian_long(0x66265781u), change_endian_long(0xea259d13u), change_endian_long(0xa31a0413u), change_endian_long(0x5b5749ccu));
            }
            else
            {
                RX65NHU_func101(change_endian_long(0x1e7a27b4u), change_endian_long(0xbd923e19u), change_endian_long(0x28e36f7cu), change_endian_long(0xc36de9c4u));
            }
            TSIP.REG_2CH.WORD = 0x00000011u;
            TSIP.REG_104H.WORD = 0x00000357u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_100H.WORD = change_endian_long(0x10000000u);
            TSIP.REG_00H.WORD = 0x00003033u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_24H.WORD = 0x00001591u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00210000u;
            RX65NHU_func100(change_endian_long(0x397841a1u), change_endian_long(0xc886276du), change_endian_long(0x3c26d6acu), change_endian_long(0xbe3f3c2du));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX65NHU_func101(change_endian_long(0xde030301u), change_endian_long(0x4c9c79efu), change_endian_long(0xf044c848u), change_endian_long(0x9a77599eu));
                continue;
            }
            else
            {
                RX65NHU_func101(change_endian_long(0x1e32111du), change_endian_long(0x3468d05eu), change_endian_long(0x118ab4b0u), change_endian_long(0xd061a945u));
            }
        }
        else
        {
            RX65NHU_func101(change_endian_long(0x572e7ed0u), change_endian_long(0xaddca687u), change_endian_long(0xc990391du), change_endian_long(0x4cd01da5u));
        }
        TSIP.REG_ECH.WORD = 0x000008c6u;
        TSIP.REG_ECH.WORD = 0x0000094au;
        for(iLoop = 0; iLoop < 96; iLoop = iLoop + 1)
        {
            TSIP.REG_ECH.WORD = 0x01003906u;
            TSIP.REG_ECH.WORD = 0x00002cc0u;
            TSIP.REG_ECH.WORD = 0x00002cc0u;
            TSIP.REG_E0H.WORD = 0x81010100u;
            TSIP.REG_00H.WORD = 0x0000303fu;
            TSIP.REG_2CH.WORD = 0x00000014u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_00H.WORD = 0x00003807u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_24H.WORD = 0x00004006u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x000009c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x00000591u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00210000u;
            RX65NHU_func100(change_endian_long(0xd0f91f17u), change_endian_long(0x6e2ca343u), change_endian_long(0x12b294edu), change_endian_long(0xc2044961u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_ECH.WORD = 0x0000d140u;
                RX65NHU_func101(change_endian_long(0x9a6d607cu), change_endian_long(0x971b61e3u), change_endian_long(0xfa4f0354u), change_endian_long(0xeac52293u));
                break;
            }
            else
            {
                RX65NHU_func101(change_endian_long(0xd220a36bu), change_endian_long(0x9145d9d9u), change_endian_long(0xedb0d2fbu), change_endian_long(0x1b5cdf58u));
            }
        }
        TSIP.REG_ECH.WORD = 0x38008940u;
        TSIP.REG_ECH.WORD = 0x00000001u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX65NHU_func100(change_endian_long(0xf241ef53u), change_endian_long(0x48953479u), change_endian_long(0xa7c43325u), change_endian_long(0x0a9695e4u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func101(change_endian_long(0xda249c4au), change_endian_long(0x6779f69cu), change_endian_long(0x5b6bf23fu), change_endian_long(0xc05d318du));
            continue;
        }
        TSIP.REG_24H.WORD = 0x000011c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x000060c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x000011c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00000591u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x000080c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_00H.WORD = 0x0000303fu;
        TSIP.REG_2CH.WORD = 0x00000014u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000057u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00010001u);
        TSIP.REG_24H.WORD = 0x00004006u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00000dc0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x000080c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x000009c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00000591u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00210000u;
        RX65NHU_func100(change_endian_long(0x2642c8deu), change_endian_long(0x6bc6fd0cu), change_endian_long(0xa12f5363u), change_endian_long(0x19b44f48u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func101(change_endian_long(0x84af20bcu), change_endian_long(0x7dc40971u), change_endian_long(0x030c4388u), change_endian_long(0x6d4a22c8u));
        }
        else
        {
            TSIP.REG_24H.WORD = 0x0000d0d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x000019c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x00000591u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x0000a0c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x000084d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_34H.WORD = 0x00000800u;
            TSIP.REG_24H.WORD = 0x8002d008u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x000005c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x00000189u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x000080c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            RX65NHU_func100(change_endian_long(0x9d27443fu), change_endian_long(0x4a37a076u), change_endian_long(0x6aebca17u), change_endian_long(0xea1d4901u));
            TSIP.REG_18H.WORD = 0x00000004u;
            TSIP.REG_38H.WORD = 0x00004080u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_18H.BIT.B10)
            {
                /* waiting */
            }
            TSIP.REG_18H.WORD = 0x00000000u;
            TSIP.REG_24H.WORD = 0x000005c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x00001191u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00210000u;
            TSIP.REG_24H.WORD = 0x000044d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x000011c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x00000991u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00210000u;
            RX65NHU_func100(change_endian_long(0x349fbf27u), change_endian_long(0xffdcdf71u), change_endian_long(0x35ccecb0u), change_endian_long(0xf3e32e10u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX65NHU_func101(change_endian_long(0xff41bb9fu), change_endian_long(0x18bdee30u), change_endian_long(0x8bcfa36eu), change_endian_long(0x76ea8469u));
                continue;
            }
            TSIP.REG_24H.WORD = 0x000098d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_ECH.WORD = 0x0000b540u;
            TSIP.REG_ECH.WORD = 0x000000c0u;
            TSIP.REG_E0H.WORD = 0x8090000au;
            TSIP.REG_00H.WORD = 0x00008343u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x0000b560u;
            TSIP.REG_ECH.WORD = 0x00000006u;
            TSIP.REG_B0H.WORD = 0x00000300u;
            TSIP.REG_A4H.WORD = 0x42e0873fu;
            TSIP.REG_00H.WORD = 0x00001343u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_104H.WORD = 0x00000051u;
            TSIP.REG_A4H.WORD = 0x00000c84u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_A4H.WORD = 0x400009cdu;
            TSIP.REG_00H.WORD = 0x00001113u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x38008a20u;
            TSIP.REG_ECH.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            RX65NHU_func100(change_endian_long(0x4865c020u), change_endian_long(0xdfa7853cu), change_endian_long(0xbc237b67u), change_endian_long(0x97b29ee6u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_7CH.WORD = 0x00000021u;
                TSIP.REG_00H.WORD = 0x00005113u;
                TSIP.REG_74H.WORD = 0x00000004u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                RX65NHU_func101(change_endian_long(0x966a0467u), change_endian_long(0x3e3bca51u), change_endian_long(0xb476d046u), change_endian_long(0x3bc3201du));
            }
            else
            {
                TSIP.REG_7CH.WORD = 0x00000041u;
                TSIP.REG_00H.WORD = 0x00005113u;
                TSIP.REG_74H.WORD = 0x00000004u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                RX65NHU_func101(change_endian_long(0xb531c70du), change_endian_long(0x93b9f252u), change_endian_long(0xad277cc4u), change_endian_long(0x9d7ef349u));
            }
            RX65NHU_func100(change_endian_long(0xaf54919cu), change_endian_long(0xfe365dadu), change_endian_long(0xf923eb06u), change_endian_long(0x06806eb5u));
            TSIP.REG_24H.WORD = 0x000011c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x0000c0c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_ECH.WORD = 0x00000929u;
            TSIP.REG_ECH.WORD = 0x0000b4c0u;
            TSIP.REG_ECH.WORD = 0x000000fcu;
            TSIP.REG_ECH.WORD = 0x00003906u;
            TSIP.REG_ECH.WORD = 0x00008d00u;
            TSIP.REG_ECH.WORD = 0xfffffffeu;
            TSIP.REG_ECH.WORD = 0x00003d06u;
            TSIP.REG_ECH.WORD = 0x00000908u;
            for(iLoop = 0; iLoop < 32; iLoop = iLoop + 1)
            {
                TSIP.REG_ECH.WORD = 0x000038e6u;
                TSIP.REG_ECH.WORD = 0x0000a8c0u;
                TSIP.REG_ECH.WORD = 0x00000004u;
                for(jLoop = 0; jLoop < 32; jLoop = jLoop + 1)
                {
                    TSIP.REG_ECH.WORD = 0x38008900u;
                    TSIP.REG_ECH.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x11816907u;
                    TSIP.REG_ECH.WORD = 0x38008900u;
                    TSIP.REG_ECH.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x10002d20u;
                    TSIP.REG_ECH.WORD = 0x000168e7u;
                }
            }
            TSIP.REG_ECH.WORD = 0x00003549u;
            TSIP.REG_ECH.WORD = 0x0000a540u;
            TSIP.REG_ECH.WORD = 0x00000003u;
            TSIP.REG_ECH.WORD = 0x0002694au;
            TSIP.REG_E0H.WORD = 0x81010140u;
            TSIP.REG_04H.WORD = 0x00000606u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
            TSIP.REG_ECH.WORD = 0x000037eau;
            TSIP.REG_24H.WORD = 0x000011c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            for(iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 1)
            {
                TSIP.REG_24H.WORD = 0x00000185u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000185u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000185u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000185u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_ECH.WORD = 0x000033e0u;
                RX65NHU_func101(change_endian_long(0x635f6b36u), change_endian_long(0x7e14002du), change_endian_long(0x5a0baadau), change_endian_long(0xf2308331u));
            }
            TSIP.REG_ECH.WORD = 0x00007c1fu;
            TSIP.REG_1CH.WORD = 0x00602000u;
            RX65NHU_func100(change_endian_long(0x42baaea5u), change_endian_long(0xf3691c97u), change_endian_long(0x30d29e44u), change_endian_long(0xdae0607du));
            TSIP.REG_ECH.WORD = 0x00026d4au;
            TSIP.REG_ECH.WORD = 0x00002949u;
            TSIP.REG_E0H.WORD = 0x81010140u;
            TSIP.REG_04H.WORD = 0x00000606u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
            TSIP.REG_ECH.WORD = 0x000037eau;
            for(iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 1)
            {
                TSIP.REG_24H.WORD = 0x2000018du;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_ECH.WORD = 0x000033e0u;
                RX65NHU_func101(change_endian_long(0x8837ac84u), change_endian_long(0x759a24aeu), change_endian_long(0xdb3909afu), change_endian_long(0xcb8ada19u));
            }
            TSIP.REG_ECH.WORD = 0x00007c1fu;
            TSIP.REG_1CH.WORD = 0x00602000u;
            RX65NHU_func100(change_endian_long(0x9636d498u), change_endian_long(0x9d2c46b9u), change_endian_long(0x3e0321afu), change_endian_long(0xc4c94695u));
            TSIP.REG_24H.WORD = 0x0000a0c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_ECH.WORD = 0x00000a52u;
            TSIP.REG_24H.WORD = 0x000084d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x00029008u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_34H.WORD = 0x00000800u;
            TSIP.REG_24H.WORD = 0x8000c0c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x00006404u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x00006c04u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_E0H.WORD = 0x81010160u;
            TSIP.REG_04H.WORD = 0x00000606u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
            for(iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 1)
            {
                TSIP.REG_2CH.WORD = 0x00000010u;
                for(jLoop = 0; jLoop < 16; jLoop = jLoop + 4)
                {
                    RX65NHU_func100(change_endian_long(0xecd219ffu), change_endian_long(0x0af15646u), change_endian_long(0x7e1c00b9u), change_endian_long(0xb9b568cau));
                    RX65NHU_func103();
                    TSIP.REG_104H.WORD = 0x00000052u;
                    TSIP.REG_C4H.WORD = 0x01000c84u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_00H.WORD = 0x00003213u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    RX65NHU_func101(change_endian_long(0x49d24224u), change_endian_long(0xb896c4a4u), change_endian_long(0x8649bb07u), change_endian_long(0x1927a5c0u));
                }
                RX65NHU_func100(change_endian_long(0xd9910ec1u), change_endian_long(0xbd2140bfu), change_endian_long(0xe9194f1au), change_endian_long(0xd1de905fu));
                TSIP.REG_24H.WORD = 0x000019c0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000591u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000591u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000591u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000591u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x0000c0c1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x000050d0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x000084d0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00029008u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_34H.WORD = 0x00000800u;
                TSIP.REG_24H.WORD = 0x8000c0c1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x000088d0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00009008u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x000011c0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000581u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000581u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x000080c1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x000019c0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000581u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000581u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000581u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000581u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x0000c0c1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x000050d0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x000084d0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00029008u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_34H.WORD = 0x00000800u;
                TSIP.REG_24H.WORD = 0x8000c0c1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x000088d0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000dc0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x000040c1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_18H.WORD = 0x00000004u;
                TSIP.REG_38H.WORD = 0x00000c40u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_18H.BIT.B10)
                {
                    /* waiting */
                }
                TSIP.REG_18H.WORD = 0x00000000u;
                TSIP.REG_24H.WORD = 0x000005c0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00001191u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00210000u;
                TSIP.REG_24H.WORD = 0x000005c0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x000040c1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x000011c0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000991u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00210000u;
                RX65NHU_func100(change_endian_long(0xeddf40d5u), change_endian_long(0xb157a452u), change_endian_long(0x6aa69a5du), change_endian_long(0x2fb13e32u));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (0u == (TSIP.REG_1CH.BIT.B22))
                {
                    TSIP.REG_ECH.WORD = 0x00002e40u;
                    RX65NHU_func101(change_endian_long(0xb0870fb1u), change_endian_long(0x29e4dc89u), change_endian_long(0x7df3b864u), change_endian_long(0x7aee2c31u));
                }
                else
                {
                    RX65NHU_func100(change_endian_long(0x4e4b023au), change_endian_long(0x9f69fcb1u), change_endian_long(0xd53dedb8u), change_endian_long(0xd65dc770u));
                    TSIP.REG_24H.WORD = 0x000019c0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_24H.WORD = 0x00000591u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_24H.WORD = 0x000040c1u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_24H.WORD = 0x00006404u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_24H.WORD = 0x00006c04u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_E0H.WORD = 0x81010120u;
                    TSIP.REG_04H.WORD = 0x00000606u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    S_RAM[0+1 + 0] = change_endian_long(TSIP.REG_100H.WORD);
                    TSIP.REG_ECH.WORD = 0x000037e9u;
                    for(jLoop = 0; jLoop < S_RAM[0+1]; jLoop = jLoop + 1)
                    {
                        TSIP.REG_24H.WORD = 0x000009c0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x00001191u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00210000u;
                        TSIP.REG_24H.WORD = 0x000011c0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x00000991u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00210000u;
                        RX65NHU_func100(change_endian_long(0x90d988b6u), change_endian_long(0x82d46554u), change_endian_long(0xa115a8fcu), change_endian_long(0xa8c4034au));
                        TSIP.REG_1CH.WORD = 0x00400000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (0u == (TSIP.REG_1CH.BIT.B22))
                        {
                            TSIP.REG_ECH.WORD = 0x00002e40u;
                            RX65NHU_func101(change_endian_long(0xd6014aecu), change_endian_long(0x6b6d9773u), change_endian_long(0x40b09d20u), change_endian_long(0xdd0eba76u));
                            break;
                        }
                        else
                        {
                            TSIP.REG_24H.WORD = 0x00004c0cu;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_24H.BIT.B21)
                            {
                                /* waiting */
                            }
                            TSIP.REG_24H.WORD = 0x0000880cu;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_24H.BIT.B21)
                            {
                                /* waiting */
                            }
                            TSIP.REG_24H.WORD = 0x000019c0u;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_24H.BIT.B21)
                            {
                                /* waiting */
                            }
                            TSIP.REG_24H.WORD = 0x00000591u;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_24H.BIT.B21)
                            {
                                /* waiting */
                            }
                            TSIP.REG_24H.WORD = 0x000040c1u;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_24H.BIT.B21)
                            {
                                /* waiting */
                            }
                            RX65NHU_func101(change_endian_long(0x4f7c6067u), change_endian_long(0x6389c71du), change_endian_long(0xe9932a7eu), change_endian_long(0xe5daa2f4u));
                        }
                    }
                    TSIP.REG_ECH.WORD = 0x38008a40u;
                    TSIP.REG_ECH.WORD = 0x00000000u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00260000u;
                    RX65NHU_func100(change_endian_long(0x61e246e6u), change_endian_long(0x53ad0d06u), change_endian_long(0x214caaf0u), change_endian_long(0x6f9637c0u));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX65NHU_func101(change_endian_long(0x4e815fbeu), change_endian_long(0x50640071u), change_endian_long(0xdb7788f7u), change_endian_long(0x9e92587fu));
                        break;
                    }
                    else
                    {
                        RX65NHU_func101(change_endian_long(0x7b299b7eu), change_endian_long(0x2a00ba25u), change_endian_long(0xa3f09f5du), change_endian_long(0xcc5a1c5du));
                    }
                }
            }
            TSIP.REG_ECH.WORD = 0x38000a4bu;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            RX65NHU_func100(change_endian_long(0xbd3a97a0u), change_endian_long(0x64fa6342u), change_endian_long(0x553de1a9u), change_endian_long(0xacdc4976u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_ECH.WORD = 0x00002e20u;
                TSIP.REG_ECH.WORD = 0x38008a20u;
                TSIP.REG_ECH.WORD = 0x00000002u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                RX65NHU_func100(change_endian_long(0xb7056ea2u), change_endian_long(0x3959746du), change_endian_long(0x2d4a817eu), change_endian_long(0x08d72561u));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    RX65NHU_func101(change_endian_long(0x9a78893au), change_endian_long(0xaf1632dcu), change_endian_long(0x0ec56622u), change_endian_long(0x660e349au));
                    break;
                }
                else
                {
                    TSIP.REG_24H.WORD = 0x000019c0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_24H.WORD = 0x0000e0c1u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    RX65NHU_func101(change_endian_long(0x4a2da3cau), change_endian_long(0x04729646u), change_endian_long(0x0893e298u), change_endian_long(0x5640d8a7u));
                }
            }
            else
            {
                RX65NHU_func101(change_endian_long(0x9ee64cdcu), change_endian_long(0xfff8f00bu), change_endian_long(0x8b44e718u), change_endian_long(0xb16e98e1u));
            }
        }
    }
    TSIP.REG_ECH.WORD = 0x38008a20u;
    TSIP.REG_ECH.WORD = 0x00000002u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX65NHU_func100(change_endian_long(0x846d8832u), change_endian_long(0xee9b2a1fu), change_endian_long(0x72defde0u), change_endian_long(0xec053deau));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0xd707efc9u), change_endian_long(0xc4fceb8bu), change_endian_long(0x2c12244eu), change_endian_long(0x3b546b0du));
        TSIP.REG_1BCH.WORD = 0x00000040u;
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
    RX65NHU_func100(change_endian_long(0xd8f3072du), change_endian_long(0x391da8f0u), change_endian_long(0x03b6a59fu), change_endian_long(0x95d3e933u));
    TSIP.REG_ECH.WORD = 0x0000b5c0u;
    TSIP.REG_ECH.WORD = 0x00010001u;
    TSIP.REG_24H.WORD = 0x000019c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00000591u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000080c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00001dc0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000800u;
    TSIP.REG_24H.WORD = 0x8000c0c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000020u;
    TSIP.REG_24H.WORD = 0x800019c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00000591u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000040c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_D0H.WORD = 0x00000700u;
    TSIP.REG_C4H.WORD = 0x42e087bfu;
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    TSIP.REG_04H.WORD = 0x00000282u;
    for (iLoop = 0; iLoop < 32; iLoop = iLoop + 4)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[iLoop + 0] = TSIP.REG_100H.WORD;
        S_HEAP[iLoop + 1] = TSIP.REG_100H.WORD;
        S_HEAP[iLoop + 2] = TSIP.REG_100H.WORD;
        S_HEAP[iLoop + 3] = TSIP.REG_100H.WORD;
    }
    RX65NHU_func100(change_endian_long(0x7727a13eu), change_endian_long(0xcb5bb30fu), change_endian_long(0x72c4f66eu), change_endian_long(0xb551338bu));
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x00000c84u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_C4H.WORD = 0x400009cdu;
    TSIP.REG_00H.WORD = 0x00002213u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_04H.WORD = 0x00000212u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    S_HEAP[iLoop + 0] = TSIP.REG_100H.WORD;
    S_HEAP[iLoop + 1] = TSIP.REG_100H.WORD;
    S_HEAP[iLoop + 2] = TSIP.REG_100H.WORD;
    S_HEAP[iLoop + 3] = TSIP.REG_100H.WORD;
    TSIP.REG_28H.WORD = 0x008f0001u;
    TSIP.REG_24H.WORD = 0x0000b0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000c8d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000e8d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    /* WAIT_LOOP */
    while(1)
    {
        TSIP.REG_24H.WORD = 0x00008006u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x000011c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00000591u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00210000u;
        RX65NHU_func100(change_endian_long(0x70f21179u), change_endian_long(0xeec60eccu), change_endian_long(0x65377054u), change_endian_long(0x30bc6397u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func101(change_endian_long(0x8f4a9160u), change_endian_long(0xa21d5053u), change_endian_long(0xb22e27b9u), change_endian_long(0xcba4924eu));
            break;
        }
        else
        {
            TSIP.REG_24H.WORD = 0x000019c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x0000d0d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x000080c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            RX65NHU_func101(change_endian_long(0xc43611d0u), change_endian_long(0xedbf351eu), change_endian_long(0x4c7d7741u), change_endian_long(0xcdbf5c5cu));
        }
    }
    TSIP.REG_24H.WORD = 0x000094d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x0000c002u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000dcd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_34H.WORD = 0x00000800u;
    TSIP.REG_24H.WORD = 0x8000980au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_00H.WORD = 0x0000307fu;
    TSIP.REG_2CH.WORD = 0x00000014u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_104H.WORD = 0x00000057u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00010001u);
    TSIP.REG_2CH.WORD = 0x00000002u;
    TSIP.REG_24H.WORD = 0x0000c002u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_E0H.WORD = 0x800100c0u;
    TSIP.REG_00H.WORD = 0x0000031fu;
    TSIP.REG_2CH.WORD = 0x00000024u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_00H.WORD = 0x00008307u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_24H.WORD = 0x000011c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00000991u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000040c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000011c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000e0c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_ECH.WORD = 0x0000b500u;
    TSIP.REG_ECH.WORD = 0x00010001u;
    TSIP.REG_24H.WORD = 0x000005c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000080c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    /* WAIT_LOOP */
    while(1)
    {
        TSIP.REG_ECH.WORD = 0x0000094au;
        TSIP.REG_E0H.WORD = 0x80a0000au;
        TSIP.REG_00H.WORD = 0x00008383u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_B0H.WORD = 0x00000714u;
        TSIP.REG_A4H.WORD = 0x00008a07u;
        TSIP.REG_00H.WORD = 0x00001383u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x008f0001u;
        TSIP.REG_24H.WORD = 0x00001dc0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_34H.WORD = 0x00000800u;
        TSIP.REG_24H.WORD = 0x800080c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x000080d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x00870001u;
        TSIP.REG_00H.WORD = 0x0000301fu;
        TSIP.REG_2CH.WORD = 0x00000014u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_E0H.WORD = 0x810100c0u;
        TSIP.REG_00H.WORD = 0x00003807u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_00H.WORD = 0x0000301fu;
        TSIP.REG_2CH.WORD = 0x00000010u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_E0H.WORD = 0x81010100u;
        TSIP.REG_00H.WORD = 0x00003807u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_2CH.WORD = 0x00000000u;
        TSIP.REG_34H.WORD = 0x00000800u;
        TSIP.REG_24H.WORD = 0x8000c002u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_ECH.WORD = 0x00003506u;
        TSIP.REG_E0H.WORD = 0x800100c0u;
        TSIP.REG_00H.WORD = 0x0000031fu;
        TSIP.REG_2CH.WORD = 0x0000002cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_00H.WORD = 0x00008307u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_ECH.WORD = 0x380088c0u;
        TSIP.REG_ECH.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX65NHU_func100(change_endian_long(0xdb33270bu), change_endian_long(0xbdb132d1u), change_endian_long(0xfc7fb57bu), change_endian_long(0x9ea76b02u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_28H.WORD = 0x008f0001u;
            TSIP.REG_104H.WORD = 0x00001f61u;
            TSIP.REG_B0H.WORD = 0x00000700u;
            TSIP.REG_A4H.WORD = 0x42f087bfu;
            TSIP.REG_00H.WORD = 0x00003143u;
            TSIP.REG_2CH.WORD = 0x00000015u;
            for (iLoop = 0; iLoop < 16; iLoop = iLoop + 4)
            {
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = S_HEAP[iLoop + 0];
                TSIP.REG_100H.WORD = S_HEAP[iLoop + 1];
                TSIP.REG_100H.WORD = S_HEAP[iLoop + 2];
                TSIP.REG_100H.WORD = S_HEAP[iLoop + 3];
            }
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_00H.WORD = 0x00003143u;
            TSIP.REG_2CH.WORD = 0x00000014u;
            for (iLoop = 16; iLoop < 32; iLoop = iLoop + 4)
            {
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = S_HEAP[iLoop + 0];
                TSIP.REG_100H.WORD = S_HEAP[iLoop + 1];
                TSIP.REG_100H.WORD = S_HEAP[iLoop + 2];
                TSIP.REG_100H.WORD = S_HEAP[iLoop + 3];
            }
            RX65NHU_func101(change_endian_long(0x3067587eu), change_endian_long(0x246cfe7fu), change_endian_long(0x80603468u), change_endian_long(0x872abb8fu));
            break;
        }
        else
        {
            TSIP.REG_28H.WORD = 0x008f0001u;
            TSIP.REG_34H.WORD = 0x00000008u;
            TSIP.REG_24H.WORD = 0x800011c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x0000e0c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_00H.WORD = 0x00003043u;
            TSIP.REG_2CH.WORD = 0x00000018u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_28H.WORD = 0x009f0001u;
            TSIP.REG_24H.WORD = 0x0000890eu;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_D0H.WORD = 0x0000071cu;
            TSIP.REG_C4H.WORD = 0x00008a83u;
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_24H.WORD = 0x000009c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x00001191u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x00001de1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x000040c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_ECH.WORD = 0x0000094au;
            TSIP.REG_E0H.WORD = 0x81a0000au;
            TSIP.REG_00H.WORD = 0x00003883u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX65NHU_func101(change_endian_long(0xa958e29cu), change_endian_long(0xd402071au), change_endian_long(0x53a07e13u), change_endian_long(0xe258abb2u));
        }
    }
    TSIP.REG_ECH.WORD = 0x00007c06u;
    TSIP.REG_1CH.WORD = 0x00602000u;
    TSIP.REG_A4H.WORD = 0x400007bdu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_HEAP[iLoop + 0];
    TSIP.REG_100H.WORD = S_HEAP[iLoop + 1];
    TSIP.REG_100H.WORD = S_HEAP[iLoop + 2];
    TSIP.REG_100H.WORD = S_HEAP[iLoop + 3];
    TSIP.REG_A4H.WORD = 0x00800c45u;
    TSIP.REG_00H.WORD = 0x00001113u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX65NHU_func100(change_endian_long(0x147a61deu), change_endian_long(0xcac2db2du), change_endian_long(0xe8a69237u), change_endian_long(0x3c9eb8efu));
    TSIP.REG_34H.WORD = 0x00000802u;
    TSIP.REG_24H.WORD = 0x800088d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000088d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000b8d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_2CH.WORD = 0x00000002u;
    TSIP.REG_24H.WORD = 0x00007b0au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000c8d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000800u;
    TSIP.REG_24H.WORD = 0x8000ccd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x009f0001u;
    RX65NHU_func103();
    RX65NHU_func100(change_endian_long(0xc7fdfc97u), change_endian_long(0x69989c4eu), change_endian_long(0xc89aeadfu), change_endian_long(0xbdd28626u));
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
    TSIP.REG_E0H.WORD = 0x81040000u;
    TSIP.REG_04H.WORD = 0x00000612u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PubKeyIndex[0] = TSIP.REG_100H.WORD;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PubKeyIndex[1] = TSIP.REG_100H.WORD;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PubKeyIndex[2] = TSIP.REG_100H.WORD;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PubKeyIndex[3] = TSIP.REG_100H.WORD;
    RX65NHU_func100(change_endian_long(0x0a903c57u), change_endian_long(0x3974602bu), change_endian_long(0x13666f95u), change_endian_long(0xb4a85794u));
    TSIP.REG_C4H.WORD = 0x200c3b0du;
    TSIP.REG_E0H.WORD = 0x81040000u;
    TSIP.REG_00H.WORD = 0x00002813u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_C4H.WORD = 0x000c0b0cu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x7a241e7bu);
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x00040804u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    TSIP.REG_D0H.WORD = 0x00000700u;
    TSIP.REG_C4H.WORD = 0x02f0888fu;
    TSIP.REG_04H.WORD = 0x00000282u;
    for(iLoop=0; iLoop<32; iLoop=iLoop+4)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[iLoop+4 + 0] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[iLoop+4 + 1] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[iLoop+4 + 2] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[iLoop+4 + 3] = TSIP.REG_100H.WORD;
    }
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX65NHU_func100(change_endian_long(0x5ee0f9e0u), change_endian_long(0xab8dc1a1u), change_endian_long(0x360ab5f7u), change_endian_long(0x8883d866u));
    TSIP.REG_C4H.WORD = 0x00f0088cu;
    TSIP.REG_E0H.WORD = 0x810101c0u;
    TSIP.REG_00H.WORD = 0x00002807u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_04H.WORD = 0x00000212u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PubKeyIndex[36] = TSIP.REG_100H.WORD;
    OutData_PubKeyIndex[37] = TSIP.REG_100H.WORD;
    OutData_PubKeyIndex[38] = TSIP.REG_100H.WORD;
    OutData_PubKeyIndex[39] = TSIP.REG_100H.WORD;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_C8H.BIT.B6)
    {
        /* waiting */
    }
    RX65NHU_func100(change_endian_long(0xff5c95cbu), change_endian_long(0x2d4143d1u), change_endian_long(0xd0367093u), change_endian_long(0x0ee4b255u));
    TSIP.REG_18H.WORD = 0x00000004u;
    TSIP.REG_24H.WORD = 0x00004404u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_18H.BIT.B10)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00004804u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_18H.BIT.B10)
    {
        /* waiting */
    }
    TSIP.REG_18H.WORD = 0x00000000u;
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    TSIP.REG_D0H.WORD = 0x00000700u;
    TSIP.REG_C4H.WORD = 0x00e087bfu;
    TSIP.REG_04H.WORD = 0x00000282u;
    for(iLoop=36; iLoop<68; iLoop=iLoop+4)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[iLoop+4 + 0] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[iLoop+4 + 1] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[iLoop+4 + 2] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[iLoop+4 + 3] = TSIP.REG_100H.WORD;
    }
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX65NHU_func100(change_endian_long(0xfa32287du), change_endian_long(0xe8bc6fe8u), change_endian_long(0x0dc3a3b0u), change_endian_long(0xf40f58ebu));
    TSIP.REG_104H.WORD = 0x00000352u;
    TSIP.REG_C4H.WORD = 0x000009cdu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000008u);
    TSIP.REG_04H.WORD = 0x00000212u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PubKeyIndex[72] = TSIP.REG_100H.WORD;
    OutData_PubKeyIndex[73] = TSIP.REG_100H.WORD;
    OutData_PubKeyIndex[74] = TSIP.REG_100H.WORD;
    OutData_PubKeyIndex[75] = TSIP.REG_100H.WORD;
    RX65NHU_func100(change_endian_long(0x574205b8u), change_endian_long(0x3d637458u), change_endian_long(0x8822835fu), change_endian_long(0xfe89ac71u));
    RX65NHU_func103();
    RX65NHU_func100(change_endian_long(0x4719d269u), change_endian_long(0xe84ccd5bu), change_endian_long(0x561115b3u), change_endian_long(0x89906d02u));
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x01000c84u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_E0H.WORD = 0x800401e0u;
    TSIP.REG_00H.WORD = 0x00008213u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_E0H.WORD = 0x810401e0u;
    TSIP.REG_04H.WORD = 0x00000612u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PrivKeyIndex[0] = TSIP.REG_100H.WORD;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PrivKeyIndex[1] = TSIP.REG_100H.WORD;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PrivKeyIndex[2] = TSIP.REG_100H.WORD;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PrivKeyIndex[3] = TSIP.REG_100H.WORD;
    RX65NHU_func100(change_endian_long(0xa4e3b38fu), change_endian_long(0x095070e3u), change_endian_long(0xa965f4dbu), change_endian_long(0x554e9fbbu));
    TSIP.REG_C4H.WORD = 0x200c3b0du;
    TSIP.REG_E0H.WORD = 0x810401e0u;
    TSIP.REG_00H.WORD = 0x00002813u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_C4H.WORD = 0x000c0b0cu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x9785ce17u);
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    TSIP.REG_D0H.WORD = 0x00000700u;
    TSIP.REG_C4H.WORD = 0x02e087bfu;
    TSIP.REG_04H.WORD = 0x00000282u;
    for(iLoop=0; iLoop<32; iLoop=iLoop+4)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PrivKeyIndex[iLoop+4 + 0] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[iLoop+4 + 1] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[iLoop+4 + 2] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[iLoop+4 + 3] = TSIP.REG_100H.WORD;
    }
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX65NHU_func100(change_endian_long(0xfeddeb99u), change_endian_long(0x5e0ea12fu), change_endian_long(0xac73046bu), change_endian_long(0x88a18b15u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    TSIP.REG_D0H.WORD = 0x00000700u;
    TSIP.REG_C4H.WORD = 0x00e087bfu;
    TSIP.REG_04H.WORD = 0x00000282u;
    for(iLoop=32; iLoop<64; iLoop=iLoop+4)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PrivKeyIndex[iLoop+4 + 0] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[iLoop+4 + 1] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[iLoop+4 + 2] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[iLoop+4 + 3] = TSIP.REG_100H.WORD;
    }
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX65NHU_func100(change_endian_long(0x40de5981u), change_endian_long(0xb14cbd91u), change_endian_long(0x62f3bc6du), change_endian_long(0xb836e845u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    TSIP.REG_D0H.WORD = 0x00000700u;
    TSIP.REG_C4H.WORD = 0x00e087bfu;
    TSIP.REG_04H.WORD = 0x00000282u;
    for(iLoop=64; iLoop<96; iLoop=iLoop+4)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PrivKeyIndex[iLoop+4 + 0] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[iLoop+4 + 1] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[iLoop+4 + 2] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[iLoop+4 + 3] = TSIP.REG_100H.WORD;
    }
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX65NHU_func100(change_endian_long(0x6a04f63bu), change_endian_long(0x6d419377u), change_endian_long(0xa3959b0fu), change_endian_long(0xd53b52cbu));
    TSIP.REG_104H.WORD = 0x00000352u;
    TSIP.REG_C4H.WORD = 0x000009cdu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000018u);
    TSIP.REG_04H.WORD = 0x00000212u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PrivKeyIndex[100] = TSIP.REG_100H.WORD;
    OutData_PrivKeyIndex[101] = TSIP.REG_100H.WORD;
    OutData_PrivKeyIndex[102] = TSIP.REG_100H.WORD;
    OutData_PrivKeyIndex[103] = TSIP.REG_100H.WORD;
    RX65NHU_func102(change_endian_long(0x00366092u), change_endian_long(0x5a6225f5u), change_endian_long(0x90b6445du), change_endian_long(0x722c1904u));
    TSIP.REG_1BCH.WORD = 0x00000040u;
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
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_p2a_r3.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_PRV_USE_RSA_1024 == 1 */
