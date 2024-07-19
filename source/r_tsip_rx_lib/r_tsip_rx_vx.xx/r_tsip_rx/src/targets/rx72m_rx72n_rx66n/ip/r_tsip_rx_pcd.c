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
* Function Name: R_TSIP_Tls13GenerateApplicationTrafficKeySub
*******************************************************************************************************************/ /**
* @details       RX72M TLS1.3 Application Traffic Key Generation
* @param[in]     InData_Cmd
* @param[in]     InData_Handle
* @param[in]     InData_Master_SecretKeyIndex
* @param[in]     InData_MsgHash
* @param[out]    OutData_ServerApplicationSecretKeyIndex
* @param[out]    OutData_ClientApplicationSecretKeyIndex
* @param[out]    OutData_ServerWriteKeyIndex
* @param[out]    OutData_ClientWriteKeyIndex
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13GenerateApplicationTrafficKeySub(uint32_t *InData_Cmd, uint32_t *InData_Handle, uint32_t *InData_Master_SecretKeyIndex, uint32_t *InData_MsgHash, uint32_t *OutData_ServerApplicationSecretKeyIndex, uint32_t *OutData_ClientApplicationSecretKeyIndex, uint32_t *OutData_ServerWriteKeyIndex, uint32_t *OutData_ClientWriteKeyIndex)
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
    TSIP.REG_84H.WORD = 0x0000cd01u;
    TSIP.REG_108H.WORD = 0x00000000u;
    TSIP.REG_C4H.WORD = 0x200e1a0du;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_RAM[16+0 + 0];
    TSIP.REG_100H.WORD = S_RAM[16+0 + 1];
    TSIP.REG_100H.WORD = S_RAM[16+0 + 2];
    TSIP.REG_100H.WORD = S_RAM[16+0 + 3];
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_104H.WORD = 0x00000068u;
    TSIP.REG_E0H.WORD = 0x80010000u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Cmd[0];
    RX72M_RX72N_RX66N_func401();
    TSIP.REG_104H.WORD = 0x00000368u;
    TSIP.REG_E0H.WORD = 0x80040280u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Handle[0];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Handle[1];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Handle[2];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Handle[3];
    RX72M_RX72N_RX66N_func072();
    TSIP.REG_C4H.WORD = 0x400c0b0cu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xc46a0014u);
    TSIP.REG_104H.WORD = 0x00000762u;
    TSIP.REG_D0H.WORD = 0x00000100u;
    TSIP.REG_C4H.WORD = 0x02f087bfu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Master_SecretKeyIndex[0];
    TSIP.REG_100H.WORD = InData_Master_SecretKeyIndex[1];
    TSIP.REG_100H.WORD = InData_Master_SecretKeyIndex[2];
    TSIP.REG_100H.WORD = InData_Master_SecretKeyIndex[3];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Master_SecretKeyIndex[4];
    TSIP.REG_100H.WORD = InData_Master_SecretKeyIndex[5];
    TSIP.REG_100H.WORD = InData_Master_SecretKeyIndex[6];
    TSIP.REG_100H.WORD = InData_Master_SecretKeyIndex[7];
    TSIP.REG_E0H.WORD = 0x80080000u;
    TSIP.REG_00H.WORD = 0x00008223u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_C4H.WORD = 0x000007bdu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Master_SecretKeyIndex[8];
    TSIP.REG_100H.WORD = InData_Master_SecretKeyIndex[9];
    TSIP.REG_100H.WORD = InData_Master_SecretKeyIndex[10];
    TSIP.REG_100H.WORD = InData_Master_SecretKeyIndex[11];
    TSIP.REG_C4H.WORD = 0x00900c45u;
    TSIP.REG_00H.WORD = 0x00002213u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0xe7b59f80u), change_endian_long(0xfb58dc66u), change_endian_long(0xaedc5917u), change_endian_long(0x58cf1c83u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0xc3175a1au), change_endian_long(0x60c896a1u), change_endian_long(0x435bdd3au), change_endian_long(0xc023a281u));
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
        RX72M_RX72N_RX66N_func100(change_endian_long(0x0456eda7u), change_endian_long(0x97b14659u), change_endian_long(0x6a226b2bu), change_endian_long(0x8be208fdu));
        TSIP.REG_ECH.WORD = 0x00000bffu;
        TSIP.REG_104H.WORD = 0x00000768u;
        TSIP.REG_E0H.WORD = 0x8088001fu;
        for (iLoop = 0; iLoop < 8; iLoop = iLoop + 1)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_MsgHash[iLoop + 0];
        }
        TSIP.REG_ECH.WORD = 0x0000b7e0u;
        TSIP.REG_ECH.WORD = 0x00000020u;
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x8081001fu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x01800000u);
        TSIP.REG_ECH.WORD = 0x0000b7e0u;
        TSIP.REG_ECH.WORD = 0x00000030u;
        TSIP.REG_104H.WORD = 0x00000458u;
        TSIP.REG_E0H.WORD = 0x8085001fu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00201274u);
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x6c733133u);
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x20732061u);
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x70207472u);
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x61666669u);
        TSIP.REG_ECH.WORD = 0x0000b540u;
        TSIP.REG_ECH.WORD = 0x00006320u;
        RX72M_RX72N_RX66N_func071();
        TSIP.REG_00H.WORD = 0x00003523u;
        TSIP.REG_74H.WORD = 0x00000008u;
        TSIP.REG_2CH.WORD = 0x00000092u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_ECH.WORD = 0x0000b7e0u;
        TSIP.REG_ECH.WORD = 0x00000030u;
        TSIP.REG_104H.WORD = 0x00000458u;
        TSIP.REG_E0H.WORD = 0x8085001fu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00201274u);
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x6c733133u);
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x20632061u);
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x70207472u);
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x61666669u);
        TSIP.REG_ECH.WORD = 0x0000b540u;
        TSIP.REG_ECH.WORD = 0x00006320u;
        RX72M_RX72N_RX66N_func071();
        TSIP.REG_00H.WORD = 0x00003523u;
        TSIP.REG_74H.WORD = 0x00000008u;
        TSIP.REG_2CH.WORD = 0x00000012u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_E0H.WORD = 0x80080000u;
        TSIP.REG_00H.WORD = 0x00008323u;
        TSIP.REG_2CH.WORD = 0x000000a2u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x008f0001u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        TSIP.REG_104H.WORD = 0x00000357u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00100974u);
        TSIP.REG_100H.WORD = change_endian_long(0x6c733133u);
        TSIP.REG_100H.WORD = change_endian_long(0x206b6579u);
        TSIP.REG_100H.WORD = change_endian_long(0x00018000u);
        TSIP.REG_00H.WORD = 0x0000302fu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000057u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000270u);
        RX72M_RX72N_RX66N_func070();
        TSIP.REG_ECH.WORD = 0x0000b7e0u;
        TSIP.REG_ECH.WORD = 0x00000050u;
        TSIP.REG_E0H.WORD = 0x8084001fu;
        TSIP.REG_00H.WORD = 0x00008513u;
        TSIP.REG_74H.WORD = 0x00000008u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_00H.WORD = 0x00000513u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x008f0001u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        TSIP.REG_104H.WORD = 0x00000357u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x000c0874u);
        TSIP.REG_100H.WORD = change_endian_long(0x6c733133u);
        TSIP.REG_100H.WORD = change_endian_long(0x20697600u);
        TSIP.REG_100H.WORD = change_endian_long(0x01800000u);
        TSIP.REG_00H.WORD = 0x0000302fu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000057u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000268u);
        RX72M_RX72N_RX66N_func070();
        TSIP.REG_ECH.WORD = 0x0000b7e0u;
        TSIP.REG_ECH.WORD = 0x00000060u;
        TSIP.REG_E0H.WORD = 0x8084001fu;
        TSIP.REG_00H.WORD = 0x00008513u;
        TSIP.REG_74H.WORD = 0x00000008u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_00H.WORD = 0x00000513u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_E0H.WORD = 0x80080000u;
        TSIP.REG_00H.WORD = 0x00008323u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x008f0001u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        TSIP.REG_104H.WORD = 0x00000357u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00100974u);
        TSIP.REG_100H.WORD = change_endian_long(0x6c733133u);
        TSIP.REG_100H.WORD = change_endian_long(0x206b6579u);
        TSIP.REG_100H.WORD = change_endian_long(0x00018000u);
        TSIP.REG_00H.WORD = 0x0000302fu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000057u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000270u);
        RX72M_RX72N_RX66N_func070();
        TSIP.REG_ECH.WORD = 0x0000b7e0u;
        TSIP.REG_ECH.WORD = 0x00000070u;
        TSIP.REG_E0H.WORD = 0x8084001fu;
        TSIP.REG_00H.WORD = 0x00008513u;
        TSIP.REG_74H.WORD = 0x00000008u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_00H.WORD = 0x00000513u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_28H.WORD = 0x008f0001u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        TSIP.REG_104H.WORD = 0x00000357u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x000c0874u);
        TSIP.REG_100H.WORD = change_endian_long(0x6c733133u);
        TSIP.REG_100H.WORD = change_endian_long(0x20697600u);
        TSIP.REG_100H.WORD = change_endian_long(0x01800000u);
        TSIP.REG_00H.WORD = 0x0000302fu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000057u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000268u);
        RX72M_RX72N_RX66N_func070();
        TSIP.REG_ECH.WORD = 0x0000b7e0u;
        TSIP.REG_ECH.WORD = 0x00000080u;
        TSIP.REG_E0H.WORD = 0x8084001fu;
        TSIP.REG_00H.WORD = 0x00008513u;
        TSIP.REG_74H.WORD = 0x00000008u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_00H.WORD = 0x00000513u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_C4H.WORD = 0x400c0b0cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x114e17c8u);
        TSIP.REG_28H.WORD = 0x00870001u;
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x02e087bfu;
        TSIP.REG_00H.WORD = 0x00002323u;
        TSIP.REG_2CH.WORD = 0x000000a2u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000352u;
        TSIP.REG_C4H.WORD = 0x000009cdu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000002u);
        TSIP.REG_04H.WORD = 0x00000232u;
        for (iLoop = 0; iLoop < 12; iLoop = iLoop + 4)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_ServerApplicationSecretKeyIndex[iLoop + 0] = TSIP.REG_100H.WORD;
            OutData_ServerApplicationSecretKeyIndex[iLoop + 1] = TSIP.REG_100H.WORD;
            OutData_ServerApplicationSecretKeyIndex[iLoop + 2] = TSIP.REG_100H.WORD;
            OutData_ServerApplicationSecretKeyIndex[iLoop + 3] = TSIP.REG_100H.WORD;
        }
        RX72M_RX72N_RX66N_func100(change_endian_long(0x3bf42924u), change_endian_long(0x26fcd726u), change_endian_long(0x0bc9325bu), change_endian_long(0x3b732adeu));
        TSIP.REG_C4H.WORD = 0x400c0b0cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x7dd8ec5au);
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x02e087bfu;
        TSIP.REG_00H.WORD = 0x00002323u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000352u;
        TSIP.REG_C4H.WORD = 0x000009cdu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000002u);
        TSIP.REG_04H.WORD = 0x00000232u;
        for (iLoop = 0; iLoop < 12; iLoop = iLoop + 4)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_ClientApplicationSecretKeyIndex[iLoop + 0] = TSIP.REG_100H.WORD;
            OutData_ClientApplicationSecretKeyIndex[iLoop + 1] = TSIP.REG_100H.WORD;
            OutData_ClientApplicationSecretKeyIndex[iLoop + 2] = TSIP.REG_100H.WORD;
            OutData_ClientApplicationSecretKeyIndex[iLoop + 3] = TSIP.REG_100H.WORD;
        }
        RX72M_RX72N_RX66N_func100(change_endian_long(0x48c69157u), change_endian_long(0xf78f9a8fu), change_endian_long(0x3f847c9cu), change_endian_long(0xd048c874u));
        TSIP.REG_C4H.WORD = 0x00082b8du;
        TSIP.REG_E0H.WORD = 0x81040280u;
        TSIP.REG_00H.WORD = 0x00002813u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_C4H.WORD = 0x400c0b0cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x98bae316u);
        TSIP.REG_ECH.WORD = 0x0000b7e0u;
        TSIP.REG_ECH.WORD = 0x00000050u;
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x02e087bfu;
        TSIP.REG_E0H.WORD = 0x8188001fu;
        TSIP.REG_00H.WORD = 0x00002823u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000352u;
        TSIP.REG_C4H.WORD = 0x000009cdu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000002u);
        TSIP.REG_04H.WORD = 0x00000232u;
        for (iLoop = 0; iLoop < 12; iLoop = iLoop + 4)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_ServerWriteKeyIndex[iLoop+4 + 0] = TSIP.REG_100H.WORD;
            OutData_ServerWriteKeyIndex[iLoop+4 + 1] = TSIP.REG_100H.WORD;
            OutData_ServerWriteKeyIndex[iLoop+4 + 2] = TSIP.REG_100H.WORD;
            OutData_ServerWriteKeyIndex[iLoop+4 + 3] = TSIP.REG_100H.WORD;
        }
        RX72M_RX72N_RX66N_func100(change_endian_long(0x96c41df2u), change_endian_long(0xad6f2145u), change_endian_long(0x16ad13d5u), change_endian_long(0xb14bcc2au));
        TSIP.REG_E0H.WORD = 0x81040280u;
        TSIP.REG_04H.WORD = 0x00000612u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_ServerWriteKeyIndex[0] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_ServerWriteKeyIndex[1] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_ServerWriteKeyIndex[2] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_ServerWriteKeyIndex[3] = TSIP.REG_100H.WORD;
        RX72M_RX72N_RX66N_func100(change_endian_long(0xff94f445u), change_endian_long(0x9e39320eu), change_endian_long(0x7ca3105fu), change_endian_long(0x045aff7au));
        TSIP.REG_C4H.WORD = 0x400c0b0cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x6ad6575eu);
        TSIP.REG_ECH.WORD = 0x0000b7e0u;
        TSIP.REG_ECH.WORD = 0x00000070u;
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x02e087bfu;
        TSIP.REG_E0H.WORD = 0x8188001fu;
        TSIP.REG_00H.WORD = 0x00002823u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000352u;
        TSIP.REG_C4H.WORD = 0x000009cdu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000002u);
        TSIP.REG_04H.WORD = 0x00000232u;
        for (iLoop = 0; iLoop < 12; iLoop = iLoop + 4)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_ClientWriteKeyIndex[iLoop+4 + 0] = TSIP.REG_100H.WORD;
            OutData_ClientWriteKeyIndex[iLoop+4 + 1] = TSIP.REG_100H.WORD;
            OutData_ClientWriteKeyIndex[iLoop+4 + 2] = TSIP.REG_100H.WORD;
            OutData_ClientWriteKeyIndex[iLoop+4 + 3] = TSIP.REG_100H.WORD;
        }
        RX72M_RX72N_RX66N_func100(change_endian_long(0xb800a525u), change_endian_long(0x64838ac6u), change_endian_long(0x94d790a8u), change_endian_long(0xecca56eau));
        TSIP.REG_E0H.WORD = 0x81040280u;
        TSIP.REG_04H.WORD = 0x00000612u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_ClientWriteKeyIndex[0] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_ClientWriteKeyIndex[1] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_ClientWriteKeyIndex[2] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_ClientWriteKeyIndex[3] = TSIP.REG_100H.WORD;
        RX72M_RX72N_RX66N_func102(change_endian_long(0x17347d62u), change_endian_long(0xd6e5ea44u), change_endian_long(0xa6c19114u), change_endian_long(0x058584d7u));
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
 End of function ./input_dir/RX72M/RX72M_pcd_r1.prc
 *********************************************************************************************************************/
