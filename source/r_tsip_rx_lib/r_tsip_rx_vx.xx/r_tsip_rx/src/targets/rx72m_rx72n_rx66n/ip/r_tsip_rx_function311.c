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
* Function Name: RX72M_RX72N_RX66N_func311
*******************************************************************************************************************/ /**
* @details       RX72M func311
* @note          None
*/
void RX72M_RX72N_RX66N_func311(void)
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0x78be8d7du), change_endian_long(0xf05f8747u), change_endian_long(0x43369b19u), change_endian_long(0x5497801du));
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc7f5dbafu), change_endian_long(0xd5336706u), change_endian_long(0x7c592cacu), change_endian_long(0x4f248b23u));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func312(680);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5fe685f6u);
    RX72M_RX72N_RX66N_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func312(748);
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
        RX72M_RX72N_RX66N_func100(change_endian_long(0x3795624eu), change_endian_long(0x979a9b9du), change_endian_long(0xaa66924du), change_endian_long(0x8bf8addfu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func101(change_endian_long(0xa53a86d4u), change_endian_long(0xd8391b27u), change_endian_long(0x6a7a6d52u), change_endian_long(0x496eef18u));
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
            RX72M_RX72N_RX66N_func101(change_endian_long(0x84cc7d46u), change_endian_long(0x87c90128u), change_endian_long(0xe7c8d861u), change_endian_long(0xa506238eu));
        }
    }
    RX72M_RX72N_RX66N_func100(change_endian_long(0x3ac4136bu), change_endian_long(0x598787d5u), change_endian_long(0xf62e2ad3u), change_endian_long(0x80182c88u));
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x951f4a10u), change_endian_long(0xb1a6c4adu), change_endian_long(0xbf7d2c2au), change_endian_long(0xf99fde1au));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func313(136);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xd78e7d12u), change_endian_long(0xd50419b7u), change_endian_long(0x375ec374u), change_endian_long(0xbc185d28u));
    RX72M_RX72N_RX66N_func314(136+64);
    TSIP.REG_24H.WORD = 0x000049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    RX72M_RX72N_RX66N_func100(change_endian_long(0x43aebfafu), change_endian_long(0xe6c39a0au), change_endian_long(0xf63df68du), change_endian_long(0x5419290bu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x05e30fedu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x359fdd31u), change_endian_long(0xfc3f2723u), change_endian_long(0x542c25ccu), change_endian_long(0x8c8511d0u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func313(204);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x5228fde2u), change_endian_long(0xebec6c4fu), change_endian_long(0x687d7c7cu), change_endian_long(0x0365aff3u));
    RX72M_RX72N_RX66N_func314(204+64);
    TSIP.REG_24H.WORD = 0x000049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_ECH.WORD = 0x00000a52u;
    for (iLoop = 0; iLoop < 256; iLoop = iLoop + 1)
    {
        RX72M_RX72N_RX66N_func100(change_endian_long(0x28454653u), change_endian_long(0x87b664dau), change_endian_long(0x934003c8u), change_endian_long(0x3e3be314u));
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
        RX72M_RX72N_RX66N_func080();
        TSIP.REG_00H.WORD = 0x00013203u;
        TSIP.REG_2CH.WORD = 0x00000014u;
        RX72M_RX72N_RX66N_func312(680);
        TSIP.REG_ECH.WORD = 0x00002e40u;
        RX72M_RX72N_RX66N_func101(change_endian_long(0x6e0aa3e3u), change_endian_long(0x6fc3e640u), change_endian_long(0xae4c2256u), change_endian_long(0x0836532bu));
    }
    RX72M_RX72N_RX66N_func100(change_endian_long(0x6eb6f0d5u), change_endian_long(0xe5666760u), change_endian_long(0x00b394b5u), change_endian_long(0xbd110726u));
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
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x53b65515u), change_endian_long(0x7bb45daeu), change_endian_long(0xcc4a2fe6u), change_endian_long(0xdc092a63u));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX72M_RX72N_RX66N_func312(680);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x73ec4b47u), change_endian_long(0x1e57cfe4u), change_endian_long(0x5887db07u), change_endian_long(0xae9ec620u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func313(408);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x8147acebu), change_endian_long(0xad33b334u), change_endian_long(0x1d43f55fu), change_endian_long(0x81ac1e05u));
    RX72M_RX72N_RX66N_func314(408+64);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x19ba5f3cu), change_endian_long(0xe5b0a016u), change_endian_long(0x5ff133e4u), change_endian_long(0xe0efc460u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xae62ddfbu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc718dadbu), change_endian_long(0xcc983bf6u), change_endian_long(0xb83532beu), change_endian_long(0xb69047f7u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func313(544);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x049db289u), change_endian_long(0x39a75e52u), change_endian_long(0x73357c88u), change_endian_long(0xc86e91cbu));
    RX72M_RX72N_RX66N_func314(544+64);
    TSIP.REG_24H.WORD = 0x000049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    RX72M_RX72N_RX66N_func100(change_endian_long(0xa77392eau), change_endian_long(0x5d8f983cu), change_endian_long(0xba173c8bu), change_endian_long(0x94e4dd2au));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xd44c1f93u), change_endian_long(0x9df0a2dbu), change_endian_long(0x63ab2693u), change_endian_long(0xc88a4238u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func313(476);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x06e9fda5u), change_endian_long(0x16ab7e91u), change_endian_long(0xe6b8e1e5u), change_endian_long(0x042b24f1u));
    RX72M_RX72N_RX66N_func314(476+64);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xd1e1499eu), change_endian_long(0xb74699edu), change_endian_long(0x1efec66du), change_endian_long(0xc2253462u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xea6a1c3du);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe715cfa1u), change_endian_long(0x043b2053u), change_endian_long(0x3264fd7bu), change_endian_long(0x30f40376u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func313(612);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xcf83bd4au), change_endian_long(0x5881019cu), change_endian_long(0x9fc43aabu), change_endian_long(0x534d1e9bu));
    RX72M_RX72N_RX66N_func314(612+64);
    TSIP.REG_ECH.WORD = 0x00000a52u;
    for (iLoop = 0; iLoop < 4; iLoop = iLoop + 1)
    {
        TSIP.REG_ECH.WORD = 0x0000b400u;
        TSIP.REG_ECH.WORD = 0x00000085u;
        RX72M_RX72N_RX66N_func101(change_endian_long(0x473cc75du), change_endian_long(0x9100d7a2u), change_endian_long(0xb4233edeu), change_endian_long(0x83347c04u));
        RX72M_RX72N_RX66N_func309();
        RX72M_RX72N_RX66N_func100(change_endian_long(0x79974f20u), change_endian_long(0x20c86f43u), change_endian_long(0x32807764u), change_endian_long(0x5d288433u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
        RX72M_RX72N_RX66N_func080();
        RX72M_RX72N_RX66N_func100(change_endian_long(0xd8f8b3c0u), change_endian_long(0x0b974212u), change_endian_long(0x64b5c140u), change_endian_long(0xa8532444u));
        TSIP.REG_00H.WORD = 0x00012303u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        RX72M_RX72N_RX66N_func313(476);
        RX72M_RX72N_RX66N_func100(change_endian_long(0x339d4f2au), change_endian_long(0xb88c6313u), change_endian_long(0xfb618bf7u), change_endian_long(0xcd0ff6feu));
        RX72M_RX72N_RX66N_func314(476+64);
        RX72M_RX72N_RX66N_func100(change_endian_long(0x74e4d674u), change_endian_long(0x0206282fu), change_endian_long(0x669a8913u), change_endian_long(0x6b48123bu));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xea6a1c3du);
        RX72M_RX72N_RX66N_func080();
        RX72M_RX72N_RX66N_func100(change_endian_long(0x3e79e594u), change_endian_long(0xa8349ac7u), change_endian_long(0x117cb803u), change_endian_long(0xd6cacad4u));
        TSIP.REG_00H.WORD = 0x00012303u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        RX72M_RX72N_RX66N_func313(612);
        RX72M_RX72N_RX66N_func100(change_endian_long(0x73b5499fu), change_endian_long(0x187a7c76u), change_endian_long(0x0c62f42cu), change_endian_long(0xc21fbef9u));
        RX72M_RX72N_RX66N_func314(612+64);
        RX72M_RX72N_RX66N_func100(change_endian_long(0x37c654b6u), change_endian_long(0xdac4dce6u), change_endian_long(0x6bdb54b0u), change_endian_long(0x85941c3du));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
        RX72M_RX72N_RX66N_func080();
        RX72M_RX72N_RX66N_func100(change_endian_long(0x29568d7fu), change_endian_long(0x677e73b4u), change_endian_long(0x9d9e88b2u), change_endian_long(0x81939c1bu));
        TSIP.REG_00H.WORD = 0x00012303u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        RX72M_RX72N_RX66N_func313(408);
        RX72M_RX72N_RX66N_func100(change_endian_long(0xc0237c24u), change_endian_long(0x10fb806au), change_endian_long(0x2a42e7e2u), change_endian_long(0xf6696b3fu));
        RX72M_RX72N_RX66N_func314(408+64);
        RX72M_RX72N_RX66N_func100(change_endian_long(0xc800b747u), change_endian_long(0x106905dbu), change_endian_long(0x7e9626d7u), change_endian_long(0x1938dd83u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xae62ddfbu);
        RX72M_RX72N_RX66N_func080();
        RX72M_RX72N_RX66N_func100(change_endian_long(0x87868b61u), change_endian_long(0x76f69b53u), change_endian_long(0x0b89f634u), change_endian_long(0xfb731dd4u));
        TSIP.REG_00H.WORD = 0x00012303u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        RX72M_RX72N_RX66N_func313(544);
        RX72M_RX72N_RX66N_func100(change_endian_long(0x51200ea0u), change_endian_long(0x8d59d428u), change_endian_long(0xf9288d10u), change_endian_long(0x866b0ab9u));
        RX72M_RX72N_RX66N_func314(544+64);
        TSIP.REG_ECH.WORD = 0x00002e40u;
        RX72M_RX72N_RX66N_func101(change_endian_long(0x78041dd4u), change_endian_long(0x2d416121u), change_endian_long(0x8e1558ccu), change_endian_long(0xd0620b79u));
    }
    TSIP.REG_ECH.WORD = 0x38008a40u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x545bb79bu), change_endian_long(0xf57c7e0au), change_endian_long(0xa7eba074u), change_endian_long(0x390d8761u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xa5f048edu), change_endian_long(0xe16bfb26u), change_endian_long(0xa660c348u), change_endian_long(0xbbbe9d46u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX72M_RX72N_RX66N_func313(476);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xbf014f04u), change_endian_long(0x7ecaef06u), change_endian_long(0x136bd5fau), change_endian_long(0x12e903cfu));
    RX72M_RX72N_RX66N_func314(476+64);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xac2892f1u), change_endian_long(0xdc651069u), change_endian_long(0x883386f1u), change_endian_long(0xc586c6a6u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xfd5daa9fu), change_endian_long(0xb8d165f5u), change_endian_long(0x14487732u), change_endian_long(0x5f8a33d8u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func313(408);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xf30890fau), change_endian_long(0x6dd0ce10u), change_endian_long(0x3356f66bu), change_endian_long(0x9137384bu));
    RX72M_RX72N_RX66N_func314(408+64);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x43d0becfu), change_endian_long(0x27aaac8du), change_endian_long(0x925fdecdu), change_endian_long(0x76648eb8u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5dd049feu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x1647a60eu), change_endian_long(0x99cf40b8u), change_endian_long(0x5ae50a4eu), change_endian_long(0x9d4dfcd4u));
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0xbc13f429u), change_endian_long(0x606b3a77u), change_endian_long(0x8d5185b1u), change_endian_long(0x96cc7530u));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX72M_RX72N_RX66N_func312(0);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xea6a1c3du);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x3933442du), change_endian_long(0x50b797b0u), change_endian_long(0xf147892cu), change_endian_long(0x49df76c9u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX72M_RX72N_RX66N_func313(612);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc1869e65u), change_endian_long(0xd3d845edu), change_endian_long(0x1476c538u), change_endian_long(0x3fca9ad6u));
    RX72M_RX72N_RX66N_func314(612+64);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x826c848au), change_endian_long(0x93b69ad0u), change_endian_long(0x349608a4u), change_endian_long(0x18de4efbu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xae62ddfbu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe10dd3fau), change_endian_long(0x5523dd3du), change_endian_long(0xdc9dbf31u), change_endian_long(0x55db130eu));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func313(544);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xbb390a4bu), change_endian_long(0xb4c13893u), change_endian_long(0xcdc22a19u), change_endian_long(0xd41aac40u));
    RX72M_RX72N_RX66N_func314(544+64);
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000086u;
    RX72M_RX72N_RX66N_func101(change_endian_long(0x5edf5f0eu), change_endian_long(0x81352131u), change_endian_long(0xe0bf0b2fu), change_endian_long(0x6719c0e5u));
    RX72M_RX72N_RX66N_func309();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x9caa1d65u), change_endian_long(0x26917618u), change_endian_long(0xa5b7cb09u), change_endian_long(0xb8e50f9fu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5dd049feu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xaef101dcu), change_endian_long(0x330c57a5u), change_endian_long(0xb7248918u), change_endian_long(0x65d3fd98u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX72M_RX72N_RX66N_func313(68);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x9b7c3254u), change_endian_long(0x038e2fbfu), change_endian_long(0xb6a3b062u), change_endian_long(0xd8acee0bu));
    RX72M_RX72N_RX66N_func314(68+64);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x9d2808d7u), change_endian_long(0xa64344b5u), change_endian_long(0x5cdbda98u), change_endian_long(0x2844b087u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x945d2158u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc3bc3105u), change_endian_long(0x7dc6f12fu), change_endian_long(0x0ea294e7u), change_endian_long(0xdc6e9a79u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func313(0);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xa58c5725u), change_endian_long(0x222c6155u), change_endian_long(0xfe28cc15u), change_endian_long(0xadc9283fu));
    RX72M_RX72N_RX66N_func314(0+64);
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000002cu;
    TSIP.REG_ECH.WORD = 0x0000381eu;
    TSIP.REG_ECH.WORD = 0x00007c00u;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX72M/RX72M_func311_r1.prc
 *********************************************************************************************************************/
