/************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018(2019) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* File Name    : r_dm_1.c
* Version      : 2.00
* Description  : DATFRX interface source file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 28.09.2018 2.00     First Release
*              : 25.01.2019 2.01     English PDF added, Fixed blank check processing and Busy check procedure
*************************************************************************************************/

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_dm_1.h"

#if(FLASH_TYPE == FLASH_TYPE_1)
/************************************************************************************************
Macro definitions
*************************************************************************************************/

/************************************************************************************************
Local Typedef definitions
*************************************************************************************************/

/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/
/* This variable may pass a value to the FLASH.
   Be careful. */
flash_dm_t       gs_flash_dm_b_data[FLASH_DM_PRG_BUF_INDEX];    /* Value of program      */

/************************************************************************************************
Private global variables and functions
*************************************************************************************************/
extern st_flash_dispatch_1_hndl_t* (g_flash_dispatch_1_hndl);
static uint8_t          r_flash_dm_drvif_overwrite(flash_dm_ptr_t addr, flash_dm_t value);
static uint8_t          r_flash_dm_block_copy_setup(void);
static uint8_t          r_flash_dm_block_fill_setup(void);
static e_flash_dm_state_t r_flash_dm_block_program_setup(void);
static uint16_t         gs_flash_dm_b_counter;          /* Counter of program                   */
static uint8_t          gs_flash_dm_b_unit;             /* Unit of program                      */
static uint8_t          gs_flash_dm_c_mode;             /* Mode of command (Erase or program)   */
static const uint8_t    gs_flash_dm_transfer_unit = FLASH_DF_MIN_PGM_SIZE; /* Burst write unit */
uint32_t                g_flash_dm_err_flg;

/* **** Code Flash Setting **** */
#if (FLASH_CFG_CODE_FLASH_ENABLE == 1)
/* Information of blocks */
static st_flash_dm_block_t gs_flash_dm_block_info[FLASH_DM_CFG_CF_BLOCK_NUM];

/* Physical block addresses of code flash */
const flash_dm_ptr_t g_r_flash_dm_block_addr[] =
{
#if (FLASH_DM_CFG_CF_MANAGEMENT_BLOCK == 0)
    /* cast from uint32_t to uint32_t* */
    (flash_dm_ptr_t)FLASH_CF_BLOCK_1,
    /* cast from uint32_t to uint32_t* */
    (flash_dm_ptr_t)FLASH_CF_BLOCK_2,
#if (FLASH_DM_CFG_CF_BLOCK_NUM == 2)
#else
    /* cast from uint32_t to uint32_t* */
    (flash_dm_ptr_t)FLASH_CF_BLOCK_3,
#if (FLASH_DM_CFG_CF_BLOCK_NUM == 3)
#else
    /* cast from uint32_t to uint32_t* */
    (flash_dm_ptr_t)FLASH_CF_BLOCK_4,
#if (FLASH_DM_CFG_CF_BLOCK_NUM == 4)
#else
    (flash_dm_ptr_t)FLASH_CF_BLOCK_5,
#if (FLASH_DM_CFG_CF_BLOCK_NUM == 5)
#else
    (flash_dm_ptr_t)FLASH_CF_BLOCK_6,
#if (FLASH_DM_CFG_CF_BLOCK_NUM == 6)
#else
    (flash_dm_ptr_t)FLASH_CF_BLOCK_7,
#if (FLASH_DM_CFG_CF_BLOCK_NUM == 7)
#else
    (flash_dm_ptr_t)FLASH_CF_BLOCK_8
#endif /* (FLASH_DM_CFG_CF_BLOCK_NUM == 7) */
#endif /* (FLASH_DM_CFG_CF_BLOCK_NUM == 6) */
#endif /* (FLASH_DM_CFG_CF_BLOCK_NUM == 5) */
#endif /* (FLASH_DM_CFG_CF_BLOCK_NUM == 4) */
#endif /* (FLASH_DM_CFG_CF_BLOCK_NUM == 3) */
#endif /* (FLASH_DM_CFG_CF_BLOCK_NUM == 2) */

#elif (FLASH_DM_CFG_CF_MANAGEMENT_BLOCK == 1) && (FLASH_CF_BLOCK_SIZE == 1024)
    (flash_dm_ptr_t)FLASH_CF_BLOCK_32,
    (flash_dm_ptr_t)FLASH_CF_BLOCK_33,
#if (FLASH_DM_CFG_CF_BLOCK_NUM == 2)
#else
    (flash_dm_ptr_t)FLASH_CF_BLOCK_34,
#if (FLASH_DM_CFG_CF_BLOCK_NUM == 3)
#else
    (flash_dm_ptr_t)FLASH_CF_BLOCK_35,
#if (FLASH_DM_CFG_CF_BLOCK_NUM == 4)
#else
    (flash_dm_ptr_t)FLASH_CF_BLOCK_36,
#if (FLASH_DM_CFG_CF_BLOCK_NUM == 5)
#else
    (flash_dm_ptr_t)FLASH_CF_BLOCK_37,
#if (FLASH_DM_CFG_CF_BLOCK_NUM == 6)
#else
    (flash_dm_ptr_t)FLASH_CF_BLOCK_38,
#if (FLASH_DM_CFG_CF_BLOCK_NUM == 7)
#else
    (flash_dm_ptr_t)FLASH_CF_BLOCK_39
#endif /* (FLASH_DM_CFG_CF_BLOCK_NUM == 7) */
#endif /* (FLASH_DM_CFG_CF_BLOCK_NUM == 6) */
#endif /* (FLASH_DM_CFG_CF_BLOCK_NUM == 5) */
#endif /* (FLASH_DM_CFG_CF_BLOCK_NUM == 4) */
#endif /* (FLASH_DM_CFG_CF_BLOCK_NUM == 3) */
#endif /* (FLASH_DM_CFG_CF_BLOCK_NUM == 2) */

#else
    (flash_dm_ptr_t)FLASH_CF_BLOCK_16,
    (flash_dm_ptr_t)FLASH_CF_BLOCK_17,
#if (FLASH_DM_CFG_CF_BLOCK_NUM == 2)
#else
    (flash_dm_ptr_t)FLASH_CF_BLOCK_18,
#if (FLASH_DM_CFG_CF_BLOCK_NUM == 3)
#else
    (flash_dm_ptr_t)FLASH_CF_BLOCK_19,
#if (FLASH_DM_CFG_CF_BLOCK_NUM == 4)
#else
    (flash_dm_ptr_t)FLASH_CF_BLOCK_20,
#if (FLASH_DM_CFG_CF_BLOCK_NUM == 5)
#else
    (flash_dm_ptr_t)FLASH_CF_BLOCK_21,
#if (FLASH_DM_CFG_CF_BLOCK_NUM == 6)
#else
    (flash_dm_ptr_t)FLASH_CF_BLOCK_22,
#if (FLASH_DM_CFG_CF_BLOCK_NUM == 7)
#else
    (flash_dm_ptr_t)FLASH_CF_BLOCK_23
#endif /* (FLASH_DM_CFG_CF_BLOCK_NUM == 7) */
#endif /* (FLASH_DM_CFG_CF_BLOCK_NUM == 6) */
#endif /* (FLASH_DM_CFG_CF_BLOCK_NUM == 5) */
#endif /* (FLASH_DM_CFG_CF_BLOCK_NUM == 4) */
#endif /* (FLASH_DM_CFG_CF_BLOCK_NUM == 3) */
#endif /* (FLASH_DM_CFG_CF_BLOCK_NUM == 2) */

#endif /* (FLASH_DM_CFG_CF_MANAGEMENT_BLOCK == 0) */
};

/* Number of blocks which data flash driver uses */
const uint8_t g_flash_dm_block_num = (uint8_t)(FLASH_DM_CFG_CF_BLOCK_NUM);

