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
* @details       RX671 TLS Root Certificate Verification
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
    RX671_func100(change_endian_long(0x293ad3bau), change_endian_long(0xfd653116u), change_endian_long(0x1ef9515eu), change_endian_long(0xfd1c4b19u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func102(change_endian_long(0xd6d357a4u), change_endian_long(0xe7263d22u), change_endian_long(0x90dcd4ceu), change_endian_long(0x7b927bb8u));
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
        RX671_func100(change_endian_long(0x4710529bu), change_endian_long(0x79bd8b47u), change_endian_long(0x73ec380au), change_endian_long(0x065b5730u));
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
            RX671_func101(change_endian_long(0x0e6ff711u), change_endian_long(0x8af30014u), change_endian_long(0x0f072489u), change_endian_long(0x337a9462u));
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
            RX671_func101(change_endian_long(0x2f93fd47u), change_endian_long(0xe29c8043u), change_endian_long(0x6bf2b9a2u), change_endian_long(0x259e5dc3u));
        }
        RX671_func100(change_endian_long(0x06aa98d4u), change_endian_long(0xa0b26ac7u), change_endian_long(0xf8e68e90u), change_endian_long(0x1ced5ebdu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func102(change_endian_long(0x328ed052u), change_endian_long(0xb6031883u), change_endian_long(0xe0ad15e3u), change_endian_long(0x222194abu));
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
            RX671_func100(change_endian_long(0xadc66001u), change_endian_long(0x07bb776au), change_endian_long(0xdaa46e90u), change_endian_long(0x7548b7e3u));
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
                RX671_func101(change_endian_long(0xd0d4c293u), change_endian_long(0x4290f539u), change_endian_long(0xe17caf9fu), change_endian_long(0xbfd37e6bu));
            }
            RX671_func100(change_endian_long(0x5f45216fu), change_endian_long(0xf0ae9838u), change_endian_long(0x343ed48au), change_endian_long(0x8f911673u));
            TSIP.REG_ECH.WORD = 0x38000805u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_1CH.WORD = 0x00402000u;
            TSIP.REG_ECH.WORD = 0x00007c0fu;
            TSIP.REG_1CH.WORD = 0x00600000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX671_func100(change_endian_long(0xa916cc7eu), change_endian_long(0xaf29bf15u), change_endian_long(0xada9beddu), change_endian_long(0xe03af621u));
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
                        RX671_func100(change_endian_long(0x7b6c8aeeu), change_endian_long(0x7ff7322bu), change_endian_long(0x401c1867u), change_endian_long(0xd5fe6660u));
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
                            RX671_func101(change_endian_long(0x949451f1u), change_endian_long(0x07eab6cbu), change_endian_long(0x55bc022eu), change_endian_long(0x531abc9eu));
                        }
                        TSIP.REG_ECH.WORD = 0x00000863u;
                        TSIP.REG_ECH.WORD = 0x3c0028c2u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x38000c63u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        RX671_func100(change_endian_long(0x9180d54bu), change_endian_long(0xb8d74669u), change_endian_long(0x8d9e645au), change_endian_long(0xcbda5cbbu));
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
                            RX671_func101(change_endian_long(0x3b1c6b7au), change_endian_long(0x93ab237du), change_endian_long(0x78b8339bu), change_endian_long(0xad106f78u));
                        }
                        TSIP.REG_ECH.WORD = 0x00002c40u;
                        RX671_func101(change_endian_long(0xe7bf7379u), change_endian_long(0x13208156u), change_endian_long(0xd0866bd0u), change_endian_long(0x5f1c687fu));
                    }
                    TSIP.REG_ECH.WORD = 0x08000c21u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00260000u;
                    TSIP.REG_1CH.WORD = 0x00402000u;
                    TSIP.REG_ECH.WORD = 0x00002ca0u;
                    RX671_func101(change_endian_long(0x0aacd7ebu), change_endian_long(0x500f830bu), change_endian_long(0x484e32b3u), change_endian_long(0x47766508u));
                }
                TSIP.REG_ECH.WORD = 0x38000805u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_1CH.WORD = 0x00402000u;
                RX671_func101(change_endian_long(0x66b64249u), change_endian_long(0xf1cc1a23u), change_endian_long(0x0057150fu), change_endian_long(0x6ca00bf3u));
            }
            else
            {
                RX671_func100(change_endian_long(0xcc0661ceu), change_endian_long(0x3dd16c3fu), change_endian_long(0x805c643eu), change_endian_long(0x9dd5fdbbu));
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
                        RX671_func100(change_endian_long(0x2db14be4u), change_endian_long(0xcccf1a0fu), change_endian_long(0x16346b99u), change_endian_long(0x2d19f37cu));
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
                            RX671_func101(change_endian_long(0xfde508a1u), change_endian_long(0x2e7742e9u), change_endian_long(0xcf5d7bd1u), change_endian_long(0x49de43fcu));
                        }
                        TSIP.REG_ECH.WORD = 0x00000863u;
                        TSIP.REG_ECH.WORD = 0x3c002857u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x3c002b02u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x38000c63u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        RX671_func100(change_endian_long(0x1220df10u), change_endian_long(0x79623715u), change_endian_long(0x7db1624bu), change_endian_long(0x5d7cc8bfu));
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
                            RX671_func101(change_endian_long(0x255e66a6u), change_endian_long(0xcf255d96u), change_endian_long(0x3100ba43u), change_endian_long(0x2534572du));
                        }
                        TSIP.REG_ECH.WORD = 0x00000863u;
                        TSIP.REG_ECH.WORD = 0x3c0028c2u;
                        TSIP.REG_ECH.WORD = 0x20002c60u;
                        TSIP.REG_ECH.WORD = 0x38000c63u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        RX671_func100(change_endian_long(0x6529d5ecu), change_endian_long(0x67f4c181u), change_endian_long(0x9e646a7fu), change_endian_long(0x75b9e18au));
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
                            RX671_func101(change_endian_long(0x6c6312a3u), change_endian_long(0x9f56d20du), change_endian_long(0x7cec2026u), change_endian_long(0x96184081u));
                        }
                        TSIP.REG_ECH.WORD = 0x00002c40u;
                        RX671_func101(change_endian_long(0xa90db831u), change_endian_long(0xe660861du), change_endian_long(0x670809fbu), change_endian_long(0x53f0ff66u));
                    }
                    TSIP.REG_ECH.WORD = 0x08000c21u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00260000u;
                    TSIP.REG_1CH.WORD = 0x00402000u;
                    TSIP.REG_ECH.WORD = 0x00002ca0u;
                    RX671_func101(change_endian_long(0x5fff0b4eu), change_endian_long(0x05e30e13u), change_endian_long(0x95da4039u), change_endian_long(0x2f6135a7u));
                }
                TSIP.REG_ECH.WORD = 0x38000805u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_1CH.WORD = 0x00402000u;
                RX671_func101(change_endian_long(0xb65719f0u), change_endian_long(0x748ae666u), change_endian_long(0x37cef309u), change_endian_long(0xe0164bd0u));
            }
            RX671_func100(change_endian_long(0x9a2bbe7au), change_endian_long(0x16d16ec3u), change_endian_long(0xa1b8a98bu), change_endian_long(0xce91d73cu));
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
                RX671_func101(change_endian_long(0x1769387eu), change_endian_long(0x0ce44957u), change_endian_long(0xaffc9ea2u), change_endian_long(0x5c0d75bcu));
            }
            RX671_func100(change_endian_long(0x01b7a31bu), change_endian_long(0x09b3d683u), change_endian_long(0x248e0812u), change_endian_long(0x93f46965u));
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
                RX671_func101(change_endian_long(0x8ed38a31u), change_endian_long(0x4cb06350u), change_endian_long(0x1f044384u), change_endian_long(0xf8487cf0u));
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
                RX671_func101(change_endian_long(0x961de27au), change_endian_long(0xae3d1358u), change_endian_long(0xab5bb52cu), change_endian_long(0xe71239f7u));
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
            RX671_func101(change_endian_long(0x71dc0c06u), change_endian_long(0x4935fab7u), change_endian_long(0x07a25050u), change_endian_long(0xb2f16e3fu));
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
            RX671_func100(change_endian_long(0x3da29ff7u), change_endian_long(0xe7cf9c4bu), change_endian_long(0x1ce86a07u), change_endian_long(0xef67e8ffu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX671_func102(change_endian_long(0x961f9063u), change_endian_long(0xe94a0ed1u), change_endian_long(0x5e3cf0b1u), change_endian_long(0x66feb75eu));
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
                RX671_func100(change_endian_long(0xfa680122u), change_endian_long(0x08b448bau), change_endian_long(0x52ba73e6u), change_endian_long(0xd5a43eabu));
                TSIP.REG_ECH.WORD = 0x00007c0fu;
                TSIP.REG_1CH.WORD = 0x00600000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    RX671_func100(change_endian_long(0x4a8739e7u), change_endian_long(0x95504d3du), change_endian_long(0xccf30cbbu), change_endian_long(0x7abbf22bu));
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
                    RX671_func100(change_endian_long(0xaeded958u), change_endian_long(0x7f68e406u), change_endian_long(0x8e065cf6u), change_endian_long(0x2c251588u));
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
                    RX671_func100(change_endian_long(0x074728f8u), change_endian_long(0x0248d143u), change_endian_long(0x3760054fu), change_endian_long(0x6ab8bc6cu));
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
                    RX671_func100(change_endian_long(0x8257f84fu), change_endian_long(0x14bfff8cu), change_endian_long(0x485d5de2u), change_endian_long(0x6866fc1cu));
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
                    RX671_func100(change_endian_long(0x5cf2fa34u), change_endian_long(0x6b64fea0u), change_endian_long(0xb4e439a7u), change_endian_long(0x89f8a75fu));
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
                    RX671_func100(change_endian_long(0x97750258u), change_endian_long(0xeb5a3092u), change_endian_long(0x965aad61u), change_endian_long(0xaefa8d39u));
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
                    RX671_func102(change_endian_long(0xdaf8eef8u), change_endian_long(0x2b380c2eu), change_endian_long(0x8c2b64b5u), change_endian_long(0xc978195au));
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
                    RX671_func100(change_endian_long(0x9c57b539u), change_endian_long(0x51259962u), change_endian_long(0x50ac35b0u), change_endian_long(0xf890db0au));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX671_func102(change_endian_long(0xa8b9cbf0u), change_endian_long(0x0d6cba0fu), change_endian_long(0xdba53186u), change_endian_long(0x8ae98fddu));
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
                        RX671_func100(change_endian_long(0x32b0c837u), change_endian_long(0x7c7ff113u), change_endian_long(0xf8cd2489u), change_endian_long(0xfc70ac93u));
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
                        RX671_func100(change_endian_long(0x41318577u), change_endian_long(0x14fa2146u), change_endian_long(0x694a7e54u), change_endian_long(0xb8dd997du));
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
                        RX671_func100(change_endian_long(0x7155833bu), change_endian_long(0x85081719u), change_endian_long(0xb4ccca8fu), change_endian_long(0xa68fa8f6u));
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
                        RX671_func100(change_endian_long(0xbeb4c1dau), change_endian_long(0x4cf5b840u), change_endian_long(0x0b6d3145u), change_endian_long(0xf77db1e6u));
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
                        RX671_func102(change_endian_long(0x0fffc961u), change_endian_long(0x945b0bd0u), change_endian_long(0xce3eba8eu), change_endian_long(0xca1e627fu));
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
 End of function ./input_dir/RX671/RX671_pe0.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_TLS == 1 */
