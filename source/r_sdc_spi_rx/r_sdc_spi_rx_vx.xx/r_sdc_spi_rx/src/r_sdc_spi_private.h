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
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * File Name    : r_sdc_spi_private
 * Version      : 1.0
 * Description  : SPI mode SD card driver private header.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 28.12.2023 1.00     First Release
 *********************************************************************************************************************/
#ifndef R_SDC_SPI_RX_PRIVATE_H
#define R_SDC_SPI_RX_PRIVATE_H

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_sdc_spi_rx_if.h"
#include "r_gpio_rx_if.h"
#include "r_memdrv_rx_if.h"
#include "r_sdc_spi_rx_config.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
/* **** Definitions of GPIO FIT direction **** */
#define SDC_GPIO_DIRECTION_INPUT (0)
#define SDC_GPIO_DIRECTION_OUTPUT (1)
#define SDC_GPIO_LEVEL_HIGH (1)
#define SDC_GPIO_LEVEL_LOW  (0)

/* **** Common command **** */
#define SDC_SPI_CMD0    (uint8_t)(0x40U | 0U)
#define SDC_SPI_CMD2    (uint8_t)(0x40U | 2U)
#define SDC_SPI_CMD3    (uint8_t)(0x40U | 3U)
#define SDC_SPI_CMD6    (uint8_t)(0x40U | 6U)
#define SDC_SPI_CMD7    (uint8_t)(0x40U | 7U)
#define SDC_SPI_CMD8    (uint8_t)(0x40U | 8U)
#define SDC_SPI_CMD9    (uint8_t)(0x40U | 9U)
#define SDC_SPI_CMD10   (uint8_t)(0x40U | 10U)
#define SDC_SPI_CMD12   (uint8_t)(0x40U | 12U)
#define SDC_SPI_CMD13   (uint8_t)(0x40U | 13U)
#define SDC_SPI_CMD17   (uint8_t)(0x40U | 17U)
#define SDC_SPI_CMD18   (uint8_t)(0x40U | 18U)
#define SDC_SPI_CMD24   (uint8_t)(0x40U | 24U)
#define SDC_SPI_CMD25   (uint8_t)(0x40U | 25U)
#define SDC_SPI_CMD55   (uint8_t)(0x40U | 55U)
#define SDC_SPI_CMD58   (uint8_t)(0x40U | 58U)
#define SDC_SPI_CMD59   (uint8_t)(0x40U | 59U)

/* **** Application command **** */
#define SDC_SPI_ACMD13  (uint8_t)(0x40U | 13U)
#define SDC_SPI_ACMD23  (uint8_t)(0x40U | 23U)
#define SDC_SPI_ACMD41  (uint8_t)(0x40U | 41U)
#define SDC_SPI_ACMD51  (uint8_t)(0x40U | 51U)

/* **** CMD7 option data **** */
#define SDC_SPI_RES_R7IDX_CHKPTN    (4)
#define SDC_SPI_RES_R7IDX_VHS       (3)
#define SDC_SPI_RES_R7_CHKPTN       (uint8_t)(SDC_SPI_CMD8_CHKPTN)
#define SDC_SPI_RES_R7_VHS          (uint8_t)(SDC_SPI_CMD8_VHS >> 8)

/* **** CMD8 option data **** */
#define SDC_SPI_CMD8_VHS            (uint32_t)(0x00000100)
#define SDC_SPI_CMD8_CHKPTN         (uint32_t)(0x000000AA)

/* **** Command argument **** */
#define SDC_SPI_ACMD41_HCS_ON   (uint32_t)(0x40000000)
#define SDC_SPI_ACMD41_HCS_OFF  (uint32_t)(0x00000000)
#define SDC_SPI_CRC_OP_ON       (uint32_t)(0x00000001)
#define SDC_SPI_CRC_OP_OFF      (uint32_t)(0x00000000)

