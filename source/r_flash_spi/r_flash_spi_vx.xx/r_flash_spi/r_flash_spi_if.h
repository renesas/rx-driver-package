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
* Copyright (C) 2011(2012-2019) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* File Name    : r_flash_spi_if.h
* Version      : 3.01
* Description  : FLASH SPI driver interface header file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 23.07.2014 2.21     Created
*              : 29.05.2015 2.32     Revised functions of same as Ver.2.32 of EEPROM SPI FIT module.
*              : 24.12.2015 2.33     Added RX130, RX23T and RX24T.
*              : 31.07.2017 2.34     Modified Application note.
*              : 21.12.2018 3.00     Change flash drive interface to Memory Access Driver Interface
*              : 04.04.2019 3.01     Added support for GNUC and ICCRX.
*                                    Fixed coding style.
*************************************************************************************************/
#ifndef __FLASH_SPI_IF_H__
#define __FLASH_SPI_IF_H__

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"


/************************************************************************************************
Macro definitions
*************************************************************************************************/
/* Define using Firmware Integration Technology (FIT) for RX family MCU. */
#define FLASH_SPI_CFG_USE_FIT

/* Driver version */
#define FLASH_SPI_VERSION_MAJOR       (3)
#define FLASH_SPI_VERSION_MINOR       (01)


/*--------------- Define no. of slots ----------------*/
#define FLASH_SPI_DEV0                (0)                   /* Device 0                             */
#define FLASH_SPI_DEV1                (1)                   /* Device 1                             */



/************************************************************************************************
Typedef definitions
*************************************************************************************************/
/* Enumeration for return values */
typedef enum e_flash_status
{
    FLASH_SPI_SUCCESS_BUSY = 1,    /* Successful operation (EERPOM is busy)    */
    FLASH_SPI_SUCCESS      = 0,    /* Successful operation                     */
    FLASH_SPI_ERR_PARAM    = -1,   /* Parameter error                          */
    FLASH_SPI_ERR_HARD     = -2,   /* Hardware error                           */
    FLASH_SPI_ERR_WP       = -4,   /* Write-protection error                   */
    FLASH_SPI_ERR_TIMEOUT  = -6,   /* Time out error                           */
    FLASH_SPI_ERR_OTHER    = -7    /* Other error                              */
} flash_spi_status_t;

typedef enum e_flash_spi_erase_mode
{
    FLASH_SPI_MODE_C_ERASE = 1,
    FLASH_SPI_MODE_B_ERASE,
    FLASH_SPI_MODE_D_ERASE,
    FLASH_SPI_MODE_B32K_ERASE,
    FLASH_SPI_MODE_B64K_ERASE,
    FLASH_SPI_MODE_S_ERASE,
    FLASH_SPI_MODE_SS_ERASE,
    FLASH_SPI_MODE_P_ERASE
} flash_spi_erase_mode_t;

typedef enum e_flash_spi_poll_mode
{
    FLASH_SPI_MODE_REG_WRITE_POLL = 1,
    FLASH_SPI_MODE_PROG_POLL,
    FLASH_SPI_MODE_ERASE_POLL
} flash_spi_poll_mode_t;

typedef enum e_flash_spi_opmode
{
    FLASH_SPI_SINGLE = 0,           /* Single-SPI mode                          */
    FLASH_SPI_DUAL,                 /* Dual-SPI mode                            */
    FLASH_SPI_QUAD                  /* Quad-SPI mode                            */
} flash_spi_opmode_t;

/* Flash memory information */
typedef struct
{
    uint32_t        addr;           /* Address to issue a command               */
    uint32_t        cnt;            /* Number of bytes to be read/written       */
    uint32_t        data_cnt;       /* Temporary counter or Number of bytes to be written in a page */
    uint8_t       * p_data;         /* Data storage buffer pointer              */
    flash_spi_opmode_t op_mode;     /* SPI operating mode                       */
} flash_spi_info_t;                 /* 20 bytes                                 */

