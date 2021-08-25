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
* Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_flash_rx671.h
* Description  : This is a private header file used internally by the FLASH API module. It should not be modified or
*                included by the user in their application.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 23.10.2020 4.70    First Release
***********************************************************************************************************************/

#ifndef R_FLASH_RX671_H
#define R_FLASH_RX671_H

#include "r_flash_targets.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#if (MCU_CFG_PART_MEMORY_SIZE == 0x9 )      // 1 Mb
    #if FLASH_IN_DUAL_BANK_MODE
        #define FLASH_NUM_BLOCKS_CF (14+8)  // 0.5 Mb per bank dual mode
    #else
        #define FLASH_NUM_BLOCKS_CF (30+8)  // 1 Mb linear mode
    #endif
#elif (MCU_CFG_PART_MEMORY_SIZE == 0xC)     // 1.5 Mb
    #if FLASH_IN_DUAL_BANK_MODE
        #define FLASH_NUM_BLOCKS_CF (22+8)  // 0.75 Mb per bank dual mode
    #else
        #define FLASH_NUM_BLOCKS_CF (46+8)  // 1.5 Mb linear mode
    #endif
#elif (MCU_CFG_PART_MEMORY_SIZE == 0xE)     // 2 Mb
    #if FLASH_IN_DUAL_BANK_MODE
        #define FLASH_NUM_BLOCKS_CF (30+8)  // 1 Mb per bank dual mode
    #else
        #define FLASH_NUM_BLOCKS_CF (62+8)  // 2 Mb linear mode
    #endif
#endif


#define FLASH_NUM_BLOCKS_DF                 (128)
#define FLASH_DF_MIN_PGM_SIZE               (4)
#define FLASH_CF_MIN_PGM_SIZE               (128)

