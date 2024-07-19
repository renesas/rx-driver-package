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
* Function Name: R_TSIP_TlsRootCertificateVerificationSub
*******************************************************************************************************************/ /**
* @details       RX72M TLS Root Certificate Verification
* @param[in]     InData_Sel_PubKeyType
* @param[in]     InData_Certificates
* @param[in]     InData_CertificatesLength
* @param[in]     InData_Signature
* @param[in]     InData_CertificatesInfo
* @param[out]    OutData_PubKey
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @note          None
*/
e_tsip_err_t R_TSIP_TlsRootCertificateVerificationSub(uint32_t *InData_Sel_PubKeyType, uint32_t *InData_Certificates, uint32_t *InData_CertificatesLength, uint32_t *InData_Signature, uint32_t *InData_CertificatesInfo, uint32_t *OutData_PubKey)
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
    TSIP.REG_84H.WORD = 0x0000e001u;
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0x1805a6e3u), change_endian_long(0x9f1062a1u), change_endian_long(0x1a1318a5u), change_endian_long(0xb85d2778u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0x613aabeeu), change_endian_long(0xa90c59feu), change_endian_long(0x7e702bc0u), change_endian_long(0x2bd4de9fu));
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
        RX72M_RX72N_RX66N_func100(change_endian_long(0x51c28ffcu), change_endian_long(0xf64a2c9cu), change_endian_long(0x85ec0612u), change_endian_long(0x24ec3fefu));
        TSIP.REG_104H.WORD = 0x00000468u;
        TSIP.REG_E0H.WORD = 0x800502c0u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_CertificatesLength[0];
        for (iLoop = 0; iLoop < 4; iLoop = iLoop + 1)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_CertificatesInfo[iLoop + 0];
        }
        TSIP.REG_ECH.WORD = 0x00007c0fu;
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
            TSIP.REG_ECH.WORD = 0x00003417u;
            TSIP.REG_ECH.WORD = 0x0000a400u;
            TSIP.REG_ECH.WORD = 0x000000ffu;
            TSIP.REG_ECH.WORD = 0x08002818u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            TSIP.REG_ECH.WORD = 0x00003419u;
            TSIP.REG_ECH.WORD = 0x0000a400u;
            TSIP.REG_ECH.WORD = 0x00000003u;
            TSIP.REG_ECH.WORD = 0x0c00281au;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0xeecf303bu), change_endian_long(0xda9f12d1u), change_endian_long(0x0b7b4d14u), change_endian_long(0xb5f76680u));
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
            RX72M_RX72N_RX66N_func101(change_endian_long(0x5a5a25e6u), change_endian_long(0x5ce18bfeu), change_endian_long(0x7a8c5b61u), change_endian_long(0x24fa13b1u));
        }
        RX72M_RX72N_RX66N_func100(change_endian_long(0xbb9f69b7u), change_endian_long(0x236428f7u), change_endian_long(0xfcec2e42u), change_endian_long(0xd99d7dd4u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func102(change_endian_long(0xe8954035u), change_endian_long(0xe853acc5u), change_endian_long(0x8a5d828bu), change_endian_long(0x8461e565u));
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
            RX72M_RX72N_RX66N_func100(change_endian_long(0xfc76c9f2u), change_endian_long(0xee1ee012u), change_endian_long(0xccc022e9u), change_endian_long(0xcb63053cu));
            TSIP.REG_C4H.WORD = 0x200c3b0du;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = S_INST[0];
            TSIP.REG_100H.WORD = S_INST[1];
            TSIP.REG_100H.WORD = S_INST[2];
            TSIP.REG_100H.WORD = S_INST[3];
            TSIP.REG_C4H.WORD = 0x00080b8cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x5cba9fa3u);
            TSIP.REG_C4H.WORD = 0x400c0b0cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000001u);
            TSIP.REG_28H.WORD = 0x00bf0001u;
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000014u;
            TSIP.REG_104H.WORD = 0x00007f62u;
            TSIP.REG_D0H.WORD = 0x00001f00u;
            TSIP.REG_C4H.WORD = 0x02f087bfu;
            for (iLoop = 0; iLoop < 64; iLoop = iLoop + 4)
            {
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = S_INST[4+iLoop + 0];
                TSIP.REG_100H.WORD = S_INST[4+iLoop + 1];
                TSIP.REG_100H.WORD = S_INST[4+iLoop + 2];
                TSIP.REG_100H.WORD = S_INST[4+iLoop + 3];
            }
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_00H.WORD = 0x00013203u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            for ( ; iLoop < 128; iLoop = iLoop + 4)
            {
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = S_INST[4+iLoop + 0];
                TSIP.REG_100H.WORD = S_INST[4+iLoop + 1];
                TSIP.REG_100H.WORD = S_INST[4+iLoop + 2];
                TSIP.REG_100H.WORD = S_INST[4+iLoop + 3];
            }
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_C4H.WORD = 0x00f007bdu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = S_INST[4+iLoop + 0];
            TSIP.REG_100H.WORD = S_INST[4+iLoop + 1];
            TSIP.REG_100H.WORD = S_INST[4+iLoop + 2];
            TSIP.REG_100H.WORD = S_INST[4+iLoop + 3];
            iLoop = iLoop+4;
            TSIP.REG_E0H.WORD = 0x80040140u;
            TSIP.REG_00H.WORD = 0x00008213u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_C4H.WORD = 0x000007bdu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = S_INST[4+iLoop + 0];
            TSIP.REG_100H.WORD = S_INST[4+iLoop + 1];
            TSIP.REG_100H.WORD = S_INST[4+iLoop + 2];
            TSIP.REG_100H.WORD = S_INST[4+iLoop + 3];
            TSIP.REG_C4H.WORD = 0x00800c45u;
            TSIP.REG_00H.WORD = 0x00002213u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
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
            TSIP.REG_04H.WORD = 0x00000707u;
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
                TSIP.REG_100H.WORD = InData_Certificates[iLoop + 0];
                TSIP.REG_100H.WORD = InData_Certificates[iLoop + 1];
                TSIP.REG_100H.WORD = InData_Certificates[iLoop + 2];
                TSIP.REG_100H.WORD = InData_Certificates[iLoop + 3];
                TSIP.REG_ECH.WORD = 0x0000a4a0u;
                TSIP.REG_ECH.WORD = 0x00000004u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0xc862a791u), change_endian_long(0xab814ebeu), change_endian_long(0x77915824u), change_endian_long(0x2cb11f5bu));
            }
            RX72M_RX72N_RX66N_func100(change_endian_long(0x97086936u), change_endian_long(0xd178d54au), change_endian_long(0xd1d21501u), change_endian_long(0xfa965591u));
            TSIP.REG_ECH.WORD = 0x38000805u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_1CH.WORD = 0x00402000u;
            TSIP.REG_ECH.WORD = 0x00007c0fu;
            TSIP.REG_1CH.WORD = 0x00600000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX72M_RX72N_RX66N_func100(change_endian_long(0xb266e6d8u), change_endian_long(0x03caec4du), change_endian_long(0x7277a61fu), change_endian_long(0x0bd996d7u));
                TSIP.REG_28H.WORD = 0x00bf0001u;
                TSIP.REG_2CH.WORD = 0x00000010u;
                TSIP.REG_ECH.WORD = 0x00003416u;
                TSIP.REG_ECH.WORD = 0x00026800u;
                TSIP.REG_ECH.WORD = 0x38008ec0u;
                TSIP.REG_ECH.WORD = 0x00000003u;
                TSIP.REG_ECH.WORD = 0x20002c00u;
                TSIP.REG_E0H.WORD = 0x81010000u;
                TSIP.REG_04H.WORD = 0x00000707u;
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
                    TSIP.REG_100H.WORD = InData_Certificates[iLoop + 0];
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
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xa333b083u), change_endian_long(0x6ef54609u), change_endian_long(0x7217311du), change_endian_long(0x0a4a1d8du));
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
                            RX72M_RX72N_RX66N_func101(change_endian_long(0x0bca5002u), change_endian_long(0x27a30e63u), change_endian_long(0x7c592752u), change_endian_long(0x3be2e97bu));
                        }
                        TSIP.REG_ECH.WORD = 0x00000863u;
                        TSIP.REG_ECH.WORD = 0x3c0028c2u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x38000c63u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xe548dd59u), change_endian_long(0x7d406637u), change_endian_long(0x65f86ba5u), change_endian_long(0x820e34fbu));
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
                            RX72M_RX72N_RX66N_func101(change_endian_long(0xc5afd8bcu), change_endian_long(0xdf89be39u), change_endian_long(0x2e06b7dau), change_endian_long(0xa7cd9ac3u));
                        }
                        TSIP.REG_ECH.WORD = 0x00002c40u;
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x10981952u), change_endian_long(0x5d19b27du), change_endian_long(0xd1e3aa2du), change_endian_long(0xe66d7660u));
                    }
                    TSIP.REG_ECH.WORD = 0x08000c21u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00260000u;
                    TSIP.REG_1CH.WORD = 0x00402000u;
                    TSIP.REG_ECH.WORD = 0x00002ca0u;
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x31938efbu), change_endian_long(0x6cd46e96u), change_endian_long(0x42300adbu), change_endian_long(0xf6ba8156u));
                }
                TSIP.REG_ECH.WORD = 0x38000805u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_1CH.WORD = 0x00402000u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0x11e3de2eu), change_endian_long(0x8452cc91u), change_endian_long(0xb79ae035u), change_endian_long(0x1951fc3au));
            }
            else
            {
                RX72M_RX72N_RX66N_func100(change_endian_long(0x42daf106u), change_endian_long(0x602d2782u), change_endian_long(0x6ae72041u), change_endian_long(0x71fc81fau));
                TSIP.REG_28H.WORD = 0x008f0001u;
                TSIP.REG_2CH.WORD = 0x00000010u;
                TSIP.REG_ECH.WORD = 0x00003416u;
                TSIP.REG_ECH.WORD = 0x00026800u;
                TSIP.REG_ECH.WORD = 0x38008ec0u;
                TSIP.REG_ECH.WORD = 0x00000003u;
                TSIP.REG_ECH.WORD = 0x20002c00u;
                TSIP.REG_E0H.WORD = 0x81010000u;
                TSIP.REG_04H.WORD = 0x00000707u;
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
                    TSIP.REG_100H.WORD = InData_Certificates[iLoop + 0];
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
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xf72f1881u), change_endian_long(0xc6125067u), change_endian_long(0x4c6aad46u), change_endian_long(0xbb49eeefu));
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
                            RX72M_RX72N_RX66N_func101(change_endian_long(0x3d14f14du), change_endian_long(0xa65848c1u), change_endian_long(0xbb1266e3u), change_endian_long(0x4f48fa55u));
                        }
                        TSIP.REG_ECH.WORD = 0x00000863u;
                        TSIP.REG_ECH.WORD = 0x3c002857u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x3c002b02u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x38000c63u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xf706d5f1u), change_endian_long(0x3b438103u), change_endian_long(0x3955adb2u), change_endian_long(0x01f24813u));
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
                            RX72M_RX72N_RX66N_func101(change_endian_long(0xed655201u), change_endian_long(0xaf1dd08fu), change_endian_long(0xaa9026a4u), change_endian_long(0xce78d650u));
                        }
                        TSIP.REG_ECH.WORD = 0x00000863u;
                        TSIP.REG_ECH.WORD = 0x3c0028c2u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x38000c63u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xe5b463afu), change_endian_long(0x3f0e8a3fu), change_endian_long(0xbeec325bu), change_endian_long(0x6fb94508u));
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
                            RX72M_RX72N_RX66N_func101(change_endian_long(0xd188662eu), change_endian_long(0xb756d4a4u), change_endian_long(0x7e527005u), change_endian_long(0xb0c4fed4u));
                        }
                        TSIP.REG_ECH.WORD = 0x00002c40u;
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x40cb44c9u), change_endian_long(0x1ed4b5f5u), change_endian_long(0xc5d2c770u), change_endian_long(0x4803ee54u));
                    }
                    TSIP.REG_ECH.WORD = 0x08000c21u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00260000u;
                    TSIP.REG_1CH.WORD = 0x00402000u;
                    TSIP.REG_ECH.WORD = 0x00002ca0u;
                    RX72M_RX72N_RX66N_func101(change_endian_long(0xa0bf4f1du), change_endian_long(0x9f7a9c54u), change_endian_long(0x38d35765u), change_endian_long(0x8d01980eu));
                }
                TSIP.REG_ECH.WORD = 0x38000805u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_1CH.WORD = 0x00402000u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0xccf9a95bu), change_endian_long(0x0aa30a13u), change_endian_long(0x164f6937u), change_endian_long(0xdbc556c3u));
            }
            RX72M_RX72N_RX66N_func100(change_endian_long(0xa2bdfa56u), change_endian_long(0xde014809u), change_endian_long(0x4fdee4b6u), change_endian_long(0x94b4cd94u));
            TSIP.REG_ECH.WORD = 0x00003416u;
            TSIP.REG_ECH.WORD = 0x00008c00u;
            TSIP.REG_ECH.WORD = 0x00000003u;
            TSIP.REG_ECH.WORD = 0x0000b440u;
            TSIP.REG_ECH.WORD = 0x00000004u;
            TSIP.REG_ECH.WORD = 0x00002840u;
            TSIP.REG_E0H.WORD = 0x81010040u;
            TSIP.REG_04H.WORD = 0x00000707u;
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0x2ea5d8dbu), change_endian_long(0x9f00d503u), change_endian_long(0x68dd79f2u), change_endian_long(0x6fc6f49cu));
            }
            RX72M_RX72N_RX66N_func100(change_endian_long(0x9fcba026u), change_endian_long(0xaac09a7eu), change_endian_long(0xdebf0af1u), change_endian_long(0xb3cfa29bu));
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
            TSIP.REG_ECH.WORD = 0x00007c0fu;
            TSIP.REG_1CH.WORD = 0x00600000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0x5cd324f4u), change_endian_long(0xee381494u), change_endian_long(0x351d4360u), change_endian_long(0x7090238eu));
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
                TSIP.REG_28H.WORD = 0x00bf0001u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0xbd087a77u), change_endian_long(0x3ba71a43u), change_endian_long(0x9dc782aeu), change_endian_long(0xa671c620u));
            }
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
            TSIP.REG_ECH.WORD = 0x000000e0u;
            TSIP.REG_ECH.WORD = 0x0000b440u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            TSIP.REG_ECH.WORD = 0x0000b4e0u;
            TSIP.REG_ECH.WORD = 0x000000f0u;
            TSIP.REG_E0H.WORD = 0x81840007u;
            TSIP.REG_C4H.WORD = 0x00000885u;
            TSIP.REG_00H.WORD = 0x00002813u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_E0H.WORD = 0x80040100u;
            TSIP.REG_00H.WORD = 0x00008213u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x42ce9c1fu), change_endian_long(0xa3514870u), change_endian_long(0xc929658fu), change_endian_long(0x1a143999u));
            R_TSIP_TlsRootCertificateVerificationSubSub();

            TSIP.REG_ECH.WORD = 0x0000b4e0u;
            TSIP.REG_ECH.WORD = 0x000000f0u;
            TSIP.REG_E0H.WORD = 0x81040100u;
            TSIP.REG_C4H.WORD = 0x00000885u;
            TSIP.REG_00H.WORD = 0x00002813u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_E0H.WORD = 0x80840007u;
            TSIP.REG_00H.WORD = 0x00008213u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX72M_RX72N_RX66N_func100(change_endian_long(0x75a83466u), change_endian_long(0x80d3fb76u), change_endian_long(0xa8fb1ce4u), change_endian_long(0x6e683cf8u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX72M_RX72N_RX66N_func102(change_endian_long(0xbb9c59a4u), change_endian_long(0x098dfeaau), change_endian_long(0x859642b3u), change_endian_long(0x0dace8b1u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xc72017cbu), change_endian_long(0xf544172cu), change_endian_long(0xaacbc6a1u), change_endian_long(0xe15fa06au));
                TSIP.REG_ECH.WORD = 0x00007c0fu;
                TSIP.REG_1CH.WORD = 0x00600000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x4dc730efu), change_endian_long(0x321a923fu), change_endian_long(0x8b18c744u), change_endian_long(0xc7f1fc69u));
                    TSIP.REG_28H.WORD = 0x00bf0001u;
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
                    TSIP.REG_100H.WORD = change_endian_long(0x05080100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x9587ee33u), change_endian_long(0x5e27bfcbu), change_endian_long(0x25a40fd8u), change_endian_long(0xd8abc754u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xa33a3cfau), change_endian_long(0xcf6f01b4u), change_endian_long(0x15d03af7u), change_endian_long(0xe67dd90bu));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x876aa716u), change_endian_long(0x7a93837du), change_endian_long(0x15eaf248u), change_endian_long(0xe339a7b7u));
                    TSIP.REG_C4H.WORD = 0x00e007bcu;
                    TSIP.REG_E0H.WORD = 0x81010200u;
                    TSIP.REG_00H.WORD = 0x00002807u;
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
                    OutData_PubKey[iLoop + 0] = TSIP.REG_100H.WORD;
                    OutData_PubKey[iLoop + 1] = TSIP.REG_100H.WORD;
                    OutData_PubKey[iLoop + 2] = TSIP.REG_100H.WORD;
                    OutData_PubKey[iLoop + 3] = TSIP.REG_100H.WORD;
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x10b716f9u), change_endian_long(0x38d52c1fu), change_endian_long(0x7a493558u), change_endian_long(0x529c3e8fu));
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
                    TSIP.REG_04H.WORD = 0x00000213u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_PubKey[iLoop + 0] = TSIP.REG_100H.WORD;
                    OutData_PubKey[iLoop + 1] = TSIP.REG_100H.WORD;
                    OutData_PubKey[iLoop + 2] = TSIP.REG_100H.WORD;
                    OutData_PubKey[iLoop + 3] = TSIP.REG_100H.WORD;
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xbd92f7cfu), change_endian_long(0x2b087d41u), change_endian_long(0x4a634b8eu), change_endian_long(0x30a3e06bu));
                    TSIP.REG_E0H.WORD = 0x81040000u;
                    TSIP.REG_04H.WORD = 0x00000613u;
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
                    RX72M_RX72N_RX66N_func102(change_endian_long(0xe4b5c2dau), change_endian_long(0x53664338u), change_endian_long(0x4f632876u), change_endian_long(0xb829a55bu));
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
                    OFS_ADR = 128;
                    RX72M_RX72N_RX66N_func004(OFS_ADR);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xce4ffe68u), change_endian_long(0x6faf2f8bu), change_endian_long(0x1379bf28u), change_endian_long(0x32ea8f7bu));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX72M_RX72N_RX66N_func102(change_endian_long(0xd75cba10u), change_endian_long(0xb5d4923bu), change_endian_long(0x2da0ccdau), change_endian_long(0x888d8e8au));
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
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xa65796cau), change_endian_long(0x154da067u), change_endian_long(0x36476f57u), change_endian_long(0x167b30eau));
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
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x6573d571u), change_endian_long(0x3ccfeb92u), change_endian_long(0x582044f0u), change_endian_long(0x289bd030u));
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
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x1b72f0b3u), change_endian_long(0x45d01ae4u), change_endian_long(0x69cefe3au), change_endian_long(0x40c12aa4u));
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
                        OutData_PubKey[iLoop + 0] = TSIP.REG_100H.WORD;
                        OutData_PubKey[iLoop + 1] = TSIP.REG_100H.WORD;
                        OutData_PubKey[iLoop + 2] = TSIP.REG_100H.WORD;
                        OutData_PubKey[iLoop + 3] = TSIP.REG_100H.WORD;
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x86d979cfu), change_endian_long(0x5a205f23u), change_endian_long(0xc1e42469u), change_endian_long(0x945429ebu));
                        TSIP.REG_E0H.WORD = 0x81040000u;
                        TSIP.REG_04H.WORD = 0x00000613u;
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
                        RX72M_RX72N_RX66N_func102(change_endian_long(0x9e80462eu), change_endian_long(0x714fe03fu), change_endian_long(0x9c202415u), change_endian_long(0xa1cea196u));
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
/**********************************************************************************************************************
 End of function ./input_dir/RX72M/RX72M_pe0.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_TLS == 1 */
