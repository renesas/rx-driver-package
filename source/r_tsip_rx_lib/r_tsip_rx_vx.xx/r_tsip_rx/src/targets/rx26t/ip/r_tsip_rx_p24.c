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

#if TSIP_FIRMWARE_UPDATE == 1
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
* Function Name: R_TSIP_GenerateFirmwareMacSub
*******************************************************************************************************************/ /**
* @details       RX26T F/W update
* @param[in]     InData_KeyIndex
* @param[in]     InData_SessionKey
* @param[in]     InData_UpProgram
* @param[in]     MAX_CNT
* @param[in]     InData_IV
* @param[out]    OutData_Program
* @param[in,out] tsip_firmware_generate_mac_resume_handle
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @note          None
*/
e_tsip_err_t R_TSIP_GenerateFirmwareMacSub(uint32_t *InData_KeyIndex, uint32_t *InData_SessionKey, uint32_t *InData_UpProgram, uint32_t *InData_IV, uint32_t *OutData_Program, uint32_t MAX_CNT, tsip_firmware_generate_mac_resume_handle_t *tsip_firmware_generate_mac_resume_handle)
{
    uint32_t counter = 0;
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

    if((tsip_firmware_generate_mac_resume_handle->use_resume_flag == true)
        && (tsip_firmware_generate_mac_resume_handle->iLoop != 0))
    {
        iLoop = tsip_firmware_generate_mac_resume_handle->iLoop;
        counter = tsip_firmware_generate_mac_resume_handle->counter;
        goto resume_point;
    }
    else
    {
        iLoop = 0;
        counter = 0;
    }

    if (0x0u != (TSIP.REG_1BCH.WORD & 0x1fu))
    {
        return TSIP_ERR_RESOURCE_CONFLICT;
    }
    TSIP.REG_84H.WORD = 0x00002401u;
    TSIP.REG_108H.WORD = 0x00000000u;
    RX26T_function001(change_endian_long(0xe5db0312u), change_endian_long(0x239c3265u), change_endian_long(0x74e2932eu), change_endian_long(0x14d4a042u));
    TSIP.REG_A4H.WORD = 0x600c3a0du;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[0];
    TSIP.REG_100H.WORD = InData_KeyIndex[1];
    TSIP.REG_100H.WORD = InData_KeyIndex[2];
    TSIP.REG_100H.WORD = InData_KeyIndex[3];
    TSIP.REG_A4H.WORD = 0x400c0a0cu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0xef138998u);
    TSIP.REG_A4H.WORD = 0x42fa063du;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[4];
    TSIP.REG_100H.WORD = InData_KeyIndex[5];
    TSIP.REG_100H.WORD = InData_KeyIndex[6];
    TSIP.REG_100H.WORD = InData_KeyIndex[7];
    TSIP.REG_A4H.WORD = 0x400006bdu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_KeyIndex[8];
    TSIP.REG_100H.WORD = InData_KeyIndex[9];
    TSIP.REG_100H.WORD = InData_KeyIndex[10];
    TSIP.REG_100H.WORD = InData_KeyIndex[11];
    TSIP.REG_A4H.WORD = 0x00900c45u;
    TSIP.REG_00H.WORD = 0x00001113u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX26T_function001(change_endian_long(0x953878ecu), change_endian_long(0x1e996c17u), change_endian_long(0x02d83c52u), change_endian_long(0x7fe87f50u));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX26T_function003(change_endian_long(0x83dc258fu), change_endian_long(0x08a7fe7fu), change_endian_long(0x260f1eaeu), change_endian_long(0xe31a0b09u));
        TSIP.REG_1BCH.WORD = 0x00000040u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_18H.BIT.B12)
        {
            /* waiting */
        }
        return TSIP_ERR_KEY_SET;
    }
    else
    {
        RX26T_function001(change_endian_long(0xde181a20u), change_endian_long(0x20bcc8d9u), change_endian_long(0x955f2aa1u), change_endian_long(0x5f6b5c80u));
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x0008680du;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_SessionKey[0];
        TSIP.REG_100H.WORD = InData_SessionKey[1];
        TSIP.REG_100H.WORD = InData_SessionKey[2];
        TSIP.REG_100H.WORD = InData_SessionKey[3];
        RX26T_function001(change_endian_long(0x2a4e8f87u), change_endian_long(0xd7eb7c5du), change_endian_long(0x2bf78907u), change_endian_long(0xf50693b3u));
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x0009680du;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_SessionKey[4];
        TSIP.REG_100H.WORD = InData_SessionKey[5];
        TSIP.REG_100H.WORD = InData_SessionKey[6];
        TSIP.REG_100H.WORD = InData_SessionKey[7];
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x00040805u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_IV[0];
        TSIP.REG_100H.WORD = InData_IV[1];
        TSIP.REG_100H.WORD = InData_IV[2];
        TSIP.REG_100H.WORD = InData_IV[3];
        TSIP.REG_104H.WORD = 0x00000051u;
        TSIP.REG_A4H.WORD = 0x00050804u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_104H.WORD = 0x00000051u;
        TSIP.REG_A4H.WORD = 0x00070804u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_A4H.WORD = 0x600c3a0cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x2fc18e32u);