#define FLASH_CF_SMALL_BLOCK_SIZE           (8192)
#define FLASH_CF_MEDIUM_BLOCK_SIZE          (32768)
#define FLASH_CF_LO_BANK_SMALL_BLOCK_ADDR   (FLASH_CF_BLOCK_45)
#define FLASH_CF_LOWEST_VALID_BLOCK         (FLASH_CF_BLOCK_INVALID + 1)
#define FLASH_DF_BLOCK_SIZE                 (64)
#define FLASH_DF_HIGHEST_VALID_BLOCK        (FLASH_DF_BLOCK_INVALID - FLASH_DF_BLOCK_SIZE)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum _flash_block_address
{
#ifndef FLASH_IN_DUAL_BANK_MODE                                              /* LINEAR MODE */
    FLASH_CF_BLOCK_END     = 0xFFFFFFFF,    /*   End of Code Flash Area       */
    FLASH_CF_BLOCK_0       = 0xFFFFE000,    /*   8KB: 0xFFFFE000 - 0xFFFFFFFF */
    FLASH_CF_BLOCK_1       = 0xFFFFC000,    /*   8KB: 0xFFFFC000 - 0xFFFFDFFF */
    FLASH_CF_BLOCK_2       = 0xFFFFA000,    /*   8KB: 0xFFFFA000 - 0xFFFFBFFF */
    FLASH_CF_BLOCK_3       = 0xFFFF8000,    /*   8KB: 0xFFFF8000 - 0xFFFF9FFF */
    FLASH_CF_BLOCK_4       = 0xFFFF6000,    /*   8KB: 0xFFFF6000 - 0xFFFF7FFF */
    FLASH_CF_BLOCK_5       = 0xFFFF4000,    /*   8KB: 0xFFFF4000 - 0xFFFF5FFF */
    FLASH_CF_BLOCK_6       = 0xFFFF2000,    /*   8KB: 0xFFFF2000 - 0xFFFF3FFF */
    FLASH_CF_BLOCK_7       = 0xFFFF0000,    /*   8KB: 0xFFFF0000 - 0xFFFF1FFF */
    FLASH_CF_BLOCK_8       = 0xFFFE8000,    /*  32KB: 0xFFFE8000 - 0xFFFEFFFF */
    FLASH_CF_BLOCK_9       = 0xFFFE0000,    /*  32KB: 0xFFFE0000 - 0xFFFE7FFF */
    FLASH_CF_BLOCK_10      = 0xFFFD8000,    /*  32KB: 0xFFFD8000 - 0xFFFDFFFF */
    FLASH_CF_BLOCK_11      = 0xFFFD0000,    /*  32KB: 0xFFFD0000 - 0xFFFD7FFF */
    FLASH_CF_BLOCK_12      = 0xFFFC8000,    /*  32KB: 0xFFFC8000 - 0xFFFCFFFF */
    FLASH_CF_BLOCK_13      = 0xFFFC0000,    /*  32KB: 0xFFFC0000 - 0xFFFC7FFF */
    FLASH_CF_BLOCK_14      = 0xFFFB8000,    /*  32KB: 0xFFFB8000 - 0xFFFBFFFF */
    FLASH_CF_BLOCK_15      = 0xFFFB0000,    /*  32KB: 0xFFFB0000 - 0xFFFB7FFF */
    FLASH_CF_BLOCK_16      = 0xFFFA8000,    /*  32KB: 0xFFFA8000 - 0xFFFAFFFF */
    FLASH_CF_BLOCK_17      = 0xFFFA0000,    /*  32KB: 0xFFFA0000 - 0xFFFA7FFF */
    FLASH_CF_BLOCK_18      = 0xFFF98000,    /*  32KB: 0xFFF98000 - 0xFFF9FFFF */
    FLASH_CF_BLOCK_19      = 0xFFF90000,    /*  32KB: 0xFFF90000 - 0xFFF97FFF */
    FLASH_CF_BLOCK_20      = 0xFFF88000,    /*  32KB: 0xFFF88000 - 0xFFF8FFFF */
    FLASH_CF_BLOCK_21      = 0xFFF80000,    /*  32KB: 0xFFF80000 - 0xFFF87FFF */
    FLASH_CF_BLOCK_22      = 0xFFF78000,    /*  32KB: 0xFFF78000 - 0xFFF7FFFF */
    FLASH_CF_BLOCK_23      = 0xFFF70000,    /*  32KB: 0xFFF70000 - 0xFFF77FFF */
    FLASH_CF_BLOCK_24      = 0xFFF68000,    /*  32KB: 0xFFF68000 - 0xFFF6FFFF */
    FLASH_CF_BLOCK_25      = 0xFFF60000,    /*  32KB: 0xFFF60000 - 0xFFF67FFF */
    FLASH_CF_BLOCK_26      = 0xFFF58000,    /*  32KB: 0xFFF58000 - 0xFFF5FFFF */
    FLASH_CF_BLOCK_27      = 0xFFF50000,    /*  32KB: 0xFFF50000 - 0xFFF57FFF */
    FLASH_CF_BLOCK_28      = 0xFFF48000,    /*  32KB: 0xFFF48000 - 0xFFF4FFFF */
    FLASH_CF_BLOCK_29      = 0xFFF40000,    /*  32KB: 0xFFF40000 - 0xFFF47FFF */
    FLASH_CF_BLOCK_30      = 0xFFF38000,    /*  32KB: 0xFFF38000 - 0xFFF3FFFF */
    FLASH_CF_BLOCK_31      = 0xFFF30000,    /*  32KB: 0xFFF30000 - 0xFFF37FFF */
    FLASH_CF_BLOCK_32      = 0xFFF28000,    /*  32KB: 0xFFF28000 - 0xFFF2FFFF */
    FLASH_CF_BLOCK_33      = 0xFFF20000,    /*  32KB: 0xFFF20000 - 0xFFF27FFF */
    FLASH_CF_BLOCK_34      = 0xFFF18000,    /*  32KB: 0xFFF18000 - 0xFFF1FFFF */
    FLASH_CF_BLOCK_35      = 0xFFF10000,    /*  32KB: 0xFFF10000 - 0xFFF17FFF */
    FLASH_CF_BLOCK_36      = 0xFFF08000,    /*  32KB: 0xFFF08000 - 0xFFF0FFFF */
    FLASH_CF_BLOCK_37      = 0xFFF00000,    /*  32KB: 0xFFF00000 - 0xFFF07FFF */
#if   MCU_CFG_PART_MEMORY_SIZE == 0x09  /*   '9' parts 1 Mb ROM */
    FLASH_CF_BLOCK_INVALID = (FLASH_CF_BLOCK_37 - 1),
#else
    FLASH_CF_BLOCK_38      = 0xFFEF8000,    /*  32KB: 0xFFEF8000 - 0xFFEFFFFF */
    FLASH_CF_BLOCK_39      = 0xFFEF0000,    /*  32KB: 0xFFEF0000 - 0xFFEF7FFF */
    FLASH_CF_BLOCK_40      = 0xFFEE8000,    /*  32KB: 0xFFEE8000 - 0xFFEEFFFF */
    FLASH_CF_BLOCK_41      = 0xFFEE0000,    /*  32KB: 0xFFEE0000 - 0xFFEE7FFF */
    FLASH_CF_BLOCK_42      = 0xFFED8000,    /*  32KB: 0xFFED8000 - 0xFFEDFFFF */
    FLASH_CF_BLOCK_43      = 0xFFED0000,    /*  32KB: 0xFFED0000 - 0xFFED7FFF */
    FLASH_CF_BLOCK_44      = 0xFFEC8000,    /*  32KB: 0xFFEC8000 - 0xFFECFFFF */
    FLASH_CF_BLOCK_45      = 0xFFEC0000,    /*  32KB: 0xFFEC0000 - 0xFFEC7FFF */
    FLASH_CF_BLOCK_46      = 0xFFEB8000,    /*  32KB: 0xFFEB8000 - 0xFFEBFFFF */
    FLASH_CF_BLOCK_47      = 0xFFEB0000,    /*  32KB: 0xFFEB0000 - 0xFFEB7FFF */
    FLASH_CF_BLOCK_48      = 0xFFEA8000,    /*  32KB: 0xFFEA8000 - 0xFFEAFFFF */
    FLASH_CF_BLOCK_49      = 0xFFEA0000,    /*  32KB: 0xFFEA0000 - 0xFFEA7FFF */
    FLASH_CF_BLOCK_50      = 0xFFE98000,    /*  32KB: 0xFFE98000 - 0xFFE9FFFF */
    FLASH_CF_BLOCK_51      = 0xFFE90000,    /*  32KB: 0xFFE90000 - 0xFFE97FFF */
    FLASH_CF_BLOCK_52      = 0xFFE88000,    /*  32KB: 0xFFE88000 - 0xFFE8FFFF */
    FLASH_CF_BLOCK_53      = 0xFFE80000,    /*  32KB: 0xFFE80000 - 0xFFE87FFF */
#if   MCU_CFG_PART_MEMORY_SIZE == 0x0C  /*   'C' parts 1.5 Mb ROM */
    FLASH_CF_BLOCK_INVALID = (FLASH_CF_BLOCK_53 - 1),
#else
    FLASH_CF_BLOCK_54      = 0xFFE78000,    /*  32KB: 0xFFE78000 - 0xFFE7FFFF */
    FLASH_CF_BLOCK_55      = 0xFFE70000,    /*  32KB: 0xFFE70000 - 0xFFE77FFF */
    FLASH_CF_BLOCK_56      = 0xFFE68000,    /*  32KB: 0xFFE68000 - 0xFFE6FFFF */
    FLASH_CF_BLOCK_57      = 0xFFE60000,    /*  32KB: 0xFFE60000 - 0xFFE67FFF */
    FLASH_CF_BLOCK_58      = 0xFFE58000,    /*  32KB: 0xFFE58000 - 0xFFE5FFFF */
    FLASH_CF_BLOCK_59      = 0xFFE50000,    /*  32KB: 0xFFE50000 - 0xFFE57FFF */
    FLASH_CF_BLOCK_60      = 0xFFE48000,    /*  32KB: 0xFFE48000 - 0xFFE4FFFF */
    FLASH_CF_BLOCK_61      = 0xFFE40000,    /*  32KB: 0xFFE40000 - 0xFFE47FFF */
    FLASH_CF_BLOCK_62      = 0xFFE38000,    /*  32KB: 0xFFE38000 - 0xFFE3FFFF */
    FLASH_CF_BLOCK_63      = 0xFFE30000,    /*  32KB: 0xFFE30000 - 0xFFE37FFF */
    FLASH_CF_BLOCK_64      = 0xFFE28000,    /*  32KB: 0xFFE28000 - 0xFFE2FFFF */
    FLASH_CF_BLOCK_65      = 0xFFE20000,    /*  32KB: 0xFFE20000 - 0xFFE27FFF */
    FLASH_CF_BLOCK_66      = 0xFFE18000,    /*  32KB: 0xFFE18000 - 0xFFE1FFFF */
    FLASH_CF_BLOCK_67      = 0xFFE10000,    /*  32KB: 0xFFE10000 - 0xFFE17FFF */
    FLASH_CF_BLOCK_68      = 0xFFE08000,    /*  32KB: 0xFFE08000 - 0xFFE0FFFF */
    FLASH_CF_BLOCK_69      = 0xFFE00000,    /*  32KB: 0xFFE00000 - 0xFFE07FFF */
    FLASH_CF_BLOCK_INVALID = (FLASH_CF_BLOCK_69 - 1), /* 'E' parts 2 Mb ROM */
#endif // > 1.5M
#endif // > 1M

#else                                       /* DUAL MODE */
    FLASH_CF_BLOCK_END     = 0xFFFFFFFF,    /*   End of Code Flash Area       */
    FLASH_CF_HI_BANK_HI_ADDR = FLASH_CF_BLOCK_END,
    FLASH_CF_BLOCK_0       = 0xFFFFE000,    /*   8KB: 0xFFFFE000 - 0xFFFFFFFF */
    FLASH_CF_BLOCK_1       = 0xFFFFC000,    /*   8KB: 0xFFFFC000 - 0xFFFFDFFF */
    FLASH_CF_BLOCK_2       = 0xFFFFA000,    /*   8KB: 0xFFFFA000 - 0xFFFFBFFF */
    FLASH_CF_BLOCK_3       = 0xFFFF8000,    /*   8KB: 0xFFFF8000 - 0xFFFF9FFF */
    FLASH_CF_BLOCK_4       = 0xFFFF6000,    /*   8KB: 0xFFFF6000 - 0xFFFF7FFF */
    FLASH_CF_BLOCK_5       = 0xFFFF4000,    /*   8KB: 0xFFFF4000 - 0xFFFF5FFF */
    FLASH_CF_BLOCK_6       = 0xFFFF2000,    /*   8KB: 0xFFFF2000 - 0xFFFF3FFF */
    FLASH_CF_BLOCK_7       = 0xFFFF0000,    /*   8KB: 0xFFFF0000 - 0xFFFF1FFF */
    FLASH_CF_BLOCK_8       = 0xFFFE8000,    /*  32KB: 0xFFFE8000 - 0xFFFEFFFF */
    FLASH_CF_BLOCK_9       = 0xFFFE0000,    /*  32KB: 0xFFFE0000 - 0xFFFE7FFF */
    FLASH_CF_BLOCK_10      = 0xFFFD8000,    /*  32KB: 0xFFFD8000 - 0xFFFDFFFF */
    FLASH_CF_BLOCK_11      = 0xFFFD0000,    /*  32KB: 0xFFFD0000 - 0xFFFD7FFF */
    FLASH_CF_BLOCK_12      = 0xFFFC8000,    /*  32KB: 0xFFFC8000 - 0xFFFCFFFF */
    FLASH_CF_BLOCK_13      = 0xFFFC0000,    /*  32KB: 0xFFFC0000 - 0xFFFC7FFF */
    FLASH_CF_BLOCK_14      = 0xFFFB8000,    /*  32KB: 0xFFFB8000 - 0xFFFBFFFF */
    FLASH_CF_BLOCK_15      = 0xFFFB0000,    /*  32KB: 0xFFFB0000 - 0xFFFB7FFF */
    FLASH_CF_BLOCK_16      = 0xFFFA8000,    /*  32KB: 0xFFFA8000 - 0xFFFAFFFF */
    FLASH_CF_BLOCK_17      = 0xFFFA0000,    /*  32KB: 0xFFFA0000 - 0xFFFA7FFF */
    FLASH_CF_BLOCK_18      = 0xFFF98000,    /*  32KB: 0xFFF98000 - 0xFFF9FFFF */
    FLASH_CF_BLOCK_19      = 0xFFF90000,    /*  32KB: 0xFFF90000 - 0xFFF97FFF */
    FLASH_CF_BLOCK_20      = 0xFFF88000,    /*  32KB: 0xFFF88000 - 0xFFF8FFFF */
    FLASH_CF_BLOCK_21      = 0xFFF80000,    /*  32KB: 0xFFF80000 - 0xFFF87FFF */
#if   MCU_CFG_PART_MEMORY_SIZE == 0x09  /*   bank end for '9' parts (1 Mb/bank) */
    FLASH_CF_HI_BANK_LO_ADDR = FLASH_CF_BLOCK_21,
#else
    FLASH_CF_BLOCK_22      = 0xFFF78000,    /*  32KB: 0xFFF78000 - 0xFFF7FFFF */
    FLASH_CF_BLOCK_23      = 0xFFF70000,    /*  32KB: 0xFFF70000 - 0xFFF77FFF */
    FLASH_CF_BLOCK_24      = 0xFFF68000,    /*  32KB: 0xFFF68000 - 0xFFF6FFFF */
    FLASH_CF_BLOCK_25      = 0xFFF60000,    /*  32KB: 0xFFF60000 - 0xFFF67FFF */
    FLASH_CF_BLOCK_26      = 0xFFF58000,    /*  32KB: 0xFFF58000 - 0xFFF5FFFF */
    FLASH_CF_BLOCK_27      = 0xFFF50000,    /*  32KB: 0xFFF50000 - 0xFFF57FFF */
    FLASH_CF_BLOCK_28      = 0xFFF48000,    /*  32KB: 0xFFF48000 - 0xFFF4FFFF */
    FLASH_CF_BLOCK_29      = 0xFFF40000,    /*  32KB: 0xFFF40000 - 0xFFF47FFF */
#if   MCU_CFG_PART_MEMORY_SIZE == 0x0C  /*   bank end for 'C' parts (1.5 Mb/bank) */
    FLASH_CF_HI_BANK_LO_ADDR = FLASH_CF_BLOCK_29,
#else
    FLASH_CF_BLOCK_30      = 0xFFF38000,    /*  32KB: 0xFFF38000 - 0xFFF3FFFF */
    FLASH_CF_BLOCK_31      = 0xFFF30000,    /*  32KB: 0xFFF30000 - 0xFFF37FFF */
    FLASH_CF_BLOCK_32      = 0xFFF28000,    /*  32KB: 0xFFF28000 - 0xFFF2FFFF */
    FLASH_CF_BLOCK_33      = 0xFFF20000,    /*  32KB: 0xFFF20000 - 0xFFF27FFF */
    FLASH_CF_BLOCK_34      = 0xFFF18000,    /*  32KB: 0xFFF18000 - 0xFFF1FFFF */
    FLASH_CF_BLOCK_35      = 0xFFF10000,    /*  32KB: 0xFFF10000 - 0xFFF17FFF */
    FLASH_CF_BLOCK_36      = 0xFFF08000,    /*  32KB: 0xFFF08000 - 0xFFF0FFFF */
    FLASH_CF_BLOCK_37      = 0xFFF00000,    /*  32KB: 0xFFF00000 - 0xFFF07FFF */
    FLASH_CF_HI_BANK_LO_ADDR = FLASH_CF_BLOCK_37,
#endif
#endif
    FLASH_CF_LO_BANK_HI_ADDR = 0xFFEFFFFF,  /* START OF NEXT BANK */

    FLASH_CF_BLOCK_38      = 0xFFEFE000,    /*   8KB: 0xFFEFE000 - 0xFFEFFFFF */
    FLASH_CF_BLOCK_39      = 0xFFEFC000,    /*   8KB: 0xFFEFC000 - 0xFFEFDFFF */
    FLASH_CF_BLOCK_40      = 0xFFEFA000,    /*   8KB: 0xFFEFA000 - 0xFFEEBFFF */
    FLASH_CF_BLOCK_41      = 0xFFEF8000,    /*   8KB: 0xFFEF8000 - 0xFFEF9FFF */
    FLASH_CF_BLOCK_42      = 0xFFEF6000,    /*   8KB: 0xFFEF6000 - 0xFFEF7FFF */
    FLASH_CF_BLOCK_43      = 0xFFEF4000,    /*   8KB: 0xFFEF4000 - 0xFFEF5FFF */
    FLASH_CF_BLOCK_44      = 0xFFEF2000,    /*   8KB: 0xFFEF2000 - 0xFFEF3FFF */
    FLASH_CF_BLOCK_45      = 0xFFEF0000,    /*   8KB: 0xFFEF0000 - 0xFFEF1FFF */
    FLASH_CF_BLOCK_46      = 0xFFEE8000,    /*  32KB: 0xFFEE8000 - 0xFFEEFFFF */
    FLASH_CF_BLOCK_47      = 0xFFEE0000,    /*  32KB: 0xFFEE0000 - 0xFFEE7FFF */
    FLASH_CF_BLOCK_48      = 0xFFED8000,    /*  32KB: 0xFFED8000 - 0xFFEDFFFF */
    FLASH_CF_BLOCK_49      = 0xFFED0000,    /*  32KB: 0xFFED0000 - 0xFFED7FFF */
    FLASH_CF_BLOCK_50      = 0xFFEC8000,    /*  32KB: 0xFFEC8000 - 0xFFECFFFF */
    FLASH_CF_BLOCK_51      = 0xFFEC0000,    /*  32KB: 0xFFEC0000 - 0xFFEC7FFF */
    FLASH_CF_BLOCK_52      = 0xFFEB8000,    /*  32KB: 0xFFEB8000 - 0xFFEBFFFF */
    FLASH_CF_BLOCK_53      = 0xFFEB0000,    /*  32KB: 0xFFEB0000 - 0xFFEB7FFF */
    FLASH_CF_BLOCK_54      = 0xFFEA8000,    /*  32KB: 0xFFEA8000 - 0xFFEAFFFF */
    FLASH_CF_BLOCK_55      = 0xFFEA0000,    /*  32KB: 0xFFEA0000 - 0xFFEA7FFF */
    FLASH_CF_BLOCK_56      = 0xFFE98000,    /*  32KB: 0xFFE98000 - 0xFFE9FFFF */
    FLASH_CF_BLOCK_57      = 0xFFE90000,    /*  32KB: 0xFFE90000 - 0xFFE97FFF */
    FLASH_CF_BLOCK_58      = 0xFFE88000,    /*  32KB: 0xFFE88000 - 0xFFE8FFFF */
    FLASH_CF_BLOCK_59      = 0xFFE80000,    /*  32KB: 0xFFE80000 - 0xFFE87FFF */
#if   MCU_CFG_PART_MEMORY_SIZE == 0x09  /*   bank end for '9' parts (1 Mb/bank) */
    FLASH_CF_HI_BANK_LO_ADDR = FLASH_CF_BLOCK_59,
#else
    FLASH_CF_BLOCK_60      = 0xFFE78000,    /*  32KB: 0xFFE78000 - 0xFFE7FFFF */
    FLASH_CF_BLOCK_61      = 0xFFE70000,    /*  32KB: 0xFFE70000 - 0xFFE77FFF */
    FLASH_CF_BLOCK_62      = 0xFFE68000,    /*  32KB: 0xFFE68000 - 0xFFE6FFFF */
    FLASH_CF_BLOCK_63      = 0xFFE60000,    /*  32KB: 0xFFE60000 - 0xFFE67FFF */
    FLASH_CF_BLOCK_64      = 0xFFE58000,    /*  32KB: 0xFFE58000 - 0xFFE5FFFF */
    FLASH_CF_BLOCK_65      = 0xFFE50000,    /*  32KB: 0xFFE50000 - 0xFFE57FFF */
    FLASH_CF_BLOCK_66      = 0xFFE48000,    /*  32KB: 0xFFE48000 - 0xFFE4FFFF */
    FLASH_CF_BLOCK_67      = 0xFFE40000,    /*  32KB: 0xFFE40000 - 0xFFE47FFF */
#if   MCU_CFG_PART_MEMORY_SIZE == 0x0C  /*   bank end for 'C' parts (1.5 Mb/bank) */
    FLASH_CF_LO_BANK_LO_ADDR = FLASH_CF_BLOCK_67,
    FLASH_CF_BLOCK_INVALID = (FLASH_CF_BLOCK_67 - 1),
#else
    FLASH_CF_BLOCK_68      = 0xFFE38000,    /*  32KB: 0xFFE38000 - 0xFFE3FFFF */
    FLASH_CF_BLOCK_69      = 0xFFE30000,    /*  32KB: 0xFFE30000 - 0xFFE37FFF */
    FLASH_CF_BLOCK_70      = 0xFFE28000,    /*  32KB: 0xFFE28000 - 0xFFE2FFFF */
    FLASH_CF_BLOCK_71      = 0xFFE20000,    /*  32KB: 0xFFE20000 - 0xFFE27FFF */
    FLASH_CF_BLOCK_72      = 0xFFE18000,    /*  32KB: 0xFFE18000 - 0xFFE1FFFF */
    FLASH_CF_BLOCK_73      = 0xFFE10000,    /*  32KB: 0xFFE10000 - 0xFFE17FFF */
    FLASH_CF_BLOCK_74      = 0xFFE08000,    /*  32KB: 0xFFE08000 - 0xFFE0FFFF */
    FLASH_CF_BLOCK_75      = 0xFFE00000,    /*  32KB: 0xFFE00000 - 0xFFE07FFF */
    FLASH_CF_LO_BANK_LO_ADDR = FLASH_CF_BLOCK_75,
    FLASH_CF_BLOCK_INVALID = (FLASH_CF_BLOCK_75 - 1),
#endif // > 1.5M
#endif // > 1M
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
    FLASH_DF_BLOCK_INVALID = FLASH_DF_BLOCK_0 + (FLASH_DF_BLOCK_SIZE * FLASH_NUM_BLOCKS_DF)
} flash_block_address_t;


