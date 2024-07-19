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
* Function Name: R_TSIP_TlsSVCertificateVerificationSub
*******************************************************************************************************************/ /**
* @details       RX65NHU TLS Certificate Verification for Server
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
e_tsip_err_t R_TSIP_TlsSVCertificateVerificationSub(uint32_t *InData_Sel_InDataPubKeyType, uint32_t *InData_Sel_OutDataPubKeyType, uint32_t *InData_PubKey, uint32_t *InData_TBSCertificate, uint32_t *InData_TBSCertificateLength, uint32_t *InData_Signature, uint32_t *InData_TBSCertificateInfo, uint32_t *OutData_PubKey)
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
    TSIP.REG_84H.WORD = 0x0000c101u;
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
    RX65NHU_func100(change_endian_long(0xc56b52bau), change_endian_long(0x901edd49u), change_endian_long(0x9d64b102u), change_endian_long(0x6aed48dcu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0x9e953160u), change_endian_long(0xc6ee6b5au), change_endian_long(0x55843d2bu), change_endian_long(0xc4d9753eu));
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
        RX65NHU_func100(change_endian_long(0xb7193e55u), change_endian_long(0xc2f68b2fu), change_endian_long(0x05601de6u), change_endian_long(0xe23d8af3u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func100(change_endian_long(0xa97a182bu), change_endian_long(0x156016a6u), change_endian_long(0x46a26074u), change_endian_long(0xc6a45956u));
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
                RX65NHU_func100(change_endian_long(0xd5232509u), change_endian_long(0x8eeac3a6u), change_endian_long(0xffae305au), change_endian_long(0xf972cc19u));
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
                    RX65NHU_func101(change_endian_long(0x737bd0f4u), change_endian_long(0x8db2da04u), change_endian_long(0xea2a2debu), change_endian_long(0x29e94e6cu));
                }
                else
                {
                    TSIP.REG_ECH.WORD = 0x00003417u;
                    TSIP.REG_ECH.WORD = 0x0000a400u;
                    TSIP.REG_ECH.WORD = 0x000001ffu;
                    TSIP.REG_ECH.WORD = 0x08002818u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00A60000u;
                    RX65NHU_func101(change_endian_long(0x04a0f396u), change_endian_long(0xd39012f0u), change_endian_long(0x409aed3eu), change_endian_long(0x27e32128u));
                }
                TSIP.REG_ECH.WORD = 0x00003419u;
                TSIP.REG_ECH.WORD = 0x0000a400u;
                TSIP.REG_ECH.WORD = 0x00000003u;
                TSIP.REG_ECH.WORD = 0x0c00281au;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00A60000u;
                RX65NHU_func101(change_endian_long(0x4af393d7u), change_endian_long(0x1936b141u), change_endian_long(0x93b33b05u), change_endian_long(0xe6f30405u));
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
                RX65NHU_func101(change_endian_long(0x4477f80bu), change_endian_long(0xeb7f4ffbu), change_endian_long(0x9966e741u), change_endian_long(0x465fe93au));
            }
            RX65NHU_func101(change_endian_long(0xdb37997du), change_endian_long(0x9a8407e9u), change_endian_long(0x04bcb492u), change_endian_long(0x8610cc65u));
        }
        else
        {
            RX65NHU_func101(change_endian_long(0x1588a0a0u), change_endian_long(0x9eeb0fdbu), change_endian_long(0x3c24285cu), change_endian_long(0x66b4f874u));
        }
        RX65NHU_func100(change_endian_long(0xbf6adc51u), change_endian_long(0x3ffcdad7u), change_endian_long(0xad62d525u), change_endian_long(0xc3595c30u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func102(change_endian_long(0x754a21ceu), change_endian_long(0xf17e13cfu), change_endian_long(0x0f2c7e2du), change_endian_long(0x428c6a9bu));
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
            TSIP.REG_ECH.WORD = 0x85000000u;
            TSIP.REG_ECH.WORD = 0x1000d3c0u;
            TSIP.REG_ECH.WORD = 0x38008be0u;
            TSIP.REG_ECH.WORD = 0x8a000000u;
            TSIP.REG_ECH.WORD = 0x1000d3c0u;
            TSIP.REG_ECH.WORD = 0x38008be0u;
            TSIP.REG_ECH.WORD = 0xb1000000u;
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
            RX65NHU_func100(change_endian_long(0x9c353e72u), change_endian_long(0x954374a6u), change_endian_long(0x9cdf9e29u), change_endian_long(0xbd3e6440u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX65NHU_func102(change_endian_long(0x2f8705f1u), change_endian_long(0x00071094u), change_endian_long(0xa63595fau), change_endian_long(0x6640431eu));
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
                RX65NHU_func100(change_endian_long(0x5bb4d562u), change_endian_long(0xa1828f14u), change_endian_long(0x566c4101u), change_endian_long(0x232e0f26u));
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
                    RX65NHU_func101(change_endian_long(0x37e7bfaeu), change_endian_long(0x914f62a5u), change_endian_long(0xec0eec84u), change_endian_long(0xc157a120u));
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
                    RX65NHU_func101(change_endian_long(0x3d2c4387u), change_endian_long(0x242531d0u), change_endian_long(0xdcd286b6u), change_endian_long(0xced48855u));
                }
                RX65NHU_func100(change_endian_long(0xddef8a23u), change_endian_long(0x06efcaacu), change_endian_long(0xeab379c1u), change_endian_long(0x6cc3ffb5u));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    RX65NHU_func102(change_endian_long(0x10c985ceu), change_endian_long(0xce7ad925u), change_endian_long(0x6638091du), change_endian_long(0xf2b1b7deu));
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
                    RX65NHU_func100(change_endian_long(0x59f3cbfeu), change_endian_long(0x07da6435u), change_endian_long(0x25d2d6bbu), change_endian_long(0x31616cd8u));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX65NHU_func100(change_endian_long(0x7e7ecd40u), change_endian_long(0x3bb4f9e7u), change_endian_long(0x27fe7d76u), change_endian_long(0xceb10f64u));
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
                            RX65NHU_func101(change_endian_long(0xeec12b1fu), change_endian_long(0x8c9c6f3fu), change_endian_long(0x98895c96u), change_endian_long(0x8e8473e9u));
                        }
                        RX65NHU_func100(change_endian_long(0x6d3fa357u), change_endian_long(0x8f15febau), change_endian_long(0x7b21a214u), change_endian_long(0x9601998eu));
                        TSIP.REG_ECH.WORD = 0x38000805u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        TSIP.REG_1CH.WORD = 0x00402000u;
                        TSIP.REG_ECH.WORD = 0x00007c0du;
                        TSIP.REG_1CH.WORD = 0x00600000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                        {
                            RX65NHU_func100(change_endian_long(0x10f0bbdcu), change_endian_long(0x882120b7u), change_endian_long(0x83a1b956u), change_endian_long(0x6925828au));
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
                                    RX65NHU_func100(change_endian_long(0xf5f5ad48u), change_endian_long(0x67b4d5e2u), change_endian_long(0x976fb3a3u), change_endian_long(0x5bb5a1a6u));
                                    TSIP.REG_1CH.WORD = 0x00400000u;
                                    TSIP.REG_1D0H.WORD = 0x00000000u;
                                    if (1u == (TSIP.REG_1CH.BIT.B22))
                                    {
                                        TSIP.REG_ECH.WORD = 0x3420a980u;
                                        TSIP.REG_ECH.WORD = 0x00000100u;
                                        TSIP.REG_E0H.WORD = 0x00000080u;
                                        TSIP.REG_1CH.WORD = 0x00260000u;
                                        RX65NHU_func100(change_endian_long(0x2c0ec7a5u), change_endian_long(0x171fdbfau), change_endian_long(0x358ba6dfu), change_endian_long(0x14600c09u));
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
                                            RX65NHU_func101(change_endian_long(0x306855fcu), change_endian_long(0x26144fbeu), change_endian_long(0x914fce96u), change_endian_long(0x80fc250bu));
                                        }
                                        else
                                        {
                                            TSIP.REG_ECH.WORD = 0x02003cebu;
                                            TSIP.REG_ECH.WORD = 0x0000a560u;
                                            TSIP.REG_ECH.WORD = 0x00000001u;
                                            RX65NHU_func101(change_endian_long(0x75b5b667u), change_endian_long(0xb97e1495u), change_endian_long(0xee996f2fu), change_endian_long(0xc8e858eeu));
                                        }
                                        RX65NHU_func101(change_endian_long(0x6a687eb9u), change_endian_long(0x46cc157eu), change_endian_long(0x8696f907u), change_endian_long(0xb1309676u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c0028c2u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX65NHU_func100(change_endian_long(0x4040b82au), change_endian_long(0x910c026cu), change_endian_long(0xaae63bd0u), change_endian_long(0xe0e2f25eu));
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
                                        RX65NHU_func101(change_endian_long(0xe8d82eccu), change_endian_long(0x710e5b3au), change_endian_long(0xd4d49962u), change_endian_long(0x3c7bb774u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00002c40u;
                                    RX65NHU_func101(change_endian_long(0x106d6772u), change_endian_long(0xcca6e76du), change_endian_long(0xfe34e53eu), change_endian_long(0xafcbdd35u));
                                }
                                TSIP.REG_ECH.WORD = 0x08000c21u;
                                TSIP.REG_E0H.WORD = 0x00000080u;
                                TSIP.REG_1CH.WORD = 0x00260000u;
                                TSIP.REG_1CH.WORD = 0x00402000u;
                                TSIP.REG_ECH.WORD = 0x00002ca0u;
                                RX65NHU_func101(change_endian_long(0x4a75e750u), change_endian_long(0xed2781dfu), change_endian_long(0x61a379acu), change_endian_long(0x31c97a5bu));
                            }
                            TSIP.REG_ECH.WORD = 0x38000805u;
                            TSIP.REG_E0H.WORD = 0x00000080u;
                            TSIP.REG_1CH.WORD = 0x00260000u;
                            TSIP.REG_1CH.WORD = 0x00402000u;
                            RX65NHU_func101(change_endian_long(0x19a5e8abu), change_endian_long(0x55e61331u), change_endian_long(0x642551b5u), change_endian_long(0xba9b7232u));
                        }
                        else
                        {
                            RX65NHU_func100(change_endian_long(0x8898c2b5u), change_endian_long(0x0f76b43eu), change_endian_long(0x9b8a3f62u), change_endian_long(0x56e37b7du));
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
                                    RX65NHU_func100(change_endian_long(0x7593fd8cu), change_endian_long(0xea634a90u), change_endian_long(0x01ad1fbdu), change_endian_long(0xf0b0ad12u));
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
                                        RX65NHU_func101(change_endian_long(0xe23afdc5u), change_endian_long(0xd4c6e124u), change_endian_long(0x574c66b5u), change_endian_long(0x6d9408f7u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c002857u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x3c002b02u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX65NHU_func100(change_endian_long(0x1d29af57u), change_endian_long(0x8da75594u), change_endian_long(0xc8eae665u), change_endian_long(0x33179238u));
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
                                        RX65NHU_func101(change_endian_long(0x4f231b96u), change_endian_long(0x46b0d3deu), change_endian_long(0x494221abu), change_endian_long(0xbd579e45u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c0028c2u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX65NHU_func100(change_endian_long(0x734b2798u), change_endian_long(0xa2be6cffu), change_endian_long(0x3b495e0au), change_endian_long(0x0ce9a8a8u));
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
                                        RX65NHU_func101(change_endian_long(0xa4fdc8bfu), change_endian_long(0x9b1460e8u), change_endian_long(0xb4cb1d44u), change_endian_long(0x636a3b5du));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00002c40u;
                                    RX65NHU_func101(change_endian_long(0xdf9db3dbu), change_endian_long(0xe25c5a8du), change_endian_long(0x078d02e7u), change_endian_long(0x8588f459u));
                                }
                                TSIP.REG_ECH.WORD = 0x08000c21u;
                                TSIP.REG_E0H.WORD = 0x00000080u;
                                TSIP.REG_1CH.WORD = 0x00260000u;
                                TSIP.REG_1CH.WORD = 0x00402000u;
                                TSIP.REG_ECH.WORD = 0x00002ca0u;
                                RX65NHU_func101(change_endian_long(0x216a4662u), change_endian_long(0xc6f61f7cu), change_endian_long(0x565106aau), change_endian_long(0x8275a38du));
                            }
                            TSIP.REG_ECH.WORD = 0x38000805u;
                            TSIP.REG_E0H.WORD = 0x00000080u;
                            TSIP.REG_1CH.WORD = 0x00260000u;
                            TSIP.REG_1CH.WORD = 0x00402000u;
                            RX65NHU_func101(change_endian_long(0x7dafc7dcu), change_endian_long(0x053d3df0u), change_endian_long(0xe3eaba7bu), change_endian_long(0x5717ee6eu));
                        }
                        RX65NHU_func101(change_endian_long(0x69bb4395u), change_endian_long(0x0fc7279eu), change_endian_long(0x93598062u), change_endian_long(0x5ccfedd6u));
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
                        RX65NHU_func100(change_endian_long(0xcf827d1eu), change_endian_long(0xe65fa1a3u), change_endian_long(0x247674c7u), change_endian_long(0x61afacf1u));
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
                            RX65NHU_func101(change_endian_long(0x5213d595u), change_endian_long(0x46b382b3u), change_endian_long(0x5e573b75u), change_endian_long(0x957ef0c7u));
                        }
                        TSIP.REG_ECH.WORD = 0x38000805u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        TSIP.REG_1CH.WORD = 0x00402000u;
                        RX65NHU_func100(change_endian_long(0x51a0a5edu), change_endian_long(0xb03b6e06u), change_endian_long(0xb8b56f53u), change_endian_long(0x616fff25u));
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
                            RX65NHU_func101(change_endian_long(0x3132487du), change_endian_long(0x4e0f3b87u), change_endian_long(0x756b0ee6u), change_endian_long(0xc6de9828u));
                        }
                        TSIP.REG_ECH.WORD = 0x38000805u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        TSIP.REG_1CH.WORD = 0x00402000u;
                        RX65NHU_func101(change_endian_long(0xb5075f3eu), change_endian_long(0x0d11a9d1u), change_endian_long(0x18288be6u), change_endian_long(0xa665bd78u));
                    }
                    RX65NHU_func100(change_endian_long(0x165b6361u), change_endian_long(0x25192522u), change_endian_long(0x139fa557u), change_endian_long(0x5c1fa77eu));
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
                        RX65NHU_func101(change_endian_long(0xa45f83c8u), change_endian_long(0x2e83333du), change_endian_long(0xce3c9860u), change_endian_long(0x6ab372cdu));
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
                    RX65NHU_func100(change_endian_long(0xd0c0eb52u), change_endian_long(0x98ae779du), change_endian_long(0xe0b1bd6eu), change_endian_long(0x790c1469u));
                    RX65NHU_func103();
                    RX65NHU_func100(change_endian_long(0x1a536cedu), change_endian_long(0xb710bb77u), change_endian_long(0xf8ef8809u), change_endian_long(0xdd377170u));
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
                    RX65NHU_func100(change_endian_long(0xa9f2a813u), change_endian_long(0x841867ffu), change_endian_long(0x3621eeb6u), change_endian_long(0xfa7f6f2fu));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX65NHU_func100(change_endian_long(0x15012d63u), change_endian_long(0xd9154bf4u), change_endian_long(0x07291235u), change_endian_long(0x55a674cau));
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
                            RX65NHU_func100(change_endian_long(0x336901ecu), change_endian_long(0x67b14c60u), change_endian_long(0xbd74ca62u), change_endian_long(0xb1fe98a2u));
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
                                RX65NHU_func101(change_endian_long(0xeacca12cu), change_endian_long(0xf6015943u), change_endian_long(0x2c330815u), change_endian_long(0x18eba787u));
                            }
                            else
                            {
                                RX65NHU_func100(change_endian_long(0xb3ab0232u), change_endian_long(0x7aec7b22u), change_endian_long(0x319450b1u), change_endian_long(0x6709a9d6u));
                                TSIP.REG_104H.WORD = 0x00000058u;
                                TSIP.REG_E0H.WORD = 0x80010020u;
                                /* WAIT_LOOP */
                                while (1u != TSIP.REG_104H.BIT.B31)
                                {
                                    /* waiting */
                                }
                                TSIP.REG_100H.WORD = change_endian_long(0xaa51aeadu);
                                RX65NHU_func080();
                                RX65NHU_func100(change_endian_long(0xcae5f54fu), change_endian_long(0xed73c4f4u), change_endian_long(0xd604cdacu), change_endian_long(0xde08408au));
                                TSIP.REG_00H.WORD = 0x00012303u;
                                TSIP.REG_2CH.WORD = 0x00000020u;
                                RX65NHU_func313(1496);
                                RX65NHU_func100(change_endian_long(0x13afe29au), change_endian_long(0x12c7239bu), change_endian_long(0x6e9e0cceu), change_endian_long(0x524d836cu));
                                RX65NHU_func314(1496+64);
                                RX65NHU_func100(change_endian_long(0xfc3c6184u), change_endian_long(0xb4a79f6du), change_endian_long(0x37777f35u), change_endian_long(0x91b408f6u));
                                TSIP.REG_104H.WORD = 0x00000058u;
                                TSIP.REG_E0H.WORD = 0x80010020u;
                                /* WAIT_LOOP */
                                while (1u != TSIP.REG_104H.BIT.B31)
                                {
                                    /* waiting */
                                }
                                TSIP.REG_100H.WORD = change_endian_long(0x3e1a01cdu);
                                RX65NHU_func080();
                                RX65NHU_func100(change_endian_long(0x5f4232c4u), change_endian_long(0xa4f46aedu), change_endian_long(0x27a16ff5u), change_endian_long(0x72f3355fu));
                                TSIP.REG_ECH.WORD = 0x00000800u;
                                TSIP.REG_E0H.WORD = 0x81c00000u;
                                TSIP.REG_00H.WORD = 0x00012803u;
                                RX65NHU_func313(1496+68);
                                RX65NHU_func100(change_endian_long(0x4df54de9u), change_endian_long(0xa3c5ea82u), change_endian_long(0xd7955b37u), change_endian_long(0x1c3cdb8au));
                                RX65NHU_func314(1496+132);
                                RX65NHU_func101(change_endian_long(0x0c2789a2u), change_endian_long(0x294eb843u), change_endian_long(0x52236f5du), change_endian_long(0x767b550eu));
                            }
                            RX65NHU_func101(change_endian_long(0x1cae46e2u), change_endian_long(0xcaa58e93u), change_endian_long(0x8228bb0fu), change_endian_long(0xb738062du));
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
                            RX65NHU_func101(change_endian_long(0x10b9e2fdu), change_endian_long(0x3a7e4151u), change_endian_long(0x8368fb48u), change_endian_long(0x503e761bu));
                        }
                        RX65NHU_func101(change_endian_long(0x6915077au), change_endian_long(0xcaadada3u), change_endian_long(0xdbe085f9u), change_endian_long(0xb673e389u));
                    }
                    else
                    {
                        RX65NHU_func101(change_endian_long(0x82aa8520u), change_endian_long(0x88ea6f01u), change_endian_long(0x6dde5c97u), change_endian_long(0x5d90c14fu));
                    }
                    RX65NHU_func100(change_endian_long(0xd5c9fd00u), change_endian_long(0x126ec973u), change_endian_long(0xbc69292bu), change_endian_long(0x21f19a82u));
                    TSIP.REG_ECH.WORD = 0x00007c0eu;
                    TSIP.REG_1CH.WORD = 0x00600000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                    {
                        RX65NHU_func100(change_endian_long(0xa96b8821u), change_endian_long(0xb6cd2e21u), change_endian_long(0xd2e5ef61u), change_endian_long(0x5095e823u));
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
                        RX65NHU_func101(change_endian_long(0x693ab8feu), change_endian_long(0xcac32aa2u), change_endian_long(0x35affe1au), change_endian_long(0x3013264du));
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
                        RX65NHU_func100(change_endian_long(0xa5f179b5u), change_endian_long(0x199352a2u), change_endian_long(0x2c0c242eu), change_endian_long(0x33074661u));
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x5fe685f6u);
                        RX65NHU_func080();
                        RX65NHU_func100(change_endian_long(0x5e43952fu), change_endian_long(0xdfa2ab50u), change_endian_long(0x89ade5f8u), change_endian_long(0xfba854a3u));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000020u;
                        RX65NHU_func313(748);
                        RX65NHU_func100(change_endian_long(0x2e0ac9feu), change_endian_long(0x7d28b0acu), change_endian_long(0xc3576563u), change_endian_long(0x17f301efu));
                        RX65NHU_func314(748+64);
                        RX65NHU_func100(change_endian_long(0xca5368e5u), change_endian_long(0x9a684517u), change_endian_long(0x8713aacfu), change_endian_long(0x16bac76fu));
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x1c5a2b81u);
                        RX65NHU_func080();
                        RX65NHU_func100(change_endian_long(0xfd1b0904u), change_endian_long(0xdfeb1fa0u), change_endian_long(0x836b03d8u), change_endian_long(0x78c39437u));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000022u;
                        RX65NHU_func313(680);
                        RX65NHU_func100(change_endian_long(0x380e9b18u), change_endian_long(0xf4e6e00au), change_endian_long(0x9d0c2664u), change_endian_long(0xe7a2696au));
                        RX65NHU_func314(680+64);
                        RX65NHU_func100(change_endian_long(0x548238adu), change_endian_long(0x50708e1au), change_endian_long(0xb3364911u), change_endian_long(0x7626b0fbu));
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
                        RX65NHU_func100(change_endian_long(0xaaa1d5a6u), change_endian_long(0x567ef624u), change_endian_long(0x916df4e1u), change_endian_long(0x2b8aa664u));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000020u;
                        RX65NHU_func313(68);
                        RX65NHU_func100(change_endian_long(0xca501101u), change_endian_long(0xd5f92cfau), change_endian_long(0xee43d8a0u), change_endian_long(0xdf7efc50u));
                        RX65NHU_func314(68+64);
                        RX65NHU_func100(change_endian_long(0xa75dff7eu), change_endian_long(0x07717be6u), change_endian_long(0x39b00832u), change_endian_long(0x0a633faau));
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
                        RX65NHU_func100(change_endian_long(0x355e360du), change_endian_long(0x28d806d3u), change_endian_long(0x9e1f4437u), change_endian_long(0x5c83863du));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000024u;
                        RX65NHU_func313(0);
                        RX65NHU_func100(change_endian_long(0x6857fbdbu), change_endian_long(0x262108c5u), change_endian_long(0xaddf91b1u), change_endian_long(0xb512fadbu));
                        RX65NHU_func314(0+64);
                        TSIP.REG_ECH.WORD = 0x0000b400u;
                        TSIP.REG_ECH.WORD = 0x000000c1u;
                        RX65NHU_func101(change_endian_long(0x5f64b050u), change_endian_long(0x0cd68d91u), change_endian_long(0x6269a8bdu), change_endian_long(0xb471600cu));
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
                        RX65NHU_func101(change_endian_long(0xdc4b807fu), change_endian_long(0xbf24cc82u), change_endian_long(0x48942d79u), change_endian_long(0xa63867d0u));
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
                        TSIP.REG_100H.WORD = change_endian_long(0x000000c1u);
                        RX65NHU_func101(change_endian_long(0x9b41d865u), change_endian_long(0x71684040u), change_endian_long(0x1e42b943u), change_endian_long(0x673413ccu));
                        RX65NHU_func003(InData_Signature);
                        RX65NHU_func101(change_endian_long(0xcfa9bd7fu), change_endian_long(0xb9936920u), change_endian_long(0x5e88bb95u), change_endian_long(0x5c1f9cf7u));
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
                        TSIP.REG_ECH.WORD = 0x000000c1u;
                        TSIP.REG_ECH.WORD = 0x0000b440u;
                        TSIP.REG_ECH.WORD = 0x00000020u;
                        RX65NHU_func101(change_endian_long(0x027620c8u), change_endian_long(0xb08a8495u), change_endian_long(0x3119db8fu), change_endian_long(0x00a08d79u));
                        RX65NHU_func015();
                        RX65NHU_func101(change_endian_long(0xf36e46d6u), change_endian_long(0x9e5cd594u), change_endian_long(0xe03b55d8u), change_endian_long(0x89fbc1ecu));
                    }
                    RX65NHU_func100(change_endian_long(0x12def152u), change_endian_long(0x4d4b5718u), change_endian_long(0x873c8ee7u), change_endian_long(0x0df60ee9u));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX65NHU_func102(change_endian_long(0x955682d0u), change_endian_long(0x20576879u), change_endian_long(0x67609c57u), change_endian_long(0x874d3399u));
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
                        RX65NHU_func100(change_endian_long(0x2e79211eu), change_endian_long(0xe2357ba1u), change_endian_long(0x396c32b2u), change_endian_long(0x8428cdf8u));
                        TSIP.REG_1CH.WORD = 0x00400000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (1u == (TSIP.REG_1CH.BIT.B22))
                        {
                            RX65NHU_func102(change_endian_long(0x90378402u), change_endian_long(0xdd1a7e27u), change_endian_long(0x8b0bc94au), change_endian_long(0xbbfac356u));
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
                            RX65NHU_func100(change_endian_long(0x89554086u), change_endian_long(0x7a12230fu), change_endian_long(0x82376881u), change_endian_long(0x41a57d7du));
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
                                RX65NHU_func100(change_endian_long(0x7798fc14u), change_endian_long(0x4f015e24u), change_endian_long(0xd50c558du), change_endian_long(0x59bccc9cu));
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
                                    TSIP.REG_100H.WORD = change_endian_long(0x85880100u);
                                    RX65NHU_func101(change_endian_long(0x9fe9f8edu), change_endian_long(0x7eb5b4eau), change_endian_long(0xa35d5463u), change_endian_long(0x317eac1cu));
                                }
                                else
                                {
                                    RX65NHU_func100(change_endian_long(0x781460e8u), change_endian_long(0x77c72711u), change_endian_long(0xbd01ceeeu), change_endian_long(0x42f880f5u));
                                    TSIP.REG_C4H.WORD = 0x000c0805u;
                                    TSIP.REG_E0H.WORD = 0x810402e0u;
                                    TSIP.REG_00H.WORD = 0x00002813u;
                                    /* WAIT_LOOP */
                                    while (0u != TSIP.REG_00H.BIT.B25)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_1CH.WORD = 0x00001800u;
                                    RX65NHU_func100(change_endian_long(0xe55c4b6eu), change_endian_long(0x45204dd3u), change_endian_long(0xfd36feaau), change_endian_long(0x31942dbdu));
                                    TSIP.REG_104H.WORD = 0x00000058u;
                                    TSIP.REG_E0H.WORD = 0x80010020u;
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_104H.BIT.B31)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_100H.WORD = change_endian_long(0xaa51aeadu);
                                    RX65NHU_func080();
                                    RX65NHU_func100(change_endian_long(0x308b0ab1u), change_endian_long(0xe926a483u), change_endian_long(0xcaf63423u), change_endian_long(0xb2569495u));
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
                                    TSIP.REG_100H.WORD = change_endian_long(0x3e1a01cdu);
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
                                    TSIP.REG_100H.WORD = change_endian_long(0x8a880100u);
                                    RX65NHU_func101(change_endian_long(0x4cc7e7d9u), change_endian_long(0xa252a8bcu), change_endian_long(0xc566faabu), change_endian_long(0xd4fd3838u));
                                }
                                RX65NHU_func100(change_endian_long(0xddd15aa1u), change_endian_long(0xf0d15587u), change_endian_long(0x53d935aeu), change_endian_long(0x96ba63a4u));
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
                                RX65NHU_func100(change_endian_long(0x084abd6bu), change_endian_long(0xc53dff41u), change_endian_long(0x4a803346u), change_endian_long(0x9e545465u));
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
                                RX65NHU_func100(change_endian_long(0xfb4c9f01u), change_endian_long(0x9ee0086cu), change_endian_long(0x898b1805u), change_endian_long(0xa66f1591u));
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
                                RX65NHU_func100(change_endian_long(0x2f94a16au), change_endian_long(0xc768f8eeu), change_endian_long(0xec582615u), change_endian_long(0xf75b6c62u));
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
                                RX65NHU_func100(change_endian_long(0xfb94435du), change_endian_long(0x0e99afb9u), change_endian_long(0xcff5bc34u), change_endian_long(0xbf87761eu));
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
                                RX65NHU_func100(change_endian_long(0x6c3bf36du), change_endian_long(0xa0083091u), change_endian_long(0x722fd0b3u), change_endian_long(0x1a0a061au));
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
                                RX65NHU_func102(change_endian_long(0x3904d9efu), change_endian_long(0x2bc6ecd5u), change_endian_long(0x9bad17b0u), change_endian_long(0x73443910u));
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
                                RX65NHU_func100(change_endian_long(0x5d29ef22u), change_endian_long(0x28913ea2u), change_endian_long(0x8f765dccu), change_endian_long(0xdc3b2c82u));
                                TSIP.REG_1CH.WORD = 0x00400000u;
                                TSIP.REG_1D0H.WORD = 0x00000000u;
                                if (1u == (TSIP.REG_1CH.BIT.B22))
                                {
                                    RX65NHU_func102(change_endian_long(0xf1295fd5u), change_endian_long(0x7beb38afu), change_endian_long(0x16a8aaf2u), change_endian_long(0xc8ef036fu));
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
                                    RX65NHU_func100(change_endian_long(0xe7bf37d8u), change_endian_long(0x0ac03716u), change_endian_long(0x8bd532dcu), change_endian_long(0x2f227708u));
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
                                    TSIP.REG_100H.WORD = change_endian_long(0xb1080100u);
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
                                    RX65NHU_func100(change_endian_long(0xea56f91au), change_endian_long(0x6caf6b87u), change_endian_long(0xa0d93c91u), change_endian_long(0x25ba27e0u));
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
                                    RX65NHU_func100(change_endian_long(0x5cb1700fu), change_endian_long(0xb75c1408u), change_endian_long(0x94c17227u), change_endian_long(0x2ac93614u));
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
                                    RX65NHU_func100(change_endian_long(0xa8eca2f5u), change_endian_long(0xb6c07fafu), change_endian_long(0x774bd949u), change_endian_long(0xab74f995u));
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
                                    RX65NHU_func102(change_endian_long(0x99e32896u), change_endian_long(0xb4ba1dc7u), change_endian_long(0x8489bd80u), change_endian_long(0xd8bd952au));
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
 End of function ./input_dir/RX65NHU/RX65NHU_pc1.prc
 *********************************************************************************************************************/
