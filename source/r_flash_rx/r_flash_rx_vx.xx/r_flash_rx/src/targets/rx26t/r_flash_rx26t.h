/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_flash_rx26t.h
* Description  : This is a private header file used internally by the FLASH API module. It should not be modified or
*                included by the user in their application.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 20.02.2023 5.00    First Release
***********************************************************************************************************************/

#ifndef R_FLASH_RX26T_H
#define R_FLASH_RX26T_H

#include "r_flash_targets.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#if ((MCU_CFG_PART_MEMORY_SIZE == 0x8) || (MCU_CFG_PART_MEMORY_SIZE == 0x9))      // 128 Kb
    #define FLASH_NUM_BLOCKS_CF (6+8)
#elif ((MCU_CFG_PART_MEMORY_SIZE == 0xA) || (MCU_CFG_PART_MEMORY_SIZE == 0xB))    // 256 Kb
    #define FLASH_NUM_BLOCKS_CF (14+8)
#elif (MCU_CFG_PART_MEMORY_SIZE == 0xF)     // 512 Kb
    #if FLASH_IN_DUAL_BANK_MODE
        #define FLASH_NUM_BLOCKS_CF (14+8)  // 256 Kb per bank dual mode
    #else
        #define FLASH_NUM_BLOCKS_CF (30+8)  // 512 Kb linear mode
    #endif
#endif


#define FLASH_NUM_BLOCKS_DF                 (256)
#define FLASH_DF_MIN_PGM_SIZE               (4)
#define FLASH_CF_MIN_PGM_SIZE               (128)

