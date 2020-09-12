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
* File Name    : r_mmcif_rx_if.h
* Device       : RX64M
* Tool-Chain   : RX Family C Compiler
* H/W Platform : RSK board for RX64M
* Description  : Interface file for MMC API for RX
***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 03.09.2014 1.00    First Release
*              : 28.12.2016 1.01    Added RX65N.
*              : 31.07.2017 1.02    Supported RX65N-2MB.
*              : 31.12.2017 1.03    The sample program corresponded to the pin setting of Smart Configurator.
*              :                    There is no change in the FIT module.
*              : 01.02.2019 1.04    Changed Minor version to 1.04.
*              : 20.05.2019 1.05    Added support for GNUC and ICCRX.
*                                   Fixed coding style.
*              : 30.07.2019 1.06    Changed Minor version to 1.06
*              :                    Added RX72M.
*              : 22.11.2019 1.07    Changed Minor version to 1.07
*              :                    Added RX72N and RX66N.
**********************************************************************************************************************/

#ifndef _MMC_IF_H
#define _MMC_IF_H

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

/* Version Number of API. */
#define RX_MMC_API_VERSION_MAJOR            (1)
#define RX_MMC_API_VERSION_MINOR            (7)

/* ==== IP selection ==== */
#define MMC_MMCMEM_MAX_TRANS                (65535)
#define MMC_MAX_AGAIN_COUNT                 (3)

/* ==== MMC chnannel ==== */
#define MMC_CH0                             (0)
#define MMC_CH1                             (1)

/* ---- Status check mode ---- */
#define MMC_MODE_POLL                       (0x0000ul)  /* Status check mode: Software polling                      */
#define MMC_MODE_HWINT                      (0x0001ul)  /* Status check mode: Hardware interrupt                    */

/* ---- Data transfer mode ---- */
#define MMC_MODE_SW                         (0x0000ul)  /* Data transfer mode: Software(PIO)                        */
#define MMC_MODE_DMA                        (0x0002ul)  /* Data transfer mode: DMA                                  */
#define MMC_MODE_DTC                        (0x0004ul)  /* Data transfer mode: DTC                                  */

/* ---- Support eMMC ---- */
#define MMC_MODE_MMC                        (0x0010ul)  /* Device type: MMC                                         */
#define MMC_MODE_eMMC                       (0x0020ul)  /* Device type: eMMC                                        */

/* ---- Support speed ---- */
#define MMC_MODE_DS                         (0x0000ul)  /* Only 26MHz is supported.                                 */
#define MMC_MODE_HS                         (0x0080ul)  /* 26MHz and 52MHz are supported.                           */

/* ---- Support mode ---- */
#define MMC_MODE_1BIT                       (0x0100ul)  /* MMC Mode: 1bit                                           */
#define MMC_MODE_4BIT                       (0x0400ul)  /* MMC Mode: 4bit                                           */
#define MMC_MODE_8BIT                       (0x0800ul)  /* MMC Mode: 8bit                                           */

/* ---- Media voltage ---- */
#define MMC_VOLT_1_7                        (0x00000010ul)  /* Low voltage card minimum                             */
#define MMC_VOLT_1_8                        (0x00000020ul)
#define MMC_VOLT_1_9                        (0x00000040ul)
#define MMC_VOLT_2_0                        (0x00000080ul)
#define MMC_VOLT_2_1                        (0x00000100ul)  /* Basic communication minimum                          */
#define MMC_VOLT_2_2                        (0x00000200ul)
#define MMC_VOLT_2_3                        (0x00000400ul)
#define MMC_VOLT_2_4                        (0x00000800ul)
#define MMC_VOLT_2_5                        (0x00001000ul)
#define MMC_VOLT_2_6                        (0x00002000ul)
#define MMC_VOLT_2_7                        (0x00004000ul)
#define MMC_VOLT_2_8                        (0x00008000ul)  /* Memory access minimum                                */
#define MMC_VOLT_2_9                        (0x00010000ul)
#define MMC_VOLT_3_0                        (0x00020000ul)
#define MMC_VOLT_3_1                        (0x00040000ul)
#define MMC_VOLT_3_2                        (0x00080000ul)
#define MMC_VOLT_3_3                        (0x00100000ul)
#define MMC_VOLT_3_4                        (0x00200000ul)
#define MMC_VOLT_3_5                        (0x00400000ul)
#define MMC_VOLT_3_6                        (0x00800000ul)