/* Data size of each id */
const uint16_t gc_dm_data_size[] =
{
    FLASH_DM_CFG_CF_SIZE_NO0,   FLASH_DM_CFG_CF_SIZE_NO1,   FLASH_DM_CFG_CF_SIZE_NO2,   FLASH_DM_CFG_CF_SIZE_NO3,
    FLASH_DM_CFG_CF_SIZE_NO4,   FLASH_DM_CFG_CF_SIZE_NO5,   FLASH_DM_CFG_CF_SIZE_NO6,   FLASH_DM_CFG_CF_SIZE_NO7,
    FLASH_DM_CFG_CF_SIZE_NO8,   FLASH_DM_CFG_CF_SIZE_NO9,   FLASH_DM_CFG_CF_SIZE_NO10,  FLASH_DM_CFG_CF_SIZE_NO11,
    FLASH_DM_CFG_CF_SIZE_NO12,  FLASH_DM_CFG_CF_SIZE_NO13,  FLASH_DM_CFG_CF_SIZE_NO14,  FLASH_DM_CFG_CF_SIZE_NO15,
    FLASH_DM_CFG_CF_SIZE_NO16,  FLASH_DM_CFG_CF_SIZE_NO17,  FLASH_DM_CFG_CF_SIZE_NO18,  FLASH_DM_CFG_CF_SIZE_NO19,
    FLASH_DM_CFG_CF_SIZE_NO20,  FLASH_DM_CFG_CF_SIZE_NO21,  FLASH_DM_CFG_CF_SIZE_NO22,  FLASH_DM_CFG_CF_SIZE_NO23,
    FLASH_DM_CFG_CF_SIZE_NO24,  FLASH_DM_CFG_CF_SIZE_NO25,  FLASH_DM_CFG_CF_SIZE_NO26,  FLASH_DM_CFG_CF_SIZE_NO27,
    FLASH_DM_CFG_CF_SIZE_NO28,  FLASH_DM_CFG_CF_SIZE_NO29,  FLASH_DM_CFG_CF_SIZE_NO30,  FLASH_DM_CFG_CF_SIZE_NO31,
    FLASH_DM_CFG_CF_SIZE_NO32,  FLASH_DM_CFG_CF_SIZE_NO33,  FLASH_DM_CFG_CF_SIZE_NO34,  FLASH_DM_CFG_CF_SIZE_NO35,
    FLASH_DM_CFG_CF_SIZE_NO36,  FLASH_DM_CFG_CF_SIZE_NO37,  FLASH_DM_CFG_CF_SIZE_NO38,  FLASH_DM_CFG_CF_SIZE_NO39,
    /*  FLASH_DM_CFG_CF_SIZE_NO40,  FLASH_DM_CFG_CF_SIZE_NO41,  FLASH_DM_CFG_CF_SIZE_NO42,  FLASH_DM_CFG_CF_SIZE_NO43, */
    /*  FLASH_DM_CFG_CF_SIZE_NO44,  FLASH_DM_CFG_CF_SIZE_NO45,  FLASH_DM_CFG_CF_SIZE_NO46,  FLASH_DM_CFG_CF_SIZE_NO47, */
    /*  FLASH_DM_CFG_CF_SIZE_NO48,  FLASH_DM_CFG_CF_SIZE_NO49,  FLASH_DM_CFG_CF_SIZE_NO50,  FLASH_DM_CFG_CF_SIZE_NO51, */
    /*  FLASH_DM_CFG_CF_SIZE_NO52,  FLASH_DM_CFG_CF_SIZE_NO53,  FLASH_DM_CFG_CF_SIZE_NO54,  FLASH_DM_CFG_CF_SIZE_NO55, */
    /*  FLASH_DM_CFG_CF_SIZE_NO56,  FLASH_DM_CFG_CF_SIZE_NO57,  FLASH_DM_CFG_CF_SIZE_NO58,  FLASH_DM_CFG_CF_SIZE_NO59, */
    /*  FLASH_DM_CFG_CF_SIZE_NO60,  FLASH_DM_CFG_CF_SIZE_NO61,  FLASH_DM_CFG_CF_SIZE_NO62,  FLASH_DM_CFG_CF_SIZE_NO63, */
    /*  FLASH_DM_CFG_CF_SIZE_NO64,  FLASH_DM_CFG_CF_SIZE_NO65,  FLASH_DM_CFG_CF_SIZE_NO66,  FLASH_DM_CFG_CF_SIZE_NO67, */
    /*  FLASH_DM_CFG_CF_SIZE_NO68,  FLASH_DM_CFG_CF_SIZE_NO69,  FLASH_DM_CFG_CF_SIZE_NO70,  FLASH_DM_CFG_CF_SIZE_NO71, */
    /*  FLASH_DM_CFG_CF_SIZE_NO72,  FLASH_DM_CFG_CF_SIZE_NO73,  FLASH_DM_CFG_CF_SIZE_NO74,  FLASH_DM_CFG_CF_SIZE_NO75, */
    /*  FLASH_DM_CFG_CF_SIZE_NO76,  FLASH_DM_CFG_CF_SIZE_NO77,  FLASH_DM_CFG_CF_SIZE_NO78,  FLASH_DM_CFG_CF_SIZE_NO79, */
    /*  FLASH_DM_CFG_CF_SIZE_NO80,  FLASH_DM_CFG_CF_SIZE_NO81,  FLASH_DM_CFG_CF_SIZE_NO82,  FLASH_DM_CFG_CF_SIZE_NO83, */
    /*  FLASH_DM_CFG_CF_SIZE_NO84,  FLASH_DM_CFG_CF_SIZE_NO85,  FLASH_DM_CFG_CF_SIZE_NO86,  FLASH_DM_CFG_CF_SIZE_NO87, */
    /*  FLASH_DM_CFG_CF_SIZE_NO88,  FLASH_DM_CFG_CF_SIZE_NO89,  FLASH_DM_CFG_CF_SIZE_NO90,  FLASH_DM_CFG_CF_SIZE_NO91, */
    /*  FLASH_DM_CFG_CF_SIZE_NO92,  FLASH_DM_CFG_CF_SIZE_NO93,  FLASH_DM_CFG_CF_SIZE_NO94,  FLASH_DM_CFG_CF_SIZE_NO95, */
    /*  FLASH_DM_CFG_CF_SIZE_NO96,  FLASH_DM_CFG_CF_SIZE_NO97,  FLASH_DM_CFG_CF_SIZE_NO98,  FLASH_DM_CFG_CF_SIZE_NO99, */
    /*  FLASH_DM_CFG_CF_SIZE_NO100, FLASH_DM_CFG_CF_SIZE_NO101, FLASH_DM_CFG_CF_SIZE_NO102, FLASH_DM_CFG_CF_SIZE_NO103, */
    /*  FLASH_DM_CFG_CF_SIZE_NO104, FLASH_DM_CFG_CF_SIZE_NO105, FLASH_DM_CFG_CF_SIZE_NO106, FLASH_DM_CFG_CF_SIZE_NO107, */
    /*  FLASH_DM_CFG_CF_SIZE_NO108, FLASH_DM_CFG_CF_SIZE_NO109, FLASH_DM_CFG_CF_SIZE_NO110, FLASH_DM_CFG_CF_SIZE_NO111, */
    /*  FLASH_DM_CFG_CF_SIZE_NO112, FLASH_DM_CFG_CF_SIZE_NO113, FLASH_DM_CFG_CF_SIZE_NO114, FLASH_DM_CFG_CF_SIZE_NO115, */
    /*  FLASH_DM_CFG_CF_SIZE_NO116, FLASH_DM_CFG_CF_SIZE_NO117, FLASH_DM_CFG_CF_SIZE_NO118, FLASH_DM_CFG_CF_SIZE_NO119, */
    /*  FLASH_DM_CFG_CF_SIZE_NO120, FLASH_DM_CFG_CF_SIZE_NO121, FLASH_DM_CFG_CF_SIZE_NO122, FLASH_DM_CFG_CF_SIZE_NO123, */
    /*  FLASH_DM_CFG_CF_SIZE_NO124, FLASH_DM_CFG_CF_SIZE_NO125, FLASH_DM_CFG_CF_SIZE_NO126, FLASH_DM_CFG_CF_SIZE_NO127, */
    /*  FLASH_DM_CFG_CF_SIZE_NO128, FLASH_DM_CFG_CF_SIZE_NO129, FLASH_DM_CFG_CF_SIZE_NO130, FLASH_DM_CFG_CF_SIZE_NO131, */
    /*  FLASH_DM_CFG_CF_SIZE_NO132, FLASH_DM_CFG_CF_SIZE_NO133, FLASH_DM_CFG_CF_SIZE_NO134, FLASH_DM_CFG_CF_SIZE_NO135, */
    /*  FLASH_DM_CFG_CF_SIZE_NO136, FLASH_DM_CFG_CF_SIZE_NO137, FLASH_DM_CFG_CF_SIZE_NO138, FLASH_DM_CFG_CF_SIZE_NO139, */
    /*  FLASH_DM_CFG_CF_SIZE_NO140, FLASH_DM_CFG_CF_SIZE_NO141, FLASH_DM_CFG_CF_SIZE_NO142, FLASH_DM_CFG_CF_SIZE_NO143, */
    /*  FLASH_DM_CFG_CF_SIZE_NO144, FLASH_DM_CFG_CF_SIZE_NO145, FLASH_DM_CFG_CF_SIZE_NO146, FLASH_DM_CFG_CF_SIZE_NO147, */
    /*  FLASH_DM_CFG_CF_SIZE_NO148, FLASH_DM_CFG_CF_SIZE_NO149, FLASH_DM_CFG_CF_SIZE_NO150, FLASH_DM_CFG_CF_SIZE_NO151, */
    /*  FLASH_DM_CFG_CF_SIZE_NO152, FLASH_DM_CFG_CF_SIZE_NO153, FLASH_DM_CFG_CF_SIZE_NO154, FLASH_DM_CFG_CF_SIZE_NO155, */
    /*  FLASH_DM_CFG_CF_SIZE_NO156, FLASH_DM_CFG_CF_SIZE_NO157, FLASH_DM_CFG_CF_SIZE_NO158, FLASH_DM_CFG_CF_SIZE_NO159, */
    /*  FLASH_DM_CFG_CF_SIZE_NO160, FLASH_DM_CFG_CF_SIZE_NO161, FLASH_DM_CFG_CF_SIZE_NO162, FLASH_DM_CFG_CF_SIZE_NO163, */
    /*  FLASH_DM_CFG_CF_SIZE_NO164, FLASH_DM_CFG_CF_SIZE_NO165, FLASH_DM_CFG_CF_SIZE_NO166, FLASH_DM_CFG_CF_SIZE_NO167, */
    /*  FLASH_DM_CFG_CF_SIZE_NO168, FLASH_DM_CFG_CF_SIZE_NO169, FLASH_DM_CFG_CF_SIZE_NO170, FLASH_DM_CFG_CF_SIZE_NO171, */
    /*  FLASH_DM_CFG_CF_SIZE_NO172, FLASH_DM_CFG_CF_SIZE_NO173, FLASH_DM_CFG_CF_SIZE_NO174, FLASH_DM_CFG_CF_SIZE_NO175, */
    /*  FLASH_DM_CFG_CF_SIZE_NO176, FLASH_DM_CFG_CF_SIZE_NO177, FLASH_DM_CFG_CF_SIZE_NO178, FLASH_DM_CFG_CF_SIZE_NO179, */
    /*  FLASH_DM_CFG_CF_SIZE_NO180, FLASH_DM_CFG_CF_SIZE_NO181, FLASH_DM_CFG_CF_SIZE_NO182, FLASH_DM_CFG_CF_SIZE_NO183, */
    /*  FLASH_DM_CFG_CF_SIZE_NO184, FLASH_DM_CFG_CF_SIZE_NO185, FLASH_DM_CFG_CF_SIZE_NO186, FLASH_DM_CFG_CF_SIZE_NO187, */
    /*  FLASH_DM_CFG_CF_SIZE_NO188, FLASH_DM_CFG_CF_SIZE_NO189, FLASH_DM_CFG_CF_SIZE_NO190, FLASH_DM_CFG_CF_SIZE_NO191, */
    /*  FLASH_DM_CFG_CF_SIZE_NO192, FLASH_DM_CFG_CF_SIZE_NO193, FLASH_DM_CFG_CF_SIZE_NO194, FLASH_DM_CFG_CF_SIZE_NO195, */
    /*  FLASH_DM_CFG_CF_SIZE_NO196, FLASH_DM_CFG_CF_SIZE_NO197, FLASH_DM_CFG_CF_SIZE_NO198, FLASH_DM_CFG_CF_SIZE_NO199, */
    /*  FLASH_DM_CFG_CF_SIZE_NO200, FLASH_DM_CFG_CF_SIZE_NO201, FLASH_DM_CFG_CF_SIZE_NO202, FLASH_DM_CFG_CF_SIZE_NO203, */
    /*  FLASH_DM_CFG_CF_SIZE_NO204, FLASH_DM_CFG_CF_SIZE_NO205, FLASH_DM_CFG_CF_SIZE_NO206, FLASH_DM_CFG_CF_SIZE_NO207, */
    /*  FLASH_DM_CFG_CF_SIZE_NO208, FLASH_DM_CFG_CF_SIZE_NO209, FLASH_DM_CFG_CF_SIZE_NO210, FLASH_DM_CFG_CF_SIZE_NO211, */
    /*  FLASH_DM_CFG_CF_SIZE_NO212, FLASH_DM_CFG_CF_SIZE_NO213, FLASH_DM_CFG_CF_SIZE_NO214, FLASH_DM_CFG_CF_SIZE_NO215, */
    /*  FLASH_DM_CFG_CF_SIZE_NO216, FLASH_DM_CFG_CF_SIZE_NO217, FLASH_DM_CFG_CF_SIZE_NO218, FLASH_DM_CFG_CF_SIZE_NO219, */
    /*  FLASH_DM_CFG_CF_SIZE_NO220, FLASH_DM_CFG_CF_SIZE_NO221, FLASH_DM_CFG_CF_SIZE_NO222, FLASH_DM_CFG_CF_SIZE_NO223, */
    /*  FLASH_DM_CFG_CF_SIZE_NO224, FLASH_DM_CFG_CF_SIZE_NO225, FLASH_DM_CFG_CF_SIZE_NO226, FLASH_DM_CFG_CF_SIZE_NO227, */
    /*  FLASH_DM_CFG_CF_SIZE_NO228, FLASH_DM_CFG_CF_SIZE_NO229, FLASH_DM_CFG_CF_SIZE_NO230, FLASH_DM_CFG_CF_SIZE_NO231, */
    /*  FLASH_DM_CFG_CF_SIZE_NO232, FLASH_DM_CFG_CF_SIZE_NO233, FLASH_DM_CFG_CF_SIZE_NO234, FLASH_DM_CFG_CF_SIZE_NO235, */
    /*  FLASH_DM_CFG_CF_SIZE_NO236, FLASH_DM_CFG_CF_SIZE_NO237, FLASH_DM_CFG_CF_SIZE_NO238, FLASH_DM_CFG_CF_SIZE_NO239, */
    /*  FLASH_DM_CFG_CF_SIZE_NO240, FLASH_DM_CFG_CF_SIZE_NO241, FLASH_DM_CFG_CF_SIZE_NO242, FLASH_DM_CFG_CF_SIZE_NO243, */
    /*  FLASH_DM_CFG_CF_SIZE_NO244, FLASH_DM_CFG_CF_SIZE_NO245, FLASH_DM_CFG_CF_SIZE_NO246, FLASH_DM_CFG_CF_SIZE_NO247, */
    /*  FLASH_DM_CFG_CF_SIZE_NO248, FLASH_DM_CFG_CF_SIZE_NO249, FLASH_DM_CFG_CF_SIZE_NO250, FLASH_DM_CFG_CF_SIZE_NO251, */
    /*  FLASH_DM_CFG_CF_SIZE_NO252, FLASH_DM_CFG_CF_SIZE_NO253, FLASH_DM_CFG_CF_SIZE_NO254  */
};

