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

#if TSIP_AES_256_GCM_DECRYPT == 1
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
* Function Name: R_TSIP_Aes256GcmDecryptFinalSub
*******************************************************************************************************************/ /**
* @details       RX66T AES-256 Decryption with GCM Mode Final
* @param[in]     InData_Text
* @param[in]     InData_TextLen
* @param[in]     InData_DataALen
* @param[in]     InData_DataT
* @param[in]     InData_DataTLen
* @param[out]    OutData_Text
* @retval        TSIP_SUCCESS
* @retval        TSIP_ERR_FAIL
* @retval        TSIP_ERR_AUTHENTICATION
* @note          None
*/
e_tsip_err_t R_TSIP_Aes256GcmDecryptFinalSub(uint32_t *InData_Text, uint32_t *InData_DataT, uint32_t *InData_DataALen, uint32_t *InData_TextLen, uint32_t *InData_DataTLen, uint32_t *OutData_Text)
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
    RX66T_RX72T_function001(change_endian_long(0x806dd654u), change_endian_long(0x57500d19u), change_endian_long(0x544e00beu), change_endian_long(0xd4e31b0eu));
    TSIP.REG_1CH.WORD = 0x00400000u;
    TSIP.REG_1D0H.WORD = 0x00000000u;
    if (1u == (TSIP.REG_1CH.BIT.B22))
    {
        TSIP.REG_1CH.WORD = 0x002d0000u;
        RX66T_RX72T_function001(change_endian_long(0xc8145206u), change_endian_long(0xa5e627e1u), change_endian_long(0x719c944fu), change_endian_long(0x78bc1d07u));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            TSIP.REG_00H.WORD = 0x00000113u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            RX66T_RX72T_function002(change_endian_long(0x91ce0c45u), change_endian_long(0x7c0c01afu), change_endian_long(0xecd000b9u), change_endian_long(0x4d7bac92u));
        }
        else
        {
            RX66T_RX72T_function002(change_endian_long(0x0e8fd511u), change_endian_long(0xadd0a5bbu), change_endian_long(0x33aba95du), change_endian_long(0x9458432fu));
        }
        RX66T_RX72T_function003(change_endian_long(0x8c90cfbfu), change_endian_long(0xf6c0ea25u), change_endian_long(0xa8923538u), change_endian_long(0xf949b814u));
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
        TSIP.REG_ECH.WORD = 0x0000b420u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00000010u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_104H.WORD = 0x00000168u;
        TSIP.REG_E0H.WORD = 0x80820001u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_TextLen[0];
        TSIP.REG_1D0H.WORD = 0x00000000u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_TextLen[1];
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x0000b440u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00000010u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00003822u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x0000a440u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00000004u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00003802u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x0000b440u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x0000007Fu;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x0000b460u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0xFFFFFF00u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x0c002860u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x04a02841u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        TSIP.REG_104H.WORD = 0x00000068u;
        TSIP.REG_E0H.WORD = 0x80010040u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_DataTLen[0];
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x38008840u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00000000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        TSIP.REG_ECH.WORD = 0x0000b460u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x00000010u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_ECH.WORD = 0x34202862u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        TSIP.REG_E0H.WORD = 0x00000080u;
        TSIP.REG_1CH.WORD = 0x00260000u;
        RX66T_RX72T_function001(change_endian_long(0x7be343bau), change_endian_long(0x2b0d8b40u), change_endian_long(0x91f07b98u), change_endian_long(0xcd624e7eu));
        TSIP.REG_1CH.WORD = 0x00400000u;
        TSIP.REG_1D0H.WORD = 0x00000000u;
        if (1u == (TSIP.REG_1CH.BIT.B22))
        {
            RX66T_RX72T_function003(change_endian_long(0x19dbf232u), change_endian_long(0x53c81936u), change_endian_long(0x958cb771u), change_endian_long(0x5c97cfdcu));
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
            TSIP.REG_ECH.WORD = 0x00036800u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x08008c00u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x0000000fu;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x00000080u;
            TSIP.REG_1CH.WORD = 0x00A60000u;
            RX66T_RX72T_function001(change_endian_long(0x78c8912bu), change_endian_long(0xc9d5599du), change_endian_long(0x293a0354u), change_endian_long(0x091d22adu));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX66T_RX72T_function001(change_endian_long(0xbae43738u), change_endian_long(0xfb627ad3u), change_endian_long(0x255b9729u), change_endian_long(0x8303dee3u));
                TSIP.REG_B0H.WORD = 0x40000020u;
                TSIP.REG_104H.WORD = 0x00000365u;
                TSIP.REG_A4H.WORD = 0x000086bdu;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_104H.BIT.B31)
                {
                    /* waiting */
                }
                TSIP.REG_100H.WORD = InData_Text[0];
                TSIP.REG_100H.WORD = InData_Text[1];
                TSIP.REG_100H.WORD = InData_Text[2];
                TSIP.REG_100H.WORD = InData_Text[3];
                TSIP.REG_ECH.WORD = 0x00000821u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_E0H.WORD = 0x80840001u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_00H.WORD = 0x00008113u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_00H.BIT.B25)
                {
                    /* waiting */
                }
                TSIP.REG_1CH.WORD = 0x00001800u;
                TSIP.REG_ECH.WORD = 0x00000863u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                for (iLoop = 0; iLoop < 16; iLoop = iLoop+1)
                {
                    TSIP.REG_ECH.WORD = 0x3c002860u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x12003c23u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                    TSIP.REG_ECH.WORD = 0x00002c60u;
                    TSIP.REG_1D0H.WORD = 0x00000000u;
                }
                TSIP.REG_ECH.WORD = 0x00000821u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_E0H.WORD = 0x81840001u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_04H.WORD = 0x00000613u;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_Text[0] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_Text[1] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_Text[2] = TSIP.REG_100H.WORD;
                /* WAIT_LOOP */
                while (1u != TSIP.REG_04H.BIT.B30)
                {
                    /* waiting */
                }
                OutData_Text[3] = TSIP.REG_100H.WORD;
                RX66T_RX72T_function002(change_endian_long(0xaf8430ffu), change_endian_long(0x7f6cafa7u), change_endian_long(0x59d86f87u), change_endian_long(0xcc636e31u));
            }
            TSIP.REG_104H.WORD = 0x00000164u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_DataALen[0];
            TSIP.REG_100H.WORD = InData_DataALen[1];
            TSIP.REG_ECH.WORD = 0x0000b420u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_ECH.WORD = 0x00000010u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x81820001u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
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
            TSIP.REG_1CH.WORD = 0x00001600u;
            TSIP.REG_74H.WORD = 0x00000000u;
            TSIP.REG_A4H.WORD = 0x00040805u;
            TSIP.REG_E0H.WORD = 0x81040080u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_00H.WORD = 0x00001813u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_B0H.WORD = 0x40000020u;
            TSIP.REG_A4H.WORD = 0x000086bdu;
            TSIP.REG_00H.WORD = 0x00001513u;
            TSIP.REG_74H.WORD = 0x00000008u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x00000821u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x80840001u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_00H.WORD = 0x00008113u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_ECH.WORD = 0x00000863u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            for (iLoop = 0; iLoop < 16; iLoop = iLoop+1)
            {
                TSIP.REG_ECH.WORD = 0x3c002862u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x12003c23u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
                TSIP.REG_ECH.WORD = 0x00002c60u;
                TSIP.REG_1D0H.WORD = 0x00000000u;
            }
            TSIP.REG_104H.WORD = 0x00000361u;
            TSIP.REG_A4H.WORD = 0x00050805u;
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_DataT[0];
            TSIP.REG_100H.WORD = InData_DataT[1];
            TSIP.REG_100H.WORD = InData_DataT[2];
            TSIP.REG_100H.WORD = InData_DataT[3];
            TSIP.REG_A4H.WORD = 0x00900c45u;
            TSIP.REG_ECH.WORD = 0x00000821u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_E0H.WORD = 0x81840001u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            TSIP.REG_00H.WORD = 0x00001813u;
            /* WAIT_LOOP */
            while (0u != TSIP.REG_00H.BIT.B25)
            {
                /* waiting */
            }
            TSIP.REG_1CH.WORD = 0x00001800u;
            TSIP.REG_A4H.WORD = 0x00000000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            RX66T_RX72T_function001(change_endian_long(0xd61313c1u), change_endian_long(0x755f0f21u), change_endian_long(0x3221db2cu), change_endian_long(0x39391f22u));
            TSIP.REG_1CH.WORD = 0x00400000u;
            TSIP.REG_1D0H.WORD = 0x00000000u;
            if (1u == (TSIP.REG_1CH.BIT.B22))
            {
                RX66T_RX72T_function003(change_endian_long(0x9a4db672u), change_endian_long(0x6f689437u), change_endian_long(0x9d65e63du), change_endian_long(0x13707e22u));
                TSIP.REG_1BCH.WORD = 0x00000040u;
                /* WAIT_LOOP */
                while (0u != TSIP.REG_18H.BIT.B12)
                {
                    /* waiting */
                }
                #if TSIP_MULTI_THREADING == 1
                TSIP_MULTI_THREADING_UNLOCK_FUNCTION();
                #endif /* TSIP_MULTI_THREADING == 1 */
                return TSIP_ERR_AUTHENTICATION;
            }
            else
            {
                RX66T_RX72T_function003(change_endian_long(0x4ff4b78bu), change_endian_long(0x8d422f58u), change_endian_long(0x9907ee5eu), change_endian_long(0x399530dcu));
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
}
/**********************************************************************************************************************
 End of function ./input_dir/RX66T/RX66T_p47.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_AES_256_GCM_DECRYPT == 1 */
