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
* File Name    : r_memdrv_rx_if.h
* Version      : 1.01
* Description  : Memory driver header file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 15.12.2018 1.00     Initial Release
*              : 04.04.2019 1.01     Added support for GNUC and ICCRX.
*                                    Fixed coding style.
*              : 22.11.2019 1.02     Modify the parameter type of structure Memory Driver I/F information.
*************************************************************************************************/

/***********************************************************************************************************************
Includes <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

#ifndef MEMDRV_IF_H
#define MEMDRV_IF_H
/* Version Number of API. */
#define MEMDRV_VERSION_MAJOR                 (1)
#define MEMDRV_VERSION_MINOR                 (02)

/* Define device no. */
#define MEMDRV_DEV0                          (0)
#define MEMDRV_DEV1                          (1)

/* Definitions of device driver channel no. */
#define MEMDRV_DRVR_CH0                      (0x00000000ul)
#define MEMDRV_DRVR_CH1                      (0x00000001ul)
#define MEMDRV_DRVR_CH2                      (0x00000002ul)
#define MEMDRV_DRVR_CH3                      (0x00000003ul)
#define MEMDRV_DRVR_CH4                      (0x00000004ul)
#define MEMDRV_DRVR_CH5                      (0x00000005ul)
#define MEMDRV_DRVR_CH6                      (0x00000006ul)
#define MEMDRV_DRVR_CH7                      (0x00000007ul)
#define MEMDRV_DRVR_CH8                      (0x00000008ul)
#define MEMDRV_DRVR_CH9                      (0x00000009ul)
#define MEMDRV_DRVR_CH10                     (0x0000000aul)
#define MEMDRV_DRVR_CH11                     (0x0000000bul)
#define MEMDRV_DRVR_CH12                     (0x0000000cul)
#define MEMDRV_DRVR_CH13                     (0x0000000dul)
#define MEMDRV_DRVR_CH14                     (0x0000000eul)
#define MEMDRV_DRVR_CH15                     (0x0000000ful)

/* Definitions of device driver */
#define MEMDRV_DRVR_RX_FIT_RSPI              (0x00000100ul)
#define MEMDRV_DRVR_RX_FIT_QSPI_SMSTR        (0x00000200ul)
#define MEMDRV_DRVR_RX_FIT_SCI_SPI           (0x00000400ul)

/* Definitions of data transfer method */
#define MEMDRV_TRNS_CPU                      (0x00001000ul)
#define MEMDRV_TRNS_DMAC                     (0x00002000ul)
#define MEMDRV_TRNS_DTC                      (0x00004000ul)

/* Definitions of input/output mode */
#define MEMDRV_MODE_SINGLE                   ((uint8_t)(0x01))
#define MEMDRV_MODE_DUAL                     ((uint8_t)(0x02))
#define MEMDRV_MODE_QUAD                     ((uint8_t)(0x03))

#define MEMDRV_TRNS_CMD                      (0x0703)
#define MEMDRV_TRNS_DATA_CMD                 (0x0203)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Enumeration for return values */
typedef enum e_memdrv_err
{
    MEMDRV_BUSY         = 1,  /* Successful operation (device is busy) */
    MEMDRV_SUCCESS      = 0,  /* Successful operation */
    MEMDRV_ERR_PARAM    = -1, /* Parameter error */
    MEMDRV_ERR_HARD     = -2, /* Hardware error */
    MEMDRV_ERR_WP       = -4, /* Write-protection error */
    MEMDRV_ERR_TIMEOUT  = -6, /* Time out error */
    MEMDRV_ERR_OTHER    = -7  /* Other error */
} memdrv_err_t;

/* Memory Driver I/F information */
typedef struct
{
    uint32_t    cnt;
    uint8_t   * p_data;
    uint8_t     io_mode;
    uint8_t     rsv[3];
} st_memdrv_info_t;
/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Public Functions
***********************************************************************************************************************/
memdrv_err_t    R_MEMDRV_Open(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t    R_MEMDRV_Close(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t    R_MEMDRV_Disable(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t    R_MEMDRV_DisableTxData(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t    R_MEMDRV_DisableRxData(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t    R_MEMDRV_Enable(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t    R_MEMDRV_EnableTxData(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t    R_MEMDRV_EnableRxData(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t    R_MEMDRV_Tx(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t    R_MEMDRV_TxData(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t    R_MEMDRV_Rx(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t    R_MEMDRV_RxData(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
void            R_MEMDRV_ClearDMACFlagTx(uint8_t devno);
void            R_MEMDRV_ClearDMACFlagRx(uint8_t devno);
void            R_MEMDRV_1msInterval(void);
memdrv_err_t    R_MEMDRV_SetLogHdlAddress(uint32_t user_long_que);
uint32_t        R_MEMDRV_Log(uint32_t flg, uint32_t fid, uint32_t line);
uint32_t        R_MEMDRV_GetVersion(void);

#endif /* MEMDRV_IF_H */
