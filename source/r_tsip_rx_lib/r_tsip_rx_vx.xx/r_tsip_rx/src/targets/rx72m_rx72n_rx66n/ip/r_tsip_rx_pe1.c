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
* @details       RX72M TLS Certificate Verification
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0xd643aa87u), change_endian_long(0x526fc1bfu), change_endian_long(0xeb5acddfu), change_endian_long(0x6e99487du));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0xfec95cd2u), change_endian_long(0x14e75d33u), change_endian_long(0x7a7c3c83u), change_endian_long(0xba1b66bcu));
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
        RX72M_RX72N_RX66N_func100(change_endian_long(0xb438ea7eu), change_endian_long(0xb226a7c5u), change_endian_long(0x5b791651u), change_endian_long(0xa3acd66eu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func100(change_endian_long(0x510d4dd7u), change_endian_long(0x303998eau), change_endian_long(0x83e3db66u), change_endian_long(0xfcadbe5bu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x949e5b5bu), change_endian_long(0x8d5c4efau), change_endian_long(0x99849c09u), change_endian_long(0xee72e587u));
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
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x7eebf49bu), change_endian_long(0x82ee6fc3u), change_endian_long(0xba02d570u), change_endian_long(0x3284f496u));
                }
                else
                {
                    TSIP.REG_ECH.WORD = 0x00003417u;
                    TSIP.REG_ECH.WORD = 0x0000a400u;
                    TSIP.REG_ECH.WORD = 0x000001ffu;
                    TSIP.REG_ECH.WORD = 0x08002818u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00A60000u;
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x0245a8b5u), change_endian_long(0xa9bb3a93u), change_endian_long(0x337cbdbau), change_endian_long(0x1e6eace9u));
                }
                TSIP.REG_ECH.WORD = 0x00003419u;
                TSIP.REG_ECH.WORD = 0x0000a400u;
                TSIP.REG_ECH.WORD = 0x00000003u;
                TSIP.REG_ECH.WORD = 0x0c00281au;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00A60000u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0xc72c7689u), change_endian_long(0x144fc2beu), change_endian_long(0x67c53ce2u), change_endian_long(0xb19424c8u));
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0x99af5500u), change_endian_long(0x67cde7c1u), change_endian_long(0xa9f5676au), change_endian_long(0x60ab6450u));
            }
            RX72M_RX72N_RX66N_func101(change_endian_long(0x68ccbaf4u), change_endian_long(0x0017c275u), change_endian_long(0xf453a192u), change_endian_long(0x8c7b283fu));
        }
        else
        {
            RX72M_RX72N_RX66N_func101(change_endian_long(0x78b9e608u), change_endian_long(0x75fc5bf7u), change_endian_long(0x63dbdc11u), change_endian_long(0xffcc14f4u));
        }
        RX72M_RX72N_RX66N_func100(change_endian_long(0x19aeedd5u), change_endian_long(0xb534bd21u), change_endian_long(0xfb964fbbu), change_endian_long(0xcfb0b5abu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func102(change_endian_long(0xcbaff381u), change_endian_long(0x8f26e662u), change_endian_long(0x43cf6808u), change_endian_long(0xb5bdab61u));
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
            RX72M_RX72N_RX66N_func100(change_endian_long(0x1845f3b2u), change_endian_long(0xe1820e35u), change_endian_long(0x030595eeu), change_endian_long(0xb2f2249fu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX72M_RX72N_RX66N_func102(change_endian_long(0x665ecca0u), change_endian_long(0xfe9df684u), change_endian_long(0x33a90ce7u), change_endian_long(0x9671ac61u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x8d88b19cu), change_endian_long(0x2f741fccu), change_endian_long(0xe4cbe51fu), change_endian_long(0x5c5046b1u));
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
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x5bb1f8e4u), change_endian_long(0x2c1f74b2u), change_endian_long(0x21dbf5d5u), change_endian_long(0x034567b5u));
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
                    RX72M_RX72N_RX66N_func101(change_endian_long(0xeefd3355u), change_endian_long(0xa383c186u), change_endian_long(0xaa12a839u), change_endian_long(0xa2d23d58u));
                }
                RX72M_RX72N_RX66N_func100(change_endian_long(0x2d647279u), change_endian_long(0x7fc61679u), change_endian_long(0xd7e18ff8u), change_endian_long(0x8bd048d4u));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    RX72M_RX72N_RX66N_func102(change_endian_long(0x1652a1d8u), change_endian_long(0x8e5a4454u), change_endian_long(0x4e28b0abu), change_endian_long(0x7ac5b867u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xc3101a33u), change_endian_long(0x91b46577u), change_endian_long(0x5a78a6dfu), change_endian_long(0xa86281e8u));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x9ca741a0u), change_endian_long(0xdd70eed7u), change_endian_long(0xf7d40bb2u), change_endian_long(0x86bec9b4u));
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
                            RX72M_RX72N_RX66N_func101(change_endian_long(0x5f9e1be1u), change_endian_long(0xfe2427d5u), change_endian_long(0x57b5cfcdu), change_endian_long(0xa0b1fdc6u));
                        }
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x8ad110cbu), change_endian_long(0x3af952b8u), change_endian_long(0x7ebf44b4u), change_endian_long(0xdf56f2e1u));
                        TSIP.REG_ECH.WORD = 0x38000805u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        TSIP.REG_1CH.WORD = 0x00402000u;
                        TSIP.REG_ECH.WORD = 0x00007c0du;
                        TSIP.REG_1CH.WORD = 0x00600000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                        {
                            RX72M_RX72N_RX66N_func100(change_endian_long(0xebbb1d90u), change_endian_long(0xc4aa6e27u), change_endian_long(0x130ce779u), change_endian_long(0xf108a8beu));
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
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0xef9e97b9u), change_endian_long(0x00962f31u), change_endian_long(0x8c151009u), change_endian_long(0xb0f7c047u));
                                    TSIP.REG_1CH.WORD = 0x00400000u;
                                    TSIP.REG_1D0H.WORD = 0x00000000u;
                                    if (1u == (TSIP.REG_1CH.BIT.B22))
                                    {
                                        TSIP.REG_ECH.WORD = 0x3420a980u;
                                        TSIP.REG_ECH.WORD = 0x00000100u;
                                        TSIP.REG_E0H.WORD = 0x00000080u;
                                        TSIP.REG_1CH.WORD = 0x00260000u;
                                        RX72M_RX72N_RX66N_func100(change_endian_long(0x0958e950u), change_endian_long(0xc6b979e5u), change_endian_long(0xa95a7e43u), change_endian_long(0x76c9295au));
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
                                            RX72M_RX72N_RX66N_func101(change_endian_long(0x98442b3bu), change_endian_long(0x8c6ebe01u), change_endian_long(0x9aa272aau), change_endian_long(0xc97241e1u));
                                        }
                                        else
                                        {
                                            TSIP.REG_ECH.WORD = 0x02003cebu;
                                            TSIP.REG_ECH.WORD = 0x0000a560u;
                                            TSIP.REG_ECH.WORD = 0x00000001u;
                                            RX72M_RX72N_RX66N_func101(change_endian_long(0x3b764e91u), change_endian_long(0x24c27323u), change_endian_long(0xdf56e641u), change_endian_long(0x6687339cu));
                                        }
                                        RX72M_RX72N_RX66N_func101(change_endian_long(0x2976550cu), change_endian_long(0x79c6f773u), change_endian_long(0xfdd78ca2u), change_endian_long(0xfd863ba6u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c0028c2u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0x0071a69bu), change_endian_long(0xe3f1eeb4u), change_endian_long(0x94e3e606u), change_endian_long(0xc882ca14u));
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
                                        RX72M_RX72N_RX66N_func101(change_endian_long(0x30799d69u), change_endian_long(0xa692e092u), change_endian_long(0x2e1b57d3u), change_endian_long(0xb45b5cb4u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00002c40u;
                                    RX72M_RX72N_RX66N_func101(change_endian_long(0xdf69397au), change_endian_long(0xf13566eau), change_endian_long(0x261abd10u), change_endian_long(0xfb78711cu));
                                }
                                TSIP.REG_ECH.WORD = 0x08000c21u;
                                TSIP.REG_E0H.WORD = 0x00000080u;
                                TSIP.REG_1CH.WORD = 0x00260000u;
                                TSIP.REG_1CH.WORD = 0x00402000u;
                                TSIP.REG_ECH.WORD = 0x00002ca0u;
                                RX72M_RX72N_RX66N_func101(change_endian_long(0x55a0897fu), change_endian_long(0xfc5f554cu), change_endian_long(0x1767d514u), change_endian_long(0x0a563df1u));
                            }
                            TSIP.REG_ECH.WORD = 0x38000805u;
                            TSIP.REG_E0H.WORD = 0x00000080u;
                            TSIP.REG_1CH.WORD = 0x00260000u;
                            TSIP.REG_1CH.WORD = 0x00402000u;
                            RX72M_RX72N_RX66N_func101(change_endian_long(0x4ae23cf6u), change_endian_long(0xa1f0b7aeu), change_endian_long(0x42e7ba54u), change_endian_long(0x26d2b0c1u));
                        }
                        else
                        {
                            RX72M_RX72N_RX66N_func100(change_endian_long(0x22d70eb5u), change_endian_long(0xfe59d935u), change_endian_long(0x7da69cc5u), change_endian_long(0xd0aab494u));
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
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0x3891a038u), change_endian_long(0x711dc49au), change_endian_long(0xd1fbb0eau), change_endian_long(0x6b48a776u));
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
                                        RX72M_RX72N_RX66N_func101(change_endian_long(0xf4164b13u), change_endian_long(0x9f1d47c2u), change_endian_long(0x25d1de3du), change_endian_long(0x102d173cu));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c002857u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x3c002b02u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0x69ee9117u), change_endian_long(0x7442d20bu), change_endian_long(0xac63cf70u), change_endian_long(0x0cd9a2e6u));
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
                                        RX72M_RX72N_RX66N_func101(change_endian_long(0x240c6437u), change_endian_long(0x594cfdeeu), change_endian_long(0xc24671c9u), change_endian_long(0xfab1ea67u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c0028c2u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0x9a0da874u), change_endian_long(0x4fafeb16u), change_endian_long(0x59399c78u), change_endian_long(0x956b0a03u));
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
                                        RX72M_RX72N_RX66N_func101(change_endian_long(0x367a9a99u), change_endian_long(0x830efc17u), change_endian_long(0xdf4ff913u), change_endian_long(0xe551fb8fu));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00002c40u;
                                    RX72M_RX72N_RX66N_func101(change_endian_long(0x9070e977u), change_endian_long(0xf826d72au), change_endian_long(0xd19bf8fau), change_endian_long(0x6155cc51u));
                                }
                                TSIP.REG_ECH.WORD = 0x08000c21u;
                                TSIP.REG_E0H.WORD = 0x00000080u;
                                TSIP.REG_1CH.WORD = 0x00260000u;
                                TSIP.REG_1CH.WORD = 0x00402000u;
                                TSIP.REG_ECH.WORD = 0x00002ca0u;
                                RX72M_RX72N_RX66N_func101(change_endian_long(0x74d22eb5u), change_endian_long(0xdeb0f30eu), change_endian_long(0x2a8bb0a9u), change_endian_long(0x17a00c5cu));
                            }
                            TSIP.REG_ECH.WORD = 0x38000805u;
                            TSIP.REG_E0H.WORD = 0x00000080u;
                            TSIP.REG_1CH.WORD = 0x00260000u;
                            TSIP.REG_1CH.WORD = 0x00402000u;
                            RX72M_RX72N_RX66N_func101(change_endian_long(0xc0d0464fu), change_endian_long(0xf4e94147u), change_endian_long(0xcd584645u), change_endian_long(0x47fe4b82u));
                        }
                        RX72M_RX72N_RX66N_func101(change_endian_long(0xa269f25cu), change_endian_long(0x3b371df6u), change_endian_long(0x5189099fu), change_endian_long(0x3d0f81beu));
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
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xd187bebcu), change_endian_long(0x61c12551u), change_endian_long(0xf8de7133u), change_endian_long(0x9ff63b60u));
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
                            RX72M_RX72N_RX66N_func101(change_endian_long(0x44feee48u), change_endian_long(0xcf495e54u), change_endian_long(0xdbbe3820u), change_endian_long(0xd90827d1u));
                        }
                        TSIP.REG_ECH.WORD = 0x38000805u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        TSIP.REG_1CH.WORD = 0x00402000u;
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xfda519ceu), change_endian_long(0xafc78050u), change_endian_long(0xb7d65be1u), change_endian_long(0x17d832bbu));
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
                            RX72M_RX72N_RX66N_func101(change_endian_long(0x8dc351a2u), change_endian_long(0x0895312au), change_endian_long(0xf460ba58u), change_endian_long(0xc5910635u));
                        }
                        TSIP.REG_ECH.WORD = 0x38000805u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        TSIP.REG_1CH.WORD = 0x00402000u;
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x5d10992bu), change_endian_long(0x12ea8054u), change_endian_long(0xc77101d7u), change_endian_long(0xdd029961u));
                    }
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x22533b44u), change_endian_long(0x4647a189u), change_endian_long(0x749524b6u), change_endian_long(0x9c4cc552u));
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
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x32d96faeu), change_endian_long(0x9fdbb7a4u), change_endian_long(0x68f5e3edu), change_endian_long(0x25f1f205u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xe27a8a0fu), change_endian_long(0x7e752c95u), change_endian_long(0x95a1ec7bu), change_endian_long(0x1d122f0cu));
                    RX72M_RX72N_RX66N_func103();
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x75a51b09u), change_endian_long(0xe5a4758au), change_endian_long(0xb557c24eu), change_endian_long(0xb790a95cu));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xb3834652u), change_endian_long(0x938b41fau), change_endian_long(0xa5074b9cu), change_endian_long(0x55276d8bu));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x59cd0c67u), change_endian_long(0xef6c6fdfu), change_endian_long(0x60e37bb0u), change_endian_long(0x5bcba361u));
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
                            RX72M_RX72N_RX66N_func100(change_endian_long(0xd9454082u), change_endian_long(0xb06d9cc0u), change_endian_long(0x663ecf6du), change_endian_long(0xbf74ed84u));
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
                                RX72M_RX72N_RX66N_func101(change_endian_long(0xab266baau), change_endian_long(0xd263b961u), change_endian_long(0x8d9f2064u), change_endian_long(0x02faf772u));
                            }
                            else
                            {
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x20ade5e5u), change_endian_long(0xa76af6ebu), change_endian_long(0x5122bc24u), change_endian_long(0xfd5db562u));
                                TSIP.REG_104H.WORD = 0x00000058u;
                                TSIP.REG_E0H.WORD = 0x80010020u;
                                /* WAIT_LOOP */
                                while (1u != TSIP.REG_104H.BIT.B31)
                                {
                                    /* waiting */
                                }
                                TSIP.REG_100H.WORD = change_endian_long(0x5d09984du);
                                RX72M_RX72N_RX66N_func080();
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x3d9bc031u), change_endian_long(0xf88fe5d6u), change_endian_long(0xf41a8f2cu), change_endian_long(0x39f7f46fu));
                                TSIP.REG_00H.WORD = 0x00012303u;
                                TSIP.REG_2CH.WORD = 0x00000020u;
                                RX72M_RX72N_RX66N_func313(1496);
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x04e1bcbdu), change_endian_long(0xab7cccc7u), change_endian_long(0xd2be25cbu), change_endian_long(0x85288d18u));
                                RX72M_RX72N_RX66N_func314(1496+64);
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x48da20f2u), change_endian_long(0x7d7c3c4fu), change_endian_long(0x13aff90du), change_endian_long(0xfc48da31u));
                                TSIP.REG_104H.WORD = 0x00000058u;
                                TSIP.REG_E0H.WORD = 0x80010020u;
                                /* WAIT_LOOP */
                                while (1u != TSIP.REG_104H.BIT.B31)
                                {
                                    /* waiting */
                                }
                                TSIP.REG_100H.WORD = change_endian_long(0xcbc8750cu);
                                RX72M_RX72N_RX66N_func080();
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x6444b45fu), change_endian_long(0x04f7b658u), change_endian_long(0x1651bb80u), change_endian_long(0x19f71d2au));
                                TSIP.REG_ECH.WORD = 0x00000800u;
                                TSIP.REG_E0H.WORD = 0x81c00000u;
                                TSIP.REG_00H.WORD = 0x00012803u;
                                RX72M_RX72N_RX66N_func313(1496+68);
                                RX72M_RX72N_RX66N_func100(change_endian_long(0xe6b96a39u), change_endian_long(0xb789c205u), change_endian_long(0x6bd7bc43u), change_endian_long(0x9a002043u));
                                RX72M_RX72N_RX66N_func314(1496+132);
                                RX72M_RX72N_RX66N_func101(change_endian_long(0x8eecd56au), change_endian_long(0xd18c390bu), change_endian_long(0x3d15589fu), change_endian_long(0x401c3a5cu));
                            }
                            RX72M_RX72N_RX66N_func101(change_endian_long(0x3a48e611u), change_endian_long(0x779e5304u), change_endian_long(0x1848bc6bu), change_endian_long(0x8b368360u));
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
                            RX72M_RX72N_RX66N_func101(change_endian_long(0x89d86446u), change_endian_long(0xca056070u), change_endian_long(0x7eb09485u), change_endian_long(0x1d5a458au));
                        }
                        RX72M_RX72N_RX66N_func101(change_endian_long(0xfe4ab2bdu), change_endian_long(0x67ce90fbu), change_endian_long(0x20f2d598u), change_endian_long(0x28a0d861u));
                    }
                    else
                    {
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x7578249eu), change_endian_long(0xf2e8e33au), change_endian_long(0xf35c87d0u), change_endian_long(0x1d9928feu));
                    }
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x66adf1feu), change_endian_long(0xd8ffe002u), change_endian_long(0x7ed6f69fu), change_endian_long(0x9175f788u));
                    TSIP.REG_ECH.WORD = 0x00007c0eu;
                    TSIP.REG_1CH.WORD = 0x00600000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                    {
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x16c27698u), change_endian_long(0x7a939d29u), change_endian_long(0x48f41fbeu), change_endian_long(0x68e03f9du));
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
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x9b8347cdu), change_endian_long(0x7ecfc07fu), change_endian_long(0x1bf2eeaau), change_endian_long(0x1246923eu));
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
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x078510c4u), change_endian_long(0x195d9f89u), change_endian_long(0x63e63d18u), change_endian_long(0x952bbe15u));
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x5fe685f6u);
                        RX72M_RX72N_RX66N_func080();
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x7f873589u), change_endian_long(0x85033efcu), change_endian_long(0x89416191u), change_endian_long(0xaced2d7fu));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000020u;
                        RX72M_RX72N_RX66N_func313(748);
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x44b560c1u), change_endian_long(0xf5e98ca7u), change_endian_long(0xcc57608du), change_endian_long(0xd6cc9d1du));
                        RX72M_RX72N_RX66N_func314(748+64);
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x7423b72bu), change_endian_long(0x25b42aa8u), change_endian_long(0x917e5190u), change_endian_long(0x34b4d68au));
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x1c5a2b81u);
                        RX72M_RX72N_RX66N_func080();
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xd640750fu), change_endian_long(0x60f0f034u), change_endian_long(0x5f386995u), change_endian_long(0x5655b1aau));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000022u;
                        RX72M_RX72N_RX66N_func313(680);
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xe19e54a2u), change_endian_long(0x0efc9817u), change_endian_long(0xe577ee94u), change_endian_long(0xb51eedddu));
                        RX72M_RX72N_RX66N_func314(680+64);
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x682f0fa0u), change_endian_long(0x5e27e0dfu), change_endian_long(0x7e5b4bf2u), change_endian_long(0xb6dd2a17u));
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
                        RX72M_RX72N_RX66N_func080();
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x978c5cbdu), change_endian_long(0x6dc94486u), change_endian_long(0x9bb6adf3u), change_endian_long(0x2f2ad44du));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000020u;
                        RX72M_RX72N_RX66N_func313(68);
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xf56d1dc4u), change_endian_long(0xa55a21dbu), change_endian_long(0x4aba20b5u), change_endian_long(0xbe0d9013u));
                        RX72M_RX72N_RX66N_func314(68+64);
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x04e1c591u), change_endian_long(0x2cf969a4u), change_endian_long(0x5b97d9f1u), change_endian_long(0x97cd82deu));
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
                        RX72M_RX72N_RX66N_func080();
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xcb1eeac9u), change_endian_long(0x36fd3edcu), change_endian_long(0x049750d2u), change_endian_long(0x6badb7bdu));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000024u;
                        RX72M_RX72N_RX66N_func313(0);
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xe1b49eaau), change_endian_long(0x89c0672au), change_endian_long(0x77dcf966u), change_endian_long(0xaa8fc39cu));
                        RX72M_RX72N_RX66N_func314(0+64);
                        TSIP.REG_ECH.WORD = 0x0000b400u;
                        TSIP.REG_ECH.WORD = 0x00000001u;
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x309eb05du), change_endian_long(0xf118b0e9u), change_endian_long(0x15c01a82u), change_endian_long(0x1b8ed573u));
                        RX72M_RX72N_RX66N_func307();
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
                        RX72M_RX72N_RX66N_func101(change_endian_long(0xe59f92cdu), change_endian_long(0xf318436bu), change_endian_long(0xa413cdafu), change_endian_long(0xbbcc32bdu));
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
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x91d666bau), change_endian_long(0x437a3311u), change_endian_long(0xa132cde3u), change_endian_long(0xe6b03621u));
                        RX72M_RX72N_RX66N_func003(InData_Signature);
                        RX72M_RX72N_RX66N_func101(change_endian_long(0xa6aa3317u), change_endian_long(0x469e985eu), change_endian_long(0x77aa9b24u), change_endian_long(0xca80c19du));
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
                        RX72M_RX72N_RX66N_func101(change_endian_long(0xee9d1637u), change_endian_long(0x658d19f4u), change_endian_long(0x34756e26u), change_endian_long(0x407fa01eu));
                        RX72M_RX72N_RX66N_func015();
                        RX72M_RX72N_RX66N_func101(change_endian_long(0xe45467d6u), change_endian_long(0x28ec7d3au), change_endian_long(0x734ebb6bu), change_endian_long(0xcfa2dec3u));
                    }
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x1559c36du), change_endian_long(0x14684878u), change_endian_long(0xe33a8b5eu), change_endian_long(0x4c7aed85u));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX72M_RX72N_RX66N_func102(change_endian_long(0x144a1ed6u), change_endian_long(0xcb2757cfu), change_endian_long(0xa3229191u), change_endian_long(0x4e2e4d31u));
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
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xf58bd90bu), change_endian_long(0x66ab4455u), change_endian_long(0xa968a352u), change_endian_long(0x239f7444u));
                        TSIP.REG_1CH.WORD = 0x00400000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (1u == (TSIP.REG_1CH.BIT.B22))
                        {
                            RX72M_RX72N_RX66N_func102(change_endian_long(0xd45d7bc9u), change_endian_long(0x964c159au), change_endian_long(0x0eaa796cu), change_endian_long(0x9c1c4af6u));
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
                            RX72M_RX72N_RX66N_func100(change_endian_long(0x6a4627f5u), change_endian_long(0x8223476bu), change_endian_long(0xcdfc0a5cu), change_endian_long(0xc91acc9fu));
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
                                RX72M_RX72N_RX66N_func100(change_endian_long(0xedd84e84u), change_endian_long(0xf1d8ec68u), change_endian_long(0x2a8df27eu), change_endian_long(0xba26c851u));
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
                                    RX72M_RX72N_RX66N_func101(change_endian_long(0x40a644ccu), change_endian_long(0xa26f0e5bu), change_endian_long(0x45cefb31u), change_endian_long(0xba406d5bu));
                                }
                                else
                                {
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0x76f38100u), change_endian_long(0x8b084033u), change_endian_long(0x509baaf7u), change_endian_long(0xe10bef6fu));
                                    TSIP.REG_C4H.WORD = 0x000c0805u;
                                    TSIP.REG_E0H.WORD = 0x810402e0u;
                                    TSIP.REG_00H.WORD = 0x00002813u;
                                    /* WAIT_LOOP */
                                    while (0u != TSIP.REG_00H.BIT.B25)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_1CH.WORD = 0x00001800u;
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0x31089ae8u), change_endian_long(0xf1b6a0cdu), change_endian_long(0xd8a89804u), change_endian_long(0x88553704u));
                                    TSIP.REG_104H.WORD = 0x00000058u;
                                    TSIP.REG_E0H.WORD = 0x80010020u;
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_104H.BIT.B31)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_100H.WORD = change_endian_long(0x5d09984du);
                                    RX72M_RX72N_RX66N_func080();
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0x5c12dbc7u), change_endian_long(0x12d9eabdu), change_endian_long(0x5cb75c37u), change_endian_long(0x65b9282bu));
                                    TSIP.REG_00H.WORD = 0x00013203u;
                                    TSIP.REG_2CH.WORD = 0x00000014u;
                                    RX72M_RX72N_RX66N_func312(1496);
                                    TSIP.REG_104H.WORD = 0x00000058u;
                                    TSIP.REG_E0H.WORD = 0x80010020u;
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_104H.BIT.B31)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_100H.WORD = change_endian_long(0xcbc8750cu);
                                    RX72M_RX72N_RX66N_func080();
                                    TSIP.REG_00H.WORD = 0x00013203u;
                                    TSIP.REG_2CH.WORD = 0x00000012u;
                                    RX72M_RX72N_RX66N_func312(1496+68);
                                    TSIP.REG_104H.WORD = 0x00000058u;
                                    TSIP.REG_E0H.WORD = 0x80010000u;
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_104H.BIT.B31)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_100H.WORD = change_endian_long(0x0a880100u);
                                    RX72M_RX72N_RX66N_func101(change_endian_long(0xc4aa35c7u), change_endian_long(0x94de3c7cu), change_endian_long(0x6abefac3u), change_endian_long(0xb18da067u));
                                }
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x42a22225u), change_endian_long(0x9e2ad46au), change_endian_long(0x0952cf77u), change_endian_long(0x1e5f0237u));
                                RX72M_RX72N_RX66N_func103();
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
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x9dc33d68u), change_endian_long(0xbeaae601u), change_endian_long(0xfe29e427u), change_endian_long(0x308c691fu));
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
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x8fbed9cdu), change_endian_long(0x0987e57eu), change_endian_long(0x5e495af6u), change_endian_long(0x11e6edd2u));
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
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x8d1c45bfu), change_endian_long(0xcc851546u), change_endian_long(0xc5d2d29cu), change_endian_long(0xb1647f0fu));
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
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x5c9ac67eu), change_endian_long(0x09783a46u), change_endian_long(0x6357c4e8u), change_endian_long(0xc1f458a6u));
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
                                RX72M_RX72N_RX66N_func100(change_endian_long(0xc7dbf50au), change_endian_long(0x0de5d537u), change_endian_long(0x8c716ebeu), change_endian_long(0x0a035af5u));
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
                                RX72M_RX72N_RX66N_func102(change_endian_long(0x95b21764u), change_endian_long(0xa2f53be6u), change_endian_long(0xac1a7118u), change_endian_long(0xeb0f487eu));
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
                                RX72M_RX72N_RX66N_func004(KEY_ADR);
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
                                RX72M_RX72N_RX66N_func008();
                                RX72M_RX72N_RX66N_func100(change_endian_long(0xb1552278u), change_endian_long(0x55e980c4u), change_endian_long(0x8645919cu), change_endian_long(0x63f8adefu));
                                TSIP.REG_1CH.WORD = 0x00400000u;
                                TSIP.REG_1D0H.WORD = 0x00000000u;
                                if (1u == (TSIP.REG_1CH.BIT.B22))
                                {
                                    RX72M_RX72N_RX66N_func102(change_endian_long(0x4d788105u), change_endian_long(0x3e8600c0u), change_endian_long(0x1415cfc6u), change_endian_long(0x3e4105e9u));
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
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0x6693bcffu), change_endian_long(0xfbbb09b8u), change_endian_long(0x18242903u), change_endian_long(0xa7a2b047u));
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
                                    RX72M_RX72N_RX66N_func103();
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
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0x0118f7dcu), change_endian_long(0x7c7f538au), change_endian_long(0x2461d6deu), change_endian_long(0x080135f2u));
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
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0x3b14e58cu), change_endian_long(0xecbc027eu), change_endian_long(0xdb3ff10bu), change_endian_long(0x620950d0u));
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
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0x34cd1ebau), change_endian_long(0x1117a3a1u), change_endian_long(0x7461439fu), change_endian_long(0xebcd66a6u));
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
                                    RX72M_RX72N_RX66N_func102(change_endian_long(0x04d84ab2u), change_endian_long(0x4fefa6f9u), change_endian_long(0xe5490cf4u), change_endian_long(0xb9c22dd4u));
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
 End of function ./input_dir/RX72M/RX72M_pe1_r2.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_TLS == 1 */
