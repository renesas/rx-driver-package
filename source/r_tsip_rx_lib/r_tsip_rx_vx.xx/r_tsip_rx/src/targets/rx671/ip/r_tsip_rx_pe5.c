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
* Function Name: R_TSIP_TlsGenerateSessionKeySub
*******************************************************************************************************************/ /**
* @details       RX671 TLS Keys Generation
* @param[in]     InData_Sel_CipherSuite
* @param[in]     InData_MasterSecret
* @param[in]     InData_ClientRandom
* @param[in]     InData_ServerRandom
* @param[in]     InData_NonceExplicit
* @param[out]    OutData_ClientMACKeyOperationCode
* @param[out]    OutData_ServerMACKeyOperationCode
* @param[out]    OutData_ClientEncKeyOperationCode
* @param[out]    OutData_ServerEncKeyOperationCode
* @param[out]    
* @param[out]    OutData_ClientIV
* @param[out]    OutData_ServerIV
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @note          None
*/
e_tsip_err_t R_TSIP_TlsGenerateSessionKeySub(uint32_t *InData_Sel_CipherSuite, uint32_t *InData_MasterSecret, uint32_t *InData_ClientRandom, uint32_t *InData_ServerRandom, uint32_t *InData_NonceExplicit, uint32_t *OutData_ClientMACKeyOperationCode, uint32_t *OutData_ServerMACKeyOperationCode, uint32_t *OutData_ClientEncKeyOperationCode, uint32_t *OutData_ServerEncKeyOperationCode, uint32_t* OutData_ClientIV, uint32_t* OutData_ServerIV)
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
    if (0x0u != (TSIP.REG_1BCH.WORD & 0x1fu))
    {
        #if TSIP_MULTI_THREADING == 1
        TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
        #endif /* TSIP_MULTI_THREADING == 1 */
        return TSIP_ERR_RESOURCE_CONFLICT;
    }
    TSIP.REG_84H.WORD = 0x0000e501u;
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
    TSIP.REG_E0H.WORD = 0x80010000u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Sel_CipherSuite[0];
    TSIP.REG_ECH.WORD = 0x000037a0u;
    TSIP.REG_ECH.WORD = 0x3420aba0u;
    TSIP.REG_ECH.WORD = 0x00000008u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(change_endian_long(0xfe51dc93u), change_endian_long(0x7c3141fbu), change_endian_long(0x366c7ea1u), change_endian_long(0x9072e1e7u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func102(change_endian_long(0x49dd60c8u), change_endian_long(0xf586a9a1u), change_endian_long(0xa580b5cbu), change_endian_long(0x016bf0dbu));
        TSIP.REG_1BCH.WORD = 0x00000040u;
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
        TSIP.REG_C4H.WORD = 0x00082b8du;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_MasterSecret[0];
        TSIP.REG_100H.WORD = InData_MasterSecret[1];
        TSIP.REG_100H.WORD = InData_MasterSecret[2];
        TSIP.REG_100H.WORD = InData_MasterSecret[3];
        TSIP.REG_C4H.WORD = 0x400c0b0cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x6226a005u);
        TSIP.REG_104H.WORD = 0x00000b62u;
        TSIP.REG_D0H.WORD = 0x00000200u;
        TSIP.REG_C4H.WORD = 0x02f087bfu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_MasterSecret[4];
        TSIP.REG_100H.WORD = InData_MasterSecret[5];
        TSIP.REG_100H.WORD = InData_MasterSecret[6];
        TSIP.REG_100H.WORD = InData_MasterSecret[7];
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_MasterSecret[8];
        TSIP.REG_100H.WORD = InData_MasterSecret[9];
        TSIP.REG_100H.WORD = InData_MasterSecret[10];
        TSIP.REG_100H.WORD = InData_MasterSecret[11];
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_MasterSecret[12];
        TSIP.REG_100H.WORD = InData_MasterSecret[13];
        TSIP.REG_100H.WORD = InData_MasterSecret[14];
        TSIP.REG_100H.WORD = InData_MasterSecret[15];
        TSIP.REG_ECH.WORD = 0x0000b400u;
        TSIP.REG_ECH.WORD = 0x00000070u;
        TSIP.REG_E0H.WORD = 0x808c0000u;
        TSIP.REG_00H.WORD = 0x00008233u;
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
        TSIP.REG_100H.WORD = InData_MasterSecret[16];
        TSIP.REG_100H.WORD = InData_MasterSecret[17];
        TSIP.REG_100H.WORD = InData_MasterSecret[18];
        TSIP.REG_100H.WORD = InData_MasterSecret[19];
        TSIP.REG_C4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00002213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX671_func100(change_endian_long(0x243b1495u), change_endian_long(0xf4bff9feu), change_endian_long(0xe4571e86u), change_endian_long(0x26aa4dafu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func102(change_endian_long(0x3cbf0c03u), change_endian_long(0x4c10a48bu), change_endian_long(0x4c5b72f1u), change_endian_long(0x5386f004u));
            TSIP.REG_1BCH.WORD = 0x00000040u;
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
            TSIP.REG_104H.WORD = 0x00000352u;
            TSIP.REG_C4H.WORD = 0x00000885u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x6b657920u);
            TSIP.REG_100H.WORD = change_endian_long(0x65787061u);
            TSIP.REG_100H.WORD = change_endian_long(0x6e73696fu);
            TSIP.REG_100H.WORD = change_endian_long(0x6e000000u);
            TSIP.REG_ECH.WORD = 0x0000b460u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            TSIP.REG_E0H.WORD = 0x80830003u;
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
            TSIP.REG_104H.WORD = 0x00000068u;
            TSIP.REG_E0H.WORD = 0x80010000u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x0000006eu);
            TSIP.REG_104H.WORD = 0x00000762u;
            TSIP.REG_D0H.WORD = 0x00000100u;
            TSIP.REG_C4H.WORD = 0x00008887u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_ServerRandom[0];
            TSIP.REG_100H.WORD = InData_ServerRandom[1];
            TSIP.REG_100H.WORD = InData_ServerRandom[2];
            TSIP.REG_100H.WORD = InData_ServerRandom[3];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_ServerRandom[4];
            TSIP.REG_100H.WORD = InData_ServerRandom[5];
            TSIP.REG_100H.WORD = InData_ServerRandom[6];
            TSIP.REG_100H.WORD = InData_ServerRandom[7];
            TSIP.REG_ECH.WORD = 0x0000b460u;
            TSIP.REG_ECH.WORD = 0x0000002cu;
            for (iLoop = 0; iLoop < 8; iLoop = iLoop + 1)
            {
                TSIP.REG_E0H.WORD = 0x80010020u;
                TSIP.REG_00H.WORD = 0x00008207u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x01986c01u;
                TSIP.REG_ECH.WORD = 0x00003c03u;
                TSIP.REG_ECH.WORD = 0x00003401u;
                TSIP.REG_ECH.WORD = 0x0000a460u;
                TSIP.REG_ECH.WORD = 0x00000004u;
            }
            TSIP.REG_104H.WORD = 0x00000762u;
            TSIP.REG_D0H.WORD = 0x00000100u;
            TSIP.REG_C4H.WORD = 0x00008887u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_ClientRandom[0];
            TSIP.REG_100H.WORD = InData_ClientRandom[1];
            TSIP.REG_100H.WORD = InData_ClientRandom[2];
            TSIP.REG_100H.WORD = InData_ClientRandom[3];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_ClientRandom[4];
            TSIP.REG_100H.WORD = InData_ClientRandom[5];
            TSIP.REG_100H.WORD = InData_ClientRandom[6];
            TSIP.REG_100H.WORD = InData_ClientRandom[7];
            for (iLoop = 0; iLoop < 8; iLoop = iLoop + 1)
            {
                TSIP.REG_E0H.WORD = 0x80010020u;
                TSIP.REG_00H.WORD = 0x00008207u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x01986c01u;
                TSIP.REG_ECH.WORD = 0x00003c03u;
                TSIP.REG_ECH.WORD = 0x00003401u;
                TSIP.REG_ECH.WORD = 0x0000a460u;
                TSIP.REG_ECH.WORD = 0x00000004u;
            }
            TSIP.REG_ECH.WORD = 0x00186c00u;
            TSIP.REG_ECH.WORD = 0x00003c03u;
            TSIP.REG_ECH.WORD = 0x0000b420u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            TSIP.REG_ECH.WORD = 0x0000b560u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            TSIP.REG_ECH.WORD = 0x0000098cu;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000004du;
            RX671_func101(change_endian_long(0xc61edbe4u), change_endian_long(0xe496c35fu), change_endian_long(0x5f5d1d11u), change_endian_long(0x4ed9c511u));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX671_func101(change_endian_long(0x90cce6fcu), change_endian_long(0x9b056c77u), change_endian_long(0xb338e2c5u), change_endian_long(0x7b950096u));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_E0H.WORD = 0x8188000cu;
            TSIP.REG_00H.WORD = 0x00003823u;
            TSIP.REG_2CH.WORD = 0x00000013u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000098cu;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            RX671_func101(change_endian_long(0x75b40730u), change_endian_long(0x46e93973u), change_endian_long(0xb3338f2eu), change_endian_long(0xf87580dfu));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX671_func101(change_endian_long(0x3dd9da91u), change_endian_long(0xb5c8e3adu), change_endian_long(0x527b71a4u), change_endian_long(0x2f77496du));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_E0H.WORD = 0x8188000cu;
            TSIP.REG_00H.WORD = 0x00003823u;
            TSIP.REG_2CH.WORD = 0x0000009au;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000098cu;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            RX671_func101(change_endian_long(0xbfb3ddfeu), change_endian_long(0x28531220u), change_endian_long(0xb67e60d7u), change_endian_long(0xae2b421du));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX671_func101(change_endian_long(0xe7348517u), change_endian_long(0x59a3a3f4u), change_endian_long(0x42c50446u), change_endian_long(0x8b1e8648u));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_E0H.WORD = 0x8188000cu;
            TSIP.REG_00H.WORD = 0x00003823u;
            TSIP.REG_2CH.WORD = 0x0000001au;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000098cu;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            RX671_func101(change_endian_long(0xd09fbcc5u), change_endian_long(0xca9bdccbu), change_endian_long(0xdd5352cau), change_endian_long(0x3ec2e254u));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX671_func101(change_endian_long(0xda1556f2u), change_endian_long(0xac2a11a0u), change_endian_long(0x57e5bd95u), change_endian_long(0xae286847u));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_E0H.WORD = 0x8188000cu;
            TSIP.REG_00H.WORD = 0x00003823u;
            TSIP.REG_2CH.WORD = 0x00000092u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000098cu;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            RX671_func101(change_endian_long(0x14ce6415u), change_endian_long(0x792e567au), change_endian_long(0xb5e86231u), change_endian_long(0x7685da71u));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX671_func101(change_endian_long(0xeb2be2c1u), change_endian_long(0x3551e158u), change_endian_long(0xa025ddd1u), change_endian_long(0x5dfa8534u));
            R_TSIP_TlsGenerateSubSub();

            RX671_func100(change_endian_long(0xef5ab011u), change_endian_long(0x18e8e001u), change_endian_long(0x337c8b0eu), change_endian_long(0xbff68c55u));
            TSIP.REG_E0H.WORD = 0x8188000cu;
            TSIP.REG_00H.WORD = 0x00003823u;
            TSIP.REG_2CH.WORD = 0x00000012u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_28H.WORD = 0x00a70001u;
            TSIP.REG_ECH.WORD = 0x000034ddu;
            TSIP.REG_ECH.WORD = 0x3420aba0u;
            TSIP.REG_ECH.WORD = 0x00000004u;
            TSIP.REG_ECH.WORD = 0x2000b4c0u;
            TSIP.REG_ECH.WORD = 0x00000002u;
            TSIP.REG_ECH.WORD = 0x3420aba0u;
            TSIP.REG_ECH.WORD = 0x00000006u;
            TSIP.REG_ECH.WORD = 0x2000b4c0u;
            TSIP.REG_ECH.WORD = 0x00000006u;
            TSIP.REG_ECH.WORD = 0x00007c06u;
            TSIP.REG_1CH.WORD = 0x00600000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX671_func100(change_endian_long(0x39d6f7f3u), change_endian_long(0x93dbd998u), change_endian_long(0x61f18b72u), change_endian_long(0xce2faa79u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x03200100u);
                RX671_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX671_func100(change_endian_long(0x3068db2eu), change_endian_long(0xdaff436au), change_endian_long(0xfe4671cbu), change_endian_long(0x686cd9d1u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002317u;
                TSIP.REG_2CH.WORD = 0x00000022u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0000200fu;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_04H.WORD = 0x00000222u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[4] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[5] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[6] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[8] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[9] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[10] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[11] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x95f31d40u), change_endian_long(0x347703a9u), change_endian_long(0xfb93d25fu), change_endian_long(0x1392afb6u));
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
                OutData_ClientMACKeyOperationCode[12] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[13] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[14] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[15] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x877fe581u), change_endian_long(0xabc5016au), change_endian_long(0x6635e556u), change_endian_long(0x62776942u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x35ce69e8u), change_endian_long(0x3792bc90u), change_endian_long(0xd189e29eu), change_endian_long(0x85dc1ef4u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x03100100u);
                RX671_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX671_func100(change_endian_long(0x8d1d127bu), change_endian_long(0x54ec274au), change_endian_long(0x3e5acf67u), change_endian_long(0xd0f5a631u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002317u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0000200fu;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_04H.WORD = 0x00000222u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[4] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[5] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[6] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[8] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[9] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[10] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[11] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x78e74a3bu), change_endian_long(0xcdf1b9ecu), change_endian_long(0x2a0219d4u), change_endian_long(0x336ec349u));
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
                OutData_ServerMACKeyOperationCode[12] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[13] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[14] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[15] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xa5ea81dbu), change_endian_long(0xeeedc8ddu), change_endian_long(0xc1ab4993u), change_endian_long(0x117bd3d9u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xaf37fbd9u), change_endian_long(0xcb57e5ceu), change_endian_long(0xafed674cu), change_endian_long(0x30160f00u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00800100u);
                RX671_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX671_func100(change_endian_long(0x36a8c466u), change_endian_long(0x51ba387eu), change_endian_long(0x174c981cu), change_endian_long(0x9757849eu));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_C4H.WORD = 0x02e007bdu;
                TSIP.REG_00H.WORD = 0x00002313u;
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
                OutData_ClientEncKeyOperationCode[4] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[5] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[6] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[7] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x35115bdbu), change_endian_long(0x1dda3a99u), change_endian_long(0xab9feca9u), change_endian_long(0xf1c7990fu));
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
                OutData_ClientEncKeyOperationCode[8] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[9] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[10] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[11] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xb94eed9eu), change_endian_long(0x20e7e923u), change_endian_long(0x6f847233u), change_endian_long(0x4f98f358u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xf9d50021u), change_endian_long(0x21435f67u), change_endian_long(0x196a16d9u), change_endian_long(0x4b43280du));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00400100u);
                RX671_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX671_func100(change_endian_long(0x4de1f0d8u), change_endian_long(0x3f9a2f5fu), change_endian_long(0xbfbb141du), change_endian_long(0xf0a8c42eu));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_C4H.WORD = 0x02e007bdu;
                TSIP.REG_00H.WORD = 0x00002313u;
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
                OutData_ServerEncKeyOperationCode[4] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[5] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[6] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[7] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x98cdbc53u), change_endian_long(0x2ca7ade5u), change_endian_long(0xa7d4da0cu), change_endian_long(0x612ba128u));
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
                OutData_ServerEncKeyOperationCode[8] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[9] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[10] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[11] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x69bdd440u), change_endian_long(0xf106d69fu), change_endian_long(0xcda45392u), change_endian_long(0xe3138740u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x8a029765u), change_endian_long(0xa6ad3a70u), change_endian_long(0x05922cd2u), change_endian_long(0x86157bb2u));
                TSIP.REG_C4H.WORD = 0x00000885u;
                TSIP.REG_00H.WORD = 0x00002313u;
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
                OutData_ClientIV[0] = TSIP.REG_100H.WORD;
                OutData_ClientIV[1] = TSIP.REG_100H.WORD;
                OutData_ClientIV[2] = TSIP.REG_100H.WORD;
                OutData_ClientIV[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x0c7b8360u), change_endian_long(0xd11edaa1u), change_endian_long(0x94ebcdb1u), change_endian_long(0x518d21fdu));
                TSIP.REG_C4H.WORD = 0x00000885u;
                TSIP.REG_00H.WORD = 0x00002313u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0000033bu;
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
                OutData_ServerIV[0] = TSIP.REG_100H.WORD;
                OutData_ServerIV[1] = TSIP.REG_100H.WORD;
                OutData_ServerIV[2] = TSIP.REG_100H.WORD;
                OutData_ServerIV[3] = TSIP.REG_100H.WORD;
                RX671_func101(change_endian_long(0xa547cb57u), change_endian_long(0x10014a31u), change_endian_long(0x7289f8f0u), change_endian_long(0x2f562093u));
            }
            else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX671_func100(change_endian_long(0x5bd34a49u), change_endian_long(0xcd5f20f5u), change_endian_long(0x0c6e84aau), change_endian_long(0x83a7d610u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x03200100u);
                RX671_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX671_func100(change_endian_long(0xd62b3d59u), change_endian_long(0x7aaabc7du), change_endian_long(0x8f8056f9u), change_endian_long(0x4af96b9au));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002317u;
                TSIP.REG_2CH.WORD = 0x00000022u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0000200fu;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_04H.WORD = 0x00000222u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[4] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[5] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[6] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[8] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[9] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[10] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[11] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x8eb84beau), change_endian_long(0xea7a9fc4u), change_endian_long(0x23aa40dcu), change_endian_long(0xfbdfeb97u));
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
                OutData_ClientMACKeyOperationCode[12] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[13] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[14] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[15] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x3fde6a54u), change_endian_long(0x4e343f29u), change_endian_long(0xe413ee4fu), change_endian_long(0x4943ba1cu));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x4647dadeu), change_endian_long(0x137b5fbau), change_endian_long(0x9427c48cu), change_endian_long(0x64644dddu));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x03100100u);
                RX671_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX671_func100(change_endian_long(0xee31bcbeu), change_endian_long(0x94947a06u), change_endian_long(0x3754dda7u), change_endian_long(0x376bb328u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002317u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0000200fu;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_04H.WORD = 0x00000222u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[4] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[5] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[6] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[8] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[9] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[10] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[11] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x97453e87u), change_endian_long(0x799faeefu), change_endian_long(0x9637de4bu), change_endian_long(0x6992459cu));
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
                OutData_ServerMACKeyOperationCode[12] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[13] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[14] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[15] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x271aee32u), change_endian_long(0x443117c7u), change_endian_long(0xf994e184u), change_endian_long(0x6af984fcu));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xe383272au), change_endian_long(0xaf6723a4u), change_endian_long(0xca4b8280u), change_endian_long(0x144440f8u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x01800100u);
                RX671_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX671_func100(change_endian_long(0x582a1a4fu), change_endian_long(0xe62d7f15u), change_endian_long(0x86bd1138u), change_endian_long(0xa679ebb8u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002323u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_04H.WORD = 0x00000222u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[4] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[5] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[6] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[8] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[9] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[10] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[11] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xf476d2dcu), change_endian_long(0x9844e365u), change_endian_long(0x53109235u), change_endian_long(0x905d4e9eu));
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
                OutData_ClientEncKeyOperationCode[12] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[13] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[14] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[15] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x8e59e00eu), change_endian_long(0x1d5c93ffu), change_endian_long(0xf11a2dc9u), change_endian_long(0x83fb71edu));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xda2e7cc4u), change_endian_long(0x82244589u), change_endian_long(0x36b60635u), change_endian_long(0xff734302u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x01400100u);
                RX671_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX671_func100(change_endian_long(0xa55fc695u), change_endian_long(0x0d7bd53bu), change_endian_long(0x8dd85911u), change_endian_long(0xe8dcf73cu));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002323u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_04H.WORD = 0x00000222u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[4] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[5] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[6] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[8] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[9] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[10] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[11] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x9dce4c96u), change_endian_long(0xf2b88c04u), change_endian_long(0xea9a877cu), change_endian_long(0x6d8d80e3u));
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
                OutData_ServerEncKeyOperationCode[12] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[13] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[14] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[15] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x261e8421u), change_endian_long(0xc37fe419u), change_endian_long(0xbff870a0u), change_endian_long(0x7d65734cu));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x4096d73bu), change_endian_long(0xe7920215u), change_endian_long(0x710c4709u), change_endian_long(0xd7984403u));
                TSIP.REG_C4H.WORD = 0x00000885u;
                TSIP.REG_00H.WORD = 0x00002313u;
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
                OutData_ClientIV[0] = TSIP.REG_100H.WORD;
                OutData_ClientIV[1] = TSIP.REG_100H.WORD;
                OutData_ClientIV[2] = TSIP.REG_100H.WORD;
                OutData_ClientIV[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x66ee8b40u), change_endian_long(0x9d3638c3u), change_endian_long(0x19b39436u), change_endian_long(0x13309390u));
                TSIP.REG_C4H.WORD = 0x00000885u;
                TSIP.REG_00H.WORD = 0x00002313u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0000031bu;
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
                OutData_ServerIV[0] = TSIP.REG_100H.WORD;
                OutData_ServerIV[1] = TSIP.REG_100H.WORD;
                OutData_ServerIV[2] = TSIP.REG_100H.WORD;
                OutData_ServerIV[3] = TSIP.REG_100H.WORD;
                RX671_func101(change_endian_long(0x9027d1a9u), change_endian_long(0xda28c334u), change_endian_long(0x52e4c738u), change_endian_long(0x4e97d861u));
            }
            else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX671_func100(change_endian_long(0x2beccf52u), change_endian_long(0xd4786090u), change_endian_long(0x000a6c30u), change_endian_long(0x6a9e3d16u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x02200100u);
                RX671_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX671_func100(change_endian_long(0x03d7df70u), change_endian_long(0x260c2fbau), change_endian_long(0x3eb672c1u), change_endian_long(0x029413bcu));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002323u;
                TSIP.REG_2CH.WORD = 0x00000022u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_04H.WORD = 0x00000222u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[4] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[5] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[6] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[8] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[9] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[10] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[11] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xde18fe53u), change_endian_long(0xe721e341u), change_endian_long(0x05398e99u), change_endian_long(0xe56d6fb0u));
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
                OutData_ClientMACKeyOperationCode[12] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[13] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[14] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[15] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xce3869c2u), change_endian_long(0x85a049ffu), change_endian_long(0xf1b5a00cu), change_endian_long(0xeca83809u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xfd42aeefu), change_endian_long(0x3d03d2f1u), change_endian_long(0x1a50c6b4u), change_endian_long(0xfea7a3c6u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x02100100u);
                RX671_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX671_func100(change_endian_long(0xbac1b8d8u), change_endian_long(0x3df01043u), change_endian_long(0x0caae05eu), change_endian_long(0xe9225ca3u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002323u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_04H.WORD = 0x00000222u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[4] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[5] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[6] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[8] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[9] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[10] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[11] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xe8d123c6u), change_endian_long(0x265adfa3u), change_endian_long(0xece34489u), change_endian_long(0x40b68865u));
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
                OutData_ServerMACKeyOperationCode[12] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[13] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[14] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[15] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xccf05958u), change_endian_long(0x2d92da9au), change_endian_long(0xbbe3fff7u), change_endian_long(0x2c8f8e12u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xafc5d0e7u), change_endian_long(0x0f7ba889u), change_endian_long(0x56ee700bu), change_endian_long(0x1d1ac7c2u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00800100u);
                RX671_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX671_func100(change_endian_long(0xf4effe49u), change_endian_long(0x79dbc86au), change_endian_long(0x0135898du), change_endian_long(0xd54e8279u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_C4H.WORD = 0x02e007bdu;
                TSIP.REG_00H.WORD = 0x00002313u;
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
                OutData_ClientEncKeyOperationCode[4] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[5] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[6] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[7] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xef7fd643u), change_endian_long(0x477dba88u), change_endian_long(0x670f7121u), change_endian_long(0xa6ff9bc7u));
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
                OutData_ClientEncKeyOperationCode[8] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[9] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[10] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[11] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x85613dffu), change_endian_long(0x478e59c8u), change_endian_long(0x1d652320u), change_endian_long(0x739f7282u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xbbfc7e53u), change_endian_long(0x45161f2eu), change_endian_long(0xa096bfdau), change_endian_long(0x190220f7u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00400100u);
                RX671_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX671_func100(change_endian_long(0x374d19efu), change_endian_long(0xe8934d51u), change_endian_long(0x2259ffc8u), change_endian_long(0x92a96c77u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_C4H.WORD = 0x02e007bdu;
                TSIP.REG_00H.WORD = 0x00002313u;
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
                OutData_ServerEncKeyOperationCode[4] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[5] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[6] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[7] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x34b823feu), change_endian_long(0xae36ee3fu), change_endian_long(0x60256d5du), change_endian_long(0x13af7828u));
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
                OutData_ServerEncKeyOperationCode[8] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[9] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[10] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[11] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x9b80c464u), change_endian_long(0xbd4b5784u), change_endian_long(0x7f1877abu), change_endian_long(0x7975e4bcu));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x96901ee5u), change_endian_long(0x31ee6e0fu), change_endian_long(0x15b44d20u), change_endian_long(0xb832c94du));
                TSIP.REG_C4H.WORD = 0x00000885u;
                TSIP.REG_00H.WORD = 0x00002313u;
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
                OutData_ClientIV[0] = TSIP.REG_100H.WORD;
                OutData_ClientIV[1] = TSIP.REG_100H.WORD;
                OutData_ClientIV[2] = TSIP.REG_100H.WORD;
                OutData_ClientIV[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xe441ca3cu), change_endian_long(0xa7e90c31u), change_endian_long(0x8226b259u), change_endian_long(0xfa756cffu));
                TSIP.REG_C4H.WORD = 0x00000885u;
                TSIP.REG_00H.WORD = 0x00002313u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x00000323u;
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
                OutData_ServerIV[0] = TSIP.REG_100H.WORD;
                OutData_ServerIV[1] = TSIP.REG_100H.WORD;
                OutData_ServerIV[2] = TSIP.REG_100H.WORD;
                OutData_ServerIV[3] = TSIP.REG_100H.WORD;
                RX671_func101(change_endian_long(0x577d7c7eu), change_endian_long(0xb43c05d0u), change_endian_long(0xa49b7c53u), change_endian_long(0x51727611u));
            }
            else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX671_func100(change_endian_long(0x46f4ba97u), change_endian_long(0x00e0fcc6u), change_endian_long(0x3c0a212au), change_endian_long(0xf633e370u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x02200100u);
                RX671_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX671_func100(change_endian_long(0xfe231dadu), change_endian_long(0x02aa2ad9u), change_endian_long(0x40e1d5d7u), change_endian_long(0x3c14664cu));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002323u;
                TSIP.REG_2CH.WORD = 0x00000022u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_04H.WORD = 0x00000222u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[4] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[5] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[6] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[8] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[9] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[10] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[11] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x78324f34u), change_endian_long(0x4ff32834u), change_endian_long(0x6d3284a4u), change_endian_long(0x410d4f8au));
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
                OutData_ClientMACKeyOperationCode[12] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[13] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[14] = TSIP.REG_100H.WORD;
                OutData_ClientMACKeyOperationCode[15] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x8bbc3a57u), change_endian_long(0xc94083a3u), change_endian_long(0x4ebc41c8u), change_endian_long(0x1f849774u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientMACKeyOperationCode[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xf208ac46u), change_endian_long(0xb333c735u), change_endian_long(0x94b4097eu), change_endian_long(0x58bba721u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x02100100u);
                RX671_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX671_func100(change_endian_long(0x78c30e23u), change_endian_long(0x83f28a58u), change_endian_long(0x3a24ae32u), change_endian_long(0x9ef22dfbu));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002323u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_04H.WORD = 0x00000222u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[4] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[5] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[6] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[8] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[9] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[10] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[11] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xc36a6289u), change_endian_long(0x8df7f6adu), change_endian_long(0x1c7e9ac8u), change_endian_long(0x12b2c22du));
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
                OutData_ServerMACKeyOperationCode[12] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[13] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[14] = TSIP.REG_100H.WORD;
                OutData_ServerMACKeyOperationCode[15] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x06739b70u), change_endian_long(0x89d31d73u), change_endian_long(0x78b67bc4u), change_endian_long(0x8f7517f6u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerMACKeyOperationCode[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xd5e27944u), change_endian_long(0xac9fb471u), change_endian_long(0x5c3e9974u), change_endian_long(0xe05ff469u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x01800100u);
                RX671_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX671_func100(change_endian_long(0xdfaf672du), change_endian_long(0x847f7a21u), change_endian_long(0xa7c0a9c6u), change_endian_long(0x14e68aa8u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002323u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_04H.WORD = 0x00000222u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[4] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[5] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[6] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[8] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[9] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[10] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[11] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x8f792d8du), change_endian_long(0xd1e1305du), change_endian_long(0xe604505au), change_endian_long(0xb821b86bu));
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
                OutData_ClientEncKeyOperationCode[12] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[13] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[14] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[15] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x0014bb09u), change_endian_long(0x4f383d47u), change_endian_long(0x936e88edu), change_endian_long(0xf0f8312au));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x371f1201u), change_endian_long(0x9b4883c1u), change_endian_long(0x486e8c40u), change_endian_long(0x413192efu));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x01400100u);
                RX671_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX671_func100(change_endian_long(0x64e5b30eu), change_endian_long(0x5f3d8e2eu), change_endian_long(0x99a44333u), change_endian_long(0x12ee47a8u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_00H.WORD = 0x00002323u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_04H.WORD = 0x00000222u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[4] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[5] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[6] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[8] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[9] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[10] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[11] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x64cc1b3au), change_endian_long(0x9c70b429u), change_endian_long(0xa5c34f98u), change_endian_long(0xb461087cu));
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
                OutData_ServerEncKeyOperationCode[12] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[13] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[14] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[15] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x377d5578u), change_endian_long(0x944f897cu), change_endian_long(0xad5a11ddu), change_endian_long(0xb433fdf0u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x101b58b5u), change_endian_long(0x7a792a69u), change_endian_long(0x27797d33u), change_endian_long(0x9eb51771u));
                TSIP.REG_C4H.WORD = 0x00000885u;
                TSIP.REG_00H.WORD = 0x00002313u;
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
                OutData_ClientIV[0] = TSIP.REG_100H.WORD;
                OutData_ClientIV[1] = TSIP.REG_100H.WORD;
                OutData_ClientIV[2] = TSIP.REG_100H.WORD;
                OutData_ClientIV[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x8771aa50u), change_endian_long(0x3662bee7u), change_endian_long(0xf8f47618u), change_endian_long(0xa63944f9u));
                TSIP.REG_C4H.WORD = 0x00000885u;
                TSIP.REG_00H.WORD = 0x00002313u;
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
                OutData_ServerIV[0] = TSIP.REG_100H.WORD;
                OutData_ServerIV[1] = TSIP.REG_100H.WORD;
                OutData_ServerIV[2] = TSIP.REG_100H.WORD;
                OutData_ServerIV[3] = TSIP.REG_100H.WORD;
                RX671_func101(change_endian_long(0x929318a9u), change_endian_long(0x70d69879u), change_endian_long(0x0fb5bfc5u), change_endian_long(0x0ece4ba5u));
            }
            else if (0x06000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX671_func100(change_endian_long(0x1a4bae7bu), change_endian_long(0x43f217f7u), change_endian_long(0x10a8a4bfu), change_endian_long(0x9ad83ba9u));
                TSIP.REG_A4H.WORD = 0x00040805u;
                TSIP.REG_00H.WORD = 0x00001313u;
                TSIP.REG_2CH.WORD = 0x00000022u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_A4H.WORD = 0x00050805u;
                TSIP.REG_00H.WORD = 0x00001313u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_E0H.WORD = 0x80010280u;
                TSIP.REG_00H.WORD = 0x00008307u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_E0H.WORD = 0x800202a0u;
                TSIP.REG_104H.WORD = 0x00000168u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_NonceExplicit[0];
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_NonceExplicit[1];
                TSIP.REG_ECH.WORD = 0x0000b6e0u;
                TSIP.REG_ECH.WORD = 0x00000001u;
                TSIP.REG_E0H.WORD = 0x81040280u;
                TSIP.REG_A4H.WORD = 0x00060805u;
                TSIP.REG_00H.WORD = 0x00001813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_E0H.WORD = 0x80010280u;
                TSIP.REG_00H.WORD = 0x00008307u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0000037bu;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_E0H.WORD = 0x81040280u;
                TSIP.REG_A4H.WORD = 0x00070805u;
                TSIP.REG_00H.WORD = 0x00001813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x12800100u);
                RX671_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX671_func100(change_endian_long(0xf49d1e4eu), change_endian_long(0xb2fed7e4u), change_endian_long(0x2ef683d9u), change_endian_long(0x8405d817u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_104H.WORD = 0x00000151u;
                TSIP.REG_A4H.WORD = 0x00000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_00H.WORD = 0x00002113u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_A4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_00H.WORD = 0x00002113u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_04H.WORD = 0x00000222u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[4] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[5] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[6] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[8] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[9] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[10] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[11] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xcda24919u), change_endian_long(0x7f37f920u), change_endian_long(0xcdc6810au), change_endian_long(0xaa9b5d8au));
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
                OutData_ClientEncKeyOperationCode[12] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[13] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[14] = TSIP.REG_100H.WORD;
                OutData_ClientEncKeyOperationCode[15] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x95e01135u), change_endian_long(0xc43f9686u), change_endian_long(0x2646895eu), change_endian_long(0x43dfc66au));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ClientEncKeyOperationCode[3] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xe807c57au), change_endian_long(0x9d6cae35u), change_endian_long(0xa876af6bu), change_endian_long(0xcafa86b3u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x12400100u);
                RX671_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80030060u;
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
                RX671_func100(change_endian_long(0x65a01ba3u), change_endian_long(0x576006ccu), change_endian_long(0xd577deedu), change_endian_long(0x6d5a6514u));
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02e087bfu;
                TSIP.REG_104H.WORD = 0x00000151u;
                TSIP.REG_A4H.WORD = 0x00000cc4u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_00H.WORD = 0x00002113u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_A4H.WORD = 0x01000cc4u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_00H.WORD = 0x00002113u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_04H.WORD = 0x00000222u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[4] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[5] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[6] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[8] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[9] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[10] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[11] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0x2f8a14cau), change_endian_long(0xf7ffb165u), change_endian_long(0x7127c91eu), change_endian_long(0x081514efu));
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
                OutData_ServerEncKeyOperationCode[12] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[13] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[14] = TSIP.REG_100H.WORD;
                OutData_ServerEncKeyOperationCode[15] = TSIP.REG_100H.WORD;
                RX671_func100(change_endian_long(0xcf4481d7u), change_endian_long(0x948da8edu), change_endian_long(0x1f15ad3au), change_endian_long(0x88874377u));
                TSIP.REG_E0H.WORD = 0x81040040u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ServerEncKeyOperationCode[3] = TSIP.REG_100H.WORD;
                RX671_func101(change_endian_long(0xb0ead7cau), change_endian_long(0xca900699u), change_endian_long(0xb364e121u), change_endian_long(0x5e119d41u));
            }
            RX671_func102(change_endian_long(0xdf2fa444u), change_endian_long(0x51ff0286u), change_endian_long(0x543e164eu), change_endian_long(0x34a88655u));
            TSIP.REG_1BCH.WORD = 0x00000040u;
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
/**********************************************************************************************************************
 End of function ./input_dir/RX671/RX671_pe5_r1.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_TLS == 1 */