#define FLASH_CF_SMALL_BLOCK_SIZE           (4096)
#define FLASH_CF_MEDIUM_BLOCK_SIZE          (16384)
#define FLASH_CF_LO_BANK_SMALL_BLOCK_ADDR   (FLASH_CF_BLOCK_29)
#define FLASH_CF_LOWEST_VALID_BLOCK         (FLASH_CF_BLOCK_INVALID + 1)
#define FLASH_DF_BLOCK_SIZE                 (64)
#define FLASH_DF_HIGHEST_VALID_BLOCK        (FLASH_DF_BLOCK_INVALID - FLASH_DF_BLOCK_SIZE)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum _flash_block_address
{
#ifndef FLASH_IN_DUAL_BANK_MODE             /* LINEAR MODE */
    FLASH_CF_BLOCK_END     = 0xFFFFFFFF,    /*   End of Code Flash Area       */
    FLASH_CF_BLOCK_0       = 0xFFFFF000,    /*   4KB: 0xFFFFF000 - 0xFFFFFFFF */
    FLASH_CF_BLOCK_1       = 0xFFFFE000,    /*   4KB: 0xFFFFE000 - 0xFFFFEFFF */
    FLASH_CF_BLOCK_2       = 0xFFFFD000,    /*   4KB: 0xFFFFD000 - 0xFFFFDFFF */
    FLASH_CF_BLOCK_3       = 0xFFFFC000,    /*   4KB: 0xFFFFC000 - 0xFFFFCFFF */
    FLASH_CF_BLOCK_4       = 0xFFFFB000,    /*   4KB: 0xFFFFB000 - 0xFFFFBFFF */
    FLASH_CF_BLOCK_5       = 0xFFFFA000,    /*   4KB: 0xFFFFA000 - 0xFFFFAFFF */
    FLASH_CF_BLOCK_6       = 0xFFFF9000,    /*   4KB: 0xFFFF9000 - 0xFFFF9FFF */
    FLASH_CF_BLOCK_7       = 0xFFFF8000,    /*   4KB: 0xFFFF8000 - 0xFFFF8FFF */
    FLASH_CF_BLOCK_8       = 0xFFFF4000,    /*  16KB: 0xFFFF4000 - 0xFFFF7FFF */
    FLASH_CF_BLOCK_9       = 0xFFFF0000,    /*  16KB: 0xFFFF0000 - 0xFFFF3FFF */
    FLASH_CF_BLOCK_10      = 0xFFFEC000,    /*  16KB: 0xFFFEC000 - 0xFFFEFFFF */
    FLASH_CF_BLOCK_11      = 0xFFFE8000,    /*  16KB: 0xFFFE8000 - 0xFFFEBFFF */
    FLASH_CF_BLOCK_12      = 0xFFFE4000,    /*  16KB: 0xFFFE4000 - 0xFFFE7FFF */
    FLASH_CF_BLOCK_13      = 0xFFFE0000,    /*  16KB: 0xFFFE0000 - 0xFFFE3FFF */
#if ((MCU_CFG_PART_MEMORY_SIZE == 0x08) || (MCU_CFG_PART_MEMORY_SIZE == 0x09)) /* '8' or '9' parts 128 Kb ROM */
    FLASH_CF_BLOCK_INVALID = (FLASH_CF_BLOCK_13 - 1),
#else
    FLASH_CF_BLOCK_14      = 0xFFFDC000,    /*  16KB: 0xFFFDC000 - 0xFFFDFFFF */
    FLASH_CF_BLOCK_15      = 0xFFFD8000,    /*  16KB: 0xFFFD8000 - 0xFFFDBFFF */
    FLASH_CF_BLOCK_16      = 0xFFFD4000,    /*  16KB: 0xFFFD4000 - 0xFFFD7FFF */
    FLASH_CF_BLOCK_17      = 0xFFFD0000,    /*  16KB: 0xFFFD0000 - 0xFFFD3FFF */
    FLASH_CF_BLOCK_18      = 0xFFFCC000,    /*  16KB: 0xFFFCC000 - 0xFFFCFFFF */
    FLASH_CF_BLOCK_19      = 0xFFFC8000,    /*  16KB: 0xFFFC8000 - 0xFFFCBFFF */
    FLASH_CF_BLOCK_20      = 0xFFFC4000,    /*  16KB: 0xFFFC4000 - 0xFFFC7FFF */
    FLASH_CF_BLOCK_21      = 0xFFFC0000,    /*  16KB: 0xFFFC0000 - 0xFFFC3FFF */
#if ((MCU_CFG_PART_MEMORY_SIZE == 0x0A) || (MCU_CFG_PART_MEMORY_SIZE == 0x0B)) /* 'A' or 'B' parts 256 Kb ROM */
    FLASH_CF_BLOCK_INVALID = (FLASH_CF_BLOCK_21 - 1),
#else
    FLASH_CF_BLOCK_22      = 0xFFFBC000,    /*  16KB: 0xFFFBC000 - 0xFFFBFFFF */
    FLASH_CF_BLOCK_23      = 0xFFFB8000,    /*  16KB: 0xFFFB8000 - 0xFFFBBFFF */
    FLASH_CF_BLOCK_24      = 0xFFFB4000,    /*  16KB: 0xFFFB4000 - 0xFFFB7FFF */
    FLASH_CF_BLOCK_25      = 0xFFFB0000,    /*  16KB: 0xFFFB0000 - 0xFFFB3FFF */
    FLASH_CF_BLOCK_26      = 0xFFFAC000,    /*  16KB: 0xFFFAC000 - 0xFFFAFFFF */
    FLASH_CF_BLOCK_27      = 0xFFFA8000,    /*  16KB: 0xFFFA8000 - 0xFFFABFFF */
    FLASH_CF_BLOCK_28      = 0xFFFA4000,    /*  16KB: 0xFFFA4000 - 0xFFFA7FFF */
    FLASH_CF_BLOCK_29      = 0xFFFA0000,    /*  16KB: 0xFFFA0000 - 0xFFFA3FFF */
    FLASH_CF_BLOCK_30      = 0xFFF9C000,    /*  16KB: 0xFFF9C000 - 0xFFF9FFFF */
    FLASH_CF_BLOCK_31      = 0xFFF98000,    /*  16KB: 0xFFF98000 - 0xFFF9BFFF */
    FLASH_CF_BLOCK_32      = 0xFFF94000,    /*  16KB: 0xFFF94000 - 0xFFF97FFF */
    FLASH_CF_BLOCK_33      = 0xFFF90000,    /*  16KB: 0xFFF90000 - 0xFFF93FFF */
    FLASH_CF_BLOCK_34      = 0xFFF8C000,    /*  16KB: 0xFFF8C000 - 0xFFF8FFFF */
    FLASH_CF_BLOCK_35      = 0xFFF88000,    /*  16KB: 0xFFF88000 - 0xFFF8BFFF */
    FLASH_CF_BLOCK_36      = 0xFFF84000,    /*  16KB: 0xFFF84000 - 0xFFF87FFF */
    FLASH_CF_BLOCK_37      = 0xFFF80000,    /*  16KB: 0xFFF80000 - 0xFFF83FFF */
    FLASH_CF_BLOCK_INVALID = (FLASH_CF_BLOCK_37 - 1), /* 'F' parts 512 Kb ROM */
#endif // > 256K
#endif // > 128K

#else                                       /* DUAL MODE */
    FLASH_CF_BLOCK_END     = 0xFFFFFFFF,    /*   End of Code Flash Area       */
    FLASH_CF_HI_BANK_HI_ADDR = FLASH_CF_BLOCK_END,
    FLASH_CF_BLOCK_0       = 0xFFFFF000,    /*   4KB: 0xFFFFF000 - 0xFFFFFFFF */
    FLASH_CF_BLOCK_1       = 0xFFFFE000,    /*   4KB: 0xFFFFE000 - 0xFFFFEFFF */
    FLASH_CF_BLOCK_2       = 0xFFFFD000,    /*   4KB: 0xFFFFD000 - 0xFFFFDFFF */
    FLASH_CF_BLOCK_3       = 0xFFFFC000,    /*   4KB: 0xFFFFC000 - 0xFFFFCFFF */
    FLASH_CF_BLOCK_4       = 0xFFFFB000,    /*   4KB: 0xFFFFB000 - 0xFFFFBFFF */
    FLASH_CF_BLOCK_5       = 0xFFFFA000,    /*   4KB: 0xFFFFA000 - 0xFFFFAFFF */
    FLASH_CF_BLOCK_6       = 0xFFFF9000,    /*   4KB: 0xFFFF9000 - 0xFFFF9FFF */
    FLASH_CF_BLOCK_7       = 0xFFFF8000,    /*   4KB: 0xFFFF8000 - 0xFFFF8FFF */
    FLASH_CF_BLOCK_8       = 0xFFFF4000,    /*  16KB: 0xFFFF4000 - 0xFFFF7FFF */
    FLASH_CF_BLOCK_9       = 0xFFFF0000,    /*  16KB: 0xFFFF0000 - 0xFFFF3FFF */
    FLASH_CF_BLOCK_10      = 0xFFFEC000,    /*  16KB: 0xFFFEC000 - 0xFFFEFFFF */
    FLASH_CF_BLOCK_11      = 0xFFFE8000,    /*  16KB: 0xFFFE8000 - 0xFFFEBFFF */
    FLASH_CF_BLOCK_12      = 0xFFFE4000,    /*  16KB: 0xFFFE4000 - 0xFFFE7FFF */
    FLASH_CF_BLOCK_13      = 0xFFFE0000,    /*  16KB: 0xFFFE0000 - 0xFFFE3FFF */
    FLASH_CF_BLOCK_14      = 0xFFFDC000,    /*  16KB: 0xFFFDC000 - 0xFFFDFFFF */
    FLASH_CF_BLOCK_15      = 0xFFFD8000,    /*  16KB: 0xFFFD8000 - 0xFFFDBFFF */
    FLASH_CF_BLOCK_16      = 0xFFFD4000,    /*  16KB: 0xFFFD4000 - 0xFFFD7FFF */
    FLASH_CF_BLOCK_17      = 0xFFFD0000,    /*  16KB: 0xFFFD0000 - 0xFFFD3FFF */
    FLASH_CF_BLOCK_18      = 0xFFFCC000,    /*  16KB: 0xFFFCC000 - 0xFFFCFFFF */
    FLASH_CF_BLOCK_19      = 0xFFFC8000,    /*  16KB: 0xFFFC8000 - 0xFFFCBFFF */
    FLASH_CF_BLOCK_20      = 0xFFFC4000,    /*  16KB: 0xFFFC4000 - 0xFFFC7FFF */
    FLASH_CF_BLOCK_21      = 0xFFFC0000,    /*  16KB: 0xFFFC0000 - 0xFFFC3FFF */
    FLASH_CF_HI_BANK_LO_ADDR = FLASH_CF_BLOCK_21,
    FLASH_CF_LO_BANK_HI_ADDR = 0xFFFBFFFF,  /* START OF NEXT BANK */
    FLASH_CF_BLOCK_22      = 0xFFFBF000,    /*   4KB: 0xFFFBF000 - 0xFFFBFFFF */
    FLASH_CF_BLOCK_23      = 0xFFFBE000,    /*   4KB: 0xFFFBE000 - 0xFFFBEFFF */
    FLASH_CF_BLOCK_24      = 0xFFFBD000,    /*   4KB: 0xFFFBD000 - 0xFFFBDFFF */
    FLASH_CF_BLOCK_25      = 0xFFFBC000,    /*   4KB: 0xFFFBC000 - 0xFFFBCFFF */
    FLASH_CF_BLOCK_26      = 0xFFFBB000,    /*   4KB: 0xFFFBB000 - 0xFFFBBFFF */
    FLASH_CF_BLOCK_27      = 0xFFFBA000,    /*   4KB: 0xFFFBA000 - 0xFFFBAFFF */
    FLASH_CF_BLOCK_28      = 0xFFFB9000,    /*   4KB: 0xFFFB9000 - 0xFFFB9FFF */
    FLASH_CF_BLOCK_29      = 0xFFFB8000,    /*   4KB: 0xFFFB8000 - 0xFFFB8FFF */
    FLASH_CF_BLOCK_30      = 0xFFFB4000,    /*  16KB: 0xFFFB4000 - 0xFFFB7FFF */
    FLASH_CF_BLOCK_31      = 0xFFFB0000,    /*  16KB: 0xFFFB0000 - 0xFFFB3FFF */
    FLASH_CF_BLOCK_32      = 0xFFFAC000,    /*  16KB: 0xFFFAC000 - 0xFFFAFFFF */
    FLASH_CF_BLOCK_33      = 0xFFFA8000,    /*  16KB: 0xFFFA8000 - 0xFFFABFFF */
    FLASH_CF_BLOCK_34      = 0xFFFA4000,    /*  16KB: 0xFFFA4000 - 0xFFFA7FFF */
    FLASH_CF_BLOCK_35      = 0xFFFA0000,    /*  16KB: 0xFFFA0000 - 0xFFFA3FFF */
    FLASH_CF_BLOCK_36      = 0xFFF9C000,    /*  16KB: 0xFFF9C000 - 0xFFF9FFFF */
    FLASH_CF_BLOCK_37      = 0xFFF98000,    /*  16KB: 0xFFF98000 - 0xFFF9BFFF */
    FLASH_CF_BLOCK_38      = 0xFFF94000,    /*  16KB: 0xFFF94000 - 0xFFF97FFF */
    FLASH_CF_BLOCK_39      = 0xFFF90000,    /*  16KB: 0xFFF90000 - 0xFFF93FFF */
    FLASH_CF_BLOCK_40      = 0xFFF8C000,    /*  16KB: 0xFFF8C000 - 0xFFF8FFFF */
    FLASH_CF_BLOCK_41      = 0xFFF88000,    /*  16KB: 0xFFF88000 - 0xFFF8BFFF */
    FLASH_CF_BLOCK_42      = 0xFFF84000,    /*  16KB: 0xFFF84000 - 0xFFF87FFF */
    FLASH_CF_BLOCK_43      = 0xFFF80000,    /*  16KB: 0xFFF80000 - 0xFFF83FFF */
    FLASH_CF_LO_BANK_LO_ADDR = FLASH_CF_BLOCK_43,
    FLASH_CF_BLOCK_INVALID = (FLASH_CF_BLOCK_43 - 1),
#endif // DUAL MODE

    FLASH_DF_BLOCK_0       = 0x00100000,    /*   64B: 0x00100000 - 0x0010003F */
    FLASH_DF_BLOCK_1       = 0x00100040,    /*   64B: 0x00100040 - 0x0010007F */
    FLASH_DF_BLOCK_2       = 0x00100080,    /*   64B: 0x00100080 - 0x001000BF */
    FLASH_DF_BLOCK_3       = 0x001000C0,    /*   64B: 0x001000C0 - 0x001000FF */
    FLASH_DF_BLOCK_4       = 0x00100100,    /*   64B: 0x00100100 - 0x0010013F */
    FLASH_DF_BLOCK_5       = 0x00100140,    /*   64B: 0x00100140 - 0x0010017F */
    FLASH_DF_BLOCK_6       = 0x00100180,    /*   64B: 0x00100180 - 0x001001BF */
    FLASH_DF_BLOCK_7       = 0x001001C0,    /*   64B: 0x001001C0 - 0x001001FF */
    FLASH_DF_BLOCK_8       = 0x00100200,    /*   64B: 0x00100200 - 0x0010023F */
    FLASH_DF_BLOCK_9       = 0x00100240,    /*   64B: 0x00100240 - 0x0010027F */
    FLASH_DF_BLOCK_10      = 0x00100280,    /*   64B: 0x00100280 - 0x001002BF */
    FLASH_DF_BLOCK_11      = 0x001002C0,    /*   64B: 0x001002C0 - 0x001002FF */
    FLASH_DF_BLOCK_12      = 0x00100300,    /*   64B: 0x00100300 - 0x0010033F */
    FLASH_DF_BLOCK_13      = 0x00100340,    /*   64B: 0x00100340 - 0x0010037F */
    FLASH_DF_BLOCK_14      = 0x00100380,    /*   64B: 0x00100380 - 0x001003BF */
    FLASH_DF_BLOCK_15      = 0x001003C0,    /*   64B: 0x001003C0 - 0x001003FF */
    FLASH_DF_BLOCK_16      = 0x00100400,    /*   64B: 0x00100400 - 0x0010043F */
    FLASH_DF_BLOCK_17      = 0x00100440,    /*   64B: 0x00100440 - 0x0010047F */
    FLASH_DF_BLOCK_18      = 0x00100480,    /*   64B: 0x00100480 - 0x001004BF */
    FLASH_DF_BLOCK_19      = 0x001004C0,    /*   64B: 0x001004C0 - 0x001004FF */
    FLASH_DF_BLOCK_20      = 0x00100500,    /*   64B: 0x00100500 - 0x0010053F */
    FLASH_DF_BLOCK_21      = 0x00100540,    /*   64B: 0x00100540 - 0x0010057F */
    FLASH_DF_BLOCK_22      = 0x00100580,    /*   64B: 0x00100580 - 0x001005BF */
    FLASH_DF_BLOCK_23      = 0x001005C0,    /*   64B: 0x001005C0 - 0x001005FF */
    FLASH_DF_BLOCK_24      = 0x00100600,    /*   64B: 0x00100600 - 0x0010063F */
    FLASH_DF_BLOCK_25      = 0x00100640,    /*   64B: 0x00100640 - 0x0010067F */
    FLASH_DF_BLOCK_26      = 0x00100680,    /*   64B: 0x00100680 - 0x001006BF */
    FLASH_DF_BLOCK_27      = 0x001006C0,    /*   64B: 0x001006C0 - 0x001006FF */
    FLASH_DF_BLOCK_28      = 0x00100700,    /*   64B: 0x00100700 - 0x0010073F */
    FLASH_DF_BLOCK_29      = 0x00100740,    /*   64B: 0x00100740 - 0x0010077F */
    FLASH_DF_BLOCK_30      = 0x00100780,    /*   64B: 0x00100780 - 0x001007BF */
    FLASH_DF_BLOCK_31      = 0x001007C0,    /*   64B: 0x001007C0 - 0x001007FF */
    FLASH_DF_BLOCK_32      = 0x00100800,    /*   64B: 0x00100800 - 0x0010083F */
    FLASH_DF_BLOCK_33      = 0x00100840,    /*   64B: 0x00100840 - 0x0010087F */
    FLASH_DF_BLOCK_34      = 0x00100880,    /*   64B: 0x00100880 - 0x001008BF */
    FLASH_DF_BLOCK_35      = 0x001008C0,    /*   64B: 0x001008C0 - 0x001008FF */
    FLASH_DF_BLOCK_36      = 0x00100900,    /*   64B: 0x00100900 - 0x0010093F */
    FLASH_DF_BLOCK_37      = 0x00100940,    /*   64B: 0x00100940 - 0x0010097F */
    FLASH_DF_BLOCK_38      = 0x00100980,    /*   64B: 0x00100980 - 0x001009BF */
    FLASH_DF_BLOCK_39      = 0x001009C0,    /*   64B: 0x001009C0 - 0x001009FF */
    FLASH_DF_BLOCK_40      = 0x00100A00,    /*   64B: 0x00100A00 - 0x00100A3F */
    FLASH_DF_BLOCK_41      = 0x00100A40,    /*   64B: 0x00100A40 - 0x00100A7F */
    FLASH_DF_BLOCK_42      = 0x00100A80,    /*   64B: 0x00100A80 - 0x00100ABF */
    FLASH_DF_BLOCK_43      = 0x00100AC0,    /*   64B: 0x00100AC0 - 0x00100AFF */
    FLASH_DF_BLOCK_44      = 0x00100B00,    /*   64B: 0x00100B00 - 0x00100B3F */
    FLASH_DF_BLOCK_45      = 0x00100B40,    /*   64B: 0x00100B40 - 0x00100B7F */
    FLASH_DF_BLOCK_46      = 0x00100B80,    /*   64B: 0x00100B80 - 0x00100BBF */
    FLASH_DF_BLOCK_47      = 0x00100BC0,    /*   64B: 0x00100BC0 - 0x00100BFF */
    FLASH_DF_BLOCK_48      = 0x00100C00,    /*   64B: 0x00100C00 - 0x00100C3F */
    FLASH_DF_BLOCK_49      = 0x00100C40,    /*   64B: 0x00100C40 - 0x00100C7F */
    FLASH_DF_BLOCK_50      = 0x00100C80,    /*   64B: 0x00100C80 - 0x00100CBF */
    FLASH_DF_BLOCK_51      = 0x00100CC0,    /*   64B: 0x00100CC0 - 0x00100CFF */
    FLASH_DF_BLOCK_52      = 0x00100D00,    /*   64B: 0x00100D00 - 0x00100D3F */
    FLASH_DF_BLOCK_53      = 0x00100D40,    /*   64B: 0x00100D40 - 0x00100D7F */
    FLASH_DF_BLOCK_54      = 0x00100D80,    /*   64B: 0x00100D80 - 0x00100DBF */
    FLASH_DF_BLOCK_55      = 0x00100DC0,    /*   64B: 0x00100DC0 - 0x00100DFF */
    FLASH_DF_BLOCK_56      = 0x00100E00,    /*   64B: 0x00100E00 - 0x00100E3F */
    FLASH_DF_BLOCK_57      = 0x00100E40,    /*   64B: 0x00100E40 - 0x00100E7F */
    FLASH_DF_BLOCK_58      = 0x00100E80,    /*   64B: 0x00100E80 - 0x00100EBF */
    FLASH_DF_BLOCK_59      = 0x00100EC0,    /*   64B: 0x00100EC0 - 0x00100EFF */
    FLASH_DF_BLOCK_60      = 0x00100F00,    /*   64B: 0x00100F00 - 0x00100F3F */
    FLASH_DF_BLOCK_61      = 0x00100F40,    /*   64B: 0x00100F40 - 0x00100F7F */
    FLASH_DF_BLOCK_62      = 0x00100F80,    /*   64B: 0x00100F80 - 0x00100FBF */
    FLASH_DF_BLOCK_63      = 0x00100FC0,    /*   64B: 0x00100FC0 - 0x00100FFF */
    FLASH_DF_BLOCK_64      = 0x00101000,    /*   64B: 0x00101000 - 0x0010103F */
    FLASH_DF_BLOCK_65      = 0x00101040,    /*   64B: 0x00101040 - 0x0010107F */
    FLASH_DF_BLOCK_66      = 0x00101080,    /*   64B: 0x00101080 - 0x001010BF */
    FLASH_DF_BLOCK_67      = 0x001010C0,    /*   64B: 0x001010C0 - 0x001010FF */
    FLASH_DF_BLOCK_68      = 0x00101100,    /*   64B: 0x00101100 - 0x0010113F */
    FLASH_DF_BLOCK_69      = 0x00101140,    /*   64B: 0x00101140 - 0x0010117F */
    FLASH_DF_BLOCK_70      = 0x00101180,    /*   64B: 0x00101180 - 0x001011BF */
    FLASH_DF_BLOCK_71      = 0x001011C0,    /*   64B: 0x001011C0 - 0x001011FF */
    FLASH_DF_BLOCK_72      = 0x00101200,    /*   64B: 0x00101200 - 0x0010123F */
    FLASH_DF_BLOCK_73      = 0x00101240,    /*   64B: 0x00101240 - 0x0010127F */
    FLASH_DF_BLOCK_74      = 0x00101280,    /*   64B: 0x00101280 - 0x001012BF */
    FLASH_DF_BLOCK_75      = 0x001012C0,    /*   64B: 0x001012C0 - 0x001012FF */
    FLASH_DF_BLOCK_76      = 0x00101300,    /*   64B: 0x00101300 - 0x0010133F */
    FLASH_DF_BLOCK_77      = 0x00101340,    /*   64B: 0x00101340 - 0x0010137F */
    FLASH_DF_BLOCK_78      = 0x00101380,    /*   64B: 0x00101380 - 0x001013BF */
    FLASH_DF_BLOCK_79      = 0x001013C0,    /*   64B: 0x001013C0 - 0x001013FF */
    FLASH_DF_BLOCK_80      = 0x00101400,    /*   64B: 0x00101400 - 0x0010143F */
    FLASH_DF_BLOCK_81      = 0x00101440,    /*   64B: 0x00101440 - 0x0010147F */
    FLASH_DF_BLOCK_82      = 0x00101480,    /*   64B: 0x00101480 - 0x001014BF */
    FLASH_DF_BLOCK_83      = 0x001014C0,    /*   64B: 0x001014C0 - 0x001014FF */
    FLASH_DF_BLOCK_84      = 0x00101500,    /*   64B: 0x00101500 - 0x0010153F */
    FLASH_DF_BLOCK_85      = 0x00101540,    /*   64B: 0x00101540 - 0x0010157F */
    FLASH_DF_BLOCK_86      = 0x00101580,    /*   64B: 0x00101580 - 0x001015BF */
    FLASH_DF_BLOCK_87      = 0x001015C0,    /*   64B: 0x001015C0 - 0x001015FF */
    FLASH_DF_BLOCK_88      = 0x00101600,    /*   64B: 0x00101600 - 0x0010163F */
    FLASH_DF_BLOCK_89      = 0x00101640,    /*   64B: 0x00101640 - 0x0010167F */
    FLASH_DF_BLOCK_90      = 0x00101680,    /*   64B: 0x00101680 - 0x001016BF */
    FLASH_DF_BLOCK_91      = 0x001016C0,    /*   64B: 0x001016C0 - 0x001016FF */
    FLASH_DF_BLOCK_92      = 0x00101700,    /*   64B: 0x00101700 - 0x0010173F */
    FLASH_DF_BLOCK_93      = 0x00101740,    /*   64B: 0x00101740 - 0x0010177F */
    FLASH_DF_BLOCK_94      = 0x00101780,    /*   64B: 0x00101780 - 0x001017BF */
    FLASH_DF_BLOCK_95      = 0x001017C0,    /*   64B: 0x001017C0 - 0x001017FF */
    FLASH_DF_BLOCK_96      = 0x00101800,    /*   64B: 0x00101800 - 0x0010183F */
    FLASH_DF_BLOCK_97      = 0x00101840,    /*   64B: 0x00101840 - 0x0010187F */
    FLASH_DF_BLOCK_98      = 0x00101880,    /*   64B: 0x00101880 - 0x001018BF */
    FLASH_DF_BLOCK_99      = 0x001018C0,    /*   64B: 0x001018C0 - 0x001018FF */
    FLASH_DF_BLOCK_100     = 0x00101900,    /*   64B: 0x00101900 - 0x0010193F */
    FLASH_DF_BLOCK_101     = 0x00101940,    /*   64B: 0x00101940 - 0x0010197F */
    FLASH_DF_BLOCK_102     = 0x00101980,    /*   64B: 0x00101980 - 0x001019BF */
    FLASH_DF_BLOCK_103     = 0x001019C0,    /*   64B: 0x001019C0 - 0x001019FF */
    FLASH_DF_BLOCK_104     = 0x00101A00,    /*   64B: 0x00101A00 - 0x00101A3F */
    FLASH_DF_BLOCK_105     = 0x00101A40,    /*   64B: 0x00101A40 - 0x00101A7F */
    FLASH_DF_BLOCK_106     = 0x00101A80,    /*   64B: 0x00101A80 - 0x00101ABF */
    FLASH_DF_BLOCK_107     = 0x00101AC0,    /*   64B: 0x00101AC0 - 0x00101AFF */
    FLASH_DF_BLOCK_108     = 0x00101B00,    /*   64B: 0x00101B00 - 0x00101B3F */
    FLASH_DF_BLOCK_109     = 0x00101B40,    /*   64B: 0x00101B40 - 0x00101B7F */
    FLASH_DF_BLOCK_110     = 0x00101B80,    /*   64B: 0x00101B80 - 0x00101BBF */
    FLASH_DF_BLOCK_111     = 0x00101BC0,    /*   64B: 0x00101BC0 - 0x00101BFF */
    FLASH_DF_BLOCK_112     = 0x00101C00,    /*   64B: 0x00101C00 - 0x00101C3F */
    FLASH_DF_BLOCK_113     = 0x00101C40,    /*   64B: 0x00101C40 - 0x00101C7F */
    FLASH_DF_BLOCK_114     = 0x00101C80,    /*   64B: 0x00101C80 - 0x00101CBF */
    FLASH_DF_BLOCK_115     = 0x00101CC0,    /*   64B: 0x00101CC0 - 0x00101CFF */
    FLASH_DF_BLOCK_116     = 0x00101D00,    /*   64B: 0x00101D00 - 0x00101D3F */
    FLASH_DF_BLOCK_117     = 0x00101D40,    /*   64B: 0x00101D40 - 0x00101D7F */
    FLASH_DF_BLOCK_118     = 0x00101D80,    /*   64B: 0x00101D80 - 0x00101DBF */
    FLASH_DF_BLOCK_119     = 0x00101DC0,    /*   64B: 0x00101DC0 - 0x00101DFF */
    FLASH_DF_BLOCK_120     = 0x00101E00,    /*   64B: 0x00101E00 - 0x00101E3F */
    FLASH_DF_BLOCK_121     = 0x00101E40,    /*   64B: 0x00101E40 - 0x00101E7F */
    FLASH_DF_BLOCK_122     = 0x00101E80,    /*   64B: 0x00101E80 - 0x00101EBF */
    FLASH_DF_BLOCK_123     = 0x00101EC0,    /*   64B: 0x00101EC0 - 0x00101EFF */
    FLASH_DF_BLOCK_124     = 0x00101F00,    /*   64B: 0x00101F00 - 0x00101F3F */
    FLASH_DF_BLOCK_125     = 0x00101F40,    /*   64B: 0x00101F40 - 0x00101F7F */
    FLASH_DF_BLOCK_126     = 0x00101F80,    /*   64B: 0x00101F80 - 0x00101FBF */
    FLASH_DF_BLOCK_127     = 0x00101FC0,    /*   64B: 0x00101FC0 - 0x00101FFF */
    FLASH_DF_BLOCK_128     = 0x00102000,    /*   64B: 0x00102000 - 0x0010203F */
    FLASH_DF_BLOCK_129     = 0x00102040,    /*   64B: 0x00102040 - 0x0010207F */
    FLASH_DF_BLOCK_130     = 0x00102080,    /*   64B: 0x00102080 - 0x001020BF */
    FLASH_DF_BLOCK_131     = 0x001020C0,    /*   64B: 0x001020C0 - 0x001020FF */
    FLASH_DF_BLOCK_132     = 0x00102100,    /*   64B: 0x00102100 - 0x0010213F */
    FLASH_DF_BLOCK_133     = 0x00102140,    /*   64B: 0x00102140 - 0x0010217F */
    FLASH_DF_BLOCK_134     = 0x00102180,    /*   64B: 0x00102180 - 0x001021BF */
    FLASH_DF_BLOCK_135     = 0x001021C0,    /*   64B: 0x001021C0 - 0x001021FF */
    FLASH_DF_BLOCK_136     = 0x00102200,    /*   64B: 0x00102200 - 0x0010223F */
    FLASH_DF_BLOCK_137     = 0x00102240,    /*   64B: 0x00102240 - 0x0010227F */
    FLASH_DF_BLOCK_138     = 0x00102280,    /*   64B: 0x00102280 - 0x001022BF */
    FLASH_DF_BLOCK_139     = 0x001022C0,    /*   64B: 0x001022C0 - 0x001022FF */
    FLASH_DF_BLOCK_140     = 0x00102300,    /*   64B: 0x00102300 - 0x0010233F */
    FLASH_DF_BLOCK_141     = 0x00102340,    /*   64B: 0x00102340 - 0x0010237F */
    FLASH_DF_BLOCK_142     = 0x00102380,    /*   64B: 0x00102380 - 0x001023BF */
    FLASH_DF_BLOCK_143     = 0x001023C0,    /*   64B: 0x001023C0 - 0x001023FF */
    FLASH_DF_BLOCK_144     = 0x00102400,    /*   64B: 0x00102400 - 0x0010243F */
    FLASH_DF_BLOCK_145     = 0x00102440,    /*   64B: 0x00102440 - 0x0010247F */
    FLASH_DF_BLOCK_146     = 0x00102480,    /*   64B: 0x00102480 - 0x001024BF */
    FLASH_DF_BLOCK_147     = 0x001024C0,    /*   64B: 0x001024C0 - 0x001024FF */
    FLASH_DF_BLOCK_148     = 0x00102500,    /*   64B: 0x00102500 - 0x0010253F */
    FLASH_DF_BLOCK_149     = 0x00102540,    /*   64B: 0x00102540 - 0x0010257F */
    FLASH_DF_BLOCK_150     = 0x00102580,    /*   64B: 0x00102580 - 0x001025BF */
    FLASH_DF_BLOCK_151     = 0x001025C0,    /*   64B: 0x001025C0 - 0x001025FF */
    FLASH_DF_BLOCK_152     = 0x00102600,    /*   64B: 0x00102600 - 0x0010263F */
    FLASH_DF_BLOCK_153     = 0x00102640,    /*   64B: 0x00102640 - 0x0010267F */
    FLASH_DF_BLOCK_154     = 0x00102680,    /*   64B: 0x00102680 - 0x001026BF */
    FLASH_DF_BLOCK_155     = 0x001026C0,    /*   64B: 0x001026C0 - 0x001026FF */
    FLASH_DF_BLOCK_156     = 0x00102700,    /*   64B: 0x00102700 - 0x0010273F */
    FLASH_DF_BLOCK_157     = 0x00102740,    /*   64B: 0x00102740 - 0x0010277F */
    FLASH_DF_BLOCK_158     = 0x00102780,    /*   64B: 0x00102780 - 0x001027BF */
    FLASH_DF_BLOCK_159     = 0x001027C0,    /*   64B: 0x001027C0 - 0x001027FF */
    FLASH_DF_BLOCK_160     = 0x00102800,    /*   64B: 0x00102800 - 0x0010283F */
    FLASH_DF_BLOCK_161     = 0x00102840,    /*   64B: 0x00102840 - 0x0010287F */
    FLASH_DF_BLOCK_162     = 0x00102880,    /*   64B: 0x00102880 - 0x001028BF */
    FLASH_DF_BLOCK_163     = 0x001028C0,    /*   64B: 0x001028C0 - 0x001028FF */
    FLASH_DF_BLOCK_164     = 0x00102900,    /*   64B: 0x00102900 - 0x0010293F */
    FLASH_DF_BLOCK_165     = 0x00102940,    /*   64B: 0x00102940 - 0x0010297F */
    FLASH_DF_BLOCK_166     = 0x00102980,    /*   64B: 0x00102980 - 0x001029BF */
    FLASH_DF_BLOCK_167     = 0x001029C0,    /*   64B: 0x001029C0 - 0x001029FF */
    FLASH_DF_BLOCK_168     = 0x00102A00,    /*   64B: 0x00102A00 - 0x00102A3F */
    FLASH_DF_BLOCK_169     = 0x00102A40,    /*   64B: 0x00102A40 - 0x00102A7F */
    FLASH_DF_BLOCK_170     = 0x00102A80,    /*   64B: 0x00102A80 - 0x00102ABF */
    FLASH_DF_BLOCK_171     = 0x00102AC0,    /*   64B: 0x00102AC0 - 0x00102AFF */
    FLASH_DF_BLOCK_172     = 0x00102B00,    /*   64B: 0x00102B00 - 0x00102B3F */
    FLASH_DF_BLOCK_173     = 0x00102B40,    /*   64B: 0x00102B40 - 0x00102B7F */
    FLASH_DF_BLOCK_174     = 0x00102B80,    /*   64B: 0x00102B80 - 0x00102BBF */
    FLASH_DF_BLOCK_175     = 0x00102BC0,    /*   64B: 0x00102BC0 - 0x00102BFF */
    FLASH_DF_BLOCK_176     = 0x00102C00,    /*   64B: 0x00102C00 - 0x00102C3F */
    FLASH_DF_BLOCK_177     = 0x00102C40,    /*   64B: 0x00102C40 - 0x00102C7F */
    FLASH_DF_BLOCK_178     = 0x00102C80,    /*   64B: 0x00102C80 - 0x00102CBF */
    FLASH_DF_BLOCK_179     = 0x00102CC0,    /*   64B: 0x00102CC0 - 0x00102CFF */
    FLASH_DF_BLOCK_180     = 0x00102D00,    /*   64B: 0x00102D00 - 0x00102D3F */
    FLASH_DF_BLOCK_181     = 0x00102D40,    /*   64B: 0x00102D40 - 0x00102D7F */
    FLASH_DF_BLOCK_182     = 0x00102D80,    /*   64B: 0x00102D80 - 0x00102DBF */
    FLASH_DF_BLOCK_183     = 0x00102DC0,    /*   64B: 0x00102DC0 - 0x00102DFF */
    FLASH_DF_BLOCK_184     = 0x00102E00,    /*   64B: 0x00102E00 - 0x00102E3F */
    FLASH_DF_BLOCK_185     = 0x00102E40,    /*   64B: 0x00102E40 - 0x00102E7F */
    FLASH_DF_BLOCK_186     = 0x00102E80,    /*   64B: 0x00102E80 - 0x00102EBF */
    FLASH_DF_BLOCK_187     = 0x00102EC0,    /*   64B: 0x00102EC0 - 0x00102EFF */
    FLASH_DF_BLOCK_188     = 0x00102F00,    /*   64B: 0x00102F00 - 0x00102F3F */
    FLASH_DF_BLOCK_189     = 0x00102F40,    /*   64B: 0x00102F40 - 0x00102F7F */
    FLASH_DF_BLOCK_190     = 0x00102F80,    /*   64B: 0x00102F80 - 0x00102FBF */
    FLASH_DF_BLOCK_191     = 0x00102FC0,    /*   64B: 0x00102FC0 - 0x00102FFF */
    FLASH_DF_BLOCK_192     = 0x00103000,    /*   64B: 0x00103000 - 0x0010303F */
    FLASH_DF_BLOCK_193     = 0x00103040,    /*   64B: 0x00103040 - 0x0010307F */
    FLASH_DF_BLOCK_194     = 0x00103080,    /*   64B: 0x00103080 - 0x001030BF */
    FLASH_DF_BLOCK_195     = 0x001030C0,    /*   64B: 0x001030C0 - 0x001030FF */
    FLASH_DF_BLOCK_196     = 0x00103100,    /*   64B: 0x00103100 - 0x0010313F */
    FLASH_DF_BLOCK_197     = 0x00103140,    /*   64B: 0x00103140 - 0x0010317F */
    FLASH_DF_BLOCK_198     = 0x00103180,    /*   64B: 0x00103180 - 0x001031BF */
    FLASH_DF_BLOCK_199     = 0x001031C0,    /*   64B: 0x001031C0 - 0x001031FF */
    FLASH_DF_BLOCK_200     = 0x00103200,    /*   64B: 0x00103200 - 0x0010323F */
    FLASH_DF_BLOCK_201     = 0x00103240,    /*   64B: 0x00103240 - 0x0010327F */
    FLASH_DF_BLOCK_202     = 0x00103280,    /*   64B: 0x00103280 - 0x001032BF */
    FLASH_DF_BLOCK_203     = 0x001032C0,    /*   64B: 0x001032C0 - 0x001032FF */
    FLASH_DF_BLOCK_204     = 0x00103300,    /*   64B: 0x00103300 - 0x0010333F */
    FLASH_DF_BLOCK_205     = 0x00103340,    /*   64B: 0x00103340 - 0x0010337F */
    FLASH_DF_BLOCK_206     = 0x00103380,    /*   64B: 0x00103380 - 0x001033BF */
    FLASH_DF_BLOCK_207     = 0x001033C0,    /*   64B: 0x001033C0 - 0x001033FF */
    FLASH_DF_BLOCK_208     = 0x00103400,    /*   64B: 0x00103400 - 0x0010343F */
    FLASH_DF_BLOCK_209     = 0x00103440,    /*   64B: 0x00103440 - 0x0010347F */
    FLASH_DF_BLOCK_210     = 0x00103480,    /*   64B: 0x00103480 - 0x001034BF */
    FLASH_DF_BLOCK_211     = 0x001034C0,    /*   64B: 0x001034C0 - 0x001034FF */
    FLASH_DF_BLOCK_212     = 0x00103500,    /*   64B: 0x00103500 - 0x0010353F */
    FLASH_DF_BLOCK_213     = 0x00103540,    /*   64B: 0x00103540 - 0x0010357F */
    FLASH_DF_BLOCK_214     = 0x00103580,    /*   64B: 0x00103580 - 0x001035BF */
    FLASH_DF_BLOCK_215     = 0x001035C0,    /*   64B: 0x001035C0 - 0x001035FF */
    FLASH_DF_BLOCK_216     = 0x00103600,    /*   64B: 0x00103600 - 0x0010363F */
    FLASH_DF_BLOCK_217     = 0x00103640,    /*   64B: 0x00103640 - 0x0010367F */
    FLASH_DF_BLOCK_218     = 0x00103680,    /*   64B: 0x00103680 - 0x001036BF */
    FLASH_DF_BLOCK_219     = 0x001036C0,    /*   64B: 0x001036C0 - 0x001036FF */
    FLASH_DF_BLOCK_220     = 0x00103700,    /*   64B: 0x00103700 - 0x0010373F */
    FLASH_DF_BLOCK_221     = 0x00103740,    /*   64B: 0x00103740 - 0x0010377F */
    FLASH_DF_BLOCK_222     = 0x00103780,    /*   64B: 0x00103780 - 0x001037BF */
    FLASH_DF_BLOCK_223     = 0x001037C0,    /*   64B: 0x001037C0 - 0x001037FF */
    FLASH_DF_BLOCK_224     = 0x00103800,    /*   64B: 0x00103800 - 0x0010383F */
    FLASH_DF_BLOCK_225     = 0x00103840,    /*   64B: 0x00103840 - 0x0010387F */
    FLASH_DF_BLOCK_226     = 0x00103880,    /*   64B: 0x00103880 - 0x001038BF */
    FLASH_DF_BLOCK_227     = 0x001038C0,    /*   64B: 0x001038C0 - 0x001038FF */
    FLASH_DF_BLOCK_228     = 0x00103900,    /*   64B: 0x00103900 - 0x0010393F */
    FLASH_DF_BLOCK_229     = 0x00103940,    /*   64B: 0x00103940 - 0x0010397F */
    FLASH_DF_BLOCK_230     = 0x00103980,    /*   64B: 0x00103980 - 0x001039BF */
    FLASH_DF_BLOCK_231     = 0x001039C0,    /*   64B: 0x001039C0 - 0x001039FF */
    FLASH_DF_BLOCK_232     = 0x00103A00,    /*   64B: 0x00103A00 - 0x00103A3F */
    FLASH_DF_BLOCK_233     = 0x00103A40,    /*   64B: 0x00103A40 - 0x00103A7F */
    FLASH_DF_BLOCK_234     = 0x00103A80,    /*   64B: 0x00103A80 - 0x00103ABF */
    FLASH_DF_BLOCK_235     = 0x00103AC0,    /*   64B: 0x00103AC0 - 0x00103AFF */
    FLASH_DF_BLOCK_236     = 0x00103B00,    /*   64B: 0x00103B00 - 0x00103B3F */
    FLASH_DF_BLOCK_237     = 0x00103B40,    /*   64B: 0x00103B40 - 0x00103B7F */
    FLASH_DF_BLOCK_238     = 0x00103B80,    /*   64B: 0x00103B80 - 0x00103BBF */
    FLASH_DF_BLOCK_239     = 0x00103BC0,    /*   64B: 0x00103BC0 - 0x00103BFF */
    FLASH_DF_BLOCK_240     = 0x00103C00,    /*   64B: 0x00103C00 - 0x00103C3F */
    FLASH_DF_BLOCK_241     = 0x00103C40,    /*   64B: 0x00103C40 - 0x00103C7F */
    FLASH_DF_BLOCK_242     = 0x00103C80,    /*   64B: 0x00103C80 - 0x00103CBF */
    FLASH_DF_BLOCK_243     = 0x00103CC0,    /*   64B: 0x00103CC0 - 0x00103CFF */
    FLASH_DF_BLOCK_244     = 0x00103D00,    /*   64B: 0x00103D00 - 0x00103D3F */
    FLASH_DF_BLOCK_245     = 0x00103D40,    /*   64B: 0x00103D40 - 0x00103D7F */
    FLASH_DF_BLOCK_246     = 0x00103D80,    /*   64B: 0x00103D80 - 0x00103DBF */
    FLASH_DF_BLOCK_247     = 0x00103DC0,    /*   64B: 0x00103DC0 - 0x00103DFF */
    FLASH_DF_BLOCK_248     = 0x00103E00,    /*   64B: 0x00103E00 - 0x00103E3F */
    FLASH_DF_BLOCK_249     = 0x00103E40,    /*   64B: 0x00103E40 - 0x00103E7F */
    FLASH_DF_BLOCK_250     = 0x00103E80,    /*   64B: 0x00103E80 - 0x00103EBF */
    FLASH_DF_BLOCK_251     = 0x00103EC0,    /*   64B: 0x00103EC0 - 0x00103EFF */
    FLASH_DF_BLOCK_252     = 0x00103F00,    /*   64B: 0x00103F00 - 0x00103F3F */
    FLASH_DF_BLOCK_253     = 0x00103F40,    /*   64B: 0x00103F40 - 0x00103F7F */
    FLASH_DF_BLOCK_254     = 0x00103F80,    /*   64B: 0x00103F80 - 0x00103FBF */
    FLASH_DF_BLOCK_255     = 0x00103FC0,    /*   64B: 0x00103FC0 - 0x00103FFF */
    FLASH_DF_BLOCK_INVALID = FLASH_DF_BLOCK_0 + (FLASH_DF_BLOCK_SIZE * FLASH_NUM_BLOCKS_DF)
} flash_block_address_t;


