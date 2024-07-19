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
* Function Name: RX72M_RX72N_RX66N_func304
*******************************************************************************************************************/ /**
* @details       RX72M func304
* @note          None
*/
void RX72M_RX72N_RX66N_func304(void)
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
    TSIP.REG_ECH.WORD = 0x00000024u;
    TSIP.REG_ECH.WORD = 0x00003c1eu;
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_ECH.WORD = 0x0000d779u;
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_ECH.WORD = 0x00000b18u;
    TSIP.REG_E0H.WORD = 0x80010300u;
    TSIP.REG_00H.WORD = 0x00008307u;
    TSIP.REG_2CH.WORD = 0x000000adu;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_00H.WORD = 0x0000031fu;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_ECH.WORD = 0x3800db1fu;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x44134eb9u), change_endian_long(0x9e0f1c27u), change_endian_long(0xc09ffd36u), change_endian_long(0xd7812e2bu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func101(change_endian_long(0x56a1dd44u), change_endian_long(0xcbe12e96u), change_endian_long(0x462408a1u), change_endian_long(0x0a51e35bu));
    }
    else
    {
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_24H.WORD = 0x000001c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_34H.WORD = 0x00000400u;
        TSIP.REG_24H.WORD = 0x800080c1u;
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
        TSIP.REG_24H.WORD = 0x0000c0c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_24H.WORD = 0x000011c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_34H.WORD = 0x00000004u;
        TSIP.REG_24H.WORD = 0x82001191u;
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
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_ECH.WORD = 0x0000d379u;
        RX72M_RX72N_RX66N_func101(change_endian_long(0xda079246u), change_endian_long(0xf5fafbe7u), change_endian_long(0x3ba9d966u), change_endian_long(0x031c339du));
    }
    RX72M_RX72N_RX66N_func100(change_endian_long(0x35a084a4u), change_endian_long(0x5245937du), change_endian_long(0xd8de0ae7u), change_endian_long(0xa27add50u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xa5a26a44u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0bbaadddu), change_endian_long(0x5177a9cau), change_endian_long(0xdb24468du), change_endian_long(0xc9e12a54u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func313(1156);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xb81f4393u), change_endian_long(0xf5a3ef93u), change_endian_long(0x20e549fau), change_endian_long(0x7e118319u));
    RX72M_RX72N_RX66N_func314(1156+64);
    TSIP.REG_ECH.WORD = 0x00000b5au;
    TSIP.REG_24H.WORD = 0x000001c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_ECH.WORD = 0x0000d77cu;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x3800db7au;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0xf85c48f4u), change_endian_long(0x43e2f6c9u), change_endian_long(0xe8522766u), change_endian_long(0x5af69eb4u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_24H.WORD = 0x000001c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_34H.WORD = 0x00000400u;
        TSIP.REG_24H.WORD = 0x800080c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_104H.WORD = 0x00000157u;
        TSIP.REG_2CH.WORD = 0x00000050u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000001u);
        TSIP.REG_E0H.WORD = 0x8181001eu;
        TSIP.REG_00H.WORD = 0x00003807u;
        TSIP.REG_2CH.WORD = 0x00000110u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_2CH.WORD = 0x00000050u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_24H.WORD = 0x000011c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x00bf0001u;
        oLoop1 = 1;
        while( 1 == oLoop1 )
    {
            TSIP.REG_1CH.WORD = 0x00210000u;
            RX72M_RX72N_RX66N_func100(change_endian_long(0x4b698849u), change_endian_long(0xa6277bc8u), change_endian_long(0x2a3e3499u), change_endian_long(0x289adab0u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_24H.WORD = 0x000009c0u;
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
                TSIP.REG_24H.WORD = 0x000040c1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_28H.WORD = 0x00800001u;
                TSIP.REG_24H.WORD = 0x000011c0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_34H.WORD = 0x00000004u;
                TSIP.REG_24H.WORD = 0x82001181u;
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
                TSIP.REG_28H.WORD = 0x00bf0001u;
                TSIP.REG_ECH.WORD = 0x00003340u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0x6f0da0dfu), change_endian_long(0xbb816324u), change_endian_long(0x685064e2u), change_endian_long(0xc8066cd7u));
            }
            else
            {
                RX72M_RX72N_RX66N_func101(change_endian_long(0xa0c5713cu), change_endian_long(0xfd11782eu), change_endian_long(0xca4c01f4u), change_endian_long(0x3cbb359du));
                oLoop1 = 0;
            }
        }
        TSIP.REG_ECH.WORD = 0x0000d37cu;
        TSIP.REG_ECH.WORD = 0x00000b9cu;
        TSIP.REG_ECH.WORD = 0x00002b9au;
        TSIP.REG_ECH.WORD = 0x0000375cu;
        RX72M_RX72N_RX66N_func101(change_endian_long(0x904b641au), change_endian_long(0x64e6e7c6u), change_endian_long(0xb92cc67fu), change_endian_long(0xca5c3cacu));
    }
    else
    {
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_E0H.WORD = 0x8181001eu;
        TSIP.REG_00H.WORD = 0x00003807u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_24H.WORD = 0x000001c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_34H.WORD = 0x00000400u;
        TSIP.REG_24H.WORD = 0x800080c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_24H.WORD = 0x000009c0u;
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
        TSIP.REG_24H.WORD = 0x000040c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_24H.WORD = 0x000011c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_34H.WORD = 0x00000004u;
        TSIP.REG_24H.WORD = 0x82001191u;
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
        TSIP.REG_28H.WORD = 0x00bf0001u;
        oLoop1 = 1;
        while( 1 == oLoop1 )
    {
            TSIP.REG_1CH.WORD = 0x00210000u;
            RX72M_RX72N_RX66N_func100(change_endian_long(0xfaaf2d0eu), change_endian_long(0xa3a0e495u), change_endian_long(0x4820c6c8u), change_endian_long(0x06992de3u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_24H.WORD = 0x000009c0u;
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
                TSIP.REG_24H.WORD = 0x000040c1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_28H.WORD = 0x00800001u;
                TSIP.REG_24H.WORD = 0x000011c0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_34H.WORD = 0x00000004u;
                TSIP.REG_24H.WORD = 0x82001181u;
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
                TSIP.REG_28H.WORD = 0x00bf0001u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0x0819fbf1u), change_endian_long(0x54125154u), change_endian_long(0x05f2fe87u), change_endian_long(0x12d6f1bau));
                oLoop1 = 0;
            }
            else
            {
                TSIP.REG_24H.WORD = 0x000009c0u;
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
                TSIP.REG_24H.WORD = 0x000040c1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_28H.WORD = 0x00800001u;
                TSIP.REG_24H.WORD = 0x000011c0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_34H.WORD = 0x00000004u;
                TSIP.REG_24H.WORD = 0x82001191u;
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
                TSIP.REG_28H.WORD = 0x00bf0001u;
                TSIP.REG_ECH.WORD = 0x00002f40u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0xa3b40435u), change_endian_long(0x034cd7f1u), change_endian_long(0x04c1137bu), change_endian_long(0x1c459675u));
            }
        }
        RX72M_RX72N_RX66N_func101(change_endian_long(0x16a84586u), change_endian_long(0xe8af87b0u), change_endian_long(0x252dde58u), change_endian_long(0xf860915fu));
    }
    TSIP.REG_24H.WORD = 0x000088d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x000000a0u;
    TSIP.REG_E0H.WORD = 0x81c0001eu;
    TSIP.REG_00H.WORD = 0x00013803u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x000000a0u;
    TSIP.REG_E0H.WORD = 0x80c0001eu;
    TSIP.REG_00H.WORD = 0x00018303u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_ECH.WORD = 0x00000b39u;
    TSIP.REG_24H.WORD = 0x000001c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_ECH.WORD = 0x0000d77du;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_ECH.WORD = 0x3800db7bu;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0xdfe6816eu), change_endian_long(0x603bf3b3u), change_endian_long(0x7557c680u), change_endian_long(0x67fc9920u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_24H.WORD = 0x000001c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_34H.WORD = 0x00000400u;
        TSIP.REG_24H.WORD = 0x800080c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_104H.WORD = 0x00000157u;
        TSIP.REG_2CH.WORD = 0x00000050u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000001u);
        TSIP.REG_E0H.WORD = 0x8181001eu;
        TSIP.REG_00H.WORD = 0x00003807u;
        TSIP.REG_2CH.WORD = 0x00000110u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_2CH.WORD = 0x00000050u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_24H.WORD = 0x000011c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x00bf0001u;
        oLoop1 = 1;
        while( 1 == oLoop1 )
    {
            TSIP.REG_1CH.WORD = 0x00210000u;
            RX72M_RX72N_RX66N_func100(change_endian_long(0x5d0314f1u), change_endian_long(0x816785b6u), change_endian_long(0x59f10d63u), change_endian_long(0x6df43852u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_24H.WORD = 0x000009c0u;
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
                TSIP.REG_24H.WORD = 0x000040c1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_28H.WORD = 0x00800001u;
                TSIP.REG_24H.WORD = 0x000011c0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_34H.WORD = 0x00000004u;
                TSIP.REG_24H.WORD = 0x82001181u;
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
                TSIP.REG_28H.WORD = 0x00bf0001u;
                TSIP.REG_ECH.WORD = 0x00003320u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0x9e3a741eu), change_endian_long(0x56125a68u), change_endian_long(0xb061e583u), change_endian_long(0x0f794aa4u));
            }
            else
            {
                RX72M_RX72N_RX66N_func101(change_endian_long(0xf8e2c127u), change_endian_long(0x74a79264u), change_endian_long(0x7f338387u), change_endian_long(0xba8352b9u));
                oLoop1 = 0;
            }
        }
        TSIP.REG_ECH.WORD = 0x0000d37du;
        TSIP.REG_ECH.WORD = 0x00000b9cu;
        TSIP.REG_ECH.WORD = 0x00002b99u;
        TSIP.REG_ECH.WORD = 0x0000373cu;
        RX72M_RX72N_RX66N_func101(change_endian_long(0x793902a6u), change_endian_long(0x6d40f4bau), change_endian_long(0x95eafa92u), change_endian_long(0xc2ffc229u));
    }
    else
    {
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_E0H.WORD = 0x8181001eu;
        TSIP.REG_00H.WORD = 0x00003807u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_24H.WORD = 0x000001c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_34H.WORD = 0x00000400u;
        TSIP.REG_24H.WORD = 0x800080c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_24H.WORD = 0x000009c0u;
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
        TSIP.REG_24H.WORD = 0x000040c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_24H.WORD = 0x000011c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_34H.WORD = 0x00000004u;
        TSIP.REG_24H.WORD = 0x82001191u;
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
        TSIP.REG_28H.WORD = 0x00bf0001u;
        oLoop1 = 1;
        while( 1 == oLoop1 )
    {
            TSIP.REG_1CH.WORD = 0x00210000u;
            RX72M_RX72N_RX66N_func100(change_endian_long(0x34d785e1u), change_endian_long(0xec4828eau), change_endian_long(0x0819841bu), change_endian_long(0x4dde166du));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_24H.WORD = 0x000009c0u;
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
                TSIP.REG_24H.WORD = 0x000040c1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_28H.WORD = 0x00800001u;
                TSIP.REG_24H.WORD = 0x000011c0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_34H.WORD = 0x00000004u;
                TSIP.REG_24H.WORD = 0x82001181u;
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
                TSIP.REG_28H.WORD = 0x00bf0001u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0x4b102311u), change_endian_long(0x884abea6u), change_endian_long(0x12622f35u), change_endian_long(0xf1d2dcbau));
                oLoop1 = 0;
            }
            else
            {
                TSIP.REG_24H.WORD = 0x000009c0u;
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
                TSIP.REG_24H.WORD = 0x000040c1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_28H.WORD = 0x00800001u;
                TSIP.REG_24H.WORD = 0x000011c0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_34H.WORD = 0x00000004u;
                TSIP.REG_24H.WORD = 0x82001191u;
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
                TSIP.REG_28H.WORD = 0x00bf0001u;
                TSIP.REG_ECH.WORD = 0x00002f20u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0x8af22e19u), change_endian_long(0xb28ccd72u), change_endian_long(0x28c739c1u), change_endian_long(0xede79737u));
            }
        }
        RX72M_RX72N_RX66N_func101(change_endian_long(0x64a1f10bu), change_endian_long(0x4ae857c7u), change_endian_long(0xa3c79e71u), change_endian_long(0x4b4dabe4u));
    }
    RX72M_RX72N_RX66N_func100(change_endian_long(0x814d6057u), change_endian_long(0x88bef775u), change_endian_long(0x1238948au), change_endian_long(0x7943f97cu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x3f46c431u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xae6fa040u), change_endian_long(0x01ace561u), change_endian_long(0xabbce036u), change_endian_long(0xdd9c31f9u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func313(1088);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x9fea3cedu), change_endian_long(0x1e80247au), change_endian_long(0xc0832a3du), change_endian_long(0x7f20de58u));
    RX72M_RX72N_RX66N_func314(1088+64);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xee3c29c3u), change_endian_long(0xf2edbe16u), change_endian_long(0xd81077a3u), change_endian_long(0x05fb5b56u));
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_24H.WORD = 0x000088d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x000000a0u;
    TSIP.REG_E0H.WORD = 0x81c0001eu;
    TSIP.REG_00H.WORD = 0x00013803u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000050u;
    TSIP.REG_E0H.WORD = 0x8088001eu;
    TSIP.REG_00H.WORD = 0x00008323u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000070u;
    TSIP.REG_E0H.WORD = 0x8088001eu;
    TSIP.REG_00H.WORD = 0x00008323u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_24H.WORD = 0x0000480cu;
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
    TSIP.REG_100H.WORD = change_endian_long(0xdb1eb308u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xde9e1486u), change_endian_long(0xcbf18399u), change_endian_long(0xac34530eu), change_endian_long(0x09bb5887u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func313(1224);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x1bf014a5u), change_endian_long(0x2fc83cf4u), change_endian_long(0x02b13fdau), change_endian_long(0xfa802539u));
    RX72M_RX72N_RX66N_func314(1224+64);
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x000000a0u;
    TSIP.REG_E0H.WORD = 0x81c0001eu;
    TSIP.REG_00H.WORD = 0x00013803u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x0000080au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_E0H.WORD = 0x80010380u;
    TSIP.REG_00H.WORD = 0x00008307u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_E0H.WORD = 0x800103a0u;
    TSIP.REG_00H.WORD = 0x00008307u;
    TSIP.REG_2CH.WORD = 0x000000adu;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_00H.WORD = 0x0000031fu;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    TSIP.REG_104H.WORD = 0x00000767u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x40000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_34H.WORD = 0x00000030u;
    TSIP.REG_24H.WORD = 0x80001dc0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_ECH.WORD = 0x3800dbbeu;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc1fb451au), change_endian_long(0x020c76afu), change_endian_long(0xfe885ad6u), change_endian_long(0xd55e0c08u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_24H.WORD = 0x00000991u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_ECH.WORD = 0x0000d375u;
        RX72M_RX72N_RX66N_func101(change_endian_long(0x69d24a88u), change_endian_long(0x59a0d4bcu), change_endian_long(0x667b33a9u), change_endian_long(0x3ce17c66u));
    }
    else
    {
        TSIP.REG_24H.WORD = 0x00000981u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_ECH.WORD = 0x0000d775u;
        RX72M_RX72N_RX66N_func101(change_endian_long(0x00727505u), change_endian_long(0xd8819fbcu), change_endian_long(0x6b8ef436u), change_endian_long(0xef6ffd07u));
    }
    RX72M_RX72N_RX66N_func100(change_endian_long(0xeed5f01bu), change_endian_long(0x25d12574u), change_endian_long(0x57665203u), change_endian_long(0xcc5ecfbeu));
    TSIP.REG_34H.WORD = 0x00000c00u;
    TSIP.REG_24H.WORD = 0x8000e0c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x45b6d37bu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x656b465bu), change_endian_long(0x15b2bbd5u), change_endian_long(0x4d2a8857u), change_endian_long(0xb35d6c31u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func313(1428);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x2b1bf980u), change_endian_long(0xe0170710u), change_endian_long(0xfbf09296u), change_endian_long(0x25b07ff8u));
    RX72M_RX72N_RX66N_func314(1428+64);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xcb7095bcu), change_endian_long(0xfa17ed3eu), change_endian_long(0x4af57116u), change_endian_long(0x3ca1595fu));
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000050u;
    TSIP.REG_E0H.WORD = 0x8188001eu;
    TSIP.REG_00H.WORD = 0x00003823u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000070u;
    TSIP.REG_E0H.WORD = 0x8188001eu;
    TSIP.REG_00H.WORD = 0x00003823u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_24H.WORD = 0x000009c0u;
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
    TSIP.REG_24H.WORD = 0x000011c0u;
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
    TSIP.REG_24H.WORD = 0x000080c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000c80cu;
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
    TSIP.REG_100H.WORD = change_endian_long(0xdb1eb308u);
    RX72M_RX72N_RX66N_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX72M_RX72N_RX66N_func312(1224);
    TSIP.REG_ECH.WORD = 0x0000d776u;
    TSIP.REG_ECH.WORD = 0x3800db75u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0895dfdeu), change_endian_long(0xe76c5ed2u), change_endian_long(0xe7af345bu), change_endian_long(0x41da287fu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_24H.WORD = 0x000019c0u;
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
        TSIP.REG_1CH.WORD = 0x00210000u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0xb38e8169u), change_endian_long(0x1cca552cu), change_endian_long(0x7634cc98u), change_endian_long(0x2823fef6u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_ECH.WORD = 0x0000d376u;
            TSIP.REG_28H.WORD = 0x00800001u;
            TSIP.REG_104H.WORD = 0x00000257u;
            TSIP.REG_2CH.WORD = 0x00000050u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000001u);
            TSIP.REG_2CH.WORD = 0x00000110u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xFFFFFFFFu);
            TSIP.REG_2CH.WORD = 0x00000050u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_28H.WORD = 0x00bf0001u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0xc2db0141u), change_endian_long(0xb83d8633u), change_endian_long(0xc75bd1fdu), change_endian_long(0xbd9d76e4u));
        }
        else
        {
            TSIP.REG_ECH.WORD = 0x0000d776u;
            TSIP.REG_28H.WORD = 0x00800001u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            TSIP.REG_104H.WORD = 0x00000067u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_28H.WORD = 0x00bf0001u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x07836f94u), change_endian_long(0xf678bb27u), change_endian_long(0x3fdb2a14u), change_endian_long(0x79dbe497u));
        }
    }
    else
    {
        TSIP.REG_24H.WORD = 0x000009c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00001991u;
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
        TSIP.REG_1CH.WORD = 0x00210000u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0x6a31e3e5u), change_endian_long(0x2d3fbf29u), change_endian_long(0x11143160u), change_endian_long(0xb5f5a1cdu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_ECH.WORD = 0x0000d376u;
            TSIP.REG_28H.WORD = 0x00800001u;
            TSIP.REG_104H.WORD = 0x00000257u;
            TSIP.REG_2CH.WORD = 0x00000050u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000001u);
            TSIP.REG_2CH.WORD = 0x00000110u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xFFFFFFFFu);
            TSIP.REG_2CH.WORD = 0x00000050u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_28H.WORD = 0x00bf0001u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x1fea3caau), change_endian_long(0x410f317cu), change_endian_long(0xcf329117u), change_endian_long(0xf0fab89au));
        }
        else
        {
            TSIP.REG_ECH.WORD = 0x0000d776u;
            TSIP.REG_28H.WORD = 0x00800001u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            TSIP.REG_104H.WORD = 0x00000067u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_28H.WORD = 0x00bf0001u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0xc816af2au), change_endian_long(0x77b19c96u), change_endian_long(0xc1b9d121u), change_endian_long(0x64bd61fdu));
        }
    }
    RX72M_RX72N_RX66N_func100(change_endian_long(0x40c07308u), change_endian_long(0x45145edfu), change_endian_long(0xa868cfffu), change_endian_long(0xce2140bau));
    TSIP.REG_24H.WORD = 0x000009c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x04000189u;
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
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_24H.WORD = 0x000011c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x02000189u;
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
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_24H.WORD = 0x000009c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x04000189u;
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
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_24H.WORD = 0x000011c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x02000189u;
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
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xcd850876u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x2af6c5d8u), change_endian_long(0x8e470959u), change_endian_long(0x147711b2u), change_endian_long(0xbb7ed5d4u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func313(1360);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x6365bea6u), change_endian_long(0x97a91ae3u), change_endian_long(0xd50feb34u), change_endian_long(0xaa31fca7u));
    RX72M_RX72N_RX66N_func314(1360+64);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc7971858u), change_endian_long(0xdb395032u), change_endian_long(0xf77700b7u), change_endian_long(0xf923c4b5u));
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000008u;
    TSIP.REG_E0H.WORD = 0x8081001eu;
    TSIP.REG_00H.WORD = 0x00008307u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_ECH.WORD = 0x000037dcu;
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xcd850876u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xfe07eddbu), change_endian_long(0x9c66ad98u), change_endian_long(0xa1080367u), change_endian_long(0x5a99d569u));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX72M_RX72N_RX66N_func312(1360);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xa5a26a44u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x38555cbau), change_endian_long(0x799ec32fu), change_endian_long(0xcedb95a7u), change_endian_long(0x433531e8u));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func312(1156);
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_E0H.WORD = 0x80010380u;
    TSIP.REG_00H.WORD = 0x00008307u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x0000080au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_E0H.WORD = 0x810103c0u;
    TSIP.REG_00H.WORD = 0x00003807u;
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
    TSIP.REG_24H.WORD = 0x000080c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_E0H.WORD = 0x81010380u;
    TSIP.REG_00H.WORD = 0x00003807u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x0000080au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x0000080au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_E0H.WORD = 0x800103c0u;
    TSIP.REG_00H.WORD = 0x00008307u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_ECH.WORD = 0x00008fc0u;
    TSIP.REG_ECH.WORD = 0x0000000fu;
    TSIP.REG_ECH.WORD = 0x0000d777u;
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x00000010u;
    TSIP.REG_ECH.WORD = 0x3000dbc3u;
    TSIP.REG_ECH.WORD = 0x00020020u;
    TSIP.REG_ECH.WORD = 0x00002bddu;
    TSIP.REG_ECH.WORD = 0x00070020u;
    TSIP.REG_ECH.WORD = 0x0000d377u;
    TSIP.REG_ECH.WORD = 0x00000b9cu;
    TSIP.REG_ECH.WORD = 0x00002b9eu;
    TSIP.REG_ECH.WORD = 0x000037dcu;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_28H.WORD = 0x00bf0001u;
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
    TSIP.REG_24H.WORD = 0x000080c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x45b6d37bu);
    RX72M_RX72N_RX66N_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func312(1428);
    TSIP.REG_ECH.WORD = 0x00000bffu;
    TSIP.REG_ECH.WORD = 0x3800db77u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x36bc185eu), change_endian_long(0xfeea7786u), change_endian_long(0x5e0838c7u), change_endian_long(0xaad2cc4bu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func100(change_endian_long(0x1252a1b5u), change_endian_long(0xc2bb895du), change_endian_long(0x70207bafu), change_endian_long(0x5f7ca747u));
        TSIP.REG_E0H.WORD = 0x810103c0u;
        TSIP.REG_04H.WORD = 0x00000606u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
        for (oLoop2 = 0; oLoop2 < S_RAM[0]; oLoop2 = oLoop2 + 1)
        {
            TSIP.REG_24H.WORD = 0x000009c0u;
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
            TSIP.REG_24H.WORD = 0x000040c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_28H.WORD = 0x00800001u;
            TSIP.REG_24H.WORD = 0x000011c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_34H.WORD = 0x00000004u;
            TSIP.REG_24H.WORD = 0x82001191u;
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
            TSIP.REG_28H.WORD = 0x00bf0001u;
            TSIP.REG_ECH.WORD = 0x00002fe0u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0xbb2ff6b9u), change_endian_long(0x7ca47f59u), change_endian_long(0xc09485a4u), change_endian_long(0xfd637ccdu));
        }
        RX72M_RX72N_RX66N_func101(change_endian_long(0x52cbe3bcu), change_endian_long(0xd9e62851u), change_endian_long(0x0a22a46au), change_endian_long(0xa3f23327u));
    }
    else
    {
        RX72M_RX72N_RX66N_func100(change_endian_long(0xaad480f7u), change_endian_long(0x62d71ad7u), change_endian_long(0x6d176bb8u), change_endian_long(0xa0c9d721u));
        TSIP.REG_E0H.WORD = 0x810103c0u;
        TSIP.REG_04H.WORD = 0x00000606u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
        for (oLoop2 = 0; oLoop2 < S_RAM[0]; oLoop2 = oLoop2 + 1)
        {
            TSIP.REG_24H.WORD = 0x000009c0u;
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
            TSIP.REG_24H.WORD = 0x000040c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_28H.WORD = 0x00800001u;
            TSIP.REG_24H.WORD = 0x000011c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_34H.WORD = 0x00000004u;
            TSIP.REG_24H.WORD = 0x82001181u;
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
            TSIP.REG_28H.WORD = 0x00bf0001u;
            TSIP.REG_ECH.WORD = 0x00002fe0u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0xda2e751cu), change_endian_long(0x041872c9u), change_endian_long(0x85c10072u), change_endian_long(0xfe4da127u));
        }
        RX72M_RX72N_RX66N_func101(change_endian_long(0xfd8d02b8u), change_endian_long(0xb2d621c6u), change_endian_long(0x1cf2ab5eu), change_endian_long(0x7781fea5u));
    }
    TSIP.REG_ECH.WORD = 0x38000bdfu;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x15d01267u), change_endian_long(0x603414e7u), change_endian_long(0xb89282f1u), change_endian_long(0xce39fd47u));
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000008u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xcd850876u);
    RX72M_RX72N_RX66N_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func312(1360);
    TSIP.REG_ECH.WORD = 0x3800db76u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0xeed78c61u), change_endian_long(0x6ded2363u), change_endian_long(0xfd61c144u), change_endian_long(0x7ada98c2u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func315(0x00000190u);
        RX72M_RX72N_RX66N_func101(change_endian_long(0x93c46f7fu), change_endian_long(0x39fe83deu), change_endian_long(0x23bd8c72u), change_endian_long(0xe1c28520u));
    }
    else
    {
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_E0H.WORD = 0x8181001eu;
        TSIP.REG_00H.WORD = 0x00003807u;
        TSIP.REG_2CH.WORD = 0x00000090u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x00bf0001u;
        RX72M_RX72N_RX66N_func101(change_endian_long(0xf1e8695bu), change_endian_long(0xeb86c58eu), change_endian_long(0x426f9f45u), change_endian_long(0xd88b077eu));
    }
    TSIP.REG_24H.WORD = 0x000009c0u;
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
    TSIP.REG_24H.WORD = 0x000040c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_24H.WORD = 0x000011c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000004u;
    TSIP.REG_24H.WORD = 0x82001181u;
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
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_ECH.WORD = 0x00000bdeu;
    TSIP.REG_ECH.WORD = 0x02816fdeu;
    TSIP.REG_ECH.WORD = 0x30008bc0u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00020020u;
    TSIP.REG_ECH.WORD = 0x0000d378u;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d778u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x3c68bbf8u), change_endian_long(0x6656cb50u), change_endian_long(0xcede5c82u), change_endian_long(0x15f2093au));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xfc2e094bu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x20ad5744u), change_endian_long(0xcb35537au), change_endian_long(0x94dc1b8fu), change_endian_long(0xa39cdfd0u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func313(1292);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xac87cff8u), change_endian_long(0x5196c12fu), change_endian_long(0xb372d273u), change_endian_long(0x3675d796u));
    RX72M_RX72N_RX66N_func314(1292+64);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x5888bf98u), change_endian_long(0x65f1f0d2u), change_endian_long(0x19b3adbcu), change_endian_long(0x73890919u));
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_E0H.WORD = 0x81010320u;
    TSIP.REG_00H.WORD = 0x00003807u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_E0H.WORD = 0x81010340u;
    TSIP.REG_00H.WORD = 0x00003807u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x0000080au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_E0H.WORD = 0x80010300u;
    TSIP.REG_00H.WORD = 0x00008307u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_ECH.WORD = 0x0000d77eu;
    TSIP.REG_ECH.WORD = 0x00000bdeu;
    TSIP.REG_ECH.WORD = 0x3000db7cu;
    TSIP.REG_ECH.WORD = 0x00020020u;
    TSIP.REG_ECH.WORD = 0x00002fc0u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x3000db7du;
    TSIP.REG_ECH.WORD = 0x00020020u;
    TSIP.REG_ECH.WORD = 0x00002fc0u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x3000dbc0u;
    TSIP.REG_ECH.WORD = 0x00030020u;
    TSIP.REG_ECH.WORD = 0x0000d77eu;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d37eu;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_28H.WORD = 0x00bf0001u;
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
    TSIP.REG_24H.WORD = 0x000080c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x3f46c431u);
    RX72M_RX72N_RX66N_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func312(1088);
    TSIP.REG_ECH.WORD = 0x3000db7cu;
    TSIP.REG_ECH.WORD = 0x00030020u;
    TSIP.REG_ECH.WORD = 0x0000d76bu;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d36bu;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x000037dau;
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000094u;
    RX72M_RX72N_RX66N_func101(change_endian_long(0xd7eb9647u), change_endian_long(0x6fc7fc8fu), change_endian_long(0xca719090u), change_endian_long(0xaf4716aau));
    RX72M_RX72N_RX66N_func310();
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x000000a0u;
    TSIP.REG_E0H.WORD = 0x81c0001eu;
    TSIP.REG_00H.WORD = 0x00013803u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_ECH.WORD = 0x3000db7du;
    TSIP.REG_ECH.WORD = 0x00030020u;
    TSIP.REG_ECH.WORD = 0x0000d76bu;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d36bu;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x000037d9u;
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000095u;
    RX72M_RX72N_RX66N_func101(change_endian_long(0xa1be2d4du), change_endian_long(0x61e04ccdu), change_endian_long(0x151118e7u), change_endian_long(0xc4062a7bu));
    RX72M_RX72N_RX66N_func310();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xbae37b34u), change_endian_long(0xb97a7c88u), change_endian_long(0xdc4998c3u), change_endian_long(0x12bedfceu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xa5a26a44u);
    RX72M_RX72N_RX66N_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func312(1156);
    TSIP.REG_ECH.WORD = 0x3000db7eu;
    TSIP.REG_ECH.WORD = 0x00030020u;
    TSIP.REG_ECH.WORD = 0x0000d76bu;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d36bu;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x000037d8u;
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000096u;
    RX72M_RX72N_RX66N_func101(change_endian_long(0xc2c3480au), change_endian_long(0x7d9e078eu), change_endian_long(0x5190e41eu), change_endian_long(0xbd8a1596u));
    RX72M_RX72N_RX66N_func310();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe2b6ab0du), change_endian_long(0xc809fa9au), change_endian_long(0x16c351ddu), change_endian_long(0xb374619bu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xfc2e094bu);
    RX72M_RX72N_RX66N_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func312(1292);
    TSIP.REG_ECH.WORD = 0x3800db78u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x821a2bf9u), change_endian_long(0xb3018656u), change_endian_long(0x9976b9ddu), change_endian_long(0x75ce6013u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_104H.WORD = 0x00000257u;
        TSIP.REG_2CH.WORD = 0x00000050u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000001u);
        TSIP.REG_2CH.WORD = 0x00000190u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xFFFFFFFFu);
        TSIP.REG_2CH.WORD = 0x00000050u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_28H.WORD = 0x00bf0001u;
        RX72M_RX72N_RX66N_func101(change_endian_long(0x81b6e2b6u), change_endian_long(0xe344b8deu), change_endian_long(0x6bbc593cu), change_endian_long(0xf49095a3u));
    }
    else
    {
        RX72M_RX72N_RX66N_func101(change_endian_long(0x4483f1b7u), change_endian_long(0xce470ae3u), change_endian_long(0x02b36db0u), change_endian_long(0x0454201eu));
    }
    TSIP.REG_24H.WORD = 0x000009c0u;
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
    TSIP.REG_24H.WORD = 0x000040c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_24H.WORD = 0x000011c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000004u;
    TSIP.REG_24H.WORD = 0x82001181u;
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
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_ECH.WORD = 0x3800db79u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x8a928a27u), change_endian_long(0x3e10033cu), change_endian_long(0x5f464177u), change_endian_long(0xd876d554u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func100(change_endian_long(0x2e220d3cu), change_endian_long(0x00417960u), change_endian_long(0x256d9512u), change_endian_long(0x8466913eu));
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_24H.WORD = 0x000001c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_34H.WORD = 0x00000400u;
        TSIP.REG_24H.WORD = 0x800080c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_24H.WORD = 0x000001c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x04000991u;
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
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_34H.WORD = 0x00000004u;
        TSIP.REG_24H.WORD = 0x800011c0u;
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
        TSIP.REG_24H.WORD = 0x000080c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_ECH.WORD = 0x00000bdeu;
        TSIP.REG_ECH.WORD = 0x02816fdeu;
        TSIP.REG_ECH.WORD = 0x30008bc0u;
        TSIP.REG_ECH.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00020020u;
        TSIP.REG_ECH.WORD = 0x0000d368u;
        TSIP.REG_ECH.WORD = 0x00000060u;
        TSIP.REG_ECH.WORD = 0x0000d768u;
        TSIP.REG_ECH.WORD = 0x00000080u;
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_ECH.WORD = 0x0000b7c0u;
        TSIP.REG_ECH.WORD = 0x0000001cu;
        TSIP.REG_E0H.WORD = 0x8081001eu;
        TSIP.REG_00H.WORD = 0x00008307u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xdb1eb308u);
        RX72M_RX72N_RX66N_func080();
        TSIP.REG_00H.WORD = 0x00013203u;
        TSIP.REG_2CH.WORD = 0x00000014u;
        RX72M_RX72N_RX66N_func312(1224);
        TSIP.REG_24H.WORD = 0x000019c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x04000991u;
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
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_34H.WORD = 0x00000004u;
        TSIP.REG_24H.WORD = 0x800011c0u;
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
        TSIP.REG_24H.WORD = 0x000080c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_ECH.WORD = 0x00000bdeu;
        TSIP.REG_ECH.WORD = 0x02816fdeu;
        TSIP.REG_ECH.WORD = 0x30008bc0u;
        TSIP.REG_ECH.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00020020u;
        TSIP.REG_ECH.WORD = 0x0000d372u;
        TSIP.REG_ECH.WORD = 0x00000060u;
        TSIP.REG_ECH.WORD = 0x0000d772u;
        TSIP.REG_ECH.WORD = 0x00000080u;
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_ECH.WORD = 0x0000b7c0u;
        TSIP.REG_ECH.WORD = 0x00000018u;
        TSIP.REG_E0H.WORD = 0x8081001eu;
        TSIP.REG_00H.WORD = 0x00008307u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_24H.WORD = 0x000098d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        RX72M_RX72N_RX66N_func101(change_endian_long(0xf304331bu), change_endian_long(0x19f0eec4u), change_endian_long(0xd236fda0u), change_endian_long(0x549ce078u));
    }
    else
    {
        RX72M_RX72N_RX66N_func100(change_endian_long(0x35fb08bau), change_endian_long(0x83c458f7u), change_endian_long(0xd757d0a0u), change_endian_long(0x5b4020e7u));
        TSIP.REG_ECH.WORD = 0x00000bdeu;
        TSIP.REG_ECH.WORD = 0x02816fdeu;
        TSIP.REG_ECH.WORD = 0x30008bc0u;
        TSIP.REG_ECH.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00020020u;
        TSIP.REG_ECH.WORD = 0x0000d368u;
        TSIP.REG_ECH.WORD = 0x00000060u;
        TSIP.REG_ECH.WORD = 0x0000d768u;
        TSIP.REG_ECH.WORD = 0x00000080u;
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_ECH.WORD = 0x0000b7c0u;
        TSIP.REG_ECH.WORD = 0x0000001cu;
        TSIP.REG_E0H.WORD = 0x8081001eu;
        TSIP.REG_00H.WORD = 0x00008307u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_ECH.WORD = 0x0000b7c0u;
        TSIP.REG_ECH.WORD = 0x00000018u;
        TSIP.REG_24H.WORD = 0x000001c0u;
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
        TSIP.REG_E0H.WORD = 0x8081001eu;
        TSIP.REG_00H.WORD = 0x00008307u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_ECH.WORD = 0x0000d772u;
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xdb1eb308u);
        RX72M_RX72N_RX66N_func080();
        TSIP.REG_00H.WORD = 0x00013203u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        RX72M_RX72N_RX66N_func312(1224);
        RX72M_RX72N_RX66N_func101(change_endian_long(0x91657ebcu), change_endian_long(0x1edf9095u), change_endian_long(0x03054ef8u), change_endian_long(0xdab422eeu));
    }
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000024u;
    TSIP.REG_ECH.WORD = 0x0000381eu;
    TSIP.REG_ECH.WORD = 0x00007c00u;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX72M/RX72M_func304_r1.prc
 *********************************************************************************************************************/
