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
* Function Name: R_TSIP_Rsa2048DhKeyAgreementSub
*******************************************************************************************************************/ /**
* @details       RX72M RSA-2048 DH Key Exchange
* @param[in]     InData_KeyIndex
* @param[in]     InData_PrimNumIndex
* @param[in]     InData_Message
* @param[in]     InData_ModExp
* @param[out]    OutData_ModExp
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_Rsa2048DhKeyAgreementSub(uint32_t *InData_KeyIndex, uint32_t *InData_PrimNumIndex, uint32_t *InData_Message, uint32_t *InData_ModExp, uint32_t *OutData_ModExp)
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
    TSIP.REG_84H.WORD = 0x00009401u;
    TSIP.REG_108H.WORD = 0x00000000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x3148c41du), change_endian_long(0x359924e3u), change_endian_long(0x6af292d5u), change_endian_long(0x4c5a1233u));
    TSIP.REG_104H.WORD = 0x00000362u;
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
    TSIP.REG_C4H.WORD = 0x000c0b0cu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x4e2279dbu);
    TSIP.REG_C4H.WORD = 0x02f8073du;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[4];
    TSIP.REG_100H.WORD = InData_KeyIndex[5];
    TSIP.REG_100H.WORD = InData_KeyIndex[6];
    TSIP.REG_100H.WORD = InData_KeyIndex[7];
    TSIP.REG_C4H.WORD = 0x000007bdu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[8];
    TSIP.REG_100H.WORD = InData_KeyIndex[9];
    TSIP.REG_100H.WORD = InData_KeyIndex[10];
    TSIP.REG_100H.WORD = InData_KeyIndex[11];
    TSIP.REG_C4H.WORD = 0x00900c45u;
    TSIP.REG_00H.WORD = 0x00002213u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x43be0f2fu), change_endian_long(0x38620e77u), change_endian_long(0x5544fc8cu), change_endian_long(0xdbb73f80u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0xda7769e9u), change_endian_long(0xa05cc0eau), change_endian_long(0x3555c98au), change_endian_long(0xa31d6e72u));
        TSIP.REG_1B8H.WORD = 0x00000040u;
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
        TSIP.REG_C4H.WORD = 0x200c3b0du;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_PrimNumIndex[0];
        TSIP.REG_100H.WORD = InData_PrimNumIndex[1];
        TSIP.REG_100H.WORD = InData_PrimNumIndex[2];
        TSIP.REG_100H.WORD = InData_PrimNumIndex[3];
        TSIP.REG_C4H.WORD = 0x000c0b0cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x3770f901u);
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_104H.WORD = 0x0000bf62u;
        TSIP.REG_D0H.WORD = 0x00000f00u;
        TSIP.REG_C4H.WORD = 0x02f0873fu;
        for(iLoop=4; iLoop<68; iLoop=iLoop+4)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_PrimNumIndex[iLoop + 0];
            TSIP.REG_100H.WORD = InData_PrimNumIndex[iLoop + 1];
            TSIP.REG_100H.WORD = InData_PrimNumIndex[iLoop + 2];
            TSIP.REG_100H.WORD = InData_PrimNumIndex[iLoop + 3];
        }
        TSIP.REG_00H.WORD = 0x00013203u;
        TSIP.REG_2CH.WORD = 0x00000014u;
        TSIP.REG_D0H.WORD = 0x00000f00u;
        TSIP.REG_C4H.WORD = 0x00f087bfu;
        for(iLoop=68; iLoop<132; iLoop=iLoop+4)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_PrimNumIndex[iLoop + 0];
            TSIP.REG_100H.WORD = InData_PrimNumIndex[iLoop + 1];
            TSIP.REG_100H.WORD = InData_PrimNumIndex[iLoop + 2];
            TSIP.REG_100H.WORD = InData_PrimNumIndex[iLoop + 3];
        }
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_D0H.WORD = 0x00000f00u;
        TSIP.REG_C4H.WORD = 0x00f0873fu;
        for(iLoop=132; iLoop<196; iLoop=iLoop+4)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_PrimNumIndex[iLoop + 0];
            TSIP.REG_100H.WORD = InData_PrimNumIndex[iLoop + 1];
            TSIP.REG_100H.WORD = InData_PrimNumIndex[iLoop + 2];
            TSIP.REG_100H.WORD = InData_PrimNumIndex[iLoop + 3];
        }
        TSIP.REG_104H.WORD = 0x00000362u;
        TSIP.REG_C4H.WORD = 0x000007bdu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_PrimNumIndex[196];
        TSIP.REG_100H.WORD = InData_PrimNumIndex[197];
        TSIP.REG_100H.WORD = InData_PrimNumIndex[198];
        TSIP.REG_100H.WORD = InData_PrimNumIndex[199];
        TSIP.REG_C4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00002213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0x0193b470u), change_endian_long(0x5077f8a3u), change_endian_long(0x46dafc82u), change_endian_long(0x5459e7eeu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func102(change_endian_long(0x4a8199f8u), change_endian_long(0x2ea7a264u), change_endian_long(0x6618a94du), change_endian_long(0xe528e7ffu));
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
            TSIP.REG_18H.WORD = 0x00000004u;
            TSIP.REG_24H.WORD = 0x00004404u;
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
            TSIP.REG_2CH.WORD = 0x00000010u;
            for(iLoop = 0; iLoop < 64; iLoop = iLoop + 4)
            {
                RX72M_RX72N_RX66N_func100(change_endian_long(0x50e486bau), change_endian_long(0xe04ad328u), change_endian_long(0x4bda9b76u), change_endian_long(0x12e93e21u));
                RX72M_RX72N_RX66N_func103();
                TSIP.REG_104H.WORD = 0x00000052u;
                TSIP.REG_C4H.WORD = 0x01000c84u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
                TSIP.REG_00H.WORD = 0x00003213u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0x4b1204bcu), change_endian_long(0x57028472u), change_endian_long(0x3099dd21u), change_endian_long(0xe957b9c3u));
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
            TSIP.REG_24H.WORD = 0x0000c0c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x00009008u;
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
            TSIP.REG_24H.WORD = 0x00000581u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x00000581u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x0000c0c1u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            RX72M_RX72N_RX66N_func100(change_endian_long(0x0f1a16b3u), change_endian_long(0xa0107e6fu), change_endian_long(0x68f2069eu), change_endian_long(0x54ad58dbu));
            TSIP.REG_ECH.WORD = 0x00000800u;
            TSIP.REG_E0H.WORD = 0x80c00000u;
            TSIP.REG_00H.WORD = 0x00018303u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            TSIP.REG_104H.WORD = 0x00003f67u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_Message[0];
            TSIP.REG_100H.WORD = InData_Message[1];
            TSIP.REG_100H.WORD = InData_Message[2];
            TSIP.REG_100H.WORD = InData_Message[3];
            TSIP.REG_100H.WORD = InData_Message[4];
            TSIP.REG_100H.WORD = InData_Message[5];
            TSIP.REG_100H.WORD = InData_Message[6];
            TSIP.REG_100H.WORD = InData_Message[7];
            TSIP.REG_100H.WORD = InData_Message[8];
            TSIP.REG_100H.WORD = InData_Message[9];
            TSIP.REG_100H.WORD = InData_Message[10];
            TSIP.REG_100H.WORD = InData_Message[11];
            TSIP.REG_100H.WORD = InData_Message[12];
            TSIP.REG_100H.WORD = InData_Message[13];
            TSIP.REG_100H.WORD = InData_Message[14];
            TSIP.REG_100H.WORD = InData_Message[15];
            TSIP.REG_100H.WORD = InData_Message[16];
            TSIP.REG_100H.WORD = InData_Message[17];
            TSIP.REG_100H.WORD = InData_Message[18];
            TSIP.REG_100H.WORD = InData_Message[19];
            TSIP.REG_100H.WORD = InData_Message[20];
            TSIP.REG_100H.WORD = InData_Message[21];
            TSIP.REG_100H.WORD = InData_Message[22];
            TSIP.REG_100H.WORD = InData_Message[23];
            TSIP.REG_100H.WORD = InData_Message[24];
            TSIP.REG_100H.WORD = InData_Message[25];
            TSIP.REG_100H.WORD = InData_Message[26];
            TSIP.REG_100H.WORD = InData_Message[27];
            TSIP.REG_100H.WORD = InData_Message[28];
            TSIP.REG_100H.WORD = InData_Message[29];
            TSIP.REG_100H.WORD = InData_Message[30];
            TSIP.REG_100H.WORD = InData_Message[31];
            TSIP.REG_100H.WORD = InData_Message[32];
            TSIP.REG_100H.WORD = InData_Message[33];
            TSIP.REG_100H.WORD = InData_Message[34];
            TSIP.REG_100H.WORD = InData_Message[35];
            TSIP.REG_100H.WORD = InData_Message[36];
            TSIP.REG_100H.WORD = InData_Message[37];
            TSIP.REG_100H.WORD = InData_Message[38];
            TSIP.REG_100H.WORD = InData_Message[39];
            TSIP.REG_100H.WORD = InData_Message[40];
            TSIP.REG_100H.WORD = InData_Message[41];
            TSIP.REG_100H.WORD = InData_Message[42];
            TSIP.REG_100H.WORD = InData_Message[43];
            TSIP.REG_100H.WORD = InData_Message[44];
            TSIP.REG_100H.WORD = InData_Message[45];
            TSIP.REG_100H.WORD = InData_Message[46];
            TSIP.REG_100H.WORD = InData_Message[47];
            TSIP.REG_100H.WORD = InData_Message[48];
            TSIP.REG_100H.WORD = InData_Message[49];
            TSIP.REG_100H.WORD = InData_Message[50];
            TSIP.REG_100H.WORD = InData_Message[51];
            TSIP.REG_100H.WORD = InData_Message[52];
            TSIP.REG_100H.WORD = InData_Message[53];
            TSIP.REG_100H.WORD = InData_Message[54];
            TSIP.REG_100H.WORD = InData_Message[55];
            TSIP.REG_100H.WORD = InData_Message[56];
            TSIP.REG_100H.WORD = InData_Message[57];
            TSIP.REG_100H.WORD = InData_Message[58];
            TSIP.REG_100H.WORD = InData_Message[59];
            TSIP.REG_100H.WORD = InData_Message[60];
            TSIP.REG_100H.WORD = InData_Message[61];
            TSIP.REG_100H.WORD = InData_Message[62];
            TSIP.REG_100H.WORD = InData_Message[63];
            TSIP.REG_E0H.WORD = 0x00000100u;
            TSIP.REG_ECH.WORD = 0x0000b7e0u;
            TSIP.REG_ECH.WORD = 0x000000fcu;
            TSIP.REG_18H.WORD = 0x00000004u;
            TSIP.REG_38H.WORD = 0x00000248u;
            TSIP.REG_E0H.WORD = 0x81c0001fu;
            TSIP.REG_00H.WORD = 0x00013803u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_18H.BIT.B10)
            {
                /* waiting */
            }
            TSIP.REG_18H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x00000000u;
            RX72M_RX72N_RX66N_func100(change_endian_long(0x5f3311c2u), change_endian_long(0x3167805eu), change_endian_long(0xc751fe7du), change_endian_long(0x9ae70c78u));
            TSIP.REG_00H.WORD = 0x000023f3u;
            TSIP.REG_2CH.WORD = 0x00000020u;
            TSIP.REG_D0H.WORD = 0x00000e00u;
            TSIP.REG_C4H.WORD = 0x02e08887u;
            TSIP.REG_04H.WORD = 0x000002f2u;
            for(iLoop=0; iLoop<60; iLoop=iLoop+4)
            {
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_ModExp[iLoop + 0] = TSIP.REG_100H.WORD;
                OutData_ModExp[iLoop + 1] = TSIP.REG_100H.WORD;
                OutData_ModExp[iLoop + 2] = TSIP.REG_100H.WORD;
                OutData_ModExp[iLoop + 3] = TSIP.REG_100H.WORD;
            }
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX72M_RX72N_RX66N_func100(change_endian_long(0xc1c7c97fu), change_endian_long(0x8e402418u), change_endian_long(0x77874788u), change_endian_long(0x5e835950u));
            TSIP.REG_104H.WORD = 0x00000052u;
            TSIP.REG_C4H.WORD = 0x00450944u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_C4H.WORD = 0x00e00885u;
            TSIP.REG_00H.WORD = 0x00002313u;
            TSIP.REG_04H.WORD = 0x00000212u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_ModExp[iLoop + 0] = TSIP.REG_100H.WORD;
            OutData_ModExp[iLoop + 1] = TSIP.REG_100H.WORD;
            OutData_ModExp[iLoop + 2] = TSIP.REG_100H.WORD;
            OutData_ModExp[iLoop + 3] = TSIP.REG_100H.WORD;
            RX72M_RX72N_RX66N_func100(change_endian_long(0x01fda277u), change_endian_long(0x98e5af03u), change_endian_long(0x724ac7d4u), change_endian_long(0xeb1a25a3u));
            TSIP.REG_104H.WORD = 0x00000052u;
            TSIP.REG_C4H.WORD = 0x00000cc4u;
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
            OutData_ModExp[64] = TSIP.REG_100H.WORD;
            OutData_ModExp[65] = TSIP.REG_100H.WORD;
            OutData_ModExp[66] = TSIP.REG_100H.WORD;
            OutData_ModExp[67] = TSIP.REG_100H.WORD;
            TSIP.REG_00H.WORD = 0x000032f3u;
            TSIP.REG_2CH.WORD = 0x00000010u;
            TSIP.REG_104H.WORD = 0x00003b62u;
            TSIP.REG_D0H.WORD = 0x00000e00u;
            TSIP.REG_C4H.WORD = 0x02f08887u;
            for(iLoop=0; iLoop<60; iLoop=iLoop+4)
            {
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_ModExp[iLoop + 0];
                TSIP.REG_100H.WORD = InData_ModExp[iLoop + 1];
                TSIP.REG_100H.WORD = InData_ModExp[iLoop + 2];
                TSIP.REG_100H.WORD = InData_ModExp[iLoop + 3];
            }
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_104H.WORD = 0x00000052u;
            TSIP.REG_C4H.WORD = 0x00450944u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_104H.WORD = 0x00000362u;
            TSIP.REG_C4H.WORD = 0x00f00885u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_ModExp[iLoop + 0];
            TSIP.REG_100H.WORD = InData_ModExp[iLoop + 1];
            TSIP.REG_100H.WORD = InData_ModExp[iLoop + 2];
            TSIP.REG_100H.WORD = InData_ModExp[iLoop + 3];
            TSIP.REG_00H.WORD = 0x00003213u;
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
            TSIP.REG_100H.WORD = InData_ModExp[64];
            TSIP.REG_100H.WORD = InData_ModExp[65];
            TSIP.REG_100H.WORD = InData_ModExp[66];
            TSIP.REG_100H.WORD = InData_ModExp[67];
            TSIP.REG_C4H.WORD = 0x00900c45u;
            TSIP.REG_00H.WORD = 0x00002213u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX72M_RX72N_RX66N_func100(change_endian_long(0xcba06ac9u), change_endian_long(0xa4ac926du), change_endian_long(0x775cd77bu), change_endian_long(0xe74f6d6cu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX72M_RX72N_RX66N_func102(change_endian_long(0xaf83b7c8u), change_endian_long(0x7e53c3e2u), change_endian_long(0xd5083ff8u), change_endian_long(0x57ac2b1au));
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
                TSIP.REG_18H.WORD = 0x00000004u;
                TSIP.REG_24H.WORD = 0x00004404u;
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
                RX72M_RX72N_RX66N_func100(change_endian_long(0x5cca8dd9u), change_endian_long(0x7f14dd91u), change_endian_long(0x1af2ed66u), change_endian_long(0xe4661ac2u));
                TSIP.REG_E0H.WORD = 0x00000100u;
                TSIP.REG_ECH.WORD = 0x0000b7e0u;
                TSIP.REG_ECH.WORD = 0x000000fcu;
                TSIP.REG_18H.WORD = 0x00000004u;
                TSIP.REG_38H.WORD = 0x00000248u;
                TSIP.REG_E0H.WORD = 0x81c0001fu;
                TSIP.REG_00H.WORD = 0x00013803u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_18H.BIT.B10)
                {
                    /* waiting */
                }
                TSIP.REG_18H.WORD = 0x00000000u;
                TSIP.REG_E0H.WORD = 0x00000000u;
                RX72M_RX72N_RX66N_func102(change_endian_long(0x780c404au), change_endian_long(0xeb37f5c9u), change_endian_long(0x938b8e0bu), change_endian_long(0xa1322a0au));
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
/**********************************************************************************************************************
 End of function ./input_dir/RX72M/RX72M_p94_r1.prc
 *********************************************************************************************************************/
