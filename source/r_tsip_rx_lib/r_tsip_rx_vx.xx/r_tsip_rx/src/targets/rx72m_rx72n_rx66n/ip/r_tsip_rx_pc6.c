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
* Function Name: R_TSIP_TlsSVGenerateExtendedMasterSecretSub
*******************************************************************************************************************/ /**
* @details       RX72M TLS Extended MasterSecret Generation for Server
* @param[in]     InData_Sel_CipherSuite
* @param[in]     InData_PreMasterSecret
* @param[in]     InData_MsgHash
* @param[out]    OutData_ExMasterSecret
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @note          None
*/
e_tsip_err_t R_TSIP_TlsSVGenerateExtendedMasterSecretSub(uint32_t *InData_Sel_CipherSuite, uint32_t *InData_PreMasterSecret, uint32_t *InData_MsgHash, uint32_t *OutData_ExMasterSecret)
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
    TSIP.REG_84H.WORD = 0x0000fc01u;
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
    TSIP.REG_100H.WORD = InData_Sel_CipherSuite[0];
    TSIP.REG_ECH.WORD = 0x3420a800u;
    TSIP.REG_ECH.WORD = 0x00000002u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_ECH.WORD = 0x3420a800u;
    TSIP.REG_ECH.WORD = 0x00000008u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0xbb6f0b22u), change_endian_long(0xd2778850u), change_endian_long(0xba081128u), change_endian_long(0xfd6738b7u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0x4d5963fcu), change_endian_long(0x8f9f1dacu), change_endian_long(0x698b4b28u), change_endian_long(0xbb49bb6fu));
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
        TSIP.REG_104H.WORD = 0x00000362u;
        TSIP.REG_C4H.WORD = 0x00082b8du;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_PreMasterSecret[0];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[1];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[2];
        TSIP.REG_100H.WORD = InData_PreMasterSecret[3];
        TSIP.REG_ECH.WORD = 0x3420a800u;
        TSIP.REG_ECH.WORD = 0x00000004u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0x9706c1b5u), change_endian_long(0xa752a300u), change_endian_long(0xa57ee5e4u), change_endian_long(0x181462d7u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_C4H.WORD = 0x400c0b0cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x7090aa94u);
            TSIP.REG_104H.WORD = 0x00000b62u;
            TSIP.REG_D0H.WORD = 0x00000200u;
            TSIP.REG_C4H.WORD = 0x02f087bfu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_PreMasterSecret[4];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[5];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[6];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[7];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_PreMasterSecret[8];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[9];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[10];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[11];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_PreMasterSecret[12];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[13];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[14];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[15];
            TSIP.REG_ECH.WORD = 0x0000b400u;
            TSIP.REG_ECH.WORD = 0x00000070u;
            TSIP.REG_E0H.WORD = 0x808c0000u;
            TSIP.REG_00H.WORD = 0x00008233u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_104H.WORD = 0x00000362u;
            TSIP.REG_C4H.WORD = 0x000007bdu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_PreMasterSecret[16];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[17];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[18];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[19];
            RX72M_RX72N_RX66N_func101(change_endian_long(0x9f62d572u), change_endian_long(0x3a268927u), change_endian_long(0x950061b5u), change_endian_long(0x94901710u));
        }
        else
        {
            TSIP.REG_C4H.WORD = 0x400c0b0cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x39fa4edcu);
            TSIP.REG_104H.WORD = 0x00000762u;
            TSIP.REG_D0H.WORD = 0x00000100u;
            TSIP.REG_C4H.WORD = 0x02f087bfu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_PreMasterSecret[4];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[5];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[6];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[7];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_PreMasterSecret[8];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[9];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[10];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[11];
            TSIP.REG_ECH.WORD = 0x0000b400u;
            TSIP.REG_ECH.WORD = 0x00000070u;
            TSIP.REG_E0H.WORD = 0x808c0000u;
            TSIP.REG_00H.WORD = 0x00008223u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_00H.WORD = 0x00008013u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_104H.WORD = 0x00000362u;
            TSIP.REG_C4H.WORD = 0x000007bdu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_PreMasterSecret[12];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[13];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[14];
            TSIP.REG_100H.WORD = InData_PreMasterSecret[15];
            RX72M_RX72N_RX66N_func101(change_endian_long(0x5608e2bau), change_endian_long(0x58ca682du), change_endian_long(0xe696cb8du), change_endian_long(0x2c759348u));
        }
        TSIP.REG_C4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00002213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0x1d70033du), change_endian_long(0x76acbda6u), change_endian_long(0x35eb7b99u), change_endian_long(0x1755077bu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func102(change_endian_long(0x572c7002u), change_endian_long(0x41872d37u), change_endian_long(0xc7be02beu), change_endian_long(0x6f37bfd1u));
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
            TSIP.REG_104H.WORD = 0x00000352u;
            TSIP.REG_C4H.WORD = 0x00000885u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x65787465u);
            TSIP.REG_100H.WORD = change_endian_long(0x6e646564u);
            TSIP.REG_100H.WORD = change_endian_long(0x206d6173u);
            TSIP.REG_100H.WORD = change_endian_long(0x74657220u);
            TSIP.REG_ECH.WORD = 0x0000b460u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            TSIP.REG_E0H.WORD = 0x80840003u;
            TSIP.REG_00H.WORD = 0x00008213u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x0000b460u;
            TSIP.REG_ECH.WORD = 0x00000030u;
            TSIP.REG_104H.WORD = 0x00000058u;
            TSIP.REG_E0H.WORD = 0x80810003u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x73656372u);
            TSIP.REG_ECH.WORD = 0x0000b400u;
            TSIP.REG_ECH.WORD = 0x00006574u;
            TSIP.REG_104H.WORD = 0x00000762u;
            TSIP.REG_D0H.WORD = 0x00000100u;
            TSIP.REG_C4H.WORD = 0x00008887u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_MsgHash[0];
            TSIP.REG_100H.WORD = InData_MsgHash[1];
            TSIP.REG_100H.WORD = InData_MsgHash[2];
            TSIP.REG_100H.WORD = InData_MsgHash[3];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_MsgHash[4];
            TSIP.REG_100H.WORD = InData_MsgHash[5];
            TSIP.REG_100H.WORD = InData_MsgHash[6];
            TSIP.REG_100H.WORD = InData_MsgHash[7];
            TSIP.REG_ECH.WORD = 0x0000b460u;
            TSIP.REG_ECH.WORD = 0x00000034u;
            for (iLoop = 0; iLoop < 8; iLoop = iLoop + 1)
            {
                TSIP.REG_E0H.WORD = 0x80010020u;
                TSIP.REG_00H.WORD = 0x00008207u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x01906c01u;
                TSIP.REG_ECH.WORD = 0x00003c03u;
                TSIP.REG_ECH.WORD = 0x00003401u;
                TSIP.REG_ECH.WORD = 0x0000a460u;
                TSIP.REG_ECH.WORD = 0x00000004u;
            }
            TSIP.REG_ECH.WORD = 0x00106c00u;
            TSIP.REG_ECH.WORD = 0x00003c03u;
            TSIP.REG_ECH.WORD = 0x0000b420u;
            TSIP.REG_ECH.WORD = 0x00000041u;
            TSIP.REG_ECH.WORD = 0x0000b560u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            TSIP.REG_ECH.WORD = 0x0000098cu;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x00000036u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0xecb7c04du), change_endian_long(0xd06012b2u), change_endian_long(0xfc5bb217u), change_endian_long(0x84d4c21fu));
            RX72M_RX72N_RX66N_func032();
            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x00000056u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x81a3b3b8u), change_endian_long(0xdff3b775u), change_endian_long(0x867b3729u), change_endian_long(0x33230f3cu));
            RX72M_RX72N_RX66N_func032();
            TSIP.REG_E0H.WORD = 0x8188000cu;
            TSIP.REG_00H.WORD = 0x00003823u;
            TSIP.REG_2CH.WORD = 0x0000001bu;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000098cu;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x5141ed69u), change_endian_long(0xea352222u), change_endian_long(0x219f08c6u), change_endian_long(0x8d55520fu));
            RX72M_RX72N_RX66N_func032();
            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x00000056u;
            RX72M_RX72N_RX66N_func101(change_endian_long(0xf3de1b8bu), change_endian_long(0x863e093cu), change_endian_long(0x35a3b7d6u), change_endian_long(0x6bbc15e4u));
            RX72M_RX72N_RX66N_func032();
            RX72M_RX72N_RX66N_func100(change_endian_long(0xf336a811u), change_endian_long(0xc379a6d2u), change_endian_long(0x247217c2u), change_endian_long(0xa41356d4u));
            TSIP.REG_E0H.WORD = 0x8184000cu;
            TSIP.REG_00H.WORD = 0x00003813u;
            TSIP.REG_2CH.WORD = 0x0000009bu;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_00H.WORD = 0x00003013u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x00000821u;
            TSIP.REG_E0H.WORD = 0x808c0001u;
            TSIP.REG_00H.WORD = 0x00008323u;
            TSIP.REG_2CH.WORD = 0x0000002bu;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_00H.WORD = 0x00008313u;
            TSIP.REG_2CH.WORD = 0x000000abu;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_00H.WORD = 0x00000313u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX72M_RX72N_RX66N_func103();
            RX72M_RX72N_RX66N_func100(change_endian_long(0xe9405f40u), change_endian_long(0x6f21d655u), change_endian_long(0xc5ebb212u), change_endian_long(0x221c5e7cu));
            TSIP.REG_104H.WORD = 0x00000052u;
            TSIP.REG_C4H.WORD = 0x0108258cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_C4H.WORD = 0x400c0b0cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x9c5b7bb2u);
            TSIP.REG_D0H.WORD = 0x00000200u;
            TSIP.REG_C4H.WORD = 0x02e087bfu;
            TSIP.REG_E0H.WORD = 0x818c0001u;
            TSIP.REG_00H.WORD = 0x00002833u;
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
            OutData_ExMasterSecret[4] = TSIP.REG_100H.WORD;
            OutData_ExMasterSecret[5] = TSIP.REG_100H.WORD;
            OutData_ExMasterSecret[6] = TSIP.REG_100H.WORD;
            OutData_ExMasterSecret[7] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_ExMasterSecret[8] = TSIP.REG_100H.WORD;
            OutData_ExMasterSecret[9] = TSIP.REG_100H.WORD;
            OutData_ExMasterSecret[10] = TSIP.REG_100H.WORD;
            OutData_ExMasterSecret[11] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_ExMasterSecret[12] = TSIP.REG_100H.WORD;
            OutData_ExMasterSecret[13] = TSIP.REG_100H.WORD;
            OutData_ExMasterSecret[14] = TSIP.REG_100H.WORD;
            OutData_ExMasterSecret[15] = TSIP.REG_100H.WORD;
            RX72M_RX72N_RX66N_func100(change_endian_long(0x57de0261u), change_endian_long(0x186eba1au), change_endian_long(0xb5b1a321u), change_endian_long(0xd02f292fu));
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
            OutData_ExMasterSecret[16] = TSIP.REG_100H.WORD;
            OutData_ExMasterSecret[17] = TSIP.REG_100H.WORD;
            OutData_ExMasterSecret[18] = TSIP.REG_100H.WORD;
            OutData_ExMasterSecret[19] = TSIP.REG_100H.WORD;
            RX72M_RX72N_RX66N_func100(change_endian_long(0x63cb5a4bu), change_endian_long(0xcf413641u), change_endian_long(0xa9169b16u), change_endian_long(0xc4c57925u));
            TSIP.REG_104H.WORD = 0x00000052u;
            TSIP.REG_C4H.WORD = 0x01000c84u;
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
            OutData_ExMasterSecret[0] = TSIP.REG_100H.WORD;
            OutData_ExMasterSecret[1] = TSIP.REG_100H.WORD;
            OutData_ExMasterSecret[2] = TSIP.REG_100H.WORD;
            OutData_ExMasterSecret[3] = TSIP.REG_100H.WORD;
            RX72M_RX72N_RX66N_func102(change_endian_long(0x6a8576f0u), change_endian_long(0x3ce51ed7u), change_endian_long(0x4b24d63au), change_endian_long(0xfb7d24a6u));
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
/**********************************************************************************************************************
 End of function ./input_dir/RX72M/RX72M_pc6.prc
 *********************************************************************************************************************/
