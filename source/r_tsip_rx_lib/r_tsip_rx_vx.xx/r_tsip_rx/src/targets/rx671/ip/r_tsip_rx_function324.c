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
* Function Name: RX671_func324
*******************************************************************************************************************/ /**
* @details       RX671 func324
* @note          None
*/
void RX671_func324(void)
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
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_ECH.WORD = 0x00003fbeu;
    RX671_func100(change_endian_long(0x46d7620eu), change_endian_long(0x55f5d0c6u), change_endian_long(0xec8a0222u), change_endian_long(0x43b4d058u));
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_24H.WORD = 0x000005c0u;
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
    TSIP.REG_24H.WORD = 0x000001c0u;
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
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xfa21a8a9u);
    RX671_func080();
    RX671_func100(change_endian_long(0xe769ea29u), change_endian_long(0xd6323b68u), change_endian_long(0x25259501u), change_endian_long(0xedb60261u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func320(680);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x899d7651u);
    RX671_func080();
    RX671_func100(change_endian_long(0x409b35b7u), change_endian_long(0x636502ceu), change_endian_long(0x04361a6eu), change_endian_long(0x332fc322u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000015u;
    RX671_func320(716);
    TSIP.REG_24H.WORD = 0x0000a0c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x7775adb6u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func320(752);
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_24H.WORD = 0x000001c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x04001991u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x060049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x02001191u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00210000u;
    oLoop1 = 1;
    while( 1 == oLoop1 )
    {
        RX671_func100(change_endian_long(0x911c498bu), change_endian_long(0x2e07b820u), change_endian_long(0x793222d1u), change_endian_long(0x30a13f1bu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func101(change_endian_long(0x96f69251u), change_endian_long(0xf3579a81u), change_endian_long(0x1bec2906u), change_endian_long(0x50c74fa4u));
            oLoop1 = 0;
        }
        else
        {
            TSIP.REG_24H.WORD = 0x000049c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x04001991u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x060049c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x02001191u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00210000u;
            RX671_func101(change_endian_long(0xc287182cu), change_endian_long(0x7324f402u), change_endian_long(0x93a78010u), change_endian_long(0x6454d2dau));
        }
    }
    RX671_func100(change_endian_long(0x1f8cbf42u), change_endian_long(0xc00cab78u), change_endian_long(0xa5882395u), change_endian_long(0x86c76fadu));
    TSIP.REG_24H.WORD = 0x000049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x04001981u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x060049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x02001181u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5f55b804u);
    RX671_func080();
    RX671_func100(change_endian_long(0x70ceee2bu), change_endian_long(0x54b2a640u), change_endian_long(0x286193ecu), change_endian_long(0x927ddac0u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func319(136);
    RX671_func100(change_endian_long(0x3ee69319u), change_endian_long(0xd62ef09fu), change_endian_long(0xd8970045u), change_endian_long(0xe3dabc4eu));
    RX671_func314(136+32);
    RX671_func100(change_endian_long(0x2c1179e7u), change_endian_long(0x1b5a2e3du), change_endian_long(0xe5e2fc9eu), change_endian_long(0x0bea69dcu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x22880c7eu);
    RX671_func080();
    RX671_func100(change_endian_long(0x7d0d328du), change_endian_long(0x3b89d864u), change_endian_long(0x391accb8u), change_endian_long(0x782a2ccfu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX671_func319(172);
    RX671_func100(change_endian_long(0xe49d6088u), change_endian_long(0x273df1c6u), change_endian_long(0x34cf92f0u), change_endian_long(0xd58f3fd9u));
    RX671_func314(172+32);
    RX671_func100(change_endian_long(0x727c322bu), change_endian_long(0x0cd4211fu), change_endian_long(0x07a4a560u), change_endian_long(0x0a679a6du));
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_24H.WORD = 0x000049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8e78b588u);
    RX671_func080();
    RX671_func100(change_endian_long(0x3b0564c4u), change_endian_long(0xc949d492u), change_endian_long(0xee2135a5u), change_endian_long(0xe3be223fu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func319(208);
    RX671_func100(change_endian_long(0x9239c271u), change_endian_long(0x759403eeu), change_endian_long(0xeed1b370u), change_endian_long(0xbde183b8u));
    RX671_func314(208+32);
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_24H.WORD = 0x000049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_ECH.WORD = 0x00000a52u;
    for (iLoop = 0; iLoop < 128; iLoop = iLoop + 1)
    {
        RX671_func100(change_endian_long(0x72b960d0u), change_endian_long(0x299d2129u), change_endian_long(0xfc59771fu), change_endian_long(0xaaf72d0eu));
        TSIP.REG_24H.WORD = 0x040049c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x04000149u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x060049c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x0400d0d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x02000149u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x009f0001u;
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xfa21a8a9u);
        RX671_func080();
        RX671_func100(change_endian_long(0x5979e50cu), change_endian_long(0xac8aac50u), change_endian_long(0xcb4c5ee0u), change_endian_long(0xf9334cbau));
        TSIP.REG_00H.WORD = 0x00003283u;
        TSIP.REG_2CH.WORD = 0x00000014u;
        RX671_func320(680);
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x899d7651u);
        RX671_func080();
        TSIP.REG_00H.WORD = 0x00003283u;
        TSIP.REG_2CH.WORD = 0x00000015u;
        RX671_func320(716);
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_ECH.WORD = 0x00002e40u;
        RX671_func101(change_endian_long(0x661b3452u), change_endian_long(0x09fc0cf8u), change_endian_long(0x006895f8u), change_endian_long(0x268eadfau));
    }
    TSIP.REG_ECH.WORD = 0x38008a40u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    RX671_func100(change_endian_long(0x989aa33fu), change_endian_long(0x9c6b7f6cu), change_endian_long(0x3f2075e2u), change_endian_long(0xaf79e619u));
    TSIP.REG_24H.WORD = 0x040049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x040019a1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x060049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0400d0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x020019a1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xfa21a8a9u);
    RX671_func080();
    RX671_func100(change_endian_long(0x72af019cu), change_endian_long(0xb2a31727u), change_endian_long(0x557a8a4eu), change_endian_long(0x96bddcd8u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func320(680);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x899d7651u);
    RX671_func080();
    RX671_func100(change_endian_long(0xd3023cb2u), change_endian_long(0xc7849498u), change_endian_long(0x651f1e48u), change_endian_long(0x68739ddau));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000015u;
    RX671_func320(716);
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
    RX671_func080();
    RX671_func100(change_endian_long(0x9b896cf0u), change_endian_long(0x45e8fae8u), change_endian_long(0x1061e6ceu), change_endian_long(0x83a0964bu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func319(408);
    RX671_func100(change_endian_long(0xa021afd4u), change_endian_long(0x24960afau), change_endian_long(0x1970e7e4u), change_endian_long(0xe47bec26u));
    RX671_func314(408+32);
    RX671_func100(change_endian_long(0x72a47e28u), change_endian_long(0x84904a2du), change_endian_long(0x46c5560cu), change_endian_long(0x8fd478a3u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
    RX671_func080();
    RX671_func100(change_endian_long(0x5e432adbu), change_endian_long(0xc54b1e8eu), change_endian_long(0x3d2295f1u), change_endian_long(0x5490e2a4u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX671_func319(444);
    RX671_func100(change_endian_long(0x8f85e609u), change_endian_long(0x0236bff1u), change_endian_long(0x873be160u), change_endian_long(0xb28edb42u));
    RX671_func314(444+32);
    RX671_func100(change_endian_long(0xf0717b54u), change_endian_long(0x8922d256u), change_endian_long(0xfda0c847u), change_endian_long(0x562880c5u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
    RX671_func080();
    RX671_func100(change_endian_long(0x6e53d642u), change_endian_long(0xc2ffb691u), change_endian_long(0x9a68e7d1u), change_endian_long(0xf4a07765u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func319(544);
    RX671_func100(change_endian_long(0x9187c34du), change_endian_long(0xdcfbf52cu), change_endian_long(0x26824713u), change_endian_long(0xa14793f1u));
    RX671_func314(544+32);
    RX671_func100(change_endian_long(0x29c8b11eu), change_endian_long(0x231256dbu), change_endian_long(0xd1720624u), change_endian_long(0x622b0609u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
    RX671_func080();
    RX671_func100(change_endian_long(0x423116e2u), change_endian_long(0x6b3620a2u), change_endian_long(0x0e97aeb1u), change_endian_long(0x64213508u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX671_func319(580);
    RX671_func100(change_endian_long(0x58e1f27au), change_endian_long(0x26a90a94u), change_endian_long(0x9f362705u), change_endian_long(0x81d5be86u));
    RX671_func314(580+32);
    RX671_func100(change_endian_long(0x00d1a10fu), change_endian_long(0xb58ca1bbu), change_endian_long(0x1ca0a4bfu), change_endian_long(0xf70d1f2fu));
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_24H.WORD = 0x000049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
    RX671_func080();
    RX671_func100(change_endian_long(0xf728ac6bu), change_endian_long(0x0a63ce2eu), change_endian_long(0xe4cc3805u), change_endian_long(0x8220186au));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func319(480);
    RX671_func100(change_endian_long(0x2a02503du), change_endian_long(0x4debd7d2u), change_endian_long(0x9edcd3acu), change_endian_long(0xe31c22bdu));
    RX671_func314(480+32);
    RX671_func100(change_endian_long(0xd22a0fccu), change_endian_long(0x5182e427u), change_endian_long(0x4d0cc99au), change_endian_long(0xbac0b9f8u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
    RX671_func080();
    RX671_func100(change_endian_long(0x9d49ad69u), change_endian_long(0xd1f6f466u), change_endian_long(0x199eeba8u), change_endian_long(0x8d3e4e8au));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func319(616);
    RX671_func100(change_endian_long(0x856ecc0bu), change_endian_long(0x80590773u), change_endian_long(0x3ea8aeecu), change_endian_long(0xb3778bc9u));
    RX671_func314(616+32);
    TSIP.REG_ECH.WORD = 0x00000a52u;
    for (iLoop = 0; iLoop < 3; iLoop = iLoop + 1)
    {
        TSIP.REG_ECH.WORD = 0x0000b7a0u;
        TSIP.REG_ECH.WORD = 0x000000f0u;
        RX671_func101(change_endian_long(0x65c84d9bu), change_endian_long(0x0ff53a3eu), change_endian_long(0x0ac807f7u), change_endian_long(0x51c2e0a9u));
        RX671_func318();
        RX671_func100(change_endian_long(0x6c2db10fu), change_endian_long(0x8761cc46u), change_endian_long(0xf17982e7u), change_endian_long(0x7548b50au));
        TSIP.REG_28H.WORD = 0x009f0001u;
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
        RX671_func080();
        RX671_func100(change_endian_long(0xc0f8a955u), change_endian_long(0x3e48f058u), change_endian_long(0xdb8354cdu), change_endian_long(0x7e8e1eb6u));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        RX671_func319(408);
        RX671_func100(change_endian_long(0xa1b0a306u), change_endian_long(0x77c6052eu), change_endian_long(0x408da7f3u), change_endian_long(0x6c9aec8fu));
        RX671_func314(408+32);
        RX671_func100(change_endian_long(0x1de2d85cu), change_endian_long(0x2de89a4au), change_endian_long(0xe72659eau), change_endian_long(0xe6107c8bu));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
        RX671_func080();
        RX671_func100(change_endian_long(0xe58b5e61u), change_endian_long(0xe2e2dd6cu), change_endian_long(0x53a9e17au), change_endian_long(0xdbf4d444u));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000023u;
        RX671_func319(444);
        RX671_func100(change_endian_long(0xdf7d3040u), change_endian_long(0x0fe4ea4cu), change_endian_long(0x0a06f63eu), change_endian_long(0x536f8a12u));
        RX671_func314(444+32);
        RX671_func100(change_endian_long(0x2f030a61u), change_endian_long(0x4b31b3f2u), change_endian_long(0x96590de1u), change_endian_long(0x48bcf6b0u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
        RX671_func080();
        RX671_func100(change_endian_long(0x613a08cbu), change_endian_long(0x0d47088du), change_endian_long(0x39ffe92du), change_endian_long(0xd5d9d7d2u));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        RX671_func319(480);
        RX671_func100(change_endian_long(0x501153dfu), change_endian_long(0xb606416au), change_endian_long(0x2a984565u), change_endian_long(0xca49f000u));
        RX671_func314(480+32);
        RX671_func100(change_endian_long(0xb92e2aecu), change_endian_long(0x54615c93u), change_endian_long(0x38af831eu), change_endian_long(0x83ae96a3u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
        RX671_func080();
        RX671_func100(change_endian_long(0x797cfa96u), change_endian_long(0xf7936f9cu), change_endian_long(0xe8bb0beeu), change_endian_long(0xad506513u));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        RX671_func319(544);
        RX671_func100(change_endian_long(0xf4947c25u), change_endian_long(0xd5d82f2fu), change_endian_long(0xa8a4ba4au), change_endian_long(0x75935a0bu));
        RX671_func314(544+32);
        RX671_func100(change_endian_long(0x7a63f9b7u), change_endian_long(0x19fe6a4cu), change_endian_long(0x04d6ed96u), change_endian_long(0xe6cae491u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
        RX671_func080();
        RX671_func100(change_endian_long(0xd8ed5ca9u), change_endian_long(0xb010594cu), change_endian_long(0x4b687310u), change_endian_long(0xe52a7709u));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000023u;
        RX671_func319(580);
        RX671_func100(change_endian_long(0x9fc27b18u), change_endian_long(0xb18e5c8bu), change_endian_long(0xe9fabca6u), change_endian_long(0x25c17dbau));
        RX671_func314(580+32);
        RX671_func100(change_endian_long(0x1e767f47u), change_endian_long(0x9430b82du), change_endian_long(0x1a64d715u), change_endian_long(0x80ea991cu));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
        RX671_func080();
        RX671_func100(change_endian_long(0x6a7a735eu), change_endian_long(0xfd47cea8u), change_endian_long(0x70ff7aadu), change_endian_long(0x06c2ed89u));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        RX671_func319(616);
        RX671_func100(change_endian_long(0x7abeeb83u), change_endian_long(0x408bf2fbu), change_endian_long(0xd3247201u), change_endian_long(0xbb045655u));
        RX671_func314(616+32);
        TSIP.REG_ECH.WORD = 0x00002e40u;
        RX671_func101(change_endian_long(0x77534455u), change_endian_long(0x2899ac04u), change_endian_long(0x94276283u), change_endian_long(0x07128f78u));
    }
    TSIP.REG_ECH.WORD = 0x38008a40u;
    TSIP.REG_ECH.WORD = 0x00000003u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f1u;
    RX671_func101(change_endian_long(0x5d8beecau), change_endian_long(0xa5a986b9u), change_endian_long(0xc4cd7d62u), change_endian_long(0xb768b0ccu));
    RX671_func318();
    RX671_func100(change_endian_long(0x536f765eu), change_endian_long(0x20dc08bcu), change_endian_long(0x9e4972d5u), change_endian_long(0x71f37197u));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
    RX671_func080();
    RX671_func100(change_endian_long(0x9a0699d1u), change_endian_long(0xa0e7198eu), change_endian_long(0xce06d92au), change_endian_long(0x26392fbdu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func319(408);
    RX671_func100(change_endian_long(0x63738caau), change_endian_long(0xd3ea1192u), change_endian_long(0x0d870896u), change_endian_long(0xcc901e2cu));
    RX671_func314(408+32);
    RX671_func100(change_endian_long(0xb09cb317u), change_endian_long(0xc91fbca0u), change_endian_long(0xb0306c26u), change_endian_long(0x81756ef7u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
    RX671_func080();
    RX671_func100(change_endian_long(0xa9b985c3u), change_endian_long(0xaed9e8bfu), change_endian_long(0x11d67141u), change_endian_long(0x2d9a1ed4u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX671_func319(444);
    RX671_func100(change_endian_long(0x4fada4feu), change_endian_long(0x20f97aeeu), change_endian_long(0x0a07cdf9u), change_endian_long(0x80902134u));
    RX671_func314(444+32);
    RX671_func100(change_endian_long(0xf56a65b2u), change_endian_long(0x4e236476u), change_endian_long(0x7c2dbdf6u), change_endian_long(0x80ca0653u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
    RX671_func080();
    RX671_func100(change_endian_long(0xc257d2a0u), change_endian_long(0xbbee51b4u), change_endian_long(0x468f0972u), change_endian_long(0xc5d43de2u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func319(480);
    RX671_func100(change_endian_long(0x3bcdd844u), change_endian_long(0x8e3186b5u), change_endian_long(0xf0fc1bbfu), change_endian_long(0x254d5bf7u));
    RX671_func314(480+32);
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f2u;
    RX671_func101(change_endian_long(0x987947ecu), change_endian_long(0x3ca90929u), change_endian_long(0xc381e867u), change_endian_long(0x2308179eu));
    RX671_func318();
    RX671_func100(change_endian_long(0xef5c1788u), change_endian_long(0xeaef68beu), change_endian_long(0x4c8baad5u), change_endian_long(0xb74be96cu));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x1d2d1d3au);
    RX671_func080();
    RX671_func100(change_endian_long(0x388922d1u), change_endian_long(0x3ac11999u), change_endian_long(0x46991f82u), change_endian_long(0xa22c0442u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func319(272);
    RX671_func100(change_endian_long(0x0a84009eu), change_endian_long(0x01763e4fu), change_endian_long(0xc8dcb853u), change_endian_long(0x33fa4b26u));
    RX671_func314(272+32);
    RX671_func100(change_endian_long(0xd527969bu), change_endian_long(0xc7bfd81cu), change_endian_long(0x1f7f1576u), change_endian_long(0x2a72c44au));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x24fe6a76u);
    RX671_func080();
    RX671_func100(change_endian_long(0x7db5b6e5u), change_endian_long(0x30697248u), change_endian_long(0xa516867fu), change_endian_long(0x69d42048u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX671_func319(308);
    RX671_func100(change_endian_long(0x3eae3869u), change_endian_long(0x99bbab10u), change_endian_long(0xccdefeaeu), change_endian_long(0xbebaec9fu));
    RX671_func314(308+32);
    RX671_func100(change_endian_long(0x574d3b8fu), change_endian_long(0xdb50259au), change_endian_long(0xf7f9a9e2u), change_endian_long(0x9496c04fu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x6176b072u);
    RX671_func080();
    RX671_func100(change_endian_long(0xb2a3a60du), change_endian_long(0x292d4353u), change_endian_long(0xbd1f41f3u), change_endian_long(0x38466f1bu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func319(344);
    RX671_func100(change_endian_long(0x29d6f2c2u), change_endian_long(0xbd31a2f8u), change_endian_long(0xee468004u), change_endian_long(0x098e7645u));
    RX671_func314(344+32);
    RX671_func100(change_endian_long(0x4b0f4777u), change_endian_long(0xeaea2219u), change_endian_long(0xcedc3f73u), change_endian_long(0x60a65e27u));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
    RX671_func080();
    RX671_func100(change_endian_long(0xfec1188eu), change_endian_long(0x471f6207u), change_endian_long(0x79ea7a93u), change_endian_long(0x906a1493u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func319(408);
    RX671_func100(change_endian_long(0x24071616u), change_endian_long(0x2e840867u), change_endian_long(0x80ffab0du), change_endian_long(0xa6b9ea78u));
    RX671_func314(408+32);
    RX671_func100(change_endian_long(0xcb86bbe9u), change_endian_long(0xb32a1c62u), change_endian_long(0xe3b4fe89u), change_endian_long(0x437acec8u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
    RX671_func080();
    RX671_func100(change_endian_long(0xdd1e7da9u), change_endian_long(0xdeea76d2u), change_endian_long(0x99cf6de5u), change_endian_long(0x15e16cb1u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX671_func319(444);
    RX671_func100(change_endian_long(0xbe8e6cb7u), change_endian_long(0xe9252d5cu), change_endian_long(0xfab53fe5u), change_endian_long(0x281e258du));
    RX671_func314(444+32);
    RX671_func100(change_endian_long(0x7ce7742eu), change_endian_long(0xc031624cu), change_endian_long(0xd65d6effu), change_endian_long(0xd7041b85u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
    RX671_func080();
    RX671_func100(change_endian_long(0x9ed09735u), change_endian_long(0x19b1f76bu), change_endian_long(0xb727c787u), change_endian_long(0xa14f00afu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func319(480);
    RX671_func100(change_endian_long(0x9f9daa5du), change_endian_long(0x6a3b278cu), change_endian_long(0xd458c36du), change_endian_long(0x5eab08e3u));
    RX671_func314(480+32);
    RX671_func100(change_endian_long(0x6095d361u), change_endian_long(0x7816a1e6u), change_endian_long(0x87ca1ea4u), change_endian_long(0xf656ebfbu));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x84992ba5u);
    RX671_func080();
    RX671_func100(change_endian_long(0xde1180dau), change_endian_long(0xfab67b93u), change_endian_long(0xcdaa2a1cu), change_endian_long(0xf7b0148fu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func320(0);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x064417e5u);
    RX671_func080();
    RX671_func100(change_endian_long(0x395e22b6u), change_endian_long(0xbf8d9966u), change_endian_long(0x081948e5u), change_endian_long(0xdbf87d1eu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000013u;
    RX671_func320(36);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x49880ba2u);
    RX671_func080();
    RX671_func100(change_endian_long(0x41c140e7u), change_endian_long(0xb9ae6211u), change_endian_long(0x2339be26u), change_endian_long(0xdf8d89d0u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func320(72);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
    RX671_func080();
    RX671_func100(change_endian_long(0xd9c3f350u), change_endian_long(0x48ded530u), change_endian_long(0x5c4f3866u), change_endian_long(0x33e54a89u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func319(544);
    RX671_func100(change_endian_long(0xcc04dcc1u), change_endian_long(0x869e49d5u), change_endian_long(0xb20a164bu), change_endian_long(0x647f8709u));
    RX671_func314(544+32);
    RX671_func100(change_endian_long(0xae18e6dau), change_endian_long(0xfca859e7u), change_endian_long(0xd9c9f445u), change_endian_long(0x880b3be8u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
    RX671_func080();
    RX671_func100(change_endian_long(0x8c3409b4u), change_endian_long(0x8b671408u), change_endian_long(0xb1c2b82eu), change_endian_long(0xbd89ec0bu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX671_func319(580);
    RX671_func100(change_endian_long(0xdc96cbbfu), change_endian_long(0x6cbc93a3u), change_endian_long(0xd62cfee7u), change_endian_long(0x997af21bu));
    RX671_func314(580+32);
    RX671_func100(change_endian_long(0x716b131bu), change_endian_long(0xaa75b912u), change_endian_long(0x66209b76u), change_endian_long(0x293faec5u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
    RX671_func080();
    RX671_func100(change_endian_long(0x995535a4u), change_endian_long(0xb36bafb1u), change_endian_long(0x14c8fd87u), change_endian_long(0x80f250a9u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func319(616);
    RX671_func100(change_endian_long(0xce0f194cu), change_endian_long(0x3fd9ec92u), change_endian_long(0x8e0cd951u), change_endian_long(0x5f83760bu));
    RX671_func314(616+32);
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f3u;
    RX671_func101(change_endian_long(0x9e56e12bu), change_endian_long(0x3c1bb3e6u), change_endian_long(0x38a84ef3u), change_endian_long(0x4b752bbau));
    RX671_func318();
    RX671_func100(change_endian_long(0xe3ec95d9u), change_endian_long(0x5f6478c0u), change_endian_long(0xd24a093cu), change_endian_long(0x220f4ddfu));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x84992ba5u);
    RX671_func080();
    RX671_func100(change_endian_long(0x4690bd58u), change_endian_long(0x9ca591bcu), change_endian_long(0xed0246b6u), change_endian_long(0xbe1f1e25u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func319(0);
    RX671_func100(change_endian_long(0x9a0533ebu), change_endian_long(0x606a2b7du), change_endian_long(0x2a2b32deu), change_endian_long(0x21fa15feu));
    RX671_func314(0+32);
    RX671_func100(change_endian_long(0xe4a3dfa1u), change_endian_long(0xe472cfa8u), change_endian_long(0x301ad9e4u), change_endian_long(0x5521985fu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x064417e5u);
    RX671_func080();
    RX671_func100(change_endian_long(0x20804267u), change_endian_long(0x5a671953u), change_endian_long(0xa5295c1au), change_endian_long(0x7c1ae5a7u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX671_func319(36);
    RX671_func100(change_endian_long(0xb23caa31u), change_endian_long(0x5cf62a37u), change_endian_long(0xff0d1f73u), change_endian_long(0xb555e982u));
    RX671_func314(36+32);
    RX671_func100(change_endian_long(0x96b55fd8u), change_endian_long(0xa536e1abu), change_endian_long(0x20918d46u), change_endian_long(0xb156f0ecu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x49880ba2u);
    RX671_func080();
    RX671_func100(change_endian_long(0x11847334u), change_endian_long(0xfd0ae0bcu), change_endian_long(0x81aca63eu), change_endian_long(0x3abfbb04u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func319(72);
    RX671_func100(change_endian_long(0x4aa780ffu), change_endian_long(0x076cd03eu), change_endian_long(0x71c2ba3fu), change_endian_long(0xbdc2a5d8u));
    RX671_func314(72+32);
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_ECH.WORD = 0x00003bbeu;
    TSIP.REG_ECH.WORD = 0x00007c1du;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX671/RX671_func324.prc
 *********************************************************************************************************************/
