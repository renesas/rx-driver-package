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
* Function Name: RX72M_RX72N_RX66N_func318
*******************************************************************************************************************/ /**
* @details       RX72M func318
* @note          None
*/
void RX72M_RX72N_RX66N_func318(void)
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0x6ec233ddu), change_endian_long(0x361ac9d6u), change_endian_long(0x17c9fff6u), change_endian_long(0x0c837011u));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x47f72600u), change_endian_long(0x99aaeb4bu), change_endian_long(0x9560d580u), change_endian_long(0x5b48c923u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000015u;
    RX72M_RX72N_RX66N_func320(480);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x501c3e9fu), change_endian_long(0x1eb82bd0u), change_endian_long(0x1c7f856bu), change_endian_long(0x244a5d44u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func320(616);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x4b80f36du), change_endian_long(0x168dac15u), change_endian_long(0x5141cc11u), change_endian_long(0x9f430e22u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX72M_RX72N_RX66N_func320(444);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xda1e8294u), change_endian_long(0xc91baa16u), change_endian_long(0x40e2a3a2u), change_endian_long(0x853566a8u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func320(580);
    RX72M_RX72N_RX66N_func323();
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc2e5ce15u), change_endian_long(0xeee0ad8fu), change_endian_long(0x363dfa07u), change_endian_long(0xdccd5621u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX72M_RX72N_RX66N_func321(816);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x00251e40u), change_endian_long(0x9d32d0d0u), change_endian_long(0xd3de5f8fu), change_endian_long(0x5c82e9d9u));
    RX72M_RX72N_RX66N_func314(816+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc12e7568u), change_endian_long(0x8a8ae713u), change_endian_long(0x61ed8a88u), change_endian_long(0x11b32414u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc916c9ebu), change_endian_long(0x5e4c76c7u), change_endian_long(0xf4ddfaa3u), change_endian_long(0x12c10c3du));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func321(856);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0edba318u), change_endian_long(0xe5967cdbu), change_endian_long(0x7bd6a61eu), change_endian_long(0x4f2f13f1u));
    RX72M_RX72N_RX66N_func314(856+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0d2986d9u), change_endian_long(0xe8566bc1u), change_endian_long(0xcae30811u), change_endian_long(0xfb94e1ebu));
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x073a7ca2u), change_endian_long(0xb2828ef4u), change_endian_long(0x0d03d653u), change_endian_long(0xc9f740c0u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000015u;
    RX72M_RX72N_RX66N_func320(408);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe00be483u), change_endian_long(0x329544dfu), change_endian_long(0x803a4481u), change_endian_long(0x43071959u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func320(544);
    RX72M_RX72N_RX66N_func323();
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x05e7f839u), change_endian_long(0xec0ffa92u), change_endian_long(0x92a0d4ecu), change_endian_long(0xb1d75e1cu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX72M_RX72N_RX66N_func321(992);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0f3ece54u), change_endian_long(0xbdad271au), change_endian_long(0x0cd09f29u), change_endian_long(0x1afc9abfu));
    RX72M_RX72N_RX66N_func314(992+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe9110cdeu), change_endian_long(0x2e198313u), change_endian_long(0x0d3d602du), change_endian_long(0x3359a951u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x60172f54u), change_endian_long(0xc2f3a082u), change_endian_long(0xae071e77u), change_endian_long(0xdc5dc042u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func322(816);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x2f5b0bc0u), change_endian_long(0x90dc31eeu), change_endian_long(0x89bb2553u), change_endian_long(0xef07ae24u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX72M_RX72N_RX66N_func321(816);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x08a1f39au), change_endian_long(0xa30e3cefu), change_endian_long(0x03c7d599u), change_endian_long(0xcf342030u));
    RX72M_RX72N_RX66N_func314(816+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x7e15fb01u), change_endian_long(0xb56ccd10u), change_endian_long(0x3450a507u), change_endian_long(0x0063ee04u));
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x6f70e155u), change_endian_long(0x426c771eu), change_endian_long(0xedc662d1u), change_endian_long(0x335e2688u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000015u;
    RX72M_RX72N_RX66N_func320(444);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x40eda3f1u), change_endian_long(0x2d887960u), change_endian_long(0x39880a15u), change_endian_long(0xc913316fu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func320(580);
    RX72M_RX72N_RX66N_func323();
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xe1793e8bu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xb78f053eu), change_endian_long(0xc7e1aac9u), change_endian_long(0x8df748bfu), change_endian_long(0xab109042u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX72M_RX72N_RX66N_func321(952);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x53c39f0bu), change_endian_long(0x107427e5u), change_endian_long(0xbd0240ebu), change_endian_long(0xf97c261fu));
    RX72M_RX72N_RX66N_func314(952+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xb552d45eu), change_endian_long(0xdf11e563u), change_endian_long(0x4416d6a1u), change_endian_long(0x416be3eau));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x685d5644u), change_endian_long(0xf8c1ac6du), change_endian_long(0x8ce28c6au), change_endian_long(0x4cae637du));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func322(992);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xd08e2d1du), change_endian_long(0x45a6aec6u), change_endian_long(0x1ee1f48eu), change_endian_long(0x48f88cedu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX72M_RX72N_RX66N_func321(992);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc31cac4fu), change_endian_long(0xd1e4392du), change_endian_long(0x0fd3fefeu), change_endian_long(0xeaa38b2au));
    RX72M_RX72N_RX66N_func314(992+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x133bc38fu), change_endian_long(0xf004200bu), change_endian_long(0x20b8cf1fu), change_endian_long(0x075166bbu));
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x59abc3b6u), change_endian_long(0xdbe2ddc8u), change_endian_long(0xbd74755eu), change_endian_long(0x5d9e534cu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(856);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xf4d33f9bu), change_endian_long(0x2d891386u), change_endian_long(0x5453468cu), change_endian_long(0x1617839fu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(856);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe60cc83au), change_endian_long(0x6997e9f6u), change_endian_long(0xb145e2e5u), change_endian_long(0xb86471ccu));
    RX72M_RX72N_RX66N_func314(856+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x18270724u), change_endian_long(0xbcd7bdaeu), change_endian_long(0x60116379u), change_endian_long(0xb56845edu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x7f02a06eu), change_endian_long(0x77706d14u), change_endian_long(0xc99c0bbeu), change_endian_long(0x15ef9054u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(992);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x06890f72u), change_endian_long(0x69e870c3u), change_endian_long(0xdd267062u), change_endian_long(0xb8878376u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(992);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xb9b9447eu), change_endian_long(0x8da1b0a2u), change_endian_long(0x61f10a37u), change_endian_long(0x37c67e0du));
    RX72M_RX72N_RX66N_func314(992+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x8580eac9u), change_endian_long(0xe834d8b6u), change_endian_long(0x6defe20fu), change_endian_long(0x1d845e8au));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xb266630cu), change_endian_long(0x0990d50au), change_endian_long(0x0f99dd52u), change_endian_long(0x30f95991u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(816);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x8f9f76a0u), change_endian_long(0x23e20ab8u), change_endian_long(0xe30cd1f7u), change_endian_long(0xa514e100u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(816);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x45f2db29u), change_endian_long(0x966a4206u), change_endian_long(0xacbdd407u), change_endian_long(0xc3aebaf8u));
    RX72M_RX72N_RX66N_func314(816+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x4c7e17f6u), change_endian_long(0xab9e0855u), change_endian_long(0xec64c9a4u), change_endian_long(0x59a6d85bu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xe1793e8bu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xcaf7d00eu), change_endian_long(0x55ba5dd2u), change_endian_long(0x31a803fau), change_endian_long(0x334c5f9du));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(952);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x66809b5du), change_endian_long(0x66dbcb3du), change_endian_long(0xd1ef2eddu), change_endian_long(0xe204912cu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(952);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe7c0f2d0u), change_endian_long(0x2bf0b13bu), change_endian_long(0x17edc931u), change_endian_long(0xac5d9cffu));
    RX72M_RX72N_RX66N_func314(952+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x25d1bceau), change_endian_long(0x7eb41d65u), change_endian_long(0xbef7add1u), change_endian_long(0x9e47b066u));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xbe27ea65u), change_endian_long(0x79e2149du), change_endian_long(0x3b60e1f6u), change_endian_long(0x61bb30f0u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX72M_RX72N_RX66N_func320(480);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xba8f29c2u), change_endian_long(0x83e1c5f3u), change_endian_long(0x6940ac8au), change_endian_long(0xc6ae4ff9u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func320(616);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x051d1a5au), change_endian_long(0x9b6668e2u), change_endian_long(0x7b6c65a0u), change_endian_long(0x343e312au));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(896);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xebc12150u), change_endian_long(0xa30fa55bu), change_endian_long(0x1498694bu), change_endian_long(0x0e8fbb41u));
    RX72M_RX72N_RX66N_func314(896+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x3352298eu), change_endian_long(0x70e289d8u), change_endian_long(0x6eaf8a93u), change_endian_long(0x89b7f816u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x196a5225u), change_endian_long(0x4cb0a208u), change_endian_long(0x486fc863u), change_endian_long(0xe58ad355u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(856);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xaec4438au), change_endian_long(0xfc9c3cddu), change_endian_long(0x396d4190u), change_endian_long(0x476131ffu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(856);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe959005du), change_endian_long(0x6f2a47eeu), change_endian_long(0x56b25d15u), change_endian_long(0xa737d440u));
    RX72M_RX72N_RX66N_func314(856+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x8688b42au), change_endian_long(0x619d1a36u), change_endian_long(0xd64f3525u), change_endian_long(0xc9c2e74du));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xb339abfau), change_endian_long(0x980bed83u), change_endian_long(0xac7073d0u), change_endian_long(0x184614beu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(816);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xd645959du), change_endian_long(0x63970605u), change_endian_long(0x54c33147u), change_endian_long(0xa804679eu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(816);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc8084adbu), change_endian_long(0xb898fabdu), change_endian_long(0x001ada04u), change_endian_long(0xcf2f390eu));
    RX72M_RX72N_RX66N_func314(816+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xd460806cu), change_endian_long(0x89deb5ceu), change_endian_long(0x2cab4adcu), change_endian_long(0xd8cbc7d6u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x189d7fc7u), change_endian_long(0xe6c86780u), change_endian_long(0x4c489722u), change_endian_long(0xb2cf54fcu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(992);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xbfa912c9u), change_endian_long(0x0a414ba6u), change_endian_long(0x721f6b6eu), change_endian_long(0x21ed0e24u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(992);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xaa4d9813u), change_endian_long(0xc43ecefcu), change_endian_long(0x7a15f322u), change_endian_long(0xd76bbdafu));
    RX72M_RX72N_RX66N_func314(992+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x1b8accfeu), change_endian_long(0x1b665917u), change_endian_long(0xaed05b9bu), change_endian_long(0xd029da45u));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xfa21a8a9u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x648201c0u), change_endian_long(0xd4f9e6a7u), change_endian_long(0xf645f999u), change_endian_long(0xc68fd596u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func320(680);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xebaf3f06u), change_endian_long(0xb912bf78u), change_endian_long(0xd804b9bdu), change_endian_long(0x702d3381u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX72M_RX72N_RX66N_func320(408);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
    RX72M_RX72N_RX66N_func080();
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func320(544);
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_24H.WORD = 0x000060d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f0u;
    RX72M_RX72N_RX66N_func101(change_endian_long(0x41b3e6d9u), change_endian_long(0x8cbebf37u), change_endian_long(0x88f92cd4u), change_endian_long(0xf2d8ea64u));
    RX72M_RX72N_RX66N_func316();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0f7149c8u), change_endian_long(0x8aa0c616u), change_endian_long(0x8799eaa8u), change_endian_long(0x6fe36252u));
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe93e02d6u), change_endian_long(0x91a30a76u), change_endian_long(0xddfe1e15u), change_endian_long(0x21cc1727u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(816);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xf01bb077u), change_endian_long(0xebd95284u), change_endian_long(0xa54ed934u), change_endian_long(0x567d0f60u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(816);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x20dee2a9u), change_endian_long(0xa7dd8d6bu), change_endian_long(0xa149747du), change_endian_long(0x7222bcb0u));
    RX72M_RX72N_RX66N_func314(816+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xd928fc88u), change_endian_long(0xfd884c4eu), change_endian_long(0x0c9d8534u), change_endian_long(0x93e265efu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xe1793e8bu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc5b51387u), change_endian_long(0xc2839dd8u), change_endian_long(0x5748920eu), change_endian_long(0xf4831d15u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(952);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xb699557au), change_endian_long(0xb4916309u), change_endian_long(0xbaf18a6bu), change_endian_long(0x4cb04b37u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(952);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x9097bb31u), change_endian_long(0x9b802d80u), change_endian_long(0xb763a164u), change_endian_long(0x2635013bu));
    RX72M_RX72N_RX66N_func314(952+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xd2df0e01u), change_endian_long(0x913a20ccu), change_endian_long(0xda845289u), change_endian_long(0x76996c7cu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0af5fce9u), change_endian_long(0x3ffa97c2u), change_endian_long(0x38886fd6u), change_endian_long(0x89808e9au));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(992);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xb28951f0u), change_endian_long(0x1272c180u), change_endian_long(0x40b307c4u), change_endian_long(0x89164988u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(992);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xb2ecd4ecu), change_endian_long(0xa1826323u), change_endian_long(0x0d352095u), change_endian_long(0x05657203u));
    RX72M_RX72N_RX66N_func314(992+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x891b7c43u), change_endian_long(0xa33fefcfu), change_endian_long(0xcf3e75dbu), change_endian_long(0xed33945bu));
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe569bef5u), change_endian_long(0xbb371f1bu), change_endian_long(0x6207aad3u), change_endian_long(0x7995685eu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func320(752);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x70e7431bu), change_endian_long(0x780dd9a2u), change_endian_long(0x431ca787u), change_endian_long(0x9c6f01ebu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(896);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc67d0de3u), change_endian_long(0x380eab13u), change_endian_long(0xf8bcb771u), change_endian_long(0x1b2cc953u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(896);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x899a8346u), change_endian_long(0xeb0cb91cu), change_endian_long(0x62119adeu), change_endian_long(0xb2bb7bb3u));
    RX72M_RX72N_RX66N_func314(896+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x3ed55a5bu), change_endian_long(0x4f5f7f91u), change_endian_long(0xf902d98bu), change_endian_long(0xdab3eaaeu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x6a90db40u), change_endian_long(0x39d4ad78u), change_endian_long(0x6db21815u), change_endian_long(0x741e0370u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(856);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x37c68315u), change_endian_long(0x95ad2c7du), change_endian_long(0xaa793698u), change_endian_long(0x03e6ad68u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(856);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xf2e1ae5fu), change_endian_long(0x823a8935u), change_endian_long(0xff4cb97fu), change_endian_long(0xeeb8667eu));
    RX72M_RX72N_RX66N_func314(856+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x423f46b3u), change_endian_long(0x2863df99u), change_endian_long(0x974be9e1u), change_endian_long(0xb1260defu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x57e29c7eu), change_endian_long(0xad798564u), change_endian_long(0xb51f96dfu), change_endian_long(0xfe788371u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(816);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0b3e376cu), change_endian_long(0xf5171be5u), change_endian_long(0x59f31eceu), change_endian_long(0xdcf437f8u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(816);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x542409c3u), change_endian_long(0xe903dc64u), change_endian_long(0x7d9df4d7u), change_endian_long(0x87e71f01u));
    RX72M_RX72N_RX66N_func314(816+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xb957925fu), change_endian_long(0x42dc4a73u), change_endian_long(0xffb5d717u), change_endian_long(0xc12c28d2u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x2b823cc8u), change_endian_long(0x7609bd7du), change_endian_long(0xc88a9044u), change_endian_long(0x7fce953bu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(992);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x861cc7f6u), change_endian_long(0x7fb3721bu), change_endian_long(0x795c49b1u), change_endian_long(0x7831ed6au));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(992);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x732ab0d2u), change_endian_long(0xb45fbc17u), change_endian_long(0x58829b7du), change_endian_long(0x3724e71du));
    RX72M_RX72N_RX66N_func314(992+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x7f05d32eu), change_endian_long(0x5af80066u), change_endian_long(0xd318bd53u), change_endian_long(0xd5c57f0du));
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x3a0fe9f3u), change_endian_long(0x7d25cbb3u), change_endian_long(0x789430fau), change_endian_long(0xec0a5933u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func320(716);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe131b149u), change_endian_long(0x854f4369u), change_endian_long(0x64f4a4e8u), change_endian_long(0xbf968376u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(856);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x35b78758u), change_endian_long(0x07dd8164u), change_endian_long(0x8b5be394u), change_endian_long(0x0353f66eu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(856);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xd48d3272u), change_endian_long(0x442bba42u), change_endian_long(0x656aa465u), change_endian_long(0x75b46530u));
    RX72M_RX72N_RX66N_func314(856+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x9f8f0710u), change_endian_long(0x7e99349cu), change_endian_long(0x9f8cca14u), change_endian_long(0xc1a0fc3cu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x5d51bd35u), change_endian_long(0xf7a818d5u), change_endian_long(0xe74b865du), change_endian_long(0xc4b8c9d0u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(816);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x2dcd3e12u), change_endian_long(0xb37f7816u), change_endian_long(0x2ac6a36eu), change_endian_long(0x70f5dc36u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(816);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x602c5aa2u), change_endian_long(0x1f07c92cu), change_endian_long(0xc05fc2e8u), change_endian_long(0x9a5ea54bu));
    RX72M_RX72N_RX66N_func314(816+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x8fdc7534u), change_endian_long(0x87cc9c60u), change_endian_long(0x9c944a4bu), change_endian_long(0xfc6906b1u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xe1793e8bu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x5be2b6d7u), change_endian_long(0x18f65763u), change_endian_long(0x6d6b3365u), change_endian_long(0x647abe0bu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(952);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x7da89eb1u), change_endian_long(0xabf8083cu), change_endian_long(0x3e8ee30fu), change_endian_long(0xd689f05eu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(952);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x3a74e756u), change_endian_long(0x044f6be3u), change_endian_long(0xb4f917ecu), change_endian_long(0xb576bbd4u));
    RX72M_RX72N_RX66N_func314(952+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x47a2362eu), change_endian_long(0xcc7d57d0u), change_endian_long(0x22449ab0u), change_endian_long(0x6a467261u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x549e7868u), change_endian_long(0x46355db8u), change_endian_long(0xea2acd8fu), change_endian_long(0xf71965f7u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(992);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x282a2e24u), change_endian_long(0x351b4ee6u), change_endian_long(0x0a89432fu), change_endian_long(0xab7ce65du));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(992);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x84d2938eu), change_endian_long(0x58536f4fu), change_endian_long(0x555bfc63u), change_endian_long(0x9be1108au));
    RX72M_RX72N_RX66N_func314(992+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0e3c3e24u), change_endian_long(0x85f27e27u), change_endian_long(0x32c75e42u), change_endian_long(0x5802623au));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xfa21a8a9u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x13ab42c0u), change_endian_long(0x6fa6148du), change_endian_long(0xd15aeb3du), change_endian_long(0x9021faf7u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func320(680);
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
    RX72M_RX72N_RX66N_func080();
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX72M_RX72N_RX66N_func322(952);
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f1u;
    RX72M_RX72N_RX66N_func101(change_endian_long(0x7a8362c8u), change_endian_long(0x50762626u), change_endian_long(0x8d7c6dafu), change_endian_long(0x95b2e028u));
    RX72M_RX72N_RX66N_func316();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xdbaec0e8u), change_endian_long(0x61ce93ceu), change_endian_long(0xbfb3732bu), change_endian_long(0x3f1ee1e4u));
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x3920e996u), change_endian_long(0x4f0750c2u), change_endian_long(0xcad2128du), change_endian_long(0x818c9666u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(992);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x8277b328u), change_endian_long(0xfbdd1d08u), change_endian_long(0x79ebf789u), change_endian_long(0xb30d1b33u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(992);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x7d808f42u), change_endian_long(0x6dbef214u), change_endian_long(0x8231635au), change_endian_long(0x06a7f888u));
    RX72M_RX72N_RX66N_func314(992+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x80279a92u), change_endian_long(0x33eaacadu), change_endian_long(0x584bc7bau), change_endian_long(0x661bc31cu));
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x85f518ccu), change_endian_long(0x712b14f0u), change_endian_long(0xee22676fu), change_endian_long(0x9f6a8568u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func320(752);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x7b40ff88u), change_endian_long(0x0180278eu), change_endian_long(0x7952505au), change_endian_long(0x9b2a287fu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(856);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc38e663eu), change_endian_long(0xa9d2b4beu), change_endian_long(0xe10e968cu), change_endian_long(0x999ee333u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(856);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xacd7be51u), change_endian_long(0xc97864b8u), change_endian_long(0xee63a5c2u), change_endian_long(0x73c03d15u));
    RX72M_RX72N_RX66N_func314(856+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xb9ce9c71u), change_endian_long(0x69add583u), change_endian_long(0xeda511c8u), change_endian_long(0xb1f606d5u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x5fbbb935u), change_endian_long(0x91ac5cd1u), change_endian_long(0x236ae19fu), change_endian_long(0x12936ae2u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(816);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x557c3a62u), change_endian_long(0xb338226du), change_endian_long(0xe4e0d1b2u), change_endian_long(0x8ce898dbu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(816);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x8d7fd7e3u), change_endian_long(0x2a732859u), change_endian_long(0x53f462a7u), change_endian_long(0x0eeb0900u));
    RX72M_RX72N_RX66N_func314(816+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xa97f9babu), change_endian_long(0x7df85bb3u), change_endian_long(0xed4ea9c1u), change_endian_long(0xdb4d022du));
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe916d40au), change_endian_long(0x112fa609u), change_endian_long(0x96dfcae8u), change_endian_long(0x00e9482eu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func320(716);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xf101b284u), change_endian_long(0x384a7d30u), change_endian_long(0x44e5b9fdu), change_endian_long(0xaa356005u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(816);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xea69e357u), change_endian_long(0x7f69e944u), change_endian_long(0x68e4e685u), change_endian_long(0x24c17935u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(816);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x5480fd63u), change_endian_long(0x1a04268eu), change_endian_long(0xfdf33196u), change_endian_long(0xb70d8c85u));
    RX72M_RX72N_RX66N_func314(816+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x498a1266u), change_endian_long(0x747400e8u), change_endian_long(0x6543678fu), change_endian_long(0x891c607au));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x8458ae5bu), change_endian_long(0x0c3eedb0u), change_endian_long(0x0893ec22u), change_endian_long(0xfde4d64eu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(992);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x8a747f10u), change_endian_long(0xb4e02affu), change_endian_long(0x00c24db3u), change_endian_long(0x50b9e44au));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(992);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc0df7028u), change_endian_long(0xc683f5d4u), change_endian_long(0x118b9542u), change_endian_long(0x888c36d8u));
    RX72M_RX72N_RX66N_func314(992+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xd194e6afu), change_endian_long(0x6113bbf0u), change_endian_long(0xde310196u), change_endian_long(0x32eee80au));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xfa21a8a9u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0cd433f2u), change_endian_long(0x34b98cd4u), change_endian_long(0x9440ef72u), change_endian_long(0xc2d52dc3u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func320(680);
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
    RX72M_RX72N_RX66N_func080();
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX72M_RX72N_RX66N_func322(992);
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f2u;
    RX72M_RX72N_RX66N_func101(change_endian_long(0x3568048cu), change_endian_long(0xf1a69861u), change_endian_long(0x50051b91u), change_endian_long(0x7565b922u));
    RX72M_RX72N_RX66N_func316();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x03e396f3u), change_endian_long(0x554be0a6u), change_endian_long(0x8dcb2b0eu), change_endian_long(0xa39c4c8cu));
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x150a1820u), change_endian_long(0x74c77fbau), change_endian_long(0x883f6192u), change_endian_long(0x082cf608u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(816);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x3c8cf0d0u), change_endian_long(0x573a8ab4u), change_endian_long(0x307d2abdu), change_endian_long(0x03d9b6adu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(816);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x3147bc14u), change_endian_long(0x08b7c5b6u), change_endian_long(0xdd735312u), change_endian_long(0x09871d32u));
    RX72M_RX72N_RX66N_func314(816+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xd72e751bu), change_endian_long(0x77ffa42fu), change_endian_long(0x7ec3bf26u), change_endian_long(0xc9b1e75fu));
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x8bf77f09u), change_endian_long(0x7fe8bae9u), change_endian_long(0x7b6ade4bu), change_endian_long(0xa6ea8637u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func320(752);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xdd43349eu), change_endian_long(0x24c60021u), change_endian_long(0x6a850e68u), change_endian_long(0xf6907b2au));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(896);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x9e2d10b6u), change_endian_long(0xf90ff2b5u), change_endian_long(0xf2b82d49u), change_endian_long(0x1bc1020fu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(896);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x350d7addu), change_endian_long(0x4914dfefu), change_endian_long(0xeda8e638u), change_endian_long(0x970e3537u));
    RX72M_RX72N_RX66N_func314(896+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x51e19938u), change_endian_long(0x781866dfu), change_endian_long(0x4897a7b7u), change_endian_long(0xbd910b33u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xf8a73517u), change_endian_long(0x4575d6f2u), change_endian_long(0xde0b47e3u), change_endian_long(0xee9e676fu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(856);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xfff94152u), change_endian_long(0x235e9c10u), change_endian_long(0xc79f8a6du), change_endian_long(0x4b2dfed5u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(856);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe1e15940u), change_endian_long(0x704cfb4fu), change_endian_long(0xe62cf398u), change_endian_long(0xd29db588u));
    RX72M_RX72N_RX66N_func314(856+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x755814fbu), change_endian_long(0x238f3d72u), change_endian_long(0x1e7d91d2u), change_endian_long(0xa95c24a3u));
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xd17b5bb4u), change_endian_long(0x2aa07155u), change_endian_long(0xdf07b9fcu), change_endian_long(0xbebd8900u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func320(716);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x75b326d4u), change_endian_long(0x9c40bcbcu), change_endian_long(0x17a55d81u), change_endian_long(0xa25ec217u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(856);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x978c8c1cu), change_endian_long(0x791cff18u), change_endian_long(0x8ad7c4e0u), change_endian_long(0xd78030abu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func322(816);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x081ee079u), change_endian_long(0x40a441a0u), change_endian_long(0x7f2150e3u), change_endian_long(0x63d1657cu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX72M_RX72N_RX66N_func321(816);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x02555ad3u), change_endian_long(0x6ca5b3edu), change_endian_long(0x5037bd72u), change_endian_long(0x09542351u));
    RX72M_RX72N_RX66N_func314(816+36);
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0x3013341cu), change_endian_long(0x8c12a5c6u), change_endian_long(0xcb92fdf2u), change_endian_long(0xb300ddd7u));
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
        RX72M_RX72N_RX66N_func101(change_endian_long(0xacb55555u), change_endian_long(0x1ad6f07cu), change_endian_long(0x0a989b98u), change_endian_long(0x1dc3de40u));
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
        RX72M_RX72N_RX66N_func101(change_endian_long(0x5d41d73bu), change_endian_long(0x907c28e5u), change_endian_long(0xf5b899c6u), change_endian_long(0x6dc193fau));
    }
    RX72M_RX72N_RX66N_func100(change_endian_long(0x791df332u), change_endian_long(0x35317477u), change_endian_long(0xbf84e4dau), change_endian_long(0x1105842eu));
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc8c48ea7u), change_endian_long(0x65d8ac09u), change_endian_long(0xfa9e2684u), change_endian_long(0x73da6ccfu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func321(856);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x723314dau), change_endian_long(0x928d413au), change_endian_long(0x7e10ba42u), change_endian_long(0x943da228u));
    RX72M_RX72N_RX66N_func314(856+36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xcf47ed35u), change_endian_long(0x1cf08646u), change_endian_long(0x278e6b5au), change_endian_long(0x663ad94cu));
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
    RX72M_RX72N_RX66N_func080();
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX72M_RX72N_RX66N_func322(896);
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0x814ee952u), change_endian_long(0x25f66b7bu), change_endian_long(0x3d696019u), change_endian_long(0xda2fe15bu));
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
        RX72M_RX72N_RX66N_func101(change_endian_long(0xf099cb7cu), change_endian_long(0x42b9d5efu), change_endian_long(0xe971b086u), change_endian_long(0x8fe08207u));
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
        RX72M_RX72N_RX66N_func101(change_endian_long(0x98286cc5u), change_endian_long(0x0732fd6fu), change_endian_long(0xc4fd1d5au), change_endian_long(0x984de42fu));
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe61e55d8u), change_endian_long(0xf951e3ebu), change_endian_long(0x065f8033u), change_endian_long(0x50f81e87u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x4a59f2a4u), change_endian_long(0x799d7e32u), change_endian_long(0xa5c39221u), change_endian_long(0xe2af06beu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func322(816);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x7cf82ffau), change_endian_long(0xe4721466u), change_endian_long(0xaaf6cd2cu), change_endian_long(0x48ab8322u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func322(856);
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xff5654f3u), change_endian_long(0xf8ab7e0cu), change_endian_long(0x8ec6fc4fu), change_endian_long(0xe296fe07u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func320(680);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x899d7651u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x82c7ddaeu), change_endian_long(0x830a1bb6u), change_endian_long(0x85b252d1u), change_endian_long(0xd492150bu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000015u;
    RX72M_RX72N_RX66N_func320(716);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x7775adb6u);
    RX72M_RX72N_RX66N_func080();
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func320(752);
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0xfe758b1bu), change_endian_long(0x9a3feaa7u), change_endian_long(0x28f87b7cu), change_endian_long(0x33806ed7u));
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
            RX72M_RX72N_RX66N_func100(change_endian_long(0x12dc50adu), change_endian_long(0x39928415u), change_endian_long(0x1683d95du), change_endian_long(0x34deb521u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX72M_RX72N_RX66N_func101(change_endian_long(0x78c92ec8u), change_endian_long(0x678f6040u), change_endian_long(0x5adc0241u), change_endian_long(0x85e72452u));
                oLoop1 = 0;
            }
            else
            {
                RX72M_RX72N_RX66N_func101(change_endian_long(0x08bd056du), change_endian_long(0xcfc67f81u), change_endian_long(0x9228a634u), change_endian_long(0x4ef05d4fu));
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
            RX72M_RX72N_RX66N_func100(change_endian_long(0xf2f1c7cau), change_endian_long(0x36faba76u), change_endian_long(0x9558f305u), change_endian_long(0xa5332f41u));
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0x05622fd2u), change_endian_long(0x665eea55u), change_endian_long(0x537bf5feu), change_endian_long(0x077e2aa1u));
                oLoop1 = 0;
            }
            else
            {
                RX72M_RX72N_RX66N_func101(change_endian_long(0xd2c84ec6u), change_endian_long(0xaf32dc87u), change_endian_long(0x03c51488u), change_endian_long(0xb11d1babu));
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
 End of function ./input_dir/RX72M/RX72M_func318.prc
 *********************************************************************************************************************/