/* Number of id of code flash which driver handles */
#if (FLASH_CF_BLOCK_SIZE == 1024)
/* cast from uint16_t to uint32_t */
const flash_dm_t g_r_flash_dm_block_erase_flag[FLASH_DM_BH_ERASE_FLAG_INDEX] = {0xaaaaaaaa, 0xaaaaaaaa};
#endif /* (FLASH_CF_BLOCK_SIZE == 1024) */
#if (FLASH_CF_BLOCK_SIZE == 2048)
/* cast from uint16_t to uint32_t */
const flash_dm_t g_r_flash_dm_block_erase_flag[FLASH_DM_BH_ERASE_FLAG_INDEX] = {0xaaaaaaaa, 0xaaaaaaaa,
                                                                                0xaaaaaaaa, 0xaaaaaaaa
                                                                               };
#endif /* (FLASH_CF_BLOCK_SIZE == 2048) */
#endif /* (FLASH_CFG_CODE_FLASH_ENABLE == 1) */


/* **** Data Flash Setting **** */
#if (FLASH_CFG_CODE_FLASH_ENABLE == 0)
/* Information of blocks */
static st_flash_dm_block_t gs_flash_dm_block_info[FLASH_DM_CFG_DF_BLOCK_NUM];

/* Physical block addresses of data flash */
const flash_dm_ptr_t g_r_flash_dm_block_addr[] =
{
    /* cast from uint32_t to flash_dm_ptr_t */
    (flash_dm_ptr_t)FLASH_DF_BLOCK_0,
    /* cast from uint32_t to flash_dm_ptr_t */
    (flash_dm_ptr_t)FLASH_DF_BLOCK_1,
#if (FLASH_DM_CFG_DF_BLOCK_NUM == 2)
#else
    /* cast from uint32_t to flash_dm_ptr_t */
    (flash_dm_ptr_t)FLASH_DF_BLOCK_2,
#if (FLASH_DM_CFG_DF_BLOCK_NUM == 3)
#else
    /* cast from uint32_t to flash_dm_ptr_t */
    (flash_dm_ptr_t)FLASH_DF_BLOCK_3,
#if (FLASH_DM_CFG_DF_BLOCK_NUM == 4)
#else
    /* cast from uint32_t to flash_dm_ptr_t */
    (flash_dm_ptr_t)FLASH_DF_BLOCK_4,
#if (FLASH_DM_CFG_DF_BLOCK_NUM == 5)
#else
    /* cast from uint32_t to flash_dm_ptr_t */
    (flash_dm_ptr_t)FLASH_DF_BLOCK_5,
#if (FLASH_DM_CFG_DF_BLOCK_NUM == 6)
#else
    /* cast from uint32_t to flash_dm_ptr_t */
    (flash_dm_ptr_t)FLASH_DF_BLOCK_6,
#if (FLASH_DM_CFG_DF_BLOCK_NUM == 7)
#else
    /* cast from uint32_t to flash_dm_ptr_t */
    (flash_dm_ptr_t)FLASH_DF_BLOCK_7
#endif /* (FLASH_DM_CFG_DF_BLOCK_NUM == 7) */
#endif /* (FLASH_DM_CFG_DF_BLOCK_NUM == 6) */
#endif /* (FLASH_DM_CFG_DF_BLOCK_NUM == 5) */
#endif /* (FLASH_DM_CFG_DF_BLOCK_NUM == 4) */
#endif /* (FLASH_DM_CFG_DF_BLOCK_NUM == 3) */
#endif /* (FLASH_DM_CFG_DF_BLOCK_NUM == 2) */
};

/* Number of blocks which data flash driver uses */
const uint8_t g_flash_dm_block_num = (uint8_t)(FLASH_DM_CFG_DF_BLOCK_NUM);

