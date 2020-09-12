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
* File Name    : r_mmcif_rx_private.h
* Device       : RX64M
* Tool-Chain   : RX Family C Compiler
* H/W Platform : 
* Description  : MMC for the RX64M
***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 03.09.2014 1.00    First Release
*              : 28.12.2016 1.01    Added r_mmcif_dev_hardware_lock() and r_mmcif_dev_hardware_unlock().
*              : 20.05.2019 1.05    Added support for GNUC and ICCRX.
*                                   Fixed coding style.
**********************************************************************************************************************/

#ifndef _MMC_PRIVATE_H
#define _MMC_PRIVATE_H

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
/* User specific options for Flash API */
#include "r_mmcif_rx_config.h"

#ifdef MMC_CFG_USE_FIT
/* Includes board and MCU related header files. */
#include "platform.h"
#endif  /* MMC_CFG_USE_FIT */


/**********************************************************************************************************************
Error checking
**********************************************************************************************************************/
#ifdef MMC_CFG_USE_FIT
/* Make sure that a valid version of the r_bsp package is present. */
#if (R_BSP_VERSION_MAJOR < 0)
    #error "ERROR !!! MMC API requires r_bsp >= v0.01"
#endif

/* This catches to make sure the user specified a MCU clock */
#if !defined(BSP_ICLK_HZ)
    #error "ERROR !!! You must specify the System Clock Frequency (BSP_ICLK_HZ)!"
#endif
#endif  /* MMC_CFG_USE_FIT */


/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
/* ==== Uses FIT LongQ module for getting error log. ==== */
#ifdef MMC_CFG_LONGQ_ENABLE                         /* Uses FIT LongQ module                                        */
#define MMC_DRIVER_ID               (5)             /* MMC debug log id                                             */
#define MMC_DEBUG_ERR_ID            (1)             /* MMC debug log error id                                       */
#endif /* MMC_CFG_LONGQ_ENABLE */

/* ==== endian ==== */
#define MMC_BIG_ENDIAN              (defined(__BIG) || defined(__RX_BIG_ENDIAN__))

/* ==== mount status ==== */
#define MMC_UNMOUNT_CARD            (0x00)          /* Unmount                                                      */
#define MMC_MOUNT_UNLOCKED_CARD     (0x01)          /* Mount and unlocked card                                      */
#define MMC_MOUNT_LOCKED_CARD       (0x02)          /* Mount and locked card                                        */
#define MMC_CARD_LOCKED             (0x04)          /* Card locked                                                  */

/* ==== MMC register address ==== */
#define MMC_CECMDSET                (0x00u)         /* Command Set                                                  */
#define MMC_CEARG                   (0x08u)         /* Command Argument                                             */
#define MMC_CEARGCMD12              (0x0cu)         /* Command Argument (CMD12 Auto Issue)                          */
#define MMC_CECMDCTRL               (0x10u)         /* Command Control                                              */
#define MMC_CEBLOCKSET              (0x14u)         /* Block Set                                                    */
#define MMC_CECLKCTRL               (0x18u)         /* Clock Control                                                */
#define MMC_CEBUFACC                (0x1cu)         /* Buffer Access Control                                        */
#define MMC_CERESP3                 (0x20u)         /* R2[127:96]                                                   */
#define MMC_CERESP2                 (0x24u)         /* R2[95:64]                                                    */
#define MMC_CERESP1                 (0x28u)         /* R2[63:32]                                                    */
#define MMC_CERESP0                 (0x2cu)         /* R2[31:0] or Response[39:8] but for R2                        */
#define MMC_CERESPCMD12             (0x30u)         /* R1[39:8] (CMD12 Auto Issue)                                  */
#define MMC_CEDATA                  (0x34u)         /* Buffer Access                                                */
#define MMC_CEBOOT                  (0x3cu)         /* Boot Operation set register                                  */
#define MMC_CEINT                   (0x40u)         /* Interrupt Flag                                               */
#define MMC_CEINTEN                 (0x44u)         /* Interrupt Flag Mask                                          */
#define MMC_CEHOSTSTS1              (0x48u)         /* Host Status (Command Sequence Status)                        */
#define MMC_CEHOSTSTS2              (0x4cu)         /* Host Status (Error Status)                                   */
#define MMC_CEDETECT                (0x70u)         /* Device Detect Control                                        */
#define MMC_CEADDMODE               (0x74u)         /* Special Mode                                                 */
#define MMC_CEVERSION               (0x7cu)         /* Host Version                                                 */

