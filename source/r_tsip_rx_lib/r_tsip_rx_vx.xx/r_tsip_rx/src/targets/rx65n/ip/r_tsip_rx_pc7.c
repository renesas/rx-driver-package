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
* @details       RX65NHU TLS Keys Generation for Server
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
    RX65NHU_func100(change_endian_long(0x3ebe3ceau), change_endian_long(0xed625415u), change_endian_long(0x2ce0a53cu), change_endian_long(0x145b3935u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0xb5d06e10u), change_endian_long(0xabc6153au), change_endian_long(0xe1806a04u), change_endian_long(0xb92fbf3eu));
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
        TSIP.REG_100H.WORD = change_endian_long(0xf042a442u);
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
        RX65NHU_func100(change_endian_long(0xff9fefe5u), change_endian_long(0x57e62775u), change_endian_long(0x361579c3u), change_endian_long(0x185fc21eu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func102(change_endian_long(0x116d273cu), change_endian_long(0xa32b5238u), change_endian_long(0xb9c64c3cu), change_endian_long(0x54e188a6u));
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
            TSIP.REG_100H.WORD = change_endian_long(0x54a104e7u);
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
            RX65NHU_func100(change_endian_long(0x1ded34b1u), change_endian_long(0xc5a926eeu), change_endian_long(0xa2720c4du), change_endian_long(0x63add570u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX65NHU_func102(change_endian_long(0x1a0551dau), change_endian_long(0x20e6febbu), change_endian_long(0xee024208u), change_endian_long(0x32e014b4u));
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
                RX65NHU_func101(change_endian_long(0xa4cdb9c9u), change_endian_long(0xd712cbb8u), change_endian_long(0xd741ce52u), change_endian_long(0x31b8a6e5u));
                RX65NHU_func032();
                TSIP.REG_ECH.WORD = 0x00002c20u;
                TSIP.REG_ECH.WORD = 0x0000096bu;
                TSIP.REG_ECH.WORD = 0x0000b580u;
                TSIP.REG_ECH.WORD = 0x000000a0u;
                TSIP.REG_ECH.WORD = 0x0000b5c0u;
                TSIP.REG_ECH.WORD = 0x0000006du;
                RX65NHU_func101(change_endian_long(0xc835d290u), change_endian_long(0xe322b437u), change_endian_long(0x1218fdb0u), change_endian_long(0x9e4c9cd8u));
                RX65NHU_func032();
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
                RX65NHU_func101(change_endian_long(0xcc8882b7u), change_endian_long(0xae07602du), change_endian_long(0x6df4d12eu), change_endian_long(0x828f1638u));
                RX65NHU_func032();
                TSIP.REG_ECH.WORD = 0x00002c20u;
                TSIP.REG_ECH.WORD = 0x0000096bu;
                TSIP.REG_ECH.WORD = 0x0000b580u;
                TSIP.REG_ECH.WORD = 0x000000a0u;
                TSIP.REG_ECH.WORD = 0x0000b5c0u;
                TSIP.REG_ECH.WORD = 0x0000006du;
                RX65NHU_func101(change_endian_long(0xc5e5100cu), change_endian_long(0xbf472ad4u), change_endian_long(0xa7c60940u), change_endian_long(0x6a38ba85u));
                RX65NHU_func032();
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
                RX65NHU_func101(change_endian_long(0xfa10a9c9u), change_endian_long(0x95fe23e9u), change_endian_long(0xaf129a7cu), change_endian_long(0x2dbc9606u));
                RX65NHU_func032();
                TSIP.REG_ECH.WORD = 0x00002c20u;
                TSIP.REG_ECH.WORD = 0x0000096bu;
                TSIP.REG_ECH.WORD = 0x0000b580u;
                TSIP.REG_ECH.WORD = 0x000000a0u;
                TSIP.REG_ECH.WORD = 0x0000b5c0u;
                TSIP.REG_ECH.WORD = 0x0000006du;
                RX65NHU_func101(change_endian_long(0xe73f2544u), change_endian_long(0x75fe0a5cu), change_endian_long(0xd641cfabu), change_endian_long(0xb5dde685u));
                RX65NHU_func032();
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
                RX65NHU_func101(change_endian_long(0x9eb55d84u), change_endian_long(0xf739f9b8u), change_endian_long(0x333714c6u), change_endian_long(0xbdc62f58u));
                RX65NHU_func032();
                TSIP.REG_ECH.WORD = 0x00002c20u;
                TSIP.REG_ECH.WORD = 0x0000096bu;
                TSIP.REG_ECH.WORD = 0x0000b580u;
                TSIP.REG_ECH.WORD = 0x000000a0u;
                TSIP.REG_ECH.WORD = 0x0000b5c0u;
                TSIP.REG_ECH.WORD = 0x0000006du;
                RX65NHU_func101(change_endian_long(0x0f126c4au), change_endian_long(0x4de7f6a8u), change_endian_long(0x3bb0e84bu), change_endian_long(0x4e502831u));
                RX65NHU_func032();
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
                RX65NHU_func101(change_endian_long(0xda93179cu), change_endian_long(0x11f9eea4u), change_endian_long(0x7918b512u), change_endian_long(0x1c2fa29cu));
                RX65NHU_func032();
                TSIP.REG_ECH.WORD = 0x00002c20u;
                TSIP.REG_ECH.WORD = 0x0000096bu;
                TSIP.REG_ECH.WORD = 0x0000b580u;
                TSIP.REG_ECH.WORD = 0x000000a0u;
                TSIP.REG_ECH.WORD = 0x0000b5c0u;
                TSIP.REG_ECH.WORD = 0x0000006du;
                RX65NHU_func101(change_endian_long(0x6615573bu), change_endian_long(0x8a89304eu), change_endian_long(0x59168c87u), change_endian_long(0xee882c42u));
                RX65NHU_func032();
                RX65NHU_func100(change_endian_long(0x1fb6e547u), change_endian_long(0xcaf4da68u), change_endian_long(0x5e5c06c6u), change_endian_long(0x13f43587u));
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
                    RX65NHU_func100(change_endian_long(0x55112049u), change_endian_long(0x34fe4aeau), change_endian_long(0xccb3fde7u), change_endian_long(0x02818b2fu));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x83100100u);
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
                    RX65NHU_func100(change_endian_long(0x4025148cu), change_endian_long(0x71768296u), change_endian_long(0x9723f607u), change_endian_long(0x79c6696du));
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
                    RX65NHU_func100(change_endian_long(0x447944d2u), change_endian_long(0xea9ba5c6u), change_endian_long(0x5b86f8e7u), change_endian_long(0xf616b99eu));
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
                    RX65NHU_func100(change_endian_long(0xd6385849u), change_endian_long(0x710e0588u), change_endian_long(0xf4b24661u), change_endian_long(0x7a539820u));
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
                    RX65NHU_func100(change_endian_long(0x78f9886bu), change_endian_long(0xacca6449u), change_endian_long(0x3996dba1u), change_endian_long(0x6748cd73u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x83200100u);
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
                    RX65NHU_func100(change_endian_long(0xee2ec4c2u), change_endian_long(0x777f69eeu), change_endian_long(0x602b2a35u), change_endian_long(0x31280ebcu));
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
                    RX65NHU_func100(change_endian_long(0x3d6fd62du), change_endian_long(0x02986621u), change_endian_long(0xfa4bbe3bu), change_endian_long(0x93601e70u));
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
                    RX65NHU_func100(change_endian_long(0xa53a1161u), change_endian_long(0x98c9cdb4u), change_endian_long(0x5f678335u), change_endian_long(0x1d2282cbu));
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
                    RX65NHU_func100(change_endian_long(0x0f47c69cu), change_endian_long(0xb5ef947du), change_endian_long(0x48431fe6u), change_endian_long(0xe9248e92u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x80400100u);
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
                    RX65NHU_func100(change_endian_long(0x671c1e4cu), change_endian_long(0x1db7e062u), change_endian_long(0xdab1aa92u), change_endian_long(0x5a0ddf9eu));
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
                    RX65NHU_func100(change_endian_long(0x894d0e8cu), change_endian_long(0xebaeb845u), change_endian_long(0xb24c9af0u), change_endian_long(0x6daca52au));
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
                    RX65NHU_func100(change_endian_long(0xf961b345u), change_endian_long(0xe9902ef4u), change_endian_long(0x2968c53cu), change_endian_long(0x854c4cd0u));
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
                    RX65NHU_func100(change_endian_long(0xca89c885u), change_endian_long(0xa2c4a9ccu), change_endian_long(0xe47959e7u), change_endian_long(0x3f3f4ca5u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x80800100u);
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
                    RX65NHU_func100(change_endian_long(0x6d291c1du), change_endian_long(0xd9407f6eu), change_endian_long(0x07d4edfbu), change_endian_long(0x2663f5e2u));
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
                    RX65NHU_func100(change_endian_long(0x45fdf281u), change_endian_long(0x1c5561edu), change_endian_long(0x72045150u), change_endian_long(0xbf44759au));
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
                    RX65NHU_func100(change_endian_long(0x2be74a7fu), change_endian_long(0xad520096u), change_endian_long(0x81f94911u), change_endian_long(0x63c12450u));
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
                    RX65NHU_func100(change_endian_long(0xda292e50u), change_endian_long(0x03e65d23u), change_endian_long(0x11dd1216u), change_endian_long(0x9f43b323u));
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
                    RX65NHU_func100(change_endian_long(0x68d78fc6u), change_endian_long(0xede2d2c1u), change_endian_long(0x84df6f0du), change_endian_long(0x7cfd9876u));
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
                    RX65NHU_func101(change_endian_long(0x27c07017u), change_endian_long(0xa14b374cu), change_endian_long(0xb89ac4e2u), change_endian_long(0x7ae87fb8u));
                }
                else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    RX65NHU_func100(change_endian_long(0x3e8ea45au), change_endian_long(0xe5dce322u), change_endian_long(0x081c7451u), change_endian_long(0x25593e95u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x83100100u);
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
                    RX65NHU_func100(change_endian_long(0x86092212u), change_endian_long(0xeeac22d5u), change_endian_long(0x568f3b82u), change_endian_long(0x7ed38cbeu));
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
                    RX65NHU_func100(change_endian_long(0x3c94bf79u), change_endian_long(0xe14b28eeu), change_endian_long(0xbdbbcb59u), change_endian_long(0x0d28f08eu));
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
                    RX65NHU_func100(change_endian_long(0x91a70935u), change_endian_long(0x5016b55cu), change_endian_long(0x5e02ee21u), change_endian_long(0x062e4c26u));
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
                    RX65NHU_func100(change_endian_long(0xe6c754c3u), change_endian_long(0x8dae3fdeu), change_endian_long(0x0661817fu), change_endian_long(0x1385890du));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x83200100u);
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
                    RX65NHU_func100(change_endian_long(0xf7b05219u), change_endian_long(0xdb797bcbu), change_endian_long(0x4b8bdb18u), change_endian_long(0xf6184d75u));
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
                    RX65NHU_func100(change_endian_long(0xf32f46fdu), change_endian_long(0x40532624u), change_endian_long(0xb43c9bc3u), change_endian_long(0x20e96d44u));
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
                    RX65NHU_func100(change_endian_long(0x2cda9065u), change_endian_long(0x24d01d43u), change_endian_long(0x20a4634du), change_endian_long(0x4eaca1aeu));
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
                    RX65NHU_func100(change_endian_long(0x02aaf516u), change_endian_long(0x661d151bu), change_endian_long(0xf7ac6ec2u), change_endian_long(0xa99302d5u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x81400100u);
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
                    RX65NHU_func100(change_endian_long(0x2c8eace2u), change_endian_long(0x75203feau), change_endian_long(0x84634bcau), change_endian_long(0xd56712adu));
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
                    RX65NHU_func100(change_endian_long(0x3a5159bfu), change_endian_long(0x84753193u), change_endian_long(0xc2c86114u), change_endian_long(0x44bcd8adu));
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
                    RX65NHU_func100(change_endian_long(0x0059629fu), change_endian_long(0x9be1325cu), change_endian_long(0x7d43bcd4u), change_endian_long(0x5a301e4cu));
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
                    RX65NHU_func100(change_endian_long(0x597d079eu), change_endian_long(0x8347128eu), change_endian_long(0x443534eau), change_endian_long(0xf36837cbu));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x81800100u);
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
                    RX65NHU_func100(change_endian_long(0x48641173u), change_endian_long(0xb9105eb2u), change_endian_long(0x1e14ec08u), change_endian_long(0x75228837u));
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
                    RX65NHU_func100(change_endian_long(0x3b4f8a18u), change_endian_long(0x8ee56b69u), change_endian_long(0x292e3f47u), change_endian_long(0xedbd0bfcu));
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
                    RX65NHU_func100(change_endian_long(0xb49e72e1u), change_endian_long(0xf3fdc32du), change_endian_long(0x5ad28367u), change_endian_long(0x1014314cu));
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
                    RX65NHU_func100(change_endian_long(0x64605689u), change_endian_long(0x2e228cf7u), change_endian_long(0x01d2bb75u), change_endian_long(0x2cbeec06u));
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
                    RX65NHU_func100(change_endian_long(0xc7106b7du), change_endian_long(0xbd1c427au), change_endian_long(0x92389784u), change_endian_long(0x3a68326bu));
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
                    RX65NHU_func101(change_endian_long(0x17958f25u), change_endian_long(0x9c7e301eu), change_endian_long(0xeea9e3f5u), change_endian_long(0xb57a4377u));
                }
                else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    RX65NHU_func100(change_endian_long(0x18dbc9afu), change_endian_long(0x9fe69bf6u), change_endian_long(0x8bbda681u), change_endian_long(0x25b52af7u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x82100100u);
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
                    RX65NHU_func100(change_endian_long(0xdb774f3fu), change_endian_long(0x1dbb8d1fu), change_endian_long(0x590a0702u), change_endian_long(0x3fc2788au));
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
                    RX65NHU_func100(change_endian_long(0x5c646a77u), change_endian_long(0xa3269c31u), change_endian_long(0x2aafe847u), change_endian_long(0x04f83373u));
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
                    RX65NHU_func100(change_endian_long(0x3b37d649u), change_endian_long(0x77d163bbu), change_endian_long(0x1b215162u), change_endian_long(0x91467f71u));
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
                    RX65NHU_func100(change_endian_long(0x80e56d4eu), change_endian_long(0x31ec313eu), change_endian_long(0xeff2ddafu), change_endian_long(0x903e06bau));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x82200100u);
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
                    RX65NHU_func100(change_endian_long(0x68b02b88u), change_endian_long(0xfe9687c2u), change_endian_long(0xffcdc413u), change_endian_long(0x7879e5c6u));
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
                    RX65NHU_func100(change_endian_long(0x7e374af2u), change_endian_long(0xc1574a0eu), change_endian_long(0x2f7432d9u), change_endian_long(0xb60f8986u));
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
                    RX65NHU_func100(change_endian_long(0x37ec7d1bu), change_endian_long(0xa6418a78u), change_endian_long(0xa4ab650bu), change_endian_long(0xefade77bu));
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
                    RX65NHU_func100(change_endian_long(0x2f5500cdu), change_endian_long(0x784e380au), change_endian_long(0x8b9e3650u), change_endian_long(0x8c0e6bb6u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x80400100u);
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
                    RX65NHU_func100(change_endian_long(0x3ead1386u), change_endian_long(0x7012e8bau), change_endian_long(0x805660f8u), change_endian_long(0xe439c4ccu));
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
                    RX65NHU_func100(change_endian_long(0x85d58932u), change_endian_long(0x9879887du), change_endian_long(0x84268244u), change_endian_long(0x92320c34u));
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
                    RX65NHU_func100(change_endian_long(0xc57a775fu), change_endian_long(0x56ebbdc1u), change_endian_long(0x74acede1u), change_endian_long(0xe8ecc781u));
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
                    RX65NHU_func100(change_endian_long(0xe05c0ce1u), change_endian_long(0x45860bd2u), change_endian_long(0x4262e4b0u), change_endian_long(0x4c74a6e3u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x80800100u);
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
                    RX65NHU_func100(change_endian_long(0x02f94721u), change_endian_long(0x68c3f593u), change_endian_long(0x5b7caf02u), change_endian_long(0xe9ddbdc8u));
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
                    RX65NHU_func100(change_endian_long(0x664d2996u), change_endian_long(0xa91312cdu), change_endian_long(0xc67bea88u), change_endian_long(0x6e89a323u));
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
                    RX65NHU_func100(change_endian_long(0x74a10ad1u), change_endian_long(0xdf76c7d0u), change_endian_long(0x4de5b180u), change_endian_long(0x6f7339bau));
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
                    RX65NHU_func100(change_endian_long(0x323dcbafu), change_endian_long(0xf3852e27u), change_endian_long(0xbe4d9f96u), change_endian_long(0x2c910f85u));
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
                    RX65NHU_func100(change_endian_long(0x20ed23ffu), change_endian_long(0xbe27f9e3u), change_endian_long(0x11969108u), change_endian_long(0x64e8720eu));
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
                    RX65NHU_func101(change_endian_long(0x7e76081cu), change_endian_long(0x0b1b5e63u), change_endian_long(0x6226cedau), change_endian_long(0xd6c391b1u));
                }
                else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    RX65NHU_func100(change_endian_long(0xf11afc0bu), change_endian_long(0x4a9012e7u), change_endian_long(0xd9c88068u), change_endian_long(0x04c3fde0u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x82100100u);
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
                    RX65NHU_func100(change_endian_long(0x53b5666fu), change_endian_long(0x8f910985u), change_endian_long(0x85760465u), change_endian_long(0xc49705c5u));
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
                    RX65NHU_func100(change_endian_long(0xb82847c4u), change_endian_long(0x1082be03u), change_endian_long(0xd3c9ff05u), change_endian_long(0x39a78e2du));
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
                    RX65NHU_func100(change_endian_long(0x0a75a1d2u), change_endian_long(0x9c522c8du), change_endian_long(0x0c44648bu), change_endian_long(0x4ea70b37u));
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
                    RX65NHU_func100(change_endian_long(0x7bbcafe5u), change_endian_long(0xc4e5eea6u), change_endian_long(0x6ec88ab9u), change_endian_long(0xce26ba8bu));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x82200100u);
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
                    RX65NHU_func100(change_endian_long(0x6eaaf1cdu), change_endian_long(0x2bba145eu), change_endian_long(0x9b070fbau), change_endian_long(0xd6b760f2u));
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
                    RX65NHU_func100(change_endian_long(0x679007d4u), change_endian_long(0x806561fdu), change_endian_long(0x31332144u), change_endian_long(0x1127ae6bu));
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
                    RX65NHU_func100(change_endian_long(0x564d7cb8u), change_endian_long(0x4e478f08u), change_endian_long(0xd30bee72u), change_endian_long(0xcc9f2a5fu));
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
                    RX65NHU_func100(change_endian_long(0xd7bca0cfu), change_endian_long(0xe9459cb7u), change_endian_long(0xaa94f41au), change_endian_long(0x2fbc2935u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x81400100u);
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
                    RX65NHU_func100(change_endian_long(0x856d814cu), change_endian_long(0x17104894u), change_endian_long(0x64c0daa8u), change_endian_long(0x1e62b154u));
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
                    RX65NHU_func100(change_endian_long(0x8bb8b2bcu), change_endian_long(0x386a2909u), change_endian_long(0xb5fffb9au), change_endian_long(0x5fb2fc6bu));
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
                    RX65NHU_func100(change_endian_long(0xf2a790bdu), change_endian_long(0xabc783d0u), change_endian_long(0x125e61cau), change_endian_long(0xa7247ba9u));
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
                    RX65NHU_func100(change_endian_long(0xc39c54b2u), change_endian_long(0xc7b2eec2u), change_endian_long(0x3f907cbdu), change_endian_long(0x744c90ceu));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x81800100u);
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
                    RX65NHU_func100(change_endian_long(0x1e360a3du), change_endian_long(0xb5d03cd4u), change_endian_long(0xef5d39bdu), change_endian_long(0x92de44c2u));
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
                    RX65NHU_func100(change_endian_long(0xfe449fe0u), change_endian_long(0x7b1a9caau), change_endian_long(0xfd7bde08u), change_endian_long(0x001f9f02u));
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
                    RX65NHU_func100(change_endian_long(0x51f49f6bu), change_endian_long(0x5678aa2bu), change_endian_long(0x16731809u), change_endian_long(0x3f4c1328u));
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
                    RX65NHU_func100(change_endian_long(0xd4053ed1u), change_endian_long(0x0c947058u), change_endian_long(0x3c2dba66u), change_endian_long(0x927e2b0du));
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
                    RX65NHU_func100(change_endian_long(0xa494ed71u), change_endian_long(0x800f6c36u), change_endian_long(0x74c86373u), change_endian_long(0x69beb72eu));
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
                    RX65NHU_func101(change_endian_long(0x67bc4618u), change_endian_long(0x8b46689du), change_endian_long(0xdba29e1au), change_endian_long(0xad0a6b7eu));
                }
                else if (0x06000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    RX65NHU_func100(change_endian_long(0x0498b309u), change_endian_long(0x814fe3e8u), change_endian_long(0x419c82e6u), change_endian_long(0x9369251du));
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
                    RX65NHU_func100(change_endian_long(0xc64d25e2u), change_endian_long(0x67328000u), change_endian_long(0x0fd3ec01u), change_endian_long(0xc091693au));
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
                    RX65NHU_func100(change_endian_long(0xe97f5807u), change_endian_long(0xbb74951du), change_endian_long(0x22c11f21u), change_endian_long(0x2e235954u));
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
                    RX65NHU_func100(change_endian_long(0xec8c0683u), change_endian_long(0xa8cbc6d0u), change_endian_long(0xfdbeba2cu), change_endian_long(0x59cda21du));
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
                    RX65NHU_func100(change_endian_long(0x0f190c42u), change_endian_long(0xc634b1b0u), change_endian_long(0xd12a6426u), change_endian_long(0xa8fbabf1u));
                    TSIP.REG_104H.WORD = 0x00000058u;
                    TSIP.REG_E0H.WORD = 0x80010040u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x92800100u);
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
                    RX65NHU_func100(change_endian_long(0x22a6403bu), change_endian_long(0xa5d47559u), change_endian_long(0xa92cfbdeu), change_endian_long(0x9320f4dbu));
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
                    RX65NHU_func100(change_endian_long(0x70fc41f5u), change_endian_long(0xe91acab0u), change_endian_long(0x75442151u), change_endian_long(0x339e9b07u));
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
                    RX65NHU_func100(change_endian_long(0x4c948546u), change_endian_long(0x1d712588u), change_endian_long(0xf6ffc1a8u), change_endian_long(0xc60de09bu));
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
                    RX65NHU_func101(change_endian_long(0x2b9e65c8u), change_endian_long(0xf63b7afcu), change_endian_long(0x06875301u), change_endian_long(0xc0ca339cu));
                }
                RX65NHU_func102(change_endian_long(0x538f0aeeu), change_endian_long(0x72cc459au), change_endian_long(0x969c2e28u), change_endian_long(0x801ed1ecu));
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
 End of function ./input_dir/RX65NHU/RX65NHU_pc7.prc
 *********************************************************************************************************************/