R_BSP_PRAGMA_UNPACK
typedef union
{
    unsigned long LONG;
    R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_6(
        unsigned long BTFLG:1,
        unsigned long :2,
        unsigned long FAWE:13,
        unsigned long FSPR:1,
        unsigned long :2,
        unsigned long FAWS:13
    ) BIT;
} fawreg_t;
R_BSP_PRAGMA_PACKOPTION


/*  According to HW Manual the Max Programming Time for 128 bytes (ROM)
    is 13ms.  This is with a FCLK of 4 MHz. The calculation below
    calculates the number of ICLK ticks needed for the timeout delay.
    The 13ms number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_ROM_WRITE \
        ((int32_t)(13000 * (4.0/(MCU_CFG_FCLK_HZ/1000000)))*(MCU_CFG_ICLK_HZ/1000000))

/*  According to HW Manual the Max Programming Time for 4 bytes
    (Data Flash) is 3.8ms.  This is with a FCLK of 4MHz. The calculation
    below calculates the number of ICLK ticks needed for the timeout delay.
    The 3.8ms number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_DF_WRITE \
        ((int32_t)(3800 * (4.0/(MCU_CFG_FCLK_HZ/1000000)))*(MCU_CFG_ICLK_HZ/1000000))

/*  According to HW Manual the Max Blank Check time for 4 bytes
    (Data Flash) is 84 usec.  This is with a FCLK of 4MHz. The calculation
    below calculates the number of ICLK ticks needed for the timeout delay.
    The 84 usec number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_BLANK_CHECK \
        ((int32_t)(84 * (4.0/(MCU_CFG_FCLK_HZ/1000000)))*(MCU_CFG_ICLK_HZ/1000000))

/*  According to HW Manual the max timeout value when using the peripheral
    clock notification command is 60us. This is with a FCLK of 50MHz. The
    calculation below calculates the number of ICLK ticks needed for the
    timeout delay. The 60us number is adjusted linearly depending on
    the FCLK frequency.
*/
#define WAIT_MAX_NOTIFY_FCU_CLOCK \
        ((int32_t)(60 * (50.0/(MCU_CFG_FCLK_HZ/1000000)))*(MCU_CFG_ICLK_HZ/1000000))

