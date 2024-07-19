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
* Function Name: RX671_func309
*******************************************************************************************************************/ /**
* @details       RX671 func309
* @note          None
*/
void RX671_func309(void)
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
    RX671_func100(change_endian_long(0x5f7f6ba6u), change_endian_long(0xceb52531u), change_endian_long(0x2422f8c3u), change_endian_long(0x54872c8eu));
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000028u;
    TSIP.REG_ECH.WORD = 0x00003c1eu;
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_ECH.WORD = 0x00000b7bu;
    TSIP.REG_ECH.WORD = 0x00000b9cu;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00003f9eu;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
    RX671_func080();
    RX671_func100(change_endian_long(0x808bd035u), change_endian_long(0xa5c5d3dfu), change_endian_long(0xf6b95de6u), change_endian_long(0x41983217u));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func312(476);
    TSIP.REG_ECH.WORD = 0x0000d77au;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_ECH.WORD = 0x00003f9eu;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xea6a1c3du);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func312(612);
    TSIP.REG_ECH.WORD = 0x0000d77bu;
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000090u;
    RX671_func101(change_endian_long(0xc2662383u), change_endian_long(0xbb62fec5u), change_endian_long(0x4c973de7u), change_endian_long(0xf6e22b17u));
    RX671_func308();
    RX671_func100(change_endian_long(0xdecf4397u), change_endian_long(0xc27196c1u), change_endian_long(0xd274b7ecu), change_endian_long(0x34e3165fu));
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000018u;
    TSIP.REG_ECH.WORD = 0x00003bbeu;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000010u;
    TSIP.REG_ECH.WORD = 0x00003fbeu;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xed6b89f0u);
    RX671_func080();
    RX671_func100(change_endian_long(0xff3ad2b1u), change_endian_long(0x6d2f6643u), change_endian_long(0x2cd0df13u), change_endian_long(0x00cb0bd2u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func313(884);
    RX671_func100(change_endian_long(0x54a2c59bu), change_endian_long(0x793be066u), change_endian_long(0x9c398207u), change_endian_long(0x05d62b39u));
    RX671_func314(884+64);
    RX671_func100(change_endian_long(0x380bf248u), change_endian_long(0xe170d7dfu), change_endian_long(0x4a53ef8du), change_endian_long(0x18954341u));
    TSIP.REG_ECH.WORD = 0x3000db72u;
    TSIP.REG_ECH.WORD = 0x00030020u;
    TSIP.REG_ECH.WORD = 0x0000d770u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_24H.WORD = 0x0000d0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000001cu;
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
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000018u;
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
    TSIP.REG_34H.WORD = 0x00000400u;
    TSIP.REG_24H.WORD = 0x800080c1u;
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
    TSIP.REG_ECH.WORD = 0x0000d366u;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d766u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xda61c233u);
    RX671_func080();
    RX671_func100(change_endian_long(0xfbd83c36u), change_endian_long(0x49f2dd2bu), change_endian_long(0xfc9a3354u), change_endian_long(0xd0ad8795u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    RX671_func313(816);
    RX671_func100(change_endian_long(0xac7ee613u), change_endian_long(0x583f2a09u), change_endian_long(0xd3ef78ecu), change_endian_long(0x8ab7b962u));
    RX671_func314(816+64);
    RX671_func100(change_endian_long(0xa8c528c0u), change_endian_long(0xdebdf722u), change_endian_long(0x6096c268u), change_endian_long(0xb744ed00u));
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000000cu;
    TSIP.REG_E0H.WORD = 0x8081001eu;
    TSIP.REG_00H.WORD = 0x00008307u;
    TSIP.REG_2CH.WORD = 0x000000a0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
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
    TSIP.REG_24H.WORD = 0x000001c0u;
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
    TSIP.REG_ECH.WORD = 0x0000d371u;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d771u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x41273050u);
    RX671_func080();
    RX671_func100(change_endian_long(0x843d697cu), change_endian_long(0x719dff3du), change_endian_long(0xc0e52913u), change_endian_long(0x02ee7985u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(952);
    RX671_func100(change_endian_long(0x52d29537u), change_endian_long(0x808a7dbau), change_endian_long(0xbdf4afd4u), change_endian_long(0xa9e2d504u));
    RX671_func314(952+64);
    RX671_func100(change_endian_long(0xed12f47cu), change_endian_long(0xf2595a76u), change_endian_long(0x9ee1b534u), change_endian_long(0x3352ece7u));
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000014u;
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
    TSIP.REG_100H.WORD = change_endian_long(0xae62ddfbu);
    RX671_func080();
    RX671_func100(change_endian_long(0x91e0252bu), change_endian_long(0x5a95a4abu), change_endian_long(0x7ecf8351u), change_endian_long(0x09bc2236u));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func312(544);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xea6a1c3du);
    RX671_func080();
    RX671_func100(change_endian_long(0x29d686c8u), change_endian_long(0x502155b2u), change_endian_long(0xeedddea2u), change_endian_long(0x8fc8598cu));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func312(612);
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
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000004u;
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
    TSIP.REG_ECH.WORD = 0x0000d77bu;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x000000a0u;
    TSIP.REG_E0H.WORD = 0x80c0001eu;
    TSIP.REG_00H.WORD = 0x00018303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
    RX671_func080();
    RX671_func100(change_endian_long(0x2b305a06u), change_endian_long(0x08c17113u), change_endian_long(0xaf09ff34u), change_endian_long(0xa3a5c558u));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func312(408);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func312(476);
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
    TSIP.REG_34H.WORD = 0x00000008u;
    TSIP.REG_24H.WORD = 0x800011c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x0000000cu;
    TSIP.REG_24H.WORD = 0x82001181u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000800u;
    TSIP.REG_24H.WORD = 0x800080c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_E0H.WORD = 0x8081001eu;
    TSIP.REG_00H.WORD = 0x00008307u;
    TSIP.REG_2CH.WORD = 0x00000028u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_ECH.WORD = 0x0000d77au;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x000000a0u;
    TSIP.REG_E0H.WORD = 0x81c0001eu;
    TSIP.REG_00H.WORD = 0x00013803u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000091u;
    RX671_func101(change_endian_long(0x93724889u), change_endian_long(0x840930f3u), change_endian_long(0xab8cce55u), change_endian_long(0xbf866ffbu));
    RX671_func308();
    RX671_func100(change_endian_long(0x60475bd5u), change_endian_long(0x6dc7a355u), change_endian_long(0xcc885966u), change_endian_long(0x28ca69c0u));
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x000000a0u;
    TSIP.REG_E0H.WORD = 0x80c0001eu;
    TSIP.REG_00H.WORD = 0x00018303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_24H.WORD = 0x0000d0d0u;
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
    TSIP.REG_100H.WORD = change_endian_long(0xda61c233u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func312(816);
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000000cu;
    TSIP.REG_ECH.WORD = 0x3800db66u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(change_endian_long(0xced93855u), change_endian_long(0x6d364f1eu), change_endian_long(0x7a5d483cu), change_endian_long(0xfffa7effu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func315(0x00000110u);
        RX671_func101(change_endian_long(0x8e133871u), change_endian_long(0x9dc5a520u), change_endian_long(0x604b0835u), change_endian_long(0x2219e5eau));
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
        TSIP.REG_28H.WORD = 0x00bf0001u;
        RX671_func101(change_endian_long(0x175c4d25u), change_endian_long(0x14c1088fu), change_endian_long(0x5dafe52eu), change_endian_long(0x5cd82a32u));
    }
    RX671_func100(change_endian_long(0xed73c228u), change_endian_long(0x3e541afcu), change_endian_long(0x2b033bc1u), change_endian_long(0x7fc273d3u));
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000018u;
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
    TSIP.REG_ECH.WORD = 0x0000d766u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xda61c233u);
    RX671_func080();
    RX671_func100(change_endian_long(0xd85dc915u), change_endian_long(0xbc52a611u), change_endian_long(0xed7d67bcu), change_endian_long(0x9544c05du));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(816);
    RX671_func100(change_endian_long(0x93d78d1eu), change_endian_long(0xca9d20b9u), change_endian_long(0x0598ec57u), change_endian_long(0xc438aa52u));
    RX671_func314(816+64);
    RX671_func100(change_endian_long(0x2f8b69feu), change_endian_long(0xbe5f8b82u), change_endian_long(0x6691f822u), change_endian_long(0x8bfdea2fu));
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000000cu;
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
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x41273050u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func312(952);
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000014u;
    TSIP.REG_ECH.WORD = 0x3800db71u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(change_endian_long(0x0c8626b8u), change_endian_long(0xc38a769bu), change_endian_long(0x4d877c67u), change_endian_long(0x784b3c4du));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func315(0x00000110u);
        RX671_func101(change_endian_long(0x237ebbb5u), change_endian_long(0xa9ad626cu), change_endian_long(0x41f8c9cbu), change_endian_long(0x30cee7beu));
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
        TSIP.REG_28H.WORD = 0x00bf0001u;
        RX671_func101(change_endian_long(0xd22e5da6u), change_endian_long(0xf4796752u), change_endian_long(0xfadc56adu), change_endian_long(0xddfdf329u));
    }
    RX671_func100(change_endian_long(0x0affa82cu), change_endian_long(0x37f33a66u), change_endian_long(0xcd88f5e7u), change_endian_long(0xfeabdee1u));
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000001cu;
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
    TSIP.REG_ECH.WORD = 0x0000d371u;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d771u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x41273050u);
    RX671_func080();
    RX671_func100(change_endian_long(0x1f3bef4eu), change_endian_long(0xde31817bu), change_endian_long(0xe7b259d3u), change_endian_long(0x0bca401du));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(952);
    RX671_func100(change_endian_long(0x06753d24u), change_endian_long(0xffb7428cu), change_endian_long(0x514e6df5u), change_endian_long(0x2d50d78du));
    RX671_func314(952+64);
    RX671_func100(change_endian_long(0x169550f3u), change_endian_long(0x0f901063u), change_endian_long(0x497df184u), change_endian_long(0xd63e4186u));
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000014u;
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
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_ECH.WORD = 0x00000b9cu;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00003f9eu;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
    RX671_func080();
    RX671_func100(change_endian_long(0x33a9f423u), change_endian_long(0xc3c19359u), change_endian_long(0x79744e24u), change_endian_long(0x0fd4ad1cu));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func312(408);
    TSIP.REG_ECH.WORD = 0x0000d77au;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_ECH.WORD = 0x00003f9eu;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xae62ddfbu);
    RX671_func080();
    RX671_func100(change_endian_long(0x3854886bu), change_endian_long(0x2611cfbfu), change_endian_long(0x31d213f8u), change_endian_long(0xece0463fu));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func312(544);
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
    TSIP.REG_ECH.WORD = 0x0000d77bu;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x1c5a2b81u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func312(680);
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000092u;
    RX671_func101(change_endian_long(0x2a4ac399u), change_endian_long(0x993d2638u), change_endian_long(0xd4d7bd5au), change_endian_long(0xd1349feeu));
    RX671_func304();
    RX671_func100(change_endian_long(0x66a1b55fu), change_endian_long(0x6f3ef8dfu), change_endian_long(0xad1899d7u), change_endian_long(0x12b7fc47u));
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x000000a0u;
    TSIP.REG_E0H.WORD = 0x80c0001eu;
    TSIP.REG_00H.WORD = 0x00018303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_24H.WORD = 0x0000d0d0u;
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
    TSIP.REG_100H.WORD = change_endian_long(0xda61c233u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func312(816);
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000000cu;
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
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000018u;
    TSIP.REG_ECH.WORD = 0x3800db72u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(change_endian_long(0xec7639fdu), change_endian_long(0x900765e6u), change_endian_long(0x8202de20u), change_endian_long(0xb6d981bfu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func315(0x00000190u);
        RX671_func101(change_endian_long(0xa2d7f159u), change_endian_long(0x4fa03d63u), change_endian_long(0xc822f53au), change_endian_long(0xb8996a44u));
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
        RX671_func101(change_endian_long(0x1812b2c7u), change_endian_long(0x42a4679cu), change_endian_long(0x5ec281c9u), change_endian_long(0x30ffcd38u));
    }
    RX671_func100(change_endian_long(0xb247848fu), change_endian_long(0xf30c7fc0u), change_endian_long(0xa29c2aedu), change_endian_long(0xfb4bebc1u));
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
    TSIP.REG_ECH.WORD = 0x00000bdeu;
    TSIP.REG_ECH.WORD = 0x02816fdeu;
    TSIP.REG_ECH.WORD = 0x30008bc0u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00020020u;
    TSIP.REG_ECH.WORD = 0x0000d366u;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d766u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xda61c233u);
    RX671_func080();
    RX671_func100(change_endian_long(0xd79bd2b0u), change_endian_long(0x0ef11067u), change_endian_long(0xc2dcf257u), change_endian_long(0x8d4138e8u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(816);
    RX671_func100(change_endian_long(0x3452580bu), change_endian_long(0x35c2031eu), change_endian_long(0x2b12c850u), change_endian_long(0x035fbdc4u));
    RX671_func314(816+64);
    RX671_func100(change_endian_long(0x6689e0c4u), change_endian_long(0xc46680edu), change_endian_long(0x36eecb2fu), change_endian_long(0x7d799640u));
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000000cu;
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
    TSIP.REG_100H.WORD = change_endian_long(0x41273050u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func312(952);
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000014u;
    TSIP.REG_ECH.WORD = 0x3800db71u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(change_endian_long(0xbb17273fu), change_endian_long(0x0c15ffe2u), change_endian_long(0x47bf3b9fu), change_endian_long(0xbeaeb1a7u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func315(0x00000110u);
        RX671_func101(change_endian_long(0x2bbf7003u), change_endian_long(0xdcb639f2u), change_endian_long(0x8246570au), change_endian_long(0x2173fe60u));
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
        TSIP.REG_28H.WORD = 0x00bf0001u;
        RX671_func101(change_endian_long(0x5d930fa8u), change_endian_long(0x6cbde96au), change_endian_long(0x466b4c79u), change_endian_long(0x17af1fc8u));
    }
    RX671_func100(change_endian_long(0x9a1f1d83u), change_endian_long(0xe6971bd5u), change_endian_long(0x1e187242u), change_endian_long(0x941d1d84u));
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
    TSIP.REG_ECH.WORD = 0x0000d371u;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d771u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x41273050u);
    RX671_func080();
    RX671_func100(change_endian_long(0x6c6e5b93u), change_endian_long(0x49566a6du), change_endian_long(0xefb93174u), change_endian_long(0xbfdff01cu));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(952);
    RX671_func100(change_endian_long(0x21be55beu), change_endian_long(0xf6825636u), change_endian_long(0xa9f15076u), change_endian_long(0x6698647bu));
    RX671_func314(952+64);
    RX671_func100(change_endian_long(0x0d784ab5u), change_endian_long(0x721ecd7fu), change_endian_long(0x3b23f472u), change_endian_long(0x6ae10e91u));
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000014u;
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
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000001cu;
    TSIP.REG_ECH.WORD = 0x00003b9eu;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00003f9eu;
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
    TSIP.REG_ECH.WORD = 0x3000db68u;
    TSIP.REG_ECH.WORD = 0x00030020u;
    TSIP.REG_ECH.WORD = 0x0000d77au;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d37au;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x00000b9cu;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_ECH.WORD = 0x00003f9eu;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5fe685f6u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func312(748);
    TSIP.REG_ECH.WORD = 0x0000d77bu;
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000093u;
    RX671_func101(change_endian_long(0x576205fbu), change_endian_long(0xe3909630u), change_endian_long(0x74a3c4d9u), change_endian_long(0x101368cbu));
    RX671_func308();
    RX671_func100(change_endian_long(0x7656a340u), change_endian_long(0x3c685c24u), change_endian_long(0xfd515214u), change_endian_long(0x04038195u));
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x000000a0u;
    TSIP.REG_E0H.WORD = 0x80c0001eu;
    TSIP.REG_00H.WORD = 0x00018303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_24H.WORD = 0x0000d0d0u;
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
    TSIP.REG_100H.WORD = change_endian_long(0xed6b89f0u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func312(884);
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000010u;
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
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000018u;
    TSIP.REG_ECH.WORD = 0x3800db72u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(change_endian_long(0xcb0395c4u), change_endian_long(0x891d96e3u), change_endian_long(0x09589e7cu), change_endian_long(0xb3dc820eu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func315(0x00000190u);
        RX671_func101(change_endian_long(0xdd0da7f1u), change_endian_long(0xe9547374u), change_endian_long(0x89fd8f21u), change_endian_long(0x3694b755u));
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
        RX671_func101(change_endian_long(0xc1bb3947u), change_endian_long(0x993d059au), change_endian_long(0x94debba6u), change_endian_long(0xf5e24954u));
    }
    RX671_func100(change_endian_long(0x8fce57f7u), change_endian_long(0xc23850ebu), change_endian_long(0x8345d2e1u), change_endian_long(0x960dbbf6u));
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
    TSIP.REG_ECH.WORD = 0x0000d370u;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d770u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xed6b89f0u);
    RX671_func080();
    RX671_func100(change_endian_long(0x6e91cd4fu), change_endian_long(0x0d880ceeu), change_endian_long(0x0de29b5cu), change_endian_long(0x70da06e4u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(884);
    RX671_func100(change_endian_long(0xbcc0aa9fu), change_endian_long(0x0559c00cu), change_endian_long(0x46294b11u), change_endian_long(0xc97a8c77u));
    RX671_func314(884+64);
    RX671_func100(change_endian_long(0x5a3c76cfu), change_endian_long(0xa5562507u), change_endian_long(0xe2149c39u), change_endian_long(0xd0152a9fu));
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000010u;
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
    TSIP.REG_100H.WORD = change_endian_long(0xda61c233u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func312(816);
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000000cu;
    TSIP.REG_ECH.WORD = 0x3800db66u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(change_endian_long(0xace2e92fu), change_endian_long(0x8a81e57bu), change_endian_long(0xc5d992f9u), change_endian_long(0x09a095e2u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func315(0x00000110u);
        RX671_func101(change_endian_long(0xc14a63bdu), change_endian_long(0xc904e9f3u), change_endian_long(0x847e212du), change_endian_long(0x67867e58u));
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
        TSIP.REG_28H.WORD = 0x00bf0001u;
        RX671_func101(change_endian_long(0x4121153au), change_endian_long(0x5422ac41u), change_endian_long(0xc5f688d6u), change_endian_long(0x518b26c8u));
    }
    RX671_func100(change_endian_long(0x59367262u), change_endian_long(0xf91a44d1u), change_endian_long(0xf568630au), change_endian_long(0x75d7f515u));
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
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000001cu;
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
    TSIP.REG_ECH.WORD = 0x0000d366u;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d766u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xda61c233u);
    RX671_func080();
    RX671_func100(change_endian_long(0x1748ab3bu), change_endian_long(0x05ffaabdu), change_endian_long(0x1fe5ff42u), change_endian_long(0x0d6f3a32u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(816);
    RX671_func100(change_endian_long(0xb9f717acu), change_endian_long(0xb719bd8bu), change_endian_long(0xd3d080a3u), change_endian_long(0xb8e95beau));
    RX671_func314(816+64);
    RX671_func100(change_endian_long(0x27adc1beu), change_endian_long(0xbb6a0cb9u), change_endian_long(0x6fca41ebu), change_endian_long(0x4ebcd133u));
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000000cu;
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
    TSIP.REG_100H.WORD = change_endian_long(0x41273050u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func312(952);
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000014u;
    TSIP.REG_ECH.WORD = 0x3800db71u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(change_endian_long(0xcbdeeb92u), change_endian_long(0x8e63a617u), change_endian_long(0xb424889eu), change_endian_long(0x4752b56au));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func315(0x00000110u);
        RX671_func101(change_endian_long(0x782ab4afu), change_endian_long(0xafa869e0u), change_endian_long(0x25d21fe0u), change_endian_long(0xd6918f66u));
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
        TSIP.REG_28H.WORD = 0x00bf0001u;
        RX671_func101(change_endian_long(0x84035f7bu), change_endian_long(0xc197834eu), change_endian_long(0xa701e117u), change_endian_long(0xcfe82931u));
    }
    RX671_func100(change_endian_long(0x9c32558du), change_endian_long(0xf0ab99c4u), change_endian_long(0x01361e1fu), change_endian_long(0xa573a1e5u));
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
    TSIP.REG_ECH.WORD = 0x00000bdeu;
    TSIP.REG_ECH.WORD = 0x02816fdeu;
    TSIP.REG_ECH.WORD = 0x30008bc0u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00020020u;
    TSIP.REG_ECH.WORD = 0x0000d371u;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d771u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000014u;
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
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000014u;
    TSIP.REG_ECH.WORD = 0x00003b9eu;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00003f9eu;
    TSIP.REG_ECH.WORD = 0x3000db71u;
    TSIP.REG_ECH.WORD = 0x00030020u;
    TSIP.REG_ECH.WORD = 0x0000d77au;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d37au;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x00000b9cu;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_ECH.WORD = 0x00003f9eu;
    TSIP.REG_24H.WORD = 0x000001c0u;
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
    TSIP.REG_ECH.WORD = 0x0000d77bu;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x1c5a2b81u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func312(680);
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000094u;
    RX671_func101(change_endian_long(0xefeaedc4u), change_endian_long(0xf1bee0c5u), change_endian_long(0x52e5356bu), change_endian_long(0x3ae7863bu));
    RX671_func304();
    RX671_func100(change_endian_long(0x13b02b2eu), change_endian_long(0xe8065cccu), change_endian_long(0xd79b6c9eu), change_endian_long(0xf81e0e04u));
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x000000a0u;
    TSIP.REG_E0H.WORD = 0x80c0001eu;
    TSIP.REG_00H.WORD = 0x00018303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_24H.WORD = 0x0000d0d0u;
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
    TSIP.REG_100H.WORD = change_endian_long(0xda61c233u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func312(816);
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000000cu;
    TSIP.REG_ECH.WORD = 0x3800db66u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(change_endian_long(0x015c19bfu), change_endian_long(0x9f10c389u), change_endian_long(0x200f9c2fu), change_endian_long(0xc234ae33u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func315(0x00000110u);
        RX671_func101(change_endian_long(0x79a3cec0u), change_endian_long(0x190b80f5u), change_endian_long(0xe839651au), change_endian_long(0x777fd723u));
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
        TSIP.REG_28H.WORD = 0x00bf0001u;
        RX671_func101(change_endian_long(0x31018aecu), change_endian_long(0xcbfdcabcu), change_endian_long(0x603164b9u), change_endian_long(0xef67a5d4u));
    }
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000018u;
    TSIP.REG_ECH.WORD = 0x3800db72u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(change_endian_long(0x716bcc6au), change_endian_long(0x398f9840u), change_endian_long(0x900b0634u), change_endian_long(0x8046f446u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func315(0x00000190u);
        RX671_func101(change_endian_long(0xedc8a552u), change_endian_long(0x9b643b43u), change_endian_long(0x82b3da8au), change_endian_long(0x4309b931u));
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
        RX671_func101(change_endian_long(0x99cd9ea8u), change_endian_long(0x0717dbccu), change_endian_long(0xd3fb1424u), change_endian_long(0xc56cdea9u));
    }
    RX671_func100(change_endian_long(0x1ee38e83u), change_endian_long(0xdd13a960u), change_endian_long(0x46d61a14u), change_endian_long(0x1e026a1cu));
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
    TSIP.REG_ECH.WORD = 0x0000d366u;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d766u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xda61c233u);
    RX671_func080();
    RX671_func100(change_endian_long(0x6df08c89u), change_endian_long(0x1c357321u), change_endian_long(0xceeb1464u), change_endian_long(0xf27517d0u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(816);
    RX671_func100(change_endian_long(0x5520e2b1u), change_endian_long(0xd230c309u), change_endian_long(0x3e1fca6cu), change_endian_long(0x1f7fdfc8u));
    RX671_func314(816+64);
    RX671_func100(change_endian_long(0x1918ec3au), change_endian_long(0xd20c0fbbu), change_endian_long(0xb4faf231u), change_endian_long(0xb941ceccu));
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000000cu;
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
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000001cu;
    TSIP.REG_ECH.WORD = 0x00003b9eu;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00003f9eu;
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
    TSIP.REG_ECH.WORD = 0x3000db68u;
    TSIP.REG_ECH.WORD = 0x00030020u;
    TSIP.REG_ECH.WORD = 0x0000d77au;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d37au;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x00000b9cu;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_ECH.WORD = 0x00003f9eu;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5fe685f6u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func312(748);
    TSIP.REG_ECH.WORD = 0x0000d77bu;
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000095u;
    RX671_func101(change_endian_long(0x29bf07a6u), change_endian_long(0x09e74c39u), change_endian_long(0xbea55eb4u), change_endian_long(0xc02adf6bu));
    RX671_func308();
    RX671_func100(change_endian_long(0xb72b0389u), change_endian_long(0x845ba5e7u), change_endian_long(0x498fd2c5u), change_endian_long(0x3aaaec17u));
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
    TSIP.REG_100H.WORD = change_endian_long(0xda61c233u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func312(816);
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000000cu;
    TSIP.REG_ECH.WORD = 0x3800db66u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(change_endian_long(0xa454cee6u), change_endian_long(0xad8a8b05u), change_endian_long(0xaab7c868u), change_endian_long(0xb9877b02u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func315(0x00000110u);
        RX671_func101(change_endian_long(0x6f6044c4u), change_endian_long(0x58c1f4e9u), change_endian_long(0xf7aa7305u), change_endian_long(0x1c23e66cu));
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
        TSIP.REG_28H.WORD = 0x00bf0001u;
        RX671_func101(change_endian_long(0xaa732a3cu), change_endian_long(0x42217ac0u), change_endian_long(0xfbc4a983u), change_endian_long(0xa05ae192u));
    }
    RX671_func100(change_endian_long(0xafb8f11cu), change_endian_long(0x7864f7d6u), change_endian_long(0xabc0db51u), change_endian_long(0x879bf6b8u));
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000001cu;
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
    TSIP.REG_ECH.WORD = 0x00000bdeu;
    TSIP.REG_ECH.WORD = 0x02816fdeu;
    TSIP.REG_ECH.WORD = 0x30008bc0u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00020020u;
    TSIP.REG_ECH.WORD = 0x0000d366u;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d766u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000000cu;
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
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x000000a0u;
    TSIP.REG_E0H.WORD = 0x80c0001eu;
    TSIP.REG_00H.WORD = 0x00018303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xed6b89f0u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func312(884);
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000010u;
    TSIP.REG_ECH.WORD = 0x3800db70u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(change_endian_long(0x2ef3f50cu), change_endian_long(0xb2304402u), change_endian_long(0x8ca5cf9fu), change_endian_long(0xe55840b3u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func315(0x00000110u);
        RX671_func101(change_endian_long(0x661ab063u), change_endian_long(0x3e7a7560u), change_endian_long(0x7f8277b8u), change_endian_long(0x657b8891u));
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
        TSIP.REG_28H.WORD = 0x00bf0001u;
        RX671_func101(change_endian_long(0xb025b1fdu), change_endian_long(0xa2a1bee6u), change_endian_long(0xaf420e97u), change_endian_long(0xca59e7cfu));
    }
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000018u;
    TSIP.REG_ECH.WORD = 0x3800db72u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(change_endian_long(0x6be60aa7u), change_endian_long(0x0754e70cu), change_endian_long(0x118fbdd2u), change_endian_long(0xe8eafe95u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func315(0x00000190u);
        RX671_func101(change_endian_long(0x915021aau), change_endian_long(0x7c5fe6d4u), change_endian_long(0xa8c97596u), change_endian_long(0x1df19255u));
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
        RX671_func101(change_endian_long(0xa8812128u), change_endian_long(0x863fe2c6u), change_endian_long(0x466a0692u), change_endian_long(0x3c2eda65u));
    }
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
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000000cu;
    TSIP.REG_ECH.WORD = 0x3800db66u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(change_endian_long(0xb2ad1559u), change_endian_long(0xc637d14fu), change_endian_long(0x2d9bd3e7u), change_endian_long(0x7a9a1eb2u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_24H.WORD = 0x000001c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x00870001u;
        TSIP.REG_34H.WORD = 0x00000800u;
        TSIP.REG_24H.WORD = 0x800080c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_24H.WORD = 0x04000591u;
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
        TSIP.REG_28H.WORD = 0x00870001u;
        TSIP.REG_34H.WORD = 0x00000008u;
        TSIP.REG_24H.WORD = 0x800011c0u;
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
        TSIP.REG_ECH.WORD = 0x0000b7c0u;
        TSIP.REG_ECH.WORD = 0x00000030u;
        TSIP.REG_E0H.WORD = 0x8088001eu;
        TSIP.REG_00H.WORD = 0x00008323u;
        TSIP.REG_2CH.WORD = 0x00000024u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_ECH.WORD = 0x0000b7c0u;
        TSIP.REG_ECH.WORD = 0x0000000cu;
        TSIP.REG_ECH.WORD = 0x00003b9eu;
        TSIP.REG_ECH.WORD = 0x0000b7c0u;
        TSIP.REG_ECH.WORD = 0x0000004cu;
        TSIP.REG_ECH.WORD = 0x00003f9eu;
        TSIP.REG_ECH.WORD = 0x0000b7c0u;
        TSIP.REG_ECH.WORD = 0x00000030u;
        TSIP.REG_E0H.WORD = 0x8188001eu;
        TSIP.REG_00H.WORD = 0x00003823u;
        TSIP.REG_2CH.WORD = 0x00000014u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x00bf0001u;
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
        RX671_func101(change_endian_long(0xab9263c2u), change_endian_long(0xf187d293u), change_endian_long(0x79f35af8u), change_endian_long(0xa0f31965u));
    }
    else
    {
        TSIP.REG_24H.WORD = 0x000001c0u;
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
        TSIP.REG_E0H.WORD = 0x8181001eu;
        TSIP.REG_00H.WORD = 0x00003807u;
        TSIP.REG_2CH.WORD = 0x00000014u;
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
        RX671_func101(change_endian_long(0x5013daa8u), change_endian_long(0xfcb32196u), change_endian_long(0x620a7fecu), change_endian_long(0xfd4e66c3u));
    }
    RX671_func100(change_endian_long(0xd90cc5cdu), change_endian_long(0xa394555eu), change_endian_long(0x474c3421u), change_endian_long(0x1053523bu));
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
    TSIP.REG_ECH.WORD = 0x0000d37eu;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d77eu;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000020u;
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
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x000000a0u;
    TSIP.REG_E0H.WORD = 0x81c0001eu;
    TSIP.REG_00H.WORD = 0x00013803u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x1c5a2b81u);
    RX671_func080();
    RX671_func100(change_endian_long(0xee57eeffu), change_endian_long(0xed6ad02au), change_endian_long(0x18fe01abu), change_endian_long(0x30632220u));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func312(680);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5fe685f6u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func312(748);
    TSIP.REG_ECH.WORD = 0x3800db7eu;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(change_endian_long(0xc60124b3u), change_endian_long(0x47de0634u), change_endian_long(0x553df8b2u), change_endian_long(0x6488e817u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_ECH.WORD = 0x0000b7c0u;
        TSIP.REG_ECH.WORD = 0x000000a0u;
        TSIP.REG_E0H.WORD = 0x80c0001eu;
        TSIP.REG_00H.WORD = 0x00018303u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_24H.WORD = 0x000049c1u;
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
        TSIP.REG_24H.WORD = 0x00000591u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00210000u;
        TSIP.REG_24H.WORD = 0x000049c1u;
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
        oLoop1 = 1;
        while( 1 == oLoop1 )
    {
            RX671_func100(change_endian_long(0x695fe2cau), change_endian_long(0xbf04c7d4u), change_endian_long(0xbc9765a8u), change_endian_long(0xde600151u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
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
                TSIP.REG_24H.WORD = 0x06001181u;
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
                TSIP.REG_24H.WORD = 0x000080c1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_28H.WORD = 0x00870001u;
                TSIP.REG_ECH.WORD = 0x0000b7c0u;
                TSIP.REG_ECH.WORD = 0x00000050u;
                TSIP.REG_E0H.WORD = 0x8088001eu;
                TSIP.REG_00H.WORD = 0x00008323u;
                TSIP.REG_2CH.WORD = 0x00000020u;
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
                TSIP.REG_2CH.WORD = 0x000000a0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_28H.WORD = 0x00bf0001u;
                TSIP.REG_28H.WORD = 0x00800001u;
                TSIP.REG_ECH.WORD = 0x0000b7c0u;
                TSIP.REG_ECH.WORD = 0x00000020u;
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
                TSIP.REG_1CH.WORD = 0x00210000u;
                TSIP.REG_28H.WORD = 0x00870001u;
                TSIP.REG_ECH.WORD = 0x0000b7c0u;
                TSIP.REG_ECH.WORD = 0x00000050u;
                TSIP.REG_E0H.WORD = 0x8188001eu;
                TSIP.REG_00H.WORD = 0x00003823u;
                TSIP.REG_2CH.WORD = 0x00000010u;
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
                TSIP.REG_2CH.WORD = 0x00000090u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_28H.WORD = 0x00bf0001u;
                TSIP.REG_24H.WORD = 0x000011c0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_ECH.WORD = 0x0000b7c0u;
                TSIP.REG_ECH.WORD = 0x000000a0u;
                TSIP.REG_E0H.WORD = 0x81c0001eu;
                TSIP.REG_00H.WORD = 0x00013803u;
                TSIP.REG_2CH.WORD = 0x00000010u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                RX671_func101(change_endian_long(0xf678a9fcu), change_endian_long(0xc78b03ccu), change_endian_long(0x733633a0u), change_endian_long(0xec5c21f0u));
            }
            else
            {
                RX671_func101(change_endian_long(0x3cbcc718u), change_endian_long(0xe3028fb5u), change_endian_long(0xd63b30b2u), change_endian_long(0x1e17b897u));
                oLoop1 = 0;
            }
        }
        RX671_func101(change_endian_long(0xd27efcbcu), change_endian_long(0x1532270du), change_endian_long(0xd749b637u), change_endian_long(0xfc69c914u));
    }
    else
    {
        oLoop1 = 1;
        while( 1 == oLoop1 )
    {
            TSIP.REG_24H.WORD = 0x000049c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_ECH.WORD = 0x0000b7c0u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_E0H.WORD = 0x80c0001eu;
            TSIP.REG_00H.WORD = 0x00018303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
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
            TSIP.REG_24H.WORD = 0x000049c1u;
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
            TSIP.REG_24H.WORD = 0x000049c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_ECH.WORD = 0x0000b7c0u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_E0H.WORD = 0x80c0001eu;
            TSIP.REG_00H.WORD = 0x00018303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_24H.WORD = 0x000049c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x06001191u;
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
            TSIP.REG_24H.WORD = 0x000080c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_28H.WORD = 0x00870001u;
            TSIP.REG_ECH.WORD = 0x0000b7c0u;
            TSIP.REG_ECH.WORD = 0x00000050u;
            TSIP.REG_E0H.WORD = 0x8088001eu;
            TSIP.REG_00H.WORD = 0x00008323u;
            TSIP.REG_2CH.WORD = 0x00000020u;
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
            TSIP.REG_2CH.WORD = 0x000000a0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_28H.WORD = 0x00bf0001u;
            TSIP.REG_28H.WORD = 0x00800001u;
            TSIP.REG_ECH.WORD = 0x0000b7c0u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            TSIP.REG_E0H.WORD = 0x8181001eu;
            TSIP.REG_00H.WORD = 0x00003807u;
            TSIP.REG_2CH.WORD = 0x00000090u;
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
            TSIP.REG_24H.WORD = 0x000080c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
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
            TSIP.REG_28H.WORD = 0x00bf0001u;
            TSIP.REG_1CH.WORD = 0x00210000u;
            TSIP.REG_28H.WORD = 0x00870001u;
            TSIP.REG_ECH.WORD = 0x0000b7c0u;
            TSIP.REG_ECH.WORD = 0x00000050u;
            TSIP.REG_E0H.WORD = 0x8188001eu;
            TSIP.REG_00H.WORD = 0x00003823u;
            TSIP.REG_2CH.WORD = 0x00000010u;
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
            TSIP.REG_2CH.WORD = 0x00000090u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_28H.WORD = 0x00bf0001u;
            TSIP.REG_24H.WORD = 0x000011c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_ECH.WORD = 0x0000b7c0u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_E0H.WORD = 0x81c0001eu;
            TSIP.REG_00H.WORD = 0x00013803u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX671_func100(change_endian_long(0xe27fb693u), change_endian_long(0x5ad8066cu), change_endian_long(0x928f74fau), change_endian_long(0x71f85df9u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX671_func101(change_endian_long(0x4d5a87b4u), change_endian_long(0x60e02c03u), change_endian_long(0x2c725d2fu), change_endian_long(0xbf576ba6u));
                oLoop1 = 0;
            }
            else
            {
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
                TSIP.REG_24H.WORD = 0x06001191u;
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
                TSIP.REG_24H.WORD = 0x000080c1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_28H.WORD = 0x00870001u;
                TSIP.REG_ECH.WORD = 0x0000b7c0u;
                TSIP.REG_ECH.WORD = 0x00000050u;
                TSIP.REG_E0H.WORD = 0x8088001eu;
                TSIP.REG_00H.WORD = 0x00008323u;
                TSIP.REG_2CH.WORD = 0x00000020u;
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
                TSIP.REG_2CH.WORD = 0x000000a0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_28H.WORD = 0x00bf0001u;
                TSIP.REG_28H.WORD = 0x00800001u;
                TSIP.REG_ECH.WORD = 0x0000b7c0u;
                TSIP.REG_ECH.WORD = 0x00000020u;
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
                TSIP.REG_28H.WORD = 0x00870001u;
                TSIP.REG_ECH.WORD = 0x0000b7c0u;
                TSIP.REG_ECH.WORD = 0x00000050u;
                TSIP.REG_E0H.WORD = 0x8188001eu;
                TSIP.REG_00H.WORD = 0x00003823u;
                TSIP.REG_2CH.WORD = 0x00000010u;
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
                TSIP.REG_2CH.WORD = 0x00000090u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_28H.WORD = 0x00bf0001u;
                TSIP.REG_24H.WORD = 0x000011c0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_ECH.WORD = 0x0000b7c0u;
                TSIP.REG_ECH.WORD = 0x000000a0u;
                TSIP.REG_E0H.WORD = 0x81c0001eu;
                TSIP.REG_00H.WORD = 0x00013803u;
                TSIP.REG_2CH.WORD = 0x00000010u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                RX671_func101(change_endian_long(0x2bce9ec8u), change_endian_long(0xac7e97b9u), change_endian_long(0x340ce931u), change_endian_long(0xd76b6214u));
            }
        }
        RX671_func101(change_endian_long(0x810577feu), change_endian_long(0xb2d28497u), change_endian_long(0x10561680u), change_endian_long(0x94dd13c0u));
    }
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
    TSIP.REG_ECH.WORD = 0x00000028u;
    TSIP.REG_ECH.WORD = 0x0000381eu;
    TSIP.REG_ECH.WORD = 0x00007c00u;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX671/RX671_func309_r1.prc
 *********************************************************************************************************************/
