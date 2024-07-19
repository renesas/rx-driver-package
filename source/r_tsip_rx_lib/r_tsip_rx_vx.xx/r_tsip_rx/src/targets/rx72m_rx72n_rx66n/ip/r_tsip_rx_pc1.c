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
* @details       RX72M TLS Certificate Verification for Server
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0x83ab9e45u), change_endian_long(0x0a2d5757u), change_endian_long(0x3c51cc3cu), change_endian_long(0x689a4720u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0x091b1b93u), change_endian_long(0x2fa8437bu), change_endian_long(0x858573fcu), change_endian_long(0x55066747u));
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
        RX72M_RX72N_RX66N_func100(change_endian_long(0x73d98ddfu), change_endian_long(0xfa0f3800u), change_endian_long(0x20c27a1au), change_endian_long(0xf7d55d27u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func100(change_endian_long(0x3e4ad0e5u), change_endian_long(0x9b55014au), change_endian_long(0x5b26ce6bu), change_endian_long(0xde0f51dbu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x8d72a039u), change_endian_long(0xda91d910u), change_endian_long(0x4ef9a5f0u), change_endian_long(0xba9a510bu));
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
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x5e69b3d3u), change_endian_long(0x341e92e2u), change_endian_long(0x2db6b20au), change_endian_long(0x463ea189u));
                }
                else
                {
                    TSIP.REG_ECH.WORD = 0x00003417u;
                    TSIP.REG_ECH.WORD = 0x0000a400u;
                    TSIP.REG_ECH.WORD = 0x000001ffu;
                    TSIP.REG_ECH.WORD = 0x08002818u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00A60000u;
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x8d238854u), change_endian_long(0xac71a291u), change_endian_long(0xa8404068u), change_endian_long(0x3847adbdu));
                }
                TSIP.REG_ECH.WORD = 0x00003419u;
                TSIP.REG_ECH.WORD = 0x0000a400u;
                TSIP.REG_ECH.WORD = 0x00000003u;
                TSIP.REG_ECH.WORD = 0x0c00281au;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00A60000u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0x03d770bdu), change_endian_long(0xc1582b3eu), change_endian_long(0x75057f55u), change_endian_long(0x7656545fu));
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0xdccb9dbcu), change_endian_long(0x2a6cbb24u), change_endian_long(0xbd38f09du), change_endian_long(0x30590ebfu));
            }
            RX72M_RX72N_RX66N_func101(change_endian_long(0xf776f44du), change_endian_long(0xfcf12a40u), change_endian_long(0x1f0d274eu), change_endian_long(0xfeaa5cebu));
        }
        else
        {
            RX72M_RX72N_RX66N_func101(change_endian_long(0x3fba7afau), change_endian_long(0xe4d9e5bdu), change_endian_long(0x766d49b2u), change_endian_long(0x69b81504u));
        }
        RX72M_RX72N_RX66N_func100(change_endian_long(0x86470c50u), change_endian_long(0x287ff9c0u), change_endian_long(0xed131c16u), change_endian_long(0x8e47e665u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func102(change_endian_long(0xc17767a4u), change_endian_long(0xff128e79u), change_endian_long(0x2fbc81cfu), change_endian_long(0xc4aa7ea4u));
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
            RX72M_RX72N_RX66N_func100(change_endian_long(0x66e56915u), change_endian_long(0x7ee713e6u), change_endian_long(0xc6fd9683u), change_endian_long(0x6309e36fu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX72M_RX72N_RX66N_func102(change_endian_long(0x2bd1308bu), change_endian_long(0xd62940c1u), change_endian_long(0x51925cfdu), change_endian_long(0x7cfe9561u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x0b2331bau), change_endian_long(0xaa41a261u), change_endian_long(0x74cbe226u), change_endian_long(0xb41a6222u));
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
                    RX72M_RX72N_RX66N_func101(change_endian_long(0xf5a0e555u), change_endian_long(0x5ca68c11u), change_endian_long(0x6f323d4au), change_endian_long(0xff375063u));
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
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x45f9ea88u), change_endian_long(0x7a34b0b6u), change_endian_long(0x6774e9f2u), change_endian_long(0x889abee8u));
                }
                RX72M_RX72N_RX66N_func100(change_endian_long(0xfa7afaf4u), change_endian_long(0x869d5527u), change_endian_long(0x42afc356u), change_endian_long(0x93e0c91eu));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    RX72M_RX72N_RX66N_func102(change_endian_long(0x77c3b03eu), change_endian_long(0x6467c911u), change_endian_long(0x09d612f3u), change_endian_long(0xd02a0788u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xce7ae854u), change_endian_long(0xc7d60928u), change_endian_long(0x930c29d0u), change_endian_long(0x08fbb702u));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x0980551fu), change_endian_long(0x8da66efdu), change_endian_long(0x7a46c80eu), change_endian_long(0x6f254801u));
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
                            RX72M_RX72N_RX66N_func101(change_endian_long(0xcaae885eu), change_endian_long(0xc3f143fau), change_endian_long(0xfb1db10bu), change_endian_long(0x85c76862u));
                        }
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x867d20eeu), change_endian_long(0x528c0d72u), change_endian_long(0xcbe87952u), change_endian_long(0x2ece55ddu));
                        TSIP.REG_ECH.WORD = 0x38000805u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        TSIP.REG_1CH.WORD = 0x00402000u;
                        TSIP.REG_ECH.WORD = 0x00007c0du;
                        TSIP.REG_1CH.WORD = 0x00600000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                        {
                            RX72M_RX72N_RX66N_func100(change_endian_long(0xae8306e2u), change_endian_long(0x16712498u), change_endian_long(0x4eaaa117u), change_endian_long(0x104bb830u));
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
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0xddf4e41fu), change_endian_long(0x5dd5610du), change_endian_long(0x792eec84u), change_endian_long(0xfb816436u));
                                    TSIP.REG_1CH.WORD = 0x00400000u;
                                    TSIP.REG_1D0H.WORD = 0x00000000u;
                                    if (1u == (TSIP.REG_1CH.BIT.B22))
                                    {
                                        TSIP.REG_ECH.WORD = 0x3420a980u;
                                        TSIP.REG_ECH.WORD = 0x00000100u;
                                        TSIP.REG_E0H.WORD = 0x00000080u;
                                        TSIP.REG_1CH.WORD = 0x00260000u;
                                        RX72M_RX72N_RX66N_func100(change_endian_long(0x97826b83u), change_endian_long(0x0fea3779u), change_endian_long(0x5af82a5bu), change_endian_long(0xd0bc5285u));
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
                                            RX72M_RX72N_RX66N_func101(change_endian_long(0x24092f5fu), change_endian_long(0x41378a5eu), change_endian_long(0x59b2ae16u), change_endian_long(0xa7d54d87u));
                                        }
                                        else
                                        {
                                            TSIP.REG_ECH.WORD = 0x02003cebu;
                                            TSIP.REG_ECH.WORD = 0x0000a560u;
                                            TSIP.REG_ECH.WORD = 0x00000001u;
                                            RX72M_RX72N_RX66N_func101(change_endian_long(0xad377bc7u), change_endian_long(0xdba11fd7u), change_endian_long(0xa7fd83feu), change_endian_long(0x162b5ce6u));
                                        }
                                        RX72M_RX72N_RX66N_func101(change_endian_long(0x56807ac8u), change_endian_long(0xa37432c6u), change_endian_long(0x01b19577u), change_endian_long(0x43b21342u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c0028c2u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0x8483c0d1u), change_endian_long(0xe1e03e86u), change_endian_long(0x151b07a7u), change_endian_long(0xa113ceb3u));
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
                                        RX72M_RX72N_RX66N_func101(change_endian_long(0x8c7d76f4u), change_endian_long(0x913eebf7u), change_endian_long(0xb74b7a80u), change_endian_long(0xb84566cdu));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00002c40u;
                                    RX72M_RX72N_RX66N_func101(change_endian_long(0x1cf7ee3fu), change_endian_long(0x7a43059bu), change_endian_long(0xb092fc7cu), change_endian_long(0xeb618cc3u));
                                }
                                TSIP.REG_ECH.WORD = 0x08000c21u;
                                TSIP.REG_E0H.WORD = 0x00000080u;
                                TSIP.REG_1CH.WORD = 0x00260000u;
                                TSIP.REG_1CH.WORD = 0x00402000u;
                                TSIP.REG_ECH.WORD = 0x00002ca0u;
                                RX72M_RX72N_RX66N_func101(change_endian_long(0xd1bb8845u), change_endian_long(0xe233454bu), change_endian_long(0x11b8dc27u), change_endian_long(0xe4c188e3u));
                            }
                            TSIP.REG_ECH.WORD = 0x38000805u;
                            TSIP.REG_E0H.WORD = 0x00000080u;
                            TSIP.REG_1CH.WORD = 0x00260000u;
                            TSIP.REG_1CH.WORD = 0x00402000u;
                            RX72M_RX72N_RX66N_func101(change_endian_long(0x76f7f4c0u), change_endian_long(0x6e46fc4au), change_endian_long(0x617f3d0fu), change_endian_long(0x53428263u));
                        }
                        else
                        {
                            RX72M_RX72N_RX66N_func100(change_endian_long(0x7e15bbfdu), change_endian_long(0x5c1ed14du), change_endian_long(0xce756335u), change_endian_long(0x8b3292a5u));
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
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0x34aa1b57u), change_endian_long(0xc7624b99u), change_endian_long(0xd9d88510u), change_endian_long(0xe7c2f029u));
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
                                        RX72M_RX72N_RX66N_func101(change_endian_long(0x193df58cu), change_endian_long(0xb22ec48cu), change_endian_long(0x0020fafcu), change_endian_long(0x9fb619beu));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c002857u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x3c002b02u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0xca0453b4u), change_endian_long(0xe378bbbdu), change_endian_long(0xef3ccfb7u), change_endian_long(0x35bad62cu));
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
                                        RX72M_RX72N_RX66N_func101(change_endian_long(0xda7d0857u), change_endian_long(0xe92c8e3fu), change_endian_long(0x4bd0dc59u), change_endian_long(0x60db8dfeu));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c0028c2u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0xcb0d485du), change_endian_long(0x76f8a216u), change_endian_long(0xf9b8ef0cu), change_endian_long(0x59ea6b47u));
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
                                        RX72M_RX72N_RX66N_func101(change_endian_long(0x454fb157u), change_endian_long(0x54214cfdu), change_endian_long(0x797cac51u), change_endian_long(0x583cc6f4u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00002c40u;
                                    RX72M_RX72N_RX66N_func101(change_endian_long(0xd7e75ec9u), change_endian_long(0x28b9cccau), change_endian_long(0x674dd4d4u), change_endian_long(0x4195f4e8u));
                                }
                                TSIP.REG_ECH.WORD = 0x08000c21u;
                                TSIP.REG_E0H.WORD = 0x00000080u;
                                TSIP.REG_1CH.WORD = 0x00260000u;
                                TSIP.REG_1CH.WORD = 0x00402000u;
                                TSIP.REG_ECH.WORD = 0x00002ca0u;
                                RX72M_RX72N_RX66N_func101(change_endian_long(0x2379faf8u), change_endian_long(0x1558ddf3u), change_endian_long(0xb658c0ebu), change_endian_long(0x7429e5f3u));
                            }
                            TSIP.REG_ECH.WORD = 0x38000805u;
                            TSIP.REG_E0H.WORD = 0x00000080u;
                            TSIP.REG_1CH.WORD = 0x00260000u;
                            TSIP.REG_1CH.WORD = 0x00402000u;
                            RX72M_RX72N_RX66N_func101(change_endian_long(0x49326bb2u), change_endian_long(0x73293775u), change_endian_long(0x121c9abcu), change_endian_long(0xb066ecafu));
                        }
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x06572705u), change_endian_long(0x4dd7dbcdu), change_endian_long(0x29131e3bu), change_endian_long(0x978a8996u));
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
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x8827be2fu), change_endian_long(0x3e5ce7e6u), change_endian_long(0xcd20aa71u), change_endian_long(0x4f21de40u));
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
                            RX72M_RX72N_RX66N_func101(change_endian_long(0x7cf18f05u), change_endian_long(0xd06e2c70u), change_endian_long(0x426d6669u), change_endian_long(0x6126552au));
                        }
                        TSIP.REG_ECH.WORD = 0x38000805u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        TSIP.REG_1CH.WORD = 0x00402000u;
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xd5adedd0u), change_endian_long(0xd1c95ffdu), change_endian_long(0x2d37d455u), change_endian_long(0x8ca0b602u));
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
                            RX72M_RX72N_RX66N_func101(change_endian_long(0x94a5c7a9u), change_endian_long(0x15e45b55u), change_endian_long(0x979d3a2cu), change_endian_long(0x94533f36u));
                        }
                        TSIP.REG_ECH.WORD = 0x38000805u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        TSIP.REG_1CH.WORD = 0x00402000u;
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x61eae9fdu), change_endian_long(0xd4082296u), change_endian_long(0x79e3fea3u), change_endian_long(0x43abd92cu));
                    }
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x85875fd9u), change_endian_long(0xad902ed6u), change_endian_long(0x4d9bcc81u), change_endian_long(0x4f668955u));
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
                        RX72M_RX72N_RX66N_func101(change_endian_long(0xc795310du), change_endian_long(0x5574c547u), change_endian_long(0x45013a6du), change_endian_long(0x72c95801u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xd4b3e7f5u), change_endian_long(0x885f9d55u), change_endian_long(0xb7329cffu), change_endian_long(0xd4b0e6eau));
                    RX72M_RX72N_RX66N_func103();
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xd84f55b1u), change_endian_long(0xf6c6af1eu), change_endian_long(0xf85efb8bu), change_endian_long(0xf10d34adu));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x3c46bdf3u), change_endian_long(0xbc3c3297u), change_endian_long(0xc1381001u), change_endian_long(0xe52717bau));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x3fbd9cf7u), change_endian_long(0x8764d51eu), change_endian_long(0x591017b6u), change_endian_long(0xb81eeb70u));
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
                            RX72M_RX72N_RX66N_func100(change_endian_long(0xd54cc27cu), change_endian_long(0xbabcd224u), change_endian_long(0xef439cbbu), change_endian_long(0xf72dc252u));
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
                                RX72M_RX72N_RX66N_func101(change_endian_long(0x10db003bu), change_endian_long(0xba0607c9u), change_endian_long(0x73d3471eu), change_endian_long(0x992100c8u));
                            }
                            else
                            {
                                RX72M_RX72N_RX66N_func100(change_endian_long(0xa6d0d4dbu), change_endian_long(0x549b001au), change_endian_long(0x210b714au), change_endian_long(0xcd9be3ecu));
                                TSIP.REG_104H.WORD = 0x00000058u;
                                TSIP.REG_E0H.WORD = 0x80010020u;
                                /* WAIT_LOOP */
                                while (1u != TSIP.REG_104H.BIT.B31)
                                {
                                    /* waiting */
                                }
                                TSIP.REG_100H.WORD = change_endian_long(0x4ce2e786u);
                                RX72M_RX72N_RX66N_func080();
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x875eaf4cu), change_endian_long(0xaa044c00u), change_endian_long(0xd93fbb38u), change_endian_long(0x54a3b799u));
                                TSIP.REG_00H.WORD = 0x00012303u;
                                TSIP.REG_2CH.WORD = 0x00000020u;
                                RX72M_RX72N_RX66N_func313(1496);
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x37a7e4f6u), change_endian_long(0x655f21c1u), change_endian_long(0x9a6fb49eu), change_endian_long(0x7c92477cu));
                                RX72M_RX72N_RX66N_func314(1496+64);
                                RX72M_RX72N_RX66N_func100(change_endian_long(0xb0ea7ba5u), change_endian_long(0x14e2aa8fu), change_endian_long(0x8940e9a6u), change_endian_long(0xe618d3b7u));
                                TSIP.REG_104H.WORD = 0x00000058u;
                                TSIP.REG_E0H.WORD = 0x80010020u;
                                /* WAIT_LOOP */
                                while (1u != TSIP.REG_104H.BIT.B31)
                                {
                                    /* waiting */
                                }
                                TSIP.REG_100H.WORD = change_endian_long(0xa37060edu);
                                RX72M_RX72N_RX66N_func080();
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x076a8caau), change_endian_long(0x300c5a99u), change_endian_long(0x836778e4u), change_endian_long(0x6961c60au));
                                TSIP.REG_ECH.WORD = 0x00000800u;
                                TSIP.REG_E0H.WORD = 0x81c00000u;
                                TSIP.REG_00H.WORD = 0x00012803u;
                                RX72M_RX72N_RX66N_func313(1496+68);
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x7e3bd2a1u), change_endian_long(0xaaed63beu), change_endian_long(0xe25565e3u), change_endian_long(0xfd4f89a2u));
                                RX72M_RX72N_RX66N_func314(1496+132);
                                RX72M_RX72N_RX66N_func101(change_endian_long(0x7c19068fu), change_endian_long(0xd1e38fb8u), change_endian_long(0x38e23b9bu), change_endian_long(0xe4ebcd26u));
                            }
                            RX72M_RX72N_RX66N_func101(change_endian_long(0x37bd964du), change_endian_long(0xc492693du), change_endian_long(0x317eed96u), change_endian_long(0x5cb205bau));
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
                            RX72M_RX72N_RX66N_func101(change_endian_long(0x351b0583u), change_endian_long(0x436d91f5u), change_endian_long(0xac3d872eu), change_endian_long(0x9cf4cd6fu));
                        }
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x56759714u), change_endian_long(0xf20f34b7u), change_endian_long(0x7f053dc2u), change_endian_long(0x196dd1b2u));
                    }
                    else
                    {
                        RX72M_RX72N_RX66N_func101(change_endian_long(0xdb987c6fu), change_endian_long(0x676626efu), change_endian_long(0xb7b2aac0u), change_endian_long(0x8a94f4b0u));
                    }
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x5ede2db6u), change_endian_long(0x7e29d477u), change_endian_long(0x1c59bcbdu), change_endian_long(0xba0f8e2cu));
                    TSIP.REG_ECH.WORD = 0x00007c0eu;
                    TSIP.REG_1CH.WORD = 0x00600000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                    {
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x34f67944u), change_endian_long(0xab06c9bfu), change_endian_long(0xbe8fa92du), change_endian_long(0xcaacac27u));
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
                        RX72M_RX72N_RX66N_func101(change_endian_long(0xcf8be625u), change_endian_long(0xf4faf078u), change_endian_long(0xf782f002u), change_endian_long(0x8df09b9cu));
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
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xf77ae777u), change_endian_long(0x253cc93eu), change_endian_long(0x3e58719fu), change_endian_long(0xa8b8008au));
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x5fe685f6u);
                        RX72M_RX72N_RX66N_func080();
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xfa1c8f3eu), change_endian_long(0xd6d94c12u), change_endian_long(0x9f135369u), change_endian_long(0xe4398e50u));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000020u;
                        RX72M_RX72N_RX66N_func313(748);
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xf862268bu), change_endian_long(0xe15e3079u), change_endian_long(0xbb295804u), change_endian_long(0x3dc0452fu));
                        RX72M_RX72N_RX66N_func314(748+64);
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xa532faa1u), change_endian_long(0xbf463934u), change_endian_long(0x57795c50u), change_endian_long(0x2b0bc9ffu));
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x1c5a2b81u);
                        RX72M_RX72N_RX66N_func080();
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xa1590293u), change_endian_long(0x5ee8f228u), change_endian_long(0x94765614u), change_endian_long(0x70b8a3a2u));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000022u;
                        RX72M_RX72N_RX66N_func313(680);
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xb6d2bc50u), change_endian_long(0x6043f54du), change_endian_long(0xdc4f9758u), change_endian_long(0x053bd891u));
                        RX72M_RX72N_RX66N_func314(680+64);
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xd0511cb5u), change_endian_long(0xa9b60a31u), change_endian_long(0x2f855bf1u), change_endian_long(0xb8c1d551u));
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
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x9b18a9d4u), change_endian_long(0xc2b42445u), change_endian_long(0x738c4d69u), change_endian_long(0x89803e0bu));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000020u;
                        RX72M_RX72N_RX66N_func313(68);
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xb5f75de3u), change_endian_long(0x02e92129u), change_endian_long(0x3a48d921u), change_endian_long(0x5d2960cfu));
                        RX72M_RX72N_RX66N_func314(68+64);
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xcfdd196au), change_endian_long(0x2670cdbeu), change_endian_long(0x7e079eebu), change_endian_long(0xcf647dc5u));
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
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xf6d3d263u), change_endian_long(0x4c07216eu), change_endian_long(0x9151b315u), change_endian_long(0xc032575du));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000024u;
                        RX72M_RX72N_RX66N_func313(0);
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x6c640e4du), change_endian_long(0xee928d40u), change_endian_long(0x06721c3bu), change_endian_long(0x7bf9c62fu));
                        RX72M_RX72N_RX66N_func314(0+64);
                        TSIP.REG_ECH.WORD = 0x0000b400u;
                        TSIP.REG_ECH.WORD = 0x000000c1u;
                        RX72M_RX72N_RX66N_func101(change_endian_long(0xa409d785u), change_endian_long(0x6e35cdd0u), change_endian_long(0xaf5fcc74u), change_endian_long(0x44149ff3u));
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
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x769256a4u), change_endian_long(0x0f12bf4bu), change_endian_long(0xe826d1ffu), change_endian_long(0x75bdcec2u));
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
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x9cfecee7u), change_endian_long(0x7fcf918fu), change_endian_long(0xbaac70d8u), change_endian_long(0x47d56a7du));
                        RX72M_RX72N_RX66N_func003(InData_Signature);
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x14ea0380u), change_endian_long(0xb6c72dd4u), change_endian_long(0x1ddeb8b3u), change_endian_long(0xd3a3c279u));
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
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x4a6e43f6u), change_endian_long(0xdd402d79u), change_endian_long(0x357227e6u), change_endian_long(0x7fb84d38u));
                        RX72M_RX72N_RX66N_func015();
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x351a6078u), change_endian_long(0x17457701u), change_endian_long(0x7287556bu), change_endian_long(0x7427e851u));
                    }
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x3eeb0047u), change_endian_long(0x75f19187u), change_endian_long(0xdb804046u), change_endian_long(0x1141ae2eu));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX72M_RX72N_RX66N_func102(change_endian_long(0x11c5d9a7u), change_endian_long(0x15030b6du), change_endian_long(0x134abc3du), change_endian_long(0xf420cc67u));
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
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xac7a3d5bu), change_endian_long(0x09a38ad2u), change_endian_long(0x4ad85ed2u), change_endian_long(0x287fb3e1u));
                        TSIP.REG_1CH.WORD = 0x00400000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (1u == (TSIP.REG_1CH.BIT.B22))
                        {
                            RX72M_RX72N_RX66N_func102(change_endian_long(0x873ecd21u), change_endian_long(0x3f5180b7u), change_endian_long(0x2ed9a9f9u), change_endian_long(0xcb0f473eu));
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
                            RX72M_RX72N_RX66N_func100(change_endian_long(0x8506a9ddu), change_endian_long(0xbdfb0edeu), change_endian_long(0x4d5aebfcu), change_endian_long(0x77bd0f51u));
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
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x66b63a2du), change_endian_long(0x80ab5a4fu), change_endian_long(0xafa25507u), change_endian_long(0x405f7b46u));
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
                                    RX72M_RX72N_RX66N_func101(change_endian_long(0x96d17be1u), change_endian_long(0x712c44f4u), change_endian_long(0x861e1bb4u), change_endian_long(0xafd0e13cu));
                                }
                                else
                                {
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0x2b2aa546u), change_endian_long(0x9fbcfddfu), change_endian_long(0x333e4215u), change_endian_long(0xa3566c2au));
                                    TSIP.REG_C4H.WORD = 0x000c0805u;
                                    TSIP.REG_E0H.WORD = 0x810402e0u;
                                    TSIP.REG_00H.WORD = 0x00002813u;
                                    /* WAIT_LOOP */
                                    while (0u != TSIP.REG_00H.BIT.B25)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_1CH.WORD = 0x00001800u;
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0x56d79d28u), change_endian_long(0x71c521c9u), change_endian_long(0x3d5ee97eu), change_endian_long(0xf037f1a7u));
                                    TSIP.REG_104H.WORD = 0x00000058u;
                                    TSIP.REG_E0H.WORD = 0x80010020u;
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_104H.BIT.B31)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_100H.WORD = change_endian_long(0x4ce2e786u);
                                    RX72M_RX72N_RX66N_func080();
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0xd2731799u), change_endian_long(0x339674b6u), change_endian_long(0x935d4a5eu), change_endian_long(0x158c7667u));
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
                                    TSIP.REG_100H.WORD = change_endian_long(0xa37060edu);
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
                                    TSIP.REG_100H.WORD = change_endian_long(0x8a880100u);
                                    RX72M_RX72N_RX66N_func101(change_endian_long(0x49cd7fe8u), change_endian_long(0xe7f6b6bbu), change_endian_long(0x903134e9u), change_endian_long(0xb7d630beu));
                                }
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x4aa0c127u), change_endian_long(0x78ffbb9bu), change_endian_long(0x77264787u), change_endian_long(0x494a089bu));
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
                                RX72M_RX72N_RX66N_func100(change_endian_long(0xe466aa5du), change_endian_long(0x2bd63395u), change_endian_long(0xb8eea745u), change_endian_long(0x0bd6eebfu));
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
                                RX72M_RX72N_RX66N_func100(change_endian_long(0xbadd517du), change_endian_long(0x51d201ebu), change_endian_long(0xbb438ba4u), change_endian_long(0xd7c0d7a4u));
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
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x18435731u), change_endian_long(0x560cdd30u), change_endian_long(0x4fff6ef9u), change_endian_long(0x0d11469au));
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
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x5fa5b1aau), change_endian_long(0xa0675c99u), change_endian_long(0xed99b02eu), change_endian_long(0x6ae91e13u));
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
                                RX72M_RX72N_RX66N_func100(change_endian_long(0x4cd93253u), change_endian_long(0x259ffeb8u), change_endian_long(0x1b3f06feu), change_endian_long(0x4939b37cu));
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
                                RX72M_RX72N_RX66N_func102(change_endian_long(0x4c89d013u), change_endian_long(0xbff70648u), change_endian_long(0xade3187du), change_endian_long(0xa77cbde4u));
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
                                RX72M_RX72N_RX66N_func100(change_endian_long(0xa353b536u), change_endian_long(0x49717c45u), change_endian_long(0x74a9e7f3u), change_endian_long(0x21480529u));
                                TSIP.REG_1CH.WORD = 0x00400000u;
                                TSIP.REG_1D0H.WORD = 0x00000000u;
                                if (1u == (TSIP.REG_1CH.BIT.B22))
                                {
                                    RX72M_RX72N_RX66N_func102(change_endian_long(0xa73c5ec6u), change_endian_long(0xd3175c6au), change_endian_long(0xcd0b1d79u), change_endian_long(0x1eddc2dbu));
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
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0x441a005au), change_endian_long(0x6f7d3dedu), change_endian_long(0xf1e62d7fu), change_endian_long(0xd0548547u));
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
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0xf557ecfau), change_endian_long(0xfd6f489fu), change_endian_long(0x85fb95f7u), change_endian_long(0x6330edccu));
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
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0x57e1d91eu), change_endian_long(0x4d7230dfu), change_endian_long(0x6e9a173cu), change_endian_long(0xccf581b8u));
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
                                    RX72M_RX72N_RX66N_func100(change_endian_long(0xb7ce7c0fu), change_endian_long(0xb0c0fd7cu), change_endian_long(0x7dba79afu), change_endian_long(0xb90183a2u));
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
                                    RX72M_RX72N_RX66N_func102(change_endian_long(0x29e4346du), change_endian_long(0xf4417e16u), change_endian_long(0xc35593b2u), change_endian_long(0x6dd64829u));
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
 End of function ./input_dir/RX72M/RX72M_pc1.prc
 *********************************************************************************************************************/
