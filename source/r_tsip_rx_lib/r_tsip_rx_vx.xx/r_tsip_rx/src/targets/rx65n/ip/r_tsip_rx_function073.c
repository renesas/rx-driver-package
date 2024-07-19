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
* Function Name: RX65NHU_func073
*******************************************************************************************************************/ /**
* @details       RX65NHU func073
* @param[in]     ARG1
* @note          None
*/
void RX65NHU_func073(uint32_t *ARG1)
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
    TSIP.REG_ECH.WORD = 0x38000e10u;
    TSIP.REG_E0H.WORD = 0x00000080u;
    TSIP.REG_1CH.WORD = 0x00260000u;
    RX65NHU_func100(change_endian_long(0xca861312u), change_endian_long(0x3ef458b5u), change_endian_long(0x33481247u), change_endian_long(0x322c3d17u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_28H.WORD = 0x008f0001u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        TSIP.REG_104H.WORD = 0x00000557u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00201074u);
        TSIP.REG_100H.WORD = change_endian_long(0x6c733133u);
        TSIP.REG_100H.WORD = change_endian_long(0x20726573u);
        TSIP.REG_100H.WORD = change_endian_long(0x756d7074u);
        TSIP.REG_100H.WORD = change_endian_long(0x696f6e00u);
        TSIP.REG_100H.WORD = change_endian_long(0x01800000u);
        TSIP.REG_00H.WORD = 0x00003027u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000057u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x000002A8u);
        RX65NHU_func070();
        RX65NHU_func101(change_endian_long(0x59c5bb2du), change_endian_long(0x35c5430cu), change_endian_long(0x8cf8d6e7u), change_endian_long(0xd7ada65du));
    }
    else
    {
        RX65NHU_func100(change_endian_long(0xb0b7dadbu), change_endian_long(0x9788af4eu), change_endian_long(0x586a4c48u), change_endian_long(0x0195c70au));
        TSIP.REG_ECH.WORD = 0x00003630u;
        TSIP.REG_ECH.WORD = 0x0000a620u;
        TSIP.REG_ECH.WORD = 0x00000003u;
        TSIP.REG_ECH.WORD = 0x00026a31u;
        TSIP.REG_ECH.WORD = 0x00003220u;
        TSIP.REG_E0H.WORD = 0x81010220u;
        TSIP.REG_04H.WORD = 0x00000606u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        S_RAM[0] = change_endian_long(TSIP.REG_100H.WORD);
        TSIP.REG_7CH.WORD = 0x00000011u;
        TSIP.REG_104H.WORD = 0x00000754u;
        TSIP.REG_74H.WORD = 0x00000004u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x6a09e667u);
        TSIP.REG_100H.WORD = change_endian_long(0xbb67ae85u);
        TSIP.REG_100H.WORD = change_endian_long(0x3c6ef372u);
        TSIP.REG_100H.WORD = change_endian_long(0xa54ff53au);
        TSIP.REG_100H.WORD = change_endian_long(0x510e527fu);
        TSIP.REG_100H.WORD = change_endian_long(0x9b05688cu);
        TSIP.REG_100H.WORD = change_endian_long(0x1f83d9abu);
        TSIP.REG_100H.WORD = change_endian_long(0x5be0cd19u);
        TSIP.REG_74H.WORD = 0x00000002u;
        RX65NHU_func001();
        TSIP.REG_104H.WORD = 0x00000354u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00201074u);
        TSIP.REG_100H.WORD = change_endian_long(0x6c733133u);
        TSIP.REG_100H.WORD = change_endian_long(0x20726573u);
        TSIP.REG_100H.WORD = change_endian_long(0x756d7074u);
        TSIP.REG_ECH.WORD = 0x0000b620u;
        TSIP.REG_ECH.WORD = 0x696f6e00u;
        TSIP.REG_ECH.WORD = 0x00001230u;
        TSIP.REG_E0H.WORD = 0x81010220u;
        TSIP.REG_00H.WORD = 0x00005807u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x000000b4u;
        for (iLoop = 0; iLoop < S_RAM[0]; iLoop = iLoop + 1)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = ARG1[iLoop + 0];
        }
        TSIP.REG_104H.WORD = 0x00000000u;
        TSIP.REG_104H.WORD = 0x00000068u;
        TSIP.REG_E0H.WORD = 0x80010100u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = ARG1[iLoop + 0];
        TSIP.REG_ECH.WORD = 0x0000b520u;
        TSIP.REG_ECH.WORD = 0x01800000u;
        TSIP.REG_ECH.WORD = 0x00003630u;
        TSIP.REG_ECH.WORD = 0x00008e20u;
        TSIP.REG_ECH.WORD = 0x00000003u;
        TSIP.REG_ECH.WORD = 0x00036e31u;
        TSIP.REG_ECH.WORD = 0x0000b640u;
        TSIP.REG_ECH.WORD = 0x00000020u;
        TSIP.REG_ECH.WORD = 0x00002a51u;
        TSIP.REG_ECH.WORD = 0x00004412u;
        TSIP.REG_ECH.WORD = 0x00007508u;
        TSIP.REG_ECH.WORD = 0x01807909u;
        TSIP.REG_ECH.WORD = 0x00007929u;
        TSIP.REG_E0H.WORD = 0x81020100u;
        TSIP.REG_00H.WORD = 0x0000580bu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_F8H.WORD = 0x00000040u;
        TSIP.REG_ECH.WORD = 0x00000908u;
        TSIP.REG_ECH.WORD = 0x00003530u;
        TSIP.REG_ECH.WORD = 0x00036d29u;
        TSIP.REG_ECH.WORD = 0x0000a520u;
        TSIP.REG_ECH.WORD = 0x000002A8u;
        TSIP.REG_E0H.WORD = 0x81020100u;
        TSIP.REG_00H.WORD = 0x0000580bu;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_74H.BIT.B18)
        {
            /* waiting */
        }
        TSIP.REG_74H.WORD = 0x00000000u;
        TSIP.REG_1CH.WORD = 0x00001600u;
        TSIP.REG_28H.WORD = 0x00870001u;
        RX65NHU_func061();
        RX65NHU_func101(change_endian_long(0x360f5ac0u), change_endian_long(0x874df688u), change_endian_long(0x52674394u), change_endian_long(0x7f41da5bu));
    }
    TSIP.REG_ECH.WORD = 0x00007c1du;
    TSIP.REG_1CH.WORD = 0x00602000u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_func073_r2.prc
 *********************************************************************************************************************/
