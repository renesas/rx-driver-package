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
* Function Name: RX65NHU_func304
*******************************************************************************************************************/ /**
* @details       RX65NHU func304
* @note          None
*/
void RX65NHU_func304(void)
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
    RX65NHU_func100(change_endian_long(0x8bbaf5f3u), change_endian_long(0x17880359u), change_endian_long(0xde608faeu), change_endian_long(0x2193529fu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func101(change_endian_long(0x82a245bcu), change_endian_long(0xf967ec16u), change_endian_long(0x737f2358u), change_endian_long(0x8a6ab13bu));
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
        RX65NHU_func101(change_endian_long(0x9c778f35u), change_endian_long(0xcad46bbbu), change_endian_long(0x2d0f41ceu), change_endian_long(0xab475805u));
    }
    RX65NHU_func100(change_endian_long(0x2d521d00u), change_endian_long(0x7dc38fdeu), change_endian_long(0xa9d6a559u), change_endian_long(0x4f8911d1u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xa5a26a44u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x6057b909u), change_endian_long(0x8e3b4890u), change_endian_long(0x3d7c8a78u), change_endian_long(0xc4da470bu));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func313(1156);
    RX65NHU_func100(change_endian_long(0x5a48f492u), change_endian_long(0x2cafabe2u), change_endian_long(0x0e469f1fu), change_endian_long(0x5edf4012u));
    RX65NHU_func314(1156+64);
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
    RX65NHU_func100(change_endian_long(0xbd99ba51u), change_endian_long(0xf3bef693u), change_endian_long(0x62d17865u), change_endian_long(0x28469126u));
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
            RX65NHU_func100(change_endian_long(0x433edbf1u), change_endian_long(0xc133c6a3u), change_endian_long(0xa0520f9bu), change_endian_long(0x46da8e58u));
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
                RX65NHU_func101(change_endian_long(0xe1b1d1fau), change_endian_long(0xcf618772u), change_endian_long(0x59cf6297u), change_endian_long(0xeed67b96u));
            }
            else
            {
                RX65NHU_func101(change_endian_long(0x30ccd568u), change_endian_long(0x85e243dfu), change_endian_long(0x48e7df21u), change_endian_long(0x60c00c4fu));
                oLoop1 = 0;
            }
        }
        TSIP.REG_ECH.WORD = 0x0000d37cu;
        TSIP.REG_ECH.WORD = 0x00000b9cu;
        TSIP.REG_ECH.WORD = 0x00002b9au;
        TSIP.REG_ECH.WORD = 0x0000375cu;
        RX65NHU_func101(change_endian_long(0x7551b7e1u), change_endian_long(0x965bb1b1u), change_endian_long(0x12ed0a65u), change_endian_long(0x72f771d8u));
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
            RX65NHU_func100(change_endian_long(0x2e4e1ab5u), change_endian_long(0xe8872c89u), change_endian_long(0x7c73c29eu), change_endian_long(0x153e80e4u));
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
                RX65NHU_func101(change_endian_long(0xdac3b745u), change_endian_long(0x833d2822u), change_endian_long(0x9eb81e23u), change_endian_long(0x33f200f5u));
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
                RX65NHU_func101(change_endian_long(0x31667fd7u), change_endian_long(0x3acbd724u), change_endian_long(0x4087eccau), change_endian_long(0x416238d3u));
            }
        }
        RX65NHU_func101(change_endian_long(0x1f9510b0u), change_endian_long(0x3ae80260u), change_endian_long(0x70432707u), change_endian_long(0x181641e5u));
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
    RX65NHU_func100(change_endian_long(0xacedd2a5u), change_endian_long(0xa539bbabu), change_endian_long(0xca9b20c5u), change_endian_long(0x0effe199u));
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
            RX65NHU_func100(change_endian_long(0x2f8c94d4u), change_endian_long(0x5c6336d2u), change_endian_long(0x949a4f53u), change_endian_long(0x75b3cc3eu));
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
                RX65NHU_func101(change_endian_long(0x73bcc8deu), change_endian_long(0x1610f023u), change_endian_long(0xf12b78d4u), change_endian_long(0xfaf64432u));
            }
            else
            {
                RX65NHU_func101(change_endian_long(0x226d3948u), change_endian_long(0xd1ebf2ddu), change_endian_long(0xe62a8803u), change_endian_long(0x253c8418u));
                oLoop1 = 0;
            }
        }
        TSIP.REG_ECH.WORD = 0x0000d37du;
        TSIP.REG_ECH.WORD = 0x00000b9cu;
        TSIP.REG_ECH.WORD = 0x00002b99u;
        TSIP.REG_ECH.WORD = 0x0000373cu;
        RX65NHU_func101(change_endian_long(0x22beb314u), change_endian_long(0xb68a0a9du), change_endian_long(0x63dd913au), change_endian_long(0xfda97fcdu));
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
            RX65NHU_func100(change_endian_long(0xc64c6755u), change_endian_long(0x4629b66fu), change_endian_long(0x543f9119u), change_endian_long(0x2f8dbc79u));
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
                RX65NHU_func101(change_endian_long(0x43633ac4u), change_endian_long(0x771af4f1u), change_endian_long(0xf7e0db86u), change_endian_long(0xb36fbbafu));
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
                RX65NHU_func101(change_endian_long(0x6e8c439au), change_endian_long(0xd86e2ddcu), change_endian_long(0x312e63a6u), change_endian_long(0xafd57f29u));
            }
        }
        RX65NHU_func101(change_endian_long(0xfc74ed24u), change_endian_long(0x0313758eu), change_endian_long(0x8a8a48ecu), change_endian_long(0xa633793du));
    }
    RX65NHU_func100(change_endian_long(0x5157b731u), change_endian_long(0x0f28b47du), change_endian_long(0x52d91c9bu), change_endian_long(0x18e3c14du));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x3f46c431u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xac9b3353u), change_endian_long(0x7d22e8c6u), change_endian_long(0x7885f673u), change_endian_long(0xb20ff824u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func313(1088);
    RX65NHU_func100(change_endian_long(0x98d88b56u), change_endian_long(0xeae0526bu), change_endian_long(0x7fc8a05fu), change_endian_long(0x07f585ccu));
    RX65NHU_func314(1088+64);
    RX65NHU_func100(change_endian_long(0x729eaf60u), change_endian_long(0x2973051du), change_endian_long(0x53a5d518u), change_endian_long(0x90c43908u));
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
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xd8437a78u), change_endian_long(0x6e892abcu), change_endian_long(0xeb4e0f99u), change_endian_long(0x3b069673u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func313(1224);
    RX65NHU_func100(change_endian_long(0x7252885au), change_endian_long(0xfae2b2c8u), change_endian_long(0x487a2530u), change_endian_long(0x317cd15bu));
    RX65NHU_func314(1224+64);
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
    RX65NHU_func100(change_endian_long(0xec3408bdu), change_endian_long(0x4be64619u), change_endian_long(0x95c08d16u), change_endian_long(0x02e505b7u));
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
        RX65NHU_func101(change_endian_long(0x88af02ccu), change_endian_long(0x5e1f5220u), change_endian_long(0x7adf3dcau), change_endian_long(0x947af1d2u));
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
        RX65NHU_func101(change_endian_long(0xd0566157u), change_endian_long(0x5885d1f8u), change_endian_long(0x199d61acu), change_endian_long(0x54853269u));
    }
    RX65NHU_func100(change_endian_long(0x3da1fd26u), change_endian_long(0x3cd3665au), change_endian_long(0x444b94fcu), change_endian_long(0x8e4842fbu));
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
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x0ffa28d9u), change_endian_long(0x3bce54b2u), change_endian_long(0xf590db02u), change_endian_long(0x4fd89967u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func313(1428);
    RX65NHU_func100(change_endian_long(0xa553327cu), change_endian_long(0xad68d2d5u), change_endian_long(0xd5e03d6fu), change_endian_long(0x48a78d8bu));
    RX65NHU_func314(1428+64);
    RX65NHU_func100(change_endian_long(0xfad1b0a0u), change_endian_long(0x64d34c9du), change_endian_long(0x85beab54u), change_endian_long(0x8ef7a2e5u));
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
    RX65NHU_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX65NHU_func312(1224);
    TSIP.REG_ECH.WORD = 0x0000d776u;
    TSIP.REG_ECH.WORD = 0x3800db75u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX65NHU_func100(change_endian_long(0x7c617e6bu), change_endian_long(0xde744f11u), change_endian_long(0x2a5ae1d0u), change_endian_long(0xe49386dcu));
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
        RX65NHU_func100(change_endian_long(0x8004ee61u), change_endian_long(0x515e628eu), change_endian_long(0x20a164b7u), change_endian_long(0x4b721576u));
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
            RX65NHU_func101(change_endian_long(0xd3f92e08u), change_endian_long(0x6f5f5a37u), change_endian_long(0x146bc66cu), change_endian_long(0x5110b22au));
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
            RX65NHU_func101(change_endian_long(0x6a6d828bu), change_endian_long(0x06e281d0u), change_endian_long(0x9187954du), change_endian_long(0xdd554788u));
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
        RX65NHU_func100(change_endian_long(0xefebf644u), change_endian_long(0x6f59178au), change_endian_long(0xf449f174u), change_endian_long(0xa9c6ee3du));
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
            RX65NHU_func101(change_endian_long(0x6779c149u), change_endian_long(0xee71e078u), change_endian_long(0xe187f278u), change_endian_long(0x9d491769u));
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
            RX65NHU_func101(change_endian_long(0x4c59d36fu), change_endian_long(0xf1250b5du), change_endian_long(0xe454f14au), change_endian_long(0xcc82db9fu));
        }
    }
    RX65NHU_func100(change_endian_long(0x726596cau), change_endian_long(0x47f20884u), change_endian_long(0xbf8df3aau), change_endian_long(0x39479758u));
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
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xd4c5482du), change_endian_long(0x183d23bfu), change_endian_long(0xbf3633ffu), change_endian_long(0xce25920eu));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func313(1360);
    RX65NHU_func100(change_endian_long(0xf7639397u), change_endian_long(0x3eddd3b8u), change_endian_long(0x4a8cac84u), change_endian_long(0xc8badfc1u));
    RX65NHU_func314(1360+64);
    RX65NHU_func100(change_endian_long(0xf7c7be22u), change_endian_long(0x4618822bu), change_endian_long(0x73179383u), change_endian_long(0x4dff9847u));
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
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x4866e7dbu), change_endian_long(0x7edf601bu), change_endian_long(0x861cb492u), change_endian_long(0x23654cbeu));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX65NHU_func312(1360);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xa5a26a44u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xe0feab00u), change_endian_long(0x7ba62e84u), change_endian_long(0xaf5f80dau), change_endian_long(0xc432d1d4u));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func312(1156);
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
    RX65NHU_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func312(1428);
    TSIP.REG_ECH.WORD = 0x00000bffu;
    TSIP.REG_ECH.WORD = 0x3800db77u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX65NHU_func100(change_endian_long(0x71763b6cu), change_endian_long(0x6746aa9eu), change_endian_long(0x9e2793a0u), change_endian_long(0x6fb7568au));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func100(change_endian_long(0x204f0771u), change_endian_long(0x0e480a91u), change_endian_long(0x8a210f96u), change_endian_long(0x88a1409eu));
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
            RX65NHU_func101(change_endian_long(0x1663fb66u), change_endian_long(0x4a9c0e1bu), change_endian_long(0x4257e3b6u), change_endian_long(0xd0e7405eu));
        }
        RX65NHU_func101(change_endian_long(0x4c604849u), change_endian_long(0xe5fc68ccu), change_endian_long(0x91e8cf5bu), change_endian_long(0xf3da8298u));
    }
    else
    {
        RX65NHU_func100(change_endian_long(0xce13f0c6u), change_endian_long(0x9c2422f8u), change_endian_long(0x0f6026aau), change_endian_long(0xcb0ae438u));
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
            RX65NHU_func101(change_endian_long(0xc43ebbfeu), change_endian_long(0xc03d6d9cu), change_endian_long(0x1cd37612u), change_endian_long(0xb9d1f88au));
        }
        RX65NHU_func101(change_endian_long(0x003422feu), change_endian_long(0x53f32f55u), change_endian_long(0xba652a2eu), change_endian_long(0x01403652u));
    }
    TSIP.REG_ECH.WORD = 0x38000bdfu;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    RX65NHU_func100(change_endian_long(0xae9bff5du), change_endian_long(0x302e4155u), change_endian_long(0x6ef38154u), change_endian_long(0xce52f361u));
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
    RX65NHU_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func312(1360);
    TSIP.REG_ECH.WORD = 0x3800db76u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX65NHU_func100(change_endian_long(0x74901e0au), change_endian_long(0x116061e7u), change_endian_long(0x5ece625fu), change_endian_long(0x9db6965fu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func315(0x00000190u);
        RX65NHU_func101(change_endian_long(0x284e2ff5u), change_endian_long(0xc8eb1e1fu), change_endian_long(0xf19f9926u), change_endian_long(0x7bae8724u));
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
        RX65NHU_func101(change_endian_long(0x7798b28eu), change_endian_long(0xa2d3520cu), change_endian_long(0xdd011c1au), change_endian_long(0x7dd23274u));
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
    RX65NHU_func100(change_endian_long(0x20cb9bbbu), change_endian_long(0x013b3db1u), change_endian_long(0xa7adc14cu), change_endian_long(0x3c2352d2u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xfc2e094bu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xfcdf9b30u), change_endian_long(0xe22a7148u), change_endian_long(0x845319cau), change_endian_long(0x957c765du));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func313(1292);
    RX65NHU_func100(change_endian_long(0xd4703942u), change_endian_long(0x31963244u), change_endian_long(0x891ae672u), change_endian_long(0x52d1e789u));
    RX65NHU_func314(1292+64);
    RX65NHU_func100(change_endian_long(0x36febae6u), change_endian_long(0xbb73bdbeu), change_endian_long(0xe70553c4u), change_endian_long(0x3c0e2704u));
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
    RX65NHU_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func312(1088);
    TSIP.REG_ECH.WORD = 0x3000db7cu;
    TSIP.REG_ECH.WORD = 0x00030020u;
    TSIP.REG_ECH.WORD = 0x0000d76bu;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d36bu;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x000037dau;
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000094u;
    RX65NHU_func101(change_endian_long(0x117733d4u), change_endian_long(0xb3157707u), change_endian_long(0x8ef6435cu), change_endian_long(0x48147b87u));
    RX65NHU_func310();
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
    RX65NHU_func101(change_endian_long(0xff961497u), change_endian_long(0x8384047fu), change_endian_long(0x8df7d32au), change_endian_long(0x617d974eu));
    RX65NHU_func310();
    RX65NHU_func100(change_endian_long(0x475ac3cdu), change_endian_long(0xb1ab0daeu), change_endian_long(0x16c5e67bu), change_endian_long(0x5d32be62u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xa5a26a44u);
    RX65NHU_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func312(1156);
    TSIP.REG_ECH.WORD = 0x3000db7eu;
    TSIP.REG_ECH.WORD = 0x00030020u;
    TSIP.REG_ECH.WORD = 0x0000d76bu;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d36bu;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x000037d8u;
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000096u;
    RX65NHU_func101(change_endian_long(0x7c5f5437u), change_endian_long(0x8d3b2a7du), change_endian_long(0xf8c3e825u), change_endian_long(0x5ff82dacu));
    RX65NHU_func310();
    RX65NHU_func100(change_endian_long(0x0fe4afa7u), change_endian_long(0x646bde5cu), change_endian_long(0xd16d6b2bu), change_endian_long(0x40cf79c5u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xfc2e094bu);
    RX65NHU_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func312(1292);
    TSIP.REG_ECH.WORD = 0x3800db78u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX65NHU_func100(change_endian_long(0x4fa14079u), change_endian_long(0x58411164u), change_endian_long(0xb89e2b4du), change_endian_long(0xb2d92e83u));
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
        RX65NHU_func101(change_endian_long(0x77fd0d5eu), change_endian_long(0x7ad2f308u), change_endian_long(0x71d5f978u), change_endian_long(0x59fd4c9eu));
    }
    else
    {
        RX65NHU_func101(change_endian_long(0xae1c6a28u), change_endian_long(0xb3618482u), change_endian_long(0x532c33e3u), change_endian_long(0xd727a538u));
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
    RX65NHU_func100(change_endian_long(0xfa39b641u), change_endian_long(0x3bc64648u), change_endian_long(0xd0a76f26u), change_endian_long(0xb2151344u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func100(change_endian_long(0xcd98d075u), change_endian_long(0xedaf0f5bu), change_endian_long(0x353eaa32u), change_endian_long(0x32e91a5du));
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
        RX65NHU_func080();
        TSIP.REG_00H.WORD = 0x00013203u;
        TSIP.REG_2CH.WORD = 0x00000014u;
        RX65NHU_func312(1224);
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
        RX65NHU_func101(change_endian_long(0x42907536u), change_endian_long(0x298c54b0u), change_endian_long(0x3c7c9297u), change_endian_long(0xf2400bceu));
    }
    else
    {
        RX65NHU_func100(change_endian_long(0xfb317880u), change_endian_long(0x60e1153fu), change_endian_long(0x51c1d3e4u), change_endian_long(0xaa861d86u));
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
        RX65NHU_func080();
        TSIP.REG_00H.WORD = 0x00013203u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        RX65NHU_func312(1224);
        RX65NHU_func101(change_endian_long(0xb28d26ceu), change_endian_long(0x67c6011fu), change_endian_long(0x06a44667u), change_endian_long(0x1a7ecb39u));
    }
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000024u;
    TSIP.REG_ECH.WORD = 0x0000381eu;
    TSIP.REG_ECH.WORD = 0x00007c00u;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_func304_r1.prc
 *********************************************************************************************************************/
