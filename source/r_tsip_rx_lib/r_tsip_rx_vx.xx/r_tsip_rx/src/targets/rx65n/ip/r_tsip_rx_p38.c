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
* Function Name: R_TSIP_Tls13SVGenerateEcdheSharedSecretSub
*******************************************************************************************************************/ /**
* @details       RX65NHU TLS1.3 ECDHE Shared Secret Calculation for Server
* @param[in]     InData_Cmd
* @param[in]     InData_PubKey
* @param[in]     InData_KeyIndex
* @param[out]    OutData_SharedSecretKeyIndex
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13SVGenerateEcdheSharedSecretSub(uint32_t *InData_Cmd, uint32_t *InData_PubKey, uint32_t *InData_KeyIndex, uint32_t *OutData_SharedSecretKeyIndex)
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
    TSIP.REG_84H.WORD = 0x00003801u;
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
    TSIP.REG_ECH.WORD = 0x0000094au;
    TSIP.REG_104H.WORD = 0x00000f68u;
    TSIP.REG_E0H.WORD = 0x8090000au;
    for (iLoop = 0; iLoop < 16; iLoop = iLoop + 1)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_PubKey[iLoop + 0];
    }
    TSIP.REG_ECH.WORD = 0x00000b9cu;
    OFS_ADR = 128;
    RX65NHU_func004(OFS_ADR);
    TSIP.REG_ECH.WORD = 0x00000908u;
    TSIP.REG_E0H.WORD = 0x81880008u;
    TSIP.REG_00H.WORD = 0x00003823u;
    TSIP.REG_2CH.WORD = 0x00000013u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_ECH.WORD = 0x0000b500u;
    TSIP.REG_ECH.WORD = 0x00000020u;
    TSIP.REG_E0H.WORD = 0x81880008u;
    TSIP.REG_00H.WORD = 0x00003823u;
    TSIP.REG_2CH.WORD = 0x0000001bu;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
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
    TSIP.REG_34H.WORD = 0x00000800u;
    TSIP.REG_24H.WORD = 0x8002d008u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    RX65NHU_func008();
    RX65NHU_func100(change_endian_long(0x34158e44u), change_endian_long(0x0e743ac6u), change_endian_long(0x7f998b9fu), change_endian_long(0xa47b6412u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0x6df214f6u), change_endian_long(0x6f37d282u), change_endian_long(0x5e2baa1au), change_endian_long(0xb38990bdu));
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
        TSIP.REG_E0H.WORD = 0x80010000u;
        TSIP.REG_104H.WORD = 0x00000068u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_Cmd[0];
        RX65NHU_func401();
        TSIP.REG_104H.WORD = 0x00000368u;
        TSIP.REG_E0H.WORD = 0x80040280u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[0];
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[1];
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[2];
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[3];
        RX65NHU_func075();
        TSIP.REG_C4H.WORD = 0x400c0b0cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x984520bcu);
        TSIP.REG_104H.WORD = 0x00000762u;
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x02f087bfu;
        TSIP.REG_00H.WORD = 0x00003223u;
        TSIP.REG_2CH.WORD = 0x00000011u;
        for (iLoop = 0; iLoop < 8; iLoop = iLoop + 4)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[iLoop+4 + 0];
            TSIP.REG_100H.WORD = InData_KeyIndex[iLoop+4 + 1];
            TSIP.REG_100H.WORD = InData_KeyIndex[iLoop+4 + 2];
            TSIP.REG_100H.WORD = InData_KeyIndex[iLoop+4 + 3];
        }
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
        TSIP.REG_100H.WORD = InData_KeyIndex[12];
        TSIP.REG_100H.WORD = InData_KeyIndex[13];
        TSIP.REG_100H.WORD = InData_KeyIndex[14];
        TSIP.REG_100H.WORD = InData_KeyIndex[15];
        TSIP.REG_C4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00002213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX65NHU_func100(change_endian_long(0x22c9dd42u), change_endian_long(0x2927c889u), change_endian_long(0x6350e04cu), change_endian_long(0xc515588au));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func102(change_endian_long(0xd71ae118u), change_endian_long(0x91a1d8fau), change_endian_long(0x0d73bc77u), change_endian_long(0xa87c18fdu));
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
            RX65NHU_func100(change_endian_long(0xb2d9bebfu), change_endian_long(0x071d7148u), change_endian_long(0x9c1f75d1u), change_endian_long(0xa0860184u));
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
            TSIP.REG_34H.WORD = 0x00000003u;
            TSIP.REG_24H.WORD = 0x8000e808u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_34H.WORD = 0x00000003u;
            TSIP.REG_24H.WORD = 0x800048d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_ECH.WORD = 0x00000908u;
            TSIP.REG_E0H.WORD = 0x81880008u;
            TSIP.REG_00H.WORD = 0x00003823u;
            TSIP.REG_2CH.WORD = 0x00000010u;
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
            TSIP.REG_34H.WORD = 0x00000800u;
            TSIP.REG_24H.WORD = 0x800050d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_ECH.WORD = 0x0000b500u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            TSIP.REG_E0H.WORD = 0x81880008u;
            TSIP.REG_00H.WORD = 0x00003823u;
            TSIP.REG_2CH.WORD = 0x00000010u;
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
            TSIP.REG_34H.WORD = 0x00000800u;
            TSIP.REG_24H.WORD = 0x800070d0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
            TSIP.REG_24H.WORD = 0x00005cd0u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_24H.BIT.B21)
            {
                /* waiting */
            }
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
            TSIP.REG_18H.WORD = 0x00000004u;
            TSIP.REG_38H.WORD = 0x00001000u;
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
            RX65NHU_func100(change_endian_long(0xb9012402u), change_endian_long(0x7f28416bu), change_endian_long(0x176284ceu), change_endian_long(0xaec83a2au));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX65NHU_func102(change_endian_long(0x776a46b1u), change_endian_long(0x70c2eb28u), change_endian_long(0x542937bau), change_endian_long(0x2f6f112eu));
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
                RX65NHU_func100(change_endian_long(0xf9f90e94u), change_endian_long(0x41fd4a04u), change_endian_long(0xd56c4b1fu), change_endian_long(0x6af08b49u));
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
                RX65NHU_func100(change_endian_long(0xabcfd923u), change_endian_long(0x1e991a11u), change_endian_long(0x4d5f4a75u), change_endian_long(0x91907c9cu));
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
                TSIP.REG_100H.WORD = change_endian_long(0x8343adc1u);
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
                TSIP.REG_04H.WORD = 0x00000222u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_SharedSecretKeyIndex[4] = TSIP.REG_100H.WORD;
                OutData_SharedSecretKeyIndex[5] = TSIP.REG_100H.WORD;
                OutData_SharedSecretKeyIndex[6] = TSIP.REG_100H.WORD;
                OutData_SharedSecretKeyIndex[7] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_SharedSecretKeyIndex[8] = TSIP.REG_100H.WORD;
                OutData_SharedSecretKeyIndex[9] = TSIP.REG_100H.WORD;
                OutData_SharedSecretKeyIndex[10] = TSIP.REG_100H.WORD;
                OutData_SharedSecretKeyIndex[11] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x4dbb150fu), change_endian_long(0x0a27298cu), change_endian_long(0x71401bfau), change_endian_long(0x44523737u));
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
                OutData_SharedSecretKeyIndex[12] = TSIP.REG_100H.WORD;
                OutData_SharedSecretKeyIndex[13] = TSIP.REG_100H.WORD;
                OutData_SharedSecretKeyIndex[14] = TSIP.REG_100H.WORD;
                OutData_SharedSecretKeyIndex[15] = TSIP.REG_100H.WORD;
                RX65NHU_func100(change_endian_long(0x2fc8bfc9u), change_endian_long(0x4375d19eu), change_endian_long(0x79f5ae47u), change_endian_long(0x6bd13eafu));
                TSIP.REG_E0H.WORD = 0x81040280u;
                TSIP.REG_04H.WORD = 0x00000612u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_SharedSecretKeyIndex[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_SharedSecretKeyIndex[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_SharedSecretKeyIndex[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_SharedSecretKeyIndex[3] = TSIP.REG_100H.WORD;
                RX65NHU_func102(change_endian_long(0x415c4bb2u), change_endian_long(0xfdb46d1eu), change_endian_long(0x7fe47cfeu), change_endian_long(0x450113eeu));
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
    }
}
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_p38_r1.prc
 *********************************************************************************************************************/