/*  According to HW Manual the Max Erasure Time for a 16KB block is
    around 864ms.  This is with a FCLK of 4 MHz. The calculation below
    calculates the number of ICLK ticks needed for the timeout delay.
    The 864ms number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_ERASE_CF_16K \
        ((int32_t)(864000 * (4.0/(MCU_CFG_FCLK_HZ/1000000)))*(MCU_CFG_ICLK_HZ/1000000))
#define WAIT_MAX_ERASE_CF_MEDIUM WAIT_MAX_ERASE_CF_16K

/*  According to HW Manual the Max Erasure Time for a 4KB block is
    around 130ms.  This is with a FCLK of 4 MHz. The calculation below
    calculates the number of ICLK ticks needed for the timeout delay.
    The 130ms number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_ERASE_CF_4K \
        ((int32_t)(130000 * (4.0/(MCU_CFG_FCLK_HZ/1000000)))*(MCU_CFG_ICLK_HZ/1000000))
#define WAIT_MAX_ERASE_CF_SMALL WAIT_MAX_ERASE_CF_4K

/*  According to HW Manual the Max Erasure Time for a 64 byte Data Flash block is
    around 18ms.  This is with a FCLK of 4MHz. The calculation below
    calculates the number of ICLK ticks needed for the timeout delay.
    The 10ms number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_ERASE_DF \
        ((int32_t)(18000 * (4.0/(MCU_CFG_FCLK_HZ/1000000)))*(MCU_CFG_ICLK_HZ/1000000))

/******************************************************************************
Error checking
******************************************************************************/
// frequency range checking moved to flash_api_open()

#endif/* R_FLASH_RX26T_H */