R_BSP_PRAGMA_UNPACK
typedef union
{
    unsigned long LONG;
    R_BSP_ATTRIB_STRUCT_BIT_ORDER_LEFT_6(
        unsigned long BTFLG:1,
        unsigned long :3,
        unsigned long FAWE:12,
        unsigned long FSPR:1,
        unsigned long :3,
        unsigned long FAWS:12
    ) BIT;
} fawreg_t;
R_BSP_PRAGMA_PACKOPTION


/*  According to HW Manual the Max Programming Time for 128 bytes (ROM)
    is 13.2ms.  This is with a FCLK of 4 MHz. The calculation below
    calculates the number of ICLK ticks needed for the timeout delay.
    The 31.2ms number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_ROM_WRITE \
        ((int32_t)(13200 * (60.0/(MCU_CFG_FCLK_HZ/1000000)))*(MCU_CFG_ICLK_HZ/1000000))

/*  According to HW Manual the Max Programming Time for 4 bytes
    (Data Flash) is 1.7ms.  This is with a FCLK of 60MHz. The calculation
    below calculates the number of ICLK ticks needed for the timeout delay.
    The 1.7ms number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_DF_WRITE \
        ((int32_t)(1700 * (60.0/(MCU_CFG_FCLK_HZ/1000000)))*(MCU_CFG_ICLK_HZ/1000000))

/*  According to HW Manual the Max Blank Check time for 4 bytes
    (Data Flash) is 30 usec.  This is with a FCLK of 60MHz. The calculation
    below calculates the number of ICLK ticks needed for the timeout delay.
    The 30 usec number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_BLANK_CHECK \
        ((int32_t)(30 * (60.0/(MCU_CFG_FCLK_HZ/1000000)))*(MCU_CFG_ICLK_HZ/1000000))

/*  According to HW Manual the max timeout value when using the peripheral
    clock notification command is 60us. This is with a FCLK of 50MHz. The
    calculation below calculates the number of ICLK ticks needed for the
    timeout delay. The 60us number is adjusted linearly depending on
    the FCLK frequency.
*/
#define WAIT_MAX_NOTIFY_FCU_CLOCK \
        ((int32_t)(60 * (50.0/(MCU_CFG_FCLK_HZ/1000000)))*(MCU_CFG_ICLK_HZ/1000000))

