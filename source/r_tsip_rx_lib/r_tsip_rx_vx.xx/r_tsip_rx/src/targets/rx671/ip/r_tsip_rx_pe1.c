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
* @details       RX671 TLS Certificate Verification
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
    RX671_func100(change_endian_long(0xcb3334f0u), change_endian_long(0xea668c7bu), change_endian_long(0x87dca37cu), change_endian_long(0x66664fb4u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func102(change_endian_long(0x832b4529u), change_endian_long(0xbde10fb1u), change_endian_long(0xe5cefe24u), change_endian_long(0x8eebfefeu));
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
        RX671_func100(change_endian_long(0x6fe88ab4u), change_endian_long(0x27ebc324u), change_endian_long(0x6f3fb792u), change_endian_long(0x90f6d70du));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func100(change_endian_long(0x469fb086u), change_endian_long(0x7ac9015du), change_endian_long(0xef7a9421u), change_endian_long(0xa4169568u));
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
                RX671_func100(change_endian_long(0x44ceb8c5u), change_endian_long(0x3a4a1cc4u), change_endian_long(0xacae22eeu), change_endian_long(0x69b5b94du));
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
                    RX671_func101(change_endian_long(0x7a42c3d3u), change_endian_long(0x43ce7c33u), change_endian_long(0xe7642e5au), change_endian_long(0xf53ecc37u));
                }
                else
                {
                    TSIP.REG_ECH.WORD = 0x00003417u;
                    TSIP.REG_ECH.WORD = 0x0000a400u;
                    TSIP.REG_ECH.WORD = 0x000001ffu;
                    TSIP.REG_ECH.WORD = 0x08002818u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00A60000u;
                    RX671_func101(change_endian_long(0x461dd2f0u), change_endian_long(0xec684490u), change_endian_long(0xf55f8c73u), change_endian_long(0x36199fa9u));
                }
                TSIP.REG_ECH.WORD = 0x00003419u;
                TSIP.REG_ECH.WORD = 0x0000a400u;
                TSIP.REG_ECH.WORD = 0x00000003u;
                TSIP.REG_ECH.WORD = 0x0c00281au;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00A60000u;
                RX671_func101(change_endian_long(0x81946a2bu), change_endian_long(0xa51dd493u), change_endian_long(0xc6b3a570u), change_endian_long(0x6e7f87a4u));
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
                RX671_func101(change_endian_long(0xc9bdd5b4u), change_endian_long(0x6fbd8ff8u), change_endian_long(0xd7d035f7u), change_endian_long(0x70babc20u));
            }
            RX671_func101(change_endian_long(0x4a75c429u), change_endian_long(0x176f7271u), change_endian_long(0x18b041a9u), change_endian_long(0x019bda39u));
        }
        else
        {
            RX671_func101(change_endian_long(0x0d5fab3fu), change_endian_long(0x938f69ceu), change_endian_long(0xbbcf4be5u), change_endian_long(0x20a4dd16u));
        }
        RX671_func100(change_endian_long(0x13b600d5u), change_endian_long(0xc84a10ffu), change_endian_long(0xe60bb492u), change_endian_long(0x3319278eu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func102(change_endian_long(0xe62cfd53u), change_endian_long(0xab8282dau), change_endian_long(0xa7aa4e7eu), change_endian_long(0x5bc5f8dbu));
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
            RX671_func100(change_endian_long(0x48b5bf35u), change_endian_long(0x42aa7518u), change_endian_long(0xf0d6dd26u), change_endian_long(0x59102114u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX671_func102(change_endian_long(0xeae6064du), change_endian_long(0x5730c00fu), change_endian_long(0xbd3ef8d1u), change_endian_long(0x75d3c673u));
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
                RX671_func100(change_endian_long(0xc87a75f3u), change_endian_long(0x87595baeu), change_endian_long(0x28aa5d06u), change_endian_long(0x4f30fd36u));
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
                    RX671_func101(change_endian_long(0xf9f7abbcu), change_endian_long(0x26a396a6u), change_endian_long(0xbb3c079bu), change_endian_long(0x73c2265cu));
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
                    RX671_func101(change_endian_long(0x907f78bfu), change_endian_long(0x226ce84eu), change_endian_long(0xe411fbbbu), change_endian_long(0x27c7177du));
                }
                RX671_func100(change_endian_long(0x31ea80f6u), change_endian_long(0xab58cb3du), change_endian_long(0x9617bb4du), change_endian_long(0xdb6ef38au));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    RX671_func102(change_endian_long(0x1c6120f0u), change_endian_long(0x632a57abu), change_endian_long(0xa0a1c0a7u), change_endian_long(0xb0c73146u));
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
                    RX671_func100(change_endian_long(0xce06d6dcu), change_endian_long(0x0abfea01u), change_endian_long(0xe0a65bd3u), change_endian_long(0x9939ada3u));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX671_func100(change_endian_long(0x7981a069u), change_endian_long(0xa1addeeeu), change_endian_long(0x8e81150du), change_endian_long(0xda939224u));
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
                            RX671_func101(change_endian_long(0x4c9770c4u), change_endian_long(0x10cb88abu), change_endian_long(0x46218b8du), change_endian_long(0x8e90ef62u));
                        }
                        RX671_func100(change_endian_long(0xd69edd8du), change_endian_long(0xd77ceca6u), change_endian_long(0x8140d4fbu), change_endian_long(0xd1ba09a6u));
                        TSIP.REG_ECH.WORD = 0x38000805u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        TSIP.REG_1CH.WORD = 0x00402000u;
                        TSIP.REG_ECH.WORD = 0x00007c0du;
                        TSIP.REG_1CH.WORD = 0x00600000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                        {
                            RX671_func100(change_endian_long(0x91fa7544u), change_endian_long(0x20d3961eu), change_endian_long(0x2dde3df5u), change_endian_long(0x1791bd0au));
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
                                    RX671_func100(change_endian_long(0xbcc95f18u), change_endian_long(0x2f5f8287u), change_endian_long(0xcc340504u), change_endian_long(0x4cfc3a3eu));
                                    TSIP.REG_1CH.WORD = 0x00400000u;
                                    TSIP.REG_1D0H.WORD = 0x00000000u;
                                    if (1u == (TSIP.REG_1CH.BIT.B22))
                                    {
                                        TSIP.REG_ECH.WORD = 0x3420a980u;
                                        TSIP.REG_ECH.WORD = 0x00000100u;
                                        TSIP.REG_E0H.WORD = 0x00000080u;
                                        TSIP.REG_1CH.WORD = 0x00260000u;
                                        RX671_func100(change_endian_long(0x72140b61u), change_endian_long(0xc260a684u), change_endian_long(0xae19b86fu), change_endian_long(0x22cc6496u));
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
                                            RX671_func101(change_endian_long(0xd7c4098bu), change_endian_long(0x046461c9u), change_endian_long(0x4c652be0u), change_endian_long(0x6e0a2de3u));
                                        }
                                        else
                                        {
                                            TSIP.REG_ECH.WORD = 0x02003cebu;
                                            TSIP.REG_ECH.WORD = 0x0000a560u;
                                            TSIP.REG_ECH.WORD = 0x00000001u;
                                            RX671_func101(change_endian_long(0xc6ce8343u), change_endian_long(0x15653f63u), change_endian_long(0x8e7f54e2u), change_endian_long(0x88b38a0fu));
                                        }
                                        RX671_func101(change_endian_long(0xa7c0349fu), change_endian_long(0x45bfef8cu), change_endian_long(0xdb577055u), change_endian_long(0x505f4da7u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c0028c2u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX671_func100(change_endian_long(0x9e0f99e2u), change_endian_long(0x5e45c415u), change_endian_long(0xa037d3efu), change_endian_long(0x7fb78ac9u));
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
                                        RX671_func101(change_endian_long(0x797287aau), change_endian_long(0xaca5548bu), change_endian_long(0xd1e13e7bu), change_endian_long(0xde5ce5cfu));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00002c40u;
                                    RX671_func101(change_endian_long(0xfc433c6cu), change_endian_long(0x4c3e26c7u), change_endian_long(0x0d0b618du), change_endian_long(0x77b6cd7du));
                                }
                                TSIP.REG_ECH.WORD = 0x08000c21u;
                                TSIP.REG_E0H.WORD = 0x00000080u;
                                TSIP.REG_1CH.WORD = 0x00260000u;
                                TSIP.REG_1CH.WORD = 0x00402000u;
                                TSIP.REG_ECH.WORD = 0x00002ca0u;
                                RX671_func101(change_endian_long(0xc0411525u), change_endian_long(0x0ac44d55u), change_endian_long(0xa66ed094u), change_endian_long(0xaea93331u));
                            }
                            TSIP.REG_ECH.WORD = 0x38000805u;
                            TSIP.REG_E0H.WORD = 0x00000080u;
                            TSIP.REG_1CH.WORD = 0x00260000u;
                            TSIP.REG_1CH.WORD = 0x00402000u;
                            RX671_func101(change_endian_long(0x5f839a3cu), change_endian_long(0x303672a7u), change_endian_long(0x56742059u), change_endian_long(0xddae0058u));
                        }
                        else
                        {
                            RX671_func100(change_endian_long(0xf2f1ed7du), change_endian_long(0x1a0ac0c4u), change_endian_long(0x122bbd6du), change_endian_long(0x4ec4cf88u));
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
                                    RX671_func100(change_endian_long(0x32572ad5u), change_endian_long(0x1132ce08u), change_endian_long(0xa7aca60au), change_endian_long(0xd051dfadu));
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
                                        RX671_func101(change_endian_long(0xc2de11bfu), change_endian_long(0xe37c0095u), change_endian_long(0xcfd91c3eu), change_endian_long(0xa7d33623u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c002857u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x3c002b02u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX671_func100(change_endian_long(0x55f65952u), change_endian_long(0x14c7ff16u), change_endian_long(0xfdc738bcu), change_endian_long(0xc3bf2a0au));
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
                                        RX671_func101(change_endian_long(0x82b68f3cu), change_endian_long(0x99d50027u), change_endian_long(0xa21f5a82u), change_endian_long(0xf9b4e4efu));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00000863u;
                                    TSIP.REG_ECH.WORD = 0x3c0028c2u;
                                    TSIP.REG_ECH.WORD = 0x20002c60u;
                                    TSIP.REG_ECH.WORD = 0x38000c63u;
                                    TSIP.REG_E0H.WORD = 0x00000080u;
                                    TSIP.REG_1CH.WORD = 0x00260000u;
                                    RX671_func100(change_endian_long(0x2c2650f2u), change_endian_long(0x9094f4f5u), change_endian_long(0x8e92d9f0u), change_endian_long(0xd4b5eb79u));
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
                                        RX671_func101(change_endian_long(0x3ce6dabfu), change_endian_long(0x5b4e40c1u), change_endian_long(0x18814126u), change_endian_long(0x18eed7b2u));
                                    }
                                    TSIP.REG_ECH.WORD = 0x00002c40u;
                                    RX671_func101(change_endian_long(0x021df081u), change_endian_long(0x220a8785u), change_endian_long(0x909cb095u), change_endian_long(0x2ef138aau));
                                }
                                TSIP.REG_ECH.WORD = 0x08000c21u;
                                TSIP.REG_E0H.WORD = 0x00000080u;
                                TSIP.REG_1CH.WORD = 0x00260000u;
                                TSIP.REG_1CH.WORD = 0x00402000u;
                                TSIP.REG_ECH.WORD = 0x00002ca0u;
                                RX671_func101(change_endian_long(0xe3f5f47au), change_endian_long(0x147df75eu), change_endian_long(0xef3b03efu), change_endian_long(0x20d22bafu));
                            }
                            TSIP.REG_ECH.WORD = 0x38000805u;
                            TSIP.REG_E0H.WORD = 0x00000080u;
                            TSIP.REG_1CH.WORD = 0x00260000u;
                            TSIP.REG_1CH.WORD = 0x00402000u;
                            RX671_func101(change_endian_long(0x2db4f5eeu), change_endian_long(0xc4ee853cu), change_endian_long(0x3caa7d73u), change_endian_long(0xc5e9ba1cu));
                        }
                        RX671_func101(change_endian_long(0x3fedbecfu), change_endian_long(0xcb9f256au), change_endian_long(0x29ad9787u), change_endian_long(0x9f7ce952u));
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
                        RX671_func100(change_endian_long(0xaea71ed3u), change_endian_long(0x5f7277cau), change_endian_long(0x58092dc7u), change_endian_long(0xde260b0bu));
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
                            RX671_func101(change_endian_long(0x5db2fd38u), change_endian_long(0xa90ee62au), change_endian_long(0x90b55e74u), change_endian_long(0x3d928986u));
                        }
                        TSIP.REG_ECH.WORD = 0x38000805u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        TSIP.REG_1CH.WORD = 0x00402000u;
                        RX671_func100(change_endian_long(0xb69c56d7u), change_endian_long(0x8df5703du), change_endian_long(0x05f16c8au), change_endian_long(0xe59dc6c3u));
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
                            RX671_func101(change_endian_long(0x5a0aad72u), change_endian_long(0xcab28192u), change_endian_long(0x2e9cfd19u), change_endian_long(0xd5eaf6a2u));
                        }
                        TSIP.REG_ECH.WORD = 0x38000805u;
                        TSIP.REG_E0H.WORD = 0x00000080u;
                        TSIP.REG_1CH.WORD = 0x00260000u;
                        TSIP.REG_1CH.WORD = 0x00402000u;
                        RX671_func101(change_endian_long(0xe5b0e4e6u), change_endian_long(0xe682ec7cu), change_endian_long(0xf3035876u), change_endian_long(0x300c7274u));
                    }
                    RX671_func100(change_endian_long(0x629d9160u), change_endian_long(0xbc984b85u), change_endian_long(0xa765fd25u), change_endian_long(0xb18c816fu));
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
                        RX671_func101(change_endian_long(0x2645a036u), change_endian_long(0xc1532cadu), change_endian_long(0x999ec866u), change_endian_long(0x5838cfb1u));
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
                    RX671_func100(change_endian_long(0xb77ea485u), change_endian_long(0x67f263b4u), change_endian_long(0x24b26524u), change_endian_long(0xf5eb7798u));
                    RX671_func103();
                    RX671_func100(change_endian_long(0x0da7de7cu), change_endian_long(0x4d59fb0du), change_endian_long(0xb8ad9685u), change_endian_long(0xe9cf503fu));
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
                    RX671_func100(change_endian_long(0xa08a3631u), change_endian_long(0xee1dd35du), change_endian_long(0x764c4dbbu), change_endian_long(0x75538053u));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX671_func100(change_endian_long(0x5ad10aadu), change_endian_long(0xdaee36ebu), change_endian_long(0x0ad81e38u), change_endian_long(0x298669b0u));
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
                            RX671_func100(change_endian_long(0x29dc6544u), change_endian_long(0x3e7e2751u), change_endian_long(0x017403f9u), change_endian_long(0xb62ede4cu));
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
                                RX671_func101(change_endian_long(0x027eac8eu), change_endian_long(0x3ca503b6u), change_endian_long(0x2d0e16b7u), change_endian_long(0x7e54f545u));
                            }
                            else
                            {
                                RX671_func100(change_endian_long(0xb9cbdc1fu), change_endian_long(0x1d7bf13cu), change_endian_long(0xcd636358u), change_endian_long(0x0cf16ea4u));
                                TSIP.REG_104H.WORD = 0x00000058u;
                                TSIP.REG_E0H.WORD = 0x80010020u;
                                /* WAIT_LOOP */
                                while (1u != TSIP.REG_104H.BIT.B31)
                                {
                                    /* waiting */
                                }
                                TSIP.REG_100H.WORD = change_endian_long(0x5d09984du);
                                RX671_func080();
                                RX671_func100(change_endian_long(0x3198cf33u), change_endian_long(0xa87bb257u), change_endian_long(0x6cd0feb4u), change_endian_long(0xc2f9f7feu));
                                TSIP.REG_00H.WORD = 0x00012303u;
                                TSIP.REG_2CH.WORD = 0x00000020u;
                                RX671_func313(1496);
                                RX671_func100(change_endian_long(0xdb391345u), change_endian_long(0xe26417acu), change_endian_long(0xf7a79cebu), change_endian_long(0xa3c1f032u));
                                RX671_func314(1496+64);
                                RX671_func100(change_endian_long(0xf7b6b407u), change_endian_long(0x9f3f4c8cu), change_endian_long(0x90ccc0f6u), change_endian_long(0x0df75fa4u));
                                TSIP.REG_104H.WORD = 0x00000058u;
                                TSIP.REG_E0H.WORD = 0x80010020u;
                                /* WAIT_LOOP */
                                while (1u != TSIP.REG_104H.BIT.B31)
                                {
                                    /* waiting */
                                }
                                TSIP.REG_100H.WORD = change_endian_long(0xcbc8750cu);
                                RX671_func080();
                                RX671_func100(change_endian_long(0x4774ea7fu), change_endian_long(0xcde981abu), change_endian_long(0x61592281u), change_endian_long(0xf8b7d7d5u));
                                TSIP.REG_ECH.WORD = 0x00000800u;
                                TSIP.REG_E0H.WORD = 0x81c00000u;
                                TSIP.REG_00H.WORD = 0x00012803u;
                                RX671_func313(1496+68);
                                RX671_func100(change_endian_long(0xe5d65c95u), change_endian_long(0x146e0529u), change_endian_long(0xa72fa93au), change_endian_long(0xbdcba21fu));
                                RX671_func314(1496+132);
                                RX671_func101(change_endian_long(0x644f27dbu), change_endian_long(0xc93d1592u), change_endian_long(0xa49303c0u), change_endian_long(0xedabbcb3u));
                            }
                            RX671_func101(change_endian_long(0xb3466277u), change_endian_long(0xe66820d1u), change_endian_long(0xc07c35d3u), change_endian_long(0x14a21a4du));
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
                            RX671_func101(change_endian_long(0xfd7e206du), change_endian_long(0x19835cd8u), change_endian_long(0xd6e64638u), change_endian_long(0xcf1ee7cfu));
                        }
                        RX671_func101(change_endian_long(0xa125e112u), change_endian_long(0x9f96556cu), change_endian_long(0x52c741e1u), change_endian_long(0xe30a1b5fu));
                    }
                    else
                    {
                        RX671_func101(change_endian_long(0x8e05234cu), change_endian_long(0x4d247c2fu), change_endian_long(0xdf06202cu), change_endian_long(0x5bd0b22eu));
                    }
                    RX671_func100(change_endian_long(0x3036d016u), change_endian_long(0xf3c57a0bu), change_endian_long(0x5017a06cu), change_endian_long(0x4880075bu));
                    TSIP.REG_ECH.WORD = 0x00007c0eu;
                    TSIP.REG_1CH.WORD = 0x00600000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                    {
                        RX671_func100(change_endian_long(0x07c348f1u), change_endian_long(0x10188930u), change_endian_long(0x0d62af0eu), change_endian_long(0xa3e55c8au));
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
                        RX671_func101(change_endian_long(0xefcac71fu), change_endian_long(0x2a05b5c5u), change_endian_long(0x45d45c86u), change_endian_long(0xa4f189c3u));
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
                        RX671_func100(change_endian_long(0xac0b50eau), change_endian_long(0x65188051u), change_endian_long(0x38777922u), change_endian_long(0x6f6839b5u));
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x5fe685f6u);
                        RX671_func080();
                        RX671_func100(change_endian_long(0xd195715eu), change_endian_long(0x0a5d0094u), change_endian_long(0xea5b7177u), change_endian_long(0x53df9babu));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000020u;
                        RX671_func313(748);
                        RX671_func100(change_endian_long(0x03ed9257u), change_endian_long(0x42a78cbfu), change_endian_long(0xc47d9b6fu), change_endian_long(0x816bcc7bu));
                        RX671_func314(748+64);
                        RX671_func100(change_endian_long(0x1d873e1eu), change_endian_long(0x4457027eu), change_endian_long(0x5ff2b13eu), change_endian_long(0xa33a87eau));
                        TSIP.REG_104H.WORD = 0x00000058u;
                        TSIP.REG_E0H.WORD = 0x80010020u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_104H.BIT.B31)
                        {
                            /* waiting */
                        }
                        TSIP.REG_100H.WORD = change_endian_long(0x1c5a2b81u);
                        RX671_func080();
                        RX671_func100(change_endian_long(0xa6c5b8b1u), change_endian_long(0xa96694d5u), change_endian_long(0xb8017612u), change_endian_long(0x518ee142u));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000022u;
                        RX671_func313(680);
                        RX671_func100(change_endian_long(0xf64d7976u), change_endian_long(0x5df37d70u), change_endian_long(0xa3e288a5u), change_endian_long(0x21f21465u));
                        RX671_func314(680+64);
                        RX671_func100(change_endian_long(0xd16d9c8au), change_endian_long(0x9b0e30efu), change_endian_long(0x52aa76c4u), change_endian_long(0x7f316624u));
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
                        RX671_func080();
                        RX671_func100(change_endian_long(0x10dd5a96u), change_endian_long(0xde55cb5bu), change_endian_long(0xc4067884u), change_endian_long(0xbc02a608u));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000020u;
                        RX671_func313(68);
                        RX671_func100(change_endian_long(0xcb654590u), change_endian_long(0xa31a7d79u), change_endian_long(0x74e5ce73u), change_endian_long(0xd9184230u));
                        RX671_func314(68+64);
                        RX671_func100(change_endian_long(0x2e2728bcu), change_endian_long(0xdf59f0e8u), change_endian_long(0x941a8cd9u), change_endian_long(0x4a6caceau));
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
                        RX671_func080();
                        RX671_func100(change_endian_long(0xd05d8057u), change_endian_long(0x08e75c30u), change_endian_long(0x5bdf6391u), change_endian_long(0xf4675ab1u));
                        TSIP.REG_00H.WORD = 0x00012303u;
                        TSIP.REG_2CH.WORD = 0x00000024u;
                        RX671_func313(0);
                        RX671_func100(change_endian_long(0x5b5e9e31u), change_endian_long(0x5f32248fu), change_endian_long(0xbeaca7d1u), change_endian_long(0xb158b696u));
                        RX671_func314(0+64);
                        TSIP.REG_ECH.WORD = 0x0000b400u;
                        TSIP.REG_ECH.WORD = 0x00000001u;
                        RX671_func101(change_endian_long(0x51f92793u), change_endian_long(0xb6ae91aeu), change_endian_long(0x5e8a50c7u), change_endian_long(0xfdca4207u));
                        RX671_func307();
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
                        RX671_func101(change_endian_long(0xb84045d2u), change_endian_long(0x6fc50d95u), change_endian_long(0x2e2db718u), change_endian_long(0x5662ceafu));
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
                        RX671_func101(change_endian_long(0x0ac61591u), change_endian_long(0x67787da5u), change_endian_long(0x399a504au), change_endian_long(0x3d5cf489u));
                        RX671_func003(InData_Signature);
                        RX671_func101(change_endian_long(0x0be1a6c8u), change_endian_long(0xf297461cu), change_endian_long(0xe23d9ee6u), change_endian_long(0xd600d13fu));
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
                        RX671_func101(change_endian_long(0x48497317u), change_endian_long(0x92e3fb2bu), change_endian_long(0xe13081d4u), change_endian_long(0xf1e9a00bu));
                        RX671_func015();
                        RX671_func101(change_endian_long(0x3345694eu), change_endian_long(0x82e7558au), change_endian_long(0x8f044766u), change_endian_long(0x9da40fd6u));
                    }
                    RX671_func100(change_endian_long(0xeabb4c24u), change_endian_long(0x18df6047u), change_endian_long(0x09126647u), change_endian_long(0xef64a309u));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX671_func102(change_endian_long(0x2a31ae86u), change_endian_long(0x36546513u), change_endian_long(0x3beb1d40u), change_endian_long(0x1e95e5b9u));
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
                        RX671_func100(change_endian_long(0xf3a32625u), change_endian_long(0xb06a29d8u), change_endian_long(0x91ae65d5u), change_endian_long(0xdb205d47u));
                        TSIP.REG_1CH.WORD = 0x00400000u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        if (1u == (TSIP.REG_1CH.BIT.B22))
                        {
                            RX671_func102(change_endian_long(0x1e00a2a9u), change_endian_long(0x5ebd27acu), change_endian_long(0x545b08fbu), change_endian_long(0x857fef2au));
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
                            RX671_func100(change_endian_long(0x9eb33379u), change_endian_long(0x1a322040u), change_endian_long(0xde073ce1u), change_endian_long(0x602c0f6au));
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
                                RX671_func100(change_endian_long(0x316cf340u), change_endian_long(0xf4812211u), change_endian_long(0xb38c2b70u), change_endian_long(0x7e0d9d0bu));
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
                                    RX671_func101(change_endian_long(0x287307d3u), change_endian_long(0xa631404fu), change_endian_long(0x15adf5cdu), change_endian_long(0xb6ed0fbau));
                                }
                                else
                                {
                                    RX671_func100(change_endian_long(0x9e229a65u), change_endian_long(0x54a81df5u), change_endian_long(0xe9278870u), change_endian_long(0x3503fc66u));
                                    TSIP.REG_C4H.WORD = 0x000c0805u;
                                    TSIP.REG_E0H.WORD = 0x810402e0u;
                                    TSIP.REG_00H.WORD = 0x00002813u;
                                    /* WAIT_LOOP */
                                    while (0u != TSIP.REG_00H.BIT.B25)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_1CH.WORD = 0x00001800u;
                                    RX671_func100(change_endian_long(0x5f87c1a9u), change_endian_long(0xf0b6db9eu), change_endian_long(0x84f778f6u), change_endian_long(0x2a97ac40u));
                                    TSIP.REG_104H.WORD = 0x00000058u;
                                    TSIP.REG_E0H.WORD = 0x80010020u;
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_104H.BIT.B31)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_100H.WORD = change_endian_long(0x5d09984du);
                                    RX671_func080();
                                    RX671_func100(change_endian_long(0x3b69929du), change_endian_long(0x7ad8a9c3u), change_endian_long(0xe251617du), change_endian_long(0x1390817au));
                                    TSIP.REG_00H.WORD = 0x00013203u;
                                    TSIP.REG_2CH.WORD = 0x00000014u;
                                    RX671_func312(1496);
                                    TSIP.REG_104H.WORD = 0x00000058u;
                                    TSIP.REG_E0H.WORD = 0x80010020u;
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_104H.BIT.B31)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_100H.WORD = change_endian_long(0xcbc8750cu);
                                    RX671_func080();
                                    TSIP.REG_00H.WORD = 0x00013203u;
                                    TSIP.REG_2CH.WORD = 0x00000012u;
                                    RX671_func312(1496+68);
                                    TSIP.REG_104H.WORD = 0x00000058u;
                                    TSIP.REG_E0H.WORD = 0x80010000u;
                                    /* WAIT_LOOP */
                                    while (1u != TSIP.REG_104H.BIT.B31)
                                    {
                                        /* waiting */
                                    }
                                    TSIP.REG_100H.WORD = change_endian_long(0x0a880100u);
                                    RX671_func101(change_endian_long(0x8b9cbaaau), change_endian_long(0x66514658u), change_endian_long(0xf36fb84fu), change_endian_long(0x4d5092e5u));
                                }
                                RX671_func100(change_endian_long(0x213c5cc7u), change_endian_long(0xe9f45d32u), change_endian_long(0xef15d2e4u), change_endian_long(0xcb4a331au));
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
                                RX671_func100(change_endian_long(0xe40537f5u), change_endian_long(0x468849deu), change_endian_long(0x0f495c7au), change_endian_long(0x862ca027u));
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
                                RX671_func100(change_endian_long(0xfd11ab72u), change_endian_long(0xdf4b5371u), change_endian_long(0xe6d5abe1u), change_endian_long(0xa473c9bau));
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
                                RX671_func100(change_endian_long(0x2823fedcu), change_endian_long(0x2fd8d68fu), change_endian_long(0xdc47492bu), change_endian_long(0x9b9e8dd0u));
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
                                RX671_func100(change_endian_long(0xdf378742u), change_endian_long(0x5df25020u), change_endian_long(0x007d0348u), change_endian_long(0xfa254067u));
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
                                RX671_func100(change_endian_long(0xb53fe86bu), change_endian_long(0x43f3b307u), change_endian_long(0x6ef447b5u), change_endian_long(0x6f7b69a7u));
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
                                RX671_func102(change_endian_long(0x89e6e7d3u), change_endian_long(0xcd7ee58eu), change_endian_long(0x47a6ee9du), change_endian_long(0x2f88e68au));
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
                                RX671_func004(KEY_ADR);
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
                                RX671_func100(change_endian_long(0xe63afa63u), change_endian_long(0x914c8bd3u), change_endian_long(0x863339b8u), change_endian_long(0x91257642u));
                                TSIP.REG_1CH.WORD = 0x00400000u;
                                TSIP.REG_1D0H.WORD = 0x00000000u;
                                if (1u == (TSIP.REG_1CH.BIT.B22))
                                {
                                    RX671_func102(change_endian_long(0x0a773b7du), change_endian_long(0x19fb4189u), change_endian_long(0x5bb325afu), change_endian_long(0x84b0d2cfu));
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
                                    RX671_func100(change_endian_long(0x16bd2976u), change_endian_long(0xa5fb4e9fu), change_endian_long(0x3f505a62u), change_endian_long(0x6ded0c38u));
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
                                    RX671_func100(change_endian_long(0xd9f3dba5u), change_endian_long(0xab8ca3fbu), change_endian_long(0xd6fd7fa8u), change_endian_long(0x7a4553b5u));
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
                                    RX671_func100(change_endian_long(0xa3f09d67u), change_endian_long(0x3c195590u), change_endian_long(0xfe9002f2u), change_endian_long(0xb8aad4a7u));
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
                                    RX671_func100(change_endian_long(0xd7b150d0u), change_endian_long(0x868e440cu), change_endian_long(0xacc8dec2u), change_endian_long(0x21411b75u));
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
                                    RX671_func102(change_endian_long(0x454d1b6cu), change_endian_long(0x4380da35u), change_endian_long(0xb87b2c56u), change_endian_long(0xd60f8c80u));
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
 End of function ./input_dir/RX671/RX671_pe1_r2.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_TLS == 1 */
