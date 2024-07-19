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

#if TSIP_AES_128_CCM_DECRYPT == 1
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
* Function Name: R_TSIP_Aes128CcmDecryptFinalSub
*******************************************************************************************************************/ /**
* @details       RX65NHU AES-128 Decryption and MAC verification with CCM Mode Final
* @param[in]     InData_Text
* @param[in]     InData_MAC
* @param[out]    OutData_Text
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CcmDecryptFinalSub(uint32_t *InData_Text, uint32_t *InData_MAC, uint32_t *OutData_Text)
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
    RX65NHU_func100(change_endian_long(0x40c96d2cu), change_endian_long(0xba289077u), change_endian_long(0xf7dfac42u), change_endian_long(0x41249a4bu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0xff25ce05u), change_endian_long(0x7c4a5fe8u), change_endian_long(0x9a009b7au), change_endian_long(0x8d3b66f2u));
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
        TSIP.REG_ECH.WORD = 0x38008940u;
        TSIP.REG_ECH.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        TSIP.REG_ECH.WORD = 0x0000b7e0u;
        TSIP.REG_ECH.WORD = 0x00000010u;
        TSIP.REG_ECH.WORD = 0x34202beau;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX65NHU_func100(change_endian_long(0xdad88b75u), change_endian_long(0xc249268au), change_endian_long(0x23663467u), change_endian_long(0x0c6a43a5u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func102(change_endian_long(0xdcf14d2cu), change_endian_long(0x42c5db68u), change_endian_long(0xd686cb55u), change_endian_long(0x5f5d0c02u));
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
            TSIP.REG_ECH.WORD = 0x00003409u;
            TSIP.REG_ECH.WORD = 0x00008c00u;
            TSIP.REG_ECH.WORD = 0x0000000fu;
            TSIP.REG_ECH.WORD = 0x38008800u;
            TSIP.REG_ECH.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            RX65NHU_func100(change_endian_long(0xdb46bc50u), change_endian_long(0x0c9939e5u), change_endian_long(0x54eb1afbu), change_endian_long(0x780c8a91u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_104H.WORD = 0x00000361u;
                TSIP.REG_A4H.WORD = 0x000007b5u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_Text[0];
                TSIP.REG_100H.WORD = InData_Text[1];
                TSIP.REG_100H.WORD = InData_Text[2];
                TSIP.REG_100H.WORD = InData_Text[3];
                TSIP.REG_ECH.WORD = 0x0000b420u;
                TSIP.REG_ECH.WORD = 0x00000080u;
                TSIP.REG_E0H.WORD = 0x80840001u;
                TSIP.REG_00H.WORD = 0x00008113u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x00000821u;
                TSIP.REG_ECH.WORD = 0x0000a400u;
                TSIP.REG_ECH.WORD = 0x00000080u;
                TSIP.REG_ECH.WORD = 0x0000b7e0u;
                TSIP.REG_ECH.WORD = 0x00000080u;
                for (iLoop = 0; iLoop < 16; iLoop = iLoop+1)
                {
                    TSIP.REG_ECH.WORD = 0x3c002be0u;
                    TSIP.REG_ECH.WORD = 0x12003c3fu;
                    TSIP.REG_ECH.WORD = 0x00002fe0u;
                }
                RX65NHU_func100(change_endian_long(0x8607499cu), change_endian_long(0x6ba2c50fu), change_endian_long(0x2ed7e256u), change_endian_long(0xb2c92483u));
                TSIP.REG_ECH.WORD = 0x0000b420u;
                TSIP.REG_ECH.WORD = 0x00000080u;
                TSIP.REG_E0H.WORD = 0x81840001u;
                TSIP.REG_04H.WORD = 0x00000612u;
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
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_Text[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_Text[3] = TSIP.REG_100H.WORD;
                TSIP.REG_A4H.WORD = 0x00000e55u;
                TSIP.REG_E0H.WORD = 0x81840001u;
                TSIP.REG_00H.WORD = 0x00001813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                RX65NHU_func101(change_endian_long(0xb8d1087fu), change_endian_long(0xa3a7b295u), change_endian_long(0xb37a125eu), change_endian_long(0x78d62a6au));
            }
            else
            {
                RX65NHU_func101(change_endian_long(0x7a54efb0u), change_endian_long(0x667ef9ceu), change_endian_long(0x2528e861u), change_endian_long(0x10f903e0u));
            }
            TSIP.REG_104H.WORD = 0x00000051u;
            TSIP.REG_A4H.WORD = 0x00000cc4u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_A4H.WORD = 0x010007b5u;
            TSIP.REG_00H.WORD = 0x00001113u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x0000b420u;
            TSIP.REG_ECH.WORD = 0x00000080u;
            TSIP.REG_E0H.WORD = 0x80840001u;
            TSIP.REG_00H.WORD = 0x00008113u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x0000a540u;
            TSIP.REG_ECH.WORD = 0x00000080u;
            TSIP.REG_ECH.WORD = 0x0000b7e0u;
            TSIP.REG_ECH.WORD = 0x00000080u;
            TSIP.REG_ECH.WORD = 0x00000821u;
            for (iLoop = 0; iLoop < 16; iLoop = iLoop+1)
            {
                TSIP.REG_ECH.WORD = 0x3c002beau;
                TSIP.REG_ECH.WORD = 0x12003c3fu;
                TSIP.REG_ECH.WORD = 0x00002fe0u;
            }
            TSIP.REG_104H.WORD = 0x00000361u;
            TSIP.REG_A4H.WORD = 0x00050805u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_MAC[0];
            TSIP.REG_100H.WORD = InData_MAC[1];
            TSIP.REG_100H.WORD = InData_MAC[2];
            TSIP.REG_100H.WORD = InData_MAC[3];
            TSIP.REG_A4H.WORD = 0x00900c45u;
            TSIP.REG_ECH.WORD = 0x0000b420u;
            TSIP.REG_ECH.WORD = 0x00000080u;
            TSIP.REG_E0H.WORD = 0x81840001u;
            TSIP.REG_00H.WORD = 0x00001813u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_A4H.WORD = 0x00000000u;
            RX65NHU_func100(change_endian_long(0x51cb9896u), change_endian_long(0x170c74e7u), change_endian_long(0xfcd707b2u), change_endian_long(0xdfa7cdaeu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX65NHU_func102(change_endian_long(0x5c0d7fc5u), change_endian_long(0xcf220939u), change_endian_long(0xad308d9du), change_endian_long(0xbeb5fdd5u));
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
                RX65NHU_func102(change_endian_long(0x324ea3bbu), change_endian_long(0xce90d080u), change_endian_long(0x974201a8u), change_endian_long(0x96bfbe0du));
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
 End of function ./input_dir/RX65NHU/RX65NHU_p98f_r1.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_AES_128_CCM_DECRYPT == 1 */
