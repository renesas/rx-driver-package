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
* Copyright (C) 2004(2005-2019) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* File Name    : r_eeprom_spi_if.h
* Version      : 3.01
* Description  : EEPROM driver interface header file
*************************************************************************************************/
/************************************************************************************************
* History      : Date          No.             Comment
*              : 2004/12/06                    wrote it.
*              : 2004/12/08                    eep_Write_Status()
*              :                                   Deleted review result.
*              : 2004/12/10                    Moved eep_PWrite_En(), eep_Write_Di()
*              :                                   to eep_io.c.
*              : 2004/12/27                    Modified for MISRA-C.
*              : 2005/06/09                    Changed definition of EEPROM capacity.
*              : 2006/01/27                    Added SIO transmission/DMA interruption setting.
*              : 2006.01.27    Ver1.10 --------------------------------------------------------
*              : 2007.02.27    Ver1.20 --------------------------------------------------------
*              : 2007.11.07    Ver1.21 --------------------------------------------------------
*              : 2009.01.31    #### CF-0005    Divided between SIO processing and CRC calculation.
*              : 2009.03.06    Ver1.30 --------------------------------------------------------
*              : 2011.02.15    Ver2.00 --------------------------------------------------------
*              : 2011.04.15    Ver2.01 --------------------------------------------------------
*              : 2012.03.19    Changed History to English comment.
*              : 2012.03.19    Ver2.02 --------------------------------------------------------
*              : 2013.09.30    Ver2.03 Corrected comments.
*
*              : DD.MM.YYYY Version  Description
*              : 28.11.2014 2.30     Revised functions of same as Ver.2.30 of other middleware.
*              : 30.01.2015 2.31     Added RX71M.
*              : 29.05.2015 2.32     Added RX231 and RX230.
*              : 24.12.2015 2.33     Added RX130, RX23T and RX24T.
*              : 31.07.2017 2.34     Modified Application note.
*              : 21.12.2018 3.00     Change eeprom drive interface to Memory Access Driver Interface
*              : 04.04.2019 3.01     Added support for GNUC and ICCRX.
*                                    Fixed coding style.
*************************************************************************************************/
#ifndef __EEPROM_SPI_IF_H__
#define __EEPROM_SPI_IF_H__

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"


/************************************************************************************************
Macro definitions
*************************************************************************************************/
/* Define using Firmware Integration Technology (FIT) for RX family MCU. */
#define EEPROM_SPI_CFG_USE_FIT

/* Driver version */
#define EEPROM_SPI_VERSION_MAJOR       (3)
#define EEPROM_SPI_VERSION_MINOR       (01)


/*--------------- Define no. of slots ----------------*/
#define EEPROM_SPI_DEV0                (0)                  /* Device 0                             */
#define EEPROM_SPI_DEV1                (1)                  /* Device 1                             */


/*------ Definitions of write-protection status ------*/
/*      2k      4k      8k     16k     32k     64k    128k    256k    512k                          */
/*  +-------+-------+-------+-------+-------+-------+-------+-------+-------+ - - - - - - - - - -   */
/*  | 0000h | 0000h | 0000h | 0000h | 0000h | 0000h | 0000h | 0000h | 0000h |                   |   */
/*  |       |       |       |       |       |       |       |       |       |                   |   */
/*  |       |       |       |       |       |       |       |       |       |                   |   */
/*  |       |       |       |       |       |       |       |       |       |                   |   */
/*  +-------+-------+-------+-------+-------+-------+-------+-------+-------+                   |   */
/*  | 0040h | 0080h | 0100h | 0200h | 0400h | 0800h |1000h  | 2000h | 4000h |                   |   */
/*  |       |       |       |       |       |       |       |       |       |                   |   */
/*  |       |       |       |       |       |       |       |       |       |                   |   */
/*  |       |       |       |       |       |       |       |       |       |               Whole   */
/*  +-------+-------+-------+-------+-------+-------+-------+-------+-------+ - - - - - -   memory  */
/*  | 0080h | 0100h | 0200h | 0400h | 0800h | 1000h | 2000h | 4000h | 8000h |           |       |   */
/*  |       |       |       |       |       |       |       |       |       |           |       |   */
/*  |       |       |       |       |       |       |       |       |       |           |       |   */
/*  |       |       |       |       |       |       |       |       |       |       Upper       |   */
/*  +-------+-------+-------+-------+-------+-------+-------+-------+-------+ - -    1/2        |   */
/*  | 00C0h | 0180h | 0300h | 0600h | 0C00h | 1800h | 3000h | 6000h | C000h |   |       |       |   */
/*  |       |       |       |       |       |       |       |       |       |Upper      |       |   */
/*  |       |       |       |       |       |       |       |       |       | 1/4       |       |   */
/*  | 00FFh | 01FFh | 03FFh | 07FFh | 0FFFh | 1FFFh | 3FFFh | 7FFFh | FFFFh |   |       |       |   */
/*  +-------+-------+-------+-------+-------+-------+-------+-------+-------+ - - - - - - - - - -   */
#define EEPROM_SPI_WP_NONE             (uint8_t)(0x00)     /* None setting                         */
#define EEPROM_SPI_WP_UPPER_QUART      (uint8_t)(0x01)     /* Upper quarter setting                */
#define EEPROM_SPI_WP_UPPER_HALF       (uint8_t)(0x02)     /* Upper half setting                   */
#define EEPROM_SPI_WP_WHOLE_MEM        (uint8_t)(0x03)     /* Whole memory setting                 */


#define EEPROM_SPI_CMD_SIZE            (uint8_t)(1)        /* Command size(Byte)                   */
#define EEPROM_SPI_STSREG_SIZE         (uint16_t)(1)       /* Status register size(Byte)           */