/* ---- Memory card write mode ---- */
#define MMC_OPEN_END                        (0x00000000ul)  /* Transfer mode: open-ended                            */
#define MMC_PRE_DEF                         (0x00000001ul)  /* Transfer mode: pre-defined                           */
#define MMC_RELIB_W                         (0x00000002ul)  /* Transfer mode: reliable write                        */

/* ---- Write protect info --- */
#define MMC_WP_OFF                          (0x0000u)   /* Card is not write protect                                */
#define MMC_WP_HW                           (0x0001u)   /* Card is H/W write protect (Not use to MMC)               */
#define MMC_WP_TEMP                         (0x0002u)   /* Card is TEMP_WRITE_PROTECT                               */
#define MMC_WP_PERM                         (0x0004u)   /* Card is PERM_WRITE_PROTECT                               */
#define MMC_WP_ROM                          (0x0010u)   /* Card is MMC-ROM (Not use to MMC)                         */

/* ---- MMC clock div ---- */
#define MMC_DIV_1024                        (0x00090000ul)  /* MMCCLOCK = IMCLK/1024                                */
#define MMC_DIV_512                         (0x00080000ul)  /* MMCCLOCK = IMCLK/512                                 */
#define MMC_DIV_256                         (0x00070000ul)  /* MMCCLOCK = IMCLK/256                                 */
#define MMC_DIV_128                         (0x00060000ul)  /* MMCCLOCK = IMCLK/128                                 */
#define MMC_DIV_64                          (0x00050000ul)  /* MMCCLOCK = IMCLK/64                                  */
#define MMC_DIV_32                          (0x00040000ul)  /* MMCCLOCK = IMCLK/32                                  */
#define MMC_DIV_16                          (0x00030000ul)  /* MMCCLOCK = IMCLK/16                                  */
#define MMC_DIV_8                           (0x00020000ul)  /* MMCCLOCK = IMCLK/8                                   */
#define MMC_DIV_4                           (0x00010000ul)  /* MMCCLOCK = IMCLK/4                                   */
#define MMC_DIV_2                           (0x00000000ul)  /* MMCCLOCK = IMCLK/2                                   */
#define MMC_DIV_1                           (0x000F0000ul)  /* MMCCLOCK = IMCLK (option)                            */

/* ---- MMC clock define ---- */ /* Max. frequency */
#define MMC_CLK_400kHz                      (0x0000u)   /* 400kHz                                                   */
#define MMC_CLK_1MHz                        (0x0001u)   /* 1MHz                                                     */
#define MMC_CLK_5MHz                        (0x0002u)   /* 5MHz                                                     */
#define MMC_CLK_10MHz                       (0x0003u)   /* 10MHz                                                    */
#define MMC_CLK_20MHz                       (0x0004u)   /* 20MHz (system spec ver3.1)                               */
#define MMC_CLK_26MHz                       (0x0005u)   /* 26MHz (system spec ver4.x)                               */
#define MMC_CLK_52MHz                       (0x0006u)   /* 52MHz (system spec ver4.x)                               */

/* --- MMC specification version ---- */
#define MMC_SPEC_10                         (0)         /* MMC system spec: 1.0-1.2                                 */
#define MMC_SPEC_14                         (1)         /* MMC system spec: 1.4                                     */
#define MMC_SPEC_20                         (2)         /* MMC system spec: 2.0-2.2                                 */
#define MMC_SPEC_30                         (3)         /* MMC system spec: 3.1-3.2-3.31                            */
#define MMC_SPEC_40                         (4)         /* MMC system spec: 4.0-4.1-4.2-4.3-4.4-4.41                */

/* ---- MMC port mode ---- */
#define MMC_PORT_1BIT                       (0x0000)    /* 1bit mode                                                */
#define MMC_PORT_4BIT                       (0x0001)    /* 4bits mode                                               */
#define MMC_PORT_8BIT                       (0x0002)    /* 8bits mode                                               */
#define MMC_PORT_CD                         (0x0003)    /* CD mode                                                  */

/* ---- MMC Card detect interrupt ---- */
#define MMC_CD_INT_DISABLE                  (0x0000u)   /* Card detect interrupt disable                            */
#define MMC_CD_INT_ENABLE                   (0x0001u)   /* Card detect interrupt enable                             */

