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

#if TSIP_TLS == 1
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
* Function Name: R_TSIP_TlsGenerateMasterSecretSub
*******************************************************************************************************************/ /**
* @details       RX72M TLS MasterSecret Generation
* @param[in]     InData_Sel_CipherSuite
* @param[in]     InData_PreMasterSecret
* @param[in]     InData_ClientRandom
* @param[in]     InData_ServerRandom
* @param[out]    OutData_MasterSecret
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @note          None
*/
e_tsip_err_t R_TSIP_TlsGenerateMasterSecretSub(uint32_t *InData_Sel_CipherSuite, uint32_t *InData_PreMasterSecret, uint32_t *InData_ClientRandom, uint32_t *InData_ServerRandom, uint32_t *OutData_MasterSecret)
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
    TSIP.REG_84H.WORD = 0x0000e401u;
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
    TSIP.REG_E0H.WORD = 0x80010000u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Sel_CipherSuite[0];
    TSIP.REG_ECH.WORD = 0x3420a800u;
    TSIP.REG_ECH.WORD = 0x00000008u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX72M_RX72N_RX66N_func100(change_endian_long(0x28156fadu), change_endian_long(0x18e3eebbu), change_endian_long(0xc1460519u), change_endian_long(0xb6159a3du));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX72M_RX72N_RX66N_func102(change_endian_long(0xfb1e6378u), change_endian_long(0xfb67f298u), change_endian_long(0xf7b2863bu), change_endian_long(0x79ad0129u));
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
        RX72M_RX72N_RX66N_func100(change_endian_long(0x872882ceu), change_endian_long(0x13d6a785u), change_endian_long(0x21f94df1u), change_endian_long(0x098e7e44u));
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
            TSIP.REG_100H.WORD = change_endian_long(0x0954569cu);
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
            RX72M_RX72N_RX66N_func101(change_endian_long(0xa5d45894u), change_endian_long(0x6b4cfe00u), change_endian_long(0x7d343f54u), change_endian_long(0xfb619556u));
        }
        else
        {
            TSIP.REG_C4H.WORD = 0x400c0b0cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x24c1817fu);
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
            RX72M_RX72N_RX66N_func101(change_endian_long(0xf82e86e5u), change_endian_long(0xdb9d585cu), change_endian_long(0x997d32ecu), change_endian_long(0x9aeab0e2u));
        }
        TSIP.REG_C4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00002213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX72M_RX72N_RX66N_func100(change_endian_long(0xe76430e9u), change_endian_long(0xbbb6379du), change_endian_long(0x341ae6dcu), change_endian_long(0x383b7f37u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX72M_RX72N_RX66N_func102(change_endian_long(0x1dc66420u), change_endian_long(0x05d8cbd5u), change_endian_long(0xf6b56d95u), change_endian_long(0xc841a640u));
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
            TSIP.REG_100H.WORD = change_endian_long(0x6d617374u);
            TSIP.REG_100H.WORD = change_endian_long(0x65722073u);
            TSIP.REG_100H.WORD = change_endian_long(0x65637265u);
            TSIP.REG_100H.WORD = change_endian_long(0x74000000u);
            TSIP.REG_ECH.WORD = 0x0000b460u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            TSIP.REG_E0H.WORD = 0x80830003u;
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
            TSIP.REG_104H.WORD = 0x00000068u;
            TSIP.REG_E0H.WORD = 0x80010000u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000074u);
            TSIP.REG_104H.WORD = 0x00000762u;
            TSIP.REG_D0H.WORD = 0x00000100u;
            TSIP.REG_C4H.WORD = 0x00008887u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_ClientRandom[0];
            TSIP.REG_100H.WORD = InData_ClientRandom[1];
            TSIP.REG_100H.WORD = InData_ClientRandom[2];
            TSIP.REG_100H.WORD = InData_ClientRandom[3];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_ClientRandom[4];
            TSIP.REG_100H.WORD = InData_ClientRandom[5];
            TSIP.REG_100H.WORD = InData_ClientRandom[6];
            TSIP.REG_100H.WORD = InData_ClientRandom[7];
            TSIP.REG_ECH.WORD = 0x0000b460u;
            TSIP.REG_ECH.WORD = 0x0000002cu;
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
                TSIP.REG_ECH.WORD = 0x01986c01u;
                TSIP.REG_ECH.WORD = 0x00003c03u;
                TSIP.REG_ECH.WORD = 0x00003401u;
                TSIP.REG_ECH.WORD = 0x0000a460u;
                TSIP.REG_ECH.WORD = 0x00000004u;
            }
            TSIP.REG_104H.WORD = 0x00000762u;
            TSIP.REG_D0H.WORD = 0x00000100u;
            TSIP.REG_C4H.WORD = 0x00008887u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_ServerRandom[0];
            TSIP.REG_100H.WORD = InData_ServerRandom[1];
            TSIP.REG_100H.WORD = InData_ServerRandom[2];
            TSIP.REG_100H.WORD = InData_ServerRandom[3];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_ServerRandom[4];
            TSIP.REG_100H.WORD = InData_ServerRandom[5];
            TSIP.REG_100H.WORD = InData_ServerRandom[6];
            TSIP.REG_100H.WORD = InData_ServerRandom[7];
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
                TSIP.REG_ECH.WORD = 0x01986c01u;
                TSIP.REG_ECH.WORD = 0x00003c03u;
                TSIP.REG_ECH.WORD = 0x00003401u;
                TSIP.REG_ECH.WORD = 0x0000a460u;
                TSIP.REG_ECH.WORD = 0x00000004u;
            }
            TSIP.REG_ECH.WORD = 0x00186c00u;
            TSIP.REG_ECH.WORD = 0x00003c03u;
            TSIP.REG_ECH.WORD = 0x0000b420u;
            TSIP.REG_ECH.WORD = 0x00000001u;
            TSIP.REG_ECH.WORD = 0x0000b560u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            TSIP.REG_ECH.WORD = 0x0000098cu;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000004du;
            RX72M_RX72N_RX66N_func101(change_endian_long(0xb0fde3aeu), change_endian_long(0xf644afc0u), change_endian_long(0x56b6d9ebu), change_endian_long(0x4feeac19u));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x048f3bd1u), change_endian_long(0xa54c55b6u), change_endian_long(0x04790cd5u), change_endian_long(0xf32b4e24u));
            R_TSIP_TlsGenerateSubSub();

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
            RX72M_RX72N_RX66N_func101(change_endian_long(0x83064accu), change_endian_long(0x53aadb08u), change_endian_long(0x4686ed74u), change_endian_long(0x5174cd85u));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000b580u;
            TSIP.REG_ECH.WORD = 0x000000a0u;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000006du;
            RX72M_RX72N_RX66N_func101(change_endian_long(0x52914083u), change_endian_long(0x3fdd0c5eu), change_endian_long(0xf67a3bbcu), change_endian_long(0x7a81dabau));
            R_TSIP_TlsGenerateSubSub();

            RX72M_RX72N_RX66N_func100(change_endian_long(0x4fe9bbd4u), change_endian_long(0xbb9b1ae5u), change_endian_long(0x8fea8a14u), change_endian_long(0xeba4bfe4u));
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
            RX72M_RX72N_RX66N_func100(change_endian_long(0x97bf479eu), change_endian_long(0xefad7ca1u), change_endian_long(0x3b00d9d4u), change_endian_long(0x4757da22u));
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
            TSIP.REG_100H.WORD = change_endian_long(0x6226a005u);
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
            TSIP.REG_04H.WORD = 0x00000233u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_MasterSecret[4] = TSIP.REG_100H.WORD;
            OutData_MasterSecret[5] = TSIP.REG_100H.WORD;
            OutData_MasterSecret[6] = TSIP.REG_100H.WORD;
            OutData_MasterSecret[7] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_MasterSecret[8] = TSIP.REG_100H.WORD;
            OutData_MasterSecret[9] = TSIP.REG_100H.WORD;
            OutData_MasterSecret[10] = TSIP.REG_100H.WORD;
            OutData_MasterSecret[11] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_MasterSecret[12] = TSIP.REG_100H.WORD;
            OutData_MasterSecret[13] = TSIP.REG_100H.WORD;
            OutData_MasterSecret[14] = TSIP.REG_100H.WORD;
            OutData_MasterSecret[15] = TSIP.REG_100H.WORD;
            RX72M_RX72N_RX66N_func100(change_endian_long(0x1d23e5edu), change_endian_long(0xfaf0c562u), change_endian_long(0x5d3f3357u), change_endian_long(0x80a6e4b7u));
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
            OutData_MasterSecret[16] = TSIP.REG_100H.WORD;
            OutData_MasterSecret[17] = TSIP.REG_100H.WORD;
            OutData_MasterSecret[18] = TSIP.REG_100H.WORD;
            OutData_MasterSecret[19] = TSIP.REG_100H.WORD;
            RX72M_RX72N_RX66N_func100(change_endian_long(0x8534f9afu), change_endian_long(0x3ab3c7deu), change_endian_long(0xfc8bec9du), change_endian_long(0x2c150cbfu));
            TSIP.REG_104H.WORD = 0x00000052u;
            TSIP.REG_C4H.WORD = 0x01000c84u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_04H.WORD = 0x00000213u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_MasterSecret[0] = TSIP.REG_100H.WORD;
            OutData_MasterSecret[1] = TSIP.REG_100H.WORD;
            OutData_MasterSecret[2] = TSIP.REG_100H.WORD;
            OutData_MasterSecret[3] = TSIP.REG_100H.WORD;
            RX72M_RX72N_RX66N_func102(change_endian_long(0x1df4bd9eu), change_endian_long(0x58ccd834u), change_endian_long(0xcb20bbd9u), change_endian_long(0x09692cacu));
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
 End of function ./input_dir/RX72M/RX72M_pe4.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_TLS == 1 */
