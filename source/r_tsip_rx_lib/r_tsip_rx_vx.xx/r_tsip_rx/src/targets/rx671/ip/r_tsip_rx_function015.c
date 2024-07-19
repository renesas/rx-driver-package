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
* Function Name: RX671_func015
*******************************************************************************************************************/ /**
* @details       RX671 func015
* @note          None
*/
void RX671_func015(void)
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
    TSIP.REG_28H.WORD = 0x00bf0001u;
    RX671_func100(change_endian_long(0x40c5989au), change_endian_long(0x10b08e45u), change_endian_long(0xe1b47f1eu), change_endian_long(0x6e583f2eu));
    TSIP.REG_18H.WORD = 0x00000004u;
    TSIP.REG_34H.WORD = 0x00000000u;
    TSIP.REG_38H.WORD = 0x00000338u;
    TSIP.REG_E0H.WORD = 0x81010020u;
    TSIP.REG_00H.WORD = 0x00003807u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_18H.BIT.B10)
    {
        /* waiting */
    }
    TSIP.REG_18H.WORD = 0x00000000u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B20)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00000e00u;
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_00H.WORD = 0x00003523u;
    TSIP.REG_74H.WORD = 0x00000008u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_ECH.WORD = 0x00000821u;
    TSIP.REG_E0H.WORD = 0x80a00001u;
    TSIP.REG_00H.WORD = 0x00008383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_ECH.WORD = 0x0000b420u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_E0H.WORD = 0x80980001u;
    TSIP.REG_00H.WORD = 0x0000835fu;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_00H.WORD = 0x0800830fu;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_00H.WORD = 0x08008007u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_ECH.WORD = 0x0000b420u;
    TSIP.REG_ECH.WORD = 0x000000e0u;
    TSIP.REG_E0H.WORD = 0x80880001u;
    TSIP.REG_00H.WORD = 0x08008383u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_E0H.WORD = 0x80010020u;
    TSIP.REG_00H.WORD = 0x0800800fu;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_00H.WORD = 0x08008307u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_ECH.WORD = 0x3800a820u;
    TSIP.REG_ECH.WORD = 0x000000bcu;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    TSIP.REG_ECH.WORD = 0x00000821u;
    TSIP.REG_ECH.WORD = 0x00003861u;
    TSIP.REG_ECH.WORD = 0x38008c60u;
    TSIP.REG_ECH.WORD = 0x80000000u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_ECH.WORD = 0x0000b420u;
    TSIP.REG_ECH.WORD = 0x000000e0u;
    TSIP.REG_E0H.WORD = 0x81880001u;
    TSIP.REG_00H.WORD = 0x00003823u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_ECH.WORD = 0x00000863u;
    TSIP.REG_ECH.WORD = 0x00000884u;
    for (iLoop = 0; iLoop < 7; iLoop = iLoop + 1)
    {
        TSIP.REG_7CH.WORD = 0x00000011u;
        TSIP.REG_104H.WORD = 0x00000754u;
        TSIP.REG_74H.WORD = 0x00000004u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x6a09e667u);
        TSIP.REG_100H.WORD = change_endian_long(0xbb67ae85u);
        TSIP.REG_100H.WORD = change_endian_long(0x3c6ef372u);
        TSIP.REG_100H.WORD = change_endian_long(0xa54ff53au);
        TSIP.REG_100H.WORD = change_endian_long(0x510e527fu);
        TSIP.REG_100H.WORD = change_endian_long(0x9b05688cu);
        TSIP.REG_100H.WORD = change_endian_long(0x1f83d9abu);
        TSIP.REG_100H.WORD = change_endian_long(0x5be0cd19u);
        TSIP.REG_00H.WORD = 0x00005323u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        TSIP.REG_74H.WORD = 0x00000002u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_E0H.WORD = 0x81010080u;
        TSIP.REG_00H.WORD = 0x00005807u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000054u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x80000000u);
        TSIP.REG_F8H.WORD = 0x00000040u;
        TSIP.REG_104H.WORD = 0x00000154u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000120u);
        /* WAIT_LOOP */
        while (0u != TSIP.REG_74H.BIT.B18)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001600u;
        TSIP.REG_74H.WORD = 0x00000000u;
        TSIP.REG_74H.WORD = 0x00000008u;
        for (jLoop = 0; jLoop < 2; jLoop = jLoop + 1)
        {
            TSIP.REG_C4H.WORD = 0x00040805u;
            TSIP.REG_00H.WORD = 0x00002513u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_C4H.WORD = 0x00000c85u;
            TSIP.REG_E0H.WORD = 0x81840003u;
            TSIP.REG_00H.WORD = 0x00002813u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_E0H.WORD = 0x80840003u;
            TSIP.REG_00H.WORD = 0x00008213u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x0000a460u;
            TSIP.REG_ECH.WORD = 0x00000010u;
        }
        TSIP.REG_ECH.WORD = 0x00002c80u;
    }
    TSIP.REG_ECH.WORD = 0x00000821u;
    TSIP.REG_ECH.WORD = 0x00003861u;
    TSIP.REG_ECH.WORD = 0x00008c60u;
    TSIP.REG_ECH.WORD = 0x7fffffffu;
    TSIP.REG_ECH.WORD = 0x00003c61u;
    TSIP.REG_ECH.WORD = 0x00000821u;
    TSIP.REG_ECH.WORD = 0x0000b460u;
    TSIP.REG_ECH.WORD = 0x000000deu;
    TSIP.REG_ECH.WORD = 0x00002862u;
    for (iLoop = 0; iLoop < 223; iLoop = iLoop + 1)
    {
        TSIP.REG_ECH.WORD = 0x02003881u;
        TSIP.REG_ECH.WORD = 0x30002823u;
        TSIP.REG_ECH.WORD = 0x00030020u;
        TSIP.REG_ECH.WORD = 0x3800a880u;
        TSIP.REG_ECH.WORD = 0x00000001u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_ECH.WORD = 0x00000060u;
        TSIP.REG_ECH.WORD = 0x30002823u;
        TSIP.REG_ECH.WORD = 0x00000020u;
        TSIP.REG_ECH.WORD = 0x3800a880u;
        TSIP.REG_ECH.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_ECH.WORD = 0x00000060u;
        TSIP.REG_ECH.WORD = 0x38000884u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_ECH.WORD = 0x00000080u;
        TSIP.REG_ECH.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00A60000u;
        TSIP.REG_ECH.WORD = 0x00002c20u;
    }
    TSIP.REG_7CH.WORD = 0x00000011u;
    TSIP.REG_104H.WORD = 0x00000754u;
    TSIP.REG_74H.WORD = 0x00000004u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x6a09e667u);
    TSIP.REG_100H.WORD = change_endian_long(0xbb67ae85u);
    TSIP.REG_100H.WORD = change_endian_long(0x3c6ef372u);
    TSIP.REG_100H.WORD = change_endian_long(0xa54ff53au);
    TSIP.REG_100H.WORD = change_endian_long(0x510e527fu);
    TSIP.REG_100H.WORD = change_endian_long(0x9b05688cu);
    TSIP.REG_100H.WORD = change_endian_long(0x1f83d9abu);
    TSIP.REG_100H.WORD = change_endian_long(0x5be0cd19u);
    TSIP.REG_00H.WORD = 0x0000500bu;
    TSIP.REG_74H.WORD = 0x00000002u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_00H.WORD = 0x00005323u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_ECH.WORD = 0x00003422u;
    TSIP.REG_ECH.WORD = 0x0000b460u;
    TSIP.REG_ECH.WORD = 0x000000dfu;
    TSIP.REG_ECH.WORD = 0x00002862u;
    RX671_func100(change_endian_long(0x5d38dc48u), change_endian_long(0xf28a4ec1u), change_endian_long(0xb39252e6u), change_endian_long(0xe106938au));
    TSIP.REG_E0H.WORD = 0x81010020u;
    TSIP.REG_04H.WORD = 0x00000707u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
    for (iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 1)
    {
        TSIP.REG_ECH.WORD = 0x00003020u;
        TSIP.REG_ECH.WORD = 0x02003883u;
        TSIP.REG_E0H.WORD = 0x81010080u;
        TSIP.REG_00H.WORD = 0x0800080fu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_00H.WORD = 0x08005807u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_ECH.WORD = 0x00002c60u;
        RX671_func101(change_endian_long(0xae0ef081u), change_endian_long(0x1ee07ccdu), change_endian_long(0x31170592u), change_endian_long(0xc2d32d00u));
    }
    TSIP.REG_ECH.WORD = 0x00007c01u;
    TSIP.REG_1CH.WORD = 0x00602000u;
    TSIP.REG_ECH.WORD = 0x00003462u;
    TSIP.REG_ECH.WORD = 0x00008c60u;
    TSIP.REG_ECH.WORD = 0x00000003u;
    TSIP.REG_ECH.WORD = 0x0000b420u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_ECH.WORD = 0x00002823u;
    TSIP.REG_ECH.WORD = 0x0000b460u;
    TSIP.REG_ECH.WORD = 0x80000000u;
    RX671_func100(change_endian_long(0x57b22ba7u), change_endian_long(0xf0685adbu), change_endian_long(0x1f4b4e31u), change_endian_long(0xe11f97efu));
    TSIP.REG_E0H.WORD = 0x81010020u;
    TSIP.REG_04H.WORD = 0x00000707u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
    for (iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 1)
    {
        TSIP.REG_ECH.WORD = 0x00003020u;
        TSIP.REG_E0H.WORD = 0x81010060u;
        TSIP.REG_00H.WORD = 0x08005807u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_00H.WORD = 0x0800080fu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_ECH.WORD = 0x00086c63u;
        RX671_func101(change_endian_long(0x82d8f72au), change_endian_long(0x606dbc2bu), change_endian_long(0xc67cebe7u), change_endian_long(0xf2542dd8u));
    }
    TSIP.REG_ECH.WORD = 0x00007c01u;
    TSIP.REG_1CH.WORD = 0x00602000u;
    TSIP.REG_F8H.WORD = 0x00000040u;
    TSIP.REG_ECH.WORD = 0x00000863u;
    TSIP.REG_ECH.WORD = 0x00003482u;
    TSIP.REG_ECH.WORD = 0x0000a480u;
    TSIP.REG_ECH.WORD = 0x00000028u;
    TSIP.REG_ECH.WORD = 0x01836c64u;
    TSIP.REG_ECH.WORD = 0x00036c84u;
    TSIP.REG_E0H.WORD = 0x81020060u;
    TSIP.REG_00H.WORD = 0x0000580bu;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_74H.BIT.B18)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001600u;
    TSIP.REG_74H.WORD = 0x00000000u;
    TSIP.REG_00H.WORD = 0x00003523u;
    TSIP.REG_74H.WORD = 0x00000008u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_24H.WORD = 0x000009c0u;
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
    TSIP.REG_1CH.WORD = 0x00210000u;
    TSIP.REG_24H.WORD = 0x000011c0u;
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
    TSIP.REG_1CH.WORD = 0x00210000u;
    TSIP.REG_ECH.WORD = 0x00007c00u;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX671/RX671_func015.prc
 *********************************************************************************************************************/