/* ---- MMC Card detect ---- */
#define MMC_CD_REMOVE                       (0x0000u)   /* Removal                                                  */
#define MMC_CD_INSERT                       (0x0001u)   /* Insert                                                   */

/* ---- Lock/Unlock mode ---- */
#define MMC_UNLOCK_CARD                     (0x00)
#define MMC_SET_PWD                         (0x01)
#define MMC_CLR_PWD                         (0x02)
#define MMC_LOCK_CARD                       (0x04)
#define MMC_FORCE_ERASE                     (0x08)

/* ==== Command type ==== */
/* ---- Basic commands (class0 and class1) ---- */
#define MMC_CMD0                    (0x00000000ul)      /* GO_IDLE_STATE [no resp]                                  */
#define MMC_CMD1                    (0x01400000ul)      /* SD_SEND_OP_COND [R3]                                     */
#define MMC_CMD2                    (0x02800000ul)      /* ALL_SEND_CID [R2]                                        */
#define MMC_CMD3                    (0x03400000ul)      /* SET_RELATIVE_ADDR [R1]                                   */
#define MMC_CMD4                    (0x04000000ul)      /* SET_DSR [no resp]                                        */
#define MMC_CMD5                    (0x05600000ul)      /* SLEEP_AWAKE [R1b]                                        */
#define MMC_CMD6                    (0x06600000ul)      /* SWITCH [R1b]                                             */
#define MMC_CMD6_BGO                (0x06400000ul)      /* SWITCH [R1] Baxkground Operation                         */
#define MMC_CMD7                    (0x07400000ul)      /* SELECT/DESELECT_CARD [R1]                                */
#define MMC_CMD8                    (0x08480000ul)      /* SEND_EXT_CSD [R1:single read]                            */
#define MMC_CMD9                    (0x09800000ul)      /* SEND_CSD [R2]                                            */
#define MMC_CMD10                   (0x0a800000ul)      /* SEND_CID [R2]                                            */
#define MMC_CMD12                   (0x0c600000ul)      /* STOP_TRANSMISSION [R1b]                                  */
#define MMC_CMD13                   (0x0d400000ul)      /* SEND_STATUS [R1]                                         */
#define MMC_CMD13_HPI               (0x0d600000ul)      /* SEND_STATUS [R1b]                                        */
#define MMC_CMD14                   (0x0e480000ul)      /* BUSTEST_R [R1:single read]                               */
#define MMC_CMD15                   (0x0f000000ul)      /* GO_INACTIVE_STATE [no resp]                              */
#define MMC_CMD19                   (0x134c0000ul)      /* BUSTEST_W [R1:single write]                              */

/* ---- Block read commands (class2) ---- */
#define MMC_CMD16                   (0x10400000ul)      /* SET_BLOCK_LEN [R1]                                       */
#define MMC_CMD17                   (0x11480000ul)      /* READ_SINGLE_BLOCK [R1]                                   */
#define MMC_CMD18                   (0x124a0000ul)      /* READ_MULTIPLE_BLOCK [R1:multi read]                      */
#define MMC_CMD23                   (0x17400000ul)      /* SET_BLOCK_COUNT [R1]                                     */

/* ---- Block write commands (class4) ---- */
#define MMC_CMD24                   (0x184c0000ul)      /* WRITE_SINGLE_BLOCK [R1:single write]                     */
#define MMC_CMD25                   (0x194e0000ul)      /* WRITE_MULTIPLE_BLOCK [R1:multi write]                    */
#define MMC_CMD26                   (0x1a4c0000ul)      /* PROGRAM_CID [R1:single write]                            */
#define MMC_CMD27                   (0x1b4c0000ul)      /* PROGRAM_CSD [R1:single write]                            */

/* ---- Protection commands (class6) ---- */
#define MMC_CMD28                   (0x1c600000ul)      /* SET_WRITE_PROT [R1b]                                     */
#define MMC_CMD29                   (0x1d600000ul)      /* CLR_WRITE_PROT [R1b]                                     */
#define MMC_CMD30                   (0x1e480000ul)      /* SEND_WRITE_PROT [R1:single read]                         */

/* ---- Erase commands (class5) ---- */
#define MMC_CMD35                   (0x23400000ul)      /* ERASE_GROUP_START [R1]                                   */
#define MMC_CMD36                   (0x24400000ul)      /* ERASE_GROUP_END [R1]                                     */
/* #define MMC_CMD38                (0x26600000ul) */   /* ERASE [R1b]                                              */
#define MMC_CMD38                   (0x26400000ul)      /* ERASE [R1b] busy check by CMD13                          */

