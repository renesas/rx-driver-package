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

#if (TSIP_ECDSA_P192 | TSIP_ECDSA_P224 | TSIP_ECDSA_P256) != 0
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
* Function Name: R_TSIP_EcdsaSigunatureVerificationSub
*******************************************************************************************************************/ /**
* @details       RX72M Signature verification of ECDSA-192/224/256
* @param[in]     InData_Cmd
* @param[in]     InData_KeyIndex
* @param[in]     InData_MsgDgst
* @param[in]     InData_Signature
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_EcdsaSigunatureVerificationSub(uint32_t *InData_Cmd, uint32_t *InData_KeyIndex, uint32_t *InData_MsgDgst, uint32_t *InData_Signature)
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
    TSIP.REG_84H.WORD = 0x0000f101u;
    TSIP.REG_108H.WORD = 0x00000000u;
    TSIP.REG_104H.WORD = 0x00000068u;
    TSIP.REG_E0H.WORD = 0x80010380u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Cmd[0];
    RX72M_RX72N_RX66N_func402();
    TSIP.REG_ECH.WORD = 0x00000bdeu;
    TSIP.REG_104H.WORD = 0x00001768u;
    TSIP.REG_E0H.WORD = 0x8098001eu;
    for (iLoop = 0; iLoop < 16; iLoop = iLoop + 1)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_Signature[iLoop + 0];
    }
    for (iLoop = 0; iLoop < 8; iLoop = iLoop + 1)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_MsgDgst[iLoop + 0];
    }
    for (iLoop = 0; iLoop < 3; iLoop = iLoop + 1)
    {
        TSIP.REG_ECH.WORD = 0x00000bffu;
        TSIP.REG_ECH.WORD = 0x30003380u;
        TSIP.REG_ECH.WORD = 0x00070020u;
        TSIP.REG_ECH.WORD = 0x0000d3e0u;
        TSIP.REG_ECH.WORD = 0x00030040u;
        TSIP.REG_ECH.WORD = 0x0000381eu;
        TSIP.REG_ECH.WORD = 0x38000c00u;
        TSIP.REG_ECH.WORD = 0x1000d3e0u;
        TSIP.REG_ECH.WORD = 0x00050040u;
        TSIP.REG_ECH.WORD = 0x0000381eu;
        TSIP.REG_ECH.WORD = 0x000037beu;
        TSIP.REG_ECH.WORD = 0x0000a7a0u;
        TSIP.REG_ECH.WORD = 0x00000004u;
        TSIP.REG_ECH.WORD = 0x0000383du;
        TSIP.REG_ECH.WORD = 0x38001001u;
        TSIP.REG_ECH.WORD = 0x1000d3e0u;
        TSIP.REG_ECH.WORD = 0x00000080u;
        TSIP.REG_ECH.WORD = 0x38000fffu;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        TSIP.REG_ECH.WORD = 0x0000a7c0u;
        TSIP.REG_ECH.WORD = 0x00000020u;
    }
    RX72M_RX72N_RX66N_func100(change_endian_long(0x32bc5cdfu), change_endian_long(0x0b4b01aeu), change_endian_long(0xc4327eacu), change_endian_long(0x213de0a5u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0x5630938bu), change_endian_long(0xa8484635u), change_endian_long(0x72a2a149u), change_endian_long(0xee1d640fu));
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
        TSIP.REG_28H.WORD = 0x00870001u;
        TSIP.REG_C4H.WORD = 0x200c3b0du;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[0];
        TSIP.REG_100H.WORD = InData_KeyIndex[1];
        TSIP.REG_100H.WORD = InData_KeyIndex[2];
        TSIP.REG_100H.WORD = InData_KeyIndex[3];
        RX72M_RX72N_RX66N_func023();
        TSIP.REG_104H.WORD = 0x00000b62u;
        TSIP.REG_D0H.WORD = 0x00000200u;
        TSIP.REG_C4H.WORD = 0x02f0888fu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[4];
        TSIP.REG_100H.WORD = InData_KeyIndex[5];
        TSIP.REG_100H.WORD = InData_KeyIndex[6];
        TSIP.REG_100H.WORD = InData_KeyIndex[7];
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[8];
        TSIP.REG_100H.WORD = InData_KeyIndex[9];
        TSIP.REG_100H.WORD = InData_KeyIndex[10];
        TSIP.REG_100H.WORD = InData_KeyIndex[11];
        TSIP.REG_00H.WORD = 0x00003223u;
        TSIP.REG_2CH.WORD = 0x0000009bu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[12];
        TSIP.REG_100H.WORD = InData_KeyIndex[13];
        TSIP.REG_100H.WORD = InData_KeyIndex[14];
        TSIP.REG_100H.WORD = InData_KeyIndex[15];
        TSIP.REG_C4H.WORD = 0x0045094cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_C4H.WORD = 0x00f0088du;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[16];
        TSIP.REG_100H.WORD = InData_KeyIndex[17];
        TSIP.REG_100H.WORD = InData_KeyIndex[18];
        TSIP.REG_100H.WORD = InData_KeyIndex[19];
        TSIP.REG_00H.WORD = 0x00003223u;
        TSIP.REG_2CH.WORD = 0x00000091u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000362u;
        TSIP.REG_C4H.WORD = 0x00000885u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[20];
        TSIP.REG_100H.WORD = InData_KeyIndex[21];
        TSIP.REG_100H.WORD = InData_KeyIndex[22];
        TSIP.REG_100H.WORD = InData_KeyIndex[23];
        TSIP.REG_C4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00002213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0x565bda65u), change_endian_long(0x60e17beau), change_endian_long(0x3dfab8b2u), change_endian_long(0x36d029b3u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func102(change_endian_long(0x5651c527u), change_endian_long(0xf719a343u), change_endian_long(0x448f1131u), change_endian_long(0x3b259c0eu));
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
            RX72M_RX72N_RX66N_func100(change_endian_long(0x1a8c6cc1u), change_endian_long(0x49fd01e7u), change_endian_long(0x81e10616u), change_endian_long(0xbc0bd6e0u));
            TSIP.REG_7CH.WORD = 0x00000011u;
            TSIP.REG_ECH.WORD = 0x0000b7c0u;
            TSIP.REG_ECH.WORD = 0x00000040u;
            TSIP.REG_E0H.WORD = 0x8188001eu;
            TSIP.REG_00H.WORD = 0x00005823u;
            TSIP.REG_74H.WORD = 0x00000004u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x30003380u;
            TSIP.REG_ECH.WORD = 0x00070020u;
            TSIP.REG_ECH.WORD = 0x0000b400u;
            TSIP.REG_ECH.WORD = 0x00000080u;
            TSIP.REG_ECH.WORD = 0x00030040u;
            TSIP.REG_ECH.WORD = 0x0000b400u;
            TSIP.REG_ECH.WORD = 0x0000013Cu;
            TSIP.REG_ECH.WORD = 0x00050040u;
            TSIP.REG_ECH.WORD = 0x0000b400u;
            TSIP.REG_ECH.WORD = 0x000001F8u;
            TSIP.REG_ECH.WORD = 0x00000080u;
            TSIP.REG_E0H.WORD = 0x81010000u;
            TSIP.REG_04H.WORD = 0x00000606u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
            OFS_ADR = S_RAM[0];
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x800103a0u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x000000f1u);
            RX72M_RX72N_RX66N_func101(change_endian_long(0x5f84057au), change_endian_long(0xb0f54303u), change_endian_long(0xf0fa3299u), change_endian_long(0x5998b62cu));
            RX72M_RX72N_RX66N_func010(OFS_ADR);
            RX72M_RX72N_RX66N_func100(change_endian_long(0x1cc008a4u), change_endian_long(0x83f859d9u), change_endian_long(0x9eed945du), change_endian_long(0x5753165bu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX72M_RX72N_RX66N_func102(change_endian_long(0x5da922d1u), change_endian_long(0x2aedc804u), change_endian_long(0x4f35c437u), change_endian_long(0xd128c81bu));
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
                RX72M_RX72N_RX66N_func102(change_endian_long(0xfe1199ccu), change_endian_long(0xc8f0d328u), change_endian_long(0x5abec99eu), change_endian_long(0x3c7061e8u));
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
 End of function ./input_dir/RX72M/RX72M_pf1_r1.prc
 *********************************************************************************************************************/
#endif /* #if (TSIP_ECDSA_P192 | TSIP_ECDSA_P224 | TSIP_ECDSA_P256) != 0 */
