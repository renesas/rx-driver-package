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
* Function Name: RX671_func307
*******************************************************************************************************************/ /**
* @details       RX671 func307
* @note          None
*/
void RX671_func307(void)
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
    RX671_func101(change_endian_long(0x823f5f8du), change_endian_long(0x93992f76u), change_endian_long(0x517fe65cu), change_endian_long(0xbdb9bc04u));
    RX671_func311();
    TSIP.REG_ECH.WORD = 0x00000a73u;
    TSIP.REG_ECH.WORD = 0x00000a31u;
    for(jLoop = 0; jLoop < 32; jLoop = jLoop + 1)
    {
        TSIP.REG_ECH.WORD = 0x00002e20u;
        TSIP.REG_ECH.WORD = 0x38002673u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00A60000u;
        RX671_func100(change_endian_long(0x5ddd0a6eu), change_endian_long(0xcc69fe46u), change_endian_long(0x55ecc0dbu), change_endian_long(0x894cac18u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (0u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func101(change_endian_long(0xf23b476du), change_endian_long(0x613998aeu), change_endian_long(0xba31587cu), change_endian_long(0x12f520beu));
        }
        else
        {
            RX671_func100(change_endian_long(0xa6a08d39u), change_endian_long(0x6a82bb2bu), change_endian_long(0x05954421u), change_endian_long(0x8aa90370u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x05e30fedu);
            RX671_func080();
            RX671_func100(change_endian_long(0x192bb5a6u), change_endian_long(0xf30fde82u), change_endian_long(0x7306f0b3u), change_endian_long(0x9be4d363u));
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            RX671_func312(204);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xe4ce560bu);
            RX671_func080();
            RX671_func100(change_endian_long(0x4487c856u), change_endian_long(0x18c3c510u), change_endian_long(0x6f908b86u), change_endian_long(0x5b44d584u));
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            RX671_func312(136);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
            RX671_func080();
            RX671_func100(change_endian_long(0x0b962d41u), change_endian_long(0x62be00a3u), change_endian_long(0xa7d7c05du), change_endian_long(0x24085445u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX671_func313(476);
            RX671_func100(change_endian_long(0xb94e1420u), change_endian_long(0xb5b941e3u), change_endian_long(0xc7485b0au), change_endian_long(0x4167efd4u));
            RX671_func314(476+64);
            RX671_func100(change_endian_long(0x6bf9546bu), change_endian_long(0x07e6b524u), change_endian_long(0xce5cffcau), change_endian_long(0xcdca25a3u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xea6a1c3du);
            RX671_func080();
            RX671_func100(change_endian_long(0xf3b00f07u), change_endian_long(0x099c4231u), change_endian_long(0xf4ca9487u), change_endian_long(0x02257dd3u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX671_func313(612);
            RX671_func100(change_endian_long(0xdbd9dff8u), change_endian_long(0x0969019bu), change_endian_long(0x618a1d6bu), change_endian_long(0x4642c930u));
            RX671_func314(612+64);
            RX671_func100(change_endian_long(0x28ecb885u), change_endian_long(0xe1d30f6du), change_endian_long(0x3aa98ce4u), change_endian_long(0xe7cf250eu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
            RX671_func080();
            RX671_func100(change_endian_long(0xf4b16c66u), change_endian_long(0x417edd46u), change_endian_long(0xd3336a30u), change_endian_long(0xdd312436u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX671_func313(408);
            RX671_func100(change_endian_long(0xbfed8c2cu), change_endian_long(0x7ef5e14fu), change_endian_long(0xc7df3d3bu), change_endian_long(0x5e330d7bu));
            RX671_func314(408+64);
            RX671_func100(change_endian_long(0x4d25c991u), change_endian_long(0x4d34ddeau), change_endian_long(0xb6f02f8cu), change_endian_long(0xebf57df6u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xae62ddfbu);
            RX671_func080();
            RX671_func100(change_endian_long(0x76d04585u), change_endian_long(0x53df7717u), change_endian_long(0x3530d505u), change_endian_long(0x8dd568ceu));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX671_func313(544);
            RX671_func100(change_endian_long(0x0fbf5f4au), change_endian_long(0xba1e72c7u), change_endian_long(0xb4b58aceu), change_endian_long(0x408db58cu));
            RX671_func314(544+64);
            TSIP.REG_ECH.WORD = 0x0000b400u;
            TSIP.REG_ECH.WORD = 0x00000081u;
            RX671_func101(change_endian_long(0x372a1e6du), change_endian_long(0xd6b3f83au), change_endian_long(0x4588a173u), change_endian_long(0x43981df2u));
            RX671_func309();
            RX671_func100(change_endian_long(0xb732c3b7u), change_endian_long(0x3993ad24u), change_endian_long(0xc4e52c7fu), change_endian_long(0x504e5a87u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x05e30fedu);
            RX671_func080();
            RX671_func100(change_endian_long(0xd11b9bfeu), change_endian_long(0xbf46e3e7u), change_endian_long(0x330f000cu), change_endian_long(0x8f752f1eu));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX671_func313(204);
            RX671_func100(change_endian_long(0x54188fcbu), change_endian_long(0xa5eb1288u), change_endian_long(0xdae9abcau), change_endian_long(0xfc2ad2e9u));
            RX671_func314(204+64);
            RX671_func100(change_endian_long(0x3256f3bdu), change_endian_long(0xa3737caau), change_endian_long(0x974d7375u), change_endian_long(0xdf584464u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xe4ce560bu);
            RX671_func080();
            RX671_func100(change_endian_long(0xe3c1566cu), change_endian_long(0x2c04068cu), change_endian_long(0x84c40d0eu), change_endian_long(0xa84c3a87u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX671_func313(136);
            RX671_func100(change_endian_long(0x2ceacc62u), change_endian_long(0xf3f2e428u), change_endian_long(0x979bb283u), change_endian_long(0xc921bab7u));
            RX671_func314(136+64);
            RX671_func101(change_endian_long(0xb2e5c841u), change_endian_long(0x3d352e0fu), change_endian_long(0xce383301u), change_endian_long(0xf9e2e3a0u));
        }
        TSIP.REG_ECH.WORD = 0x3800da9fu;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00A60000u;
        RX671_func100(change_endian_long(0xc0a749d2u), change_endian_long(0x641c81d6u), change_endian_long(0xdd8ac2d0u), change_endian_long(0xe47e3912u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (0u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func101(change_endian_long(0x8e7a6f2cu), change_endian_long(0x6b08f92du), change_endian_long(0xd805aaf8u), change_endian_long(0x3a0aeffeu));
        }
        else
        {
            RX671_func100(change_endian_long(0x2fa71d1au), change_endian_long(0x931dd5b4u), change_endian_long(0x2a16071du), change_endian_long(0x66ae2d54u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x05e30fedu);
            RX671_func080();
            RX671_func100(change_endian_long(0x940b9178u), change_endian_long(0x8d3aacc8u), change_endian_long(0x20d173dfu), change_endian_long(0x2af2764fu));
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            RX671_func312(204);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xe4ce560bu);
            RX671_func080();
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            RX671_func312(136);
            RX671_func100(change_endian_long(0xa4c2a9a8u), change_endian_long(0x7167521du), change_endian_long(0x4e96449bu), change_endian_long(0x268aa3e0u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
            RX671_func080();
            RX671_func100(change_endian_long(0x8e05076fu), change_endian_long(0xa75a30b6u), change_endian_long(0x6166f7a0u), change_endian_long(0x9f4cf5c1u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX671_func313(476);
            RX671_func100(change_endian_long(0x78d964eeu), change_endian_long(0xfb5610d1u), change_endian_long(0xf4a88f6bu), change_endian_long(0xe6ffb48fu));
            RX671_func314(476+64);
            RX671_func100(change_endian_long(0xe44c6061u), change_endian_long(0xa5d723d7u), change_endian_long(0x54cff39bu), change_endian_long(0x8c8c0db4u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
            RX671_func080();
            RX671_func100(change_endian_long(0x0e3d77b1u), change_endian_long(0x37136086u), change_endian_long(0x06d5983fu), change_endian_long(0x26b231cau));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX671_func313(408);
            RX671_func100(change_endian_long(0x98e419bdu), change_endian_long(0x9a94af63u), change_endian_long(0xcd934a7eu), change_endian_long(0x636b80aau));
            RX671_func314(408+64);
            RX671_func100(change_endian_long(0x307af21au), change_endian_long(0x0d6fc32cu), change_endian_long(0xb4ea3f4cu), change_endian_long(0x1ddb2d19u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x5dd049feu);
            RX671_func080();
            RX671_func100(change_endian_long(0x1228d3cfu), change_endian_long(0x4883f2d0u), change_endian_long(0xdd270b5du), change_endian_long(0xb9e6dc2cu));
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            RX671_func312(68);
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x945d2158u);
            RX671_func080();
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            RX671_func312(0);
            RX671_func100(change_endian_long(0x63168ae1u), change_endian_long(0x31844ccfu), change_endian_long(0x28ae56bau), change_endian_long(0x2cc87ef9u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xea6a1c3du);
            RX671_func080();
            RX671_func100(change_endian_long(0xa3157d1eu), change_endian_long(0x8f3b7ad7u), change_endian_long(0x34f0b035u), change_endian_long(0xd3ed4bdfu));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX671_func313(612);
            RX671_func100(change_endian_long(0x04e4e680u), change_endian_long(0xb2db9f61u), change_endian_long(0x4f001de1u), change_endian_long(0x2aad1abbu));
            RX671_func314(612+64);
            RX671_func100(change_endian_long(0xc652ee93u), change_endian_long(0x961e4835u), change_endian_long(0x4d000962u), change_endian_long(0x43904277u));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xae62ddfbu);
            RX671_func080();
            RX671_func100(change_endian_long(0x3ed2bde5u), change_endian_long(0xec3d637au), change_endian_long(0x3bd52408u), change_endian_long(0xfbaa3d22u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX671_func313(544);
            RX671_func100(change_endian_long(0xeb11301bu), change_endian_long(0xbfe06364u), change_endian_long(0xa5051269u), change_endian_long(0xb4b94ae4u));
            RX671_func314(544+64);
            TSIP.REG_ECH.WORD = 0x0000b400u;
            TSIP.REG_ECH.WORD = 0x00000082u;
            RX671_func101(change_endian_long(0x4bdf18e5u), change_endian_long(0x35917683u), change_endian_long(0xa7bdf6e3u), change_endian_long(0x2dd37d88u));
            RX671_func309();
            RX671_func100(change_endian_long(0xd29579efu), change_endian_long(0x8e3bf830u), change_endian_long(0x614631c4u), change_endian_long(0x6bc944feu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x05e30fedu);
            RX671_func080();
            RX671_func100(change_endian_long(0xf34aa4c0u), change_endian_long(0x3fff717du), change_endian_long(0xb9ef2782u), change_endian_long(0x17cfcff1u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            RX671_func313(204);
            RX671_func100(change_endian_long(0xa63a45dau), change_endian_long(0x04aba399u), change_endian_long(0x1d8967feu), change_endian_long(0xb9a664d1u));
            RX671_func314(204+64);
            RX671_func100(change_endian_long(0x6aa4a131u), change_endian_long(0x8f32baf4u), change_endian_long(0x3da6c9d7u), change_endian_long(0x6135f56cu));
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80010020u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xe4ce560bu);
            RX671_func080();
            RX671_func100(change_endian_long(0xc86c2487u), change_endian_long(0x13beacacu), change_endian_long(0x65e3dda8u), change_endian_long(0x093dc8c5u));
            TSIP.REG_00H.WORD = 0x00012303u;
            TSIP.REG_2CH.WORD = 0x00000022u;
            RX671_func313(136);
            RX671_func100(change_endian_long(0x3d5ae5d8u), change_endian_long(0xe7aa91eeu), change_endian_long(0xc91d6336u), change_endian_long(0x31c0632au));
            RX671_func314(136+64);
            TSIP.REG_ECH.WORD = 0x0000d260u;
            RX671_func101(change_endian_long(0xcd25d086u), change_endian_long(0xb38f67b1u), change_endian_long(0xe7f4589fu), change_endian_long(0xa4cd9befu));
        }
        TSIP.REG_ECH.WORD = 0x01816e94u;
        RX671_func101(change_endian_long(0x5e4c4fa6u), change_endian_long(0x9b11f2a8u), change_endian_long(0x9d267626u), change_endian_long(0xee669a54u));
    }
    TSIP.REG_ECH.WORD = 0x38008a20u;
    TSIP.REG_ECH.WORD = 0x00000020u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    RX671_func100(change_endian_long(0xf77e2515u), change_endian_long(0x31228a94u), change_endian_long(0xcb5f0d27u), change_endian_long(0x39d3d20eu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x05e30fedu);
    RX671_func080();
    RX671_func100(change_endian_long(0x01a951f5u), change_endian_long(0xdf848de6u), change_endian_long(0x4b6628f3u), change_endian_long(0x8883fcf5u));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX671_func312(204);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xe4ce560bu);
    RX671_func080();
    RX671_func100(change_endian_long(0x0057d534u), change_endian_long(0x36f803d3u), change_endian_long(0x599db2eeu), change_endian_long(0x4e83b965u));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func312(136);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
    RX671_func080();
    RX671_func100(change_endian_long(0x725fa2b2u), change_endian_long(0xceb32218u), change_endian_long(0x91d41d9eu), change_endian_long(0x48b6898bu));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func313(476);
    RX671_func100(change_endian_long(0x055d061bu), change_endian_long(0xc76d277fu), change_endian_long(0x67a2d245u), change_endian_long(0xf3d705b8u));
    RX671_func314(476+64);
    RX671_func100(change_endian_long(0x94c2d5fdu), change_endian_long(0xac7fbbbcu), change_endian_long(0x4211a23du), change_endian_long(0x03193d9fu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
    RX671_func080();
    RX671_func100(change_endian_long(0xa8e053c5u), change_endian_long(0xd2e629beu), change_endian_long(0xf7002660u), change_endian_long(0xc3f816f9u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(408);
    RX671_func100(change_endian_long(0xfa2deaeau), change_endian_long(0x668c1c8bu), change_endian_long(0x54573339u), change_endian_long(0xb1b61298u));
    RX671_func314(408+64);
    RX671_func100(change_endian_long(0xdb69577cu), change_endian_long(0x0e53910cu), change_endian_long(0x15fcc856u), change_endian_long(0xaaad1529u));
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
    RX671_func080();
    RX671_func100(change_endian_long(0x9db1c0c8u), change_endian_long(0xf255126du), change_endian_long(0x3651b4d0u), change_endian_long(0x3639d2f8u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func313(612);
    RX671_func100(change_endian_long(0xfbd92803u), change_endian_long(0xef9d2274u), change_endian_long(0xc4a03735u), change_endian_long(0xb120a4b2u));
    RX671_func314(612+64);
    RX671_func100(change_endian_long(0x72023152u), change_endian_long(0xc97e4771u), change_endian_long(0x4f7ea2b6u), change_endian_long(0xf5f2a807u));
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
    RX671_func080();
    RX671_func100(change_endian_long(0x04c8c1f8u), change_endian_long(0x7514dbb8u), change_endian_long(0x6476c5d4u), change_endian_long(0x657b0ee0u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(544);
    RX671_func100(change_endian_long(0x83a3ff3bu), change_endian_long(0xddf0ea87u), change_endian_long(0xc8d95d51u), change_endian_long(0x2bc502b0u));
    RX671_func314(544+64);
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000083u;
    RX671_func101(change_endian_long(0xa1819e91u), change_endian_long(0xada543ddu), change_endian_long(0xb1ed2177u), change_endian_long(0xeb6ab293u));
    RX671_func309();
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000090u;
    TSIP.REG_ECH.WORD = 0x0000381eu;
    TSIP.REG_ECH.WORD = 0x00007c00u;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX671/RX671_func307_r1.prc
 *********************************************************************************************************************/
