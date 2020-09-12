/**********************************************************************************************************************
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
* Copyright (C) 2014(2015-2019) Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/
/**********************************************************************************************************************
* File Name    : r_sdc_sd_rx_private.h
* Description  : SDC SD for the RX
***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 05.09.2014 1.00    First Release
*              : 17.07.2015 1.10    Standardized by the combo.
*              : 30.09.2016 1.21    Definitions for exchanging data by endian structure type (sdc_sd_exchg_short_t).
*              :                    Added r_sdc_sdio_clear_crc16(), r_sdc_sdio_set_crc16() and r_sdc_sdio_get_crc16().
*              :                    Added r_sdc_sdio_chk_support_type().
*              : 31.07.2017 2.00    SDHI FIT module separated into hardware low level layer and middleware layer.
*              :                    Changed prefix from SDHI to SDC_SD.
*              : 29.06.2018 2.02    Corresponded to SD Specifications Part1 Physical Layer Simplified Specification.
*              : 10.02.2020 3.00    Changed macro "SDC_SD_BIG_ENDIAN".
**********************************************************************************************************************/

#ifndef SDC_SD_RX_PRIVATE_H
#define SDC_SD_RX_PRIVATE_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "platform.h"
#include "r_sdc_sd_rx_config.h"
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
#include ".\src\sdio\r_sdc_sdio_target.h"
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */

/******************************************************************************
Error checking
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
#ifdef SDC_SD_CFG_LONGQ_ENABLE              /* Uses FIT LONGQ module */
#define SDC_SD_DRIVER_ID            (5)     /* sdc sd debug log id */
#define SDC_SD_DEBUG_ERR_ID         (1)     /* sdc sd debug log error id */
#endif /* SDC_SD_CFG_LONGQ_ENABLE */

#define SDC_SD_BIG_ENDIAN           (defined(__BIG))

/* ==== option ==== */
#define SDC_SD_UNMOUNT_CARD         (0x00)
#define SDC_SD_MOUNT_UNLOCKED_CARD  (0x01)
#define SDC_SD_MOUNT_LOCKED_CARD    (0x02)
#define SDC_SD_CARD_LOCKED          (0x04)

/* --- R1 response error bit ---- */
#define SDC_SD_RES_SW_INTERNAL      (0xe8400000ul)  /* Driver illegal process */
                                                    /* OUT_OF_RANGE */
                                                    /* ADDRESS_ERROR */
                                                    /* BLOCK_LEN_ERROR */
                                                    /* ERASE_PARAM */
                                                    /* RES_ILLEGAL_COMMAND */
#define SDC_SD_RES_ERASE_SEQ_ERROR          (0x10008000ul)  /* ERASE_SEQ_ERROR + WP_ERASE_SKIP */
#define SDC_SD_RES_WP_VIOLATION             (0x04000000ul)
#define SDC_SD_RES_CARD_IS_LOCKED           (0x02000000ul)
#define SDC_SD_RES_CARD_UNLOCKED_FAILED     (0x01000000ul)
#define SDC_SD_RES_COM_CRC_ERROR            (0x00800000ul)
#define SDC_SD_RES_CARD_ECC_FAILED          (0x00200000ul)
#define SDC_SD_RES_CC_ERROR                 (0x00100000ul)
#define SDC_SD_RES_ERROR                    (0x00080000ul)
#define SDC_SD_RES_AKE_SEQ_ERROR            (0x00000008ul)
#define SDC_SD_RES_STATE                    (0x00001e00ul)
#define SDC_SD_RES_READY_FOR_DATA           (0x00000100ul)
#define SDC_SD_RES_ERR                      (0xffffe008ul)
#define SDC_SD_RES_ERR_UPPER                (0xfff90000ul)
#define SDC_SD_RES_ERR_IGNORE_LOCK          (0xfdffe008ul)
#define SDC_SD_RES_ERR_IGNORE_OUT_OF_RAGE   (0x7fffe008ul)

/* ---- Shift values ---- */
#define SDC_SD_BIT_SHIFT_1          (1u)
#define SDC_SD_BIT_SHIFT_2          (2u)
#define SDC_SD_BIT_SHIFT_3          (3u)
#define SDC_SD_BIT_SHIFT_4          (4u)
#define SDC_SD_BIT_SHIFT_5          (5u)
#define SDC_SD_BIT_SHIFT_6          (6u)
#define SDC_SD_BIT_SHIFT_7          (7u)
#define SDC_SD_BIT_SHIFT_8          (8u)
#define SDC_SD_BIT_SHIFT_9          (9u)
#define SDC_SD_BIT_SHIFT_10         (10u)
#define SDC_SD_BIT_SHIFT_11         (11u)
#define SDC_SD_BIT_SHIFT_12         (12u)
#define SDC_SD_BIT_SHIFT_13         (13u)
#define SDC_SD_BIT_SHIFT_14         (14u)
#define SDC_SD_BIT_SHIFT_15         (15u)
#define SDC_SD_BIT_SHIFT_16         (16u)
#define SDC_SD_BIT_SHIFT_17         (17u)
#define SDC_SD_BIT_SHIFT_18         (18u)
#define SDC_SD_BIT_SHIFT_19         (19u)
#define SDC_SD_BIT_SHIFT_20         (20u)
#define SDC_SD_BIT_SHIFT_21         (21u)
#define SDC_SD_BIT_SHIFT_22         (22u)
#define SDC_SD_BIT_SHIFT_23         (23u)
#define SDC_SD_BIT_SHIFT_24         (24u)
#define SDC_SD_BIT_SHIFT_25         (25u)
#define SDC_SD_BIT_SHIFT_26         (26u)
#define SDC_SD_BIT_SHIFT_27         (27u)
#define SDC_SD_BIT_SHIFT_28         (28u)
#define SDC_SD_BIT_SHIFT_29         (29u)
#define SDC_SD_BIT_SHIFT_30         (30u)
#define SDC_SD_BIT_SHIFT_31         (31u)

