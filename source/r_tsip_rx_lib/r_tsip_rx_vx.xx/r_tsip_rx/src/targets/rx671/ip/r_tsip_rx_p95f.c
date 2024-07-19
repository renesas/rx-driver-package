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

#if TSIP_AES_128_CCM_ENCRYPT == 1
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
* Function Name: R_TSIP_Aes128CcmEncryptFinalSub
*******************************************************************************************************************/ /**
* @details       RX671 AES-128 Encryption with CCM Mode Final
* @param[in]     InData_Text
* @param[out]    OutData_Text
* @param[out]    OutData_MAC
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CcmEncryptFinalSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t *OutData_MAC)
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
    RX671_func100(change_endian_long(0xbdfda339u), change_endian_long(0x5e4d17f9u), change_endian_long(0x16b57178u), change_endian_long(0x5fc19f5du));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func102(change_endian_long(0x00db3422u), change_endian_long(0x33c2238eu), change_endian_long(0x6a366590u), change_endian_long(0xc0c87445u));
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
        RX671_func100(change_endian_long(0x99f31b3du), change_endian_long(0xc1b57462u), change_endian_long(0x33c82512u), change_endian_long(0x35460100u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_104H.WORD = 0x00000361u;
            TSIP.REG_A4H.WORD = 0x00e007b5u;
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
            RX671_func100(change_endian_long(0x5db418e0u), change_endian_long(0xae048ca7u), change_endian_long(0x0fe67ee7u), change_endian_long(0xeb5a57a7u));
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
            RX671_func101(change_endian_long(0xdda23c4du), change_endian_long(0xbc974dd5u), change_endian_long(0xd87be84cu), change_endian_long(0x4a9a9487u));
        }
        else
        {
            RX671_func101(change_endian_long(0x21a1b114u), change_endian_long(0xff884d41u), change_endian_long(0x48e13695u), change_endian_long(0xe2595852u));
        }
        TSIP.REG_104H.WORD = 0x00000051u;
        TSIP.REG_A4H.WORD = 0x01000c84u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        RX671_func100(change_endian_long(0xd8bde98cu), change_endian_long(0xb2df1282u), change_endian_long(0xb6d15ee2u), change_endian_long(0x2adfb3f9u));
        TSIP.REG_A4H.WORD = 0x000009c5u;
        TSIP.REG_00H.WORD = 0x00001113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_04H.WORD = 0x00000112u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_MAC[0] = TSIP.REG_100H.WORD;
        OutData_MAC[1] = TSIP.REG_100H.WORD;
        OutData_MAC[2] = TSIP.REG_100H.WORD;
        OutData_MAC[3] = TSIP.REG_100H.WORD;
        RX671_func102(change_endian_long(0xd7c226c8u), change_endian_long(0x3a946c58u), change_endian_long(0x1bbf157eu), change_endian_long(0xa592d048u));
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
 End of function ./input_dir/RX671/RX671_p95f_r1.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_AES_128_CCM_ENCRYPT == 1 */
