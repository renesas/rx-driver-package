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
* @details       RX66T initialization2
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
    RX66T_RX72T_function001(0x086f4af6u, 0x026003ddu, 0x0b8fab81u, 0xc1aee098u);
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
        RX66T_RX72T_function002(0x6c3eed23u, 0x37ef6462u, 0x3dca4269u, 0xd93140b7u);
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
        RX66T_RX72T_function002(0x4205b32du, 0x445fe35du, 0x3415d3f5u, 0xeb60fa4au);
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
        RX66T_RX72T_function002(0x383a58efu, 0x9b33edadu, 0xfab7c72bu, 0xbb600b7bu);
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
    RX66T_RX72T_function001(0xb4258456u, 0x1b9e7f15u, 0x3abf3975u, 0x2287c683u);
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX66T_RX72T_function003(0x6887c36eu, 0x0e5bb533u, 0xe1489401u, 0xa7002826u);
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
        RX66T_RX72T_function001(0xb0639365u, 0x9225fc09u, 0x53c292cau, 0x12f15929u);
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
        RX66T_RX72T_function001(0xcbb4540au, 0x57d8ff36u, 0x0ebe4a3fu, 0x5f73dff6u);
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
        RX66T_RX72T_function001(0x64597ef2u, 0x33fbcc8eu, 0x8a0e1783u, 0xd997693bu);
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
        RX66T_RX72T_function001(0xb1c1d56bu, 0xf5dbbc38u, 0xfd1876c2u, 0xa9a8c198u);
        TSIP.REG_A4H.WORD = 0x000d0805u;
        TSIP.REG_00H.WORD = 0x00001113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX66T_RX72T_function001(0xcf9d197eu, 0x92d99677u, 0x5f0687eau, 0xd4fc9c60u);
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
        RX66T_RX72T_function001(0xbf485705u, 0x54125236u, 0x3cac7d9eu, 0x8f1234a0u);
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
        RX66T_RX72T_function003(0xf58254a0u, 0xc1ae2d52u, 0x220a8fe5u, 0xeca2fcc8u);
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
 End of function ./input_dir/RX66T/RX66T_p02.prc
 *********************************************************************************************************************/
TSIP_SEC_DEFAULT