resume_point:
        for (; iLoop < MAX_CNT-8; iLoop = iLoop + 4)
        {
            if ( 0u != iLoop )
            {
                TSIP_GEN_MAC_CB_FUNC(TSIP_FW_CB_REQ_BUFF_CNT, iLoop, &counter, InData_UpProgram, OutData_Program, MAX_CNT);
                TSIP_GEN_MAC_CB_FUNC(TSIP_FW_CB_REQ_PRG_WT, iLoop, &counter, InData_UpProgram, OutData_Program, MAX_CNT);
            }
            TSIP_GEN_MAC_CB_FUNC(TSIP_FW_CB_REQ_PRG_RD, iLoop, &counter, InData_UpProgram, OutData_Program, MAX_CNT);
            RX26T_function001(change_endian_long(0x41606109u), change_endian_long(0xca83a0e9u), change_endian_long(0x9c2d6881u), change_endian_long(0xa06b635fu));
            TSIP.REG_104H.WORD = 0x00000361u;
            TSIP.REG_A4H.WORD = 0x00d049adu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_UpProgram[(counter) + 0u];
            TSIP.REG_100H.WORD = InData_UpProgram[(counter) + 1u];
            TSIP.REG_100H.WORD = InData_UpProgram[(counter) + 2u];
            TSIP.REG_100H.WORD = InData_UpProgram[(counter) + 3u];
            TSIP.REG_A4H.WORD = 0x41f00885u;
            TSIP.REG_00H.WORD = 0x00001113u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_04H.WORD = 0x00000113u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_Program[(counter) + 0u] = TSIP.REG_100H.WORD;
            OutData_Program[(counter) + 1u] = TSIP.REG_100H.WORD;
            OutData_Program[(counter) + 2u] = TSIP.REG_100H.WORD;
            OutData_Program[(counter) + 3u] = TSIP.REG_100H.WORD;
            RX26T_function002(change_endian_long(0x0f5e20efu), change_endian_long(0x17e9927fu), change_endian_long(0x797190d1u), change_endian_long(0xeebeed77u));
            if(tsip_firmware_generate_mac_resume_handle->use_resume_flag == true)
            {
                tsip_firmware_generate_mac_resume_handle->iLoop = iLoop + 4;
                tsip_firmware_generate_mac_resume_handle->counter = counter;
                return TSIP_RESUME_FIRMWARE_GENERATE_MAC;
            }
            else
            {
                R_BSP_NOP();
            }
        }
        RX26T_function001(change_endian_long(0xdd1032f3u), change_endian_long(0x37896837u), change_endian_long(0x7de79033u), change_endian_long(0x4a09d354u));
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x00d049adu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP_GEN_MAC_CB_FUNC(TSIP_FW_CB_REQ_BUFF_CNT, iLoop, &counter, InData_UpProgram, OutData_Program, MAX_CNT);
        TSIP_GEN_MAC_CB_FUNC(TSIP_FW_CB_REQ_PRG_RD, iLoop, &counter, InData_UpProgram, OutData_Program, MAX_CNT);
        TSIP.REG_100H.WORD = InData_UpProgram[(counter) + 0u];
        TSIP.REG_100H.WORD = InData_UpProgram[(counter) + 1u];
        TSIP.REG_100H.WORD = InData_UpProgram[(counter) + 2u];
        TSIP.REG_100H.WORD = InData_UpProgram[(counter) + 3u];
        TSIP.REG_E0H.WORD = 0x80040000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x00008113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_E0H.WORD = 0x81040000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_04H.WORD = 0x00000613u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_Program[(counter) + 0u + 0u] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_Program[(counter) + 1u + 0u] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_Program[(counter) + 2u + 0u] = TSIP.REG_100H.WORD;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_Program[(counter) + 3u + 0u] = TSIP.REG_100H.WORD;
        TSIP_GEN_MAC_CB_FUNC(TSIP_FW_CB_REQ_PRG_WT_LAST_BLK, iLoop, &counter, InData_UpProgram, OutData_Program, MAX_CNT);
        TSIP.REG_104H.WORD = 0x00000061u;
        TSIP.REG_A4H.WORD = 0x4000088cu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_A4H.WORD = 0x414008c5u;
        TSIP.REG_00H.WORD = 0x00001113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_A4H.WORD = 0x01070645u;
        TSIP.REG_00H.WORD = 0x00001113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_A4H.WORD = 0x41070c5du;
        TSIP.REG_E0H.WORD = 0x81040000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_00H.WORD = 0x00001813u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_104H.WORD = 0x00000361u;
        TSIP.REG_A4H.WORD = 0x000049adu;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP_GEN_MAC_CB_FUNC(TSIP_FW_CB_REQ_GET_UPDATE_PRG_CHKSUM, iLoop, &counter, InData_UpProgram, OutData_Program, MAX_CNT);
        TSIP.REG_100H.WORD = InData_UpProgram[0];
        TSIP.REG_100H.WORD = InData_UpProgram[1];
        TSIP.REG_100H.WORD = InData_UpProgram[2];
        TSIP.REG_100H.WORD = InData_UpProgram[3];
        TSIP.REG_A4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00001113u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX26T_function001(change_endian_long(0xfbf6df50u), change_endian_long(0x3f7804a1u), change_endian_long(0x3fe00ac6u), change_endian_long(0xab01e19bu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX26T_function003(change_endian_long(0xbf13b7deu), change_endian_long(0x582186fdu), change_endian_long(0x8e17d7dau), change_endian_long(0x6c1a1b95u));
            TSIP.REG_1BCH.WORD = 0x00000040u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_18H.BIT.B12)
            {
                /* waiting */
            }
            return TSIP_ERR_FAIL;
        }
        else
        {
            RX26T_function001(change_endian_long(0xf982cd80u), change_endian_long(0x9de8fcc5u), change_endian_long(0xe7da73bbu), change_endian_long(0xba15afcau));
            TSIP.REG_104H.WORD = 0x00000051u;
            TSIP.REG_A4H.WORD = 0x010006c4u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_04H.WORD = 0x00000113u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_Program[0] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_Program[1] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_Program[2] = TSIP.REG_100H.WORD;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_Program[3] = TSIP.REG_100H.WORD;
            TSIP_GEN_MAC_CB_FUNC(TSIP_FW_CB_REQ_STORE_MAC, iLoop, &counter, InData_UpProgram, OutData_Program, MAX_CNT);
            TSIP.REG_13CH.WORD = 0x000002ffu;
            RX26T_function003(change_endian_long(0x00730a1bu), change_endian_long(0x15c5e61bu), change_endian_long(0x686cf980u), change_endian_long(0xafa0edbfu));
            TSIP.REG_1BCH.WORD = 0x00000040u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_18H.BIT.B12)
            {
                /* waiting */
            }
            return TSIP_SUCCESS;
        }
    }
}
/**********************************************************************************************************************
 End of function ./input_dir/RX26T/RX26T_p24.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_FIRMWARE_UPDATE == 1 */
TSIP_SEC_DEFAULT
