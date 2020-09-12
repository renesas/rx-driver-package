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
* Copyright (C) 2014(2015-2017) Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/

/**********************************************************************************************************************
* File Name    : r_sdc_sd_rx_if.h
* Device       : RX
* Tool-Chain   : RX Family C Compiler
* H/W Platform : RSK board for RX
* Description  : Interface file for SDC SD API for RX
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 05.09.2014 1.00    First Release
*         : 17.07.2015 1.10    Standardized by the combo.
*         : 06.01.2016 1.20    Changed r_sdhi_set_speed().
*         : 30.09.2016 1.21    Added error code of SDC_SD_ERR_UNSUPPORTED_TYPE.
*         :                    Changed driver version.
*         : 31.07.2017 2.00    SDHI FIT module separated into hardware low level layer and middleware layer.
*         :                    Changed prefix from SDHI to SDC_SD.
**********************************************************************************************************************/

#ifndef SDC_SD_RX_H
#define SDC_SD_RX_H

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
/* Used to get which MCU is currently being used. */
#include "platform.h"
#include "r_sdhi_rx_if.h"
#include "r_sdhi_rx_config.h"

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
/* #### SD Memory and common definitions #### */
/* Version Number of API. */
#define RX_SDC_SD_VERSION_MAJOR     (2)
#define RX_SDC_SD_VERSION_MINOR     (00)

#define SDC_SD_CARD_NO0             (0)
#define SDC_SD_CARD_NO1             (1)

#define SDC_SDMEM_MAX_TRANS         (65535)
#define SDC_SDIO_MAX_TRANS          (256)
#define SDC_SD_MAX_AGAIN_COUNT      (3)

/* ---- Driver mode ---- */
#define SDC_SD_MODE_POLL            (0x0000ul)  /* Status check mode: Software polling */
#define SDC_SD_MODE_HWINT           (0x0001ul)  /* Status check mode: Hardware interrupt */
#define SDC_SD_MODE_SW              (0x0000ul)  /* Data transfer mode: Software(PIO) */
#define SDC_SD_MODE_DMA             (0x0002ul)  /* Data transfer mode: DMA */
#define SDC_SD_MODE_DTC             (0x0004ul)  /* Data transfer mode: DTC */
#define SDC_SD_MODE_SW_SINGLE       (0x0008ul)  /* Data transfer mode: Software(PIO) (SINGLE READ:Not User SET) */

/* ---- Support mode ---- */
#define SDC_SD_MODE_MEM             (0x0020ul)  /* Memory card (SD Memory, MMC) */
#define SDC_SD_MODE_IO              (0x0010ul)  /* Memory and SDIO */
#define SDC_SD_MODE_COMBO           (0x0030ul)  /* SD Memory, SDIO and SD Combo */
#define SDC_SD_MODE_1BIT            (0x0100ul)  /* SD Mode: 1bit only */
#define SDC_SD_MODE_4BIT            (0x0200ul)  /* SD Mode: 1bit and 4bit */

/* ---- Media voltage ---- */
#define SDC_SD_VOLT_1_7             (0x00000010ul)  /* Low voltage card minimum */
#define SDC_SD_VOLT_1_8             (0x00000020ul)
#define SDC_SD_VOLT_1_9             (0x00000040ul)
#define SDC_SD_VOLT_2_0             (0x00000080ul)
#define SDC_SD_VOLT_2_1             (0x00000100ul)  /* Basic communication minimum */
#define SDC_SD_VOLT_2_2             (0x00000200ul)
#define SDC_SD_VOLT_2_3             (0x00000400ul)
#define SDC_SD_VOLT_2_4             (0x00000800ul)
#define SDC_SD_VOLT_2_5             (0x00001000ul)
#define SDC_SD_VOLT_2_6             (0x00002000ul)
#define SDC_SD_VOLT_2_7             (0x00004000ul)
#define SDC_SD_VOLT_2_8             (0x00008000ul)  /* Memory access minimum */
#define SDC_SD_VOLT_2_9             (0x00010000ul)
#define SDC_SD_VOLT_3_0             (0x00020000ul)
#define SDC_SD_VOLT_3_1             (0x00040000ul)
#define SDC_SD_VOLT_3_2             (0x00080000ul)
#define SDC_SD_VOLT_3_3             (0x00100000ul)
#define SDC_SD_VOLT_3_4             (0x00200000ul)
#define SDC_SD_VOLT_3_5             (0x00400000ul)
#define SDC_SD_VOLT_3_6             (0x00800000ul)