/* --- Current state --- */
#define SDC_SD_STATE_IDLE           (0)
#define SDC_SD_STATE_READY          (1u << 9u)
#define SDC_SD_STATE_IDENT          (2u << 9u)
#define SDC_SD_STATE_STBY           (3u << 9u)
#define SDC_SD_STATE_TRAN           (4u << 9u)
#define SDC_SD_STATE_DATA           (5u << 9u)
#define SDC_SD_STATE_RCV            (6u << 9u)
#define SDC_SD_STATE_PRG            (7u << 9u)
#define SDC_SD_STATE_DIS            (8u << 9u)
#define SDC_SDIO_STATE_DIS          (0)
#define SDC_SDIO_STATE_CMD          (1u << 12u)
#define SDC_SDIO_STATE_TRN          (2u << 12u)
#define SDC_SDIO_STATE_RFU          (3u << 12u)
/* ---- Trans block size ---- */
#define SDC_SD_TRANS_BLOCK_SIZE     (512)   /* 1 block */

/* ---- Maximum block count per multiple command ---- */
#define SDC_SDMEM_TRANS_SECTORS     (p_hndl->sdmem_trans_sectors)   /* max. 65535 blocks */

#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
#define SDC_SDIO_TRANS_SECTORS      (p_hndl->sdio_trans_sectors)    /* max. 511 blocks */
#endif /* SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO */

/* ---- Time out count ---- */
#define SDC_SD_TIMEOUT_CMD          (100)       /* Command timeout */
#define SDC_SD_TIMEOUT_MULTIPLE     (1000)      /* Block transfer timeout */
#define SDC_SD_TIMEOUT_RESP         (1000)      /* Command sequence timeout */
#define SDC_SD_TIMEOUT_RESP48       (1000)      /* CMD48 timeout 1s */
#define SDC_SD_TIMEOUT_DMA_END      (1000)      /* DMA transfer timeout */
#define SDC_SD_TIMEOUT_ERASE_CMD    (10000)     /* Erase timeout */
#define SDC_SD_TIMEOUT_PROG_CMD     (10000)     /* Programming timeout */
#define SDC_SD_TIMEOUT_FORCE_CMD    (600000)    /* Force erase timeout */

/* ---- Data transfer direction ---- */
#define SDC_SD_TRANS_READ           (0)         /* Host <- SD */
#define SDC_SD_TRANS_WRITE          (1)         /* SD -> Host */

/* ---- card register size ---- */
#define SDC_SD_STATUS_DATA_BYTE     (64)        /* STATUS_DATA size */
#define SDC_SD_STATUS_BYTE          (64)        /* SD STATUS size */
#define SDC_SD_SCR_REGISTER_BYTE    (8)         /* SCR register size */

/* ---- area distinction ---- */
#define SDC_SD_USER_AREA            (1u)
#define SDC_SD_PROT_AREA            (2u)

/* --- specification version ---- */
#define SDC_SD_SPEC_10              (0)         /* Ver.1.01 */
#define SDC_SD_SPEC_11              (1)         /* Ver.1.10 */
#define SDC_SD_SPEC_20              (2)         /* Ver.2.00 */

/* --- SD Card Speed ---- */
#define SDC_SD_CUR_SPEED            (0x01u)     /* Current high-speed mode */
#define SDC_SD_CUR_SPEED_LOW        (0x02u)     /* Current low-speed mode */
#define SDC_SD_SUP_SPEED            (0x10u)     /* Supported high-speed mode */
#define SDC_SD_SUP_SPEED_LOW        (0x20u)     /* Supported low-speed mode */

/* ==== format parameter ==== */
#define SDC_SD_SIZE_CARD_256KB      (256*1024/SDC_SD_TRANS_BLOCK_SIZE)          /*  256*1KB/(sector size) */
#define SDC_SD_SIZE_CARD_1MB        (1024*1024/SDC_SD_TRANS_BLOCK_SIZE)         /* 1024*1KB/(sector size) */
#define SDC_SD_SIZE_CARD_2MB        (2*1024*1024/SDC_SD_TRANS_BLOCK_SIZE)       /*    2*1MB/(sector size) */
#define SDC_SD_SIZE_CARD_4MB        (4*1024*1024/SDC_SD_TRANS_BLOCK_SIZE)       /*    4*1MB/(sector size) */
#define SDC_SD_SIZE_CARD_8MB        (8*1024*1024/SDC_SD_TRANS_BLOCK_SIZE)       /*    8*1MB/(sector size) */
#define SDC_SD_SIZE_CARD_16MB       (16*1024*1024/SDC_SD_TRANS_BLOCK_SIZE)      /*   16*1MB/(sector size) */
#define SDC_SD_SIZE_CARD_32MB       (32*1024*1024/SDC_SD_TRANS_BLOCK_SIZE)      /*   32*1MB/(sector size) */
#define SDC_SD_SIZE_CARD_64MB       (64*1024*1024/SDC_SD_TRANS_BLOCK_SIZE)      /*   64*1MB/(sector size) */
#define SDC_SD_SIZE_CARD_128MB      (128*1024*1024/SDC_SD_TRANS_BLOCK_SIZE)     /*  128*1MB/(sector size) */
#define SDC_SD_SIZE_CARD_256MB      (256*1024*1024/SDC_SD_TRANS_BLOCK_SIZE)     /*  256*1MB/(sector size) */
#define SDC_SD_SIZE_CARD_504MB      (504*1024*1024/SDC_SD_TRANS_BLOCK_SIZE)     /*  504*1MB/(sector size) */
#define SDC_SD_SIZE_CARD_1008MB     (1008*1024*1024/SDC_SD_TRANS_BLOCK_SIZE)    /* 1008*1MB/(sector size) */
#define SDC_SD_SIZE_CARD_1024MB     (1024*1024*1024/SDC_SD_TRANS_BLOCK_SIZE)    /* 1024*1MB/(sector size) */
#define SDC_SD_SIZE_CARD_2016MB     (2016*1024*1024/SDC_SD_TRANS_BLOCK_SIZE)    /* 2016*1MB/(sector size) */
#define SDC_SD_SIZE_CARD_2048MB     (2048ul*1024ul*1024ul / (uint32_t)SDC_SD_TRANS_BLOCK_SIZE)
                                                                                /* 2048*1MB/(sector size) */
