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
* @details       RX671 TLS Server's ephemeral ECDH public Key Retrieves
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
    TSIP.REG_E0H.WORD = 0x800101e0u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Sel_PubKeyType[0];
    TSIP.REG_ECH.WORD = 0x00000bffu;
    TSIP.REG_ECH.WORD = 0x38000defu;
    TSIP.REG_ECH.WORD = 0x1000d3e0u;
    TSIP.REG_ECH.WORD = 0x380089e0u;
    TSIP.REG_ECH.WORD = 0x00000002u;
    TSIP.REG_ECH.WORD = 0x1000d3e1u;
    TSIP.REG_ECH.WORD = 0x38000fffu;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    RX671_func100(change_endian_long(0x8f48827au), change_endian_long(0xc6b33bd9u), change_endian_long(0x70c39d61u), change_endian_long(0x388d0298u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func102(change_endian_long(0xae23ecfdu), change_endian_long(0xc4c7a1e8u), change_endian_long(0x955a1b7bu), change_endian_long(0xf005f290u));
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
        RX671_func100(change_endian_long(0x16d90ba3u), change_endian_long(0x05b51b28u), change_endian_long(0x157c386fu), change_endian_long(0xe19ba637u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func102(change_endian_long(0x4dc12913u), change_endian_long(0x9ec98a03u), change_endian_long(0x689233d9u), change_endian_long(0x0af0ae28u));
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
            TSIP.REG_ECH.WORD = 0x300089e0u;
            TSIP.REG_ECH.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00030020u;
            TSIP.REG_ECH.WORD = 0x38008be0u;
            TSIP.REG_ECH.WORD = 0x05000000u;
            TSIP.REG_ECH.WORD = 0x1000d3c0u;
            TSIP.REG_ECH.WORD = 0x00000060u;
            TSIP.REG_ECH.WORD = 0x38008be0u;
            TSIP.REG_ECH.WORD = 0x31000000u;
            TSIP.REG_ECH.WORD = 0x1000d3c0u;
            TSIP.REG_ECH.WORD = 0x00000080u;
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
            RX671_func100(change_endian_long(0x261cd4f3u), change_endian_long(0x0beaedb0u), change_endian_long(0x7a770b54u), change_endian_long(0x89240aa5u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX671_func102(change_endian_long(0x6b3778ccu), change_endian_long(0x0441cbedu), change_endian_long(0x85791befu), change_endian_long(0xcbdd8707u));
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
                RX671_func100(change_endian_long(0x975f09bcu), change_endian_long(0x6b319b3du), change_endian_long(0x2d66e76du), change_endian_long(0xe2673a94u));
                TSIP.REG_C4H.WORD = 0x00082b8du;
                TSIP.REG_E0H.WORD = 0x81040000u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x00007c0fu;
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
                    RX671_func101(change_endian_long(0x62ffc45du), change_endian_long(0x35b99992u), change_endian_long(0xc88cbeeau), change_endian_long(0x40a4afb3u));
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
                    RX671_func101(change_endian_long(0xe3f649aeu), change_endian_long(0xc10ab94cu), change_endian_long(0x5b019cfau), change_endian_long(0x2c14879bu));
                }
                RX671_func100(change_endian_long(0x7c33a91du), change_endian_long(0x5628e8fau), change_endian_long(0x11aa3e83u), change_endian_long(0xdcd47c82u));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    RX671_func102(change_endian_long(0x1ba69f5cu), change_endian_long(0x3d2ac50eu), change_endian_long(0xdd0223d1u), change_endian_long(0x5ffa9557u));
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
                    RX671_func100(change_endian_long(0xf7bd9384u), change_endian_long(0x185413fdu), change_endian_long(0xd040b182u), change_endian_long(0xbff8de65u));
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_74H.BIT.B18)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001600u;
                    TSIP.REG_74H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00007c0fu;
                    TSIP.REG_1CH.WORD = 0x00600000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                    {
                        RX671_func100(change_endian_long(0xf928cce0u), change_endian_long(0x2b8db646u), change_endian_long(0xe216799bu), change_endian_long(0xf6bd941cu));
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
                        RX671_func101(change_endian_long(0x66a08ec0u), change_endian_long(0xffce38e1u), change_endian_long(0x141d9ac4u), change_endian_long(0x7559ca10u));
                    }
                    else
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
                        RX671_func101(change_endian_long(0xbd3b7b68u), change_endian_long(0xcdfa95beu), change_endian_long(0x7a90b67fu), change_endian_long(0x5972d103u));
                        RX671_func003(InData_SKE_Signature);
                        RX671_func101(change_endian_long(0xeb136cf4u), change_endian_long(0x1fae990bu), change_endian_long(0x6d685fc3u), change_endian_long(0x6da8f366u));
                    }
                    RX671_func100(change_endian_long(0x5b4cfc14u), change_endian_long(0x4568220fu), change_endian_long(0xd1a11325u), change_endian_long(0x6a4882c9u));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX671_func102(change_endian_long(0xfacb8f96u), change_endian_long(0xd7689864u), change_endian_long(0xffba3ea8u), change_endian_long(0xfc65b687u));
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
                        OFS_ADR = 128;
                        RX671_func004(OFS_ADR);
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
                        RX671_func008();
                        RX671_func100(change_endian_long(0xb0965bceu), change_endian_long(0xd4fdcf6au), change_endian_long(0x197cffdeu), change_endian_long(0x7c34bd22u));
                        TSIP.REG_1CH.WORD = 0x00400000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (1u == (TSIP.REG_1CH.BIT.B22))
                        {
                            RX671_func102(change_endian_long(0xb8227f82u), change_endian_long(0xe1ce2ac4u), change_endian_long(0x11669aabu), change_endian_long(0x90ecf32au));
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
                            RX671_func100(change_endian_long(0x2d64668du), change_endian_long(0x2ef60ddau), change_endian_long(0xd69007a8u), change_endian_long(0x1ea05877u));
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
                            RX671_func103();
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
                            RX671_func100(change_endian_long(0xb6726cbcu), change_endian_long(0xda3b86eeu), change_endian_long(0xd5f4db98u), change_endian_long(0xd218c68cu));
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
                            RX671_func100(change_endian_long(0x7ce438bcu), change_endian_long(0x8fede80fu), change_endian_long(0x403a5baeu), change_endian_long(0x21b54e63u));
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
                            TSIP.REG_04H.WORD = 0x00000213u;
                            /* WAIT_LOOP */
                            while (1u != TSIP.REG_04H.BIT.B30)
                            {
                                /* waiting */
                            }
                            OutData_EphemeralPubKey[iLoop + 0] = TSIP.REG_100H.WORD;
                            OutData_EphemeralPubKey[iLoop + 1] = TSIP.REG_100H.WORD;
                            OutData_EphemeralPubKey[iLoop + 2] = TSIP.REG_100H.WORD;
                            OutData_EphemeralPubKey[iLoop + 3] = TSIP.REG_100H.WORD;
                            RX671_func100(change_endian_long(0xfb26a916u), change_endian_long(0xeaa2b310u), change_endian_long(0x0fd298acu), change_endian_long(0x92cf9251u));
                            TSIP.REG_E0H.WORD = 0x81040000u;
                            TSIP.REG_04H.WORD = 0x00000613u;
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
                            RX671_func102(change_endian_long(0x39c8321cu), change_endian_long(0x3ceb5afau), change_endian_long(0x4827ddffu), change_endian_long(0x78ee675eu));
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
 End of function ./input_dir/RX671/RX671_pe7.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_TLS == 1 */