/* **** Command response size  **** */
#define SDC_SPI_R1_SIZE         (uint8_t)(1)
#define SDC_SPI_R1B_SIZE        (uint8_t)(1)
#define SDC_SPI_R2_SIZE         (uint8_t)(2)
#define SDC_SPI_R3_SIZE         (uint8_t)(5)
#define SDC_SPI_R7_SIZE         (uint8_t)(5)
#define SDC_SPI_MAX_RES_SIZE    (uint8_t)(5)

/* **** Access size information **** */
#define SDC_SPI_BLK_SIZE            (uint16_t)(512)
#define SDC_SPI_CRC_SIZE            (uint16_t)(2)
#define SDC_SPI_CSD_SIZE            (uint16_t)(16)
#define SDC_SPI_CID_SIZE            (uint16_t)(16)
#define SDC_SPI_OCR_SIZE            (uint16_t)(4)
#define SDC_SPI_SCR_SIZE            (uint16_t)(8)
#define SDC_SPI_SDSTAT_SIZE         (uint16_t)(64)
#define SDC_SPI_SWSTAT_SIZE         (uint16_t)(64)
#define SDC_SPI_READ_BLK_LEN_MIN    (8)
#define SDC_SPI_READ_BLK_LEN_MAX    (12)
#define SDC_SPI_CMD_SIZE            (uint16_t)(6)

/* **** Access cycle information **** */
#define SDC_SPI_NRC_MIN             (uint16_t)(1)
#define SDC_SPI_NCR_MIN             (uint16_t)(1)
#define SDC_SPI_NCR_MAX             (uint16_t)(8)
#define SDC_SPI_NCX_MAX             (uint16_t)(9)
#define SDC_SPI_NAC_MIN             (uint16_t)(1)
#define SDC_SPI_NWR_MIN             (uint16_t)(1)
#define SDC_SPI_NBR_MIN             (uint16_t)(1)
#define SDC_SPI_SPI_SEL             (uint16_t)(16)

/* **** Data index information **** */
#define SDC_SPI_RES_R1IDX       (0)
#define SDC_SPI_RES_R2IDX       (1)
#define SDC_SPI_RES_R3IDX       (1)
#define SDC_SPI_CMD_CMD_IDX     (0)
#define SDC_SPI_CMD_ARG_IDX     (1)
#define SDC_SPI_CMD_CRC_IDX     (5)
#define SDC_SPI_CMD_NCR_IDX     (6)
#define SDC_SPI_CSD_CRC7IDX     (15)
#define SDC_SPI_CID_CRC7IDX     (15)

/* **** Data response **** */
#define SDC_SPI_DATARES_SIZE        (uint16_t)(1)
#define SDC_SPI_DATARES_MASK        (uint8_t)(0x1F)
#define SDC_SPI_DATARES_OK          (uint8_t)(0x05)
#define SDC_SPI_DATARES_CRCERR      (uint8_t)(0x0B)
#define SDC_SPI_DATARES_WERR        (uint8_t)(0x0D)

/* **** Token information **** */
#define SDC_SPI_DTOKEN_SIZE     (uint16_t)(1)
#define SDC_SPI_DTOKEN_STR_SR   (uint8_t)(0xFE)
#define SDC_SPI_DTOKEN_STR_MR   (uint8_t)(0xFE)
#define SDC_SPI_DTOKEN_STR_SW   (uint8_t)(0xFE)
#define SDC_SPI_DTOKEN_STR_MW   (uint8_t)(0xFC)
#define SDC_SPI_DTOKEN_STP_MW   (uint8_t)(0xFD)

/* **** Status sequence **** */
#define SDC_SPI_IDLE_STAT   (uint8_t)(1)
#define SDC_SPI_OTHER_STAT  (uint8_t)(2)
#define SDC_SPI_ALL_STAT    (uint8_t)(3)

