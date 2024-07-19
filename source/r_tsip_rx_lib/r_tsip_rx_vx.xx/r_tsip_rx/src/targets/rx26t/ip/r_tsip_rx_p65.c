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
* Function Name: R_TSIP_Aes256EncryptDecryptFinalSub
*******************************************************************************************************************/ /**
* @details       RX26T AES-256 Encryption/Decryption with ECB/CBC/CTR Mode Final
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256EncryptDecryptFinalSub(void)
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
        RX26T_function002(change_endian_long(0x88973677u), change_endian_long(0x88773c23u), change_endian_long(0x7d7afca1u), change_endian_long(0x6dcd95eeu));
    }
    else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX26T_function002(change_endian_long(0x90fb38ddu), change_endian_long(0x3bf9ccafu), change_endian_long(0xc251e4c3u), change_endian_long(0x8dc4c592u));
    }
    else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX26T_function002(change_endian_long(0xa6172d1bu), change_endian_long(0x45fd2fbeu), change_endian_long(0x1b183303u), change_endian_long(0x79d47b67u));
    }
    else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX26T_function002(change_endian_long(0x3edca010u), change_endian_long(0xee6a9f2du), change_endian_long(0x297083afu), change_endian_long(0x30536f26u));
    }
    else if (0x04000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX26T_function002(change_endian_long(0x088d9d97u), change_endian_long(0xf52e0e3eu), change_endian_long(0x53f65e97u), change_endian_long(0x2755773cu));
    }
    RX26T_function001(change_endian_long(0x1b416143u), change_endian_long(0xa2a25c82u), change_endian_long(0x1f3da879u), change_endian_long(0x7235cbc4u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_1CH.WORD = 0x002d0000u;
        RX26T_function001(change_endian_long(0x2dd9d0c5u), change_endian_long(0x194b1be2u), change_endian_long(0x8169c590u), change_endian_long(0xb1e70862u));
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
            RX26T_function002(change_endian_long(0xc1fb1f87u), change_endian_long(0x8fa4cc5eu), change_endian_long(0x06488375u), change_endian_long(0x414bf1f6u));
        }
        else
        {
            RX26T_function002(change_endian_long(0x4db21124u), change_endian_long(0x54931e2du), change_endian_long(0x366fd797u), change_endian_long(0x5aaf8ae1u));
        }
        RX26T_function003(change_endian_long(0x91e72524u), change_endian_long(0x7024e43cu), change_endian_long(0x6246ee58u), change_endian_long(0x816a0814u));
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
        RX26T_function003(change_endian_long(0x1ce7e715u), change_endian_long(0xf266d40fu), change_endian_long(0xa7ce4411u), change_endian_long(0x95d31878u));
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
 End of function ./input_dir/RX26T/RX26T_p65.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_PRV_USE_AES_256 == 1 */