/* ---- Memory card write mode ---- */
#define SDC_SD_WRITE_WITH_PREERASE  (0x0000u)   /* Pre-erase write */
#define SDC_SD_WRITE_OVERWRITE      (0x0001u)   /* Overwrite */

 /* ---- Media type ---- */
#define SDC_SD_MEDIA_UNKNOWN        (0x0000u)   /* Unknown media */
#define SDC_SD_MEDIA_MMC            (0x0010u)   /* MMC card */
#define SDC_SD_MEDIA_SDMEM          (0x0020u)   /* SD Memory card */
#define SDC_SD_MEDIA_SDIO           (0x0001u)   /* SDIO card */
#define SDC_SD_MEDIA_MEM            (0x0030u)   /* Memory card */
#define SDC_SD_MEDIA_COMBO          (0x0021u)   /* SD COMBO card */

/* ---- Write protect information --- */
#define SDC_SD_WP_OFF               (0x0000u)   /* No write protect */
#define SDC_SD_WP_HW                (0x0001u)   /* H/W write protect */
#define SDC_SD_WP_TEMP              (0x0002u)   /* TEMP_WRITE_PROTECT */
#define SDC_SD_WP_PERM              (0x0004u)   /* PERM_WRITE_PROTECT */
#define SDC_SD_WP_ROM               (0x0010u)   /* SD ROM */

/* ---- SD clock define ---- */ /* Max. frequency */
#define SDC_SD_CLK_400kHz           (0x0000u)   /* 400kHz */
#define SDC_SD_CLK_1MHz             (0x0001u)   /* 1MHz */
#define SDC_SD_CLK_5MHz             (0x0002u)   /* 5MHz */
#define SDC_SD_CLK_10MHz            (0x0003u)   /* 10MHz */
#define SDC_SD_CLK_20MHz            (0x0004u)   /* 20MHz */
#define SDC_SD_CLK_25MHz            (0x0005u)   /* 25MHz */
#define SDC_SD_CLK_50MHz            (0x0006u)   /* 50MHz (SD Specifications Part 1 Physical Layer
                                                          Specification Ver.1.10) */

/* ---- Speed class ---- */
#define SDC_SD_SPEED_CLASS_0        (0x00u)     /* Not defined, or less than SD Specifications Part 1
                                                   Physical Layer Specification Ver.2.00 */
#define SDC_SD_SPEED_CLASS_2        (0x01u)     /* 2MB/sec */
#define SDC_SD_SPEED_CLASS_4        (0x02u)     /* 4MB/sec */
#define SDC_SD_SPEED_CLASS_6        (0x03u)     /* 6MB/sec */
#define SDC_SD_SPEED_CLASS_10       (0x04u)     /* 10MB/sec */

/* ---- SD Card detect port ---- */
#define SDC_SD_CD_SOCKET            (0x0000u)   /* CD pin */
#define SDC_SD_CD_DAT3              (0x0001u)   /* DAT3 pin */

/* ---- SD Card detect interrupt ---- */
#define SDC_SD_CD_INT_DISABLE       (0x0000u)   /* Card detect interrupt disable */
#define SDC_SD_CD_INT_ENABLE        (0x0001u)   /* Card detect interrupt enable */

/* ---- SD Card detect ---- */
#define SDC_SD_CD_REMOVE            (0x0000u)   /* Removal */
#define SDC_SD_CD_INSERT            (0x0001u)   /* Insert */

/* ---- Format mode ---- */
#define SDC_SD_FORMAT_QUICK         (0x0000u)   /* Quick format */
#define SDC_SD_FORMAT_FULL          (0x0001u)   /* Full format */

/* ---- Lock/Unlock mode ---- */
#define SDC_SD_UNLOCK_CARD          (0x00)
#define SDC_SD_SET_PWD              (0x01)
#define SDC_SD_CLR_PWD              (0x02)
#define SDC_SD_LOCK_CARD            (0x04)
#define SDC_SD_FORCE_ERASE          (0x08)

