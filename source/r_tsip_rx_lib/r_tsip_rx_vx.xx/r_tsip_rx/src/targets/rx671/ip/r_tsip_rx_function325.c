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
* Function Name: RX671_func325
*******************************************************************************************************************/ /**
* @details       RX671 func325
* @note          None
*/
void RX671_func325(void)
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
    RX671_func101(change_endian_long(0xc9b25bd4u), change_endian_long(0x6a2ed5afu), change_endian_long(0x67094462u), change_endian_long(0xfa9cd26eu));
    RX671_func324();
    TSIP.REG_ECH.WORD = 0x00000a73u;
    TSIP.REG_ECH.WORD = 0x00000a31u;
    for(jLoop = 0; jLoop < 32; jLoop = jLoop + 1)
    {
        TSIP.REG_ECH.WORD = 0x00002e20u;
        TSIP.REG_ECH.WORD = 0x38002673u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00A60000u;
        RX671_func100(change_endian_long(0x54f00a98u), change_endian_long(0x8cea2ca1u), change_endian_long(0xc2a4b597u), change_endian_long(0x73a3d570u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (0u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func101(change_endian_long(0xc15256edu), change_endian_long(0xa01278f0u), change_endian_long(0x8def5c63u), change_endian_long(0xb78ddea8u));
        }
        else
        {
            RX671_func100(change_endian_long(0xd6e667c0u), change_endian_long(0x519c2763u), change_endian_long(0x1b8b89c3u), change_endian_long(0x91bd212au));
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
            RX671_func100(change_endian_long(0xff6f3aa5u), change_endian_long(0xf43d4f6eu), change_endian_long(0x1319ac79u), change_endian_long(0xc6adffcbu));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            RX671_func320(136);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x22880c7eu);
            RX671_func080();
            RX671_func100(change_endian_long(0x9e3c8549u), change_endian_long(0x92ccd06bu), change_endian_long(0x0df85848u), change_endian_long(0x021160fdu));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000013u;
            RX671_func320(172);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x8e78b588u);
            RX671_func080();
            RX671_func100(change_endian_long(0x6e0f9c4bu), change_endian_long(0x12f22c4cu), change_endian_long(0xb1813b11u), change_endian_long(0xffd9542fu));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            RX671_func320(208);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
            RX671_func080();
            RX671_func100(change_endian_long(0x0480f94au), change_endian_long(0x2f7a80ecu), change_endian_long(0xef7f7e14u), change_endian_long(0x6fc6ba32u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX671_func319(408);
            RX671_func100(change_endian_long(0xc8f81a8du), change_endian_long(0xd64ee543u), change_endian_long(0x18935ca2u), change_endian_long(0x910547abu));
            RX671_func314(408+32);
            RX671_func100(change_endian_long(0x2bb845b2u), change_endian_long(0xf3a085b8u), change_endian_long(0xa0787d04u), change_endian_long(0xbfd2a637u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
            RX671_func080();
            RX671_func100(change_endian_long(0xfdad32f0u), change_endian_long(0xd8e16f41u), change_endian_long(0xaea217e4u), change_endian_long(0xf05c1d6cu));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000023u;
            RX671_func319(444);
            RX671_func100(change_endian_long(0xb9f459adu), change_endian_long(0xb271949cu), change_endian_long(0x84c56832u), change_endian_long(0xd82d1fb7u));
            RX671_func314(444+32);
            RX671_func100(change_endian_long(0x35a3202eu), change_endian_long(0x41c66137u), change_endian_long(0xf09185b6u), change_endian_long(0x7b4d1880u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
            RX671_func080();
            RX671_func100(change_endian_long(0x1dda38f4u), change_endian_long(0xee8a34c5u), change_endian_long(0xaf771b88u), change_endian_long(0xe03fb7a9u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX671_func319(480);
            RX671_func100(change_endian_long(0x21b20c2au), change_endian_long(0x65985e7au), change_endian_long(0x761b0af3u), change_endian_long(0x08df97e5u));
            RX671_func314(480+32);
            RX671_func100(change_endian_long(0x8c9a5932u), change_endian_long(0x68680e56u), change_endian_long(0x2caf9060u), change_endian_long(0xbee4b6e1u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
            RX671_func080();
            RX671_func100(change_endian_long(0x893534a9u), change_endian_long(0xd4692b02u), change_endian_long(0x007647c3u), change_endian_long(0x94d2c280u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX671_func319(544);
            RX671_func100(change_endian_long(0xf494f4dcu), change_endian_long(0x7e70b3c3u), change_endian_long(0xe96aba50u), change_endian_long(0xfd841268u));
            RX671_func314(544+32);
            RX671_func100(change_endian_long(0x559e0992u), change_endian_long(0xe4f99ee4u), change_endian_long(0x9cfd3adbu), change_endian_long(0x2bced2a5u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
            RX671_func080();
            RX671_func100(change_endian_long(0x8e8256b3u), change_endian_long(0xf2df7931u), change_endian_long(0x7e9f1802u), change_endian_long(0x220078a7u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000023u;
            RX671_func319(580);
            RX671_func100(change_endian_long(0x343f7e4bu), change_endian_long(0x7d5b271bu), change_endian_long(0x57714df0u), change_endian_long(0x23823130u));
            RX671_func314(580+32);
            RX671_func100(change_endian_long(0xf7dc3d0eu), change_endian_long(0xe0087378u), change_endian_long(0xe838751bu), change_endian_long(0x0a7b9d0bu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
            RX671_func080();
            RX671_func100(change_endian_long(0xc1bcefabu), change_endian_long(0xce7c86a9u), change_endian_long(0x3664af85u), change_endian_long(0x0c378b4cu));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX671_func319(616);
            RX671_func100(change_endian_long(0x05c5d6b7u), change_endian_long(0xeeb5bc4fu), change_endian_long(0xa6ee8c66u), change_endian_long(0x4c6b0eb3u));
            RX671_func314(616+32);
            TSIP.REG_ECH.WORD = 0x0000b7a0u;
            TSIP.REG_ECH.WORD = 0x000000f4u;
            RX671_func101(change_endian_long(0x8f19ea94u), change_endian_long(0xb15a8fb3u), change_endian_long(0x3b7be949u), change_endian_long(0xb9d8b3bfu));
            RX671_func318();
            RX671_func100(change_endian_long(0xef5945bcu), change_endian_long(0xc7dc86d4u), change_endian_long(0x9e369641u), change_endian_long(0xebc1fad3u));
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
            RX671_func100(change_endian_long(0x19a6e7abu), change_endian_long(0x4b345233u), change_endian_long(0x0da706f5u), change_endian_long(0xd3ebef05u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX671_func319(136);
            RX671_func100(change_endian_long(0x9f728454u), change_endian_long(0xc46eead2u), change_endian_long(0xbea5a26eu), change_endian_long(0x2d97331eu));
            RX671_func314(136+32);
            RX671_func100(change_endian_long(0xfeec03ebu), change_endian_long(0x47bd8bcdu), change_endian_long(0x97a1f0fbu), change_endian_long(0x5f92fa6fu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x22880c7eu);
            RX671_func080();
            RX671_func100(change_endian_long(0xacbd8077u), change_endian_long(0x928af244u), change_endian_long(0x590e4109u), change_endian_long(0xb1dc7b6fu));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000023u;
            RX671_func319(172);
            RX671_func100(change_endian_long(0xd2baa4cdu), change_endian_long(0x95fc1d6bu), change_endian_long(0x607cb2fbu), change_endian_long(0xa5c54a9au));
            RX671_func314(172+32);
            RX671_func100(change_endian_long(0x48999e4fu), change_endian_long(0xfd8a88acu), change_endian_long(0xb391ec5eu), change_endian_long(0x5d7ee159u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x8e78b588u);
            RX671_func080();
            RX671_func100(change_endian_long(0x055f93c8u), change_endian_long(0xa2fbb2abu), change_endian_long(0x41d2b76cu), change_endian_long(0xb2401d5cu));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX671_func319(208);
            RX671_func100(change_endian_long(0x9ec69b5cu), change_endian_long(0x7cd609bau), change_endian_long(0xaa68bb77u), change_endian_long(0xb20bde5eu));
            RX671_func314(208+32);
            RX671_func101(change_endian_long(0x5467364cu), change_endian_long(0xde349c6eu), change_endian_long(0x312c5918u), change_endian_long(0x3fe5b516u));
        }
        TSIP.REG_ECH.WORD = 0x3800da9fu;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00A60000u;
        RX671_func100(change_endian_long(0x10f2c84bu), change_endian_long(0xc98fd335u), change_endian_long(0xb1a5942du), change_endian_long(0xc62265d8u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (0u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func101(change_endian_long(0x1f3e3b2cu), change_endian_long(0x3b161e64u), change_endian_long(0xa1002019u), change_endian_long(0xb50a01d9u));
        }
        else
        {
            RX671_func100(change_endian_long(0x5e2e48bfu), change_endian_long(0x18fd0eccu), change_endian_long(0xb8a7f789u), change_endian_long(0x5d494b6du));
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
            RX671_func100(change_endian_long(0x26520c79u), change_endian_long(0xc3b862cbu), change_endian_long(0x4ba503beu), change_endian_long(0x06794d48u));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            RX671_func320(136);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x22880c7eu);
            RX671_func080();
            RX671_func100(change_endian_long(0x5927b74cu), change_endian_long(0x5df598e6u), change_endian_long(0x8dac5b07u), change_endian_long(0xad2c5b1du));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000013u;
            RX671_func320(172);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x8e78b588u);
            RX671_func080();
            RX671_func100(change_endian_long(0x19ddd6bfu), change_endian_long(0x87a5924cu), change_endian_long(0x0b9f07c3u), change_endian_long(0x231074d3u));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            RX671_func320(208);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
            RX671_func080();
            RX671_func100(change_endian_long(0x4e12e265u), change_endian_long(0xce79ff3au), change_endian_long(0x34d8583fu), change_endian_long(0x3004fc45u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX671_func319(408);
            RX671_func100(change_endian_long(0x8db4a289u), change_endian_long(0x395f9577u), change_endian_long(0x7706d589u), change_endian_long(0xecb52500u));
            RX671_func314(408+32);
            RX671_func100(change_endian_long(0xcd0a9a83u), change_endian_long(0x9f5256cau), change_endian_long(0x8cde1643u), change_endian_long(0x93684dc8u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
            RX671_func080();
            RX671_func100(change_endian_long(0x7e19afb5u), change_endian_long(0xdf80a8fdu), change_endian_long(0x4a3b931du), change_endian_long(0xc430d487u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000023u;
            RX671_func319(444);
            RX671_func100(change_endian_long(0x4af83200u), change_endian_long(0xfbf54cd2u), change_endian_long(0xa650514bu), change_endian_long(0x85ed54deu));
            RX671_func314(444+32);
            RX671_func100(change_endian_long(0x7eeb3604u), change_endian_long(0x24545babu), change_endian_long(0x120ab89cu), change_endian_long(0xcdbae98cu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
            RX671_func080();
            RX671_func100(change_endian_long(0x0e87f6e5u), change_endian_long(0xb4ce452cu), change_endian_long(0x9e56efa2u), change_endian_long(0x48c156a2u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX671_func319(480);
            RX671_func100(change_endian_long(0x125b5fffu), change_endian_long(0xdcbe6a93u), change_endian_long(0xccd8a277u), change_endian_long(0xffbc0897u));
            RX671_func314(480+32);
            RX671_func100(change_endian_long(0x46a9b5acu), change_endian_long(0xf8b296e8u), change_endian_long(0x8f7498abu), change_endian_long(0x917bf7a5u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x84992ba5u);
            RX671_func080();
            RX671_func100(change_endian_long(0x9661e7dfu), change_endian_long(0x213eafcbu), change_endian_long(0x73d5d562u), change_endian_long(0x30bc2a4au));
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
            RX671_func100(change_endian_long(0x53bc2d91u), change_endian_long(0x53004491u), change_endian_long(0xbaf47992u), change_endian_long(0xc6183e4fu));
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
            RX671_func100(change_endian_long(0xb9d99b15u), change_endian_long(0x55f50cbau), change_endian_long(0x55ef5dc0u), change_endian_long(0x45caf442u));
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
            RX671_func100(change_endian_long(0xd472337eu), change_endian_long(0x679a5d21u), change_endian_long(0xd7e282bcu), change_endian_long(0x2857bae8u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX671_func319(544);
            RX671_func100(change_endian_long(0xa6c08e5eu), change_endian_long(0x5f284006u), change_endian_long(0x7d858b40u), change_endian_long(0x9756f88eu));
            RX671_func314(544+32);
            RX671_func100(change_endian_long(0xb212fb84u), change_endian_long(0xdabdaf00u), change_endian_long(0xdff4d2acu), change_endian_long(0xdec0f076u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
            RX671_func080();
            RX671_func100(change_endian_long(0xd7c5db64u), change_endian_long(0xe96219eau), change_endian_long(0xc172a93fu), change_endian_long(0x8c2e8ccdu));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000023u;
            RX671_func319(580);
            RX671_func100(change_endian_long(0xef91fed1u), change_endian_long(0x73fad7afu), change_endian_long(0xe216e56du), change_endian_long(0xb78ebcadu));
            RX671_func314(580+32);
            RX671_func100(change_endian_long(0xa6d898a0u), change_endian_long(0x2c26b560u), change_endian_long(0xd816aab9u), change_endian_long(0x75a583bau));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
            RX671_func080();
            RX671_func100(change_endian_long(0x9dfae2b5u), change_endian_long(0xaebffe68u), change_endian_long(0x8cea8082u), change_endian_long(0xa51afb2bu));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX671_func319(616);
            RX671_func100(change_endian_long(0xa0f7f519u), change_endian_long(0x7b61ac48u), change_endian_long(0x1376e55du), change_endian_long(0x1e224ab2u));
            RX671_func314(616+32);
            TSIP.REG_ECH.WORD = 0x0000b7a0u;
            TSIP.REG_ECH.WORD = 0x000000f5u;
            RX671_func101(change_endian_long(0xb03d235eu), change_endian_long(0xa3641556u), change_endian_long(0xa4571900u), change_endian_long(0xdf14f152u));
            RX671_func318();
            RX671_func100(change_endian_long(0xfec55aeau), change_endian_long(0xa6658419u), change_endian_long(0xa47fcd97u), change_endian_long(0xea2d9a0bu));
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
            RX671_func100(change_endian_long(0x850db2bbu), change_endian_long(0xe2626d5au), change_endian_long(0x5e5f7b29u), change_endian_long(0xb4564a1cu));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX671_func319(136);
            RX671_func100(change_endian_long(0xf7bb7130u), change_endian_long(0x7abefe91u), change_endian_long(0x4a8505f8u), change_endian_long(0xb017f432u));
            RX671_func314(136+32);
            RX671_func100(change_endian_long(0xff6bba06u), change_endian_long(0x5fcf599eu), change_endian_long(0xc3c68105u), change_endian_long(0x577539ddu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x22880c7eu);
            RX671_func080();
            RX671_func100(change_endian_long(0x7372a236u), change_endian_long(0xeced23a5u), change_endian_long(0x1a96fb36u), change_endian_long(0x85a607eeu));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000023u;
            RX671_func319(172);
            RX671_func100(change_endian_long(0xd90b96bcu), change_endian_long(0x4216e25cu), change_endian_long(0xb84e9f97u), change_endian_long(0x36b50c1bu));
            RX671_func314(172+32);
            RX671_func100(change_endian_long(0x0d213339u), change_endian_long(0x86f093f7u), change_endian_long(0x134b3176u), change_endian_long(0x2edac498u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x8e78b588u);
            RX671_func080();
            RX671_func100(change_endian_long(0x59db5d9bu), change_endian_long(0x35d0418au), change_endian_long(0x12f291c9u), change_endian_long(0xcf207c4au));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX671_func319(208);
            RX671_func100(change_endian_long(0xfc5fffbau), change_endian_long(0xa2f005efu), change_endian_long(0x086accd9u), change_endian_long(0x12f801edu));
            RX671_func314(208+32);
            TSIP.REG_ECH.WORD = 0x0000d260u;
            RX671_func101(change_endian_long(0x51e39f54u), change_endian_long(0x24935da2u), change_endian_long(0xf25e176au), change_endian_long(0x21a141d1u));
        }
        TSIP.REG_ECH.WORD = 0x01816e94u;
        RX671_func101(change_endian_long(0xa810d68eu), change_endian_long(0xfd42b8b1u), change_endian_long(0x407a8e4cu), change_endian_long(0x4cd1616eu));
    }
    TSIP.REG_ECH.WORD = 0x38008a20u;
    TSIP.REG_ECH.WORD = 0x00000020u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    RX671_func100(change_endian_long(0x85b98c61u), change_endian_long(0x1abd7376u), change_endian_long(0xfa98ee00u), change_endian_long(0xfc5b7f1fu));
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
    RX671_func100(change_endian_long(0xefcff561u), change_endian_long(0x9d637437u), change_endian_long(0x92175181u), change_endian_long(0x6c8e0652u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func320(136);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x22880c7eu);
    RX671_func080();
    RX671_func100(change_endian_long(0x662fddbfu), change_endian_long(0x276a2666u), change_endian_long(0xbb4ebe9eu), change_endian_long(0x2024d943u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000013u;
    RX671_func320(172);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8e78b588u);
    RX671_func080();
    RX671_func100(change_endian_long(0xd9b707ceu), change_endian_long(0x4ed49829u), change_endian_long(0x659b703au), change_endian_long(0xc0052d2cu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func320(208);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
    RX671_func080();
    RX671_func100(change_endian_long(0xdacd833bu), change_endian_long(0x50d6ec48u), change_endian_long(0xcbc0ca67u), change_endian_long(0xfd685e09u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func319(408);
    RX671_func100(change_endian_long(0x312909a4u), change_endian_long(0xf8cca752u), change_endian_long(0x6a4e0250u), change_endian_long(0x935dc7aeu));
    RX671_func314(408+32);
    RX671_func100(change_endian_long(0xb1ba6994u), change_endian_long(0x6987387fu), change_endian_long(0x6d68b0fcu), change_endian_long(0xdc98e3b7u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
    RX671_func080();
    RX671_func100(change_endian_long(0x6e079a9du), change_endian_long(0x9771039au), change_endian_long(0xf3f960efu), change_endian_long(0xd12bad63u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX671_func319(444);
    RX671_func100(change_endian_long(0xc5e20f48u), change_endian_long(0x9ce35cc0u), change_endian_long(0xf3ad4445u), change_endian_long(0x32775e4du));
    RX671_func314(444+32);
    RX671_func100(change_endian_long(0x2ef0d25du), change_endian_long(0x0bc23fecu), change_endian_long(0x1b0fcd90u), change_endian_long(0x3fb07e2au));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
    RX671_func080();
    RX671_func100(change_endian_long(0x6db5a182u), change_endian_long(0x642f0dcdu), change_endian_long(0xbac1c4afu), change_endian_long(0x1dc9c719u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func319(480);
    RX671_func100(change_endian_long(0x6b41cfb2u), change_endian_long(0x7fbcd4dcu), change_endian_long(0x093e634cu), change_endian_long(0xe18060ddu));
    RX671_func314(480+32);
    RX671_func100(change_endian_long(0x4fd9c84bu), change_endian_long(0x53078239u), change_endian_long(0xf332f934u), change_endian_long(0x52efb0c2u));
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
    RX671_func080();
    RX671_func100(change_endian_long(0x2fd9c356u), change_endian_long(0xaec68f3au), change_endian_long(0xdfdd260cu), change_endian_long(0x9dc3848cu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func319(616);
    RX671_func100(change_endian_long(0xd9db03edu), change_endian_long(0x94362d7eu), change_endian_long(0x34cf96f9u), change_endian_long(0x21ffeb70u));
    RX671_func314(616+32);
    RX671_func100(change_endian_long(0x67d30e9cu), change_endian_long(0x31dbd272u), change_endian_long(0x86bb1ba7u), change_endian_long(0xc7e30d24u));
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
    RX671_func080();
    RX671_func100(change_endian_long(0x60886389u), change_endian_long(0x546c37a9u), change_endian_long(0x11138dbcu), change_endian_long(0xb76fcc8cu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func319(544);
    RX671_func100(change_endian_long(0x28504ac3u), change_endian_long(0x3ce9d8d7u), change_endian_long(0xf972b7efu), change_endian_long(0xcd7de183u));
    RX671_func314(544+32);
    RX671_func100(change_endian_long(0xf80b0a38u), change_endian_long(0x42714753u), change_endian_long(0x641662e9u), change_endian_long(0x748f52fdu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
    RX671_func080();
    RX671_func100(change_endian_long(0x783d5855u), change_endian_long(0xb895b3f1u), change_endian_long(0xa9d7f6ecu), change_endian_long(0xe1517ea1u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX671_func319(580);
    RX671_func100(change_endian_long(0x4e0919b4u), change_endian_long(0x61b4b404u), change_endian_long(0x4769955du), change_endian_long(0x6e9f7e4bu));
    RX671_func314(580+32);
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f6u;
    RX671_func101(change_endian_long(0x96fb886fu), change_endian_long(0x34a83947u), change_endian_long(0x0cdbc4deu), change_endian_long(0x711eefc7u));
    RX671_func318();
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00003bbeu;
    TSIP.REG_ECH.WORD = 0x00007c1du;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX671/RX671_func325_r1.prc
 *********************************************************************************************************************/
