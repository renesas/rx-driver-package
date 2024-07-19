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

#if TSIP_SHA_256_HMAC == 1
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
* Function Name: R_TSIP_Sha256HmacInitSub
*******************************************************************************************************************/ /**
* @details       RX72M HMAC-SHA256 Message Digest Generation Init
* @param[in]     InData_KeyType
* @param[in]     InData_Cmd
* @param[in]     InData_KeyIndex
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256HmacInitSub(uint32_t *InData_KeyType, uint32_t *InData_Cmd, uint32_t *InData_KeyIndex)
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
    TSIP.REG_84H.WORD = 0x00007601u;
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
    TSIP.REG_7CH.WORD = 0x00000011u;
    TSIP.REG_104H.WORD = 0x00000068u;
    TSIP.REG_E0H.WORD = 0x80010000u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyType[0];
    TSIP.REG_ECH.WORD = 0x3020a800u;
    TSIP.REG_ECH.WORD = 0x00000005u;
    TSIP.REG_ECH.WORD = 0x00060020u;
    TSIP.REG_ECH.WORD = 0x0000b400u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x38008800u;
    TSIP.REG_ECH.WORD = 0x00000003u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0xa9978ab8u), change_endian_long(0x616f30d7u), change_endian_long(0x7302e312u), change_endian_long(0xe4f137bau));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0x20d4a236u), change_endian_long(0x2c42bc75u), change_endian_long(0x61b55b1eu), change_endian_long(0x8881b351u));
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
        TSIP.REG_ECH.WORD = 0x38000c00u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        TSIP.REG_ECH.WORD = 0x38008800u;
        TSIP.REG_ECH.WORD = 0x00000002u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0x33b2274bu), change_endian_long(0xbc0d9c73u), change_endian_long(0x34614bb8u), change_endian_long(0x95e21fc2u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_ECH.WORD = 0x38000c00u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            RX72M_RX72N_RX66N_func100(change_endian_long(0xa198d885u), change_endian_long(0x55bde4ddu), change_endian_long(0x23b4cadeu), change_endian_long(0x22c5312bu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_A4H.WORD = 0x200c3b0du;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_KeyIndex[0];
                TSIP.REG_100H.WORD = InData_KeyIndex[1];
                TSIP.REG_100H.WORD = InData_KeyIndex[2];
                TSIP.REG_100H.WORD = InData_KeyIndex[3];
                TSIP.REG_A4H.WORD = 0x000c0b0cu;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0xe75dd718u);
                RX72M_RX72N_RX66N_func101(change_endian_long(0xe47ffd2du), change_endian_long(0x7adea71au), change_endian_long(0xa2ba52e9u), change_endian_long(0xea16953cu));
            }
            else
            {
                TSIP.REG_A4H.WORD = 0x000c2b0du;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_KeyIndex[0];
                TSIP.REG_100H.WORD = InData_KeyIndex[1];
                TSIP.REG_100H.WORD = InData_KeyIndex[2];
                TSIP.REG_100H.WORD = InData_KeyIndex[3];
                TSIP.REG_A4H.WORD = 0x000c0b0cu;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0xb6e3697fu);
                RX72M_RX72N_RX66N_func101(change_endian_long(0x7c52a49bu), change_endian_long(0x8d3e99d6u), change_endian_long(0x4d019a2eu), change_endian_long(0x0e2fe1e6u));
            }
            TSIP.REG_104H.WORD = 0x00000761u;
            TSIP.REG_B0H.WORD = 0x00000100u;
            TSIP.REG_A4H.WORD = 0x02f087bfu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[4];
            TSIP.REG_100H.WORD = InData_KeyIndex[5];
            TSIP.REG_100H.WORD = InData_KeyIndex[6];
            TSIP.REG_100H.WORD = InData_KeyIndex[7];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[8];
            TSIP.REG_100H.WORD = InData_KeyIndex[9];
            TSIP.REG_100H.WORD = InData_KeyIndex[10];
            TSIP.REG_100H.WORD = InData_KeyIndex[11];
            TSIP.REG_E0H.WORD = 0x80080000u;
            TSIP.REG_00H.WORD = 0x00008123u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_104H.WORD = 0x00000361u;
            TSIP.REG_A4H.WORD = 0x000007bdu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[12];
            TSIP.REG_100H.WORD = InData_KeyIndex[13];
            TSIP.REG_100H.WORD = InData_KeyIndex[14];
            TSIP.REG_100H.WORD = InData_KeyIndex[15];
            TSIP.REG_A4H.WORD = 0x00900c45u;
            TSIP.REG_00H.WORD = 0x00001113u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x0000b7a0u;
            TSIP.REG_ECH.WORD = 0x00300000u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x68cc8396u), change_endian_long(0x061f5592u), change_endian_long(0xb3c382a3u), change_endian_long(0xe35ef6b4u));
        }
        else
        {
            TSIP.REG_ECH.WORD = 0x38008800u;
            TSIP.REG_ECH.WORD = 0x00000001u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            RX72M_RX72N_RX66N_func100(change_endian_long(0x5f9ef229u), change_endian_long(0xd4885387u), change_endian_long(0x28d4f219u), change_endian_long(0x5d42de5du));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_104H.WORD = 0x00000068u;
                TSIP.REG_E0H.WORD = 0x80010100u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_Cmd[0];
                TSIP.REG_ECH.WORD = 0x3020a900u;
                TSIP.REG_ECH.WORD = 0x00000002u;
                TSIP.REG_ECH.WORD = 0x00060020u;
                TSIP.REG_ECH.WORD = 0x0000b500u;
                TSIP.REG_ECH.WORD = 0x00000001u;
                TSIP.REG_ECH.WORD = 0x00000080u;
                TSIP.REG_104H.WORD = 0x00000368u;
                TSIP.REG_E0H.WORD = 0x80040000u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_KeyIndex[0];
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_KeyIndex[1];
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_KeyIndex[2];
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_KeyIndex[3];
                TSIP.REG_ECH.WORD = 0x00000bdeu;
                TSIP.REG_ECH.WORD = 0x000037e0u;
                TSIP.REG_ECH.WORD = 0x00008fe0u;
                TSIP.REG_ECH.WORD = 0xff000000u;
                TSIP.REG_ECH.WORD = 0x30203100u;
                TSIP.REG_ECH.WORD = 0x00070020u;
                TSIP.REG_ECH.WORD = 0x38008be0u;
                TSIP.REG_ECH.WORD = 0x02000000u;
                TSIP.REG_ECH.WORD = 0x1000d3c0u;
                TSIP.REG_ECH.WORD = 0x00000060u;
                TSIP.REG_ECH.WORD = 0x38008be0u;
                TSIP.REG_ECH.WORD = 0x82000000u;
                TSIP.REG_ECH.WORD = 0x1000d3c0u;
                TSIP.REG_ECH.WORD = 0x00000080u;
                TSIP.REG_ECH.WORD = 0x000037e0u;
                TSIP.REG_ECH.WORD = 0x38008fe0u;
                TSIP.REG_ECH.WORD = 0x00300000u;
                TSIP.REG_ECH.WORD = 0x2000d3c1u;
                TSIP.REG_ECH.WORD = 0x000037a0u;
                TSIP.REG_ECH.WORD = 0x00008fa0u;
                TSIP.REG_ECH.WORD = 0x00300000u;
                TSIP.REG_ECH.WORD = 0x000037e0u;
                TSIP.REG_ECH.WORD = 0x00008fe0u;
                TSIP.REG_ECH.WORD = 0x0000fe00u;
                TSIP.REG_ECH.WORD = 0x38008be0u;
                TSIP.REG_ECH.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x1000d3c2u;
                TSIP.REG_ECH.WORD = 0x38008bc0u;
                TSIP.REG_ECH.WORD = 0x00000007u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00A60000u;
                TSIP.REG_C4H.WORD = 0x00082b8du;
                TSIP.REG_E0H.WORD = 0x81040000u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0xc445c1a0u), change_endian_long(0x9655fda9u), change_endian_long(0x85ef2bd1u), change_endian_long(0x7508ae73u));
            }
            else
            {
                RX72M_RX72N_RX66N_func100(change_endian_long(0xe91cf7aau), change_endian_long(0x4bfa4f20u), change_endian_long(0x32043a08u), change_endian_long(0xde7d55f4u));
                TSIP.REG_104H.WORD = 0x00000068u;
                TSIP.REG_E0H.WORD = 0x80010000u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_Cmd[0];
                TSIP.REG_104H.WORD = 0x00000368u;
                TSIP.REG_E0H.WORD = 0x80040280u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_KeyIndex[0];
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_KeyIndex[1];
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_KeyIndex[2];
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_KeyIndex[3];
                TSIP.REG_ECH.WORD = 0x3020a800u;
                TSIP.REG_ECH.WORD = 0x00000007u;
                TSIP.REG_ECH.WORD = 0x00060020u;
                TSIP.REG_ECH.WORD = 0x0000b400u;
                TSIP.REG_ECH.WORD = 0x00000007u;
                TSIP.REG_ECH.WORD = 0x00000080u;
                TSIP.REG_ECH.WORD = 0x00007c00u;
                TSIP.REG_1CH.WORD = 0x00600000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    RX72M_RX72N_RX66N_func072();
                    TSIP.REG_A4H.WORD = 0x40080b8cu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x01249674u);
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x2682fe66u), change_endian_long(0x966f7ebau), change_endian_long(0x8049aaa7u), change_endian_long(0xf91f8af1u));
                }
                else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    RX72M_RX72N_RX66N_func072();
                    TSIP.REG_A4H.WORD = 0x40080b8cu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x01249674u);
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x971c93b2u), change_endian_long(0x7081a532u), change_endian_long(0x257845b3u), change_endian_long(0xa2fa0abfu));
                }
                else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    RX72M_RX72N_RX66N_func072();
                    TSIP.REG_A4H.WORD = 0x40080b8cu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x01249674u);
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x79d6b2acu), change_endian_long(0x854a46b8u), change_endian_long(0x755fef1au), change_endian_long(0x2a1f72afu));
                }
                else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
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
                    TSIP.REG_C4H.WORD = 0x40080b8cu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0xc41c104cu);
                    RX72M_RX72N_RX66N_func101(change_endian_long(0xd7ec4c4bu), change_endian_long(0xae253d95u), change_endian_long(0x81fa54cbu), change_endian_long(0xdac972deu));
                }
                else if (0x04000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    TSIP.REG_ECH.WORD = 0x0000b400u;
                    TSIP.REG_ECH.WORD = 0x00000000u;
                    RX72M_RX72N_RX66N_func075();
                    TSIP.REG_A4H.WORD = 0x40080b8cu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x7167f7f2u);
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x7a390dc5u), change_endian_long(0x5d5ca4e3u), change_endian_long(0x604cce58u), change_endian_long(0x1506d761u));
                }
                else if (0x05000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    TSIP.REG_ECH.WORD = 0x0000b400u;
                    TSIP.REG_ECH.WORD = 0x00000001u;
                    RX72M_RX72N_RX66N_func075();
                    TSIP.REG_A4H.WORD = 0x40080b8cu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x7167f7f2u);
                    RX72M_RX72N_RX66N_func101(change_endian_long(0xe41cbd35u), change_endian_long(0x4ce6c583u), change_endian_long(0x1e7d9398u), change_endian_long(0xcfaa1055u));
                }
                else if (0x06000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    TSIP.REG_ECH.WORD = 0x0000b400u;
                    TSIP.REG_ECH.WORD = 0x00000002u;
                    RX72M_RX72N_RX66N_func075();
                    TSIP.REG_A4H.WORD = 0x40080b8cu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0x7167f7f2u);
                    RX72M_RX72N_RX66N_func101(change_endian_long(0xfc21bbb6u), change_endian_long(0xc9cec390u), change_endian_long(0xa54f4e44u), change_endian_long(0xa6b4cdefu));
                }
                else if (0x07000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
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
                    TSIP.REG_C4H.WORD = 0x40080b8cu;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = change_endian_long(0xf6ddfecau);
                    RX72M_RX72N_RX66N_func101(change_endian_long(0xfcc686e1u), change_endian_long(0x9900418fu), change_endian_long(0xc2da506bu), change_endian_long(0x142e649du));
                }
                TSIP.REG_ECH.WORD = 0x0000b7a0u;
                TSIP.REG_ECH.WORD = 0x00300000u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0xc9dde687u), change_endian_long(0x9985738bu), change_endian_long(0x16f03fe2u), change_endian_long(0xcc894fbau));
            }
            TSIP.REG_104H.WORD = 0x00000361u;
            TSIP.REG_A4H.WORD = 0x42f007bdu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[4];
            TSIP.REG_100H.WORD = InData_KeyIndex[5];
            TSIP.REG_100H.WORD = InData_KeyIndex[6];
            TSIP.REG_100H.WORD = InData_KeyIndex[7];
            TSIP.REG_104H.WORD = 0x00000361u;
            TSIP.REG_A4H.WORD = 0x40f007bdu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[8];
            TSIP.REG_100H.WORD = InData_KeyIndex[9];
            TSIP.REG_100H.WORD = InData_KeyIndex[10];
            TSIP.REG_100H.WORD = InData_KeyIndex[11];
            TSIP.REG_E0H.WORD = 0x80080000u;
            TSIP.REG_00H.WORD = 0x00008123u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_104H.WORD = 0x00000361u;
            TSIP.REG_A4H.WORD = 0x400007bdu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[12];
            TSIP.REG_100H.WORD = InData_KeyIndex[13];
            TSIP.REG_100H.WORD = InData_KeyIndex[14];
            TSIP.REG_100H.WORD = InData_KeyIndex[15];
            TSIP.REG_A4H.WORD = 0x00900c45u;
            TSIP.REG_00H.WORD = 0x00001113u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x67147914u), change_endian_long(0x19ff8715u), change_endian_long(0xdd1da661u), change_endian_long(0xeaa7874bu));
        }
        RX72M_RX72N_RX66N_func100(change_endian_long(0x9bcb4c7eu), change_endian_long(0xe7f87a67u), change_endian_long(0x72363a6au), change_endian_long(0x3e4e5fe5u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func102(change_endian_long(0x34cf9867u), change_endian_long(0x5bfcc07eu), change_endian_long(0x30d31360u), change_endian_long(0xf4671c6eu));
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
            RX72M_RX72N_RX66N_func001();
            return TSIP_SUCCESS;
        }
    }
}
/**********************************************************************************************************************
 End of function ./input_dir/RX72M/RX72M_p76i_r7.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_SHA_256_HMAC == 1 */
