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
* @details       RX72M AES-128/256 Key wrap
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
    if (0x0u != (TSIP.REG_1B4H.WORD & 0x1du))
    {
        #if TSIP_MULTI_THREADING == 1
        TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
        #endif /* TSIP_MULTI_THREADING == 1 */
        return TSIP_ERR_RESOURCE_CONFLICT;
    }
    TSIP.REG_84H.WORD = 0x00006a01u;
    TSIP.REG_108H.WORD = 0x00000000u;
    TSIP.REG_104H.WORD = 0x00000168u;
    TSIP.REG_E0H.WORD = 0x80020100u;
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
    TSIP.REG_100H.WORD = InData_WrappedKeyType[0];
    TSIP.REG_ECH.WORD = 0x00003409u;
    TSIP.REG_ECH.WORD = 0x3420a900u;
    TSIP.REG_ECH.WORD = 0x00000002u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    TSIP.REG_ECH.WORD = 0x3420a800u;
    TSIP.REG_ECH.WORD = 0x00000003u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe7f7b98au), change_endian_long(0x24971519u), change_endian_long(0x90ef29cdu), change_endian_long(0x19510ec3u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0x8b88f72cu), change_endian_long(0xe90a6fe2u), change_endian_long(0x2d63e3e3u), change_endian_long(0x82eb73edu));
        TSIP.REG_1B4H.WORD = 0x00000040u;
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
        TSIP.REG_ECH.WORD = 0x38000d08u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0xb7c4384cu), change_endian_long(0x8dc0f4e7u), change_endian_long(0xc3172001u), change_endian_long(0xfdb3b3aau));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func100(change_endian_long(0xa9d9203au), change_endian_long(0x98d9bb73u), change_endian_long(0x6a0fcac6u), change_endian_long(0x38f3ddcbu));
            TSIP.REG_A4H.WORD = 0x000c0b0cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x4e2279dbu);
            TSIP.REG_A4H.WORD = 0x02f8073du;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[4];
            TSIP.REG_100H.WORD = InData_KeyIndex[5];
            TSIP.REG_100H.WORD = InData_KeyIndex[6];
            TSIP.REG_100H.WORD = InData_KeyIndex[7];
            TSIP.REG_A4H.WORD = 0x000007bdu;
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
            RX72M_RX72N_RX66N_func101(change_endian_long(0x28233e4bu), change_endian_long(0x4b5c73a4u), change_endian_long(0xb9c6df84u), change_endian_long(0x301f9d82u));
        }
        else
        {
            RX72M_RX72N_RX66N_func100(change_endian_long(0x7d89fcc2u), change_endian_long(0x3309d59fu), change_endian_long(0x16cb0a9du), change_endian_long(0x60fc64acu));
            TSIP.REG_A4H.WORD = 0x000c0b0cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x6e78d475u);
            TSIP.REG_A4H.WORD = 0x02f8073du;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[4];
            TSIP.REG_100H.WORD = InData_KeyIndex[5];
            TSIP.REG_100H.WORD = InData_KeyIndex[6];
            TSIP.REG_100H.WORD = InData_KeyIndex[7];
            RX72M_RX72N_RX66N_func100(change_endian_long(0x6581d72eu), change_endian_long(0x56b2b3e9u), change_endian_long(0xcd60239bu), change_endian_long(0xb13264d6u));
            TSIP.REG_A4H.WORD = 0x00f9073du;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[8];
            TSIP.REG_100H.WORD = InData_KeyIndex[9];
            TSIP.REG_100H.WORD = InData_KeyIndex[10];
            TSIP.REG_100H.WORD = InData_KeyIndex[11];
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
            TSIP.REG_B0H.WORD = 0x40000000u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0xd1484c5bu), change_endian_long(0x2bf5eb49u), change_endian_long(0x43076a9fu), change_endian_long(0x9213746du));
        }
        TSIP.REG_A4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00001113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0x22c25a99u), change_endian_long(0x8051e097u), change_endian_long(0x581ec562u), change_endian_long(0x9d571934u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func102(change_endian_long(0xed6bbc8cu), change_endian_long(0x1c951eabu), change_endian_long(0xf804451du), change_endian_long(0x3799ed19u));
            TSIP.REG_1B4H.WORD = 0x00000040u;
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
            RX72M_RX72N_RX66N_func030();
            TSIP.REG_A4H.WORD = 0x200c3b0du;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_WrappedKeyIndex[0];
            TSIP.REG_100H.WORD = InData_WrappedKeyIndex[1];
            TSIP.REG_100H.WORD = InData_WrappedKeyIndex[2];
            TSIP.REG_100H.WORD = InData_WrappedKeyIndex[3];
            TSIP.REG_A4H.WORD = 0x000c0b0cu;
            TSIP.REG_E0H.WORD = 0x810100c0u;
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
            TSIP.REG_ECH.WORD = 0x0000b7e0u;
            TSIP.REG_ECH.WORD = 0x00000008u;
            TSIP.REG_ECH.WORD = 0x0000b780u;
            TSIP.REG_ECH.WORD = 0x00000010u;
            TSIP.REG_ECH.WORD = 0x00000bdeu;
            TSIP.REG_ECH.WORD = 0x0000b760u;
            TSIP.REG_ECH.WORD = 0x00000004u;
            TSIP.REG_ECH.WORD = 0x0000a8e0u;
            TSIP.REG_ECH.WORD = 0x00000008u;
            for(iLoop = 0; iLoop < KEY_INDEX_SIZE-8; iLoop = iLoop + 4)
            {
                TSIP.REG_A4H.WORD = 0x00f007bdu;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_WrappedKeyIndex[iLoop+4 + 0];
                TSIP.REG_100H.WORD = InData_WrappedKeyIndex[iLoop+4 + 1];
                TSIP.REG_100H.WORD = InData_WrappedKeyIndex[iLoop+4 + 2];
                TSIP.REG_100H.WORD = InData_WrappedKeyIndex[iLoop+4 + 3];
                TSIP.REG_E0H.WORD = 0x8084001fu;
                TSIP.REG_00H.WORD = 0x00008113u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x000027fcu;
                TSIP.REG_ECH.WORD = 0x000027dbu;
                RX72M_RX72N_RX66N_func101(change_endian_long(0xb0ffef35u), change_endian_long(0x45e22eb4u), change_endian_long(0x4d424708u), change_endian_long(0xec6132a8u));
            }
            TSIP.REG_ECH.WORD = 0x38000bc7u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_1CH.WORD = 0x00402000u;
            TSIP.REG_A4H.WORD = 0x000007bdu;
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
            RX72M_RX72N_RX66N_func100(change_endian_long(0x42ececcbu), change_endian_long(0xd2320b20u), change_endian_long(0x302d9a28u), change_endian_long(0x0c50848eu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX72M_RX72N_RX66N_func102(change_endian_long(0x90a64ca8u), change_endian_long(0xeea8913fu), change_endian_long(0x106c6f93u), change_endian_long(0x0e4f1cefu));
                TSIP.REG_1B4H.WORD = 0x00000040u;
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
                TSIP.REG_ECH.WORD = 0xa6a6a6a6u;
                TSIP.REG_ECH.WORD = 0x00003420u;
                TSIP.REG_ECH.WORD = 0x0000b760u;
                TSIP.REG_ECH.WORD = 0x00000008u;
                TSIP.REG_ECH.WORD = 0x0000b780u;
                TSIP.REG_ECH.WORD = 0x00000004u;
                TSIP.REG_ECH.WORD = 0x0000b480u;
                TSIP.REG_ECH.WORD = 0x00000001u;
                TSIP.REG_ECH.WORD = 0x00000bdeu;
                for(jLoop = 0; jLoop <= 5; jLoop = jLoop + 1)
                {
                    TSIP.REG_ECH.WORD = 0x000037fbu;
                    TSIP.REG_ECH.WORD = 0x00000bbdu;
                    for(iLoop = 1; iLoop <= (WRAPPED_KEY_SIZE-2)/2; iLoop = iLoop + 1)
                    {
                        TSIP.REG_A4H.WORD = 0x00008a85u;
                        TSIP.REG_E0H.WORD = 0x81020000u;
                        TSIP.REG_00H.WORD = 0x0000180bu;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_00H.BIT.B25)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00001800u;
                        TSIP.REG_E0H.WORD = 0x8182001fu;
                        TSIP.REG_00H.WORD = 0x0000180bu;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_00H.BIT.B25)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00001800u;
                        TSIP.REG_E0H.WORD = 0x80040000u;
                        TSIP.REG_00H.WORD = 0x00008113u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_00H.BIT.B25)
                        {
                            /* waiting */
                        }
                        TSIP.REG_1CH.WORD = 0x00001800u;
                        TSIP.REG_ECH.WORD = 0x00000824u;
                        TSIP.REG_ECH.WORD = 0x00003c5fu;
                        TSIP.REG_ECH.WORD = 0x000027fcu;
                        TSIP.REG_ECH.WORD = 0x00003c7fu;
                        TSIP.REG_ECH.WORD = 0x000027fcu;
                        TSIP.REG_ECH.WORD = 0x00002c80u;
                        TSIP.REG_ECH.WORD = 0x00002fa0u;
                        RX72M_RX72N_RX66N_func101(change_endian_long(0x134985dfu), change_endian_long(0x0a713db1u), change_endian_long(0xad8e6418u), change_endian_long(0x65161741u));
                    }
                    TSIP.REG_ECH.WORD = 0x38000ba5u;
                    TSIP.REG_E0H.WORD = 0x00000080u;
                    TSIP.REG_1CH.WORD = 0x00260000u;
                    TSIP.REG_1CH.WORD = 0x00402000u;
                    TSIP.REG_ECH.WORD = 0x00002fc0u;
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x3e1f31e3u), change_endian_long(0xb0fb6f82u), change_endian_long(0x508b7c5cu), change_endian_long(0x8832f8a2u));
                }
                TSIP.REG_ECH.WORD = 0x38008bc0u;
                TSIP.REG_ECH.WORD = 0x00000006u;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_1CH.WORD = 0x00402000u;
                TSIP.REG_ECH.WORD = 0x00000bffu;
                TSIP.REG_ECH.WORD = 0x00003c1fu;
                TSIP.REG_ECH.WORD = 0x000027fcu;
                TSIP.REG_ECH.WORD = 0x00003c3fu;
                TSIP.REG_ECH.WORD = 0x00000bffu;
                TSIP.REG_ECH.WORD = 0x00000bdeu;
                TSIP.REG_ECH.WORD = 0x000037a5u;
                TSIP.REG_ECH.WORD = 0x00002fa0u;
                TSIP.REG_ECH.WORD = 0x0000b780u;
                TSIP.REG_ECH.WORD = 0x00000008u;
                for(iLoop = 0; iLoop < WRAPPED_KEY_SIZE; iLoop = iLoop + 2)
                {
                    TSIP.REG_ECH.WORD = 0x000008c6u;
                    TSIP.REG_ECH.WORD = 0x34202bddu;
                    TSIP.REG_ECH.WORD = 0x2000d0c0u;
                    TSIP.REG_ECH.WORD = 0x00007c06u;
                    TSIP.REG_1CH.WORD = 0x00602000u;
                    RX72M_RX72N_RX66N_func100(change_endian_long(0xa1971faeu), change_endian_long(0xd1c45d1cu), change_endian_long(0x0b7ba131u), change_endian_long(0xac3534a5u));
                    TSIP.REG_E0H.WORD = 0x8182001fu;
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
                    TSIP.REG_ECH.WORD = 0x000027fcu;
                    TSIP.REG_ECH.WORD = 0x00002fc0u;
                    RX72M_RX72N_RX66N_func101(change_endian_long(0xe9425755u), change_endian_long(0x6af62752u), change_endian_long(0xeadc938fu), change_endian_long(0x5afb0b67u));
                }
                TSIP.REG_ECH.WORD = 0x38000bddu;
                TSIP.REG_E0H.WORD = 0x00000080u;
                TSIP.REG_1CH.WORD = 0x00260000u;
                TSIP.REG_1CH.WORD = 0x00402000u;
                RX72M_RX72N_RX66N_func102(change_endian_long(0x0019f7b9u), change_endian_long(0x8af13356u), change_endian_long(0xff20dd36u), change_endian_long(0x4046b144u));
                TSIP.REG_1B4H.WORD = 0x00000040u;
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
 End of function ./input_dir/RX72M/RX72M_p6a.prc
 *********************************************************************************************************************/
#endif /* #if (TSIP_AES_128_KEY_WRAP == 1) || (TSIP_AES_256_KEY_WRAP == 1) */