/* Data size of each id */
const uint16_t gc_dm_data_size[] =
{
    FLASH_DM_CFG_DF_SIZE_NO0,   FLASH_DM_CFG_DF_SIZE_NO1,   FLASH_DM_CFG_DF_SIZE_NO2,   FLASH_DM_CFG_DF_SIZE_NO3,
    FLASH_DM_CFG_DF_SIZE_NO4,   FLASH_DM_CFG_DF_SIZE_NO5,   FLASH_DM_CFG_DF_SIZE_NO6,   FLASH_DM_CFG_DF_SIZE_NO7,
    FLASH_DM_CFG_DF_SIZE_NO8,   FLASH_DM_CFG_DF_SIZE_NO9,   FLASH_DM_CFG_DF_SIZE_NO10,  FLASH_DM_CFG_DF_SIZE_NO11,
    FLASH_DM_CFG_DF_SIZE_NO12,  FLASH_DM_CFG_DF_SIZE_NO13,  FLASH_DM_CFG_DF_SIZE_NO14,  FLASH_DM_CFG_DF_SIZE_NO15,
    FLASH_DM_CFG_DF_SIZE_NO16,  FLASH_DM_CFG_DF_SIZE_NO17,  FLASH_DM_CFG_DF_SIZE_NO18,  FLASH_DM_CFG_DF_SIZE_NO19,
    FLASH_DM_CFG_DF_SIZE_NO20,  FLASH_DM_CFG_DF_SIZE_NO21,  FLASH_DM_CFG_DF_SIZE_NO22,  FLASH_DM_CFG_DF_SIZE_NO23,
    FLASH_DM_CFG_DF_SIZE_NO24,  FLASH_DM_CFG_DF_SIZE_NO25,  FLASH_DM_CFG_DF_SIZE_NO26,  FLASH_DM_CFG_DF_SIZE_NO27,
    FLASH_DM_CFG_DF_SIZE_NO28,  FLASH_DM_CFG_DF_SIZE_NO29,  FLASH_DM_CFG_DF_SIZE_NO30,  FLASH_DM_CFG_DF_SIZE_NO31,
    FLASH_DM_CFG_DF_SIZE_NO32,  FLASH_DM_CFG_DF_SIZE_NO33,  FLASH_DM_CFG_DF_SIZE_NO34,  FLASH_DM_CFG_DF_SIZE_NO35,
    FLASH_DM_CFG_DF_SIZE_NO36,  FLASH_DM_CFG_DF_SIZE_NO37,  FLASH_DM_CFG_DF_SIZE_NO38,  FLASH_DM_CFG_DF_SIZE_NO39,
    /*  FLASH_DM_CFG_DF_SIZE_NO40,  FLASH_DM_CFG_DF_SIZE_NO41,  FLASH_DM_CFG_DF_SIZE_NO42,  FLASH_DM_CFG_DF_SIZE_NO43, */
    /*  FLASH_DM_CFG_DF_SIZE_NO44,  FLASH_DM_CFG_DF_SIZE_NO45,  FLASH_DM_CFG_DF_SIZE_NO46,  FLASH_DM_CFG_DF_SIZE_NO47, */
    /*  FLASH_DM_CFG_DF_SIZE_NO48,  FLASH_DM_CFG_DF_SIZE_NO49,  FLASH_DM_CFG_DF_SIZE_NO50,  FLASH_DM_CFG_DF_SIZE_NO51, */
    /*  FLASH_DM_CFG_DF_SIZE_NO52,  FLASH_DM_CFG_DF_SIZE_NO53,  FLASH_DM_CFG_DF_SIZE_NO54,  FLASH_DM_CFG_DF_SIZE_NO55, */
    /*  FLASH_DM_CFG_DF_SIZE_NO56,  FLASH_DM_CFG_DF_SIZE_NO57,  FLASH_DM_CFG_DF_SIZE_NO58,  FLASH_DM_CFG_DF_SIZE_NO59, */
    /*  FLASH_DM_CFG_DF_SIZE_NO60,  FLASH_DM_CFG_DF_SIZE_NO61,  FLASH_DM_CFG_DF_SIZE_NO62,  FLASH_DM_CFG_DF_SIZE_NO63, */
    /*  FLASH_DM_CFG_DF_SIZE_NO64,  FLASH_DM_CFG_DF_SIZE_NO65,  FLASH_DM_CFG_DF_SIZE_NO66,  FLASH_DM_CFG_DF_SIZE_NO67, */
    /*  FLASH_DM_CFG_DF_SIZE_NO68,  FLASH_DM_CFG_DF_SIZE_NO69,  FLASH_DM_CFG_DF_SIZE_NO70,  FLASH_DM_CFG_DF_SIZE_NO71, */
    /*  FLASH_DM_CFG_DF_SIZE_NO72,  FLASH_DM_CFG_DF_SIZE_NO73,  FLASH_DM_CFG_DF_SIZE_NO74,  FLASH_DM_CFG_DF_SIZE_NO75, */
    /*  FLASH_DM_CFG_DF_SIZE_NO76,  FLASH_DM_CFG_DF_SIZE_NO77,  FLASH_DM_CFG_DF_SIZE_NO78,  FLASH_DM_CFG_DF_SIZE_NO79, */
    /*  FLASH_DM_CFG_DF_SIZE_NO80,  FLASH_DM_CFG_DF_SIZE_NO81,  FLASH_DM_CFG_DF_SIZE_NO82,  FLASH_DM_CFG_DF_SIZE_NO83, */
    /*  FLASH_DM_CFG_DF_SIZE_NO84,  FLASH_DM_CFG_DF_SIZE_NO85,  FLASH_DM_CFG_DF_SIZE_NO86,  FLASH_DM_CFG_DF_SIZE_NO87, */
    /*  FLASH_DM_CFG_DF_SIZE_NO88,  FLASH_DM_CFG_DF_SIZE_NO89,  FLASH_DM_CFG_DF_SIZE_NO90,  FLASH_DM_CFG_DF_SIZE_NO91, */
    /*  FLASH_DM_CFG_DF_SIZE_NO92,  FLASH_DM_CFG_DF_SIZE_NO93,  FLASH_DM_CFG_DF_SIZE_NO94,  FLASH_DM_CFG_DF_SIZE_NO95, */
    /*  FLASH_DM_CFG_DF_SIZE_NO96,  FLASH_DM_CFG_DF_SIZE_NO97,  FLASH_DM_CFG_DF_SIZE_NO98,  FLASH_DM_CFG_DF_SIZE_NO99, */
    /*  FLASH_DM_CFG_DF_SIZE_NO100, FLASH_DM_CFG_DF_SIZE_NO101, FLASH_DM_CFG_DF_SIZE_NO102, FLASH_DM_CFG_DF_SIZE_NO103, */
    /*  FLASH_DM_CFG_DF_SIZE_NO104, FLASH_DM_CFG_DF_SIZE_NO105, FLASH_DM_CFG_DF_SIZE_NO106, FLASH_DM_CFG_DF_SIZE_NO107, */
    /*  FLASH_DM_CFG_DF_SIZE_NO108, FLASH_DM_CFG_DF_SIZE_NO109, FLASH_DM_CFG_DF_SIZE_NO110, FLASH_DM_CFG_DF_SIZE_NO111, */
    /*  FLASH_DM_CFG_DF_SIZE_NO112, FLASH_DM_CFG_DF_SIZE_NO113, FLASH_DM_CFG_DF_SIZE_NO114, FLASH_DM_CFG_DF_SIZE_NO115, */
    /*  FLASH_DM_CFG_DF_SIZE_NO116, FLASH_DM_CFG_DF_SIZE_NO117, FLASH_DM_CFG_DF_SIZE_NO118, FLASH_DM_CFG_DF_SIZE_NO119, */
    /*  FLASH_DM_CFG_DF_SIZE_NO120, FLASH_DM_CFG_DF_SIZE_NO121, FLASH_DM_CFG_DF_SIZE_NO122, FLASH_DM_CFG_DF_SIZE_NO123, */
    /*  FLASH_DM_CFG_DF_SIZE_NO124, FLASH_DM_CFG_DF_SIZE_NO125, FLASH_DM_CFG_DF_SIZE_NO126, FLASH_DM_CFG_DF_SIZE_NO127, */
    /*  FLASH_DM_CFG_DF_SIZE_NO128, FLASH_DM_CFG_DF_SIZE_NO129, FLASH_DM_CFG_DF_SIZE_NO130, FLASH_DM_CFG_DF_SIZE_NO131, */
    /*  FLASH_DM_CFG_DF_SIZE_NO132, FLASH_DM_CFG_DF_SIZE_NO133, FLASH_DM_CFG_DF_SIZE_NO134, FLASH_DM_CFG_DF_SIZE_NO135, */
    /*  FLASH_DM_CFG_DF_SIZE_NO136, FLASH_DM_CFG_DF_SIZE_NO137, FLASH_DM_CFG_DF_SIZE_NO138, FLASH_DM_CFG_DF_SIZE_NO139, */
    /*  FLASH_DM_CFG_DF_SIZE_NO140, FLASH_DM_CFG_DF_SIZE_NO141, FLASH_DM_CFG_DF_SIZE_NO142, FLASH_DM_CFG_DF_SIZE_NO143, */
    /*  FLASH_DM_CFG_DF_SIZE_NO144, FLASH_DM_CFG_DF_SIZE_NO145, FLASH_DM_CFG_DF_SIZE_NO146, FLASH_DM_CFG_DF_SIZE_NO147, */
    /*  FLASH_DM_CFG_DF_SIZE_NO148, FLASH_DM_CFG_DF_SIZE_NO149, FLASH_DM_CFG_DF_SIZE_NO150, FLASH_DM_CFG_DF_SIZE_NO151, */
    /*  FLASH_DM_CFG_DF_SIZE_NO152, FLASH_DM_CFG_DF_SIZE_NO153, FLASH_DM_CFG_DF_SIZE_NO154, FLASH_DM_CFG_DF_SIZE_NO155, */
    /*  FLASH_DM_CFG_DF_SIZE_NO156, FLASH_DM_CFG_DF_SIZE_NO157, FLASH_DM_CFG_DF_SIZE_NO158, FLASH_DM_CFG_DF_SIZE_NO159, */
    /*  FLASH_DM_CFG_DF_SIZE_NO160, FLASH_DM_CFG_DF_SIZE_NO161, FLASH_DM_CFG_DF_SIZE_NO162, FLASH_DM_CFG_DF_SIZE_NO163, */
    /*  FLASH_DM_CFG_DF_SIZE_NO164, FLASH_DM_CFG_DF_SIZE_NO165, FLASH_DM_CFG_DF_SIZE_NO166, FLASH_DM_CFG_DF_SIZE_NO167, */
    /*  FLASH_DM_CFG_DF_SIZE_NO168, FLASH_DM_CFG_DF_SIZE_NO169, FLASH_DM_CFG_DF_SIZE_NO170, FLASH_DM_CFG_DF_SIZE_NO171, */
    /*  FLASH_DM_CFG_DF_SIZE_NO172, FLASH_DM_CFG_DF_SIZE_NO173, FLASH_DM_CFG_DF_SIZE_NO174, FLASH_DM_CFG_DF_SIZE_NO175, */
    /*  FLASH_DM_CFG_DF_SIZE_NO176, FLASH_DM_CFG_DF_SIZE_NO177, FLASH_DM_CFG_DF_SIZE_NO178, FLASH_DM_CFG_DF_SIZE_NO179, */
    /*  FLASH_DM_CFG_DF_SIZE_NO180, FLASH_DM_CFG_DF_SIZE_NO181, FLASH_DM_CFG_DF_SIZE_NO182, FLASH_DM_CFG_DF_SIZE_NO183, */
    /*  FLASH_DM_CFG_DF_SIZE_NO184, FLASH_DM_CFG_DF_SIZE_NO185, FLASH_DM_CFG_DF_SIZE_NO186, FLASH_DM_CFG_DF_SIZE_NO187, */
    /*  FLASH_DM_CFG_DF_SIZE_NO188, FLASH_DM_CFG_DF_SIZE_NO189, FLASH_DM_CFG_DF_SIZE_NO190, FLASH_DM_CFG_DF_SIZE_NO191, */
    /*  FLASH_DM_CFG_DF_SIZE_NO192, FLASH_DM_CFG_DF_SIZE_NO193, FLASH_DM_CFG_DF_SIZE_NO194, FLASH_DM_CFG_DF_SIZE_NO195, */
    /*  FLASH_DM_CFG_DF_SIZE_NO196, FLASH_DM_CFG_DF_SIZE_NO197, FLASH_DM_CFG_DF_SIZE_NO198, FLASH_DM_CFG_DF_SIZE_NO199, */
    /*  FLASH_DM_CFG_DF_SIZE_NO200, FLASH_DM_CFG_DF_SIZE_NO201, FLASH_DM_CFG_DF_SIZE_NO202, FLASH_DM_CFG_DF_SIZE_NO203, */
    /*  FLASH_DM_CFG_DF_SIZE_NO204, FLASH_DM_CFG_DF_SIZE_NO205, FLASH_DM_CFG_DF_SIZE_NO206, FLASH_DM_CFG_DF_SIZE_NO207, */
    /*  FLASH_DM_CFG_DF_SIZE_NO208, FLASH_DM_CFG_DF_SIZE_NO209, FLASH_DM_CFG_DF_SIZE_NO210, FLASH_DM_CFG_DF_SIZE_NO211, */
    /*  FLASH_DM_CFG_DF_SIZE_NO212, FLASH_DM_CFG_DF_SIZE_NO213, FLASH_DM_CFG_DF_SIZE_NO214, FLASH_DM_CFG_DF_SIZE_NO215, */
    /*  FLASH_DM_CFG_DF_SIZE_NO216, FLASH_DM_CFG_DF_SIZE_NO217, FLASH_DM_CFG_DF_SIZE_NO218, FLASH_DM_CFG_DF_SIZE_NO219, */
    /*  FLASH_DM_CFG_DF_SIZE_NO220, FLASH_DM_CFG_DF_SIZE_NO221, FLASH_DM_CFG_DF_SIZE_NO222, FLASH_DM_CFG_DF_SIZE_NO223, */
    /*  FLASH_DM_CFG_DF_SIZE_NO224, FLASH_DM_CFG_DF_SIZE_NO225, FLASH_DM_CFG_DF_SIZE_NO226, FLASH_DM_CFG_DF_SIZE_NO227, */
    /*  FLASH_DM_CFG_DF_SIZE_NO228, FLASH_DM_CFG_DF_SIZE_NO229, FLASH_DM_CFG_DF_SIZE_NO230, FLASH_DM_CFG_DF_SIZE_NO231, */
    /*  FLASH_DM_CFG_DF_SIZE_NO232, FLASH_DM_CFG_DF_SIZE_NO233, FLASH_DM_CFG_DF_SIZE_NO234, FLASH_DM_CFG_DF_SIZE_NO235, */
    /*  FLASH_DM_CFG_DF_SIZE_NO236, FLASH_DM_CFG_DF_SIZE_NO237, FLASH_DM_CFG_DF_SIZE_NO238, FLASH_DM_CFG_DF_SIZE_NO239, */
    /*  FLASH_DM_CFG_DF_SIZE_NO240, FLASH_DM_CFG_DF_SIZE_NO241, FLASH_DM_CFG_DF_SIZE_NO242, FLASH_DM_CFG_DF_SIZE_NO243, */
    /*  FLASH_DM_CFG_DF_SIZE_NO244, FLASH_DM_CFG_DF_SIZE_NO245, FLASH_DM_CFG_DF_SIZE_NO246, FLASH_DM_CFG_DF_SIZE_NO247, */
    /*  FLASH_DM_CFG_DF_SIZE_NO248, FLASH_DM_CFG_DF_SIZE_NO249, FLASH_DM_CFG_DF_SIZE_NO250, FLASH_DM_CFG_DF_SIZE_NO251, */
    /*  FLASH_DM_CFG_DF_SIZE_NO252, FLASH_DM_CFG_DF_SIZE_NO253, FLASH_DM_CFG_DF_SIZE_NO254  */
};