/* --- R1 response error bit ---- */
#define MMC_RES_SW_INTERNAL         (0xe8400000ul)  /* Driver illegal process                                       */
                                                            /* OUT_OF_RANGE                                         */
                                                            /* ADDRESS_ERROR                                        */
                                                            /* BLOCK_LEN_ERROR                                      */
                                                            /* ERASE_PARAM                                          */
                                                            /* RES_ILLEGAL_COMMAND                                  */
#define MMC_RES_ERASE_SEQ_ERROR         (0x10008000ul)  /* ERASE_SEQ_ERROR + WP_ERASE_SKIP                          */
#define MMC_RES_WP_VIOLATION            (0x04000000ul)
#define MMC_RES_CARD_IS_LOCKED          (0x02000000ul)
#define MMC_RES_CARD_UNLOCKED_FAILED    (0x01000000ul)
#define MMC_RES_COM_CRC_ERROR           (0x00800000ul)
#define MMC_RES_CARD_ECC_FAILED         (0x00200000ul)
#define MMC_RES_CC_ERROR                (0x00100000ul)
#define MMC_RES_ERROR                   (0x00080000ul)
#define MMC_RES_AKE_SEQ_ERROR           (0x00000008ul)
#define MMC_RES_STATE                   (0x00001e00ul)
#define MMC_RES_SWITCH_ERROR            (0x00000080ul)

/* --- Current_state --- */
#define MMC_STATE_IDLE              (0)
#define MMC_STATE_READY             (1u<<9u)
#define MMC_STATE_IDENT             (2u<<9u)
#define MMC_STATE_STBY              (3u<<9u)
#define MMC_STATE_TRAN              (4u<<9u)
#define MMC_STATE_DATA              (5u<<9u)
#define MMC_STATE_RCV               (6u<<9u)
#define MMC_STATE_PRG               (7u<<9u)
#define MMC_STATE_DIS               (8u<<9u)

/* ---- Trans block size ---- */
#define MMC_TRANS_BLOCK_SIZE        (512)           /* 1 block                                                      */

/* ---- Maximum block count per multiple command ---- */
#define MMCMEM_TRANS_SECTORS        (p_hndl->mmcmem_trans_sectors)  /* Max. 65535 blocks                            */

/* ---- Set block address, if HC card ---- */
#define MMC_SET_ACC_ADDR            ((p_hndl->density_type >= 0x01) ? (p_mmc_Access->lbn) : \
                                    (p_mmc_Access->lbn * MMC_TRANS_BLOCK_SIZE))

/* ---- MMC clock control ---- */
#define MMC_CLOCK_ENABLE            (1)             /* Supply clock.                                                */
#define MMC_CLOCK_DISABLE           (0)             /* Halt clock.                                                  */

/* ---- RCA number ---- */
#define MMC_INIT_RCA_NUM            (0x0001)

/* ---- SWITCH command (CMD6) setting ---- */
#define MMC_SWITCH_CMD_SET          (0x00000000ul)  /* Access bits 00'b means command set.
                                                       Index and value fields are ignored.                          */
#define MMC_SWITCH_WRITE_BYTE       (0x03000000ul)  /* Access bits 11'b means write byte.                           */
#define MMC_SWITCH_BUS_WIDTH        (0x03b70000ul)  /* Access bits 11b, index field is BUS_WIDTH[183]               */
#define MMC_SWITCH_HS_TIMING        (0x03b90100ul)  /* Access bits 11b, index field is HS_TIMING [185]              */
#define MMC_SWITCH_DS_TIMING        (0x03b90000ul)  /* Access bits 11b, index field is HS_TIMING [185]
                                                       backwards compatibility interface timing */

/* ---- Card Command Class setting ---- */
#define MMC_CCC_CLASS0              (0x00000001ul)  /* Card command class 0                                         */
#define MMC_CCC_CLASS1              (0x00000002ul)  /* Card command class 1                                         */
#define MMC_CCC_CLASS2              (0x00000004ul)  /* Card command class 2                                         */
#define MMC_CCC_CLASS3              (0x00000008ul)  /* Card command class 3                                         */
#define MMC_CCC_CLASS4              (0x00000010ul)  /* Card command class 4                                         */
#define MMC_CCC_CLASS5              (0x00000020ul)  /* Card command class 5                                         */
#define MMC_CCC_CLASS6              (0x00000040ul)  /* Card command class 6                                         */
#define MMC_CCC_CLASS7              (0x00000080ul)  /* Card command class 7                                         */
#define MMC_CCC_CLASS8              (0x00000100ul)  /* Card command class 8                                         */
#define MMC_CCC_CLASS9              (0x00000200ul)  /* Card command class 9                                         */
#define MMC_CCC_CLASS10             (0x00000400ul)  /* Card command class 10                                        */
#define MMC_CCC_CLASS11             (0x00000800ul)  /* Card command class 11                                        */