/* ==== Command type ==== */
/* ---- SD commands ---- */
#define SDC_SD_CMD0                 (0u)    /* GO_IDLE_STATE */
#define SDC_SD_CMD2                 (2u)    /* ALL_SEND_CID */
#define SDC_SD_CMD3                 (3u)    /* SEND_RELATIVE_ADDR */
#define SDC_SD_CMD4                 (4u)    /* SET_DSR */
#define SDC_SD_CMD7                 (7u)    /* SELECT/DESELECT_CARD */
#define SDC_SD_CMD9                 (9u)    /* SEND_CSD */
#define SDC_SD_CMD10                (10u)   /* SEND_CID */
#define SDC_SD_CMD12                (12u)   /* STOP_TRANSMISSION */
#define SDC_SD_CMD13                (13u)   /* SEND_STATUS */
#define SDC_SD_CMD15                (15u)   /* GO_INACTIVE_STATE */
#define SDC_SD_CMD16                (16u)   /* SET_BLOCK_LEN */
#define SDC_SD_CMD17                (17u)   /* READ_SINGLE_BLOCK */
#define SDC_SD_CMD18                (18u)   /* READ_MULTIPLE_BLOCK */
#define SDC_SD_CMD24                (24u)   /* WRITE_SINGLE_BLOCK */
#define SDC_SD_CMD25                (25u)   /* WRITE_MULTIPLE_BLOCK */
#define SDC_SD_CMD27                (27u)   /* PROGRAM_CSD */
#define SDC_SD_CMD28                (28u)   /* SET_WRITE_PROT */
#define SDC_SD_CMD29                (29u)   /* CLR_WRITE_PROT */
#define SDC_SD_CMD30                (30u)   /* SEND_WRITE_PROT */
#define SDC_SD_CMD32                (32u)   /* ERASE_WR_BLK_START */
#define SDC_SD_CMD33                (33u)   /* ERASE_WR_BLK_END */
#define SDC_SD_CMD38                (38u)   /* ERASE */
#define SDC_SD_CMD42                (42u)   /* LOCK_UNLOCK */
#define SDC_SD_CMD55                (55u)   /* APP_SDC_SD_CMD */

/* ---- IO commands ---- */  /* add for IO */
#define SDC_SD_CMD5                 (0x4705u)   /* IO_SEND_OP_COND */
#define SDC_SD_CMD52_W              (0x4434u)   /* IO_WRITE_DIRECT */
#define SDC_SD_CMD52_R              (0x5434u)   /* IO_READ_DIRECT */
#define SDC_SD_CMD53_W_BLOCK        (0x6c35u)   /* IO_WRITE_EXTENDED_BLOCK */
#define SDC_SD_CMD53_W_BYTE         (0x4c35u)   /* IO_WRITE_EXTENDED_BYTE */
#define SDC_SD_CMD53_R_BLOCK        (0x7c35u)   /* IO_READ_EXTENDED_BLOCK */
#define SDC_SD_CMD53_R_BYTE         (0x5c35u)   /* IO_READ_EXTENDED_BYTE */

/* ---- Wwitch function command (SD Specifications Part 1 Physical Layer Specification Ver.1.10) ---- */
#define SDC_SD_CMD6                 (0x1C06u)   /* SWITCH_FUNC */

/* ---- Dual voltage inquiry command (SD Specifications Part 1 Physical Layer Specification Ver.2.0) ---- */
#define SDC_SD_CMD8                 (0x0408u)   /* SEND_IF_COND */

/* ---- Application specific commands ---- */
#define SDC_SD_ACMD                 (0x40u)
#define SDC_SD_ACMD6                (SDC_SD_ACMD|6u)  /* SET_BUS_WIDTH */
#define SDC_SD_ACMD13               (SDC_SD_ACMD|13u) /* SDC_SD_STATUS */
#define SDC_SD_ACMD22               (SDC_SD_ACMD|22u) /* SEND_NUM_WR_BLOCKS */
#define SDC_SD_ACMD23               (SDC_SD_ACMD|23u) /* SET_WR_BLK_ERASE_COUNT */
#define SDC_SD_ACMD41               (SDC_SD_ACMD|41u) /* SDC_SD_SEND_OP_COND */
#define SDC_SD_ACMD42               (SDC_SD_ACMD|42u) /* SET_CLR_CARD_DETECT */
#define SDC_SD_ACMD51               (SDC_SD_ACMD|51u) /* SEND_SCR */

/* ==== Constants ==== */
/* --- Command arguments --- */
#define SDC_SD_ARG_ACMD6_1bit       (0)
#define SDC_SD_ARG_ACMD6_4bit       (2)

/* ---- Response type  ---- */
#define SDC_SD_RESP_NON             (0)     /* No response */
#define SDC_SD_RESP_R1              (1)     /* Normal response */
#define SDC_SD_RESP_R1b             (2)     /* Normal response with an optional busy signal */
#define SDC_SD_RESP_R1_SCR          (3)     /* Normal response with an optional busy signal */
#define SDC_SD_RESP_R2_CID          (4)     /* CID register */
#define SDC_SD_RESP_R2_CSD          (5)     /* CSD register */
#define SDC_SD_RESP_R3              (6)     /* OCR register */
#define SDC_SD_RESP_R6              (7)     /* Published RCA response */
#define SDC_SD_RESP_R4              (8)     /* IO OCR register */
#define SDC_SD_RESP_R5              (9)     /* IO RW response */
#define SDC_SD_RESP_R7              (10)    /* Card Interface Condition response */

