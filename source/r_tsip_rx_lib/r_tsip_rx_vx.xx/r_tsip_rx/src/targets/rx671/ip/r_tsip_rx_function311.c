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
* Function Name: RX671_func311
*******************************************************************************************************************/ /**
* @details       RX671 func311
* @note          None
*/
void RX671_func311(void)
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
    RX671_func100(change_endian_long(0x541109d5u), change_endian_long(0xbea66470u), change_endian_long(0x2d7de1f4u), change_endian_long(0xb3ddbbeau));
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000002cu;
    TSIP.REG_ECH.WORD = 0x00003c1eu;
    TSIP.REG_28H.WORD = 0x00bf0001u;
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
    TSIP.REG_24H.WORD = 0x000040c1u;
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
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x1c5a2b81u);
    RX671_func080();
    RX671_func100(change_endian_long(0x37f7e497u), change_endian_long(0xe81e099fu), change_endian_long(0x9fa97955u), change_endian_long(0x9d6f5eefu));
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
    TSIP.REG_24H.WORD = 0x000001c0u;
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
    TSIP.REG_24H.WORD = 0x060049c1u;
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
    TSIP.REG_24H.WORD = 0x00000581u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00210000u;
    oLoop1 = 1;
    while( 1 == oLoop1 )
    {
        RX671_func100(change_endian_long(0xf78846cau), change_endian_long(0xc83f819du), change_endian_long(0x01d38977u), change_endian_long(0x1006cab4u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func101(change_endian_long(0x1648c8d9u), change_endian_long(0x6a288ba6u), change_endian_long(0x3ba94c5cu), change_endian_long(0xdda17a52u));
            oLoop1 = 0;
        }
        else
        {
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
            TSIP.REG_24H.WORD = 0x060049c1u;
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
            TSIP.REG_1CH.WORD = 0x00210000u;
            RX671_func101(change_endian_long(0x476ee741u), change_endian_long(0xcbb30f3fu), change_endian_long(0x411ccd49u), change_endian_long(0x83f58277u));
        }
    }
    RX671_func100(change_endian_long(0xed71c4a5u), change_endian_long(0xf9a19c2cu), change_endian_long(0xcdd19e65u), change_endian_long(0x904062d0u));
    TSIP.REG_24H.WORD = 0x000049c1u;
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
    TSIP.REG_24H.WORD = 0x060049c1u;
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
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xe4ce560bu);
    RX671_func080();
    RX671_func100(change_endian_long(0xc0ef1d35u), change_endian_long(0xc9025451u), change_endian_long(0x0cc10c06u), change_endian_long(0x7f1f0528u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(136);
    RX671_func100(change_endian_long(0xf11a6550u), change_endian_long(0xb59b12ebu), change_endian_long(0xff17229du), change_endian_long(0xd9ec3b53u));
    RX671_func314(136+64);
    TSIP.REG_24H.WORD = 0x000049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    RX671_func100(change_endian_long(0x7516f078u), change_endian_long(0x7c93488eu), change_endian_long(0x525cd7b6u), change_endian_long(0x779d0c67u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x05e30fedu);
    RX671_func080();
    RX671_func100(change_endian_long(0x9453454bu), change_endian_long(0x9413ed30u), change_endian_long(0x3c5d31a8u), change_endian_long(0x5ae6197au));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(204);
    RX671_func100(change_endian_long(0xb136df3au), change_endian_long(0x3d60eb08u), change_endian_long(0x29bda864u), change_endian_long(0xee1fcd13u));
    RX671_func314(204+64);
    TSIP.REG_24H.WORD = 0x000049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_ECH.WORD = 0x00000a52u;
    for (iLoop = 0; iLoop < 256; iLoop = iLoop + 1)
    {
        RX671_func100(change_endian_long(0x40164d51u), change_endian_long(0x59345382u), change_endian_long(0x8675c147u), change_endian_long(0x333591dfu));
        TSIP.REG_24H.WORD = 0x040049c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x04000149u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x060049c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x0400d0d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x02000149u;
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
        TSIP.REG_00H.WORD = 0x00013203u;
        TSIP.REG_2CH.WORD = 0x00000014u;
        RX671_func312(680);
        TSIP.REG_ECH.WORD = 0x00002e40u;
        RX671_func101(change_endian_long(0xdce24523u), change_endian_long(0x59eeb49cu), change_endian_long(0x7c6cdeefu), change_endian_long(0x213c32c7u));
    }
    RX671_func100(change_endian_long(0xf61f9028u), change_endian_long(0x0553a8e9u), change_endian_long(0x9b2e27f2u), change_endian_long(0x5f307186u));
    TSIP.REG_ECH.WORD = 0x38008a40u;
    TSIP.REG_ECH.WORD = 0x00000100u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    TSIP.REG_24H.WORD = 0x040049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x040019a1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x060049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0400d0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x020019a1u;
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
    RX671_func100(change_endian_long(0x579a9588u), change_endian_long(0xdb0e2b43u), change_endian_long(0x9470db8eu), change_endian_long(0xea59403cu));
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
    TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
    RX671_func080();
    RX671_func100(change_endian_long(0x67d4931eu), change_endian_long(0xc0edb857u), change_endian_long(0xd6e30fa6u), change_endian_long(0xfab8c963u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(408);
    RX671_func100(change_endian_long(0x6480226fu), change_endian_long(0xda945243u), change_endian_long(0x0abbd906u), change_endian_long(0x501ba7acu));
    RX671_func314(408+64);
    RX671_func100(change_endian_long(0x12023f2fu), change_endian_long(0x5246ebb0u), change_endian_long(0x9f113f96u), change_endian_long(0xfe6f704du));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xae62ddfbu);
    RX671_func080();
    RX671_func100(change_endian_long(0x2e5e9116u), change_endian_long(0x0b5c45cfu), change_endian_long(0xee2a75e5u), change_endian_long(0x5484cb1du));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(544);
    RX671_func100(change_endian_long(0x72cd0ea4u), change_endian_long(0x336f1e60u), change_endian_long(0xc4b952a3u), change_endian_long(0x921aba7eu));
    RX671_func314(544+64);
    TSIP.REG_24H.WORD = 0x000049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    RX671_func100(change_endian_long(0x921ac80au), change_endian_long(0x6fc4d4d6u), change_endian_long(0xf5b4a881u), change_endian_long(0xbf30572du));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
    RX671_func080();
    RX671_func100(change_endian_long(0xf18f0496u), change_endian_long(0x7f44f08du), change_endian_long(0xfcc7f9fau), change_endian_long(0x3b7a1495u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(476);
    RX671_func100(change_endian_long(0x51867e6bu), change_endian_long(0xe062fe2eu), change_endian_long(0x74aafd7fu), change_endian_long(0x801f5fabu));
    RX671_func314(476+64);
    RX671_func100(change_endian_long(0x048c9691u), change_endian_long(0xa6edf03bu), change_endian_long(0x4078e6f6u), change_endian_long(0xf3cf8407u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xea6a1c3du);
    RX671_func080();
    RX671_func100(change_endian_long(0x109ec8b4u), change_endian_long(0xa31e03c0u), change_endian_long(0x66f4218du), change_endian_long(0x15f200bau));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(612);
    RX671_func100(change_endian_long(0xb2a83bd0u), change_endian_long(0x91496435u), change_endian_long(0xdf7ae3ebu), change_endian_long(0xd6526814u));
    RX671_func314(612+64);
    TSIP.REG_ECH.WORD = 0x00000a52u;
    for (iLoop = 0; iLoop < 4; iLoop = iLoop + 1)
    {
        TSIP.REG_ECH.WORD = 0x0000b400u;
        TSIP.REG_ECH.WORD = 0x00000085u;
        RX671_func101(change_endian_long(0x7b6a977du), change_endian_long(0x2fa62653u), change_endian_long(0x8afb3b5eu), change_endian_long(0xcbb6a767u));
        RX671_func309();
        RX671_func100(change_endian_long(0xe2062a59u), change_endian_long(0x72ad9dbcu), change_endian_long(0xe14e82b3u), change_endian_long(0xfe8d4306u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
        RX671_func080();
        RX671_func100(change_endian_long(0xf000505du), change_endian_long(0x0ef3f698u), change_endian_long(0x9ddf16d2u), change_endian_long(0x6a484bb4u));
        TSIP.REG_00H.WORD = 0x00012303u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        RX671_func313(476);
        RX671_func100(change_endian_long(0x44fe652bu), change_endian_long(0x5b620e92u), change_endian_long(0x235b1312u), change_endian_long(0xb6b745d9u));
        RX671_func314(476+64);
        RX671_func100(change_endian_long(0xe735d2a9u), change_endian_long(0x978f77beu), change_endian_long(0x99c3f962u), change_endian_long(0x113ce496u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xea6a1c3du);
        RX671_func080();
        RX671_func100(change_endian_long(0xcabecb66u), change_endian_long(0xe963ad41u), change_endian_long(0xebbf5b06u), change_endian_long(0x5da15d48u));
        TSIP.REG_00H.WORD = 0x00012303u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        RX671_func313(612);
        RX671_func100(change_endian_long(0x79268cd5u), change_endian_long(0x2fc72a39u), change_endian_long(0x25a9da42u), change_endian_long(0x0fc0e7f5u));
        RX671_func314(612+64);
        RX671_func100(change_endian_long(0x213b54adu), change_endian_long(0xc0d3ad76u), change_endian_long(0xd7b9359du), change_endian_long(0xa900de5eu));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
        RX671_func080();
        RX671_func100(change_endian_long(0x35b8bef0u), change_endian_long(0xca065a16u), change_endian_long(0x909c5f7cu), change_endian_long(0x2cde99f8u));
        TSIP.REG_00H.WORD = 0x00012303u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        RX671_func313(408);
        RX671_func100(change_endian_long(0x01859dd6u), change_endian_long(0x470b5836u), change_endian_long(0xd5c06f2fu), change_endian_long(0xc751eec5u));
        RX671_func314(408+64);
        RX671_func100(change_endian_long(0xe4f5f86bu), change_endian_long(0x26eb11ddu), change_endian_long(0x5e487b15u), change_endian_long(0x97c2463fu));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xae62ddfbu);
        RX671_func080();
        RX671_func100(change_endian_long(0x3d15bb16u), change_endian_long(0xe0b4171du), change_endian_long(0x28490564u), change_endian_long(0xd5aa9e09u));
        TSIP.REG_00H.WORD = 0x00012303u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        RX671_func313(544);
        RX671_func100(change_endian_long(0x72470d88u), change_endian_long(0x329abd2cu), change_endian_long(0x52aee7ddu), change_endian_long(0x4c5bfa42u));
        RX671_func314(544+64);
        TSIP.REG_ECH.WORD = 0x00002e40u;
        RX671_func101(change_endian_long(0xe623ce71u), change_endian_long(0x6de6bb75u), change_endian_long(0x096a9bf4u), change_endian_long(0xce7f2674u));
    }
    TSIP.REG_ECH.WORD = 0x38008a40u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    RX671_func100(change_endian_long(0xcc255092u), change_endian_long(0x3d113a0eu), change_endian_long(0xaf613b9au), change_endian_long(0xaaf8fb4eu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
    RX671_func080();
    RX671_func100(change_endian_long(0xd5d5e528u), change_endian_long(0x9055c6b8u), change_endian_long(0x553ea885u), change_endian_long(0xfbdd6085u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func313(476);
    RX671_func100(change_endian_long(0x2d444d0au), change_endian_long(0x6c903962u), change_endian_long(0xb911c160u), change_endian_long(0x64129e2au));
    RX671_func314(476+64);
    RX671_func100(change_endian_long(0xedeebd8eu), change_endian_long(0x99da64f8u), change_endian_long(0x3a9a8179u), change_endian_long(0x8bce2acdu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
    RX671_func080();
    RX671_func100(change_endian_long(0xd38dc8f6u), change_endian_long(0xca325242u), change_endian_long(0x96494aabu), change_endian_long(0xb26b2158u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(408);
    RX671_func100(change_endian_long(0x31edd3abu), change_endian_long(0x0f20682au), change_endian_long(0xb84f5759u), change_endian_long(0x259dec83u));
    RX671_func314(408+64);
    RX671_func100(change_endian_long(0xb0b6f6bcu), change_endian_long(0x08b8fe1bu), change_endian_long(0x1843c899u), change_endian_long(0x11922b6cu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5dd049feu);
    RX671_func080();
    RX671_func100(change_endian_long(0xced5323eu), change_endian_long(0x758a4e2du), change_endian_long(0x139c7744u), change_endian_long(0x85fd45a5u));
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
    RX671_func100(change_endian_long(0x1193c563u), change_endian_long(0xe52dc7a2u), change_endian_long(0xd4e29e73u), change_endian_long(0x601b2f2eu));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX671_func312(0);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xea6a1c3du);
    RX671_func080();
    RX671_func100(change_endian_long(0x719ebaeau), change_endian_long(0x74cbb696u), change_endian_long(0xf382d75fu), change_endian_long(0xd80f0535u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func313(612);
    RX671_func100(change_endian_long(0xac3b337fu), change_endian_long(0x9b557a2cu), change_endian_long(0x95da9c50u), change_endian_long(0xdf3ecb6bu));
    RX671_func314(612+64);
    RX671_func100(change_endian_long(0xe72406dbu), change_endian_long(0xe6a7f835u), change_endian_long(0x1e2648b5u), change_endian_long(0x7783bf48u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xae62ddfbu);
    RX671_func080();
    RX671_func100(change_endian_long(0xa95f6d17u), change_endian_long(0xf6646063u), change_endian_long(0x387a50e4u), change_endian_long(0xe8696d4bu));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(544);
    RX671_func100(change_endian_long(0xb8617a36u), change_endian_long(0x6c27d2e2u), change_endian_long(0x7477286eu), change_endian_long(0xf8bea167u));
    RX671_func314(544+64);
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000086u;
    RX671_func101(change_endian_long(0x6738ab88u), change_endian_long(0xc5ce58c6u), change_endian_long(0x704b2bd1u), change_endian_long(0xadec064au));
    RX671_func309();
    RX671_func100(change_endian_long(0xad9c33c1u), change_endian_long(0x96820bf5u), change_endian_long(0x511cb232u), change_endian_long(0x83ca35b3u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5dd049feu);
    RX671_func080();
    RX671_func100(change_endian_long(0x6d04b9b6u), change_endian_long(0x4d109a5eu), change_endian_long(0xf638d27bu), change_endian_long(0x3a6a74edu));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func313(68);
    RX671_func100(change_endian_long(0x3067f105u), change_endian_long(0xa16aa2eau), change_endian_long(0xecd69145u), change_endian_long(0xcd09fc7eu));
    RX671_func314(68+64);
    RX671_func100(change_endian_long(0x54d99403u), change_endian_long(0x6e0e2750u), change_endian_long(0xb459dd8du), change_endian_long(0xe77f5ebeu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x945d2158u);
    RX671_func080();
    RX671_func100(change_endian_long(0x231a0ca9u), change_endian_long(0xa066fd58u), change_endian_long(0xae955738u), change_endian_long(0x29e66e6fu));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(0);
    RX671_func100(change_endian_long(0xe8dd2e7bu), change_endian_long(0x587826e8u), change_endian_long(0xcc507a7eu), change_endian_long(0xb8751b09u));
    RX671_func314(0+64);
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000002cu;
    TSIP.REG_ECH.WORD = 0x0000381eu;
    TSIP.REG_ECH.WORD = 0x00007c00u;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX671/RX671_func311_r1.prc
 *********************************************************************************************************************/