/* Number of id of data flash which driver handles */
const flash_dm_t g_r_flash_dm_block_erase_flag[FLASH_DM_BH_ERASE_FLAG_INDEX] =
{0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa};

#endif /* (FLASH_CFG_CODE_FLASH_ENABLE == 0) */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_open
* Description  : Opens the driver interface.
* Arguments    : None
* Return Value : 1: Successful operation
*              : 0: Error
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash, must place the source code to the ROM.
*              : (Prohibit the placement to RAM)
*************************************************************************************************/
uint8_t r_flash_dm_drvif_open(void)
{
    if (R_FLASH_Open() != FLASH_SUCCESS)
    {
        return 0;
    }

#if ((FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CFG_CODE_FLASH_BGO == 1)) || \
    ((FLASH_CFG_CODE_FLASH_ENABLE == 0) && (FLASH_CFG_DATA_FLASH_BGO == 1))
    flash_interrupt_config_t cb_func_info;
    /* Set callback function and interrupt priority
       if use BGO of the Code Flash or the Data Flash. */
    cb_func_info.pcallback    = r_flash_dm_drvif_callback;
    cb_func_info.int_priority = FLASH_DM_CFG_FRDYI_INT_PRIORITY;
    /* cast from &flash_interrupt_config_t to void* */
    if (R_FLASH_Control(FLASH_CMD_SET_BGO_CALLBACK, (void *)&cb_func_info) != FLASH_SUCCESS)
    {
        return 0;
    }
#endif /* ((FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CFG_CODE_FLASH_BGO == 1)) || \
          ((FLASH_CFG_CODE_FLASH_ENABLE == 0) && (FLASH_CFG_DATA_FLASH_BGO == 1)) */

    return 1;
} /* End of function r_flash_dm_drvif_open() */

#if (FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CFG_CODE_FLASH_BGO == 1)
#pragma section FRAM
#endif

/************************************************************************************************
* Function Name: r_flash_dm_drvif_init_device
* Description  : Initializes the driver interface.
* Arguments    : None
* Return Value : 1: Successful operation
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint8_t r_flash_dm_drvif_init_device(void)
{
    return 1;
} /* End of function r_flash_dm_drvif_init_device() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_erase_error
* Description  : Checks the erase error.
* Arguments    : None
* Return Value : 1: Successful operation
*              : 0: Error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint8_t r_flash_dm_drvif_erase_error(void)
{
    if (1 == g_flash_dm_err_flg)
    {
        g_flash_dm_err_flg = 0;
        return 0;
    }
    return 1;
} /* End of function r_flash_dm_drvif_erase_error() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_program_error
* Description  : Checks the program error.
* Arguments    : None
* Return Value : 1: Successful operation
*              : 0: Error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint8_t r_flash_dm_drvif_program_error(void)
{
    if (1 == g_flash_dm_err_flg)
    {
        g_flash_dm_err_flg = 0;
        return 0;
    }
    return 1;
} /* End of function r_flash_dm_drvif_program_error() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_overwrite_block_flag
* Description  : Overwrites the block flag.
* Arguments    : flash_dm_ptr_t     p_addr      ; Overwrite address
* Return Value : 1: Successful operation
*              : 0: Error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint8_t r_flash_dm_drvif_overwrite_block_flag(flash_dm_ptr_t p_addr)
{
    uint8_t ret = 1;
    flash_dm_ptr_t p = p_addr;
    /* cast from uint8_t to uint8_t */
    if (FLASH_DM_BH_FLAG_FF == p[1])
    {
        /* cast from uint8_t to uint8_t */
        ret = r_flash_dm_drvif_overwrite(&p[1], FLASH_DM_BH_FLAG_00);
    }
    return ret;
} /* End of function r_flash_dm_drvif_overwrite_block_flag() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_get_block_num
* Description  : Gets the number of the block.
* Arguments    : None
* Return Value : The number of the block
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint8_t r_flash_dm_drvif_get_block_num(void)
{
    return g_flash_dm_block_num;
} /* End of function r_flash_dm_drvif_get_block_num() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_get_block_info
* Description  : Gets the information of the block.
* Arguments    : uint8_t    index   ; The block no.
* Return Value : Pointer to the information of the block
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_dm_block_ptr_t r_flash_dm_drvif_get_block_info(uint8_t index)
{
    return &gs_flash_dm_block_info[index];
} /* End of function r_flash_dm_drvif_get_block_info() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_get_block_addr
* Description  : Gets the address of the block.
* Arguments    : uint8_t    index   ; The block no.
* Return Value : Pinter to the address of the block
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_dm_ptr_t r_flash_dm_drvif_get_block_addr(uint8_t index)
{
    return g_r_flash_dm_block_addr[index];
} /* End of function r_flash_dm_drvif_get_block_addr() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_set_addr_h
* Description  : If controls the code flash, sets the upper address of the block.
* Arguments    : st_flash_dispatch_1_hndl_t * p_hndl   ; FLASH DM information
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_flash_dm_drvif_set_addr_h(st_flash_dispatch_1_hndl_t * p_hndl)
{
#if (FLASH_CFG_CODE_FLASH_ENABLE == 1)
    /* cast from uint8_t to uint8_t */
    p_hndl->dh.addr_h = FLASH_DM_ADDR_MASK_H;

