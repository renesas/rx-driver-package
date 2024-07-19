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

#if TSIP_PRV_USE_TDES == 1
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
* Function Name: R_TSIP_TdesEncryptDecryptInitSub
*******************************************************************************************************************/ /**
* @details       RX671 TDES Encryption/Decryption with ECB/CBC/CTR Mode Init
* @param[in]     InData_Cmd
* @param[in]     InData_KeyIndex
* @param[in]     InData_IV
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_TdesEncryptDecryptInitSub(uint32_t *InData_Cmd, uint32_t *InData_KeyIndex, uint32_t *InData_IV)
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
    TSIP.REG_84H.WORD = 0x0000d901u;
    TSIP.REG_108H.WORD = 0x00000000u;
    TSIP.REG_104H.WORD = 0x00000361u;
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
    TSIP.REG_100H.WORD = change_endian_long(0x971df67bu);
    RX671_func100(change_endian_long(0xf04a29f6u), change_endian_long(0x7925dc43u), change_endian_long(0x0274087du), change_endian_long(0x1bd04a65u));
    TSIP.REG_A4H.WORD = 0x02f8073du;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[4];
    TSIP.REG_100H.WORD = InData_KeyIndex[5];
    TSIP.REG_100H.WORD = InData_KeyIndex[6];
    TSIP.REG_100H.WORD = InData_KeyIndex[7];
    RX671_func100(change_endian_long(0x69cf965fu), change_endian_long(0x8ab2c16cu), change_endian_long(0x4527e231u), change_endian_long(0x2edea25du));
    TSIP.REG_A4H.WORD = 0x00f9073du;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[8];
    TSIP.REG_100H.WORD = InData_KeyIndex[9];
    TSIP.REG_100H.WORD = InData_KeyIndex[10];
    TSIP.REG_100H.WORD = InData_KeyIndex[11];
    TSIP.REG_A4H.WORD = 0x000007bdu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[12];
    TSIP.REG_100H.WORD = InData_KeyIndex[13];
    TSIP.REG_100H.WORD = InData_KeyIndex[14];
    TSIP.REG_100H.WORD = InData_KeyIndex[15];
    TSIP.REG_A4H.WORD = 0x00900c45u;
    TSIP.REG_00H.WORD = 0x00001113u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX671_func100(change_endian_long(0x76847ae7u), change_endian_long(0x601dd34eu), change_endian_long(0x2696305eu), change_endian_long(0xff70b721u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func102(change_endian_long(0x92f577e4u), change_endian_long(0x1a714353u), change_endian_long(0x40f5824bu), change_endian_long(0xfd726cfbu));
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
        TSIP.REG_E0H.WORD = 0x80010000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_104H.WORD = 0x00000068u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_Cmd[0];
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x3000a800u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00000004u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00010020u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x0000b400u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00000004u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00000080u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        RX671_func100(change_endian_long(0xd550f7fbu), change_endian_long(0xee1e8d47u), change_endian_long(0x5120a8f3u), change_endian_long(0xca2fc371u));
        TSIP.REG_ECH.WORD = 0x00007c00u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_1CH.WORD = 0x00600000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
        {
            RX671_func101(change_endian_long(0xc0dccd82u), change_endian_long(0x61ae047cu), change_endian_long(0xff739d87u), change_endian_long(0x64f53a11u));
        }
        else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
        {
            RX671_func101(change_endian_long(0x392c97efu), change_endian_long(0x134c6d08u), change_endian_long(0xa9045651u), change_endian_long(0x31bba93du));
        }
        else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
        {
            TSIP.REG_104H.WORD = 0x00000361u;
            TSIP.REG_A4H.WORD = 0x00040805u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_IV[0];
            TSIP.REG_100H.WORD = InData_IV[1];
            TSIP.REG_100H.WORD = InData_IV[2];
            TSIP.REG_100H.WORD = InData_IV[3];
            RX671_func101(change_endian_long(0xf4d9653du), change_endian_long(0x1d46f5c9u), change_endian_long(0xce277e43u), change_endian_long(0x00037ccdu));
        }
        else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
        {
            TSIP.REG_104H.WORD = 0x00000361u;
            TSIP.REG_A4H.WORD = 0x00040805u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_IV[0];
            TSIP.REG_100H.WORD = InData_IV[1];
            TSIP.REG_100H.WORD = InData_IV[2];
            TSIP.REG_100H.WORD = InData_IV[3];
            RX671_func101(change_endian_long(0xbfec736du), change_endian_long(0xa6171535u), change_endian_long(0xb9fc4585u), change_endian_long(0xa9856067u));
        }
        else if (0x04000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
        {
            TSIP.REG_104H.WORD = 0x00000361u;
            TSIP.REG_A4H.WORD = 0x00040805u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_IV[0];
            TSIP.REG_100H.WORD = InData_IV[1];
            TSIP.REG_100H.WORD = InData_IV[2];
            TSIP.REG_100H.WORD = InData_IV[3];
            RX671_func101(change_endian_long(0x8b62da6cu), change_endian_long(0x4e60d85du), change_endian_long(0x18119748u), change_endian_long(0x9b84473du));
        }
        return TSIP_SUCCESS;
    }
}
/**********************************************************************************************************************
 End of function ./input_dir/RX671/RX671_pd9i.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_PRV_USE_TDES == 1 */
