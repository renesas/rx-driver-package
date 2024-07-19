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

#if (TSIP_AES_128_KEY_WRAP == 1) || (TSIP_AES_256_KEY_WRAP == 1)
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
* Function Name: R_TSIP_AESKeyUnwrapSub
*******************************************************************************************************************/ /**
* @details       RX66T AES-128/256 Key unwrap
* @param[in]     InData_Cmd
* @param[in]     InData_KeyIndex
* @param[in]     InData_WrappedKeyType
* @param[in]     InData_Text
* @param[out]    OutData_KeyIndex
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_AESKeyUnwrapSub(uint32_t *InData_Cmd, uint32_t *InData_KeyIndex, uint32_t KEY_INDEX_SIZE, uint32_t *InData_WrappedKeyType, uint32_t *InData_Text, uint32_t WRAPPED_KEY_SIZE, uint32_t *OutData_KeyIndex)
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
    TSIP.REG_84H.WORD = 0x00006b01u;
    TSIP.REG_108H.WORD = 0x00000000u;
    TSIP.REG_104H.WORD = 0x00000168u;
    TSIP.REG_E0H.WORD = 0x800200c0u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Cmd[0];
    TSIP.REG_1D0H.WORD = 0x00000000u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_WrappedKeyType[0];
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00003407u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x3420a8c0u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00000002u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    TSIP.REG_ECH.WORD = 0x3420a800u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00000003u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    TSIP.REG_ECH.WORD = 0x38008800u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00000001u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    RX66T_RX72T_function001(change_endian_long(0x8bfcbc30u), change_endian_long(0x3e597485u), change_endian_long(0x17c321afu), change_endian_long(0x6cd86ef1u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX66T_RX72T_function003(change_endian_long(0x019528cfu), change_endian_long(0x128ba1edu), change_endian_long(0xb7a30d6fu), change_endian_long(0x19035e08u));
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
        TSIP.REG_A4H.WORD = 0x600c3a0du;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[0];
        TSIP.REG_100H.WORD = InData_KeyIndex[1];
        TSIP.REG_100H.WORD = InData_KeyIndex[2];
        TSIP.REG_100H.WORD = InData_KeyIndex[3];
        TSIP.REG_ECH.WORD = 0x38000cc6u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX66T_RX72T_function001(change_endian_long(0xbb6d97e8u), change_endian_long(0x918479b0u), change_endian_long(0x86b082d1u), change_endian_long(0x6d6cb361u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX66T_RX72T_function001(change_endian_long(0x8c432ba9u), change_endian_long(0xcf94536fu), change_endian_long(0x0837ed9fu), change_endian_long(0xfea6931cu));
            TSIP.REG_A4H.WORD = 0x400c0a0cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0xef138998u);
            TSIP.REG_A4H.WORD = 0x42f8063du;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[4];
            TSIP.REG_100H.WORD = InData_KeyIndex[5];
            TSIP.REG_100H.WORD = InData_KeyIndex[6];
            TSIP.REG_100H.WORD = InData_KeyIndex[7];
            TSIP.REG_A4H.WORD = 0x400006bdu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[8];
            TSIP.REG_100H.WORD = InData_KeyIndex[9];
            TSIP.REG_100H.WORD = InData_KeyIndex[10];
            TSIP.REG_100H.WORD = InData_KeyIndex[11];
            TSIP.REG_B0H.WORD = 0x00000000u;
            RX66T_RX72T_function002(change_endian_long(0xb592acfbu), change_endian_long(0x327cbb72u), change_endian_long(0x3a279f0fu), change_endian_long(0xda79ef0au));
        }
        else
        {
            RX66T_RX72T_function001(change_endian_long(0x6e534824u), change_endian_long(0x3cd337beu), change_endian_long(0xe67190dau), change_endian_long(0x0c7eebc8u));
            TSIP.REG_A4H.WORD = 0x400c0a0cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x213f32e4u);
            TSIP.REG_A4H.WORD = 0x42f8063du;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[4];
            TSIP.REG_100H.WORD = InData_KeyIndex[5];
            TSIP.REG_100H.WORD = InData_KeyIndex[6];
            TSIP.REG_100H.WORD = InData_KeyIndex[7];
            RX66T_RX72T_function001(change_endian_long(0x05c3d0b5u), change_endian_long(0x1a9f7765u), change_endian_long(0x3bcff65eu), change_endian_long(0xb697916bu));
            TSIP.REG_A4H.WORD = 0x40f9063du;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[8];
            TSIP.REG_100H.WORD = InData_KeyIndex[9];
            TSIP.REG_100H.WORD = InData_KeyIndex[10];
            TSIP.REG_100H.WORD = InData_KeyIndex[11];
            TSIP.REG_A4H.WORD = 0x400006bdu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[12];
            TSIP.REG_100H.WORD = InData_KeyIndex[13];
            TSIP.REG_100H.WORD = InData_KeyIndex[14];
            TSIP.REG_100H.WORD = InData_KeyIndex[15];
            TSIP.REG_B0H.WORD = 0x40000000u;
            RX66T_RX72T_function002(change_endian_long(0xe1be6bc0u), change_endian_long(0x55ce48ebu), change_endian_long(0x217dbfc6u), change_endian_long(0x432406e6u));
        }
        TSIP.REG_A4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00001113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX66T_RX72T_function001(change_endian_long(0xdfafdd5du), change_endian_long(0x6ab4650au), change_endian_long(0x2938f0a4u), change_endian_long(0xa032a5f5u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX66T_RX72T_function003(change_endian_long(0x52cff4cau), change_endian_long(0xf2ea2a5fu), change_endian_long(0x1f235982u), change_endian_long(0x1012fc91u));
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
            RX66T_RX72T_function030();
            TSIP.REG_ECH.WORD = 0x00002ca0u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_A4H.WORD = 0x00040804u;
            TSIP.REG_E0H.WORD = 0x810100c0u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_00H.WORD = 0x00001807u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_A4H.WORD = 0x00050804u;
            TSIP.REG_E0H.WORD = 0x810100e0u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_00H.WORD = 0x00001807u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_104H.WORD = 0x00000168u;
            TSIP.REG_E0H.WORD = 0x80020000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_Text[0];
            TSIP.REG_1D0H.WORD = 0x00000000u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_Text[1];
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000884u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000863u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00002c60u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            for(iLoop = 2; iLoop < WRAPPED_KEY_SIZE; iLoop = iLoop + 2)
            {
                TSIP.REG_104H.WORD = 0x00000168u;
                TSIP.REG_E0H.WORD = 0x80820004u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_Text[iLoop + 0];
                TSIP.REG_1D0H.WORD = 0x00000000u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_Text[iLoop+1 + 0];
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x0000a480u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x00000008u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x00002c60u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                RX66T_RX72T_function002(change_endian_long(0x52410404u), change_endian_long(0x1c544761u), change_endian_long(0x20ced7efu), change_endian_long(0x0eb65fa2u));
            }
            TSIP.REG_ECH.WORD = 0x38000865u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_1CH.WORD = 0x00402000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_A4H.WORD = 0x00060804u;
            TSIP.REG_E0H.WORD = 0x810100a0u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_00H.WORD = 0x00001807u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x0000a880u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000008u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_A4H.WORD = 0x00070804u;
            TSIP.REG_E0H.WORD = 0x81010080u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_00H.WORD = 0x00001807u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x000034c5u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x000030c0u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00003486u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00026c84u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00016cc6u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00002486u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x000008a5u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            for(jLoop = 5; jLoop >= 0; jLoop = jLoop - 1)
            {
                TSIP.REG_104H.WORD = 0x00000051u;
                TSIP.REG_A4H.WORD = 0x01000ec4u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x800100e0u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_00H.WORD = 0x00008107u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0000010fu;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_104H.WORD = 0x00000051u;
                TSIP.REG_A4H.WORD = 0x01000e84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x800100c0u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_00H.WORD = 0x00008107u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0000010fu;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x000030c0u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                for(iLoop = (WRAPPED_KEY_SIZE/2)-1; iLoop >= 1; iLoop = iLoop - 1)
                {
                    TSIP.REG_ECH.WORD = 0x00000824u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_A4H.WORD = 0x0000c88du;
                    TSIP.REG_E0H.WORD = 0x81020000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_00H.WORD = 0x0000180bu;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_E0H.WORD = 0x81820007u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_00H.WORD = 0x0000180bu;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_E0H.WORD = 0x80040000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_00H.WORD = 0x00008113u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_ECH.WORD = 0x00003c47u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x0000a4e0u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00000004u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00003c67u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x0000a8e0u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x0000000cu;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00003080u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x000030c0u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    RX66T_RX72T_function002(change_endian_long(0x2cd227feu), change_endian_long(0xf281fcf9u), change_endian_long(0x37d97d6cu), change_endian_long(0xed13f59bu));
                }
                TSIP.REG_ECH.WORD = 0x38000cc6u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_1CH.WORD = 0x00402000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x00002ca0u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                RX66T_RX72T_function002(change_endian_long(0x6f30379eu), change_endian_long(0xbfcf4a6eu), change_endian_long(0x48f60020u), change_endian_long(0xb7be5025u));
            }
            TSIP.REG_ECH.WORD = 0x380088a0u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000006u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_1CH.WORD = 0x00402000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x38008800u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0xa6a6a6a6u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            TSIP.REG_ECH.WORD = 0x38008820u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0xa6a6a6a6u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            RX66T_RX72T_function001(change_endian_long(0x4500d1d0u), change_endian_long(0x710b7daau), change_endian_long(0xfcbb3c5cu), change_endian_long(0x3d60568du));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX66T_RX72T_function003(change_endian_long(0x2f164fa3u), change_endian_long(0x24a42baeu), change_endian_long(0x231f4c0au), change_endian_long(0x071a81e6u));
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
                RX66T_RX72T_function001(change_endian_long(0x0f9190fdu), change_endian_long(0x00624a9cu), change_endian_long(0x57bd0474u), change_endian_long(0x8c199577u));
                TSIP.REG_104H.WORD = 0x00000051u;
                TSIP.REG_A4H.WORD = 0x00000ec4u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x800100a0u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_00H.WORD = 0x00008107u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0000010fu;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x0000a8a0u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x00000008u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_104H.WORD = 0x00000051u;
                TSIP.REG_A4H.WORD = 0x00000e84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x800100c0u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_00H.WORD = 0x00008107u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_00H.WORD = 0x0000010fu;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_104H.WORD = 0x00000251u;
                TSIP.REG_A4H.WORD = 0x0020363cu;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_A4H.WORD = 0x002036bcu;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_A4H.WORD = 0x002036bcu;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_A4H.WORD = 0x00060805u;
                TSIP.REG_00H.WORD = 0x00001113u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_104H.WORD = 0x00000151u;
                TSIP.REG_A4H.WORD = 0x0021340cu;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_A4H.WORD = 0x000b0805u;
                TSIP.REG_00H.WORD = 0x00001113u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_A4H.WORD = 0x01000684u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_E0H.WORD = 0x80040000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_00H.WORD = 0x00008113u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_A4H.WORD = 0x600c3a0du;
                TSIP.REG_E0H.WORD = 0x81040000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_00H.WORD = 0x00001813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_A4H.WORD = 0x400c0a0cu;
                TSIP.REG_E0H.WORD = 0x810100c0u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_00H.WORD = 0x00001807u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x000008e7u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_104H.WORD = 0x00000051u;
                TSIP.REG_A4H.WORD = 0x00040804u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_104H.WORD = 0x00000051u;
                TSIP.REG_A4H.WORD = 0x00050804u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_ECH.WORD = 0x00000884u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                for(iLoop = 0; iLoop < KEY_INDEX_SIZE-8; iLoop = iLoop + 4)
                {
                    TSIP.REG_ECH.WORD = 0x000008c6u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x34202885u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x2000d0c0u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00007c06u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_1CH.WORD = 0x00602000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    RX66T_RX72T_function001(change_endian_long(0xcc7a0779u), change_endian_long(0x93fc9d21u), change_endian_long(0xd155590du), change_endian_long(0xc27b45ebu));
                    TSIP.REG_A4H.WORD = 0x40e006bdu;
                    TSIP.REG_E0H.WORD = 0x81840007u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_00H.WORD = 0x00001813u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_04H.WORD = 0x00000112u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_KeyIndex[iLoop+4 + 0] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex[iLoop+4 + 1] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex[iLoop+4 + 2] = TSIP.REG_100H.WORD;
                    OutData_KeyIndex[iLoop+4 + 3] = TSIP.REG_100H.WORD;
                    TSIP.REG_ECH.WORD = 0x0000a4e0u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00000010u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x0000a480u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00000004u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    RX66T_RX72T_function002(change_endian_long(0x6c5828ddu), change_endian_long(0x5b1ad6d5u), change_endian_long(0xf626d317u), change_endian_long(0xc0c55fa3u));
                }
                TSIP.REG_ECH.WORD = 0x38000885u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_1CH.WORD = 0x00402000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                RX66T_RX72T_function001(change_endian_long(0xf8af5b0bu), change_endian_long(0x08661aebu), change_endian_long(0xdd120323u), change_endian_long(0xdb2cac01u));
                TSIP.REG_104H.WORD = 0x00000051u;
                TSIP.REG_A4H.WORD = 0x00000684u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_A4H.WORD = 0x400009cdu;
                TSIP.REG_00H.WORD = 0x00001113u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_04H.WORD = 0x00000112u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_KeyIndex[iLoop+4 + 0] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[iLoop+4 + 1] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[iLoop+4 + 2] = TSIP.REG_100H.WORD;
                OutData_KeyIndex[iLoop+4 + 3] = TSIP.REG_100H.WORD;
                RX66T_RX72T_function001(change_endian_long(0xdd6f77f1u), change_endian_long(0x92d80135u), change_endian_long(0x7cd3a90du), change_endian_long(0xdfa42534u));
                TSIP.REG_E0H.WORD = 0x81040000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
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
                RX66T_RX72T_function003(change_endian_long(0x970d5680u), change_endian_long(0xc8cbeea1u), change_endian_long(0x3e89b84eu), change_endian_long(0xe40692f1u));
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
 End of function ./input_dir/RX66T/RX66T_p6b.prc
 *********************************************************************************************************************/
#endif /* #if (TSIP_AES_128_KEY_WRAP == 1) || (TSIP_AES_256_KEY_WRAP == 1) */