#if (FLASH_CF_BLOCK_SIZE == 2048)
    /* cast from uint8_t to uint32_t */
    p_hndl->dh.rsv = FLASH_DM_DH_FLAG_FF;   /* Reserve */
#endif /* (FLASH_CF_BLOCK_SIZE == 2048) */

#else
    /* Do nothing */
#endif /* (FLASH_CFG_CODE_FLASH_ENABLE == 1) */
} /* End of function r_flash_dm_drvif_set_addr_h() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_get_addr
* Description  : Gets the lower address of the block.
* Arguments    : st_flash_dm_dh_t * p_dh_hndl  ; FLASH DM data header information
* Return Value : The lower address of the block
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint32_t r_flash_dm_drvif_get_addr(flash_dm_dh_ptr_t p_dh_hndl)
{
#if (FLASH_CFG_CODE_FLASH_ENABLE == 1)
    /* cast from uint8_t to uint32_t */
    return ((FLASH_DM_ADDR_MASK | ((uint32_t)((uint32_t)p_dh_hndl->addr_h << 16))) | ((uint32_t)p_dh_hndl->addr));
#else
    /* cast from uint16_t to uint32_t */
    return (FLASH_DM_ADDR_MASK | p_dh_hndl->addr);
#endif
} /* End of function r_flash_dm_drvif_get_addr() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_get_block_erase_flag
* Description  : Gets block_erase_flag.
* Arguments    : None
* Return Value : Pointer of block_erase_flag
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_dm_ptr_t r_flash_dm_drvif_get_block_erase_flag(void)
{
    /* cast from uint8_t to flash_dm_ptr_t */
    return (flash_dm_ptr_t)&g_r_flash_dm_block_erase_flag[0];
} /* End of function r_flash_dm_drvif_get_block_erase_flag() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_set_dh_flag
* Description  : Sets data header_flag.
* Arguments    : flash_dm_t * p_flag    ; Pointer to the header flag
*              : flash_dm_t data        ; data
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_flash_dm_drvif_set_dh_flag(flash_dm_t * p_flag, flash_dm_t data)
{
    /* cast from uint8_t to uint8_t */
    *p_flag++ = FLASH_DM_DH_EXIST_FLAG;
#if (FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CF_BLOCK_SIZE == 2048)
    /* The header area is doubled */
    /* cast from uint8_t to uint8_t */
    *p_flag++ = FLASH_DM_DH_EXIST_FLAG;
#endif /* (FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CF_BLOCK_SIZE == 2048) */
} /* End of function r_flash_dm_drvif_set_dh_flag() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_overwrite
* Description  : Overwrites the block flag and busy wait.
* Arguments    : flash_dm_ptr_t     p_addr  ; Pointer to the overwrite address
*              : flash_dm_t         value   ; Overwrite value
* Return Value : 1: Successful operation
*              : 0: Error
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash, place the source code to the RAM.
*************************************************************************************************/
static uint8_t r_flash_dm_drvif_overwrite(flash_dm_ptr_t p_addr, flash_dm_t value)
{
    flash_err_t ret = FLASH_SUCCESS;
    uint32_t loop_cnt = FLASH_DM_BUSY_WAIT;     /* Wait = (1ms * FLASH_DM_BUSY_WAIT) */
    uint8_t  index = 0;
    flash_dm_t buf[FLASH_DM_PRG_BUF_INDEX];

    /* WAIT_LOOP */
    for (index = 0; index < FLASH_DM_PRG_BUF_INDEX; index++)
    {
        buf[index] = value;
    }

    if (r_flash_dm_drvif_write(p_addr, &buf[0]) == FLASH_DM_ERR)
    {
        return 0;
    }

    /* WAIT_LOOP */
    do
    {
        ret = R_FLASH_Control(FLASH_CMD_STATUS_GET, NULL);
        if (FLASH_SUCCESS == ret)
        {
            return 1;
        }
        else if (FLASH_ERR_BUSY != ret)
        {
            break;
        }
        else
        {
            /* Busy */
        }

        if (true != r_flash_dm_softwaredelay(1, FLASH_DM_DELAY_MILLISECS))  /* 1ms */
        {
            break;
        }
        loop_cnt--;
    }
    while (0 != loop_cnt);

    return 0;
} /* End of function r_flash_dm_drvif_overwrite() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_api_flag
* Description  : Checks API flag.
* Arguments    : st_flash_dispatch_1_hndl_t * p_hndl   ; FLASH DM information
* Return Value : 1: Unlock
*              : 0: Lock
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash, place the source code to the RAM.
*************************************************************************************************/
uint8_t r_flash_dm_drvif_api_flag(st_flash_dispatch_1_hndl_t * p_hndl)
{
    /* cast from uint32_t to uint32_t */
    int32_t is_locked = FLASH_DM_API_CALL;

    /* ==== Check API calling ==== */
    xchg(&is_locked, &p_hndl->api_call);
    /* cast from uint32_t to uint32_t */
    if (FLASH_DM_API_CALL == is_locked)
    {
        return 1;   /* Unlock */
    }
    return 0;       /* Lock */
}

/************************************************************************************************
* Function Name: r_flash_dm_drvif_write
* Description  : Writes the data.
* Arguments    : flash_dm_ptr_t     p_addr      ; Pointer to the write address
*              : flash_dm_ptr_t     p_value     ; Pointer to the write value
* Return Value : FLASH_DM_ACCEPT                ; Successful operation
*              : FLASH_DM_ERR                   ; Error
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash, place the source code to the RAM.
*************************************************************************************************/
e_flash_dm_status_t r_flash_dm_drvif_write(flash_dm_ptr_t p_addr, flash_dm_ptr_t p_value)
{
    flash_err_t ret = FLASH_SUCCESS; /* debug */
#if !defined(FLASH_DM_DEBUG)
    g_flash_dm_err_flg = 0;
#endif /* !defined(FLASH_DM_DEBUG) */

#if (FLASH_HAS_ROM_CACHE == 1) && (FLASH_CFG_CODE_FLASH_ENABLE == 1)
    uint8_t tmp_status = 0;

    /* Get ROM cache status. */
    if (!r_flash_dm_drvif_get_romcache(&tmp_status))
    {
        g_flash_dm_err_flg = 1;
        return FLASH_DM_ERR;
    }

    if (1 == tmp_status) /* Enable ROM cache. */
    {
        /* Disable ROM cache. */
        if (!r_flash_dm_drvif_set_romcache(FLASH_DM_ROM_CACHE_DISABLE))
        {
            g_flash_dm_err_flg = 1;
            r_flash_dm_drvif_set_romcache(FLASH_DM_ROM_CACHE_ENABLE);
            return FLASH_DM_ERR;
        }
    }
#endif /* (FLASH_HAS_ROM_CACHE == 1) && (FLASH_CFG_CODE_FLASH_ENABLE == 1) */

    /* ==== Write data. ==== */
    g_flash_dispatch_1_hndl->flash_state = DATF_FLASH_CALL;
    /* cast from uint32_t to uint32_t */
    ret = R_FLASH_Write((uint32_t)p_value, (uint32_t)p_addr, (uint32_t)FLASH_DF_MIN_PGM_SIZE);
    if(ret != FLASH_SUCCESS)
    {

        g_flash_dm_err_flg = 1;
#if (FLASH_HAS_ROM_CACHE == 1) && (FLASH_CFG_CODE_FLASH_ENABLE == 1)
        if (1 == tmp_status) /* Enable ROM cache. */
        {
            r_flash_dm_drvif_set_romcache(FLASH_DM_ROM_CACHE_ENABLE);
        }
#endif /* (FLASH_HAS_ROM_CACHE == 1) && (FLASH_CFG_CODE_FLASH_ENABLE == 1) */
        return FLASH_DM_ERR;
    }

#if (FLASH_HAS_ROM_CACHE == 1) && (FLASH_CFG_CODE_FLASH_ENABLE == 1)
    if (1 == tmp_status) /* Enable ROM cache. */
    {
        /* Enable ROM cache */
        if (!r_flash_dm_drvif_set_romcache(FLASH_DM_ROM_CACHE_ENABLE))
        {
            g_flash_dm_err_flg = 1;
            r_flash_dm_drvif_set_romcache(FLASH_DM_ROM_CACHE_ENABLE);
            return FLASH_DM_ERR;
        }
    }
#endif /* (FLASH_HAS_ROM_CACHE == 1) && (FLASH_CFG_CODE_FLASH_ENABLE == 1) */

    return FLASH_DM_ACCEPT;
} /* End of function r_flash_dm_drvif_write() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_erase
* Description  : Erases the 1 block.
* Arguments    : flash_dm_ptr_t     p_addr  ; Pointer to the erase block address
* Return Value : FLASH_DM_ACCEPT            ; Successful operation
*              : FLASH_DM_ERR               ; Error
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash, place the source code to the RAM.
*************************************************************************************************/
e_flash_dm_status_t r_flash_dm_drvif_erase(flash_dm_ptr_t p_addr)
{
#if !defined(FLASH_DM_DEBUG)
    g_flash_dm_err_flg = 0;
#endif /* !defined(FLASH_DM_DEBUG) */

    /* ==== Disable ROM cache. ==== */
#if (FLASH_HAS_ROM_CACHE == 1) && (FLASH_CFG_CODE_FLASH_ENABLE == 1)
    uint8_t tmp_status = 0;

    /* Get ROM cache status. */
    if (!r_flash_dm_drvif_get_romcache(&tmp_status))
    {
        g_flash_dm_err_flg = 1;
        return FLASH_DM_ERR;
    }

    if (1 == tmp_status) /* Enable ROM cache. */
    {
        /* Disable ROM cache. */
        if (!r_flash_dm_drvif_set_romcache(FLASH_DM_ROM_CACHE_DISABLE))
        {
            g_flash_dm_err_flg = 1;
            r_flash_dm_drvif_set_romcache(FLASH_DM_ROM_CACHE_ENABLE);
            return FLASH_DM_ERR;
        }
    }
#endif /* (FLASH_HAS_ROM_CACHE == 1) && (FLASH_CFG_CODE_FLASH_ENABLE == 1) */

    /* ==== Erase block. ==== */
    g_flash_dispatch_1_hndl->flash_state = DATF_FLASH_CALL;
    /* cast from uint8_t to uint8_t */
    if (R_FLASH_Erase((flash_block_address_t)p_addr, 1) != FLASH_SUCCESS)
    {
        g_flash_dm_err_flg = 1;
#if (FLASH_HAS_ROM_CACHE == 1) && (FLASH_CFG_CODE_FLASH_ENABLE == 1)
        if (1 == tmp_status) /* Enable ROM cache. */
        {
            r_flash_dm_drvif_set_romcache(FLASH_DM_ROM_CACHE_ENABLE);
        }
#endif /* (FLASH_HAS_ROM_CACHE == 1) && (FLASH_CFG_CODE_FLASH_ENABLE == 1) */
        return FLASH_DM_ERR;
    }

    /* ==== Enable ROM cache. ==== */
#if (FLASH_HAS_ROM_CACHE == 1) && (FLASH_CFG_CODE_FLASH_ENABLE == 1)
    if (1 == tmp_status) /* Enable ROM cache. */
    {
        /* Enable ROM cache */
        if (!r_flash_dm_drvif_set_romcache(FLASH_DM_ROM_CACHE_ENABLE))
        {
            g_flash_dm_err_flg = 1;
            r_flash_dm_drvif_set_romcache(FLASH_DM_ROM_CACHE_ENABLE);
            return FLASH_DM_ERR;
        }
    }
#endif /* (FLASH_HAS_ROM_CACHE == 1) && (FLASH_CFG_CODE_FLASH_ENABLE == 1) */

    return FLASH_DM_ACCEPT;
} /* End of function r_flash_dm_drvif_erase() */

