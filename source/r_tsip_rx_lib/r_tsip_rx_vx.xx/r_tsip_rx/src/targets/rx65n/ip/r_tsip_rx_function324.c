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
* Function Name: RX65NHU_func324
*******************************************************************************************************************/ /**
* @details       RX65NHU func324
* @note          None
*/
void RX65NHU_func324(void)
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
    RX65NHU_func100(change_endian_long(0xc68f439du), change_endian_long(0x9d1693d2u), change_endian_long(0x7e93e008u), change_endian_long(0x8f01db9fu));
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
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xb206d82du), change_endian_long(0x52e357d1u), change_endian_long(0x319150ecu), change_endian_long(0x91fff7e3u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func320(680);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x899d7651u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x366ccde4u), change_endian_long(0x05d18395u), change_endian_long(0x686c978eu), change_endian_long(0x47f746f3u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000015u;
    RX65NHU_func320(716);
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
    RX65NHU_func080();
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func320(752);
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
        RX65NHU_func100(change_endian_long(0x00487bbeu), change_endian_long(0xd5271e01u), change_endian_long(0xd466f23eu), change_endian_long(0xfd6a1b9fu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func101(change_endian_long(0x7191f36eu), change_endian_long(0xb06c0502u), change_endian_long(0xccb94055u), change_endian_long(0x4c893f0eu));
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
            RX65NHU_func101(change_endian_long(0x89a5c3b9u), change_endian_long(0x0f28225eu), change_endian_long(0x50c4bd0du), change_endian_long(0xeafda79eu));
        }
    }
    RX65NHU_func100(change_endian_long(0xc82217d3u), change_endian_long(0x65be6936u), change_endian_long(0x78dd75c2u), change_endian_long(0x52f14983u));
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
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x3090cd8du), change_endian_long(0x95092646u), change_endian_long(0x94555dd6u), change_endian_long(0xac9e1073u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func319(136);
    RX65NHU_func100(change_endian_long(0x465c2a09u), change_endian_long(0x742ddb91u), change_endian_long(0x1f0d1d72u), change_endian_long(0x2434545au));
    RX65NHU_func314(136+32);
    RX65NHU_func100(change_endian_long(0xc10d497fu), change_endian_long(0x31e7f818u), change_endian_long(0x8a7a33deu), change_endian_long(0xbc88e61au));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x22880c7eu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x83305da9u), change_endian_long(0x6faf2215u), change_endian_long(0x04db1394u), change_endian_long(0xdd69bfcbu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX65NHU_func319(172);
    RX65NHU_func100(change_endian_long(0x0fc3f546u), change_endian_long(0x433c0009u), change_endian_long(0x2ca9edeau), change_endian_long(0x4b4372adu));
    RX65NHU_func314(172+32);
    RX65NHU_func100(change_endian_long(0x8620cc69u), change_endian_long(0x8f8cf392u), change_endian_long(0xc2692ba8u), change_endian_long(0x922d33a1u));
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
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x070bcdcfu), change_endian_long(0x67da5a1cu), change_endian_long(0xb8e67f03u), change_endian_long(0x3d2fc1a8u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func319(208);
    RX65NHU_func100(change_endian_long(0x6dc3fea4u), change_endian_long(0xe7ec3b28u), change_endian_long(0xbc43554bu), change_endian_long(0x3f28b4eau));
    RX65NHU_func314(208+32);
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
        RX65NHU_func100(change_endian_long(0xc7fe7f2du), change_endian_long(0xc43e6e5eu), change_endian_long(0x6f0e4863u), change_endian_long(0x1fe5b21eu));
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
        RX65NHU_func080();
        RX65NHU_func100(change_endian_long(0xa636c1ecu), change_endian_long(0x4ee02c58u), change_endian_long(0xd94670c7u), change_endian_long(0x672a9082u));
        TSIP.REG_00H.WORD = 0x00003283u;
        TSIP.REG_2CH.WORD = 0x00000014u;
        RX65NHU_func320(680);
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x899d7651u);
        RX65NHU_func080();
        TSIP.REG_00H.WORD = 0x00003283u;
        TSIP.REG_2CH.WORD = 0x00000015u;
        RX65NHU_func320(716);
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_ECH.WORD = 0x00002e40u;
        RX65NHU_func101(change_endian_long(0x463e9b67u), change_endian_long(0xc0e25cddu), change_endian_long(0xb038b1d9u), change_endian_long(0x507e3bd2u));
    }
    TSIP.REG_ECH.WORD = 0x38008a40u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    RX65NHU_func100(change_endian_long(0x274df67bu), change_endian_long(0xf083d31cu), change_endian_long(0x68001e05u), change_endian_long(0x5c6179d0u));
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
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x87094021u), change_endian_long(0xede0bfc4u), change_endian_long(0xa854835fu), change_endian_long(0x7fec374bu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func320(680);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x899d7651u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x51240f5cu), change_endian_long(0xf20c93bfu), change_endian_long(0x5b0f80d4u), change_endian_long(0x14041288u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000015u;
    RX65NHU_func320(716);
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
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xca487219u), change_endian_long(0xff9f398du), change_endian_long(0x5f9f9261u), change_endian_long(0xa9b28455u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func319(408);
    RX65NHU_func100(change_endian_long(0x6163a2e7u), change_endian_long(0xca5f1b4au), change_endian_long(0xa6082186u), change_endian_long(0x52285a70u));
    RX65NHU_func314(408+32);
    RX65NHU_func100(change_endian_long(0x869a7537u), change_endian_long(0x928218abu), change_endian_long(0x46cb054au), change_endian_long(0xcd2002f0u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xfd0cc4d6u), change_endian_long(0x378d2bbeu), change_endian_long(0x6dada1e1u), change_endian_long(0xbbc96beeu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX65NHU_func319(444);
    RX65NHU_func100(change_endian_long(0x94aa862bu), change_endian_long(0x4e3d5133u), change_endian_long(0x8fed6195u), change_endian_long(0x10980968u));
    RX65NHU_func314(444+32);
    RX65NHU_func100(change_endian_long(0x81becbb1u), change_endian_long(0x6d541a5fu), change_endian_long(0x68b63535u), change_endian_long(0x4b6606a5u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x0cb7e17bu), change_endian_long(0x0fe5d112u), change_endian_long(0x7d166e13u), change_endian_long(0x4a360cafu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func319(544);
    RX65NHU_func100(change_endian_long(0x1887262cu), change_endian_long(0x216a73a4u), change_endian_long(0x6fbb758au), change_endian_long(0x5937f32fu));
    RX65NHU_func314(544+32);
    RX65NHU_func100(change_endian_long(0x1d6c8c4au), change_endian_long(0x942f5ef8u), change_endian_long(0x095cda19u), change_endian_long(0xee562d72u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x05eb5424u), change_endian_long(0x56a4ee5fu), change_endian_long(0xf130c7dau), change_endian_long(0x7b833585u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX65NHU_func319(580);
    RX65NHU_func100(change_endian_long(0x4181be0eu), change_endian_long(0xf093bae2u), change_endian_long(0x81121de3u), change_endian_long(0x168a0557u));
    RX65NHU_func314(580+32);
    RX65NHU_func100(change_endian_long(0xf2a79c08u), change_endian_long(0x119f346fu), change_endian_long(0xf5a85b71u), change_endian_long(0x40f7b853u));
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
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x39687bbdu), change_endian_long(0xc92cd4e6u), change_endian_long(0xd437c814u), change_endian_long(0x1f7e6899u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func319(480);
    RX65NHU_func100(change_endian_long(0x6560f8f9u), change_endian_long(0x08134650u), change_endian_long(0x4eee70c1u), change_endian_long(0x542e22cfu));
    RX65NHU_func314(480+32);
    RX65NHU_func100(change_endian_long(0x3d3bb054u), change_endian_long(0x8dd56edbu), change_endian_long(0xa3774e98u), change_endian_long(0xd46c6bafu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xe6cffb28u), change_endian_long(0xeb3e4658u), change_endian_long(0xe250cd08u), change_endian_long(0x907eeebdu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func319(616);
    RX65NHU_func100(change_endian_long(0xab49d198u), change_endian_long(0x503d31dau), change_endian_long(0xe7c0f5ddu), change_endian_long(0x7cb5832eu));
    RX65NHU_func314(616+32);
    TSIP.REG_ECH.WORD = 0x00000a52u;
    for (iLoop = 0; iLoop < 3; iLoop = iLoop + 1)
    {
        TSIP.REG_ECH.WORD = 0x0000b7a0u;
        TSIP.REG_ECH.WORD = 0x000000f0u;
        RX65NHU_func101(change_endian_long(0x0355f031u), change_endian_long(0x9cd05ecau), change_endian_long(0x0b58a613u), change_endian_long(0x05dc1d22u));
        RX65NHU_func318();
        RX65NHU_func100(change_endian_long(0xd3888d40u), change_endian_long(0x7aa8e2b1u), change_endian_long(0x23e9b8f0u), change_endian_long(0x1e5009f8u));
        TSIP.REG_28H.WORD = 0x009f0001u;
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
        RX65NHU_func080();
        RX65NHU_func100(change_endian_long(0x522ea64bu), change_endian_long(0x4982cdd0u), change_endian_long(0xb6df8aa7u), change_endian_long(0x22010302u));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        RX65NHU_func319(408);
        RX65NHU_func100(change_endian_long(0x2d461b62u), change_endian_long(0x9d1094c0u), change_endian_long(0x78efdee4u), change_endian_long(0x5475fe0au));
        RX65NHU_func314(408+32);
        RX65NHU_func100(change_endian_long(0x58fbdfffu), change_endian_long(0x5533ea19u), change_endian_long(0x71342b9au), change_endian_long(0xff8fb5c1u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
        RX65NHU_func080();
        RX65NHU_func100(change_endian_long(0x896cba09u), change_endian_long(0x72cd8f58u), change_endian_long(0x943882e3u), change_endian_long(0xba12452au));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000023u;
        RX65NHU_func319(444);
        RX65NHU_func100(change_endian_long(0x4619d4cfu), change_endian_long(0x7d12b0a5u), change_endian_long(0x9de9b19au), change_endian_long(0x14040082u));
        RX65NHU_func314(444+32);
        RX65NHU_func100(change_endian_long(0x047856b0u), change_endian_long(0x0c8126a2u), change_endian_long(0x71c6a01bu), change_endian_long(0xa4f5c8edu));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
        RX65NHU_func080();
        RX65NHU_func100(change_endian_long(0xbef9d101u), change_endian_long(0xe2a7e326u), change_endian_long(0xb132b577u), change_endian_long(0x81418cacu));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        RX65NHU_func319(480);
        RX65NHU_func100(change_endian_long(0x802b68feu), change_endian_long(0xab8d7c5bu), change_endian_long(0xa64b6751u), change_endian_long(0x07114aeeu));
        RX65NHU_func314(480+32);
        RX65NHU_func100(change_endian_long(0x5d87fe8fu), change_endian_long(0x41c10b68u), change_endian_long(0x1bd68041u), change_endian_long(0xe060234eu));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
        RX65NHU_func080();
        RX65NHU_func100(change_endian_long(0x9fddb9fbu), change_endian_long(0xeb344fc8u), change_endian_long(0x74e5058du), change_endian_long(0xa563f6f1u));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        RX65NHU_func319(544);
        RX65NHU_func100(change_endian_long(0x4c5222cdu), change_endian_long(0x061ecee9u), change_endian_long(0x91848b6fu), change_endian_long(0xba34de55u));
        RX65NHU_func314(544+32);
        RX65NHU_func100(change_endian_long(0xd42a56e5u), change_endian_long(0xb4408854u), change_endian_long(0xcfa53f36u), change_endian_long(0xa5dc3405u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
        RX65NHU_func080();
        RX65NHU_func100(change_endian_long(0xe9ee431fu), change_endian_long(0xdf9277c3u), change_endian_long(0x17cc583fu), change_endian_long(0xad2fc225u));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000023u;
        RX65NHU_func319(580);
        RX65NHU_func100(change_endian_long(0x7e039963u), change_endian_long(0x39e138b0u), change_endian_long(0xcfeb1e01u), change_endian_long(0xeaf91d92u));
        RX65NHU_func314(580+32);
        RX65NHU_func100(change_endian_long(0x9f70b16du), change_endian_long(0xda033ef1u), change_endian_long(0xebbf622bu), change_endian_long(0x4e73fb0du));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
        RX65NHU_func080();
        RX65NHU_func100(change_endian_long(0x2157d7bcu), change_endian_long(0x22f7fdeau), change_endian_long(0x1c85526du), change_endian_long(0xdb572684u));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        RX65NHU_func319(616);
        RX65NHU_func100(change_endian_long(0x911b1963u), change_endian_long(0x93ffe3dau), change_endian_long(0xd87c0157u), change_endian_long(0x8a512103u));
        RX65NHU_func314(616+32);
        TSIP.REG_ECH.WORD = 0x00002e40u;
        RX65NHU_func101(change_endian_long(0x9189cb17u), change_endian_long(0x246076a2u), change_endian_long(0xa05f6c33u), change_endian_long(0xf00ef48du));
    }
    TSIP.REG_ECH.WORD = 0x38008a40u;
    TSIP.REG_ECH.WORD = 0x00000003u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f1u;
    RX65NHU_func101(change_endian_long(0x6d329ff6u), change_endian_long(0x4f62c949u), change_endian_long(0x2576e31au), change_endian_long(0xbfeec567u));
    RX65NHU_func318();
    RX65NHU_func100(change_endian_long(0x29081f45u), change_endian_long(0xd6fde6cau), change_endian_long(0x7fe80668u), change_endian_long(0xa3e31103u));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x7f01326cu), change_endian_long(0x99f0d579u), change_endian_long(0xdbedc0fau), change_endian_long(0x134eff1cu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func319(408);
    RX65NHU_func100(change_endian_long(0x040e5fd1u), change_endian_long(0x6b8c1bbdu), change_endian_long(0xab7fca5eu), change_endian_long(0x7d973ef0u));
    RX65NHU_func314(408+32);
    RX65NHU_func100(change_endian_long(0xcaa39a39u), change_endian_long(0xf5df130du), change_endian_long(0x28a68ee3u), change_endian_long(0x76ce8ea1u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xbeb5a4ebu), change_endian_long(0xe5797fd8u), change_endian_long(0x36f54f03u), change_endian_long(0xe1bd2da4u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX65NHU_func319(444);
    RX65NHU_func100(change_endian_long(0x168d9930u), change_endian_long(0x94a13ce1u), change_endian_long(0x060dbbffu), change_endian_long(0x57c676fbu));
    RX65NHU_func314(444+32);
    RX65NHU_func100(change_endian_long(0x5efb653eu), change_endian_long(0x7aa9e97du), change_endian_long(0x8fecb699u), change_endian_long(0xde4fdeeeu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x2b4c2b12u), change_endian_long(0x92054a7cu), change_endian_long(0xd18f92dbu), change_endian_long(0x9ebec7efu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX65NHU_func319(480);
    RX65NHU_func100(change_endian_long(0x91b1d8b9u), change_endian_long(0x77fc1394u), change_endian_long(0x03e931c6u), change_endian_long(0x14eb4b31u));
    RX65NHU_func314(480+32);
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f2u;
    RX65NHU_func101(change_endian_long(0x81b0a6e7u), change_endian_long(0xc1a207e4u), change_endian_long(0x3032ac12u), change_endian_long(0xdf3bb21du));
    RX65NHU_func318();
    RX65NHU_func100(change_endian_long(0x21f9ab17u), change_endian_long(0x25cee42au), change_endian_long(0xbd2d67cbu), change_endian_long(0x1e943848u));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x1d2d1d3au);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x52df8fafu), change_endian_long(0x93b3cd4eu), change_endian_long(0xda220a70u), change_endian_long(0x73a2d159u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func319(272);
    RX65NHU_func100(change_endian_long(0xc1c425b4u), change_endian_long(0xf87a97b8u), change_endian_long(0xcb3fdc38u), change_endian_long(0x6713c93cu));
    RX65NHU_func314(272+32);
    RX65NHU_func100(change_endian_long(0xcc5a0c20u), change_endian_long(0x95acbc92u), change_endian_long(0x563da393u), change_endian_long(0xc71c87a2u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x24fe6a76u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xb58cd838u), change_endian_long(0x38816ad1u), change_endian_long(0x849f8cd4u), change_endian_long(0x6ba15431u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX65NHU_func319(308);
    RX65NHU_func100(change_endian_long(0x5150aa62u), change_endian_long(0xef8156a0u), change_endian_long(0xa82e5cfau), change_endian_long(0x652135e3u));
    RX65NHU_func314(308+32);
    RX65NHU_func100(change_endian_long(0xb70e12abu), change_endian_long(0xcd1b151au), change_endian_long(0x04d4df10u), change_endian_long(0xae66338eu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x6176b072u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x80dae78du), change_endian_long(0x3c069577u), change_endian_long(0x4e3016adu), change_endian_long(0x71ade6feu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX65NHU_func319(344);
    RX65NHU_func100(change_endian_long(0x463c3c0fu), change_endian_long(0xec2bb4a7u), change_endian_long(0x9dbcf9a5u), change_endian_long(0xdbe05675u));
    RX65NHU_func314(344+32);
    RX65NHU_func100(change_endian_long(0x4101237eu), change_endian_long(0x08f84de5u), change_endian_long(0xcbd37426u), change_endian_long(0x07856920u));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x9f03200bu), change_endian_long(0x620c59cdu), change_endian_long(0xeff2e384u), change_endian_long(0x462831dfu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func319(408);
    RX65NHU_func100(change_endian_long(0xa57b55ccu), change_endian_long(0xf86f8659u), change_endian_long(0x07afe812u), change_endian_long(0x4fbcd325u));
    RX65NHU_func314(408+32);
    RX65NHU_func100(change_endian_long(0x4ef7ff64u), change_endian_long(0x59a27461u), change_endian_long(0xa0955912u), change_endian_long(0x203e33dbu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xb46fdeb0u), change_endian_long(0x9a55e49eu), change_endian_long(0x84071855u), change_endian_long(0xd11f632cu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX65NHU_func319(444);
    RX65NHU_func100(change_endian_long(0x5e3f7e1fu), change_endian_long(0xfa92eb64u), change_endian_long(0x90274c4au), change_endian_long(0xfb0fb246u));
    RX65NHU_func314(444+32);
    RX65NHU_func100(change_endian_long(0xac55693bu), change_endian_long(0xf1bed898u), change_endian_long(0x66f214b5u), change_endian_long(0xce2772beu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x864876bcu), change_endian_long(0xf91d0f97u), change_endian_long(0x1e147b97u), change_endian_long(0xc528d933u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX65NHU_func319(480);
    RX65NHU_func100(change_endian_long(0xb212ee45u), change_endian_long(0xb2a2118fu), change_endian_long(0x7946bdddu), change_endian_long(0x3bc792f5u));
    RX65NHU_func314(480+32);
    RX65NHU_func100(change_endian_long(0xfe075d48u), change_endian_long(0x6aedc5dcu), change_endian_long(0x73765ff3u), change_endian_long(0x61ac75f6u));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x84992ba5u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x0dd1e0c2u), change_endian_long(0x2c00a1bau), change_endian_long(0x5bca77b5u), change_endian_long(0x36eeb56cu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX65NHU_func320(0);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x064417e5u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x78fdba35u), change_endian_long(0x68f3bc89u), change_endian_long(0xb4e34015u), change_endian_long(0x02759264u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000013u;
    RX65NHU_func320(36);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x49880ba2u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x9b2da389u), change_endian_long(0xc7c14325u), change_endian_long(0x376d8451u), change_endian_long(0xb2d9301cu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func320(72);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x2c53a07eu), change_endian_long(0xb06ff7a2u), change_endian_long(0xe85dbb9bu), change_endian_long(0xff13c4fbu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func319(544);
    RX65NHU_func100(change_endian_long(0x9aef139bu), change_endian_long(0xd151acfdu), change_endian_long(0xee4f4400u), change_endian_long(0x1e20a663u));
    RX65NHU_func314(544+32);
    RX65NHU_func100(change_endian_long(0xdd31eae3u), change_endian_long(0x7f0eca4bu), change_endian_long(0x207963a8u), change_endian_long(0x6c19ee1bu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x8b8e463fu), change_endian_long(0x863b08dcu), change_endian_long(0xbaab31f1u), change_endian_long(0x821012f1u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX65NHU_func319(580);
    RX65NHU_func100(change_endian_long(0x5593c6c7u), change_endian_long(0xa5b74881u), change_endian_long(0xaf9bf41eu), change_endian_long(0xc467ca1fu));
    RX65NHU_func314(580+32);
    RX65NHU_func100(change_endian_long(0x6994c793u), change_endian_long(0x337e9747u), change_endian_long(0xe2a0cd55u), change_endian_long(0x21bb9cc0u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xa43d2428u), change_endian_long(0x640f32e1u), change_endian_long(0x687de9bfu), change_endian_long(0x787221deu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX65NHU_func319(616);
    RX65NHU_func100(change_endian_long(0x5b1ef812u), change_endian_long(0xcd1f3757u), change_endian_long(0x5dd299aau), change_endian_long(0xde90e25bu));
    RX65NHU_func314(616+32);
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f3u;
    RX65NHU_func101(change_endian_long(0x59103b8fu), change_endian_long(0xfd543841u), change_endian_long(0x8bbc161au), change_endian_long(0xa1abe88bu));
    RX65NHU_func318();
    RX65NHU_func100(change_endian_long(0x10b9ff70u), change_endian_long(0xb52e02eeu), change_endian_long(0x7dec2249u), change_endian_long(0xe727990fu));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x84992ba5u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x2b0b9e1du), change_endian_long(0x62ce46dbu), change_endian_long(0xf5b372c3u), change_endian_long(0x71994133u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func319(0);
    RX65NHU_func100(change_endian_long(0x30863619u), change_endian_long(0xc5e5186au), change_endian_long(0x90dbb179u), change_endian_long(0x8bbbe358u));
    RX65NHU_func314(0+32);
    RX65NHU_func100(change_endian_long(0xdbcd1d0eu), change_endian_long(0xe91837bcu), change_endian_long(0x5fa5052eu), change_endian_long(0xcdead510u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x064417e5u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x27e612abu), change_endian_long(0xa0ad77a6u), change_endian_long(0x24781303u), change_endian_long(0x6013c1b9u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX65NHU_func319(36);
    RX65NHU_func100(change_endian_long(0x15040acau), change_endian_long(0xb1df5b3cu), change_endian_long(0x2033d40au), change_endian_long(0x9eaa70dbu));
    RX65NHU_func314(36+32);
    RX65NHU_func100(change_endian_long(0x47316907u), change_endian_long(0x6686616cu), change_endian_long(0x1be53759u), change_endian_long(0x44c33e2au));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x49880ba2u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x930ab700u), change_endian_long(0xb9b3dfc2u), change_endian_long(0x0305f751u), change_endian_long(0x52c5939au));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX65NHU_func319(72);
    RX65NHU_func100(change_endian_long(0xd65d6002u), change_endian_long(0x2b07cea5u), change_endian_long(0x0c8f2470u), change_endian_long(0x638004e0u));
    RX65NHU_func314(72+32);
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_ECH.WORD = 0x00003bbeu;
    TSIP.REG_ECH.WORD = 0x00007c1du;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_func324.prc
 *********************************************************************************************************************/
