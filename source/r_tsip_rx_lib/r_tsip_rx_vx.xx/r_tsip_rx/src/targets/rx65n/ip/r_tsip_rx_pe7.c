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

#if TSIP_TLS == 1
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
* Function Name: R_TSIP_TlsServersEphemeralEcdhPublicKeyRetrievesSub
*******************************************************************************************************************/ /**
* @details       RX65NHU TLS Ephemeral Key Generation
* @param[in]     InData_Sel_PubKeyType
* @param[in]     InData_ClientRandom
* @param[in]     InData_ServerRandom
* @param[in]     InData_Sel_CompressType
* @param[in]     InData_SKE_Message
* @param[in]     InData_SKE_Signature
* @param[in]     InData_PubKey
* @param[out]    OutData_EphemeralPubKey
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @note          None
*/
e_tsip_err_t R_TSIP_TlsServersEphemeralEcdhPublicKeyRetrievesSub(uint32_t *InData_Sel_PubKeyType, uint32_t *InData_ClientRandom, uint32_t *InData_ServerRandom, uint32_t *InData_Sel_CompressType, uint32_t *InData_SKE_Message, uint32_t *InData_SKE_Signature, uint32_t *InData_PubKey, uint32_t *OutData_EphemeralPubKey)
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
    if (0x0u != (TSIP.REG_1B8H.WORD & 0x1eu))
    {
        #if TSIP_MULTI_THREADING == 1
        TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
        #endif /* TSIP_MULTI_THREADING == 1 */
        return TSIP_ERR_RESOURCE_CONFLICT;
    }
    TSIP.REG_84H.WORD = 0x0000e701u;
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
    TSIP.REG_104H.WORD = 0x00000068u;
    TSIP.REG_E0H.WORD = 0x800101c0u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Sel_PubKeyType[0];
    TSIP.REG_ECH.WORD = 0x3420a9c0u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX65NHU_func100(change_endian_long(0x6f37cf8fu), change_endian_long(0xfe237f1fu), change_endian_long(0x5324e9fdu), change_endian_long(0xf91d4f5au));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0x72f7644cu), change_endian_long(0x0d7579acu), change_endian_long(0xea446cbau), change_endian_long(0x11eece3cu));
        TSIP.REG_1B8H.WORD = 0x00000040u;
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
    else
    {
        TSIP.REG_104H.WORD = 0x00000068u;
        TSIP.REG_E0H.WORD = 0x80010200u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_Sel_CompressType[0];
        TSIP.REG_ECH.WORD = 0x00000bffu;
        TSIP.REG_ECH.WORD = 0x38000e10u;
        TSIP.REG_ECH.WORD = 0x1000d3e0u;
        TSIP.REG_ECH.WORD = 0x38000fffu;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX65NHU_func100(change_endian_long(0x6e012b66u), change_endian_long(0x7ef7fd67u), change_endian_long(0x1c175c89u), change_endian_long(0x7f32b357u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func102(change_endian_long(0x7bfe49e0u), change_endian_long(0x785603bau), change_endian_long(0x78c6cc68u), change_endian_long(0x5eca4858u));
            TSIP.REG_1B8H.WORD = 0x00000040u;
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
        else
        {
            TSIP.REG_104H.WORD = 0x00000368u;
            TSIP.REG_E0H.WORD = 0x80040000u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_PubKey[0];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_PubKey[1];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_PubKey[2];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_PubKey[3];
            TSIP.REG_ECH.WORD = 0x00000bdeu;
            TSIP.REG_ECH.WORD = 0x000037e0u;
            TSIP.REG_ECH.WORD = 0x00008fe0u;
            TSIP.REG_ECH.WORD = 0xff000000u;
            TSIP.REG_ECH.WORD = 0x38008be0u;
            TSIP.REG_ECH.WORD = 0x05000000u;
            TSIP.REG_ECH.WORD = 0x1000d3c0u;
            TSIP.REG_ECH.WORD = 0x38008be0u;
            TSIP.REG_ECH.WORD = 0x0a000000u;
            TSIP.REG_ECH.WORD = 0x1000d3c0u;
            TSIP.REG_ECH.WORD = 0x38008be0u;
            TSIP.REG_ECH.WORD = 0x31000000u;
            TSIP.REG_ECH.WORD = 0x1000d3c0u;
            TSIP.REG_ECH.WORD = 0x3800d813u;
            TSIP.REG_ECH.WORD = 0x2000d3c1u;
            TSIP.REG_ECH.WORD = 0x000037e0u;
            TSIP.REG_ECH.WORD = 0x00008fe0u;
            TSIP.REG_ECH.WORD = 0x0000ff00u;
            TSIP.REG_ECH.WORD = 0x38008be0u;
            TSIP.REG_ECH.WORD = 0x00000100u;
            TSIP.REG_ECH.WORD = 0x1000d3c2u;
            TSIP.REG_ECH.WORD = 0x38008bc0u;
            TSIP.REG_ECH.WORD = 0x00000007u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            RX65NHU_func100(change_endian_long(0x0b29c139u), change_endian_long(0x322de4fbu), change_endian_long(0xe1cb69a9u), change_endian_long(0x0703c2efu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX65NHU_func102(change_endian_long(0xcfe601b9u), change_endian_long(0x8a987972u), change_endian_long(0xbebd63c3u), change_endian_long(0x0a4bc88cu));
                TSIP.REG_1B8H.WORD = 0x00000040u;
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
            else
            {
                RX65NHU_func100(change_endian_long(0x7a377a17u), change_endian_long(0xe412898fu), change_endian_long(0x7c418756u), change_endian_long(0x6ed5b0c7u));
                TSIP.REG_C4H.WORD = 0x00082b8du;
                TSIP.REG_E0H.WORD = 0x81040000u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x0000362eu;
                TSIP.REG_ECH.WORD = 0x38008a20u;
                TSIP.REG_ECH.WORD = 0x00000002u;
                TSIP.REG_ECH.WORD = 0x2000b620u;
                TSIP.REG_ECH.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x00007c11u;
                TSIP.REG_1CH.WORD = 0x00600000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    TSIP.REG_28H.WORD = 0x00bf0001u;
                    TSIP.REG_104H.WORD = 0x00008362u;
                    TSIP.REG_D0H.WORD = 0x00001f00u;
                    TSIP.REG_C4H.WORD = 0x42f087bfu;
                    TSIP.REG_00H.WORD = 0x00013203u;
                    TSIP.REG_2CH.WORD = 0x00000014u;
                    for (iLoop = 0; iLoop < 64; iLoop = iLoop + 4)
                    {
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+4 + 0];
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+4 + 1];
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+4 + 2];
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+4 + 3];
                    }
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_00H.WORD = 0x00013203u;
                    TSIP.REG_2CH.WORD = 0x00000012u;
                    for (iLoop = 0; iLoop < 64; iLoop = iLoop + 4)
                    {
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+68 + 0];
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+68 + 1];
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+68 + 2];
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+68 + 3];
                    }
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x40f007bdu;
                    for (iLoop = 0; iLoop < 4; iLoop = iLoop + 4)
                    {
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+132 + 0];
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+132 + 1];
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+132 + 2];
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+132 + 3];
                    }
                    TSIP.REG_E0H.WORD = 0x80010140u;
                    TSIP.REG_00H.WORD = 0x00008207u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_00H.WORD = 0x0000020fu;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x400007bdu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = InData_PubKey[136];
                    TSIP.REG_100H.WORD = InData_PubKey[137];
                    TSIP.REG_100H.WORD = InData_PubKey[138];
                    TSIP.REG_100H.WORD = InData_PubKey[139];
                    TSIP.REG_C4H.WORD = 0x00900c45u;
                    TSIP.REG_00H.WORD = 0x00002213u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    RX65NHU_func101(change_endian_long(0xf2e1dc55u), change_endian_long(0x839e1c93u), change_endian_long(0xa98ed588u), change_endian_long(0xef516a26u));
                }
                else
                {
                    TSIP.REG_28H.WORD = 0x00870001u;
                    TSIP.REG_104H.WORD = 0x00000f62u;
                    TSIP.REG_D0H.WORD = 0x00000300u;
                    TSIP.REG_C4H.WORD = 0x42f087bfu;
                    TSIP.REG_00H.WORD = 0x00003223u;
                    TSIP.REG_2CH.WORD = 0x0000009bu;
                    for (iLoop = 0; iLoop < 8; iLoop = iLoop + 4)
                    {
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+4 + 0];
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+4 + 1];
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+4 + 2];
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+4 + 3];
                    }
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_00H.WORD = 0x00003223u;
                    TSIP.REG_2CH.WORD = 0x00000091u;
                    for (iLoop = 8; iLoop < 16; iLoop = iLoop + 4)
                    {
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+4 + 0];
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+4 + 1];
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+4 + 2];
                        TSIP.REG_100H.WORD = InData_PubKey[iLoop+4 + 3];
                    }
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_C4H.WORD = 0x400007bdu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = InData_PubKey[20];
                    TSIP.REG_100H.WORD = InData_PubKey[21];
                    TSIP.REG_100H.WORD = InData_PubKey[22];
                    TSIP.REG_100H.WORD = InData_PubKey[23];
                    TSIP.REG_C4H.WORD = 0x00900c45u;
                    TSIP.REG_00H.WORD = 0x00002213u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    RX65NHU_func101(change_endian_long(0x5c587f66u), change_endian_long(0x585c1cc1u), change_endian_long(0xb2ab247du), change_endian_long(0xf5d9d369u));
                }
                RX65NHU_func100(change_endian_long(0x49217632u), change_endian_long(0x2abe9996u), change_endian_long(0x32d83c61u), change_endian_long(0x3e6df034u));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    RX65NHU_func102(change_endian_long(0x986bfe01u), change_endian_long(0x961cdb7cu), change_endian_long(0x8f06ec0eu), change_endian_long(0xeb4b1506u));
                    TSIP.REG_1B8H.WORD = 0x00000040u;
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
                else
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
                    TSIP.REG_74H.WORD = 0x00000002u;
                    TSIP.REG_104H.WORD = 0x00000f64u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = InData_ClientRandom[0];
                    TSIP.REG_100H.WORD = InData_ClientRandom[1];
                    TSIP.REG_100H.WORD = InData_ClientRandom[2];
                    TSIP.REG_100H.WORD = InData_ClientRandom[3];
                    TSIP.REG_100H.WORD = InData_ClientRandom[4];
                    TSIP.REG_100H.WORD = InData_ClientRandom[5];
                    TSIP.REG_100H.WORD = InData_ClientRandom[6];
                    TSIP.REG_100H.WORD = InData_ClientRandom[7];
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = InData_ServerRandom[0];
                    TSIP.REG_100H.WORD = InData_ServerRandom[1];
                    TSIP.REG_100H.WORD = InData_ServerRandom[2];
                    TSIP.REG_100H.WORD = InData_ServerRandom[3];
                    TSIP.REG_100H.WORD = InData_ServerRandom[4];
                    TSIP.REG_100H.WORD = InData_ServerRandom[5];
                    TSIP.REG_100H.WORD = InData_ServerRandom[6];
                    TSIP.REG_100H.WORD = InData_ServerRandom[7];
                    TSIP.REG_ECH.WORD = 0x0000b7c0u;
                    TSIP.REG_ECH.WORD = 0x03001741u;
                    TSIP.REG_E0H.WORD = 0x810103c0u;
                    TSIP.REG_00H.WORD = 0x00005807u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_104H.WORD = 0x00000068u;
                    TSIP.REG_E0H.WORD = 0x800103c0u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = InData_SKE_Message[0];
                    TSIP.REG_E0H.WORD = 0x810103c0u;
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
                    TSIP.REG_D0H.WORD = 0x00000314u;
                    TSIP.REG_C4H.WORD = 0x00008a07u;
                    TSIP.REG_104H.WORD = 0x00000f62u;
                    for (iLoop = 0; iLoop < 16; iLoop = iLoop + 4)
                    {
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = InData_SKE_Message[iLoop+1 + 0];
                        TSIP.REG_100H.WORD = InData_SKE_Message[iLoop+1 + 1];
                        TSIP.REG_100H.WORD = InData_SKE_Message[iLoop+1 + 2];
                        TSIP.REG_100H.WORD = InData_SKE_Message[iLoop+1 + 3];
                    }
                    TSIP.REG_D0H.WORD = 0x0000031cu;
                    TSIP.REG_C4H.WORD = 0x00008a83u;
                    TSIP.REG_00H.WORD = 0x08015203u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_ECH.WORD = 0x0000b7c0u;
                    TSIP.REG_ECH.WORD = 0x80000000u;
                    TSIP.REG_E0H.WORD = 0x810103c0u;
                    TSIP.REG_00H.WORD = 0x0800580fu;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_00H.WORD = 0x08000807u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_F8H.WORD = 0x00000040u;
                    TSIP.REG_104H.WORD = 0x00000154u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000428u);
                    RX65NHU_func100(change_endian_long(0x4a91ecb7u), change_endian_long(0xc68fdd5fu), change_endian_long(0x9d865a95u), change_endian_long(0xeb652952u));
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_74H.BIT.B18)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001600u;
                    TSIP.REG_74H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00007c0eu;
                    TSIP.REG_1CH.WORD = 0x00600000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                    {
                        RX65NHU_func100(change_endian_long(0x2fe4cb0cu), change_endian_long(0x009ae872u), change_endian_long(0x10dc6213u), change_endian_long(0x73f56e0au));
                        TSIP.REG_28H.WORD = 0x00bf0001u;
                        TSIP.REG_2CH.WORD = 0x00000010u;
                        TSIP.REG_104H.WORD = 0x00003f67u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = InData_SKE_Signature[0];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[1];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[2];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[3];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[4];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[5];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[6];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[7];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[8];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[9];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[10];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[11];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[12];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[13];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[14];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[15];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[16];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[17];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[18];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[19];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[20];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[21];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[22];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[23];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[24];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[25];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[26];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[27];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[28];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[29];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[30];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[31];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[32];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[33];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[34];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[35];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[36];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[37];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[38];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[39];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[40];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[41];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[42];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[43];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[44];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[45];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[46];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[47];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[48];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[49];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[50];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[51];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[52];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[53];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[54];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[55];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[56];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[57];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[58];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[59];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[60];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[61];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[62];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[63];
                        TSIP.REG_18H.WORD = 0x00000004u;
                        TSIP.REG_34H.WORD = 0x00000000u;
                        TSIP.REG_38H.WORD = 0x00000338u;
                        TSIP.REG_E0H.WORD = 0x81010140u;
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
                        TSIP.REG_104H.WORD = 0x00003757u;
                        TSIP.REG_2CH.WORD = 0x00000012u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x0001ffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0x00303130u);
                        TSIP.REG_100H.WORD = change_endian_long(0x0d060960u);
                        TSIP.REG_100H.WORD = change_endian_long(0x86480165u);
                        TSIP.REG_100H.WORD = change_endian_long(0x03040201u);
                        TSIP.REG_100H.WORD = change_endian_long(0x05000420u);
                        TSIP.REG_00H.WORD = 0x00003523u;
                        TSIP.REG_74H.WORD = 0x00000008u;
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
                        RX65NHU_func101(change_endian_long(0x2c0d1c34u), change_endian_long(0x050abafeu), change_endian_long(0x242e23dau), change_endian_long(0x10fa94eeu));
                    }
                    else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                    {
                        TSIP.REG_28H.WORD = 0x00bf0001u;
                        TSIP.REG_24H.WORD = 0x000098d0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_ECH.WORD = 0x0000368au;
                        RX65NHU_func100(change_endian_long(0x8c865cbbu), change_endian_long(0x583569a2u), change_endian_long(0x7b21a5fau), change_endian_long(0x00713767u));
                        RX65NHU_func103();
                        RX65NHU_func100(change_endian_long(0x76012fadu), change_endian_long(0x7a01ee46u), change_endian_long(0x58ca6985u), change_endian_long(0x04791f4fu));
                        TSIP.REG_104H.WORD = 0x00000052u;
                        TSIP.REG_C4H.WORD = 0x01000c84u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                        TSIP.REG_E0H.WORD = 0x800402e0u;
                        TSIP.REG_00H.WORD = 0x00008213u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_00H.BIT.B25)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00001800u;
                        TSIP.REG_C4H.WORD = 0x000c0805u;
                        TSIP.REG_E0H.WORD = 0x810402e0u;
                        TSIP.REG_00H.WORD = 0x00002813u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_00H.BIT.B25)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00001800u;
                        RX65NHU_func100(change_endian_long(0x5340c8e7u), change_endian_long(0x5d16e121u), change_endian_long(0xe1ad9321u), change_endian_long(0x84b4a50fu));
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x5fe685f6u);
                        RX65NHU_func080();
                        RX65NHU_func100(change_endian_long(0x1005b135u), change_endian_long(0xc982461du), change_endian_long(0xa2e9497eu), change_endian_long(0x7504a68bu));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000020u;
                        RX65NHU_func313(748);
                        RX65NHU_func100(change_endian_long(0x3292b7edu), change_endian_long(0x24f0793eu), change_endian_long(0xb3bc7e67u), change_endian_long(0x4a066ce5u));
                        RX65NHU_func314(748+64);
                        RX65NHU_func100(change_endian_long(0x72800b19u), change_endian_long(0xd73f6e33u), change_endian_long(0x0c0705b8u), change_endian_long(0x049372e0u));
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x1c5a2b81u);
                        RX65NHU_func080();
                        RX65NHU_func100(change_endian_long(0x20477777u), change_endian_long(0x0728a080u), change_endian_long(0x94be0492u), change_endian_long(0xde9ad973u));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000022u;
                        RX65NHU_func313(680);
                        RX65NHU_func100(change_endian_long(0xad913b45u), change_endian_long(0x6a8d313eu), change_endian_long(0x9977e43cu), change_endian_long(0x59e0f8f8u));
                        RX65NHU_func314(680+64);
                        RX65NHU_func100(change_endian_long(0xa4092b82u), change_endian_long(0x706d071du), change_endian_long(0xcbdb3309u), change_endian_long(0x318e3d2eu));
                        TSIP.REG_2CH.WORD = 0x00000010u;
                        TSIP.REG_104H.WORD = 0x00003f67u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = InData_SKE_Signature[0];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[1];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[2];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[3];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[4];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[5];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[6];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[7];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[8];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[9];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[10];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[11];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[12];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[13];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[14];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[15];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[16];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[17];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[18];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[19];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[20];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[21];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[22];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[23];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[24];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[25];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[26];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[27];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[28];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[29];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[30];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[31];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[32];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[33];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[34];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[35];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[36];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[37];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[38];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[39];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[40];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[41];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[42];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[43];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[44];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[45];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[46];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[47];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[48];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[49];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[50];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[51];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[52];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[53];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[54];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[55];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[56];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[57];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[58];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[59];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[60];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[61];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[62];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[63];
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x5dd049feu);
                        RX65NHU_func080();
                        RX65NHU_func100(change_endian_long(0x230941c8u), change_endian_long(0x52d26171u), change_endian_long(0x5556c363u), change_endian_long(0xec2d1e47u));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000020u;
                        RX65NHU_func313(68);
                        RX65NHU_func100(change_endian_long(0xce97eba4u), change_endian_long(0x9abd4437u), change_endian_long(0x2524c37eu), change_endian_long(0xf0994a92u));
                        RX65NHU_func314(68+64);
                        RX65NHU_func100(change_endian_long(0x5a371394u), change_endian_long(0x33e03957u), change_endian_long(0xbb370dc1u), change_endian_long(0xbb7aacfbu));
                        TSIP.REG_2CH.WORD = 0x00000014u;
                        TSIP.REG_104H.WORD = 0x00003f67u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = InData_SKE_Signature[64];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[65];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[66];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[67];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[68];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[69];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[70];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[71];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[72];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[73];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[74];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[75];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[76];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[77];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[78];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[79];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[80];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[81];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[82];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[83];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[84];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[85];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[86];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[87];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[88];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[89];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[90];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[91];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[92];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[93];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[94];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[95];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[96];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[97];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[98];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[99];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[100];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[101];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[102];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[103];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[104];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[105];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[106];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[107];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[108];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[109];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[110];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[111];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[112];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[113];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[114];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[115];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[116];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[117];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[118];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[119];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[120];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[121];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[122];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[123];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[124];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[125];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[126];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[127];
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x945d2158u);
                        RX65NHU_func080();
                        RX65NHU_func100(change_endian_long(0xac049f40u), change_endian_long(0x7de913b3u), change_endian_long(0x23f3da36u), change_endian_long(0xf11f8297u));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000024u;
                        RX65NHU_func313(0);
                        RX65NHU_func100(change_endian_long(0x89cb8812u), change_endian_long(0x82ea3753u), change_endian_long(0x0503b2b2u), change_endian_long(0x2b393be4u));
                        RX65NHU_func314(0+64);
                        TSIP.REG_ECH.WORD = 0x0000b400u;
                        TSIP.REG_ECH.WORD = 0x000000e7u;
                        RX65NHU_func101(change_endian_long(0x62fef666u), change_endian_long(0x47592c66u), change_endian_long(0x613d59f1u), change_endian_long(0x7cfc0083u));
                        RX65NHU_func307();
                        TSIP.REG_104H.WORD = 0x00003f57u;
                        TSIP.REG_2CH.WORD = 0x00000014u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x0001ffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_24H.WORD = 0x000019c0u;
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
                        TSIP.REG_24H.WORD = 0x00001991u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00210000u;
                        TSIP.REG_104H.WORD = 0x00003757u;
                        TSIP.REG_2CH.WORD = 0x00000014u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0xffffffffu);
                        TSIP.REG_100H.WORD = change_endian_long(0x00303130u);
                        TSIP.REG_100H.WORD = change_endian_long(0x0d060960u);
                        TSIP.REG_100H.WORD = change_endian_long(0x86480165u);
                        TSIP.REG_100H.WORD = change_endian_long(0x03040201u);
                        TSIP.REG_100H.WORD = change_endian_long(0x05000420u);
                        TSIP.REG_00H.WORD = 0x00003523u;
                        TSIP.REG_74H.WORD = 0x00000008u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_00H.BIT.B25)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00001800u;
                        TSIP.REG_24H.WORD = 0x000019c0u;
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
                        TSIP.REG_24H.WORD = 0x000009c0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x00001991u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00210000u;
                        TSIP.REG_ECH.WORD = 0x000036e5u;
                        TSIP.REG_ECH.WORD = 0x00003706u;
                        TSIP.REG_ECH.WORD = 0x00003727u;
                        TSIP.REG_ECH.WORD = 0x00003748u;
                        RX65NHU_func101(change_endian_long(0x4990a264u), change_endian_long(0x6dfc3cadu), change_endian_long(0x51d35835u), change_endian_long(0x1e8fb929u));
                    }
                    else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                    {
                        TSIP.REG_28H.WORD = 0x00870001u;
                        TSIP.REG_ECH.WORD = 0x00000b9cu;
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x800103a0u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x000000e7u);
                        RX65NHU_func101(change_endian_long(0x68aa3580u), change_endian_long(0xd5431e80u), change_endian_long(0xb18287f5u), change_endian_long(0x15f130b9u));
                        RX65NHU_func003(InData_SKE_Signature);
                        RX65NHU_func101(change_endian_long(0x34b1b6a6u), change_endian_long(0x6995b8e8u), change_endian_long(0x63a03b27u), change_endian_long(0x9b6ebcedu));
                    }
                    else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                    {
                        TSIP.REG_28H.WORD = 0x00bf0001u;
                        TSIP.REG_2CH.WORD = 0x00000010u;
                        TSIP.REG_104H.WORD = 0x00003f67u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = InData_SKE_Signature[0];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[1];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[2];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[3];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[4];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[5];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[6];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[7];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[8];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[9];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[10];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[11];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[12];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[13];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[14];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[15];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[16];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[17];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[18];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[19];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[20];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[21];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[22];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[23];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[24];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[25];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[26];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[27];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[28];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[29];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[30];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[31];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[32];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[33];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[34];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[35];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[36];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[37];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[38];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[39];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[40];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[41];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[42];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[43];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[44];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[45];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[46];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[47];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[48];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[49];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[50];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[51];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[52];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[53];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[54];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[55];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[56];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[57];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[58];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[59];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[60];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[61];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[62];
                        TSIP.REG_100H.WORD = InData_SKE_Signature[63];
                        TSIP.REG_ECH.WORD = 0x0000342au;
                        TSIP.REG_ECH.WORD = 0x0000b400u;
                        TSIP.REG_ECH.WORD = 0x000000e7u;
                        TSIP.REG_ECH.WORD = 0x0000b440u;
                        TSIP.REG_ECH.WORD = 0x00000020u;
                        RX65NHU_func101(change_endian_long(0x35551ebdu), change_endian_long(0xb92f300au), change_endian_long(0x39efeb98u), change_endian_long(0xcd0927b8u));
                        RX65NHU_func015();
                        RX65NHU_func101(change_endian_long(0xe77c0a19u), change_endian_long(0x1ce552f9u), change_endian_long(0xa386804cu), change_endian_long(0x724c0354u));
                    }
                    RX65NHU_func100(change_endian_long(0xb32d4913u), change_endian_long(0xc483aaf6u), change_endian_long(0xd4482095u), change_endian_long(0xa0ac94cdu));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX65NHU_func102(change_endian_long(0x402cfbefu), change_endian_long(0x3f0c0cd8u), change_endian_long(0xb2a83fcdu), change_endian_long(0x183c8479u));
                        TSIP.REG_1B8H.WORD = 0x00000040u;
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
                    else
                    {
                        TSIP.REG_28H.WORD = 0x00870001u;
                        TSIP.REG_ECH.WORD = 0x00000b9cu;
                        KEY_ADR = 128;
                        RX65NHU_func004(KEY_ADR);
                        TSIP.REG_24H.WORD = 0x0000dcd0u;
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
                        TSIP.REG_28H.WORD = 0x008f0001u;
                        TSIP.REG_D0H.WORD = 0x0000031cu;
                        TSIP.REG_C4H.WORD = 0x00008a83u;
                        TSIP.REG_00H.WORD = 0x00003243u;
                        TSIP.REG_2CH.WORD = 0x00000010u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_00H.BIT.B25)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00001800u;
                        TSIP.REG_28H.WORD = 0x00870001u;
                        TSIP.REG_34H.WORD = 0x00000004u;
                        TSIP.REG_24H.WORD = 0x800070d0u;
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
                        RX65NHU_func008();
                        RX65NHU_func100(change_endian_long(0x6de57f2eu), change_endian_long(0xc9aac9f4u), change_endian_long(0xf0f5e92du), change_endian_long(0x1a47572cu));
                        TSIP.REG_1CH.WORD = 0x00400000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (1u == (TSIP.REG_1CH.BIT.B22))
                        {
                            RX65NHU_func102(change_endian_long(0x90f651ebu), change_endian_long(0xbae4e679u), change_endian_long(0x2233dbe8u), change_endian_long(0x077f0edbu));
                            TSIP.REG_1B8H.WORD = 0x00000040u;
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
                        else
                        {
                            RX65NHU_func100(change_endian_long(0x4aabffb4u), change_endian_long(0x7adc89c0u), change_endian_long(0x3340e243u), change_endian_long(0xeb091cddu));
                            TSIP.REG_34H.WORD = 0x00000400u;
                            TSIP.REG_24H.WORD = 0x80008cd0u;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_24H.BIT.B21)
                            {
                                /* waiting */
                            }
                            TSIP.REG_34H.WORD = 0x00000404u;
                            TSIP.REG_24H.WORD = 0x80009008u;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_24H.BIT.B21)
                            {
                                /* waiting */
                            }
                            TSIP.REG_34H.WORD = 0x00000002u;
                            TSIP.REG_24H.WORD = 0x80008cd0u;
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
                            TSIP.REG_28H.WORD = 0x008f0001u;
                            TSIP.REG_104H.WORD = 0x00000058u;
                            TSIP.REG_E0H.WORD = 0x80010000u;
                            /* WAIT_LOOP */
                            while (1u != TSIP.REG_104H.BIT.B31)
                            {
                                /* waiting */
                            }
                            TSIP.REG_100H.WORD = change_endian_long(0x31020100u);
                            RX65NHU_func103();
                            TSIP.REG_104H.WORD = 0x00000052u;
                            TSIP.REG_C4H.WORD = 0x01000c84u;
                            /* WAIT_LOOP */
                            while (1u != TSIP.REG_104H.BIT.B31)
                            {
                                /* waiting */
                            }
                            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                            TSIP.REG_E0H.WORD = 0x80030020u;
                            TSIP.REG_00H.WORD = 0x0000820fu;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_00H.BIT.B25)
                            {
                                /* waiting */
                            }
                            TSIP.REG_1CH.WORD = 0x00001800u;
                            TSIP.REG_00H.WORD = 0x00000207u;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_00H.BIT.B25)
                            {
                                /* waiting */
                            }
                            TSIP.REG_1CH.WORD = 0x00001800u;
                            RX65NHU_func100(change_endian_long(0x66b3b21bu), change_endian_long(0x28db5945u), change_endian_long(0xf0779972u), change_endian_long(0x3c9a9111u));
                            TSIP.REG_C4H.WORD = 0x000c2b0du;
                            TSIP.REG_E0H.WORD = 0x81040000u;
                            TSIP.REG_00H.WORD = 0x00002813u;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_00H.BIT.B25)
                            {
                                /* waiting */
                            }
                            TSIP.REG_1CH.WORD = 0x00001800u;
                            TSIP.REG_00H.WORD = 0x00002343u;
                            TSIP.REG_2CH.WORD = 0x00000020u;
                            TSIP.REG_D0H.WORD = 0x00000300u;
                            TSIP.REG_C4H.WORD = 0x02e087bfu;
                            TSIP.REG_04H.WORD = 0x00000242u;
                            for (iLoop = 4; iLoop < 20; iLoop = iLoop + 4)
                            {
                                /* WAIT_LOOP */
                                while (1u != TSIP.REG_04H.BIT.B30)
                                {
                                    /* waiting */
                                }
                                OutData_EphemeralPubKey[iLoop + 0] = TSIP.REG_100H.WORD;
                                OutData_EphemeralPubKey[iLoop + 1] = TSIP.REG_100H.WORD;
                                OutData_EphemeralPubKey[iLoop + 2] = TSIP.REG_100H.WORD;
                                OutData_EphemeralPubKey[iLoop + 3] = TSIP.REG_100H.WORD;
                            }
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_00H.BIT.B25)
                            {
                                /* waiting */
                            }
                            TSIP.REG_1CH.WORD = 0x00001800u;
                            RX65NHU_func100(change_endian_long(0x46c30b32u), change_endian_long(0x023206beu), change_endian_long(0x6d02e43cu), change_endian_long(0xda56ba96u));
                            TSIP.REG_104H.WORD = 0x00000052u;
                            TSIP.REG_C4H.WORD = 0x00000c84u;
                            /* WAIT_LOOP */
                            while (1u != TSIP.REG_104H.BIT.B31)
                            {
                                /* waiting */
                            }
                            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                            TSIP.REG_C4H.WORD = 0x000009cdu;
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
                            OutData_EphemeralPubKey[iLoop + 0] = TSIP.REG_100H.WORD;
                            OutData_EphemeralPubKey[iLoop + 1] = TSIP.REG_100H.WORD;
                            OutData_EphemeralPubKey[iLoop + 2] = TSIP.REG_100H.WORD;
                            OutData_EphemeralPubKey[iLoop + 3] = TSIP.REG_100H.WORD;
                            RX65NHU_func100(change_endian_long(0x077330d0u), change_endian_long(0xc4617312u), change_endian_long(0x1a7fa2cdu), change_endian_long(0x673d486du));
                            TSIP.REG_E0H.WORD = 0x81040000u;
                            TSIP.REG_04H.WORD = 0x00000612u;
                            /* WAIT_LOOP */
                            while (1u != TSIP.REG_04H.BIT.B30)
                            {
                                /* waiting */
                            }
                            OutData_EphemeralPubKey[0] = TSIP.REG_100H.WORD;
                            /* WAIT_LOOP */
                            while (1u != TSIP.REG_04H.BIT.B30)
                            {
                                /* waiting */
                            }
                            OutData_EphemeralPubKey[1] = TSIP.REG_100H.WORD;
                            /* WAIT_LOOP */
                            while (1u != TSIP.REG_04H.BIT.B30)
                            {
                                /* waiting */
                            }
                            OutData_EphemeralPubKey[2] = TSIP.REG_100H.WORD;
                            /* WAIT_LOOP */
                            while (1u != TSIP.REG_04H.BIT.B30)
                            {
                                /* waiting */
                            }
                            OutData_EphemeralPubKey[3] = TSIP.REG_100H.WORD;
                            RX65NHU_func102(change_endian_long(0xc7c2fe21u), change_endian_long(0x1d511819u), change_endian_long(0x93d9a0aau), change_endian_long(0x708db25bu));
                            TSIP.REG_1B8H.WORD = 0x00000040u;
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
                    }
                }
            }
        }
    }
}
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_pe7_r6.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_TLS == 1 */