#if (FLASH_CFG_CODE_FLASH_ENABLE == 1)
/************************************************************************************************
* Function Name: r_flash_dm_drvif_protect
* Description  : Executes protect setting.
*              : FLASH_DM_PROTECTION_ENABLE  : Set access window.
*              :                               The ROM area of this driver management can be accessed.
*              : FLASH_DM_PROTECTION_DISABLE : Release access window.
*              :                               All ROM areas can be accessed.
* Arguments    : uint8_t    protect     ; FLASH_DM_PROTECTION_ENABLE
*              :                        ; FLASH_DM_PROTECTION_DISABLE
* Return Value : 1: Successful operation
*              : 0: Error
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash, place the source code to the RAM.
*************************************************************************************************/
uint8_t r_flash_dm_drvif_protect(uint8_t protect)
{
    flash_access_window_config_t access_info;
    uint32_t start_addr = 0;
    uint32_t end_addr = 0;

    if (FLASH_DM_PROTECTION_ENABLE == protect)
    {
        /* Set access window.
           The ROM area of this driver management can be accessed. */
        start_addr = (uint32_t)r_flash_dm_drvif_get_block_addr(FLASH_DM_CFG_CF_BLOCK_NUM - 1);
        /* cast from uint32_t to uint32_t */
        end_addr = (uint32_t)((uint32_t)r_flash_dm_drvif_get_block_addr(0) + FLASH_DM_FLASH_BLOCK_SIZE);

        access_info.start_addr = start_addr;
        access_info.end_addr = end_addr;
        /* cast from uint8_t to uint8_t */
        g_flash_dispatch_1_hndl->activity = FLASH_DM_ACT_PROTECTING;
        /* cast from flash_access_window_config_t to void* */
        if (R_FLASH_Control(FLASH_CMD_ACCESSWINDOW_SET, (void *)&access_info) != FLASH_SUCCESS)
        {
            return 0;
        }
    }
    else
    {
        /* Release access window.
           All ROM areas can be accessed. */
        start_addr = (uint32_t)r_flash_dm_drvif_get_block_addr(FLASH_DM_CFG_CF_BLOCK_NUM - 1);
        end_addr = start_addr;

        access_info.start_addr = start_addr;
        access_info.end_addr = end_addr;
        /* cast from uint8_t to uint8_t */
        g_flash_dispatch_1_hndl->activity = FLASH_DM_ACT_PROTECTING;
        /* cast from flash_access_window_config_t to void* */
        if (R_FLASH_Control(FLASH_CMD_ACCESSWINDOW_SET, (void *)&access_info) != FLASH_SUCCESS)
        {
            return 0;
        }
    }

    return 1;
} /* End of function r_flash_dm_drvif_protect() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_set_romcache
* Description  : Executes rom cache setting.
* Arguments    : uint8_t    rom_cache           ; FLASH_DM_ROM_CACHE_ENABLE
*              :                                ; FLASH_DM_ROM_CACHE_DISABLE
* Return Value : 1: Successful operation
*              : 0: Error
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash, place the source code to the RAM.
*************************************************************************************************/
uint8_t r_flash_dm_drvif_set_romcache(uint8_t rom_cache)
{
#if (FLASH_HAS_ROM_CACHE == 1)
    if (FLASH_DM_ROM_CACHE_ENABLE == rom_cache)
    {
        if (R_FLASH_Control(FLASH_CMD_ROM_CACHE_ENABLE, NULL) != FLASH_SUCCESS)
        {
            return 0;
        }
    }
    else
    {
        if (R_FLASH_Control(FLASH_CMD_ROM_CACHE_DISABLE, NULL) != FLASH_SUCCESS)
        {
            return 0;
        }
    }
    return 1;
#else
    return 0;
#endif /* (FLASH_HAS_ROM_CACHE == 1) */
} /* End of function r_flash_dm_drvif_set_romcache() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_get_romcache
* Description  : Gets rom cache status.
* Arguments    : uint8_t    * p_status      ;  Pointer to the rom cache stutas.
* Return Value : 1: Successful operation
*              : 0: Error
*------------------------------------------------------------------------------------------------
* Notes        : If control the code flash, place the source code to the RAM.
*************************************************************************************************/
uint8_t r_flash_dm_drvif_get_romcache(uint8_t * p_status)
{
#if (FLASH_HAS_ROM_CACHE == 1)
    if (R_FLASH_Control(FLASH_CMD_ROM_CACHE_STATUS, &p_status) != FLASH_SUCCESS)
    {
        return 0;
    }
    return 1;
#else
    return 0;
#endif /* (FLASH_HAS_ROM_CACHE == 1) */
} /* End of function r_flash_dm_drvif_get_romcache() */
#endif /* (FLASH_CFG_CODE_FLASH_ENABLE == 1) */

/************************************************************************************************
* Function Name: r_flash_dm_block_init
* Description  : Initializes block variables.
* Arguments    : None
* Return Value : 1: Successful operation
*              : 0: Error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
uint8_t r_flash_dm_block_init(void)
{
    uint8_t b_ret = 0;

    gs_flash_dm_b_unit = gs_flash_dm_transfer_unit;
    b_ret = r_flash_dm_drvif_init_device();
    return b_ret;
} /* End of function r_flash_dm_block_init() */

/************************************************************************************************
* Function Name: r_flash_dm_block_program_init
* Description  : Initializes block program variables.
* Arguments    : flash_dm_ptr_t     p_src   ; Pointer of the start address of source
*              : uint16_t           counter ; Counter of program
*              : flash_dm_t         data    ; Value of program
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_flash_dm_block_program_init(flash_dm_ptr_t p_src, uint16_t counter, flash_dm_t data)
{
    uint8_t index = 0;

    g_flash_dispatch_1_hndl->flash_dm_a_src = p_src;
    gs_flash_dm_b_counter = counter;
    /* WAIT_LOOP */
    for (index = 0; index < FLASH_DM_PRG_BUF_INDEX; index++)
    {
        gs_flash_dm_b_data[index] = data;
    }
    /* cast from uint8_t to uint8_t */
    gs_flash_dm_c_mode = FLASH_DM_MODE_PROGRAM;
} /* End of function r_flash_dm_block_program_init() */

/************************************************************************************************
* Function Name: r_flash_dm_block_erase_init
* Description  : Initializes block erase variables.
* Arguments    : None
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_flash_dm_block_erase_init(void)
{
    /* cast from uint8_t to uint8_t */
    gs_flash_dm_c_mode = FLASH_DM_MODE_ERASE;
} /* End of function r_flash_dm_block_erase_init() */

/************************************************************************************************
* Function Name: r_flash_dm_block_copy_setup
* Description  : Checks program completion and set program data.
* Arguments    : None
* Return Value : 1: Not finish program and set program data.
*              : 0: Finish program
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static uint8_t r_flash_dm_block_copy_setup(void)
{
    uint8_t index = 0;

    /* If data is 0xff, omit the program. */
    /* WAIT_LOOP */
    while ((gs_flash_dm_b_counter > 0) &&
            /* cast from uint8_t to uint8_t */
            (((flash_dm_t)FLASH_DM_DH_FLAG_FF) == (*g_flash_dispatch_1_hndl->flash_dm_a_src)))
    {
        g_flash_dispatch_1_hndl->flash_dm_a_addr++;
        gs_flash_dm_b_counter--;
        g_flash_dispatch_1_hndl->flash_dm_a_src++;
    }

    /* Check to finish program. */
    if (0 == gs_flash_dm_b_counter)
    {
        return 0;
    }
    /* WAIT_LOOP */
    for (index = 0; index < FLASH_DM_PRG_BUF_INDEX; index++)
    {
        gs_flash_dm_b_data[index] = *g_flash_dispatch_1_hndl->flash_dm_a_src++;
    }

    /* cast from flash_dm_ptr_t to uint32_t */
    g_flash_dispatch_1_hndl->flash_dm_a_src = (flash_dm_ptr_t)((uint32_t)g_flash_dispatch_1_hndl->flash_dm_a_src -
            /* cast from flash_dm_t to uint32_t */
            (uint32_t)(sizeof(flash_dm_t) * FLASH_DM_PRG_BUF_INDEX));

    if (gs_flash_dm_b_counter > gs_flash_dm_transfer_unit)
    {
        gs_flash_dm_b_unit = gs_flash_dm_transfer_unit;
    }
    else
    {
        /* cast from uint16_t to uint8_t */
        gs_flash_dm_b_unit = (uint8_t)gs_flash_dm_b_counter;
    }
    return 1;
} /* End of function r_flash_dm_block_copy_setup() */