#define SDC_SD_SIZE_CARD_4032MB     (4032ul*1024ul*2ul)                         /* 4032*(1MB/sector size) */
#define SDC_SD_SIZE_CARD_4096MB     (4096ul*1024ul*2ul)                         /* 4096*(1MB/sector size) */
#define SDC_SD_SIZE_CARD_8192MB     (8192ul*1024ul*2ul)                         /* 2048*(1MB/sector size) */
#define SDC_SD_SIZE_CARD_16384MB    (16384ul*1024ul*2ul)                        /* 2048*(1MB/sector size) */
#define SDC_SD_SIZE_CARD_32768MB    (32768ul*1024ul*2ul)                        /* 2048*(1MB/sector size) */
#define SDC_SD_SIZE_CARD_128GB      (128ul*1024ul*1024ul*2ul)                   /*  128*(1GB/sector size) */
#define SDC_SD_SIZE_CARD_512GB      (512ul*1024ul*1024ul*2ul)                   /*  512*(1GB/sector size) */
#define SDC_SD_SIZE_CARD_2TB        (0xFFFFFFFF)                                /*    2*(1TB/sector size) */
                                                                                /*  over 32bit max value! */

#define SDC_SD_SECTOR_SIZE          (SDC_SD_TRANS_BLOCK_SIZE)

/* Maximum AU size */
#define SDC_SD_ERASE_SECTOR         ((4096 * 1024) / SDC_SD_SECTOR_SIZE)

/* ==== Macro functions ==== */
#define SDC_SD_GET_HNDL(a)          (g_sdc_sd_handle[(a)])


/* ==== Response ==== */
#define SDC_SD_R5_CARD_STATUS_ERR   (0x0000cb00ul)
#define SDC_SD_R6_NEW_PUBLISHED_RCA (0xffff0000ul)
#define SDC_SD_R7_CHK_PATTERN       (0x000000aaul)
#define SDC_SD_R7_CHK_PATTERN_MASK  (0x000000fful)
#define SDC_SD_R7_VOLT_ACCEPT_HIGH  (0x00ff8000ul)
#define SDC_SD_R7_VOLT_ACCEPT_LOW   (0x00000f00ul)

/* ==== CSD ==== */
#define SDC_SD_CSD_STRUCTURE        (0xc0000000ul)
#define SDC_SD_CSD_TRAN_SPEED       (0x00000007ul)
#define SDC_SD_CSD_TIME_VALUE       (0x00000078ul)
#define SDC_SD_CSD_TRAN_SPEED_5000  (5000)
#define SDC_SD_CSD_TRAN_SPEED_2500  (2500)
#define SDC_SD_CSD_TRAN_SPEED_2000  (2000)
#define SDC_SD_CSD_TRAN_SPEED_1000  (1000)
#define SDC_SD_CSD_TRAN_SPEED_500   (500)
#define SDC_SD_CSD_TRAN_SPEED_100   (100)
#define SDC_SD_CSD_CCC              (0xfff00000ul)
#define SDC_SD_CSD_PROTECT          (0x00003000ul)
#define SDC_SD_CSD_ERASE_SEC_SIZE   (0x00003f80ul)
#define SDC_SD_CSD_ERASE_GRP_SIZE   (0x0000007ful)
#define SDC_SD_CSD_READ_BL_LEN      (0x000f0000ul)
#define SDC_SD_CSD_C_SIZE_MULT      (0x00038000ul)
#define SDC_SD_CSD_STR_VER_1_0      (0x00)
#define SDC_SD_CSD_STR_VER_2_0      (0x01)
#define SDC_SD_CSD_2_0_C_SIZE_UPPER (0x0000003ful)
#define SDC_SD_CSD_2_0_C_SIZE_LOWER (0xffff0000ul)
#define SDC_SD_CSD_1_0_C_SIZE_UPPER (0x000003fful)
#define SDC_SD_CSD_1_0_C_SIZE_LOWER (0xc0000000ul)
#define SDC_SD_CSD_DSR_IMP          (0x00001000ul)
#define SDC_SD_CSD_TRAN_SPEED_MASK  (0x000000fful)
#define SDC_SD_CSD_TRAN_SPEED_50MHz (0x0000005aul)

