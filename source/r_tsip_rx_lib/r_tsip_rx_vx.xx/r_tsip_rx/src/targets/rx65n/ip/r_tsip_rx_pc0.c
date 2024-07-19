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
* Function Name: R_TSIP_TlsSVRootCertificateVerificationSub
*******************************************************************************************************************/ /**
* @details       RX65NHU TLS Root Certificate Verification for Server
* @param[in]     InData_Sel_PubKeyType
* @param[out]    
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
e_tsip_err_t R_TSIP_TlsSVRootCertificateVerificationSub(uint32_t *InData_Sel_PubKeyType, uint32_t *InData_Certificates, uint32_t *InData_CertificatesLength, uint32_t *InData_Signature, uint32_t *InData_CertificatesInfo, uint32_t *OutData_PubKey)
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
    TSIP.REG_84H.WORD = 0x0000c001u;
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
    RX65NHU_func100(change_endian_long(0xf32235c9u), change_endian_long(0xbf2cb941u), change_endian_long(0x670d506bu), change_endian_long(0x85641908u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0xa5c1d452u), change_endian_long(0xa4ce44c7u), change_endian_long(0xa52d9b6du), change_endian_long(0x28b6a427u));
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
        RX65NHU_func100(change_endian_long(0xb5a53dc2u), change_endian_long(0xdbf4a37fu), change_endian_long(0x7bd3137fu), change_endian_long(0x3f78212eu));
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
            RX65NHU_func101(change_endian_long(0x9711e6ffu), change_endian_long(0xbb3c0534u), change_endian_long(0xef0c01e9u), change_endian_long(0xdf797b6eu));
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
            RX65NHU_func101(change_endian_long(0xc371d1e0u), change_endian_long(0x144c038fu), change_endian_long(0xdd196e34u), change_endian_long(0xcf918667u));
        }
        RX65NHU_func100(change_endian_long(0x9fa4471bu), change_endian_long(0xa4e0813du), change_endian_long(0x40501e1au), change_endian_long(0x45cd2088u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func102(change_endian_long(0x6e7175cbu), change_endian_long(0x81805fb4u), change_endian_long(0xc23bffffu), change_endian_long(0x583a2664u));
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
            RX65NHU_func100(change_endian_long(0xdc120c31u), change_endian_long(0xc2d104aeu), change_endian_long(0x69f713fdu), change_endian_long(0x58248879u));
            TSIP.REG_C4H.WORD = 0x200c3b0du;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = S_INST[140];
            TSIP.REG_100H.WORD = S_INST[141];
            TSIP.REG_100H.WORD = S_INST[142];
            TSIP.REG_100H.WORD = S_INST[143];
            TSIP.REG_C4H.WORD = 0x00080b8cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x45c55efau);
            TSIP.REG_C4H.WORD = 0x400c0b0cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000002u);
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
                TSIP.REG_100H.WORD = S_INST[144+iLoop + 0];
                TSIP.REG_100H.WORD = S_INST[144+iLoop + 1];
                TSIP.REG_100H.WORD = S_INST[144+iLoop + 2];
                TSIP.REG_100H.WORD = S_INST[144+iLoop + 3];
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
                TSIP.REG_100H.WORD = S_INST[144+iLoop + 0];
                TSIP.REG_100H.WORD = S_INST[144+iLoop + 1];
                TSIP.REG_100H.WORD = S_INST[144+iLoop + 2];
                TSIP.REG_100H.WORD = S_INST[144+iLoop + 3];
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
            TSIP.REG_100H.WORD = S_INST[144+iLoop + 0];
            TSIP.REG_100H.WORD = S_INST[144+iLoop + 1];
            TSIP.REG_100H.WORD = S_INST[144+iLoop + 2];
            TSIP.REG_100H.WORD = S_INST[144+iLoop + 3];
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
            TSIP.REG_100H.WORD = S_INST[144+iLoop + 0];
            TSIP.REG_100H.WORD = S_INST[144+iLoop + 1];
            TSIP.REG_100H.WORD = S_INST[144+iLoop + 2];
            TSIP.REG_100H.WORD = S_INST[144+iLoop + 3];
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
                TSIP.REG_100H.WORD = InData_Certificates[iLoop + 0];
                TSIP.REG_100H.WORD = InData_Certificates[iLoop + 1];
                TSIP.REG_100H.WORD = InData_Certificates[iLoop + 2];
                TSIP.REG_100H.WORD = InData_Certificates[iLoop + 3];
                TSIP.REG_ECH.WORD = 0x0000a4a0u;
                TSIP.REG_ECH.WORD = 0x00000004u;
                RX65NHU_func101(change_endian_long(0xe1023414u), change_endian_long(0xcb6e73e6u), change_endian_long(0xcd56e082u), change_endian_long(0xb4707838u));
            }
            RX65NHU_func100(change_endian_long(0x28dc253au), change_endian_long(0x3fbb0c07u), change_endian_long(0x8a2151c3u), change_endian_long(0xdee6605du));
            TSIP.REG_ECH.WORD = 0x38000805u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_1CH.WORD = 0x00402000u;
            TSIP.REG_ECH.WORD = 0x00007c0fu;
            TSIP.REG_1CH.WORD = 0x00600000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX65NHU_func100(change_endian_long(0x9ecc2323u), change_endian_long(0xee0b384eu), change_endian_long(0xb5b51619u), change_endian_long(0xf3b26b94u));
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
                        RX65NHU_func100(change_endian_long(0x610c8bfdu), change_endian_long(0xd88dc21eu), change_endian_long(0xfec45f64u), change_endian_long(0xecae006bu));
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
                            RX65NHU_func101(change_endian_long(0xdf58cd5du), change_endian_long(0x1144b183u), change_endian_long(0x72d7efc7u), change_endian_long(0x7f403fa7u));
                        }
                        TSIP.REG_ECH.WORD = 0x00000863u;
                        TSIP.REG_ECH.WORD = 0x3c0028c2u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x38000c63u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        RX65NHU_func100(change_endian_long(0x102e6f0du), change_endian_long(0x7ddf1ef9u), change_endian_long(0x12b54733u), change_endian_long(0xb8066a26u));
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
                            RX65NHU_func101(change_endian_long(0x7ad1a69cu), change_endian_long(0xe9fa5853u), change_endian_long(0xefe69a02u), change_endian_long(0x467d2320u));
                        }
                        TSIP.REG_ECH.WORD = 0x00002c40u;
                        RX65NHU_func101(change_endian_long(0x30f7b3f4u), change_endian_long(0x2f57f653u), change_endian_long(0xb2dcac9au), change_endian_long(0x4e4c509eu));
                    }
                    TSIP.REG_ECH.WORD = 0x08000c21u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00260000u;
                    TSIP.REG_1CH.WORD = 0x00402000u;
                    TSIP.REG_ECH.WORD = 0x00002ca0u;
                    RX65NHU_func101(change_endian_long(0x539402dau), change_endian_long(0xaa6ed17bu), change_endian_long(0x02e3b7b5u), change_endian_long(0x4be5ea54u));
                }
                TSIP.REG_ECH.WORD = 0x38000805u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_1CH.WORD = 0x00402000u;
                RX65NHU_func101(change_endian_long(0x340cd863u), change_endian_long(0xb94ee78fu), change_endian_long(0x1f2e0ac3u), change_endian_long(0xf426df97u));
            }
            else
            {
                RX65NHU_func100(change_endian_long(0xf01e1f53u), change_endian_long(0x034817cfu), change_endian_long(0x57818064u), change_endian_long(0xeadf92f5u));
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
                        RX65NHU_func100(change_endian_long(0x0c8dd4f3u), change_endian_long(0xa0060d98u), change_endian_long(0x3e32a9fbu), change_endian_long(0x6c6d3d73u));
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
                            RX65NHU_func101(change_endian_long(0x324cc4a2u), change_endian_long(0x416eac74u), change_endian_long(0x8c064b07u), change_endian_long(0x575263f4u));
                        }
                        TSIP.REG_ECH.WORD = 0x00000863u;
                        TSIP.REG_ECH.WORD = 0x3c002857u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x3c002b02u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x38000c63u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        RX65NHU_func100(change_endian_long(0xbf4d2cb0u), change_endian_long(0x408552bfu), change_endian_long(0x54fbe699u), change_endian_long(0xb4a04662u));
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
                            RX65NHU_func101(change_endian_long(0x98f2e667u), change_endian_long(0x4de56a5du), change_endian_long(0x963bf026u), change_endian_long(0x292dd283u));
                        }
                        TSIP.REG_ECH.WORD = 0x00000863u;
                        TSIP.REG_ECH.WORD = 0x3c0028c2u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x38000c63u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        RX65NHU_func100(change_endian_long(0xfea6903eu), change_endian_long(0x64d80b14u), change_endian_long(0x3bff2a27u), change_endian_long(0x48836de5u));
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
                            RX65NHU_func101(change_endian_long(0x878c1da3u), change_endian_long(0xfd7f912fu), change_endian_long(0xfda7c52fu), change_endian_long(0x536aa84eu));
                        }
                        TSIP.REG_ECH.WORD = 0x00002c40u;
                        RX65NHU_func101(change_endian_long(0x2308ce35u), change_endian_long(0xd6c3cd18u), change_endian_long(0x36ecc8b8u), change_endian_long(0xa4406d38u));
                    }
                    TSIP.REG_ECH.WORD = 0x08000c21u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00260000u;
                    TSIP.REG_1CH.WORD = 0x00402000u;
                    TSIP.REG_ECH.WORD = 0x00002ca0u;
                    RX65NHU_func101(change_endian_long(0x53aaf390u), change_endian_long(0x645d84e1u), change_endian_long(0x5f05371bu), change_endian_long(0xeb83042fu));
                }
                TSIP.REG_ECH.WORD = 0x38000805u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_1CH.WORD = 0x00402000u;
                RX65NHU_func101(change_endian_long(0x4a97c95du), change_endian_long(0x531feadfu), change_endian_long(0xccc7a6fau), change_endian_long(0x08167148u));
            }
            RX65NHU_func100(change_endian_long(0xcdf7143cu), change_endian_long(0xa7a7e50au), change_endian_long(0xb2a5a8f6u), change_endian_long(0xc81ea198u));
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
                RX65NHU_func101(change_endian_long(0xc7eb623du), change_endian_long(0xa012a26bu), change_endian_long(0x415ebc95u), change_endian_long(0xeeec27a1u));
            }
            RX65NHU_func100(change_endian_long(0xa150479eu), change_endian_long(0x6a051f35u), change_endian_long(0xa693b587u), change_endian_long(0xc2470658u));
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
                RX65NHU_func101(change_endian_long(0x9f37596eu), change_endian_long(0x73233c11u), change_endian_long(0x026062e3u), change_endian_long(0xcd8abcf0u));
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
                RX65NHU_func101(change_endian_long(0xf83f6134u), change_endian_long(0x58abde44u), change_endian_long(0x4d8d5eb4u), change_endian_long(0xe941eed1u));
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
            TSIP.REG_ECH.WORD = 0x000000c0u;
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
            RX65NHU_func101(change_endian_long(0x5f068ad9u), change_endian_long(0x47336b8du), change_endian_long(0x73f8683eu), change_endian_long(0xe824e377u));
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
            RX65NHU_func100(change_endian_long(0xcea13b4eu), change_endian_long(0xf5efb47cu), change_endian_long(0xa9e53c64u), change_endian_long(0x85c95357u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX65NHU_func102(change_endian_long(0xdcadacd9u), change_endian_long(0xf0578193u), change_endian_long(0xfc3e2264u), change_endian_long(0x30d78c67u));
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
                RX65NHU_func100(change_endian_long(0xb1f6f60eu), change_endian_long(0xb68fe734u), change_endian_long(0xc4df1ad0u), change_endian_long(0xac166e93u));
                TSIP.REG_ECH.WORD = 0x00007c0fu;
                TSIP.REG_1CH.WORD = 0x00600000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    RX65NHU_func100(change_endian_long(0x787f670eu), change_endian_long(0x8d6f74d2u), change_endian_long(0xf2270ff1u), change_endian_long(0x039e689fu));
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
                    TSIP.REG_100H.WORD = change_endian_long(0x85080100u);
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
                    RX65NHU_func100(change_endian_long(0x37bf003eu), change_endian_long(0x01f92a38u), change_endian_long(0x3097cdadu), change_endian_long(0xc6bcb052u));
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
                    RX65NHU_func100(change_endian_long(0x2b28b3d3u), change_endian_long(0x9dc39c91u), change_endian_long(0x911dbadcu), change_endian_long(0xdec27613u));
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
                    RX65NHU_func100(change_endian_long(0x9ad63085u), change_endian_long(0x34292f39u), change_endian_long(0xca4c876eu), change_endian_long(0x635f52e9u));
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
                    RX65NHU_func100(change_endian_long(0x7002db38u), change_endian_long(0xa9b0e1bdu), change_endian_long(0xda3f2af2u), change_endian_long(0xe34859fbu));
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
                    RX65NHU_func100(change_endian_long(0xc6427b4fu), change_endian_long(0xcf9c4cb5u), change_endian_long(0x8445489cu), change_endian_long(0x0278ca17u));
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
                    RX65NHU_func102(change_endian_long(0xb5e99cddu), change_endian_long(0x17bb3e5du), change_endian_long(0x5ad2b5edu), change_endian_long(0x1d30151fu));
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
                    RX65NHU_func100(change_endian_long(0x2b1a89e3u), change_endian_long(0x9861f8beu), change_endian_long(0xe4992783u), change_endian_long(0x84be4aadu));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX65NHU_func102(change_endian_long(0x1208e2bdu), change_endian_long(0x1891e86bu), change_endian_long(0xe1d74437u), change_endian_long(0x86aa4c11u));
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
                        RX65NHU_func100(change_endian_long(0x797f3e9fu), change_endian_long(0x9d45f872u), change_endian_long(0x4d4934c2u), change_endian_long(0x38d32f5au));
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
                        RX65NHU_func100(change_endian_long(0x290ca078u), change_endian_long(0x3c465999u), change_endian_long(0xee435ce7u), change_endian_long(0x8da8a2c4u));
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
                        RX65NHU_func100(change_endian_long(0x5e6aaf7bu), change_endian_long(0x79d82c15u), change_endian_long(0x8265d5c0u), change_endian_long(0xd02e9859u));
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
                        RX65NHU_func100(change_endian_long(0x5ff76c96u), change_endian_long(0xf23f560eu), change_endian_long(0x068dac2du), change_endian_long(0x7b3f5279u));
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
                        RX65NHU_func102(change_endian_long(0x4491af8du), change_endian_long(0xb8fdd4c9u), change_endian_long(0x22696dd4u), change_endian_long(0x2abc38e4u));
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
 End of function ./input_dir/RX65NHU/RX65NHU_pc0.prc
 *********************************************************************************************************************/
