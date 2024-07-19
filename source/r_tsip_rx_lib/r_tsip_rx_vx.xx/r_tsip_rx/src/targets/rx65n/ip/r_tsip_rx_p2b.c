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

#if (TSIP_PRV_USE_RSA_2048 == 1) || (TSIP_TLS == 1)
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
* Function Name: R_TSIP_GenerateRsa2048RandomKeyIndexSub
*******************************************************************************************************************/ /**
* @details       RX65NHU RSA-2048 Key Generation
* @param[out]    OutData_PubKeyIndex
* @param[out]    OutData_PrivKeyIndex
* @retval        TSIP_SUCCESS
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRsa2048RandomKeyIndexSub(uint32_t MAX_CNT, uint32_t *OutData_PubKeyIndex, uint32_t *OutData_PrivKeyIndex)
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
    TSIP.REG_84H.WORD = 0x00002b01u;
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
    RX65NHU_func100(change_endian_long(0x624a6783u), change_endian_long(0xcc566610u), change_endian_long(0xcae06f3cu), change_endian_long(0x4514e7bfu));
    RX65NHU_func103();
    RX65NHU_func100(change_endian_long(0xb5d2c795u), change_endian_long(0x1cc705aeu), change_endian_long(0x7ce232dbu), change_endian_long(0x8a7b26c1u));
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
        TSIP.REG_28H.WORD = 0x009f0001u;
        RX65NHU_func100(change_endian_long(0xb1ea9804u), change_endian_long(0xe296dc14u), change_endian_long(0x26dd08a6u), change_endian_long(0x002914e4u));
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
        for(iLoop = 0; iLoop < 24; iLoop = iLoop + 4)
        {
            RX65NHU_func100(change_endian_long(0x98458c73u), change_endian_long(0x9ce44624u), change_endian_long(0xa2008892u), change_endian_long(0xbc7457b2u));
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
            RX65NHU_func101(change_endian_long(0x746a70f0u), change_endian_long(0x26717dd5u), change_endian_long(0x0bc12337u), change_endian_long(0xef0d5b24u));
        }
        RX65NHU_func100(change_endian_long(0x98458c73u), change_endian_long(0x9ce44624u), change_endian_long(0xa2008892u), change_endian_long(0xbc7457b2u));
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
        TSIP.REG_104H.WORD = 0x00001f57u;
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
        TSIP.REG_100H.WORD = change_endian_long(0x851EB851u);
        TSIP.REG_100H.WORD = change_endian_long(0xEB851EB8u);
        TSIP.REG_100H.WORD = change_endian_long(0x51EB851Eu);
        TSIP.REG_100H.WORD = change_endian_long(0xB851EB85u);
        TSIP.REG_100H.WORD = change_endian_long(0x1EB851EBu);
        TSIP.REG_100H.WORD = change_endian_long(0x851EB851u);
        TSIP.REG_100H.WORD = change_endian_long(0xEB851B5Cu);
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
        RX65NHU_func100(change_endian_long(0xa222be64u), change_endian_long(0xd14ebd52u), change_endian_long(0x90e99026u), change_endian_long(0xd4ef6c58u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func101(change_endian_long(0x020cb126u), change_endian_long(0x2015a3e7u), change_endian_long(0xe84d22aau), change_endian_long(0xa4451637u));
            continue;
        }
        else
        {
            RX65NHU_func101(change_endian_long(0xe250d016u), change_endian_long(0x884a1df2u), change_endian_long(0xfa0311e7u), change_endian_long(0x67cebfd4u));
        }
        TSIP.REG_ECH.WORD = 0x38008a20u;
        TSIP.REG_ECH.WORD = 0x00000001u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX65NHU_func100(change_endian_long(0xdd6c4976u), change_endian_long(0xc4664f21u), change_endian_long(0x26b0fac6u), change_endian_long(0xeddea25cu));
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
            RX65NHU_func100(change_endian_long(0xb90a2044u), change_endian_long(0xc3ba151cu), change_endian_long(0xac6f7219u), change_endian_long(0x918449f3u));
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
                RX65NHU_func101(change_endian_long(0x18b2eb26u), change_endian_long(0x2944817du), change_endian_long(0x69663dc1u), change_endian_long(0xc6ddd7dcu));
            }
            else
            {
                RX65NHU_func101(change_endian_long(0x360ca22eu), change_endian_long(0xb715fb62u), change_endian_long(0xf78b8dbeu), change_endian_long(0x921fd23fu));
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
            TSIP.REG_00H.WORD = 0x00003073u;
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
            RX65NHU_func100(change_endian_long(0x8203e5eau), change_endian_long(0x27e84643u), change_endian_long(0x8affddbeu), change_endian_long(0x7039740du));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX65NHU_func101(change_endian_long(0x9250207du), change_endian_long(0xaddeb813u), change_endian_long(0x2125205au), change_endian_long(0x29d5147au));
                continue;
            }
            else
            {
                RX65NHU_func101(change_endian_long(0x5a285f5du), change_endian_long(0x31e70789u), change_endian_long(0xd07de888u), change_endian_long(0x61f59adau));
            }
        }
        else
        {
            RX65NHU_func101(change_endian_long(0x3a15fe84u), change_endian_long(0xa35e7a29u), change_endian_long(0x8a136328u), change_endian_long(0x1e945f38u));
        }
        TSIP.REG_ECH.WORD = 0x000008c6u;
        TSIP.REG_ECH.WORD = 0x0000094au;
        for(iLoop = 0; iLoop < 96; iLoop = iLoop + 1)
        {
            TSIP.REG_ECH.WORD = 0x01003906u;
            TSIP.REG_ECH.WORD = 0x00002cc0u;
            TSIP.REG_ECH.WORD = 0x00002cc0u;
            TSIP.REG_E0H.WORD = 0x81010100u;
            TSIP.REG_00H.WORD = 0x0000307fu;
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
            RX65NHU_func100(change_endian_long(0x97bb2cc2u), change_endian_long(0xcdfceafcu), change_endian_long(0xdffc3e8eu), change_endian_long(0x29049a3du));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_ECH.WORD = 0x0000d140u;
                RX65NHU_func101(change_endian_long(0xf0000a6bu), change_endian_long(0x5488780fu), change_endian_long(0x3bce9316u), change_endian_long(0xfcde85c9u));
                break;
            }
            else
            {
                RX65NHU_func101(change_endian_long(0x799f679du), change_endian_long(0xefcc22b3u), change_endian_long(0xe7e2a82bu), change_endian_long(0xa1f10f0au));
            }
        }
        TSIP.REG_ECH.WORD = 0x38008940u;
        TSIP.REG_ECH.WORD = 0x00000001u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX65NHU_func100(change_endian_long(0x6a2e5087u), change_endian_long(0xbe4b4a6cu), change_endian_long(0x993680e9u), change_endian_long(0x4f83bc9eu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func101(change_endian_long(0x0154c560u), change_endian_long(0x6b84f832u), change_endian_long(0x7e29bc3eu), change_endian_long(0x4f3cf257u));
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
        RX65NHU_func100(change_endian_long(0x67a6c989u), change_endian_long(0x7624a2e4u), change_endian_long(0x3506dbd6u), change_endian_long(0x17544039u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func101(change_endian_long(0xcbf8fa67u), change_endian_long(0x1cdb03ceu), change_endian_long(0xa3ae573fu), change_endian_long(0x8743396bu));
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
            RX65NHU_func100(change_endian_long(0xa928b4ecu), change_endian_long(0x59524ab5u), change_endian_long(0xa1c1ae25u), change_endian_long(0x829995b7u));
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
            RX65NHU_func100(change_endian_long(0x2cf29d2eu), change_endian_long(0x5243679du), change_endian_long(0xecb95cd7u), change_endian_long(0x398b2fabu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX65NHU_func101(change_endian_long(0x989eae76u), change_endian_long(0xe7a72cd0u), change_endian_long(0xe333dab5u), change_endian_long(0xc1f19ea0u));
                continue;
            }
            TSIP.REG_24H.WORD = 0x000098d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_ECH.WORD = 0x0000b540u;
            TSIP.REG_ECH.WORD = 0x00000080u;
            TSIP.REG_E0H.WORD = 0x80a0000au;
            TSIP.REG_00H.WORD = 0x00008383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x0000b560u;
            TSIP.REG_ECH.WORD = 0x00000003u;
            TSIP.REG_B0H.WORD = 0x00000700u;
            TSIP.REG_A4H.WORD = 0x42e0873fu;
            TSIP.REG_00H.WORD = 0x00001383u;
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
            RX65NHU_func100(change_endian_long(0xcb83b158u), change_endian_long(0xec568065u), change_endian_long(0x1d1d49e0u), change_endian_long(0xf031f086u));
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
                RX65NHU_func101(change_endian_long(0x09427b80u), change_endian_long(0x1953d055u), change_endian_long(0xd1669eccu), change_endian_long(0xbbf81d28u));
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
                RX65NHU_func101(change_endian_long(0x52784eeeu), change_endian_long(0x0559478cu), change_endian_long(0xf55124e7u), change_endian_long(0x82ed8bf6u));
            }
            RX65NHU_func100(change_endian_long(0x3f3cb137u), change_endian_long(0x91317ffeu), change_endian_long(0xb7f24451u), change_endian_long(0x07a8210bu));
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
                RX65NHU_func101(change_endian_long(0x2944c2bfu), change_endian_long(0x7358aab0u), change_endian_long(0x6d6ab1a0u), change_endian_long(0xb0568283u));
            }
            TSIP.REG_ECH.WORD = 0x00007c1fu;
            TSIP.REG_1CH.WORD = 0x00602000u;
            RX65NHU_func100(change_endian_long(0xb69b9515u), change_endian_long(0x6abc1799u), change_endian_long(0x3d1e825cu), change_endian_long(0x95bcb0a7u));
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
                RX65NHU_func101(change_endian_long(0x2097c536u), change_endian_long(0xbe631de9u), change_endian_long(0x854da7c5u), change_endian_long(0x4863e52au));
            }
            TSIP.REG_ECH.WORD = 0x00007c1fu;
            TSIP.REG_1CH.WORD = 0x00602000u;
            RX65NHU_func100(change_endian_long(0x8451ba92u), change_endian_long(0xe0e64065u), change_endian_long(0x6a719afcu), change_endian_long(0xaacba266u));
            TSIP.REG_24H.WORD = 0x0000a0c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_ECH.WORD = 0x00000a52u;
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
                for(jLoop = 0; jLoop < 32; jLoop = jLoop + 4)
                {
                    RX65NHU_func100(change_endian_long(0xd6e7ca77u), change_endian_long(0x667df516u), change_endian_long(0x76b0be35u), change_endian_long(0x7fbff67bu));
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
                    RX65NHU_func101(change_endian_long(0x0e66e881u), change_endian_long(0xee43cccau), change_endian_long(0x4d04f7e9u), change_endian_long(0x12b9a692u));
                }
                RX65NHU_func100(change_endian_long(0x7ef461d7u), change_endian_long(0x02753b86u), change_endian_long(0x5f88893fu), change_endian_long(0xdb0f32f2u));
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
                RX65NHU_func100(change_endian_long(0xa1633574u), change_endian_long(0xbd30754eu), change_endian_long(0x593e4002u), change_endian_long(0x09f5ea33u));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (0u == (TSIP.REG_1CH.BIT.B22))
                {
                    TSIP.REG_ECH.WORD = 0x00002e40u;
                    RX65NHU_func101(change_endian_long(0x82a4b26fu), change_endian_long(0x2e2a557du), change_endian_long(0x61186582u), change_endian_long(0xa880b5abu));
                }
                else
                {
                    RX65NHU_func100(change_endian_long(0x329ad149u), change_endian_long(0x251a0e32u), change_endian_long(0x5cedfa52u), change_endian_long(0x24cbdbfbu));
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
                        RX65NHU_func100(change_endian_long(0x59079e1au), change_endian_long(0x2837b05bu), change_endian_long(0x88270895u), change_endian_long(0x74efde4eu));
                        TSIP.REG_1CH.WORD = 0x00400000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (0u == (TSIP.REG_1CH.BIT.B22))
                        {
                            TSIP.REG_ECH.WORD = 0x00002e40u;
                            RX65NHU_func101(change_endian_long(0xa485ae77u), change_endian_long(0x526e924cu), change_endian_long(0x66ab824fu), change_endian_long(0x4b9a4b5eu));
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
                            RX65NHU_func101(change_endian_long(0xdefedbd6u), change_endian_long(0xcc76c14cu), change_endian_long(0x8505b8a4u), change_endian_long(0x160c0e68u));
                        }
                    }
                    TSIP.REG_ECH.WORD = 0x38008a40u;
                    TSIP.REG_ECH.WORD = 0x00000000u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00260000u;
                    RX65NHU_func100(change_endian_long(0xb3146825u), change_endian_long(0xfee9f51cu), change_endian_long(0x88242155u), change_endian_long(0xb13b0969u));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX65NHU_func101(change_endian_long(0x2a83e8dcu), change_endian_long(0x87acee2eu), change_endian_long(0x82129069u), change_endian_long(0x3ac52622u));
                        break;
                    }
                    else
                    {
                        RX65NHU_func101(change_endian_long(0x58fc5b06u), change_endian_long(0xe135100fu), change_endian_long(0x4986a1f3u), change_endian_long(0x114e925eu));
                    }
                }
            }
            TSIP.REG_ECH.WORD = 0x38000a4bu;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            RX65NHU_func100(change_endian_long(0x76b684a1u), change_endian_long(0x7c589c79u), change_endian_long(0x008c257bu), change_endian_long(0x31a9a448u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_ECH.WORD = 0x00002e20u;
                TSIP.REG_ECH.WORD = 0x38008a20u;
                TSIP.REG_ECH.WORD = 0x00000002u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                RX65NHU_func100(change_endian_long(0xc51bdedbu), change_endian_long(0x6cc096b7u), change_endian_long(0x28fa2502u), change_endian_long(0x8f17045eu));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    RX65NHU_func101(change_endian_long(0x0c9730ccu), change_endian_long(0x70accf99u), change_endian_long(0xbb11743cu), change_endian_long(0x96897b71u));
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
                    RX65NHU_func101(change_endian_long(0xc97f0517u), change_endian_long(0x64c73eceu), change_endian_long(0xcc39cdcbu), change_endian_long(0x203e076du));
                }
            }
            else
            {
                RX65NHU_func101(change_endian_long(0x3693ca1eu), change_endian_long(0xfd191a45u), change_endian_long(0x8b5cb304u), change_endian_long(0x6f006219u));
            }
        }
    }
    TSIP.REG_ECH.WORD = 0x38008a20u;
    TSIP.REG_ECH.WORD = 0x00000002u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX65NHU_func100(change_endian_long(0x80206fdfu), change_endian_long(0x3e5df8c2u), change_endian_long(0x8bfb988cu), change_endian_long(0xf0aafe08u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0x0a79e745u), change_endian_long(0xefce132eu), change_endian_long(0xdba36a79u), change_endian_long(0x1406f7f5u));
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
    RX65NHU_func100(change_endian_long(0x879f31cau), change_endian_long(0x2bf51cb8u), change_endian_long(0x243c7de7u), change_endian_long(0xace5d21fu));
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
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_D0H.WORD = 0x00000f00u;
    TSIP.REG_C4H.WORD = 0x42e087bfu;
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    TSIP.REG_04H.WORD = 0x00000202u;
    for (iLoop = 0; iLoop < 64; iLoop = iLoop + 4)
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
    RX65NHU_func100(change_endian_long(0xbcc2c515u), change_endian_long(0x85044042u), change_endian_long(0x2cedb47bu), change_endian_long(0x5f219c6cu));
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
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_24H.WORD = 0x000011c0u;
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
    TSIP.REG_24H.WORD = 0x000009c0u;
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
    TSIP.REG_24H.WORD = 0x0000e0c1u;
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
        RX65NHU_func100(change_endian_long(0x163f0f6cu), change_endian_long(0x57f93c53u), change_endian_long(0x8b425899u), change_endian_long(0x4fd35141u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func101(change_endian_long(0x12f1f9fau), change_endian_long(0x90b892f6u), change_endian_long(0x1da7f44au), change_endian_long(0x0f56608au));
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
            RX65NHU_func101(change_endian_long(0xdaa51f0fu), change_endian_long(0x5ae48ad6u), change_endian_long(0xb9a8b20cu), change_endian_long(0x3431fd0au));
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
    TSIP.REG_24H.WORD = 0x0000b80au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_00H.WORD = 0x000030ffu;
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
    TSIP.REG_28H.WORD = 0x00bf0001u;
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
    TSIP.REG_24H.WORD = 0x0000c0c1u;
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
        TSIP.REG_E0H.WORD = 0x80c0000au;
        TSIP.REG_00H.WORD = 0x00018303u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_B0H.WORD = 0x00000f14u;
        TSIP.REG_A4H.WORD = 0x00008a07u;
        TSIP.REG_00H.WORD = 0x00011303u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x009f0001u;
        TSIP.REG_24H.WORD = 0x000019c0u;
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
        RX65NHU_func100(change_endian_long(0x93a90f5du), change_endian_long(0x9bf0b790u), change_endian_long(0x8238aa9du), change_endian_long(0x7f6dbfe4u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_28H.WORD = 0x00bf0001u;
            TSIP.REG_104H.WORD = 0x00003f61u;
            TSIP.REG_B0H.WORD = 0x00000f00u;
            TSIP.REG_A4H.WORD = 0x42f087bfu;
            TSIP.REG_00H.WORD = 0x00013103u;
            TSIP.REG_2CH.WORD = 0x00000014u;
            for (iLoop = 0; iLoop < 64; iLoop = iLoop + 4)
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
            RX65NHU_func101(change_endian_long(0x65613da3u), change_endian_long(0xe063a670u), change_endian_long(0xb76a38feu), change_endian_long(0x58a0f9f6u));
            break;
        }
        else
        {
            TSIP.REG_28H.WORD = 0x009f0001u;
            TSIP.REG_24H.WORD = 0x000015c0u;
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
            TSIP.REG_00H.WORD = 0x00003083u;
            TSIP.REG_2CH.WORD = 0x00000011u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_28H.WORD = 0x00bf0001u;
            TSIP.REG_24H.WORD = 0x0000880eu;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_D0H.WORD = 0x00000f1cu;
            TSIP.REG_C4H.WORD = 0x00008a83u;
            TSIP.REG_00H.WORD = 0x00013203u;
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
            TSIP.REG_24H.WORD = 0x000019a1u;
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
            TSIP.REG_E0H.WORD = 0x81c0000au;
            TSIP.REG_00H.WORD = 0x00013803u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX65NHU_func101(change_endian_long(0x1c1642ddu), change_endian_long(0xae096aa4u), change_endian_long(0x0c0f06d1u), change_endian_long(0x2e3f65cfu));
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
    RX65NHU_func100(change_endian_long(0x48d24151u), change_endian_long(0xe3435808u), change_endian_long(0xc8f3b49bu), change_endian_long(0xb3bc4291u));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_B0H.WORD = 0x00000714u;
    TSIP.REG_A4H.WORD = 0x00008a07u;
    TSIP.REG_00H.WORD = 0x00001383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_B0H.WORD = 0x08000714u;
    TSIP.REG_A4H.WORD = 0x00008a07u;
    TSIP.REG_00H.WORD = 0x00001383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
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
    TSIP.REG_28H.WORD = 0xfcbf0001u;
    TSIP.REG_24H.WORD = 0x0000c8d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    RX65NHU_func103();
    RX65NHU_func100(change_endian_long(0x831b5bafu), change_endian_long(0xe9a0e449u), change_endian_long(0x27a0e873u), change_endian_long(0x7d93d984u));
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
    RX65NHU_func100(change_endian_long(0x436d6470u), change_endian_long(0x10097586u), change_endian_long(0x2d81e5efu), change_endian_long(0x869aab88u));
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
    TSIP.REG_100H.WORD = change_endian_long(0x671c43cbu);
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x00040804u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    TSIP.REG_D0H.WORD = 0x00000f00u;
    TSIP.REG_C4H.WORD = 0x02f0888fu;
    TSIP.REG_04H.WORD = 0x00000202u;
    for(iLoop=0; iLoop<64; iLoop=iLoop+4)
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
    RX65NHU_func100(change_endian_long(0xf66aa12bu), change_endian_long(0x4c8d88d5u), change_endian_long(0x5d2f0f9eu), change_endian_long(0x8e538f3au));
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
    OutData_PubKeyIndex[68] = TSIP.REG_100H.WORD;
    OutData_PubKeyIndex[69] = TSIP.REG_100H.WORD;
    OutData_PubKeyIndex[70] = TSIP.REG_100H.WORD;
    OutData_PubKeyIndex[71] = TSIP.REG_100H.WORD;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_C8H.BIT.B6)
    {
        /* waiting */
    }
    RX65NHU_func100(change_endian_long(0x0cf8a997u), change_endian_long(0xddb5e093u), change_endian_long(0x0f2529ccu), change_endian_long(0x1846973fu));
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
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    TSIP.REG_D0H.WORD = 0x00000f00u;
    TSIP.REG_C4H.WORD = 0x00e087bfu;
    TSIP.REG_04H.WORD = 0x00000202u;
    for(iLoop=68; iLoop<132; iLoop=iLoop+4)
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
    RX65NHU_func100(change_endian_long(0xbf229965u), change_endian_long(0xa3bec761u), change_endian_long(0x7ca77e1eu), change_endian_long(0x8b207a50u));
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
    TSIP.REG_100H.WORD = change_endian_long(0x00000010u);
    TSIP.REG_04H.WORD = 0x00000212u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PubKeyIndex[136] = TSIP.REG_100H.WORD;
    OutData_PubKeyIndex[137] = TSIP.REG_100H.WORD;
    OutData_PubKeyIndex[138] = TSIP.REG_100H.WORD;
    OutData_PubKeyIndex[139] = TSIP.REG_100H.WORD;
    RX65NHU_func100(change_endian_long(0x1d301abbu), change_endian_long(0xab5e7442u), change_endian_long(0x57db9435u), change_endian_long(0xa4880408u));
    RX65NHU_func103();
    RX65NHU_func100(change_endian_long(0x458c38c0u), change_endian_long(0x9e8a0b69u), change_endian_long(0x1ad47ba5u), change_endian_long(0x78ed4cf1u));
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
    RX65NHU_func100(change_endian_long(0x549a08afu), change_endian_long(0x8a3ccd26u), change_endian_long(0xa5ede106u), change_endian_long(0x2414e132u));
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
    TSIP.REG_100H.WORD = change_endian_long(0x3770f901u);
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    TSIP.REG_D0H.WORD = 0x00000f00u;
    TSIP.REG_C4H.WORD = 0x02e087bfu;
    TSIP.REG_04H.WORD = 0x00000202u;
    for(iLoop=0; iLoop<64; iLoop=iLoop+4)
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
    RX65NHU_func100(change_endian_long(0x2adca0b0u), change_endian_long(0x00a3b7f3u), change_endian_long(0xb84892bfu), change_endian_long(0x2cf082d5u));
    TSIP.REG_B0H.WORD = 0x00000f1cu;
    TSIP.REG_A4H.WORD = 0x00008a83u;
    TSIP.REG_00H.WORD = 0x00012103u;
    TSIP.REG_D0H.WORD = 0x00000f00u;
    TSIP.REG_C4H.WORD = 0x00e087bfu;
    TSIP.REG_04H.WORD = 0x00000202u;
    for(iLoop=64; iLoop<128; iLoop=iLoop+4)
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
    RX65NHU_func100(change_endian_long(0x6049dc88u), change_endian_long(0x607d87e2u), change_endian_long(0x707fbdfeu), change_endian_long(0xdde58e32u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    TSIP.REG_D0H.WORD = 0x00000f00u;
    TSIP.REG_C4H.WORD = 0x00e087bfu;
    TSIP.REG_04H.WORD = 0x00000202u;
    for(iLoop=128; iLoop<192; iLoop=iLoop+4)
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
    RX65NHU_func100(change_endian_long(0x62ffacd1u), change_endian_long(0xde3d2c89u), change_endian_long(0x4a6055f8u), change_endian_long(0x50e5bb4au));
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
    TSIP.REG_100H.WORD = change_endian_long(0x00000030u);
    TSIP.REG_04H.WORD = 0x00000212u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PrivKeyIndex[196] = TSIP.REG_100H.WORD;
    OutData_PrivKeyIndex[197] = TSIP.REG_100H.WORD;
    OutData_PrivKeyIndex[198] = TSIP.REG_100H.WORD;
    OutData_PrivKeyIndex[199] = TSIP.REG_100H.WORD;
    RX65NHU_func102(change_endian_long(0x1bef8cf5u), change_endian_long(0x740b717eu), change_endian_long(0x6691c743u), change_endian_long(0x2358cc65u));
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
 End of function ./input_dir/RX65NHU/RX65NHU_p2b_r3.prc
 *********************************************************************************************************************/
#endif /* #if (TSIP_PRV_USE_RSA_2048 == 1) || (TSIP_TLS == 1) */