/* ==== MMC Register ==== */
/* ---- Definnition of Command Setting Register (CECMDSET) ---- */
#define MMC_CECMDSET_MASK_DATW      (0x00000003ul)  /* Mask data bus width setting                                  */
#define MMC_CECMDSET_MASK_RES_TYPE  (0x000c0000ul)  /* Mask resopnse type                                           */
#define MMC_CECMDSET_MASK_CMD       (0xcf000000ul)  /* Mask command index                                           */
#define MMC_CECMDSET_CMD12EN        (0x00010000ul)  /* Issues CMD12 automatically (= automatic CMD12)               */


/* ---- Definnition of Automatically Issued CMD12 Argument Register (CEARGCMD12) ---- */
#define MMC_CEARGCMD12_INIT         (0x00000000ul)  /* All init                                                     */

/* ---- Definition of Command Control Register (CECMDCTRL) ---- */
#define MMC_CECMDCTRL_INIT          (0x00000000ul)  /* All init                                                     */
#define MMC_CECMDCTRL_STOP          (0x00000001ul)  /* Stop command sequence                                        */

/* ---- Definnition of Transfer Block Setting Register (CEBLOCK_SET) ---- */
#define MMC_CEBLOCKSET_BLOCK_SIZE   ((0x00000000ul) | MMC_TRANS_BLOCK_SIZE)     /* Trans block size (1 block)       */

/* ---- Definition of Clock Control Register (CECLKCTRL) ---- */
#define MMC_CECLKCTRL_MASK_CLKEN    (0x01000000ul)  /* CLKEN                                                        */
#define MMC_CECLKCTRL_MASK_MMCBUSBSY (0x80000000ul) /* MMC Clock Output Control / Division Ratio Setting Disable    */
#define MMC_CECLKCTRL_MASK_TIMEOUT  (0x00003ff0ul)  /* Mask time out                                                */

/* ---- Definnition of Buffer Access Configuration Register (CEBUFACC) ---- */
#define MMC_CEBUFACC_MASK_DMARWEN   (0x07000000ul)  /* Mask DMA transfer                                            */
#define MMC_CEBUFACC_DMA_WRITE      (0x06000000ul)  /* Enable DMA transfer request to write. And set to DMATYP bit. */
#define MMC_CEBUFACC_DMA_READ       (0x05000000ul)  /* Enable DMA transfer request to read.  And set to DMATYP bit. */
#if (MMC_BIG_ENDIAN)                                /*     Big endian                                               */
    #define MMC_CEBUFACC_ENDIAN     (0x00000000ul)  /* When swapped byte-wise.                                      */
#else                                               /*     Little endian                                            */
    #define MMC_CEBUFACC_ENDIAN     (0x00010000ul)  /* When not swapped byte-wise.                                  */
#endif  /* #ifdef MMC_BIG_ENDIAN */

/* ---- Definnition of Boot Mode Setting Register (CEBOOT) ---- */
#define MMC_CEBOOT_TIMEOUT          (MMC_CFG_TIMEOUT_BOOT_DATA | MMC_CFG_TIMEOUT_BOOT_FIRSTDATA | MMC_CFG_TIMEOUT_BOOT_ACKNOW)

/* ---- Definition of Interrupt Flag Register (CEINT) ---- */
#define MMC_CEINT_INIT              (0xf80c30e0ul)  /* All init                                                     */