/* ==== SCR ==== */
#define SDC_SD_SCR_SD_SPEC_2_0      (0x02000000ul)
#define SDC_SD_SCR_SD_SPEC_MASK     (0x0f000000ul)

/* ==== DSR ==== */
#define SDC_SD_DSR_DEFAULT          (0x00000404ul)

/* ==== SD Status ==== */
#define SDC_SD_STATUS_SD_CARD_TYPE_UPPER    (0x0000ff00ul)
#define SDC_SD_STATUS_SD_CARD_TYPE_LOWER    (0x000000fful)
#define SDC_SD_STATUS_SPEED_CLASS           (0xff000000ul)
#define SDC_SD_STATUS_PERFORMANCE_MOVE      (0x00ff0000ul)
#define SDC_SD_STATUS_AU_SIZE               (0x0000000ful)
#define SDC_SD_STATUS_ERASE_SIZE            (0x0000ff00ul)

/* ==== IF_COND ==== */
#define SDC_SD_IF_COND_2_7_TO_3_6       (0x00000100ul)
#define SDC_SD_IF_COND_LOW_RANGE        (0x00000200ul)
#define SDC_SD_IF_COND_VOLT_SUP_MASK    (0x00000f00ul)

#define SDC_SD_SCLKDIVEN_LOOP_COUNT     (10000) /* SCLKDIVEN bit loop count */

#define SDC_SD_TIMER_CARD_MAX_NUM       (8)
#define SDC_SD_TIMER_CARD_FLG           (0)
#define SDC_SD_TIMER_CARD_COUNT         (1)
#define SDC_SD_TIMER_CARD_MAX_COUNT     (2)

#ifdef SDC_SD_CFG_LONGQ_ENABLE      /* Uses FIT LONGQ module */
#define SDC_SD_LOG_FUNC(x, y, z)     (r_sdc_sd_log((x), (y), (z)))
#else
#define SDC_SD_LOG_FUNC(x, y, z)
#endif /* SDC_SD_CFG_LONGQ_ENABLE */

#define SDC_SD_CALLBACK_DISABLE         (0)
#define SDC_SD_CALLBACK_ENABLE          (1)

#ifdef SDC_SD_CFG_LONGQ_ENABLE    /* Uses FIT LONGQ module */
typedef enum e_sdc_sd_log
{
    SDC_SD_OPEN             =  0,
    SDC_SD_INT              =  1,
    SDC_SD_REGISTER         =  2,
    SDC_SD_CD               =  3,
    SDC_SD_CMD              =  4,
    SDC_SD_INITIALIZE       =  5,
    SDC_SD_READ             =  6,
    SDC_SD_WRITE            =  7,
    SDC_SD_TRANS            =  8,
    SDC_SD_UTIL             =  9,
    SDC_SD_DEV              =  10,
    SDC_SD_UPCASE_TABLE     =  11,
    SDC_SD_FORMAT           =  12,
    SDC_SD_WRAPPER          =  13,
    SDC_SD_CONFIG           =  14,
    SDC_SDMEM_INITIALIZE    =  20,
    SDC_SDMEM_READ          =  21,
    SDC_SDMEM_WRITE         =  22,
    SDC_SDMEM_UTIL          =  23,
    SDC_SDMEM_PARAM         =  24,
    SDC_SDMEM_WRAPPER       =  25,
    SDC_SDIO_DIRECT         =  40,
    SDC_SDIO_INT            =  41,
    SDC_SDIO_INITIALIZE     =  42,
    SDC_SDIO_READ           =  43,
    SDC_SDIO_WRITE          =  44,
    SDC_SDIO_UTIL           =  45,
    SDC_SDIO_TOOL           =  46,
    SDC_SDIO_TARGET         =  47,
    SDC_SDIO_PARAM          =  48,
    SDC_SDIO_WRAPPER        =  49,
    SDC_SDCOMBO_INITIALIZE  =  50
} sdc_sd_logid_t;
#endif /* SDC_SD_CFG_LONGQ_ENABLE */

/* ==== Format parameter structure ==== */
typedef struct _sdc_sd_chs_recom_t
{
    uint32_t capa;
    uint8_t  heads;
    uint8_t  spt;
    uint8_t  rsvd[2];   /* Reserved */
} sdc_sd_chs_recom_t;

typedef struct _sdc_sd_scbu_recom_t
{
    uint32_t capa;
    uint32_t sc;
    uint32_t bu;
} sdc_sd_scbu_recom_t;