/* ---- I/O mode commands (class9) ---- */
#define MMC_CMD39                   (0x27400000ul)      /* FAST_IO [R4]                                             */
#define MMC_CMD40_CMD               (0x28400000ul)      /* GO_IRQ_STATE [R5] Send CMD                               */
#define MMC_CMD40_RSP               (0x284000c0ul)      /* GO_IRQ_STATE [R5] Send RSP                               */

/* ---- Lock commands (class7) ---- */
#define MMC_CMD42                   (0x2a6c0000ul)      /* LOCK_UNLOCK [R1b:single write]                           */

/* ---- Application commands (class8) ---- */
#define MMC_CMD55                   (0x37400000ul)      /* APP_CMD [R1]                                             */
#define MMC_CMD56_R                 (0x386a0000ul)      /* GEN_CMD_R [R1b:multi read]                               */
#define MMC_CMD56_W                 (0x386e0000ul)      /* GEN_CMD_W [R1b:multi write]                              */

/* ==== Constants ==== */
/* ---- Response type  ---- */
#define MMC_RESP_NON                (0)                 /* No response                                              */
#define MMC_RESP_R1                 (1)                 /* Nomal response                                           */
#define MMC_RESP_R1b                (2)                 /* Nomal response with an optional busy signal              */
#define MMC_RESP_R2_CID             (4)                 /* CID register                                             */
#define MMC_RESP_R2_CSD             (5)                 /* CSD register                                             */
#define MMC_RESP_R3                 (6)                 /* OCR register                                             */
#define MMC_RESP_R6                 (7)                 /* Published RCA response                                   */
#define MMC_RESP_R4                 (8)                 /* ATA Taskfile register                                    */
#define MMC_RESP_R5                 (9)                 /* IRQ data                                                 */
#define MMC_RESP_R7                 (10)                /* Card Interface Condition response                        */
#define MMC_RESP_AUTO12             (11)                /* Auto CMD12 response                                      */


