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

#if TSIP_SHA_256_HMAC == 1
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
* Function Name: R_TSIP_Sha256HmacFinalSub
*******************************************************************************************************************/ /**
* @details       RX671 HMAC-SHA256 Message Digest Final
* @param[in]     InData_Cmd
* @param[in]     InData_MAC
* @param[in]     InData_length
* @param[out]    OutData_MAC
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @note          None
*/
e_tsip_err_t R_TSIP_Sha256HmacFinalSub(uint32_t *InData_Cmd, uint32_t *InData_MAC, uint32_t *InData_length, uint32_t *OutData_MAC)
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
    TSIP.REG_74H.WORD = 0x00000000u;
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_00H.WORD = 0x00003523u;
    TSIP.REG_74H.WORD = 0x00000008u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_7CH.WORD = 0x00000011u;
    TSIP.REG_104H.WORD = 0x00000754u;
    TSIP.REG_74H.WORD = 0x00000004u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x6a09e667u);
    TSIP.REG_100H.WORD = change_endian_long(0xbb67ae85u);
    TSIP.REG_100H.WORD = change_endian_long(0x3c6ef372u);
    TSIP.REG_100H.WORD = change_endian_long(0xa54ff53au);
    TSIP.REG_100H.WORD = change_endian_long(0x510e527fu);
    TSIP.REG_100H.WORD = change_endian_long(0x9b05688cu);
    TSIP.REG_100H.WORD = change_endian_long(0x1f83d9abu);
    TSIP.REG_100H.WORD = change_endian_long(0x5be0cd19u);
    TSIP.REG_74H.WORD = 0x00000002u;
    RX671_func002();
    TSIP.REG_00H.WORD = 0x00005323u;
    TSIP.REG_2CH.WORD = 0x00000020u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_104H.WORD = 0x00000054u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x80000000u);
    TSIP.REG_F8H.WORD = 0x00000040u;
    TSIP.REG_104H.WORD = 0x00000154u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_100H.WORD = change_endian_long(0x00000300u);
    /* WAIT_LOOP */
    while (0u != TSIP.REG_74H.BIT.B18)
    {
        /* waiting */
    }
    TSIP.REG_74H.WORD = 0x00000000u;
    TSIP.REG_1CH.WORD = 0x00001600u;
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
    TSIP.REG_ECH.WORD = 0x38000c00u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x1000b780u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00200000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x2000b780u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00100000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x38000f9du;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    RX671_func100(change_endian_long(0x183858c4u), change_endian_long(0xa374ed9bu), change_endian_long(0x87aa9594u), change_endian_long(0x20032bdbu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func102(change_endian_long(0xc0f71963u), change_endian_long(0x6f4fd399u), change_endian_long(0x94b6b9f1u), change_endian_long(0x72e9393cu));
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
        TSIP.REG_ECH.WORD = 0x38000c00u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX671_func100(change_endian_long(0x04735fdcu), change_endian_long(0xf9c78b7au), change_endian_long(0x9fad864cu), change_endian_long(0xb1871d6fu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX671_func100(change_endian_long(0x1b4c836bu), change_endian_long(0x35c9b8abu), change_endian_long(0x20c8841eu), change_endian_long(0x5fb9231au));
            TSIP.REG_74H.WORD = 0x00000008u;
            TSIP.REG_04H.WORD = 0x00000523u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_MAC[0] = TSIP.REG_100H.WORD;
            OutData_MAC[1] = TSIP.REG_100H.WORD;
            OutData_MAC[2] = TSIP.REG_100H.WORD;
            OutData_MAC[3] = TSIP.REG_100H.WORD;
            OutData_MAC[4] = TSIP.REG_100H.WORD;
            OutData_MAC[5] = TSIP.REG_100H.WORD;
            OutData_MAC[6] = TSIP.REG_100H.WORD;
            OutData_MAC[7] = TSIP.REG_100H.WORD;
            RX671_func102(change_endian_long(0x595f2953u), change_endian_long(0x4664a213u), change_endian_long(0xe09e57ddu), change_endian_long(0x0125c0f5u));
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
            TSIP.REG_E0H.WORD = 0x80010020u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_length[0];
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x3420a820u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000004u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            TSIP.REG_ECH.WORD = 0x3420a820u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000021u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            RX671_func100(change_endian_long(0x19781412u), change_endian_long(0x8ea30cb4u), change_endian_long(0x3864dbf0u), change_endian_long(0x35edd1c1u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX671_func102(change_endian_long(0x61ae8536u), change_endian_long(0xb096ef95u), change_endian_long(0xe98d2305u), change_endian_long(0xe5860c70u));
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
                TSIP.REG_ECH.WORD = 0x00000bffu;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_E0H.WORD = 0x8088001fu;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_00H.WORD = 0x00008523u;
                TSIP.REG_74H.WORD = 0x00000008u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x00000800u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                for (iLoop = 0; iLoop < 32; iLoop = iLoop+1)
                {
                    TSIP.REG_ECH.WORD = 0x3c002be1u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x12003c1fu;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00002fe0u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                }
                TSIP.REG_A4H.WORD = 0x00040805u;
                TSIP.REG_ECH.WORD = 0x00000bffu;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_E0H.WORD = 0x8188001fu;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_00H.WORD = 0x00001813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_A4H.WORD = 0x00050805u;
                TSIP.REG_00H.WORD = 0x00001813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_104H.WORD = 0x00000761u;
                TSIP.REG_A4H.WORD = 0x00900c05u;
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
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_MAC[4];
                TSIP.REG_100H.WORD = InData_MAC[5];
                TSIP.REG_100H.WORD = InData_MAC[6];
                TSIP.REG_100H.WORD = InData_MAC[7];
                RX671_func100(change_endian_long(0x5c8b67a4u), change_endian_long(0xe30a4770u), change_endian_long(0xcb8eeba9u), change_endian_long(0xc96a7f5bu));
                TSIP.REG_1CH.WORD = 0x00400000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (1u == (TSIP.REG_1CH.BIT.B22))
                {
                    RX671_func102(change_endian_long(0xcef103aau), change_endian_long(0xb6f6cfb2u), change_endian_long(0x6f3d5ae8u), change_endian_long(0x4fe84f62u));
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
                    RX671_func102(change_endian_long(0xe645468fu), change_endian_long(0xebb5ddceu), change_endian_long(0x66a10b52u), change_endian_long(0x3acb355du));
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
 End of function ./input_dir/RX671/RX671_p76f.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_SHA_256_HMAC == 1 */