/* ==== SD Driver work buffer (allocated by File system) ==== */
/* ---- ""VAL DEF"" ---- */
typedef struct __sdc_sd_hndl_t                          /* SD handle */
{
    uint8_t         if_mode;                            /* Bus width (1bit:0 4bits:1) */
    uint8_t         speed_mode;                         /* Card speed mode;           */
                                                        /*     Current speed   : 0    */
                                                        /*     Supported speed :0x10  */
    uint8_t         io_speed_mode;                      /* IO speed mode              */
    uint8_t         media_type;                         /* Card type */
    uint8_t         mount;                              /* Memory Mount flag              */
                                                        /*     SDC_SD_UNMOUNT_CARD        */
                                                        /*     SDC_SD_MOUNT_UNLOCKED_CARD */
                                                        /*     SDC_SD_MOUNT_LOCKED_CARD   */
                                                        /*     SDC_SD_CARD_LOCKED         */
    uint8_t         int_mode;                           /* Interrupt flag detect method   */
                                                        /*       Polling:0                */
                                                        /*       H/W interrupt:1          */
    uint8_t         trans_mode;                         /* Data transfer method           */
                                                        /*       PIO : 0                  */
                                                        /*       SDHI_BUF DMA : 0x2       */
    uint8_t         sup_card;                           /* Support card;                  */
                                                        /*        Memory (include MMC) : 0 */
                                                        /*        IO : 1                   */
    uint32_t        voltage;                            /* System supplied voltage  */
    sdc_sd_status_t error;                              /* Error detail information */
    uint32_t        resp_status;                        /* R1/R1b response status */
    uint32_t        if_cond[1];                         /* IF_COND value */
    int32_t         sup_if_mode;                        /* Supported bus width (1bit:0 4bits:1) */
    uint32_t        channel;                            /* Driver channel no */
/* ------------------- SD Memory ------------------- */
    uint32_t        card_sector_size;                   /* Sector size (user area) */
    uint32_t        prot_sector_size;                   /* Sector size (protect area) */
    uint32_t        erase_sect;                         /* Erase block size */
    uint32_t        csd_ccc;                            /* CSD command class */
    uint8_t         fat_type;                           /* FAT type (FAT12:1 FAT16:2 FAT32:3 unknown:0)  */
    uint8_t         csd_structure;                      /* CSD structure (Standard capacity:0 High capacity:1) */
    uint8_t         csd_tran_speed;                     /* CSD transfer speed(MEM) */
    uint8_t         sd_spec;                            /* SCR spec version (Ver1.0-1.01:0 Ver1.10:1) */
    uint8_t         speed_class;                        /* Card speed class */
    uint8_t         perform_move;                       /* Card move performance */
    uint8_t         write_protect;                      /* Write protect:         OFF : 0    */
                                                        /*                     H/W WP : 1    */
                                                        /*     CSD  TMP_WRITE_PROTECT : 2    */
                                                        /*     CSD PERM_WRITE_PROTECT : 4    */
                                                        /*     SD ROM                 : 0x10 */
    uint8_t         cd_port;                            /* Card detect method (CD pin:0 DAT3:1) */
    uint32_t        mem_tran_speed;                     /* MEM transfer speed */
    uint32_t        sdsts1;                             /* SDHI_SDSTS1 status */
    uint32_t        sdsts2;                             /* SDHI_SDSTS2 status */
    uint32_t        sdimsk1;                            /* SDHI_SDIMSK1 status */
    uint32_t        sdimsk2;                            /* SDHI_SDIMSK2 status */
    uint32_t        sdiosts;                            /* SDHI_SDIOSTS status */
    uint32_t        sdioimsk;                           /* SDHI_SDIOIMSK status */
    uint32_t        stop;                               /* Compulsory stop flag */
    int32_t         sdmem_trans_sectors;                /* Maximum block counts per multiple command */
    sdc_sd_status_t   (*sdc_sd_int_cd_callback)(int32_t);
    sdc_sd_status_t   (*sdc_sd_int_format_callback)(int32_t);  /* Callback function for card format */
    sdc_sd_status_t   (*sdc_sd_int_callback)(int32_t);
    sdc_sd_status_t   (*sdc_sd_int_io_callback)(int32_t);

    uint32_t        ocr[1];                             /* OCR value */
    uint32_t        cid[4];                             /* CID value */
    uint32_t        csd[4];                             /* CSD value */
    uint32_t        dsr[1];                             /* DSR value */
    uint32_t        rca[2];                             /* RCA value */
    uint32_t        scr[2];                             /* SCR value */
    uint32_t        sdstatus[4];                        /* SD STATUS value */
    uint32_t        status_data[5];                     /* STATUS DATA value */
/* ------------------- SDIO ------------------- */
#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)
    uint8_t         io_flag;                            /* IO initialization flag */
                                                        /*  Interrupt enable        : bit4 */
                                                        /*  Power on initialized    : bit2 */
                                                        /*  Memory initialized      : bit1 */
                                                        /*  IO initialized          : bit0 */
    uint8_t         io_info;                            /* IO function's information */
                                                        /*  IO ready                : bit7      */
                                                        /*  Number of IO function   : bit6-bit4 */
                                                        /*  Memory present          : bit3      */
                                                        /*  Reserved                : bit2-bit0 */
    uint8_t         rsv2[2];                            /* Reserved */
    uint32_t        io_tran_speed;                      /* IO transfer speed */
    int32_t         sdio_trans_sectors;                 /* Maximum block counts per multiple command */
    uint32_t        io_ocr[1];                          /* IO OCR value */
    int32_t         io_len[SDC_SDIO_FUNC_MAXNUM+1];     /* IO block length common:0 func:more than 1 */
    uint8_t         io_reg[SDC_SDIO_FUNC_MAXNUM+1][SDC_SDIO_REG_SIZE]; /* CCCR(=0) and FBR(1 to 7) value */
#endif /* SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO */

}sdc_sd_hndl_t;

/* Definitions for exchanging data by endian structure type */
typedef union
{
    uint16_t    us;
    uint8_t     uc[2];
}sdc_sd_exchg_short_t;

extern sdc_sd_hndl_t * g_sdc_sd_handle[SDC_SD_CFG_CARD_NUM];
extern int32_t g_sdc_sd_api_locks[SDC_SD_CFG_CARD_NUM];
extern uint32_t g_sdc_sd_int_dmacdtcflg[SDC_SD_CFG_CARD_NUM];

