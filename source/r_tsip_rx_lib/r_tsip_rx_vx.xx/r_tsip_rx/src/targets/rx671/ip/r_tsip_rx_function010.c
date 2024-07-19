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
* Function Name: RX671_func010
*******************************************************************************************************************/ /**
* @details       RX671 func010
* @param[in]     ARG1
* @note          None
*/
void RX671_func010(uint32_t ARG1)
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
    OFS_ADR = ARG1;
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
    TSIP.REG_100H.WORD = change_endian_long(0x000000f5u);
    RX671_func101(change_endian_long(0x8492fbc6u), change_endian_long(0xab738e09u), change_endian_long(0x11ae9d59u), change_endian_long(0x9ec79caau));
    RX671_func006();
    RX671_func100(change_endian_long(0x8394c9dcu), change_endian_long(0xd1361bd5u), change_endian_long(0xd56ce56fu), change_endian_long(0x5c31aea9u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_ECH.WORD = 0x00000800u;
        RX671_func101(change_endian_long(0xd6748b04u), change_endian_long(0xf54d061cu), change_endian_long(0xb933fb32u), change_endian_long(0x59243c85u));
    }
    else
    {
        RX671_func100(change_endian_long(0x565f8782u), change_endian_long(0x8272d36fu), change_endian_long(0xe64bd51fu), change_endian_long(0x28fdba67u));
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
        RX671_func100(change_endian_long(0x1dac321fu), change_endian_long(0xd9dbae71u), change_endian_long(0x8852c562u), change_endian_long(0x1a168fe4u));
        RX671_func103();
        RX671_func100(change_endian_long(0x8398fd71u), change_endian_long(0x7e1a9cf2u), change_endian_long(0xc9f42411u), change_endian_long(0xbafafd33u));
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x01080c84u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        RX671_func100(change_endian_long(0x387cd66au), change_endian_long(0x2bc8095eu), change_endian_long(0x8394456fu), change_endian_long(0x568f4d73u));
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
        RX671_func100(change_endian_long(0x8f534a77u), change_endian_long(0x2195aac7u), change_endian_long(0xbc0a0341u), change_endian_long(0x2928cf95u));
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
        RX671_func100(change_endian_long(0x9960d3b8u), change_endian_long(0xc646f458u), change_endian_long(0x80667928u), change_endian_long(0xba0f439fu));
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
        RX671_func100(change_endian_long(0x4cf5f7c4u), change_endian_long(0x6b3eef01u), change_endian_long(0x84f9bccfu), change_endian_long(0x460b5bb4u));
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
        RX671_func100(change_endian_long(0x41793137u), change_endian_long(0x9217c651u), change_endian_long(0x3cad6b08u), change_endian_long(0x3d69c536u));
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
        RX671_func100(change_endian_long(0x6742e4f1u), change_endian_long(0xc3838590u), change_endian_long(0xa47f6254u), change_endian_long(0x490ab6e6u));
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
        RX671_func100(change_endian_long(0x79f12c21u), change_endian_long(0x35ead2ebu), change_endian_long(0xddd120beu), change_endian_long(0xfa5752deu));
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
        RX671_func100(change_endian_long(0xeee8c1f7u), change_endian_long(0x9cb4e5f1u), change_endian_long(0xe5a4cb13u), change_endian_long(0x682f0e57u));
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
        RX671_func100(change_endian_long(0x75acd52eu), change_endian_long(0xa4a435a4u), change_endian_long(0xfffe4519u), change_endian_long(0x5acdc35du));
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
        TSIP.REG_ECH.WORD = 0x00000908u;
        TSIP.REG_E0H.WORD = 0x81900008u;
        TSIP.REG_00H.WORD = 0x00003823u;
        TSIP.REG_2CH.WORD = 0x0000001du;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_00H.WORD = 0x00003823u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
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
        RX671_func100(change_endian_long(0x8b4efc49u), change_endian_long(0xcd30c3cbu), change_endian_long(0xe882ba49u), change_endian_long(0xa302603cu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_ECH.WORD = 0x00000800u;
            RX671_func101(change_endian_long(0x97322c73u), change_endian_long(0xe3770205u), change_endian_long(0xbfc6464fu), change_endian_long(0x1a7e8739u));
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
            RX671_func100(change_endian_long(0xf081a234u), change_endian_long(0x0d067617u), change_endian_long(0x749c71feu), change_endian_long(0x177b80b3u));
            RX671_func009();
            RX671_func100(change_endian_long(0xe03247fau), change_endian_long(0xa231b864u), change_endian_long(0x093b8f1eu), change_endian_long(0xb8cf0919u));
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
            RX671_func011(S_HEAP);
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
            RX671_func100(change_endian_long(0x30a84446u), change_endian_long(0xfd020e47u), change_endian_long(0x617daaa5u), change_endian_long(0x030b6a0bu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_ECH.WORD = 0x00000800u;
                RX671_func101(change_endian_long(0xb655f3d4u), change_endian_long(0x34247facu), change_endian_long(0xac764e35u), change_endian_long(0x34eb5f12u));
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
                RX671_func100(change_endian_long(0x09a4f3bcu), change_endian_long(0xa1b66418u), change_endian_long(0xa6376f37u), change_endian_long(0x2782553cu));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    TSIP.REG_ECH.WORD = 0x00000800u;
                    RX671_func101(change_endian_long(0x2a83f615u), change_endian_long(0x364e73beu), change_endian_long(0x5fbab58du), change_endian_long(0x41bfc19eu));
                }
                else
                {
                    TSIP.REG_ECH.WORD = 0x0000d000u;
                    RX671_func101(change_endian_long(0x0bb81425u), change_endian_long(0x504eac79u), change_endian_long(0x0cee0129u), change_endian_long(0x04bd87a2u));
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
 End of function ./input_dir/RX671/RX671_func010.prc
 *********************************************************************************************************************/
