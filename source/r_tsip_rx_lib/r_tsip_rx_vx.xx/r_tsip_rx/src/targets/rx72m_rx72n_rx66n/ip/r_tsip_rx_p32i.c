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

#if TSIP_AES_128_GCM_DECRYPT == 1
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
* Function Name: R_TSIP_Aes128GcmDecryptInitSub
*******************************************************************************************************************/ /**
* @details       RX72M AES-128 Decryption with GCM Mode Init
* @param[in]     InData_KeyType
* @param[in]     InData_DataType
* @param[in]     InData_Cmd
* @param[in]     InData_KeyIndex
* @param[in]     InData_IV
* @param[in]     InData_SeqNum
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_Aes128GcmDecryptInitSub(uint32_t *InData_KeyType, uint32_t *InData_DataType, uint32_t *InData_Cmd, uint32_t *InData_KeyIndex, uint32_t *InData_IV, uint32_t *InData_SeqNum)
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
    TSIP.REG_84H.WORD = 0x00003201u;
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
    TSIP.REG_104H.WORD = 0x00000068u;
    TSIP.REG_E0H.WORD = 0x80010100u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyType[0];
    TSIP.REG_ECH.WORD = 0x3020a900u;
    TSIP.REG_ECH.WORD = 0x00000005u;
    TSIP.REG_ECH.WORD = 0x00060020u;
    TSIP.REG_ECH.WORD = 0x0000b500u;
    TSIP.REG_ECH.WORD = 0x00000004u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_ECH.WORD = 0x38000d08u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_ECH.WORD = 0x38008900u;
    TSIP.REG_ECH.WORD = 0x00000002u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x83f2e156u), change_endian_long(0xae7b863eu), change_endian_long(0x59a24b7fu), change_endian_long(0x12f2a564u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_ECH.WORD = 0x38000d08u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0x47ae27bbu), change_endian_long(0x35d60baau), change_endian_long(0x6e621a89u), change_endian_long(0x9575a768u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
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
            TSIP.REG_C4H.WORD = 0x000c0b0cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x4e2279dbu);
            RX72M_RX72N_RX66N_func101(change_endian_long(0x5be6634eu), change_endian_long(0x9d484aaau), change_endian_long(0x7d8b085bu), change_endian_long(0x9a2bc581u));
        }
        else
        {
            TSIP.REG_C4H.WORD = 0x000c2b0du;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_KeyIndex[0];
            TSIP.REG_100H.WORD = InData_KeyIndex[1];
            TSIP.REG_100H.WORD = InData_KeyIndex[2];
            TSIP.REG_100H.WORD = InData_KeyIndex[3];
            TSIP.REG_C4H.WORD = 0x000c0b0cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x2a46c04bu);
            RX72M_RX72N_RX66N_func101(change_endian_long(0x039ae7f7u), change_endian_long(0x32ee84edu), change_endian_long(0x60f9753bu), change_endian_long(0xca524511u));
        }
        RX72M_RX72N_RX66N_func100(change_endian_long(0xa014b310u), change_endian_long(0x64c5e74eu), change_endian_long(0x7da63ad4u), change_endian_long(0x31f5a1c2u));
        TSIP.REG_C4H.WORD = 0x02f8073du;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[4];
        TSIP.REG_100H.WORD = InData_KeyIndex[5];
        TSIP.REG_100H.WORD = InData_KeyIndex[6];
        TSIP.REG_100H.WORD = InData_KeyIndex[7];
        TSIP.REG_C4H.WORD = 0x000007bdu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[8];
        TSIP.REG_100H.WORD = InData_KeyIndex[9];
        TSIP.REG_100H.WORD = InData_KeyIndex[10];
        TSIP.REG_100H.WORD = InData_KeyIndex[11];
        TSIP.REG_C4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00002213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000368u;
        TSIP.REG_E0H.WORD = 0x80040080u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_IV[0];
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_IV[1];
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_IV[2];
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_IV[3];
        RX72M_RX72N_RX66N_func101(change_endian_long(0xe63aa21au), change_endian_long(0xababf59au), change_endian_long(0x6df4b2b5u), change_endian_long(0x62798780u));
    }
    else
    {
        TSIP.REG_ECH.WORD = 0x38008900u;
        TSIP.REG_ECH.WORD = 0x00000001u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0x1cf1b07eu), change_endian_long(0x74fa214bu), change_endian_long(0x3d9dd87au), change_endian_long(0xc469f6ebu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_104H.WORD = 0x00000368u;
            TSIP.REG_E0H.WORD = 0x80040000u;
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
            TSIP.REG_104H.WORD = 0x00000068u;
            TSIP.REG_E0H.WORD = 0x80010080u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_DataType[0];
            TSIP.REG_ECH.WORD = 0x00000bdeu;
            TSIP.REG_ECH.WORD = 0x000037e0u;
            TSIP.REG_ECH.WORD = 0x00008fe0u;
            TSIP.REG_ECH.WORD = 0xff000000u;
            TSIP.REG_ECH.WORD = 0x30203080u;
            TSIP.REG_ECH.WORD = 0x00070020u;
            TSIP.REG_ECH.WORD = 0x38008be0u;
            TSIP.REG_ECH.WORD = 0x12000000u;
            TSIP.REG_ECH.WORD = 0x1000d3c0u;
            TSIP.REG_ECH.WORD = 0x00000060u;
            TSIP.REG_ECH.WORD = 0x38008be0u;
            TSIP.REG_ECH.WORD = 0x92000000u;
            TSIP.REG_ECH.WORD = 0x1000d3c0u;
            TSIP.REG_ECH.WORD = 0x00000080u;
            TSIP.REG_ECH.WORD = 0x3800d816u;
            TSIP.REG_ECH.WORD = 0x2000d3c1u;
            TSIP.REG_ECH.WORD = 0x000037e0u;
            TSIP.REG_ECH.WORD = 0x00008fe0u;
            TSIP.REG_ECH.WORD = 0x0000fe00u;
            TSIP.REG_ECH.WORD = 0x38008be0u;
            TSIP.REG_ECH.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x1000d3c2u;
            TSIP.REG_ECH.WORD = 0x38008bc0u;
            TSIP.REG_ECH.WORD = 0x00000007u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            TSIP.REG_C4H.WORD = 0x00082b8du;
            TSIP.REG_E0H.WORD = 0x81040000u;
            TSIP.REG_00H.WORD = 0x00002813u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x675fc283u), change_endian_long(0x82b70788u), change_endian_long(0xf0056594u), change_endian_long(0x0d1f791cu));
        }
        else
        {
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
            TSIP.REG_ECH.WORD = 0x3820a900u;
            TSIP.REG_ECH.WORD = 0x00000003u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            RX72M_RX72N_RX66N_func100(change_endian_long(0x320d55adu), change_endian_long(0x1f7bf1ecu), change_endian_long(0xa5ef32c1u), change_endian_long(0xd0727940u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_C4H.WORD = 0x00082b8du;
                TSIP.REG_E0H.WORD = 0x81040280u;
                TSIP.REG_00H.WORD = 0x00002813u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x0000b7e0u;
                TSIP.REG_ECH.WORD = 0x7a52b394u;
                RX72M_RX72N_RX66N_func101(change_endian_long(0xa3f45ec8u), change_endian_long(0x30e15351u), change_endian_long(0x0885f3bfu), change_endian_long(0x590b6b69u));
            }
            else
            {
                RX72M_RX72N_RX66N_func100(change_endian_long(0x13a9af25u), change_endian_long(0x18fb8513u), change_endian_long(0xf1677f33u), change_endian_long(0x7aabb148u));
                TSIP.REG_104H.WORD = 0x00000068u;
                TSIP.REG_E0H.WORD = 0x80010020u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_DataType[0];
                TSIP.REG_ECH.WORD = 0x3020a820u;
                TSIP.REG_ECH.WORD = 0x00000003u;
                TSIP.REG_ECH.WORD = 0x00060020u;
                TSIP.REG_ECH.WORD = 0x0000b420u;
                TSIP.REG_ECH.WORD = 0x00000003u;
                TSIP.REG_ECH.WORD = 0x00000080u;
                TSIP.REG_ECH.WORD = 0x00007c01u;
                TSIP.REG_1CH.WORD = 0x00600000u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    TSIP.REG_104H.WORD = 0x00000068u;
                    TSIP.REG_E0H.WORD = 0x80010000u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = InData_Cmd[0];
                    RX72M_RX72N_RX66N_func401();
                    RX72M_RX72N_RX66N_func072();
                    TSIP.REG_ECH.WORD = 0x0000b7e0u;
                    TSIP.REG_ECH.WORD = 0x8026ee7fu;
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x83c7b388u), change_endian_long(0x209e0bf3u), change_endian_long(0x18e738acu), change_endian_long(0xb76b6470u));
                }
                else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    TSIP.REG_C4H.WORD = 0x00082b8du;
                    TSIP.REG_E0H.WORD = 0x81040280u;
                    TSIP.REG_00H.WORD = 0x00002813u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_ECH.WORD = 0x0000b7e0u;
                    TSIP.REG_ECH.WORD = 0x98bae316u;
                    RX72M_RX72N_RX66N_func101(change_endian_long(0xa54245b4u), change_endian_long(0x36eecad5u), change_endian_long(0xdbeb5cd6u), change_endian_long(0xede253b2u));
                }
                else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    TSIP.REG_104H.WORD = 0x00000068u;
                    TSIP.REG_E0H.WORD = 0x80010000u;
                    /* WAIT_LOOP */
                    while (1u != TSIP.REG_104H.BIT.B31)
                    {
                        /* waiting */
                    }
                    TSIP.REG_100H.WORD = InData_Cmd[0];
                    RX72M_RX72N_RX66N_func401();
                    RX72M_RX72N_RX66N_func075();
                    TSIP.REG_ECH.WORD = 0x0000b7e0u;
                    TSIP.REG_ECH.WORD = 0x091b4e6eu;
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x5613a6ebu), change_endian_long(0x575c8205u), change_endian_long(0x18da68a5u), change_endian_long(0xf171ec30u));
                }
                else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
                {
                    TSIP.REG_C4H.WORD = 0x00082b8du;
                    TSIP.REG_E0H.WORD = 0x81040280u;
                    TSIP.REG_00H.WORD = 0x00002813u;
                    /* WAIT_LOOP */
                    while (0u != TSIP.REG_00H.BIT.B25)
                    {
                        /* waiting */
                    }
                    TSIP.REG_1CH.WORD = 0x00001800u;
                    TSIP.REG_ECH.WORD = 0x0000b7e0u;
                    TSIP.REG_ECH.WORD = 0x512272a1u;
                    RX72M_RX72N_RX66N_func101(change_endian_long(0x6fbdb14du), change_endian_long(0x99b59a75u), change_endian_long(0xc6ffcec6u), change_endian_long(0xa4c2ed76u));
                }
                TSIP.REG_104H.WORD = 0x00000168u;
                TSIP.REG_E0H.WORD = 0x800203a0u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_SeqNum[0];
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_SeqNum[1];
                RX72M_RX72N_RX66N_func101(change_endian_long(0x98cc4b8du), change_endian_long(0x3dd44d3du), change_endian_long(0x3eba4c21u), change_endian_long(0xa2db623bu));
            }
            TSIP.REG_C4H.WORD = 0x40080b8cu;
            TSIP.REG_E0H.WORD = 0x810103e0u;
            TSIP.REG_00H.WORD = 0x00002807u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x62078ba1u), change_endian_long(0x415ef585u), change_endian_long(0x59260e5du), change_endian_long(0x832cb331u));
        }
        RX72M_RX72N_RX66N_func100(change_endian_long(0x6c412460u), change_endian_long(0xe4f2af02u), change_endian_long(0xf3b5015du), change_endian_long(0xcbec52adu));
        TSIP.REG_C4H.WORD = 0x42f8073du;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[4];
        TSIP.REG_100H.WORD = InData_KeyIndex[5];
        TSIP.REG_100H.WORD = InData_KeyIndex[6];
        TSIP.REG_100H.WORD = InData_KeyIndex[7];
        TSIP.REG_C4H.WORD = 0x40f007bdu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_KeyIndex[8];
        TSIP.REG_100H.WORD = InData_KeyIndex[9];
        TSIP.REG_100H.WORD = InData_KeyIndex[10];
        TSIP.REG_100H.WORD = InData_KeyIndex[11];
        TSIP.REG_E0H.WORD = 0x80040080u;
        TSIP.REG_00H.WORD = 0x00008213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_C4H.WORD = 0x400007bdu;
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
        RX72M_RX72N_RX66N_func101(change_endian_long(0x376cdf56u), change_endian_long(0x96310d6cu), change_endian_long(0xb6989c99u), change_endian_long(0xdebda733u));
    }
    RX72M_RX72N_RX66N_func100(change_endian_long(0x2236cb9au), change_endian_long(0x1ef03c04u), change_endian_long(0x486fe06fu), change_endian_long(0xf28186e5u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0xfe597f42u), change_endian_long(0x074827c9u), change_endian_long(0x4b7fd954u), change_endian_long(0x50008a1du));
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
        TSIP.REG_ECH.WORD = 0x3020a900u;
        TSIP.REG_ECH.WORD = 0x00000004u;
        TSIP.REG_ECH.WORD = 0x00060020u;
        TSIP.REG_ECH.WORD = 0x000008bdu;
        TSIP.REG_ECH.WORD = 0x000008deu;
        TSIP.REG_ECH.WORD = 0x0000b4e0u;
        TSIP.REG_ECH.WORD = 0x00000001u;
        TSIP.REG_ECH.WORD = 0x00000080u;
        TSIP.REG_7CH.WORD = 0x00000001u;
        TSIP.REG_7CH.WORD = 0x00000041u;
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x00000a84u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_00H.WORD = 0x00005213u;
        TSIP.REG_74H.WORD = 0x00001000u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_74H.WORD = 0x00000004u;
        TSIP.REG_104H.WORD = 0x00000354u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_74H.WORD = 0x00000002u;
        RX72M_RX72N_RX66N_func101(change_endian_long(0xe8797e8au), change_endian_long(0x30177dfcu), change_endian_long(0xbc4b52fcu), change_endian_long(0x3e7fc9e8u));
        return TSIP_SUCCESS;
    }
}
/**********************************************************************************************************************
 End of function ./input_dir/RX72M/RX72M_p32i_r5.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_AES_128_GCM_DECRYPT == 1 */
