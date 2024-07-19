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

#if TSIP_PRV_USE_RSA_3072 == 1
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
* Function Name: R_TSIP_Rsa3072ModularExponentEncryptSub
*******************************************************************************************************************/ /**
* @details       RX671 RSA-3072 Encryption
* @param[in]     InData_KeyIndex
* @param[in]     InData_Text
* @param[out]    OutData_Text
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_Rsa3072ModularExponentEncryptSub(uint32_t *InData_KeyIndex, uint32_t *InData_Text, uint32_t *OutData_Text)
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
    TSIP.REG_84H.WORD = 0x00007901u;
    TSIP.REG_108H.WORD = 0x00000000u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x0819a788u);
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x00040804u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_104H.WORD = 0x00006362u;
    TSIP.REG_D0H.WORD = 0x00001800u;
    TSIP.REG_C4H.WORD = 0x02e0888fu;
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    for(iLoop=0; iLoop<32; iLoop=iLoop+4)
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
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    for(iLoop=32; iLoop<64; iLoop=iLoop+4)
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
    TSIP.REG_00H.WORD = 0x00003283u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    for(iLoop=64; iLoop<96; iLoop=iLoop+4)
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
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[iLoop+4 + 0];
    TSIP.REG_100H.WORD = InData_KeyIndex[iLoop+4 + 1];
    TSIP.REG_100H.WORD = InData_KeyIndex[iLoop+4 + 2];
    TSIP.REG_100H.WORD = InData_KeyIndex[iLoop+4 + 3];
    TSIP.REG_E0H.WORD = 0x80010280u;
    TSIP.REG_00H.WORD = 0x00008207u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_00H.WORD = 0x0000020fu;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_104H.WORD = 0x00000362u;
    TSIP.REG_C4H.WORD = 0x000007bdu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[iLoop+8 + 0];
    TSIP.REG_100H.WORD = InData_KeyIndex[iLoop+8 + 1];
    TSIP.REG_100H.WORD = InData_KeyIndex[iLoop+8 + 2];
    TSIP.REG_100H.WORD = InData_KeyIndex[iLoop+8 + 3];
    TSIP.REG_C4H.WORD = 0x00900c45u;
    TSIP.REG_00H.WORD = 0x00002213u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX671_func100(change_endian_long(0x46894c9du), change_endian_long(0xb23e3cefu), change_endian_long(0x86f6c325u), change_endian_long(0x5fb6d987u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func102(change_endian_long(0x2398c0f0u), change_endian_long(0x19290834u), change_endian_long(0x1f6ef0c7u), change_endian_long(0xb285cfdeu));
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
        RX671_func100(change_endian_long(0x6ca3ac1bu), change_endian_long(0x44fa6048u), change_endian_long(0x0ae6a97bu), change_endian_long(0xe8e50917u));
        RX671_func103();
        RX671_func100(change_endian_long(0x44201370u), change_endian_long(0x6f730b4eu), change_endian_long(0x5f16e6fbu), change_endian_long(0x51c3a687u));
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x010c0c04u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        RX671_func100(change_endian_long(0xd97a2d0au), change_endian_long(0xff66ff62u), change_endian_long(0x4eb58edcu), change_endian_long(0xd11b838fu));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x7775adb6u);
        RX671_func080();
        RX671_func100(change_endian_long(0x0260698cu), change_endian_long(0xf289cc14u), change_endian_long(0xd69cc921u), change_endian_long(0x6a5ec75eu));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        RX671_func319(752);
        RX671_func100(change_endian_long(0x802b904fu), change_endian_long(0x4bef6c54u), change_endian_long(0x90412a54u), change_endian_long(0x0b6da93au));
        RX671_func314(752+32);
        RX671_func100(change_endian_long(0x83d0eebeu), change_endian_long(0x1e89157bu), change_endian_long(0x12927eeeu), change_endian_long(0x9c6e7956u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x899d7651u);
        RX671_func080();
        RX671_func100(change_endian_long(0x3d48cd01u), change_endian_long(0xbf00d540u), change_endian_long(0x1f93fc9fu), change_endian_long(0x385c094fu));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        RX671_func319(716);
        RX671_func100(change_endian_long(0x75b7e7e7u), change_endian_long(0x55bf0e31u), change_endian_long(0xe7b9b625u), change_endian_long(0x912a3afbu));
        RX671_func314(716+32);
        RX671_func100(change_endian_long(0x8586f5f0u), change_endian_long(0xf6b7ce2du), change_endian_long(0xd160557fu), change_endian_long(0x0b544aa6u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xfa21a8a9u);
        RX671_func080();
        RX671_func100(change_endian_long(0x47d8f184u), change_endian_long(0x0bf00ac2u), change_endian_long(0xd0488f08u), change_endian_long(0xa45f0d02u));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000024u;
        RX671_func319(680);
        RX671_func100(change_endian_long(0x98ffe6ecu), change_endian_long(0xec27f84fu), change_endian_long(0xd156aff7u), change_endian_long(0x0e6724beu));
        RX671_func314(680+32);
        TSIP.REG_00H.WORD = 0x0000037fu;
        TSIP.REG_2CH.WORD = 0x00000024u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_E0H.WORD = 0x800103e0u;
        TSIP.REG_00H.WORD = 0x00008307u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_ECH.WORD = 0x3800dbe0u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        TSIP.REG_104H.WORD = 0x00001f67u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_Text[0];
        TSIP.REG_100H.WORD = InData_Text[1];
        TSIP.REG_100H.WORD = InData_Text[2];
        TSIP.REG_100H.WORD = InData_Text[3];
        TSIP.REG_100H.WORD = InData_Text[4];
        TSIP.REG_100H.WORD = InData_Text[5];
        TSIP.REG_100H.WORD = InData_Text[6];
        TSIP.REG_100H.WORD = InData_Text[7];
        TSIP.REG_100H.WORD = InData_Text[8];
        TSIP.REG_100H.WORD = InData_Text[9];
        TSIP.REG_100H.WORD = InData_Text[10];
        TSIP.REG_100H.WORD = InData_Text[11];
        TSIP.REG_100H.WORD = InData_Text[12];
        TSIP.REG_100H.WORD = InData_Text[13];
        TSIP.REG_100H.WORD = InData_Text[14];
        TSIP.REG_100H.WORD = InData_Text[15];
        TSIP.REG_100H.WORD = InData_Text[16];
        TSIP.REG_100H.WORD = InData_Text[17];
        TSIP.REG_100H.WORD = InData_Text[18];
        TSIP.REG_100H.WORD = InData_Text[19];
        TSIP.REG_100H.WORD = InData_Text[20];
        TSIP.REG_100H.WORD = InData_Text[21];
        TSIP.REG_100H.WORD = InData_Text[22];
        TSIP.REG_100H.WORD = InData_Text[23];
        TSIP.REG_100H.WORD = InData_Text[24];
        TSIP.REG_100H.WORD = InData_Text[25];
        TSIP.REG_100H.WORD = InData_Text[26];
        TSIP.REG_100H.WORD = InData_Text[27];
        TSIP.REG_100H.WORD = InData_Text[28];
        TSIP.REG_100H.WORD = InData_Text[29];
        TSIP.REG_100H.WORD = InData_Text[30];
        TSIP.REG_100H.WORD = InData_Text[31];
        TSIP.REG_24H.WORD = 0x0000a0d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_2CH.WORD = 0x00000014u;
        TSIP.REG_104H.WORD = 0x00003f67u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_Text[32];
        TSIP.REG_100H.WORD = InData_Text[33];
        TSIP.REG_100H.WORD = InData_Text[34];
        TSIP.REG_100H.WORD = InData_Text[35];
        TSIP.REG_100H.WORD = InData_Text[36];
        TSIP.REG_100H.WORD = InData_Text[37];
        TSIP.REG_100H.WORD = InData_Text[38];
        TSIP.REG_100H.WORD = InData_Text[39];
        TSIP.REG_100H.WORD = InData_Text[40];
        TSIP.REG_100H.WORD = InData_Text[41];
        TSIP.REG_100H.WORD = InData_Text[42];
        TSIP.REG_100H.WORD = InData_Text[43];
        TSIP.REG_100H.WORD = InData_Text[44];
        TSIP.REG_100H.WORD = InData_Text[45];
        TSIP.REG_100H.WORD = InData_Text[46];
        TSIP.REG_100H.WORD = InData_Text[47];
        TSIP.REG_100H.WORD = InData_Text[48];
        TSIP.REG_100H.WORD = InData_Text[49];
        TSIP.REG_100H.WORD = InData_Text[50];
        TSIP.REG_100H.WORD = InData_Text[51];
        TSIP.REG_100H.WORD = InData_Text[52];
        TSIP.REG_100H.WORD = InData_Text[53];
        TSIP.REG_100H.WORD = InData_Text[54];
        TSIP.REG_100H.WORD = InData_Text[55];
        TSIP.REG_100H.WORD = InData_Text[56];
        TSIP.REG_100H.WORD = InData_Text[57];
        TSIP.REG_100H.WORD = InData_Text[58];
        TSIP.REG_100H.WORD = InData_Text[59];
        TSIP.REG_100H.WORD = InData_Text[60];
        TSIP.REG_100H.WORD = InData_Text[61];
        TSIP.REG_100H.WORD = InData_Text[62];
        TSIP.REG_100H.WORD = InData_Text[63];
        TSIP.REG_100H.WORD = InData_Text[64];
        TSIP.REG_100H.WORD = InData_Text[65];
        TSIP.REG_100H.WORD = InData_Text[66];
        TSIP.REG_100H.WORD = InData_Text[67];
        TSIP.REG_100H.WORD = InData_Text[68];
        TSIP.REG_100H.WORD = InData_Text[69];
        TSIP.REG_100H.WORD = InData_Text[70];
        TSIP.REG_100H.WORD = InData_Text[71];
        TSIP.REG_100H.WORD = InData_Text[72];
        TSIP.REG_100H.WORD = InData_Text[73];
        TSIP.REG_100H.WORD = InData_Text[74];
        TSIP.REG_100H.WORD = InData_Text[75];
        TSIP.REG_100H.WORD = InData_Text[76];
        TSIP.REG_100H.WORD = InData_Text[77];
        TSIP.REG_100H.WORD = InData_Text[78];
        TSIP.REG_100H.WORD = InData_Text[79];
        TSIP.REG_100H.WORD = InData_Text[80];
        TSIP.REG_100H.WORD = InData_Text[81];
        TSIP.REG_100H.WORD = InData_Text[82];
        TSIP.REG_100H.WORD = InData_Text[83];
        TSIP.REG_100H.WORD = InData_Text[84];
        TSIP.REG_100H.WORD = InData_Text[85];
        TSIP.REG_100H.WORD = InData_Text[86];
        TSIP.REG_100H.WORD = InData_Text[87];
        TSIP.REG_100H.WORD = InData_Text[88];
        TSIP.REG_100H.WORD = InData_Text[89];
        TSIP.REG_100H.WORD = InData_Text[90];
        TSIP.REG_100H.WORD = InData_Text[91];
        TSIP.REG_100H.WORD = InData_Text[92];
        TSIP.REG_100H.WORD = InData_Text[93];
        TSIP.REG_100H.WORD = InData_Text[94];
        TSIP.REG_100H.WORD = InData_Text[95];
        TSIP.REG_28H.WORD = 0x009f0001u;
        RX671_func100(change_endian_long(0x7ebea6dbu), change_endian_long(0x38236b69u), change_endian_long(0x50200dc3u), change_endian_long(0x686cb965u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x49880ba2u);
        RX671_func080();
        RX671_func100(change_endian_long(0xba890266u), change_endian_long(0xf0a53d19u), change_endian_long(0x9ef4fd05u), change_endian_long(0xf3a6c1f9u));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        RX671_func319(72);
        RX671_func100(change_endian_long(0x35b207bfu), change_endian_long(0xe3f72db5u), change_endian_long(0xce938a5eu), change_endian_long(0xd6c624b7u));
        RX671_func314(72+32);
        RX671_func100(change_endian_long(0x27a9bc1fu), change_endian_long(0x56bb9b5eu), change_endian_long(0xa94c4c2bu), change_endian_long(0xc000b399u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x064417e5u);
        RX671_func080();
        RX671_func100(change_endian_long(0xe1fc7879u), change_endian_long(0xe63190bcu), change_endian_long(0x163710c9u), change_endian_long(0xa1df638fu));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000025u;
        RX671_func319(36);
        RX671_func100(change_endian_long(0x02dae573u), change_endian_long(0xb11d9cceu), change_endian_long(0xc4bc550fu), change_endian_long(0x05e17df5u));
        RX671_func314(36+32);
        RX671_func100(change_endian_long(0xe3600626u), change_endian_long(0xd1253580u), change_endian_long(0x067fe23du), change_endian_long(0xec574d52u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x84992ba5u);
        RX671_func080();
        RX671_func100(change_endian_long(0x45d22406u), change_endian_long(0x64da61c4u), change_endian_long(0x825d4651u), change_endian_long(0x2f01984fu));
        TSIP.REG_00H.WORD = 0x00002383u;
        TSIP.REG_2CH.WORD = 0x00000024u;
        RX671_func319(0);
        RX671_func100(change_endian_long(0x9e2d0916u), change_endian_long(0x42388385u), change_endian_long(0x283b5262u), change_endian_long(0x7ef00a55u));
        RX671_func314(0+32);
        RX671_func100(change_endian_long(0x68147a55u), change_endian_long(0x966db521u), change_endian_long(0x3b2399afu), change_endian_long(0xf1f5b33fu));
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_24H.WORD = 0x000040d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x000005c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x04001991u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x000049c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x02001191u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00a10000u;
        TSIP.REG_24H.WORD = 0x000005c0u;
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
        TSIP.REG_28H.WORD = 0x009f0001u;
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xfa21a8a9u);
        RX671_func080();
        RX671_func100(change_endian_long(0x01aff458u), change_endian_long(0x784c586eu), change_endian_long(0xaedaa891u), change_endian_long(0xb774e954u));
        TSIP.REG_00H.WORD = 0x00003283u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        RX671_func320(680);
        TSIP.REG_24H.WORD = 0x000011c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x899d7651u);
        RX671_func080();
        RX671_func100(change_endian_long(0x709ee677u), change_endian_long(0xc1ef6344u), change_endian_long(0x9e7642eeu), change_endian_long(0x6120df81u));
        TSIP.REG_00H.WORD = 0x00003283u;
        TSIP.REG_2CH.WORD = 0x00000011u;
        RX671_func320(716);
        TSIP.REG_24H.WORD = 0x000016c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x7775adb6u);
        RX671_func080();
        RX671_func100(change_endian_long(0xd6b01ae7u), change_endian_long(0x4bdc32bdu), change_endian_long(0x4f832a5fu), change_endian_long(0xd25d8a7eu));
        TSIP.REG_00H.WORD = 0x00003283u;
        TSIP.REG_2CH.WORD = 0x00000012u;
        RX671_func320(752);
        TSIP.REG_24H.WORD = 0x000060d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_24H.WORD = 0x000080d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x04001991u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x060049c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x02001191u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x000049c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x009f0001u;
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x84992ba5u);
        RX671_func080();
        RX671_func100(change_endian_long(0x7eaf8abeu), change_endian_long(0xe6bf5415u), change_endian_long(0xcccdec5bu), change_endian_long(0x218ccdf3u));
        TSIP.REG_00H.WORD = 0x00003283u;
        TSIP.REG_2CH.WORD = 0x00000014u;
        RX671_func320(0);
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x064417e5u);
        RX671_func080();
        RX671_func100(change_endian_long(0x3bdcc77au), change_endian_long(0x5625fab1u), change_endian_long(0xdc4efe12u), change_endian_long(0xfbcd81eau));
        TSIP.REG_00H.WORD = 0x00003283u;
        TSIP.REG_2CH.WORD = 0x00000015u;
        RX671_func320(36);
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x49880ba2u);
        RX671_func080();
        TSIP.REG_00H.WORD = 0x00003283u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        RX671_func320(72);
        TSIP.REG_24H.WORD = 0x0000a0d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_28H.WORD = 0x00bf0001u;
        TSIP.REG_24H.WORD = 0x04001991u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x060049c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x02001191u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00210000u;
        RX671_func100(change_endian_long(0x1988db94u), change_endian_long(0x5360e121u), change_endian_long(0x65bad5c5u), change_endian_long(0xa87bd9ebu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func102(change_endian_long(0xa5cc9024u), change_endian_long(0x0ec06b41u), change_endian_long(0x25e1303bu), change_endian_long(0x99129f64u));
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
            TSIP.REG_ECH.WORD = 0x0000b7a0u;
            TSIP.REG_ECH.WORD = 0x00000079u;
            RX671_func101(change_endian_long(0x498f1b5du), change_endian_long(0xe414cae4u), change_endian_long(0x9832f36fu), change_endian_long(0xe2ccf70eu));
            RX671_func325();
            TSIP.REG_28H.WORD = 0x009f0001u;
            RX671_func100(change_endian_long(0x78c20711u), change_endian_long(0xf8373f7au), change_endian_long(0x698c90cfu), change_endian_long(0x19d38e8bu));
            TSIP.REG_2CH.WORD = 0x00000020u;
            TSIP.REG_04H.WORD = 0x00000382u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_Text[0] = TSIP.REG_100H.WORD;
            OutData_Text[1] = TSIP.REG_100H.WORD;
            OutData_Text[2] = TSIP.REG_100H.WORD;
            OutData_Text[3] = TSIP.REG_100H.WORD;
            OutData_Text[4] = TSIP.REG_100H.WORD;
            OutData_Text[5] = TSIP.REG_100H.WORD;
            OutData_Text[6] = TSIP.REG_100H.WORD;
            OutData_Text[7] = TSIP.REG_100H.WORD;
            OutData_Text[8] = TSIP.REG_100H.WORD;
            OutData_Text[9] = TSIP.REG_100H.WORD;
            OutData_Text[10] = TSIP.REG_100H.WORD;
            OutData_Text[11] = TSIP.REG_100H.WORD;
            OutData_Text[12] = TSIP.REG_100H.WORD;
            OutData_Text[13] = TSIP.REG_100H.WORD;
            OutData_Text[14] = TSIP.REG_100H.WORD;
            OutData_Text[15] = TSIP.REG_100H.WORD;
            OutData_Text[16] = TSIP.REG_100H.WORD;
            OutData_Text[17] = TSIP.REG_100H.WORD;
            OutData_Text[18] = TSIP.REG_100H.WORD;
            OutData_Text[19] = TSIP.REG_100H.WORD;
            OutData_Text[20] = TSIP.REG_100H.WORD;
            OutData_Text[21] = TSIP.REG_100H.WORD;
            OutData_Text[22] = TSIP.REG_100H.WORD;
            OutData_Text[23] = TSIP.REG_100H.WORD;
            OutData_Text[24] = TSIP.REG_100H.WORD;
            OutData_Text[25] = TSIP.REG_100H.WORD;
            OutData_Text[26] = TSIP.REG_100H.WORD;
            OutData_Text[27] = TSIP.REG_100H.WORD;
            OutData_Text[28] = TSIP.REG_100H.WORD;
            OutData_Text[29] = TSIP.REG_100H.WORD;
            OutData_Text[30] = TSIP.REG_100H.WORD;
            OutData_Text[31] = TSIP.REG_100H.WORD;
            TSIP.REG_28H.WORD = 0x00bf0001u;
            RX671_func100(change_endian_long(0x8d98b513u), change_endian_long(0x2f54f9d6u), change_endian_long(0x5a58dd72u), change_endian_long(0xbde7c585u));
            TSIP.REG_2CH.WORD = 0x00000022u;
            TSIP.REG_04H.WORD = 0x00000302u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_Text[32] = TSIP.REG_100H.WORD;
            OutData_Text[33] = TSIP.REG_100H.WORD;
            OutData_Text[34] = TSIP.REG_100H.WORD;
            OutData_Text[35] = TSIP.REG_100H.WORD;
            OutData_Text[36] = TSIP.REG_100H.WORD;
            OutData_Text[37] = TSIP.REG_100H.WORD;
            OutData_Text[38] = TSIP.REG_100H.WORD;
            OutData_Text[39] = TSIP.REG_100H.WORD;
            OutData_Text[40] = TSIP.REG_100H.WORD;
            OutData_Text[41] = TSIP.REG_100H.WORD;
            OutData_Text[42] = TSIP.REG_100H.WORD;
            OutData_Text[43] = TSIP.REG_100H.WORD;
            OutData_Text[44] = TSIP.REG_100H.WORD;
            OutData_Text[45] = TSIP.REG_100H.WORD;
            OutData_Text[46] = TSIP.REG_100H.WORD;
            OutData_Text[47] = TSIP.REG_100H.WORD;
            OutData_Text[48] = TSIP.REG_100H.WORD;
            OutData_Text[49] = TSIP.REG_100H.WORD;
            OutData_Text[50] = TSIP.REG_100H.WORD;
            OutData_Text[51] = TSIP.REG_100H.WORD;
            OutData_Text[52] = TSIP.REG_100H.WORD;
            OutData_Text[53] = TSIP.REG_100H.WORD;
            OutData_Text[54] = TSIP.REG_100H.WORD;
            OutData_Text[55] = TSIP.REG_100H.WORD;
            OutData_Text[56] = TSIP.REG_100H.WORD;
            OutData_Text[57] = TSIP.REG_100H.WORD;
            OutData_Text[58] = TSIP.REG_100H.WORD;
            OutData_Text[59] = TSIP.REG_100H.WORD;
            OutData_Text[60] = TSIP.REG_100H.WORD;
            OutData_Text[61] = TSIP.REG_100H.WORD;
            OutData_Text[62] = TSIP.REG_100H.WORD;
            OutData_Text[63] = TSIP.REG_100H.WORD;
            OutData_Text[64] = TSIP.REG_100H.WORD;
            OutData_Text[65] = TSIP.REG_100H.WORD;
            OutData_Text[66] = TSIP.REG_100H.WORD;
            OutData_Text[67] = TSIP.REG_100H.WORD;
            OutData_Text[68] = TSIP.REG_100H.WORD;
            OutData_Text[69] = TSIP.REG_100H.WORD;
            OutData_Text[70] = TSIP.REG_100H.WORD;
            OutData_Text[71] = TSIP.REG_100H.WORD;
            OutData_Text[72] = TSIP.REG_100H.WORD;
            OutData_Text[73] = TSIP.REG_100H.WORD;
            OutData_Text[74] = TSIP.REG_100H.WORD;
            OutData_Text[75] = TSIP.REG_100H.WORD;
            OutData_Text[76] = TSIP.REG_100H.WORD;
            OutData_Text[77] = TSIP.REG_100H.WORD;
            OutData_Text[78] = TSIP.REG_100H.WORD;
            OutData_Text[79] = TSIP.REG_100H.WORD;
            OutData_Text[80] = TSIP.REG_100H.WORD;
            OutData_Text[81] = TSIP.REG_100H.WORD;
            OutData_Text[82] = TSIP.REG_100H.WORD;
            OutData_Text[83] = TSIP.REG_100H.WORD;
            OutData_Text[84] = TSIP.REG_100H.WORD;
            OutData_Text[85] = TSIP.REG_100H.WORD;
            OutData_Text[86] = TSIP.REG_100H.WORD;
            OutData_Text[87] = TSIP.REG_100H.WORD;
            OutData_Text[88] = TSIP.REG_100H.WORD;
            OutData_Text[89] = TSIP.REG_100H.WORD;
            OutData_Text[90] = TSIP.REG_100H.WORD;
            OutData_Text[91] = TSIP.REG_100H.WORD;
            OutData_Text[92] = TSIP.REG_100H.WORD;
            OutData_Text[93] = TSIP.REG_100H.WORD;
            OutData_Text[94] = TSIP.REG_100H.WORD;
            OutData_Text[95] = TSIP.REG_100H.WORD;
            RX671_func102(change_endian_long(0x52a146e2u), change_endian_long(0xcac1318au), change_endian_long(0x729f047eu), change_endian_long(0x552ba023u));
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
/**********************************************************************************************************************
 End of function ./input_dir/RX671/RX671_p79_r1.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_PRV_USE_RSA_3072 == 1 */