#define MMC_CEINT1_BUFR_COMP12      (0x0400u)       /* Buffer read complete for CMD12                               */
#define MMC_CEINT1_RESPB_COMP12     (0x0200u)       /* Resp busy complete for CMD12                                 */
#define MMC_CEINT1_RESP_COMP12      (0x0100u)       /* Command resp complete for CMD12                              */
#define MMC_CEINT1_WRITE_COMP       (0x0080u)       /* Data write complete                                          */
#define MMC_CEINT1_READ_COMP        (0x0040u)       /* Data read complete                                           */
#define MMC_CEINT1_BUFW_ENAB        (0x0020u)       /* Buffer write enable                                          */
#define MMC_CEINT1_BUFR_ENAB        (0x0010u)       /* Buffer read enable                                           */
#define MMC_CEINT1_RESPB_COMP       (0x0002u)       /* Resp busy complete                                           */
#define MMC_CEINT1_RESP_COMP        (0x0001u)       /* Command resp complete                                        */
#define MMC_CEINT1_TRNSW_RESP       (0x0081u)       /* Write complete and command resp                              */
#define MMC_CEINT1_TRNSR_RESP       (0x0041u)       /* Read complete and command resp                               */
#define MMC_CEINT1_TRNSW_ENAB       (0x00a0u)       /* Write complete and write enable                              */
#define MMC_CEINT1_TRNSR_ENAB       (0x0050u)       /* Read complete and read enable                                */
#define MMC_CEINT1_ALL_INT          (0x07f3u)       /* All int flag                                                 */

#define MMC_CEINT2_CMD_ERR          (0x8000u)       /* Command error                                                */
#define MMC_CEINT2_BUFACC_ERR       (0x4000u)       /* Buffer access error                                          */
#define MMC_CEINT2_WRITE_ERR        (0x0800u)       /* Write error                                                  */
#define MMC_CEINT2_READ_ERR         (0x0400u)       /* Read error                                                   */
#define MMC_CEINT2_RESPIND_ERR      (0x0200u)       /* Resp index error                                             */
#define MMC_CEINT2_RESP_ERR         (0x0100u)       /* Resp error                                                   */
#define MMC_CEINT2_CRC_TOE          (0x0010u)       /* CRC status timeout                                           */
#define MMC_CEINT2_WRITE_TOE        (0x0008u)       /* Write timeout                                                */
#define MMC_CEINT2_READ_TOE         (0x0004u)       /* Read timeout                                                 */
#define MMC_CEINT2_RESPB_TOE        (0x0002u)       /* Resp busy timeout                                            */
#define MMC_CEINT2_RESP_TOE         (0x0001u)       /* Resp timeout                                                 */
#define MMC_CEINT2_ALL_ERR          (0xcf1fu)       /* All errors                                                   */

/* ---- Definition of Interrupt Enable Register (CEINTEN) ---- */
#define MMC_CEINTEN_INIT            (0x00000000ul)  /* All init                                                     */

/* ---- Definnition of Version Register (CEVERSION) ---- */
#define MMC_CEHOSTSTS1_MASK_CMDSEQ  (0x80000000ul)  /* Mask command sequence status flag                            */

/* ---- Definition of Card Detect/Port Control Register (CEDETECT) ---- */
#define MMC_CEDETECT_INIT           (0x00000000ul)  /* All init                                                     */
#define MMC_CEDETECT_DET            (0x00003000ul)  /* Card insert and remove                                       */
#define MMC_CEDETECT_INS            (0x00001000ul)  /* Card insert                                                  */
#define MMC_CEDETECT_REM            (0x00002000ul)  /* Card remove                                                  */
#define MMC_CEDETECT_MASK_DET       (0x00000030ul)  /* Mask card insert and remove                                  */
#define MMC_CEDETECT_MASK_INS       (0x00000010ul)  /* Mask card insert                                             */
#define MMC_CEDETECT_MASK_REM       (0x00000020ul)  /* Mask card remove                                             */
#define MMC_CEDETECT_MASK_CDSIG     (0x00004000ul)  /* Mask MMC_CD pin status indication                            */

/* ---- Definnition of Version Register (CEVERSION) ---- */
#define MMC_CEVERSION_INIT          (0x00000000ul)  /* All init.                                                    */
#define MMC_CEVERSION_ENABLE        (0x80000000ul)  /* Executes software reset.                                     */


/* ---- Time out count ---- */
#define MMC_TIMEOUT_CMD                 (100)       /* Commnad timeout                                              */
#define MMC_TIMEOUT_MULTIPLE            (1000)      /* Block transfer timeout                                       */
#define MMC_TIMEOUT_RESP                (1000)      /* Command sequence timeout                                     */
#define MMC_TIMEOUT_BUSY_CMD            (10000)     /* Busy command timeout                                         */
#define MMC_TIMEOUT_ERASE_CMD           (10000)     /* Erase timeout                                                */
#define MMC_TIMEOUT_BOOT_ACK            (30000)     /* Boot Acknowledge timeout                                     */
#define MMC_TIMEOUT_FORCE_ERASE_RESP    (600000)    /* Force erase response timeout                                 */

