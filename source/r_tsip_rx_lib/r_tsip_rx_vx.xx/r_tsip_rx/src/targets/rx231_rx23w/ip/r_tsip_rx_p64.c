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

#if TSIP_PRV_USE_AES_256 == 1
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
* Function Name: R_TSIP_Aes256EncryptDecryptUpdateSub
*******************************************************************************************************************/ /**
* @details       RX231HU AES-256 Encryption/Decryption with ECB/CBC/CTR Mode Update
* @param[in]     InData_Text
* @param[in]     MAX_CNT
* @param[out]    OutData_Text
* @note          None
*/
void R_TSIP_Aes256EncryptDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
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
    if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        TSIP.REG_104H.WORD = 0x00000361u;
        RX231_RX23W_function001(change_endian_long(0x36cad6c5u), change_endian_long(0x08ac0c79u), change_endian_long(0x9e221b7cu), change_endian_long(0xab0cb91cu));
        TSIP.REG_104H.WORD = 0x000000b1u;
        TSIP.REG_B0H.WORD = 0x40000000u;
        TSIP.REG_A4H.WORD = 0x0000888eu;
        TSIP.REG_04H.WORD = 0x0000c100u;
    }
    else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        TSIP.REG_104H.WORD = 0x00000361u;
        RX231_RX23W_function001(change_endian_long(0x5f0a0c58u), change_endian_long(0xa2be086au), change_endian_long(0x6addc6f8u), change_endian_long(0x622a84c3u));
        TSIP.REG_104H.WORD = 0x000000b1u;
        TSIP.REG_B0H.WORD = 0x40000000u;
        TSIP.REG_A4H.WORD = 0x0000c88eu;
        TSIP.REG_04H.WORD = 0x0000c100u;
    }
    else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        TSIP.REG_104H.WORD = 0x00000361u;
        RX231_RX23W_function001(change_endian_long(0x5c23045eu), change_endian_long(0x416e10d8u), change_endian_long(0x54b2094cu), change_endian_long(0x3e83e126u));
        TSIP.REG_104H.WORD = 0x000000b1u;
        TSIP.REG_B0H.WORD = 0x40000000u;
        TSIP.REG_A4H.WORD = 0x00008c9eu;
        TSIP.REG_04H.WORD = 0x0000c100u;
    }
    else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        TSIP.REG_104H.WORD = 0x00000361u;
        RX231_RX23W_function001(change_endian_long(0x98306dcdu), change_endian_long(0x5a3d58a9u), change_endian_long(0x2fabb4bfu), change_endian_long(0x8e4c5077u));
        TSIP.REG_104H.WORD = 0x000000b1u;
        TSIP.REG_B0H.WORD = 0x40000000u;
        TSIP.REG_A4H.WORD = 0x0000c9aeu;
        TSIP.REG_04H.WORD = 0x0000c100u;
    }
    else if (0x04000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        TSIP.REG_104H.WORD = 0x00000361u;
        RX231_RX23W_function001(change_endian_long(0xd2879811u), change_endian_long(0xd8958ba3u), change_endian_long(0xb19f6447u), change_endian_long(0xb1f0ab7cu));
        TSIP.REG_104H.WORD = 0x000000b1u;
        TSIP.REG_B0H.WORD = 0x40000000u;
        TSIP.REG_A4H.WORD = 0x000086beu;
        TSIP.REG_04H.WORD = 0x0000c100u;
    }
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Text[0];
    TSIP.REG_100H.WORD = InData_Text[1];
    TSIP.REG_100H.WORD = InData_Text[2];
    TSIP.REG_100H.WORD = InData_Text[3];
    for (iLoop = 4; iLoop < MAX_CNT ; iLoop = iLoop + 4)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_Text[iLoop + 0];
        TSIP.REG_100H.WORD = InData_Text[iLoop + 1];
        TSIP.REG_100H.WORD = InData_Text[iLoop + 2];
        TSIP.REG_100H.WORD = InData_Text[iLoop + 3];
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_Text[iLoop-4 + 0] = TSIP.REG_100H.WORD;
        OutData_Text[iLoop-4 + 1] = TSIP.REG_100H.WORD;
        OutData_Text[iLoop-4 + 2] = TSIP.REG_100H.WORD;
        OutData_Text[iLoop-4 + 3] = TSIP.REG_100H.WORD;
    }
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_Text[MAX_CNT-4 + 0] = TSIP.REG_100H.WORD;
    OutData_Text[MAX_CNT-4 + 1] = TSIP.REG_100H.WORD;
    OutData_Text[MAX_CNT-4 + 2] = TSIP.REG_100H.WORD;
    OutData_Text[MAX_CNT-4 + 3] = TSIP.REG_100H.WORD;
    if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX231_RX23W_function206();
        RX231_RX23W_function002(change_endian_long(0xbb2da97bu), change_endian_long(0x287d4b17u), change_endian_long(0x6b160b67u), change_endian_long(0x74a675a3u));
    }
    else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX231_RX23W_function206();
        RX231_RX23W_function002(change_endian_long(0xf465f4ecu), change_endian_long(0xee6118e5u), change_endian_long(0xde7e6749u), change_endian_long(0xea451efcu));
    }
    else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX231_RX23W_function206();
        RX231_RX23W_function002(change_endian_long(0x7e9ea1feu), change_endian_long(0xab9fde74u), change_endian_long(0xf9ffce9au), change_endian_long(0x159316abu));
    }
    else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX231_RX23W_function206();
        RX231_RX23W_function002(change_endian_long(0xb599c130u), change_endian_long(0x8dc3b597u), change_endian_long(0x55798525u), change_endian_long(0x9d55e5bcu));
    }
    else if (0x04000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX231_RX23W_function206();
        RX231_RX23W_function002(change_endian_long(0x93f31023u), change_endian_long(0x8ed38bbbu), change_endian_long(0xa2fe62c4u), change_endian_long(0xa133f884u));
    }
}
/**********************************************************************************************************************
 End of function ./input_dir/RX231HU/RX231HU_p64.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_PRV_USE_AES_256 == 1 */
