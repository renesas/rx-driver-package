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
* Function Name: R_TSIP_Tls13GenerateResumptionHandshakeSecretSub
*******************************************************************************************************************/ /**
* @details       RX65NHU TLS1.3 Resumption/0-RTT Handshake Secret Generation
* @param[in]     InData_Handle
* @param[in]     InData_SharedSecretKeyIndexType
* @param[in]     InData_PreSharedKeyIndex
* @param[in]     InData_SharedSecretKeyIndex
* @param[out]    OutData_Handshake_SecretKeyIndex
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13GenerateResumptionHandshakeSecretSub(uint32_t *InData_Handle, uint32_t *InData_SharedSecretKeyIndexType, uint32_t *InData_PreSharedKeyIndex, uint32_t *InData_SharedSecretKeyIndex, uint32_t *OutData_Handshake_SecretKeyIndex)
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
    TSIP.REG_84H.WORD = 0x0000dd01u;
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
    TSIP.REG_E0H.WORD = 0x80010100u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_SharedSecretKeyIndexType[0];
    TSIP.REG_ECH.WORD = 0x00000bffu;
    TSIP.REG_ECH.WORD = 0x38000d08u;
    TSIP.REG_ECH.WORD = 0x1000d3e0u;
    TSIP.REG_ECH.WORD = 0x38000fffu;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX65NHU_func100(change_endian_long(0x7cb28272u), change_endian_long(0x2a80cb0au), change_endian_long(0x4034c0eeu), change_endian_long(0x490b8c44u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0xc3893d0bu), change_endian_long(0xc3699549u), change_endian_long(0x73b4e331u), change_endian_long(0xbbcca5e4u));
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
        TSIP.REG_104H.WORD = 0x00000362u;
        TSIP.REG_C4H.WORD = 0x00050805u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_Handle[0];
        TSIP.REG_100H.WORD = InData_Handle[1];
        TSIP.REG_100H.WORD = InData_Handle[2];
        TSIP.REG_100H.WORD = InData_Handle[3];
        TSIP.REG_104H.WORD = 0x00000368u;
        TSIP.REG_E0H.WORD = 0x80040280u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_SharedSecretKeyIndex[0];
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_SharedSecretKeyIndex[1];
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_SharedSecretKeyIndex[2];
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_SharedSecretKeyIndex[3];
        TSIP.REG_C4H.WORD = 0x00900c45u;
        TSIP.REG_E0H.WORD = 0x81040280u;
        TSIP.REG_00H.WORD = 0x00002813u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX65NHU_func100(change_endian_long(0xade36532u), change_endian_long(0x19fa3a3bu), change_endian_long(0x54067d38u), change_endian_long(0x7f152fbcu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func102(change_endian_long(0x0f5f3b6au), change_endian_long(0x263c61dau), change_endian_long(0x697ba8a3u), change_endian_long(0x1ddd7658u));
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
            TSIP.REG_E0H.WORD = 0x81040280u;
            TSIP.REG_00H.WORD = 0x00002813u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_C4H.WORD = 0x400c0b0cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x44f612dfu);
            TSIP.REG_104H.WORD = 0x00000762u;
            TSIP.REG_D0H.WORD = 0x00000100u;
            TSIP.REG_C4H.WORD = 0x02f087bfu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_PreSharedKeyIndex[0];
            TSIP.REG_100H.WORD = InData_PreSharedKeyIndex[1];
            TSIP.REG_100H.WORD = InData_PreSharedKeyIndex[2];
            TSIP.REG_100H.WORD = InData_PreSharedKeyIndex[3];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_PreSharedKeyIndex[4];
            TSIP.REG_100H.WORD = InData_PreSharedKeyIndex[5];
            TSIP.REG_100H.WORD = InData_PreSharedKeyIndex[6];
            TSIP.REG_100H.WORD = InData_PreSharedKeyIndex[7];
            TSIP.REG_00H.WORD = 0x00003223u;
            TSIP.REG_2CH.WORD = 0x00000090u;
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
            TSIP.REG_100H.WORD = InData_PreSharedKeyIndex[8];
            TSIP.REG_100H.WORD = InData_PreSharedKeyIndex[9];
            TSIP.REG_100H.WORD = InData_PreSharedKeyIndex[10];
            TSIP.REG_100H.WORD = InData_PreSharedKeyIndex[11];
            TSIP.REG_C4H.WORD = 0x00900c45u;
            TSIP.REG_00H.WORD = 0x00002213u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX65NHU_func100(change_endian_long(0x8b0fb5ddu), change_endian_long(0x93f3c1edu), change_endian_long(0xec7cf310u), change_endian_long(0x66ba2874u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX65NHU_func102(change_endian_long(0x16989eb3u), change_endian_long(0x1e07d602u), change_endian_long(0xedee54a2u), change_endian_long(0xfc785b91u));
                TSIP.REG_1BCH.WORD = 0x00000040u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_18H.BIT.B12)
                {
                    /* waiting */
                }
                #if TSIP_MULTI_THREADING == 1
                TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
                #endif /* TSIP_MULTI_THREADING == 1 */
                return TSIP_ERR_KEY_SET;
            }
            else
            {
                TSIP.REG_C4H.WORD = 0x400c0b0cu;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0xe274e1d0u);
                TSIP.REG_104H.WORD = 0x00000762u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02f087bfu;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_SharedSecretKeyIndex[4];
                TSIP.REG_100H.WORD = InData_SharedSecretKeyIndex[5];
                TSIP.REG_100H.WORD = InData_SharedSecretKeyIndex[6];
                TSIP.REG_100H.WORD = InData_SharedSecretKeyIndex[7];
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_SharedSecretKeyIndex[8];
                TSIP.REG_100H.WORD = InData_SharedSecretKeyIndex[9];
                TSIP.REG_100H.WORD = InData_SharedSecretKeyIndex[10];
                TSIP.REG_100H.WORD = InData_SharedSecretKeyIndex[11];
                TSIP.REG_00H.WORD = 0x00003223u;
                TSIP.REG_2CH.WORD = 0x00000018u;
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
                TSIP.REG_100H.WORD = InData_SharedSecretKeyIndex[12];
                TSIP.REG_100H.WORD = InData_SharedSecretKeyIndex[13];
                TSIP.REG_100H.WORD = InData_SharedSecretKeyIndex[14];
                TSIP.REG_100H.WORD = InData_SharedSecretKeyIndex[15];
                TSIP.REG_C4H.WORD = 0x00900c45u;
                TSIP.REG_00H.WORD = 0x00002213u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                RX65NHU_func100(change_endian_long(0xad6cf056u), change_endian_long(0x1e735659u), change_endian_long(0x6786bf13u), change_endian_long(0x9b7737d8u));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    RX65NHU_func102(change_endian_long(0x24b44e05u), change_endian_long(0x7fcace03u), change_endian_long(0x77f40289u), change_endian_long(0x3e313799u));
                    TSIP.REG_1BCH.WORD = 0x00000040u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_18H.BIT.B12)
                    {
                        /* waiting */
                    }
                    #if TSIP_MULTI_THREADING == 1
                    TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
                    #endif /* TSIP_MULTI_THREADING == 1 */
                    return TSIP_ERR_KEY_SET;
                }
                else
                {
                    TSIP.REG_ECH.WORD = 0x00000800u;
                    TSIP.REG_ECH.WORD = 0x00000821u;
                    TSIP.REG_ECH.WORD = 0x00000842u;
                    TSIP.REG_ECH.WORD = 0x00000863u;
                    TSIP.REG_ECH.WORD = 0x00000884u;
                    TSIP.REG_ECH.WORD = 0x000008a5u;
                    TSIP.REG_ECH.WORD = 0x000008c6u;
                    TSIP.REG_ECH.WORD = 0x000008e7u;
                    TSIP.REG_2CH.WORD = 0x00000012u;
                    TSIP.REG_104H.WORD = 0x00000057u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x80000000u);
                    TSIP.REG_00H.WORD = 0x0000301bu;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_104H.WORD = 0x00000057u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x00000300u);
                    TSIP.REG_24H.WORD = 0x000088d0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_28H.WORD = 0x008f0001u;
                    RX65NHU_func070();
                    TSIP.REG_E0H.WORD = 0x80080000u;
                    TSIP.REG_00H.WORD = 0x00008523u;
                    TSIP.REG_74H.WORD = 0x00000008u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_28H.WORD = 0x008f0001u;
                    TSIP.REG_2CH.WORD = 0x00000010u;
                    TSIP.REG_104H.WORD = 0x00000f57u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x00200d74u);
                    TSIP.REG_100H.WORD = change_endian_long(0x6c733133u);
                    TSIP.REG_100H.WORD = change_endian_long(0x20646572u);
                    TSIP.REG_100H.WORD = change_endian_long(0x69766564u);
                    TSIP.REG_100H.WORD = change_endian_long(0x20e3b0c4u);
                    TSIP.REG_100H.WORD = change_endian_long(0x4298fc1cu);
                    TSIP.REG_100H.WORD = change_endian_long(0x149afbf4u);
                    TSIP.REG_100H.WORD = change_endian_long(0xc8996fb9u);
                    TSIP.REG_100H.WORD = change_endian_long(0x2427ae41u);
                    TSIP.REG_100H.WORD = change_endian_long(0xe4649b93u);
                    TSIP.REG_100H.WORD = change_endian_long(0x4ca49599u);
                    TSIP.REG_100H.WORD = change_endian_long(0x1b7852b8u);
                    TSIP.REG_100H.WORD = change_endian_long(0x55018000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000390u);
                    RX65NHU_func070();
                    TSIP.REG_E0H.WORD = 0x80080000u;
                    TSIP.REG_00H.WORD = 0x00008523u;
                    TSIP.REG_74H.WORD = 0x00000008u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_34H.WORD = 0x00000408u;
                    TSIP.REG_24H.WORD = 0x800090d0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_24H.WORD = 0x000088d0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_28H.WORD = 0x008f0001u;
                    RX65NHU_func070();
                    RX65NHU_func100(change_endian_long(0xc789b0a2u), change_endian_long(0x17442257u), change_endian_long(0xb651cb7eu), change_endian_long(0x2573b3a2u));
                    TSIP.REG_ECH.WORD = 0x00003408u;
                    RX65NHU_func401();
                    RX65NHU_func072();
                    TSIP.REG_C4H.WORD = 0x400c0b0cu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x89fe475eu);
                    TSIP.REG_D0H.WORD = 0x00000100u;
                    TSIP.REG_C4H.WORD = 0x02e087bfu;
                    TSIP.REG_00H.WORD = 0x00002523u;
                    TSIP.REG_74H.WORD = 0x00000008u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_104H.WORD = 0x00000352u;
                    TSIP.REG_C4H.WORD = 0x000009cdu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                    TSIP.REG_100H.WORD = change_endian_long(0x00000002u);
                    TSIP.REG_04H.WORD = 0x00000232u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_Handshake_SecretKeyIndex[4] = TSIP.REG_100H.WORD;
                    OutData_Handshake_SecretKeyIndex[5] = TSIP.REG_100H.WORD;
                    OutData_Handshake_SecretKeyIndex[6] = TSIP.REG_100H.WORD;
                    OutData_Handshake_SecretKeyIndex[7] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_Handshake_SecretKeyIndex[8] = TSIP.REG_100H.WORD;
                    OutData_Handshake_SecretKeyIndex[9] = TSIP.REG_100H.WORD;
                    OutData_Handshake_SecretKeyIndex[10] = TSIP.REG_100H.WORD;
                    OutData_Handshake_SecretKeyIndex[11] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_Handshake_SecretKeyIndex[12] = TSIP.REG_100H.WORD;
                    OutData_Handshake_SecretKeyIndex[13] = TSIP.REG_100H.WORD;
                    OutData_Handshake_SecretKeyIndex[14] = TSIP.REG_100H.WORD;
                    OutData_Handshake_SecretKeyIndex[15] = TSIP.REG_100H.WORD;
                    RX65NHU_func100(change_endian_long(0xeb39bcb1u), change_endian_long(0x890620ffu), change_endian_long(0x06e475aau), change_endian_long(0xb87b2bc5u));
                    TSIP.REG_E0H.WORD = 0x81040280u;
                    TSIP.REG_04H.WORD = 0x00000612u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_Handshake_SecretKeyIndex[0] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_Handshake_SecretKeyIndex[1] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_Handshake_SecretKeyIndex[2] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_Handshake_SecretKeyIndex[3] = TSIP.REG_100H.WORD;
                    RX65NHU_func102(change_endian_long(0x441084e9u), change_endian_long(0xc44d6795u), change_endian_long(0xfab57768u), change_endian_long(0x45ea9eb5u));
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
}
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_pdd_r4.prc
 *********************************************************************************************************************/
