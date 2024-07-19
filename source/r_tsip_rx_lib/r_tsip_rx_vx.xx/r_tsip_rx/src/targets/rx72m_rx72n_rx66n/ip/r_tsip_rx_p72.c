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

#if TSIP_SHA_256 == 1
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
* Function Name: R_TSIP_Sha224256GenerateMessageDigestSub
*******************************************************************************************************************/ /**
* @details       RX72M SHA-224/256 Message Digest Generation
* @param[in]     InData_SHA_InitVal
* @param[in]     InData_PaddedMsg
* @param[in]     MAX_CNT
* @param[out]    OutData_MsgDigest
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @note          None
*/
e_tsip_err_t R_TSIP_Sha224256GenerateMessageDigestSub(uint32_t *InData_SHA_InitVal, uint32_t *InData_PaddedMsg, uint32_t MAX_CNT, uint32_t *OutData_MsgDigest)
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
    TSIP.REG_84H.WORD = 0x00007201u;
    TSIP.REG_108H.WORD = 0x00000000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x20eddffau), change_endian_long(0xdb09d74bu), change_endian_long(0x955d865eu), change_endian_long(0xb1af64bau));
    TSIP.REG_7CH.WORD = 0x00000011u;
    TSIP.REG_104H.WORD = 0x00000764u;
    TSIP.REG_74H.WORD = 0x00000004u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_SHA_InitVal[0];
    TSIP.REG_100H.WORD = InData_SHA_InitVal[1];
    TSIP.REG_100H.WORD = InData_SHA_InitVal[2];
    TSIP.REG_100H.WORD = InData_SHA_InitVal[3];
    TSIP.REG_100H.WORD = InData_SHA_InitVal[4];
    TSIP.REG_100H.WORD = InData_SHA_InitVal[5];
    TSIP.REG_100H.WORD = InData_SHA_InitVal[6];
    TSIP.REG_100H.WORD = InData_SHA_InitVal[7];
    TSIP.REG_104H.WORD = 0x000000b4u;
    TSIP.REG_74H.WORD = 0x08000002u;
    for (iLoop = 0; iLoop < MAX_CNT ; iLoop = iLoop + 16)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_PaddedMsg[0+iLoop + 0];
        TSIP.REG_100H.WORD = InData_PaddedMsg[0+iLoop + 1];
        TSIP.REG_100H.WORD = InData_PaddedMsg[0+iLoop + 2];
        TSIP.REG_100H.WORD = InData_PaddedMsg[0+iLoop + 3];
        TSIP.REG_100H.WORD = InData_PaddedMsg[0+iLoop + 4];
        TSIP.REG_100H.WORD = InData_PaddedMsg[0+iLoop + 5];
        TSIP.REG_100H.WORD = InData_PaddedMsg[0+iLoop + 6];
        TSIP.REG_100H.WORD = InData_PaddedMsg[0+iLoop + 7];
        TSIP.REG_100H.WORD = InData_PaddedMsg[0+iLoop + 8];
        TSIP.REG_100H.WORD = InData_PaddedMsg[0+iLoop + 9];
        TSIP.REG_100H.WORD = InData_PaddedMsg[0+iLoop + 10];
        TSIP.REG_100H.WORD = InData_PaddedMsg[0+iLoop + 11];
        TSIP.REG_100H.WORD = InData_PaddedMsg[0+iLoop + 12];
        TSIP.REG_100H.WORD = InData_PaddedMsg[0+iLoop + 13];
        TSIP.REG_100H.WORD = InData_PaddedMsg[0+iLoop + 14];
        TSIP.REG_100H.WORD = InData_PaddedMsg[0+iLoop + 15];
    }
    /* WAIT_LOOP */
    while (0u != TSIP.REG_74H.BIT.B18)
    {
        /* waiting */
    }
    TSIP.REG_74H.WORD = 0x00000000u;
    TSIP.REG_104H.WORD = 0x00000000u;
    TSIP.REG_1CH.WORD = 0x00001600u;
    TSIP.REG_74H.WORD = 0x00000008u;
    TSIP.REG_04H.WORD = 0x00000523u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_MsgDigest[0] = TSIP.REG_100H.WORD;
    OutData_MsgDigest[1] = TSIP.REG_100H.WORD;
    OutData_MsgDigest[2] = TSIP.REG_100H.WORD;
    OutData_MsgDigest[3] = TSIP.REG_100H.WORD;
    OutData_MsgDigest[4] = TSIP.REG_100H.WORD;
    OutData_MsgDigest[5] = TSIP.REG_100H.WORD;
    OutData_MsgDigest[6] = TSIP.REG_100H.WORD;
    OutData_MsgDigest[7] = TSIP.REG_100H.WORD;
    RX72M_RX72N_RX66N_func102(change_endian_long(0x1794e51fu), change_endian_long(0x0370f401u), change_endian_long(0x98cbef1cu), change_endian_long(0xa844c1b2u));
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
/**********************************************************************************************************************
 End of function ./input_dir/RX72M/RX72M_p72.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_SHA_256 == 1 */