/* ---- COR register bit ---- */
#define SDC_SD_OCR_HCS              (0x40000000ul)  /* High Capacity Support */
#define SDC_SD_OCR_BUSY             (0x80000000ul)  /* Card power up status bit (busy) */

#define SDC_SD_ADDR_BOUNDARY        (0x00000003ul)


/* #### SDIO definitions #### */
/* ---- CCCR Format Version number ---- */
#define SDC_SDIO_CCCRFBR_VER_10     (0x00)  /* CCCR/FBR defined in SDIO Ver.1.00 */
#define SDC_SDIO_CCCRFBR_VER_11     (0x01)  /* CCCR/FBR defined in SDIO Ver.1.10 */
#define SDC_SDIO_CCCRFBR_VER_20     (0x02)  /* CCCR/FBR defined in SDIO Ver.2.00 */
#define SDC_SDIO_CCCRFBR_VER_30     (0x03)  /* CCCR/FBR defined in SDIO Ver.3.00 */
#define SDC_SDIO_CCCRFBR_VER_40     (0x04)  /* CCCR/FBR defined in SDIO Ver.4.00 */

/* ---- IO initialize flags define ---- */
#define SDC_SDIO_INT_ENAB           (0x10u) /* Interrupt enable */
#define SDC_SDIO_POWER_INIT         (0x04u) /* Power on initialized */
#define SDC_SDIO_MEM_INIT           (0x02u) /* Memory initialized */
#define SDC_SDIO_FUNC_INIT          (0x01u) /* IO function initialized */

/* ---- IO function's information ---- */
#define SDC_SDIO_FUNC_READY         (0x80u) /* IO Ready */
#define SDC_SDIO_FUNC_NUM           (0x70u) /* Number of IO functions */
#define SDC_SDIO_FUNC_EXISTS        (0x04u) /* Memory present */

/* ---- Function No definitions ---- */
#define SDC_SDIO_FNO_0              (0x00u) /* Function No. 0 */
#define SDC_SDIO_FNO_1              (0x01u) /* Function No. 1 */
#define SDC_SDIO_FNO_2              (0x02u) /* Function No. 2 */
#define SDC_SDIO_FNO_3              (0x03u) /* Function No. 3 */
#define SDC_SDIO_FNO_4              (0x04u) /* Function No. 4 */
#define SDC_SDIO_FNO_5              (0x05u) /* Function No. 5 */
#define SDC_SDIO_FNO_6              (0x06u) /* Function No. 6 */
#define SDC_SDIO_FNO_7              (0x07u) /* Function No. 7 */

/* ---- CMD52 R/W flag definitions ---- */
#define SDC_SDIO_RWFLAG_R           (0x00u) /* R/W flag : Read */
#define SDC_SDIO_RWFLAG_W           (0x01u) /* R/W flag : Write */

/* ---- CMD53 flag definitions ---- */
#define SDC_SDIO_BLOCKMODE_RWFLAG_R (0x08000000u)    /* CMD53 Block Mode and R/W flag is Read  */
#define SDC_SDIO_BLOCKMODE_RWFLAG_W (0x88000000u)    /* CMD53 Block Mode and R/W flag is Write */
#define SDC_SDIO_BYTEMODE_RWFLAG_R  (0x00000000u)    /* CMD53 Byte Mode  and R/W flag is Read  */
#define SDC_SDIO_BYTEMODE_RWFLAG_W  (0x80000000u)    /* CMD53 Byte Mode  and R/W flag is Write */