/* ---- Data transafer direction ---- */
#define MMC_TRANS_READ              (0)             /* Host <- MMC                                                  */
#define MMC_TRANS_WRITE             (1)             /* MMC -> Host                                                  */

/* ---- Card register size ---- */
#define MMC_EXTCSD_BYTE             (512)           /* Extended CSD Data size                                       */

/* ---- MMC Speed ---- */
#define MMC_CUR_SPEED_26MHz         (0x01u)         /* Current speed mode (26MHz of high-speed-mode)                */
#define MMC_CUR_SPEED_52MHz         (0x03u)         /* Current speed mode (52MHz of high-speed-mode)                */
#define MMC_SUP_SPEED_26MHz         (0x10u)         /* Supported speed mode (26MHz of high-speed-mode)              */
#define MMC_SUP_SPEED_52MHz         (0x30u)         /* Supported speed mode (52MHz of high-speed-mode)              */

/* ==== format parameter ==== */
#define MMC_SIZE_CARD_256KB         (256*1024/MMC_TRANS_BLOCK_SIZE)                 /*  256*1KB/(sector size)       */
#define MMC_SIZE_CARD_1MB           (1024*1024/MMC_TRANS_BLOCK_SIZE)                /* 1024*1KB/(sector size)       */
#define MMC_SIZE_CARD_2MB           (2*1024*1024/MMC_TRANS_BLOCK_SIZE)              /*    2*1MB/(sector size)       */
#define MMC_SIZE_CARD_4MB           (4*1024*1024/MMC_TRANS_BLOCK_SIZE)              /*    4*1MB/(sector size)       */
#define MMC_SIZE_CARD_8MB           (8*1024*1024/MMC_TRANS_BLOCK_SIZE)              /*    8*1MB/(sector size)       */
#define MMC_SIZE_CARD_16MB          (16*1024*1024/MMC_TRANS_BLOCK_SIZE)             /*   16*1MB/(sector size)       */
#define MMC_SIZE_CARD_32MB          (32*1024*1024/MMC_TRANS_BLOCK_SIZE)             /*   32*1MB/(sector size)       */
#define MMC_SIZE_CARD_64MB          (64*1024*1024/MMC_TRANS_BLOCK_SIZE)             /*   64*1MB/(sector size)       */
#define MMC_SIZE_CARD_128MB         (128*1024*1024/MMC_TRANS_BLOCK_SIZE)            /*  128*1MB/(sector size)       */
#define MMC_SIZE_CARD_256MB         (256*1024*1024/MMC_TRANS_BLOCK_SIZE)            /*  256*1MB/(sector size)       */
#define MMC_SIZE_CARD_504MB         (504*1024*1024/MMC_TRANS_BLOCK_SIZE)            /*  504*1MB/(sector size)       */
#define MMC_SIZE_CARD_1008MB        (1008*1024*1024/MMC_TRANS_BLOCK_SIZE)           /* 1008*1MB/(sector size)       */
#define MMC_SIZE_CARD_1024MB        (1024*1024*1024/MMC_TRANS_BLOCK_SIZE)           /* 1024*1MB/(sector size)       */
#define MMC_SIZE_CARD_2016MB        (2016*1024*1024/MMC_TRANS_BLOCK_SIZE)           /* 2016*1MB/(sector size)       */
#define MMC_SIZE_CARD_2048MB        (2048ul*1024ul*1024ul / (uint32_t)MMC_TRANS_BLOCK_SIZE)
                                                                                    /* 2048*1MB/(sector size)       */
#define MMC_SIZE_CARD_4032MB        (4032ul*1024ul*2ul)                             /* 4032*(1MB/sector size)       */
#define MMC_SIZE_CARD_4096MB        (4096ul*1024ul*2ul)                             /* 4096*(1MB/sector size)       */
#define MMC_SIZE_CARD_8192MB        (8192ul*1024ul*2ul)                             /* 2048*(1MB/sector size)       */
#define MMC_SIZE_CARD_16384MB       (16384ul*1024ul*2ul)                            /* 2048*(1MB/sector size)       */
#define MMC_SIZE_CARD_32768MB       (32768ul*1024ul*2ul)                            /* 2048*(1MB/sector size)       */
#define MMC_SIZE_CARD_128GB         (128ul*1024ul*1024ul*2ul)                       /*  128*(1GB/sector size)       */
#define MMC_SIZE_CARD_512GB         (512ul*1024ul*1024ul*2ul)                       /*  512*(1GB/sector size)       */
#define MMC_SIZE_CARD_2TB           (0xFFFFFFFF)                                    /*    2*(1TB/sector size)       */


