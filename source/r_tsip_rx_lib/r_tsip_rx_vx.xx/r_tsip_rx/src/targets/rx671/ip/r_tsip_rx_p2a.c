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

#if TSIP_PRV_USE_RSA_1024 == 1
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
* Function Name: R_TSIP_GenerateRsa1024RandomKeyIndexSub
*******************************************************************************************************************/ /**
* @details       RX671 RSA-1024 Key Generation
* @param[out]    OutData_PubKeyIndex
* @param[out]    OutData_PrivKeyIndex
* @retval        TSIP_SUCCESS
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateRsa1024RandomKeyIndexSub(uint32_t MAX_CNT, uint32_t *OutData_PubKeyIndex, uint32_t *OutData_PrivKeyIndex)
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
    #if TSIP_MULTI_THREADING == 1
    TSIP_MULTI_THREADING_LOCK_FUNCTION();
    #endif /* TSIP_MULTI_THREADING == 1 */
    if (0x0u != (TSIP.REG_1BCH.WORD & 0x1fu))
    {
        #if TSIP_MULTI_THREADING == 1
        TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
        #endif /* TSIP_MULTI_THREADING == 1 */
        return TSIP_ERR_RESOURCE_CONFLICT;
    }
    TSIP.REG_84H.WORD = 0x00002a01u;
    TSIP.REG_108H.WORD = 0x00000000u;
    TSIP.REG_C4H.WORD = 0x200e1a0du;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_RAM[16+0 + 0];
    TSIP.REG_100H.WORD = S_RAM[16+0 + 1];
    TSIP.REG_100H.WORD = S_RAM[16+0 + 2];
    TSIP.REG_100H.WORD = S_RAM[16+0 + 3];
    RX671_func100(change_endian_long(0x79d54269u), change_endian_long(0x338e5ca6u), change_endian_long(0x909947c7u), change_endian_long(0xa69fdc4au));
    RX671_func103();
    RX671_func100(change_endian_long(0xecdbc262u), change_endian_long(0x5cfcdd06u), change_endian_long(0xaa2050c6u), change_endian_long(0xce593440u));
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x01080c84u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_ECH.WORD = 0x00000a31u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    for(kLoop = 0; kLoop < MAX_CNT; kLoop = kLoop + 1)
    {
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_00H.WORD = 0x00003043u;
        TSIP.REG_2CH.WORD = 0x00000012u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00002f57u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00030005u);
        TSIP.REG_100H.WORD = change_endian_long(0x0007000bu);
        TSIP.REG_100H.WORD = change_endian_long(0x000d0011u);
        TSIP.REG_100H.WORD = change_endian_long(0x00130017u);
        TSIP.REG_100H.WORD = change_endian_long(0x001d001fu);
        TSIP.REG_100H.WORD = change_endian_long(0x00250029u);
        TSIP.REG_100H.WORD = change_endian_long(0x002b002fu);
        TSIP.REG_100H.WORD = change_endian_long(0x003b003du);
        TSIP.REG_100H.WORD = change_endian_long(0x00430047u);
        TSIP.REG_100H.WORD = change_endian_long(0x0049004fu);
        TSIP.REG_100H.WORD = change_endian_long(0x00530059u);
        TSIP.REG_100H.WORD = change_endian_long(0x00610065u);
        TSIP.REG_100H.WORD = change_endian_long(0x0067006bu);
        TSIP.REG_100H.WORD = change_endian_long(0x006d0071u);
        TSIP.REG_100H.WORD = change_endian_long(0x007f0083u);
        TSIP.REG_100H.WORD = change_endian_long(0x0089008bu);
        TSIP.REG_100H.WORD = change_endian_long(0x00950097u);
        TSIP.REG_100H.WORD = change_endian_long(0x009d00a3u);
        TSIP.REG_100H.WORD = change_endian_long(0x00a700adu);
        TSIP.REG_100H.WORD = change_endian_long(0x00b300b5u);
        TSIP.REG_100H.WORD = change_endian_long(0x00bf00c1u);
        TSIP.REG_100H.WORD = change_endian_long(0x00c500c7u);
        TSIP.REG_100H.WORD = change_endian_long(0x00d300dfu);
        TSIP.REG_100H.WORD = change_endian_long(0x00e300e5u);
        TSIP.REG_100H.WORD = change_endian_long(0x00e900efu);
        TSIP.REG_100H.WORD = change_endian_long(0x00f100fbu);
        TSIP.REG_100H.WORD = change_endian_long(0x01010107u);
        TSIP.REG_100H.WORD = change_endian_long(0x010d010fu);
        TSIP.REG_100H.WORD = change_endian_long(0x01150119u);
        TSIP.REG_100H.WORD = change_endian_long(0x011b0125u);
        TSIP.REG_100H.WORD = change_endian_long(0x01330137u);
        TSIP.REG_100H.WORD = change_endian_long(0x0139013du);
        TSIP.REG_100H.WORD = change_endian_long(0x014b0151u);
        TSIP.REG_100H.WORD = change_endian_long(0x015b015du);
        TSIP.REG_100H.WORD = change_endian_long(0x01610167u);
        TSIP.REG_100H.WORD = change_endian_long(0x016f0175u);
        TSIP.REG_100H.WORD = change_endian_long(0x017b017fu);
        TSIP.REG_100H.WORD = change_endian_long(0x0185018du);
        TSIP.REG_100H.WORD = change_endian_long(0x01910199u);
        TSIP.REG_100H.WORD = change_endian_long(0x01a301a5u);
        TSIP.REG_100H.WORD = change_endian_long(0x01af01b1u);
        TSIP.REG_100H.WORD = change_endian_long(0x01b701bbu);
        TSIP.REG_100H.WORD = change_endian_long(0x01c101c9u);
        TSIP.REG_100H.WORD = change_endian_long(0x01cd01cfu);
        TSIP.REG_100H.WORD = change_endian_long(0x01d301dfu);
        TSIP.REG_100H.WORD = change_endian_long(0x01e701ebu);
        TSIP.REG_100H.WORD = change_endian_long(0x01f301f7u);
        TSIP.REG_100H.WORD = change_endian_long(0x01fd0000u);
        TSIP.REG_ECH.WORD = 0x000008c6u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x00000343u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_E0H.WORD = 0x80b00006u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x000083c3u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x008f0001u;
        RX671_func100(change_endian_long(0xfaeb3eefu), change_endian_long(0xf9f8fbceu), change_endian_long(0x16c41092u), change_endian_long(0xd8454f64u));
        RX671_func103();
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x01000c84u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_E0H.WORD = 0x80040000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x00008213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_ECH.WORD = 0x0000d01fu;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x81040000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x00003813u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        for(iLoop = 0; iLoop < 8; iLoop = iLoop + 4)
        {
            RX671_func100(change_endian_long(0x13bff8ddu), change_endian_long(0xe2baf7a2u), change_endian_long(0x7c25a1bfu), change_endian_long(0x42895ebfu));
            RX671_func103();
            TSIP.REG_104H.WORD = 0x00000052u;
            TSIP.REG_C4H.WORD = 0x01000c84u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_00H.WORD = 0x00003213u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX671_func101(change_endian_long(0x75c2e33du), change_endian_long(0x604d4c08u), change_endian_long(0x84cc1fa0u), change_endian_long(0x83abf8f8u));
        }
        RX671_func100(change_endian_long(0x13bff8ddu), change_endian_long(0xe2baf7a2u), change_endian_long(0x7c25a1bfu), change_endian_long(0x42895ebfu));
        RX671_func103();
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x01000c84u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_E0H.WORD = 0x80040000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x00008213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_ECH.WORD = 0x0000d060u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x81040000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x00003813u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_2CH.WORD = 0x00000011u;
        TSIP.REG_104H.WORD = 0x00000f57u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xB51EB851u);
        TSIP.REG_100H.WORD = change_endian_long(0xEB851EB8u);
        TSIP.REG_100H.WORD = change_endian_long(0x51EB851Eu);
        TSIP.REG_100H.WORD = change_endian_long(0xB851EB85u);
        TSIP.REG_100H.WORD = change_endian_long(0x1EB851EBu);
        TSIP.REG_100H.WORD = change_endian_long(0x851EB851u);
        TSIP.REG_100H.WORD = change_endian_long(0xEB851EB8u);
        TSIP.REG_100H.WORD = change_endian_long(0x51EB851Eu);
        TSIP.REG_100H.WORD = change_endian_long(0xB851EB85u);
        TSIP.REG_100H.WORD = change_endian_long(0x1EB851EBu);
        TSIP.REG_100H.WORD = change_endian_long(0x851EB851u);
        TSIP.REG_100H.WORD = change_endian_long(0xEB851EB8u);
        TSIP.REG_100H.WORD = change_endian_long(0x51EB851Eu);
        TSIP.REG_100H.WORD = change_endian_long(0xB851EB85u);
        TSIP.REG_100H.WORD = change_endian_long(0x1EB851EBu);
        TSIP.REG_100H.WORD = change_endian_long(0x851EB80Eu);
        TSIP.REG_24H.WORD = 0x000011c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00001591u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00210000u;
        RX671_func100(change_endian_long(0x1e85ef41u), change_endian_long(0x4c360d38u), change_endian_long(0x7184179cu), change_endian_long(0xf8bfff09u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func101(change_endian_long(0x7b4f901du), change_endian_long(0xb506d329u), change_endian_long(0xd39f4eb8u), change_endian_long(0x223ce2b4u));
            continue;
        }
        else
        {
            RX671_func101(change_endian_long(0xb7f39e57u), change_endian_long(0xecb4df17u), change_endian_long(0x6697155bu), change_endian_long(0xa15e6b36u));
        }
        TSIP.REG_ECH.WORD = 0x38008a20u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00000001u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX671_func100(change_endian_long(0x11e8511bu), change_endian_long(0xcbf58bf6u), change_endian_long(0xce05e4cau), change_endian_long(0xcc5a2201u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_24H.WORD = 0x00001dc0u;
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
            RX671_func100(change_endian_long(0x652a3f84u), change_endian_long(0x5198f1d5u), change_endian_long(0x2455a546u), change_endian_long(0x779a7c18u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_24H.WORD = 0x000011c0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00001d91u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                RX671_func101(change_endian_long(0x859d196fu), change_endian_long(0xdc7cf0f9u), change_endian_long(0xa30d2a71u), change_endian_long(0x4be20114u));
            }
            else
            {
                RX671_func101(change_endian_long(0x0a950db4u), change_endian_long(0x464fc2e1u), change_endian_long(0xa9878f05u), change_endian_long(0x4d6e90aeu));
            }
            TSIP.REG_2CH.WORD = 0x00000011u;
            TSIP.REG_104H.WORD = 0x00000357u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_100H.WORD = change_endian_long(0x10000000u);
            TSIP.REG_00H.WORD = 0x00003033u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_24H.WORD = 0x00001591u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00210000u;
            RX671_func100(change_endian_long(0xa625b004u), change_endian_long(0x1de0f8b7u), change_endian_long(0xacee3f76u), change_endian_long(0xf9fdd4eau));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX671_func101(change_endian_long(0x75390255u), change_endian_long(0x53f407e8u), change_endian_long(0xde35c8abu), change_endian_long(0x4cf9caa6u));
                continue;
            }
            else
            {
                RX671_func101(change_endian_long(0xca475471u), change_endian_long(0x8f80d563u), change_endian_long(0xd2f42169u), change_endian_long(0x5399dfb6u));
            }
        }
        else
        {
            RX671_func101(change_endian_long(0x65b0aab5u), change_endian_long(0x629adf58u), change_endian_long(0x9e489fc3u), change_endian_long(0x4b53c3d7u));
        }
        TSIP.REG_ECH.WORD = 0x000008c6u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x0000094au;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        for(iLoop = 0; iLoop < 96; iLoop = iLoop + 1)
        {
            TSIP.REG_ECH.WORD = 0x01003906u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00002cc0u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00002cc0u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x81010100u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_00H.WORD = 0x0000303fu;
            TSIP.REG_2CH.WORD = 0x00000014u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_00H.WORD = 0x00003807u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_24H.WORD = 0x00004006u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x000009c0u;
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
            RX671_func100(change_endian_long(0xb996276au), change_endian_long(0x955f920du), change_endian_long(0xf455f506u), change_endian_long(0x2b1ebe49u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_ECH.WORD = 0x0000d140u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                RX671_func101(change_endian_long(0x502a8472u), change_endian_long(0x4bc8dd16u), change_endian_long(0xc1146c3du), change_endian_long(0x355298ddu));
                break;
            }
            else
            {
                RX671_func101(change_endian_long(0x8df85b52u), change_endian_long(0xb376a6d3u), change_endian_long(0xa61ba049u), change_endian_long(0xb1f220acu));
            }
        }
        TSIP.REG_ECH.WORD = 0x38008940u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00000001u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX671_func100(change_endian_long(0x9cb89437u), change_endian_long(0x40e43f46u), change_endian_long(0x5b916f67u), change_endian_long(0x02f184b9u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func101(change_endian_long(0x8cacdff6u), change_endian_long(0xa7a37a4bu), change_endian_long(0xee4162a1u), change_endian_long(0x52dbe9cbu));
            continue;
        }
        TSIP.REG_24H.WORD = 0x000011c0u;
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
        TSIP.REG_24H.WORD = 0x000011c0u;
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
        TSIP.REG_24H.WORD = 0x000080c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_00H.WORD = 0x0000303fu;
        TSIP.REG_2CH.WORD = 0x00000014u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000057u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00010001u);
        TSIP.REG_24H.WORD = 0x00004006u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00000dc0u;
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
        TSIP.REG_24H.WORD = 0x000009c0u;
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
        RX671_func100(change_endian_long(0x863b625eu), change_endian_long(0x59128809u), change_endian_long(0x07b015efu), change_endian_long(0x0b64ea76u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func101(change_endian_long(0xe67034d9u), change_endian_long(0xfe2e891du), change_endian_long(0xfc4f921bu), change_endian_long(0x0231f8f8u));
        }
        else
        {
            TSIP.REG_24H.WORD = 0x0000d0d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x000019c0u;
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
            TSIP.REG_24H.WORD = 0x0000a0c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x000084d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_34H.WORD = 0x00000800u;
            TSIP.REG_24H.WORD = 0x8002d008u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x000005c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x00000189u;
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
            RX671_func100(change_endian_long(0x2f20911du), change_endian_long(0x1c93f6fcu), change_endian_long(0x2ab11bacu), change_endian_long(0xdbb8a126u));
            TSIP.REG_18H.WORD = 0x00000004u;
            TSIP.REG_38H.WORD = 0x00004080u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_18H.BIT.B10)
            {
                /* waiting */
            }
            TSIP.REG_18H.WORD = 0x00000000u;
            TSIP.REG_24H.WORD = 0x000005c0u;
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
            TSIP.REG_24H.WORD = 0x000044d0u;
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
            TSIP.REG_24H.WORD = 0x00000991u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00210000u;
            RX671_func100(change_endian_long(0x4ec84cceu), change_endian_long(0xe4311d17u), change_endian_long(0x5db75661u), change_endian_long(0x46960b42u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX671_func101(change_endian_long(0x96408697u), change_endian_long(0x3d7b8e60u), change_endian_long(0xa04e0e73u), change_endian_long(0xe0a51ec2u));
                continue;
            }
            TSIP.REG_24H.WORD = 0x000098d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_ECH.WORD = 0x0000b540u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x000000c0u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x8090000au;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_00H.WORD = 0x00008343u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x0000b560u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000006u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_B0H.WORD = 0x00000300u;
            TSIP.REG_A4H.WORD = 0x42e0873fu;
            TSIP.REG_00H.WORD = 0x00001343u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_104H.WORD = 0x00000051u;
            TSIP.REG_A4H.WORD = 0x00000c84u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_A4H.WORD = 0x400009cdu;
            TSIP.REG_00H.WORD = 0x00001113u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x38008a20u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            RX671_func100(change_endian_long(0x4f52436au), change_endian_long(0x0467db39u), change_endian_long(0x5e022f2au), change_endian_long(0x31795239u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_7CH.WORD = 0x00000021u;
                TSIP.REG_00H.WORD = 0x00005113u;
                TSIP.REG_74H.WORD = 0x00000004u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                RX671_func101(change_endian_long(0xfd15ff7fu), change_endian_long(0x0fd9dd26u), change_endian_long(0x07ded05bu), change_endian_long(0xa2887f27u));
            }
            else
            {
                TSIP.REG_7CH.WORD = 0x00000041u;
                TSIP.REG_00H.WORD = 0x00005113u;
                TSIP.REG_74H.WORD = 0x00000004u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                RX671_func101(change_endian_long(0xb8032c36u), change_endian_long(0x0da3dc25u), change_endian_long(0x9008ea53u), change_endian_long(0x70abfac3u));
            }
            RX671_func100(change_endian_long(0x8841df3bu), change_endian_long(0xd6216886u), change_endian_long(0x6a33754fu), change_endian_long(0x49ff986eu));
            TSIP.REG_24H.WORD = 0x000011c0u;
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
            TSIP.REG_ECH.WORD = 0x00000929u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x0000b4c0u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x000000fcu;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00003906u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00008d00u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0xfffffffeu;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00003d06u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000908u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            for(iLoop = 0; iLoop < 32; iLoop = iLoop + 1)
            {
                TSIP.REG_ECH.WORD = 0x000038e6u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x0000a8c0u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x00000004u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                for(jLoop = 0; jLoop < 32; jLoop = jLoop + 1)
                {
                    TSIP.REG_ECH.WORD = 0x38008900u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00000000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x11816907u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x38008900u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00000000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x10002d20u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x000168e7u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                }
            }
            TSIP.REG_ECH.WORD = 0x00003549u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x0000a540u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000003u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x0002694au;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x81010140u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_04H.WORD = 0x00000606u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
            TSIP.REG_ECH.WORD = 0x000037eau;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_24H.WORD = 0x000011c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            for(iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 1)
            {
                TSIP.REG_24H.WORD = 0x00000185u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000185u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000185u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000185u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_ECH.WORD = 0x000033e0u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                RX671_func101(change_endian_long(0xeb8788c9u), change_endian_long(0xa7938dbdu), change_endian_long(0xe2ccfeb8u), change_endian_long(0x63477d7cu));
            }
            TSIP.REG_ECH.WORD = 0x00007c1fu;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_1CH.WORD = 0x00602000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            RX671_func100(change_endian_long(0x0808b889u), change_endian_long(0xd1f04808u), change_endian_long(0x0f211208u), change_endian_long(0xc0b162d4u));
            TSIP.REG_ECH.WORD = 0x00026d4au;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00002949u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x81010140u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_04H.WORD = 0x00000606u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
            TSIP.REG_ECH.WORD = 0x000037eau;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            for(iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 1)
            {
                TSIP.REG_24H.WORD = 0x2000018du;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_ECH.WORD = 0x000033e0u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                RX671_func101(change_endian_long(0xe07e20f0u), change_endian_long(0x5587b267u), change_endian_long(0xf1119044u), change_endian_long(0x295a01b9u));
            }
            TSIP.REG_ECH.WORD = 0x00007c1fu;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_1CH.WORD = 0x00602000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            RX671_func100(change_endian_long(0x85d75656u), change_endian_long(0x02d96fa0u), change_endian_long(0x985d6227u), change_endian_long(0x8331d5a9u));
            TSIP.REG_24H.WORD = 0x0000a0c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_ECH.WORD = 0x00000a52u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_24H.WORD = 0x000084d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x00029008u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_34H.WORD = 0x00000800u;
            TSIP.REG_24H.WORD = 0x8000c0c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x00006404u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x00006c04u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_E0H.WORD = 0x81010160u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_04H.WORD = 0x00000606u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
            for(iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 1)
            {
                TSIP.REG_2CH.WORD = 0x00000010u;
                for(jLoop = 0; jLoop < 16; jLoop = jLoop + 4)
                {
                    RX671_func100(change_endian_long(0x31fd4ab3u), change_endian_long(0x11c12819u), change_endian_long(0xe8135cc3u), change_endian_long(0xf0e0295cu));
                    RX671_func103();
                    TSIP.REG_104H.WORD = 0x00000052u;
                    TSIP.REG_C4H.WORD = 0x01000c84u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_00H.WORD = 0x00003213u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    RX671_func101(change_endian_long(0xae181b83u), change_endian_long(0x211f2353u), change_endian_long(0x4ad1e50cu), change_endian_long(0xb9dd5469u));
                }
                RX671_func100(change_endian_long(0x74bb2a48u), change_endian_long(0x2bb7cbd7u), change_endian_long(0x6ad32b0bu), change_endian_long(0x5e7a5a86u));
                TSIP.REG_24H.WORD = 0x000019c0u;
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
                TSIP.REG_24H.WORD = 0x00000591u;
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
                TSIP.REG_24H.WORD = 0x00000591u;
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
                TSIP.REG_24H.WORD = 0x000050d0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x000084d0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00029008u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_34H.WORD = 0x00000800u;
                TSIP.REG_24H.WORD = 0x8000c0c1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x000088d0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00009008u;
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
                TSIP.REG_24H.WORD = 0x00000581u;
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
                TSIP.REG_24H.WORD = 0x000019c0u;
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
                TSIP.REG_24H.WORD = 0x00000581u;
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
                TSIP.REG_24H.WORD = 0x00000581u;
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
                TSIP.REG_24H.WORD = 0x000050d0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x000084d0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00029008u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_34H.WORD = 0x00000800u;
                TSIP.REG_24H.WORD = 0x8000c0c1u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x000088d0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x00000dc0u;
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
                TSIP.REG_18H.WORD = 0x00000004u;
                TSIP.REG_38H.WORD = 0x00000c40u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_18H.BIT.B10)
                {
                    /* waiting */
                }
                TSIP.REG_18H.WORD = 0x00000000u;
                TSIP.REG_24H.WORD = 0x000005c0u;
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
                TSIP.REG_24H.WORD = 0x000005c0u;
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
                RX671_func100(change_endian_long(0x882c2bb3u), change_endian_long(0xb82f7a33u), change_endian_long(0x60f58c0au), change_endian_long(0xa21cd73du));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (0u == (TSIP.REG_1CH.BIT.B22))
                {
                    TSIP.REG_ECH.WORD = 0x00002e40u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    RX671_func101(change_endian_long(0x22e94638u), change_endian_long(0xf8576567u), change_endian_long(0xe76cc5d7u), change_endian_long(0x66e83fe9u));
                }
                else
                {
                    RX671_func100(change_endian_long(0x6b1a303eu), change_endian_long(0x7052abb8u), change_endian_long(0xa6973067u), change_endian_long(0x86a50a36u));
                    TSIP.REG_24H.WORD = 0x000019c0u;
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
                    TSIP.REG_24H.WORD = 0x000040c1u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_24H.WORD = 0x00006404u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_24H.WORD = 0x00006c04u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_E0H.WORD = 0x81010120u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_04H.WORD = 0x00000606u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    S_RAM[0+1 + 0] = change_endian_long(TSIP.REG_100H.WORD);
                    TSIP.REG_ECH.WORD = 0x000037e9u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    for(jLoop = 0; jLoop < S_RAM[0+1]; jLoop = jLoop + 1)
                    {
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
                        RX671_func100(change_endian_long(0x2c66b706u), change_endian_long(0x56aec6c6u), change_endian_long(0x536700e1u), change_endian_long(0xaee46608u));
                        TSIP.REG_1CH.WORD = 0x00400000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (0u == (TSIP.REG_1CH.BIT.B22))
                        {
                            TSIP.REG_ECH.WORD = 0x00002e40u;
                            TSIP.REG_1D0H.WORD = 0x00000000u;
                            RX671_func101(change_endian_long(0xca8ba996u), change_endian_long(0x447612d4u), change_endian_long(0x09cdb4e1u), change_endian_long(0xdd9a906au));
                            break;
                        }
                        else
                        {
                            TSIP.REG_24H.WORD = 0x00004c0cu;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_24H.BIT.B21)
                            {
                                /* waiting */
                            }
                            TSIP.REG_24H.WORD = 0x0000880cu;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_24H.BIT.B21)
                            {
                                /* waiting */
                            }
                            TSIP.REG_24H.WORD = 0x000019c0u;
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
                            TSIP.REG_24H.WORD = 0x000040c1u;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_24H.BIT.B21)
                            {
                                /* waiting */
                            }
                            RX671_func101(change_endian_long(0xc965ea2du), change_endian_long(0xc26a77a7u), change_endian_long(0x2222bdcbu), change_endian_long(0x3f645fa9u));
                        }
                    }
                    TSIP.REG_ECH.WORD = 0x38008a40u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00000000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00260000u;
                    RX671_func100(change_endian_long(0xc055fb49u), change_endian_long(0x42c430a7u), change_endian_long(0x41141d43u), change_endian_long(0xae60057eu));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX671_func101(change_endian_long(0x23c95f2au), change_endian_long(0xeded0ddau), change_endian_long(0xf5b222aeu), change_endian_long(0x945ca6c0u));
                        break;
                    }
                    else
                    {
                        RX671_func101(change_endian_long(0x83ae8f2fu), change_endian_long(0x94ac8920u), change_endian_long(0x01a48cfeu), change_endian_long(0x86888778u));
                    }
                }
            }
            TSIP.REG_ECH.WORD = 0x38000a4bu;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            RX671_func100(change_endian_long(0x2d21f2cfu), change_endian_long(0xc0c4ec01u), change_endian_long(0x51d308d5u), change_endian_long(0xbb089f4du));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_ECH.WORD = 0x00002e20u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x38008a20u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x00000002u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                RX671_func100(change_endian_long(0x8c9edea3u), change_endian_long(0x7bb6a872u), change_endian_long(0xc7b53e9au), change_endian_long(0xdec6f9ebu));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    RX671_func101(change_endian_long(0xa2a8841du), change_endian_long(0xbcaa458du), change_endian_long(0x3b392333u), change_endian_long(0x0ef01fdeu));
                    break;
                }
                else
                {
                    TSIP.REG_24H.WORD = 0x000019c0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_24H.WORD = 0x0000e0c1u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    RX671_func101(change_endian_long(0x45539144u), change_endian_long(0x9a94c7dfu), change_endian_long(0x4969b582u), change_endian_long(0xe870ed38u));
                }
            }
            else
            {
                RX671_func101(change_endian_long(0x0d150ce9u), change_endian_long(0xa9c4c133u), change_endian_long(0x796baec4u), change_endian_long(0x7d86acdfu));
            }
        }
    }
    TSIP.REG_ECH.WORD = 0x38008a20u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00000002u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(change_endian_long(0x4cd954d9u), change_endian_long(0x3fb44112u), change_endian_long(0x9c13acc4u), change_endian_long(0xcef71207u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func102(change_endian_long(0x52e13844u), change_endian_long(0x026a0090u), change_endian_long(0x4179ef4bu), change_endian_long(0xffc668f8u));
        TSIP.REG_1BCH.WORD = 0x00000040u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_18H.BIT.B12)
        {
            /* waiting */
        }
        #if TSIP_MULTI_THREADING == 1
        TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
        #endif /* TSIP_MULTI_THREADING == 1 */
        return TSIP_ERR_FAIL;
    }
    RX671_func100(change_endian_long(0x5578eea6u), change_endian_long(0x5fe0e6f6u), change_endian_long(0xe7b9208bu), change_endian_long(0x14cf5142u));
    TSIP.REG_ECH.WORD = 0x0000b5c0u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00010001u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x000019c0u;
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
    TSIP.REG_24H.WORD = 0x000080c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00001dc0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000800u;
    TSIP.REG_24H.WORD = 0x8000c0c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000020u;
    TSIP.REG_24H.WORD = 0x800019c0u;
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
    TSIP.REG_24H.WORD = 0x000040c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_D0H.WORD = 0x00000700u;
    TSIP.REG_C4H.WORD = 0x42e087bfu;
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    TSIP.REG_04H.WORD = 0x00000282u;
    for (iLoop = 0; iLoop < 32; iLoop = iLoop + 4)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[iLoop + 0] = TSIP.REG_100H.WORD;
        S_HEAP[iLoop + 1] = TSIP.REG_100H.WORD;
        S_HEAP[iLoop + 2] = TSIP.REG_100H.WORD;
        S_HEAP[iLoop + 3] = TSIP.REG_100H.WORD;
    }
    RX671_func100(change_endian_long(0x583d328du), change_endian_long(0xd8a8bc66u), change_endian_long(0xa26135f9u), change_endian_long(0xc90d817du));
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x00000c84u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_C4H.WORD = 0x400009cdu;
    TSIP.REG_00H.WORD = 0x00002213u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_04H.WORD = 0x00000212u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    S_HEAP[iLoop + 0] = TSIP.REG_100H.WORD;
    S_HEAP[iLoop + 1] = TSIP.REG_100H.WORD;
    S_HEAP[iLoop + 2] = TSIP.REG_100H.WORD;
    S_HEAP[iLoop + 3] = TSIP.REG_100H.WORD;
    TSIP.REG_28H.WORD = 0x008f0001u;
    TSIP.REG_24H.WORD = 0x0000b0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000c8d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000e8d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    /* WAIT_LOOP */
    while(1)
    {
        TSIP.REG_24H.WORD = 0x00008006u;
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
        TSIP.REG_24H.WORD = 0x00000591u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00210000u;
        RX671_func100(change_endian_long(0x19818226u), change_endian_long(0x81c323dfu), change_endian_long(0x7334d7d6u), change_endian_long(0x8af40f5cu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func101(change_endian_long(0x61fb9512u), change_endian_long(0x57a240f0u), change_endian_long(0xdc89cf1bu), change_endian_long(0x032c40c9u));
            break;
        }
        else
        {
            TSIP.REG_24H.WORD = 0x000019c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x0000d0d0u;
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
            RX671_func101(change_endian_long(0xe0d09860u), change_endian_long(0x669d4ceeu), change_endian_long(0x475d2c4eu), change_endian_long(0x2e3f92c2u));
        }
    }
    TSIP.REG_24H.WORD = 0x000094d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x0000c002u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000dcd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_2CH.WORD = 0x00000000u;
    TSIP.REG_34H.WORD = 0x00000800u;
    TSIP.REG_24H.WORD = 0x8000980au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_00H.WORD = 0x0000307fu;
    TSIP.REG_2CH.WORD = 0x00000014u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_104H.WORD = 0x00000057u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00010001u);
    TSIP.REG_2CH.WORD = 0x00000002u;
    TSIP.REG_24H.WORD = 0x0000c002u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_E0H.WORD = 0x800100c0u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_00H.WORD = 0x0000031fu;
    TSIP.REG_2CH.WORD = 0x00000024u;
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
    TSIP.REG_28H.WORD = 0x009f0001u;
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
    TSIP.REG_24H.WORD = 0x000040c1u;
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
    TSIP.REG_24H.WORD = 0x0000e0c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_ECH.WORD = 0x0000b500u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00010001u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x000005c0u;
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
    /* WAIT_LOOP */
    while(1)
    {
        TSIP.REG_ECH.WORD = 0x0000094au;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x80a0000au;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x00008383u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_B0H.WORD = 0x00000714u;
        TSIP.REG_A4H.WORD = 0x00008a07u;
        TSIP.REG_00H.WORD = 0x00001383u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x008f0001u;
        TSIP.REG_24H.WORD = 0x00001dc0u;
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
        TSIP.REG_24H.WORD = 0x000080d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x00870001u;
        TSIP.REG_00H.WORD = 0x0000301fu;
        TSIP.REG_2CH.WORD = 0x00000014u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_E0H.WORD = 0x810100c0u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x00003807u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_00H.WORD = 0x0000301fu;
        TSIP.REG_2CH.WORD = 0x00000010u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_E0H.WORD = 0x81010100u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x00003807u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_2CH.WORD = 0x00000000u;
        TSIP.REG_34H.WORD = 0x00000800u;
        TSIP.REG_24H.WORD = 0x8000c002u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_ECH.WORD = 0x00003506u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x800100c0u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x0000031fu;
        TSIP.REG_2CH.WORD = 0x0000002cu;
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
        TSIP.REG_ECH.WORD = 0x380088c0u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00000000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX671_func100(change_endian_long(0xc032003bu), change_endian_long(0xd91c9b43u), change_endian_long(0x2f247dd2u), change_endian_long(0x18ed5e1cu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_28H.WORD = 0x008f0001u;
            TSIP.REG_104H.WORD = 0x00001f61u;
            TSIP.REG_B0H.WORD = 0x00000700u;
            TSIP.REG_A4H.WORD = 0x42f087bfu;
            TSIP.REG_00H.WORD = 0x00003143u;
            TSIP.REG_2CH.WORD = 0x00000015u;
            for (iLoop = 0; iLoop < 16; iLoop = iLoop + 4)
            {
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = S_HEAP[iLoop + 0];
                TSIP.REG_100H.WORD = S_HEAP[iLoop + 1];
                TSIP.REG_100H.WORD = S_HEAP[iLoop + 2];
                TSIP.REG_100H.WORD = S_HEAP[iLoop + 3];
            }
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_00H.WORD = 0x00003143u;
            TSIP.REG_2CH.WORD = 0x00000014u;
            for (iLoop = 16; iLoop < 32; iLoop = iLoop + 4)
            {
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = S_HEAP[iLoop + 0];
                TSIP.REG_100H.WORD = S_HEAP[iLoop + 1];
                TSIP.REG_100H.WORD = S_HEAP[iLoop + 2];
                TSIP.REG_100H.WORD = S_HEAP[iLoop + 3];
            }
            RX671_func101(change_endian_long(0xf34e532au), change_endian_long(0x66a900c3u), change_endian_long(0x1c9027e1u), change_endian_long(0x13d339a8u));
            break;
        }
        else
        {
            TSIP.REG_28H.WORD = 0x008f0001u;
            TSIP.REG_34H.WORD = 0x00000008u;
            TSIP.REG_24H.WORD = 0x800011c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x0000e0c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_00H.WORD = 0x00003043u;
            TSIP.REG_2CH.WORD = 0x00000018u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_28H.WORD = 0x009f0001u;
            TSIP.REG_24H.WORD = 0x0000890eu;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_D0H.WORD = 0x0000071cu;
            TSIP.REG_C4H.WORD = 0x00008a83u;
            TSIP.REG_00H.WORD = 0x00003283u;
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
            TSIP.REG_24H.WORD = 0x00001de1u;
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
            TSIP.REG_ECH.WORD = 0x0000094au;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x81a0000au;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_00H.WORD = 0x00003883u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX671_func101(change_endian_long(0xbec02e97u), change_endian_long(0x2e13d47cu), change_endian_long(0xd10ba650u), change_endian_long(0x0eff5667u));
        }
    }
    TSIP.REG_ECH.WORD = 0x00007c06u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_1CH.WORD = 0x00602000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_A4H.WORD = 0x400007bdu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_HEAP[iLoop + 0];
    TSIP.REG_100H.WORD = S_HEAP[iLoop + 1];
    TSIP.REG_100H.WORD = S_HEAP[iLoop + 2];
    TSIP.REG_100H.WORD = S_HEAP[iLoop + 3];
    TSIP.REG_A4H.WORD = 0x00800c45u;
    TSIP.REG_00H.WORD = 0x00001113u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX671_func100(change_endian_long(0x309806abu), change_endian_long(0x9a8ee64du), change_endian_long(0x241b1662u), change_endian_long(0xae2b89b8u));
    TSIP.REG_34H.WORD = 0x00000802u;
    TSIP.REG_24H.WORD = 0x800088d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000088d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000b8d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_2CH.WORD = 0x00000002u;
    TSIP.REG_24H.WORD = 0x00007b0au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000c8d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000800u;
    TSIP.REG_24H.WORD = 0x8000ccd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x009f0001u;
    RX671_func103();
    RX671_func100(change_endian_long(0xa91db7e0u), change_endian_long(0xff92407eu), change_endian_long(0xc518ee69u), change_endian_long(0x81da4becu));
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x01000c84u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_E0H.WORD = 0x80040000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_00H.WORD = 0x00008213u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_E0H.WORD = 0x81040000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_04H.WORD = 0x00000612u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PubKeyIndex[0] = TSIP.REG_100H.WORD;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PubKeyIndex[1] = TSIP.REG_100H.WORD;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PubKeyIndex[2] = TSIP.REG_100H.WORD;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PubKeyIndex[3] = TSIP.REG_100H.WORD;
    RX671_func100(change_endian_long(0x86a96ad8u), change_endian_long(0x0359e9f8u), change_endian_long(0xa5e3d0c6u), change_endian_long(0xc5ac8141u));
    TSIP.REG_C4H.WORD = 0x200c3b0du;
    TSIP.REG_E0H.WORD = 0x81040000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_00H.WORD = 0x00002813u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_C4H.WORD = 0x000c0b0cu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x7a241e7bu);
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x00040804u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    TSIP.REG_D0H.WORD = 0x00000700u;
    TSIP.REG_C4H.WORD = 0x02f0888fu;
    TSIP.REG_04H.WORD = 0x00000282u;
    for(iLoop=0; iLoop<32; iLoop=iLoop+4)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[iLoop+4 + 0] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[iLoop+4 + 1] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[iLoop+4 + 2] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[iLoop+4 + 3] = TSIP.REG_100H.WORD;
    }
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX671_func100(change_endian_long(0xda898d1bu), change_endian_long(0xa8994f68u), change_endian_long(0x823f58a7u), change_endian_long(0x89eed7a6u));
    TSIP.REG_C4H.WORD = 0x00f0088cu;
    TSIP.REG_E0H.WORD = 0x810101c0u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_00H.WORD = 0x00002807u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_04H.WORD = 0x00000212u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PubKeyIndex[36] = TSIP.REG_100H.WORD;
    OutData_PubKeyIndex[37] = TSIP.REG_100H.WORD;
    OutData_PubKeyIndex[38] = TSIP.REG_100H.WORD;
    OutData_PubKeyIndex[39] = TSIP.REG_100H.WORD;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_C8H.BIT.B6)
    {
        /* waiting */
    }
    RX671_func100(change_endian_long(0x17ff8977u), change_endian_long(0x466c87eau), change_endian_long(0x7217b903u), change_endian_long(0xbdb1e47bu));
    TSIP.REG_18H.WORD = 0x00000004u;
    TSIP.REG_24H.WORD = 0x00004404u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_18H.BIT.B10)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00004804u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_18H.BIT.B10)
    {
        /* waiting */
    }
    TSIP.REG_18H.WORD = 0x00000000u;
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    TSIP.REG_D0H.WORD = 0x00000700u;
    TSIP.REG_C4H.WORD = 0x00e087bfu;
    TSIP.REG_04H.WORD = 0x00000282u;
    for(iLoop=36; iLoop<68; iLoop=iLoop+4)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[iLoop+4 + 0] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[iLoop+4 + 1] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[iLoop+4 + 2] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[iLoop+4 + 3] = TSIP.REG_100H.WORD;
    }
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX671_func100(change_endian_long(0xbb52b76eu), change_endian_long(0x1e13247au), change_endian_long(0x48908738u), change_endian_long(0xe482a249u));
    TSIP.REG_104H.WORD = 0x00000352u;
    TSIP.REG_C4H.WORD = 0x000009cdu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000008u);
    TSIP.REG_04H.WORD = 0x00000212u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PubKeyIndex[72] = TSIP.REG_100H.WORD;
    OutData_PubKeyIndex[73] = TSIP.REG_100H.WORD;
    OutData_PubKeyIndex[74] = TSIP.REG_100H.WORD;
    OutData_PubKeyIndex[75] = TSIP.REG_100H.WORD;
    RX671_func100(change_endian_long(0x7587d152u), change_endian_long(0xc9af641au), change_endian_long(0xc8252357u), change_endian_long(0x92a7d4acu));
    RX671_func103();
    RX671_func100(change_endian_long(0x220b4aacu), change_endian_long(0x3d28157bu), change_endian_long(0x2c437473u), change_endian_long(0xf1718c77u));
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x01000c84u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_E0H.WORD = 0x800401e0u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_00H.WORD = 0x00008213u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_E0H.WORD = 0x810401e0u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_04H.WORD = 0x00000612u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PrivKeyIndex[0] = TSIP.REG_100H.WORD;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PrivKeyIndex[1] = TSIP.REG_100H.WORD;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PrivKeyIndex[2] = TSIP.REG_100H.WORD;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PrivKeyIndex[3] = TSIP.REG_100H.WORD;
    RX671_func100(change_endian_long(0x3a6df01eu), change_endian_long(0x38fe8369u), change_endian_long(0x2519ddaeu), change_endian_long(0x39eff150u));
    TSIP.REG_C4H.WORD = 0x200c3b0du;
    TSIP.REG_E0H.WORD = 0x810401e0u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_00H.WORD = 0x00002813u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_C4H.WORD = 0x000c0b0cu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x9785ce17u);
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000024u;
    TSIP.REG_D0H.WORD = 0x00000700u;
    TSIP.REG_C4H.WORD = 0x02e087bfu;
    TSIP.REG_04H.WORD = 0x00000282u;
    for(iLoop=0; iLoop<32; iLoop=iLoop+4)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PrivKeyIndex[iLoop+4 + 0] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[iLoop+4 + 1] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[iLoop+4 + 2] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[iLoop+4 + 3] = TSIP.REG_100H.WORD;
    }
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX671_func100(change_endian_long(0x386e6e0du), change_endian_long(0xf3267698u), change_endian_long(0x1cce8140u), change_endian_long(0x88236499u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    TSIP.REG_D0H.WORD = 0x00000700u;
    TSIP.REG_C4H.WORD = 0x00e087bfu;
    TSIP.REG_04H.WORD = 0x00000282u;
    for(iLoop=32; iLoop<64; iLoop=iLoop+4)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PrivKeyIndex[iLoop+4 + 0] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[iLoop+4 + 1] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[iLoop+4 + 2] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[iLoop+4 + 3] = TSIP.REG_100H.WORD;
    }
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX671_func100(change_endian_long(0xdd428d24u), change_endian_long(0x33359aa6u), change_endian_long(0x779f38a3u), change_endian_long(0x3d5ba7c2u));
    TSIP.REG_00H.WORD = 0x00002383u;
    TSIP.REG_2CH.WORD = 0x00000022u;
    TSIP.REG_D0H.WORD = 0x00000700u;
    TSIP.REG_C4H.WORD = 0x00e087bfu;
    TSIP.REG_04H.WORD = 0x00000282u;
    for(iLoop=64; iLoop<96; iLoop=iLoop+4)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PrivKeyIndex[iLoop+4 + 0] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[iLoop+4 + 1] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[iLoop+4 + 2] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[iLoop+4 + 3] = TSIP.REG_100H.WORD;
    }
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX671_func100(change_endian_long(0xef00cfcfu), change_endian_long(0x61d1b626u), change_endian_long(0xee7bc47bu), change_endian_long(0xf9a7defeu));
    TSIP.REG_104H.WORD = 0x00000352u;
    TSIP.REG_C4H.WORD = 0x000009cdu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000018u);
    TSIP.REG_04H.WORD = 0x00000212u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_PrivKeyIndex[100] = TSIP.REG_100H.WORD;
    OutData_PrivKeyIndex[101] = TSIP.REG_100H.WORD;
    OutData_PrivKeyIndex[102] = TSIP.REG_100H.WORD;
    OutData_PrivKeyIndex[103] = TSIP.REG_100H.WORD;
    RX671_func102(change_endian_long(0xf6ec0f6cu), change_endian_long(0x34e12117u), change_endian_long(0xdc24e68fu), change_endian_long(0x16336784u));
    TSIP.REG_1BCH.WORD = 0x00000040u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_18H.BIT.B12)
    {
        /* waiting */
    }
    #if TSIP_MULTI_THREADING == 1
    TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
    #endif /* TSIP_MULTI_THREADING == 1 */
    return TSIP_SUCCESS;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX671/RX671_p2a.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_PRV_USE_RSA_1024 == 1 */
