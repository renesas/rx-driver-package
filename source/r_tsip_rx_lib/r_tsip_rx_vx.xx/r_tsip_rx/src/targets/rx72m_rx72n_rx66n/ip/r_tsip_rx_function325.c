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
* Function Name: RX72M_RX72N_RX66N_func325
*******************************************************************************************************************/ /**
* @details       RX72M func325
* @note          None
*/
void RX72M_RX72N_RX66N_func325(void)
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
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00003fbeu;
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f0u;
    RX72M_RX72N_RX66N_func101(change_endian_long(0xa434335cu), change_endian_long(0x17a18ed1u), change_endian_long(0xa8386276u), change_endian_long(0xc3545863u));
    RX72M_RX72N_RX66N_func324();
    TSIP.REG_ECH.WORD = 0x00000a73u;
    TSIP.REG_ECH.WORD = 0x00000a31u;
    for(jLoop = 0; jLoop < 32; jLoop = jLoop + 1)
    {
        TSIP.REG_ECH.WORD = 0x00002e20u;
        TSIP.REG_ECH.WORD = 0x38002673u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00A60000u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0xfa110da9u), change_endian_long(0x7d66c83bu), change_endian_long(0xe0038fbfu), change_endian_long(0x7e0e68adu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (0u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func101(change_endian_long(0x3b04f8c9u), change_endian_long(0xd7439966u), change_endian_long(0xcc215751u), change_endian_long(0x5fa08452u));
        }
        else
        {
            RX72M_RX72N_RX66N_func100(change_endian_long(0x70d960bcu), change_endian_long(0x54b1cb10u), change_endian_long(0x0b31ee62u), change_endian_long(0x53a06ab6u));
            TSIP.REG_28H.WORD = 0x009f0001u;
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x5f55b804u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x6b391915u), change_endian_long(0x7888d331u), change_endian_long(0xa322da4fu), change_endian_long(0xc06f7b7fu));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            RX72M_RX72N_RX66N_func320(136);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x22880c7eu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x7fe93b4bu), change_endian_long(0x82b86b61u), change_endian_long(0xd2ec8c1au), change_endian_long(0xd5cf4ee7u));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000013u;
            RX72M_RX72N_RX66N_func320(172);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x8e78b588u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x565577e9u), change_endian_long(0x5660d3d5u), change_endian_long(0x1e6b4426u), change_endian_long(0xdeead341u));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            RX72M_RX72N_RX66N_func320(208);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0xff2393b6u), change_endian_long(0xa65cea1eu), change_endian_long(0x9c1ef229u), change_endian_long(0x0c4001ebu));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX72M_RX72N_RX66N_func319(408);
            RX72M_RX72N_RX66N_func100(change_endian_long(0xed9b7f01u), change_endian_long(0x0724a75au), change_endian_long(0x1e2fcca1u), change_endian_long(0x605ffa87u));
            RX72M_RX72N_RX66N_func314(408+32);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x395bd0f2u), change_endian_long(0x9ea1824cu), change_endian_long(0xd133fedfu), change_endian_long(0xfb72bd9cu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x8f94bde3u), change_endian_long(0x3ec34772u), change_endian_long(0xe89018d3u), change_endian_long(0xba5bd15du));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000023u;
            RX72M_RX72N_RX66N_func319(444);
            RX72M_RX72N_RX66N_func100(change_endian_long(0xace3d6bfu), change_endian_long(0x2b165413u), change_endian_long(0x49a29ae1u), change_endian_long(0x690fca8du));
            RX72M_RX72N_RX66N_func314(444+32);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x684117beu), change_endian_long(0x296d1280u), change_endian_long(0x3d122940u), change_endian_long(0x660a0d75u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x79d63913u), change_endian_long(0x25821785u), change_endian_long(0xaae3f62cu), change_endian_long(0x6f64b48eu));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX72M_RX72N_RX66N_func319(480);
            RX72M_RX72N_RX66N_func100(change_endian_long(0xafc8d755u), change_endian_long(0x31948323u), change_endian_long(0x796ebff3u), change_endian_long(0xa0b41e39u));
            RX72M_RX72N_RX66N_func314(480+32);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x6441c723u), change_endian_long(0x10df1b55u), change_endian_long(0x51d66d07u), change_endian_long(0xfa63b9e9u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0xb39db5b3u), change_endian_long(0x9a5419eeu), change_endian_long(0xd5ac67a7u), change_endian_long(0x11f7cd03u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX72M_RX72N_RX66N_func319(544);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x9e061a5fu), change_endian_long(0x366f6aecu), change_endian_long(0x6fc7f502u), change_endian_long(0xfdce3176u));
            RX72M_RX72N_RX66N_func314(544+32);
            RX72M_RX72N_RX66N_func100(change_endian_long(0xea26484eu), change_endian_long(0xe77ec990u), change_endian_long(0x5c255456u), change_endian_long(0x9d5ade94u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x5190070bu), change_endian_long(0xd2866467u), change_endian_long(0xf446ad72u), change_endian_long(0x80a26b9au));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000023u;
            RX72M_RX72N_RX66N_func319(580);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x78fcbffdu), change_endian_long(0x093f3b99u), change_endian_long(0x017bfb72u), change_endian_long(0xbd6889f1u));
            RX72M_RX72N_RX66N_func314(580+32);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x04e06ddbu), change_endian_long(0xf4481586u), change_endian_long(0x385aae53u), change_endian_long(0xa6f36e70u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x802147b0u), change_endian_long(0x774fd04eu), change_endian_long(0x0d386bd8u), change_endian_long(0x6f729deau));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX72M_RX72N_RX66N_func319(616);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x99cc8b34u), change_endian_long(0x6c6981d2u), change_endian_long(0xc54fb1b5u), change_endian_long(0x46e0038bu));
            RX72M_RX72N_RX66N_func314(616+32);
            TSIP.REG_ECH.WORD = 0x0000b7a0u;
            TSIP.REG_ECH.WORD = 0x000000f4u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x085eccceu), change_endian_long(0xcdbeff89u), change_endian_long(0xe197ad27u), change_endian_long(0x0b821e4fu));
            RX72M_RX72N_RX66N_func318();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x4157fd34u), change_endian_long(0xd0c81f8eu), change_endian_long(0xd1d4c874u), change_endian_long(0xb3a5276au));
            TSIP.REG_28H.WORD = 0x009f0001u;
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x5f55b804u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x08341f1bu), change_endian_long(0x7c213606u), change_endian_long(0xe01a785au), change_endian_long(0x15c594f0u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX72M_RX72N_RX66N_func319(136);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x78626c6eu), change_endian_long(0xda44fcb6u), change_endian_long(0xbabb0b67u), change_endian_long(0xbc7de2efu));
            RX72M_RX72N_RX66N_func314(136+32);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x7c9ee454u), change_endian_long(0xd9fbee8bu), change_endian_long(0x7bbfd7a4u), change_endian_long(0x6fb51610u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x22880c7eu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x805824b4u), change_endian_long(0xd02b90e8u), change_endian_long(0xf9c7998bu), change_endian_long(0x4f5bb6d2u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000023u;
            RX72M_RX72N_RX66N_func319(172);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x0b6436dbu), change_endian_long(0x4ff196abu), change_endian_long(0xd6b4b0a4u), change_endian_long(0x04917056u));
            RX72M_RX72N_RX66N_func314(172+32);
            RX72M_RX72N_RX66N_func100(change_endian_long(0xd09c9d83u), change_endian_long(0xd45e3506u), change_endian_long(0xc7d0e1aeu), change_endian_long(0x3ac59d69u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x8e78b588u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x5ae86051u), change_endian_long(0x76ee09e3u), change_endian_long(0xcafde28eu), change_endian_long(0xcd840e33u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX72M_RX72N_RX66N_func319(208);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x458321b2u), change_endian_long(0x677e9cf8u), change_endian_long(0x7edee79fu), change_endian_long(0xa6683d8du));
            RX72M_RX72N_RX66N_func314(208+32);
            RX72M_RX72N_RX66N_func101(change_endian_long(0x2b9e4db1u), change_endian_long(0x7ec57bc2u), change_endian_long(0x9c938faeu), change_endian_long(0x451789ddu));
        }
        TSIP.REG_ECH.WORD = 0x3800da9fu;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00A60000u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0x2ba3b480u), change_endian_long(0x4a883f75u), change_endian_long(0x604f6956u), change_endian_long(0xc640eba7u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (0u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func101(change_endian_long(0xd38110d7u), change_endian_long(0x5bbca081u), change_endian_long(0x47ca7ddbu), change_endian_long(0x765aa4f7u));
        }
        else
        {
            RX72M_RX72N_RX66N_func100(change_endian_long(0xb31d7068u), change_endian_long(0x4e7c062du), change_endian_long(0x78a5a385u), change_endian_long(0x258c8a31u));
            TSIP.REG_28H.WORD = 0x009f0001u;
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x5f55b804u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x723fcb65u), change_endian_long(0xaaf46e7du), change_endian_long(0x1cc97604u), change_endian_long(0x68961e25u));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            RX72M_RX72N_RX66N_func320(136);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x22880c7eu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0xb4e5b17bu), change_endian_long(0x0b8fef26u), change_endian_long(0x625f4a2du), change_endian_long(0xa44fbeabu));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000013u;
            RX72M_RX72N_RX66N_func320(172);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x8e78b588u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0xa38d63a0u), change_endian_long(0xc65d8baau), change_endian_long(0x82bbe67cu), change_endian_long(0x1c6d462au));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            RX72M_RX72N_RX66N_func320(208);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x21f138a6u), change_endian_long(0x76666c29u), change_endian_long(0xd8053f21u), change_endian_long(0x993e9da6u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX72M_RX72N_RX66N_func319(408);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x5e5f9870u), change_endian_long(0xe49170e8u), change_endian_long(0x27486605u), change_endian_long(0xb0464bdbu));
            RX72M_RX72N_RX66N_func314(408+32);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x533621a7u), change_endian_long(0x0bc26339u), change_endian_long(0x99169581u), change_endian_long(0x1851a624u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x69546ee0u), change_endian_long(0x5217aba3u), change_endian_long(0x3f376b96u), change_endian_long(0x560091dfu));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000023u;
            RX72M_RX72N_RX66N_func319(444);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x35719933u), change_endian_long(0x0ea70192u), change_endian_long(0xbd6bad89u), change_endian_long(0xf141887eu));
            RX72M_RX72N_RX66N_func314(444+32);
            RX72M_RX72N_RX66N_func100(change_endian_long(0xe8c8c534u), change_endian_long(0xb50bb6f3u), change_endian_long(0x7ca90ac8u), change_endian_long(0x06e924feu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0xfce6eac4u), change_endian_long(0xf554958bu), change_endian_long(0x45225fa0u), change_endian_long(0x89d8a06bu));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX72M_RX72N_RX66N_func319(480);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x8f6a4108u), change_endian_long(0xbf306c87u), change_endian_long(0x4c9156c7u), change_endian_long(0x4764024fu));
            RX72M_RX72N_RX66N_func314(480+32);
            RX72M_RX72N_RX66N_func100(change_endian_long(0xebe57554u), change_endian_long(0xcb1381ceu), change_endian_long(0x77e62f0bu), change_endian_long(0x07b41a5bu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x84992ba5u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x13e0d907u), change_endian_long(0xb697aeb2u), change_endian_long(0x5376a003u), change_endian_long(0xc8836fccu));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            RX72M_RX72N_RX66N_func320(0);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x064417e5u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0xf6351766u), change_endian_long(0x2a047232u), change_endian_long(0xb9766fe2u), change_endian_long(0x339e615du));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000013u;
            RX72M_RX72N_RX66N_func320(36);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x49880ba2u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x6f4db600u), change_endian_long(0x78cae77du), change_endian_long(0x8f6b7300u), change_endian_long(0xac748b60u));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            RX72M_RX72N_RX66N_func320(72);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0xfc71a908u), change_endian_long(0xd5f219c3u), change_endian_long(0xdb22ecd1u), change_endian_long(0x78079b77u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX72M_RX72N_RX66N_func319(544);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x31b3d813u), change_endian_long(0xad670f87u), change_endian_long(0xbec88e26u), change_endian_long(0x49d123c0u));
            RX72M_RX72N_RX66N_func314(544+32);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x4c42aba4u), change_endian_long(0xb8995fc7u), change_endian_long(0xf89789a1u), change_endian_long(0xdfae4a01u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x123bfadcu), change_endian_long(0xac8b4618u), change_endian_long(0xe0f71a05u), change_endian_long(0x6d40f409u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000023u;
            RX72M_RX72N_RX66N_func319(580);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x2f5b8918u), change_endian_long(0x3345d39cu), change_endian_long(0x736dd176u), change_endian_long(0x887fd010u));
            RX72M_RX72N_RX66N_func314(580+32);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x7333db47u), change_endian_long(0x70fc3fb6u), change_endian_long(0xa0148d90u), change_endian_long(0xd21a8097u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x6237ab35u), change_endian_long(0xe6ebdebbu), change_endian_long(0x4f1b09f6u), change_endian_long(0xc0e57b5cu));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX72M_RX72N_RX66N_func319(616);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x2922a160u), change_endian_long(0x12348291u), change_endian_long(0xef3c3581u), change_endian_long(0xc453b591u));
            RX72M_RX72N_RX66N_func314(616+32);
            TSIP.REG_ECH.WORD = 0x0000b7a0u;
            TSIP.REG_ECH.WORD = 0x000000f5u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0xa4411511u), change_endian_long(0x98b90359u), change_endian_long(0x1ce1d632u), change_endian_long(0x8ce21d84u));
            RX72M_RX72N_RX66N_func318();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x715e8f11u), change_endian_long(0x3389dc02u), change_endian_long(0x65fbfdf9u), change_endian_long(0xc8d9e361u));
            TSIP.REG_28H.WORD = 0x009f0001u;
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x5f55b804u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0xf7f147d4u), change_endian_long(0x8f1621d9u), change_endian_long(0x4f15fd45u), change_endian_long(0xa8530899u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX72M_RX72N_RX66N_func319(136);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x1a7925ccu), change_endian_long(0x148ae13cu), change_endian_long(0x544b6e09u), change_endian_long(0xcc9e1d68u));
            RX72M_RX72N_RX66N_func314(136+32);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x4e8fc4b5u), change_endian_long(0xe617c80fu), change_endian_long(0x3c6b50f5u), change_endian_long(0x27ab6050u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x22880c7eu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x6b9724cbu), change_endian_long(0xfae88631u), change_endian_long(0xc3ffb7bdu), change_endian_long(0x4b20a854u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000023u;
            RX72M_RX72N_RX66N_func319(172);
            RX72M_RX72N_RX66N_func100(change_endian_long(0xb2adb3ecu), change_endian_long(0x392ba789u), change_endian_long(0x85258926u), change_endian_long(0xb35a6864u));
            RX72M_RX72N_RX66N_func314(172+32);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x3deac58fu), change_endian_long(0xb95a90b6u), change_endian_long(0x6b8cf41bu), change_endian_long(0xe5ddd4aau));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x8e78b588u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x1207392eu), change_endian_long(0x1478edd7u), change_endian_long(0xe48cff65u), change_endian_long(0xe552018bu));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX72M_RX72N_RX66N_func319(208);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x79708b70u), change_endian_long(0x37d07336u), change_endian_long(0x1ad72b39u), change_endian_long(0xba00d3bdu));
            RX72M_RX72N_RX66N_func314(208+32);
            TSIP.REG_ECH.WORD = 0x0000d260u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x0525b082u), change_endian_long(0xe062752du), change_endian_long(0x05ba436bu), change_endian_long(0x6f560ba7u));
        }
        TSIP.REG_ECH.WORD = 0x01816e94u;
        RX72M_RX72N_RX66N_func101(change_endian_long(0x58211060u), change_endian_long(0xd56f4373u), change_endian_long(0x2309c7b2u), change_endian_long(0x90ccb024u));
    }
    TSIP.REG_ECH.WORD = 0x38008a20u;
    TSIP.REG_ECH.WORD = 0x00000020u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0xf7be825eu), change_endian_long(0xfd4c9856u), change_endian_long(0xd49f709cu), change_endian_long(0xa1ab5b2bu));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5f55b804u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xdc073785u), change_endian_long(0xd6ffc471u), change_endian_long(0x6316bf6au), change_endian_long(0x228ade15u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX72M_RX72N_RX66N_func320(136);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x22880c7eu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x7849106cu), change_endian_long(0x19ab28bfu), change_endian_long(0x78d2be06u), change_endian_long(0x6031d6deu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000013u;
    RX72M_RX72N_RX66N_func320(172);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8e78b588u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x83cecea4u), change_endian_long(0xf6d0afb5u), change_endian_long(0x7c75746cu), change_endian_long(0x33aa655cu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func320(208);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x712675bdu), change_endian_long(0x9ff4358au), change_endian_long(0x82739d93u), change_endian_long(0xb0caa3bau));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func319(408);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x7948746cu), change_endian_long(0x1d6d63acu), change_endian_long(0xdc3b425du), change_endian_long(0x3560d9d2u));
    RX72M_RX72N_RX66N_func314(408+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe6078ccdu), change_endian_long(0x8f3cc940u), change_endian_long(0xeba92825u), change_endian_long(0x9da4d8c4u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0519f81cu), change_endian_long(0xd57ef309u), change_endian_long(0x5c643e8au), change_endian_long(0xa36e3e87u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX72M_RX72N_RX66N_func319(444);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x88468f73u), change_endian_long(0x12b05c75u), change_endian_long(0xcbe3c80cu), change_endian_long(0xac388910u));
    RX72M_RX72N_RX66N_func314(444+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x90bc9ba5u), change_endian_long(0x6d60d571u), change_endian_long(0xcb5514eeu), change_endian_long(0x08e2d228u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x91c00498u), change_endian_long(0x1e5f4b45u), change_endian_long(0xdf70a927u), change_endian_long(0xcf2486aau));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX72M_RX72N_RX66N_func319(480);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0d2b39aeu), change_endian_long(0xf9da7c1fu), change_endian_long(0x0b2a911fu), change_endian_long(0x06f1a31cu));
    RX72M_RX72N_RX66N_func314(480+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x3920ab70u), change_endian_long(0x38955675u), change_endian_long(0x75e0da25u), change_endian_long(0xf6a7dfc2u));
    TSIP.REG_28H.WORD = 0x00bf0001u;
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
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x79bcabbeu), change_endian_long(0xd37368eau), change_endian_long(0x765c5d7cu), change_endian_long(0x5ce228e2u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX72M_RX72N_RX66N_func319(616);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc36e6649u), change_endian_long(0x4d690ed2u), change_endian_long(0xadadb91du), change_endian_long(0x06d535dbu));
    RX72M_RX72N_RX66N_func314(616+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xa89d586fu), change_endian_long(0xd2cb1471u), change_endian_long(0x004af99cu), change_endian_long(0x5a21f33fu));
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_24H.WORD = 0x00000581u;
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
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xf2c355adu), change_endian_long(0x4a191bf2u), change_endian_long(0xb5a9779du), change_endian_long(0x1bb94f24u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func319(544);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xf003f528u), change_endian_long(0xcf5a3813u), change_endian_long(0x54d4cfe1u), change_endian_long(0x3b1b41fcu));
    RX72M_RX72N_RX66N_func314(544+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0e94ef71u), change_endian_long(0x15bfebc3u), change_endian_long(0x21084f30u), change_endian_long(0xbb02abffu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x904d3ff0u), change_endian_long(0x8bcc0010u), change_endian_long(0x9bed23a5u), change_endian_long(0xf7962db3u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX72M_RX72N_RX66N_func319(580);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x19afcffeu), change_endian_long(0xa9ac4647u), change_endian_long(0xfd115339u), change_endian_long(0xef3a1b9bu));
    RX72M_RX72N_RX66N_func314(580+32);
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f6u;
    RX72M_RX72N_RX66N_func101(change_endian_long(0x0c357d33u), change_endian_long(0x32d12762u), change_endian_long(0x6be37a1cu), change_endian_long(0xc339168eu));
    RX72M_RX72N_RX66N_func318();
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00003bbeu;
    TSIP.REG_ECH.WORD = 0x00007c1du;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX72M/RX72M_func325_r1.prc
 *********************************************************************************************************************/