/* ==== Prototype ==== */
/* ---- r_sdc_sd_cd.c ---- */
sdc_sd_status_t r_sdc_sd_CdInt(uint32_t card_no, int32_t enable, sdc_sd_status_t (* callback)(int32_t));
sdc_sd_status_t r_sdc_sd_GetCardDetection(uint32_t card_no);
sdc_sd_status_t r_sdc_sd_get_card_detection(uint32_t card_no);

/* ---- r_sdc_sd_cmd.c ---- */
sdc_sd_status_t r_sdc_sd_send_cmd(uint32_t card_no, uint32_t cmd);
sdc_sd_status_t r_sdc_sd_send_acmd(uint32_t card_no, uint32_t cmd, uint32_t arg);
sdc_sd_status_t r_sdc_sd_send_cmd_arg(uint32_t card_no, uint32_t cmd, int32_t resp, uint32_t arg);
sdc_sd_status_t r_sdc_sd_send_ocr(uint32_t card_no, int32_t type);
sdc_sd_status_t r_sdc_sd_check_resp_error(uint32_t card_no);
sdc_sd_status_t r_sdc_sd_check_resp5_error(uint32_t card_no);
sdc_sd_status_t r_sdc_sd_get_resp(uint32_t card_no, int32_t resp);
sdc_sd_status_t r_sdc_sd_check_csd(uint32_t card_no);
sdc_sd_status_t r_sdc_sd_check_sdsts2_err(uint32_t card_no);

/* ---- r_sdc_sd_config.c ---- */
sdc_sd_status_t r_sdc_sd_set_int_callback(uint32_t card_no, uint32_t enable);
sdc_sd_status_t r_sdc_sd_disable_icu_int(uint32_t card_no);
sdc_sd_status_t r_sdc_sd_enable_icu_int(uint32_t card_no);
uint32_t        r_sdc_sd_get_div_init_speed(void);
uint32_t        r_sdc_sd_get_div_default_speed(void);

/* ---- r_sdc_sd_initialize.c ---- */
sdc_sd_status_t r_sdc_sd_initialize(uint32_t card_no, uint32_t init_type);
sdc_sd_status_t r_sdc_sd_Initialize(uint32_t card_no, sdc_sd_cfg_t * p_sdc_sd_config, uint32_t init_type);
sdc_sd_status_t r_sdc_sd_End(uint32_t card_no, uint32_t end_type);

/* ---- r_sdc_sd_int.c ---- */
sdc_sd_status_t r_sdc_sd_set_int_mask(uint32_t card_no, uint32_t mask1, uint32_t mask2);
sdc_sd_status_t r_sdc_sd_clear_int_mask(uint32_t card_no, uint32_t mask1, uint32_t mask2);
sdc_sd_status_t r_sdc_sd_clear_sdsts(uint32_t card_no, uint32_t clear_sdsts1, uint32_t clear_sdsts2);
sdc_sd_status_t r_sdc_sd_get_int(uint32_t card_no, uint32_t sdsts1, uint32_t sdsts2);
sdc_sd_status_t r_sdc_sd_get_intstatus(uint32_t card_no);
sdc_sd_status_t r_sdc_sd_IntCallback(uint32_t card_no, sdc_sd_status_t (* callback)(int32_t));

/* ---- r_sdc_sd_open.c ---- */
sdc_sd_status_t r_sdc_sd_Open(uint32_t card_no, uint32_t channel, void * p_sdc_sd_workarea);
sdc_sd_status_t r_sdc_sd_Close(uint32_t card_no);
sdc_sd_status_t r_sdc_sd_init_hndl(uint32_t card_no, uint32_t mode, uint32_t voltage, uint32_t type);

/* ---- r_sdc_sd_trns.c ---- */
sdc_sd_status_t r_sdc_sd_software_trans(uint32_t card_no, uint8_t * p_buff, int32_t cnt, int32_t dir);
sdc_sd_status_t r_sdc_sd_software_trans_blk(uint32_t card_no, uint8_t * p_buff, int32_t cnt, int32_t dir, int32_t blk,
                                            uint32_t func_no);
sdc_sd_status_t r_sdc_sd_dmacdtc_trans(uint32_t card_no, int32_t cnt);