/*------- Definitions of software timer value --------*/
/* Transmit&receive waiting time */
#define EEPROM_SPI_BUSY_WAIT           (uint32_t)(8000)    /* Write busy waiting time   8000* 1us = 8ms */

/* Definition of unit of software wait timer */
#define EEPROM_SPI_UINT_MICROSECS      (uint8_t)(0)        /* Unit : us                            */
#define EEPROM_SPI_UINT_MILLISECS      (uint8_t)(1)        /* Unit : ms                            */

/* Access time */
#define EEPROM_SPI_T_READY_WAIT        EEPROM_SPI_UINT_MILLISECS
                                                           /* Write busy completion polling time   */
#define EEPROM_SPI_T_CS_HOLD           EEPROM_SPI_UINT_MICROSECS
                                                           /* CS stability waiting time            */
#define EEPROM_SPI_T_R_ACCESS          EEPROM_SPI_UINT_MICROSECS
                                                           /* Reading start waiting time           */


/*------- Definitions of status register value -------*/
#define EEPROM_SPI_REG_SRWD            (uint8_t)(0x80)     /* Status Register Write Disable        */
#define EEPROM_SPI_REG_BP1             (uint8_t)(0x08)     /* Block Protect Bit1                   */
#define EEPROM_SPI_REG_BP0             (uint8_t)(0x04)     /* Block Protect Bit0                   */
#define EEPROM_SPI_REG_WEL             (uint8_t)(0x02)     /* Write Enable Latch Bit               */
#define EEPROM_SPI_REG_WIP             (uint8_t)(0x01)     /* Write In Progress Bit                */


/*------- Definitions of transfer mode -------*/
#define EEPROM_SPI_MODE_RX_NONFIT_RSPI         (0x00000001)    /* RX Non-FIT RSPI driver           */
#define EEPROM_SPI_MODE_RX_NONFIT_SCI          (0x00000002)    /* RX Non-FIT RSPI driver           */
#define EEPROM_SPI_MODE_RL78_NONFIT_SAU_CSI    (0x00000004)    /* RL78 Non-FIT SAU CSI driver      */
#define EEPROM_SPI_MODE_78K0R_NONFIT_SAU_CSI   (0x00000008)    /* RL78 Non-FIT SAU CSI driver      */
#define EEPROM_SPI_MODE_RX_FIT_RSPI            (0x00000010)    /* RX FIT RSPI driver               */
#define EEPROM_SPI_MODE_RX_FIT_QSPI            (0x00000020)    /* RX FIT QSPI driver               */
#define EEPROM_SPI_MODE_RX_FIT_SCIFA           (0x00000040)    /* RX FIT SCIFA driver              */
#define EEPROM_SPI_MODE_RX_FIT_DMAC            (0x00000080)    /* RX FIT DMAC driver               */
#define EEPROM_SPI_MODE_RX_FIT_DTC             (0x00000100)    /* RX FIT DTC driver                */


/************************************************************************************************
Typedef definitions
*************************************************************************************************/
/* Enumeration for return values */
typedef enum e_eeprom_status
{
    EEPROM_SPI_SUCCESS_BUSY = 1,    /* Successful operation (EERPOM is busy)    */
    EEPROM_SPI_SUCCESS      = 0,    /* Successful operation                     */
    EEPROM_SPI_ERR_PARAM    = -1,   /* Parameter error                          */
    EEPROM_SPI_ERR_HARD     = -2,   /* Hardware error                           */
    EEPROM_SPI_ERR_WP       = -4,   /* Write-protection error                   */
    EEPROM_SPI_ERR_OTHER    = -7    /* Other error                              */
} eeprom_status_t;


/* EEPROM information */
typedef struct
{
    uint32_t        addr;           /* Address to issue a command               */
    uint32_t        cnt;            /* Number of bytes to be read/written       */
    uint32_t        data_cnt;       /* Temporary counter or Number of bytes to be written in a page */
    uint8_t       * p_data;         /* Data storage buffer pointer              */
} eeprom_info_t;                    /* 16 bytes                                 */


/* EEPROM size information */
typedef struct
{
    uint32_t        mem_size;       /* Max memory size                          */
    uint32_t        wpag_size;      /* Write page size                          */
} eeprom_mem_info_t;                /* 8 bytes                                  */


/************************************************************************************************
Exported global variables
*************************************************************************************************/


/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/
eeprom_status_t R_EEPROM_SPI_Open(uint8_t devno);
eeprom_status_t R_EEPROM_SPI_Read_Status(uint8_t devno, uint8_t * p_status);
eeprom_status_t R_EEPROM_SPI_Set_Write_Protect(uint8_t devno, uint8_t wpsts);
eeprom_status_t R_EEPROM_SPI_Write_Di(uint8_t devno);
eeprom_status_t R_EEPROM_SPI_Read_Data(uint8_t devno, eeprom_info_t * p_eeprom_info);
eeprom_status_t R_EEPROM_SPI_Write_Data_Page(uint8_t devno, eeprom_info_t * p_eeprom_info);
eeprom_status_t R_EEPROM_SPI_Polling(uint8_t devno);
eeprom_status_t R_EEPROM_SPI_GetMemoryInfo(uint8_t devno, eeprom_mem_info_t * p_eeprom_mem_info);
eeprom_status_t R_EEPROM_SPI_Close(uint8_t devno);
uint32_t        R_EEPROM_SPI_GetVersion(void);
eeprom_status_t R_EEPROM_SPI_Set_LogHdlAddress(uint32_t user_long_que);
uint32_t        R_EEPROM_SPI_Log(uint32_t flg, uint32_t fid, uint32_t line);
void            R_EEPROM_SPI_1ms_Interval(void);


#endif /* __EEPROM_SPI_IF_H__ */

/* End of File */
