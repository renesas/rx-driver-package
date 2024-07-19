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
* Function Name: RX65NHU_func325
*******************************************************************************************************************/ /**
* @details       RX65NHU func325
* @note          None
*/
void RX65NHU_func325(void)
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
    RX65NHU_func101(change_endian_long(0x41611d52u), change_endian_long(0x62b1e5e6u), change_endian_long(0x0c3f4734u), change_endian_long(0xd2d6644au));
    RX65NHU_func324();
    TSIP.REG_ECH.WORD = 0x00000a73u;
    TSIP.REG_ECH.WORD = 0x00000a31u;
    for(jLoop = 0; jLoop < 32; jLoop = jLoop + 1)
    {
        TSIP.REG_ECH.WORD = 0x00002e20u;
        TSIP.REG_ECH.WORD = 0x38002673u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00A60000u;
        RX65NHU_func100(change_endian_long(0xa568fb39u), change_endian_long(0xcf469517u), change_endian_long(0x52d34f38u), change_endian_long(0xdf28bfeau));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (0u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func101(change_endian_long(0xac92b760u), change_endian_long(0x2aa2c38au), change_endian_long(0xb0f87bbcu), change_endian_long(0x89cffbb2u));
        }
        else
        {
            RX65NHU_func100(change_endian_long(0xf0448f89u), change_endian_long(0x9d3ea765u), change_endian_long(0x87aa9f5au), change_endian_long(0x7cb41685u));
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
            RX65NHU_func100(change_endian_long(0xdb86538au), change_endian_long(0x377f57ceu), change_endian_long(0xd68e63f5u), change_endian_long(0xc1de74d8u));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            RX65NHU_func320(136);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x22880c7eu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x5229f841u), change_endian_long(0x35357601u), change_endian_long(0x603c0f0au), change_endian_long(0x8096aac6u));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000013u;
            RX65NHU_func320(172);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x8e78b588u);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x159a3e99u), change_endian_long(0xbdfd1e97u), change_endian_long(0x612ea8ffu), change_endian_long(0x4ae83b22u));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            RX65NHU_func320(208);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x1ce2dbd0u), change_endian_long(0x2b67d918u), change_endian_long(0xeac8c479u), change_endian_long(0x6e9782fau));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX65NHU_func319(408);
            RX65NHU_func100(change_endian_long(0xc86b4380u), change_endian_long(0xa95000e2u), change_endian_long(0xedd3e9bbu), change_endian_long(0x70553557u));
            RX65NHU_func314(408+32);
            RX65NHU_func100(change_endian_long(0x12865b91u), change_endian_long(0x478e84f5u), change_endian_long(0xaa206b10u), change_endian_long(0xa67a6396u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0xbbde3e51u), change_endian_long(0xdbb53a63u), change_endian_long(0x880da53eu), change_endian_long(0xfa01e7a3u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000023u;
            RX65NHU_func319(444);
            RX65NHU_func100(change_endian_long(0x91b442e6u), change_endian_long(0x80a3ba82u), change_endian_long(0x5c8078d6u), change_endian_long(0x52366fa8u));
            RX65NHU_func314(444+32);
            RX65NHU_func100(change_endian_long(0x03da079au), change_endian_long(0x05002227u), change_endian_long(0xf0dbccb6u), change_endian_long(0xdff2ff2du));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0xca34c6b2u), change_endian_long(0xfe2d9731u), change_endian_long(0x0df72d8du), change_endian_long(0x6615c5e3u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX65NHU_func319(480);
            RX65NHU_func100(change_endian_long(0x93688b98u), change_endian_long(0xa39ae6e0u), change_endian_long(0x4e2a7018u), change_endian_long(0xe8fec794u));
            RX65NHU_func314(480+32);
            RX65NHU_func100(change_endian_long(0x417529c5u), change_endian_long(0xd377674du), change_endian_long(0x1007763eu), change_endian_long(0x8fd50f23u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x6267d173u), change_endian_long(0x8299f553u), change_endian_long(0xb0398902u), change_endian_long(0xbceeb5e3u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX65NHU_func319(544);
            RX65NHU_func100(change_endian_long(0x4f002f53u), change_endian_long(0x4ec48859u), change_endian_long(0x8a30cc09u), change_endian_long(0x1637a865u));
            RX65NHU_func314(544+32);
            RX65NHU_func100(change_endian_long(0x94dfc4e7u), change_endian_long(0x99356000u), change_endian_long(0x4c8d05b3u), change_endian_long(0xa1beeef6u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0xad3022afu), change_endian_long(0x5152c9ccu), change_endian_long(0xb6920248u), change_endian_long(0x5940702du));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000023u;
            RX65NHU_func319(580);
            RX65NHU_func100(change_endian_long(0xf0d253b6u), change_endian_long(0x5d5d3c7fu), change_endian_long(0xf8dabadfu), change_endian_long(0xdae09923u));
            RX65NHU_func314(580+32);
            RX65NHU_func100(change_endian_long(0xbdad63a0u), change_endian_long(0x491e9997u), change_endian_long(0x74d59eb7u), change_endian_long(0x1a8615f5u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x33e424f6u), change_endian_long(0x2b970499u), change_endian_long(0xda5fcc88u), change_endian_long(0x7508ff07u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX65NHU_func319(616);
            RX65NHU_func100(change_endian_long(0xc9dcb581u), change_endian_long(0xf1f9609au), change_endian_long(0x11b932bfu), change_endian_long(0x920023d7u));
            RX65NHU_func314(616+32);
            TSIP.REG_ECH.WORD = 0x0000b7a0u;
            TSIP.REG_ECH.WORD = 0x000000f4u;
            RX65NHU_func101(change_endian_long(0x0f5873b9u), change_endian_long(0xde7133dau), change_endian_long(0xcaf89bd5u), change_endian_long(0xb5c82918u));
            RX65NHU_func318();
            RX65NHU_func100(change_endian_long(0xb3096aebu), change_endian_long(0x3564a32fu), change_endian_long(0x06a81c6eu), change_endian_long(0xc525eb15u));
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
            RX65NHU_func100(change_endian_long(0x15032346u), change_endian_long(0xfb2a4c09u), change_endian_long(0xac17164bu), change_endian_long(0xd64cac2eu));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX65NHU_func319(136);
            RX65NHU_func100(change_endian_long(0x3c575df9u), change_endian_long(0xebd89d9bu), change_endian_long(0xe9902e32u), change_endian_long(0xc459338cu));
            RX65NHU_func314(136+32);
            RX65NHU_func100(change_endian_long(0xf67cee4bu), change_endian_long(0x6e4ac650u), change_endian_long(0x09a7659eu), change_endian_long(0x8f96382fu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x22880c7eu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x244351e4u), change_endian_long(0xad6bcfd8u), change_endian_long(0x1338fbf8u), change_endian_long(0xd00425dau));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000023u;
            RX65NHU_func319(172);
            RX65NHU_func100(change_endian_long(0xd9ab7fdbu), change_endian_long(0x16986cd2u), change_endian_long(0x37bece9eu), change_endian_long(0x56d98c81u));
            RX65NHU_func314(172+32);
            RX65NHU_func100(change_endian_long(0x3f044fc4u), change_endian_long(0xfb1628a9u), change_endian_long(0xfc88812bu), change_endian_long(0x5fcc3e0eu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x8e78b588u);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0xe04ef408u), change_endian_long(0xa81dc57bu), change_endian_long(0x34118368u), change_endian_long(0x9bb0d708u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX65NHU_func319(208);
            RX65NHU_func100(change_endian_long(0x7b18f298u), change_endian_long(0x56f86f84u), change_endian_long(0x966e5272u), change_endian_long(0x752f3295u));
            RX65NHU_func314(208+32);
            RX65NHU_func101(change_endian_long(0x4f881615u), change_endian_long(0x5f3cd874u), change_endian_long(0x0f0643bau), change_endian_long(0xbd5a3e1bu));
        }
        TSIP.REG_ECH.WORD = 0x3800da9fu;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00A60000u;
        RX65NHU_func100(change_endian_long(0x1c15650du), change_endian_long(0x528a3ea3u), change_endian_long(0x83aa8bc8u), change_endian_long(0x2c0d82b8u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (0u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func101(change_endian_long(0xf2371d43u), change_endian_long(0x38c2f615u), change_endian_long(0x20526010u), change_endian_long(0x10b6c728u));
        }
        else
        {
            RX65NHU_func100(change_endian_long(0xa591703cu), change_endian_long(0xcc0219b7u), change_endian_long(0x0beed431u), change_endian_long(0xfac7c63au));
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
            RX65NHU_func100(change_endian_long(0xb525687fu), change_endian_long(0x9449fde5u), change_endian_long(0x1fcd3b32u), change_endian_long(0xd3779b1eu));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            RX65NHU_func320(136);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x22880c7eu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x149decf9u), change_endian_long(0xfdd68a30u), change_endian_long(0x14f7c274u), change_endian_long(0xc8cc15a3u));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000013u;
            RX65NHU_func320(172);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x8e78b588u);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0xd311cfb4u), change_endian_long(0xdaa712dcu), change_endian_long(0x321d9070u), change_endian_long(0xd80a4518u));
            TSIP.REG_00H.WORD = 0x00003283u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            RX65NHU_func320(208);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x7d2dfa89u), change_endian_long(0x6f82fc48u), change_endian_long(0xe0403b0du), change_endian_long(0xa938eae2u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX65NHU_func319(408);
            RX65NHU_func100(change_endian_long(0x285f6e3bu), change_endian_long(0x9deaa66fu), change_endian_long(0x6b795011u), change_endian_long(0x64885debu));
            RX65NHU_func314(408+32);
            RX65NHU_func100(change_endian_long(0xdf9d94eeu), change_endian_long(0x327b3b0du), change_endian_long(0x433dc974u), change_endian_long(0x6b8a5f48u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x4bfb985eu), change_endian_long(0xcb9c7f51u), change_endian_long(0x2f51fd39u), change_endian_long(0x50de33fau));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000023u;
            RX65NHU_func319(444);
            RX65NHU_func100(change_endian_long(0x80e93032u), change_endian_long(0x43d599cdu), change_endian_long(0x34b90015u), change_endian_long(0x6481e6a4u));
            RX65NHU_func314(444+32);
            RX65NHU_func100(change_endian_long(0xb414f129u), change_endian_long(0x0f660375u), change_endian_long(0xc5162efcu), change_endian_long(0xba7e50c6u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x99de9e4du), change_endian_long(0xe87a8c7cu), change_endian_long(0xc609939fu), change_endian_long(0x6c0b456fu));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX65NHU_func319(480);
            RX65NHU_func100(change_endian_long(0x124ca6a3u), change_endian_long(0xe5a409ffu), change_endian_long(0xb85c7ec6u), change_endian_long(0xf189eb2eu));
            RX65NHU_func314(480+32);
            RX65NHU_func100(change_endian_long(0xcf0eeb9du), change_endian_long(0xdfca7040u), change_endian_long(0xe05f9329u), change_endian_long(0xbf78b564u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x84992ba5u);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x00eaddf5u), change_endian_long(0x2b8092cdu), change_endian_long(0x87e47dcdu), change_endian_long(0xc7d8ca41u));
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
            RX65NHU_func100(change_endian_long(0xf24b2723u), change_endian_long(0x2722d2b2u), change_endian_long(0x3fcd408fu), change_endian_long(0x036bc7b5u));
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
            RX65NHU_func100(change_endian_long(0xfbdb8259u), change_endian_long(0x10863967u), change_endian_long(0xbeed34d8u), change_endian_long(0xbc7f542cu));
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
            RX65NHU_func100(change_endian_long(0xe73e92dau), change_endian_long(0x3b717551u), change_endian_long(0x19e2f573u), change_endian_long(0x8565b183u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX65NHU_func319(544);
            RX65NHU_func100(change_endian_long(0xc2fa6957u), change_endian_long(0xdd1faa47u), change_endian_long(0x49d001a3u), change_endian_long(0x658d457fu));
            RX65NHU_func314(544+32);
            RX65NHU_func100(change_endian_long(0x101a7543u), change_endian_long(0x3d31b753u), change_endian_long(0x6c766da9u), change_endian_long(0x65c14090u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x649d375du), change_endian_long(0x1eb90323u), change_endian_long(0x77f37179u), change_endian_long(0x44c334a9u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000023u;
            RX65NHU_func319(580);
            RX65NHU_func100(change_endian_long(0x0d309b15u), change_endian_long(0x9004011bu), change_endian_long(0x7b127cfau), change_endian_long(0x0c17a142u));
            RX65NHU_func314(580+32);
            RX65NHU_func100(change_endian_long(0x73f99ff8u), change_endian_long(0x324e796bu), change_endian_long(0x56f483cfu), change_endian_long(0x8b88876fu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0xf6a10738u), change_endian_long(0x7f4ca954u), change_endian_long(0x3780183au), change_endian_long(0x941bda8bu));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX65NHU_func319(616);
            RX65NHU_func100(change_endian_long(0x651c313fu), change_endian_long(0xe7c19bbfu), change_endian_long(0xd08670cdu), change_endian_long(0xec46054fu));
            RX65NHU_func314(616+32);
            TSIP.REG_ECH.WORD = 0x0000b7a0u;
            TSIP.REG_ECH.WORD = 0x000000f5u;
            RX65NHU_func101(change_endian_long(0x022a94cbu), change_endian_long(0x84368cb2u), change_endian_long(0x8af0acc8u), change_endian_long(0x576434e0u));
            RX65NHU_func318();
            RX65NHU_func100(change_endian_long(0x999b2e27u), change_endian_long(0x13e90050u), change_endian_long(0xd2bdd5adu), change_endian_long(0x7cac9b87u));
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
            RX65NHU_func100(change_endian_long(0x612700ffu), change_endian_long(0x898f7686u), change_endian_long(0x9a956c04u), change_endian_long(0x8daea3b9u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX65NHU_func319(136);
            RX65NHU_func100(change_endian_long(0x05cc442fu), change_endian_long(0x82c8b744u), change_endian_long(0x4c0da19bu), change_endian_long(0x6e1a078au));
            RX65NHU_func314(136+32);
            RX65NHU_func100(change_endian_long(0xa3cfae7eu), change_endian_long(0xfa8603a2u), change_endian_long(0xe84feaf2u), change_endian_long(0xe03e3464u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x22880c7eu);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0xd0bf2a3bu), change_endian_long(0xc83b7fc0u), change_endian_long(0x58d48e17u), change_endian_long(0x4ca46320u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000023u;
            RX65NHU_func319(172);
            RX65NHU_func100(change_endian_long(0x88a9becdu), change_endian_long(0xf73ddb9au), change_endian_long(0xf36ecbaeu), change_endian_long(0xf1513506u));
            RX65NHU_func314(172+32);
            RX65NHU_func100(change_endian_long(0x6865f537u), change_endian_long(0x3d01c6b9u), change_endian_long(0x4748182du), change_endian_long(0xe8788142u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x8e78b588u);
            RX65NHU_func080();
            RX65NHU_func100(change_endian_long(0x5e4e398eu), change_endian_long(0x60d0e2eeu), change_endian_long(0x408883c2u), change_endian_long(0x049be6c1u));
            TSIP.REG_00H.WORD = 0x00002383u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX65NHU_func319(208);
            RX65NHU_func100(change_endian_long(0x0b825d3du), change_endian_long(0x49187958u), change_endian_long(0x8bad2baau), change_endian_long(0xa13890e0u));
            RX65NHU_func314(208+32);
            TSIP.REG_ECH.WORD = 0x0000d260u;
            RX65NHU_func101(change_endian_long(0xbb2f51c6u), change_endian_long(0x2d8a3a66u), change_endian_long(0x352178dau), change_endian_long(0x565d268du));
        }
        TSIP.REG_ECH.WORD = 0x01816e94u;
        RX65NHU_func101(change_endian_long(0x35bc713eu), change_endian_long(0x06e8b05fu), change_endian_long(0x2f3888cbu), change_endian_long(0x0762ec76u));
    }
    TSIP.REG_ECH.WORD = 0x38008a20u;
    TSIP.REG_ECH.WORD = 0x00000020u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    RX65NHU_func100(change_endian_long(0x0673928eu), change_endian_long(0x95871bceu), change_endian_long(0x0e6cac9au), change_endian_long(0x6a1107e2u));
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
    RX65NHU_func100(change_endian_long(0xe4cb7c45u), change_endian_long(0x840220edu), change_endian_long(0xa1db8375u), change_endian_long(0x624b5aafu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX65NHU_func320(136);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x22880c7eu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x126051e9u), change_endian_long(0x4ab18007u), change_endian_long(0x09336a79u), change_endian_long(0x18dc3e4eu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000013u;
    RX65NHU_func320(172);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8e78b588u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xdf4b346cu), change_endian_long(0xc95af5f3u), change_endian_long(0x14a33920u), change_endian_long(0x0415b23eu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func320(208);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x0a2f7d48u), change_endian_long(0x21b62198u), change_endian_long(0xe3d90c13u), change_endian_long(0xf1e672d2u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func319(408);
    RX65NHU_func100(change_endian_long(0xbd2732a6u), change_endian_long(0x495f3b5bu), change_endian_long(0x36c0d469u), change_endian_long(0x87b270b7u));
    RX65NHU_func314(408+32);
    RX65NHU_func100(change_endian_long(0x79a68538u), change_endian_long(0x7e269933u), change_endian_long(0xe8eb63f1u), change_endian_long(0x365c0e71u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x1665958du), change_endian_long(0x3287514bu), change_endian_long(0x2bd63d4cu), change_endian_long(0xfe477b9eu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX65NHU_func319(444);
    RX65NHU_func100(change_endian_long(0x3f64ff6eu), change_endian_long(0x69d1b910u), change_endian_long(0xf0b12c3au), change_endian_long(0xd089b2d7u));
    RX65NHU_func314(444+32);
    RX65NHU_func100(change_endian_long(0x51bb7539u), change_endian_long(0xe6c95ee2u), change_endian_long(0xf2710b29u), change_endian_long(0x68b45049u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x1283275bu), change_endian_long(0xdcadc539u), change_endian_long(0x8799fcccu), change_endian_long(0xd342fa0fu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX65NHU_func319(480);
    RX65NHU_func100(change_endian_long(0x6bf2bc71u), change_endian_long(0x517ee965u), change_endian_long(0xa8c57b31u), change_endian_long(0x1d2dad4du));
    RX65NHU_func314(480+32);
    RX65NHU_func100(change_endian_long(0xc8eba84bu), change_endian_long(0xa024e1c2u), change_endian_long(0xa0292d94u), change_endian_long(0xac4ffb7cu));
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
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x6447b18cu), change_endian_long(0x38794008u), change_endian_long(0xafc1bab8u), change_endian_long(0x78b24d07u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX65NHU_func319(616);
    RX65NHU_func100(change_endian_long(0x6159d5d9u), change_endian_long(0x6cbbc9adu), change_endian_long(0x0b8fc437u), change_endian_long(0x4f926291u));
    RX65NHU_func314(616+32);
    RX65NHU_func100(change_endian_long(0x92857910u), change_endian_long(0x999dce4bu), change_endian_long(0xb2daa159u), change_endian_long(0x2a5aeeb0u));
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
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xc4963ed2u), change_endian_long(0x760f9830u), change_endian_long(0x0b7392e4u), change_endian_long(0xc269689bu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func319(544);
    RX65NHU_func100(change_endian_long(0xbbfe535au), change_endian_long(0x0971a49du), change_endian_long(0xb359aa87u), change_endian_long(0xe75ca039u));
    RX65NHU_func314(544+32);
    RX65NHU_func100(change_endian_long(0xcf862a15u), change_endian_long(0xf7a34897u), change_endian_long(0x9f38885au), change_endian_long(0xafba0eb5u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x16ef0550u), change_endian_long(0x8c53090du), change_endian_long(0x4e60950au), change_endian_long(0x3e5fae39u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX65NHU_func319(580);
    RX65NHU_func100(change_endian_long(0xc498ec3du), change_endian_long(0xf9286c53u), change_endian_long(0x4598baaau), change_endian_long(0x503cfa4du));
    RX65NHU_func314(580+32);
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f6u;
    RX65NHU_func101(change_endian_long(0xda0a415bu), change_endian_long(0x623ca145u), change_endian_long(0x73348e5fu), change_endian_long(0x9db6f41au));
    RX65NHU_func318();
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00003bbeu;
    TSIP.REG_ECH.WORD = 0x00007c1du;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_func325_r1.prc
 *********************************************************************************************************************/