/* ---- r_sdc_sd_util.c ---- */
uint32_t        r_sdc_sd_iswp(uint32_t card_no);
int32_t         r_sdc_sd_bit_search(uint32_t data);
sdc_sd_status_t r_sdc_sd_set_err(uint32_t card_no, sdc_sd_status_t error);
sdc_sd_status_t r_sdc_sd_GetModeStatus(uint32_t card_no, uint8_t * p_mode);
sdc_sd_status_t r_sdc_sd_GetCardStatus(uint32_t card_no, sdc_sd_card_status_t * p_sdc_sd_card_status);
sdc_sd_status_t r_sdc_sd_GetCardInfo(uint32_t card_no, sdc_sd_card_reg_t * p_sdc_sd_card_reg);
sdc_sd_status_t r_sdc_sd_get_size(uint32_t card_no, uint32_t area);
sdc_sd_status_t r_sdc_sd_GetErrCode(uint32_t card_no);
sdc_sd_status_t r_sdc_sd_Control(uint32_t card_no, sdc_sd_cmd_t * p_sdc_sd_cmd);
sdc_sd_status_t r_sdc_sdmemset(uint8_t * p_ptr, uint8_t data, uint32_t cnt);
sdc_sd_status_t r_sdc_sdmemcpy(uint8_t * p_dst, uint8_t * p_src, uint32_t cnt);
uint32_t        r_sdc_sd_rand(void);
void            r_sdc_sd_srand(uint32_t seed);
sdc_sd_status_t r_sdc_sd_wait_rbusy(uint32_t card_no, uint32_t time);
sdc_sd_status_t r_sdc_sd_GetBuffRegAddress(uint32_t card_no, uint32_t * p_reg_buff);
uint32_t        r_sdc_sd_GetVersion(void);
sdc_sd_status_t r_sdc_sd_SetLogHdlAddress(uint32_t user_long_que);
#ifdef SDC_SD_CFG_LONGQ_ENABLE    /* Uses FIT LONGQ module */
uint32_t        r_sdc_sd_log(uint32_t flg, uint32_t fid, uint32_t line);
#endif /* SDC_SD_CFG_LONGQ_ENABLE */
uint32_t        r_sdc_sd_Log(uint32_t flg, uint32_t fid, uint32_t line);
sdc_sd_status_t r_sdc_sd_read_data(uint8_t * p_buff, uint32_t reg_addr, int32_t num);
sdc_sd_status_t r_sdc_sd_write_data(uint8_t * p_buff, uint32_t reg_addr, int32_t num);
sdc_sd_status_t r_sdc_sd_read_data_byte(uint8_t * p_buff, uint32_t reg_addr, int32_t num);
sdc_sd_status_t r_sdc_sd_write_data_byte(uint8_t * p_buff, uint32_t reg_addr, int32_t num);
sdc_sd_status_t r_sdc_sd_int_wait(uint32_t card_no, uint32_t time);
sdc_sd_status_t r_sdc_sd_int_mem_wait(uint32_t card_no, uint32_t msec);
sdc_sd_status_t r_sdc_sd_int_io_wait(uint32_t card_no, uint32_t time, uint32_t func);
sdc_sd_status_t r_sdc_sd_int_err_mem_wait(uint32_t card_no, uint32_t msec);
sdc_sd_status_t r_sdc_sd_int_err_io_wait(uint32_t card_no, uint32_t time, uint32_t func);
sdc_sd_status_t r_sdc_sd_wait(uint32_t card_no, uint32_t msec);
sdc_sd_status_t r_sdc_sd_SetDmacDtcTransFlg(uint32_t card_no, uint32_t flg);
void            r_sdc_sd_1msInterval(void);
sdc_sd_status_t r_sdc_sd_start_timer(uint32_t card_no, uint32_t msec);
sdc_sd_status_t r_sdc_sd_check_timer(uint32_t card_no);
void            r_sdc_sd_end_timer(uint32_t card_no);
bool            r_sdc_sd_api_lock(int32_t * p_lock);
bool            r_sdc_sd_api_unlock(int32_t * p_lock);

/* ---- r_sdc_sdcombo_initialize.c ---- */
sdc_sd_status_t r_sdc_sdcombo_initialize(uint32_t card_no, uint32_t init_type);

/* ---- r_sdc_sdmem_initialize.c ---- */
sdc_sd_status_t r_sdc_sdmem_initialize(uint32_t card_no, uint32_t init_type);
sdc_sd_status_t r_sdc_sdmem_mount_chk_reg(uint32_t card_no);
sdc_sd_status_t r_sdc_sdmem_mount_get_scr(uint32_t card_no);
sdc_sd_status_t r_sdc_sdmem_init(uint32_t card_no);
sdc_sd_status_t r_sdc_sdmem_mount(uint32_t card_no);
sdc_sd_status_t r_sdc_sdmem_card_get_sdstatus(uint32_t card_no);
sdc_sd_status_t r_sdc_sdmem_card_get_scr(uint32_t card_no);
sdc_sd_status_t r_sdc_sdmem_read_byte(uint32_t card_no, uint32_t cmd, uint32_t arg, uint8_t * p_readbuff, uint32_t byte);
sdc_sd_status_t r_sdc_sdmem_write_byte(uint32_t card_no, uint32_t cmd, uint32_t arg, uint8_t * p_writebuff, uint32_t byte);

/* ---- r_sdc_sdmem_param.c ---- */
sdc_sd_status_t r_sdc_sdmem_chk_param(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access);

/* ---- r_sdc_sdmem_read.c ---- */
sdc_sd_status_t r_sdc_sdmem_Read(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access);
sdc_sd_status_t r_sdc_sdmem_ReadSoftwareTrans(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access);
sdc_sd_status_t r_sdc_sdmem_ReadSoftwareTransSingleCmd(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access);

/* ---- r_sdc_sdmem_util.c ---- */
sdc_sd_status_t r_sdc_sdmem_set_port(uint32_t card_no, int32_t port);
sdc_sd_status_t r_sdc_sdmem_read_write_error(uint32_t card_no);
sdc_sd_status_t r_sdc_sdmem_GetSpeed(uint32_t card_no, uint8_t * p_cls, uint8_t * p_move);

/* ---- r_sdc_sdmem_write.c ---- */
sdc_sd_status_t r_sdc_sdmem_Write(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access);
sdc_sd_status_t r_sdc_sdmem_WriteSoftwareTrans(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access);

/* ---- r_sdc_sdio_crc.c ---- */
void            r_sdc_sdio_clear_crc16(uint32_t card_no);
void            r_sdc_sdio_set_crc16(uint32_t card_no, uint8_t * p_buff);
uint16_t        r_sdc_sdio_get_crc16(uint32_t card_no);

