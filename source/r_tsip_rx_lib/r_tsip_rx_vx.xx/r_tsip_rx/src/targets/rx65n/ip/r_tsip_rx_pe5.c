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
* @details       RX65NHU TLS Keys Generation
* @param[in]     InData_Sel_CipherSuite
* @param[in]     InData_MasterSecret
* @param[in]     InData_ClientRandom
* @param[in]     InData_ServerRandom
* @param[in]     InData_NonceExplicit
* @param[out]    OutData_ClientMACKeyOperationCode
* @param[out]    OutData_ServerMACKeyOperationCode
* @param[out]    OutData_ClientEncKeyOperationCode
* @param[out]    OutData_ServerEncKeyOperationCode
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
    RX65NHU_func100(change_endian_long(0x04e022d9u), change_endian_long(0x537baa21u), change_endian_long(0x200b1afbu), change_endian_long(0xd807b35du));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0x1fe7df12u), change_endian_long(0xf0bffb12u), change_endian_long(0x45c429e4u), change_endian_long(0x8ab54d3au));
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
        RX65NHU_func100(change_endian_long(0x3d656d01u), change_endian_long(0x8ae39cfdu), change_endian_long(0x4bfe4689u), change_endian_long(0x2e925367u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func102(change_endian_long(0x9715bd61u), change_endian_long(0xf5147337u), change_endian_long(0x62d421aeu), change_endian_long(0x9d8d0741u));
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
            RX65NHU_func101(change_endian_long(0x68118b7bu), change_endian_long(0x95368b57u), change_endian_long(0x93e2b129u), change_endian_long(0xa4c5d65du));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX65NHU_func101(change_endian_long(0xc499e6deu), change_endian_long(0x0f358b59u), change_endian_long(0xcdfa1dd0u), change_endian_long(0xa5a15584u));
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
            RX65NHU_func101(change_endian_long(0x9049b4b2u), change_endian_long(0x51bc1eebu), change_endian_long(0xd7b5f581u), change_endian_long(0x4d529c19u));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX65NHU_func101(change_endian_long(0xec7891d7u), change_endian_long(0x2c42bc92u), change_endian_long(0x09eb242cu), change_endian_long(0x92cbd7e9u));
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
            RX65NHU_func101(change_endian_long(0xbb4afd96u), change_endian_long(0x9c3aef37u), change_endian_long(0x61a78e34u), change_endian_long(0xb455a772u));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX65NHU_func101(change_endian_long(0x0f311f79u), change_endian_long(0x71e38700u), change_endian_long(0x3e8e4c7bu), change_endian_long(0x13200aedu));
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
            RX65NHU_func101(change_endian_long(0xcf2e36f9u), change_endian_long(0xccf8a76au), change_endian_long(0x24593a86u), change_endian_long(0x5256efcbu));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX65NHU_func101(change_endian_long(0x128c2676u), change_endian_long(0x2823f378u), change_endian_long(0x4050096cu), change_endian_long(0x22e868a2u));
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
            RX65NHU_func101(change_endian_long(0xf3cb88e7u), change_endian_long(0x27daf4fdu), change_endian_long(0x8ca716b4u), change_endian_long(0x82f1f52eu));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX65NHU_func101(change_endian_long(0x72c4996eu), change_endian_long(0x93c22421u), change_endian_long(0xce0b0bcdu), change_endian_long(0x8554e80au));
            R_TSIP_TlsGenerateSubSub();

            RX65NHU_func100(change_endian_long(0x0598c659u), change_endian_long(0x38709334u), change_endian_long(0x33806a1cu), change_endian_long(0xbdf68733u));
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
                RX65NHU_func100(change_endian_long(0xd591fb39u), change_endian_long(0xd62f16f1u), change_endian_long(0x93ce050fu), change_endian_long(0xb0a238b9u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x03200100u);
                RX65NHU_func103();
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
                RX65NHU_func100(change_endian_long(0xd3c00ce1u), change_endian_long(0x40b7ee91u), change_endian_long(0xd5860d40u), change_endian_long(0x94a2c221u));
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
                RX65NHU_func100(change_endian_long(0xe52151f0u), change_endian_long(0x9c029808u), change_endian_long(0x07ce2aa2u), change_endian_long(0xa9b7e787u));
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
                RX65NHU_func100(change_endian_long(0x841a76e9u), change_endian_long(0x0e4a22a2u), change_endian_long(0x63745336u), change_endian_long(0x657f6d2eu));
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
                RX65NHU_func100(change_endian_long(0xd2adf555u), change_endian_long(0x56bf41a3u), change_endian_long(0xddb7b2e0u), change_endian_long(0xe5407e3du));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x03100100u);
                RX65NHU_func103();
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
                RX65NHU_func100(change_endian_long(0xeeb0268au), change_endian_long(0xa3c132b7u), change_endian_long(0xf522b1b3u), change_endian_long(0x96b90bd6u));
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
                RX65NHU_func100(change_endian_long(0x32be576bu), change_endian_long(0x59d028e3u), change_endian_long(0x53039882u), change_endian_long(0xac339ab7u));
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
                RX65NHU_func100(change_endian_long(0x7d58c6ebu), change_endian_long(0xb6c6eea6u), change_endian_long(0x3d2b6343u), change_endian_long(0x73f356c5u));
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
                RX65NHU_func100(change_endian_long(0xee297e6du), change_endian_long(0x1fcb2d67u), change_endian_long(0x14aa599au), change_endian_long(0xfb326ac4u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00800100u);
                RX65NHU_func103();
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
                RX65NHU_func100(change_endian_long(0xa95d7cd1u), change_endian_long(0x8f1c9dc2u), change_endian_long(0x18b10602u), change_endian_long(0xf0b0426cu));
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
                RX65NHU_func100(change_endian_long(0x71dfd76cu), change_endian_long(0x9240320bu), change_endian_long(0xc12d1d7du), change_endian_long(0xe1178379u));
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
                RX65NHU_func100(change_endian_long(0xf4cedab3u), change_endian_long(0xe8da3e38u), change_endian_long(0xe9606485u), change_endian_long(0x5314d30fu));
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
                RX65NHU_func100(change_endian_long(0x39c0c764u), change_endian_long(0x0ed7ab64u), change_endian_long(0x2686977fu), change_endian_long(0xebc7fc1au));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00400100u);
                RX65NHU_func103();
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
                RX65NHU_func100(change_endian_long(0x1a27514eu), change_endian_long(0x38b05d6au), change_endian_long(0xacc7d63eu), change_endian_long(0xa8fd2786u));
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
                RX65NHU_func100(change_endian_long(0xfc8a0c40u), change_endian_long(0xa4baf4f1u), change_endian_long(0xccb52683u), change_endian_long(0xf866a3aau));
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
                RX65NHU_func100(change_endian_long(0xb45ba2f9u), change_endian_long(0x5c98da4eu), change_endian_long(0x8f6e81ecu), change_endian_long(0xe6e5da3fu));
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
                RX65NHU_func100(change_endian_long(0xbacfe1cdu), change_endian_long(0x9face297u), change_endian_long(0x50fd76c6u), change_endian_long(0x65ce5176u));
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
                RX65NHU_func100(change_endian_long(0x00c3426du), change_endian_long(0x105d4709u), change_endian_long(0x360ac3f8u), change_endian_long(0xe19ab9c7u));
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
                RX65NHU_func101(change_endian_long(0x6f9eb1bdu), change_endian_long(0x9eb1bd7cu), change_endian_long(0xa3d05bedu), change_endian_long(0x373c6611u));
            }
            else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX65NHU_func100(change_endian_long(0x6e060331u), change_endian_long(0xd64b7dbcu), change_endian_long(0x057c42c4u), change_endian_long(0xa8f4f690u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x03200100u);
                RX65NHU_func103();
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
                RX65NHU_func100(change_endian_long(0xdfe8a094u), change_endian_long(0xd0aeff1cu), change_endian_long(0xb26b51ecu), change_endian_long(0x8dce2929u));
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
                RX65NHU_func100(change_endian_long(0xdd0eb135u), change_endian_long(0xdabba0beu), change_endian_long(0x61c9f0acu), change_endian_long(0x0228cd23u));
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
                RX65NHU_func100(change_endian_long(0xe87e6f3fu), change_endian_long(0xd0f64ca2u), change_endian_long(0x0c192b61u), change_endian_long(0x251c6d9eu));
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
                RX65NHU_func100(change_endian_long(0x24104fbcu), change_endian_long(0x4aeb7f92u), change_endian_long(0x084c1316u), change_endian_long(0x4455a49du));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x03100100u);
                RX65NHU_func103();
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
                RX65NHU_func100(change_endian_long(0x7f6d1558u), change_endian_long(0x3d4ab85au), change_endian_long(0xb819d4f8u), change_endian_long(0x809364beu));
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
                RX65NHU_func100(change_endian_long(0x7a26c4b0u), change_endian_long(0x3b82a112u), change_endian_long(0x8e5436b8u), change_endian_long(0x337689b7u));
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
                RX65NHU_func100(change_endian_long(0xeedc0cb9u), change_endian_long(0x61dac593u), change_endian_long(0xa7eedb7cu), change_endian_long(0x0097c765u));
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
                RX65NHU_func100(change_endian_long(0xaf6e03e8u), change_endian_long(0x0f9c9285u), change_endian_long(0x0825244bu), change_endian_long(0x52427dbbu));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x01800100u);
                RX65NHU_func103();
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
                RX65NHU_func100(change_endian_long(0x6ee9abd2u), change_endian_long(0xc956badau), change_endian_long(0x698687afu), change_endian_long(0x02ca3617u));
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
                RX65NHU_func100(change_endian_long(0xb8946442u), change_endian_long(0xac28411eu), change_endian_long(0x3557cb4fu), change_endian_long(0x35c72c55u));
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
                RX65NHU_func100(change_endian_long(0xbe006410u), change_endian_long(0xeca98e27u), change_endian_long(0x0efa5298u), change_endian_long(0xa9421d2eu));
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
                RX65NHU_func100(change_endian_long(0xad4883c4u), change_endian_long(0xf663d90au), change_endian_long(0xb738b8a4u), change_endian_long(0x5e5f27d6u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x01400100u);
                RX65NHU_func103();
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
                RX65NHU_func100(change_endian_long(0x96b0c03eu), change_endian_long(0xf5876496u), change_endian_long(0x7f33f33eu), change_endian_long(0xee16036fu));
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
                RX65NHU_func100(change_endian_long(0x81d045b8u), change_endian_long(0x4b38ea59u), change_endian_long(0xcd1a8559u), change_endian_long(0x949b0b72u));
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
                RX65NHU_func100(change_endian_long(0xc19ee6e8u), change_endian_long(0x91ba307du), change_endian_long(0xeecd9f3cu), change_endian_long(0x5cb541d6u));
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
                RX65NHU_func100(change_endian_long(0x1ecf16b3u), change_endian_long(0x3190087cu), change_endian_long(0xf1e9943au), change_endian_long(0xb01832a0u));
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
                RX65NHU_func100(change_endian_long(0x4111b11du), change_endian_long(0xa84c9c5fu), change_endian_long(0x2fa612fcu), change_endian_long(0xc44ebdb1u));
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
                RX65NHU_func101(change_endian_long(0x0ba59817u), change_endian_long(0xad2a841fu), change_endian_long(0x67ed0b82u), change_endian_long(0x79d40561u));
            }
            else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX65NHU_func100(change_endian_long(0x86dd363au), change_endian_long(0x3639316eu), change_endian_long(0x6d4998bau), change_endian_long(0xd6955b90u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x02200100u);
                RX65NHU_func103();
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
                RX65NHU_func100(change_endian_long(0xb667b23du), change_endian_long(0x2a9a7b73u), change_endian_long(0x913ff396u), change_endian_long(0x5cbf46c9u));
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
                RX65NHU_func100(change_endian_long(0x5089bbaeu), change_endian_long(0xa5305f61u), change_endian_long(0x245fef02u), change_endian_long(0x62a044b1u));
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
                RX65NHU_func100(change_endian_long(0x3cd62939u), change_endian_long(0x6b79fb6au), change_endian_long(0x83972942u), change_endian_long(0x6a031306u));
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
                RX65NHU_func100(change_endian_long(0x2edab168u), change_endian_long(0xd2ffd3ceu), change_endian_long(0xc3ad3f63u), change_endian_long(0xe0aaab55u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x02100100u);
                RX65NHU_func103();
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
                RX65NHU_func100(change_endian_long(0x21b696f7u), change_endian_long(0x8542d275u), change_endian_long(0x614da13fu), change_endian_long(0x2ee9255fu));
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
                RX65NHU_func100(change_endian_long(0xadf57b1cu), change_endian_long(0xc51c4b24u), change_endian_long(0x22ba1265u), change_endian_long(0x07edf3e3u));
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
                RX65NHU_func100(change_endian_long(0x7788ef45u), change_endian_long(0xa75febddu), change_endian_long(0x93dcca27u), change_endian_long(0xb7f39c89u));
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
                RX65NHU_func100(change_endian_long(0x736fc10eu), change_endian_long(0xd470d9fbu), change_endian_long(0x88c13017u), change_endian_long(0xe816179du));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00800100u);
                RX65NHU_func103();
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
                RX65NHU_func100(change_endian_long(0xc052e86cu), change_endian_long(0xc8e1d067u), change_endian_long(0x65694a18u), change_endian_long(0x0a12d700u));
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
                RX65NHU_func100(change_endian_long(0x14602c22u), change_endian_long(0x8a831c22u), change_endian_long(0xde7c2e12u), change_endian_long(0xe7b7570eu));
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
                RX65NHU_func100(change_endian_long(0xec83e9f6u), change_endian_long(0x389cffedu), change_endian_long(0xd5ae3c8bu), change_endian_long(0x1d6bd007u));
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
                RX65NHU_func100(change_endian_long(0x81c1d50cu), change_endian_long(0x02f19923u), change_endian_long(0x34392585u), change_endian_long(0x8e79ec42u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00400100u);
                RX65NHU_func103();
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
                RX65NHU_func100(change_endian_long(0xf6c0a6d6u), change_endian_long(0xa7413587u), change_endian_long(0xab6eed89u), change_endian_long(0x7c1f7e36u));
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
                RX65NHU_func100(change_endian_long(0xf6fe3456u), change_endian_long(0x7fa82ab6u), change_endian_long(0x94982cdbu), change_endian_long(0x9c65d994u));
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
                RX65NHU_func100(change_endian_long(0x78b9c1b2u), change_endian_long(0x7b1284f7u), change_endian_long(0x440d08b2u), change_endian_long(0x6e35126eu));
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
                RX65NHU_func100(change_endian_long(0x296dfe5cu), change_endian_long(0x6130d10au), change_endian_long(0xf50d1fceu), change_endian_long(0x7dfc793fu));
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
                RX65NHU_func100(change_endian_long(0x27e131edu), change_endian_long(0xf1157e5fu), change_endian_long(0x4a056139u), change_endian_long(0x0a766076u));
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
                RX65NHU_func101(change_endian_long(0xba0bd392u), change_endian_long(0xf4d3dcfcu), change_endian_long(0x19b5b975u), change_endian_long(0xb35a7792u));
            }
            else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX65NHU_func100(change_endian_long(0x040652ceu), change_endian_long(0x4f1d103fu), change_endian_long(0xa6b915fdu), change_endian_long(0x1cc443a8u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x02200100u);
                RX65NHU_func103();
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
                RX65NHU_func100(change_endian_long(0x6c152c35u), change_endian_long(0xbb1bfdc8u), change_endian_long(0xa717b2adu), change_endian_long(0x3d662077u));
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
                RX65NHU_func100(change_endian_long(0xb9475a18u), change_endian_long(0x252f1ee1u), change_endian_long(0x5bf6db03u), change_endian_long(0xfaf145f1u));
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
                RX65NHU_func100(change_endian_long(0x7ba2d64du), change_endian_long(0xdce3562au), change_endian_long(0xbb886204u), change_endian_long(0xb10d6e82u));
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
                RX65NHU_func100(change_endian_long(0xdb34b4b6u), change_endian_long(0xd7097f2bu), change_endian_long(0xe96e0a4du), change_endian_long(0xc33ad141u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x02100100u);
                RX65NHU_func103();
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
                RX65NHU_func100(change_endian_long(0x4f95bd98u), change_endian_long(0x78f3ffafu), change_endian_long(0x0fcffd68u), change_endian_long(0x83010c73u));
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
                RX65NHU_func100(change_endian_long(0xa3b78cfau), change_endian_long(0x4da4a096u), change_endian_long(0x623b79d6u), change_endian_long(0x43869d14u));
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
                RX65NHU_func100(change_endian_long(0x31b33400u), change_endian_long(0x9253b82eu), change_endian_long(0x7155b380u), change_endian_long(0x867ae5b0u));
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
                RX65NHU_func100(change_endian_long(0x637cd23eu), change_endian_long(0xfb105f8cu), change_endian_long(0x995bf9d6u), change_endian_long(0x9a17f537u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x01800100u);
                RX65NHU_func103();
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
                RX65NHU_func100(change_endian_long(0x236d6a69u), change_endian_long(0x8ae3facfu), change_endian_long(0x125d778fu), change_endian_long(0x5678aadbu));
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
                RX65NHU_func100(change_endian_long(0x3fc574f4u), change_endian_long(0x525983beu), change_endian_long(0x2aeb2f79u), change_endian_long(0xd17cc83fu));
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
                RX65NHU_func100(change_endian_long(0x5f37589au), change_endian_long(0x2452f8c9u), change_endian_long(0xf1a4b64fu), change_endian_long(0xd3608586u));
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
                RX65NHU_func100(change_endian_long(0x4fb9f39fu), change_endian_long(0x5d9d2f82u), change_endian_long(0xa6fef5a9u), change_endian_long(0x72431853u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x01400100u);
                RX65NHU_func103();
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
                RX65NHU_func100(change_endian_long(0xc7dcdae2u), change_endian_long(0x179e769bu), change_endian_long(0x16bf428fu), change_endian_long(0x92e99f6au));
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
                RX65NHU_func100(change_endian_long(0x0bf718a2u), change_endian_long(0x35637843u), change_endian_long(0xb348a806u), change_endian_long(0x03ce8e0fu));
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
                RX65NHU_func100(change_endian_long(0x9ce49014u), change_endian_long(0x27416daau), change_endian_long(0x7d0b8d10u), change_endian_long(0x3c4a16f0u));
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
                RX65NHU_func100(change_endian_long(0x81079257u), change_endian_long(0x901192e9u), change_endian_long(0x693e17efu), change_endian_long(0x7f7c85e7u));
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
                RX65NHU_func100(change_endian_long(0xa5914a05u), change_endian_long(0xb1856b7du), change_endian_long(0xd31938c9u), change_endian_long(0xc60189d0u));
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
                RX65NHU_func101(change_endian_long(0x0a33d9f8u), change_endian_long(0xb2ed1e83u), change_endian_long(0xedeae13fu), change_endian_long(0x648888acu));
            }
            else if (0x06000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
            {
                RX65NHU_func100(change_endian_long(0x78c16842u), change_endian_long(0x68aac487u), change_endian_long(0x4565b285u), change_endian_long(0x402443a5u));
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
                RX65NHU_func103();
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
                RX65NHU_func100(change_endian_long(0x133e6e5du), change_endian_long(0x9296f03eu), change_endian_long(0xac2ad213u), change_endian_long(0xae428cf8u));
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
                RX65NHU_func100(change_endian_long(0x21ddaba7u), change_endian_long(0x11576bb4u), change_endian_long(0x0a2d5336u), change_endian_long(0xe854653au));
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
                RX65NHU_func100(change_endian_long(0x47f9c0ffu), change_endian_long(0x7b263139u), change_endian_long(0xf09f357fu), change_endian_long(0x939c907au));
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
                RX65NHU_func100(change_endian_long(0x8cc38f2bu), change_endian_long(0xbd69f9d5u), change_endian_long(0xbaa9b90au), change_endian_long(0x23263a34u));
                TSIP.REG_104H.WORD = 0x00000058u;
                TSIP.REG_E0H.WORD = 0x80010040u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x12400100u);
                RX65NHU_func103();
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
                RX65NHU_func100(change_endian_long(0x4bd50dfbu), change_endian_long(0x318ae7fau), change_endian_long(0xa99a37d1u), change_endian_long(0x5f60dd54u));
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
                RX65NHU_func100(change_endian_long(0xf5e6bd70u), change_endian_long(0xc0f67e40u), change_endian_long(0x6be46cb2u), change_endian_long(0x293e663au));
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
                RX65NHU_func100(change_endian_long(0x9979b585u), change_endian_long(0x72523016u), change_endian_long(0x3b373be9u), change_endian_long(0x662c7c25u));
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
                RX65NHU_func101(change_endian_long(0xd0af43e3u), change_endian_long(0x72e8f13fu), change_endian_long(0x15d80b17u), change_endian_long(0x06d2de99u));
            }
            RX65NHU_func102(change_endian_long(0xadee0906u), change_endian_long(0xe6066c51u), change_endian_long(0x7ed49af7u), change_endian_long(0x5d82e922u));
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
 End of function ./input_dir/RX65NHU/RX65NHU_pe5_r3.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_TLS == 1 */
