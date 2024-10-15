/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
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
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 15.10.2024 1.00     First Release.
 *********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "./r_rsip_primitive.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
RSIP_SEC_P_SECURE_BOOT_ERASE
rsip_err_t r_rsip_p6e_r1(uint32_t InData_LC[], const uint32_t InData_Cmd[], const uint32_t InData_SharedKeyIndex[], const uint32_t InData_SessionKey[], const uint32_t InData_IV[], const uint32_t InData_InstData[], uint32_t INST_DATA_SIZE, uint32_t OutData_KeyIndex[])
{
    int32_t iLoop = 0U, jLoop = 0U, kLoop = 0U, oLoop = 0U;
    uint32_t OFS_ADR = 0U;
    (void)iLoop;
    (void)jLoop;
    (void)kLoop;
    (void)oLoop;
    (void)OFS_ADR;
    if (0x0U != (RSIP.REG_006CH.WORD & 0x17U))
    {
        return RSIP_ERR_RESOURCE_CONFLICT;
    }
    RSIP.REG_0070H.WORD = 0x006e0001U;
    RSIP.REG_004CH.WORD = 0x00000000U;
    r_rsip_func048(InData_LC);
    RSIP.REG_00A0H.WORD = 0x00010000U;
    RSIP.REG_0094H.WORD = 0x0000b4c0U;
    RSIP.REG_0094H.WORD = 0x0000001cU;
    RSIP.REG_00B0H.WORD = 0x00000892U;
    RSIP.REG_009CH.WORD = 0x80810006U;
    RSIP.REG_0000H.WORD = 0x00030005U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    RSIP.REG_0000H.WORD = 0x03430005U;
    /* WAIT_LOOP */
    while (0U != RSIP.REG_0004H.BIT.B30)
    {
        /* waiting */
    }
    RSIP.REG_0040H.WORD = 0x00001800U;
    r_rsip_func049(InData_Cmd);
    RSIP.REG_0014H.WORD = 0x000000c7U;
    RSIP.REG_009CH.WORD = 0x80010020U;
    /* WAIT_LOOP */
    while (1U != RSIP.REG_0014H.BIT.B31)
    {
        /* waiting */
    }
    RSIP.REG_002CH.WORD = change_endian_long(InData_SharedKeyIndex[0]);
    RSIP.REG_0024H.WORD = 0x00000000U;
    RSIP.REG_0094H.WORD = 0x00000842U;
    RSIP.REG_0094H.WORD = 0x38008800U;
    RSIP.REG_0094H.WORD = 0x00000000U;
    RSIP.REG_0094H.WORD = 0x1000b440U;
    RSIP.REG_0094H.WORD = 0x00000011U;
    RSIP.REG_0094H.WORD = 0x38008800U;
    RSIP.REG_0094H.WORD = 0x00000001U;
    RSIP.REG_0094H.WORD = 0x1000b440U;
    RSIP.REG_0094H.WORD = 0x3CC003AEU;
    RSIP.REG_0094H.WORD = 0x38008800U;
    RSIP.REG_0094H.WORD = 0x00000002U;
    RSIP.REG_0094H.WORD = 0x1000b440U;
    RSIP.REG_0094H.WORD = 0x3CC003AEU;
    RSIP.REG_0094H.WORD = 0x38008800U;
    RSIP.REG_0094H.WORD = 0x00000003U;
    RSIP.REG_0094H.WORD = 0x1000b440U;
    RSIP.REG_0094H.WORD = 0x3CC003A4U;
    RSIP.REG_0094H.WORD = 0x000034a4U;
    RSIP.REG_0094H.WORD = 0x38008880U;
    RSIP.REG_0094H.WORD = 0x000000ffU;
    RSIP.REG_0094H.WORD = 0x1000b480U;
    RSIP.REG_0094H.WORD = 0x00000005U;
    RSIP.REG_0094H.WORD = 0x38008880U;
    RSIP.REG_0094H.WORD = 0x00000023U;
    RSIP.REG_0094H.WORD = 0x1000b480U;
    RSIP.REG_0094H.WORD = 0x00000005U;
    RSIP.REG_0094H.WORD = 0x38008880U;
    RSIP.REG_0094H.WORD = 0x00000022U;
    RSIP.REG_0094H.WORD = 0x1000b480U;
    RSIP.REG_0094H.WORD = 0x00000005U;
    r_rsip_func056();
    RSIP.REG_0094H.WORD = 0x3420a820U;
    RSIP.REG_0094H.WORD = 0x00000010U;
    RSIP.REG_009CH.WORD = 0x00000080U;
    RSIP.REG_0040H.WORD = 0x00A60000U;
    r_rsip_func100(0x57fd6583U, 0x5b3c5f7bU, 0xa99f9bafU, 0x02d3135bU);
    RSIP.REG_0040H.WORD = 0x00400000U;
    RSIP.REG_0024H.WORD = 0x00000000U;
    if (1U == (RSIP.REG_0040H.BIT.B22))
    {
        r_rsip_func102(0x465014e3U, 0xf60352e1U, 0x1e0a308eU, 0x089d1276U);
        RSIP.REG_006CH.WORD = 0x00000040U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0020H.BIT.B12)
        {
            /* waiting */
        }
        return RSIP_ERR_FAIL;
    }
    else
    {
        RSIP.REG_0094H.WORD = 0x00003485U;
        RSIP.REG_0014H.WORD = 0x000001a1U;
        RSIP.REG_00D0H.WORD = 0x07330c04U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        RSIP.REG_00D0H.WORD = 0x07330d04U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        RSIP.REG_009CH.WORD = 0x80010000U;
        RSIP.REG_0000H.WORD = 0x03410005U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_0000H.WORD = 0x0001000dU;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_0014H.WORD = 0x000001a1U;
        RSIP.REG_00D0H.WORD = 0x07330d04U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        RSIP.REG_00D0H.WORD = 0x06330104U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x00000000U);
        RSIP.REG_00B0H.WORD = 0x00001404U;
        RSIP.REG_0000H.WORD = 0x00c10021U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        OFS_ADR = InData_SharedKeyIndex[0] << 3;
        RSIP.REG_0014H.WORD = 0x000000a7U;
        RSIP.REG_009CH.WORD = 0x800100e0U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x0000006eU);
        RSIP.REG_0024H.WORD = 0x00000000U;
        r_rsip_func101(0x60251494U, 0x660fc2e7U, 0xe1e6ea57U, 0xdd7144c8U);
        r_rsip_func058(InData_SessionKey, OFS_ADR);
        RSIP.REG_0014H.WORD = 0x000000a7U;
        RSIP.REG_009CH.WORD = 0x800100e0U;
        /* WAIT_LOOP */
        while (1U != RSIP.REG_0014H.BIT.B31)
        {
            /* waiting */
        }
        RSIP.REG_002CH.WORD = change_endian_long(0x0000006eU);
        RSIP.REG_0024H.WORD = 0x00000000U;
        r_rsip_func101(0x4ca62094U, 0x7a8f18eaU, 0x1f5fb30bU, 0x873033deU);
        r_rsip_func057_r1(InData_IV, InData_InstData, OutData_KeyIndex, INST_DATA_SIZE);
        r_rsip_func100(0x925d6c23U, 0xfe1af6bfU, 0x4c303a91U, 0xc2872c4dU);
        RSIP.REG_00B0H.WORD = 0x00001804U;
        RSIP.REG_00D0H.WORD = 0x080000b5U;
        RSIP.REG_0000H.WORD = 0x00430011U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        RSIP.REG_00D0H.WORD = 0x08000075U;
        RSIP.REG_0000H.WORD = 0x00430011U;
        /* WAIT_LOOP */
        while (0U != RSIP.REG_0004H.BIT.B30)
        {
            /* waiting */
        }
        RSIP.REG_0040H.WORD = 0x00001800U;
        r_rsip_func100(0xc1fa1590U, 0x9f98d37cU, 0x1827c03fU, 0x6c636db2U);
        RSIP.REG_0040H.WORD = 0x00400000U;
        RSIP.REG_0024H.WORD = 0x00000000U;
        if (1U == (RSIP.REG_0040H.BIT.B22))
        {
            r_rsip_func102(0x94c50e1cU, 0xf5b1c7efU, 0xc400c1d0U, 0xe11b472dU);
            RSIP.REG_006CH.WORD = 0x00000040U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0020H.BIT.B12)
            {
                /* waiting */
            }
            return RSIP_ERR_FAIL;
        }
        else
        {
            r_rsip_func100(0x5b71d61cU, 0x878e01d5U, 0xdd2ee974U, 0xc60b2e97U);
            RSIP.REG_009CH.WORD = 0x81010000U;
            RSIP.REG_0008H.WORD = 0x00005006U;
            /* WAIT_LOOP */
            while (1U != RSIP.REG_0008H.BIT.B30)
            {
                /* waiting */
            }
            OutData_KeyIndex[0] = RSIP.REG_002CH.WORD;
            r_rsip_func102(0xdf0779b7U, 0x03f21cc8U, 0x3a571a0eU, 0x4e1b378bU);
            RSIP.REG_006CH.WORD = 0x00000040U;
            /* WAIT_LOOP */
            while (0U != RSIP.REG_0020H.BIT.B12)
            {
                /* waiting */
            }
            return RSIP_SUCCESS;
        }
    }
}
RSIP_SEC_DEFAULT