/* ---- I/O Address definitions ---- */
/* Card Common Control Registers (CCCR) */
#define SDC_SDIO_ADRS_CCCRSDIO      (0x00u)     /* CCCR/SDIO Revision */
#define SDC_SDIO_ADRS_IOENABLE      (0x02u)     /* I/O Enable */
#define SDC_SDIO_ADRS_IOREADY       (0x03u)     /* I/O Ready */
#define SDC_SDIO_ADRS_INTENABLE     (0x04u)     /* Int Enable */
#define SDC_SDIO_ADRS_IOABORT       (0x06u)     /* I/O Abort */
#define SDC_SDIO_ADRS_BUSINTRFACE   (0x07u)     /* Bus Interface */
#define SDC_SDIO_ADRS_CARDCAPAB     (0x08u)     /* Card Capability */
#define SDC_SDIO_ADRS_CISPTR0       (0x09u)     /* CIS Pointer (LSB) */
#define SDC_SDIO_ADRS_CISPTR1       (0x0Au)     /* CIS Pointer */
#define SDC_SDIO_ADRS_CISPTR2       (0x0Bu)     /* CIS Pointer (MSB) */
#define SDC_SDIO_ADRS_BLOCKSIZE0    (0x10u)     /* Block Size (LSB) */
#define SDC_SDIO_ADRS_BLOCKSIZE1    (0x11u)     /* Block Size (MSB) */
#define SDC_SDIO_ADRS_BUSSPEED      (0x13u)     /* Bus Speed Select */
#define SDC_SDIO_ADRS_MAX           (0x1FFFFu)  /* Bus Speed Select */

/* Function Basic Registers (FBR) */
#define SDC_SDIO_ADRS_CSAPTR0       (0x0Cu)     /* CSA Pointer (LSB)*/
#define SDC_SDIO_ADRS_CSAPTR1       (0x0Du)     /* CSA Pointer */
#define SDC_SDIO_ADRS_CSAPTR2       (0x0Eu)     /* CSA Pointer (LSB) */
#define SDC_SDIO_ADRS_CSADAW        (0x0Fu)     /* CSA Data Access Window */

/* ---- CMD5 Response definitions ---- */
#define SDC_SDIO_CMD5_R4_C          (0x80000000u)    /* Card Ready */
#define SDC_SDIO_CMD5_R4_NUMIOFNCS  (0x70000000u)    /* Number of I/O Functions */
#define SDC_SDIO_CMD5_R4_MEMPRESENT (0x08000000u)    /* Memory Present */
#define SDC_SDIO_CMD5_R4_UHS2STATUS (0x04000000u)    /* UHS-II Card Status */
#define SDC_SDIO_CMD5_R4_S18A       (0x01000000u)    /* S18A */
#define SDC_SDIO_CMD5_R4_IOOCR      (0x00FF8000u)    /* I/O OCR : 2.7V or more */
                                                     /* Set the range. */
                                                     /* move to config file */
#define SDC_SDIO_MEMPRESENT         (0x08u)     /* Memory Present */

/* ---- CCCR Flags definitions ---- */
#define SDC_SDIO_CCCRSDIO_CCCR      (0xFFu)     /* CCCR mask */
#define SDC_SDIO_IOENABLE_IOE1      (0x02u)     /* IOE1 (For 1 Function) */
#define SDC_SDIO_IOABORT_RES        (0x08u)     /* RES */
#define SDC_SDIO_INTENABLE_IENM     (0x01u)     /* IENM */
#define SDC_SDIO_CD_DIS_BUS_1BIT    (0x80u)     /* CD Disable, 1-bit bus */
#define SDC_SDIO_CD_DIS_BUS_4BIT    (0x82u)     /* CD Disable, 4-bit bus */
#define SDC_SDIO_CARDCAPAB_LSC      (0x40u)     /* Low Speed Card */
#define SDC_SDIO_CARDCAPAB_4BLS     (0x80u)     /* 4-bit Mode Support for Low Speed Card */
#define SDC_SDIO_CARDCAPAB_SMB      (0x02u)     /* Multiple Block Mode */
#define SDC_SDIO_BUSSPEED_SHS       (0x01u)     /* High Speed Supporting */
#define SDC_SDIO_BUSSPEED_BSS       (0x02u)     /* High Speed Setting */

#define SDC_SDIO_SDSIZE_512         (0x200u)    /* SDSIZE 512 bytes (default) */
#define SDC_SDIO_SDSIZE_INVALID     (0xFFFFu)   /* Invalid SDSIZE : 0xFFFF is not set to SDSIZE. */

/* ---- sdc_sdio_access_t op_code definitions ---- */
#define SDC_SDIO_BLOCK_MODE         (0x10u)     /* SDIO_BLOCK_MODE */
#define SDC_SDIO_BYTE_MODE          (0x00u)     /* SDIO_BYTE_MODE */
#define SDC_SDIO_FIXED_ADDR         (0x00u)     /* R/W fixed address */
#define SDC_SDIO_INCREMENT_ADDR     (0x01u)     /* R/W increment address */

/* ---- IO register write mode ---- */
#define SDC_SDIO_SIMPLE_WRITE       (0x0000u)   /* Just Write */
#define SDC_SDIO_READ_AFTER_WRITE   (0x0001u)   /* Read after Write */
#define SDC_SDIO_SIMPLE_READ        (0x0000u)   /* Just Read */

