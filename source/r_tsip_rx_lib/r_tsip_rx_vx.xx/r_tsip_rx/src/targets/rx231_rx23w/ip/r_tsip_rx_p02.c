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
TSIP_SEC_P_SECURE_BOOT

/***********************************************************************************************************************
* Function Name: R_TSIP_SelfCheck2Sub
*******************************************************************************************************************/ /**
* @details       RX231HU initialization2
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_RETRY
* @note          None
*/
e_tsip_err_t R_TSIP_SelfCheck2Sub(void)
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
    if (0x0u != (TSIP.REG_1BCH.WORD & 0x1fu))
    {
        return TSIP_ERR_RESOURCE_CONFLICT;
    }
    TSIP.REG_84H.WORD = 0x00000201u;
    TSIP.REG_108H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x3000a800u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00000003u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00010020u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00000800u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    RX231_RX23W_function001(0x05e6e8cau, 0xdcd6caa4u, 0x7c515e03u, 0xe429028au);
    TSIP.REG_ECH.WORD = 0x00007c00u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_1CH.WORD = 0x00600000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        /* WAIT_LOOP */
        while (0u != TSIP.REG_64H.BIT.B11)
        {
            /* waiting */
        }
        TSIP.REG_64H.WORD = 0x00000008u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_64H.BIT.B11)
        {
            /* waiting */
        }
        TSIP.REG_64H.WORD = 0x0000005Cu;
        RX231_RX23W_function002(0xd10f04c0u, 0x84081358u, 0x749c48d1u, 0x335dbde3u);
    }
    else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        /* WAIT_LOOP */
        while (0u != TSIP.REG_64H.BIT.B11)
        {
            /* waiting */
        }
        TSIP.REG_64H.WORD = 0x00000008u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_64H.BIT.B11)
        {
            /* waiting */
        }
        TSIP.REG_64H.WORD = 0x0000005eu;
        RX231_RX23W_function002(0xbded0750u, 0xa4478467u, 0x1e9ee576u, 0x6a9b3edcu);
    }
    else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        /* WAIT_LOOP */
        while (0u != TSIP.REG_64H.BIT.B11)
        {
            /* waiting */
        }
        TSIP.REG_64H.WORD = 0x00000008u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_64H.BIT.B11)
        {
            /* waiting */
        }
        TSIP.REG_64H.WORD = 0x0000005du;
        RX231_RX23W_function002(0xa2b7e869u, 0x4035d63au, 0x621bd188u, 0x8fdbda4fu);
    }
    TSIP.REG_A4H.WORD = 0x00040804u;
    TSIP.REG_E0H.WORD = 0x81010000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_00H.WORD = 0x00001807u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    R_TSIP_SelfCheck2SubSub();

    /* WAIT_LOOP */
    while (0u != TSIP.REG_64H.BIT.B11)
    {
        /* waiting */
    }
    TSIP.REG_104H.WORD = 0x00000051u;
    TSIP.REG_A4H.WORD = 0x00000684u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = 0x00000000u;
    TSIP.REG_E0H.WORD = 0x80010000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    TSIP.REG_00H.WORD = 0x00008107u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_00H.WORD = 0x0000010fu;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_ECH.WORD = 0x00002c00u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    RX231_RX23W_function001(0xe3846fa9u, 0x13c660bau, 0x5f9ed620u, 0xaba3759bu);
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX231_RX23W_function003(0xd22633b0u, 0x8a6fbe32u, 0x2c4bf85bu, 0x4904a6a7u);
        TSIP.REG_1BCH.WORD = 0x00000040u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_18H.BIT.B12)
        {
            /* waiting */
        }
        return TSIP_ERR_RETRY;
    }
    else
    {
        /* WAIT_LOOP */
        while (0u != TSIP.REG_64H.BIT.B11)
        {
            /* waiting */
        }
        TSIP.REG_64H.WORD = 0x0000001cu;
        TSIP.REG_104H.WORD = 0x00000051u;
        TSIP.REG_A4H.WORD = 0x00200684u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x80040000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x00008113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_A4H.WORD = 0xc0200c9du;
        TSIP.REG_00H.WORD = 0x00001413u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_64H.BIT.B11)
        {
            /* waiting */
        }
        TSIP.REG_64H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x80040080u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x00008113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX231_RX23W_function001(0x65d95bc4u, 0xa4905b25u, 0x7b9754f8u, 0x446d717fu);
        TSIP.REG_104H.WORD = 0x00000051u;
        TSIP.REG_A4H.WORD = 0x000b0804u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = 0x00000000u;
        TSIP.REG_104H.WORD = 0x00000351u;
        TSIP.REG_A4H.WORD = 0x00010805u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = 0x00000000u;
        TSIP.REG_100H.WORD = 0x00000000u;
        TSIP.REG_100H.WORD = 0x00000000u;
        TSIP.REG_100H.WORD = 0x00000001u;
        RX231_RX23W_function001(0xb63c663fu, 0x8d5cb59eu, 0x3898c664u, 0xc6e7fa67u);
        TSIP.REG_B0H.WORD = 0x00000100u;
        TSIP.REG_A4H.WORD = 0x0020b6bfu;
        TSIP.REG_E0H.WORD = 0x81080000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x00001823u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_A4H.WORD = 0x000b0805u;
        TSIP.REG_00H.WORD = 0x00001113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_A4H.WORD = 0x00010805u;
        TSIP.REG_00H.WORD = 0x00001113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX231_RX23W_function001(0x150e400eu, 0x19c16e2fu, 0xa3f652bcu, 0x3b580967u);
        TSIP.REG_104H.WORD = 0x00000251u;
        TSIP.REG_A4H.WORD = 0x0020363cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = 0x00000000u;
        TSIP.REG_A4H.WORD = 0x002036bcu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = 0x00000000u;
        TSIP.REG_A4H.WORD = 0x002036bcu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = 0x00000000u;
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
        TSIP.REG_100H.WORD = 0x00000000u;
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
        TSIP.REG_100H.WORD = 0x00000000u;
        RX231_RX23W_function001(0x6ce3c132u, 0x88ea5de7u, 0x767721b9u, 0xc7abbee8u);
        TSIP.REG_A4H.WORD = 0x000d0805u;
        TSIP.REG_00H.WORD = 0x00001113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX231_RX23W_function001(0xdcb047f7u, 0x3ae333d5u, 0x89efa260u, 0x03c272f0u);
        TSIP.REG_104H.WORD = 0x00000251u;
        TSIP.REG_A4H.WORD = 0x0020363cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = 0x00000000u;
        TSIP.REG_A4H.WORD = 0x002036bcu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = 0x00000000u;
        TSIP.REG_A4H.WORD = 0x002036bcu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = 0x00000000u;
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
        TSIP.REG_100H.WORD = 0x00000000u;
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
        TSIP.REG_100H.WORD = 0x00000000u;
        RX231_RX23W_function001(0x17c401d2u, 0xb2792d68u, 0xc62a57d8u, 0x13fcd15eu);
        TSIP.REG_04H.WORD = 0x00000113u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_RAM[4+0 + 0] = change_endian_long(TSIP.REG_100H.WORD);
        S_RAM[4+0 + 1] = change_endian_long(TSIP.REG_100H.WORD);
        S_RAM[4+0 + 2] = change_endian_long(TSIP.REG_100H.WORD);
        S_RAM[4+0 + 3] = change_endian_long(TSIP.REG_100H.WORD);
        TSIP.REG_13CH.WORD = 0x00000202u;
        RX231_RX23W_function003(0xac50cdb8u, 0xacbc0948u, 0xf3d7db80u, 0x8178193du);
        TSIP.REG_1BCH.WORD = 0x00000040u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_18H.BIT.B12)
        {
            /* waiting */
        }
        return TSIP_SUCCESS;
    }
}
/**********************************************************************************************************************
 End of function ./input_dir/RX231HU/RX231HU_p02.prc
 *********************************************************************************************************************/
TSIP_SEC_DEFAULT
