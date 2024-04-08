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
* Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* File Name    : r_dispatch_1_df_256b.h
* Version      : 2.30
* Description  : DATFRX interface header file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 31.08.2023 1.00     First Release
*              : 14.03.2024 2.30     Delete macro "FLASH_CFG_CODE_FLASH_ENABLE" judgment
*************************************************************************************************/

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_datfrx_rx_config.h"
#include "r_flash_rx_if.h"
#include "r_flash_dm_rx_if.h"                   /* DATFRX I/F definitions            */

/************************************************************************************************
Macro definitions
*************************************************************************************************/
#ifndef R_DISPATCH_1_DF_256B_H
#define R_DISPATCH_1_DF_256B_H

#if (FLASH_TYPE == FLASH_TYPE_1)

/* Block header */
#if !defined(FLASH_DM_DEBUG)
#define FLASH_DM_BH_FLAG_FF         ((flash_dm_t)(0xff))
#define FLASH_DM_BH_FLAG_00         ((flash_dm_t)(0x00))
#define FLASH_DM_BH_FLAG_AA         ((flash_dm_t)(0xaa))
#endif /* !defined(FLASH_DM_DEBUG) */
#define FLASH_DM_BH_FLAG_55         ((flash_dm_t)(0x55))
#define FLASH_DM_BH_ERASE_FLAG_SIZE ((uint16_t)(8))
#define FLASH_DM_BH_FLAG_SIZE       ((uint16_t)(2))
#define FLASH_DM_BH_ERASE_FLAG_INDEX    ((uint16_t)(FLASH_DM_BH_ERASE_FLAG_SIZE / sizeof(flash_dm_t)))

/* Data header */
#define FLASH_DM_DH_FLAG_FF         ((flash_dm_t)(0xff))
#define FLASH_DM_DH_FLAG_00         ((flash_dm_t)(0x00))
#define FLASH_DM_DH_EXIST_FLAG      ((flash_dm_t)(0x7f))
#define FLASH_DM_DH_VALIDATE_FLAG   ((flash_dm_t)(0xbf))
#define FLASH_DM_DH_VALID_FLAG      ((flash_dm_t)(0xdf))
#define FLASH_DM_DH_VALIDITY_FLAG   ((flash_dm_t)(0x0f))
#define FLASH_DM_DH_INVALID_FLAG    ((flash_dm_t)(0xf0))
#define FLASH_DM_DH_FLAG_SIZE       ((uint16_t)(1))
#define FLASH_DM_FLASH_BLOCK_SIZE   ((uint32_t)(256))
#define FLASH_DM_BLOCK_MAX_MASK     ((uint32_t)(0x0010ff00))
#define FLASH_DM_HEADER_MAX         (((FLASH_DM_FLASH_BLOCK_SIZE - sizeof(st_flash_dm_bh_t)) / sizeof(st_flash_dm_dh_t)))
#define FLASH_DM_STACKSIZE          ((uint8_t)(16))
#define FLASH_DM_MODE_PROGRAM       ((uint8_t)(0x00))
#define FLASH_DM_MODE_ERASE         ((uint8_t)(0x01))
#define FLASH_DM_ADDR_MASK          ((uint32_t)(0x00100000))
#define FLASH_DM_PRG_BUF_INDEX      (1)

#endif /* (FLASH_TYPE == FLASH_TYPE_1) */

/************************************************************************************************
Global Typedef definitions
*************************************************************************************************/
#if (FLASH_TYPE == FLASH_TYPE_1)

typedef uint8_t   * flash_dm_ptr_t;
typedef uint8_t     flash_dm_t;

/* Block header */
#pragma pack
typedef struct
{
    flash_dm_t  erase_start[2]; /*  2 bytes */
    flash_dm_t  erase_end[FLASH_DM_BH_ERASE_FLAG_INDEX]; /* 8 bytes */
    flash_dm_t  ready[2];       /*  2 bytes */
    flash_dm_t  active[2];      /*  2 bytes */
    flash_dm_t  full[2];        /*  2 bytes */
    flash_dm_t  reclaim[2];     /*  2 bytes */
} st_flash_dm_bh_t;                /* 18 bytes */
#pragma packoption

typedef st_flash_dm_bh_t  * flash_dm_bh_ptr_t;

