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
* @details       RX72M RSA-1024 Key Generation
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0xfbcab5fbu), change_endian_long(0x6b432c8eu), change_endian_long(0xd78ff7d1u), change_endian_long(0x0807a94au));
    RX72M_RX72N_RX66N_func103();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x066d5592u), change_endian_long(0x13fd3a7fu), change_endian_long(0x4ca9b32au), change_endian_long(0x11e3442bu));
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
        RX72M_RX72N_RX66N_func100(change_endian_long(0xfd5ed203u), change_endian_long(0x5bd73a29u), change_endian_long(0x739f33f5u), change_endian_long(0xed1344d3u));
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
            RX72M_RX72N_RX66N_func100(change_endian_long(0x420d6cc1u), change_endian_long(0x97694ac1u), change_endian_long(0x7907805eu), change_endian_long(0xb036cda8u));
            RX72M_RX72N_RX66N_func103();
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
            RX72M_RX72N_RX66N_func101(change_endian_long(0x14be551bu), change_endian_long(0xa6417918u), change_endian_long(0xb5adccdfu), change_endian_long(0xfccb346fu));
        }
        RX72M_RX72N_RX66N_func100(change_endian_long(0x420d6cc1u), change_endian_long(0x97694ac1u), change_endian_long(0x7907805eu), change_endian_long(0xb036cda8u));
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
        RX72M_RX72N_RX66N_func100(change_endian_long(0xdf006f91u), change_endian_long(0xf915b5fbu), change_endian_long(0x7b6845cau), change_endian_long(0xb733ce69u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func101(change_endian_long(0x5135f3feu), change_endian_long(0xd9e61c55u), change_endian_long(0x559fd98bu), change_endian_long(0x2d3e6d59u));
            continue;
        }
        else
        {
            RX72M_RX72N_RX66N_func101(change_endian_long(0x28d34ecbu), change_endian_long(0xaf87bc69u), change_endian_long(0xd9d738e9u), change_endian_long(0x92104860u));
        }
        TSIP.REG_ECH.WORD = 0x38008a20u;
        TSIP.REG_ECH.WORD = 0x00000001u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0x3d1015f5u), change_endian_long(0x927b9f73u), change_endian_long(0x286da62fu), change_endian_long(0x3775dd30u));
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
            RX72M_RX72N_RX66N_func100(change_endian_long(0xf92d9462u), change_endian_long(0xc108009eu), change_endian_long(0x82db30ffu), change_endian_long(0x20335133u));
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0xb547edf5u), change_endian_long(0x944ffb31u), change_endian_long(0xe1276baau), change_endian_long(0x0e332b86u));
            }
            else
            {
                RX72M_RX72N_RX66N_func101(change_endian_long(0x8b457ad4u), change_endian_long(0x4640860bu), change_endian_long(0xabc3bc24u), change_endian_long(0x7069d1a5u));
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
            RX72M_RX72N_RX66N_func100(change_endian_long(0x46a493b2u), change_endian_long(0x906d8230u), change_endian_long(0xdd796832u), change_endian_long(0xd2cc86c5u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX72M_RX72N_RX66N_func101(change_endian_long(0x2ca3a77du), change_endian_long(0xfc5efae9u), change_endian_long(0x954d3259u), change_endian_long(0x7ed10c0bu));
                continue;
            }
            else
            {
                RX72M_RX72N_RX66N_func101(change_endian_long(0xda55ccbbu), change_endian_long(0xef0680bau), change_endian_long(0xcbd05494u), change_endian_long(0x292ae92cu));
            }
        }
        else
        {
            RX72M_RX72N_RX66N_func101(change_endian_long(0x7e4574c3u), change_endian_long(0x4f0cf515u), change_endian_long(0xfaca7cd0u), change_endian_long(0x94bb6e6eu));
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
            RX72M_RX72N_RX66N_func100(change_endian_long(0xacd0ae84u), change_endian_long(0x4cfd9f6du), change_endian_long(0x8b785ce1u), change_endian_long(0xd400408bu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_ECH.WORD = 0x0000d140u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0x3d0590f9u), change_endian_long(0x1f12d7deu), change_endian_long(0x531fbbcau), change_endian_long(0x39fd55d3u));
                break;
            }
            else
            {
                RX72M_RX72N_RX66N_func101(change_endian_long(0xb1c1e430u), change_endian_long(0x4f9d0ed0u), change_endian_long(0x5caa48e3u), change_endian_long(0x60810952u));
            }
        }
        TSIP.REG_ECH.WORD = 0x38008940u;
        TSIP.REG_ECH.WORD = 0x00000001u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0xade8da67u), change_endian_long(0x11e467e3u), change_endian_long(0x321007e5u), change_endian_long(0xa351baf6u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func101(change_endian_long(0xe4f88a9eu), change_endian_long(0x45900718u), change_endian_long(0x790c212au), change_endian_long(0xecb575cdu));
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
        RX72M_RX72N_RX66N_func100(change_endian_long(0xe92e33f8u), change_endian_long(0xaa0ae796u), change_endian_long(0xe0512c5du), change_endian_long(0x41a6fd8au));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func101(change_endian_long(0xe448500du), change_endian_long(0xa79b1f82u), change_endian_long(0xb5da8f35u), change_endian_long(0xb862837fu));
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
            RX72M_RX72N_RX66N_func100(change_endian_long(0xf2f0b82au), change_endian_long(0x9de5506eu), change_endian_long(0x89d65411u), change_endian_long(0x9f6ed781u));
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
            RX72M_RX72N_RX66N_func100(change_endian_long(0x099ebf46u), change_endian_long(0x3646d8a0u), change_endian_long(0x5ac55aa7u), change_endian_long(0x58d95a51u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX72M_RX72N_RX66N_func101(change_endian_long(0xbecbc890u), change_endian_long(0x4f13da4au), change_endian_long(0x9fae99d3u), change_endian_long(0xcfcf2b2du));
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
            RX72M_RX72N_RX66N_func100(change_endian_long(0x1e43059cu), change_endian_long(0x531d37dfu), change_endian_long(0xee7a5d3au), change_endian_long(0x0e4d0de8u));
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0xdcf0648au), change_endian_long(0x16f1d45eu), change_endian_long(0x18bd0de6u), change_endian_long(0x6f9a7981u));
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0xd723842cu), change_endian_long(0xd6424351u), change_endian_long(0xeb5e3a2eu), change_endian_long(0xf2943912u));
            }
            RX72M_RX72N_RX66N_func100(change_endian_long(0xaaa1bfe1u), change_endian_long(0x8a83e428u), change_endian_long(0x2c922258u), change_endian_long(0x7d2befeau));
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0xb17a9833u), change_endian_long(0x7fd0248fu), change_endian_long(0xf6dc8462u), change_endian_long(0xa65dc7dcu));
            }
            TSIP.REG_ECH.WORD = 0x00007c1fu;
            TSIP.REG_1CH.WORD = 0x00602000u;
            RX72M_RX72N_RX66N_func100(change_endian_long(0x83271a2cu), change_endian_long(0x1e67233au), change_endian_long(0x2b2401e6u), change_endian_long(0x091d3ad0u));
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0x28756b5cu), change_endian_long(0x63f95fd9u), change_endian_long(0x696c2c31u), change_endian_long(0xf6bf7d1cu));
            }
            TSIP.REG_ECH.WORD = 0x00007c1fu;
            TSIP.REG_1CH.WORD = 0x00602000u;
            RX72M_RX72N_RX66N_func100(change_endian_long(0xb6929239u), change_endian_long(0xa52ad1b9u), change_endian_long(0x09efe9bdu), change_endian_long(0x4caa746eu));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x576f6f3eu), change_endian_long(0x80a36af2u), change_endian_long(0x9c5fd534u), change_endian_long(0xd50ba8beu));
                    RX72M_RX72N_RX66N_func103();
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
                    RX72M_RX72N_RX66N_func101(change_endian_long(0xf67e2c0au), change_endian_long(0x0fb7b266u), change_endian_long(0xd4413502u), change_endian_long(0xa985def6u));
                }
                RX72M_RX72N_RX66N_func100(change_endian_long(0x937ac362u), change_endian_long(0x1491730bu), change_endian_long(0xc39c678cu), change_endian_long(0x578519dcu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xf17a9e47u), change_endian_long(0xb4afbbb9u), change_endian_long(0x54b54a17u), change_endian_long(0x1558b288u));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (0u == (TSIP.REG_1CH.BIT.B22))
                {
                    TSIP.REG_ECH.WORD = 0x00002e40u;
                    RX72M_RX72N_RX66N_func101(change_endian_long(0xce0662bfu), change_endian_long(0xc4386976u), change_endian_long(0xaafbd8adu), change_endian_long(0xffbadbd1u));
                }
                else
                {
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x89ef69eeu), change_endian_long(0x6788d62fu), change_endian_long(0x02ba4972u), change_endian_long(0x3ab30219u));
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
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xe2f8da65u), change_endian_long(0x3c11b78fu), change_endian_long(0x70e41127u), change_endian_long(0x609e4588u));
                        TSIP.REG_1CH.WORD = 0x00400000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (0u == (TSIP.REG_1CH.BIT.B22))
                        {
                            TSIP.REG_ECH.WORD = 0x00002e40u;
                            RX72M_RX72N_RX66N_func101(change_endian_long(0x3ecd83c8u), change_endian_long(0xc045469fu), change_endian_long(0x246665d4u), change_endian_long(0x27e6e9b0u));
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
                            RX72M_RX72N_RX66N_func101(change_endian_long(0x38e273c9u), change_endian_long(0x205f8706u), change_endian_long(0x4bd51dc2u), change_endian_long(0x4ae580c9u));
                        }
                    }
                    TSIP.REG_ECH.WORD = 0x38008a40u;
                    TSIP.REG_ECH.WORD = 0x00000000u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00260000u;
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x7e847d71u), change_endian_long(0x0693d365u), change_endian_long(0x06e0a0c2u), change_endian_long(0xef302faeu));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x402707ceu), change_endian_long(0xd1a4fe90u), change_endian_long(0xb018a8cbu), change_endian_long(0x17da09f4u));
                        break;
                    }
                    else
                    {
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x23dc75d7u), change_endian_long(0x6e71f261u), change_endian_long(0xb900c6aau), change_endian_long(0x0f5d3316u));
                    }
                }
            }
            TSIP.REG_ECH.WORD = 0x38000a4bu;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            RX72M_RX72N_RX66N_func100(change_endian_long(0xce71769fu), change_endian_long(0x5d1bb4cbu), change_endian_long(0x92742a4au), change_endian_long(0x77625207u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_ECH.WORD = 0x00002e20u;
                TSIP.REG_ECH.WORD = 0x38008a20u;
                TSIP.REG_ECH.WORD = 0x00000002u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                RX72M_RX72N_RX66N_func100(change_endian_long(0x2ef30317u), change_endian_long(0x3b804325u), change_endian_long(0xb7a009deu), change_endian_long(0xacd77612u));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    RX72M_RX72N_RX66N_func101(change_endian_long(0xe2a4874bu), change_endian_long(0xb732dc69u), change_endian_long(0x81c7a208u), change_endian_long(0x36f4860cu));
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
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x8d4fd48au), change_endian_long(0x2f77d86cu), change_endian_long(0x77511126u), change_endian_long(0xf1f4e69eu));
                }
            }
            else
            {
                RX72M_RX72N_RX66N_func101(change_endian_long(0xf362db1cu), change_endian_long(0x95a61ab9u), change_endian_long(0x1494baf9u), change_endian_long(0xe70d3473u));
            }
        }
    }
    TSIP.REG_ECH.WORD = 0x38008a20u;
    TSIP.REG_ECH.WORD = 0x00000002u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x5c677157u), change_endian_long(0xbbbe50deu), change_endian_long(0xe7bd95f9u), change_endian_long(0x83f6d980u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0x21f2fe3eu), change_endian_long(0x3f9252d3u), change_endian_long(0x70eac25eu), change_endian_long(0x2eefd694u));
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0xa67e5660u), change_endian_long(0x9d3e0fbcu), change_endian_long(0x8bdce2b7u), change_endian_long(0x8d515458u));
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc53a2469u), change_endian_long(0x56909e3eu), change_endian_long(0xced19f75u), change_endian_long(0xffa514aeu));
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
        RX72M_RX72N_RX66N_func100(change_endian_long(0xc0f51b19u), change_endian_long(0xdb29354bu), change_endian_long(0x7ae12777u), change_endian_long(0xd3493530u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func101(change_endian_long(0x28c7ffadu), change_endian_long(0x8e2540adu), change_endian_long(0xfe77eeb0u), change_endian_long(0x54fe8d17u));
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
            RX72M_RX72N_RX66N_func101(change_endian_long(0xda939706u), change_endian_long(0xda72b520u), change_endian_long(0x0eb8e1f1u), change_endian_long(0x83a16294u));
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
        RX72M_RX72N_RX66N_func100(change_endian_long(0xd22a8ea1u), change_endian_long(0x2618912au), change_endian_long(0xae6e489au), change_endian_long(0x272431a1u));
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
            RX72M_RX72N_RX66N_func101(change_endian_long(0xc08ec91au), change_endian_long(0x5b546d49u), change_endian_long(0xd5dea522u), change_endian_long(0x04dcf1b6u));
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
            RX72M_RX72N_RX66N_func101(change_endian_long(0x03f15897u), change_endian_long(0x05bc7dfau), change_endian_long(0x022c5ee8u), change_endian_long(0x7c26e50fu));
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0xbe5cf683u), change_endian_long(0x4d4dc5cau), change_endian_long(0x1f656874u), change_endian_long(0x34364541u));
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
    RX72M_RX72N_RX66N_func103();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xb210b321u), change_endian_long(0x023d5233u), change_endian_long(0x2db784a9u), change_endian_long(0x15d24980u));
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0x51a20fecu), change_endian_long(0xf1a54e6cu), change_endian_long(0xc0e2c225u), change_endian_long(0x893af218u));
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0x6ef5eeacu), change_endian_long(0x91dbe949u), change_endian_long(0xfe77aec9u), change_endian_long(0xba8383fdu));
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0x74be59a8u), change_endian_long(0x0cc631c9u), change_endian_long(0xdd6a0efeu), change_endian_long(0xacdd2a61u));
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0x8cad1970u), change_endian_long(0xbe4993b4u), change_endian_long(0x6e3c4878u), change_endian_long(0x3963dc19u));
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0xab69e326u), change_endian_long(0x4b07bc51u), change_endian_long(0xb1aef97fu), change_endian_long(0x01aec3efu));
    RX72M_RX72N_RX66N_func103();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x7d13ed61u), change_endian_long(0x8ecf320au), change_endian_long(0x7a2c2439u), change_endian_long(0xbafd5c24u));
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0xbe60f8b5u), change_endian_long(0x701b83c9u), change_endian_long(0x24ea1331u), change_endian_long(0x5453cf84u));
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0x383f2159u), change_endian_long(0x4cc3ad63u), change_endian_long(0x31164c72u), change_endian_long(0xb7a11f80u));
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0x7d0785b9u), change_endian_long(0x03997f61u), change_endian_long(0x2a7efa8du), change_endian_long(0x206ba68cu));
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0x7a72c440u), change_endian_long(0x6d04b06eu), change_endian_long(0x09ecf15du), change_endian_long(0x6900533eu));
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
    RX72M_RX72N_RX66N_func102(change_endian_long(0x22e768fcu), change_endian_long(0xfd06291du), change_endian_long(0x680684a9u), change_endian_long(0xe4837f6du));
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
 End of function ./input_dir/RX72M/RX72M_p2a_r3.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_PRV_USE_RSA_1024 == 1 */