/************************************************************************************************
* Function Name: r_flash_dm_block_fill_setup
* Description  : Checks program completion and update the address and counter of program.
* Arguments    : None
* Return Value : 1: Not finish program and update program data.
*              : 0: Finish program
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static uint8_t r_flash_dm_block_fill_setup(void)
{
    /* WAIT_LOOP */
    while ((gs_flash_dm_b_counter > 0) &&
            /* cast from uint8_t to uint8_t */
            ((FLASH_DM_DH_FLAG_FF != (*g_flash_dispatch_1_hndl->flash_dm_a_addr)) &&
             (0 != gs_flash_dm_b_data[0])))
    {
        g_flash_dispatch_1_hndl->flash_dm_a_addr++;
        gs_flash_dm_b_counter--;
    }

    /* Check to finish program. */
    if (0 == gs_flash_dm_b_counter)
    {
        return 0;
    }
    gs_flash_dm_b_unit = gs_flash_dm_transfer_unit;

    return 1;
} /* End of function r_flash_dm_block_fill_setup() */

/************************************************************************************************
* Function Name: r_flash_dm_block_program_setup
* Description  : Sets block program.
* Arguments    : None
* Return Value : FLASH_DM_TRUE  : Not finish program and set program data.
*              : FLASH_DM_FALSE : Finish program
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
static e_flash_dm_state_t r_flash_dm_block_program_setup(void)
{
    uint8_t ret = 0;

    if (gs_flash_dm_b_counter > 0)
    {
        if (0 != g_flash_dispatch_1_hndl->flash_dm_a_src)
        {
            ret = r_flash_dm_block_copy_setup();
        }
        else
        {
            ret = r_flash_dm_block_fill_setup();
        }
    }

    if (1 == ret)
    {
        return FLASH_DM_TRUE;
    }
    else
    {
        return FLASH_DM_FALSE;
    }
} /* End of function r_flash_dm_block_program_setup() */

/************************************************************************************************
* Function Name: r_flash_dm_block_program_check
* Description  : Checks program.
* Arguments    : None
* Return Value : FLASH_DM_PROGRAM_CHECK_END     ; Successful program operation
*              : FLASH_DM_PROGRAM_ERROR         ; Program error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
e_flash_dm_state_t r_flash_dm_block_program_check(void)
{
    uint8_t index = 0;

    if (!r_flash_dm_drvif_program_error())
    {
        return FLASH_DM_PROGRAM_ERROR;
    }
    else
    {
        gs_flash_dm_b_counter -= gs_flash_dm_b_unit;
        /* cast from uint32_t to flash_dm_ptr_t */
        g_flash_dispatch_1_hndl->flash_dm_a_addr = (flash_dm_ptr_t)((uint32_t)g_flash_dispatch_1_hndl->flash_dm_a_addr + (uint32_t)gs_flash_dm_b_unit);

        if (0 != g_flash_dispatch_1_hndl->flash_dm_a_src)
        {
            /* cast from uint32_t to flash_dm_ptr_t */
            g_flash_dispatch_1_hndl->flash_dm_a_src = (flash_dm_ptr_t)((uint32_t)g_flash_dispatch_1_hndl->flash_dm_a_src + (uint32_t)gs_flash_dm_b_unit);
            /* WAIT_LOOP */
            for (index = 0; index < FLASH_DM_PRG_BUF_INDEX; index++)
            {
                gs_flash_dm_b_data[index] = *g_flash_dispatch_1_hndl->flash_dm_a_src++;
            }
            /* cast from uint32_t to flash_dm_ptr_t */
            g_flash_dispatch_1_hndl->flash_dm_a_src = (flash_dm_ptr_t)((uint32_t)g_flash_dispatch_1_hndl->flash_dm_a_src -
                    /* cast from flash_dm_t to uint32_t */
                    (uint32_t)(sizeof(flash_dm_t) * FLASH_DM_PRG_BUF_INDEX));
        }
    }
    return FLASH_DM_PROGRAM_CHECK_END;
} /* End of function r_flash_dm_block_program_check() */

/************************************************************************************************
* Function Name: r_flash_dm_block_erase_check
* Description  : Checks erase.
* Arguments    : None
* Return Value : FLASH_DM_ERASE_END     ; Successful erase operation
*              : FLASH_DM_ERASE_ERROR   ; Erase error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
e_flash_dm_state_t r_flash_dm_block_erase_check(void)
{
    if (!r_flash_dm_drvif_erase_error())
    {
        return FLASH_DM_ERASE_ERROR;
    }
    return FLASH_DM_ERASE_END;
} /* End of function r_flash_dm_block_erase_check() */

/************************************************************************************************
* Function Name: r_flash_dm_block_command
* Description  : Issues the program command or erase command.
* Arguments    : st_flash_dispatch_1_hndl_t * p_hndl   ; FLASH DM information
* Return Value : FLASH_DM_PROGRAM_END       ; Finish program
*              : FLASH_DM_COMMAND_END       ; Finish to issue the command
*              : FLASH_DM_COMMAND_FALSE     ; Error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
e_flash_dm_state_t r_flash_dm_block_command(st_flash_dispatch_1_hndl_t * p_hndl)
{
    /* cast from uint8_t to uint8_t */
    if (FLASH_DM_MODE_PROGRAM == gs_flash_dm_c_mode)
    {
        if (r_flash_dm_block_program_setup() == FLASH_DM_FALSE)
        {
            return FLASH_DM_PROGRAM_END;
        }

        if (r_flash_dm_drvif_write(g_flash_dispatch_1_hndl->flash_dm_a_addr, &gs_flash_dm_b_data[0]) == FLASH_DM_ERR)
        {
            return FLASH_DM_COMMAND_FALSE;
        }
    }
    /* cast from uint8_t to uint8_t */
    else if (FLASH_DM_MODE_ERASE == gs_flash_dm_c_mode)
    {
        if (r_flash_dm_drvif_erase(g_flash_dispatch_1_hndl->flash_dm_a_addr) == FLASH_DM_ERR)
        {
            return FLASH_DM_COMMAND_FALSE;
        }
    }
    else
    {
        return FLASH_DM_COMMAND_FALSE;
    }
    return FLASH_DM_COMMAND_END;
} /* End of function r_flash_dm_block_command() */

/************************************************************************************************
* Function Name: r_flash_dm_drvif_callback
* Description  : Executes call back function.
* Arguments    : void * event ; Pointer to the event
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_flash_dm_drvif_callback(void * event)
{
    flash_int_cb_args_t *ready_event = event;

    if (FLASH_INT_EVENT_ERR_DF_ACCESS <= ready_event->event)
    {
        g_flash_dm_err_flg = 1; /* Error */
    }

    g_flash_dispatch_1_hndl->flash_state = DATF_FLASH_IDLE;/* Due to call APIs. */

    switch (ready_event->event)
    {
        case    FLASH_INT_EVENT_ERASE_COMPLETE:
        case    FLASH_INT_EVENT_WRITE_COMPLETE:
        {
            g_flash_dispatch_1_hndl->error_code = r_flash_dm_advance();
        }
        break;

        default:
        {
            /* Do Nothing */
        }
        break;
    }

    if (0 != r_flash_dm_callbackfunc)
    {
        if(g_flash_dispatch_1_hndl->error_code == FLASH_DM_ERR_REQUEST_INIT)
        {
            switch(g_flash_dispatch_1_hndl->activity)
            {
                case    FLASH_DM_ACT_FORMATTING:
                {
                    /* cast from e_flash_dm_status_t to void* */
                    r_flash_dm_callbackfunc((void*)FLASH_DM_ERR_FORMAT);
                }
                break;

                case    FLASH_DM_ACT_WRITING:
                {
                    /* cast from e_flash_dm_status_t to void* */
                    r_flash_dm_callbackfunc((void*)FLASH_DM_ERR_WRITE);
                }
                break;

                case    FLASH_DM_ACT_ERASING:
                {
                    /* cast from e_flash_dm_status_t to void* */
                    r_flash_dm_callbackfunc((void*)FLASH_DM_ERR_ERASE);
                }
                break;

                case    FLASH_DM_ACT_RECLAIMING:
                {
                    /* cast from e_flash_dm_status_t to void* */
                    r_flash_dm_callbackfunc((void*)FLASH_DM_ERR_RECLAIM);
                }
                break;

                default :
                {
                    r_flash_dm_callbackfunc((void*)FLASH_DM_ERR);
                }
            }
        }
    }
}

/************************************************************************************************
* Function Name: r_flash_dm_set_dm
* Description  : Copy the handle's address.
* Arguments    : uint32_t   handle   ; Current handle
* Return Value : void
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_flash_dm_set_dm(uint32_t handle)
{
    /* cast from uint32_t to st_flash_dispatch_1_hndl_t* */
    g_flash_dispatch_1_hndl = (st_flash_dispatch_1_hndl_t*)handle;
}

#if (FLASH_CFG_CODE_FLASH_ENABLE == 1) && (FLASH_CFG_CODE_FLASH_BGO == 1)
#pragma section
#endif

#endif /* (FLASH_TYPE == FLASH_TYPE_1) */

/* End of File */