/* **** CSD version **** */
#define SDC_SPI_CSD_VER1 (SDC_SPI_FALSE)
#define SDC_SPI_CSD_VER2 (SDC_SPI_TRUE)

/* **** Card structure information**** */
#define SDC_SPI_CARD_UNDETECT   (uint16_t)(0x0000)
#define SDC_SPI_CARD_SD         (uint16_t)(0x0002)
#define SDC_SPI_CARD_SDHC       (uint16_t)(0x0004)
#define SDC_SPI_CARD_SDXC       (uint16_t)(0x0008)
#define SDC_SPI_CARD_SDUC       (uint16_t)(0x00FF)

/* **** Card status **** */
#define SDC_SPI_CARD_WR     (uint16_t)(0x0100)
#define SDC_SPI_CARD_ROM    (uint16_t)(0x0200)
#define SDC_SPI_CARD_OTP    (uint16_t)(0x0400)

/* **** Protect status **** */
#define SDC_SPI_NO_PROTECT      (uint8_t)(0x00)
#define SDC_SPI_W_PROTECT_HARD  (uint8_t)(0x01)
#define SDC_SPI_W_PROTECT_SOFT  (uint8_t)(0x02)

/* **** Write information **** */
#define SDC_SPI_MODE_NORMAL         (uint8_t)(0x00)
#define SDC_SPI_MODE_DIRECT         (uint8_t)(0x01)
#define SDC_SPI_MODE_FORCED_W       (uint8_t)(0x02)
#define SDC_SPI_MODE_WRITE          (uint8_t)(0x03)

/* **** CSD struct version **** */
#define SDC_SPI_CSD_STRUCTURE_V1 (uint8_t)(0x00)
#define SDC_SPI_CSD_STRUCTURE_V2 (uint8_t)(0x40)

/* **** Wait ready data **** */
#define SDC_SPI_READY_SIZE          (uint16_t)(1)
#define SDC_SPI_READY_DATA          (uint8_t)(0xFF)


/* **** Log file id and error id **** */
#define SDC_SPI_DRIVER_ID       (9)
#define SDC_SPI_DEBUG_ERR_ID    (1)
#define SDC_SPI_RX              (0)
#define SDC_SPI_OPEN            (1)
#define SDC_SPI_CLOSE           (2)
#define SDC_SPI_GETCARDDETECT   (3)
#define SDC_SPI_INITIALIZE      (4)
#define SDC_SPI_READ            (5)
#define SDC_SPI_WRITE           (6)
#define SDC_SPI_SUB             (7)

/* **** Shift number **** */
#define SDC_SPI_BIT_SHIFT_8     (8)
#define SDC_SPI_BIT_SHIFT_16    (16)
#define SDC_SPI_BIT_SHIFT_21    (21)
#define SDC_SPI_BIT_SHIFT_24    (24)
#define SDC_SPI_BIT_SHIFT_27    (27)
#define SDC_SPI_BIT_SHIFT_31    (31)

/* **** Definitions of other **** */
#define SDC_SPI_MEMSIZE_DUMMY   (uint32_t)(0xFFFFFFFF)  /* Memory size dummy data */
#define SDC_SPI_DUMMY_DATA      (0xff)                  /* Dummy send data */

#define SDC_SPI_SPIMODE_SEL (16)                /* SPI mode select clock count */
#define SDC_SPI_READY_WAIT  (uint16_t)(50000)   /* Ready wait try count */
#define SDC_SPI_RTY_CNT     (uint8_t)(5)        /* CMD12 command retry count */

#define SDC_SPI_RES2_CHKDATA        (uint8_t)(0x00)
#define SDC_SPI_CSD_STRUCTER_MASK   (0x00ff)        /* CSD structer mask data */
#define SDC_SPI_OCR_CCS             (uint8_t)(0x40) /* Card capacity bit */
#define SDC_SPI_OCR_BUSY            (0x80)          /* OCR busy */

