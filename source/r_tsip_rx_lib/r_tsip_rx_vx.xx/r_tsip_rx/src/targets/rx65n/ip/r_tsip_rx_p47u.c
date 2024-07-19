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

#if TSIP_PRV_USE_AES_128 == 1
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
* Function Name: R_TSIP_Aes128EncryptDecryptUpdateSub
*******************************************************************************************************************/ /**
* @details       RX65NHU AES-128 Encryption/Decryption with ECB/CBC/CTR Mode Update
* @param[in]     InData_Text
* @param[in]     MAX_CNT
* @param[out]    OutData_Text
* @note          None
*/
void R_TSIP_Aes128EncryptDecryptUpdateSub(uint32_t *InData_Text, uint32_t *OutData_Text, uint32_t MAX_CNT)
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
    if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        TSIP.REG_104H.WORD = 0x00000362u;
        RX65NHU_func100(change_endian_long(0xc5731cd7u), change_endian_long(0x118e3c5du), change_endian_long(0x13c675a3u), change_endian_long(0xbd229a18u));
        TSIP.REG_104H.WORD = 0x000000b1u;
        TSIP.REG_A4H.WORD = 0x00000a86u;
        TSIP.REG_04H.WORD = 0x0000c100u;
    }
    else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        TSIP.REG_104H.WORD = 0x00000362u;
        RX65NHU_func100(change_endian_long(0xce07aac9u), change_endian_long(0x52aba4b4u), change_endian_long(0x06fd10d6u), change_endian_long(0x63a0d8dbu));
        TSIP.REG_104H.WORD = 0x000000b1u;
        TSIP.REG_A4H.WORD = 0x00004a86u;
        TSIP.REG_04H.WORD = 0x0000c100u;
    }
    else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        TSIP.REG_104H.WORD = 0x00000362u;
        RX65NHU_func100(change_endian_long(0x88441d4bu), change_endian_long(0xa6f18f4cu), change_endian_long(0x0ace8ce4u), change_endian_long(0x603d4f3fu));
        TSIP.REG_104H.WORD = 0x000000b1u;
        TSIP.REG_A4H.WORD = 0x00000e96u;
        TSIP.REG_04H.WORD = 0x0000c100u;
    }
    else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        TSIP.REG_104H.WORD = 0x00000362u;
        RX65NHU_func100(change_endian_long(0xf3991a56u), change_endian_long(0xe8699d6bu), change_endian_long(0x615c3de8u), change_endian_long(0x2612a1a7u));
        TSIP.REG_104H.WORD = 0x000000b1u;
        TSIP.REG_A4H.WORD = 0x000049a6u;
        TSIP.REG_04H.WORD = 0x0000c100u;
    }
    else if (0x04000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        TSIP.REG_104H.WORD = 0x00000362u;
        RX65NHU_func100(change_endian_long(0x063d1450u), change_endian_long(0xad8c54e5u), change_endian_long(0x6134ea7bu), change_endian_long(0xed6f51aeu));
        TSIP.REG_104H.WORD = 0x000000b1u;
        TSIP.REG_A4H.WORD = 0x000007b6u;
        TSIP.REG_04H.WORD = 0x0000c100u;
    }
    if(MAX_CNT != 0)
    {
        /* WAIT_LOOP */
        while (1u != TSIP.REG_104H.BIT.B31)
        {
            /* waiting */
        }
        TSIP.REG_100H.WORD = InData_Text[0];
        TSIP.REG_100H.WORD = InData_Text[1];
        TSIP.REG_100H.WORD = InData_Text[2];
        TSIP.REG_100H.WORD = InData_Text[3];
        for (iLoop = 4; iLoop < MAX_CNT ; iLoop = iLoop + 4)
        {
            /* WAIT_LOOP */
            while (1u != TSIP.REG_104H.BIT.B31)
            {
                /* waiting */
            }
            TSIP.REG_100H.WORD = InData_Text[iLoop + 0];
            TSIP.REG_100H.WORD = InData_Text[iLoop + 1];
            TSIP.REG_100H.WORD = InData_Text[iLoop + 2];
            TSIP.REG_100H.WORD = InData_Text[iLoop + 3];
            /* WAIT_LOOP */
            while (1u != TSIP.REG_04H.BIT.B30)
            {
                /* waiting */
            }
            OutData_Text[iLoop-4 + 0] = TSIP.REG_100H.WORD;
            OutData_Text[iLoop-4 + 1] = TSIP.REG_100H.WORD;
            OutData_Text[iLoop-4 + 2] = TSIP.REG_100H.WORD;
            OutData_Text[iLoop-4 + 3] = TSIP.REG_100H.WORD;
        }
        /* WAIT_LOOP */
        while (1u != TSIP.REG_04H.BIT.B30)
        {
            /* waiting */
        }
        OutData_Text[MAX_CNT-4 + 0] = TSIP.REG_100H.WORD;
        OutData_Text[MAX_CNT-4 + 1] = TSIP.REG_100H.WORD;
        OutData_Text[MAX_CNT-4 + 2] = TSIP.REG_100H.WORD;
        OutData_Text[MAX_CNT-4 + 3] = TSIP.REG_100H.WORD;
    }
    if (0x00000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX65NHU_func206();//DisableINTEGRATE_WRRDYBandINTEGRATE_RDRDYBinthisfunction.
        RX65NHU_func101(change_endian_long(0xd634b6bcu), change_endian_long(0xd6da2360u), change_endian_long(0x823f1162u), change_endian_long(0xf188f061u));
    }
    else if (0x01000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX65NHU_func206();//DisableINTEGRATE_WRRDYBandINTEGRATE_RDRDYBinthisfunction.
        RX65NHU_func101(change_endian_long(0x153419beu), change_endian_long(0x96a1e2dau), change_endian_long(0x7c283ec7u), change_endian_long(0x41730b9fu));
    }
    else if (0x02000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX65NHU_func206();//DisableINTEGRATE_WRRDYBandINTEGRATE_RDRDYBinthisfunction.
        RX65NHU_func101(change_endian_long(0xda58ee17u), change_endian_long(0xfe04e7c8u), change_endian_long(0x03ca596bu), change_endian_long(0x6adea2e9u));
    }
    else if (0x03000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX65NHU_func206();//DisableINTEGRATE_WRRDYBandINTEGRATE_RDRDYBinthisfunction.
        RX65NHU_func101(change_endian_long(0xf8c03007u), change_endian_long(0xc5393578u), change_endian_long(0x56678dd2u), change_endian_long(0xb53befe7u));
    }
    else if (0x04000000u == (TSIP.REG_1CH.WORD & 0xff000000u))
    {
        RX65NHU_func206();//DisableINTEGRATE_WRRDYBandINTEGRATE_RDRDYBinthisfunction.
        RX65NHU_func101(change_endian_long(0x3792abccu), change_endian_long(0x341ab0c1u), change_endian_long(0xbf987510u), change_endian_long(0x5152d2f4u));
    }
}
/**********************************************************************************************************************
 End of function ./input_dir/RX65NHU/RX65NHU_p47u_r1.prc
 *********************************************************************************************************************/
#endif /* #if TSIP_PRV_USE_AES_128 == 1 */
