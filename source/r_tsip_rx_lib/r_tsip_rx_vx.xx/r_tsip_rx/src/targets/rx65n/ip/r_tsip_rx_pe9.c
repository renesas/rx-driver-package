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
* Function Name: R_TSIP_GenerateTlsP256EccKeyIndexSub
*******************************************************************************************************************/ /**
* @details       RX65NHU TLS P-256 ECC Key Generation
* @param[out]    OutData_KeyIndex
* @param[out]    OutData_PubKey
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateTlsP256EccKeyIndexSub(uint32_t *OutData_KeyIndex, uint32_t *OutData_PubKey)
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
    if (0x0u != (TSIP.REG_1B8H.WORD & 0x1eu))
    {
        #if TSIP_MULTI_THREADING == 1
        TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
        #endif /* TSIP_MULTI_THREADING == 1 */
        return TSIP_ERR_RESOURCE_CONFLICT;
    }
    TSIP.REG_84H.WORD = 0x0000e901u;
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
    TSIP.REG_ECH.WORD = 0x00000b9cu;
    KEY_ADR = 128;
    RX65NHU_func004(KEY_ADR);
    TSIP.REG_24H.WORD = 0x000084d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00029008u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000800u;
    TSIP.REG_24H.WORD = 0x8000c0c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000400u;
    TSIP.REG_24H.WORD = 0x8000c0d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00890001u;
    RX65NHU_func100(change_endian_long(0xe5e13b93u), change_endian_long(0x0e688489u), change_endian_long(0x6970785eu), change_endian_long(0xa8808ff9u));
    RX65NHU_func103();
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x01000c84u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_00H.WORD = 0x00003213u;
    TSIP.REG_2CH.WORD = 0x00000011u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX65NHU_func100(change_endian_long(0x17b57eceu), change_endian_long(0xd2518523u), change_endian_long(0xcfa75f65u), change_endian_long(0x3354e7ddu));
    RX65NHU_func103();
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x01000c84u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_00H.WORD = 0x00003213u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX65NHU_func100(change_endian_long(0x185f9b3fu), change_endian_long(0x19954d5du), change_endian_long(0x2be2e33fu), change_endian_long(0xf71b0a14u));
    RX65NHU_func103();
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x01000c84u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
    TSIP.REG_00H.WORD = 0x0000320bu;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_00H.WORD = 0x0000020bu;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_24H.WORD = 0x000019c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00000591u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000c0c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_18H.WORD = 0x00000004u;
    TSIP.REG_24H.WORD = 0x0000a206u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_18H.BIT.B10)
    {
        /* waiting */
    }
    TSIP.REG_18H.WORD = 0x00000000u;
    TSIP.REG_24H.WORD = 0x000016c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00000682u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x0000a0c2u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_24H.WORD = 0x0000dcd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x000084d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00029008u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000800u;
    TSIP.REG_24H.WORD = 0x8000c0c1u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    RX65NHU_func005(KEY_ADR);
    RX65NHU_func100(change_endian_long(0x9070018du), change_endian_long(0x6f683202u), change_endian_long(0x07e651f9u), change_endian_long(0x9a7cc875u));
    TSIP.REG_34H.WORD = 0x00000802u;
    TSIP.REG_24H.WORD = 0x800088d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000802u;
    TSIP.REG_24H.WORD = 0x8000acd0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_34H.WORD = 0x00000003u;
    TSIP.REG_24H.WORD = 0x8000e808u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_18H.WORD = 0x00000004u;
    TSIP.REG_38H.WORD = 0x00001030u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_18H.BIT.B10)
    {
        /* waiting */
    }
    TSIP.REG_18H.WORD = 0x00000000u;
    TSIP.REG_34H.WORD = 0x00000c00u;
    TSIP.REG_24H.WORD = 0x800094d0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00001dc0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_24H.WORD = 0x00000591u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001f00u;
    TSIP.REG_1CH.WORD = 0x00210000u;
    RX65NHU_func100(change_endian_long(0xb683e810u), change_endian_long(0x1b4e9ab9u), change_endian_long(0xf4c8f211u), change_endian_long(0x00dcb106u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0xfa3e01b6u), change_endian_long(0xeb4913f1u), change_endian_long(0xafbf60e1u), change_endian_long(0x0c6bb0c8u));
        TSIP.REG_1B8H.WORD = 0x00000040u;
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
        RX65NHU_func100(change_endian_long(0xc2c8ae1au), change_endian_long(0x64701074u), change_endian_long(0x81f6bb01u), change_endian_long(0x2904032cu));
        TSIP.REG_24H.WORD = 0x00009cd0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x000019c0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00000591u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00000591u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x0000a0c1u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_18H.WORD = 0x00000004u;
        TSIP.REG_38H.WORD = 0x000000b0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_18H.BIT.B10)
        {
            /* waiting */
        }
        TSIP.REG_18H.WORD = 0x00000000u;
        TSIP.REG_34H.WORD = 0x00000003u;
        TSIP.REG_24H.WORD = 0x800048d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x0000880cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x0000a8d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_34H.WORD = 0x00000008u;
        TSIP.REG_24H.WORD = 0x800050d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00004a0cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x0000480cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x0000480cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_34H.WORD = 0x00000008u;
        TSIP.REG_24H.WORD = 0x800074d0u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00006e0cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00006c0cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00006c0cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00006c0cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_24H.WORD = 0x00006e0cu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_24H.BIT.B21)
        {
            /* waiting */
        }
        TSIP.REG_104H.WORD = 0x00000058u;
        TSIP.REG_E0H.WORD = 0x80010000u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x39020100u);
        RX65NHU_func100(change_endian_long(0x9132cda7u), change_endian_long(0x6e4ee1ccu), change_endian_long(0x2017ad12u), change_endian_long(0xda414cc2u));
        RX65NHU_func103();
        RX65NHU_func100(change_endian_long(0x33a09072u), change_endian_long(0x979cb5c4u), change_endian_long(0x8b6a710fu), change_endian_long(0x48c57b19u));
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x01000c84u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_E0H.WORD = 0x80030020u;
        TSIP.REG_00H.WORD = 0x0000820fu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_00H.WORD = 0x00000207u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_C4H.WORD = 0x000c2b0du;
        TSIP.REG_E0H.WORD = 0x81040000u;
        TSIP.REG_00H.WORD = 0x00002813u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x02e087bfu;
        TSIP.REG_00H.WORD = 0x00002323u;
        TSIP.REG_2CH.WORD = 0x000000a8u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_04H.WORD = 0x00000223u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_KeyIndex[4] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[5] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[6] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[7] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_KeyIndex[8] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[9] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[10] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[11] = TSIP.REG_100H.WORD;
        RX65NHU_func100(change_endian_long(0xc6e1e506u), change_endian_long(0xce0e15eeu), change_endian_long(0xf0a6ec4du), change_endian_long(0x91613aa0u));
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x00000c84u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_C4H.WORD = 0x000009cdu;
        TSIP.REG_00H.WORD = 0x00002213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_04H.WORD = 0x00000213u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_KeyIndex[12] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[13] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[14] = TSIP.REG_100H.WORD;
        OutData_KeyIndex[15] = TSIP.REG_100H.WORD;
        RX65NHU_func100(change_endian_long(0x5f654fcfu), change_endian_long(0x97666ec7u), change_endian_long(0xe6c25c20u), change_endian_long(0x6fc96a03u));
        TSIP.REG_E0H.WORD = 0x81040000u;
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
        RX65NHU_func100(change_endian_long(0xb82b40ceu), change_endian_long(0xb472c030u), change_endian_long(0xdf12ffe4u), change_endian_long(0x2823c83fu));
        TSIP.REG_2CH.WORD = 0x00000022u;
        TSIP.REG_04H.WORD = 0x00000322u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKey[0] = TSIP.REG_100H.WORD;
        OutData_PubKey[1] = TSIP.REG_100H.WORD;
        OutData_PubKey[2] = TSIP.REG_100H.WORD;
        OutData_PubKey[3] = TSIP.REG_100H.WORD;
        OutData_PubKey[4] = TSIP.REG_100H.WORD;
        OutData_PubKey[5] = TSIP.REG_100H.WORD;
        OutData_PubKey[6] = TSIP.REG_100H.WORD;
        OutData_PubKey[7] = TSIP.REG_100H.WORD;
        RX65NHU_func100(change_endian_long(0x69464cacu), change_endian_long(0xeec63231u), change_endian_long(0xb45dc80au), change_endian_long(0x0842b8e9u));
        TSIP.REG_2CH.WORD = 0x00000023u;
        TSIP.REG_04H.WORD = 0x00000322u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKey[8] = TSIP.REG_100H.WORD;
        OutData_PubKey[9] = TSIP.REG_100H.WORD;
        OutData_PubKey[10] = TSIP.REG_100H.WORD;
        OutData_PubKey[11] = TSIP.REG_100H.WORD;
        OutData_PubKey[12] = TSIP.REG_100H.WORD;
        OutData_PubKey[13] = TSIP.REG_100H.WORD;
        OutData_PubKey[14] = TSIP.REG_100H.WORD;
        OutData_PubKey[15] = TSIP.REG_100H.WORD;
        RX65NHU_func102(change_endian_long(0x461943f5u), change_endian_long(0x9dd683eau), change_endian_long(0xd0c790a2u), change_endian_long(0xc2e8b347u));
        TSIP.REG_1B8H.WORD = 0x00000040u;
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
 End of function ./input_dir/RX65NHU/RX65NHU_pe9_r2.prc
 *********************************************************************************************************************/
