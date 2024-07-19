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

#if TSIP_PRV_USE_AES_128 == 1
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
* Function Name: R_TSIP_Aes128EncryptDecryptUpdateSub
*******************************************************************************************************************/ /**
* @details       RX66T AES-128 Encryption/Decryption with ECB/CBC/CTR Mode Update
* @param[in]     InData_Text
* @param[in]     MAX_CNT
* @param[out]    OutData_Text
* @note          None
*/
void R_TSIP_Aes128EncryptDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
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
        RX66T_RX72T_function001(change_endian_long(0x01d8d97fu), change_endian_long(0x175802f5u), change_endian_long(0x81972702u), change_endian_long(0xae82257eu));
        TSIP.REG_104H.WORD = 0x000000b1u;
        TSIP.REG_A4H.WORD = 0x0000088eu;
        TSIP.REG_04H.WORD = 0x0000c100u;
    }
    else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        TSIP.REG_104H.WORD = 0x00000361u;
        RX66T_RX72T_function001(change_endian_long(0x84adfd53u), change_endian_long(0x8f68ca38u), change_endian_long(0x8f2110c4u), change_endian_long(0x4cd06adeu));
        TSIP.REG_104H.WORD = 0x000000b1u;
        TSIP.REG_A4H.WORD = 0x0000488eu;
        TSIP.REG_04H.WORD = 0x0000c100u;
    }
    else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        TSIP.REG_104H.WORD = 0x00000361u;
        RX66T_RX72T_function001(change_endian_long(0xe61164dau), change_endian_long(0x7da48a90u), change_endian_long(0xc2b72de6u), change_endian_long(0xc6bcaf55u));
        TSIP.REG_104H.WORD = 0x000000b1u;
        TSIP.REG_A4H.WORD = 0x00000c9eu;
        TSIP.REG_04H.WORD = 0x0000c100u;
    }
    else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        TSIP.REG_104H.WORD = 0x00000361u;
        RX66T_RX72T_function001(change_endian_long(0x9166aabbu), change_endian_long(0x1c695d58u), change_endian_long(0xa2e397f0u), change_endian_long(0xd92ccfb5u));
        TSIP.REG_104H.WORD = 0x000000b1u;
        TSIP.REG_A4H.WORD = 0x000049aeu;
        TSIP.REG_04H.WORD = 0x0000c100u;
    }
    else if (0x04000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        TSIP.REG_104H.WORD = 0x00000361u;
        RX66T_RX72T_function001(change_endian_long(0x0d0a82dbu), change_endian_long(0x2c0a75b8u), change_endian_long(0xdeb5a1ceu), change_endian_long(0x1aac69f8u));
        TSIP.REG_104H.WORD = 0x000000b1u;
        TSIP.REG_A4H.WORD = 0x000006beu;
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
        RX66T_RX72T_function206();
        RX66T_RX72T_function002(change_endian_long(0xc1e59e53u), change_endian_long(0x21146553u), change_endian_long(0x921026b7u), change_endian_long(0xa5cd50f8u));
    }
    else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX66T_RX72T_function206();
        RX66T_RX72T_function002(change_endian_long(0xdf56cbceu), change_endian_long(0x546af07cu), change_endian_long(0xcfc8db24u), change_endian_long(0x6b2ff28bu));
    }
    else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX66T_RX72T_function206();
        RX66T_RX72T_function002(change_endian_long(0x9169a3cau), change_endian_long(0xae4f5dd8u), change_endian_long(0xc08eec14u), change_endian_long(0x0679a632u));
    }
    else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX66T_RX72T_function206();
        RX66T_RX72T_function002(change_endian_long(0xbaec2a7fu), change_endian_long(0xc0037587u), change_endian_long(0x118d62f6u), change_endian_long(0xbc2436b5u));
    }
    else if (0x04000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX66T_RX72T_function206();
        RX66T_RX72T_function002(change_endian_long(0x253a339du), change_endian_long(0x9378abbau), change_endian_long(0x2f7a0ce2u), change_endian_long(0x2b9b4d36u));
    }
}
/**********************************************************************************************************************
 End of function ./input_dir/RX66T/RX66T_p61.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_PRV_USE_AES_128 == 1 */
