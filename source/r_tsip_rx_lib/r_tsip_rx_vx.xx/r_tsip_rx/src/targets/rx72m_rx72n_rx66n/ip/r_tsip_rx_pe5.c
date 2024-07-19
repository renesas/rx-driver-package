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
* @details       RX72M TLS Keys Generation
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0xfae0d7b5u), change_endian_long(0x2aa8cf2au), change_endian_long(0xfbc81ec7u), change_endian_long(0xea9502dau));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0xde2d3024u), change_endian_long(0x70bccb45u), change_endian_long(0x0b7e397au), change_endian_long(0x5dad6e40u));
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
        RX72M_RX72N_RX66N_func100(change_endian_long(0x873ca8a4u), change_endian_long(0x9e315199u), change_endian_long(0x754a2071u), change_endian_long(0x6c4d38c5u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func102(change_endian_long(0xbc316e67u), change_endian_long(0x012a5388u), change_endian_long(0x2f8f48dcu), change_endian_long(0x61edcff6u));
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
            RX72M_RX72N_RX66N_func101(change_endian_long(0x67aab183u), change_endian_long(0xd2a65a93u), change_endian_long(0x990dd270u), change_endian_long(0x2441b4b2u));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX72M_RX72N_RX66N_func101(change_endian_long(0xaef036d3u), change_endian_long(0xdf1f23b5u), change_endian_long(0xddf0e1f2u), change_endian_long(0xd21b12bbu));
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
            RX72M_RX72N_RX66N_func101(change_endian_long(0x20846a16u), change_endian_long(0xf6999868u), change_endian_long(0x56b0b670u), change_endian_long(0xae1ab0c2u));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x807eaefau), change_endian_long(0xa7e25f13u), change_endian_long(0x118b8223u), change_endian_long(0x7c8e963cu));
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
            RX72M_RX72N_RX66N_func101(change_endian_long(0x9a7f285cu), change_endian_long(0x12ba815au), change_endian_long(0x24edc839u), change_endian_long(0xcac5dce4u));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX72M_RX72N_RX66N_func101(change_endian_long(0xd1181fa0u), change_endian_long(0x3d8c6c96u), change_endian_long(0x1c6db434u), change_endian_long(0x371464bbu));
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
            RX72M_RX72N_RX66N_func101(change_endian_long(0x23497807u), change_endian_long(0x5ad4d382u), change_endian_long(0x964fa77du), change_endian_long(0xf7c5f766u));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x8f852249u), change_endian_long(0x30d07582u), change_endian_long(0xb39bad2cu), change_endian_long(0xf4e466c6u));
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
            RX72M_RX72N_RX66N_func101(change_endian_long(0x9c3efa05u), change_endian_long(0x216a020bu), change_endian_long(0xd6928495u), change_endian_long(0x627700bbu));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x9b9523deu), change_endian_long(0x62d4db48u), change_endian_long(0x2de4ba6bu), change_endian_long(0xa7782977u));
            R_TSIP_TlsGenerateSubSub();

            RX72M_RX72N_RX66N_func100(change_endian_long(0x950fa2dau), change_endian_long(0x2191ff30u), change_endian_long(0xeac8787du), change_endian_long(0xe9b169bcu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x40b18bafu), change_endian_long(0xbd714f6fu), change_endian_long(0x28ebf291u), change_endian_long(0xcb2e0da6u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x03200100u);
                RX72M_RX72N_RX66N_func103();
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x7704ac0bu), change_endian_long(0xe74ac276u), change_endian_long(0xaeeade3bu), change_endian_long(0x91b5c6b2u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xc15869b9u), change_endian_long(0x6b4cb92fu), change_endian_long(0x8b3f08e5u), change_endian_long(0x2addd588u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x9c3e4b3bu), change_endian_long(0xe51524dfu), change_endian_long(0xc6dc25e8u), change_endian_long(0xe7811c2cu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x5ecfc63bu), change_endian_long(0x00339581u), change_endian_long(0x4063e6f4u), change_endian_long(0xa5f5d2afu));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x03100100u);
                RX72M_RX72N_RX66N_func103();
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xe4130e07u), change_endian_long(0x02471f74u), change_endian_long(0x9c0606c7u), change_endian_long(0x08820e93u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xa81c8a78u), change_endian_long(0xf89d7bb6u), change_endian_long(0x4c4a94a6u), change_endian_long(0x143c6d85u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x30132199u), change_endian_long(0xbfb2dc0bu), change_endian_long(0x094fe4f6u), change_endian_long(0xa7b9e26cu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x9efae348u), change_endian_long(0xf75696e3u), change_endian_long(0x1c035debu), change_endian_long(0x50b993f3u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00800100u);
                RX72M_RX72N_RX66N_func103();
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x95a61a77u), change_endian_long(0x10bd65d8u), change_endian_long(0x0f6b665au), change_endian_long(0xf507d1d6u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xf2be9fdbu), change_endian_long(0x1ef03a4cu), change_endian_long(0x0c2dbc02u), change_endian_long(0x6ede9924u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xae333f4fu), change_endian_long(0xce65bd81u), change_endian_long(0x4d7cd70eu), change_endian_long(0xb59103a5u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x567733afu), change_endian_long(0x0ec75802u), change_endian_long(0xd087e7e9u), change_endian_long(0x6c5fe47bu));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00400100u);
                RX72M_RX72N_RX66N_func103();
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x1a553402u), change_endian_long(0x48323a46u), change_endian_long(0xf8e9bbf7u), change_endian_long(0x87bb211eu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x08c2745cu), change_endian_long(0xcb152d4au), change_endian_long(0x300bc578u), change_endian_long(0xcea598d6u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xcbcdee19u), change_endian_long(0x7acb89c5u), change_endian_long(0x9a30186au), change_endian_long(0xd9b2bf4bu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xb4e95c94u), change_endian_long(0x68551fd4u), change_endian_long(0xb6ce1ae9u), change_endian_long(0x95bb2214u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x41b99e44u), change_endian_long(0x27a8f4a4u), change_endian_long(0x5c5865f6u), change_endian_long(0xe9e6f237u));
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0xa29dc235u), change_endian_long(0xd86ff1efu), change_endian_long(0xb779d2b7u), change_endian_long(0x251d87f3u));
            }
            else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX72M_RX72N_RX66N_func100(change_endian_long(0xa33acc34u), change_endian_long(0x074c4bc1u), change_endian_long(0x8ecb4e39u), change_endian_long(0x8941d85eu));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x03200100u);
                RX72M_RX72N_RX66N_func103();
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x8e6b822au), change_endian_long(0xa4d22fadu), change_endian_long(0x02bdd349u), change_endian_long(0x470aa035u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x455a2c5bu), change_endian_long(0x97944c5cu), change_endian_long(0x8e9e2f92u), change_endian_long(0xe9bd6f70u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xda2b0986u), change_endian_long(0xdec9d638u), change_endian_long(0xa6eb3f7bu), change_endian_long(0xd0b30ffbu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x12ba34bdu), change_endian_long(0xe99eefe7u), change_endian_long(0xce3f0d66u), change_endian_long(0xdc2958a9u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x03100100u);
                RX72M_RX72N_RX66N_func103();
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x104e2892u), change_endian_long(0x2134f456u), change_endian_long(0xf4b68bb5u), change_endian_long(0xb1f6a446u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x21a59268u), change_endian_long(0x28e58915u), change_endian_long(0x20e8d0e7u), change_endian_long(0xf78c13afu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xe38a8ca5u), change_endian_long(0xfab94f51u), change_endian_long(0x1d374061u), change_endian_long(0xdaef9f2au));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xe2c587e5u), change_endian_long(0xb555a3d4u), change_endian_long(0x177b2a15u), change_endian_long(0xc7b59d40u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x01800100u);
                RX72M_RX72N_RX66N_func103();
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xdd8f00d6u), change_endian_long(0x1d1952ccu), change_endian_long(0xb4ab9db4u), change_endian_long(0x6a1f6a83u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x0e367c2fu), change_endian_long(0xcd476905u), change_endian_long(0x8936b297u), change_endian_long(0xa0246614u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xbfb49a49u), change_endian_long(0x6da5e3a6u), change_endian_long(0xe5099b93u), change_endian_long(0x58cbc732u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xd5164db6u), change_endian_long(0x0007c3bfu), change_endian_long(0x137fba19u), change_endian_long(0xe97729a8u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x01400100u);
                RX72M_RX72N_RX66N_func103();
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x21138c60u), change_endian_long(0x0d32b479u), change_endian_long(0x4233e441u), change_endian_long(0x76f8561du));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x1f85ee39u), change_endian_long(0x1baea062u), change_endian_long(0x2c7dc257u), change_endian_long(0x0e63d850u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x5067c3bbu), change_endian_long(0x15a9193du), change_endian_long(0x2d69aeacu), change_endian_long(0xc3a4defau));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x53663a3bu), change_endian_long(0x79f794d1u), change_endian_long(0xffb9428eu), change_endian_long(0x26bf0617u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xe4ed5cb4u), change_endian_long(0xabfa6b09u), change_endian_long(0xe5bfeefau), change_endian_long(0x712dc222u));
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0x11e6834fu), change_endian_long(0xc8a82180u), change_endian_long(0x28202afeu), change_endian_long(0xddff4e19u));
            }
            else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX72M_RX72N_RX66N_func100(change_endian_long(0xa416c10au), change_endian_long(0x0e4da961u), change_endian_long(0xe9f9c67eu), change_endian_long(0xbeb7d236u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x02200100u);
                RX72M_RX72N_RX66N_func103();
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x1fc22d75u), change_endian_long(0x8200d994u), change_endian_long(0x59bd32dbu), change_endian_long(0x0cdc2880u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xdca5812eu), change_endian_long(0x6cecf21bu), change_endian_long(0x9bc77481u), change_endian_long(0x906cb0d6u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x6704a934u), change_endian_long(0x5584ee3du), change_endian_long(0x1d2d61c5u), change_endian_long(0x5cb23c87u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x119fa39bu), change_endian_long(0x5cc87e42u), change_endian_long(0xe59f968au), change_endian_long(0xe877f241u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x02100100u);
                RX72M_RX72N_RX66N_func103();
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x8199b0e3u), change_endian_long(0x27c59c89u), change_endian_long(0x80e074f4u), change_endian_long(0x9c511ec7u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xcdd42e75u), change_endian_long(0xf119d8dfu), change_endian_long(0xd7325a7au), change_endian_long(0x9534d1eeu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xf4c60a25u), change_endian_long(0xcf04e51bu), change_endian_long(0xc8d4912du), change_endian_long(0x44ef1448u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x1a415e95u), change_endian_long(0x5bcb0861u), change_endian_long(0xe79b5ea0u), change_endian_long(0xffce2afeu));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00800100u);
                RX72M_RX72N_RX66N_func103();
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xeacd64ccu), change_endian_long(0xde400cd2u), change_endian_long(0xa6f0d989u), change_endian_long(0xf628957eu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x4cde545cu), change_endian_long(0x3625a0f5u), change_endian_long(0x810ac7fau), change_endian_long(0x4b629231u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xc47e87feu), change_endian_long(0x6c119e41u), change_endian_long(0xfcc3fca5u), change_endian_long(0x556a8ea5u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x863d512cu), change_endian_long(0x8736411eu), change_endian_long(0x1a584d7du), change_endian_long(0xb6ed5fb4u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00400100u);
                RX72M_RX72N_RX66N_func103();
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xf1a2c8cdu), change_endian_long(0xf0a65596u), change_endian_long(0x920dfdfeu), change_endian_long(0x842e571eu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x6c20643au), change_endian_long(0x75aeb665u), change_endian_long(0xe8af2f3du), change_endian_long(0x95d6e819u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x35b4ecaeu), change_endian_long(0x5fed3a88u), change_endian_long(0xec3fad1cu), change_endian_long(0x5d14bc32u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x69d8bf93u), change_endian_long(0x0f499b96u), change_endian_long(0x8ae2efeau), change_endian_long(0xc8fd9a51u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x2b851770u), change_endian_long(0xfd6848f9u), change_endian_long(0xcba58d80u), change_endian_long(0x4c887958u));
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0x95174b2au), change_endian_long(0xfbc5fe9eu), change_endian_long(0x3f3fa3b4u), change_endian_long(0x98b01e47u));
            }
            else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX72M_RX72N_RX66N_func100(change_endian_long(0x6ff9452bu), change_endian_long(0x7a2649bbu), change_endian_long(0x3e205fa4u), change_endian_long(0xed45d4d7u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x02200100u);
                RX72M_RX72N_RX66N_func103();
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x3d47dd2bu), change_endian_long(0xa04297fau), change_endian_long(0xa2c09dbfu), change_endian_long(0x7a14ade9u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xd95533d9u), change_endian_long(0x76029f11u), change_endian_long(0xf1ecb770u), change_endian_long(0x77eaef46u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x7c1a8e11u), change_endian_long(0xd15c8d3cu), change_endian_long(0x4a9a98bbu), change_endian_long(0x0b99300eu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xeaee4875u), change_endian_long(0x368aceccu), change_endian_long(0xe42d8c38u), change_endian_long(0x70ee17b5u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x02100100u);
                RX72M_RX72N_RX66N_func103();
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x7a8782e9u), change_endian_long(0xd2872bdeu), change_endian_long(0xd3ca1380u), change_endian_long(0x5c6c93c7u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x06f7068au), change_endian_long(0xf2c652cfu), change_endian_long(0x9e2691f0u), change_endian_long(0x68fa044au));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x864af479u), change_endian_long(0x1677d9bbu), change_endian_long(0xcf7c1791u), change_endian_long(0xed951b11u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x610c7cb5u), change_endian_long(0x2045526au), change_endian_long(0x1cbfc903u), change_endian_long(0x1e776003u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x01800100u);
                RX72M_RX72N_RX66N_func103();
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x4f485616u), change_endian_long(0xba6b7718u), change_endian_long(0xa485c27cu), change_endian_long(0x57cbf2ddu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xc72fe65eu), change_endian_long(0x66deb3f3u), change_endian_long(0xa93a7051u), change_endian_long(0x218589c0u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xc4f42303u), change_endian_long(0x2f3fc4f5u), change_endian_long(0xd2ceb728u), change_endian_long(0xe1992aa9u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x7efa320cu), change_endian_long(0xecf8df8cu), change_endian_long(0x1b939e2du), change_endian_long(0x8b8dbf25u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x01400100u);
                RX72M_RX72N_RX66N_func103();
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x2b9b1854u), change_endian_long(0xb8c21d6du), change_endian_long(0xc7a5bf8fu), change_endian_long(0xb1d22f96u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xd36118e8u), change_endian_long(0xee626e47u), change_endian_long(0x7fbd19beu), change_endian_long(0xce66bfb2u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xf3dea2f8u), change_endian_long(0x5388148eu), change_endian_long(0x9931405bu), change_endian_long(0xb0a4e4edu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x6b946af9u), change_endian_long(0xf54ec0beu), change_endian_long(0x58bdfd8eu), change_endian_long(0x6b1fd692u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xfb5ab297u), change_endian_long(0x86be40b0u), change_endian_long(0x30ea4b5du), change_endian_long(0xfe2733ccu));
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0xc575a13du), change_endian_long(0x43bfb3edu), change_endian_long(0x12206abbu), change_endian_long(0xe3e088a5u));
            }
            else if (0x06000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX72M_RX72N_RX66N_func100(change_endian_long(0xe2984e86u), change_endian_long(0x25dbf473u), change_endian_long(0x29e79d01u), change_endian_long(0xe3523a03u));
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
                RX72M_RX72N_RX66N_func103();
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x8bcd747bu), change_endian_long(0x9ed19731u), change_endian_long(0xb92551f5u), change_endian_long(0xe76b52beu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x2f4b5b70u), change_endian_long(0x5b699421u), change_endian_long(0x2a2b83f7u), change_endian_long(0x1b114e5fu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x2d365337u), change_endian_long(0x3c0b754bu), change_endian_long(0x38dfb15eu), change_endian_long(0x736031f8u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x7ac792cau), change_endian_long(0x80cab55fu), change_endian_long(0x1589d105u), change_endian_long(0x689845bbu));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x12400100u);
                RX72M_RX72N_RX66N_func103();
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x71e8ec53u), change_endian_long(0x8a52dd05u), change_endian_long(0x1277f4adu), change_endian_long(0xb6f8744bu));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xc7ac77adu), change_endian_long(0x9bd05b1fu), change_endian_long(0x146bae6au), change_endian_long(0x73544fd2u));
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0xf4dce6dcu), change_endian_long(0x50787e78u), change_endian_long(0x25615731u), change_endian_long(0x2c93d5edu));
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0x197c25c9u), change_endian_long(0x2caeaec1u), change_endian_long(0xf0e44015u), change_endian_long(0xe8699030u));
            }
            RX72M_RX72N_RX66N_func102(change_endian_long(0xb18e8488u), change_endian_long(0xec88e4dfu), change_endian_long(0x86b45ab8u), change_endian_long(0xe56974e4u));
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
 End of function ./input_dir/RX72M/RX72M_pe5_r1.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_TLS == 1 */