#define SDC_SPI_ENABLE  (1) /* Enable */
#define SDC_SPI_DISABLE (0) /* Disable*/
#define SDC_SPI_TRUE    (1) /* True */
#define SDC_SPI_FALSE   (0) /* False */

#define SDC_SPI_CARD_NUM (2)	/* Max card number */


/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
/* **** CSD information struct **** */
typedef struct
{
    uint32_t taac;
    uint32_t nac;
    uint32_t memsize;
    uint32_t maxblkno;
    uint16_t nsac;
    uint8_t  wp;
    uint8_t reserve[1];

}csd_info_t;

/* **** SPI mode SDC card driver handler **** */
typedef struct
{
    uint16_t media;                             /* Card information */
    uint16_t addrunit;                          /* Address multiple unit */
    uint8_t wp;                                 /* Write protection */
    uint32_t bitrate;                           /* Bit rate */
    csd_info_t csdinfo;                         /* CSD information struct */
    uint8_t csdbuf[SDC_SPI_CSD_SIZE];           /* CSD Register buffer */
    uint8_t cidbuf[SDC_SPI_CID_SIZE];           /* CID Register buffer */
    uint8_t ocrbuf[SDC_SPI_OCR_SIZE];           /* ORC Register buffer */
    uint8_t scrbuf[SDC_SPI_SCR_SIZE];           /* SCR Register buffer */
    uint8_t sdstatbuf[SDC_SPI_SDSTAT_SIZE];     /* SD card status buffer */
    gpio_port_pin_t cd_pin;                     /* CD pin */
    gpio_port_pin_t cs_pin;                     /* CS pin */
    gpio_port_pin_t wp_pin;                     /* WP pin */
    uint8_t use_cd_pin;                         /* Select whether to use CD pin */
    uint8_t use_cs_pin;                         /* Select whether to use CS pin */
    uint8_t use_wp_pin;                         /* Select whether to use WP pin */
    uint8_t dev_no;                             /* MEMDRV FIT device to use number  */
    uint8_t init_flag;                          /* Module initialize flag */
} sdc_spi_hndl_t;

/* **** Exchange short type　union **** */
typedef union
{
    uint16_t    us;
    uint8_t     uc[2];
} exchg_short_t;

/* **** Exchange long type　union **** */
typedef union
{
    uint32_t    ul;
    uint8_t     uc[4];
} exchg_long_t;


/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
/* **** r_sdc_spi_rx.c **** */
sdc_spi_status_t r_sdc_spi_check_cardno_and_hndl (uint32_t card_no);

/* **** r_sdc_spi_open.c **** */
sdc_spi_status_t r_sdc_spi_open (uint32_t card_no, uint32_t dev_no, void * p_sdc_spi_workarea);
sdc_spi_hndl_t * r_sdc_spi_handle_get (uint32_t card_no);
void r_sdc_spi_handle_clear(uint32_t card_no);

/* **** r_sdc_spi_close.c **** */
sdc_spi_status_t r_sdc_spi_close (uint32_t card_no);

/* **** r_sdc_spi_getcarddetection.c **** */
sdc_spi_status_t r_sdc_spi_getcarddetection (uint32_t card_no);

