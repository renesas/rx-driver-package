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
* Function Name: R_TSIP_TlsGenerateVerifyDataSub
*******************************************************************************************************************/ /**
* @details       RX65NHU TLS VerifyData Generation
* @param[in]     InData_Sel_VerifyData
* @param[in]     InData_MasterSecret
* @param[in]     InData_HandShakeHash
* @param[out]    OutData_VerifyData
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @note          None
*/
e_tsip_err_t R_TSIP_TlsGenerateVerifyDataSub(uint32_t *InData_Sel_VerifyData, uint32_t *InData_MasterSecret, uint32_t *InData_HandShakeHash, uint32_t *OutData_VerifyData)
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
    TSIP.REG_84H.WORD = 0x0000e601u;
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
    TSIP.REG_E0H.WORD = 0x800103a0u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_Sel_VerifyData[0];
    TSIP.REG_ECH.WORD = 0x00000bdeu;
    TSIP.REG_ECH.WORD = 0x000037fdu;
    TSIP.REG_ECH.WORD = 0x00008fe0u;
    TSIP.REG_ECH.WORD = 0xfffffffeu;
    TSIP.REG_ECH.WORD = 0x38008be0u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x1000d3c0u;
    TSIP.REG_ECH.WORD = 0x38008bc0u;
    TSIP.REG_ECH.WORD = 0x00000001u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX65NHU_func100(change_endian_long(0xf4fdc7f0u), change_endian_long(0xbdce5015u), change_endian_long(0xc673c3e1u), change_endian_long(0x5ac18f3du));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0x9c9de19cu), change_endian_long(0x4063c58au), change_endian_long(0xef9dabe6u), change_endian_long(0x82cfd8a5u));
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
        TSIP.REG_104H.WORD = 0x00000362u;
        TSIP.REG_C4H.WORD = 0x00082b8du;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_MasterSecret[0];
        TSIP.REG_100H.WORD = InData_MasterSecret[1];
        TSIP.REG_100H.WORD = InData_MasterSecret[2];
        TSIP.REG_100H.WORD = InData_MasterSecret[3];
        TSIP.REG_C4H.WORD = 0x400c0b0cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x6226a005u);
        TSIP.REG_104H.WORD = 0x00000b62u;
        TSIP.REG_D0H.WORD = 0x00000200u;
        TSIP.REG_C4H.WORD = 0x02f087bfu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_MasterSecret[4];
        TSIP.REG_100H.WORD = InData_MasterSecret[5];
        TSIP.REG_100H.WORD = InData_MasterSecret[6];
        TSIP.REG_100H.WORD = InData_MasterSecret[7];
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_MasterSecret[8];
        TSIP.REG_100H.WORD = InData_MasterSecret[9];
        TSIP.REG_100H.WORD = InData_MasterSecret[10];
        TSIP.REG_100H.WORD = InData_MasterSecret[11];
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_MasterSecret[12];
        TSIP.REG_100H.WORD = InData_MasterSecret[13];
        TSIP.REG_100H.WORD = InData_MasterSecret[14];
        TSIP.REG_100H.WORD = InData_MasterSecret[15];
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
        TSIP.REG_100H.WORD = InData_MasterSecret[16];
        TSIP.REG_100H.WORD = InData_MasterSecret[17];
        TSIP.REG_100H.WORD = InData_MasterSecret[18];
        TSIP.REG_100H.WORD = InData_MasterSecret[19];
        TSIP.REG_C4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00002213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX65NHU_func100(change_endian_long(0xec948434u), change_endian_long(0x4c42a1ceu), change_endian_long(0x0f783c07u), change_endian_long(0xc705b6b7u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func102(change_endian_long(0x48301585u), change_endian_long(0xd7e37d2fu), change_endian_long(0x7cac42b3u), change_endian_long(0xb65c9c96u));
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
            TSIP.REG_ECH.WORD = 0x000037fdu;
            TSIP.REG_ECH.WORD = 0x00008fe0u;
            TSIP.REG_ECH.WORD = 0x0000000fu;
            TSIP.REG_ECH.WORD = 0x38008be0u;
            TSIP.REG_ECH.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00260000u;
            RX65NHU_func100(change_endian_long(0x26b28a53u), change_endian_long(0x9cd5debfu), change_endian_long(0x42a853f0u), change_endian_long(0xeaa0767au));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                TSIP.REG_104H.WORD = 0x00000352u;
                TSIP.REG_C4H.WORD = 0x00000885u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = change_endian_long(0x636c6965u);
                TSIP.REG_100H.WORD = change_endian_long(0x6e742066u);
                TSIP.REG_100H.WORD = change_endian_long(0x696e6973u);
                TSIP.REG_100H.WORD = change_endian_long(0x68656400u);
                RX65NHU_func101(change_endian_long(0x0cc164ccu), change_endian_long(0xec1784d0u), change_endian_long(0x1ebc6b29u), change_endian_long(0xa237579du));
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
                TSIP.REG_100H.WORD = change_endian_long(0x73657276u);
                TSIP.REG_100H.WORD = change_endian_long(0x65722066u);
                TSIP.REG_100H.WORD = change_endian_long(0x696e6973u);
                TSIP.REG_100H.WORD = change_endian_long(0x68656400u);
                RX65NHU_func101(change_endian_long(0xcfd7ce6eu), change_endian_long(0xe95794afu), change_endian_long(0x03450fdbu), change_endian_long(0x6e09f2b7u));
            }
            TSIP.REG_ECH.WORD = 0x0000b420u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            TSIP.REG_E0H.WORD = 0x80830001u;
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
            TSIP.REG_100H.WORD = change_endian_long(0x00686564u);
            TSIP.REG_104H.WORD = 0x00000762u;
            TSIP.REG_D0H.WORD = 0x00000100u;
            TSIP.REG_C4H.WORD = 0x00008887u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_HandShakeHash[0];
            TSIP.REG_100H.WORD = InData_HandShakeHash[1];
            TSIP.REG_100H.WORD = InData_HandShakeHash[2];
            TSIP.REG_100H.WORD = InData_HandShakeHash[3];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_HandShakeHash[4];
            TSIP.REG_100H.WORD = InData_HandShakeHash[5];
            TSIP.REG_100H.WORD = InData_HandShakeHash[6];
            TSIP.REG_100H.WORD = InData_HandShakeHash[7];
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
                TSIP.REG_ECH.WORD = 0x01886c01u;
                TSIP.REG_ECH.WORD = 0x00003c03u;
                TSIP.REG_ECH.WORD = 0x00003401u;
                TSIP.REG_ECH.WORD = 0x0000a460u;
                TSIP.REG_ECH.WORD = 0x00000004u;
            }
            TSIP.REG_ECH.WORD = 0x00086c00u;
            TSIP.REG_ECH.WORD = 0x00003c03u;
            TSIP.REG_ECH.WORD = 0x0000b420u;
            TSIP.REG_ECH.WORD = 0x00000010u;
            TSIP.REG_ECH.WORD = 0x0000b560u;
            TSIP.REG_ECH.WORD = 0x00000020u;
            TSIP.REG_ECH.WORD = 0x0000098cu;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000002fu;
            RX65NHU_func101(change_endian_long(0x5e7ae8e4u), change_endian_long(0xb1c0aff0u), change_endian_long(0xc5187d06u), change_endian_long(0xd1ef767au));
            R_TSIP_TlsGenerateSubSub();

            TSIP.REG_ECH.WORD = 0x00002c20u;
            TSIP.REG_ECH.WORD = 0x0000096bu;
            TSIP.REG_ECH.WORD = 0x0000098cu;
            TSIP.REG_ECH.WORD = 0x0000b5c0u;
            TSIP.REG_ECH.WORD = 0x0000004fu;
            RX65NHU_func101(change_endian_long(0x0eee7519u), change_endian_long(0x60b34740u), change_endian_long(0x850c58dau), change_endian_long(0x5e5c58bfu));
            R_TSIP_TlsGenerateSubSub();

            RX65NHU_func100(change_endian_long(0x1de8ca68u), change_endian_long(0x7e565f8cu), change_endian_long(0x265a2ff6u), change_endian_long(0xc4bbbd07u));
            TSIP.REG_C4H.WORD = 0x00000885u;
            TSIP.REG_ECH.WORD = 0x00000821u;
            TSIP.REG_E0H.WORD = 0x81830001u;
            TSIP.REG_00H.WORD = 0x0000280fu;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_00H.WORD = 0x00002007u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_04H.WORD = 0x0000020fu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_VerifyData[0] = TSIP.REG_100H.WORD;
            OutData_VerifyData[1] = TSIP.REG_100H.WORD;
            OutData_VerifyData[2] = TSIP.REG_100H.WORD;
            TSIP.REG_00H.WORD = 0x00000207u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX65NHU_func102(change_endian_long(0xfbc1139fu), change_endian_long(0xcf074016u), change_endian_long(0xd6e5840au), change_endian_long(0xdf414893u));
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
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_pe6.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_TLS == 1 */