/* ---- Mask CCCR regiter big ---- */
#define SDC_SDIO_MASK_CD_DIS_BUS_WIDTH   (0x83u)     /* Mask CD Disable bit and bus width bit */

typedef enum e_enum_sdc_sd_trans
{
    SDC_SD_SET_TRANS_STOP,
    SDC_SD_SET_TRANS_START
} enum_sdc_sd_trans_t;

typedef enum e_enum_sdc_sd_cmd
{
    SDC_SD_SET_STOP,
    SDC_SD_SET_BUFFER
} enum_sdc_sd_cmd_t;

/* ---- Error code ---- */
typedef enum e_sdc_sd_status
{
    SDC_SD_SUCCESS_LOCKED_CARD  =   1,    /* OK but card is locked status. */
    SDC_SD_SUCCESS              =   0,    /* OK */
    SDC_SD_ERR                  =  -1,    /* General error */
    SDC_SD_ERR_WP               =  -2,    /* Write protect error */
    SDC_SD_ERR_RO               =  -3,    /* Read only error */
    SDC_SD_ERR_RES_TOE          =  -4,    /* Response time out error */
    SDC_SD_ERR_CARD_TOE         =  -5,    /* Card time out error */
    SDC_SD_ERR_END_BIT          =  -6,    /* End bit error */
    SDC_SD_ERR_CRC              =  -7,    /* CRC error */
    SDC_SD_ERR_CARD_RES         =  -8,    /* Card response error */
    SDC_SD_ERR_HOST_TOE         =  -9,    /* Host time out error */
    SDC_SD_ERR_CARD_ERASE       = -10,    /* Card erase error */
    SDC_SD_ERR_CARD_LOCK        = -11,    /* Card lock error */
    SDC_SD_ERR_CARD_UNLOCK      = -12,    /* Card unlock error */
    SDC_SD_ERR_CARD_CRC         = -13,    /* Card CRC error */
    SDC_SD_ERR_CARD_ECC         = -14,    /* Card internal ECC error */
    SDC_SD_ERR_CARD_CC          = -15,    /* Card internal error */
    SDC_SD_ERR_CARD_ERROR       = -16,    /* Unknown card error */
    SDC_SD_ERR_CARD_TYPE        = -17,    /* Non supported card type */
    SDC_SD_ERR_NO_CARD          = -18,    /* No card */
    SDC_SD_ERR_ILL_READ         = -19,    /* Illegal buffer read */
    SDC_SD_ERR_ILL_WRITE        = -20,    /* Illegal buffer write */
    SDC_SD_ERR_AKE_SEQ          = -21,    /* Sequence of authentication process */
    SDC_SD_ERR_OVERWRITE        = -22,    /* CID/CSD overwrite error */
    SDC_SD_ERR_CPU_IF           = -30,    /* Target MCU interface function error */
    SDC_SD_ERR_STOP             = -31,    /* User stop */
    SDC_SD_ERR_CSD_VER          = -50,    /* CSD register version error */
    SDC_SD_ERR_SCR_VER          = -51,    /* SCR register version error */
    SDC_SD_ERR_FILE_FORMAT      = -52,    /* CSD register file format error  */
    SDC_SD_ERR_NOTSUP_CMD       = -53,    /* Not supported command */
    SDC_SD_ERR_ILL_FUNC         = -60,    /* Invalid function request error */
    SDC_SD_ERR_IO_VERIFY        = -61,    /* Direct write verify error */
    SDC_SD_ERR_IO_CAPAB         = -62,    /* IO capability error */
    SDC_SD_ERR_IFCOND_VER       = -70,    /* Interface condition version error */
    SDC_SD_ERR_IFCOND_VOLT      = -71,    /* Interface condition voltage error */
    SDC_SD_ERR_IFCOND_ECHO      = -72,    /* Interface condition echo back pattern error */
    SDC_SD_ERR_OUT_OF_RANGE     = -80,    /* Argument was out of range */
    SDC_SD_ERR_ADDRESS_ERROR    = -81,    /* Misaligned address */
    SDC_SD_ERR_BLOCK_LEN_ERROR  = -82,    /* Transferred block length is not allowed. */
    SDC_SD_ERR_ILLEGAL_COMMAND  = -83,    /* Command not legal */
    SDC_SD_ERR_RESERVED_ERROR18 = -84,    /* Reserved bit 18 Error */
    SDC_SD_ERR_RESERVED_ERROR17 = -85,    /* Reserved bit 17 Error */
    SDC_SD_ERR_CMD_ERROR        = -86,    /* SDHI_SDSTS2 bit 0 CMD error */
    SDC_SD_ERR_CBSY_ERROR       = -87,    /* SDHI_SDSTS2 bit 14 CMD Type Reg Busy error */
    SDC_SD_ERR_NO_RESP_ERROR    = -88,    /* SDHI_SDSTS1 bit 0 No Response error */
    SDC_SD_ERR_ADDRESS_BOUNDARY = -89,    /* Not specified buffer address in 4-byte unit */
    SDC_SD_ERR_UNSUPPORTED_TYPE = -97,    /* Unsupported type */
    SDC_SD_ERR_API_LOCK         = -98,    /* Lock the driver API */
    SDC_SD_ERR_INTERNAL         = -99     /* Driver software internal error */
} sdc_sd_status_t;