/* **** r_sdc_spi_initialize.c **** */
sdc_spi_status_t r_sdc_spi_initialize (uint32_t card_no, sdc_spi_cfg_t * p_sdc_spi_config, uint32_t init_type);
sdc_spi_status_t r_sdc_spi_initialize_sub (sdc_spi_hndl_t * p_hndl, sdc_spi_cfg_t * p_sdc_spi_config, uint32_t init_type);
sdc_spi_status_t r_sdc_spi_reset_sd (sdc_spi_hndl_t * p_hndl, sdc_spi_cfg_t * p_sdc_spi_config, uint32_t init_type);
sdc_spi_status_t r_sdc_spi_get_wp (sdc_spi_hndl_t * p_hndl);
sdc_spi_status_t r_sdc_spi_get_sdcard_type (sdc_spi_hndl_t * p_hndl);
int32_t r_sdc_spi_get_sd_specver (sdc_spi_hndl_t * p_hndl);
sdc_spi_status_t r_sdc_spi_get_sd_csdinfov1 (sdc_spi_hndl_t * p_hndl);
sdc_spi_status_t r_sdc_spi_get_sd_csdinfov2 (sdc_spi_hndl_t * p_hndl, uint32_t * p_csize);
sdc_spi_status_t r_sdc_spi_get_csd (sdc_spi_hndl_t * p_hndl);
sdc_spi_status_t r_sdc_spi_get_cid (sdc_spi_hndl_t * p_hndl);
sdc_spi_status_t r_sdc_spi_get_ocr (sdc_spi_hndl_t * p_hndl, uint8_t stat);
sdc_spi_status_t r_sdc_spi_chk_ccs (sdc_spi_hndl_t * p_hndl);
sdc_spi_status_t r_sdc_spi_chk_csdver (sdc_spi_hndl_t * p_hndl, uint8_t * p_csdver);
sdc_spi_status_t r_sdc_spi_sd_init (sdc_spi_hndl_t * p_hndl, uint8_t csdver);
sdc_spi_status_t r_sdc_spi_end (uint32_t card_no);
void r_sdc_spi_handle_initialize (sdc_spi_hndl_t * p_hndl);
void r_sdc_spi_port_initialize (sdc_spi_hndl_t * p_hndl);
void r_sdc_spi_mem_clear (uint8_t * p_ptr,uint32_t cnt);

/* **** r_sdc_spi_write.c **** */
sdc_spi_status_t r_sdc_spi_write (uint32_t card_no, sdc_spi_access_t * p_sdc_spi_access);
sdc_spi_status_t r_sdc_spi_write_sb (sdc_spi_hndl_t * p_hndl, sdc_spi_access_t * p_sdc_spi_access);
sdc_spi_status_t r_sdc_spi_sbwrite_start (sdc_spi_hndl_t * p_hndl, uint16_t addrunit, uint32_t blkno);
sdc_spi_status_t r_sdc_spi_sbwrite_blkdata (sdc_spi_hndl_t * p_hndl, uint8_t * p_pdata);
sdc_spi_status_t r_sdc_spi_sbwrite_end (sdc_spi_hndl_t * p_hndl);
sdc_spi_status_t r_sdc_spi_write_mb (sdc_spi_hndl_t * p_hndl, sdc_spi_access_t * p_sdc_spi_access);
sdc_spi_status_t r_sdc_spi_mbwrite_start (sdc_spi_hndl_t * p_hndl, uint16_t addrunit, uint32_t blkno);
sdc_spi_status_t r_sdc_spi_mbwrite_blkdata (sdc_spi_hndl_t * p_hndl, uint8_t * p_data);
sdc_spi_status_t r_sdc_spi_mbwrite_end (sdc_spi_hndl_t * p_hndl);
sdc_spi_status_t r_sdc_spi_write_data_crc (sdc_spi_hndl_t * p_hndl, uint8_t * p_data);
sdc_spi_status_t r_sdc_spi_tx_data_crc16 (sdc_spi_hndl_t * p_hndl, uint16_t txcnt, uint8_t * p_data, uint16_t * p_crc16);
sdc_spi_status_t r_sdc_spi_long_wait (sdc_spi_hndl_t * p_hndl);
sdc_spi_status_t r_sdc_spi_chk_datares (sdc_spi_hndl_t * p_hndl);
sdc_spi_status_t r_sdc_spi_preerase (sdc_spi_hndl_t * p_hndl, uint32_t blkcnt);