/* ---- Sector related parameter ---- */
#define MMC_SECTOR_SIZE             MMC_TRANS_BLOCK_SIZE

#define MMC_CLKDIVEN_LOOP_COUNT     (10000)         /* Check SCLKDIVEN bit loop count                               */

/* ==== Macro functions ==== */
#define MMC_GET_HNDL(a)             (MMCHandle[a])

#define MMC_OUTREG(h,offset,data)   (*(volatile uint32_t *)((h)->reg_base+(offset)) = (data))
#define MMC_INREG(h,offset)         (*(volatile uint32_t *)((h)->reg_base+(offset)))

/* ---- System unique ---- */
#ifdef WITH_FIFO
#define MMC_SET_FIFO(h,data)        (*(volatile uint32_t *)((h)->reg_base+(MMCFIFOCONT)) = (data))
#else
#define MMC_SET_FIFO(h,data)
#endif

/* ---- OCR parameter ---- */
#define MMC_OCR_ACCESS_MODE_MASK    (0x60000000)    /* OCR[30:29] Access mode mask (11 b)                           */
#define MMC_OCR_SECTOR_MODE_MASK    (0x40000000)    /* OCR[30:29] sector mode mask (10 b)                           */


#ifdef MMC_CFG_LONGQ_ENABLE                         /* Uses FIT LongQ module                                        */
typedef enum e_mmc_log
{
    MMC_INIT                =  32,
    MMC_INT                 =  33,
    MMC_REGISTER            =  34,
    MMC_CD                  =  35,
    MMC_CMD                 =  36,
    MMC_MOUNT               =  37,
    MMC_READ                =  38,
    MMC_WRITE               =  39,
    MMC_TRANS               =  40,
    MMC_UTIL                =  41,
    MMC_DEV_PORT            =  42,
    MMC_UPCASE_TABLE        =  43,
    MMC_FORMAT              =  44,
    MMC_OPT                 =  45,
} mmc_logid_t;
#endif  /*MMC_CFG_LONGQ_ENABLE  */