typedef struct
{
    uint32_t    cmd;
    uint32_t    mode;
    uint8_t     *p_buff;
    uint32_t    size;
}sdc_sd_cmd_t;

typedef struct
{
    uint32_t    mode;
    uint32_t    voltage;
}sdc_sd_cfg_t;

typedef struct
{
    uint8_t     *p_buff;
    uint32_t    lbn;
    int32_t     cnt;
    uint32_t    mode;
    uint32_t    write_mode;
}sdc_sd_access_t;

typedef struct
{
    uint32_t card_sector_size;  /* Sector size (user area) */
    uint32_t prot_sector_size;  /* Sector size (protect area) */
    uint8_t  write_protect;     /* Write protect:           OFF : 0x00  */
                                /*                       H/W WP : 0x01  */
                                /*       CSD  TMP_WRITE_PROTECT : 0x02  */
                                /*       CSD PERM_WRITE_PROTECT : 0x04  */
                                /*       SD ROM                 : 0x10  */
    uint8_t  media_type;        /* Card media type                      */
    uint8_t  csd_structure;     /* CSD structure (Standard capacity:0 High capacity:1) */
    uint8_t  speed_mode;        /* Card speed mode: current speed  :bit0      Default:0 / High-speed:1 */
                                /* bit:             supported speed:bit4      Default:0 / High-speed:1 */
    uint8_t  io_speed_mode;     /* IO speed mode:   current speed  :bit[0:1]  Default:0 / High-speed:1 / Low-speed:2 */
                                /* bit:             supported speed:bit[4:5]  Default:0 / High-speed:1 / Low-speed:2 */
    uint8_t  rsv[3];
}sdc_sd_card_status_t;

typedef struct
{
    uint32_t    sdio_ocr[1];    /* SDIO OCR value */
    uint32_t    ocr[1];         /* OCR value */
    uint32_t    cid[4];         /* CID value */
    uint32_t    csd[4];         /* CSD value */
    uint32_t    dsr[1];         /* DSR value */
    uint32_t    rca[2];         /* RCA value */
    uint32_t    scr[2];         /* SCR value */
    uint32_t    sdstatus[4];    /* SD STATUS value */
    uint32_t    switch_func_status[5];  /* SWITCH FUNCTION STATUS DATA */
}sdc_sd_card_reg_t;

typedef struct
{
    uint8_t     *p_buff;
    uint32_t    func;
    uint32_t    adr;
    uint32_t    raw_flag;
    uint32_t    rw_flag;
} sdc_sdio_daccess_t;

typedef struct
{
    uint8_t     *p_buff;
    uint32_t    func;
    uint32_t    adr;
    int32_t     cnt;
    uint32_t    op_code;
    uint8_t     trans_mode;
    uint8_t     rsv[3];
} sdc_sdio_access_t;

typedef struct
{
    uint32_t    func;
    int32_t     cnt;
    uint32_t    *p_comm_cis_ptr;
    uint8_t     *p_cis_buff;
} sdc_sdio_cis_t;