/* Flash memory size information */
typedef struct
{
    uint32_t        mem_size;       /* Max memory size                          */
    uint32_t        wpag_size;      /* Write page size                          */
} flash_spi_mem_info_t;             /* 8 bytes                                  */

/* Flash memory erase information */
typedef struct
{
    uint32_t        addr;           /* Address to issue a command               */
    flash_spi_erase_mode_t  mode;   /* Mode of erase                            */
} flash_spi_erase_info_t;           /* 8 bytes                                  */

/* Flash memory register information */
typedef struct
{
    uint8_t         status;         /* Status register                           */
    uint8_t         config1;        /* Configuration or Configuration-1 register */
    uint8_t         config2;        /* Configuration-2 register                  */
    uint8_t         rsv[1];
} flash_spi_reg_info_t;             /* 4 bytes                                   */


/************************************************************************************************
Exported global variables
*************************************************************************************************/


/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/

/* ---- API for all flash memory  ---- */
flash_spi_status_t R_FLASH_SPI_Open(uint8_t devno);
flash_spi_status_t R_FLASH_SPI_Close(uint8_t devno);
flash_spi_status_t R_FLASH_SPI_Read_Status(uint8_t devno, uint8_t * p_status);
flash_spi_status_t R_FLASH_SPI_Set_Write_Protect(uint8_t devno, uint8_t wpsts);
flash_spi_status_t R_FLASH_SPI_Write_Di(uint8_t devno);
flash_spi_status_t R_FLASH_SPI_Read_Data(uint8_t devno, flash_spi_info_t * p_flash_spi_info);
flash_spi_status_t R_FLASH_SPI_Write_Data_Page(uint8_t devno, flash_spi_info_t * p_flash_spi_info);
flash_spi_status_t R_FLASH_SPI_Erase(uint8_t devno, flash_spi_erase_info_t * p_flash_spi_erase_info);
flash_spi_status_t R_FLASH_SPI_Polling(uint8_t devno, flash_spi_poll_mode_t mode);
flash_spi_status_t R_FLASH_SPI_Read_ID(uint8_t devno, uint8_t * p_data);
flash_spi_status_t R_FLASH_SPI_GetMemoryInfo(uint8_t devno, flash_spi_mem_info_t * p_flash_spi_mem_info);
uint32_t           R_FLASH_SPI_GetVersion(void);
flash_spi_status_t R_FLASH_SPI_Set_LogHdlAddress(uint32_t user_long_que);
uint32_t           R_FLASH_SPI_Log(uint32_t flg, uint32_t fid, uint32_t line);
void               R_FLASH_SPI_1ms_Interval(void);

/* ---- API for specific flash memory ---- */
flash_spi_status_t R_FLASH_SPI_Read_Configuration(uint8_t devno, uint8_t * p_config);
flash_spi_status_t R_FLASH_SPI_Write_Configuration(uint8_t devno, flash_spi_reg_info_t * p_reg);
flash_spi_status_t R_FLASH_SPI_Set_4byte_Address_Mode(uint8_t devno);
flash_spi_status_t R_FLASH_SPI_Read_Security(uint8_t devno, uint8_t * p_scur);
flash_spi_status_t R_FLASH_SPI_Quad_Enable(uint8_t devno);
flash_spi_status_t R_FLASH_SPI_Quad_Disable(uint8_t devno);
flash_spi_status_t R_FLASH_SPI_Read_Flag_Status(uint8_t devno, uint8_t * p_status);
flash_spi_status_t R_FLASH_SPI_Clear_Status(uint8_t devno);
flash_spi_status_t R_FLASH_SPI_Read_Status2(uint8_t devno, uint8_t * p_status);
flash_spi_status_t R_FLASH_SPI_DeepPDown(uint8_t devno);
flash_spi_status_t R_FLASH_SPI_ReleaseDeepPDown(uint8_t devno, uint8_t * p_data);


#endif /* __FLASH_SPI_IF_H__ */

/* End of File */
