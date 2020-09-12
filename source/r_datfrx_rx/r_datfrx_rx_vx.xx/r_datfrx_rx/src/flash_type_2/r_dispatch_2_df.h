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
* File Name    : r_dispatch_2_df.h
* Version      : 2.01
* Description  : DATFRX data flash interface header file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 28.09.2018 2.00     First Release
*              : 25.01.2019 2.01     English PDF added, Fixed blank check processing
*************************************************************************************************/

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_datfrx_rx_config.h"
#include "r_flash_dm_rx_if.h"         /* DATFRX I/F definitions            */

/************************************************************************************************
Macro definitions
*************************************************************************************************/
#ifndef R_DISPATCH_2_DF_H
#define R_DISPATCH_2_DF_H

#if(FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4)
#if(FLASH_DF_BLOCK_SIZE == 128)
/* physical block size */
#define DATF_F1_DATASIZE            (111)                   /* fixed value */
#define DATF_F2_DATASIZE            (121)                   /* fixed value */
#define DATF_F1_INFO_SIZE           (11)                    /* fixed value */
#define DATF_F2_INFO_SIZE           (3)                     /* fixed value */
#define DATF_F1_CRC_CALC_SIZE       (6)                     /* fixed value */
#define DATF_BLANK                  (0xFFFF)                /* fixed value */

#elif(FLASH_DF_BLOCK_SIZE == 64)
/* physical block size */
#define DATF_F1_DATASIZE            (41)                    /* fixed value */
#define DATF_F2_DATASIZE            (53)                    /* fixed value */
#define DATF_F1_INFO_SIZE           (11)                    /* fixed value */
#define DATF_F2_INFO_SIZE           (3)                     /* fixed value */
#define DATF_F1_CRC_CALC_SIZE       (6)                     /* fixed value */
#define DATF_BLANK                  (0xFFFF)                /* fixed value */

#elif(FLASH_DF_BLOCK_SIZE == 32)
/* physical block size */
#define DATF_F1_DATASIZE            (15)                    /* fixed value */
#define DATF_F2_DATASIZE            (25)                    /* fixed value */
#define DATF_F1_INFO_SIZE           (11)                    /* fixed value */
#define DATF_F2_INFO_SIZE           (3)                     /* fixed value */
#define DATF_F1_CRC_CALC_SIZE       (6)                     /* fixed value */
#define DATF_BLANK                  (0xFFFF)                /* fixed value */

#endif /* FLASH_DF_BLOCK_SIZE */

#define BLANK_ROUND_NUM             (FLASH_DF_MIN_PGM_SIZE - 1)

/************************************************************************************************
Global Typedef definitions
*************************************************************************************************/
typedef uint32_t  * flash_dm_ptr_t;
typedef uint32_t    flash_dm_t;

/* block DATF_STATUS_FORMAT */
#pragma pack
typedef union
{
    uint8_t raw[FLASH_DF_BLOCK_SIZE];
    struct
    {
        uint8_t     data_type;                      /* 1Byte */ /* data_type */
        uint16_t    chain;                          /* 2Byte */ /* block address chain */

        /* DATF_STATUS_FORMAT type */
        union
        {
            /* block format1 */
            struct
            {
                uint16_t    data_no;                /* 2Byte */ /* data No. */
                uint32_t    ser_no;                 /* 4Byte */ /* serial No. and type */
                uint16_t    crc_ccitt;              /* 2Byte */ /* crc16 */
                uint8_t     data[DATF_F1_DATASIZE]; /* 1Byte * DATF_F1DATASIZE */ /* data */
#if(FLASH_TYPE == FLASH_TYPE_2)
                uint16_t    write_end;              /* 4Byte */
#else
                uint32_t    write_end;              /* 4Byte */
#endif
            } f1;

            /* block format2 */
            struct
            {
                uint8_t     data[DATF_F2_DATASIZE]; /* data */
            } f2;

        } ft;
#if(FLASH_TYPE == FLASH_TYPE_2)
        uint16_t    erase_start;                    /* DATF_STATUS_ERASE start flag */
        uint16_t    erase_end;                      /* DATF_STATUS_ERASE end flag */
#else
        uint32_t    erase_start;                    /* DATF_STATUS_ERASE start flag */
        uint32_t    erase_end;                      /* DATF_STATUS_ERASE end flag */
#endif
    } bf;
} u_flash_dispatch_2_hndl_block_t;

/* driver's environment */
typedef struct
{
    e_flash_dm_status_t error_code;                 /* Error_code of the last operation */
    int32_t    api_call;                            /* 4Bytes  */ /* this variable is used for xchg(). */
    uint32_t   max_serial_no;                       /* 4Bytes  */
#if(FLASH_TYPE == FLASH_TYPE_2)
    uint16_t   *program_data;                       /* source */
#elif(FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4)
    uint32_t   *program_data;                       /* 4Bytes  */  /* source */
#endif
    uint32_t   command_addr;                        /* 4Bytes  */  /* destination */
    u_flash_dispatch_2_hndl_block_t b;              /* 4Bytes  */
    uint16_t   block_no;                            /* 2Bytes  */
    uint16_t   erase_block_num;                     /* 2Bytes  */
    uint16_t   invalid_block_num;                   /* 2Bytes  */
    uint16_t   erase_chain_first;                   /* 2Bytes  */
    uint16_t   erase_chain_last;                    /* 2Bytes  */
    uint16_t   invalid_chain_first;                 /* 2Bytes  */
    uint16_t   invalid_chain_last;                  /* 2Bytes  */
    uint16_t   block_num;                           /* 2Bytes  */
    uint16_t   writeblock_cnt;                      /* 2Bytes  */
    uint16_t   data_no;                             /* 2Bytes  */
    uint16_t   program_counter;                     /* 2Bytes  */
    uint16_t   flash_state;                         /* 2Bytes  */
    uint16_t   blankcheck_result;                   /* 2Bytes  */
    uint8_t    *user_data;                          /* 4Bytes  */
    uint8_t    sp;                                  /* 1Byte   */
    uint8_t    initialized;                         /* 1Byte   */
    uint8_t    activity;                            /* 1Byte   */
    uint8_t    stack[16];                           /* 16Bytes */
    uint16_t   data_info[FLASH_DM_CFG_DF_DATA_NUM]; /* 2Bytes  * FLASH_DM_CFG_DF_DATA_NUM */
} st_flash_dispatch_2_hndl_t;                       /* 73Bytes * FLASH_DM_CFG_DF_DATA_NUM Bytes*/

/* The structure of block information table */
typedef struct
{
    uint8_t     block_type;
    uint16_t    chain;
} st_flash_dispatch_2_hndl_blk_info_t;
#pragma packoption

/************************************************************************************************
Exported global variables
*************************************************************************************************/

/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/

#endif /* (FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4) */

#endif /* R_DISPATCH_2_DF_H */

/* End of File */

