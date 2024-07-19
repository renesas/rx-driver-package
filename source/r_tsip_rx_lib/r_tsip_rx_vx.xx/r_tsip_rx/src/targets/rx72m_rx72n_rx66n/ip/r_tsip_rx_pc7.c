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
* Function Name: R_TSIP_TlsSVGenerateSessionKeySub
*******************************************************************************************************************/ /**
* @details       RX72M TLS Keys Generation for Server
* @param[in]     InData_Sel_CipherSuite
* @param[in]     InData_MasterSecret
* @param[in]     InData_ClientRandom
* @param[in]     InData_ServerRandom
* @param[in]     InData_MAC
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
e_tsip_err_t R_TSIP_TlsSVGenerateSessionKeySub(uint32_t *InData_Sel_CipherSuite, uint32_t *InData_MasterSecret, uint32_t *InData_ClientRandom, uint32_t *InData_ServerRandom, uint32_t *InData_MAC, uint32_t *InData_NonceExplicit, uint32_t *OutData_ClientMACKeyOperationCode, uint32_t *OutData_ServerMACKeyOperationCode, uint32_t *OutData_ClientEncKeyOperationCode, uint32_t *OutData_ServerEncKeyOperationCode, uint32_t* OutData_ClientIV, uint32_t* OutData_ServerIV)
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
    TSIP.REG_84H.WORD = 0x0000c701u;
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
    TSIP.REG_28H.WORD = 0x00870001u;
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0x6e07db5au), change_endian_long(0x6fb1b74fu), change_endian_long(0x113339e6u), change_endian_long(0x1c048fefu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0x7ef417f7u), change_endian_long(0x9e3cd558u), change_endian_long(0x9adb6056u), change_endian_long(0xde807845u));
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
        TSIP.REG_C4H.WORD = 0x000c2b0cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x2f8b2ce0u);
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x00040804u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_104H.WORD = 0x00000762u;
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x02e0888fu;
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
        TSIP.REG_00H.WORD = 0x00003223u;
        TSIP.REG_2CH.WORD = 0x00000012u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000362u;
        TSIP.REG_C4H.WORD = 0x000007bdu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_MAC[0];
        TSIP.REG_100H.WORD = InData_MAC[1];
        TSIP.REG_100H.WORD = InData_MAC[2];
        TSIP.REG_100H.WORD = InData_MAC[3];
        TSIP.REG_C4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00002213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0x86f764b1u), change_endian_long(0x509fe25eu), change_endian_long(0x1661f584u), change_endian_long(0xeadf2099u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func102(change_endian_long(0x7109e214u), change_endian_long(0xc627aae4u), change_endian_long(0x9d9fcfcbu), change_endian_long(0xa2a1d329u));
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
            TSIP.REG_100H.WORD = change_endian_long(0x9c5b7bb2u);
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
            RX72M_RX72N_RX66N_func100(change_endian_long(0x9352810cu), change_endian_long(0x12f8a522u), change_endian_long(0x21d6c42cu), change_endian_long(0x469368aau));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX72M_RX72N_RX66N_func102(change_endian_long(0x05c66bacu), change_endian_long(0x309752d8u), change_endian_long(0xe06ebbfau), change_endian_long(0x80c58883u));
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
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x00008887u;
                TSIP.REG_00H.WORD = 0x00002323u;
                TSIP.REG_2CH.WORD = 0x00000022u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
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
                TSIP.REG_ECH.WORD = 0x00000060u;
                TSIP.REG_ECH.WORD = 0x0000b560u;
                TSIP.REG_ECH.WORD = 0x00000020u;
                TSIP.REG_ECH.WORD = 0x0000098cu;
                TSIP.REG_ECH.WORD = 0x0000b5c0u;
                TSIP.REG_ECH.WORD = 0x0000004du;
                RX72M_RX72N_RX66N_func101(change_endian_long(0x9176306du), change_endian_long(0xa3e404f2u), change_endian_long(0xdc74162fu), change_endian_long(0x201cb8f9u));
                RX72M_RX72N_RX66N_func032();
                TSIP.REG_ECH.WORD = 0x00002c20u;
                TSIP.REG_ECH.WORD = 0x0000096bu;
                TSIP.REG_ECH.WORD = 0x0000b580u;
                TSIP.REG_ECH.WORD = 0x000000a0u;
                TSIP.REG_ECH.WORD = 0x0000b5c0u;
                TSIP.REG_ECH.WORD = 0x0000006du;
                RX72M_RX72N_RX66N_func101(change_endian_long(0xcea26885u), change_endian_long(0x63c2cccfu), change_endian_long(0x8b1e39eeu), change_endian_long(0xecb78b6eu));
                RX72M_RX72N_RX66N_func032();
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0xd87ed954u), change_endian_long(0x1bdc2b8eu), change_endian_long(0x0067f571u), change_endian_long(0x0a0e9b58u));
                RX72M_RX72N_RX66N_func032();
                TSIP.REG_ECH.WORD = 0x00002c20u;
                TSIP.REG_ECH.WORD = 0x0000096bu;
                TSIP.REG_ECH.WORD = 0x0000b580u;
                TSIP.REG_ECH.WORD = 0x000000a0u;
                TSIP.REG_ECH.WORD = 0x0000b5c0u;
                TSIP.REG_ECH.WORD = 0x0000006du;
                RX72M_RX72N_RX66N_func101(change_endian_long(0xab72671cu), change_endian_long(0xe6f41f1eu), change_endian_long(0x813c9f4au), change_endian_long(0x6af1e2f3u));
                RX72M_RX72N_RX66N_func032();
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0x38b896feu), change_endian_long(0x872e22d0u), change_endian_long(0x42cb9042u), change_endian_long(0xbb1cc8d6u));
                RX72M_RX72N_RX66N_func032();
                TSIP.REG_ECH.WORD = 0x00002c20u;
                TSIP.REG_ECH.WORD = 0x0000096bu;
                TSIP.REG_ECH.WORD = 0x0000b580u;
                TSIP.REG_ECH.WORD = 0x000000a0u;
                TSIP.REG_ECH.WORD = 0x0000b5c0u;
                TSIP.REG_ECH.WORD = 0x0000006du;
                RX72M_RX72N_RX66N_func101(change_endian_long(0x409c1792u), change_endian_long(0xa9b460aau), change_endian_long(0x2ca602c5u), change_endian_long(0x9bc459e7u));
                RX72M_RX72N_RX66N_func032();
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0xaabbcd64u), change_endian_long(0xa7631f51u), change_endian_long(0x6055561du), change_endian_long(0xb06cee0cu));
                RX72M_RX72N_RX66N_func032();
                TSIP.REG_ECH.WORD = 0x00002c20u;
                TSIP.REG_ECH.WORD = 0x0000096bu;
                TSIP.REG_ECH.WORD = 0x0000b580u;
                TSIP.REG_ECH.WORD = 0x000000a0u;
                TSIP.REG_ECH.WORD = 0x0000b5c0u;
                TSIP.REG_ECH.WORD = 0x0000006du;
                RX72M_RX72N_RX66N_func101(change_endian_long(0x35ce50b0u), change_endian_long(0x935faf58u), change_endian_long(0x486cb1bcu), change_endian_long(0x4a6df238u));
                RX72M_RX72N_RX66N_func032();
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
                RX72M_RX72N_RX66N_func101(change_endian_long(0x4e44ea34u), change_endian_long(0xe18adc4bu), change_endian_long(0x29346b7eu), change_endian_long(0x018a5d3eu));
                RX72M_RX72N_RX66N_func032();
                TSIP.REG_ECH.WORD = 0x00002c20u;
                TSIP.REG_ECH.WORD = 0x0000096bu;
                TSIP.REG_ECH.WORD = 0x0000b580u;
                TSIP.REG_ECH.WORD = 0x000000a0u;
                TSIP.REG_ECH.WORD = 0x0000b5c0u;
                TSIP.REG_ECH.WORD = 0x0000006du;
                RX72M_RX72N_RX66N_func101(change_endian_long(0x1cd92af7u), change_endian_long(0x932397d1u), change_endian_long(0x70f5aba8u), change_endian_long(0xcaf73056u));
                RX72M_RX72N_RX66N_func032();
                RX72M_RX72N_RX66N_func100(change_endian_long(0x622c5a6du), change_endian_long(0x96a3036du), change_endian_long(0xe5ca714eu), change_endian_long(0x858caddau));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x78500afcu), change_endian_long(0x91211788u), change_endian_long(0x52444537u), change_endian_long(0xe75f9755u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x83100100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xc70d79beu), change_endian_long(0x0e9e5f9fu), change_endian_long(0xcb2ec3cau), change_endian_long(0xd704d8f9u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xb5df479fu), change_endian_long(0xa63fe07bu), change_endian_long(0xd558e26fu), change_endian_long(0x00651cc2u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xa2bbdecfu), change_endian_long(0xcad63270u), change_endian_long(0xe213c196u), change_endian_long(0xb67d6f4cu));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xee0ed66fu), change_endian_long(0x092cc934u), change_endian_long(0x1f61593bu), change_endian_long(0x4b109d30u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x83200100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x63cf2382u), change_endian_long(0xe04f16ffu), change_endian_long(0xe117860du), change_endian_long(0x5fd84bd7u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x541cfbb4u), change_endian_long(0x93c26be6u), change_endian_long(0x9481da35u), change_endian_long(0x8544eb4du));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xee402731u), change_endian_long(0x6a0f7cc4u), change_endian_long(0xda9bebdau), change_endian_long(0xb6efc08eu));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x69208e52u), change_endian_long(0x98ff18ebu), change_endian_long(0x2700c75du), change_endian_long(0x3f18cc2cu));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x80400100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xba438dbau), change_endian_long(0x454ea58eu), change_endian_long(0x9b4698c1u), change_endian_long(0xfee4c6abu));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x06da83cbu), change_endian_long(0xd83a83d8u), change_endian_long(0x8eff2797u), change_endian_long(0x8eac6a02u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x3b5f5030u), change_endian_long(0xd00888feu), change_endian_long(0xb25d4c9fu), change_endian_long(0xfa52c9d5u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x81cf07c6u), change_endian_long(0x9ca01ab4u), change_endian_long(0x33704eb8u), change_endian_long(0x68543c95u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x80800100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x8450a8a8u), change_endian_long(0x22b9a89eu), change_endian_long(0x3d86f317u), change_endian_long(0x37e27df0u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xe830ae7fu), change_endian_long(0x83946a9eu), change_endian_long(0x4e559404u), change_endian_long(0xeadf2b10u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x75f1ec8au), change_endian_long(0xe52472f9u), change_endian_long(0x9016bb72u), change_endian_long(0x45c2c8efu));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x80ad1226u), change_endian_long(0xac9da27du), change_endian_long(0x5ee4afcfu), change_endian_long(0x92924517u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xc9cbb616u), change_endian_long(0xba9b4403u), change_endian_long(0x6065f857u), change_endian_long(0xa40d736fu));
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
                    RX72M_RX72N_RX66N_func101(change_endian_long(0xd518f3edu), change_endian_long(0xd07b9220u), change_endian_long(0xfb7fb843u), change_endian_long(0xd8222c40u));
                }
                else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xbd354514u), change_endian_long(0x49947293u), change_endian_long(0xb17bc740u), change_endian_long(0x0760070du));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x83100100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xb9ce6e3eu), change_endian_long(0x63ee91e3u), change_endian_long(0x74423065u), change_endian_long(0x3ba26397u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xe45b4358u), change_endian_long(0x49eca7beu), change_endian_long(0x3dce42f9u), change_endian_long(0x0838d287u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xd6829d24u), change_endian_long(0x983f1385u), change_endian_long(0x6e9f90deu), change_endian_long(0x1e18bc36u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xf792dc78u), change_endian_long(0x24fe57acu), change_endian_long(0xcf3bc10cu), change_endian_long(0x907837dcu));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x83200100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xe0fdcac8u), change_endian_long(0x0898e1d4u), change_endian_long(0x76dd3075u), change_endian_long(0xa2bac050u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xb9026d78u), change_endian_long(0xcdb423ceu), change_endian_long(0xab9b68c4u), change_endian_long(0xb0fafa89u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xd4e8b5fau), change_endian_long(0x42dd6337u), change_endian_long(0x70742aaau), change_endian_long(0x0bb02da0u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x3c130e45u), change_endian_long(0xe4c5ff98u), change_endian_long(0xb73e2e5eu), change_endian_long(0x575af73eu));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x81400100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xc7022f41u), change_endian_long(0xe5a3622fu), change_endian_long(0x213442d9u), change_endian_long(0x5233d79bu));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x73b89409u), change_endian_long(0x84d7c0f4u), change_endian_long(0x617986f9u), change_endian_long(0xd31f34d3u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xb618bbf1u), change_endian_long(0xdb90a194u), change_endian_long(0xf64d0a31u), change_endian_long(0x56523d4cu));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x9203d821u), change_endian_long(0xedbb8ff9u), change_endian_long(0x19169a67u), change_endian_long(0x8462cf39u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x81800100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x3a1f43dcu), change_endian_long(0x140d2c09u), change_endian_long(0x29323302u), change_endian_long(0x91885b20u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xc10d7e6au), change_endian_long(0xe6a87f0bu), change_endian_long(0x80388a96u), change_endian_long(0xf7a8f21fu));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x02b1780eu), change_endian_long(0xaa36aa92u), change_endian_long(0x8aa85988u), change_endian_long(0x4c4b9059u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x6187c557u), change_endian_long(0xde2dace7u), change_endian_long(0xfcb4f37cu), change_endian_long(0x60a4468fu));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xe06daf02u), change_endian_long(0x5fdbdf2du), change_endian_long(0x07a09253u), change_endian_long(0x4c23a7ceu));
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
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x57ec70aeu), change_endian_long(0xb7de4d1fu), change_endian_long(0x2683c363u), change_endian_long(0x293b87c9u));
                }
                else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x60f858b8u), change_endian_long(0xcb3d8e1au), change_endian_long(0x948c8f16u), change_endian_long(0x3252854cu));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x82100100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xecfb1619u), change_endian_long(0xeda5c542u), change_endian_long(0xa8244b72u), change_endian_long(0x43e9bf25u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x578ef2acu), change_endian_long(0x48212811u), change_endian_long(0x77334c1du), change_endian_long(0xe5f3de54u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x0186e2a4u), change_endian_long(0xad83d5b0u), change_endian_long(0x0b7f7e1fu), change_endian_long(0x98ab9f0au));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x93d92fcau), change_endian_long(0xf553bfb2u), change_endian_long(0x325bc4b3u), change_endian_long(0xfe20be93u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x82200100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x6699be69u), change_endian_long(0x0cca47fdu), change_endian_long(0xae5fff37u), change_endian_long(0x2f497a92u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x969f115au), change_endian_long(0x9c0500a3u), change_endian_long(0x513e1fc3u), change_endian_long(0xf8bde812u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x4f0e8255u), change_endian_long(0xdce9fe1cu), change_endian_long(0x4bfbbf8eu), change_endian_long(0x3d2eb790u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x2bbd2885u), change_endian_long(0x48bb2038u), change_endian_long(0xaa405838u), change_endian_long(0xce9ee641u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x80400100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xc7792bd3u), change_endian_long(0x96331a80u), change_endian_long(0x6643f126u), change_endian_long(0xe4607a28u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x2e4adc14u), change_endian_long(0xe20f3c18u), change_endian_long(0xb238a3d9u), change_endian_long(0xa43f2effu));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xffb543e6u), change_endian_long(0xa26acdeau), change_endian_long(0xa3664d28u), change_endian_long(0xc4b4680fu));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x83667296u), change_endian_long(0x1e98e377u), change_endian_long(0x0877e1e5u), change_endian_long(0x99bc709fu));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x80800100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xe7c01cfbu), change_endian_long(0xc7675979u), change_endian_long(0x9ed6ddb7u), change_endian_long(0x693af8adu));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x76a9cf3au), change_endian_long(0xb8f90412u), change_endian_long(0x37dbcff3u), change_endian_long(0x8f0f400du));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x056801feu), change_endian_long(0x823d7ca6u), change_endian_long(0xb18e80f8u), change_endian_long(0x1ba50e1fu));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x6292957fu), change_endian_long(0x26d77da1u), change_endian_long(0x4d9bd4bdu), change_endian_long(0x5ccdbbb7u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xb60a5f85u), change_endian_long(0x6d406400u), change_endian_long(0x86fd9bd7u), change_endian_long(0x1fe10320u));
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
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x8e23475au), change_endian_long(0xd949fccau), change_endian_long(0xa49d4362u), change_endian_long(0x7d386c8eu));
                }
                else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x15d9c176u), change_endian_long(0x93625d47u), change_endian_long(0x5de2c42bu), change_endian_long(0xbc55beb1u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x82100100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x7bfafd36u), change_endian_long(0xc1b2ea87u), change_endian_long(0xa32f5b69u), change_endian_long(0x124b3597u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x76cdbcf9u), change_endian_long(0xa81d742fu), change_endian_long(0xc90a31ddu), change_endian_long(0xaf4be9d0u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xd98631a3u), change_endian_long(0x3f066330u), change_endian_long(0xb4fd503eu), change_endian_long(0x5260eb79u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x04bd5352u), change_endian_long(0x3d79d72au), change_endian_long(0x1f3f96afu), change_endian_long(0x635a3396u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x82200100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xb5725a55u), change_endian_long(0x16c03721u), change_endian_long(0xe24006cfu), change_endian_long(0xa520869du));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xa5b558d0u), change_endian_long(0xd14592b4u), change_endian_long(0xe43d9d40u), change_endian_long(0x84e59291u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x76b84adcu), change_endian_long(0xdcab777bu), change_endian_long(0x84da107eu), change_endian_long(0xfb0567e8u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x22a203b6u), change_endian_long(0xdea68b7du), change_endian_long(0xa6410a4du), change_endian_long(0x3f12a664u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x81400100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x0021c3abu), change_endian_long(0xdb20df16u), change_endian_long(0x4daf53a3u), change_endian_long(0xe2728838u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xbb366fa0u), change_endian_long(0x7cbdd04cu), change_endian_long(0xd976b24cu), change_endian_long(0xb5efbf01u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x79895aa7u), change_endian_long(0x4d14a1b1u), change_endian_long(0xf52a8042u), change_endian_long(0xdac6b208u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x6492bb92u), change_endian_long(0xf07719b6u), change_endian_long(0x41a859eeu), change_endian_long(0xf973e366u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x81800100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xae3da265u), change_endian_long(0x6a458cdbu), change_endian_long(0x74ca6d84u), change_endian_long(0x574deb8cu));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x0d6ab3edu), change_endian_long(0x24fc7bb7u), change_endian_long(0x9a7547f8u), change_endian_long(0x3151d205u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x98c475d7u), change_endian_long(0x02ebc992u), change_endian_long(0x128b6886u), change_endian_long(0x6f22e27du));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xa891484fu), change_endian_long(0x26c54d88u), change_endian_long(0x2e83aab9u), change_endian_long(0x45e8e446u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x880715dbu), change_endian_long(0xaf7fc435u), change_endian_long(0xb2a55e4bu), change_endian_long(0xb17b3bf9u));
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
                    RX72M_RX72N_RX66N_func101(change_endian_long(0xba34423bu), change_endian_long(0xc113ab5bu), change_endian_long(0x181e5bd1u), change_endian_long(0x16394febu));
                }
                else if (0x06000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x386dff85u), change_endian_long(0xdb88d21fu), change_endian_long(0x5053adcfu), change_endian_long(0x1f7fd03eu));
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
                    TSIP.REG_100H.WORD = change_endian_long(0x92400100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x263b7126u), change_endian_long(0x8125b6fdu), change_endian_long(0x604074bau), change_endian_long(0x682f3fa4u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x55a9ea82u), change_endian_long(0x422629efu), change_endian_long(0xeea80edcu), change_endian_long(0xb8d66915u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x26028b47u), change_endian_long(0xeba667e6u), change_endian_long(0x6f2008efu), change_endian_long(0xc60adac4u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x8f63bb7eu), change_endian_long(0x40e44c10u), change_endian_long(0xed54421au), change_endian_long(0x820abb70u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x92800100u);
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x5c3ef9f9u), change_endian_long(0xf074b86au), change_endian_long(0x74a8dc79u), change_endian_long(0x47418387u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x454b4c13u), change_endian_long(0xb5dfa9cbu), change_endian_long(0x3a665ca0u), change_endian_long(0x0577d8b0u));
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x1b20c0f2u), change_endian_long(0x7f53ef54u), change_endian_long(0x121069ebu), change_endian_long(0x2d6062bbu));
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
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x8770394fu), change_endian_long(0xa5a3fe55u), change_endian_long(0x27ea3cd1u), change_endian_long(0x0913b93bu));
                }
                RX72M_RX72N_RX66N_func102(change_endian_long(0xf92d36e7u), change_endian_long(0x98328089u), change_endian_long(0x9e81f869u), change_endian_long(0x297a944du));
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
}
/**********************************************************************************************************************
 End of function ./input_dir/RX72M/RX72M_pc7.prc
 *********************************************************************************************************************/