/* ==== MMC Driver work buffer (allocated by File system) ==== */
/* hide from users */
/* ---- ""VAL DEF"" ---- */
typedef struct __mmc_mmchndl_t                      /* MMC handle                                                   */
{
    uint32_t       reg_base;                        /* MMC base address                                             */
    uint32_t       card_sector_size;                /* Sector size (user area)                                      */
    uint32_t       erase_sect;                      /* Erase block size                                             */
    uint32_t       sec_count;                       /* Card capacity parameter for more than 2GB                    */
    uint32_t       boot_size;                       /* Boot partition sector size                                   */
    uint8_t        csd_structure;                   /* CSD structure (Standard capacity:0 High capacity:1)          */
    uint8_t        csd_spec;                        /* CSD spec version                                             */
    uint8_t        csd_tran_speed;                  /* CSD transfer speed(MEM)                                      */
    uint8_t        csd_file_format;                 /* CSD file format group                                        */
    uint32_t       csd_ccc;                         /* CSD command class                                            */
    uint32_t       csd_read_len;                    /* CSD read block length                                        */
    uint32_t       csd_write_len;                   /* CSD write block length                                       */
    uint8_t        if_mode;                         /* Bus width (1bit:00 4bits:01 8bits:10)                        */
    uint8_t        speed_mode;                      /* Speed mode;
                                                            current speed  :0x01 Default:0/Hi-speed:1
                                                            supported speed:0x10 Default:0/Hi-speed:1               */
    uint8_t        density_type;                    /* Memory density (not more than 2GB:0 more than 2GB:1)         */
    uint8_t        cur_app;                         /* Current MMC application:
                                                            standard MMC                    : 0
                                                            secure MMC                      : 1
                                                            content protection secure MMC   : 2
                                                            CE-ATA or ATA on MMC            : 4                     */
    uint8_t        sup_app;                         /* Supported MMC application                                    */
    uint8_t        rel_seccnt;                      /* Reliable write sector count                                  */
    uint8_t        boot_ack;                        /* Boot acknowledge:
                                                            no boot acknowledge sent        : 0
                                                            boot acknowledge sent           : 1                     */
    uint8_t        boot_enab;                       /* Boot partition enable:
                                                            not boot partition enabled      : 0x0
                                                            boot partition 1 enabled        : 0x1
                                                            boot partition 2 enabled        : 0x2
                                                            reserved                        : 0x3-0x6
                                                            user area enabled               : 0x7                   */
    uint8_t        boot_acc;                        /* Boot partition access:
                                                            no access to boot partition     : 0x0
                                                            access boot partition 1         : 0x1
                                                            access boot partition 2         : 0x2
                                                            reserved                        : 0x3-0x7               */
    uint8_t        reset_boot_width;                /* Reset or retain boot bus width:
                                                            reset boot bus width to 1bit    : 0x0
                                                            retain boot bus width           : 0x1                   */
    uint8_t        boot_width;                      /* Current boot bus width:
                                                            1bit(default)                   : 0x0
                                                            4bits                           : 0x1
                                                            8bits                           : 0x2
                                                            reserved                        : 0x3                   */
    uint8_t        mount;                           /* Mount flag (mount:0 unmount:1)                               */
    uint32_t       ceint1;                          /* CEINT[31:16] status (Normal Interrupt Flag)                  */
    uint32_t       ceint2;                          /* CEINT[15: 0] status (Error Interrupt Flag)                   */
    uint32_t       ceinten1;                        /* CEINTEN[31:16] status (Normal Interrupt Enable)              */
    uint32_t       ceinten2;                        /* CEINTEN[15: 0] status (Error Interrupt Enable)               */
    uint32_t       int_det;                         /* Interrupt flag card detect status                            */
    uint32_t       int_det_mask;                    /* Int_det mask status                                          */
    uint32_t       voltage;                         /* System supplied voltage                                      */
    mmc_status_t   error;                           /* Error detail information                                     */
    uint32_t       stop;                            /* Compulsory stop flag                                         */
    uint8_t        int_mode;                        /* Interrupt flag detect method (polling:0 H/W interrupt:1)     */
    uint8_t        trans_mode;                      /* Data transfer method
                                                            PIO                             : 0
                                                            MMC_BUF DMA                     : 0x2                   */
    uint8_t        sup_eMMC;                        /* Support eMMC (not support:0 support:1)                       */
    uint8_t        sup_bus;                         /* Support bus width (1bit:0 4bits:1 8bits:2)                   */
    int32_t        mmcmem_trans_sectors;            /* Maximum block counts per multiple command                    */
    mmc_status_t  (*int_cd_callback)(int32_t);      /* Callback function for card detection                         */
    mmc_status_t  (*int_format_callback)(int32_t);  /* Callback function for card format                            */
    mmc_status_t  (*int_callback)(int32_t);         /* Callback function for interrupt flags                        */
    uint32_t       resp_status;                     /* R1/R1b response status                                       */
    uint32_t       ocr[1];                          /* OCR value                                                    */
    uint32_t       resp_r4[1];                      /* R4 value                                                     */
    uint32_t       resp_r5[1];                      /* R5 value                                                     */
    uint32_t       cid[4];                          /* CID value                                                    */
    uint32_t       csd[4];                          /* CSD value                                                    */
    uint32_t       dsr[1];                          /* DSR value                                                    */
    uint32_t       rca[1];                          /* RCA value                                                    */
    uint8_t        *p_rw_buff;                      /* Work buffer pointer                                          */
} mmc_mmchndl_t;


/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/
/* ---- r_mmcif_register.c ---- */
extern const void * MMC_CFG_IP_BASE[1];

/* ---- r_mmcif_init.c ---- */
mmc_status_t r_mmcif_init_hndl(uint32_t channel, uint32_t mode, uint32_t voltage);
extern mmc_mmchndl_t *MMCHandle[(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t))];
mmc_status_t r_mmcif_init_reg(uint32_t channel);
mmc_status_t r_mmcif_reset_reg(uint32_t channel);
mmc_status_t r_mmcif_cmd_seq_force_stop(uint32_t channel);

/* ---- r_mmcif_mount.c ---- */
mmc_status_t r_mmcif_preboot(uint32_t channel);
mmc_status_t r_mmcif_card_init(uint32_t channel);
mmc_status_t r_mmcif_card_switch_func(uint32_t channel, uint32_t arg);
mmc_status_t r_mmcif_read_byte(uint32_t channel, uint32_t cmd, uint32_t arg, uint8_t *p_readbuff,
                                       uint32_t byte);
mmc_status_t r_mmcif_write_byte(uint32_t channel, uint32_t cmd, uint32_t arg, uint8_t *p_writebuff,
                                        uint32_t byte);

/* ---- r_mmcif_trans.c ---- */
mmc_status_t r_mmcif_software_trans(uint32_t channel, uint8_t *p_buff, int32_t cnt, int32_t dir);

