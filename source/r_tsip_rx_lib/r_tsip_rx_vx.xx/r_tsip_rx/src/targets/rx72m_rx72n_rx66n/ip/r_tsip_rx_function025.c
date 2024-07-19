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
* Function Name: RX72M_RX72N_RX66N_func025
*******************************************************************************************************************/ /**
* @details       
* @param[in]     ARG1
* @note          None
*/
void RX72M_RX72N_RX66N_func025(uint32_t ARG1)
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
    TSIP.REG_ECH.WORD = 0x30003380u;
    TSIP.REG_ECH.WORD = 0x00070020u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x1f8473b6u;
    TSIP.REG_ECH.WORD = 0x00030040u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x32ab9622u;
    TSIP.REG_ECH.WORD = 0x00050040u;
    TSIP.REG_ECH.WORD = 0x0000b7c0u;
    TSIP.REG_ECH.WORD = 0x5d38e816u;
    TSIP.REG_ECH.WORD = 0x00000080u;
    TSIP.REG_28H.WORD = 0x00870001u;
    TSIP.REG_C4H.WORD = 0x000c3b0cu;
    TSIP.REG_E0H.WORD = 0x810103c0u;
    TSIP.REG_00H.WORD = 0x00002807u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_104H.WORD = 0x00003f62u;
    TSIP.REG_D0H.WORD = 0x00000f00u;
    TSIP.REG_C4H.WORD = 0x02f087bfu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_FLASH[ARG1+120 + 0];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+120 + 1];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+120 + 2];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+120 + 3];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_FLASH[ARG1+124 + 0];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+124 + 1];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+124 + 2];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+124 + 3];
    TSIP.REG_00H.WORD = 0x00003223u;
    TSIP.REG_2CH.WORD = 0x00000090u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_34H.WORD = 0x00000104u;
    TSIP.REG_24H.WORD = 0x800011c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_FLASH[ARG1+128 + 0];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+128 + 1];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+128 + 2];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+128 + 3];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_FLASH[ARG1+132 + 0];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+132 + 1];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+132 + 2];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+132 + 3];
    TSIP.REG_00H.WORD = 0x00003223u;
    TSIP.REG_2CH.WORD = 0x00000018u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_34H.WORD = 0x00000108u;
    TSIP.REG_24H.WORD = 0x800012c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_FLASH[ARG1+136 + 0];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+136 + 1];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+136 + 2];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+136 + 3];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_FLASH[ARG1+140 + 0];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+140 + 1];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+140 + 2];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+140 + 3];
    TSIP.REG_00H.WORD = 0x00003223u;
    TSIP.REG_2CH.WORD = 0x00000091u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_34H.WORD = 0x00000304u;
    TSIP.REG_24H.WORD = 0x800015c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_FLASH[ARG1+144 + 0];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+144 + 1];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+144 + 2];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+144 + 3];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_FLASH[ARG1+148 + 0];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+148 + 1];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+148 + 2];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+148 + 3];
    TSIP.REG_00H.WORD = 0x00003223u;
    TSIP.REG_2CH.WORD = 0x00000019u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_34H.WORD = 0x00000308u;
    TSIP.REG_24H.WORD = 0x800016c0u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_24H.BIT.B21)
    {
        /* waiting */
    }
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_FLASH[ARG1+152 + 0];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+152 + 1];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+152 + 2];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+152 + 3];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_FLASH[ARG1+156 + 0];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+156 + 1];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+156 + 2];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+156 + 3];
    TSIP.REG_00H.WORD = 0x00003223u;
    TSIP.REG_2CH.WORD = 0x00000092u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_FLASH[ARG1+160 + 0];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+160 + 1];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+160 + 2];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+160 + 3];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_FLASH[ARG1+164 + 0];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+164 + 1];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+164 + 2];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+164 + 3];
    TSIP.REG_00H.WORD = 0x00003223u;
    TSIP.REG_2CH.WORD = 0x00000093u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_FLASH[ARG1+168 + 0];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+168 + 1];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+168 + 2];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+168 + 3];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_FLASH[ARG1+172 + 0];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+172 + 1];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+172 + 2];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+172 + 3];
    TSIP.REG_00H.WORD = 0x00003223u;
    TSIP.REG_2CH.WORD = 0x0000009au;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_FLASH[ARG1+176 + 0];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+176 + 1];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+176 + 2];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+176 + 3];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = S_FLASH[ARG1+180 + 0];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+180 + 1];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+180 + 2];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+180 + 3];
    TSIP.REG_00H.WORD = 0x00003223u;
    TSIP.REG_2CH.WORD = 0x0000009bu;
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
    TSIP.REG_100H.WORD = S_FLASH[ARG1+184 + 0];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+184 + 1];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+184 + 2];
    TSIP.REG_100H.WORD = S_FLASH[ARG1+184 + 3];
    TSIP.REG_C4H.WORD = 0x00800c45u;
    TSIP.REG_00H.WORD = 0x00002213u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
}
/**********************************************************************************************************************
 End of function ./input_dir/RX72M/RX72M_func025.prc
 *********************************************************************************************************************/
