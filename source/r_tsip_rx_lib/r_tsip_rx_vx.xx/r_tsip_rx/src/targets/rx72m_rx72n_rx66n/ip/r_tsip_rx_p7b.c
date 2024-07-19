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

#if TSIP_PRV_USE_RSA_4096 == 1
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
* Function Name: R_TSIP_Rsa4096ModularExponentEncryptSub
*******************************************************************************************************************/ /**
* @details       RX72M RSA-4096 Encryption
* @param[in]     InData_KeyIndex
* @param[in]     InData_Text
* @param[out]    OutData_Text
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_Rsa4096ModularExponentEncryptSub(uint32_t *InData_KeyIndex, uint32_t *InData_Text, uint32_t *OutData_Text)
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
    TSIP.REG_84H.WORD = 0x00007b01u;
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
    TSIP.REG_100H.WORD = change_endian_long(0xed334f67u);
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x00040804u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_28H.WORD = 0x00bf0001u;
    TSIP.REG_104H.WORD = 0x00007f62u;
    TSIP.REG_D0H.WORD = 0x00001f00u;
    TSIP.REG_C4H.WORD = 0x02e0888fu;
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    for(iLoop=0; iLoop<64; iLoop=iLoop+4)
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
    TSIP.REG_00H.WORD = 0x00013203u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    for(iLoop=64; iLoop<128; iLoop=iLoop+4)
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
    TSIP.REG_104H.WORD = 0x00000362u;
    TSIP.REG_C4H.WORD = 0x00e0088du;
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
    RX72M_RX72N_RX66N_func100(change_endian_long(0x57754128u), change_endian_long(0x43097923u), change_endian_long(0x1ae4d275u), change_endian_long(0xdd4d5783u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0x05a5068cu), change_endian_long(0x4773c2b9u), change_endian_long(0x394d2fb7u), change_endian_long(0x63394c64u));
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
        RX72M_RX72N_RX66N_func100(change_endian_long(0x837643dau), change_endian_long(0x9c970613u), change_endian_long(0xcef74a00u), change_endian_long(0x91a7c819u));
        RX72M_RX72N_RX66N_func103();
        RX72M_RX72N_RX66N_func100(change_endian_long(0x0d0ba4dcu), change_endian_long(0x47a574bcu), change_endian_long(0x9ba3131au), change_endian_long(0xe8ab3c1au));
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x010c0c04u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        RX72M_RX72N_RX66N_func100(change_endian_long(0x86b8fc90u), change_endian_long(0x3c1fb838u), change_endian_long(0x96c52561u), change_endian_long(0xcae588e6u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x5fe685f6u);
        RX72M_RX72N_RX66N_func080();
        RX72M_RX72N_RX66N_func100(change_endian_long(0x20c9f8ecu), change_endian_long(0x9bcc182cu), change_endian_long(0xbadece17u), change_endian_long(0x70177a7bu));
        TSIP.REG_00H.WORD = 0x00012303u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        RX72M_RX72N_RX66N_func313(748);
        RX72M_RX72N_RX66N_func100(change_endian_long(0x48167952u), change_endian_long(0x606ba830u), change_endian_long(0xf4efe4bfu), change_endian_long(0x73a3e20cu));
        RX72M_RX72N_RX66N_func314(748+64);
        RX72M_RX72N_RX66N_func100(change_endian_long(0x7d7b0874u), change_endian_long(0xe1fa4983u), change_endian_long(0x518f7d84u), change_endian_long(0x754b1c82u));
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x1c5a2b81u);
        RX72M_RX72N_RX66N_func080();
        RX72M_RX72N_RX66N_func100(change_endian_long(0xff8ffc86u), change_endian_long(0xf9227d9cu), change_endian_long(0x9550054du), change_endian_long(0x60f044a4u));
        TSIP.REG_00H.WORD = 0x00012303u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        RX72M_RX72N_RX66N_func313(680);
        RX72M_RX72N_RX66N_func100(change_endian_long(0xdc3d6bc2u), change_endian_long(0xd9c1a188u), change_endian_long(0xc21b672au), change_endian_long(0xe22fd8b7u));
        RX72M_RX72N_RX66N_func314(680+64);
        TSIP.REG_00H.WORD = 0x000003ffu;
        TSIP.REG_2CH.WORD = 0x00000022u;
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
        RX72M_RX72N_RX66N_func100(change_endian_long(0x75abceb5u), change_endian_long(0xd37e472bu), change_endian_long(0xa23e22eeu), change_endian_long(0x66c7f89cu));
        TSIP.REG_2CH.WORD = 0x00000010u;
        TSIP.REG_104H.WORD = 0x00003f67u;
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
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x5dd049feu);
        RX72M_RX72N_RX66N_func080();
        RX72M_RX72N_RX66N_func100(change_endian_long(0x68141b13u), change_endian_long(0xd69edd32u), change_endian_long(0xf191875eu), change_endian_long(0xb168dad0u));
        TSIP.REG_00H.WORD = 0x00012303u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        RX72M_RX72N_RX66N_func313(68);
        RX72M_RX72N_RX66N_func100(change_endian_long(0x27d94f95u), change_endian_long(0x4c871a47u), change_endian_long(0xc5982b5bu), change_endian_long(0xd252bed0u));
        RX72M_RX72N_RX66N_func314(68+64);
        RX72M_RX72N_RX66N_func100(change_endian_long(0x051b8bc9u), change_endian_long(0x16c71d82u), change_endian_long(0xa42a1a7cu), change_endian_long(0x96fd50fau));
        TSIP.REG_2CH.WORD = 0x00000014u;
        TSIP.REG_104H.WORD = 0x00003f67u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
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
        TSIP.REG_100H.WORD = InData_Text[96];
        TSIP.REG_100H.WORD = InData_Text[97];
        TSIP.REG_100H.WORD = InData_Text[98];
        TSIP.REG_100H.WORD = InData_Text[99];
        TSIP.REG_100H.WORD = InData_Text[100];
        TSIP.REG_100H.WORD = InData_Text[101];
        TSIP.REG_100H.WORD = InData_Text[102];
        TSIP.REG_100H.WORD = InData_Text[103];
        TSIP.REG_100H.WORD = InData_Text[104];
        TSIP.REG_100H.WORD = InData_Text[105];
        TSIP.REG_100H.WORD = InData_Text[106];
        TSIP.REG_100H.WORD = InData_Text[107];
        TSIP.REG_100H.WORD = InData_Text[108];
        TSIP.REG_100H.WORD = InData_Text[109];
        TSIP.REG_100H.WORD = InData_Text[110];
        TSIP.REG_100H.WORD = InData_Text[111];
        TSIP.REG_100H.WORD = InData_Text[112];
        TSIP.REG_100H.WORD = InData_Text[113];
        TSIP.REG_100H.WORD = InData_Text[114];
        TSIP.REG_100H.WORD = InData_Text[115];
        TSIP.REG_100H.WORD = InData_Text[116];
        TSIP.REG_100H.WORD = InData_Text[117];
        TSIP.REG_100H.WORD = InData_Text[118];
        TSIP.REG_100H.WORD = InData_Text[119];
        TSIP.REG_100H.WORD = InData_Text[120];
        TSIP.REG_100H.WORD = InData_Text[121];
        TSIP.REG_100H.WORD = InData_Text[122];
        TSIP.REG_100H.WORD = InData_Text[123];
        TSIP.REG_100H.WORD = InData_Text[124];
        TSIP.REG_100H.WORD = InData_Text[125];
        TSIP.REG_100H.WORD = InData_Text[126];
        TSIP.REG_100H.WORD = InData_Text[127];
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x945d2158u);
        RX72M_RX72N_RX66N_func080();
        RX72M_RX72N_RX66N_func100(change_endian_long(0x98f9ebb0u), change_endian_long(0xc236a8f4u), change_endian_long(0x9e26b5ebu), change_endian_long(0x7223455bu));
        TSIP.REG_00H.WORD = 0x00012303u;
        TSIP.REG_2CH.WORD = 0x00000024u;
        RX72M_RX72N_RX66N_func313(0);
        RX72M_RX72N_RX66N_func100(change_endian_long(0x7016324fu), change_endian_long(0x8b699e8cu), change_endian_long(0xadbc99e3u), change_endian_long(0x54bf229au));
        RX72M_RX72N_RX66N_func314(0+64);
        RX72M_RX72N_RX66N_func100(change_endian_long(0x44dd8e4au), change_endian_long(0xefe4aee8u), change_endian_long(0x08498923u), change_endian_long(0x43a004a3u));
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
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x1c5a2b81u);
        RX72M_RX72N_RX66N_func080();
        RX72M_RX72N_RX66N_func100(change_endian_long(0xee8ec39cu), change_endian_long(0xd232357du), change_endian_long(0xff059a99u), change_endian_long(0x94cb4582u));
        TSIP.REG_00H.WORD = 0x00013203u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        RX72M_RX72N_RX66N_func312(680);
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
        TSIP.REG_100H.WORD = change_endian_long(0x5fe685f6u);
        RX72M_RX72N_RX66N_func080();
        RX72M_RX72N_RX66N_func100(change_endian_long(0x035f627eu), change_endian_long(0xb6e63029u), change_endian_long(0x186115c1u), change_endian_long(0x10e8a3a9u));
        TSIP.REG_00H.WORD = 0x00013203u;
        TSIP.REG_2CH.WORD = 0x00000012u;
        RX72M_RX72N_RX66N_func312(748);
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
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x945d2158u);
        RX72M_RX72N_RX66N_func080();
        RX72M_RX72N_RX66N_func100(change_endian_long(0x2af27fe6u), change_endian_long(0x27d64e2fu), change_endian_long(0x6e712381u), change_endian_long(0x7fdd6d0au));
        TSIP.REG_00H.WORD = 0x00013203u;
        TSIP.REG_2CH.WORD = 0x00000014u;
        RX72M_RX72N_RX66N_func312(0);
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010020u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x5dd049feu);
        RX72M_RX72N_RX66N_func080();
        TSIP.REG_00H.WORD = 0x00013203u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        RX72M_RX72N_RX66N_func312(68);
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
        RX72M_RX72N_RX66N_func100(change_endian_long(0xf9b5e1c3u), change_endian_long(0x08af1160u), change_endian_long(0xdb38bce1u), change_endian_long(0xa7e97f75u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func102(change_endian_long(0xf893e1f3u), change_endian_long(0x1ad514f9u), change_endian_long(0xeddb3d07u), change_endian_long(0xfb0575b4u));
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
            TSIP.REG_ECH.WORD = 0x0000b400u;
            TSIP.REG_ECH.WORD = 0x0000007bu;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x0124e76cu), change_endian_long(0x7dedd1d6u), change_endian_long(0xda73275au), change_endian_long(0xf3e62371u));
            RX72M_RX72N_RX66N_func307();
            RX72M_RX72N_RX66N_func100(change_endian_long(0xace9fe09u), change_endian_long(0x3f26ca88u), change_endian_long(0xfcf67515u), change_endian_long(0x1f225593u));
            TSIP.REG_2CH.WORD = 0x00000020u;
            TSIP.REG_04H.WORD = 0x00000302u;
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
            RX72M_RX72N_RX66N_func100(change_endian_long(0x49fc91b5u), change_endian_long(0x8532aa1au), change_endian_long(0x06861128u), change_endian_long(0x0b3b5b6eu));
            TSIP.REG_2CH.WORD = 0x00000022u;
            TSIP.REG_04H.WORD = 0x00000302u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
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
            OutData_Text[96] = TSIP.REG_100H.WORD;
            OutData_Text[97] = TSIP.REG_100H.WORD;
            OutData_Text[98] = TSIP.REG_100H.WORD;
            OutData_Text[99] = TSIP.REG_100H.WORD;
            OutData_Text[100] = TSIP.REG_100H.WORD;
            OutData_Text[101] = TSIP.REG_100H.WORD;
            OutData_Text[102] = TSIP.REG_100H.WORD;
            OutData_Text[103] = TSIP.REG_100H.WORD;
            OutData_Text[104] = TSIP.REG_100H.WORD;
            OutData_Text[105] = TSIP.REG_100H.WORD;
            OutData_Text[106] = TSIP.REG_100H.WORD;
            OutData_Text[107] = TSIP.REG_100H.WORD;
            OutData_Text[108] = TSIP.REG_100H.WORD;
            OutData_Text[109] = TSIP.REG_100H.WORD;
            OutData_Text[110] = TSIP.REG_100H.WORD;
            OutData_Text[111] = TSIP.REG_100H.WORD;
            OutData_Text[112] = TSIP.REG_100H.WORD;
            OutData_Text[113] = TSIP.REG_100H.WORD;
            OutData_Text[114] = TSIP.REG_100H.WORD;
            OutData_Text[115] = TSIP.REG_100H.WORD;
            OutData_Text[116] = TSIP.REG_100H.WORD;
            OutData_Text[117] = TSIP.REG_100H.WORD;
            OutData_Text[118] = TSIP.REG_100H.WORD;
            OutData_Text[119] = TSIP.REG_100H.WORD;
            OutData_Text[120] = TSIP.REG_100H.WORD;
            OutData_Text[121] = TSIP.REG_100H.WORD;
            OutData_Text[122] = TSIP.REG_100H.WORD;
            OutData_Text[123] = TSIP.REG_100H.WORD;
            OutData_Text[124] = TSIP.REG_100H.WORD;
            OutData_Text[125] = TSIP.REG_100H.WORD;
            OutData_Text[126] = TSIP.REG_100H.WORD;
            OutData_Text[127] = TSIP.REG_100H.WORD;
            RX72M_RX72N_RX66N_func102(change_endian_long(0x3b58004cu), change_endian_long(0x60e65d03u), change_endian_long(0x72eea4b8u), change_endian_long(0x6974bd86u));
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
 End of function ./input_dir/RX72M/RX72M_p7b_r1.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_PRV_USE_RSA_4096 == 1 */