/*  According to HW Manual the Max Erasure Time for a 32KB block is
    around 480ms.  This is with a FCLK of 4 MHz. The calculation below
    calculates the number of ICLK ticks needed for the timeout delay.
    The 480ms number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_ERASE_CF_32K \
        ((int32_t)(480000 * (60.0/(MCU_CFG_FCLK_HZ/1000000)))*(MCU_CFG_ICLK_HZ/1000000))

/*  According to HW Manual the Max Erasure Time for a 8KB block is
    around 120ms.  This is with a FCLK of 4 MHz. The calculation below
    calculates the number of ICLK ticks needed for the timeout delay.
    The 120ms number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_ERASE_CF_8K \
        ((int32_t)(120000 * (60.0/(MCU_CFG_FCLK_HZ/1000000)))*(MCU_CFG_ICLK_HZ/1000000))

/*  According to HW Manual the Max Erasure Time for a 64 byte Data Flash block is
    around 10ms.  This is with a FCLK of 60MHz. The calculation below
    calculates the number of ICLK ticks needed for the timeout delay.
    The 10ms number is adjusted linearly depending on the FCLK frequency.
*/
#define WAIT_MAX_ERASE_DF \
        ((int32_t)(10000 * (60.0/(MCU_CFG_FCLK_HZ/1000000)))*(MCU_CFG_ICLK_HZ/1000000))

/******************************************************************************
Error checking
******************************************************************************/
// frequency range checking moved to flash_api_open()

#endif/* R_FLASH_RX671_H */
