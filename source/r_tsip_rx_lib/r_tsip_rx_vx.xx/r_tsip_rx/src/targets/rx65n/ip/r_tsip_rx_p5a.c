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

#if TSIP_ECDH_P256 == 1
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
* Function Name: R_TSIP_DlmsCosemQeuSignatureVerificationSub
*******************************************************************************************************************/ /**
* @details       RX65NHU DLMS/COSEM Signature verification of QeU
* @param[in]     InData_Cmd
* @param[in]     InData_KeyIndex
* @param[in]     InData_data
* @param[in]     InData_Signature
* @param[out]    OutData_KeyIndex
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_DlmsCosemQeuSignatureVerificationSub(uint32_t *InData_Cmd, uint32_t *InData_KeyIndex, uint32_t *InData_data, uint32_t *InData_Signature, uint32_t *OutData_KeyIndex)
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
    TSIP.REG_84H.WORD = 0x00005a01u;
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
    TSIP.REG_ECH.WORD = 0x00000b9cu;
    TSIP.REG_ECH.WORD = 0x00000bdeu;
    TSIP.REG_104H.WORD = 0x00000f68u;
    TSIP.REG_E0H.WORD = 0x8090001eu;
    for (iLoop = 0; iLoop < 16; iLoop = iLoop + 1)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_Signature[iLoop + 0];
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_C4H.WORD = 0x200c3b0du;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[0];
    TSIP.REG_100H.WORD = InData_KeyIndex[1];
    TSIP.REG_100H.WORD = InData_KeyIndex[2];
    TSIP.REG_100H.WORD = InData_KeyIndex[3];
    RX65NHU_func023();
    TSIP.REG_104H.WORD = 0x00000b62u;
    TSIP.REG_D0H.WORD = 0x00000200u;
    TSIP.REG_C4H.WORD = 0x02f0888fu;
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
    TSIP.REG_00H.WORD = 0x00003223u;
    TSIP.REG_2CH.WORD = 0x0000009bu;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[12];
    TSIP.REG_100H.WORD = InData_KeyIndex[13];
    TSIP.REG_100H.WORD = InData_KeyIndex[14];
    TSIP.REG_100H.WORD = InData_KeyIndex[15];
    TSIP.REG_C4H.WORD = 0x0045094cu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_C4H.WORD = 0x00f0088du;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[16];
    TSIP.REG_100H.WORD = InData_KeyIndex[17];
    TSIP.REG_100H.WORD = InData_KeyIndex[18];
    TSIP.REG_100H.WORD = InData_KeyIndex[19];
    TSIP.REG_00H.WORD = 0x00003223u;
    TSIP.REG_2CH.WORD = 0x00000091u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_104H.WORD = 0x00000362u;
    TSIP.REG_C4H.WORD = 0x00000885u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[20];
    TSIP.REG_100H.WORD = InData_KeyIndex[21];
    TSIP.REG_100H.WORD = InData_KeyIndex[22];
    TSIP.REG_100H.WORD = InData_KeyIndex[23];
    TSIP.REG_C4H.WORD = 0x00900c45u;
    TSIP.REG_00H.WORD = 0x00002213u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX65NHU_func100(change_endian_long(0x47f1a31au), change_endian_long(0x6075369eu), change_endian_long(0x4cbe1876u), change_endian_long(0x82519507u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0x97da77abu), change_endian_long(0x391a93aau), change_endian_long(0x34d5fd8du), change_endian_long(0xbf0fa054u));
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
        RX65NHU_func100(change_endian_long(0xa292f10fu), change_endian_long(0x56d7694fu), change_endian_long(0xfc9687b2u), change_endian_long(0x5c8b50ceu));
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
        TSIP.REG_ECH.WORD = 0x0000b7c0u;
        TSIP.REG_ECH.WORD = 0x000000c0u;
        TSIP.REG_104H.WORD = 0x0000106cu;
        TSIP.REG_E0H.WORD = 0x8091001eu;
        for (iLoop = 0; iLoop < 17; iLoop = iLoop + 1)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_data[iLoop + 0];
        }
        TSIP.REG_104H.WORD = 0x00000e64u;
        for(iLoop = 17; iLoop < 32; iLoop = iLoop + 1)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_data[iLoop + 0];
        }
        /* WAIT_LOOP */
        while (0u != TSIP.REG_74H.BIT.B18)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001600u;
        TSIP.REG_74H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x30003380u;
        TSIP.REG_ECH.WORD = 0x00070020u;
        TSIP.REG_ECH.WORD = 0x0000b400u;
        TSIP.REG_ECH.WORD = 0x00000080u;
        TSIP.REG_ECH.WORD = 0x00030040u;
        TSIP.REG_ECH.WORD = 0x0000b400u;
        TSIP.REG_ECH.WORD = 0x0000013Cu;
        TSIP.REG_ECH.WORD = 0x00050040u;
        TSIP.REG_ECH.WORD = 0x0000b400u;
        TSIP.REG_ECH.WORD = 0x000001F8u;
        TSIP.REG_ECH.WORD = 0x00000080u;
        TSIP.REG_E0H.WORD = 0x81010000u;
        TSIP.REG_04H.WORD = 0x00000606u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
        OFS_ADR = S_RAM[0];
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x800103a0u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x0000005au);
        RX65NHU_func101(change_endian_long(0x8cbd60b0u), change_endian_long(0x8b449854u), change_endian_long(0x6b86ebb4u), change_endian_long(0x0378c057u));
        RX65NHU_func010(OFS_ADR);
        RX65NHU_func100(change_endian_long(0x3d405a1eu), change_endian_long(0x38536341u), change_endian_long(0xacd71e2du), change_endian_long(0x6dcb5e67u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func102(change_endian_long(0x320dc5d4u), change_endian_long(0x7b1d119bu), change_endian_long(0x2fc13290u), change_endian_long(0xb7486c74u));
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
            TSIP.REG_28H.WORD = 0x008f0001u;
            TSIP.REG_ECH.WORD = 0x0000b7c0u;
            TSIP.REG_ECH.WORD = 0x000000c0u;
            TSIP.REG_104H.WORD = 0x00000068u;
            TSIP.REG_E0H.WORD = 0x80010340u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_Cmd[0];
            TSIP.REG_ECH.WORD = 0x38000f5au;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            RX65NHU_func100(change_endian_long(0x72a80791u), change_endian_long(0xa031097bu), change_endian_long(0x3ac2369bu), change_endian_long(0x130fcc06u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_E0H.WORD = 0x8190001eu;
                TSIP.REG_00H.WORD = 0x00003843u;
                TSIP.REG_2CH.WORD = 0x00000010u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                RX65NHU_func101(change_endian_long(0xef2c7388u), change_endian_long(0x2edc1870u), change_endian_long(0xc1b8eaedu), change_endian_long(0x87e4999eu));
            }
            else
            {
                TSIP.REG_E0H.WORD = 0x8191001eu;
                TSIP.REG_00H.WORD = 0x08000807u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x08013803u;
                TSIP.REG_2CH.WORD = 0x00000010u;
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
                RX65NHU_func101(change_endian_long(0x65abaa57u), change_endian_long(0x9905ed23u), change_endian_long(0xdb8645feu), change_endian_long(0x3d5b9929u));
            }
            RX65NHU_func100(change_endian_long(0x7c2bd2abu), change_endian_long(0xde30b07eu), change_endian_long(0xefba51b1u), change_endian_long(0xf3fdcc2du));
            RX65NHU_func103();
            RX65NHU_func100(change_endian_long(0xf8c9e3a3u), change_endian_long(0x8f1680cfu), change_endian_long(0x0846204du), change_endian_long(0x9f9022dau));
            TSIP.REG_104H.WORD = 0x00000052u;
            TSIP.REG_C4H.WORD = 0x01000c84u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_E0H.WORD = 0x80040000u;
            TSIP.REG_00H.WORD = 0x00008213u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_C4H.WORD = 0x000c2b0du;
            TSIP.REG_E0H.WORD = 0x81040000u;
            TSIP.REG_00H.WORD = 0x00002813u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_C4H.WORD = 0x000c0b0cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x75614788u);
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
                OutData_KeyIndex[iLoop + 0] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[iLoop + 1] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[iLoop + 2] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[iLoop + 3] = TSIP.REG_100H.WORD;
            }
            RX65NHU_func100(change_endian_long(0x4b1710f1u), change_endian_long(0x9a1db23du), change_endian_long(0xccd9045eu), change_endian_long(0x131aef76u));
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
            OutData_KeyIndex[iLoop + 0] = TSIP.REG_100H.WORD;
            OutData_KeyIndex[iLoop + 1] = TSIP.REG_100H.WORD;
            OutData_KeyIndex[iLoop + 2] = TSIP.REG_100H.WORD;
            OutData_KeyIndex[iLoop + 3] = TSIP.REG_100H.WORD;
            RX65NHU_func100(change_endian_long(0x55074a1eu), change_endian_long(0xc2d7308cu), change_endian_long(0x894c8e70u), change_endian_long(0xa370faa3u));
            TSIP.REG_E0H.WORD = 0x81040000u;
            TSIP.REG_04H.WORD = 0x00000612u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex[0] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex[1] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex[2] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex[3] = TSIP.REG_100H.WORD;
            RX65NHU_func102(change_endian_long(0xbbbc68bbu), change_endian_long(0x5cceb6cfu), change_endian_long(0x5180d47bu), change_endian_long(0x5439cbf4u));
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
 End of function ./input_dir/RX65NHU/RX65NHU_p5a.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_ECDH_P256 == 1 */
