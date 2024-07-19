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

#if TSIP_PRV_USE_RSA_1024 == 1
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
* Function Name: R_TSIP_Rsa1024ModularExponentEncryptSub
*******************************************************************************************************************/ /**
* @details       RX72M RSA1024 Encryption with Public Key
* @param[in]     InData_KeyIndex
* @param[in]     InData_Text
* @param[out]    OutData_Text
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_Rsa1024ModularExponentEncryptSub(uint32_t *InData_KeyIndex, uint32_t *InData_Text, uint32_t *OutData_Text)
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
    TSIP.REG_84H.WORD = 0x00005301u;
    TSIP.REG_108H.WORD = 0x00000000u;
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
    TSIP.REG_A4H.WORD = 0x000c0b0cu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x7a241e7bu);
    TSIP.REG_104H.WORD = 0x00000051u;
    TSIP.REG_A4H.WORD = 0x00040804u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_28H.WORD = 0x00000001u;
    TSIP.REG_28H.WORD = 0x009f0001u;
    TSIP.REG_00H.WORD = 0x00003183u;
    TSIP.REG_2CH.WORD = 0x00000014u;
    TSIP.REG_104H.WORD = 0x00004361u;
    TSIP.REG_B0H.WORD = 0x00000700u;
    TSIP.REG_A4H.WORD = 0x02f0888fu;
    for(iLoop=4; iLoop<36; iLoop=iLoop+4)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 0];
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 1];
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 2];
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 3];
    }
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_A4H.WORD = 0x00f0088du;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[36];
    TSIP.REG_100H.WORD = InData_KeyIndex[37];
    TSIP.REG_100H.WORD = InData_KeyIndex[38];
    TSIP.REG_100H.WORD = InData_KeyIndex[39];
    TSIP.REG_E0H.WORD = 0x80010000u;
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
    TSIP.REG_00H.WORD = 0x00003183u;
    TSIP.REG_2CH.WORD = 0x00000012u;
    TSIP.REG_B0H.WORD = 0x00000700u;
    TSIP.REG_A4H.WORD = 0x00f087bfu;
    for(iLoop=40; iLoop<72; iLoop=iLoop+4)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 0];
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 1];
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 2];
        TSIP.REG_100H.WORD = InData_KeyIndex[iLoop + 3];
    }
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_104H.WORD = 0x00000361u;
    TSIP.REG_A4H.WORD = 0x000007bdu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[72];
    TSIP.REG_100H.WORD = InData_KeyIndex[73];
    TSIP.REG_100H.WORD = InData_KeyIndex[74];
    TSIP.REG_100H.WORD = InData_KeyIndex[75];
    TSIP.REG_A4H.WORD = 0x00900c45u;
    TSIP.REG_00H.WORD = 0x00001113u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x6b72b99eu), change_endian_long(0x9fd88ac1u), change_endian_long(0xc3e6c062u), change_endian_long(0x98e2f29eu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0xa2a74fb7u), change_endian_long(0x53e952fbu), change_endian_long(0x5ef62fcfu), change_endian_long(0x655f0670u));
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
        RX72M_RX72N_RX66N_func100(change_endian_long(0x96fffb42u), change_endian_long(0x6e17422eu), change_endian_long(0xa493b738u), change_endian_long(0xcfb9821bu));
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
        TSIP.REG_24H.WORD = 0x000068d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_18H.WORD = 0x00000004u;
        TSIP.REG_38H.WORD = 0x00000730u;
        TSIP.REG_E0H.WORD = 0x81010000u;
        TSIP.REG_00H.WORD = 0x00003807u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_18H.BIT.B10)
        {
            /* waiting */
        }
        TSIP.REG_18H.WORD = 0x00000000u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0x31b0a25au), change_endian_long(0x930e2896u), change_endian_long(0xf218012au), change_endian_long(0xe5555be7u));
        TSIP.REG_2CH.WORD = 0x00000020u;
        TSIP.REG_04H.WORD = 0x00000383u;
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
        RX72M_RX72N_RX66N_func102(change_endian_long(0xd54d2232u), change_endian_long(0x4e4db9edu), change_endian_long(0x7ddadfc2u), change_endian_long(0x13aae5bcu));
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
/**********************************************************************************************************************
 End of function ./input_dir/RX72M/RX72M_p53.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_PRV_USE_RSA_1024 == 1 */