/* ---- r_sdc_sdio_direct.c ---- */
sdc_sd_status_t r_sdc_sdio_ReadDirect(uint32_t card_no, sdc_sdio_daccess_t * p_sdc_sdio_daccess);
sdc_sd_status_t r_sdc_sdio_WriteDirect(uint32_t card_no, sdc_sdio_daccess_t * p_sdc_sdio_daccess);
sdc_sd_status_t r_sdc_sdio_direct(uint32_t card_no, sdc_sdio_daccess_t * p_sdc_sdio_daccess);

/* ---- r_sdc_sdio_initialize.c ---- */
sdc_sd_status_t r_sdc_sdio_initialize(uint32_t card_no, uint32_t init_type);
sdc_sd_status_t r_sdc_sdio_init(uint32_t card_no);
sdc_sd_status_t r_sdc_sdio_mount(uint32_t card_no);
sdc_sd_status_t r_sdc_sdio_set_speed(uint32_t card_no);
sdc_sd_status_t r_sdc_sdio_chk_support_type(uint32_t card_no);

/* ---- r_sdc_sdio_int.c ---- */
sdc_sd_status_t r_sdc_sdio_set_int_mask(uint32_t card_no, uint32_t mask);
sdc_sd_status_t r_sdc_sdio_clear_int_mask(uint32_t card_no, uint32_t mask);
sdc_sd_status_t r_sdc_sdio_clear_sdiosts(uint32_t card_no, uint32_t clear);
sdc_sd_status_t r_sdc_sdio_get_int(uint32_t card_no, uint32_t sdiosts);
sdc_sd_status_t r_sdc_sdio_get_intstatus(uint32_t card_no);
sdc_sd_status_t r_sdc_sdio_IntCallback(uint32_t card_no, sdc_sd_status_t (* callback)(int32_t));
sdc_sd_status_t r_sdc_sdio_enable_int_ioirqm(uint32_t card_no);
sdc_sd_status_t r_sdc_sdio_disable_int_ioirqm(uint32_t card_no);
sdc_sd_status_t r_sdc_sdio_EnableInt(uint32_t card_no);
sdc_sd_status_t r_sdc_sdio_DisableInt(uint32_t card_no);

/* ---- r_sdc_sdio_param.c ---- */
sdc_sd_status_t r_sdc_sdio_chk_param_extend(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access);
sdc_sd_status_t r_sdc_sdio_chk_param_direct(uint32_t card_no, sdc_sdio_daccess_t * p_sdc_sdio_daccess);
sdc_sd_status_t r_sdc_sdio_chk_param_blocklen(uint32_t card_no, uint32_t func);

/* ---- r_sdc_sdio_read.c ---- */
sdc_sd_status_t r_sdc_sdio_Read(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access);
sdc_sd_status_t r_sdc_sdio_ReadSoftwareTrans(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access);
sdc_sd_status_t r_sdc_sdio_read_block_sub(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access);
sdc_sd_status_t r_sdc_sdio_read_byte_sub(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access);

/* ---- r_sdc_sdio_target.c ---- */
uint32_t        r_sdc_sdio_get_func_max_num(void);
int32_t         r_sdc_sdio_get_io_reg_size(void);
int32_t         r_sdc_sdio_get_block_size(uint32_t func);

/* ---- r_sdc_sdio_tool.c ---- */
sdc_sd_status_t r_sdc_sdio_IOAbort(uint32_t card_no, uint32_t func);
sdc_sd_status_t r_sdc_sdio_SetIOEnable(uint32_t card_no, uint32_t func);
sdc_sd_status_t r_sdc_sdio_ClearIOEnable(uint32_t card_no, uint32_t func);
sdc_sd_status_t r_sdc_sdio_GetIOReady(uint32_t card_no, uint8_t * p_data);
sdc_sd_status_t r_sdc_sdio_IOReset(uint32_t card_no);
sdc_sd_status_t r_sdc_sdio_SetIntEnable(uint32_t card_no);
sdc_sd_status_t r_sdc_sdio_ClearIntEnable(uint32_t card_no);
sdc_sd_status_t r_sdc_sdio_GetIntEnable(uint32_t card_no, uint8_t * p_data);
sdc_sd_status_t r_sdc_sdio_GetCIS(uint32_t card_no, sdc_sdio_cis_t * p_sdc_sdio_cis);

/* ---- r_sdc_sdio_util.c ---- */
sdc_sd_status_t r_sdc_sdio_SetBlocklen(uint32_t card_no, int32_t len, uint32_t func);
sdc_sd_status_t r_sdc_sdio_set_blocklen_sub(uint32_t card_no, int32_t len, uint32_t func);
sdc_sd_status_t r_sdc_sdio_GetBlocklen(uint32_t card_no, int32_t * p_len, uint32_t func);
sdc_sd_status_t r_sdc_sdio_read_write_error(uint32_t card_no, uint32_t func);
sdc_sd_status_t r_sdc_sdio_check_retry_cmd_state(uint32_t card_no);
sdc_sd_status_t r_sdc_sdio_set_port(uint32_t card_no, int32_t port);

/* ---- r_sdc_sdio_write.c ---- */
sdc_sd_status_t r_sdc_sdio_Write(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access);
sdc_sd_status_t r_sdc_sdio_WriteSoftwareTrans(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access);
sdc_sd_status_t r_sdc_sdio_write_block_sub(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access);
sdc_sd_status_t r_sdc_sdio_write_byte_sub(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access);

#endif /* SDC_SD_RX_PRIVATE_H */

