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
* @details       RX671 RSA-2048 Key Generation
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
    RX671_func100(change_endian_long(0x471964eeu), change_endian_long(0xff78aab5u), change_endian_long(0xd350f788u), change_endian_long(0x2a2e9244u));
    RX671_func103();
    RX671_func100(change_endian_long(0x486f5fbeu), change_endian_long(0x0bd6cd9au), change_endian_long(0x2345657au), change_endian_long(0x6465739fu));
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x01080c84u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_ECH.WORD = 0x00000a31u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
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
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x00000343u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_E0H.WORD = 0x80b00006u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x000083c3u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x009f0001u;
        RX671_func100(change_endian_long(0xa41c8600u), change_endian_long(0x62b4ebd4u), change_endian_long(0xf6a80211u), change_endian_long(0xaadc986eu));
        RX671_func103();
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x01000c84u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_E0H.WORD = 0x80040000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x00008213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_ECH.WORD = 0x0000d01fu;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x81040000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
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
            RX671_func100(change_endian_long(0xb307666eu), change_endian_long(0x80067525u), change_endian_long(0xd0a1cfb3u), change_endian_long(0x129f2fd9u));
            RX671_func103();
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
            RX671_func101(change_endian_long(0x86e0b017u), change_endian_long(0xfa4520f4u), change_endian_long(0xd5f147deu), change_endian_long(0xe16325b2u));
        }
        RX671_func100(change_endian_long(0xb307666eu), change_endian_long(0x80067525u), change_endian_long(0xd0a1cfb3u), change_endian_long(0x129f2fd9u));
        RX671_func103();
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x01000c84u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_E0H.WORD = 0x80040000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x00008213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_ECH.WORD = 0x0000d060u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x81040000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
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
        RX671_func100(change_endian_long(0x13353f68u), change_endian_long(0xd3a89701u), change_endian_long(0xdc4070dfu), change_endian_long(0x45e2eeb5u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func101(change_endian_long(0x332480eeu), change_endian_long(0x9f6c940eu), change_endian_long(0xcd0fb8dfu), change_endian_long(0x4f94c87au));
            continue;
        }
        else
        {
            RX671_func101(change_endian_long(0xdf63a5e6u), change_endian_long(0x30921456u), change_endian_long(0x2994b990u), change_endian_long(0x8bea0262u));
        }
        TSIP.REG_ECH.WORD = 0x38008a20u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00000001u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX671_func100(change_endian_long(0xc5834f0bu), change_endian_long(0x0f55c894u), change_endian_long(0x5105d0e6u), change_endian_long(0xc7f91a8eu));
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
            RX671_func100(change_endian_long(0x67a732d9u), change_endian_long(0x4d6e926au), change_endian_long(0x181cdf4du), change_endian_long(0xae5754c0u));
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
                RX671_func101(change_endian_long(0xccd514b6u), change_endian_long(0xf706e6c0u), change_endian_long(0xd3bdd575u), change_endian_long(0x2507e73du));
            }
            else
            {
                RX671_func101(change_endian_long(0xeb75f710u), change_endian_long(0x5522cc9fu), change_endian_long(0xfad6922bu), change_endian_long(0x7094af8du));
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
            RX671_func100(change_endian_long(0x16b65460u), change_endian_long(0x9d59cc60u), change_endian_long(0x7e20626du), change_endian_long(0x7aa0e54du));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX671_func101(change_endian_long(0xdecde0fcu), change_endian_long(0x4a03ef9fu), change_endian_long(0x7e8178d6u), change_endian_long(0xcd7cef02u));
                continue;
            }
            else
            {
                RX671_func101(change_endian_long(0xa42e55ccu), change_endian_long(0x2e73ed10u), change_endian_long(0x4e679551u), change_endian_long(0x7e1ad49du));
            }
        }
        else
        {
            RX671_func101(change_endian_long(0xcd46fe7bu), change_endian_long(0x551115bbu), change_endian_long(0xdacdcf35u), change_endian_long(0xa89666cbu));
        }
        TSIP.REG_ECH.WORD = 0x000008c6u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x0000094au;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        for(iLoop = 0; iLoop < 96; iLoop = iLoop + 1)
        {
            TSIP.REG_ECH.WORD = 0x01003906u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00002cc0u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00002cc0u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x81010100u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
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
            RX671_func100(change_endian_long(0x7c249d8cu), change_endian_long(0x2225dd6cu), change_endian_long(0x4dd552ecu), change_endian_long(0xbd58fb4cu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_ECH.WORD = 0x0000d140u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                RX671_func101(change_endian_long(0x47bb9f44u), change_endian_long(0x16ea9cc4u), change_endian_long(0xe665ff83u), change_endian_long(0x3c9da1a7u));
                break;
            }
            else
            {
                RX671_func101(change_endian_long(0x7c31af8au), change_endian_long(0x71300db4u), change_endian_long(0x25c90e8bu), change_endian_long(0x40e7516au));
            }
        }
        TSIP.REG_ECH.WORD = 0x38008940u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00000001u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX671_func100(change_endian_long(0x4ca4245fu), change_endian_long(0xbb9704ffu), change_endian_long(0x699724aeu), change_endian_long(0x1f9d9debu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func101(change_endian_long(0x2dfc1806u), change_endian_long(0x2d4daf1au), change_endian_long(0x1cbea15cu), change_endian_long(0x1357542du));
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
        RX671_func100(change_endian_long(0x30857837u), change_endian_long(0x76d59c7eu), change_endian_long(0xabf36130u), change_endian_long(0x955a6a9bu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func101(change_endian_long(0x21ce305du), change_endian_long(0x7bab458au), change_endian_long(0x3301d74au), change_endian_long(0x4f829332u));
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
            RX671_func100(change_endian_long(0x8db60991u), change_endian_long(0x3b6f2002u), change_endian_long(0x029e1c27u), change_endian_long(0xc682e888u));
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
            RX671_func100(change_endian_long(0x92d7019au), change_endian_long(0xbcd737a5u), change_endian_long(0x86b9f4e2u), change_endian_long(0x7ec19a49u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX671_func101(change_endian_long(0xf4470c88u), change_endian_long(0x54bad65bu), change_endian_long(0x8a1dd019u), change_endian_long(0x7c0efdb2u));
                continue;
            }
            TSIP.REG_24H.WORD = 0x000098d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_ECH.WORD = 0x0000b540u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000080u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x80a0000au;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_00H.WORD = 0x00008383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x0000b560u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000003u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
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
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            RX671_func100(change_endian_long(0xd2a2e078u), change_endian_long(0x8f614f40u), change_endian_long(0xbfa19cefu), change_endian_long(0x09dc1b3fu));
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
                RX671_func101(change_endian_long(0xa040863au), change_endian_long(0xe89d588fu), change_endian_long(0xf3543ac8u), change_endian_long(0x8a568e67u));
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
                RX671_func101(change_endian_long(0x309f29b4u), change_endian_long(0x028a2759u), change_endian_long(0xc9d76be2u), change_endian_long(0x70304db1u));
            }
            RX671_func100(change_endian_long(0x89255fb0u), change_endian_long(0xc042a546u), change_endian_long(0xa67b9287u), change_endian_long(0x1b59acfdu));
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
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x0000b4c0u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x000000fcu;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00003906u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00008d00u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0xfffffffeu;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00003d06u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000908u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            for(iLoop = 0; iLoop < 32; iLoop = iLoop + 1)
            {
                TSIP.REG_ECH.WORD = 0x000038e6u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x0000a8c0u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x00000004u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                for(jLoop = 0; jLoop < 32; jLoop = jLoop + 1)
                {
                    TSIP.REG_ECH.WORD = 0x38008900u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00000000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x11816907u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x38008900u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00000000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x10002d20u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x000168e7u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                }
            }
            TSIP.REG_ECH.WORD = 0x00003549u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x0000a540u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000003u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x0002694au;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x81010140u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_04H.WORD = 0x00000606u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
            TSIP.REG_ECH.WORD = 0x000037eau;
            TSIP.REG_1D0H.WORD = 0x00000000u;
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
                TSIP.REG_1D0H.WORD = 0x00000000u;
                RX671_func101(change_endian_long(0x7c37701cu), change_endian_long(0x081f0060u), change_endian_long(0x9337358fu), change_endian_long(0xe6f8958bu));
            }
            TSIP.REG_ECH.WORD = 0x00007c1fu;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_1CH.WORD = 0x00602000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            RX671_func100(change_endian_long(0xf7f9985cu), change_endian_long(0x2cda3713u), change_endian_long(0x5dcd8aafu), change_endian_long(0xd7b83440u));
            TSIP.REG_ECH.WORD = 0x00026d4au;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00002949u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x81010140u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_04H.WORD = 0x00000606u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
            TSIP.REG_ECH.WORD = 0x000037eau;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            for(iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 1)
            {
                TSIP.REG_24H.WORD = 0x2000018du;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_ECH.WORD = 0x000033e0u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                RX671_func101(change_endian_long(0x69394cdbu), change_endian_long(0x953ab8a4u), change_endian_long(0x483ec280u), change_endian_long(0x32aaa69au));
            }
            TSIP.REG_ECH.WORD = 0x00007c1fu;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_1CH.WORD = 0x00602000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            RX671_func100(change_endian_long(0x7b2d07ecu), change_endian_long(0x0cdbd9d7u), change_endian_long(0x1b90f70du), change_endian_long(0xb51f4ad4u));
            TSIP.REG_24H.WORD = 0x0000a0c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_ECH.WORD = 0x00000a52u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
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
            TSIP.REG_1D0H.WORD = 0x00000000u;
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
                    RX671_func100(change_endian_long(0x7c6c7631u), change_endian_long(0x152fca9eu), change_endian_long(0x9ab1ceffu), change_endian_long(0xfc3c3977u));
                    RX671_func103();
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
                    RX671_func101(change_endian_long(0x1c4a9a4du), change_endian_long(0x287cb652u), change_endian_long(0xb83af4eau), change_endian_long(0x0b7cf42au));
                }
                RX671_func100(change_endian_long(0x35d4887au), change_endian_long(0x42e423aau), change_endian_long(0xbb091493u), change_endian_long(0xedc44d8cu));
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
                RX671_func100(change_endian_long(0x91efcee8u), change_endian_long(0x7aeb40ebu), change_endian_long(0xa2fe8e1du), change_endian_long(0x171b59fbu));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (0u == (TSIP.REG_1CH.BIT.B22))
                {
                    TSIP.REG_ECH.WORD = 0x00002e40u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    RX671_func101(change_endian_long(0x4560bd5cu), change_endian_long(0x62320659u), change_endian_long(0xe1e4a7deu), change_endian_long(0x636e1b7bu));
                }
                else
                {
                    RX671_func100(change_endian_long(0xb55b2f04u), change_endian_long(0xbe924f86u), change_endian_long(0x15efcf19u), change_endian_long(0x345153d8u));
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
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_04H.WORD = 0x00000606u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    S_RAM[0+1 + 0] = change_endian_long(TSIP.REG_100H.WORD);
                    TSIP.REG_ECH.WORD = 0x000037e9u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
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
                        RX671_func100(change_endian_long(0x8a564d62u), change_endian_long(0x6ede652fu), change_endian_long(0x11309d21u), change_endian_long(0x7c033512u));
                        TSIP.REG_1CH.WORD = 0x00400000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (0u == (TSIP.REG_1CH.BIT.B22))
                        {
                            TSIP.REG_ECH.WORD = 0x00002e40u;
                            TSIP.REG_1D0H.WORD = 0x00000000u;
                            RX671_func101(change_endian_long(0xaa223fc4u), change_endian_long(0xed2ebc5fu), change_endian_long(0xec1c244du), change_endian_long(0x5c9c0850u));
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
                            RX671_func101(change_endian_long(0x40de32d0u), change_endian_long(0x6ce31994u), change_endian_long(0xdec8dd31u), change_endian_long(0xd71c524bu));
                        }
                    }
                    TSIP.REG_ECH.WORD = 0x38008a40u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00000000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00260000u;
                    RX671_func100(change_endian_long(0xd03973acu), change_endian_long(0x6defc6bbu), change_endian_long(0x2b676fb4u), change_endian_long(0xac34ac94u));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX671_func101(change_endian_long(0x0c5e1efcu), change_endian_long(0xe2ab171cu), change_endian_long(0xb2dc5729u), change_endian_long(0x1f1c436du));
                        break;
                    }
                    else
                    {
                        RX671_func101(change_endian_long(0xe419ca9au), change_endian_long(0x5e722ec9u), change_endian_long(0x3f040fceu), change_endian_long(0xf84d1079u));
                    }
                }
            }
            TSIP.REG_ECH.WORD = 0x38000a4bu;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            RX671_func100(change_endian_long(0x62887b13u), change_endian_long(0x2165a952u), change_endian_long(0x22bffe4du), change_endian_long(0x91d01ffeu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_ECH.WORD = 0x00002e20u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x38008a20u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x00000002u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                RX671_func100(change_endian_long(0x80785d88u), change_endian_long(0x5d78b05cu), change_endian_long(0x630e7466u), change_endian_long(0x9a0267efu));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    RX671_func101(change_endian_long(0xa51dd0b7u), change_endian_long(0xe434ec86u), change_endian_long(0xc3bbd1e4u), change_endian_long(0xedd2f798u));
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
                    RX671_func101(change_endian_long(0x03d6b330u), change_endian_long(0x966cbef7u), change_endian_long(0x5e68757bu), change_endian_long(0x8f31dcb6u));
                }
            }
            else
            {
                RX671_func101(change_endian_long(0x465c0210u), change_endian_long(0x957e9527u), change_endian_long(0x69bc730bu), change_endian_long(0x2bcb95fbu));
            }
        }
    }
    TSIP.REG_ECH.WORD = 0x38008a20u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00000002u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(change_endian_long(0x3697b6ddu), change_endian_long(0x360325a4u), change_endian_long(0x2bf41b04u), change_endian_long(0x1b984850u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func102(change_endian_long(0x031163acu), change_endian_long(0x505ee1b9u), change_endian_long(0x16bdfd66u), change_endian_long(0x329c2bf4u));
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
    RX671_func100(change_endian_long(0xe0c01ba7u), change_endian_long(0x1d3ad71cu), change_endian_long(0xb09a68bau), change_endian_long(0xbbbf00fdu));
    TSIP.REG_ECH.WORD = 0x0000b5c0u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00010001u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
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
    RX671_func100(change_endian_long(0xa5909d9cu), change_endian_long(0xa71c4c7du), change_endian_long(0xe652e4d5u), change_endian_long(0x60827eb6u));
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
        RX671_func100(change_endian_long(0x645ba332u), change_endian_long(0x91403558u), change_endian_long(0xf4f535d2u), change_endian_long(0xf037adc7u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func101(change_endian_long(0xfa9bd261u), change_endian_long(0x30541d3eu), change_endian_long(0xf6e4a16cu), change_endian_long(0x6795d6b6u));
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
            RX671_func101(change_endian_long(0xb7b50af4u), change_endian_long(0xae6ac613u), change_endian_long(0xd2bb7a85u), change_endian_long(0xbddadbdeu));
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
    TSIP.REG_1D0H.WORD = 0x00000000u;
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
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00010001u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
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
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x80c0000au;
        TSIP.REG_1D0H.WORD = 0x00000000u;
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
        TSIP.REG_1D0H.WORD = 0x00000000u;
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
        TSIP.REG_1D0H.WORD = 0x00000000u;
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
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x800100c0u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
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
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00000000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX671_func100(change_endian_long(0x6cce1a29u), change_endian_long(0x0da6848au), change_endian_long(0xff40384du), change_endian_long(0x63fc417bu));
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
            RX671_func101(change_endian_long(0xf9466635u), change_endian_long(0xe9541740u), change_endian_long(0x4da9811bu), change_endian_long(0x0a6068d4u));
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
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x81c0000au;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_00H.WORD = 0x00013803u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX671_func101(change_endian_long(0x9ccbc9b3u), change_endian_long(0xd5816006u), change_endian_long(0x99acde3du), change_endian_long(0xe0fc58c7u));
        }
    }
    TSIP.REG_ECH.WORD = 0x00007c06u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_1CH.WORD = 0x00602000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
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
    RX671_func100(change_endian_long(0x8b427d46u), change_endian_long(0x6c8088d3u), change_endian_long(0xd0a54c60u), change_endian_long(0x50898e97u));
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
    RX671_func103();
    RX671_func100(change_endian_long(0xcc1df264u), change_endian_long(0xd00d2610u), change_endian_long(0x6f9eb274u), change_endian_long(0x6790f935u));
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x01000c84u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_E0H.WORD = 0x80040000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_00H.WORD = 0x00008213u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_E0H.WORD = 0x81040000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
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
    RX671_func100(change_endian_long(0x2adbcad1u), change_endian_long(0x77d357d0u), change_endian_long(0x8934bb47u), change_endian_long(0x2d2eb978u));
    TSIP.REG_C4H.WORD = 0x200c3b0du;
    TSIP.REG_E0H.WORD = 0x81040000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
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
    RX671_func100(change_endian_long(0xd07b26bbu), change_endian_long(0x073468ecu), change_endian_long(0x1de4aebdu), change_endian_long(0x2c79c96au));
    TSIP.REG_C4H.WORD = 0x00f0088cu;
    TSIP.REG_E0H.WORD = 0x810101c0u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
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
    RX671_func100(change_endian_long(0x19ebe316u), change_endian_long(0x99a8dbdcu), change_endian_long(0xac149868u), change_endian_long(0x9b912639u));
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
    RX671_func100(change_endian_long(0xa2976c71u), change_endian_long(0x6d56b88au), change_endian_long(0x0a955960u), change_endian_long(0x94f6449au));
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
    RX671_func100(change_endian_long(0x6ef5dbf3u), change_endian_long(0x7cb04270u), change_endian_long(0x0244424fu), change_endian_long(0x57368170u));
    RX671_func103();
    RX671_func100(change_endian_long(0xbace259eu), change_endian_long(0x5c741597u), change_endian_long(0xbc989c6fu), change_endian_long(0xf36a38bdu));
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x01000c84u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_E0H.WORD = 0x800401e0u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_00H.WORD = 0x00008213u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_E0H.WORD = 0x810401e0u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
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
    RX671_func100(change_endian_long(0x31dee091u), change_endian_long(0xaee19788u), change_endian_long(0x09111fb4u), change_endian_long(0x6ccd144bu));
    TSIP.REG_C4H.WORD = 0x200c3b0du;
    TSIP.REG_E0H.WORD = 0x810401e0u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
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
    RX671_func100(change_endian_long(0x6f07f31fu), change_endian_long(0xdc46ecfbu), change_endian_long(0xc40cdeedu), change_endian_long(0xcdddff84u));
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
    RX671_func100(change_endian_long(0x4b3b8920u), change_endian_long(0x6ca7f445u), change_endian_long(0xfbc2dc38u), change_endian_long(0xa6806785u));
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
    RX671_func100(change_endian_long(0x0a507838u), change_endian_long(0x099de61fu), change_endian_long(0x1137e883u), change_endian_long(0x2c75ad38u));
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
    RX671_func102(change_endian_long(0xe6aa4fd1u), change_endian_long(0xa2869b37u), change_endian_long(0xadb658e4u), change_endian_long(0x0fa29cd9u));
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
 End of function ./input_dir/RX671/RX671_p2b.prc
 *********************************************************************************************************************/
#endif /* #if (TSIP_PRV_USE_RSA_2048 == 1) || (TSIP_TLS == 1) */
