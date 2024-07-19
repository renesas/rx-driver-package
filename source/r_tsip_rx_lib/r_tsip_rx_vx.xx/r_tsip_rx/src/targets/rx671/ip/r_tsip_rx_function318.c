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
* Function Name: RX671_func318
*******************************************************************************************************************/ /**
* @details       RX671 func318
* @note          None
*/
void RX671_func318(void)
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
    RX671_func100(change_endian_long(0x451ae57bu), change_endian_long(0xda6903e8u), change_endian_long(0xe08cb146u), change_endian_long(0xd30bbd56u));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
    RX671_func080();
    RX671_func100(change_endian_long(0x5c7eb91au), change_endian_long(0xe697cb55u), change_endian_long(0xf9c9dc3bu), change_endian_long(0xfce3fc8cu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000015u;
    RX671_func320(480);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
    RX671_func080();
    RX671_func100(change_endian_long(0x9a2d1b34u), change_endian_long(0x7037af1eu), change_endian_long(0xa513cfe1u), change_endian_long(0x9c3254d6u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func320(616);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
    RX671_func080();
    RX671_func100(change_endian_long(0xdd9bd4c3u), change_endian_long(0xa12f5001u), change_endian_long(0xa92e458au), change_endian_long(0x48f0fc55u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func320(444);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
    RX671_func080();
    RX671_func100(change_endian_long(0x8415f0c6u), change_endian_long(0x54e7dd73u), change_endian_long(0xf9ddb209u), change_endian_long(0x7fdf4388u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func320(580);
    RX671_func323();
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX671_func080();
    RX671_func100(change_endian_long(0x32ca65c7u), change_endian_long(0xfb033752u), change_endian_long(0x5ed38574u), change_endian_long(0x1ff74f5cu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func321(816);
    RX671_func100(change_endian_long(0x7fca055du), change_endian_long(0x57362132u), change_endian_long(0xa6c18753u), change_endian_long(0x23a1df10u));
    RX671_func314(816+36);
    RX671_func100(change_endian_long(0x6e73b22du), change_endian_long(0x73c21dc3u), change_endian_long(0xbef3b9feu), change_endian_long(0xc6cd0cf3u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX671_func080();
    RX671_func100(change_endian_long(0x878d34e0u), change_endian_long(0xdaa09638u), change_endian_long(0x5181fe87u), change_endian_long(0x9af11d2fu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func321(856);
    RX671_func100(change_endian_long(0x73da0374u), change_endian_long(0x833f53d7u), change_endian_long(0x4aadbef5u), change_endian_long(0x9e2b26c9u));
    RX671_func314(856+36);
    RX671_func100(change_endian_long(0xfebcfb4eu), change_endian_long(0x907b1dcau), change_endian_long(0x81baf838u), change_endian_long(0x8f08d6afu));
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
    RX671_func080();
    RX671_func100(change_endian_long(0xbfb34b6cu), change_endian_long(0x8c11f686u), change_endian_long(0xfbb84014u), change_endian_long(0xc8f7f10eu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000015u;
    RX671_func320(408);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
    RX671_func080();
    RX671_func100(change_endian_long(0xc4365deau), change_endian_long(0x334f75b5u), change_endian_long(0xe1a1be98u), change_endian_long(0x6644b7bcu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func320(544);
    RX671_func323();
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX671_func080();
    RX671_func100(change_endian_long(0xfd15224fu), change_endian_long(0xd9d35eb8u), change_endian_long(0xa0614aedu), change_endian_long(0xb4999d57u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func321(992);
    RX671_func100(change_endian_long(0x8a75d94eu), change_endian_long(0xa0e55577u), change_endian_long(0x029d6887u), change_endian_long(0xd434f7beu));
    RX671_func314(992+36);
    RX671_func100(change_endian_long(0xc2e0e565u), change_endian_long(0xf5f0952au), change_endian_long(0x05d2f62fu), change_endian_long(0x0317982eu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX671_func080();
    RX671_func100(change_endian_long(0x24561808u), change_endian_long(0x502b5ba4u), change_endian_long(0xf21460abu), change_endian_long(0xe47f35f4u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func322(816);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x16a8a319u), change_endian_long(0xec4fd6b7u), change_endian_long(0x4b142bffu), change_endian_long(0x061f798au));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func321(816);
    RX671_func100(change_endian_long(0x50e4fc0au), change_endian_long(0x28464b73u), change_endian_long(0xa28884a2u), change_endian_long(0x0e3572ccu));
    RX671_func314(816+36);
    RX671_func100(change_endian_long(0x77f8b0deu), change_endian_long(0x4b16cb58u), change_endian_long(0x8de80e3cu), change_endian_long(0xe9d44b7bu));
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
    RX671_func080();
    RX671_func100(change_endian_long(0xf93bf9d7u), change_endian_long(0xb6f919ebu), change_endian_long(0x2dfa6edeu), change_endian_long(0xc23255fbu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000015u;
    RX671_func320(444);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
    RX671_func080();
    RX671_func100(change_endian_long(0xefd6e186u), change_endian_long(0x0947e359u), change_endian_long(0x84996c95u), change_endian_long(0xd7a69786u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func320(580);
    RX671_func323();
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xe1793e8bu);
    RX671_func080();
    RX671_func100(change_endian_long(0xcf421a98u), change_endian_long(0x9050e92eu), change_endian_long(0x5761cd1du), change_endian_long(0x2fe8f9d8u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func321(952);
    RX671_func100(change_endian_long(0x84766a4bu), change_endian_long(0x6f543690u), change_endian_long(0x277a0038u), change_endian_long(0xd620e25au));
    RX671_func314(952+36);
    RX671_func100(change_endian_long(0xff1c205fu), change_endian_long(0x9497711cu), change_endian_long(0x6a263575u), change_endian_long(0xd76fb4dfu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX671_func080();
    RX671_func100(change_endian_long(0xdec99bc6u), change_endian_long(0x5fc0baa1u), change_endian_long(0xd41fe72cu), change_endian_long(0x50518912u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func322(992);
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
    RX671_func080();
    RX671_func100(change_endian_long(0xd64aee74u), change_endian_long(0x5fa7fe17u), change_endian_long(0x4b608a96u), change_endian_long(0xa86eaaecu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func321(992);
    RX671_func100(change_endian_long(0x5b318342u), change_endian_long(0x11d92de7u), change_endian_long(0x7a6920d6u), change_endian_long(0xf7de1e94u));
    RX671_func314(992+36);
    RX671_func100(change_endian_long(0xef4ab841u), change_endian_long(0x8bb09b67u), change_endian_long(0x389f288fu), change_endian_long(0x1ae1d321u));
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
    RX671_func080();
    RX671_func100(change_endian_long(0x84171381u), change_endian_long(0x8882b3b1u), change_endian_long(0xb264df25u), change_endian_long(0x0450d6ccu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(856);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x9a44d637u), change_endian_long(0x270c31eau), change_endian_long(0x3c4a6bf8u), change_endian_long(0xbc4e4d2du));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(856);
    RX671_func100(change_endian_long(0x4c2ea882u), change_endian_long(0xed5b8f1au), change_endian_long(0xb7b7d7fdu), change_endian_long(0x6e42206eu));
    RX671_func314(856+36);
    RX671_func100(change_endian_long(0x08731004u), change_endian_long(0x58de3e5bu), change_endian_long(0x1ef068e8u), change_endian_long(0xf6c9006eu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX671_func080();
    RX671_func100(change_endian_long(0x6565e2f9u), change_endian_long(0x9d019a7au), change_endian_long(0x86afb7b5u), change_endian_long(0x1e6ee5d3u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(992);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x5c29553du), change_endian_long(0xf7e19c98u), change_endian_long(0xfd94cb44u), change_endian_long(0x1ce2f316u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(992);
    RX671_func100(change_endian_long(0xb8d69d9du), change_endian_long(0xf7428297u), change_endian_long(0x0a8e8efau), change_endian_long(0xe8771fb3u));
    RX671_func314(992+36);
    RX671_func100(change_endian_long(0xe8fe4d5au), change_endian_long(0x0fedf1d6u), change_endian_long(0x4c45e8cbu), change_endian_long(0xb72ce464u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX671_func080();
    RX671_func100(change_endian_long(0x97a079a0u), change_endian_long(0x1faa712du), change_endian_long(0xd04b85a1u), change_endian_long(0x974d1340u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(816);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x70337d14u), change_endian_long(0x2d84569fu), change_endian_long(0x9fa4450cu), change_endian_long(0x16aabcd7u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(816);
    RX671_func100(change_endian_long(0xfbadeb01u), change_endian_long(0xd0d607cau), change_endian_long(0xb2898726u), change_endian_long(0xe0ffd97eu));
    RX671_func314(816+36);
    RX671_func100(change_endian_long(0x7be15251u), change_endian_long(0x2438c24cu), change_endian_long(0xfe2e9479u), change_endian_long(0xbbc6cab9u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xe1793e8bu);
    RX671_func080();
    RX671_func100(change_endian_long(0xda71f3a2u), change_endian_long(0xa6ebf90eu), change_endian_long(0x960894afu), change_endian_long(0xf68fdf10u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(952);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x45a597bbu), change_endian_long(0xb195c6fbu), change_endian_long(0x0a7b0dc2u), change_endian_long(0xae0da8a7u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(952);
    RX671_func100(change_endian_long(0x00ab6a56u), change_endian_long(0x3a118282u), change_endian_long(0x97f242e3u), change_endian_long(0x4893f4f0u));
    RX671_func314(952+36);
    RX671_func100(change_endian_long(0x2304f091u), change_endian_long(0x6f8c12eeu), change_endian_long(0xa90ae1d2u), change_endian_long(0x03b4c345u));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
    RX671_func080();
    RX671_func100(change_endian_long(0x9cf64403u), change_endian_long(0xdb505f83u), change_endian_long(0x66f11725u), change_endian_long(0xab2d122bu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func320(480);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
    RX671_func080();
    RX671_func100(change_endian_long(0xc9937772u), change_endian_long(0xf0032e05u), change_endian_long(0xfdf3fbd2u), change_endian_long(0xd6be8c01u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func320(616);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x9fa01b50u), change_endian_long(0x6d85c238u), change_endian_long(0xa71749b4u), change_endian_long(0x78f1ca0fu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(896);
    RX671_func100(change_endian_long(0x2b0d6c9du), change_endian_long(0x34220349u), change_endian_long(0xd073e14du), change_endian_long(0xb07ba985u));
    RX671_func314(896+36);
    RX671_func100(change_endian_long(0x86de8269u), change_endian_long(0x9ecd3523u), change_endian_long(0xfac20c7du), change_endian_long(0xa953050cu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX671_func080();
    RX671_func100(change_endian_long(0xe76b700cu), change_endian_long(0xd1df7902u), change_endian_long(0x1635ce9fu), change_endian_long(0xff1a2065u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(856);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x4b44a5d9u), change_endian_long(0xaa9765d6u), change_endian_long(0xdbb3ec01u), change_endian_long(0xe1ded824u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(856);
    RX671_func100(change_endian_long(0x11f1449eu), change_endian_long(0x855cf3a3u), change_endian_long(0xe20d6e2bu), change_endian_long(0xb120551au));
    RX671_func314(856+36);
    RX671_func100(change_endian_long(0x145dbbd6u), change_endian_long(0xc5536fc2u), change_endian_long(0xfa22ff0cu), change_endian_long(0x35fabd6cu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX671_func080();
    RX671_func100(change_endian_long(0xf20e22d1u), change_endian_long(0x9a03fd30u), change_endian_long(0xcca51ddbu), change_endian_long(0x210add3au));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(816);
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
    RX671_func080();
    RX671_func100(change_endian_long(0xd7f23802u), change_endian_long(0xb2a20929u), change_endian_long(0x33d75f8eu), change_endian_long(0x85681590u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(816);
    RX671_func100(change_endian_long(0xd3f3a9b8u), change_endian_long(0x75e15fb2u), change_endian_long(0x46cbeb05u), change_endian_long(0x27b71bb9u));
    RX671_func314(816+36);
    RX671_func100(change_endian_long(0xee867852u), change_endian_long(0xf2d82d1eu), change_endian_long(0x47f35c95u), change_endian_long(0x92604747u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX671_func080();
    RX671_func100(change_endian_long(0x83d42c42u), change_endian_long(0x75ac69a4u), change_endian_long(0xc3970dafu), change_endian_long(0x8645c480u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(992);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x9a39820cu), change_endian_long(0xbd47131fu), change_endian_long(0x0cfb7ceeu), change_endian_long(0xa2928713u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(992);
    RX671_func100(change_endian_long(0x55656731u), change_endian_long(0x60ce93dau), change_endian_long(0x69284809u), change_endian_long(0xdd73126bu));
    RX671_func314(992+36);
    RX671_func100(change_endian_long(0x59e12930u), change_endian_long(0xef650288u), change_endian_long(0x07126db7u), change_endian_long(0xeeb63e60u));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xfa21a8a9u);
    RX671_func080();
    RX671_func100(change_endian_long(0x65df3fcfu), change_endian_long(0xec97394cu), change_endian_long(0x49b7530cu), change_endian_long(0x555fd9fbu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func320(680);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
    RX671_func080();
    RX671_func100(change_endian_long(0x4c2f53bbu), change_endian_long(0x8bbedb4eu), change_endian_long(0xac5deb1cu), change_endian_long(0xe0e180adu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func320(408);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func320(544);
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_24H.WORD = 0x000060d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f0u;
    RX671_func101(change_endian_long(0xcde888d9u), change_endian_long(0x69d99dabu), change_endian_long(0x0c7762bcu), change_endian_long(0x00da7fe2u));
    RX671_func316();
    RX671_func100(change_endian_long(0x08d1b36au), change_endian_long(0x9692e350u), change_endian_long(0x9a8ac89fu), change_endian_long(0xd131f58du));
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX671_func080();
    RX671_func100(change_endian_long(0xe316bb6eu), change_endian_long(0x3f19520fu), change_endian_long(0xfa6316b5u), change_endian_long(0x2e5c7f66u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(816);
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
    RX671_func080();
    RX671_func100(change_endian_long(0xe4a58c94u), change_endian_long(0x2955f8cau), change_endian_long(0x46db7c22u), change_endian_long(0x02f9c786u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(816);
    RX671_func100(change_endian_long(0xe124f801u), change_endian_long(0x38f268b3u), change_endian_long(0x717a3300u), change_endian_long(0x5b4be9e6u));
    RX671_func314(816+36);
    RX671_func100(change_endian_long(0xde701728u), change_endian_long(0x3d34ebb4u), change_endian_long(0xc269352au), change_endian_long(0x272ace86u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xe1793e8bu);
    RX671_func080();
    RX671_func100(change_endian_long(0x7f9bb764u), change_endian_long(0x5fb9665cu), change_endian_long(0x1c43f4fbu), change_endian_long(0x5d9e025eu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(952);
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
    RX671_func080();
    RX671_func100(change_endian_long(0xf535d07eu), change_endian_long(0xf531606eu), change_endian_long(0x2e2e0e33u), change_endian_long(0x38e3d755u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(952);
    RX671_func100(change_endian_long(0xac78c276u), change_endian_long(0x84eb1a1au), change_endian_long(0x9d398dcfu), change_endian_long(0x7c4d4b54u));
    RX671_func314(952+36);
    RX671_func100(change_endian_long(0xac176778u), change_endian_long(0x379ee59du), change_endian_long(0xe6318f31u), change_endian_long(0x63ac3293u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX671_func080();
    RX671_func100(change_endian_long(0x44b5ae1bu), change_endian_long(0x67b11ca6u), change_endian_long(0xa6c7f8d5u), change_endian_long(0xa6c9325cu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(992);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x48362689u), change_endian_long(0xd4918756u), change_endian_long(0x2066cf0au), change_endian_long(0xf2443305u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(992);
    RX671_func100(change_endian_long(0xb4be05aeu), change_endian_long(0xa57e6516u), change_endian_long(0x233cd0afu), change_endian_long(0xa70e92f7u));
    RX671_func314(992+36);
    RX671_func100(change_endian_long(0x9e7c7fb9u), change_endian_long(0x49197778u), change_endian_long(0xc4eb0924u), change_endian_long(0xdacf0ee6u));
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
    RX671_func080();
    RX671_func100(change_endian_long(0x374ff5bbu), change_endian_long(0xc0c50f6fu), change_endian_long(0x6dd5425eu), change_endian_long(0xa2e95613u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func320(752);
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
    RX671_func080();
    RX671_func100(change_endian_long(0xacf299d0u), change_endian_long(0xa14e2e9eu), change_endian_long(0x18a45e0eu), change_endian_long(0x3d29d8d8u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(896);
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
    RX671_func080();
    RX671_func100(change_endian_long(0xadda805eu), change_endian_long(0x2c3d9aa6u), change_endian_long(0x6e317e61u), change_endian_long(0x299fba9au));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(896);
    RX671_func100(change_endian_long(0x917ef7a2u), change_endian_long(0x0fafb900u), change_endian_long(0x46cf8010u), change_endian_long(0x1d86ba51u));
    RX671_func314(896+36);
    RX671_func100(change_endian_long(0xda5e6ab6u), change_endian_long(0x318e5eeeu), change_endian_long(0xdda7a477u), change_endian_long(0x107cb01du));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX671_func080();
    RX671_func100(change_endian_long(0x72ab0348u), change_endian_long(0xca499c13u), change_endian_long(0xcbf6a3fdu), change_endian_long(0xd80441f1u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(856);
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
    RX671_func080();
    RX671_func100(change_endian_long(0xad4e939cu), change_endian_long(0x360f937cu), change_endian_long(0x43f83efbu), change_endian_long(0x109bc0b1u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(856);
    RX671_func100(change_endian_long(0x10572f23u), change_endian_long(0xfe9e0aebu), change_endian_long(0xe2b9220du), change_endian_long(0x0da928aeu));
    RX671_func314(856+36);
    RX671_func100(change_endian_long(0x7225e379u), change_endian_long(0x0e14722fu), change_endian_long(0x36d88c5eu), change_endian_long(0x0bfa1e70u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX671_func080();
    RX671_func100(change_endian_long(0xa8931b69u), change_endian_long(0x3d4ab7b3u), change_endian_long(0x437c23d6u), change_endian_long(0xc9f5c93au));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(816);
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
    RX671_func080();
    RX671_func100(change_endian_long(0xc0fde80bu), change_endian_long(0xe42f81ceu), change_endian_long(0x5c2e03f2u), change_endian_long(0x0eb46752u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(816);
    RX671_func100(change_endian_long(0x83470df4u), change_endian_long(0xe9579ea0u), change_endian_long(0x48ba57b0u), change_endian_long(0x75c16a29u));
    RX671_func314(816+36);
    RX671_func100(change_endian_long(0x263b455bu), change_endian_long(0x36117cd0u), change_endian_long(0x4a2d543bu), change_endian_long(0x1d14a89du));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX671_func080();
    RX671_func100(change_endian_long(0xed7672c4u), change_endian_long(0xff0c8155u), change_endian_long(0xad12f5c5u), change_endian_long(0x33792295u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(992);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x66178446u), change_endian_long(0xefc84e21u), change_endian_long(0x68e92481u), change_endian_long(0x9d6efe9du));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(992);
    RX671_func100(change_endian_long(0xe9659743u), change_endian_long(0x9443e9d3u), change_endian_long(0x92ca79ffu), change_endian_long(0x24562a8cu));
    RX671_func314(992+36);
    RX671_func100(change_endian_long(0xfd4edcb6u), change_endian_long(0xd321cb4au), change_endian_long(0x417e862au), change_endian_long(0x961f8a01u));
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
    RX671_func080();
    RX671_func100(change_endian_long(0x4c7c918fu), change_endian_long(0x722c4233u), change_endian_long(0xd4ca1662u), change_endian_long(0x23638a47u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func320(716);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x7a9479bbu), change_endian_long(0x270a8d72u), change_endian_long(0x1a4bb9c5u), change_endian_long(0x8e905fbcu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(856);
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
    RX671_func080();
    RX671_func100(change_endian_long(0xcd4c600du), change_endian_long(0x3ca7abbfu), change_endian_long(0xd4c3f3e4u), change_endian_long(0x3ae123ddu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(856);
    RX671_func100(change_endian_long(0x34620a04u), change_endian_long(0x547d45adu), change_endian_long(0xe2f14878u), change_endian_long(0x4eeb2384u));
    RX671_func314(856+36);
    RX671_func100(change_endian_long(0x0383fcc7u), change_endian_long(0x0a73d082u), change_endian_long(0xa684f23eu), change_endian_long(0x9621aea1u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX671_func080();
    RX671_func100(change_endian_long(0x59162075u), change_endian_long(0x65f7ae90u), change_endian_long(0x2ede0798u), change_endian_long(0xfcab09eau));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(816);
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
    RX671_func080();
    RX671_func100(change_endian_long(0xe8ceba2eu), change_endian_long(0x829f4b1bu), change_endian_long(0x2d46c7fcu), change_endian_long(0xef399d9cu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(816);
    RX671_func100(change_endian_long(0xa51849c2u), change_endian_long(0x609b9241u), change_endian_long(0x0db8d382u), change_endian_long(0xf78bfbb0u));
    RX671_func314(816+36);
    RX671_func100(change_endian_long(0x3a1a03ecu), change_endian_long(0x040d57a1u), change_endian_long(0xbf63f152u), change_endian_long(0xea01770fu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xe1793e8bu);
    RX671_func080();
    RX671_func100(change_endian_long(0x68be73b1u), change_endian_long(0x28fb7592u), change_endian_long(0x45ecf08du), change_endian_long(0xca8a6d06u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(952);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x4689d871u), change_endian_long(0xc87a459fu), change_endian_long(0x7664b6beu), change_endian_long(0x1e5bd279u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(952);
    RX671_func100(change_endian_long(0xaeeee892u), change_endian_long(0x8e0b37b9u), change_endian_long(0xed1372c4u), change_endian_long(0x7cd7a422u));
    RX671_func314(952+36);
    RX671_func100(change_endian_long(0x2dcc1eedu), change_endian_long(0xc797d978u), change_endian_long(0xecfcf26fu), change_endian_long(0x9577195eu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX671_func080();
    RX671_func100(change_endian_long(0x210a77bau), change_endian_long(0xdd55241du), change_endian_long(0x41925c8cu), change_endian_long(0x6659fbf8u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(992);
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
    RX671_func080();
    RX671_func100(change_endian_long(0xca448e0bu), change_endian_long(0xf820bbccu), change_endian_long(0x0ebde198u), change_endian_long(0x842f1f28u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(992);
    RX671_func100(change_endian_long(0x98a45341u), change_endian_long(0x4f6aa0b6u), change_endian_long(0xcc79b7a2u), change_endian_long(0x486bf324u));
    RX671_func314(992+36);
    RX671_func100(change_endian_long(0x85941a3fu), change_endian_long(0x1e4c50f9u), change_endian_long(0x818c4d35u), change_endian_long(0xdf61ed54u));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xfa21a8a9u);
    RX671_func080();
    RX671_func100(change_endian_long(0x756ec7efu), change_endian_long(0x7cbb5ed5u), change_endian_long(0x6abf249cu), change_endian_long(0x408f8850u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func320(680);
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
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func322(952);
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f1u;
    RX671_func101(change_endian_long(0x2181b0eeu), change_endian_long(0xb8e1d4b8u), change_endian_long(0x5c6eb0efu), change_endian_long(0xfee5e617u));
    RX671_func316();
    RX671_func100(change_endian_long(0x68636c78u), change_endian_long(0x3b8045b6u), change_endian_long(0x14c734fcu), change_endian_long(0x0fc7a670u));
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX671_func080();
    RX671_func100(change_endian_long(0xd71d3a2eu), change_endian_long(0x699d4265u), change_endian_long(0x41d02dbfu), change_endian_long(0xe4369f6eu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(992);
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
    RX671_func080();
    RX671_func100(change_endian_long(0xf604c526u), change_endian_long(0xcd4d629cu), change_endian_long(0xd3b84a5fu), change_endian_long(0x1cb55e3bu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(992);
    RX671_func100(change_endian_long(0x1f5f6005u), change_endian_long(0x2a155eadu), change_endian_long(0xc9b42bdcu), change_endian_long(0x1a88c250u));
    RX671_func314(992+36);
    RX671_func100(change_endian_long(0xf19140a0u), change_endian_long(0xda7f1b1eu), change_endian_long(0xda89b563u), change_endian_long(0x5849634bu));
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
    RX671_func080();
    RX671_func100(change_endian_long(0x170b33d1u), change_endian_long(0x92ab84b4u), change_endian_long(0x197b6668u), change_endian_long(0x663337a2u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func320(752);
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
    RX671_func080();
    RX671_func100(change_endian_long(0xfade2b0eu), change_endian_long(0xc2414df4u), change_endian_long(0x3e6e1210u), change_endian_long(0x25a98a09u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(856);
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
    RX671_func080();
    RX671_func100(change_endian_long(0xb6216dedu), change_endian_long(0x0377df66u), change_endian_long(0xdd968423u), change_endian_long(0xc11da818u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(856);
    RX671_func100(change_endian_long(0x021d8f29u), change_endian_long(0xb741c715u), change_endian_long(0x868f8f22u), change_endian_long(0xbc5c590du));
    RX671_func314(856+36);
    RX671_func100(change_endian_long(0x7a37274bu), change_endian_long(0x95e03f8du), change_endian_long(0xd741c508u), change_endian_long(0x5eddfc7cu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX671_func080();
    RX671_func100(change_endian_long(0xda015e9au), change_endian_long(0xbdf8703bu), change_endian_long(0x8fb69158u), change_endian_long(0x2e0b1a0au));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(816);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x10c7db5bu), change_endian_long(0x76908ab4u), change_endian_long(0x1d3df5d6u), change_endian_long(0x084da01au));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(816);
    RX671_func100(change_endian_long(0x1d94d513u), change_endian_long(0x5775e62bu), change_endian_long(0xa9cdc2c6u), change_endian_long(0x4a8da7d3u));
    RX671_func314(816+36);
    RX671_func100(change_endian_long(0xd52bce27u), change_endian_long(0x82f070cdu), change_endian_long(0xe37151efu), change_endian_long(0x3f0f6804u));
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
    RX671_func080();
    RX671_func100(change_endian_long(0xa596c0c9u), change_endian_long(0x22e36313u), change_endian_long(0xede0b74bu), change_endian_long(0xfb39ccdbu));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func320(716);
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
    RX671_func080();
    RX671_func100(change_endian_long(0xf86e5cffu), change_endian_long(0x654086efu), change_endian_long(0x13224938u), change_endian_long(0x20143deeu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(816);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x397576abu), change_endian_long(0xe61e8fcbu), change_endian_long(0xf685f87eu), change_endian_long(0x6fa99adcu));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(816);
    RX671_func100(change_endian_long(0xa16b33acu), change_endian_long(0x8b224639u), change_endian_long(0x8a70c65eu), change_endian_long(0xdb72fe72u));
    RX671_func314(816+36);
    RX671_func100(change_endian_long(0xe88d3055u), change_endian_long(0xa8fb221eu), change_endian_long(0xaf72590cu), change_endian_long(0x80bf462eu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x8823a563u);
    RX671_func080();
    RX671_func100(change_endian_long(0x1e69c601u), change_endian_long(0xca89c6f8u), change_endian_long(0x2c45a7f0u), change_endian_long(0x849c4d12u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(992);
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
    RX671_func080();
    RX671_func100(change_endian_long(0xaf1fb7d1u), change_endian_long(0xb4a520feu), change_endian_long(0x680b635eu), change_endian_long(0xffc3a8a6u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(992);
    RX671_func100(change_endian_long(0xb393685bu), change_endian_long(0x9109ee85u), change_endian_long(0x42137295u), change_endian_long(0x3367e809u));
    RX671_func314(992+36);
    RX671_func100(change_endian_long(0xb728304au), change_endian_long(0x6eeaa4c3u), change_endian_long(0xa7f38589u), change_endian_long(0xebb9c14du));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xfa21a8a9u);
    RX671_func080();
    RX671_func100(change_endian_long(0x2b01ba48u), change_endian_long(0xb557ea63u), change_endian_long(0x451bfd71u), change_endian_long(0x847ee8c4u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func320(680);
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
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func322(992);
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f2u;
    RX671_func101(change_endian_long(0x0f69385au), change_endian_long(0xb91613c1u), change_endian_long(0x4064e093u), change_endian_long(0x785c1c0fu));
    RX671_func316();
    RX671_func100(change_endian_long(0xe30f0742u), change_endian_long(0xda2c6bb3u), change_endian_long(0xfa0a9c67u), change_endian_long(0xb1426c00u));
    TSIP.REG_28H.WORD = 0x00a30001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX671_func080();
    RX671_func100(change_endian_long(0x6c9fd8e5u), change_endian_long(0xc8cc7c1du), change_endian_long(0xbef5eb1eu), change_endian_long(0x1d75db8au));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(816);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x9400fa58u), change_endian_long(0x42265eb0u), change_endian_long(0x36134986u), change_endian_long(0xb912e6a0u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(816);
    RX671_func100(change_endian_long(0x3e1a0a5eu), change_endian_long(0xf4874ac6u), change_endian_long(0x9f7404bfu), change_endian_long(0xd385b88fu));
    RX671_func314(816+36);
    RX671_func100(change_endian_long(0xdf2a22dfu), change_endian_long(0x5f541f3fu), change_endian_long(0xbf0d11efu), change_endian_long(0x03650619u));
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
    RX671_func080();
    RX671_func100(change_endian_long(0x6e8f1c55u), change_endian_long(0x65b5d7a5u), change_endian_long(0xd5532fdeu), change_endian_long(0x150e9de8u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func320(752);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x4a2292c0u), change_endian_long(0xfb594adbu), change_endian_long(0x33e5441fu), change_endian_long(0xacb911e5u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(896);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x19d995a6u), change_endian_long(0xa2bd9076u), change_endian_long(0x2532d336u), change_endian_long(0xfc6b4499u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(896);
    RX671_func100(change_endian_long(0x85a04ecfu), change_endian_long(0xf502ac47u), change_endian_long(0x27ec770au), change_endian_long(0xaf2f32cau));
    RX671_func314(896+36);
    RX671_func100(change_endian_long(0x779edc74u), change_endian_long(0xb966c4abu), change_endian_long(0xd217459fu), change_endian_long(0x95539552u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc45881f4u);
    RX671_func080();
    RX671_func100(change_endian_long(0x890d46e2u), change_endian_long(0xdac8c7dbu), change_endian_long(0x5c9a01f1u), change_endian_long(0x04246353u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(856);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x376404ccu), change_endian_long(0xc1653c06u), change_endian_long(0x51d7623fu), change_endian_long(0x72dfc65du));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(856);
    RX671_func100(change_endian_long(0xd76e576bu), change_endian_long(0xb523d138u), change_endian_long(0xb6590f2cu), change_endian_long(0x2e9c6bfeu));
    RX671_func314(856+36);
    RX671_func100(change_endian_long(0xaeec01deu), change_endian_long(0x8bc1ddc1u), change_endian_long(0x9a736d74u), change_endian_long(0xa2c1f6d6u));
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
    RX671_func080();
    RX671_func100(change_endian_long(0x9cc15216u), change_endian_long(0xf884f00cu), change_endian_long(0xb0e636fau), change_endian_long(0x45172015u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func320(716);
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
    RX671_func080();
    RX671_func100(change_endian_long(0xceb6050eu), change_endian_long(0xa44e0317u), change_endian_long(0x4290ccedu), change_endian_long(0x7c6f4c11u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(856);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x217674e3u), change_endian_long(0xe6ffc0d8u), change_endian_long(0x0e09b291u), change_endian_long(0x1223f98bu));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func322(816);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x229a2f57u), change_endian_long(0x8008635cu), change_endian_long(0x5aaac9ebu), change_endian_long(0xfe7272d6u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func321(816);
    RX671_func100(change_endian_long(0xf3ced577u), change_endian_long(0x0f5e2d93u), change_endian_long(0xfad3cf1cu), change_endian_long(0x8f67e306u));
    RX671_func314(816+36);
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
    RX671_func100(change_endian_long(0x5cb76380u), change_endian_long(0x8c18d399u), change_endian_long(0x57bd2c29u), change_endian_long(0x4c18ef8cu));
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
        RX671_func101(change_endian_long(0x4c9cdfd1u), change_endian_long(0xdab870c8u), change_endian_long(0x71fc10f6u), change_endian_long(0xc87c1ee1u));
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
        RX671_func101(change_endian_long(0x5322a680u), change_endian_long(0xaeb0f7c4u), change_endian_long(0x1b49c72fu), change_endian_long(0x3546757fu));
    }
    RX671_func100(change_endian_long(0xdb37fdb0u), change_endian_long(0xc8135eacu), change_endian_long(0x46c67783u), change_endian_long(0xb18c40e5u));
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
    RX671_func080();
    RX671_func100(change_endian_long(0xff57cc65u), change_endian_long(0xaf052c59u), change_endian_long(0x86239160u), change_endian_long(0x15873b41u));
    TSIP.REG_00H.WORD = 0x00002393u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func321(856);
    RX671_func100(change_endian_long(0x77c80548u), change_endian_long(0x38af0447u), change_endian_long(0xcb829b3bu), change_endian_long(0x54279db7u));
    RX671_func314(856+36);
    RX671_func100(change_endian_long(0x5859bb23u), change_endian_long(0x1ad1d18du), change_endian_long(0xb7e7629eu), change_endian_long(0xcbe256a6u));
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
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func322(896);
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
    RX671_func100(change_endian_long(0x7bfcd2eeu), change_endian_long(0x3f59962du), change_endian_long(0x5f312eadu), change_endian_long(0x2cdd7b08u));
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
        RX671_func101(change_endian_long(0x4a271fe4u), change_endian_long(0xd49ab3a2u), change_endian_long(0xa6a0e557u), change_endian_long(0x0a022dd6u));
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
        RX671_func101(change_endian_long(0x9b4a6bc0u), change_endian_long(0x13b9718fu), change_endian_long(0x08aee65eu), change_endian_long(0x7b2de34eu));
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
    RX671_func100(change_endian_long(0x3151eed6u), change_endian_long(0x80a5442bu), change_endian_long(0xaca00bc4u), change_endian_long(0xbd94ce14u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5ab44e5fu);
    RX671_func080();
    RX671_func100(change_endian_long(0xf88c13f3u), change_endian_long(0x97fbf09fu), change_endian_long(0x80784925u), change_endian_long(0x294c5116u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func322(816);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x75f651b4u), change_endian_long(0x32aa0bf7u), change_endian_long(0x04ee0e74u), change_endian_long(0x1c02c8d8u));
    TSIP.REG_00H.WORD = 0x00003293u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func322(856);
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
    RX671_func080();
    RX671_func100(change_endian_long(0x08933071u), change_endian_long(0x3b1addd2u), change_endian_long(0x0e722b67u), change_endian_long(0x63475af0u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func320(680);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x899d7651u);
    RX671_func080();
    RX671_func100(change_endian_long(0x2863c699u), change_endian_long(0xec4c565bu), change_endian_long(0xcee77396u), change_endian_long(0x2f04bd77u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000015u;
    RX671_func320(716);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x7775adb6u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func320(752);
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
    RX671_func100(change_endian_long(0x5bfdfc4du), change_endian_long(0x53f5d0b4u), change_endian_long(0xfb2e7cf4u), change_endian_long(0x4c6a6315u));
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
            RX671_func100(change_endian_long(0x2805bc8eu), change_endian_long(0xd8b79125u), change_endian_long(0x7ed37e75u), change_endian_long(0x91ab6a7eu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX671_func101(change_endian_long(0x77bb5c11u), change_endian_long(0x0ce05309u), change_endian_long(0xf0a8ec03u), change_endian_long(0xd4b42133u));
                oLoop1 = 0;
            }
            else
            {
                RX671_func101(change_endian_long(0xf1562102u), change_endian_long(0x2abee42fu), change_endian_long(0xee623633u), change_endian_long(0xf659cce8u));
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
            RX671_func100(change_endian_long(0x0afffc45u), change_endian_long(0x52e363a8u), change_endian_long(0x1428ce56u), change_endian_long(0xcd39a2d2u));
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
                RX671_func101(change_endian_long(0xd4e2e6f4u), change_endian_long(0x35f7e6b3u), change_endian_long(0x3b439c9au), change_endian_long(0xcd1bf8d1u));
                oLoop1 = 0;
            }
            else
            {
                RX671_func101(change_endian_long(0x9bf8a6a3u), change_endian_long(0x16d08f3eu), change_endian_long(0x3d56a1deu), change_endian_long(0x0dc30e7du));
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
 End of function ./input_dir/RX671/RX671_func318.prc
 *********************************************************************************************************************/