/* ---- r_mmcif_read.c ---- */
 /* no function */

/* ---- r_mmcif_write.c ---- */
 /* no function */

/* ---- r_mmcif_cd.c ---- */
mmc_status_t r_mmcif_Get_CardDetection(uint32_t channel);
mmc_status_t r_mmcif_set_det_mask(uint32_t channel, uint32_t mask);
mmc_status_t r_mmcif_clear_det_mask(uint32_t channel, uint32_t mask);
mmc_status_t r_mmcif_get_det_int(uint32_t channel);

/* ---- r_mmcif_cmd.c ---- */
mmc_status_t r_mmcif_send_cmd(uint32_t channel, uint32_t cmd);
mmc_status_t r_mmcif_send_cmd_arg(uint32_t channel, uint32_t cmd, int32_t resp, uint32_t arg);
mmc_status_t r_mmcif_send_ocr(uint32_t channel);
mmc_status_t r_mmcif_check_resp_error(uint32_t channel);
mmc_status_t r_mmcif_get_resp(uint32_t channel, int32_t resp);
mmc_status_t r_mmcif_check_csd(uint32_t channel);
mmc_status_t r_mmcif_check_info2_err(uint32_t channel);
void         r_mmcif_get_info2(uint32_t channel);

/* ---- r_mmcif_int.c ---- */
mmc_status_t r_mmcif_set_int_mask(uint32_t channel, uint32_t mask1, uint32_t mask2);
mmc_status_t r_mmcif_clear_int_mask(uint32_t channel,uint32_t mask1, uint32_t mask2);
mmc_status_t r_mmcif_clear_info(uint32_t channel, uint32_t clear_info1, uint32_t clear_info2);
mmc_status_t r_mmcif_clear_info_reg(uint32_t channel, uint32_t clear_info1, uint32_t clear_info2);
mmc_status_t r_mmcif_get_int(uint32_t channel);
mmc_status_t r_mmcif_get_intstatus(uint32_t channel);

/* ---- r_mmcif_util.c ---- */
mmc_status_t r_mmcif_set_clock(uint32_t channel, uint32_t clock, int32_t enable);
mmc_status_t r_mmcif_set_port(uint32_t channel, int32_t port);
mmc_status_t r_mmcif_set_err(uint32_t channel, mmc_status_t error);
int32_t      r_mmcif_bit_search(uint32_t data);
mmc_status_t r_mmcif_get_size(uint32_t channel);
mmc_status_t r_mmcif_wait_rbusy(uint32_t channel, uint32_t time);
mmc_status_t r_mmcif_stop(uint32_t channel);
mmc_status_t r_mmcif_read_write_error(uint32_t channel);

#ifdef MMC_CFG_LONGQ_ENABLE                        /* Uses FIT LongQ module */
    uint32_t     r_mmcif_log(uint32_t flg, uint32_t fid, uint32_t line);
    #define R_MMCIF_Log_Func(x, y, z) r_mmcif_log(x, y, z)
#else
    #define R_MMCIF_Log_Func(x, y, z)
#endif  /* MMC_CFG_LONGQ_ENABLE */

/* ---- target MCU I/F ---- */
mmc_status_t r_mmcif_dev_init(uint32_t channel);
mmc_status_t r_mmcif_dev_finalize(uint32_t channel);
mmc_status_t r_mmcif_dev_read_data(uint8_t *p_buff, uint32_t reg_addr, int32_t num);
mmc_status_t r_mmcif_dev_write_data(uint8_t *p_buff, uint32_t reg_addr, int32_t num);
uint32_t     r_mmcif_dev_get_clockdiv(int32_t clock);
mmc_status_t r_mmcif_dev_int_wait(uint32_t channel, uint32_t msec);
mmc_status_t r_mmcif_dev_wait(uint32_t channel, uint32_t time);
mmc_status_t r_mmcif_dev_disable_int(uint32_t channel);
mmc_status_t r_mmcif_dev_enable_int(uint32_t channel);
mmc_status_t r_mmcif_dev_dmacdtc_trans(uint32_t channel, int32_t cnt);
uint32_t     r_mmcif_dev_ceclkctrl_init(void);
mmc_status_t r_mmcif_dev_cd_layout(uint32_t channel);
bool         r_mmcif_dev_hardware_lock(uint32_t channel);
bool         r_mmcif_dev_hardware_unlock(uint32_t channel);

#endif /* _MMC_PRIVATE_H */


