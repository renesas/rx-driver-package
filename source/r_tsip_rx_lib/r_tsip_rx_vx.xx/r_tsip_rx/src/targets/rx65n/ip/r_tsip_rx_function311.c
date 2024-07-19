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
* Function Name: RX65NHU_func311
*******************************************************************************************************************/ /**
* @details       RX65NHU func311
* @note          None
*/
void RX65NHU_func311(void)
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
    RX65NHU_func100(change_endian_long(0x8789d392u), change_endian_long(0x57d593b1u), change_endian_long(0xc60c71a1u), change_endian_long(0xcbaede7fu));
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
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x3d3ce6f9u), change_endian_long(0x1a783f41u), change_endian_long(0x4c043982u), change_endian_long(0x81f41ee5u));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func312(680);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5fe685f6u);
    RX65NHU_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    RX65NHU_func312(748);
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
        RX65NHU_func100(change_endian_long(0x8cf96dffu), change_endian_long(0x5f69f0bbu), change_endian_long(0xa1d85aedu), change_endian_long(0x2be12fe2u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func101(change_endian_long(0x15082f4fu), change_endian_long(0xe43224b6u), change_endian_long(0xd9a859acu), change_endian_long(0xb8689905u));
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
            RX65NHU_func101(change_endian_long(0xd0ec8e60u), change_endian_long(0xabce9124u), change_endian_long(0x90d0f155u), change_endian_long(0x1746f696u));
        }
    }
    RX65NHU_func100(change_endian_long(0xede7368bu), change_endian_long(0xe5bf7548u), change_endian_long(0xb387049fu), change_endian_long(0xb50bd48bu));
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
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x8be17797u), change_endian_long(0x50c10ce3u), change_endian_long(0xf07106dfu), change_endian_long(0xa6d3eb82u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func313(136);
    RX65NHU_func100(change_endian_long(0xb2d94648u), change_endian_long(0x520478ccu), change_endian_long(0x4fccde1fu), change_endian_long(0x4b78c02au));
    RX65NHU_func314(136+64);
    TSIP.REG_24H.WORD = 0x000049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    RX65NHU_func100(change_endian_long(0x8f247d9eu), change_endian_long(0x4c33dcf6u), change_endian_long(0xe1f36bacu), change_endian_long(0x93c25c85u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x05e30fedu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x92521cedu), change_endian_long(0x19d113e1u), change_endian_long(0x990af265u), change_endian_long(0xb1ad3b64u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func313(204);
    RX65NHU_func100(change_endian_long(0x4835b297u), change_endian_long(0x66043d9bu), change_endian_long(0x2c3bb5d4u), change_endian_long(0x7dbfe97du));
    RX65NHU_func314(204+64);
    TSIP.REG_24H.WORD = 0x000049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_ECH.WORD = 0x00000a52u;
    for (iLoop = 0; iLoop < 256; iLoop = iLoop + 1)
    {
        RX65NHU_func100(change_endian_long(0x186cb01fu), change_endian_long(0xf025a12eu), change_endian_long(0xcd2a5596u), change_endian_long(0x2926352au));
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
        RX65NHU_func080();
        TSIP.REG_00H.WORD = 0x00013203u;
        TSIP.REG_2CH.WORD = 0x00000014u;
        RX65NHU_func312(680);
        TSIP.REG_ECH.WORD = 0x00002e40u;
        RX65NHU_func101(change_endian_long(0xe5c7ae6bu), change_endian_long(0x2f6710e9u), change_endian_long(0x73443658u), change_endian_long(0x07a6d24cu));
    }
    RX65NHU_func100(change_endian_long(0x6d53b975u), change_endian_long(0xea2357d3u), change_endian_long(0x5209c935u), change_endian_long(0x57430ba4u));
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
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x9226dc5au), change_endian_long(0x11009d42u), change_endian_long(0x0a367312u), change_endian_long(0xe1610fdau));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX65NHU_func312(680);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x2315ef48u), change_endian_long(0x7ea1f383u), change_endian_long(0x3809f082u), change_endian_long(0xf05a49c8u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func313(408);
    RX65NHU_func100(change_endian_long(0x9451c331u), change_endian_long(0xbb612135u), change_endian_long(0x8c098deau), change_endian_long(0x3002c099u));
    RX65NHU_func314(408+64);
    RX65NHU_func100(change_endian_long(0x9c05dba4u), change_endian_long(0xc126ae31u), change_endian_long(0xf0a752ceu), change_endian_long(0xf673e597u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xae62ddfbu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x58192747u), change_endian_long(0x2fcfb6c5u), change_endian_long(0x7b002ae2u), change_endian_long(0xb455ebc4u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func313(544);
    RX65NHU_func100(change_endian_long(0x9fbf351eu), change_endian_long(0x7674905du), change_endian_long(0x20c3da7eu), change_endian_long(0xbc88ea35u));
    RX65NHU_func314(544+64);
    TSIP.REG_24H.WORD = 0x000049c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    RX65NHU_func100(change_endian_long(0x7810f74eu), change_endian_long(0x8f3468e3u), change_endian_long(0x804803e0u), change_endian_long(0xfded333eu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x85ce71c7u), change_endian_long(0x01887a6bu), change_endian_long(0x993b869eu), change_endian_long(0xb13455e6u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func313(476);
    RX65NHU_func100(change_endian_long(0x7fb2b549u), change_endian_long(0x46391839u), change_endian_long(0x23eb83b8u), change_endian_long(0x28476342u));
    RX65NHU_func314(476+64);
    RX65NHU_func100(change_endian_long(0x28d5d2f0u), change_endian_long(0xaa97ef2cu), change_endian_long(0xb3313acbu), change_endian_long(0xb9cfa8aeu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xea6a1c3du);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x6a129ce9u), change_endian_long(0xb1bf6e5au), change_endian_long(0x2e067f0eu), change_endian_long(0x12c644a9u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func313(612);
    RX65NHU_func100(change_endian_long(0xaf582798u), change_endian_long(0xef13515du), change_endian_long(0x16383cb3u), change_endian_long(0xb276f73au));
    RX65NHU_func314(612+64);
    TSIP.REG_ECH.WORD = 0x00000a52u;
    for (iLoop = 0; iLoop < 4; iLoop = iLoop + 1)
    {
        TSIP.REG_ECH.WORD = 0x0000b400u;
        TSIP.REG_ECH.WORD = 0x00000085u;
        RX65NHU_func101(change_endian_long(0xe73049e9u), change_endian_long(0x2e3c63cbu), change_endian_long(0x606ae006u), change_endian_long(0xcee11915u));
        RX65NHU_func309();
        RX65NHU_func100(change_endian_long(0x431d1fb1u), change_endian_long(0xc4decdcdu), change_endian_long(0xb91cde89u), change_endian_long(0x761f17bcu));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
        RX65NHU_func080();
        RX65NHU_func100(change_endian_long(0x26d7584du), change_endian_long(0xccff544au), change_endian_long(0x123a5653u), change_endian_long(0x1a43e611u));
        TSIP.REG_00H.WORD = 0x00012303u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        RX65NHU_func313(476);
        RX65NHU_func100(change_endian_long(0xf30d97ccu), change_endian_long(0xd17a3099u), change_endian_long(0x24c3648eu), change_endian_long(0x93c767fau));
        RX65NHU_func314(476+64);
        RX65NHU_func100(change_endian_long(0x10dcb3ebu), change_endian_long(0xfa36215bu), change_endian_long(0x563e8b32u), change_endian_long(0xdaace9b8u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xea6a1c3du);
        RX65NHU_func080();
        RX65NHU_func100(change_endian_long(0x9a634a9au), change_endian_long(0x9d774adcu), change_endian_long(0x17c6ff02u), change_endian_long(0xa247a8f6u));
        TSIP.REG_00H.WORD = 0x00012303u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        RX65NHU_func313(612);
        RX65NHU_func100(change_endian_long(0x31962f37u), change_endian_long(0x0eb8a3f0u), change_endian_long(0xf7bc0be9u), change_endian_long(0x7a31fbc7u));
        RX65NHU_func314(612+64);
        RX65NHU_func100(change_endian_long(0x4659c27cu), change_endian_long(0x51406332u), change_endian_long(0xc484dec7u), change_endian_long(0x9650fdc9u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
        RX65NHU_func080();
        RX65NHU_func100(change_endian_long(0xac2a2454u), change_endian_long(0x6864f857u), change_endian_long(0xf05bfdb8u), change_endian_long(0x31318641u));
        TSIP.REG_00H.WORD = 0x00012303u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        RX65NHU_func313(408);
        RX65NHU_func100(change_endian_long(0x694fd6fdu), change_endian_long(0x1c342e7du), change_endian_long(0x22ad0921u), change_endian_long(0x6d607efeu));
        RX65NHU_func314(408+64);
        RX65NHU_func100(change_endian_long(0xd3fb1950u), change_endian_long(0x4b049696u), change_endian_long(0x0a0bbeb6u), change_endian_long(0x3054d051u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xae62ddfbu);
        RX65NHU_func080();
        RX65NHU_func100(change_endian_long(0x0634d5d0u), change_endian_long(0xff61495eu), change_endian_long(0xc98c21e6u), change_endian_long(0xb1fb9116u));
        TSIP.REG_00H.WORD = 0x00012303u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        RX65NHU_func313(544);
        RX65NHU_func100(change_endian_long(0xdaa94f1fu), change_endian_long(0xfa88fe5fu), change_endian_long(0x7e54c407u), change_endian_long(0x3c266be4u));
        RX65NHU_func314(544+64);
        TSIP.REG_ECH.WORD = 0x00002e40u;
        RX65NHU_func101(change_endian_long(0x42512e2fu), change_endian_long(0x357a0f56u), change_endian_long(0xdbba8ec4u), change_endian_long(0x8d5ad547u));
    }
    TSIP.REG_ECH.WORD = 0x38008a40u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    RX65NHU_func100(change_endian_long(0x399cb917u), change_endian_long(0x473fab64u), change_endian_long(0x18872e95u), change_endian_long(0xad7a5572u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc73a412cu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x346e4da9u), change_endian_long(0xecc71e4fu), change_endian_long(0x548ae6dfu), change_endian_long(0x061aa17eu));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX65NHU_func313(476);
    RX65NHU_func100(change_endian_long(0x58199a25u), change_endian_long(0x5f86dce6u), change_endian_long(0x76cae712u), change_endian_long(0xff9bb703u));
    RX65NHU_func314(476+64);
    RX65NHU_func100(change_endian_long(0xdac0a281u), change_endian_long(0xe046e54eu), change_endian_long(0x79de5528u), change_endian_long(0x4b27b3dbu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xa9437d20u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x9a0ffd6bu), change_endian_long(0x7ee45db5u), change_endian_long(0x7b01ae8du), change_endian_long(0xbc2ac40du));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func313(408);
    RX65NHU_func100(change_endian_long(0x4c1b6d7au), change_endian_long(0xf9865da6u), change_endian_long(0xea7a2128u), change_endian_long(0x9526155fu));
    RX65NHU_func314(408+64);
    RX65NHU_func100(change_endian_long(0xbde83e18u), change_endian_long(0xb3679799u), change_endian_long(0xcf1024a8u), change_endian_long(0x747b5f03u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5dd049feu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0x706ab944u), change_endian_long(0xd5056fe3u), change_endian_long(0xd45a4eceu), change_endian_long(0xc254b8deu));
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
    RX65NHU_func100(change_endian_long(0x42673df5u), change_endian_long(0x1b3a95b7u), change_endian_long(0x165f7343u), change_endian_long(0x109690e7u));
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    RX65NHU_func312(0);
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xea6a1c3du);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xc7316442u), change_endian_long(0x81c380eau), change_endian_long(0x8ac7b868u), change_endian_long(0xe71fae58u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX65NHU_func313(612);
    RX65NHU_func100(change_endian_long(0x45808e4eu), change_endian_long(0x7fe86080u), change_endian_long(0x4f616ce2u), change_endian_long(0x03c81ff9u));
    RX65NHU_func314(612+64);
    RX65NHU_func100(change_endian_long(0xf88d1d70u), change_endian_long(0x9fbcbe30u), change_endian_long(0x92f6a567u), change_endian_long(0x209abc9bu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xae62ddfbu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xd659d5dfu), change_endian_long(0xc0e4d689u), change_endian_long(0xd59ac975u), change_endian_long(0x6d72cc98u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func313(544);
    RX65NHU_func100(change_endian_long(0xefb36480u), change_endian_long(0x8795aab1u), change_endian_long(0xa40b3fc9u), change_endian_long(0x28730afbu));
    RX65NHU_func314(544+64);
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000086u;
    RX65NHU_func101(change_endian_long(0x472dfb7cu), change_endian_long(0xe930b3a4u), change_endian_long(0xf1820d1du), change_endian_long(0xbbf83bcau));
    RX65NHU_func309();
    RX65NHU_func100(change_endian_long(0xecf5fcc8u), change_endian_long(0x2cf7db9eu), change_endian_long(0x4873435cu), change_endian_long(0x670dddc3u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x5dd049feu);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xda65d5dbu), change_endian_long(0x4b9eec24u), change_endian_long(0x3566092au), change_endian_long(0xf6418f28u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX65NHU_func313(68);
    RX65NHU_func100(change_endian_long(0x83daaf5eu), change_endian_long(0xafc85e1cu), change_endian_long(0xd3bdf692u), change_endian_long(0x102b7ef2u));
    RX65NHU_func314(68+64);
    RX65NHU_func100(change_endian_long(0xab78458eu), change_endian_long(0x3f8be13fu), change_endian_long(0x3f7921f0u), change_endian_long(0x6fc37be0u));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x945d2158u);
    RX65NHU_func080();
    RX65NHU_func100(change_endian_long(0xa1054989u), change_endian_long(0x380e187bu), change_endian_long(0x8910f164u), change_endian_long(0x646568c6u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX65NHU_func313(0);
    RX65NHU_func100(change_endian_long(0x005e15b6u), change_endian_long(0x646abaa8u), change_endian_long(0x62981901u), change_endian_long(0x3541afceu));
    RX65NHU_func314(0+64);
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000002cu;
    TSIP.REG_ECH.WORD = 0x0000381eu;
    TSIP.REG_ECH.WORD = 0x00007c00u;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_func311_r1.prc
 *********************************************************************************************************************/