/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/
/* ---- Error code ---- */
typedef enum e_mmc_status
{
    MMC_SUCCESS_LOCKED_CARD     =   1,                  /* OK but card is locked status.                            */
    MMC_SUCCESS                 =   0,                  /* OK                                                       */
    MMC_ERR                     =  -1,                  /* General error                                            */
    MMC_ERR_WP                  =  -2,                  /* Write protect error                                      */
    MMC_ERR_RO                  =  -3,                  /* Read only error                                          */
    MMC_ERR_RES_TOE             =  -4,                  /* Response time out error                                  */
    MMC_ERR_CARD_TOE            =  -5,                  /* Card time out error                                      */
    MMC_ERR_END_BIT             =  -6,                  /* End bit error                                            */
    MMC_ERR_CRC                 =  -7,                  /* CRC error                                                */
    MMC_ERR_CARD_RES            =  -8,                  /* Card response error                                      */
    MMC_ERR_HOST_TOE            =  -9,                  /* Host time out error                                      */
    MMC_ERR_CARD_ERASE          = -10,                  /* Card erase error                                         */
    MMC_ERR_CARD_LOCK           = -11,                  /* Card lock error                                          */
    MMC_ERR_CARD_UNLOCK         = -12,                  /* Card unlock error                                        */
    MMC_ERR_CARD_CRC            = -13,                  /* Host CRC error                                           */
    MMC_ERR_CARD_ECC            = -14,                  /* Card internal ECC error                                  */
    MMC_ERR_CARD_CC             = -15,                  /* Card internal error                                      */
    MMC_ERR_CARD_ERROR          = -16,                  /* Unknown card error                                       */
    MMC_ERR_CARD_TYPE           = -17,                  /* Non supported card type                                  */
    MMC_ERR_NO_CARD             = -18,                  /* No card                                                  */
    MMC_ERR_ILL_READ            = -19,                  /* Illegal buffer read                                      */
    MMC_ERR_ILL_WRITE           = -20,                  /* Illegal buffer write                                     */
    MMC_ERR_AKE_SEQ             = -21,                  /* Sequence of authentication process                       */
    MMC_ERR_OVERWRITE           = -22,                  /* CID/CSD overwrite error                                  */
    MMC_ERR_CPU_IF              = -30,                  /* Target MCU interface function error                      */
    MMC_ERR_STOP                = -31,                  /* User stop                                                */
    MMC_ERR_CMD                 = -32,                  /* Command isuue error                                      */
    MMC_ERR_BUFACC              = -33,                  /* Buffer access error                                      */
    MMC_ERR_WRITE               = -34,                  /* Write error                                              */
    MMC_ERR_READ                = -35,                  /* Read error                                               */
    MMC_ERR_RESPIND             = -36,                  /* Resp index error                                         */
    MMC_ERR_RESP                = -37,                  /* Resp error                                               */
    MMC_ERR_CRC_TOE             = -38,                  /* CRC status timeout error                                 */
    MMC_ERR_WRITE_TOE           = -39,                  /* Write timeout error                                      */
    MMC_ERR_READ_TOE            = -40,                  /* Read timeout error                                       */
    MMC_ERR_RESPB_TOE           = -41,                  /* Resp busy timeout error                                  */
    MMC_ERR_RESP_TOE            = -42,                  /* Resp timeout error                                       */
    MMC_ERR_FAST_IO             = -43,                  /* FAST IO operation error                                  */
    MMC_ERR_CHANGE_BUS          = -44,                  /* Change bus width error                                   */
    MMC_ERR_IP                  = -45,                  /* HW IP error                                              */
    MMC_ERR_UNKNOWN             = -46,                  /* Card internal error related to command                   */
    MMC_ERR_SWITCH              = -47,                  /* Switch error                                             */
    MMC_ERR_BOOT_ACK            = -48,                  /* Boot Acknowledge error                                   */
    MMC_ERR_CSD_RLEN            = -49,                  /* CSD read block length error                              */
    MMC_ERR_CSD_VER             = -50,                  /* CSD register version error                               */
    MMC_ERR_SCR_VER             = -51,                  /* SCR register version error                               */
    MMC_ERR_FILE_FORMAT         = -52,                  /* CSD register file format error                           */
    MMC_ERR_NOTSUP_CMD          = -53,                  /* Not supported command                                    */
    MMC_ERR_CSD_WLEN            = -54,                  /* CSD write block length error                             */
    MMC_ERR_ILL_FUNC            = -60,                  /* Invalid function request error                           */
    MMC_ERR_IFCOND_VER          = -70,                  /* Interface condition version error                        */
    MMC_ERR_IFCOND_VOLT         = -71,                  /* Interface condition voltage error                        */
    MMC_ERR_IFCOND_ECHO         = -72,                  /* Interface condition echo back pattern error              */
    MMC_ERR_OUT_OF_RANGE        = -80,                  /* Argument was out of range                                */
    MMC_ERR_ADDRESS_ERROR       = -81,                  /* Misaligned address                                       */
    MMC_ERR_BLOCK_LEN_ERROR     = -82,                  /* Transferred block length is not allowed.                 */
    MMC_ERR_ILLEGAL_COMMAND     = -83,                  /* Command not legal                                        */
    MMC_ERR_RESERVED_ERROR18    = -84,                  /* Reserved bit 18 error                                    */
    MMC_ERR_RESERVED_ERROR17    = -85,                  /* Reserved bit 17 error                                    */
    MMC_ERR_CBSY_ERROR          = -87,                  /* MMC_CECLKCTRL bit 31 MMC bus busy error                  */
    MMC_ERR_NO_RESP_ERROR       = -88,                  /* MMC_CECLKCTRL bit  0 No Response error                   */
    MMC_ERR_ADDRESS_BOUNDARY    = -89,                  /* Not specified buffer address in 4-byte unit              */
    MMC_ERR_INTERNAL            = -99                   /* Driver software internal error                           */
} mmc_status_t;

typedef enum e_mmc_enum_cmd
{
    MMC_SET_STOP,
} mmc_enum_cmd_t;

typedef enum e_mmc_enum_trans
{
    MMC_SET_TRANS_STOP,
    MMC_SET_TRANS_START
} mmc_enum_trans_t;

typedef struct
{
    mmc_enum_cmd_t      cmd;
    uint32_t            mode;                           /* Lock/Unlock operation code                               */
    uint8_t             *p_buff;
    uint32_t            size;
}mmc_cmd_t;

typedef struct
{
    uint32_t            mode;                           /* MMC Driver operation mode                                */
    uint32_t            voltage;                        /* Operation voltage                                        */
}mmc_cfg_t;