/* Data header */
#pragma pack
typedef struct
{
    flash_dm_t  header_exist[1];    /*  1 byte  */
    uint8_t     no;                 /*  1 byte  */
    uint16_t    addr;               /*  2 bytes */
    flash_dm_t  header_valid[1];    /*  1 byte  */
    flash_dm_t  data_valid[1];      /*  1 byte  */
    flash_dm_t  validity[1];        /*  1 byte  */
} st_flash_dm_dh_t;                    /*  7 bytes */
#pragma packoption

typedef st_flash_dm_dh_t  * flash_dm_dh_ptr_t;

/* Block format */
typedef union
{
    /* cast from uint32_t to uint8_t */
    uint8_t     raw[FLASH_DM_FLASH_BLOCK_SIZE];
    struct
    {
        st_flash_dm_bh_t   b;
        /* cast from uint32_t to st_flash_dm_dh_t */
        st_flash_dm_dh_t   d[FLASH_DM_HEADER_MAX];
    } h;
} u_flash_dm_block_format_t;      /* 256 bytes */

typedef u_flash_dm_block_format_t  * flash_dm_block_format_ptr_t;

/* Block information */
typedef struct
{
    u_flash_dm_block_format_t  * p_img;   /* Block address */
    uint8_t     next;                   /* Next block no. */
    uint8_t     prev;                   /* Previous block no. */
    uint8_t     index;                  /* Block no. */
    uint8_t     rsv[1];
    e_flash_dm_block_status_t   stat;   /* Block state */
} st_flash_dm_block_t;     /* 12 bytes */

typedef st_flash_dm_block_t  * flash_dm_block_ptr_t;

/* Driver information */
typedef struct
{
    e_flash_dm_status_t   error_code;     /* Error code of the last operation */
    st_flash_dm_block_t  * p_reclaim;      /* Reclaim source/garbage/error block*/
    st_flash_dm_block_t  * p_full;         /* Post active block */
    st_flash_dm_block_t  * p_active;       /* Write destination, reclaim destination block */
    st_flash_dm_block_t  * p_new_active;   /* The ready block to become the new active block */
    flash_dm_ptr_t      p_data;         /* For data <= xxx */
    flash_dm_bh_ptr_t   p_bh;           /* Active block header */
    flash_dm_dh_ptr_t   p_dh;           /* For keeping data to be written */
    uint16_t            flash_state;    /* added by datfrx */
    const uint8_t     * p_user_data;    /* user erase block */
    st_flash_dm_dh_t       dh;             /* For keeping data to write */
    uint8_t             rsv1[1];
    int32_t             api_call;       /* API call flag */
    uint16_t            header_write_size;
    uint8_t             header_index;   /* For header <= xxx */
    uint8_t             initialized;    /* Initialized flag */
    uint8_t             activity;       /* Activity */
    uint8_t             counter;        /* Format, reclaim */
    uint8_t             data_top;       /* Init, reclaim */
    uint8_t             sp;             /* Stack pointer */
    uint8_t             nblock;
    flash_dm_ptr_t      flash_dm_a_addr;     /* Pointer of the start address of destination  */
    flash_dm_ptr_t      flash_dm_a_src;      /* Pointer of the start address of source       */
    uint8_t             rsv2[3];
    /* cast from uint8_t to e_flash_dm_driver_status_t */
    e_flash_dm_driver_status_t stack[FLASH_DM_STACKSIZE]; /* Execution stack */
} st_flash_dispatch_1_hndl_t;      /* 140 bytes */

#endif /* (FLASH_TYPE == FLASH_TYPE_1) */

/************************************************************************************************
Exported global variables
*************************************************************************************************/
#if (FLASH_TYPE == FLASH_TYPE_1)

#if defined(FLASH_DM_DEBUG)
extern flash_dm_t FLASH_DM_BH_FLAG_FF;
extern flash_dm_t FLASH_DM_BH_FLAG_00;
extern flash_dm_t FLASH_DM_BH_FLAG_AA;
#endif /* FLASH_DM_DEBUG */

/* user callback */
extern p_flash_dm_callback r_flash_dm_callbackfunc;

#endif /* (FLASH_TYPE == FLASH_TYPE_1) */

/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/

#endif /* R_DISPATCH_1_DF_256B_H */

/* End of File */

