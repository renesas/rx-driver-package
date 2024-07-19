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

#if TSIP_ECDSA_P384 == 1
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
* Function Name: R_TSIP_GenerateEccP384RandomKeyIndexSub
*******************************************************************************************************************/ /**
* @details       RX65NHU P-384 ECC Key Generation
* @param[out]    OutData_PubKeyIndex
* @param[out]    OutData_PrivKeyIndex
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateEccP384RandomKeyIndexSub(uint32_t *OutData_PubKeyIndex, uint32_t *OutData_PrivKeyIndex)
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
    TSIP.REG_84H.WORD = 0x0000f901u;
    TSIP.REG_108H.WORD = 0x00000000u;
    RX65NHU_func100(change_endian_long(0xc5868dd3u), change_endian_long(0xb2da9cf4u), change_endian_long(0xde216907u), change_endian_long(0x825968dau));
    TSIP.REG_28H.WORD = 0x008b0001u;
    OFS_ADR = 692;
    RX65NHU_func027(OFS_ADR);
    TSIP.REG_ECH.WORD = 0x00000bffu;
    TSIP.REG_E0H.WORD = 0x808c001fu;
    TSIP.REG_00H.WORD = 0x00008333u;
    TSIP.REG_2CH.WORD = 0x00000023u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_28H.WORD = 0x008d0001u;
    RX65NHU_func103();
    RX65NHU_func100(change_endian_long(0x9e292366u), change_endian_long(0x4722a0f3u), change_endian_long(0xf7852217u), change_endian_long(0x1234b85eu));
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
    RX65NHU_func103();
    RX65NHU_func100(change_endian_long(0x86e191efu), change_endian_long(0xa89c03f2u), change_endian_long(0xd51c378fu), change_endian_long(0x21371259u));
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
    RX65NHU_func103();
    RX65NHU_func100(change_endian_long(0xc213fc3fu), change_endian_long(0x625bb1adu), change_endian_long(0xba78c2e1u), change_endian_long(0x56075c6au));
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
    TSIP.REG_28H.WORD = 0x008b0001u;
    RX65NHU_func100(change_endian_long(0x2205a59cu), change_endian_long(0x7638d6cbu), change_endian_long(0x7c98d6e4u), change_endian_long(0xfa15cb99u));
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
    RX65NHU_func028(OFS_ADR);
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
    TSIP.REG_ECH.WORD = 0x00000bffu;
    TSIP.REG_E0H.WORD = 0x818c001fu;
    TSIP.REG_00H.WORD = 0x00003833u;
    TSIP.REG_2CH.WORD = 0x00000010u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_24H.WORD = 0x0000f008u;
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
    TSIP.REG_34H.WORD = 0x00000800u;
    TSIP.REG_24H.WORD = 0x8000f4d0u;
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
    RX65NHU_func100(change_endian_long(0xf18bfd3bu), change_endian_long(0x4b77ad1du), change_endian_long(0xc3ed5177u), change_endian_long(0x4fd0ffc0u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0x28d80544u), change_endian_long(0x1512eaabu), change_endian_long(0x8196d45eu), change_endian_long(0x5c83f42du));
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
        RX65NHU_func100(change_endian_long(0xe5cd7337u), change_endian_long(0xfcd816dcu), change_endian_long(0x98e846a4u), change_endian_long(0x12f137edu));
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
        TSIP.REG_ECH.WORD = 0x00000bffu;
        TSIP.REG_E0H.WORD = 0x818c001fu;
        TSIP.REG_00H.WORD = 0x00003833u;
        TSIP.REG_2CH.WORD = 0x00000012u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
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
        RX65NHU_func100(change_endian_long(0xae37a725u), change_endian_long(0x04ffb48fu), change_endian_long(0xcdd91d9eu), change_endian_long(0x0bbd043bu));
        RX65NHU_func103();
        RX65NHU_func100(change_endian_long(0xe8c7a9c4u), change_endian_long(0x3da2452cu), change_endian_long(0xd9ef95fdu), change_endian_long(0xe0d03411u));
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x01000c84u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_E0H.WORD = 0x80040000u;
        TSIP.REG_00H.WORD = 0x00008213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_C4H.WORD = 0x200c3b0du;
        TSIP.REG_E0H.WORD = 0x81040000u;
        TSIP.REG_00H.WORD = 0x00002813u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_C4H.WORD = 0x000c0b0cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0xf237dc7eu);
        TSIP.REG_D0H.WORD = 0x00000200u;
        TSIP.REG_C4H.WORD = 0x02e087bfu;
        TSIP.REG_00H.WORD = 0x00002333u;
        TSIP.REG_2CH.WORD = 0x0000002du;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_04H.WORD = 0x00000232u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PrivKeyIndex[4] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[5] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[6] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[7] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PrivKeyIndex[8] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[9] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[10] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[11] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PrivKeyIndex[12] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[13] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[14] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[15] = TSIP.REG_100H.WORD;
        RX65NHU_func100(change_endian_long(0x6b0e30bbu), change_endian_long(0x5b2accb8u), change_endian_long(0xee942db0u), change_endian_long(0x37d78e42u));
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
        TSIP.REG_04H.WORD = 0x00000212u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PrivKeyIndex[16] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[17] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[18] = TSIP.REG_100H.WORD;
        OutData_PrivKeyIndex[19] = TSIP.REG_100H.WORD;
        RX65NHU_func100(change_endian_long(0x6dadf4eeu), change_endian_long(0xdc27069cu), change_endian_long(0xef4a3ea5u), change_endian_long(0xffa9b658u));
        TSIP.REG_E0H.WORD = 0x81040000u;
        TSIP.REG_04H.WORD = 0x00000612u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PrivKeyIndex[0] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PrivKeyIndex[1] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PrivKeyIndex[2] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PrivKeyIndex[3] = TSIP.REG_100H.WORD;
        RX65NHU_func100(change_endian_long(0xaa7926d5u), change_endian_long(0x172b7508u), change_endian_long(0xdd507394u), change_endian_long(0x7b954f6bu));
        RX65NHU_func103();
        RX65NHU_func100(change_endian_long(0x2372f1cdu), change_endian_long(0x738ab6edu), change_endian_long(0xc31a4398u), change_endian_long(0xc5d379adu));
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x01000c84u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_E0H.WORD = 0x80040000u;
        TSIP.REG_00H.WORD = 0x00008213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_C4H.WORD = 0x200c3b0du;
        TSIP.REG_E0H.WORD = 0x81040000u;
        TSIP.REG_00H.WORD = 0x00002813u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_C4H.WORD = 0x000c0b0cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x947339e0u);
        TSIP.REG_D0H.WORD = 0x00000200u;
        TSIP.REG_C4H.WORD = 0x02e0888fu;
        TSIP.REG_00H.WORD = 0x00002333u;
        TSIP.REG_2CH.WORD = 0x00000022u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_04H.WORD = 0x00000232u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[4] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[5] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[6] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[7] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[8] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[9] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[10] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[11] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[12] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[13] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[14] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[15] = TSIP.REG_100H.WORD;
        RX65NHU_func100(change_endian_long(0xf9ef1a28u), change_endian_long(0x06fdd47du), change_endian_long(0x25e827d5u), change_endian_long(0xdf556052u));
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x00e0888fu;
        TSIP.REG_00H.WORD = 0x00002323u;
        TSIP.REG_2CH.WORD = 0x00000023u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_04H.WORD = 0x00000222u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[16] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[17] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[18] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[19] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[20] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[21] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[22] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[23] = TSIP.REG_100H.WORD;
        RX65NHU_func100(change_endian_long(0x9ba35497u), change_endian_long(0xa7678903u), change_endian_long(0x7ce91a59u), change_endian_long(0xd054e117u));
        TSIP.REG_C4H.WORD = 0x0045094cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_C4H.WORD = 0x00e0088du;
        TSIP.REG_00H.WORD = 0x00002313u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_04H.WORD = 0x00000212u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[24] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[25] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[26] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[27] = TSIP.REG_100H.WORD;
        RX65NHU_func100(change_endian_long(0x5ceabf42u), change_endian_long(0x8e5a98b4u), change_endian_long(0x69c839a7u), change_endian_long(0xee4d7fdau));
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x00000cc4u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_04H.WORD = 0x00000212u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[28] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[29] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[30] = TSIP.REG_100H.WORD;
        OutData_PubKeyIndex[31] = TSIP.REG_100H.WORD;
        RX65NHU_func100(change_endian_long(0x6d039c6cu), change_endian_long(0x581c3df2u), change_endian_long(0x40b2a50fu), change_endian_long(0x770ff2c6u));
        TSIP.REG_E0H.WORD = 0x81040000u;
        TSIP.REG_04H.WORD = 0x00000612u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[0] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[1] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[2] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_PubKeyIndex[3] = TSIP.REG_100H.WORD;
        RX65NHU_func102(change_endian_long(0x5f7230d6u), change_endian_long(0x1280b1fau), change_endian_long(0xf137ac12u), change_endian_long(0xc644484du));
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
 End of function ./input_dir/RX65NHU/RX65NHU_pf9_r1.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_ECDSA_P384 == 1 */