/* **** r_sdc_spi_read.c **** */
sdc_spi_status_t r_sdc_spi_read (uint32_t card_no, sdc_spi_access_t * p_sdc_spi_access);
sdc_spi_status_t r_sdc_spi_read_sb (sdc_spi_hndl_t * p_hndl, sdc_spi_access_t * p_sdc_spi_access);
sdc_spi_status_t r_sdc_spi_sbread_start (sdc_spi_hndl_t * p_hndl, uint16_t addrunit, uint32_t blkno);
sdc_spi_status_t r_sdc_spi_sbread_end (sdc_spi_hndl_t * p_hndl);
sdc_spi_status_t r_sdc_spi_read_mb (sdc_spi_hndl_t * p_hndl, sdc_spi_access_t * p_sdc_spi_access);
sdc_spi_status_t r_sdc_spi_mbread_start (sdc_spi_hndl_t * p_hndl, uint16_t addrunit, uint32_t blkno);
sdc_spi_status_t r_sdc_spi_mbread_end (sdc_spi_hndl_t * p_hndl);
sdc_spi_status_t r_sdc_spi_read_blkdata (sdc_spi_hndl_t * p_hndl, uint8_t * p_data);
sdc_spi_status_t r_sdc_spi_read_startblocktoken (sdc_spi_hndl_t * p_hndl);

/* **** r_sdc_spi_sub.c **** */
sdc_spi_status_t r_sdc_spi_send_cmd (sdc_spi_hndl_t * p_hndl, uint8_t cmd, uint32_t arg, uint8_t ressize, uint8_t stat);
sdc_spi_status_t r_sdc_spi_tx_dummy_data (sdc_spi_hndl_t * p_hndl, uint16_t txcnt);
sdc_spi_status_t r_sdc_spi_tx_data (sdc_spi_hndl_t * p_hndl, uint16_t txcnt, uint8_t * p_data);
sdc_spi_status_t r_sdc_spi_rx_data (sdc_spi_hndl_t * p_hndl, uint16_t rxcnt, uint8_t * p_data);
sdc_spi_status_t r_sdc_spi_wait_ready (sdc_spi_hndl_t * p_hndl);
sdc_spi_status_t r_sdc_spi_chk_sts (sdc_spi_hndl_t * p_hndl);
sdc_spi_status_t r_sdc_spi_read_data_crc (sdc_spi_hndl_t * p_hndl, uint16_t rxcnt, uint8_t * p_data);
sdc_spi_status_t r_sdc_spi_send_cmd12 (sdc_spi_hndl_t * p_hndl);
sdc_spi_status_t r_sdc_spi_chk_rxdata (sdc_spi_hndl_t * p_hndl, uint32_t rtycnt, uint8_t chkdata);
sdc_spi_status_t r_sdc_spi_rx_data_crc16 (sdc_spi_hndl_t * p_hndl, uint16_t rxcnt, uint8_t * p_data, uint16_t * p_crc16);
uint32_t r_sdc_spi_exchglong (uint32_t chgdata);
uint16_t r_sdc_spi_exchg_crc16 (void);
uint8_t r_sdc_spi_crc7 (uint8_t * p_data, uint16_t crccnt);
uint16_t r_sdc_spi_crc16 (uint8_t * p_data, uint16_t crccnt);

/* **** r_sdc_spi_getcardinfo.c **** */
sdc_spi_status_t r_sdc_spi_getcardinfo (uint32_t card_no, sdc_spi_card_reg_t * p_sdc_spi_card_reg);

/* **** r_sdc_spi_getcardstatus.c **** */
sdc_spi_status_t r_sdc_spi_getcardstatus (uint32_t card_no, sdc_spi_card_status_t * p_sdc_spi_card_status);

/* **** r_sdc_spi_log.c **** */
sdc_spi_status_t r_sdc_spi_setloghdladdress (uint32_t user_long_que);
int32_t r_sdc_spi_log (uint32_t flg, uint32_t fid, uint32_t line);

#endif
