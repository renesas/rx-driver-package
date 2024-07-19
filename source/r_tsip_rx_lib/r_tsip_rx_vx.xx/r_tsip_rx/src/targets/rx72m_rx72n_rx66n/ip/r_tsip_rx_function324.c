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
* Function Name: RX72M_RX72N_RX66N_func324
*******************************************************************************************************************/ /**
* @details       RX72M func324
* @note          None
*/
void RX72M_RX72N_RX66N_func324(void)
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
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_ECH.WORD = 0x00003fbeu;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x83849705u), change_endian_long(0x6b7e1146u), change_endian_long(0x86c73f98u), change_endian_long(0x8de9b906u));
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_24H.WORD = 0x000005c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000060c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0xfa21a8a9u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0abc883eu), change_endian_long(0x5efa6748u), change_endian_long(0xbbc8c36bu), change_endian_long(0x4b169863u));
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0xcd8379f6u), change_endian_long(0x4406bfe7u), change_endian_long(0x13244a73u), change_endian_long(0xb3a8b944u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000015u;
    RX72M_RX72N_RX66N_func320(716);
    TSIP.REG_24H.WORD = 0x0000a0c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x7775adb6u);
    RX72M_RX72N_RX66N_func080();
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func320(752);
    TSIP.REG_28H.WORD = 0x00bf0001u;
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
    TSIP.REG_1CH.WORD = 0x00210000u;
    oLoop1 = 1;
    while( 1 == oLoop1 )
    {
        RX72M_RX72N_RX66N_func100(change_endian_long(0x30582f45u), change_endian_long(0xe7e83872u), change_endian_long(0x156a5d38u), change_endian_long(0xe3d577b8u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func101(change_endian_long(0x0610ad05u), change_endian_long(0xe8fe1b2cu), change_endian_long(0x15521e4bu), change_endian_long(0x33769b5bu));
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
            RX72M_RX72N_RX66N_func101(change_endian_long(0xe250f8f8u), change_endian_long(0x4df1271eu), change_endian_long(0xb2ee5ce5u), change_endian_long(0x53635b77u));
        }
    }
    RX72M_RX72N_RX66N_func100(change_endian_long(0x761f85ffu), change_endian_long(0x88009d46u), change_endian_long(0xf51d8121u), change_endian_long(0x2f168c2bu));
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
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5f55b804u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xdfdfe367u), change_endian_long(0x750e6180u), change_endian_long(0xadfc5e12u), change_endian_long(0x23461762u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func319(136);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x9ec755c2u), change_endian_long(0xb212c9d9u), change_endian_long(0x7bdc7798u), change_endian_long(0xc4041285u));
    RX72M_RX72N_RX66N_func314(136+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x478c4e12u), change_endian_long(0xe11a8d6fu), change_endian_long(0x97160575u), change_endian_long(0x149c4993u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x22880c7eu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x79c457bau), change_endian_long(0x92a4ef91u), change_endian_long(0x781ff0eeu), change_endian_long(0xbfeda3dbu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX72M_RX72N_RX66N_func319(172);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x2a37d450u), change_endian_long(0xd3bfed60u), change_endian_long(0x6b60691eu), change_endian_long(0xf0e7034eu));
    RX72M_RX72N_RX66N_func314(172+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x98b0ddbbu), change_endian_long(0xff07ff3fu), change_endian_long(0x57c0cfe1u), change_endian_long(0x85ffda71u));
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_24H.WORD = 0x000049c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x8e78b588u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x8a766876u), change_endian_long(0xa8df4549u), change_endian_long(0xb0915254u), change_endian_long(0x43fe37ffu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func319(208);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe1d8b430u), change_endian_long(0x83662fc7u), change_endian_long(0xfe4cd404u), change_endian_long(0x4f91d21au));
    RX72M_RX72N_RX66N_func314(208+32);
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_24H.WORD = 0x000049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_ECH.WORD = 0x00000a52u;
    for (iLoop = 0; iLoop < 128; iLoop = iLoop + 1)
    {
        RX72M_RX72N_RX66N_func100(change_endian_long(0x39be133bu), change_endian_long(0xbdaa0dbcu), change_endian_long(0xeade33f6u), change_endian_long(0xe4d01db1u));
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
        RX72M_RX72N_RX66N_func100(change_endian_long(0x6bd08c19u), change_endian_long(0x6cfedac6u), change_endian_long(0xc518e420u), change_endian_long(0xef74a28bu));
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
        TSIP.REG_00H.WORD = 0x00003283u;
        TSIP.REG_2CH.WORD = 0x00000015u;
        RX72M_RX72N_RX66N_func320(716);
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_ECH.WORD = 0x00002e40u;
        RX72M_RX72N_RX66N_func101(change_endian_long(0x830d72eau), change_endian_long(0x796486d8u), change_endian_long(0x1ad901a1u), change_endian_long(0x01e99495u));
    }
    TSIP.REG_ECH.WORD = 0x38008a40u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc1c334c8u), change_endian_long(0xfdbe6663u), change_endian_long(0x261821fau), change_endian_long(0x16cec99fu));
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0xca2d54cbu), change_endian_long(0x23e33830u), change_endian_long(0x12304d8bu), change_endian_long(0xde12e9ffu));
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe737430du), change_endian_long(0x55bd30feu), change_endian_long(0xf9be6cecu), change_endian_long(0x563f8bb5u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000015u;
    RX72M_RX72N_RX66N_func320(716);
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x8aa5e82eu), change_endian_long(0xcadcec01u), change_endian_long(0xe9f53eeeu), change_endian_long(0xf0049f2bu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func319(408);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x5a5468c7u), change_endian_long(0x3906804fu), change_endian_long(0xa2415977u), change_endian_long(0x40f51756u));
    RX72M_RX72N_RX66N_func314(408+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x8f0c274fu), change_endian_long(0x4ae55520u), change_endian_long(0xd19bba5du), change_endian_long(0x6e773596u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x794bc476u), change_endian_long(0xbda65e67u), change_endian_long(0xb707d4ccu), change_endian_long(0x0d8cf9d1u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX72M_RX72N_RX66N_func319(444);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x5b7284d7u), change_endian_long(0x8df23b3eu), change_endian_long(0x3d102009u), change_endian_long(0x2e3255e5u));
    RX72M_RX72N_RX66N_func314(444+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xef3e44c6u), change_endian_long(0x7d93e596u), change_endian_long(0xfa18b497u), change_endian_long(0x6f035899u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x09f0f102u), change_endian_long(0x443680afu), change_endian_long(0xbe827e1eu), change_endian_long(0x91ccc810u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func319(544);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xc8eae193u), change_endian_long(0x81cff951u), change_endian_long(0xbe6c644cu), change_endian_long(0x23c210d5u));
    RX72M_RX72N_RX66N_func314(544+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x3eacf01au), change_endian_long(0xc328c54fu), change_endian_long(0x125ebafcu), change_endian_long(0x31d4607cu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xb3aeaa3bu), change_endian_long(0x971a0a77u), change_endian_long(0xac14600fu), change_endian_long(0xb07c13f7u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX72M_RX72N_RX66N_func319(580);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0c426169u), change_endian_long(0xdc14664du), change_endian_long(0x76dbdb3du), change_endian_long(0x578d2634u));
    RX72M_RX72N_RX66N_func314(580+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x71aa6315u), change_endian_long(0x4649500du), change_endian_long(0xf7e9f647u), change_endian_long(0xb5915a3eu));
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_24H.WORD = 0x000049c1u;
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
    TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xda2c9e85u), change_endian_long(0x124ed96fu), change_endian_long(0xda8a64a3u), change_endian_long(0x2e68300au));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func319(480);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xda76d4a0u), change_endian_long(0xea626056u), change_endian_long(0x0a2e9501u), change_endian_long(0xc9e1b5b8u));
    RX72M_RX72N_RX66N_func314(480+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x1ecd0e19u), change_endian_long(0x7dfd8282u), change_endian_long(0x95742bd5u), change_endian_long(0xa973ab8au));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x4c9015bcu), change_endian_long(0xd688dc9du), change_endian_long(0xf0f56837u), change_endian_long(0xa7ee7a31u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func319(616);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x9ad5648eu), change_endian_long(0xc739a64bu), change_endian_long(0x44be776cu), change_endian_long(0xdb4a7defu));
    RX72M_RX72N_RX66N_func314(616+32);
    TSIP.REG_ECH.WORD = 0x00000a52u;
    for (iLoop = 0; iLoop < 3; iLoop = iLoop + 1)
    {
        TSIP.REG_ECH.WORD = 0x0000b7a0u;
        TSIP.REG_ECH.WORD = 0x000000f0u;
        RX72M_RX72N_RX66N_func101(change_endian_long(0x06d8037fu), change_endian_long(0xa0f76198u), change_endian_long(0x8bb51058u), change_endian_long(0x314d0e22u));
        RX72M_RX72N_RX66N_func318();
        RX72M_RX72N_RX66N_func100(change_endian_long(0x9ab40802u), change_endian_long(0x216a1985u), change_endian_long(0x0d649413u), change_endian_long(0xeb185aaau));
        TSIP.REG_28H.WORD = 0x009f0001u;
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
        RX72M_RX72N_RX66N_func080();
        RX72M_RX72N_RX66N_func100(change_endian_long(0xa9eab505u), change_endian_long(0xa16610eeu), change_endian_long(0x78b7017cu), change_endian_long(0xbfb02945u));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        RX72M_RX72N_RX66N_func319(408);
        RX72M_RX72N_RX66N_func100(change_endian_long(0x051bc3aau), change_endian_long(0xdf754a16u), change_endian_long(0xb0794287u), change_endian_long(0xbe78dbabu));
        RX72M_RX72N_RX66N_func314(408+32);
        RX72M_RX72N_RX66N_func100(change_endian_long(0xb8d8e8a8u), change_endian_long(0x741bd0feu), change_endian_long(0x19f99f6fu), change_endian_long(0xea593ec7u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
        RX72M_RX72N_RX66N_func080();
        RX72M_RX72N_RX66N_func100(change_endian_long(0xb0086d13u), change_endian_long(0x50f78e07u), change_endian_long(0x3bf880f5u), change_endian_long(0x9e4e6cd3u));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000023u;
        RX72M_RX72N_RX66N_func319(444);
        RX72M_RX72N_RX66N_func100(change_endian_long(0xa08e6ff7u), change_endian_long(0x6049930eu), change_endian_long(0xe3a1a42au), change_endian_long(0xf5f13204u));
        RX72M_RX72N_RX66N_func314(444+32);
        RX72M_RX72N_RX66N_func100(change_endian_long(0xfc13dc17u), change_endian_long(0x6345bc53u), change_endian_long(0xcabf8937u), change_endian_long(0x01daf734u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
        RX72M_RX72N_RX66N_func080();
        RX72M_RX72N_RX66N_func100(change_endian_long(0x9d524e6eu), change_endian_long(0x3b26e8b7u), change_endian_long(0xf6cdf3a3u), change_endian_long(0x8ff03c05u));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        RX72M_RX72N_RX66N_func319(480);
        RX72M_RX72N_RX66N_func100(change_endian_long(0x68de7aedu), change_endian_long(0x9218bb35u), change_endian_long(0xd122273du), change_endian_long(0xf329672au));
        RX72M_RX72N_RX66N_func314(480+32);
        RX72M_RX72N_RX66N_func100(change_endian_long(0x9687838du), change_endian_long(0x3e1e8caeu), change_endian_long(0xbf0dfd74u), change_endian_long(0x1b263302u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
        RX72M_RX72N_RX66N_func080();
        RX72M_RX72N_RX66N_func100(change_endian_long(0x68768c6bu), change_endian_long(0x4e70a237u), change_endian_long(0x601f6a86u), change_endian_long(0x0782f749u));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        RX72M_RX72N_RX66N_func319(544);
        RX72M_RX72N_RX66N_func100(change_endian_long(0x865b719cu), change_endian_long(0x0a99d2dfu), change_endian_long(0x8ea4dd49u), change_endian_long(0xf63e674fu));
        RX72M_RX72N_RX66N_func314(544+32);
        RX72M_RX72N_RX66N_func100(change_endian_long(0x16377a93u), change_endian_long(0xad83c3f4u), change_endian_long(0x88fabef4u), change_endian_long(0x9fe1bfacu));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
        RX72M_RX72N_RX66N_func080();
        RX72M_RX72N_RX66N_func100(change_endian_long(0x4f7e86e4u), change_endian_long(0x773a3e86u), change_endian_long(0x0e405bbdu), change_endian_long(0x75488c72u));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000023u;
        RX72M_RX72N_RX66N_func319(580);
        RX72M_RX72N_RX66N_func100(change_endian_long(0xdd668f24u), change_endian_long(0x7b420980u), change_endian_long(0xaab39dfbu), change_endian_long(0x0cb83e88u));
        RX72M_RX72N_RX66N_func314(580+32);
        RX72M_RX72N_RX66N_func100(change_endian_long(0x4e2fd584u), change_endian_long(0x9b603d33u), change_endian_long(0x2cac237bu), change_endian_long(0xcd83904bu));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
        RX72M_RX72N_RX66N_func080();
        RX72M_RX72N_RX66N_func100(change_endian_long(0xbdf46be8u), change_endian_long(0x33ec7330u), change_endian_long(0x73fb4e12u), change_endian_long(0x67292fb7u));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        RX72M_RX72N_RX66N_func319(616);
        RX72M_RX72N_RX66N_func100(change_endian_long(0xbc1a3738u), change_endian_long(0xcb9082a2u), change_endian_long(0x7c4ef5b7u), change_endian_long(0xa565c2aau));
        RX72M_RX72N_RX66N_func314(616+32);
        TSIP.REG_ECH.WORD = 0x00002e40u;
        RX72M_RX72N_RX66N_func101(change_endian_long(0xf7c3ce25u), change_endian_long(0x4883bc20u), change_endian_long(0x56066121u), change_endian_long(0xd455b609u));
    }
    TSIP.REG_ECH.WORD = 0x38008a40u;
    TSIP.REG_ECH.WORD = 0x00000003u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f1u;
    RX72M_RX72N_RX66N_func101(change_endian_long(0x9697cab6u), change_endian_long(0x0a5377ffu), change_endian_long(0x20d7cfbfu), change_endian_long(0x625e5d6eu));
    RX72M_RX72N_RX66N_func318();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xfa6e7996u), change_endian_long(0xbf673f7du), change_endian_long(0xf775aaf0u), change_endian_long(0x5040b154u));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x6acc8cd9u), change_endian_long(0x6970285bu), change_endian_long(0xd1c934e9u), change_endian_long(0x95befda7u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func319(408);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0fcbf24eu), change_endian_long(0xb1da5b20u), change_endian_long(0x0934e308u), change_endian_long(0x542b578cu));
    RX72M_RX72N_RX66N_func314(408+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x24a87b82u), change_endian_long(0xed76571bu), change_endian_long(0xddeabbedu), change_endian_long(0x0110cf18u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xf7015914u), change_endian_long(0x7db1e08au), change_endian_long(0x1842928eu), change_endian_long(0x4d309d3du));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX72M_RX72N_RX66N_func319(444);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x33dc7f35u), change_endian_long(0xf41eed7fu), change_endian_long(0x9d6a2b9fu), change_endian_long(0xb87a4d73u));
    RX72M_RX72N_RX66N_func314(444+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xfc07d897u), change_endian_long(0xcc131e05u), change_endian_long(0x0ab4eedbu), change_endian_long(0xd1a77411u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x3684ba7fu), change_endian_long(0x14284d6fu), change_endian_long(0x4f376e44u), change_endian_long(0xb0708351u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX72M_RX72N_RX66N_func319(480);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xef90a69cu), change_endian_long(0x1d7eb83du), change_endian_long(0x49cdc3edu), change_endian_long(0xb8887f86u));
    RX72M_RX72N_RX66N_func314(480+32);
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f2u;
    RX72M_RX72N_RX66N_func101(change_endian_long(0x7a0cff12u), change_endian_long(0x39895daeu), change_endian_long(0xa21bd7c4u), change_endian_long(0xef0e16a4u));
    RX72M_RX72N_RX66N_func318();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe3ca86a5u), change_endian_long(0xfa97a561u), change_endian_long(0xb7bd927au), change_endian_long(0x7e6bd1dau));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x1d2d1d3au);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x39082dc4u), change_endian_long(0xc223ed60u), change_endian_long(0xc73e8f12u), change_endian_long(0xa78ccd33u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func319(272);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x8d0f918bu), change_endian_long(0xe3a54187u), change_endian_long(0x0a55ee4fu), change_endian_long(0x77255276u));
    RX72M_RX72N_RX66N_func314(272+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xbb4e433fu), change_endian_long(0xbb00a92du), change_endian_long(0xbf68883eu), change_endian_long(0x24566ff3u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x24fe6a76u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xd1d8f79bu), change_endian_long(0x945dffaeu), change_endian_long(0x18ed2132u), change_endian_long(0xb749af60u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX72M_RX72N_RX66N_func319(308);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x3c356b6eu), change_endian_long(0xe615b5eeu), change_endian_long(0x85c17df3u), change_endian_long(0x178632c8u));
    RX72M_RX72N_RX66N_func314(308+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xbdb2d0cau), change_endian_long(0x00da2fd6u), change_endian_long(0x1892b81eu), change_endian_long(0x56a8592cu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x6176b072u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x89abb722u), change_endian_long(0x48a68386u), change_endian_long(0x9ba0c9afu), change_endian_long(0x5719b157u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX72M_RX72N_RX66N_func319(344);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xb4f3e95au), change_endian_long(0xbc274a79u), change_endian_long(0x229f9dd2u), change_endian_long(0x78059af5u));
    RX72M_RX72N_RX66N_func314(344+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x782f7c72u), change_endian_long(0x9967ec9cu), change_endian_long(0x6612d019u), change_endian_long(0x39e92d3eu));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xd909ac62u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xb32b3d70u), change_endian_long(0x1c315db3u), change_endian_long(0x94fb1e93u), change_endian_long(0xce39b1abu));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func319(408);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x2ef00e46u), change_endian_long(0x7162443fu), change_endian_long(0xfbaed86du), change_endian_long(0x600f223cu));
    RX72M_RX72N_RX66N_func314(408+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x24054f9bu), change_endian_long(0xb5c029bdu), change_endian_long(0xc6f613f7u), change_endian_long(0x7f3a0dbfu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x351fe80cu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0a3d9ebdu), change_endian_long(0x5a7675abu), change_endian_long(0xd3de96bfu), change_endian_long(0x5cdd1256u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX72M_RX72N_RX66N_func319(444);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xb4d78819u), change_endian_long(0x54da72feu), change_endian_long(0xc5d8c2b2u), change_endian_long(0xd907f4aau));
    RX72M_RX72N_RX66N_func314(444+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x47315329u), change_endian_long(0x19366992u), change_endian_long(0xf8b100e4u), change_endian_long(0xb37c0481u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xdbdd06ceu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xeb2ee87fu), change_endian_long(0x621e56a9u), change_endian_long(0xe3600606u), change_endian_long(0xad92f1f8u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX72M_RX72N_RX66N_func319(480);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x131dd98fu), change_endian_long(0x874b1d2eu), change_endian_long(0x9d01bf1fu), change_endian_long(0xe250e040u));
    RX72M_RX72N_RX66N_func314(480+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x1d7bf68fu), change_endian_long(0xec0c48adu), change_endian_long(0x20883675u), change_endian_long(0x1ffbb28bu));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x84992ba5u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x77a0a7ffu), change_endian_long(0x2978bc8au), change_endian_long(0x8ba74e20u), change_endian_long(0x62ea61d2u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX72M_RX72N_RX66N_func320(0);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x064417e5u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x8776b4abu), change_endian_long(0xe2b212aeu), change_endian_long(0x940ce2e4u), change_endian_long(0x3a2ebe56u));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000013u;
    RX72M_RX72N_RX66N_func320(36);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x49880ba2u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xb77467d2u), change_endian_long(0xe876cfc3u), change_endian_long(0x9cbb3c5au), change_endian_long(0xffd7ebcau));
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX72M_RX72N_RX66N_func320(72);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x186e5c79u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x0faae8b0u), change_endian_long(0x7d0b8b7fu), change_endian_long(0xdd49a993u), change_endian_long(0xf5bcd454u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func319(544);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x242de9f5u), change_endian_long(0xd5299a00u), change_endian_long(0x46f841adu), change_endian_long(0xeb9dc950u));
    RX72M_RX72N_RX66N_func314(544+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x1889167fu), change_endian_long(0xf5bed06au), change_endian_long(0x453b6330u), change_endian_long(0xe14da63cu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xed327686u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x42a7b978u), change_endian_long(0x25db938cu), change_endian_long(0xd04168e0u), change_endian_long(0x49bc3452u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX72M_RX72N_RX66N_func319(580);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x43e0b338u), change_endian_long(0x3ac12d0bu), change_endian_long(0xd68952eau), change_endian_long(0x1a485923u));
    RX72M_RX72N_RX66N_func314(580+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0xf323dd7bu), change_endian_long(0xcfef22d3u), change_endian_long(0xdf83c49eu), change_endian_long(0xdd0bda00u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x6d0daa6fu);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x37b5ca78u), change_endian_long(0x3ba50f35u), change_endian_long(0x9e794cfau), change_endian_long(0xaa3174a4u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX72M_RX72N_RX66N_func319(616);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x56c91dc7u), change_endian_long(0xb8bac573u), change_endian_long(0xcc82e087u), change_endian_long(0xd818e6f1u));
    RX72M_RX72N_RX66N_func314(616+32);
    TSIP.REG_ECH.WORD = 0x0000b7a0u;
    TSIP.REG_ECH.WORD = 0x000000f3u;
    RX72M_RX72N_RX66N_func101(change_endian_long(0xe920d1a7u), change_endian_long(0xafaf816fu), change_endian_long(0x2719eb6du), change_endian_long(0x76e6079du));
    RX72M_RX72N_RX66N_func318();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x7dab3d5eu), change_endian_long(0xf9103b3du), change_endian_long(0x69e714abu), change_endian_long(0xb0162490u));
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x84992ba5u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x1697c472u), change_endian_long(0xe48fd090u), change_endian_long(0x2ddd2c9fu), change_endian_long(0xb83837e1u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX72M_RX72N_RX66N_func319(0);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x6a7f3c52u), change_endian_long(0x4b2d1ebeu), change_endian_long(0x4121a015u), change_endian_long(0x8c843a2cu));
    RX72M_RX72N_RX66N_func314(0+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x35e6e5c1u), change_endian_long(0x5b95943au), change_endian_long(0x99669498u), change_endian_long(0xda72ca2bu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x064417e5u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0x59b845d4u), change_endian_long(0xcd6381e3u), change_endian_long(0xed4ca4c3u), change_endian_long(0x4329fa2au));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    RX72M_RX72N_RX66N_func319(36);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x24a4c637u), change_endian_long(0xbf7ee0f7u), change_endian_long(0x7841e2d4u), change_endian_long(0x9e9105a2u));
    RX72M_RX72N_RX66N_func314(36+32);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x72325b29u), change_endian_long(0xa881840bu), change_endian_long(0x82d86151u), change_endian_long(0x6ac0b97cu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x49880ba2u);
    RX72M_RX72N_RX66N_func080();
    RX72M_RX72N_RX66N_func100(change_endian_long(0xb963e6b1u), change_endian_long(0x35059f6fu), change_endian_long(0xc6d24298u), change_endian_long(0xa8a991f9u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX72M_RX72N_RX66N_func319(72);
    RX72M_RX72N_RX66N_func100(change_endian_long(0x42ac6f38u), change_endian_long(0x8e9bd6a0u), change_endian_long(0x8b95f708u), change_endian_long(0x1da34b3au));
    RX72M_RX72N_RX66N_func314(72+32);
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_ECH.WORD = 0x00003bbeu;
    TSIP.REG_ECH.WORD = 0x00007c1du;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX72M/RX72M_func324.prc
 *********************************************************************************************************************/
