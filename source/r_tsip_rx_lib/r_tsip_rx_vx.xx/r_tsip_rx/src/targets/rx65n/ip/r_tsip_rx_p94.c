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
* @details       RX65NHU RSA-2048 DH Key Exchange
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
    RX65NHU_func100(change_endian_long(0x5e5d7bdau), change_endian_long(0x85c21c78u), change_endian_long(0x65028e99u), change_endian_long(0xf4b53771u));
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
    RX65NHU_func100(change_endian_long(0xa8f728d0u), change_endian_long(0xce52e656u), change_endian_long(0x57bd69f8u), change_endian_long(0x327e60feu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0xdd11b6bdu), change_endian_long(0xbbcc85bfu), change_endian_long(0x53da0c57u), change_endian_long(0x2f7a7995u));
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
        RX65NHU_func100(change_endian_long(0xd26c3ae9u), change_endian_long(0x321d75c3u), change_endian_long(0xaf2eda0eu), change_endian_long(0xd015d934u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func102(change_endian_long(0xb37f80d7u), change_endian_long(0x75704226u), change_endian_long(0x6fcbefbfu), change_endian_long(0xece77078u));
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
                RX65NHU_func100(change_endian_long(0xd5a439acu), change_endian_long(0x149e3468u), change_endian_long(0xa6d0f19eu), change_endian_long(0x7cb3c0cbu));
                RX65NHU_func103();
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
                RX65NHU_func101(change_endian_long(0xd07d583au), change_endian_long(0x66b8698au), change_endian_long(0x8b68b449u), change_endian_long(0x7765c486u));
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
            RX65NHU_func100(change_endian_long(0x3cd5418au), change_endian_long(0x8e6eadbdu), change_endian_long(0xbce8d50cu), change_endian_long(0xbfe6fbcau));
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
            RX65NHU_func100(change_endian_long(0x58991384u), change_endian_long(0x6e95c112u), change_endian_long(0x04994615u), change_endian_long(0x64a2789bu));
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
            RX65NHU_func100(change_endian_long(0x27b41629u), change_endian_long(0x11978485u), change_endian_long(0xd4db8f06u), change_endian_long(0x9fe147b6u));
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
            RX65NHU_func100(change_endian_long(0xcda0d804u), change_endian_long(0x58f71021u), change_endian_long(0x5ed3a46fu), change_endian_long(0x58897f1bu));
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
            RX65NHU_func100(change_endian_long(0x2d6162a8u), change_endian_long(0xd1f48272u), change_endian_long(0x0f374c9fu), change_endian_long(0x0be04f3du));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX65NHU_func102(change_endian_long(0xd437885du), change_endian_long(0x4ceb6f04u), change_endian_long(0x340aef54u), change_endian_long(0xa343164fu));
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
                RX65NHU_func100(change_endian_long(0x52230818u), change_endian_long(0x38cfe241u), change_endian_long(0xeefb21a5u), change_endian_long(0x61c88cc7u));
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
                RX65NHU_func102(change_endian_long(0x89ce22dau), change_endian_long(0x21fd0e6bu), change_endian_long(0x1ab9f1d9u), change_endian_long(0x8df8e225u));
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
 End of function ./input_dir/RX65NHU/RX65NHU_p94_r1.prc
 *********************************************************************************************************************/
