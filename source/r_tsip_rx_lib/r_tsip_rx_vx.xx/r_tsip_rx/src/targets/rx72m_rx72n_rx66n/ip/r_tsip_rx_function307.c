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
* Function Name: RX72M_RX72N_RX66N_func307
*******************************************************************************************************************/ /**
* @details       RX72M func307
* @note          None
*/
void RX72M_RX72N_RX66N_func307(void)
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
    TSIP.REG_ECH.WORD = 0x00000090u;
    TSIP.REG_ECH.WORD = 0x00003c1eu;
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    RX72M_RX72N_RX66N_func101(change_endian_long(0x3dff79fau), change_endian_long(0x2fdef22du), change_endian_long(0xaa131764u), change_endian_long(0x9332d48au));
    RX72M_RX72N_RX66N_func311();
    TSIP.REG_ECH.WORD = 0x00000a73u;
    TSIP.REG_ECH.WORD = 0x00000a31u;
    for(jLoop = 0; jLoop < 32; jLoop = jLoop + 1)
    {
        TSIP.REG_ECH.WORD = 0x00002e20u;
        TSIP.REG_ECH.WORD = 0x38002673u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00A60000u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0x5b7d8decu), change_endian_long(0x4538171fu), change_endian_long(0x9534abe2u), change_endian_long(0x906e4dd6u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (0u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func101(change_endian_long(0x5c0d46d2u), change_endian_long(0xa93f89b6u), change_endian_long(0x7012a79bu), change_endian_long(0xefe8f606u));
        }
        else
        {
            RX72M_RX72N_RX66N_func100(change_endian_long(0x3bb6dc47u), change_endian_long(0x6382784du), change_endian_long(0xcff4c91du), change_endian_long(0x89b69026u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x05e30fedu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x977bca29u), change_endian_long(0x396997f6u), change_endian_long(0x0acf4c77u), change_endian_long(0xcd064b7fu));
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            RX72M_RX72N_RX66N_func312(204);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xe4ce560bu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x10f4df5cu), change_endian_long(0xd3c5609au), change_endian_long(0x9a59d001u), change_endian_long(0xe05e7e96u));
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            RX72M_RX72N_RX66N_func312(136);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0xe590eb04u), change_endian_long(0x5bbb1b1fu), change_endian_long(0x3e8eaec1u), change_endian_long(0xa2ef8524u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX72M_RX72N_RX66N_func313(476);
            RX72M_RX72N_RX66N_func100(change_endian_long(0xef39a3f8u), change_endian_long(0x7c61515au), change_endian_long(0xb160571eu), change_endian_long(0xdedc4427u));
            RX72M_RX72N_RX66N_func314(476+64);
            RX72M_RX72N_RX66N_func100(change_endian_long(0xa2e6713du), change_endian_long(0xf31aa1d2u), change_endian_long(0x8e9f4e87u), change_endian_long(0x6750900cu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xea6a1c3du);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x14216bd5u), change_endian_long(0x0fe2846bu), change_endian_long(0x935c6d76u), change_endian_long(0xfccab804u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX72M_RX72N_RX66N_func313(612);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x1b2aff27u), change_endian_long(0xdd8be976u), change_endian_long(0xa7ab4ec5u), change_endian_long(0xfb33a3f4u));
            RX72M_RX72N_RX66N_func314(612+64);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x17bc0aaau), change_endian_long(0x3f63c218u), change_endian_long(0xb0cc15d3u), change_endian_long(0x03f1957fu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x6b6a0000u), change_endian_long(0x98291187u), change_endian_long(0x485eeb2cu), change_endian_long(0xae25fc9cu));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX72M_RX72N_RX66N_func313(408);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x87c1f4ebu), change_endian_long(0x4a1bd69bu), change_endian_long(0xcd224d8fu), change_endian_long(0x115090a5u));
            RX72M_RX72N_RX66N_func314(408+64);
            RX72M_RX72N_RX66N_func100(change_endian_long(0xa250f731u), change_endian_long(0x5f41ccc7u), change_endian_long(0x3ae682efu), change_endian_long(0x7d685f41u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xae62ddfbu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0xbeac25f5u), change_endian_long(0x08b37d62u), change_endian_long(0x8468e3fau), change_endian_long(0xf498f783u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX72M_RX72N_RX66N_func313(544);
            RX72M_RX72N_RX66N_func100(change_endian_long(0xf4972ecbu), change_endian_long(0x44d88b6au), change_endian_long(0x0fbc3c3au), change_endian_long(0x9f7f2ad2u));
            RX72M_RX72N_RX66N_func314(544+64);
            TSIP.REG_ECH.WORD = 0x0000b400u;
            TSIP.REG_ECH.WORD = 0x00000081u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x6e8c7a70u), change_endian_long(0x4c38ee56u), change_endian_long(0x53feb854u), change_endian_long(0x366b41f0u));
            RX72M_RX72N_RX66N_func309();
            RX72M_RX72N_RX66N_func100(change_endian_long(0xb147d9d1u), change_endian_long(0xb64fa7bcu), change_endian_long(0x37b4d838u), change_endian_long(0x7b284c4cu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x05e30fedu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0xb3db5e3au), change_endian_long(0x36748eb5u), change_endian_long(0xdcba9920u), change_endian_long(0x6fd8545fu));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX72M_RX72N_RX66N_func313(204);
            RX72M_RX72N_RX66N_func100(change_endian_long(0xa46ed7f2u), change_endian_long(0x6f15f8dbu), change_endian_long(0x4a5c4914u), change_endian_long(0xa047bbf9u));
            RX72M_RX72N_RX66N_func314(204+64);
            RX72M_RX72N_RX66N_func100(change_endian_long(0xb233df88u), change_endian_long(0xbf6b810bu), change_endian_long(0x09682e8au), change_endian_long(0x23f8aeecu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xe4ce560bu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0xe4328dadu), change_endian_long(0x035f8265u), change_endian_long(0x44781b76u), change_endian_long(0xe2a66156u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX72M_RX72N_RX66N_func313(136);
            RX72M_RX72N_RX66N_func100(change_endian_long(0xf4bdc238u), change_endian_long(0xdfc73542u), change_endian_long(0xc9403650u), change_endian_long(0x9756f44fu));
            RX72M_RX72N_RX66N_func314(136+64);
            RX72M_RX72N_RX66N_func101(change_endian_long(0xe4dfb496u), change_endian_long(0xc00ba557u), change_endian_long(0xb9e11322u), change_endian_long(0xda4d4385u));
        }
        TSIP.REG_ECH.WORD = 0x3800da9fu;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00A60000u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0xd5736bdcu), change_endian_long(0x4a996fadu), change_endian_long(0xdd19b08au), change_endian_long(0x60dfd3f5u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (0u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func101(change_endian_long(0x334eb780u), change_endian_long(0xfc3850ccu), change_endian_long(0xa4021a50u), change_endian_long(0x8d82bae8u));
        }
        else
        {
            RX72M_RX72N_RX66N_func100(change_endian_long(0x73b75937u), change_endian_long(0xfd8ef1ebu), change_endian_long(0x7884e48bu), change_endian_long(0x57dca56du));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x05e30fedu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x6d2cfb0eu), change_endian_long(0x0e4c9a97u), change_endian_long(0x9d0b5fe8u), change_endian_long(0xc9fd6830u));
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            RX72M_RX72N_RX66N_func312(204);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xe4ce560bu);
            RX72M_RX72N_RX66N_func080();
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            RX72M_RX72N_RX66N_func312(136);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x39b9f1feu), change_endian_long(0xbf15e95du), change_endian_long(0x0744968eu), change_endian_long(0x728739dfu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x9dfb6aa5u), change_endian_long(0x5c988983u), change_endian_long(0x38b39968u), change_endian_long(0x6c58e530u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX72M_RX72N_RX66N_func313(476);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x8e7362dfu), change_endian_long(0xc4c14f8au), change_endian_long(0xe7a3d97du), change_endian_long(0xbf194a74u));
            RX72M_RX72N_RX66N_func314(476+64);
            RX72M_RX72N_RX66N_func100(change_endian_long(0xc5400947u), change_endian_long(0x176c27a1u), change_endian_long(0x9324225du), change_endian_long(0x79887821u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x9b43fedau), change_endian_long(0x0da4c7eau), change_endian_long(0x7c45a416u), change_endian_long(0xba2c1ae5u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX72M_RX72N_RX66N_func313(408);
            RX72M_RX72N_RX66N_func100(change_endian_long(0xd02ee208u), change_endian_long(0xf38e73c1u), change_endian_long(0x8796a438u), change_endian_long(0xbef68b47u));
            RX72M_RX72N_RX66N_func314(408+64);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x03603477u), change_endian_long(0xf5c0f24eu), change_endian_long(0x9ba825a9u), change_endian_long(0xe0150c23u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x5dd049feu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0xcbb79691u), change_endian_long(0xceeb3294u), change_endian_long(0x9f57ad7bu), change_endian_long(0xe3fccc08u));
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            RX72M_RX72N_RX66N_func312(68);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x945d2158u);
            RX72M_RX72N_RX66N_func080();
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            RX72M_RX72N_RX66N_func312(0);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x9bfab961u), change_endian_long(0x409fb3eau), change_endian_long(0xb3adc958u), change_endian_long(0xcc41c29au));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xea6a1c3du);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x181d73c4u), change_endian_long(0x8f200308u), change_endian_long(0xdbb46c33u), change_endian_long(0x0f2fbc60u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX72M_RX72N_RX66N_func313(612);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x1cfe6041u), change_endian_long(0x3482d56du), change_endian_long(0x4f5bfd13u), change_endian_long(0x988e52bau));
            RX72M_RX72N_RX66N_func314(612+64);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x02073244u), change_endian_long(0xc25bfc49u), change_endian_long(0xb3dc5a63u), change_endian_long(0x8cb4ce24u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xae62ddfbu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x25af64d1u), change_endian_long(0x19936ce3u), change_endian_long(0xab8d01d1u), change_endian_long(0x538945c8u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX72M_RX72N_RX66N_func313(544);
            RX72M_RX72N_RX66N_func100(change_endian_long(0xbbb747f0u), change_endian_long(0x97984bc9u), change_endian_long(0x6babc641u), change_endian_long(0xa6d44d57u));
            RX72M_RX72N_RX66N_func314(544+64);
            TSIP.REG_ECH.WORD = 0x0000b400u;
            TSIP.REG_ECH.WORD = 0x00000082u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x15fcffaeu), change_endian_long(0x5864cc36u), change_endian_long(0xa719eec4u), change_endian_long(0x41e5439du));
            RX72M_RX72N_RX66N_func309();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x8bb68107u), change_endian_long(0xfa29ad9fu), change_endian_long(0x8ef4ea7bu), change_endian_long(0x574d9db5u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x05e30fedu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0x3ba269aeu), change_endian_long(0xdd5ec8b5u), change_endian_long(0x0539d6b2u), change_endian_long(0x0990d080u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX72M_RX72N_RX66N_func313(204);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x397ed66fu), change_endian_long(0x51764557u), change_endian_long(0x6856e32du), change_endian_long(0x5dffbfb0u));
            RX72M_RX72N_RX66N_func314(204+64);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x79130df8u), change_endian_long(0x9611e585u), change_endian_long(0x8b7455a6u), change_endian_long(0x487f8bc3u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xe4ce560bu);
            RX72M_RX72N_RX66N_func080();
            RX72M_RX72N_RX66N_func100(change_endian_long(0xe03dbda0u), change_endian_long(0xf4a83260u), change_endian_long(0x7d0e20bau), change_endian_long(0xb77fbc1bu));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX72M_RX72N_RX66N_func313(136);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x3cfd1844u), change_endian_long(0xf2c92258u), change_endian_long(0xfb3ce72fu), change_endian_long(0xe2c6d030u));
            RX72M_RX72N_RX66N_func314(136+64);
            TSIP.REG_ECH.WORD = 0x0000d260u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0xb1f4e90bu), change_endian_long(0xaea07504u), change_endian_long(0xe441bb9fu), change_endian_long(0xb2943f3du));
        }
        TSIP.REG_ECH.WORD = 0x01816e94u;
        RX72M_RX72N_RX66N_func101(change_endian_long(0xbae920cbu), change_endian_long(0xf0cada52u), change_endian_long(0xbd0ea9efu), change_endian_long(0x9839490bu));
    }
    TSIP.REG_ECH.WORD = 0x38008a20u;
    TSIP.REG_ECH.WORD = 0x00000020u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x4337398fu), change_endian_long(0x22251e8bu), change_endian_long(0x5fad4ce2u), change_endian_long(0xf3e4ee12u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x05e30fedu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x57e22c17u), change_endian_long(0x6fe50506u), change_endian_long(0xf2917b42u), change_endian_long(0xe7e2e46au));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func312(204);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xe4ce560bu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x54c4def8u), change_endian_long(0x559c960fu), change_endian_long(0x00e0fd58u), change_endian_long(0x67c4c5eeu));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX72M_RX72N_RX66N_func312(136);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xf3173ceau), change_endian_long(0xf85bc6b8u), change_endian_long(0xc85b938fu), change_endian_long(0x37511db6u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX72M_RX72N_RX66N_func313(476);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0bac8d52u), change_endian_long(0x3d9e7e8bu), change_endian_long(0xadd96b20u), change_endian_long(0xfa46370au));
    RX72M_RX72N_RX66N_func314(476+64);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x1f27dde5u), change_endian_long(0xc0f91436u), change_endian_long(0x3a4bf40cu), change_endian_long(0xc2cfef2cu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x48555dc3u), change_endian_long(0xf54623e7u), change_endian_long(0x78ef665au), change_endian_long(0xd68732d1u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func313(408);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x24d7a7b1u), change_endian_long(0x258d4e8au), change_endian_long(0xf6614762u), change_endian_long(0x5ec82c7fu));
    RX72M_RX72N_RX66N_func314(408+64);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x6309f838u), change_endian_long(0x6b3e5fb4u), change_endian_long(0xe199b988u), change_endian_long(0x39531503u));
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
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xea6a1c3du);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x323aa36au), change_endian_long(0x94d1f3deu), change_endian_long(0x900e49f6u), change_endian_long(0x1292d24bu));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX72M_RX72N_RX66N_func313(612);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xa9ec8224u), change_endian_long(0xed636696u), change_endian_long(0x666eb0f8u), change_endian_long(0xecf27862u));
    RX72M_RX72N_RX66N_func314(612+64);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x9156309du), change_endian_long(0xd59917a8u), change_endian_long(0x4a3c88e6u), change_endian_long(0x8f01c4dbu));
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
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xae62ddfbu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xf2deda2fu), change_endian_long(0xc5d1ca61u), change_endian_long(0xd5faa986u), change_endian_long(0xddf41e14u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func313(544);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x3b7929cau), change_endian_long(0x94bbdccfu), change_endian_long(0xd810e1c3u), change_endian_long(0xee0fe39cu));
    RX72M_RX72N_RX66N_func314(544+64);
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000083u;
    RX72M_RX72N_RX66N_func101(change_endian_long(0x26751533u), change_endian_long(0x15c6be30u), change_endian_long(0xc9dc9a73u), change_endian_long(0x49b68f54u));
    RX72M_RX72N_RX66N_func309();
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000090u;
    TSIP.REG_ECH.WORD = 0x0000381eu;
    TSIP.REG_ECH.WORD = 0x00007c00u;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX72M/RX72M_func307_r1.prc
 *********************************************************************************************************************/
