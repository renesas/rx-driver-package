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

#if TSIP_TLS == 1
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
* Function Name: R_TSIP_TlsGeneratePreMasterSecretWithEccP256KeySub
*******************************************************************************************************************/ /**
* @details       RX72M TLS PreMasterSecret Generation with ECC P-256 Key
* @param[in]     InData_PubKey
* @param[in]     InData_KeyIndex
* @param[out]    OutData_PreMasterSecretIndex
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @note          None
*/
e_tsip_err_t R_TSIP_TlsGeneratePreMasterSecretWithEccP256KeySub(uint32_t *InData_PubKey, uint32_t *InData_KeyIndex, uint32_t *OutData_PreMasterSecretIndex)
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
    if (0x0u != (TSIP.REG_1B8H.WORD & 0x1eu))
    {
        #if TSIP_MULTI_THREADING == 1
        TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
        #endif /* TSIP_MULTI_THREADING == 1 */
        return TSIP_ERR_RESOURCE_CONFLICT;
    }
    TSIP.REG_84H.WORD = 0x0000e801u;
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
    TSIP.REG_104H.WORD = 0x00000368u;
    TSIP.REG_E0H.WORD = 0x80040000u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_PubKey[0];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_PubKey[1];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_PubKey[2];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_PubKey[3];
    TSIP.REG_ECH.WORD = 0x00000bdeu;
    TSIP.REG_ECH.WORD = 0x000037e0u;
    TSIP.REG_ECH.WORD = 0x00008fe0u;
    TSIP.REG_ECH.WORD = 0xff000000u;
    TSIP.REG_ECH.WORD = 0x38008be0u;
    TSIP.REG_ECH.WORD = 0x31000000u;
    TSIP.REG_ECH.WORD = 0x1000d3c0u;
    TSIP.REG_ECH.WORD = 0x3800d811u;
    TSIP.REG_ECH.WORD = 0x2000d3c1u;
    TSIP.REG_ECH.WORD = 0x000037e0u;
    TSIP.REG_ECH.WORD = 0x00008fe0u;
    TSIP.REG_ECH.WORD = 0x0000ff00u;
    TSIP.REG_ECH.WORD = 0x38008be0u;
    TSIP.REG_ECH.WORD = 0x00000100u;
    TSIP.REG_ECH.WORD = 0x1000d3c2u;
    TSIP.REG_ECH.WORD = 0x38008bc0u;
    TSIP.REG_ECH.WORD = 0x00000007u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe2c9a2e1u), change_endian_long(0x6d650675u), change_endian_long(0x320c6fb9u), change_endian_long(0xa7162ed0u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0x47cccf56u), change_endian_long(0x409619ecu), change_endian_long(0x839f812eu), change_endian_long(0xe18296c7u));
        TSIP.REG_1B8H.WORD = 0x00000040u;
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
        TSIP.REG_C4H.WORD = 0x000c2b0du;
        TSIP.REG_E0H.WORD = 0x81040000u;
        TSIP.REG_00H.WORD = 0x00002813u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x00870001u;
        TSIP.REG_104H.WORD = 0x00000f62u;
        TSIP.REG_D0H.WORD = 0x00000300u;
        TSIP.REG_C4H.WORD = 0x02f087bfu;
        TSIP.REG_ECH.WORD = 0x0000094au;
        TSIP.REG_E0H.WORD = 0x8090000au;
        TSIP.REG_00H.WORD = 0x00008243u;
        for (iLoop = 0; iLoop < 16; iLoop = iLoop + 4)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_PubKey[iLoop+4 + 0];
            TSIP.REG_100H.WORD = InData_PubKey[iLoop+4 + 1];
            TSIP.REG_100H.WORD = InData_PubKey[iLoop+4 + 2];
            TSIP.REG_100H.WORD = InData_PubKey[iLoop+4 + 3];
        }
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
        TSIP.REG_100H.WORD = InData_PubKey[20];
        TSIP.REG_100H.WORD = InData_PubKey[21];
        TSIP.REG_100H.WORD = InData_PubKey[22];
        TSIP.REG_100H.WORD = InData_PubKey[23];
        TSIP.REG_C4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00002213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0x625192ceu), change_endian_long(0xec1c8d92u), change_endian_long(0x1a783300u), change_endian_long(0x39b98460u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func102(change_endian_long(0x3a2adc18u), change_endian_long(0xe62d4b91u), change_endian_long(0xfe759b3cu), change_endian_long(0xa5025270u));
            TSIP.REG_1B8H.WORD = 0x00000040u;
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
            TSIP.REG_ECH.WORD = 0x00000b9cu;
            OFS_ADR = 128;
            RX72M_RX72N_RX66N_func004(OFS_ADR);
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
            TSIP.REG_ECH.WORD = 0x38008be0u;
            TSIP.REG_ECH.WORD = 0x39000000u;
            TSIP.REG_ECH.WORD = 0x1000d3c0u;
            TSIP.REG_ECH.WORD = 0x3800d811u;
            TSIP.REG_ECH.WORD = 0x2000d3c1u;
            TSIP.REG_ECH.WORD = 0x000037e0u;
            TSIP.REG_ECH.WORD = 0x00008fe0u;
            TSIP.REG_ECH.WORD = 0x0000ff00u;
            TSIP.REG_ECH.WORD = 0x38008be0u;
            TSIP.REG_ECH.WORD = 0x00000100u;
            TSIP.REG_ECH.WORD = 0x1000d3c2u;
            TSIP.REG_ECH.WORD = 0x38008bc0u;
            TSIP.REG_ECH.WORD = 0x00000007u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            RX72M_RX72N_RX66N_func100(change_endian_long(0x0d15235bu), change_endian_long(0x9b7531b0u), change_endian_long(0xbe4c4d5bu), change_endian_long(0xba6fe7ebu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX72M_RX72N_RX66N_func102(change_endian_long(0x50f6248au), change_endian_long(0x64bce985u), change_endian_long(0x2aa4e81au), change_endian_long(0x0f758225u));
                TSIP.REG_1B8H.WORD = 0x00000040u;
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
                TSIP.REG_C4H.WORD = 0x000c2b0du;
                TSIP.REG_E0H.WORD = 0x81040000u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_28H.WORD = 0x00870001u;
                TSIP.REG_104H.WORD = 0x00000762u;
                TSIP.REG_D0H.WORD = 0x00000100u;
                TSIP.REG_C4H.WORD = 0x02f087bfu;
                TSIP.REG_00H.WORD = 0x00003223u;
                TSIP.REG_2CH.WORD = 0x00000011u;
                for (iLoop = 0; iLoop < 8; iLoop = iLoop + 4)
                {
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = InData_KeyIndex[iLoop+4 + 0];
                    TSIP.REG_100H.WORD = InData_KeyIndex[iLoop+4 + 1];
                    TSIP.REG_100H.WORD = InData_KeyIndex[iLoop+4 + 2];
                    TSIP.REG_100H.WORD = InData_KeyIndex[iLoop+4 + 3];
                }
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
                TSIP.REG_100H.WORD = InData_KeyIndex[12];
                TSIP.REG_100H.WORD = InData_KeyIndex[13];
                TSIP.REG_100H.WORD = InData_KeyIndex[14];
                TSIP.REG_100H.WORD = InData_KeyIndex[15];
                TSIP.REG_C4H.WORD = 0x00900c45u;
                TSIP.REG_00H.WORD = 0x00002213u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                RX72M_RX72N_RX66N_func100(change_endian_long(0x4e037bd8u), change_endian_long(0xd54c4151u), change_endian_long(0x9750ad76u), change_endian_long(0xeaee2671u));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    RX72M_RX72N_RX66N_func102(change_endian_long(0xa9095c1cu), change_endian_long(0x09ea6d73u), change_endian_long(0x6787ba76u), change_endian_long(0x1d398b0du));
                    TSIP.REG_1B8H.WORD = 0x00000040u;
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
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x47900e05u), change_endian_long(0x36ab5d31u), change_endian_long(0xc0e82310u), change_endian_long(0x00902069u));
                    TSIP.REG_24H.WORD = 0x0000dcd0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_24H.WORD = 0x000084d0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_24H.WORD = 0x00029008u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_34H.WORD = 0x00000800u;
                    TSIP.REG_24H.WORD = 0x8000c0c1u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_34H.WORD = 0x00000003u;
                    TSIP.REG_24H.WORD = 0x8000e808u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_34H.WORD = 0x00000003u;
                    TSIP.REG_24H.WORD = 0x800048d0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_ECH.WORD = 0x00000908u;
                    TSIP.REG_E0H.WORD = 0x81880008u;
                    TSIP.REG_00H.WORD = 0x00003823u;
                    TSIP.REG_2CH.WORD = 0x00000010u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_24H.WORD = 0x0000880cu;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_34H.WORD = 0x00000800u;
                    TSIP.REG_24H.WORD = 0x800050d0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_ECH.WORD = 0x0000b500u;
                    TSIP.REG_ECH.WORD = 0x00000020u;
                    TSIP.REG_E0H.WORD = 0x81880008u;
                    TSIP.REG_00H.WORD = 0x00003823u;
                    TSIP.REG_2CH.WORD = 0x00000010u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_24H.WORD = 0x0000880cu;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_34H.WORD = 0x00000800u;
                    TSIP.REG_24H.WORD = 0x800070d0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_24H.WORD = 0x00005cd0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_34H.WORD = 0x00000802u;
                    TSIP.REG_24H.WORD = 0x800088d0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_34H.WORD = 0x00000802u;
                    TSIP.REG_24H.WORD = 0x8000acd0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_18H.WORD = 0x00000004u;
                    TSIP.REG_38H.WORD = 0x00001000u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_18H.BIT.B10)
                    {
                        /* waiting */
                    }
                    TSIP.REG_18H.WORD = 0x00000000u;
                    TSIP.REG_34H.WORD = 0x00000c00u;
                    TSIP.REG_24H.WORD = 0x800094d0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_24H.WORD = 0x00001dc0u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_24H.WORD = 0x00000591u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_24H.BIT.B21)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001f00u;
                    TSIP.REG_1CH.WORD = 0x00210000u;
                    RX72M_RX72N_RX66N_func100(change_endian_long(0x627f81d8u), change_endian_long(0xddb6e828u), change_endian_long(0x9af0a259u), change_endian_long(0x0bcd9d27u));
                    TSIP.REG_1CH.WORD = 0x00400000u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    if (1u == (TSIP.REG_1CH.BIT.B22))
                    {
                        RX72M_RX72N_RX66N_func102(change_endian_long(0xeae6820fu), change_endian_long(0x8d403114u), change_endian_long(0x143905eeu), change_endian_long(0xe7780707u));
                        TSIP.REG_1B8H.WORD = 0x00000040u;
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
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xc68a65cdu), change_endian_long(0x2f7de180u), change_endian_long(0xbf9c38e7u), change_endian_long(0x36a89fddu));
                        TSIP.REG_24H.WORD = 0x00009cd0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x000019c0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x00000591u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x00000591u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x0000a0c1u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_18H.WORD = 0x00000004u;
                        TSIP.REG_38H.WORD = 0x000000b0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_18H.BIT.B10)
                        {
                            /* waiting */
                        }
                        TSIP.REG_18H.WORD = 0x00000000u;
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xdb99636bu), change_endian_long(0xc90bbdb7u), change_endian_long(0x9d89c2f5u), change_endian_long(0x00c589feu));
                        TSIP.REG_34H.WORD = 0x00000003u;
                        TSIP.REG_24H.WORD = 0x800048d0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x0000880cu;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x0000a8d0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_34H.WORD = 0x00000008u;
                        TSIP.REG_24H.WORD = 0x800050d0u;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x00004a0cu;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x0000480cu;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        TSIP.REG_24H.WORD = 0x0000480cu;
                        /* WAIT_LOOP */
                        while (0u != TSIP.REG_24H.BIT.B21)
                        {
                            /* waiting */
                        }
                        RX72M_RX72N_RX66N_func103();
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x1d032e9du), change_endian_long(0x949b49c6u), change_endian_long(0x15a6c938u), change_endian_long(0x0664fbebu));
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
                        TSIP.REG_C4H.WORD = 0x00082b8du;
                        TSIP.REG_E0H.WORD = 0x81040000u;
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
                        TSIP.REG_100H.WORD = change_endian_long(0x24c1817fu);
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
                        TSIP.REG_04H.WORD = 0x00000223u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        OutData_PreMasterSecretIndex[4] = TSIP.REG_100H.WORD;
                        OutData_PreMasterSecretIndex[5] = TSIP.REG_100H.WORD;
                        OutData_PreMasterSecretIndex[6] = TSIP.REG_100H.WORD;
                        OutData_PreMasterSecretIndex[7] = TSIP.REG_100H.WORD;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        OutData_PreMasterSecretIndex[8] = TSIP.REG_100H.WORD;
                        OutData_PreMasterSecretIndex[9] = TSIP.REG_100H.WORD;
                        OutData_PreMasterSecretIndex[10] = TSIP.REG_100H.WORD;
                        OutData_PreMasterSecretIndex[11] = TSIP.REG_100H.WORD;
                        RX72M_RX72N_RX66N_func100(change_endian_long(0xf60dfcb6u), change_endian_long(0x620acd15u), change_endian_long(0xe3aff537u), change_endian_long(0xd8368255u));
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
                        TSIP.REG_04H.WORD = 0x00000213u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        OutData_PreMasterSecretIndex[12] = TSIP.REG_100H.WORD;
                        OutData_PreMasterSecretIndex[13] = TSIP.REG_100H.WORD;
                        OutData_PreMasterSecretIndex[14] = TSIP.REG_100H.WORD;
                        OutData_PreMasterSecretIndex[15] = TSIP.REG_100H.WORD;
                        RX72M_RX72N_RX66N_func100(change_endian_long(0x53a991cbu), change_endian_long(0xd398f5c3u), change_endian_long(0xa5e5733cu), change_endian_long(0xfb8834b0u));
                        TSIP.REG_E0H.WORD = 0x81040000u;
                        TSIP.REG_04H.WORD = 0x00000613u;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        OutData_PreMasterSecretIndex[0] = TSIP.REG_100H.WORD;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        OutData_PreMasterSecretIndex[1] = TSIP.REG_100H.WORD;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        OutData_PreMasterSecretIndex[2] = TSIP.REG_100H.WORD;
                        /* WAIT_LOOP */
                        while (1u != TSIP.REG_04H.BIT.B30)
                        {
                            /* waiting */
                        }
                        OutData_PreMasterSecretIndex[3] = TSIP.REG_100H.WORD;
                        RX72M_RX72N_RX66N_func102(change_endian_long(0x3d6c4ebeu), change_endian_long(0x16079345u), change_endian_long(0x4f1d609fu), change_endian_long(0x209a6dd3u));
                        TSIP.REG_1B8H.WORD = 0x00000040u;
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
}
/**********************************************************************************************************************
 End of function ./input_dir/RX72M/RX72M_pe8.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_TLS == 1 */