/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/
sdc_sd_status_t R_SDC_SD_Open(uint32_t card_no, uint32_t channel, void * p_sdc_sd_workarea);
sdc_sd_status_t R_SDC_SD_Close(uint32_t card_no);
sdc_sd_status_t R_SDC_SD_GetCardDetection(uint32_t card_no);
sdc_sd_status_t R_SDC_SD_Initialize(uint32_t card_no, sdc_sd_cfg_t * p_sdc_sd_config, uint32_t init_type);
sdc_sd_status_t R_SDC_SD_End(uint32_t card_no, uint32_t end_type);
sdc_sd_status_t R_SDC_SD_Control(uint32_t card_no, sdc_sd_cmd_t * p_sdc_sd_cmd);
sdc_sd_status_t R_SDC_SD_GetModeStatus(uint32_t card_no, uint8_t * p_mode);
sdc_sd_status_t R_SDC_SD_GetCardStatus(uint32_t card_no, sdc_sd_card_status_t * p_sdc_sd_card_status);
sdc_sd_status_t R_SDC_SD_GetCardInfo(uint32_t card_no, sdc_sd_card_reg_t * p_sdc_sd_card_reg);
sdc_sd_status_t R_SDC_SD_CdInt(uint32_t card_no, int32_t enable, sdc_sd_status_t (* callback)(int32_t));
sdc_sd_status_t R_SDC_SD_IntCallback(uint32_t card_no, sdc_sd_status_t (* callback)(int32_t));
sdc_sd_status_t R_SDC_SD_GetErrCode(uint32_t card_no);
void            R_SDC_SD_1msInterval(void);
sdc_sd_status_t R_SDC_SD_SetLogHdlAddress(uint32_t user_long_que);
uint32_t        R_SDC_SD_Log(uint32_t flg, uint32_t fid, uint32_t line);
sdc_sd_status_t R_SDC_SD_GetBuffRegAddress(uint32_t card_no, uint32_t * p_reg_buff);
uint32_t        R_SDC_SD_GetVersion(void);
sdc_sd_status_t R_SDC_SD_SetDmacDtcTransFlg(uint32_t card_no, uint32_t flg);

sdc_sd_status_t R_SDC_SDMEM_Read(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access);
sdc_sd_status_t R_SDC_SDMEM_ReadSoftwareTrans(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access);
sdc_sd_status_t R_SDC_SDMEM_ReadSoftwareTransSingleCmd(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access);
sdc_sd_status_t R_SDC_SDMEM_Write(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access);
sdc_sd_status_t R_SDC_SDMEM_WriteSoftwareTrans(uint32_t card_no, sdc_sd_access_t * p_sdc_sd_access);
sdc_sd_status_t R_SDC_SDMEM_GetSpeed(uint32_t card_no, uint8_t * p_cls, uint8_t * p_move);

sdc_sd_status_t R_SDC_SDIO_IOAbort(uint32_t card_no, uint32_t func);
sdc_sd_status_t R_SDC_SDIO_SetIOEnable(uint32_t card_no, uint32_t func);
sdc_sd_status_t R_SDC_SDIO_ClearIOEnable(uint32_t card_no, uint32_t func);
sdc_sd_status_t R_SDC_SDIO_GetIOReady(uint32_t card_no, uint8_t * p_data);
sdc_sd_status_t R_SDC_SDIO_IOReset(uint32_t card_no);
sdc_sd_status_t R_SDC_SDIO_SetIntEnable(uint32_t card_no);
sdc_sd_status_t R_SDC_SDIO_ClearIntEnable(uint32_t card_no);
sdc_sd_status_t R_SDC_SDIO_GetIntEnable(uint32_t card_no, uint8_t * p_data);
sdc_sd_status_t R_SDC_SDIO_ReadDirect(uint32_t card_no, sdc_sdio_daccess_t * p_sdc_sdio_daccess);
sdc_sd_status_t R_SDC_SDIO_WriteDirect(uint32_t card_no, sdc_sdio_daccess_t * p_sdc_sdio_daccess);
sdc_sd_status_t R_SDC_SDIO_Read(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access);
sdc_sd_status_t R_SDC_SDIO_ReadSoftwareTrans(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access);
sdc_sd_status_t R_SDC_SDIO_Write(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access);
sdc_sd_status_t R_SDC_SDIO_WriteSoftwareTrans(uint32_t card_no, sdc_sdio_access_t * p_sdc_sdio_access);
sdc_sd_status_t R_SDC_SDIO_SetBlocklen(uint32_t card_no, int32_t len, uint32_t func);
sdc_sd_status_t R_SDC_SDIO_GetBlocklen(uint32_t card_no, int32_t * p_len, uint32_t func);
sdc_sd_status_t R_SDC_SDIO_EnableInt(uint32_t card_no);
sdc_sd_status_t R_SDC_SDIO_DisableInt(uint32_t card_no);
sdc_sd_status_t R_SDC_SDIO_GetCIS(uint32_t card_no, sdc_sdio_cis_t * p_sdc_sdio_cis);
sdc_sd_status_t R_SDC_SDIO_IntCallback(uint32_t card_no, sdc_sd_status_t (* callback)(int32_t));



#endif /* SDC_SD_RX_H */

