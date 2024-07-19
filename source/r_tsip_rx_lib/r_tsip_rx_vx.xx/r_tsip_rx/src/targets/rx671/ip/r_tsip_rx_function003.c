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
* Function Name: RX671_func003
*******************************************************************************************************************/ /**
* @details       RX671 func003
* @param[in]     ARG1[]
* @note          None
*/
void RX671_func003(uint32_t* ARG1)
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
    TSIP.REG_28H.WORD = 0x00870001u;
    OFS_ADR = 128;
    RX671_func004(OFS_ADR);
    TSIP.REG_24H.WORD = 0x0000dcd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000403u;
    TSIP.REG_24H.WORD = 0x8000e8d0u;
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
    TSIP.REG_34H.WORD = 0x00000010u;
    TSIP.REG_24H.WORD = 0x80005cd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000003u;
    TSIP.REG_24H.WORD = 0x80008cd0u;
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
    TSIP.REG_34H.WORD = 0x00000800u;
    TSIP.REG_24H.WORD = 0x800050d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000802u;
    TSIP.REG_24H.WORD = 0x800088d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000004u;
    TSIP.REG_24H.WORD = 0x800094d0u;
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
    TSIP.REG_34H.WORD = 0x00000800u;
    TSIP.REG_24H.WORD = 0x800070d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000802u;
    TSIP.REG_24H.WORD = 0x8000acd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_104H.WORD = 0x00000058u;
    TSIP.REG_E0H.WORD = 0x80010360u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x000000f0u);
    RX671_func101(change_endian_long(0x7b118bceu), change_endian_long(0x7ee08fd5u), change_endian_long(0x2d7da646u), change_endian_long(0x4e45ab9eu));
    RX671_func006();
    RX671_func100(change_endian_long(0xf857ba2fu), change_endian_long(0x43426013u), change_endian_long(0x75debd51u), change_endian_long(0xb3313d7du));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_ECH.WORD = 0x00000800u;
        RX671_func101(change_endian_long(0x30ea445fu), change_endian_long(0x202986f9u), change_endian_long(0x8b356486u), change_endian_long(0xe03054bdu));
    }
    else
    {
        RX671_func100(change_endian_long(0x288e181au), change_endian_long(0x756ea3f3u), change_endian_long(0x30f66ceeu), change_endian_long(0xcd512ca9u));
        TSIP.REG_24H.WORD = 0x00009cd0u;
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
        TSIP.REG_18H.WORD = 0x00000004u;
        TSIP.REG_38H.WORD = 0x000000b0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_18H.BIT.B10)
        {
            /* waiting */
        }
        TSIP.REG_18H.WORD = 0x00000000u;
        TSIP.REG_34H.WORD = 0x00000010u;
        TSIP.REG_24H.WORD = 0x80005cd0u;
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
        TSIP.REG_24H.WORD = 0x0000a8d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_34H.WORD = 0x00000008u;
        TSIP.REG_24H.WORD = 0x800050d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00004a0cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x0000480cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x0000480cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00004a0cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_34H.WORD = 0x00000008u;
        TSIP.REG_24H.WORD = 0x800074d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00006e0cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00006c0cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00006c0cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00006c0cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00006e0cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00006e0cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        RX671_func100(change_endian_long(0x3eaa9825u), change_endian_long(0x7050d0d7u), change_endian_long(0xc719f74au), change_endian_long(0xa076105cu));
        RX671_func103();
        RX671_func100(change_endian_long(0xf56a3130u), change_endian_long(0xb3d14e6eu), change_endian_long(0x67714235u), change_endian_long(0xa9e9812fu));
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x01080c84u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        RX671_func100(change_endian_long(0xb6635832u), change_endian_long(0x4c253de7u), change_endian_long(0x12f35353u), change_endian_long(0xaa6bc59bu));
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x42e087bfu;
        TSIP.REG_00H.WORD = 0x00002323u;
        TSIP.REG_2CH.WORD = 0x000000aau;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_04H.WORD = 0x00000223u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[0] = TSIP.REG_100H.WORD;
        S_HEAP[1] = TSIP.REG_100H.WORD;
        S_HEAP[2] = TSIP.REG_100H.WORD;
        S_HEAP[3] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[4] = TSIP.REG_100H.WORD;
        S_HEAP[5] = TSIP.REG_100H.WORD;
        S_HEAP[6] = TSIP.REG_100H.WORD;
        S_HEAP[7] = TSIP.REG_100H.WORD;
        RX671_func100(change_endian_long(0xefb067f7u), change_endian_long(0x2cc90148u), change_endian_long(0xbeeb0606u), change_endian_long(0x81dd90ecu));
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x40e087bfu;
        TSIP.REG_00H.WORD = 0x00002323u;
        TSIP.REG_2CH.WORD = 0x000000abu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_04H.WORD = 0x00000223u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[8] = TSIP.REG_100H.WORD;
        S_HEAP[9] = TSIP.REG_100H.WORD;
        S_HEAP[10] = TSIP.REG_100H.WORD;
        S_HEAP[11] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[12] = TSIP.REG_100H.WORD;
        S_HEAP[13] = TSIP.REG_100H.WORD;
        S_HEAP[14] = TSIP.REG_100H.WORD;
        S_HEAP[15] = TSIP.REG_100H.WORD;
        RX671_func100(change_endian_long(0xbc7d29e1u), change_endian_long(0xf0c49a40u), change_endian_long(0xffe00780u), change_endian_long(0xecccba38u));
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x40e087bfu;
        TSIP.REG_00H.WORD = 0x00002323u;
        TSIP.REG_2CH.WORD = 0x000000a8u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_04H.WORD = 0x00000223u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[16] = TSIP.REG_100H.WORD;
        S_HEAP[17] = TSIP.REG_100H.WORD;
        S_HEAP[18] = TSIP.REG_100H.WORD;
        S_HEAP[19] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[20] = TSIP.REG_100H.WORD;
        S_HEAP[21] = TSIP.REG_100H.WORD;
        S_HEAP[22] = TSIP.REG_100H.WORD;
        S_HEAP[23] = TSIP.REG_100H.WORD;
        RX671_func100(change_endian_long(0x10a1b207u), change_endian_long(0xffbe3cb5u), change_endian_long(0x4e720d37u), change_endian_long(0x110a335au));
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x40e087bfu;
        TSIP.REG_00H.WORD = 0x00002323u;
        TSIP.REG_2CH.WORD = 0x000000a9u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_04H.WORD = 0x00000223u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[24] = TSIP.REG_100H.WORD;
        S_HEAP[25] = TSIP.REG_100H.WORD;
        S_HEAP[26] = TSIP.REG_100H.WORD;
        S_HEAP[27] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[28] = TSIP.REG_100H.WORD;
        S_HEAP[29] = TSIP.REG_100H.WORD;
        S_HEAP[30] = TSIP.REG_100H.WORD;
        S_HEAP[31] = TSIP.REG_100H.WORD;
        RX671_func100(change_endian_long(0xa3b4f953u), change_endian_long(0xfd51fc9au), change_endian_long(0x566f36f4u), change_endian_long(0xfa0f9c5bu));
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x40e087bfu;
        TSIP.REG_00H.WORD = 0x00002323u;
        TSIP.REG_2CH.WORD = 0x0000002au;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_04H.WORD = 0x00000223u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[32] = TSIP.REG_100H.WORD;
        S_HEAP[33] = TSIP.REG_100H.WORD;
        S_HEAP[34] = TSIP.REG_100H.WORD;
        S_HEAP[35] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[36] = TSIP.REG_100H.WORD;
        S_HEAP[37] = TSIP.REG_100H.WORD;
        S_HEAP[38] = TSIP.REG_100H.WORD;
        S_HEAP[39] = TSIP.REG_100H.WORD;
        RX671_func100(change_endian_long(0xed865562u), change_endian_long(0xf488d2dau), change_endian_long(0x84d57b03u), change_endian_long(0xd9847a70u));
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x40e087bfu;
        TSIP.REG_00H.WORD = 0x00002323u;
        TSIP.REG_2CH.WORD = 0x0000002bu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_04H.WORD = 0x00000223u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[40] = TSIP.REG_100H.WORD;
        S_HEAP[41] = TSIP.REG_100H.WORD;
        S_HEAP[42] = TSIP.REG_100H.WORD;
        S_HEAP[43] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[44] = TSIP.REG_100H.WORD;
        S_HEAP[45] = TSIP.REG_100H.WORD;
        S_HEAP[46] = TSIP.REG_100H.WORD;
        S_HEAP[47] = TSIP.REG_100H.WORD;
        RX671_func100(change_endian_long(0xe46643d0u), change_endian_long(0xc3a67c20u), change_endian_long(0xc9456265u), change_endian_long(0xdce8fc7du));
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x40e087bfu;
        TSIP.REG_00H.WORD = 0x00002323u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_04H.WORD = 0x00000223u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[48] = TSIP.REG_100H.WORD;
        S_HEAP[49] = TSIP.REG_100H.WORD;
        S_HEAP[50] = TSIP.REG_100H.WORD;
        S_HEAP[51] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[52] = TSIP.REG_100H.WORD;
        S_HEAP[53] = TSIP.REG_100H.WORD;
        S_HEAP[54] = TSIP.REG_100H.WORD;
        S_HEAP[55] = TSIP.REG_100H.WORD;
        RX671_func100(change_endian_long(0xb2c56ec1u), change_endian_long(0x152b5088u), change_endian_long(0x10d2a9dau), change_endian_long(0xa078244fu));
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x40e087bfu;
        TSIP.REG_00H.WORD = 0x00002323u;
        TSIP.REG_2CH.WORD = 0x00000023u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_04H.WORD = 0x00000223u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[56] = TSIP.REG_100H.WORD;
        S_HEAP[57] = TSIP.REG_100H.WORD;
        S_HEAP[58] = TSIP.REG_100H.WORD;
        S_HEAP[59] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[60] = TSIP.REG_100H.WORD;
        S_HEAP[61] = TSIP.REG_100H.WORD;
        S_HEAP[62] = TSIP.REG_100H.WORD;
        S_HEAP[63] = TSIP.REG_100H.WORD;
        RX671_func100(change_endian_long(0x93bf4b15u), change_endian_long(0xba72b283u), change_endian_long(0x027960fdu), change_endian_long(0x8553a3b9u));
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
        TSIP.REG_04H.WORD = 0x00000213u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_HEAP[64] = TSIP.REG_100H.WORD;
        S_HEAP[65] = TSIP.REG_100H.WORD;
        S_HEAP[66] = TSIP.REG_100H.WORD;
        S_HEAP[67] = TSIP.REG_100H.WORD;
        RX671_func004(OFS_ADR);
        TSIP.REG_24H.WORD = 0x000019c0u;
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
        TSIP.REG_24H.WORD = 0x0000e0c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_34H.WORD = 0x00000403u;
        TSIP.REG_24H.WORD = 0x8000e8d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_34H.WORD = 0x00000002u;
        TSIP.REG_24H.WORD = 0x800048d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_34H.WORD = 0x00000c00u;
        TSIP.REG_24H.WORD = 0x800058d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_34H.WORD = 0x00000c20u;
        TSIP.REG_24H.WORD = 0x8000bcd0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_00H.WORD = 0x00003523u;
        TSIP.REG_74H.WORD = 0x00000008u;
        TSIP.REG_2CH.WORD = 0x00000094u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_2CH.WORD = 0x0000001du;
        TSIP.REG_104H.WORD = 0x00000767u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = ARG1[0];
        TSIP.REG_100H.WORD = ARG1[1];
        TSIP.REG_100H.WORD = ARG1[2];
        TSIP.REG_100H.WORD = ARG1[3];
        TSIP.REG_100H.WORD = ARG1[4];
        TSIP.REG_100H.WORD = ARG1[5];
        TSIP.REG_100H.WORD = ARG1[6];
        TSIP.REG_100H.WORD = ARG1[7];
        TSIP.REG_2CH.WORD = 0x00000010u;
        TSIP.REG_104H.WORD = 0x00000767u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = ARG1[8];
        TSIP.REG_100H.WORD = ARG1[9];
        TSIP.REG_100H.WORD = ARG1[10];
        TSIP.REG_100H.WORD = ARG1[11];
        TSIP.REG_100H.WORD = ARG1[12];
        TSIP.REG_100H.WORD = ARG1[13];
        TSIP.REG_100H.WORD = ARG1[14];
        TSIP.REG_100H.WORD = ARG1[15];
        TSIP.REG_24H.WORD = 0x0000dcd0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x0000a4d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x0002b208u;
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
        TSIP.REG_34H.WORD = 0x00000020u;
        TSIP.REG_24H.WORD = 0x80007cd0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00000d91u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001f00u;
        TSIP.REG_1CH.WORD = 0x00210000u;
        TSIP.REG_34H.WORD = 0x00000020u;
        TSIP.REG_24H.WORD = 0x80001dc0u;
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
        TSIP.REG_1CH.WORD = 0x00001f00u;
        TSIP.REG_1CH.WORD = 0x00210000u;
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
        TSIP.REG_24H.WORD = 0x00001191u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001f00u;
        TSIP.REG_1CH.WORD = 0x00210000u;
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
        TSIP.REG_1CH.WORD = 0x00001f00u;
        TSIP.REG_1CH.WORD = 0x00210000u;
        RX671_func100(change_endian_long(0x2b0f5c23u), change_endian_long(0x273175c4u), change_endian_long(0x3f5d07fbu), change_endian_long(0x950d5eeeu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_ECH.WORD = 0x00000800u;
            RX671_func101(change_endian_long(0x969a3e58u), change_endian_long(0xd8bb359bu), change_endian_long(0xaca5aeb0u), change_endian_long(0x69880f8au));
        }
        else
        {
            TSIP.REG_34H.WORD = 0x00000c20u;
            TSIP.REG_24H.WORD = 0x8000fcd0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_34H.WORD = 0x00000800u;
            TSIP.REG_24H.WORD = 0x800048d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            RX671_func100(change_endian_long(0x3906f926u), change_endian_long(0x41b3e7a1u), change_endian_long(0xaf743fc0u), change_endian_long(0xc92074c9u));
            RX671_func009();
            RX671_func100(change_endian_long(0x76309dc4u), change_endian_long(0xe6d230dfu), change_endian_long(0xeeb3e126u), change_endian_long(0x0bb1f254u));
            TSIP.REG_34H.WORD = 0x00000410u;
            TSIP.REG_24H.WORD = 0x800098d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            RX671_func005(OFS_ADR);
            TSIP.REG_34H.WORD = 0x00000110u;
            TSIP.REG_24H.WORD = 0x800019c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_34H.WORD = 0x00000110u;
            TSIP.REG_24H.WORD = 0x80001ec0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_34H.WORD = 0x00000330u;
            TSIP.REG_24H.WORD = 0x800019c0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_34H.WORD = 0x00000330u;
            TSIP.REG_24H.WORD = 0x80001ec0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            RX671_func007();
            TSIP.REG_34H.WORD = 0x00000008u;
            TSIP.REG_24H.WORD = 0x8000f0d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_34H.WORD = 0x00000008u;
            TSIP.REG_24H.WORD = 0x800050d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_18H.WORD = 0x00000004u;
            TSIP.REG_38H.WORD = 0x00231030u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_18H.BIT.B10)
            {
                /* waiting */
            }
            TSIP.REG_18H.WORD = 0x00000000u;
            TSIP.REG_24H.WORD = 0x00001dc0u;
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
            TSIP.REG_1CH.WORD = 0x00001f00u;
            TSIP.REG_1CH.WORD = 0x00210000u;
            RX671_func100(change_endian_long(0x770c6aa3u), change_endian_long(0x83a751a1u), change_endian_long(0xecfa4dadu), change_endian_long(0x6611bc4du));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_ECH.WORD = 0x00000800u;
                RX671_func101(change_endian_long(0xa2d66f45u), change_endian_long(0xa1418f3fu), change_endian_long(0x8a3505ecu), change_endian_long(0x6ba0ed68u));
            }
            else
            {
                TSIP.REG_24H.WORD = 0x0000bcd0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x0000b20cu;
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
                TSIP.REG_ECH.WORD = 0x00000908u;
                TSIP.REG_ECH.WORD = 0x0000b500u;
                TSIP.REG_ECH.WORD = 0x00000060u;
                TSIP.REG_E0H.WORD = 0x81880008u;
                TSIP.REG_00H.WORD = 0x00003823u;
                TSIP.REG_2CH.WORD = 0x00000010u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x00000908u;
                TSIP.REG_E0H.WORD = 0x81880008u;
                TSIP.REG_00H.WORD = 0x00003823u;
                TSIP.REG_2CH.WORD = 0x00000013u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_24H.WORD = 0x00008c0cu;
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
                TSIP.REG_34H.WORD = 0x00000008u;
                TSIP.REG_24H.WORD = 0x800011c0u;
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
                TSIP.REG_24H.WORD = 0x000019a1u;
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
                TSIP.REG_24H.WORD = 0x00009008u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_ECH.WORD = 0x0000b500u;
                TSIP.REG_ECH.WORD = 0x00000020u;
                TSIP.REG_E0H.WORD = 0x81880008u;
                TSIP.REG_00H.WORD = 0x00003823u;
                TSIP.REG_2CH.WORD = 0x00000014u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_24H.WORD = 0x0000a4d0u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_24H.BIT.B21)
                {
                    /* waiting */
                }
                TSIP.REG_24H.WORD = 0x0002b208u;
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
                TSIP.REG_ECH.WORD = 0x0000b500u;
                TSIP.REG_ECH.WORD = 0x00000040u;
                TSIP.REG_E0H.WORD = 0x81880008u;
                TSIP.REG_00H.WORD = 0x00003823u;
                TSIP.REG_2CH.WORD = 0x00000012u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_24H.WORD = 0x0000880cu;
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
                TSIP.REG_1CH.WORD = 0x00001f00u;
                TSIP.REG_1CH.WORD = 0x00a10000u;
                RX671_func100(change_endian_long(0x11b8081du), change_endian_long(0xdd0f277fu), change_endian_long(0x30da547eu), change_endian_long(0x937c2b0au));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    TSIP.REG_ECH.WORD = 0x00000800u;
                    RX671_func101(change_endian_long(0x9038573cu), change_endian_long(0x74adfeefu), change_endian_long(0xe1d9f784u), change_endian_long(0x9da29054u));
                }
                else
                {
                    TSIP.REG_ECH.WORD = 0x0000d000u;
                    RX671_func101(change_endian_long(0x0c42ca1au), change_endian_long(0xd070b522u), change_endian_long(0x372cad5cu), change_endian_long(0xd4f4e61du));
                }
            }
        }
    }
    TSIP.REG_ECH.WORD = 0x38000c00u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_ECH.WORD = 0x00007c1du;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX671/RX671_func003.prc
 *********************************************************************************************************************/
