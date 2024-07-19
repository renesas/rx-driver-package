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
* Function Name: R_TSIP_TlsCertificateVerificationSub
*******************************************************************************************************************/ /**
* @details       RX65NHU TLS Certificate Verification
* @param[in]     InData_Sel_InDataPubKeyType
* @param[in]     InData_Sel_OutDataPubKeyType
* @param[in]     InData_PubKey
* @param[in]     InData_TBSCertificate
* @param[in]     InData_TBSCertificateLength
* @param[in]     InData_Signature
* @param[in]     InData_TBSCertificateInfo
* @param[out]    OutData_PubKey
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @note          None
*/
e_tsip_err_t R_TSIP_TlsCertificateVerificationSub(uint32_t *InData_Sel_InDataPubKeyType, uint32_t *InData_Sel_OutDataPubKeyType, uint32_t *InData_PubKey, uint32_t *InData_TBSCertificate, uint32_t *InData_TBSCertificateLength, uint32_t *InData_Signature, uint32_t *InData_TBSCertificateInfo, uint32_t *OutData_PubKey)
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
    TSIP.REG_84H.WORD = 0x0000e101u;
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
    TSIP.REG_104H.WORD = 0x00000168u;
    TSIP.REG_E0H.WORD = 0x800201c0u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Sel_InDataPubKeyType[0];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Sel_OutDataPubKeyType[0];
    TSIP.REG_ECH.WORD = 0x3420a9c0u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    TSIP.REG_ECH.WORD = 0x380089e0u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_ECH.WORD = 0x3420a9e0u;
    TSIP.REG_ECH.WORD = 0x00000006u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX65NHU_func100(change_endian_long(0x4d398749u), change_endian_long(0xe11a0787u), change_endian_long(0xa4c5de76u), change_endian_long(0x781d8ce9u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0xd8563a46u), change_endian_long(0x378b1660u), change_endian_long(0x34720660u), change_endian_long(0x08ee00e4u));
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
        TSIP.REG_ECH.WORD = 0x000035afu;
        TSIP.REG_ECH.WORD = 0x380089a0u;
        TSIP.REG_ECH.WORD = 0x00000002u;
        TSIP.REG_ECH.WORD = 0x2000b5a0u;
        TSIP.REG_ECH.WORD = 0x00000000u;
        TSIP.REG_104H.WORD = 0x00000068u;
        TSIP.REG_E0H.WORD = 0x800102c0u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_TBSCertificateLength[0];
        TSIP.REG_ECH.WORD = 0x380089e0u;
        TSIP.REG_ECH.WORD = 0x00000005u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00A60000u;
        RX65NHU_func100(change_endian_long(0x9134a145u), change_endian_long(0xb3633043u), change_endian_long(0x24ac9084u), change_endian_long(0xae1eac22u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func100(change_endian_long(0x796aac7cu), change_endian_long(0x9b59fc28u), change_endian_long(0xfecd046eu), change_endian_long(0x02676957u));
            TSIP.REG_104H.WORD = 0x00000368u;
            TSIP.REG_E0H.WORD = 0x800402e0u;
            for (iLoop = 0; iLoop < 4; iLoop = iLoop + 1)
            {
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_TBSCertificateInfo[iLoop + 0];
            }
            TSIP.REG_ECH.WORD = 0x00007c0du;
            TSIP.REG_1CH.WORD = 0x00600000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                TSIP.REG_ECH.WORD = 0x3c002af8u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_ECH.WORD = 0x3c002b19u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_ECH.WORD = 0x3c002b59u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00A60000u;
                TSIP.REG_ECH.WORD = 0x3c002b56u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_ECH.WORD = 0x38000defu;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_ECH.WORD = 0x380089e0u;
                TSIP.REG_ECH.WORD = 0x00000003u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                RX65NHU_func100(change_endian_long(0x8f90dba2u), change_endian_long(0x0f76f9a0u), change_endian_long(0x3aa5fceeu), change_endian_long(0xc158b2dbu));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    TSIP.REG_ECH.WORD = 0x00003417u;
                    TSIP.REG_ECH.WORD = 0x0000a400u;
                    TSIP.REG_ECH.WORD = 0x000000ffu;
                    TSIP.REG_ECH.WORD = 0x08002818u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00A60000u;
                    RX65NHU_func101(change_endian_long(0xac60ea56u), change_endian_long(0xa8ef7a5fu), change_endian_long(0x3c6c98aeu), change_endian_long(0x3f55832bu));
                }
                else
                {
                    TSIP.REG_ECH.WORD = 0x00003417u;
                    TSIP.REG_ECH.WORD = 0x0000a400u;
                    TSIP.REG_ECH.WORD = 0x000001ffu;
                    TSIP.REG_ECH.WORD = 0x08002818u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00A60000u;
                    RX65NHU_func101(change_endian_long(0x0cf84800u), change_endian_long(0x814861f5u), change_endian_long(0x3260a9e9u), change_endian_long(0x1421f17fu));
                }
                TSIP.REG_ECH.WORD = 0x00003419u;
                TSIP.REG_ECH.WORD = 0x0000a400u;
                TSIP.REG_ECH.WORD = 0x00000003u;
                TSIP.REG_ECH.WORD = 0x0c00281au;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00A60000u;
                RX65NHU_func101(change_endian_long(0x03cae179u), change_endian_long(0x95b3fa70u), change_endian_long(0xb9af60b8u), change_endian_long(0x45fb668au));
            }
            else
            {
                TSIP.REG_ECH.WORD = 0x3c002af8u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_ECH.WORD = 0x3c002b19u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_ECH.WORD = 0x3c002b3au;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_ECH.WORD = 0x3c002b56u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_ECH.WORD = 0x00003417u;
                TSIP.REG_ECH.WORD = 0x0000a400u;
                TSIP.REG_ECH.WORD = 0x0000001fu;
                TSIP.REG_ECH.WORD = 0x08002818u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00A60000u;
                TSIP.REG_ECH.WORD = 0x00003419u;
                TSIP.REG_ECH.WORD = 0x0000a400u;
                TSIP.REG_ECH.WORD = 0x0000001fu;
                TSIP.REG_ECH.WORD = 0x0800281au;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00A60000u;
                RX65NHU_func101(change_endian_long(0xb7eb5967u), change_endian_long(0x0cd9822au), change_endian_long(0xf223b253u), change_endian_long(0x6704f646u));
            }
            RX65NHU_func101(change_endian_long(0x773f5692u), change_endian_long(0x5657d0f3u), change_endian_long(0x34219da4u), change_endian_long(0xc75e2d8bu));
        }
        else
        {
            RX65NHU_func101(change_endian_long(0x3f87b6b6u), change_endian_long(0x0c7f53b4u), change_endian_long(0x46d10810u), change_endian_long(0x0a98dd66u));
        }
        RX65NHU_func100(change_endian_long(0xdc90a91fu), change_endian_long(0xdcef9f28u), change_endian_long(0xc2669466u), change_endian_long(0xafe0df40u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func102(change_endian_long(0xfb2d52e8u), change_endian_long(0xa9932d16u), change_endian_long(0xa2afd3a0u), change_endian_long(0x17155ab0u));
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
            RX65NHU_func100(change_endian_long(0x75941374u), change_endian_long(0x35a85d54u), change_endian_long(0x50172891u), change_endian_long(0x8e1c0dd8u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX65NHU_func102(change_endian_long(0xbc635c01u), change_endian_long(0xe724b394u), change_endian_long(0xcbaac3c8u), change_endian_long(0x6e27b505u));
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
                RX65NHU_func100(change_endian_long(0xa7596eabu), change_endian_long(0x358f3b75u), change_endian_long(0x47f2ae4eu), change_endian_long(0x3c3b428cu));
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
                    RX65NHU_func101(change_endian_long(0xbe399b84u), change_endian_long(0x33fb7549u), change_endian_long(0x0dd15493u), change_endian_long(0xb5db1e5bu));
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
                    TSIP.REG_2CH.WORD = 0x00000012u;
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
                    RX65NHU_func101(change_endian_long(0x352d8549u), change_endian_long(0x4c608b95u), change_endian_long(0x825fa3a0u), change_endian_long(0x098e9a90u));
                }
                RX65NHU_func100(change_endian_long(0x353ce4ccu), change_endian_long(0x6dd0f88au), change_endian_long(0x25280398u), change_endian_long(0x5c24118fu));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    RX65NHU_func102(change_endian_long(0x9d19e69eu), change_endian_long(0x813fba6bu), change_endian_long(0x3799527du), change_endian_long(0x521f7f1eu));
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
                    TSIP.REG_ECH.WORD = 0x380089e0u;
                    TSIP.REG_ECH.WORD = 0x00000005u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00A60000u;
                    RX65NHU_func100(change_endian_long(0x1ca47f77u), change_endian_long(0x6327f61cu), change_endian_long(0xf8cb7397u), change_endian_long(0x7827fe7du));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX65NHU_func100(change_endian_long(0xf5c7c172u), change_endian_long(0xea065619u), change_endian_long(0xa844a5a1u), change_endian_long(0x9bede4c5u));
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
                        TSIP.REG_ECH.WORD = 0x00003417u;
                        TSIP.REG_ECH.WORD = 0x00046800u;
                        TSIP.REG_ECH.WORD = 0x00026c00u;
                        TSIP.REG_E0H.WORD = 0x81010000u;
                        TSIP.REG_04H.WORD = 0x00000606u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
                        TSIP.REG_ECH.WORD = 0x000008a5u;
                        for (iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 4)
                        {
                            TSIP.REG_104H.WORD = 0x00000364u;
                            /* WAIT_LOOP */
                            while (1u != TSIP.REG_104H.BIT.B31)
                            {
                                /* waiting */
                            }
                            TSIP.REG_100H.WORD = InData_TBSCertificate[iLoop + 0];
                            TSIP.REG_100H.WORD = InData_TBSCertificate[iLoop + 1];
                            TSIP.REG_100H.WORD = InData_TBSCertificate[iLoop + 2];
                            TSIP.REG_100H.WORD = InData_TBSCertificate[iLoop + 3];
                            TSIP.REG_ECH.WORD = 0x0000a4a0u;
                            TSIP.REG_ECH.WORD = 0x00000004u;
                            RX65NHU_func101(change_endian_long(0x0dd09182u), change_endian_long(0x5486df50u), change_endian_long(0x9d044c90u), change_endian_long(0x81e1eeb3u));
                        }
                        RX65NHU_func100(change_endian_long(0x3f1c73dbu), change_endian_long(0x191428a3u), change_endian_long(0x970a270au), change_endian_long(0xe3a642aeu));
                        TSIP.REG_ECH.WORD = 0x38000805u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        TSIP.REG_1CH.WORD = 0x00402000u;
                        TSIP.REG_ECH.WORD = 0x00007c0du;
                        TSIP.REG_1CH.WORD = 0x00600000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                        {
                            RX65NHU_func100(change_endian_long(0x967bf1a4u), change_endian_long(0xa9ba4922u), change_endian_long(0x894cf888u), change_endian_long(0x566fd3e3u));
                            TSIP.REG_ECH.WORD = 0x0000096bu;
                            TSIP.REG_ECH.WORD = 0x0000098cu;
                            TSIP.REG_28H.WORD = 0x00bf0001u;
                            TSIP.REG_2CH.WORD = 0x00000010u;
                            TSIP.REG_ECH.WORD = 0x00003416u;
                            TSIP.REG_ECH.WORD = 0x00026800u;
                            TSIP.REG_ECH.WORD = 0x38008ec0u;
                            TSIP.REG_ECH.WORD = 0x00000003u;
                            TSIP.REG_ECH.WORD = 0x20002c00u;
                            TSIP.REG_E0H.WORD = 0x81010000u;
                            TSIP.REG_04H.WORD = 0x00000606u;
                            /* WAIT_LOOP */
                            while (1u != TSIP.REG_04H.BIT.B30)
                            {
                                /* waiting */
                            }
                            S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
                            TSIP.REG_ECH.WORD = 0x00003445u;
                            TSIP.REG_ECH.WORD = 0x00026c42u;
                            TSIP.REG_ECH.WORD = 0x000034d6u;
                            TSIP.REG_ECH.WORD = 0x000030c0u;
                            TSIP.REG_ECH.WORD = 0x00000a10u;
                            for ( ; iLoop < S_RAM[0]; iLoop = iLoop + 1)
                            {
                                TSIP.REG_104H.WORD = 0x00000068u;
                                TSIP.REG_E0H.WORD = 0x80010100u;
                                /* WAIT_LOOP */
                                while (1u != TSIP.REG_104H.BIT.B31)
                                {
                                    /* waiting */
                                }
                                TSIP.REG_100H.WORD = InData_TBSCertificate[iLoop + 0];
                                TSIP.REG_ECH.WORD = 0x0000b420u;
                                TSIP.REG_ECH.WORD = 0x00000004u;
                                for (jLoop = 0; jLoop < 4; jLoop = jLoop + 1)
                                {
                                    TSIP.REG_ECH.WORD = 0x00003020u;
                                    TSIP.REG_ECH.WORD = 0x01886ce8u;
                                    TSIP.REG_ECH.WORD = 0x00086d08u;
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c002859u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x3c002b42u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x30000c63u;
                                    TSIP.REG_ECH.WORD = 0x00030020u;
                                    TSIP.REG_ECH.WORD = 0x00186ce7u;
                                    TSIP.REG_ECH.WORD = 0x01886e07u;
                                    TSIP.REG_ECH.WORD = 0x001868e7u;
                                    TSIP.REG_ECH.WORD = 0x00000080u;
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c002857u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x3c002b02u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX65NHU_func100(change_endian_long(0x536e0074u), change_endian_long(0x564d971du), change_endian_long(0xd4c778bbu), change_endian_long(0xb771d950u));
                                    TSIP.REG_1CH.WORD = 0x00400000u;
                                    TSIP.REG_1D0H.WORD = 0x00000000u;
                                    if (1u == (TSIP.REG_1CH.BIT.B22))
                                    {
                                        TSIP.REG_ECH.WORD = 0x3420a980u;
                                        TSIP.REG_ECH.WORD = 0x00000100u;
                                        TSIP.REG_E0H.WORD = 0x00000080u;
                                        TSIP.REG_1CH.WORD = 0x00260000u;
                                        RX65NHU_func100(change_endian_long(0xcff2c171u), change_endian_long(0xd1188254u), change_endian_long(0x9c0ae456u), change_endian_long(0x8d7ed1a2u));
                                        TSIP.REG_1CH.WORD = 0x00400000u;
                                        TSIP.REG_1D0H.WORD = 0x00000000u;
                                        if (1u == (TSIP.REG_1CH.BIT.B22))
                                        {
                                            TSIP.REG_E0H.WORD = 0x810100e0u;
                                            TSIP.REG_00H.WORD = 0x0800080fu;
                                            /* WAIT_LOOP */
                                            while (0u != TSIP.REG_00H.BIT.B25)
                                            {
                                                /* waiting */
                                            }
                                            TSIP.REG_1CH.WORD = 0x00001800u;
                                            TSIP.REG_00H.WORD = 0x08003807u;
                                            /* WAIT_LOOP */
                                            while (0u != TSIP.REG_00H.BIT.B25)
                                            {
                                                /* waiting */
                                            }
                                            TSIP.REG_1CH.WORD = 0x00001800u;
                                            TSIP.REG_ECH.WORD = 0x0000a580u;
                                            TSIP.REG_ECH.WORD = 0x00000001u;
                                            RX65NHU_func101(change_endian_long(0x07ac78f7u), change_endian_long(0x84789f7au), change_endian_long(0x736dadd4u), change_endian_long(0xda4416e2u));
                                        }
                                        else
                                        {
                                            TSIP.REG_ECH.WORD = 0x02003cebu;
                                            TSIP.REG_ECH.WORD = 0x0000a560u;
                                            TSIP.REG_ECH.WORD = 0x00000001u;
                                            RX65NHU_func101(change_endian_long(0x111aa967u), change_endian_long(0x6055a4bdu), change_endian_long(0x444e8194u), change_endian_long(0x5e33b9a7u));
                                        }
                                        RX65NHU_func101(change_endian_long(0x9499ac6du), change_endian_long(0x4fad8f3bu), change_endian_long(0x40426037u), change_endian_long(0xe78afe35u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c0028c2u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX65NHU_func100(change_endian_long(0x20927a0fu), change_endian_long(0x338841bdu), change_endian_long(0x093b6797u), change_endian_long(0xfa79cc18u));
                                    TSIP.REG_1CH.WORD = 0x00400000u;
                                    TSIP.REG_1D0H.WORD = 0x00000000u;
                                    if (1u == (TSIP.REG_1CH.BIT.B22))
                                    {
                                        TSIP.REG_E0H.WORD = 0x810100e0u;
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
                                        RX65NHU_func101(change_endian_long(0x2ff996d3u), change_endian_long(0x6ff3115du), change_endian_long(0x2af36c4bu), change_endian_long(0x612918aeu));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00002c40u;
                                    RX65NHU_func101(change_endian_long(0xfae15647u), change_endian_long(0xbee001eau), change_endian_long(0x6ebb47d0u), change_endian_long(0x4a364f42u));
                                }
                                TSIP.REG_ECH.WORD = 0x08000c21u;
                                TSIP.REG_E0H.WORD = 0x00000080u;
                                TSIP.REG_1CH.WORD = 0x00260000u;
                                TSIP.REG_1CH.WORD = 0x00402000u;
                                TSIP.REG_ECH.WORD = 0x00002ca0u;
                                RX65NHU_func101(change_endian_long(0xa4309a16u), change_endian_long(0x27d4fc7au), change_endian_long(0xa349d610u), change_endian_long(0x6f9b8123u));
                            }
                            TSIP.REG_ECH.WORD = 0x38000805u;
                            TSIP.REG_E0H.WORD = 0x00000080u;
                            TSIP.REG_1CH.WORD = 0x00260000u;
                            TSIP.REG_1CH.WORD = 0x00402000u;
                            RX65NHU_func101(change_endian_long(0x51e73de6u), change_endian_long(0x772becc8u), change_endian_long(0xcb9388bcu), change_endian_long(0x5eee7aadu));
                        }
                        else
                        {
                            RX65NHU_func100(change_endian_long(0xf8a68694u), change_endian_long(0x2bd5f852u), change_endian_long(0x63e485bcu), change_endian_long(0xaef92eb9u));
                            TSIP.REG_28H.WORD = 0x008f0001u;
                            TSIP.REG_2CH.WORD = 0x00000010u;
                            TSIP.REG_ECH.WORD = 0x00003416u;
                            TSIP.REG_ECH.WORD = 0x00026800u;
                            TSIP.REG_ECH.WORD = 0x38008ec0u;
                            TSIP.REG_ECH.WORD = 0x00000003u;
                            TSIP.REG_ECH.WORD = 0x20002c00u;
                            TSIP.REG_E0H.WORD = 0x81010000u;
                            TSIP.REG_04H.WORD = 0x00000606u;
                            /* WAIT_LOOP */
                            while (1u != TSIP.REG_04H.BIT.B30)
                            {
                                /* waiting */
                            }
                            S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
                            TSIP.REG_ECH.WORD = 0x00003445u;
                            TSIP.REG_ECH.WORD = 0x00026c42u;
                            TSIP.REG_ECH.WORD = 0x000034d6u;
                            TSIP.REG_ECH.WORD = 0x000030c0u;
                            TSIP.REG_ECH.WORD = 0x00000a10u;
                            for ( ; iLoop < S_RAM[0]; iLoop = iLoop + 1)
                            {
                                TSIP.REG_104H.WORD = 0x00000068u;
                                TSIP.REG_E0H.WORD = 0x80010100u;
                                /* WAIT_LOOP */
                                while (1u != TSIP.REG_104H.BIT.B31)
                                {
                                    /* waiting */
                                }
                                TSIP.REG_100H.WORD = InData_TBSCertificate[iLoop + 0];
                                TSIP.REG_ECH.WORD = 0x0000b420u;
                                TSIP.REG_ECH.WORD = 0x00000004u;
                                for (jLoop = 0; jLoop < 4; jLoop = jLoop + 1)
                                {
                                    TSIP.REG_ECH.WORD = 0x00003020u;
                                    TSIP.REG_ECH.WORD = 0x01886ce8u;
                                    TSIP.REG_ECH.WORD = 0x00086d08u;
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c002859u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x3c002b42u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX65NHU_func100(change_endian_long(0x26bebb5du), change_endian_long(0xcb499362u), change_endian_long(0x376a8b48u), change_endian_long(0x4eacb2f8u));
                                    TSIP.REG_1CH.WORD = 0x00400000u;
                                    TSIP.REG_1D0H.WORD = 0x00000000u;
                                    if (1u == (TSIP.REG_1CH.BIT.B22))
                                    {
                                        TSIP.REG_E0H.WORD = 0x810100e0u;
                                        TSIP.REG_00H.WORD = 0x0800080fu;
                                        /* WAIT_LOOP */
                                        while (0u != TSIP.REG_00H.BIT.B25)
                                        {
                                            /* waiting */
                                        }
                                        TSIP.REG_1CH.WORD = 0x00001800u;
                                        TSIP.REG_00H.WORD = 0x08003807u;
                                        /* WAIT_LOOP */
                                        while (0u != TSIP.REG_00H.BIT.B25)
                                        {
                                            /* waiting */
                                        }
                                        TSIP.REG_1CH.WORD = 0x00001800u;
                                        RX65NHU_func101(change_endian_long(0x997aef84u), change_endian_long(0xd4cbc2f4u), change_endian_long(0xa7a28bcbu), change_endian_long(0xd97f0ba7u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c002857u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x3c002b02u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX65NHU_func100(change_endian_long(0xa0b7193eu), change_endian_long(0xb201aa39u), change_endian_long(0x2851b6e9u), change_endian_long(0xed72e97eu));
                                    TSIP.REG_1CH.WORD = 0x00400000u;
                                    TSIP.REG_1D0H.WORD = 0x00000000u;
                                    if (1u == (TSIP.REG_1CH.BIT.B22))
                                    {
                                        TSIP.REG_E0H.WORD = 0x810100e0u;
                                        TSIP.REG_00H.WORD = 0x0800080fu;
                                        /* WAIT_LOOP */
                                        while (0u != TSIP.REG_00H.BIT.B25)
                                        {
                                            /* waiting */
                                        }
                                        TSIP.REG_1CH.WORD = 0x00001800u;
                                        TSIP.REG_00H.WORD = 0x08003807u;
                                        /* WAIT_LOOP */
                                        while (0u != TSIP.REG_00H.BIT.B25)
                                        {
                                            /* waiting */
                                        }
                                        TSIP.REG_1CH.WORD = 0x00001800u;
                                        RX65NHU_func101(change_endian_long(0x634691efu), change_endian_long(0x809eb77au), change_endian_long(0x91bc86a0u), change_endian_long(0x28791450u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c0028c2u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX65NHU_func100(change_endian_long(0x93fb0143u), change_endian_long(0x695414a2u), change_endian_long(0x77544c7cu), change_endian_long(0x24d8e7edu));
                                    TSIP.REG_1CH.WORD = 0x00400000u;
                                    TSIP.REG_1D0H.WORD = 0x00000000u;
                                    if (1u == (TSIP.REG_1CH.BIT.B22))
                                    {
                                        TSIP.REG_E0H.WORD = 0x810100e0u;
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
                                        RX65NHU_func101(change_endian_long(0x07a8450bu), change_endian_long(0x7573b4fdu), change_endian_long(0x7f99dc39u), change_endian_long(0xcf5ba1c5u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00002c40u;
                                    RX65NHU_func101(change_endian_long(0x8dc7e22fu), change_endian_long(0x9265866eu), change_endian_long(0xa3216b1du), change_endian_long(0x1d722110u));
                                }
                                TSIP.REG_ECH.WORD = 0x08000c21u;
                                TSIP.REG_E0H.WORD = 0x00000080u;
                                TSIP.REG_1CH.WORD = 0x00260000u;
                                TSIP.REG_1CH.WORD = 0x00402000u;
                                TSIP.REG_ECH.WORD = 0x00002ca0u;
                                RX65NHU_func101(change_endian_long(0x7e5115b2u), change_endian_long(0xe16d1325u), change_endian_long(0xaa35c43fu), change_endian_long(0xd5b2d496u));
                            }
                            TSIP.REG_ECH.WORD = 0x38000805u;
                            TSIP.REG_E0H.WORD = 0x00000080u;
                            TSIP.REG_1CH.WORD = 0x00260000u;
                            TSIP.REG_1CH.WORD = 0x00402000u;
                            RX65NHU_func101(change_endian_long(0xf83c4b9eu), change_endian_long(0xb7d913f9u), change_endian_long(0xbb44bfd6u), change_endian_long(0xdf658702u));
                        }
                        RX65NHU_func101(change_endian_long(0x62ae6ef4u), change_endian_long(0xf90282dau), change_endian_long(0x4025c263u), change_endian_long(0xd971f52bu));
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
                        RX65NHU_func100(change_endian_long(0x238b9c4au), change_endian_long(0x653d5d76u), change_endian_long(0x5f0c0bddu), change_endian_long(0x4d296bd1u));
                        TSIP.REG_ECH.WORD = 0x00003416u;
                        TSIP.REG_ECH.WORD = 0x0000a400u;
                        TSIP.REG_ECH.WORD = 0x00000003u;
                        TSIP.REG_ECH.WORD = 0x00026800u;
                        TSIP.REG_ECH.WORD = 0x00003000u;
                        TSIP.REG_E0H.WORD = 0x81010000u;
                        TSIP.REG_04H.WORD = 0x00000606u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
                        TSIP.REG_ECH.WORD = 0x000008a5u;
                        for (iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 1)
                        {
                            TSIP.REG_104H.WORD = 0x00000064u;
                            /* WAIT_LOOP */
                            while (1u != TSIP.REG_104H.BIT.B31)
                            {
                                /* waiting */
                            }
                            TSIP.REG_100H.WORD = InData_TBSCertificate[iLoop + 0];
                            TSIP.REG_ECH.WORD = 0x0000a4a0u;
                            TSIP.REG_ECH.WORD = 0x00000001u;
                            RX65NHU_func101(change_endian_long(0x2a3a9da4u), change_endian_long(0x2b695e2du), change_endian_long(0x816683e2u), change_endian_long(0xc67dbcc7u));
                        }
                        TSIP.REG_ECH.WORD = 0x38000805u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        TSIP.REG_1CH.WORD = 0x00402000u;
                        RX65NHU_func100(change_endian_long(0x1bdf74c3u), change_endian_long(0xef667128u), change_endian_long(0x81a95950u), change_endian_long(0xbce7ac1fu));
                        TSIP.REG_104H.WORD = 0x00000068u;
                        TSIP.REG_E0H.WORD = 0x80010060u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = InData_TBSCertificate[iLoop + 0];
                        TSIP.REG_ECH.WORD = 0x00003416u;
                        TSIP.REG_ECH.WORD = 0x00008c00u;
                        TSIP.REG_ECH.WORD = 0x00000003u;
                        TSIP.REG_ECH.WORD = 0x38000c00u;
                        TSIP.REG_ECH.WORD = 0x00030020u;
                        TSIP.REG_ECH.WORD = 0x0000b400u;
                        TSIP.REG_ECH.WORD = 0x00000004u;
                        TSIP.REG_ECH.WORD = 0x00000080u;
                        TSIP.REG_E0H.WORD = 0x81010000u;
                        TSIP.REG_04H.WORD = 0x00000606u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
                        TSIP.REG_ECH.WORD = 0x000008a5u;
                        for (iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 1)
                        {
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
                            TSIP.REG_ECH.WORD = 0x00002ca0u;
                            RX65NHU_func101(change_endian_long(0x9086280fu), change_endian_long(0x84ec0d5eu), change_endian_long(0x4369d465u), change_endian_long(0x385474e6u));
                        }
                        TSIP.REG_ECH.WORD = 0x38000805u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        TSIP.REG_1CH.WORD = 0x00402000u;
                        RX65NHU_func101(change_endian_long(0xce147adbu), change_endian_long(0xdf8ce0aau), change_endian_long(0xe89abaf7u), change_endian_long(0x2f01a1cau));
                    }
                    RX65NHU_func100(change_endian_long(0xc07cfe56u), change_endian_long(0x9921bfdcu), change_endian_long(0x7b5be2aeu), change_endian_long(0xde8f509bu));
                    TSIP.REG_ECH.WORD = 0x00003416u;
                    TSIP.REG_ECH.WORD = 0x00008c00u;
                    TSIP.REG_ECH.WORD = 0x00000003u;
                    TSIP.REG_ECH.WORD = 0x0000b440u;
                    TSIP.REG_ECH.WORD = 0x00000004u;
                    TSIP.REG_ECH.WORD = 0x00002840u;
                    TSIP.REG_E0H.WORD = 0x81010040u;
                    TSIP.REG_04H.WORD = 0x00000606u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
                    TSIP.REG_ECH.WORD = 0x000008a5u;
                    TSIP.REG_ECH.WORD = 0x0000b460u;
                    TSIP.REG_ECH.WORD = 0x80000000u;
                    for (iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 1)
                    {
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
                        TSIP.REG_ECH.WORD = 0x00002ca0u;
                        RX65NHU_func101(change_endian_long(0xe936da8bu), change_endian_long(0xec7f9a6eu), change_endian_long(0x471c1cb4u), change_endian_long(0x32d24f17u));
                    }
                    TSIP.REG_ECH.WORD = 0x38000845u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00260000u;
                    TSIP.REG_1CH.WORD = 0x00402000u;
                    TSIP.REG_F8H.WORD = 0x00000040u;
                    TSIP.REG_ECH.WORD = 0x00000800u;
                    TSIP.REG_ECH.WORD = 0x00003436u;
                    TSIP.REG_ECH.WORD = 0x01836c01u;
                    TSIP.REG_ECH.WORD = 0x00036c21u;
                    TSIP.REG_E0H.WORD = 0x81020000u;
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
                    RX65NHU_func100(change_endian_long(0x7840842eu), change_endian_long(0x4ef25bfcu), change_endian_long(0xf1219168u), change_endian_long(0xe22b989fu));
                    RX65NHU_func103();
                    RX65NHU_func100(change_endian_long(0xe3ec385au), change_endian_long(0x9ae14186u), change_endian_long(0x5df07824u), change_endian_long(0x77b4fbcfu));
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
                    TSIP.REG_ECH.WORD = 0x380089e0u;
                    TSIP.REG_ECH.WORD = 0x00000005u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00A60000u;
                    RX65NHU_func100(change_endian_long(0x87d55017u), change_endian_long(0x3e1eec21u), change_endian_long(0xc80606f2u), change_endian_long(0x11a859bau));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX65NHU_func100(change_endian_long(0x9c5b4b29u), change_endian_long(0x32dbe877u), change_endian_long(0x2e5fea6eu), change_endian_long(0xf3938b02u));
                        TSIP.REG_ECH.WORD = 0x00007c0du;
                        TSIP.REG_1CH.WORD = 0x00600000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                        {
                            TSIP.REG_ECH.WORD = 0x38000defu;
                            TSIP.REG_E0H.WORD = 0x00000080u;
                            TSIP.REG_1CH.WORD = 0x00260000u;
                            TSIP.REG_ECH.WORD = 0x380089e0u;
                            TSIP.REG_ECH.WORD = 0x00000003u;
                            TSIP.REG_E0H.WORD = 0x00000080u;
                            TSIP.REG_1CH.WORD = 0x00260000u;
                            RX65NHU_func100(change_endian_long(0x2e710969u), change_endian_long(0x4328f385u), change_endian_long(0x7fb0f43fu), change_endian_long(0x562c7868u));
                            TSIP.REG_1CH.WORD = 0x00400000u;
                            TSIP.REG_1D0H.WORD = 0x00000000u;
                            if (1u == (TSIP.REG_1CH.BIT.B22))
                            {
                                TSIP.REG_D0H.WORD = 0x00000f14u;
                                TSIP.REG_C4H.WORD = 0x00008a07u;
                                TSIP.REG_00H.WORD = 0x00012303u;
                                TSIP.REG_2CH.WORD = 0x00000020u;
                                /* WAIT_LOOP */
                                while (0u != TSIP.REG_00H.BIT.B25)
                                {
                                    /* waiting */
                                }
                                TSIP.REG_1CH.WORD = 0x00001800u;
                                RX65NHU_func101(change_endian_long(0xbb60ebfbu), change_endian_long(0x7ce64742u), change_endian_long(0xf9ec4d40u), change_endian_long(0x8e115d7cu));
                            }
                            else
                            {
                                RX65NHU_func100(change_endian_long(0xd720ba09u), change_endian_long(0xd49a222eu), change_endian_long(0x98ca97e0u), change_endian_long(0x333030e2u));
                                TSIP.REG_104H.WORD = 0x00000058u;
                                TSIP.REG_E0H.WORD = 0x80010020u;
                                /* WAIT_LOOP */
                                while (1u != TSIP.REG_104H.BIT.B31)
                                {
                                    /* waiting */
                                }
                                TSIP.REG_100H.WORD = change_endian_long(0x5d09984du);
                                RX65NHU_func080();
                                RX65NHU_func100(change_endian_long(0x9fa647eau), change_endian_long(0xf93ec1deu), change_endian_long(0x1e5fe375u), change_endian_long(0x587ff982u));
                                TSIP.REG_00H.WORD = 0x00012303u;
                                TSIP.REG_2CH.WORD = 0x00000020u;
                                RX65NHU_func313(1496);
                                RX65NHU_func100(change_endian_long(0xfb7fa741u), change_endian_long(0x44e715c4u), change_endian_long(0xad9d78efu), change_endian_long(0xbe12974au));
                                RX65NHU_func314(1496+64);
                                RX65NHU_func100(change_endian_long(0x54c16addu), change_endian_long(0xf855640eu), change_endian_long(0x40fee543u), change_endian_long(0xc80b5facu));
                                TSIP.REG_104H.WORD = 0x00000058u;
                                TSIP.REG_E0H.WORD = 0x80010020u;
                                /* WAIT_LOOP */
                                while (1u != TSIP.REG_104H.BIT.B31)
                                {
                                    /* waiting */
                                }
                                TSIP.REG_100H.WORD = change_endian_long(0xcbc8750cu);
                                RX65NHU_func080();
                                RX65NHU_func100(change_endian_long(0xd3503955u), change_endian_long(0x224c264fu), change_endian_long(0xc7932908u), change_endian_long(0xd6d7d41cu));
                                TSIP.REG_ECH.WORD = 0x00000800u;
                                TSIP.REG_E0H.WORD = 0x81c00000u;
                                TSIP.REG_00H.WORD = 0x00012803u;
                                RX65NHU_func313(1496+68);
                                RX65NHU_func100(change_endian_long(0x680eb427u), change_endian_long(0x385a4588u), change_endian_long(0x9c3a557bu), change_endian_long(0x03cead5au));
                                RX65NHU_func314(1496+132);
                                RX65NHU_func101(change_endian_long(0x5f869a8du), change_endian_long(0x16abea95u), change_endian_long(0xdd6ea18au), change_endian_long(0x50425cddu));
                            }
                            RX65NHU_func101(change_endian_long(0x44ca09feu), change_endian_long(0xbebc3c20u), change_endian_long(0xd6ad7eddu), change_endian_long(0xfa702f85u));
                        }
                        else
                        {
                            TSIP.REG_D0H.WORD = 0x00000314u;
                            TSIP.REG_C4H.WORD = 0x00008a07u;
                            TSIP.REG_00H.WORD = 0x00002343u;
                            TSIP.REG_2CH.WORD = 0x00000020u;
                            /* WAIT_LOOP */
                            while (0u != TSIP.REG_00H.BIT.B25)
                            {
                                /* waiting */
                            }
                            TSIP.REG_1CH.WORD = 0x00001800u;
                            RX65NHU_func101(change_endian_long(0x4aadbea4u), change_endian_long(0xf7494243u), change_endian_long(0xce5553b3u), change_endian_long(0x9b18eb12u));
                        }
                        RX65NHU_func101(change_endian_long(0x6e8ca670u), change_endian_long(0x930219bdu), change_endian_long(0x8ee9c33bu), change_endian_long(0x2130f7b5u));
                    }
                    else
                    {
                        RX65NHU_func101(change_endian_long(0x68aaaa6eu), change_endian_long(0x8a36512cu), change_endian_long(0xf94f50c8u), change_endian_long(0x69b53ddeu));
                    }
                    RX65NHU_func100(change_endian_long(0x06c7a8b6u), change_endian_long(0x811e20b2u), change_endian_long(0x063d87e7u), change_endian_long(0x6efb64e2u));
                    TSIP.REG_ECH.WORD = 0x00007c0eu;
                    TSIP.REG_1CH.WORD = 0x00600000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                    {
                        RX65NHU_func100(change_endian_long(0x2fd9c270u), change_endian_long(0xee601a1bu), change_endian_long(0x231c0f53u), change_endian_long(0x23abeac0u));
                        TSIP.REG_28H.WORD = 0x00bf0001u;
                        TSIP.REG_2CH.WORD = 0x00000010u;
                        TSIP.REG_104H.WORD = 0x00003f67u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = InData_Signature[0];
                        TSIP.REG_100H.WORD = InData_Signature[1];
                        TSIP.REG_100H.WORD = InData_Signature[2];
                        TSIP.REG_100H.WORD = InData_Signature[3];
                        TSIP.REG_100H.WORD = InData_Signature[4];
                        TSIP.REG_100H.WORD = InData_Signature[5];
                        TSIP.REG_100H.WORD = InData_Signature[6];
                        TSIP.REG_100H.WORD = InData_Signature[7];
                        TSIP.REG_100H.WORD = InData_Signature[8];
                        TSIP.REG_100H.WORD = InData_Signature[9];
                        TSIP.REG_100H.WORD = InData_Signature[10];
                        TSIP.REG_100H.WORD = InData_Signature[11];
                        TSIP.REG_100H.WORD = InData_Signature[12];
                        TSIP.REG_100H.WORD = InData_Signature[13];
                        TSIP.REG_100H.WORD = InData_Signature[14];
                        TSIP.REG_100H.WORD = InData_Signature[15];
                        TSIP.REG_100H.WORD = InData_Signature[16];
                        TSIP.REG_100H.WORD = InData_Signature[17];
                        TSIP.REG_100H.WORD = InData_Signature[18];
                        TSIP.REG_100H.WORD = InData_Signature[19];
                        TSIP.REG_100H.WORD = InData_Signature[20];
                        TSIP.REG_100H.WORD = InData_Signature[21];
                        TSIP.REG_100H.WORD = InData_Signature[22];
                        TSIP.REG_100H.WORD = InData_Signature[23];
                        TSIP.REG_100H.WORD = InData_Signature[24];
                        TSIP.REG_100H.WORD = InData_Signature[25];
                        TSIP.REG_100H.WORD = InData_Signature[26];
                        TSIP.REG_100H.WORD = InData_Signature[27];
                        TSIP.REG_100H.WORD = InData_Signature[28];
                        TSIP.REG_100H.WORD = InData_Signature[29];
                        TSIP.REG_100H.WORD = InData_Signature[30];
                        TSIP.REG_100H.WORD = InData_Signature[31];
                        TSIP.REG_100H.WORD = InData_Signature[32];
                        TSIP.REG_100H.WORD = InData_Signature[33];
                        TSIP.REG_100H.WORD = InData_Signature[34];
                        TSIP.REG_100H.WORD = InData_Signature[35];
                        TSIP.REG_100H.WORD = InData_Signature[36];
                        TSIP.REG_100H.WORD = InData_Signature[37];
                        TSIP.REG_100H.WORD = InData_Signature[38];
                        TSIP.REG_100H.WORD = InData_Signature[39];
                        TSIP.REG_100H.WORD = InData_Signature[40];
                        TSIP.REG_100H.WORD = InData_Signature[41];
                        TSIP.REG_100H.WORD = InData_Signature[42];
                        TSIP.REG_100H.WORD = InData_Signature[43];
                        TSIP.REG_100H.WORD = InData_Signature[44];
                        TSIP.REG_100H.WORD = InData_Signature[45];
                        TSIP.REG_100H.WORD = InData_Signature[46];
                        TSIP.REG_100H.WORD = InData_Signature[47];
                        TSIP.REG_100H.WORD = InData_Signature[48];
                        TSIP.REG_100H.WORD = InData_Signature[49];
                        TSIP.REG_100H.WORD = InData_Signature[50];
                        TSIP.REG_100H.WORD = InData_Signature[51];
                        TSIP.REG_100H.WORD = InData_Signature[52];
                        TSIP.REG_100H.WORD = InData_Signature[53];
                        TSIP.REG_100H.WORD = InData_Signature[54];
                        TSIP.REG_100H.WORD = InData_Signature[55];
                        TSIP.REG_100H.WORD = InData_Signature[56];
                        TSIP.REG_100H.WORD = InData_Signature[57];
                        TSIP.REG_100H.WORD = InData_Signature[58];
                        TSIP.REG_100H.WORD = InData_Signature[59];
                        TSIP.REG_100H.WORD = InData_Signature[60];
                        TSIP.REG_100H.WORD = InData_Signature[61];
                        TSIP.REG_100H.WORD = InData_Signature[62];
                        TSIP.REG_100H.WORD = InData_Signature[63];
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
                        RX65NHU_func101(change_endian_long(0x99f658ecu), change_endian_long(0x323f64a3u), change_endian_long(0x5ceb5eb3u), change_endian_long(0x602dc0b7u));
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
                        TSIP.REG_ECH.WORD = 0x000034b7u;
                        TSIP.REG_ECH.WORD = 0x000034d8u;
                        TSIP.REG_ECH.WORD = 0x000034f9u;
                        TSIP.REG_ECH.WORD = 0x0000351au;
                        RX65NHU_func100(change_endian_long(0x0205babbu), change_endian_long(0x2e98c872u), change_endian_long(0x90ac2f64u), change_endian_long(0x67d0ddc4u));
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x5fe685f6u);
                        RX65NHU_func080();
                        RX65NHU_func100(change_endian_long(0x43a4763au), change_endian_long(0xf67df4b4u), change_endian_long(0x61daa32cu), change_endian_long(0x90900274u));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000020u;
                        RX65NHU_func313(748);
                        RX65NHU_func100(change_endian_long(0x91c20dcau), change_endian_long(0x144ca452u), change_endian_long(0x970bea0cu), change_endian_long(0x5fe4d468u));
                        RX65NHU_func314(748+64);
                        RX65NHU_func100(change_endian_long(0xdfd6c0c4u), change_endian_long(0x769462c0u), change_endian_long(0x49a33e29u), change_endian_long(0xff0110ebu));
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x1c5a2b81u);
                        RX65NHU_func080();
                        RX65NHU_func100(change_endian_long(0xf900f88bu), change_endian_long(0xa3d95529u), change_endian_long(0x5493a02cu), change_endian_long(0x0dab8033u));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000022u;
                        RX65NHU_func313(680);
                        RX65NHU_func100(change_endian_long(0x09a37665u), change_endian_long(0xf18d1b88u), change_endian_long(0xfc27353au), change_endian_long(0x65f62916u));
                        RX65NHU_func314(680+64);
                        RX65NHU_func100(change_endian_long(0x91a9408eu), change_endian_long(0xf2c93c45u), change_endian_long(0xbc6a426bu), change_endian_long(0x0ac44f59u));
                        TSIP.REG_2CH.WORD = 0x00000010u;
                        TSIP.REG_104H.WORD = 0x00003f67u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = InData_Signature[0];
                        TSIP.REG_100H.WORD = InData_Signature[1];
                        TSIP.REG_100H.WORD = InData_Signature[2];
                        TSIP.REG_100H.WORD = InData_Signature[3];
                        TSIP.REG_100H.WORD = InData_Signature[4];
                        TSIP.REG_100H.WORD = InData_Signature[5];
                        TSIP.REG_100H.WORD = InData_Signature[6];
                        TSIP.REG_100H.WORD = InData_Signature[7];
                        TSIP.REG_100H.WORD = InData_Signature[8];
                        TSIP.REG_100H.WORD = InData_Signature[9];
                        TSIP.REG_100H.WORD = InData_Signature[10];
                        TSIP.REG_100H.WORD = InData_Signature[11];
                        TSIP.REG_100H.WORD = InData_Signature[12];
                        TSIP.REG_100H.WORD = InData_Signature[13];
                        TSIP.REG_100H.WORD = InData_Signature[14];
                        TSIP.REG_100H.WORD = InData_Signature[15];
                        TSIP.REG_100H.WORD = InData_Signature[16];
                        TSIP.REG_100H.WORD = InData_Signature[17];
                        TSIP.REG_100H.WORD = InData_Signature[18];
                        TSIP.REG_100H.WORD = InData_Signature[19];
                        TSIP.REG_100H.WORD = InData_Signature[20];
                        TSIP.REG_100H.WORD = InData_Signature[21];
                        TSIP.REG_100H.WORD = InData_Signature[22];
                        TSIP.REG_100H.WORD = InData_Signature[23];
                        TSIP.REG_100H.WORD = InData_Signature[24];
                        TSIP.REG_100H.WORD = InData_Signature[25];
                        TSIP.REG_100H.WORD = InData_Signature[26];
                        TSIP.REG_100H.WORD = InData_Signature[27];
                        TSIP.REG_100H.WORD = InData_Signature[28];
                        TSIP.REG_100H.WORD = InData_Signature[29];
                        TSIP.REG_100H.WORD = InData_Signature[30];
                        TSIP.REG_100H.WORD = InData_Signature[31];
                        TSIP.REG_100H.WORD = InData_Signature[32];
                        TSIP.REG_100H.WORD = InData_Signature[33];
                        TSIP.REG_100H.WORD = InData_Signature[34];
                        TSIP.REG_100H.WORD = InData_Signature[35];
                        TSIP.REG_100H.WORD = InData_Signature[36];
                        TSIP.REG_100H.WORD = InData_Signature[37];
                        TSIP.REG_100H.WORD = InData_Signature[38];
                        TSIP.REG_100H.WORD = InData_Signature[39];
                        TSIP.REG_100H.WORD = InData_Signature[40];
                        TSIP.REG_100H.WORD = InData_Signature[41];
                        TSIP.REG_100H.WORD = InData_Signature[42];
                        TSIP.REG_100H.WORD = InData_Signature[43];
                        TSIP.REG_100H.WORD = InData_Signature[44];
                        TSIP.REG_100H.WORD = InData_Signature[45];
                        TSIP.REG_100H.WORD = InData_Signature[46];
                        TSIP.REG_100H.WORD = InData_Signature[47];
                        TSIP.REG_100H.WORD = InData_Signature[48];
                        TSIP.REG_100H.WORD = InData_Signature[49];
                        TSIP.REG_100H.WORD = InData_Signature[50];
                        TSIP.REG_100H.WORD = InData_Signature[51];
                        TSIP.REG_100H.WORD = InData_Signature[52];
                        TSIP.REG_100H.WORD = InData_Signature[53];
                        TSIP.REG_100H.WORD = InData_Signature[54];
                        TSIP.REG_100H.WORD = InData_Signature[55];
                        TSIP.REG_100H.WORD = InData_Signature[56];
                        TSIP.REG_100H.WORD = InData_Signature[57];
                        TSIP.REG_100H.WORD = InData_Signature[58];
                        TSIP.REG_100H.WORD = InData_Signature[59];
                        TSIP.REG_100H.WORD = InData_Signature[60];
                        TSIP.REG_100H.WORD = InData_Signature[61];
                        TSIP.REG_100H.WORD = InData_Signature[62];
                        TSIP.REG_100H.WORD = InData_Signature[63];
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x5dd049feu);
                        RX65NHU_func080();
                        RX65NHU_func100(change_endian_long(0xc5da6684u), change_endian_long(0x2e63f7aeu), change_endian_long(0xd8736fdbu), change_endian_long(0xf7597c79u));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000020u;
                        RX65NHU_func313(68);
                        RX65NHU_func100(change_endian_long(0x9d106ba8u), change_endian_long(0x8757d106u), change_endian_long(0x108c05f0u), change_endian_long(0xadb17b0fu));
                        RX65NHU_func314(68+64);
                        RX65NHU_func100(change_endian_long(0x2d5994c2u), change_endian_long(0xc1fbaffau), change_endian_long(0xd0191974u), change_endian_long(0x1036bc3au));
                        TSIP.REG_2CH.WORD = 0x00000014u;
                        TSIP.REG_104H.WORD = 0x00003f67u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = InData_Signature[64];
                        TSIP.REG_100H.WORD = InData_Signature[65];
                        TSIP.REG_100H.WORD = InData_Signature[66];
                        TSIP.REG_100H.WORD = InData_Signature[67];
                        TSIP.REG_100H.WORD = InData_Signature[68];
                        TSIP.REG_100H.WORD = InData_Signature[69];
                        TSIP.REG_100H.WORD = InData_Signature[70];
                        TSIP.REG_100H.WORD = InData_Signature[71];
                        TSIP.REG_100H.WORD = InData_Signature[72];
                        TSIP.REG_100H.WORD = InData_Signature[73];
                        TSIP.REG_100H.WORD = InData_Signature[74];
                        TSIP.REG_100H.WORD = InData_Signature[75];
                        TSIP.REG_100H.WORD = InData_Signature[76];
                        TSIP.REG_100H.WORD = InData_Signature[77];
                        TSIP.REG_100H.WORD = InData_Signature[78];
                        TSIP.REG_100H.WORD = InData_Signature[79];
                        TSIP.REG_100H.WORD = InData_Signature[80];
                        TSIP.REG_100H.WORD = InData_Signature[81];
                        TSIP.REG_100H.WORD = InData_Signature[82];
                        TSIP.REG_100H.WORD = InData_Signature[83];
                        TSIP.REG_100H.WORD = InData_Signature[84];
                        TSIP.REG_100H.WORD = InData_Signature[85];
                        TSIP.REG_100H.WORD = InData_Signature[86];
                        TSIP.REG_100H.WORD = InData_Signature[87];
                        TSIP.REG_100H.WORD = InData_Signature[88];
                        TSIP.REG_100H.WORD = InData_Signature[89];
                        TSIP.REG_100H.WORD = InData_Signature[90];
                        TSIP.REG_100H.WORD = InData_Signature[91];
                        TSIP.REG_100H.WORD = InData_Signature[92];
                        TSIP.REG_100H.WORD = InData_Signature[93];
                        TSIP.REG_100H.WORD = InData_Signature[94];
                        TSIP.REG_100H.WORD = InData_Signature[95];
                        TSIP.REG_100H.WORD = InData_Signature[96];
                        TSIP.REG_100H.WORD = InData_Signature[97];
                        TSIP.REG_100H.WORD = InData_Signature[98];
                        TSIP.REG_100H.WORD = InData_Signature[99];
                        TSIP.REG_100H.WORD = InData_Signature[100];
                        TSIP.REG_100H.WORD = InData_Signature[101];
                        TSIP.REG_100H.WORD = InData_Signature[102];
                        TSIP.REG_100H.WORD = InData_Signature[103];
                        TSIP.REG_100H.WORD = InData_Signature[104];
                        TSIP.REG_100H.WORD = InData_Signature[105];
                        TSIP.REG_100H.WORD = InData_Signature[106];
                        TSIP.REG_100H.WORD = InData_Signature[107];
                        TSIP.REG_100H.WORD = InData_Signature[108];
                        TSIP.REG_100H.WORD = InData_Signature[109];
                        TSIP.REG_100H.WORD = InData_Signature[110];
                        TSIP.REG_100H.WORD = InData_Signature[111];
                        TSIP.REG_100H.WORD = InData_Signature[112];
                        TSIP.REG_100H.WORD = InData_Signature[113];
                        TSIP.REG_100H.WORD = InData_Signature[114];
                        TSIP.REG_100H.WORD = InData_Signature[115];
                        TSIP.REG_100H.WORD = InData_Signature[116];
                        TSIP.REG_100H.WORD = InData_Signature[117];
                        TSIP.REG_100H.WORD = InData_Signature[118];
                        TSIP.REG_100H.WORD = InData_Signature[119];
                        TSIP.REG_100H.WORD = InData_Signature[120];
                        TSIP.REG_100H.WORD = InData_Signature[121];
                        TSIP.REG_100H.WORD = InData_Signature[122];
                        TSIP.REG_100H.WORD = InData_Signature[123];
                        TSIP.REG_100H.WORD = InData_Signature[124];
                        TSIP.REG_100H.WORD = InData_Signature[125];
                        TSIP.REG_100H.WORD = InData_Signature[126];
                        TSIP.REG_100H.WORD = InData_Signature[127];
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x945d2158u);
                        RX65NHU_func080();
                        RX65NHU_func100(change_endian_long(0x61231dbfu), change_endian_long(0x92e1a3cdu), change_endian_long(0x7a8e76f5u), change_endian_long(0x877303d0u));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000024u;
                        RX65NHU_func313(0);
                        RX65NHU_func100(change_endian_long(0xbfbf6b0cu), change_endian_long(0xd6f690f2u), change_endian_long(0x99769ad3u), change_endian_long(0x026cb619u));
                        RX65NHU_func314(0+64);
                        TSIP.REG_ECH.WORD = 0x0000b400u;
                        TSIP.REG_ECH.WORD = 0x00000001u;
                        RX65NHU_func101(change_endian_long(0xdff8713fu), change_endian_long(0x31bcb324u), change_endian_long(0xa64486a8u), change_endian_long(0x809d64f9u));
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
                        RX65NHU_func101(change_endian_long(0xc95c0017u), change_endian_long(0x1b239555u), change_endian_long(0xfc59de12u), change_endian_long(0x728d3203u));
                    }
                    else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                    {
                        TSIP.REG_28H.WORD = 0x00870001u;
                        TSIP.REG_34H.WORD = 0x00000400u;
                        TSIP.REG_24H.WORD = 0x8000a8d0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_ECH.WORD = 0x00000b9cu;
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x800103a0u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x000000e1u);
                        RX65NHU_func101(change_endian_long(0x934e7ec2u), change_endian_long(0x8640b810u), change_endian_long(0x36485e20u), change_endian_long(0x8d7df0b3u));
                        RX65NHU_func003(InData_Signature);
                        RX65NHU_func101(change_endian_long(0x982cf043u), change_endian_long(0xc8bea337u), change_endian_long(0x301056b5u), change_endian_long(0x0f8cf4edu));
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
                        TSIP.REG_100H.WORD = InData_Signature[0];
                        TSIP.REG_100H.WORD = InData_Signature[1];
                        TSIP.REG_100H.WORD = InData_Signature[2];
                        TSIP.REG_100H.WORD = InData_Signature[3];
                        TSIP.REG_100H.WORD = InData_Signature[4];
                        TSIP.REG_100H.WORD = InData_Signature[5];
                        TSIP.REG_100H.WORD = InData_Signature[6];
                        TSIP.REG_100H.WORD = InData_Signature[7];
                        TSIP.REG_100H.WORD = InData_Signature[8];
                        TSIP.REG_100H.WORD = InData_Signature[9];
                        TSIP.REG_100H.WORD = InData_Signature[10];
                        TSIP.REG_100H.WORD = InData_Signature[11];
                        TSIP.REG_100H.WORD = InData_Signature[12];
                        TSIP.REG_100H.WORD = InData_Signature[13];
                        TSIP.REG_100H.WORD = InData_Signature[14];
                        TSIP.REG_100H.WORD = InData_Signature[15];
                        TSIP.REG_100H.WORD = InData_Signature[16];
                        TSIP.REG_100H.WORD = InData_Signature[17];
                        TSIP.REG_100H.WORD = InData_Signature[18];
                        TSIP.REG_100H.WORD = InData_Signature[19];
                        TSIP.REG_100H.WORD = InData_Signature[20];
                        TSIP.REG_100H.WORD = InData_Signature[21];
                        TSIP.REG_100H.WORD = InData_Signature[22];
                        TSIP.REG_100H.WORD = InData_Signature[23];
                        TSIP.REG_100H.WORD = InData_Signature[24];
                        TSIP.REG_100H.WORD = InData_Signature[25];
                        TSIP.REG_100H.WORD = InData_Signature[26];
                        TSIP.REG_100H.WORD = InData_Signature[27];
                        TSIP.REG_100H.WORD = InData_Signature[28];
                        TSIP.REG_100H.WORD = InData_Signature[29];
                        TSIP.REG_100H.WORD = InData_Signature[30];
                        TSIP.REG_100H.WORD = InData_Signature[31];
                        TSIP.REG_100H.WORD = InData_Signature[32];
                        TSIP.REG_100H.WORD = InData_Signature[33];
                        TSIP.REG_100H.WORD = InData_Signature[34];
                        TSIP.REG_100H.WORD = InData_Signature[35];
                        TSIP.REG_100H.WORD = InData_Signature[36];
                        TSIP.REG_100H.WORD = InData_Signature[37];
                        TSIP.REG_100H.WORD = InData_Signature[38];
                        TSIP.REG_100H.WORD = InData_Signature[39];
                        TSIP.REG_100H.WORD = InData_Signature[40];
                        TSIP.REG_100H.WORD = InData_Signature[41];
                        TSIP.REG_100H.WORD = InData_Signature[42];
                        TSIP.REG_100H.WORD = InData_Signature[43];
                        TSIP.REG_100H.WORD = InData_Signature[44];
                        TSIP.REG_100H.WORD = InData_Signature[45];
                        TSIP.REG_100H.WORD = InData_Signature[46];
                        TSIP.REG_100H.WORD = InData_Signature[47];
                        TSIP.REG_100H.WORD = InData_Signature[48];
                        TSIP.REG_100H.WORD = InData_Signature[49];
                        TSIP.REG_100H.WORD = InData_Signature[50];
                        TSIP.REG_100H.WORD = InData_Signature[51];
                        TSIP.REG_100H.WORD = InData_Signature[52];
                        TSIP.REG_100H.WORD = InData_Signature[53];
                        TSIP.REG_100H.WORD = InData_Signature[54];
                        TSIP.REG_100H.WORD = InData_Signature[55];
                        TSIP.REG_100H.WORD = InData_Signature[56];
                        TSIP.REG_100H.WORD = InData_Signature[57];
                        TSIP.REG_100H.WORD = InData_Signature[58];
                        TSIP.REG_100H.WORD = InData_Signature[59];
                        TSIP.REG_100H.WORD = InData_Signature[60];
                        TSIP.REG_100H.WORD = InData_Signature[61];
                        TSIP.REG_100H.WORD = InData_Signature[62];
                        TSIP.REG_100H.WORD = InData_Signature[63];
                        TSIP.REG_ECH.WORD = 0x0000342au;
                        TSIP.REG_ECH.WORD = 0x0000b400u;
                        TSIP.REG_ECH.WORD = 0x000000e1u;
                        TSIP.REG_ECH.WORD = 0x0000b440u;
                        TSIP.REG_ECH.WORD = 0x00000020u;
                        RX65NHU_func101(change_endian_long(0x4a364571u), change_endian_long(0x35fed1bdu), change_endian_long(0x2a81e125u), change_endian_long(0x13764444u));
                        RX65NHU_func015();
                        RX65NHU_func101(change_endian_long(0x4f74b6e9u), change_endian_long(0x08f0b37du), change_endian_long(0x01c00422u), change_endian_long(0x09732bcau));
                    }
                    RX65NHU_func100(change_endian_long(0xd991c229u), change_endian_long(0xb0c45a51u), change_endian_long(0x2452d67du), change_endian_long(0x55ba483au));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX65NHU_func102(change_endian_long(0x8b3d0e91u), change_endian_long(0x7a770b6fu), change_endian_long(0x223236c7u), change_endian_long(0x4ab8d6f4u));
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
                        TSIP.REG_ECH.WORD = 0x380089e0u;
                        TSIP.REG_ECH.WORD = 0x00000005u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        RX65NHU_func100(change_endian_long(0x3c3c31e2u), change_endian_long(0x3fa5e044u), change_endian_long(0x33e194b7u), change_endian_long(0xb22eb5d9u));
                        TSIP.REG_1CH.WORD = 0x00400000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (1u == (TSIP.REG_1CH.BIT.B22))
                        {
                            RX65NHU_func102(change_endian_long(0xbe1cb0adu), change_endian_long(0xc89c818au), change_endian_long(0xac3ad04du), change_endian_long(0xa48b3a4du));
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
                        else
                        {
                            RX65NHU_func100(change_endian_long(0xe4f9cdb3u), change_endian_long(0x2d79467du), change_endian_long(0xc41af69eu), change_endian_long(0x4fe66f2du));
                            TSIP.REG_ECH.WORD = 0x00007c0du;
                            TSIP.REG_1CH.WORD = 0x00600000u;
                            TSIP.REG_1D0H.WORD = 0x00000000u;
                            if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                            {
                                TSIP.REG_28H.WORD = 0x00bf0001u;
                                TSIP.REG_ECH.WORD = 0x38000defu;
                                TSIP.REG_E0H.WORD = 0x00000080u;
                                TSIP.REG_1CH.WORD = 0x00260000u;
                                TSIP.REG_ECH.WORD = 0x380089e0u;
                                TSIP.REG_ECH.WORD = 0x00000003u;
                                TSIP.REG_E0H.WORD = 0x00000080u;
                                TSIP.REG_1CH.WORD = 0x00260000u;
                                RX65NHU_func100(change_endian_long(0xe016caa8u), change_endian_long(0x6965642bu), change_endian_long(0x9132b556u), change_endian_long(0x41be94ebu));
                                TSIP.REG_1CH.WORD = 0x00400000u;
                                TSIP.REG_1D0H.WORD = 0x00000000u;
                                if (1u == (TSIP.REG_1CH.BIT.B22))
                                {
                                    TSIP.REG_D0H.WORD = 0x00000f1cu;
                                    TSIP.REG_C4H.WORD = 0x00008a83u;
                                    TSIP.REG_00H.WORD = 0x00013203u;
                                    TSIP.REG_2CH.WORD = 0x00000014u;
                                    /* WAIT_LOOP */
                                    while (0u != TSIP.REG_00H.BIT.B25)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_1CH.WORD = 0x00001800u;
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
                                    TSIP.REG_18H.WORD = 0x00000004u;
                                    TSIP.REG_24H.WORD = 0x00005004u;
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
                                    TSIP.REG_104H.WORD = 0x00000058u;
                                    TSIP.REG_E0H.WORD = 0x80010000u;
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_104H.BIT.B31)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_100H.WORD = change_endian_long(0x05880100u);
                                    RX65NHU_func101(change_endian_long(0x009e6319u), change_endian_long(0xcf088042u), change_endian_long(0x2e06fa3au), change_endian_long(0xcd101991u));
                                }
                                else
                                {
                                    RX65NHU_func100(change_endian_long(0x583d4d6bu), change_endian_long(0xb144f99bu), change_endian_long(0x40055797u), change_endian_long(0xce724dacu));
                                    TSIP.REG_C4H.WORD = 0x000c0805u;
                                    TSIP.REG_E0H.WORD = 0x810402e0u;
                                    TSIP.REG_00H.WORD = 0x00002813u;
                                    /* WAIT_LOOP */
                                    while (0u != TSIP.REG_00H.BIT.B25)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_1CH.WORD = 0x00001800u;
                                    RX65NHU_func100(change_endian_long(0x75e02dbcu), change_endian_long(0xbd8453bdu), change_endian_long(0xdf4ff864u), change_endian_long(0xddd9b26bu));
                                    TSIP.REG_104H.WORD = 0x00000058u;
                                    TSIP.REG_E0H.WORD = 0x80010020u;
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_104H.BIT.B31)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_100H.WORD = change_endian_long(0x5d09984du);
                                    RX65NHU_func080();
                                    RX65NHU_func100(change_endian_long(0xed991305u), change_endian_long(0x1cb5222fu), change_endian_long(0xfe8202fau), change_endian_long(0x74daa6a5u));
                                    TSIP.REG_00H.WORD = 0x00013203u;
                                    TSIP.REG_2CH.WORD = 0x00000014u;
                                    RX65NHU_func312(1496);
                                    TSIP.REG_104H.WORD = 0x00000058u;
                                    TSIP.REG_E0H.WORD = 0x80010020u;
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_104H.BIT.B31)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_100H.WORD = change_endian_long(0xcbc8750cu);
                                    RX65NHU_func080();
                                    TSIP.REG_00H.WORD = 0x00013203u;
                                    TSIP.REG_2CH.WORD = 0x00000012u;
                                    RX65NHU_func312(1496+68);
                                    TSIP.REG_104H.WORD = 0x00000058u;
                                    TSIP.REG_E0H.WORD = 0x80010000u;
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_104H.BIT.B31)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_100H.WORD = change_endian_long(0x0a880100u);
                                    RX65NHU_func101(change_endian_long(0xce241a2fu), change_endian_long(0xeff6192eu), change_endian_long(0xa0afc2dfu), change_endian_long(0xfec1cc3eu));
                                }
                                RX65NHU_func100(change_endian_long(0x9e69690eu), change_endian_long(0x1cb7bdefu), change_endian_long(0xefaa303fu), change_endian_long(0x4388b8cau));
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
                                RX65NHU_func100(change_endian_long(0x41825b9eu), change_endian_long(0xa66d5026u), change_endian_long(0x84c4ac7du), change_endian_long(0x1faae478u));
                                TSIP.REG_C4H.WORD = 0x000c2b0du;
                                TSIP.REG_E0H.WORD = 0x81040000u;
                                TSIP.REG_00H.WORD = 0x00002813u;
                                /* WAIT_LOOP */
                                while (0u != TSIP.REG_00H.BIT.B25)
                                {
                                    /* waiting */
                                }
                                TSIP.REG_1CH.WORD = 0x00001800u;
                                TSIP.REG_00H.WORD = 0x00012303u;
                                TSIP.REG_2CH.WORD = 0x00000024u;
                                TSIP.REG_D0H.WORD = 0x00000f00u;
                                TSIP.REG_C4H.WORD = 0x02e087bfu;
                                TSIP.REG_04H.WORD = 0x00000202u;
                                for (iLoop = 4; iLoop < 68; iLoop = iLoop + 4)
                                {
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_04H.BIT.B30)
                                    {
                                        /* waiting */
                                    }
                                    OutData_PubKey[iLoop + 0] = TSIP.REG_100H.WORD;
                                    OutData_PubKey[iLoop + 1] = TSIP.REG_100H.WORD;
                                    OutData_PubKey[iLoop + 2] = TSIP.REG_100H.WORD;
                                    OutData_PubKey[iLoop + 3] = TSIP.REG_100H.WORD;
                                }
                                RX65NHU_func100(change_endian_long(0x424280d1u), change_endian_long(0x86bd500du), change_endian_long(0x2615de03u), change_endian_long(0x256f5dc5u));
                                TSIP.REG_00H.WORD = 0x00012303u;
                                TSIP.REG_2CH.WORD = 0x00000022u;
                                TSIP.REG_D0H.WORD = 0x00000f00u;
                                TSIP.REG_C4H.WORD = 0x00e087bfu;
                                TSIP.REG_04H.WORD = 0x00000202u;
                                for ( ; iLoop < 132; iLoop = iLoop + 4)
                                {
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_04H.BIT.B30)
                                    {
                                        /* waiting */
                                    }
                                    OutData_PubKey[iLoop + 0] = TSIP.REG_100H.WORD;
                                    OutData_PubKey[iLoop + 1] = TSIP.REG_100H.WORD;
                                    OutData_PubKey[iLoop + 2] = TSIP.REG_100H.WORD;
                                    OutData_PubKey[iLoop + 3] = TSIP.REG_100H.WORD;
                                }
                                RX65NHU_func100(change_endian_long(0xaa1e2fdcu), change_endian_long(0xc640bf9eu), change_endian_long(0x17f19db9u), change_endian_long(0x6bd4b031u));
                                TSIP.REG_C4H.WORD = 0x00e007bcu;
                                TSIP.REG_E0H.WORD = 0x81010200u;
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
                                OutData_PubKey[iLoop + 0] = TSIP.REG_100H.WORD;
                                OutData_PubKey[iLoop + 1] = TSIP.REG_100H.WORD;
                                OutData_PubKey[iLoop + 2] = TSIP.REG_100H.WORD;
                                OutData_PubKey[iLoop + 3] = TSIP.REG_100H.WORD;
                                RX65NHU_func100(change_endian_long(0xaf45bb7cu), change_endian_long(0xe8b3a96fu), change_endian_long(0xac7227a0u), change_endian_long(0x6ded174fu));
                                iLoop = iLoop+4;
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
                                OutData_PubKey[iLoop + 0] = TSIP.REG_100H.WORD;
                                OutData_PubKey[iLoop + 1] = TSIP.REG_100H.WORD;
                                OutData_PubKey[iLoop + 2] = TSIP.REG_100H.WORD;
                                OutData_PubKey[iLoop + 3] = TSIP.REG_100H.WORD;
                                RX65NHU_func100(change_endian_long(0x65b7ad76u), change_endian_long(0xb9d0e505u), change_endian_long(0xe84f9a3eu), change_endian_long(0x410b0b52u));
                                TSIP.REG_E0H.WORD = 0x81040000u;
                                TSIP.REG_04H.WORD = 0x00000612u;
                                /* WAIT_LOOP */
                                while (1u != TSIP.REG_04H.BIT.B30)
                                {
                                    /* waiting */
                                }
                                OutData_PubKey[0] = TSIP.REG_100H.WORD;
                                /* WAIT_LOOP */
                                while (1u != TSIP.REG_04H.BIT.B30)
                                {
                                    /* waiting */
                                }
                                OutData_PubKey[1] = TSIP.REG_100H.WORD;
                                /* WAIT_LOOP */
                                while (1u != TSIP.REG_04H.BIT.B30)
                                {
                                    /* waiting */
                                }
                                OutData_PubKey[2] = TSIP.REG_100H.WORD;
                                /* WAIT_LOOP */
                                while (1u != TSIP.REG_04H.BIT.B30)
                                {
                                    /* waiting */
                                }
                                OutData_PubKey[3] = TSIP.REG_100H.WORD;
                                RX65NHU_func102(change_endian_long(0x58f4f34du), change_endian_long(0x572f8406u), change_endian_long(0x78331176u), change_endian_long(0x1fc819b1u));
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
                                RX65NHU_func100(change_endian_long(0x13a0c595u), change_endian_long(0xc40de983u), change_endian_long(0x8d3d488bu), change_endian_long(0xc1d6403bu));
                                TSIP.REG_1CH.WORD = 0x00400000u;
                                TSIP.REG_1D0H.WORD = 0x00000000u;
                                if (1u == (TSIP.REG_1CH.BIT.B22))
                                {
                                    RX65NHU_func102(change_endian_long(0x473ed499u), change_endian_long(0xcf1b7a84u), change_endian_long(0x32c56b9cu), change_endian_long(0xb35d260cu));
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
                                    RX65NHU_func100(change_endian_long(0xaf036d8bu), change_endian_long(0x514a992cu), change_endian_long(0xafd1532fu), change_endian_long(0xb83fd316u));
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
                                    TSIP.REG_100H.WORD = change_endian_long(0x31080100u);
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
                                    RX65NHU_func100(change_endian_long(0xd2fdd572u), change_endian_long(0x77963e42u), change_endian_long(0x84f815ceu), change_endian_long(0xa1beba0au));
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
                                        OutData_PubKey[iLoop + 0] = TSIP.REG_100H.WORD;
                                        OutData_PubKey[iLoop + 1] = TSIP.REG_100H.WORD;
                                        OutData_PubKey[iLoop + 2] = TSIP.REG_100H.WORD;
                                        OutData_PubKey[iLoop + 3] = TSIP.REG_100H.WORD;
                                    }
                                    /* WAIT_LOOP */
                                    while (0u != TSIP.REG_00H.BIT.B25)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_1CH.WORD = 0x00001800u;
                                    RX65NHU_func100(change_endian_long(0xdf1fc98du), change_endian_long(0xf1139c07u), change_endian_long(0x3af44157u), change_endian_long(0xcba69152u));
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
                                    OutData_PubKey[iLoop + 0] = TSIP.REG_100H.WORD;
                                    OutData_PubKey[iLoop + 1] = TSIP.REG_100H.WORD;
                                    OutData_PubKey[iLoop + 2] = TSIP.REG_100H.WORD;
                                    OutData_PubKey[iLoop + 3] = TSIP.REG_100H.WORD;
                                    RX65NHU_func100(change_endian_long(0xf8b9c3d2u), change_endian_long(0x7637de8au), change_endian_long(0xd0a211fdu), change_endian_long(0x3aff3a00u));
                                    TSIP.REG_E0H.WORD = 0x81040000u;
                                    TSIP.REG_04H.WORD = 0x00000612u;
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_04H.BIT.B30)
                                    {
                                        /* waiting */
                                    }
                                    OutData_PubKey[0] = TSIP.REG_100H.WORD;
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_04H.BIT.B30)
                                    {
                                        /* waiting */
                                    }
                                    OutData_PubKey[1] = TSIP.REG_100H.WORD;
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_04H.BIT.B30)
                                    {
                                        /* waiting */
                                    }
                                    OutData_PubKey[2] = TSIP.REG_100H.WORD;
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_04H.BIT.B30)
                                    {
                                        /* waiting */
                                    }
                                    OutData_PubKey[3] = TSIP.REG_100H.WORD;
                                    RX65NHU_func102(change_endian_long(0xfe0ef4bfu), change_endian_long(0x3ceffe99u), change_endian_long(0x41f0b4e4u), change_endian_long(0x63cd339fu));
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
    }
}
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_pe1_r9.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_TLS == 1 */
