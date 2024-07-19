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
* Function Name: RX65NHU_func318
*******************************************************************************************************************/ /**
* @details       RX65NHU func318
* @note          None
*/
void RX65NHU_func318(void)
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
    TSIP.REG_ECH.WORD = 0x00000008u;
    TSIP.REG_ECH.WORD = 0x00003fbeu;
    RX65NHU_func100(change_endian_long(0x5b72f6b9u), change_endian_long(0x83707a2eu), change_endian_long(0x6b50992eu), change_endian_long(0x6042bc66u));
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
    RX65NHU_func100(change_endian_long(0xcbfb86ffu), change_endian_long(0x9e61bf11u), change_endian_long(0x7f7a32cdu), change_endian_long(0x1ea1c621u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000015u;
    RX65NHU_func320(480);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x3e97a11du), change_endian_long(0xf8646a5eu), change_endian_long(0x6fce6254u), change_endian_long(0x30b40336u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func320(616);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x21ca5c7fu), change_endian_long(0x7858f875u), change_endian_long(0x0476bae2u), change_endian_long(0x619f2e75u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX65NHU_func320(444);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x2ffd7551u), change_endian_long(0x4500657du), change_endian_long(0xcfda2f0du), change_endian_long(0xc05b346au));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func320(580);
    RX65NHU_func323();
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x62a393e9u), change_endian_long(0x4025a4d4u), change_endian_long(0xe6fbb0e6u), change_endian_long(0x380aff12u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX65NHU_func321(816);
    RX65NHU_func100(change_endian_long(0x4fe8a690u), change_endian_long(0x53ea2036u), change_endian_long(0xc44ade7cu), change_endian_long(0xf3cbc049u));
    RX65NHU_func314(816+36);
    RX65NHU_func100(change_endian_long(0x901bbe9fu), change_endian_long(0x74ca877cu), change_endian_long(0x32c78a4bu), change_endian_long(0x2129c7a2u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x7e87c614u), change_endian_long(0xf5432fc2u), change_endian_long(0xa1dfcf0du), change_endian_long(0xa78f5f3cu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func321(856);
    RX65NHU_func100(change_endian_long(0xcdaa8407u), change_endian_long(0x892b5dc9u), change_endian_long(0x54490683u), change_endian_long(0x1ee98bf8u));
    RX65NHU_func314(856+36);
    RX65NHU_func100(change_endian_long(0x5a3cbb52u), change_endian_long(0x5153fc33u), change_endian_long(0xa20bd71cu), change_endian_long(0xef699184u));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_24H.WORD = 0x00005cd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000098d0u;
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
    TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xc7aa3150u), change_endian_long(0x8d15fb1eu), change_endian_long(0x78b066e0u), change_endian_long(0x3516f8c2u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000015u;
    RX65NHU_func320(408);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x7ae803fbu), change_endian_long(0xe3898e64u), change_endian_long(0xf83fa0f4u), change_endian_long(0x252635b0u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func320(544);
    RX65NHU_func323();
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x650998ccu), change_endian_long(0xe62875dau), change_endian_long(0x68977ea3u), change_endian_long(0xf43013cfu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX65NHU_func321(992);
    RX65NHU_func100(change_endian_long(0x13d3d43bu), change_endian_long(0xf391b164u), change_endian_long(0x88e9e948u), change_endian_long(0xf65f396eu));
    RX65NHU_func314(992+36);
    RX65NHU_func100(change_endian_long(0xef8d9f64u), change_endian_long(0x4c51eb72u), change_endian_long(0xcca5712fu), change_endian_long(0x93897244u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xe5b56eb8u), change_endian_long(0xc14a8d23u), change_endian_long(0xf89bed19u), change_endian_long(0x637e6c84u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func322(816);
    TSIP.REG_24H.WORD = 0x000009c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00001181u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x16b3e666u), change_endian_long(0xab30a14au), change_endian_long(0x170903bfu), change_endian_long(0x2abf7579u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX65NHU_func321(816);
    RX65NHU_func100(change_endian_long(0x76a81f3cu), change_endian_long(0x8aa8c348u), change_endian_long(0x76125df2u), change_endian_long(0x21367360u));
    RX65NHU_func314(816+36);
    RX65NHU_func100(change_endian_long(0x434bcd14u), change_endian_long(0x93a5608cu), change_endian_long(0x131bed4cu), change_endian_long(0xd5d89da3u));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_24H.WORD = 0x00005cd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000098d0u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xee7ca360u), change_endian_long(0x333d64fdu), change_endian_long(0xe83bbbeeu), change_endian_long(0x7fa0e30fu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000015u;
    RX65NHU_func320(444);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x96cee9e0u), change_endian_long(0x543c32e9u), change_endian_long(0x4f44862du), change_endian_long(0xc7de935du));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func320(580);
    RX65NHU_func323();
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xe1793e8bu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xf4528bc8u), change_endian_long(0xf9c758e3u), change_endian_long(0xcca17286u), change_endian_long(0x0446e80bu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX65NHU_func321(952);
    RX65NHU_func100(change_endian_long(0xa1c7c6beu), change_endian_long(0x34bdd65eu), change_endian_long(0xa503229eu), change_endian_long(0xe59f9ad4u));
    RX65NHU_func314(952+36);
    RX65NHU_func100(change_endian_long(0xb3170f58u), change_endian_long(0xbbdfa492u), change_endian_long(0xe193c683u), change_endian_long(0x8ba628fcu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x0b9b7319u), change_endian_long(0x2075faf5u), change_endian_long(0x48cf7183u), change_endian_long(0xf242f069u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func322(992);
    TSIP.REG_24H.WORD = 0x000009c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00001181u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x2b64b62au), change_endian_long(0x26715415u), change_endian_long(0x068c5f9fu), change_endian_long(0xd067c4c4u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX65NHU_func321(992);
    RX65NHU_func100(change_endian_long(0xece8246cu), change_endian_long(0x1a367a9au), change_endian_long(0x7a7710bbu), change_endian_long(0x30c796ccu));
    RX65NHU_func314(992+36);
    RX65NHU_func100(change_endian_long(0x075fb3efu), change_endian_long(0x3c2d0303u), change_endian_long(0x6edee302u), change_endian_long(0x10fb278au));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_24H.WORD = 0x00005cd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000098d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x0000480au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_24H.WORD = 0x000060d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000a0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xff383d40u), change_endian_long(0xbdf58770u), change_endian_long(0xe795e086u), change_endian_long(0x6ef8de34u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(856);
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
    TSIP.REG_24H.WORD = 0x0000c0c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xe1b55edbu), change_endian_long(0xc5e99747u), change_endian_long(0x44e968f4u), change_endian_long(0xc55e03bbu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(856);
    RX65NHU_func100(change_endian_long(0x69ec219bu), change_endian_long(0xe596ff86u), change_endian_long(0xaf3caacdu), change_endian_long(0x32a069a6u));
    RX65NHU_func314(856+36);
    RX65NHU_func100(change_endian_long(0x047db70cu), change_endian_long(0x1e1ee358u), change_endian_long(0x4456cdd1u), change_endian_long(0x0e5fe3cbu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x4d0b7f07u), change_endian_long(0x972ed45du), change_endian_long(0x8330036du), change_endian_long(0xe9f4ff50u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(992);
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
    TSIP.REG_24H.WORD = 0x00001181u;
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
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x80e07971u), change_endian_long(0xc625b515u), change_endian_long(0x3da2a036u), change_endian_long(0x99dbf3edu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(992);
    RX65NHU_func100(change_endian_long(0x2089d6a0u), change_endian_long(0xd48467e8u), change_endian_long(0x6cd32779u), change_endian_long(0x3cc7a0c7u));
    RX65NHU_func314(992+36);
    RX65NHU_func100(change_endian_long(0x46907211u), change_endian_long(0x2f290011u), change_endian_long(0x89d24a2du), change_endian_long(0x0705f757u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x2fe99450u), change_endian_long(0x6d8487c2u), change_endian_long(0xdd154e4cu), change_endian_long(0x2118988bu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(816);
    TSIP.REG_24H.WORD = 0x000019c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00000981u;
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
    TSIP.REG_24H.WORD = 0x0000c0c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x6974df92u), change_endian_long(0xfd1dcb47u), change_endian_long(0xb8bfc67au), change_endian_long(0x4f1bdf89u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(816);
    RX65NHU_func100(change_endian_long(0x562ec516u), change_endian_long(0x438e5758u), change_endian_long(0x87dd51fau), change_endian_long(0x863a6425u));
    RX65NHU_func314(816+36);
    RX65NHU_func100(change_endian_long(0xbabfb02du), change_endian_long(0x23b4ed5du), change_endian_long(0xfe88b50fu), change_endian_long(0x5298146au));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xe1793e8bu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xff6b9944u), change_endian_long(0x55d39e05u), change_endian_long(0x750627fcu), change_endian_long(0x94a322fbu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(952);
    TSIP.REG_24H.WORD = 0x000019c0u;
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
    TSIP.REG_24H.WORD = 0x0000c0c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0xe1793e8bu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x698e0241u), change_endian_long(0xa63d7f0fu), change_endian_long(0x1fbdfff6u), change_endian_long(0xb9026e1bu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(952);
    RX65NHU_func100(change_endian_long(0x2aa21b86u), change_endian_long(0x2708f417u), change_endian_long(0xa23d7c52u), change_endian_long(0xd03f6825u));
    RX65NHU_func314(952+36);
    RX65NHU_func100(change_endian_long(0xb2d28aebu), change_endian_long(0xbf340841u), change_endian_long(0xd222886cu), change_endian_long(0x5dbb967du));
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
    RX65NHU_func100(change_endian_long(0xc6498a18u), change_endian_long(0xd994905du), change_endian_long(0x1a10a146u), change_endian_long(0x5903fb8fu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX65NHU_func320(480);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xab8fcf4eu), change_endian_long(0x3ef8d03fu), change_endian_long(0xaebe7e6du), change_endian_long(0x4537dd7du));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func320(616);
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x0000480au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_24H.WORD = 0x000060d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000a0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_24H.WORD = 0x0000c8d0u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x1466af8eu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x82c68e31u), change_endian_long(0x378ec50du), change_endian_long(0xa202aab3u), change_endian_long(0xc534905eu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(896);
    RX65NHU_func100(change_endian_long(0xcaa1e354u), change_endian_long(0x03115a3du), change_endian_long(0xc8e22f26u), change_endian_long(0x3457f1d3u));
    RX65NHU_func314(896+36);
    RX65NHU_func100(change_endian_long(0x1fe01ee8u), change_endian_long(0xc02d996au), change_endian_long(0x8ae9cfc2u), change_endian_long(0x9b7add55u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xbbec8c11u), change_endian_long(0xc081c106u), change_endian_long(0x038e8cdcu), change_endian_long(0xdafccb0cu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(856);
    TSIP.REG_24H.WORD = 0x000019c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00001181u;
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
    TSIP.REG_24H.WORD = 0x0000c0c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x4d5f7879u), change_endian_long(0xe999fe7fu), change_endian_long(0x28d74770u), change_endian_long(0x85b3ea19u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(856);
    RX65NHU_func100(change_endian_long(0xea8e2c20u), change_endian_long(0x6eea7c17u), change_endian_long(0x45b7e392u), change_endian_long(0x4804c145u));
    RX65NHU_func314(856+36);
    RX65NHU_func100(change_endian_long(0x65bd1c8eu), change_endian_long(0x0a4e1fabu), change_endian_long(0x9706ad5bu), change_endian_long(0x0545e54du));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xa9c47382u), change_endian_long(0x25397e0fu), change_endian_long(0xe083942bu), change_endian_long(0x45a61ec6u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(816);
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
    TSIP.REG_24H.WORD = 0x00001191u;
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
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xd3863235u), change_endian_long(0x2524a775u), change_endian_long(0xa67278bcu), change_endian_long(0x15296e76u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(816);
    RX65NHU_func100(change_endian_long(0x84c1c1f3u), change_endian_long(0xf9effbd5u), change_endian_long(0xbe6bf7fdu), change_endian_long(0xe25f595eu));
    RX65NHU_func314(816+36);
    RX65NHU_func100(change_endian_long(0xfef2532fu), change_endian_long(0xe1f701e9u), change_endian_long(0xc8a92555u), change_endian_long(0x403de291u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x6d1845abu), change_endian_long(0xa64a8ffcu), change_endian_long(0xa611a6acu), change_endian_long(0x144e2fb1u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(992);
    TSIP.REG_24H.WORD = 0x000019c0u;
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
    TSIP.REG_24H.WORD = 0x0000c0c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x079ce9b3u), change_endian_long(0x7e1fff16u), change_endian_long(0x4a80cfa9u), change_endian_long(0xc86db2dau));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(992);
    RX65NHU_func100(change_endian_long(0x6b60b025u), change_endian_long(0xa554ed5au), change_endian_long(0xc5c91f46u), change_endian_long(0x0ba0d7c7u));
    RX65NHU_func314(992+36);
    RX65NHU_func100(change_endian_long(0xa9fa35ceu), change_endian_long(0x3b660878u), change_endian_long(0x87467c75u), change_endian_long(0xc37d852du));
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
    RX65NHU_func100(change_endian_long(0xf0238e3du), change_endian_long(0xb54a4149u), change_endian_long(0xf8f92312u), change_endian_long(0x85b07d09u));
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
    TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xc4f99d60u), change_endian_long(0x81e46205u), change_endian_long(0xf9826361u), change_endian_long(0xba6475c7u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX65NHU_func320(408);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
    RX65NHU_func080();
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func320(544);
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_24H.WORD = 0x000060d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f0u;
    RX65NHU_func101(change_endian_long(0xd795b7c5u), change_endian_long(0x39b0ad4cu), change_endian_long(0xe2c77564u), change_endian_long(0xcc515e7cu));
    RX65NHU_func316();
    RX65NHU_func100(change_endian_long(0x615034c0u), change_endian_long(0x18d3fed4u), change_endian_long(0x39893073u), change_endian_long(0x5b2e6310u));
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xb3e1e853u), change_endian_long(0x916f7123u), change_endian_long(0x81b8e1f9u), change_endian_long(0xaac9d27fu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(816);
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
    TSIP.REG_24H.WORD = 0x0000c0c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xfb11e1a0u), change_endian_long(0xb95fa3c6u), change_endian_long(0x88bc14e7u), change_endian_long(0xcbe42750u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(816);
    RX65NHU_func100(change_endian_long(0x26d055aeu), change_endian_long(0x0fe42cd8u), change_endian_long(0x80475cacu), change_endian_long(0xd6d1f869u));
    RX65NHU_func314(816+36);
    RX65NHU_func100(change_endian_long(0x604587e2u), change_endian_long(0x7c60c096u), change_endian_long(0x0fceb1afu), change_endian_long(0xfa9bcb86u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xe1793e8bu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x7b8dbd51u), change_endian_long(0x5e43e6c9u), change_endian_long(0x075de01fu), change_endian_long(0xba86b1fbu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(952);
    TSIP.REG_24H.WORD = 0x000019c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00000981u;
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
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xe1793e8bu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x71771a39u), change_endian_long(0x8d83bafcu), change_endian_long(0xf876ac7fu), change_endian_long(0x14823fe8u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(952);
    RX65NHU_func100(change_endian_long(0xf2b2e4cau), change_endian_long(0x52c409b6u), change_endian_long(0x26f2a7fau), change_endian_long(0x68c21464u));
    RX65NHU_func314(952+36);
    RX65NHU_func100(change_endian_long(0xb7adf5f1u), change_endian_long(0x944b3896u), change_endian_long(0xe35859f9u), change_endian_long(0x4d91f29fu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x429388bbu), change_endian_long(0x763b5f06u), change_endian_long(0xd69cf54bu), change_endian_long(0xec2a8b8cu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(992);
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
    TSIP.REG_24H.WORD = 0x0000c0c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xa9bfbafbu), change_endian_long(0x358d75ceu), change_endian_long(0x6b42d504u), change_endian_long(0x5a9691d6u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(992);
    RX65NHU_func100(change_endian_long(0xbfec6079u), change_endian_long(0x3b5f88cbu), change_endian_long(0x9a0a78e3u), change_endian_long(0xe0339ff9u));
    RX65NHU_func314(992+36);
    RX65NHU_func100(change_endian_long(0x3accdd1fu), change_endian_long(0xfb8270b8u), change_endian_long(0xdca932e2u), change_endian_long(0xfd9bb8efu));
    TSIP.REG_28H.WORD = 0x008f0001u;
    TSIP.REG_34H.WORD = 0x00000800u;
    TSIP.REG_24H.WORD = 0x800070d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_34H.WORD = 0x00000408u;
    TSIP.REG_24H.WORD = 0x8000f0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x0000080cu;
    TSIP.REG_24H.WORD = 0x8000f0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000c00u;
    TSIP.REG_24H.WORD = 0x8000f4d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_24H.WORD = 0x000050d0u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x7775adb6u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xc48673a2u), change_endian_long(0x413942f8u), change_endian_long(0x7bb287feu), change_endian_long(0x792e1822u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func320(752);
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_24H.WORD = 0x0000a0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a70001u;
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x0000480au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_34H.WORD = 0x00000003u;
    TSIP.REG_24H.WORD = 0x800068d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000c02u;
    TSIP.REG_24H.WORD = 0x800048d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000801u;
    TSIP.REG_24H.WORD = 0x800048d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000400u;
    TSIP.REG_24H.WORD = 0x800048d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000054d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000a0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x1466af8eu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x19dddd4bu), change_endian_long(0xdc508799u), change_endian_long(0xd588274cu), change_endian_long(0x33535ed7u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(896);
    TSIP.REG_24H.WORD = 0x000019c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00000981u;
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
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x1466af8eu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x7377c4dau), change_endian_long(0xeaa6573bu), change_endian_long(0x49f0b6e2u), change_endian_long(0x0dd25471u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(896);
    RX65NHU_func100(change_endian_long(0xcf2f4539u), change_endian_long(0x001b4eceu), change_endian_long(0xb26256e7u), change_endian_long(0xdabe1193u));
    RX65NHU_func314(896+36);
    RX65NHU_func100(change_endian_long(0x1db9aaf2u), change_endian_long(0x6be8cebcu), change_endian_long(0x5bebc77du), change_endian_long(0x8f0b0e3bu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x6d004304u), change_endian_long(0xafbfd967u), change_endian_long(0xfbc51ad3u), change_endian_long(0x4ec0fdf4u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(856);
    TSIP.REG_24H.WORD = 0x000019c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00000981u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00001181u;
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
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x30326adcu), change_endian_long(0xa86d5c7du), change_endian_long(0x27441ffau), change_endian_long(0x4a19a44bu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(856);
    RX65NHU_func100(change_endian_long(0x00868e78u), change_endian_long(0x1daacba9u), change_endian_long(0x107e7a1du), change_endian_long(0x3109018fu));
    RX65NHU_func314(856+36);
    RX65NHU_func100(change_endian_long(0x2002378bu), change_endian_long(0x900deb79u), change_endian_long(0x1e0528b5u), change_endian_long(0x58c194beu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xa1a208beu), change_endian_long(0xc3cee30du), change_endian_long(0xd1eb1551u), change_endian_long(0xdf8d05e7u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(816);
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
    TSIP.REG_24H.WORD = 0x00001181u;
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
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x53b616fau), change_endian_long(0xa0fbbcecu), change_endian_long(0x3e9d1493u), change_endian_long(0x0db64ea4u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(816);
    RX65NHU_func100(change_endian_long(0xc3533228u), change_endian_long(0xfd0622f0u), change_endian_long(0x97c371c8u), change_endian_long(0x3ad808a3u));
    RX65NHU_func314(816+36);
    RX65NHU_func100(change_endian_long(0xcd30a7fdu), change_endian_long(0x8f7f6ddau), change_endian_long(0x0ae91daau), change_endian_long(0x01816622u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x6fe8aea5u), change_endian_long(0xdb7c1d43u), change_endian_long(0x92aa0b3du), change_endian_long(0xc82fa64du));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(992);
    TSIP.REG_24H.WORD = 0x000019c0u;
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
    TSIP.REG_24H.WORD = 0x0000c0c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x008b4683u), change_endian_long(0x2ae156a2u), change_endian_long(0x79f8be69u), change_endian_long(0x894c33f3u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(992);
    RX65NHU_func100(change_endian_long(0x48056c2bu), change_endian_long(0x77dd9097u), change_endian_long(0x45986161u), change_endian_long(0x80b59739u));
    RX65NHU_func314(992+36);
    RX65NHU_func100(change_endian_long(0x7ac2d9a4u), change_endian_long(0x2819c57fu), change_endian_long(0xb3f3a44du), change_endian_long(0xdc9d7e2fu));
    TSIP.REG_28H.WORD = 0x008f0001u;
    TSIP.REG_34H.WORD = 0x00000002u;
    TSIP.REG_24H.WORD = 0x80004cd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_34H.WORD = 0x00000810u;
    TSIP.REG_24H.WORD = 0x80005cd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000c20u;
    TSIP.REG_24H.WORD = 0x80005cd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000030u;
    TSIP.REG_24H.WORD = 0x80007cd0u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x899d7651u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x3ea0a10cu), change_endian_long(0x8429dd1bu), change_endian_long(0x938fec02u), change_endian_long(0x2618def0u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func320(716);
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_24H.WORD = 0x0000a0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a70001u;
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x0000480au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_34H.WORD = 0x00000003u;
    TSIP.REG_24H.WORD = 0x800068d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000c02u;
    TSIP.REG_24H.WORD = 0x800048d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000801u;
    TSIP.REG_24H.WORD = 0x800048d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000400u;
    TSIP.REG_24H.WORD = 0x800048d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000054d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000a0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x71b845fau), change_endian_long(0x7ffe437cu), change_endian_long(0x545dc0c1u), change_endian_long(0x034bfaa5u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(856);
    TSIP.REG_24H.WORD = 0x000019c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00000981u;
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
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xbb828bafu), change_endian_long(0xd230098du), change_endian_long(0xa2ec62f9u), change_endian_long(0xb55251efu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(856);
    RX65NHU_func100(change_endian_long(0x9629c0a9u), change_endian_long(0x91cff4a3u), change_endian_long(0x1f2dea26u), change_endian_long(0x70216b4fu));
    RX65NHU_func314(856+36);
    RX65NHU_func100(change_endian_long(0x2096c919u), change_endian_long(0x13932a34u), change_endian_long(0x5a8e5091u), change_endian_long(0x4f7b55b6u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xfda365ecu), change_endian_long(0x616a9a09u), change_endian_long(0x8d360dccu), change_endian_long(0x3743f71eu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(816);
    TSIP.REG_24H.WORD = 0x000019c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00000981u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00001181u;
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
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x90fb4f1du), change_endian_long(0x7d741b59u), change_endian_long(0x7348c46au), change_endian_long(0x945ad25bu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(816);
    RX65NHU_func100(change_endian_long(0x9603ad87u), change_endian_long(0x8ffec6b8u), change_endian_long(0x4c49cd92u), change_endian_long(0x371a2725u));
    RX65NHU_func314(816+36);
    RX65NHU_func100(change_endian_long(0x4d8fe3a6u), change_endian_long(0x439f8475u), change_endian_long(0xab502a81u), change_endian_long(0xbffa3615u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xe1793e8bu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x310a2b1au), change_endian_long(0x933dc64au), change_endian_long(0x373c8e5eu), change_endian_long(0x6519f3a5u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(952);
    TSIP.REG_24H.WORD = 0x000019c0u;
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
    TSIP.REG_24H.WORD = 0x0000c0c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0xe1793e8bu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xa0f5ebf8u), change_endian_long(0xfcf05f34u), change_endian_long(0x1482411cu), change_endian_long(0x7b956d1eu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(952);
    RX65NHU_func100(change_endian_long(0xe81ab557u), change_endian_long(0x40243ed8u), change_endian_long(0xfa2460e0u), change_endian_long(0xce95399eu));
    RX65NHU_func314(952+36);
    RX65NHU_func100(change_endian_long(0xed6f1487u), change_endian_long(0xf42b0b94u), change_endian_long(0xd28bda57u), change_endian_long(0xd0b5719du));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xe63a6f71u), change_endian_long(0xd2a1f3deu), change_endian_long(0xdd459a14u), change_endian_long(0x4747e3bau));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(992);
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
    TSIP.REG_24H.WORD = 0x00001181u;
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
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xed4bfa35u), change_endian_long(0x7ea4aabau), change_endian_long(0x9ffa2e7bu), change_endian_long(0xfc38678fu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(992);
    RX65NHU_func100(change_endian_long(0xe1b7df89u), change_endian_long(0x8d547da0u), change_endian_long(0x5fa1fc9cu), change_endian_long(0x3b3beedbu));
    RX65NHU_func314(992+36);
    RX65NHU_func100(change_endian_long(0xfd4ee670u), change_endian_long(0x671c5957u), change_endian_long(0xc1400a05u), change_endian_long(0x8ef3384fu));
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
    RX65NHU_func100(change_endian_long(0x64b2938au), change_endian_long(0x472ed5b2u), change_endian_long(0x5b503a7bu), change_endian_long(0xe657cb4eu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func320(680);
    TSIP.REG_24H.WORD = 0x000084d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xe1793e8bu);
    RX65NHU_func080();
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX65NHU_func322(952);
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f1u;
    RX65NHU_func101(change_endian_long(0xbbab498fu), change_endian_long(0x9d94c01au), change_endian_long(0x6d427240u), change_endian_long(0x5255ca86u));
    RX65NHU_func316();
    RX65NHU_func100(change_endian_long(0xa0b5237eu), change_endian_long(0x8d2c9a33u), change_endian_long(0x65d44314u), change_endian_long(0x9d96cd2au));
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x5ec6d8d5u), change_endian_long(0xcc86e32eu), change_endian_long(0x08b0484fu), change_endian_long(0xafda01e0u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(992);
    TSIP.REG_24H.WORD = 0x000019c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00000981u;
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
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xde7bf895u), change_endian_long(0x5b34d724u), change_endian_long(0x53535551u), change_endian_long(0x263f60ebu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(992);
    RX65NHU_func100(change_endian_long(0x00f7ff55u), change_endian_long(0x5797b0edu), change_endian_long(0x25fb31c9u), change_endian_long(0x05adfffcu));
    RX65NHU_func314(992+36);
    RX65NHU_func100(change_endian_long(0xf9b6436fu), change_endian_long(0xed51824cu), change_endian_long(0x54cd53e3u), change_endian_long(0xadb9746au));
    TSIP.REG_28H.WORD = 0x008f0001u;
    TSIP.REG_34H.WORD = 0x00000800u;
    TSIP.REG_24H.WORD = 0x800070d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_34H.WORD = 0x00000408u;
    TSIP.REG_24H.WORD = 0x8000f0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x0000080cu;
    TSIP.REG_24H.WORD = 0x8000f0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000c00u;
    TSIP.REG_24H.WORD = 0x8000f4d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a70001u;
    TSIP.REG_24H.WORD = 0x000050d0u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x7775adb6u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xfe8cb5aeu), change_endian_long(0x35f14fc4u), change_endian_long(0x8a314ef0u), change_endian_long(0xb0bfab5eu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func320(752);
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_24H.WORD = 0x0000a0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a70001u;
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x0000480au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_34H.WORD = 0x00000003u;
    TSIP.REG_24H.WORD = 0x800068d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000c02u;
    TSIP.REG_24H.WORD = 0x800048d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000801u;
    TSIP.REG_24H.WORD = 0x800048d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000400u;
    TSIP.REG_24H.WORD = 0x800048d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000054d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000a0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x3868c4ebu), change_endian_long(0x45733aa7u), change_endian_long(0x73a9d373u), change_endian_long(0x673c518bu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(856);
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
    TSIP.REG_24H.WORD = 0x0000c0c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xf0fbf0a4u), change_endian_long(0x3b46a88fu), change_endian_long(0xb39519f6u), change_endian_long(0xc88b23d7u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(856);
    RX65NHU_func100(change_endian_long(0xa5430caeu), change_endian_long(0x557c562au), change_endian_long(0xa1d29b01u), change_endian_long(0x3236fc64u));
    RX65NHU_func314(856+36);
    RX65NHU_func100(change_endian_long(0xef41a66fu), change_endian_long(0x667ad10au), change_endian_long(0xefb0a84cu), change_endian_long(0x1c261c0eu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xb850f579u), change_endian_long(0x59722b18u), change_endian_long(0x709c978eu), change_endian_long(0xa5700093u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(816);
    TSIP.REG_24H.WORD = 0x000019c0u;
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
    TSIP.REG_24H.WORD = 0x0000c0c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x9d227c4eu), change_endian_long(0xc70ac984u), change_endian_long(0x2eb57af4u), change_endian_long(0xb0fc4979u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(816);
    RX65NHU_func100(change_endian_long(0x7a997b61u), change_endian_long(0x9c1e4f82u), change_endian_long(0x097faf56u), change_endian_long(0x575ce790u));
    RX65NHU_func314(816+36);
    RX65NHU_func100(change_endian_long(0x4543aaa6u), change_endian_long(0xf4a1e476u), change_endian_long(0xe1da4a3eu), change_endian_long(0xa1079313u));
    TSIP.REG_28H.WORD = 0x008f0001u;
    TSIP.REG_34H.WORD = 0x00000002u;
    TSIP.REG_24H.WORD = 0x80004cd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_34H.WORD = 0x00000810u;
    TSIP.REG_24H.WORD = 0x80005cd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000c20u;
    TSIP.REG_24H.WORD = 0x80005cd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000030u;
    TSIP.REG_24H.WORD = 0x80007cd0u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x899d7651u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x0009edc6u), change_endian_long(0x2775fb57u), change_endian_long(0x39ece606u), change_endian_long(0x43c0b812u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func320(716);
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_24H.WORD = 0x0000a0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a70001u;
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x0000480au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_34H.WORD = 0x00000003u;
    TSIP.REG_24H.WORD = 0x800068d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000c02u;
    TSIP.REG_24H.WORD = 0x800048d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000801u;
    TSIP.REG_24H.WORD = 0x800048d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000400u;
    TSIP.REG_24H.WORD = 0x800048d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000054d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000a0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x2cb06ae6u), change_endian_long(0xcb80b381u), change_endian_long(0xe28c3908u), change_endian_long(0x48fd83c3u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(816);
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
    TSIP.REG_24H.WORD = 0x0000c0c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x004a5dc9u), change_endian_long(0xc41484d4u), change_endian_long(0x5a450c1cu), change_endian_long(0x26230873u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(816);
    RX65NHU_func100(change_endian_long(0x066cf0fbu), change_endian_long(0x2df43e78u), change_endian_long(0xcf90748bu), change_endian_long(0xe694f9aau));
    RX65NHU_func314(816+36);
    RX65NHU_func100(change_endian_long(0xdfaa4a55u), change_endian_long(0xbabc7ce1u), change_endian_long(0xb4b3985au), change_endian_long(0x056b3e7eu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xbe3765b9u), change_endian_long(0xcff6715cu), change_endian_long(0x0db9af2fu), change_endian_long(0x533d8e49u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(992);
    TSIP.REG_24H.WORD = 0x000019c0u;
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
    TSIP.REG_24H.WORD = 0x0000c0c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xe7b8d427u), change_endian_long(0x53afdee9u), change_endian_long(0x261ad032u), change_endian_long(0x5f2d1767u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(992);
    RX65NHU_func100(change_endian_long(0x933a32a5u), change_endian_long(0x13df776bu), change_endian_long(0x54122714u), change_endian_long(0x72641933u));
    RX65NHU_func314(992+36);
    RX65NHU_func100(change_endian_long(0x31ffdda0u), change_endian_long(0xf2301d73u), change_endian_long(0x97614cd4u), change_endian_long(0x80cff84du));
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
    RX65NHU_func100(change_endian_long(0x508e505bu), change_endian_long(0x99f66b1fu), change_endian_long(0xef2c1ab8u), change_endian_long(0x5dde749bu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func320(680);
    TSIP.REG_24H.WORD = 0x000084d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX65NHU_func080();
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX65NHU_func322(992);
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f2u;
    RX65NHU_func101(change_endian_long(0xd1124ba2u), change_endian_long(0xa3c5073au), change_endian_long(0x838f46dfu), change_endian_long(0x381ae7e2u));
    RX65NHU_func316();
    RX65NHU_func100(change_endian_long(0x320e1690u), change_endian_long(0x534033bfu), change_endian_long(0x6c2f7932u), change_endian_long(0x538e3f9au));
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x9f03ab5bu), change_endian_long(0x5c6f328eu), change_endian_long(0xa3fdaf5cu), change_endian_long(0x5a3f9808u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(816);
    TSIP.REG_24H.WORD = 0x000019c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00000981u;
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
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xa5583171u), change_endian_long(0xc2c6cb16u), change_endian_long(0xf9fa4acdu), change_endian_long(0x01b47064u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(816);
    RX65NHU_func100(change_endian_long(0xdd74f5bdu), change_endian_long(0xdde7cb10u), change_endian_long(0xa654a60bu), change_endian_long(0x69408b72u));
    RX65NHU_func314(816+36);
    RX65NHU_func100(change_endian_long(0xc6ab21aau), change_endian_long(0xdd3f4163u), change_endian_long(0xbfd1b802u), change_endian_long(0xce43dd64u));
    TSIP.REG_28H.WORD = 0x008f0001u;
    TSIP.REG_34H.WORD = 0x00000800u;
    TSIP.REG_24H.WORD = 0x800070d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_34H.WORD = 0x00000408u;
    TSIP.REG_24H.WORD = 0x8000f0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x0000080cu;
    TSIP.REG_24H.WORD = 0x8000f0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000c00u;
    TSIP.REG_24H.WORD = 0x8000f4d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a70001u;
    TSIP.REG_24H.WORD = 0x000050d0u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x7775adb6u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xb88ded57u), change_endian_long(0x38fecedau), change_endian_long(0xf0034df2u), change_endian_long(0x90c95444u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func320(752);
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_24H.WORD = 0x0000a0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a70001u;
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x0000480au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_34H.WORD = 0x00000003u;
    TSIP.REG_24H.WORD = 0x800068d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000c02u;
    TSIP.REG_24H.WORD = 0x800048d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000801u;
    TSIP.REG_24H.WORD = 0x800048d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000400u;
    TSIP.REG_24H.WORD = 0x800048d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000054d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000a0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x1466af8eu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xd23dae82u), change_endian_long(0xa22e1718u), change_endian_long(0x63c6f2c6u), change_endian_long(0xd836a354u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(896);
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
    TSIP.REG_24H.WORD = 0x0000c0c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x1466af8eu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x2e1ddc14u), change_endian_long(0xa3c68a5cu), change_endian_long(0x4ff26841u), change_endian_long(0x424beea6u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(896);
    RX65NHU_func100(change_endian_long(0x9366fcfau), change_endian_long(0x73387bf3u), change_endian_long(0xfca4e40cu), change_endian_long(0x2b1db407u));
    RX65NHU_func314(896+36);
    RX65NHU_func100(change_endian_long(0x17c3c004u), change_endian_long(0xb8fb8a6au), change_endian_long(0xa3ad28beu), change_endian_long(0xd975f20au));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x8fc191b6u), change_endian_long(0xe62fdc0cu), change_endian_long(0x3d1d5849u), change_endian_long(0xb50e6f7eu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(856);
    TSIP.REG_24H.WORD = 0x000019c0u;
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
    TSIP.REG_24H.WORD = 0x0000c0c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xbe87b44fu), change_endian_long(0x35871040u), change_endian_long(0xdc3c953au), change_endian_long(0x2231d276u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(856);
    RX65NHU_func100(change_endian_long(0x00106ecfu), change_endian_long(0x2e5f0bd6u), change_endian_long(0xe39cc3acu), change_endian_long(0xa30856b2u));
    RX65NHU_func314(856+36);
    RX65NHU_func100(change_endian_long(0x58301667u), change_endian_long(0xe615d98au), change_endian_long(0x9cee6698u), change_endian_long(0x6553dfdfu));
    TSIP.REG_28H.WORD = 0x008f0001u;
    TSIP.REG_34H.WORD = 0x00000002u;
    TSIP.REG_24H.WORD = 0x80004cd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_34H.WORD = 0x00000810u;
    TSIP.REG_24H.WORD = 0x80005cd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000c20u;
    TSIP.REG_24H.WORD = 0x80005cd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000030u;
    TSIP.REG_24H.WORD = 0x80007cd0u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x899d7651u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x5877d035u), change_endian_long(0xa2ff820eu), change_endian_long(0xe5230cf7u), change_endian_long(0xca8a1b03u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func320(716);
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_24H.WORD = 0x0000a0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a70001u;
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x0000480au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_34H.WORD = 0x00000003u;
    TSIP.REG_24H.WORD = 0x800068d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000c02u;
    TSIP.REG_24H.WORD = 0x800048d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000801u;
    TSIP.REG_24H.WORD = 0x800048d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000400u;
    TSIP.REG_24H.WORD = 0x800048d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000054d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000a0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xfa344c18u), change_endian_long(0x661f5018u), change_endian_long(0x2d742283u), change_endian_long(0xcd94da2au));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(856);
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
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x697f2578u), change_endian_long(0xf5209ddfu), change_endian_long(0x4e6aa4ecu), change_endian_long(0x65fc4880u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func322(816);
    TSIP.REG_24H.WORD = 0x000019c0u;
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
    TSIP.REG_24H.WORD = 0x0000c0c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x3c4f7713u), change_endian_long(0xfced9772u), change_endian_long(0xce6d39bfu), change_endian_long(0xc6542490u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX65NHU_func321(816);
    RX65NHU_func100(change_endian_long(0x53527d58u), change_endian_long(0x8ae1e47cu), change_endian_long(0x1f2af750u), change_endian_long(0xce5a9b02u));
    RX65NHU_func314(816+36);
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_24H.WORD = 0x00009cd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_E0H.WORD = 0x800103e0u;
    TSIP.REG_00H.WORD = 0x0000031fu;
    TSIP.REG_2CH.WORD = 0x00000020u;
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
    TSIP.REG_E0H.WORD = 0x800103c0u;
    TSIP.REG_00H.WORD = 0x0000031fu;
    TSIP.REG_2CH.WORD = 0x00000023u;
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
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_ECH.WORD = 0x3800dbffu;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX65NHU_func100(change_endian_long(0x076e85deu), change_endian_long(0x102b36e7u), change_endian_long(0xbda4d7a4u), change_endian_long(0x10d4658bu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_ECH.WORD = 0x0000b400u;
        TSIP.REG_ECH.WORD = 0xffffffffu;
        TSIP.REG_24H.WORD = 0x000001c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x04000591u;
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
        TSIP.REG_34H.WORD = 0x00000800u;
        TSIP.REG_24H.WORD = 0x800080d0u;
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
        TSIP.REG_34H.WORD = 0x00000008u;
        TSIP.REG_24H.WORD = 0x82001191u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        RX65NHU_func101(change_endian_long(0xcdb643c6u), change_endian_long(0xf17ffcf0u), change_endian_long(0xf47eb015u), change_endian_long(0xbf7c044eu));
    }
    else
    {
        TSIP.REG_ECH.WORD = 0x00000800u;
        TSIP.REG_34H.WORD = 0x00000400u;
        TSIP.REG_24H.WORD = 0x800080d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        RX65NHU_func101(change_endian_long(0x62a99772u), change_endian_long(0x8079f568u), change_endian_long(0x104d7515u), change_endian_long(0x32298810u));
    }
    RX65NHU_func100(change_endian_long(0x7d0db70eu), change_endian_long(0x07294654u), change_endian_long(0xc1f2271au), change_endian_long(0x201903f0u));
    TSIP.REG_ECH.WORD = 0x000034ffu;
    TSIP.REG_ECH.WORD = 0x00003420u;
    TSIP.REG_ECH.WORD = 0x00003440u;
    TSIP.REG_ECH.WORD = 0x00003460u;
    TSIP.REG_ECH.WORD = 0x00003480u;
    TSIP.REG_ECH.WORD = 0x000034a0u;
    TSIP.REG_ECH.WORD = 0x000034c0u;
    TSIP.REG_E0H.WORD = 0x81080000u;
    TSIP.REG_00H.WORD = 0x00003823u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_34H.WORD = 0x00000804u;
    TSIP.REG_24H.WORD = 0x800090d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000c04u;
    TSIP.REG_24H.WORD = 0x800090d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000004u;
    TSIP.REG_24H.WORD = 0x8000b0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_24H.WORD = 0x000009c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x04001181u;
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
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xd6d6f940u), change_endian_long(0xe92245d9u), change_endian_long(0xc645605cu), change_endian_long(0xb28f666du));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func321(856);
    RX65NHU_func100(change_endian_long(0xd8bb4158u), change_endian_long(0x553ebaf1u), change_endian_long(0x9179a24au), change_endian_long(0xad5f6627u));
    RX65NHU_func314(856+36);
    RX65NHU_func100(change_endian_long(0x5aa5359fu), change_endian_long(0x71f2e646u), change_endian_long(0x03208639u), change_endian_long(0x1615baa0u));
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_34H.WORD = 0x00000004u;
    TSIP.REG_24H.WORD = 0x800090d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x1466af8eu);
    RX65NHU_func080();
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX65NHU_func322(896);
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_24H.WORD = 0x000009c0u;
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
    TSIP.REG_24H.WORD = 0x000040c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_ECH.WORD = 0x3800dbdfu;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX65NHU_func100(change_endian_long(0xb27e9fbdu), change_endian_long(0xe115f4e8u), change_endian_long(0xb201795cu), change_endian_long(0x3f06efc1u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_ECH.WORD = 0x0000b400u;
        TSIP.REG_ECH.WORD = 0xffffffffu;
        TSIP.REG_24H.WORD = 0x000001c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x04000591u;
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
        TSIP.REG_34H.WORD = 0x00000800u;
        TSIP.REG_24H.WORD = 0x800080d0u;
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
        TSIP.REG_34H.WORD = 0x00000008u;
        TSIP.REG_24H.WORD = 0x82001191u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        RX65NHU_func101(change_endian_long(0x8bdd3b88u), change_endian_long(0x2b636fa8u), change_endian_long(0x0753f7bfu), change_endian_long(0x6af5ca87u));
    }
    else
    {
        TSIP.REG_ECH.WORD = 0x00000800u;
        TSIP.REG_34H.WORD = 0x00000400u;
        TSIP.REG_24H.WORD = 0x800080d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        RX65NHU_func101(change_endian_long(0xc62eadaeu), change_endian_long(0x5862457cu), change_endian_long(0x1d100626u), change_endian_long(0x2c2ac2b0u));
    }
    TSIP.REG_ECH.WORD = 0x000034feu;
    TSIP.REG_ECH.WORD = 0x00003420u;
    TSIP.REG_ECH.WORD = 0x00003440u;
    TSIP.REG_ECH.WORD = 0x00003460u;
    TSIP.REG_ECH.WORD = 0x00003480u;
    TSIP.REG_ECH.WORD = 0x000034a0u;
    TSIP.REG_ECH.WORD = 0x000034c0u;
    TSIP.REG_E0H.WORD = 0x81080000u;
    TSIP.REG_00H.WORD = 0x00003823u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_34H.WORD = 0x00000804u;
    TSIP.REG_24H.WORD = 0x800090d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000c04u;
    TSIP.REG_24H.WORD = 0x800090d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000004u;
    TSIP.REG_24H.WORD = 0x8000b0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_24H.WORD = 0x000009c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00001181u;
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
    RX65NHU_func100(change_endian_long(0x521dad94u), change_endian_long(0xa3a128c2u), change_endian_long(0x8fbdbe9fu), change_endian_long(0xb7f1ff69u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x0d515b89u), change_endian_long(0x726d097au), change_endian_long(0x33b6b785u), change_endian_long(0x2e5873e8u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func322(816);
    TSIP.REG_24H.WORD = 0x000011c0u;
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
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xc57741f4u), change_endian_long(0x26767617u), change_endian_long(0xc3c9bea6u), change_endian_long(0x76ed15f3u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func322(856);
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_24H.WORD = 0x000012c0u;
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
    RX65NHU_func100(change_endian_long(0x7d1f18f7u), change_endian_long(0x240372e3u), change_endian_long(0x84e5cdc5u), change_endian_long(0xf47d99c7u));
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
    RX65NHU_func100(change_endian_long(0x1c4a1e93u), change_endian_long(0x40a8d866u), change_endian_long(0x902e8e6fu), change_endian_long(0x844c20fau));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000015u;
    RX65NHU_func320(716);
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
    TSIP.REG_24H.WORD = 0x0000a0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_34H.WORD = 0x00000400u;
    TSIP.REG_24H.WORD = 0x800060d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000800u;
    TSIP.REG_24H.WORD = 0x800060d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000c00u;
    TSIP.REG_24H.WORD = 0x800060d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_E0H.WORD = 0x800103e0u;
    TSIP.REG_00H.WORD = 0x0000031fu;
    TSIP.REG_2CH.WORD = 0x00000023u;
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
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_ECH.WORD = 0x3800dbffu;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX65NHU_func100(change_endian_long(0x9e10feeeu), change_endian_long(0x3a15cd62u), change_endian_long(0x5c365589u), change_endian_long(0x34b75f29u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        oLoop1 = 1;
        while( 1 == oLoop1 )
    {
            TSIP.REG_28H.WORD = 0x00bf0001u;
            TSIP.REG_24H.WORD = 0x04001981u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x000049c1u;
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
            TSIP.REG_24H.WORD = 0x000049c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_28H.WORD = 0x00870001u;
            TSIP.REG_E0H.WORD = 0x800103e0u;
            TSIP.REG_00H.WORD = 0x0000031fu;
            TSIP.REG_2CH.WORD = 0x00000023u;
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
            TSIP.REG_ECH.WORD = 0x3800dbffu;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            RX65NHU_func100(change_endian_long(0x12d69d18u), change_endian_long(0x6996b2a2u), change_endian_long(0x8a10a368u), change_endian_long(0x8224152au));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX65NHU_func101(change_endian_long(0xdcded068u), change_endian_long(0x8f7e7fedu), change_endian_long(0x205d0dfcu), change_endian_long(0x97b6eda4u));
                oLoop1 = 0;
            }
            else
            {
                RX65NHU_func101(change_endian_long(0xb87b7fffu), change_endian_long(0xfee0b41cu), change_endian_long(0x56963999u), change_endian_long(0xce3c3f18u));
            }
        }
    }
    else
    {
        oLoop1 = 1;
        while( 1 == oLoop1 )
    {
            TSIP.REG_28H.WORD = 0x00bf0001u;
            TSIP.REG_24H.WORD = 0x04001991u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x000049c1u;
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
            TSIP.REG_24H.WORD = 0x000049c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_28H.WORD = 0x00870001u;
            TSIP.REG_E0H.WORD = 0x800103e0u;
            TSIP.REG_00H.WORD = 0x0000031fu;
            TSIP.REG_2CH.WORD = 0x00000023u;
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
            TSIP.REG_ECH.WORD = 0x3800dbffu;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            RX65NHU_func100(change_endian_long(0xeab71d12u), change_endian_long(0xccdab212u), change_endian_long(0x9fc8ccf4u), change_endian_long(0x9ac3e684u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_28H.WORD = 0x00bf0001u;
                TSIP.REG_24H.WORD = 0x04001981u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x000049c1u;
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
                TSIP.REG_24H.WORD = 0x000049c1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                RX65NHU_func101(change_endian_long(0xad750607u), change_endian_long(0x47195d89u), change_endian_long(0xfe547617u), change_endian_long(0xbf2e8ccau));
                oLoop1 = 0;
            }
            else
            {
                RX65NHU_func101(change_endian_long(0x8fa8ff87u), change_endian_long(0x9f33e725u), change_endian_long(0x25ae956cu), change_endian_long(0xc4d536c2u));
            }
        }
    }
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_24H.WORD = 0x000088d0u;
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
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000008u;
    TSIP.REG_ECH.WORD = 0x00003bbeu;
    TSIP.REG_ECH.WORD = 0x00007c1du;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_func318.prc
 *********************************************************************************************************************/
