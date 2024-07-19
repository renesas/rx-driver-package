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
* Function Name: R_TSIP_UpdateAes256KeyIndexSub
*******************************************************************************************************************/ /**
* @details       RX26T Update 256bit AES Key
* @param[in]     InData_IV
* @param[in]     InData_InstData
* @param[out]    OutData_KeyIndex
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @note          None
*/
e_tsip_err_t R_TSIP_UpdateAes256KeyIndexSub(uint32_t *InData_IV, uint32_t *InData_InstData, uint32_t *OutData_KeyIndex)
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
    if (0x0u != (TSIP.REG_1B4H.WORD & 0x1du))
    {
        #if TSIP_MULTI_THREADING == 1
        TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
        #endif /* TSIP_MULTI_THREADING == 1 */
        return TSIP_ERR_RESOURCE_CONFLICT;
    }
    TSIP.REG_84H.WORD = 0x00009201u;
    TSIP.REG_108H.WORD = 0x00000000u;
    RX26T_function001(change_endian_long(0x49dcb1bbu), change_endian_long(0x8a2eaad8u), change_endian_long(0x871d3327u), change_endian_long(0xcd43d918u));
    TSIP.REG_A4H.WORD = 0x600c3a0du;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_INST2[0];
    TSIP.REG_100H.WORD = S_INST2[1];
    TSIP.REG_100H.WORD = S_INST2[2];
    TSIP.REG_100H.WORD = S_INST2[3];
    TSIP.REG_A4H.WORD = 0x400c0a0cu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xda0168d1u);
    TSIP.REG_A4H.WORD = 0x42f8063du;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_INST2[4];
    TSIP.REG_100H.WORD = S_INST2[5];
    TSIP.REG_100H.WORD = S_INST2[6];
    TSIP.REG_100H.WORD = S_INST2[7];
    RX26T_function001(change_endian_long(0xe03aa68bu), change_endian_long(0x47184796u), change_endian_long(0x20bc760fu), change_endian_long(0x823bb80du));
    TSIP.REG_A4H.WORD = 0x40f9063du;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_INST2[4+4 + 0];
    TSIP.REG_100H.WORD = S_INST2[4+4 + 1];
    TSIP.REG_100H.WORD = S_INST2[4+4 + 2];
    TSIP.REG_100H.WORD = S_INST2[4+4 + 3];
    RX26T_function001(change_endian_long(0x707aa690u), change_endian_long(0xb6147726u), change_endian_long(0xa908c2bdu), change_endian_long(0xad0f041au));
    TSIP.REG_A4H.WORD = 0x400006bdu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_INST2[4+8 + 0];
    TSIP.REG_100H.WORD = S_INST2[4+8 + 1];
    TSIP.REG_100H.WORD = S_INST2[4+8 + 2];
    TSIP.REG_100H.WORD = S_INST2[4+8 + 3];
    TSIP.REG_A4H.WORD = 0x00800c45u;
    TSIP.REG_00H.WORD = 0x00001113u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
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
    TSIP.REG_104H.WORD = 0x00000051u;
    TSIP.REG_A4H.WORD = 0x00050804u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_104H.WORD = 0x00000251u;
    TSIP.REG_A4H.WORD = 0x0020363cu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_A4H.WORD = 0x002036bcu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_A4H.WORD = 0x002036bcu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_A4H.WORD = 0x00060805u;
    TSIP.REG_00H.WORD = 0x00001113u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_104H.WORD = 0x00000151u;
    TSIP.REG_A4H.WORD = 0x0021340cu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_A4H.WORD = 0x000b0805u;
    TSIP.REG_00H.WORD = 0x00001113u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_A4H.WORD = 0x01000684u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    RX26T_function001(change_endian_long(0x21802bb5u), change_endian_long(0x2c31c4b8u), change_endian_long(0x53acab1au), change_endian_long(0x6cec4e2fu));
    TSIP.REG_E0H.WORD = 0x80040000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_00H.WORD = 0x00008113u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_A4H.WORD = 0x600c3a0du;
    TSIP.REG_E0H.WORD = 0x81040000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_00H.WORD = 0x00001813u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_A4H.WORD = 0x400c0a0cu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x213f32e4u);
    TSIP.REG_104H.WORD = 0x00000361u;
    TSIP.REG_A4H.WORD = 0x00d049adu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_InstData[0];
    TSIP.REG_100H.WORD = InData_InstData[1];
    TSIP.REG_100H.WORD = InData_InstData[2];
    TSIP.REG_100H.WORD = InData_InstData[3];
    TSIP.REG_A4H.WORD = 0x43e006bdu;
    TSIP.REG_00H.WORD = 0x00001113u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_04H.WORD = 0x00000113u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_KeyIndex[4] = TSIP.REG_100H.WORD;
    OutData_KeyIndex[5] = TSIP.REG_100H.WORD;
    OutData_KeyIndex[6] = TSIP.REG_100H.WORD;
    OutData_KeyIndex[7] = TSIP.REG_100H.WORD;
    RX26T_function001(change_endian_long(0xaa2ebc8du), change_endian_long(0x2adaf945u), change_endian_long(0x4ae6390fu), change_endian_long(0x102bec00u));
    TSIP.REG_104H.WORD = 0x00000361u;
    TSIP.REG_A4H.WORD = 0x00d049adu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_InstData[4];
    TSIP.REG_100H.WORD = InData_InstData[5];
    TSIP.REG_100H.WORD = InData_InstData[6];
    TSIP.REG_100H.WORD = InData_InstData[7];
    TSIP.REG_A4H.WORD = 0x41e006bdu;
    TSIP.REG_00H.WORD = 0x00001113u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_04H.WORD = 0x00000113u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_KeyIndex[8] = TSIP.REG_100H.WORD;
    OutData_KeyIndex[9] = TSIP.REG_100H.WORD;
    OutData_KeyIndex[10] = TSIP.REG_100H.WORD;
    OutData_KeyIndex[11] = TSIP.REG_100H.WORD;
    RX26T_function001(change_endian_long(0xe43b626au), change_endian_long(0x2d44ddd6u), change_endian_long(0xbb15186eu), change_endian_long(0x4ff81bf0u));
    TSIP.REG_104H.WORD = 0x00000051u;
    TSIP.REG_A4H.WORD = 0x01000684u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_A4H.WORD = 0x410009cdu;
    TSIP.REG_00H.WORD = 0x00001113u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_04H.WORD = 0x00000113u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_04H.BIT.B30)
    {
        /* waiting */
    }
    OutData_KeyIndex[12] = TSIP.REG_100H.WORD;
    OutData_KeyIndex[13] = TSIP.REG_100H.WORD;
    OutData_KeyIndex[14] = TSIP.REG_100H.WORD;
    OutData_KeyIndex[15] = TSIP.REG_100H.WORD;
    TSIP.REG_104H.WORD = 0x00000361u;
    TSIP.REG_A4H.WORD = 0x000049adu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_InstData[8];
    TSIP.REG_100H.WORD = InData_InstData[9];
    TSIP.REG_100H.WORD = InData_InstData[10];
    TSIP.REG_100H.WORD = InData_InstData[11];
    TSIP.REG_A4H.WORD = 0x00900c45u;
    TSIP.REG_00H.WORD = 0x00001113u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX26T_function001(change_endian_long(0x0f31ad50u), change_endian_long(0x3a380bdeu), change_endian_long(0xa0e3c684u), change_endian_long(0xa5a5748fu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX26T_function003(change_endian_long(0xd6b766a7u), change_endian_long(0x51acfc20u), change_endian_long(0x8d67c07au), change_endian_long(0x56eccc0eu));
        TSIP.REG_1B4H.WORD = 0x00000040u;
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
        RX26T_function001(change_endian_long(0xd4ee53a6u), change_endian_long(0xbb452c6eu), change_endian_long(0x66afb6b5u), change_endian_long(0x795eaae4u));
        TSIP.REG_E0H.WORD = 0x81040000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_04H.WORD = 0x00000613u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_KeyIndex[0] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_KeyIndex[1] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_KeyIndex[2] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_KeyIndex[3] = TSIP.REG_100H.WORD;
        RX26T_function003(change_endian_long(0x3c53ec59u), change_endian_long(0xd5256484u), change_endian_long(0xf4dd07b5u), change_endian_long(0xf5fd70fbu));
        TSIP.REG_1B4H.WORD = 0x00000040u;
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
 End of function ./input_dir/RX26T/RX26T_p92_r1.prc
 *********************************************************************************************************************/
