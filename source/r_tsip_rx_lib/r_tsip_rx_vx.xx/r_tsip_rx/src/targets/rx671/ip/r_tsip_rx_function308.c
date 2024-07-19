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
* Function Name: RX671_func308
*******************************************************************************************************************/ /**
* @details       RX671 func308
* @note          None
*/
void RX671_func308(void)
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
    RX671_func100(change_endian_long(0x664938fau), change_endian_long(0x13669a20u), change_endian_long(0x61fc124du), change_endian_long(0xc797c2b1u));
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000024u;
    TSIP.REG_ECH.WORD = 0x00003c1eu;
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_ECH.WORD = 0x00000b5au;
    TSIP.REG_ECH.WORD = 0x0000d77cu;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x3800db7au;
    TSIP.REG_ECH.WORD = 0x00020020u;
    TSIP.REG_ECH.WORD = 0x0000d37cu;
    TSIP.REG_ECH.WORD = 0x00003bbeu;
    TSIP.REG_ECH.WORD = 0x00002b5du;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x00003b5eu;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x488e4334u);
    RX671_func080();
    RX671_func100(change_endian_long(0x2bb63504u), change_endian_long(0xfa39ba41u), change_endian_long(0x0db6c82cu), change_endian_long(0x32386ae7u));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    RX671_func313(1020);
    RX671_func100(change_endian_long(0xddcf13c9u), change_endian_long(0xfcc7c0d2u), change_endian_long(0x6c67dd67u), change_endian_long(0x2526d245u));
    RX671_func314(1020+64);
    RX671_func100(change_endian_long(0x56f69c71u), change_endian_long(0x18f9ed2bu), change_endian_long(0x83ae0d0bu), change_endian_long(0xb0d28098u));
    TSIP.REG_ECH.WORD = 0x00000b39u;
    TSIP.REG_ECH.WORD = 0x0000d77du;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_ECH.WORD = 0x3800db7bu;
    TSIP.REG_ECH.WORD = 0x00020020u;
    TSIP.REG_ECH.WORD = 0x0000d37du;
    TSIP.REG_ECH.WORD = 0x00003bbeu;
    TSIP.REG_ECH.WORD = 0x00002b3du;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x00003b3eu;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x3f46c431u);
    RX671_func080();
    RX671_func100(change_endian_long(0xbd48609cu), change_endian_long(0xa37ac52bu), change_endian_long(0xdcd6b1abu), change_endian_long(0x274d5a2eu));
    TSIP.REG_00H.WORD = 0x00012303u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    RX671_func313(1088);
    RX671_func100(change_endian_long(0xd5a6e486u), change_endian_long(0x2478e74au), change_endian_long(0x19c49064u), change_endian_long(0x19a1bddbu));
    RX671_func314(1088+64);
    RX671_func100(change_endian_long(0x1d267510u), change_endian_long(0x5f706a8bu), change_endian_long(0x2245a0dfu), change_endian_long(0x523d0a27u));
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x0000080au;
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
    TSIP.REG_24H.WORD = 0x0000c0c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_E0H.WORD = 0x81010320u;
    TSIP.REG_00H.WORD = 0x00003807u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_E0H.WORD = 0x81010340u;
    TSIP.REG_00H.WORD = 0x00003807u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x0000080au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_E0H.WORD = 0x80010300u;
    TSIP.REG_00H.WORD = 0x00008307u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_ECH.WORD = 0x0000d77eu;
    TSIP.REG_ECH.WORD = 0x00000bdeu;
    TSIP.REG_ECH.WORD = 0x3000db7cu;
    TSIP.REG_ECH.WORD = 0x00020020u;
    TSIP.REG_ECH.WORD = 0x00002fc0u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x3000db7du;
    TSIP.REG_ECH.WORD = 0x00020020u;
    TSIP.REG_ECH.WORD = 0x00002fc0u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x3000dbc0u;
    TSIP.REG_ECH.WORD = 0x00030020u;
    TSIP.REG_ECH.WORD = 0x0000d77eu;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d37eu;
    TSIP.REG_ECH.WORD = 0x00000080u;
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
    TSIP.REG_34H.WORD = 0x00000400u;
    TSIP.REG_24H.WORD = 0x800080c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_24H.WORD = 0x000001c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000058d0u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x3f46c431u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func312(1088);
    TSIP.REG_ECH.WORD = 0x3000db7cu;
    TSIP.REG_ECH.WORD = 0x00030020u;
    TSIP.REG_ECH.WORD = 0x0000d76bu;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d36bu;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x000037dau;
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000097u;
    RX671_func101(change_endian_long(0xad6bb22cu), change_endian_long(0x2c7b29d7u), change_endian_long(0xdda94838u), change_endian_long(0x4954e18cu));
    RX671_func310();
    RX671_func100(change_endian_long(0x4ab64ff2u), change_endian_long(0xe25fb9e6u), change_endian_long(0xaae33c4bu), change_endian_long(0x4c0c462eu));
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x488e4334u);
    RX671_func080();
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    RX671_func312(1020);
    TSIP.REG_ECH.WORD = 0x3000db7du;
    TSIP.REG_ECH.WORD = 0x00030020u;
    TSIP.REG_ECH.WORD = 0x0000d76bu;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d36bu;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x000037d9u;
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000098u;
    RX671_func101(change_endian_long(0x32b13984u), change_endian_long(0x0cbe9f0fu), change_endian_long(0x2030be59u), change_endian_long(0x3f1f41ecu));
    RX671_func310();
    TSIP.REG_ECH.WORD = 0x3800db7eu;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(change_endian_long(0x11c85305u), change_endian_long(0x7c5defbeu), change_endian_long(0x3df18b46u), change_endian_long(0x143656f1u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_E0H.WORD = 0x81010300u;
        TSIP.REG_00H.WORD = 0x00003807u;
        TSIP.REG_2CH.WORD = 0x00000090u;
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
        TSIP.REG_34H.WORD = 0x00000004u;
        TSIP.REG_24H.WORD = 0x80001191u;
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
        RX671_func101(change_endian_long(0xf2952edbu), change_endian_long(0xc89cd595u), change_endian_long(0xa937efb1u), change_endian_long(0xa61983a2u));
    }
    else
    {
        TSIP.REG_28H.WORD = 0x00800001u;
        TSIP.REG_E0H.WORD = 0x81010300u;
        TSIP.REG_00H.WORD = 0x00003807u;
        TSIP.REG_2CH.WORD = 0x00000090u;
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
        TSIP.REG_34H.WORD = 0x00000004u;
        TSIP.REG_24H.WORD = 0x80001181u;
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
        RX671_func101(change_endian_long(0xa629e2ceu), change_endian_long(0xaba5b274u), change_endian_long(0xfe78c37bu), change_endian_long(0x4ad5bd85u));
    }
    TSIP.REG_ECH.WORD = 0x00000bdeu;
    TSIP.REG_ECH.WORD = 0x02816fdeu;
    TSIP.REG_ECH.WORD = 0x30008bc0u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00020020u;
    TSIP.REG_ECH.WORD = 0x0000d372u;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000d772u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_28H.WORD = 0x00800001u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x00000018u;
    TSIP.REG_E0H.WORD = 0x8081001eu;
    TSIP.REG_00H.WORD = 0x00008307u;
    TSIP.REG_2CH.WORD = 0x00000020u;
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
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x0000001cu;
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
    TSIP.REG_ECH.WORD = 0x0000d768u;
    TSIP.REG_24H.WORD = 0x000088d0u;
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
    TSIP.REG_ECH.WORD = 0x00000024u;
    TSIP.REG_ECH.WORD = 0x0000381eu;
    TSIP.REG_ECH.WORD = 0x00007c00u;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX671/RX671_func308.prc
 *********************************************************************************************************************/
