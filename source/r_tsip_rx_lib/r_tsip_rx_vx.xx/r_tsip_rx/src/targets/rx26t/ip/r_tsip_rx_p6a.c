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
* Function Name: R_TSIP_AESKeyWrapSub
*******************************************************************************************************************/ /**
* @details       RX26T AES-128/256 Key wrap
* @param[in]     InData_Cmd
* @param[in]     InData_KeyIndex
* @param[in]     InData_WrappedKeyType
* @param[in]     InData_WrappedKeyIndex
* @param[out]    OutData_Text
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_AESKeyWrapSub(uint32_t *InData_Cmd, uint32_t *InData_KeyIndex, uint32_t KEY_INDEX_SIZE, uint32_t *InData_WrappedKeyType, uint32_t *InData_WrappedKeyIndex, uint32_t WRAPPED_KEY_SIZE, uint32_t *OutData_Text)
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
    TSIP.REG_84H.WORD = 0x00006a01u;
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
    RX26T_function001(change_endian_long(0xc23c1980u), change_endian_long(0x2d664ab9u), change_endian_long(0x20f4e57du), change_endian_long(0xaead7038u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX26T_function003(change_endian_long(0xcfbc62a5u), change_endian_long(0x342b40dbu), change_endian_long(0x24b97664u), change_endian_long(0xb9ef1fa1u));
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
        RX26T_function001(change_endian_long(0xd29fb388u), change_endian_long(0x0b992e54u), change_endian_long(0x7cf60cd3u), change_endian_long(0x885a8c5fu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX26T_function001(change_endian_long(0x0456dcccu), change_endian_long(0xa7ceda75u), change_endian_long(0x780c9d1fu), change_endian_long(0x54312aeau));
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
            RX26T_function002(change_endian_long(0xe5c409ebu), change_endian_long(0xc3aa587du), change_endian_long(0x5d526843u), change_endian_long(0x071e4d6fu));
        }
        else
        {
            RX26T_function001(change_endian_long(0x03bca0b6u), change_endian_long(0x6063d182u), change_endian_long(0xac9f1d4bu), change_endian_long(0x4fd4cd0bu));
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
            RX26T_function001(change_endian_long(0x12be7963u), change_endian_long(0xe9435e46u), change_endian_long(0x562fa143u), change_endian_long(0x9f11c243u));
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
            RX26T_function002(change_endian_long(0x9191e6d4u), change_endian_long(0x11042a4du), change_endian_long(0x4d7058c9u), change_endian_long(0xbe27ed55u));
        }
        TSIP.REG_A4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00001113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX26T_function001(change_endian_long(0x36201e03u), change_endian_long(0xa62dff02u), change_endian_long(0x6533b62bu), change_endian_long(0x130ba9c1u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX26T_function003(change_endian_long(0xb6b4de52u), change_endian_long(0x398306bcu), change_endian_long(0x69566a74u), change_endian_long(0x27911c98u));
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
            RX26T_function030();
            TSIP.REG_A4H.WORD = 0x600c3a0du;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_WrappedKeyIndex[0];
            TSIP.REG_100H.WORD = InData_WrappedKeyIndex[1];
            TSIP.REG_100H.WORD = InData_WrappedKeyIndex[2];
            TSIP.REG_100H.WORD = InData_WrappedKeyIndex[3];
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
            TSIP.REG_ECH.WORD = 0x00000863u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x0000b420u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000010u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000842u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x0000b400u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000004u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x0000a8e0u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000008u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            for(iLoop = 0; iLoop < KEY_INDEX_SIZE-8; iLoop = iLoop + 4)
            {
                TSIP.REG_A4H.WORD = 0x40f006bdu;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_WrappedKeyIndex[iLoop+4 + 0];
                TSIP.REG_100H.WORD = InData_WrappedKeyIndex[iLoop+4 + 1];
                TSIP.REG_100H.WORD = InData_WrappedKeyIndex[iLoop+4 + 2];
                TSIP.REG_100H.WORD = InData_WrappedKeyIndex[iLoop+4 + 3];
                TSIP.REG_E0H.WORD = 0x80840003u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_00H.WORD = 0x00008113u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x00002461u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x00002440u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                RX26T_function002(change_endian_long(0x9ba2d577u), change_endian_long(0xb2572ef6u), change_endian_long(0x327070d1u), change_endian_long(0xc2b5b7fdu));
            }
            TSIP.REG_ECH.WORD = 0x38000847u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_1CH.WORD = 0x00402000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_A4H.WORD = 0x400006bdu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_WrappedKeyIndex[iLoop+4 + 0];
            TSIP.REG_100H.WORD = InData_WrappedKeyIndex[iLoop+4 + 1];
            TSIP.REG_100H.WORD = InData_WrappedKeyIndex[iLoop+4 + 2];
            TSIP.REG_100H.WORD = InData_WrappedKeyIndex[iLoop+4 + 3];
            TSIP.REG_A4H.WORD = 0x00900c45u;
            TSIP.REG_00H.WORD = 0x00001113u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX26T_function001(change_endian_long(0xad72242fu), change_endian_long(0x21d7b8b0u), change_endian_long(0x4ece7de0u), change_endian_long(0x07dc9c82u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX26T_function003(change_endian_long(0x4afddf27u), change_endian_long(0x72317dafu), change_endian_long(0x63173933u), change_endian_long(0x4d523219u));
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
                TSIP.REG_ECH.WORD = 0x0000b400u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0xa6a6a6a6u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x00003420u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x0000b480u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x00000001u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x000008c6u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x00036ca5u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                for(jLoop = 0; jLoop <= 5; jLoop = jLoop + 1)
                {
                    TSIP.REG_ECH.WORD = 0x000008e7u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    for(iLoop = 1; iLoop <= (WRAPPED_KEY_SIZE-2)/2; iLoop = iLoop + 1)
                    {
                        TSIP.REG_A4H.WORD = 0x0000888du;
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
                        TSIP.REG_ECH.WORD = 0x00000824u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        TSIP.REG_ECH.WORD = 0x00003c47u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        TSIP.REG_ECH.WORD = 0x0000a4e0u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        TSIP.REG_ECH.WORD = 0x00000004u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        TSIP.REG_ECH.WORD = 0x00003c67u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        TSIP.REG_ECH.WORD = 0x0000a4e0u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        TSIP.REG_ECH.WORD = 0x00000004u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        TSIP.REG_ECH.WORD = 0x00002c80u;
                        TSIP.REG_1D0H.WORD = 0x00000000u;
                        RX26T_function002(change_endian_long(0x4b8324f5u), change_endian_long(0x7820856eu), change_endian_long(0xe530f974u), change_endian_long(0x58902588u));
                    }
                    TSIP.REG_ECH.WORD = 0x380008e5u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00260000u;
                    TSIP.REG_1CH.WORD = 0x00402000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00002cc0u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    RX26T_function002(change_endian_long(0x1eaf2d9cu), change_endian_long(0x61775b0eu), change_endian_long(0xe06d982bu), change_endian_long(0xf7275c5bu));
                }
                TSIP.REG_ECH.WORD = 0x380088c0u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x00000006u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_1CH.WORD = 0x00402000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                RX26T_function001(change_endian_long(0xcf3db49au), change_endian_long(0xb360adb8u), change_endian_long(0xc0995e6du), change_endian_long(0xfc3e99d7u));
                TSIP.REG_E0H.WORD = 0x81020000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_04H.WORD = 0x0000060au;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_Text[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_Text[1] = TSIP.REG_100H.WORD;
                TSIP.REG_ECH.WORD = 0x000008e7u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x00000863u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x000368a5u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                for(iLoop = 2; iLoop < WRAPPED_KEY_SIZE; iLoop = iLoop + 2)
                {
                    TSIP.REG_ECH.WORD = 0x000008c6u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x34202865u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x2000d0c0u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00007c06u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_1CH.WORD = 0x00602000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    RX26T_function001(change_endian_long(0xa38f5bc1u), change_endian_long(0x10c51532u), change_endian_long(0x01eca5b6u), change_endian_long(0x3bb6726du));
                    TSIP.REG_E0H.WORD = 0x81820007u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_04H.WORD = 0x0000060au;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_Text[iLoop + 0] = TSIP.REG_100H.WORD;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_04H.BIT.B30)
                    {
                        /* waiting */
                    }
                    OutData_Text[iLoop+1 + 0] = TSIP.REG_100H.WORD;
                    TSIP.REG_ECH.WORD = 0x0000a4e0u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00000008u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00002c60u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    RX26T_function002(change_endian_long(0x6827ca59u), change_endian_long(0x45833bb7u), change_endian_long(0x66ed4324u), change_endian_long(0x95b537ecu));
                }
                TSIP.REG_ECH.WORD = 0x38000865u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_1CH.WORD = 0x00402000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                RX26T_function003(change_endian_long(0x304013e2u), change_endian_long(0xf59a0b85u), change_endian_long(0x496ece12u), change_endian_long(0x56ba3b6fu));
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
 End of function ./input_dir/RX26T/RX26T_p6a.prc
 *********************************************************************************************************************/
#endif /* #if (TSIP_AES_128_KEY_WRAP == 1) || (TSIP_AES_256_KEY_WRAP == 1) */
