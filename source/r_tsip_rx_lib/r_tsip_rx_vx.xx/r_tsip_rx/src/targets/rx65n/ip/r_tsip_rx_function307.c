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
* Function Name: RX65NHU_func307
*******************************************************************************************************************/ /**
* @details       RX65NHU func307
* @note          None
*/
void RX65NHU_func307(void)
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
    RX65NHU_func101(change_endian_long(0x9db9229au), change_endian_long(0x58c4bd7eu), change_endian_long(0x8b2205a7u), change_endian_long(0xef44f2ceu));
    RX65NHU_func311();
    TSIP.REG_ECH.WORD = 0x00000a73u;
    TSIP.REG_ECH.WORD = 0x00000a31u;
    for(jLoop = 0; jLoop < 32; jLoop = jLoop + 1)
    {
        TSIP.REG_ECH.WORD = 0x00002e20u;
        TSIP.REG_ECH.WORD = 0x38002673u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00A60000u;
        RX65NHU_func100(change_endian_long(0xc9616128u), change_endian_long(0x7675fafcu), change_endian_long(0xe70da5c7u), change_endian_long(0x92fa19a6u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (0u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func101(change_endian_long(0xd50771dau), change_endian_long(0x44d37301u), change_endian_long(0x9ee5b6dbu), change_endian_long(0x986b2ae1u));
        }
        else
        {
            RX65NHU_func100(change_endian_long(0x56f0fb52u), change_endian_long(0xa96d4e24u), change_endian_long(0x520d9651u), change_endian_long(0xcc02a0e1u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x05e30fedu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x5dbeabe9u), change_endian_long(0x23a48060u), change_endian_long(0xab4efe9au), change_endian_long(0x348d033bu));
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            RX65NHU_func312(204);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xe4ce560bu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0xb4bb918eu), change_endian_long(0x2a4dff77u), change_endian_long(0xd8f2ca0bu), change_endian_long(0x054a79a6u));
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            RX65NHU_func312(136);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x19c34c53u), change_endian_long(0x7cd3828au), change_endian_long(0x1f87a538u), change_endian_long(0x6e116c09u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX65NHU_func313(476);
            RX65NHU_func100(change_endian_long(0x08db7d80u), change_endian_long(0x46efbd14u), change_endian_long(0x2ca7afe3u), change_endian_long(0xf5409f07u));
            RX65NHU_func314(476+64);
            RX65NHU_func100(change_endian_long(0x597f3ca0u), change_endian_long(0x9a3879c6u), change_endian_long(0x38c2ebf2u), change_endian_long(0x2ee09e04u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xea6a1c3du);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x6a4e6912u), change_endian_long(0x17b93861u), change_endian_long(0x80da8351u), change_endian_long(0xe8c2e35cu));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX65NHU_func313(612);
            RX65NHU_func100(change_endian_long(0xc14481d6u), change_endian_long(0x5eee1392u), change_endian_long(0x980462f1u), change_endian_long(0xeeeae682u));
            RX65NHU_func314(612+64);
            RX65NHU_func100(change_endian_long(0xbbcc1af6u), change_endian_long(0x813deac9u), change_endian_long(0x5ca302f3u), change_endian_long(0xb6add62au));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0xcbc2e721u), change_endian_long(0x88fe028eu), change_endian_long(0xdaaa0f8fu), change_endian_long(0x72f230a3u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX65NHU_func313(408);
            RX65NHU_func100(change_endian_long(0x44ef3d78u), change_endian_long(0x659b6dc3u), change_endian_long(0x92882e03u), change_endian_long(0x82a39bc0u));
            RX65NHU_func314(408+64);
            RX65NHU_func100(change_endian_long(0xfdfedbd6u), change_endian_long(0xaa3e3495u), change_endian_long(0x0600f81du), change_endian_long(0xd7e85664u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xae62ddfbu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x9fc174ccu), change_endian_long(0x113fb900u), change_endian_long(0xd13d8fefu), change_endian_long(0x15e2fedau));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX65NHU_func313(544);
            RX65NHU_func100(change_endian_long(0xfc1a7539u), change_endian_long(0x25227034u), change_endian_long(0xeadfa23du), change_endian_long(0xfc7373beu));
            RX65NHU_func314(544+64);
            TSIP.REG_ECH.WORD = 0x0000b400u;
            TSIP.REG_ECH.WORD = 0x00000081u;
            RX65NHU_func101(change_endian_long(0xc6b39fafu), change_endian_long(0xeb433a5eu), change_endian_long(0x84060e73u), change_endian_long(0x34f18f4du));
            RX65NHU_func309();
            RX65NHU_func100(change_endian_long(0x84f2c8e9u), change_endian_long(0xfb4ce69bu), change_endian_long(0x393f727au), change_endian_long(0x1d8ce0c6u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x05e30fedu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x9086e2d6u), change_endian_long(0xb246360bu), change_endian_long(0x67f4d8d7u), change_endian_long(0x1ad5d516u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX65NHU_func313(204);
            RX65NHU_func100(change_endian_long(0x6710364eu), change_endian_long(0x87c81848u), change_endian_long(0x9794129au), change_endian_long(0x08968a56u));
            RX65NHU_func314(204+64);
            RX65NHU_func100(change_endian_long(0xdad87080u), change_endian_long(0xc7e317e6u), change_endian_long(0x888ac2a8u), change_endian_long(0x16ac5caeu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xe4ce560bu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x407af0b7u), change_endian_long(0xfe98af20u), change_endian_long(0xef7600a3u), change_endian_long(0x61a50e30u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX65NHU_func313(136);
            RX65NHU_func100(change_endian_long(0x83303373u), change_endian_long(0x7b5cc267u), change_endian_long(0x0a5d642cu), change_endian_long(0x4b0c8522u));
            RX65NHU_func314(136+64);
            RX65NHU_func101(change_endian_long(0x6da64c1au), change_endian_long(0xac6e8d60u), change_endian_long(0x51851745u), change_endian_long(0xaca051efu));
        }
        TSIP.REG_ECH.WORD = 0x3800da9fu;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00A60000u;
        RX65NHU_func100(change_endian_long(0x6276fc7du), change_endian_long(0xe761e952u), change_endian_long(0xb8d56380u), change_endian_long(0x40f2db77u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (0u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func101(change_endian_long(0xc65ede29u), change_endian_long(0x9a2e2e63u), change_endian_long(0x46f7cc25u), change_endian_long(0xf4928dfau));
        }
        else
        {
            RX65NHU_func100(change_endian_long(0x28513579u), change_endian_long(0xbcbf093fu), change_endian_long(0x915efc89u), change_endian_long(0xa6b3b415u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x05e30fedu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0xcb475a40u), change_endian_long(0xdfeb39efu), change_endian_long(0x74b0c2f2u), change_endian_long(0x9df2361au));
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            RX65NHU_func312(204);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xe4ce560bu);
            RX65NHU_func080();
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            RX65NHU_func312(136);
            RX65NHU_func100(change_endian_long(0x7908e038u), change_endian_long(0x9a67e716u), change_endian_long(0xd1b401a4u), change_endian_long(0x483a3045u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x2154d2eau), change_endian_long(0x7b22f6afu), change_endian_long(0x76a05620u), change_endian_long(0x3acfb92cu));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX65NHU_func313(476);
            RX65NHU_func100(change_endian_long(0x94791f4cu), change_endian_long(0xffac2d33u), change_endian_long(0x28d5639fu), change_endian_long(0x302d8c3cu));
            RX65NHU_func314(476+64);
            RX65NHU_func100(change_endian_long(0x41f316ecu), change_endian_long(0xeebb21aau), change_endian_long(0x37dfd70du), change_endian_long(0x926a84f3u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x186ddb9eu), change_endian_long(0xf10125d8u), change_endian_long(0xf9ef90f5u), change_endian_long(0xbc16abf5u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX65NHU_func313(408);
            RX65NHU_func100(change_endian_long(0x6cd582cbu), change_endian_long(0x3f580a34u), change_endian_long(0xd1a63fe7u), change_endian_long(0x69c8a5acu));
            RX65NHU_func314(408+64);
            RX65NHU_func100(change_endian_long(0x1d8bc5f0u), change_endian_long(0x03d2432fu), change_endian_long(0x7b8d238au), change_endian_long(0x33dc4bc2u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x5dd049feu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x0f09c24eu), change_endian_long(0xcf951b85u), change_endian_long(0xad4ac7c8u), change_endian_long(0x523cd80fu));
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            RX65NHU_func312(68);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x945d2158u);
            RX65NHU_func080();
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            RX65NHU_func312(0);
            RX65NHU_func100(change_endian_long(0x23fb21ddu), change_endian_long(0xfc6a56a5u), change_endian_long(0x9e90fc62u), change_endian_long(0xfa80d5d0u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xea6a1c3du);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x4de1a83cu), change_endian_long(0x110eef46u), change_endian_long(0x42e39fa2u), change_endian_long(0x4e4f84deu));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX65NHU_func313(612);
            RX65NHU_func100(change_endian_long(0x963c639bu), change_endian_long(0x57cb56e3u), change_endian_long(0x1d332e75u), change_endian_long(0x13778f08u));
            RX65NHU_func314(612+64);
            RX65NHU_func100(change_endian_long(0x29fdb9bdu), change_endian_long(0xa8257b06u), change_endian_long(0x5ac47aa6u), change_endian_long(0x2658238fu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xae62ddfbu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0xeebadb77u), change_endian_long(0x168c15c5u), change_endian_long(0xadef4d8cu), change_endian_long(0x1f132bf6u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX65NHU_func313(544);
            RX65NHU_func100(change_endian_long(0x49d25f2fu), change_endian_long(0x5f14c53au), change_endian_long(0xc0dbc9a9u), change_endian_long(0x4f1e1f50u));
            RX65NHU_func314(544+64);
            TSIP.REG_ECH.WORD = 0x0000b400u;
            TSIP.REG_ECH.WORD = 0x00000082u;
            RX65NHU_func101(change_endian_long(0x7d1f718bu), change_endian_long(0x552595a6u), change_endian_long(0x77311e4bu), change_endian_long(0x5fa2bc49u));
            RX65NHU_func309();
            RX65NHU_func100(change_endian_long(0xbb8c6546u), change_endian_long(0x64dcccd1u), change_endian_long(0x4499613fu), change_endian_long(0xf0d43f37u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x05e30fedu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x097efbc3u), change_endian_long(0x9c042965u), change_endian_long(0xd4fe4a34u), change_endian_long(0xe1be85f9u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX65NHU_func313(204);
            RX65NHU_func100(change_endian_long(0xd9f33ca8u), change_endian_long(0x53c4f3fcu), change_endian_long(0x9d928493u), change_endian_long(0x343d6971u));
            RX65NHU_func314(204+64);
            RX65NHU_func100(change_endian_long(0x1223a603u), change_endian_long(0x072368bfu), change_endian_long(0xb282297fu), change_endian_long(0x3e061056u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xe4ce560bu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0xd16b36d8u), change_endian_long(0xc7e01e06u), change_endian_long(0x1b85cea1u), change_endian_long(0xecf26990u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX65NHU_func313(136);
            RX65NHU_func100(change_endian_long(0x76501eadu), change_endian_long(0x7d85bcdcu), change_endian_long(0x3580d469u), change_endian_long(0x3f70d060u));
            RX65NHU_func314(136+64);
            TSIP.REG_ECH.WORD = 0x0000d260u;
            RX65NHU_func101(change_endian_long(0xbef675e8u), change_endian_long(0x48e81a00u), change_endian_long(0xe27e869eu), change_endian_long(0x4acad6e0u));
        }
        TSIP.REG_ECH.WORD = 0x01816e94u;
        RX65NHU_func101(change_endian_long(0xdf700114u), change_endian_long(0x90de3b37u), change_endian_long(0xb72afb4du), change_endian_long(0xb54b7bb4u));
    }
    TSIP.REG_ECH.WORD = 0x38008a20u;
    TSIP.REG_ECH.WORD = 0x00000020u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    RX65NHU_func100(change_endian_long(0x7228d1bbu), change_endian_long(0x0aad3c85u), change_endian_long(0x6d037efau), change_endian_long(0xf1a8e71du));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x05e30fedu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x78e3b967u), change_endian_long(0x5495e8ccu), change_endian_long(0x2ef1be7eu), change_endian_long(0x7d239882u));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func312(204);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xe4ce560bu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x5108c4cbu), change_endian_long(0xada86779u), change_endian_long(0x9644b79du), change_endian_long(0x5d18da4cu));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX65NHU_func312(136);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x197176f4u), change_endian_long(0x43a34b2au), change_endian_long(0x168c0a87u), change_endian_long(0x5ba6cee1u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX65NHU_func313(476);
    RX65NHU_func100(change_endian_long(0xfa2d3d04u), change_endian_long(0x41b5bf2au), change_endian_long(0x2dd98fa4u), change_endian_long(0x593e4daeu));
    RX65NHU_func314(476+64);
    RX65NHU_func100(change_endian_long(0x822d563au), change_endian_long(0x528ab9b6u), change_endian_long(0xcb0f8246u), change_endian_long(0x7636487fu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x7e923a38u), change_endian_long(0x0717204au), change_endian_long(0x79cd1f8cu), change_endian_long(0xdb919321u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func313(408);
    RX65NHU_func100(change_endian_long(0x8d0aa113u), change_endian_long(0x7d657f48u), change_endian_long(0xde378680u), change_endian_long(0x7b7b9c44u));
    RX65NHU_func314(408+64);
    RX65NHU_func100(change_endian_long(0xc5325ce5u), change_endian_long(0xcf4ef177u), change_endian_long(0x1cc6ec1du), change_endian_long(0x1762e9f5u));
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
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xcd86849cu), change_endian_long(0x3fdf07b0u), change_endian_long(0x93419cefu), change_endian_long(0xfbe6096au));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX65NHU_func313(612);
    RX65NHU_func100(change_endian_long(0xbf15c3dau), change_endian_long(0x47f88bfcu), change_endian_long(0x941b49c8u), change_endian_long(0x2022aa90u));
    RX65NHU_func314(612+64);
    RX65NHU_func100(change_endian_long(0x53f8b6e1u), change_endian_long(0x8c7b5b47u), change_endian_long(0x1b59bfbdu), change_endian_long(0xc326003cu));
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
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x78bae54fu), change_endian_long(0x9de97794u), change_endian_long(0x1b72802au), change_endian_long(0x8c43e084u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func313(544);
    RX65NHU_func100(change_endian_long(0x0a17caedu), change_endian_long(0x2c4c394cu), change_endian_long(0x50310934u), change_endian_long(0x8f2f0fcfu));
    RX65NHU_func314(544+64);
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000083u;
    RX65NHU_func101(change_endian_long(0xa578ecaeu), change_endian_long(0x7a7a95b8u), change_endian_long(0x13c5e1ebu), change_endian_long(0x8ddf262au));
    RX65NHU_func309();
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000090u;
    TSIP.REG_ECH.WORD = 0x0000381eu;
    TSIP.REG_ECH.WORD = 0x00007c00u;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_func307_r1.prc
 *********************************************************************************************************************/
