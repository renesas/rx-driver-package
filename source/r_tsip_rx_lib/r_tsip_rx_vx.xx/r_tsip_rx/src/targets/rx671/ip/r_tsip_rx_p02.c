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
* @details       RX671 TSIP Initialization 2
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
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x00093b8cu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = 0x26c7ba56u;
    TSIP.REG_104H.WORD = 0x00000052u;
    TSIP.REG_C4H.WORD = 0x00070804u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = 0x00000000u;
    TSIP.REG_ECH.WORD = 0x3000a820u;
    TSIP.REG_ECH.WORD = 0x00000003u;
    TSIP.REG_ECH.WORD = 0x00010020u;
    TSIP.REG_ECH.WORD = 0x00000821u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    RX671_func100(0xfe8a0163u, 0x77ad6b00u, 0x5056654du, 0x79ce30edu);
    TSIP.REG_ECH.WORD = 0x00007c01u;
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
        TSIP.REG_64H.WORD = 0x00002008u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_64H.BIT.B11)
        {
            /* waiting */
        }
        TSIP.REG_64H.WORD = 0x0000901Cu;
        RX671_func101(0x93c9770fu, 0x45a61a15u, 0xcc2f7f6bu, 0x34afd96au);
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
        TSIP.REG_64H.WORD = 0x00002008u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_64H.BIT.B11)
        {
            /* waiting */
        }
        TSIP.REG_64H.WORD = 0x0000901eu;
        RX671_func101(0x96d6bb5bu, 0xcbbff3e1u, 0xac9e50edu, 0xf130db92u);
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
        TSIP.REG_64H.WORD = 0x00002008u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_64H.BIT.B11)
        {
            /* waiting */
        }
        TSIP.REG_64H.WORD = 0x0000901du;
        RX671_func101(0xe201bd50u, 0x99269162u, 0x06345de1u, 0x3e03835eu);
    }
    R_TSIP_SelfCheck2SubSub();

    TSIP.REG_ECH.WORD = 0x00002c20u;
    TSIP.REG_ECH.WORD = 0x38008840u;
    TSIP.REG_ECH.WORD = 0x00002000u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    TSIP.REG_1CH.WORD = 0x00402000u;
    TSIP.REG_ECH.WORD = 0x38008860u;
    TSIP.REG_ECH.WORD = 0x00000000u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00A60000u;
    RX671_func100(0xd80f1a7fu, 0x20d9f61fu, 0x798ff25du, 0xd8514939u);
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX671_func102(0x2185baabu, 0x95b76530u, 0xd8edcd91u, 0x2be013e1u);
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
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x01000cc4u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x80040000u;
        TSIP.REG_00H.WORD = 0x00008213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_C4H.WORD = 0x41001eddu;
        TSIP.REG_00H.WORD = 0x00002413u;
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
        TSIP.REG_00H.WORD = 0x00008213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX671_func100(0x0021082au, 0x51139f15u, 0x0cf56e86u, 0x4784892du);
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x000b0804u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = 0x00000000u;
        TSIP.REG_104H.WORD = 0x00000352u;
        TSIP.REG_C4H.WORD = 0x00070805u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = 0x00000000u;
        TSIP.REG_100H.WORD = 0x00000000u;
        TSIP.REG_100H.WORD = 0x00000000u;
        TSIP.REG_100H.WORD = 0x00000001u;
        RX671_func100(0xfd57e0acu, 0x1d687f6au, 0x2a9fec49u, 0xa9cfa634u);
        TSIP.REG_D0H.WORD = 0x00000100u;
        TSIP.REG_C4H.WORD = 0x0100b7f7u;
        TSIP.REG_E0H.WORD = 0x81080000u;
        TSIP.REG_00H.WORD = 0x00002823u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_C4H.WORD = 0x000b0805u;
        TSIP.REG_00H.WORD = 0x00002213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_C4H.WORD = 0x00070805u;
        TSIP.REG_00H.WORD = 0x00002213u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX671_func100(0xb8a3c8e6u, 0xd0bc4ae6u, 0x22a5f9a5u, 0x27003819u);
        RX671_func103();
        RX671_func100(0x9c828566u, 0xe3850461u, 0xd6aa7310u, 0xc833f85fu);
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x010d0c04u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = 0x00000000u;
        RX671_func100(0xf983cbbcu, 0x24129c91u, 0x9a6d7551u, 0x77bd768bu);
        RX671_func103();
        RX671_func100(0xeb14c3c4u, 0xc8f92abbu, 0xcaf9cf9eu, 0xf45d2a79u);
        TSIP.REG_104H.WORD = 0x00000052u;
        TSIP.REG_C4H.WORD = 0x01000c84u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = 0x00000000u;
        TSIP.REG_04H.WORD = 0x00000213u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_RAM[16] = change_endian_long(TSIP.REG_100H.WORD);
        S_RAM[17] = change_endian_long(TSIP.REG_100H.WORD);
        S_RAM[18] = change_endian_long(TSIP.REG_100H.WORD);
        S_RAM[19] = change_endian_long(TSIP.REG_100H.WORD);
        TSIP.REG_13CH.WORD = 0x00000202u;
        RX671_func102(0xeea7feddu, 0xd4759f71u, 0xc670ffb5u, 0x526d23d6u);
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
 End of function ./input_dir/RX671/RX671_p02.prc
 *********************************************************************************************************************/
TSIP_SEC_DEFAULT
