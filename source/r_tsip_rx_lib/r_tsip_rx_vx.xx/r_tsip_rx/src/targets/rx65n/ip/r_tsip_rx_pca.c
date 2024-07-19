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
* Function Name: R_TSIP_Tls13ServerHandshakeVerificationSub
*******************************************************************************************************************/ /**
* @details       RX65NHU TLS1.3 Server Handshake Verify
* @param[in]     InData_Cmd
* @param[in]     InData_ServerFinishedKeyIndex
* @param[in]     InData_MsgHash
* @param[in]     InData_ServerFinished
* @param[out]    OutData_VerifyDataIndex
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_RESOURCE_CONFLICT
* @retval        TSIP_ERR_KEY_SET
* @retval        TSIP_ERR_VERIFICATION_FAIL
* @note          None
*/
e_tsip_err_t R_TSIP_Tls13ServerHandshakeVerificationSub(uint32_t *InData_Cmd, uint32_t *InData_ServerFinishedKeyIndex, uint32_t *InData_MsgHash, uint32_t *InData_ServerFinished, uint32_t *OutData_VerifyDataIndex)
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
    TSIP.REG_84H.WORD = 0x0000ca01u;
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
    TSIP.REG_100H.WORD = InData_Cmd[0];
    RX65NHU_func401();
    TSIP.REG_104H.WORD = 0x00000368u;
    TSIP.REG_E0H.WORD = 0x80040280u;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_ServerFinishedKeyIndex[0];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_ServerFinishedKeyIndex[1];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_ServerFinishedKeyIndex[2];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_ServerFinishedKeyIndex[3];
    RX65NHU_func072();
    TSIP.REG_C4H.WORD = 0x400c0b0cu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = change_endian_long(0x87012d43u);
    TSIP.REG_104H.WORD = 0x00000762u;
    TSIP.REG_D0H.WORD = 0x00000100u;
    TSIP.REG_C4H.WORD = 0x02f087bfu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_ServerFinishedKeyIndex[4];
    TSIP.REG_100H.WORD = InData_ServerFinishedKeyIndex[5];
    TSIP.REG_100H.WORD = InData_ServerFinishedKeyIndex[6];
    TSIP.REG_100H.WORD = InData_ServerFinishedKeyIndex[7];
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_ServerFinishedKeyIndex[8];
    TSIP.REG_100H.WORD = InData_ServerFinishedKeyIndex[9];
    TSIP.REG_100H.WORD = InData_ServerFinishedKeyIndex[10];
    TSIP.REG_100H.WORD = InData_ServerFinishedKeyIndex[11];
    TSIP.REG_E0H.WORD = 0x80080000u;
    TSIP.REG_00H.WORD = 0x00008223u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    TSIP.REG_C4H.WORD = 0x000007bdu;
    /* WAIT_LOOP */
    while (1u != TSIP.REG_104H.BIT.B31)
    {
        /* waiting */
    }
    TSIP.REG_100H.WORD = InData_ServerFinishedKeyIndex[12];
    TSIP.REG_100H.WORD = InData_ServerFinishedKeyIndex[13];
    TSIP.REG_100H.WORD = InData_ServerFinishedKeyIndex[14];
    TSIP.REG_100H.WORD = InData_ServerFinishedKeyIndex[15];
    TSIP.REG_C4H.WORD = 0x00900c45u;
    TSIP.REG_00H.WORD = 0x00002213u;
    /* WAIT_LOOP */
    while (0u != TSIP.REG_00H.BIT.B25)
    {
        /* waiting */
    }
    TSIP.REG_1CH.WORD = 0x00001800u;
    RX65NHU_func100(change_endian_long(0xaa947642u), change_endian_long(0xc3263d04u), change_endian_long(0x53b089adu), change_endian_long(0x9c84cb4au));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        RX65NHU_func102(change_endian_long(0x3cab10d8u), change_endian_long(0x0d2091eau), change_endian_long(0xd6359b6du), change_endian_long(0x0bf78c5fu));
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
        TSIP.REG_28H.WORD = 0x00870001u;
        TSIP.REG_2CH.WORD = 0x00000090u;
        TSIP.REG_104H.WORD = 0x00000767u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_MsgHash[0];
        TSIP.REG_100H.WORD = InData_MsgHash[1];
        TSIP.REG_100H.WORD = InData_MsgHash[2];
        TSIP.REG_100H.WORD = InData_MsgHash[3];
        TSIP.REG_100H.WORD = InData_MsgHash[4];
        TSIP.REG_100H.WORD = InData_MsgHash[5];
        TSIP.REG_100H.WORD = InData_MsgHash[6];
        TSIP.REG_100H.WORD = InData_MsgHash[7];
        TSIP.REG_2CH.WORD = 0x00000010u;
        TSIP.REG_104H.WORD = 0x00000757u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = change_endian_long(0x80000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
        TSIP.REG_100H.WORD = change_endian_long(0x00000300u);
        TSIP.REG_28H.WORD = 0x008f0001u;
        RX65NHU_func070();
        TSIP.REG_00H.WORD = 0x00003523u;
        TSIP.REG_74H.WORD = 0x00000008u;
        TSIP.REG_2CH.WORD = 0x00000010u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_ECH.WORD = 0x0000b400u;
        TSIP.REG_ECH.WORD = 0x00000050u;
        TSIP.REG_104H.WORD = 0x00000768u;
        TSIP.REG_E0H.WORD = 0x80880000u;
        for (iLoop = 0; iLoop < 8; iLoop = iLoop + 1)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_ServerFinished[iLoop + 0];
        }
        TSIP.REG_C4H.WORD = 0x00040805u;
        TSIP.REG_00H.WORD = 0x00002313u;
        TSIP.REG_2CH.WORD = 0x00000020u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_C4H.WORD = 0x00050805u;
        TSIP.REG_00H.WORD = 0x00002313u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_C4H.WORD = 0x00900c05u;
        TSIP.REG_E0H.WORD = 0x81880000u;
        TSIP.REG_00H.WORD = 0x00002813u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        TSIP.REG_C4H.WORD = 0x00900c45u;
        TSIP.REG_00H.WORD = 0x00002813u;
        /* WAIT_LOOP */
        while (0u != TSIP.REG_00H.BIT.B25)
        {
            /* waiting */
        }
        TSIP.REG_1CH.WORD = 0x00001800u;
        RX65NHU_func100(change_endian_long(0x63ac7b61u), change_endian_long(0x34dc8c6bu), change_endian_long(0x8b1a07edu), change_endian_long(0x901fea1eu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX65NHU_func102(change_endian_long(0x1f1cf76fu), change_endian_long(0xd21cee4au), change_endian_long(0x81a15f5eu), change_endian_long(0x35d3faedu));
            TSIP.REG_1BCH.WORD = 0x00000040u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_18H.BIT.B12)
            {
                /* waiting */
            }
            #if TSIP_MULTI_THREADING == 1
            TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
            #endif /* TSIP_MULTI_THREADING == 1 */
            return TSIP_ERR_VERIFICATION_FAIL;
        }
        else
        {
            RX65NHU_func100(change_endian_long(0x04822330u), change_endian_long(0xbd40761du), change_endian_long(0x08271db0u), change_endian_long(0xeadad47bu));
            TSIP.REG_C4H.WORD = 0x400c0b0cu;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x1c5c39c6u);
            RX65NHU_func103();
            RX65NHU_func100(change_endian_long(0x28e9b23cu), change_endian_long(0x4c5edfedu), change_endian_long(0x0abd3f11u), change_endian_long(0xe0cefe80u));
            TSIP.REG_104H.WORD = 0x00000052u;
            TSIP.REG_C4H.WORD = 0x01000c84u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = change_endian_long(0x00000000u);
            TSIP.REG_C4H.WORD = 0x02e007bdu;
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
            OutData_VerifyDataIndex[0] = TSIP.REG_100H.WORD;
            OutData_VerifyDataIndex[1] = TSIP.REG_100H.WORD;
            OutData_VerifyDataIndex[2] = TSIP.REG_100H.WORD;
            OutData_VerifyDataIndex[3] = TSIP.REG_100H.WORD;
            RX65NHU_func100(change_endian_long(0xd9994e69u), change_endian_long(0x101e4be7u), change_endian_long(0xfdcbaf38u), change_endian_long(0x4da4129cu));
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
            OutData_VerifyDataIndex[4] = TSIP.REG_100H.WORD;
            OutData_VerifyDataIndex[5] = TSIP.REG_100H.WORD;
            OutData_VerifyDataIndex[6] = TSIP.REG_100H.WORD;
            OutData_VerifyDataIndex[7] = TSIP.REG_100H.WORD;
            RX65NHU_func102(change_endian_long(0x57866810u), change_endian_long(0x025f7ab5u), change_endian_long(0xe9b1580eu), change_endian_long(0x621216cbu));
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
 End of function ./input_dir/RX65NHU/RX65NHU_pca_r3.prc
 *********************************************************************************************************************/
