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
* Function Name: R_TSIP_GenerateTlsSVRsaInstallDataSub
*******************************************************************************************************************/ /**
* @details       RX65NHU TLS RSA Key Install for Server
* @param[in]     InData_SharedKeyIndex
* @param[in]     InData_SessionKey
* @param[in]     InData_IV
* @param[in]     InData_InstData
* @param[out]    OutData_InstData
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateTlsSVRsaInstallDataSub(uint32_t *InData_SharedKeyIndex, uint32_t *InData_SessionKey, uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_InstData)
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
    TSIP.REG_84H.WORD = 0x0000bf01u;
    TSIP.REG_108H.WORD = 0x00000000u;
    TSIP.REG_104H.WORD = 0x00000068u;
    TSIP.REG_E0H.WORD = 0x80010000u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(InData_SharedKeyIndex[0]);
    TSIP.REG_ECH.WORD = 0x38008c00u;
    TSIP.REG_ECH.WORD = 0xfffffff0u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX65NHU_func100(change_endian_long(0x392a4f4cu), change_endian_long(0xa2b98aa3u), change_endian_long(0x4d4dcc99u), change_endian_long(0x297102c1u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0x975f13d0u), change_endian_long(0xcf3579c0u), change_endian_long(0x7d4fe17bu), change_endian_long(0x261a946du));
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
        RX65NHU_func100(change_endian_long(0xae32e385u), change_endian_long(0x160a78bbu), change_endian_long(0xb9442ca0u), change_endian_long(0xada444afu));
        KEY_ADR = InData_SharedKeyIndex[0]*8;
        TSIP.REG_C4H.WORD = 0x000c3b0cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x3b74d08au);
        TSIP.REG_C4H.WORD = 0x000c0b0cu;
        TSIP.REG_E0H.WORD = 0x81010000u;
        TSIP.REG_00H.WORD = 0x00002807u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x02fb073du;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = S_FLASH[KEY_ADR + 0];
        TSIP.REG_100H.WORD = S_FLASH[KEY_ADR + 1];
        TSIP.REG_100H.WORD = S_FLASH[KEY_ADR + 2];
        TSIP.REG_100H.WORD = S_FLASH[KEY_ADR + 3];
        RX65NHU_func100(change_endian_long(0x887add39u), change_endian_long(0x8bc3028cu), change_endian_long(0x1fab6ab2u), change_endian_long(0xb401a140u));
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x000007bdu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = S_FLASH[KEY_ADR+4 + 0];
        TSIP.REG_100H.WORD = S_FLASH[KEY_ADR+4 + 1];
        TSIP.REG_100H.WORD = S_FLASH[KEY_ADR+4 + 2];
        TSIP.REG_100H.WORD = S_FLASH[KEY_ADR+4 + 3];
        TSIP.REG_A4H.WORD = 0x00800c45u;
        TSIP.REG_00H.WORD = 0x00001113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x00087a05u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_SessionKey[0];
        TSIP.REG_100H.WORD = InData_SessionKey[1];
        TSIP.REG_100H.WORD = InData_SessionKey[2];
        TSIP.REG_100H.WORD = InData_SessionKey[3];
        RX65NHU_func100(change_endian_long(0x48bf340eu), change_endian_long(0xed74f359u), change_endian_long(0xa99ab061u), change_endian_long(0xd0ca1549u));
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x00097a05u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_SessionKey[4];
        TSIP.REG_100H.WORD = InData_SessionKey[5];
        TSIP.REG_100H.WORD = InData_SessionKey[6];
        TSIP.REG_100H.WORD = InData_SessionKey[7];
        RX65NHU_func100(change_endian_long(0xc50ef5deu), change_endian_long(0xfb313479u), change_endian_long(0x10c84012u), change_endian_long(0x4f1d5332u));
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x00040805u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_IV[0];
        TSIP.REG_100H.WORD = InData_IV[1];
        TSIP.REG_100H.WORD = InData_IV[2];
        TSIP.REG_100H.WORD = InData_IV[3];
        TSIP.REG_104H.WORD = 0x00000051u;
        TSIP.REG_A4H.WORD = 0x00050804u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        RX65NHU_func103();
        RX65NHU_func100(change_endian_long(0x98b4dbb6u), change_endian_long(0xc96d2565u), change_endian_long(0x22529221u), change_endian_long(0xc5628183u));
        TSIP.REG_C4H.WORD = 0x210c350cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_C4H.WORD = 0x00080b8cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x45c55efau);
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_ECH.WORD = 0x0000b400u;
        TSIP.REG_ECH.WORD = 0x00000002u;
        TSIP.REG_E0H.WORD = 0x81010000u;
        TSIP.REG_C4H.WORD = 0x400c0b0cu;
        TSIP.REG_00H.WORD = 0x00002807u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_00H.WORD = 0x00013103u;
        TSIP.REG_2CH.WORD = 0x00000014u;
        TSIP.REG_104H.WORD = 0x00003f61u;
        TSIP.REG_B0H.WORD = 0x00000f00u;
        TSIP.REG_A4H.WORD = 0x00d0c9a7u;
        for (iLoop = 0; iLoop < 64; iLoop = iLoop + 4)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_InstData[iLoop + 0];
            TSIP.REG_100H.WORD = InData_InstData[iLoop + 1];
            TSIP.REG_100H.WORD = InData_InstData[iLoop + 2];
            TSIP.REG_100H.WORD = InData_InstData[iLoop + 3];
        }
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
        for (jLoop = 0; jLoop < 64; jLoop = jLoop + 4)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_InstData[4+jLoop + 0] = TSIP.REG_100H.WORD;
            OutData_InstData[4+jLoop + 1] = TSIP.REG_100H.WORD;
            OutData_InstData[4+jLoop + 2] = TSIP.REG_100H.WORD;
            OutData_InstData[4+jLoop + 3] = TSIP.REG_100H.WORD;
        }
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
        RX65NHU_func100(change_endian_long(0x72dd4720u), change_endian_long(0xe904de23u), change_endian_long(0x8d723523u), change_endian_long(0xe8090095u));
        TSIP.REG_00H.WORD = 0x00012303u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        TSIP.REG_D0H.WORD = 0x00000f00u;
        TSIP.REG_C4H.WORD = 0x00e087bfu;
        TSIP.REG_04H.WORD = 0x00000202u;
        for (jLoop = 64; jLoop < 128; jLoop = jLoop + 4)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_InstData[4+jLoop + 0] = TSIP.REG_100H.WORD;
            OutData_InstData[4+jLoop + 1] = TSIP.REG_100H.WORD;
            OutData_InstData[4+jLoop + 2] = TSIP.REG_100H.WORD;
            OutData_InstData[4+jLoop + 3] = TSIP.REG_100H.WORD;
        }
        RX65NHU_func100(change_endian_long(0xea12adccu), change_endian_long(0x3855b90au), change_endian_long(0xc80e5effu), change_endian_long(0xb8e91c43u));
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x00d049a5u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_InstData[iLoop + 0];
        TSIP.REG_100H.WORD = InData_InstData[iLoop + 1];
        TSIP.REG_100H.WORD = InData_InstData[iLoop + 2];
        TSIP.REG_100H.WORD = InData_InstData[iLoop + 3];
        TSIP.REG_C4H.WORD = 0x00e007bdu;
        TSIP.REG_00H.WORD = 0x00002113u;
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
        OutData_InstData[4+jLoop + 0] = TSIP.REG_100H.WORD;
        OutData_InstData[4+jLoop + 1] = TSIP.REG_100H.WORD;
        OutData_InstData[4+jLoop + 2] = TSIP.REG_100H.WORD;
        OutData_InstData[4+jLoop + 3] = TSIP.REG_100H.WORD;
        RX65NHU_func100(change_endian_long(0x79890254u), change_endian_long(0x30b3e098u), change_endian_long(0xce80fc2cu), change_endian_long(0x0c49ed98u));
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
        OutData_InstData[4+jLoop+4 + 0] = TSIP.REG_100H.WORD;
        OutData_InstData[4+jLoop+4 + 1] = TSIP.REG_100H.WORD;
        OutData_InstData[4+jLoop+4 + 2] = TSIP.REG_100H.WORD;
        OutData_InstData[4+jLoop+4 + 3] = TSIP.REG_100H.WORD;
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x000049a5u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_InstData[68];
        TSIP.REG_100H.WORD = InData_InstData[69];
        TSIP.REG_100H.WORD = InData_InstData[70];
        TSIP.REG_100H.WORD = InData_InstData[71];
        TSIP.REG_A4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00001113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX65NHU_func100(change_endian_long(0x29bce5d5u), change_endian_long(0x55c76b1eu), change_endian_long(0xa138c5bcu), change_endian_long(0x4d13ddb5u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func102(change_endian_long(0xf4e9bd90u), change_endian_long(0xfc026a48u), change_endian_long(0xf52ae908u), change_endian_long(0x670236d9u));
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
            RX65NHU_func100(change_endian_long(0xe63cfb32u), change_endian_long(0xee0e7a06u), change_endian_long(0x84147b4fu), change_endian_long(0xcdef7777u));
            TSIP.REG_104H.WORD = 0x00000052u;
            TSIP.REG_C4H.WORD = 0x01000c84u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_04H.WORD = 0x00000212u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_InstData[0] = TSIP.REG_100H.WORD;
            OutData_InstData[1] = TSIP.REG_100H.WORD;
            OutData_InstData[2] = TSIP.REG_100H.WORD;
            OutData_InstData[3] = TSIP.REG_100H.WORD;
            RX65NHU_func102(change_endian_long(0xb50db51bu), change_endian_long(0x0109e250u), change_endian_long(0xa3e3d0a2u), change_endian_long(0x0a51f3c7u));
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
 End of function ./input_dir/RX65NHU/RX65NHU_pbf.prc
 *********************************************************************************************************************/
