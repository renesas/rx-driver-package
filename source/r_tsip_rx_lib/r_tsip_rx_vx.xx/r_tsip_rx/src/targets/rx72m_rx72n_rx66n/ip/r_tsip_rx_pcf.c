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
* Function Name: R_TSIP_Tls13UpdateApplicationTrafficKeySub
*******************************************************************************************************************/ /**
* @details       RX72M TLS1.3 Application Traffic Key Update
* @param[in]     InData_Cmd
* @param[in]     InData_UpdateKeyType
* @param[in]     InData_Handle
* @param[in]     InData_ApplicationSecretKeyIndex
* @param[out]    OutData_ApplicationSecretKeyIndex
* @param[out]    OutData_ApplicationWriteKeyIndex
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13UpdateApplicationTrafficKeySub(uint32_t *InData_Cmd, uint32_t *InData_UpdateKeyType, uint32_t *InData_Handle, uint32_t *InData_ApplicationSecretKeyIndex, uint32_t *OutData_ApplicationSecretKeyIndex, uint32_t *OutData_ApplicationWriteKeyIndex)
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
    TSIP.REG_84H.WORD = 0x0000cf01u;
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
    TSIP.REG_104H.WORD = 0x00000168u;
    TSIP.REG_E0H.WORD = 0x80020000u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Cmd[0];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_UpdateKeyType[0];
    RX72M_RX72N_RX66N_func401();
    TSIP.REG_ECH.WORD = 0x30203020u;
    TSIP.REG_ECH.WORD = 0x00070020u;
    TSIP.REG_ECH.WORD = 0x0000b500u;
    TSIP.REG_ECH.WORD = 0x114e17c8u;
    TSIP.REG_ECH.WORD = 0x0000b520u;
    TSIP.REG_ECH.WORD = 0x98bae316u;
    TSIP.REG_ECH.WORD = 0x00000060u;
    TSIP.REG_ECH.WORD = 0x0000b500u;
    TSIP.REG_ECH.WORD = 0x7dd8ec5au;
    TSIP.REG_ECH.WORD = 0x0000b520u;
    TSIP.REG_ECH.WORD = 0x6ad6575eu;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_104H.WORD = 0x00000368u;
    TSIP.REG_E0H.WORD = 0x80040280u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Handle[0];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Handle[1];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Handle[2];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Handle[3];
    RX72M_RX72N_RX66N_func072();
    TSIP.REG_C4H.WORD = 0x400c0b0cu;
    TSIP.REG_E0H.WORD = 0x81010100u;
    TSIP.REG_00H.WORD = 0x00002807u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_104H.WORD = 0x00000762u;
    TSIP.REG_D0H.WORD = 0x00000100u;
    TSIP.REG_C4H.WORD = 0x02f087bfu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_ApplicationSecretKeyIndex[0];
    TSIP.REG_100H.WORD = InData_ApplicationSecretKeyIndex[1];
    TSIP.REG_100H.WORD = InData_ApplicationSecretKeyIndex[2];
    TSIP.REG_100H.WORD = InData_ApplicationSecretKeyIndex[3];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_ApplicationSecretKeyIndex[4];
    TSIP.REG_100H.WORD = InData_ApplicationSecretKeyIndex[5];
    TSIP.REG_100H.WORD = InData_ApplicationSecretKeyIndex[6];
    TSIP.REG_100H.WORD = InData_ApplicationSecretKeyIndex[7];
    TSIP.REG_E0H.WORD = 0x80080000u;
    TSIP.REG_00H.WORD = 0x00008223u;
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
    TSIP.REG_100H.WORD = InData_ApplicationSecretKeyIndex[8];
    TSIP.REG_100H.WORD = InData_ApplicationSecretKeyIndex[9];
    TSIP.REG_100H.WORD = InData_ApplicationSecretKeyIndex[10];
    TSIP.REG_100H.WORD = InData_ApplicationSecretKeyIndex[11];
    TSIP.REG_C4H.WORD = 0x00900c45u;
    TSIP.REG_00H.WORD = 0x00002213u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x732275b4u), change_endian_long(0x18c7d0ecu), change_endian_long(0xc355f4acu), change_endian_long(0x43f51c5au));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0x539f9ff4u), change_endian_long(0x709d9263u), change_endian_long(0x97170e60u), change_endian_long(0xfae0c466u));
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
        RX72M_RX72N_RX66N_func100(change_endian_long(0x500bda01u), change_endian_long(0x82e32a0bu), change_endian_long(0x4da2b3d8u), change_endian_long(0x83e49b44u));
        TSIP.REG_28H.WORD = 0x008f0001u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        TSIP.REG_104H.WORD = 0x00000557u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00201174u);
        TSIP.REG_100H.WORD = change_endian_long(0x6c733133u);
        TSIP.REG_100H.WORD = change_endian_long(0x20747261u);
        TSIP.REG_100H.WORD = change_endian_long(0x66666963u);
        TSIP.REG_100H.WORD = change_endian_long(0x20757064u);
        TSIP.REG_100H.WORD = change_endian_long(0x00018000u);
        TSIP.REG_00H.WORD = 0x00003027u;
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
        TSIP.REG_100H.WORD = change_endian_long(0x000002B0u);
        RX72M_RX72N_RX66N_func070();
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
        TSIP.REG_104H.WORD = 0x00000357u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00100974u);
        TSIP.REG_100H.WORD = change_endian_long(0x6c733133u);
        TSIP.REG_100H.WORD = change_endian_long(0x206b6579u);
        TSIP.REG_100H.WORD = change_endian_long(0x00018000u);
        TSIP.REG_00H.WORD = 0x0000302fu;
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
        TSIP.REG_100H.WORD = change_endian_long(0x00000270u);
        RX72M_RX72N_RX66N_func070();
        TSIP.REG_ECH.WORD = 0x0000b7e0u;
        TSIP.REG_ECH.WORD = 0x00000050u;
        TSIP.REG_E0H.WORD = 0x8084001fu;
        TSIP.REG_00H.WORD = 0x00008513u;
        TSIP.REG_74H.WORD = 0x00000008u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_00H.WORD = 0x00000513u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x008f0001u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        TSIP.REG_104H.WORD = 0x00000357u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x000c0874u);
        TSIP.REG_100H.WORD = change_endian_long(0x6c733133u);
        TSIP.REG_100H.WORD = change_endian_long(0x20697600u);
        TSIP.REG_100H.WORD = change_endian_long(0x01800000u);
        TSIP.REG_00H.WORD = 0x0000302fu;
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
        TSIP.REG_100H.WORD = change_endian_long(0x00000268u);
        RX72M_RX72N_RX66N_func070();
        TSIP.REG_ECH.WORD = 0x0000b7e0u;
        TSIP.REG_ECH.WORD = 0x00000060u;
        TSIP.REG_E0H.WORD = 0x8084001fu;
        TSIP.REG_00H.WORD = 0x00008513u;
        TSIP.REG_74H.WORD = 0x00000008u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_00H.WORD = 0x00000513u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_C4H.WORD = 0x400c0b0cu;
        TSIP.REG_E0H.WORD = 0x81010100u;
        TSIP.REG_00H.WORD = 0x00002807u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x00870001u;
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x02e087bfu;
        TSIP.REG_E0H.WORD = 0x81080000u;
        TSIP.REG_00H.WORD = 0x00002823u;
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
        for (iLoop = 0; iLoop < 12; iLoop = iLoop + 4)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_ApplicationSecretKeyIndex[iLoop + 0] = TSIP.REG_100H.WORD;
            OutData_ApplicationSecretKeyIndex[iLoop + 1] = TSIP.REG_100H.WORD;
            OutData_ApplicationSecretKeyIndex[iLoop + 2] = TSIP.REG_100H.WORD;
            OutData_ApplicationSecretKeyIndex[iLoop + 3] = TSIP.REG_100H.WORD;
        }
        RX72M_RX72N_RX66N_func100(change_endian_long(0xe0095e19u), change_endian_long(0x7c0ed1f4u), change_endian_long(0x7c91b78fu), change_endian_long(0x0fa9cbfbu));
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
        TSIP.REG_E0H.WORD = 0x81010120u;
        TSIP.REG_00H.WORD = 0x00002807u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_ECH.WORD = 0x0000b7e0u;
        TSIP.REG_ECH.WORD = 0x00000050u;
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x02e087bfu;
        TSIP.REG_E0H.WORD = 0x8188001fu;
        TSIP.REG_00H.WORD = 0x00002823u;
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
        for (iLoop = 0; iLoop < 12; iLoop = iLoop + 4)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_ApplicationWriteKeyIndex[iLoop+4 + 0] = TSIP.REG_100H.WORD;
            OutData_ApplicationWriteKeyIndex[iLoop+4 + 1] = TSIP.REG_100H.WORD;
            OutData_ApplicationWriteKeyIndex[iLoop+4 + 2] = TSIP.REG_100H.WORD;
            OutData_ApplicationWriteKeyIndex[iLoop+4 + 3] = TSIP.REG_100H.WORD;
        }
        RX72M_RX72N_RX66N_func100(change_endian_long(0x8f8b3959u), change_endian_long(0xf09ffb07u), change_endian_long(0xf28eb335u), change_endian_long(0xe93535aau));
        TSIP.REG_E0H.WORD = 0x81040280u;
        TSIP.REG_04H.WORD = 0x00000612u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_ApplicationWriteKeyIndex[0] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_ApplicationWriteKeyIndex[1] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_ApplicationWriteKeyIndex[2] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_ApplicationWriteKeyIndex[3] = TSIP.REG_100H.WORD;
        RX72M_RX72N_RX66N_func102(change_endian_long(0xbe9f220bu), change_endian_long(0x186d2bd0u), change_endian_long(0xc4094afeu), change_endian_long(0xef8681bau));
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
/**********************************************************************************************************************
 End of function ./input_dir/RX72M/RX72M_pcf_r1.prc
 *********************************************************************************************************************/