typedef struct
{
    uint8_t             *p_buff;
    uint32_t            lbn;
    int32_t             cnt;
    uint32_t            mode;
    uint32_t            rw_mode;
}mmc_access_t;

typedef struct
{
    uint32_t            card_sector_size;               /* Sector size (user area)                                  */
    uint8_t             csd_structure;                  /* CSD structure
                                                           0 : CSD version No.1.0
                                                           1 : CSD version No.1.1
                                                           2 : CSD version No.1.2
                                                           3 : Version is coded in the CSD_STRUCTURE byte in
                                                               the EXT_CSD register                                 */
    uint8_t             speed_mode;                     /* Card speed mode
                                                           Supported speed bit 5,4 : 0 0 Backward-compatible
                                                                                     0 1 High-speed 26MHz Max
                                                                                     1 1 High-speed 52MHz Max
                                                           Current speed   bit 1,0 : 0 0 Backward-compatible
                                                                                     0 1 High-speed 26MHz Max
                                                                                     1 1 High-speed 52MHz Max       */
    uint8_t             csd_spec;                       /* MMC spec version                                         */
    uint8_t             if_mode;                        /* Bus width                                                */
    uint8_t             density_type;                   /* Card density type                                        */
    uint8_t             rsv[3];                         /* Reserve                                                  */
}mmc_card_status_t;

typedef struct
{
    uint32_t            ocr[1];                         /* OCR value                                                */
    uint32_t            cid[4];                         /* CID value                                                */
    uint32_t            csd[4];                         /* CSD value                                                */
    uint32_t            dsr[1];                         /* DSR value                                                */
    uint32_t            rca[1];                         /* RCA value                                                */
}mmc_card_reg_t;


/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/
mmc_status_t R_MMCIF_Open(uint32_t channel, void *p_mmc_WorkArea);
mmc_status_t R_MMCIF_Control(uint32_t channel, mmc_cmd_t *p_mmc_Cmd);
mmc_status_t R_MMCIF_Cd_Int(uint32_t channel, int32_t enable, mmc_status_t (*callback)(int32_t));
mmc_status_t R_MMCIF_Get_CardDetection(uint32_t channel);
mmc_status_t R_MMCIF_Mount(uint32_t channel, mmc_cfg_t *p_mmc_Config);
mmc_status_t R_MMCIF_Read_Memory(uint32_t channel, mmc_access_t *p_mmc_Access);
mmc_status_t R_MMCIF_Read_Memory_Software_Trans(uint32_t channel, mmc_access_t *p_mmc_Access);
mmc_status_t R_MMCIF_Write_Memory(uint32_t channel, mmc_access_t *p_mmc_Access);
mmc_status_t R_MMCIF_Write_Memory_Software_Trans(uint32_t channel, mmc_access_t *p_mmc_Access);
mmc_status_t R_MMCIF_Get_ModeStatus(uint32_t channel, uint8_t * p_mode);
mmc_status_t R_MMCIF_Get_CardStatus(uint32_t channel, mmc_card_status_t *p_mmc_CardStatus);
mmc_status_t R_MMCIF_Get_CardInfo(uint32_t channel, mmc_card_reg_t *p_mmc_CardReg);
mmc_status_t R_MMCIF_Unmount(uint32_t channel);
mmc_status_t R_MMCIF_IntCallback(uint32_t channel, mmc_status_t (*callback)(int32_t));
void         R_MMCIF_Int_Handler0(void * vect);
void         R_MMCIF_Int_Handler1(void * vect);
mmc_status_t R_MMCIF_Get_ErrCode(uint32_t channel);
mmc_status_t R_MMCIF_Close(uint32_t channel);
mmc_status_t R_MMCIF_Get_BuffRegAddress(uint32_t channel, uint32_t *p_reg_buff);
mmc_status_t R_MMCIF_Get_ExtCsd(uint32_t channel, uint8_t *p_ext_csd_buffer);
uint32_t     R_MMCIF_GetVersion(void);
void         R_MMCIF_1ms_Interval(void);
mmc_status_t R_MMCIF_Set_DmacDtc_Trans_Flg(uint32_t channel, mmc_enum_trans_t flg);
mmc_status_t R_MMCIF_Set_LogHdlAddress(uint32_t user_long_que);
uint32_t     R_MMCIF_Log(uint32_t flg, uint32_t fid, uint32_t line);


/**********************************************************************************************************************
Include specifics for chosen MCU
**********************************************************************************************************************/


#endif /* _MMC_IF_H */
