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

#if TSIP_AES_128_CMAC == 1
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
* Function Name: R_TSIP_Aes128CmacFinalSub
*******************************************************************************************************************/ /**
* @details       RX65NHU AES-128 CMAC generation/verification Final
* @param[in]     InData_Cmd
* @param[in]     InData_Text
* @param[in]     InData_DataT
* @param[in]     InData_DataTLen
* @param[out]    OutData_DataT
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_AUTHENTICATION
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128CmacFinalSub(uint32_t *InData_Cmd, uint32_t *InData_Text, uint32_t *InData_DataT, uint32_t *InData_DataTLen, uint32_t *OutData_DataT)
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
    RX65NHU_func100(change_endian_long(0xe69db767u), change_endian_long(0x1091291fu), change_endian_long(0xc51fe0d3u), change_endian_long(0xd98ac3fcu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0x94a1bc36u), change_endian_long(0x81590ab6u), change_endian_long(0x0ffc2e59u), change_endian_long(0xf3f0d536u));
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
        if ((InData_Cmd[0] == 0x00000000) || (InData_Cmd[0] == 0x00000002))
        {
            TSIP.REG_104H.WORD = 0x00000051u;
            TSIP.REG_A4H.WORD = 0x00400a84u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            RX65NHU_func101(change_endian_long(0x3b65c7e3u), change_endian_long(0x42824de4u), change_endian_long(0xdc058e10u), change_endian_long(0x37e3bd00u));
        }
        else
        {
            TSIP.REG_104H.WORD = 0x00000051u;
            TSIP.REG_A4H.WORD = 0x00500a84u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            RX65NHU_func101(change_endian_long(0xed04e6c1u), change_endian_long(0x35afe5d4u), change_endian_long(0x7a76a618u), change_endian_long(0x56dfff26u));
        }
        TSIP.REG_A4H.WORD = 0x00040c05u;
        TSIP.REG_00H.WORD = 0x00001113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        if ((InData_Cmd[0] == 0x00000000) || (InData_Cmd[0] == 0x00000001))
        {
            RX65NHU_func100(change_endian_long(0xa350ae51u), change_endian_long(0xb9731d2du), change_endian_long(0xc187c045u), change_endian_long(0x6b8e37c2u));
            TSIP.REG_104H.WORD = 0x00000361u;
            TSIP.REG_A4H.WORD = 0x00000e95u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_Text[0];
            TSIP.REG_100H.WORD = InData_Text[1];
            TSIP.REG_100H.WORD = InData_Text[2];
            TSIP.REG_100H.WORD = InData_Text[3];
            TSIP.REG_04H.WORD = 0x00000113u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_DataT[0] = TSIP.REG_100H.WORD;
            OutData_DataT[1] = TSIP.REG_100H.WORD;
            OutData_DataT[2] = TSIP.REG_100H.WORD;
            OutData_DataT[3] = TSIP.REG_100H.WORD;
            RX65NHU_func102(change_endian_long(0xeb1a986bu), change_endian_long(0x127d2bdau), change_endian_long(0x560a18f7u), change_endian_long(0x8d761376u));
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
        else
        {
            TSIP.REG_104H.WORD = 0x00000068u;
            TSIP.REG_E0H.WORD = 0x80010040u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_DataTLen[0];
            TSIP.REG_ECH.WORD = 0x3420a840u;
            TSIP.REG_ECH.WORD = 0x00000010u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_ECH.WORD = 0x0000b460u;
            TSIP.REG_ECH.WORD = 0x00000080u;
            TSIP.REG_ECH.WORD = 0x34202862u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            RX65NHU_func100(change_endian_long(0xa27b118eu), change_endian_long(0xc4c8f46bu), change_endian_long(0x5ab008e5u), change_endian_long(0xbe4e3fe2u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX65NHU_func102(change_endian_long(0x6f21a4dfu), change_endian_long(0x991da9f2u), change_endian_long(0xf6ed6f11u), change_endian_long(0x87638fd0u));
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
                TSIP.REG_104H.WORD = 0x00000361u;
                TSIP.REG_A4H.WORD = 0x00000e95u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_Text[0];
                TSIP.REG_100H.WORD = InData_Text[1];
                TSIP.REG_100H.WORD = InData_Text[2];
                TSIP.REG_100H.WORD = InData_Text[3];
                TSIP.REG_ECH.WORD = 0x00000821u;
                TSIP.REG_E0H.WORD = 0x80840001u;
                TSIP.REG_00H.WORD = 0x00008113u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x000034e2u;
                TSIP.REG_ECH.WORD = 0x000568e7u;
                TSIP.REG_ECH.WORD = 0x00026ce7u;
                TSIP.REG_ECH.WORD = 0x00003827u;
                TSIP.REG_ECH.WORD = 0x0000b4c0u;
                TSIP.REG_ECH.WORD = 0x00000020u;
                TSIP.REG_ECH.WORD = 0x00003402u;
                TSIP.REG_ECH.WORD = 0x00008c00u;
                TSIP.REG_ECH.WORD = 0x0000001fu;
                TSIP.REG_ECH.WORD = 0x000028c0u;
                TSIP.REG_ECH.WORD = 0x00008cc0u;
                TSIP.REG_ECH.WORD = 0x0000001fu;
                TSIP.REG_ECH.WORD = 0x00004406u;
                TSIP.REG_ECH.WORD = 0x00007421u;
                TSIP.REG_ECH.WORD = 0x00007821u;
                TSIP.REG_ECH.WORD = 0x00003c27u;
                TSIP.REG_ECH.WORD = 0x000034c2u;
                TSIP.REG_ECH.WORD = 0x0000a4c0u;
                TSIP.REG_ECH.WORD = 0x0000001fu;
                TSIP.REG_ECH.WORD = 0x000568c6u;
                TSIP.REG_ECH.WORD = 0x000034e6u;
                TSIP.REG_ECH.WORD = 0x00026ce7u;
                TSIP.REG_ECH.WORD = 0x00000821u;
                for (iLoop = 0; iLoop < 4; iLoop = iLoop+1)
                {
                    TSIP.REG_ECH.WORD = 0x3420a8e0u;
                    TSIP.REG_ECH.WORD = 0x0000000du;
                    TSIP.REG_ECH.WORD = 0x10003c27u;
                    TSIP.REG_ECH.WORD = 0x1000a4e0u;
                    TSIP.REG_ECH.WORD = 0x00000004u;
                }
                TSIP.REG_A4H.WORD = 0x00040805u;
                TSIP.REG_E0H.WORD = 0x81840001u;
                TSIP.REG_00H.WORD = 0x00001813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_104H.WORD = 0x00000361u;
                TSIP.REG_A4H.WORD = 0x00900c05u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_DataT[0];
                TSIP.REG_100H.WORD = InData_DataT[1];
                TSIP.REG_100H.WORD = InData_DataT[2];
                TSIP.REG_100H.WORD = InData_DataT[3];
                RX65NHU_func100(change_endian_long(0x16f87d59u), change_endian_long(0x34063281u), change_endian_long(0x2c34dedau), change_endian_long(0xa3f67aeau));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    RX65NHU_func102(change_endian_long(0xc8875d71u), change_endian_long(0xe12507a3u), change_endian_long(0x43fe722bu), change_endian_long(0x3be85740u));
                    TSIP.REG_1BCH.WORD = 0x00000040u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_18H.BIT.B12)
                    {
                        /* waiting */
                    }
                    #if TSIP_MULTI_THREADING == 1
                    TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
                    #endif /* TSIP_MULTI_THREADING == 1 */
                    return TSIP_ERR_AUTHENTICATION;
                }
                else
                {
                    RX65NHU_func102(change_endian_long(0x6281e1d7u), change_endian_long(0x179fc750u), change_endian_long(0x03934b27u), change_endian_long(0xbd2d8ae5u));
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
}
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_p41f.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_AES_128_CMAC == 1 */
