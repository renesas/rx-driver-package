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
* Function Name: R_TSIP_Aes128EncryptDecryptFinalSub
*******************************************************************************************************************/ /**
* @details       RX231HU AES-128 Encryption/Decryption with ECB/CBC/CTR Mode Final
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128EncryptDecryptFinalSub(void)
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
        RX231_RX23W_function002(change_endian_long(0x767d57b9u), change_endian_long(0xdeb54735u), change_endian_long(0x6e24d978u), change_endian_long(0xa531d23du));
    }
    else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX231_RX23W_function002(change_endian_long(0x119a1c50u), change_endian_long(0xdd330b83u), change_endian_long(0xff9c3ba5u), change_endian_long(0xd2eb391fu));
    }
    else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX231_RX23W_function002(change_endian_long(0x4c3ac402u), change_endian_long(0x87c1e752u), change_endian_long(0xcae98d77u), change_endian_long(0x345a1f3au));
    }
    else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX231_RX23W_function002(change_endian_long(0x126d3bc1u), change_endian_long(0xed9bd19au), change_endian_long(0xd36e37eau), change_endian_long(0x44d7a38fu));
    }
    else if (0x04000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX231_RX23W_function002(change_endian_long(0xee70f1fdu), change_endian_long(0x0a789375u), change_endian_long(0xed0189b0u), change_endian_long(0xc63269a6u));
    }
    RX231_RX23W_function001(change_endian_long(0x0a75a973u), change_endian_long(0x47382b35u), change_endian_long(0x68815f45u), change_endian_long(0x739efef8u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_1CH.WORD = 0x002d0000u;
        RX231_RX23W_function001(change_endian_long(0x1e7e0be3u), change_endian_long(0xb383b544u), change_endian_long(0xdd355194u), change_endian_long(0xbe30572au));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_00H.WORD = 0x00000113u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX231_RX23W_function002(change_endian_long(0x23c66eb7u), change_endian_long(0x821e788cu), change_endian_long(0xb2906b1eu), change_endian_long(0x9e3b204eu));
        }
        else
        {
            RX231_RX23W_function002(change_endian_long(0x30cadc60u), change_endian_long(0xf75c3db7u), change_endian_long(0x2fa5828au), change_endian_long(0x745e71dfu));
        }
        RX231_RX23W_function003(change_endian_long(0x2f6bbefau), change_endian_long(0xe9cbdbb9u), change_endian_long(0xcc140630u), change_endian_long(0xeac6b31cu));
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
        RX231_RX23W_function003(change_endian_long(0x89389203u), change_endian_long(0x4e483a55u), change_endian_long(0xe7e58f93u), change_endian_long(0x08546f50u));
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
 End of function ./input_dir/RX231HU/RX231HU_p62.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_PRV_USE_AES_128 == 1 */
