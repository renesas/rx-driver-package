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
* File Name    : r_memdrv_private.h
* Version      : 1.02
* Description  : MEMDRV driver private header file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 15.12.2018 1.00     Initial Release
*              : 04.04.2019 1.01     Added support for GNUC and ICCRX.
*                                    Fixed coding style.
*              : 22.11.2019 1.02     Modified check driver interface.
*************************************************************************************************/

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_memdrv_rx_config.h"
#if ((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_RSPI)) || \
    ((MEMDRV_CFG_DEV1_INCLUDED == 1) && (MEMDRV_CFG_DEV1_MODE_DRVR == MEMDRV_DRVR_RX_FIT_RSPI))
#include "r_rspi_rx_if.h"
#endif
#if ((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPI_SMSTR)) || \
    ((MEMDRV_CFG_DEV1_INCLUDED == 1) && (MEMDRV_CFG_DEV1_MODE_DRVR == MEMDRV_DRVR_RX_FIT_QSPI_SMSTR))
#include "r_qspi_smstr_rx_if.h"
#endif
#if ((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_DRVR_RX_FIT_SCI_SPI)) || \
    ((MEMDRV_CFG_DEV1_INCLUDED == 1) && (MEMDRV_CFG_DEV1_MODE_DRVR == MEMDRV_DRVR_RX_FIT_SCI_SPI))
#include "r_sci_rx_if.h"
#endif
#if ((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_TRNS == MEMDRV_TRNS_DMAC)) || \
    ((MEMDRV_CFG_DEV1_INCLUDED == 1) && (MEMDRV_CFG_DEV1_MODE_TRNS == MEMDRV_TRNS_DMAC))
#include "r_dmaca_rx_if.h"
#endif
#if ((MEMDRV_CFG_DEV0_INCLUDED == 1) && (MEMDRV_CFG_DEV0_MODE_TRNS == MEMDRV_TRNS_DTC)) || \
    ((MEMDRV_CFG_DEV1_INCLUDED == 1) && (MEMDRV_CFG_DEV1_MODE_TRNS == MEMDRV_TRNS_DTC))
#include "r_dtc_rx_if.h"
#endif
#if (MEMDRV_CFG_LONGQ_ENABLE == 1)
#include "r_longq_if.h"
#endif

/* Guards against multiple inclusion */
#ifndef MEMDRV_PRIVATE_H
#define MEMDRV_PRIVATE_H

/************************************************************************************************
Macro definitions
*************************************************************************************************/
/* Definition of unit of software wait timer */
#define MEMDRV_UINT_MICROSECS             ((uint8_t)(0))            /* Unit : us                            */
#define MEMDRV_UINT_MILLISECS             ((uint8_t)(1))            /* Unit : ms                            */

/* Access time */
#define MEMDRV_T_READY_WAIT               (MEMDRV_UINT_MILLISECS) /* Write busy completion polling time   */
#define MEMDRV_T_CS_HOLD                  (MEMDRV_UINT_MICROSECS) /* CS stability waiting time            */
#define MEMDRV_T_R_ACCESS                 (MEMDRV_UINT_MICROSECS) /* Reading start waiting time           */

/* Address boundary */
#define MEMDRV_ADDR_BOUNDARY              ((uint32_t)(0x00000003))

/* Multiple of data counter */
#define MEMDRV_CHK_MULT_OF_4              ((uint32_t)(0x00000003))
#define MEMDRV_CHK_MULT_OF_16             ((uint32_t)(0x0000000f))

/* Definitions of endian */
#define MEMDRV_LITTLE_ENDIAN              (defined(__LIT) || \
                                           defined(__LITTLE) || \
                                           defined(__RX_LITTLE_ENDIAN__))

#define MEMDRV_DEV_NUM                    (2)

#define MEMDRV_DRVR_MASK_CH               (0x0000000ful)
#define MEMDRV_DRVR_MASK_TYPE             (0x00000f00ul)

#define MEMDRV_INDX_RSPI                  (0)
#define MEMDRV_INDX_QSPI_SMSTR            (1)
#define MEMDRV_INDX_SCI_SPI               (2)
#define MEMDRV_INDX_DRVR_NUM              (3)

#define MEMDRV_INDX_OPEN                  (0)
#define MEMDRV_INDX_CLOSE                 (1)
#define MEMDRV_INDX_DISABLE               (2)
#define MEMDRV_INDX_DISABLE_TX_DATA       (3)
#define MEMDRV_INDX_DISABLE_RX_DATA       (4)
#define MEMDRV_INDX_ENABLE                (5)
#define MEMDRV_INDX_ENABLE_TX_DATA        (6)
#define MEMDRV_INDX_ENABLE_RX_DATA        (7)
#define MEMDRV_INDX_TX                    (8)
#define MEMDRV_INDX_TX_DATA               (9)
#define MEMDRV_INDX_RX                    (10)
#define MEMDRV_INDX_RX_DATA               (11)
#define MEMDRV_INDX_FUNC_NUM              (12)

#define MEMDRV_DTC_MODE_TX                (0x00000000ul)
#define MEMDRV_DTC_MODE_RX                (0x00000001ul)

#define MEMDRV_INDEX_DTC_TX               (0)
#define MEMDRV_INDEX_DTC_RX               (1)
#define MEMDRV_INDEX_DTC_NUM              (2)

/************************************************************************************************
Global Typedef definitions
*************************************************************************************************/

/* Memory Driver I/F function */
typedef struct /*  @suppress("Source line ordering") */
{
    memdrv_err_t (* p_func)(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
} st_memdrv_func_t;

/* Enumeration for error log */
#if (MEMDRV_CFG_LONGQ_ENABLE == 1)                    /* Use LONGQ driver.                    */
#define MEMDRV_DRIVER_ID      ((uint32_t)(8))         /* MEMDRV debug log id                    */
#define MEMDRV_DEBUG_ERR_ID   ((uint32_t)(1))         /* MEMDRV debug log error id              */
typedef enum e_memdrv_log
{
    MEMDRV_ERR_USER       =  11,
    MEMDRV_ERR_SUB        =  20,
    MEMDRV_ERR_DRVIF_COM  =  21,
} st_memdrv_logid_t;
#endif /* (MEMDRV_CFG_LONGQ_ENABLE == 1) */

/* DMAC transfer information */
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
typedef struct
{
    uint32_t    drv;
    uint32_t    src_adr;
    uint32_t    dst_adr;
    uint32_t    block_size;
    uint32_t    cnt;
    dmaca_transfer_data_cfg_t * p_dmac_cfg;
    dmaca_activation_source_t act_source;
    dmaca_stat_t  * p_stat_dmac;
    uint8_t     channel;
    uint8_t     rsv[3];
} st_memdrv_dmac_info_t;
#endif /* (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC) */

/* DTC transfer information */
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
typedef struct
{
    uint32_t    drv;
    uint32_t    src_adr;
    uint32_t    dst_adr;
    uint32_t    block_size;
    uint32_t    cnt;
    dtc_transfer_data_cfg_t * p_dtc_cfg;
    dtc_activation_source_t act_source;
    dtc_transfer_data_t * p_dtc_transfer;
} st_memdrv_dtc_info_t;
#endif /* (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC) */

/************************************************************************************************
Exported global variables
*************************************************************************************************/
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
extern dtc_transfer_data_t g_memdrv_dtc_info[MEMDRV_DEV_NUM][MEMDRV_INDEX_DTC_NUM];
#endif

/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/
#if (MEMDRV_CFG_LONGQ_ENABLE == 1)
/* It is a function for debugging. When invalidate definition "MEMDRV_CFG_LONGQ_ENABLE",
   the code is not generated. */
uint32_t r_memdrv_log(uint32_t flg, uint32_t fid, uint32_t line);
#define R_MEMDRV_Log(x, y, z)  (r_memdrv_log((x), (y), (z)))
#else
#define R_MEMDRV_Log(x, y, z)
#endif /* (MEMDRV_CFG_LONGQ_ENABLE == 1) */

extern const uint32_t g_memdrv_dummy_tx;
extern uint32_t g_memdrv_dummy_rx;

/* r_memdrv_rx.c */
uint8_t             r_memdrv_get_drv_ch(uint8_t devno);

/* r_memdrv_rspi.c */
memdrv_err_t r_memdrv_rspi_open(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_rspi_close(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_rspi_disable(uint8_t devno,
                                   st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_rspi_disable_tx_data(uint8_t devno,
                                           st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_rspi_disable_rx_data(uint8_t devno,
                                           st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_rspi_enable(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_rspi_enable_tx_data(uint8_t devno,
                                          st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_rspi_enable_rx_data(uint8_t devno,
                                          st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_rspi_tx(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_rspi_tx_data(uint8_t devno,
                                   st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_rspi_rx(uint8_t devno,
                              st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_rspi_rx_data(uint8_t devno,
                                   st_memdrv_info_t * p_memdrv_info);
void r_memdrv_rspi_1ms_interval(void);

/* r_memdrv_qspi_smstr.c */
memdrv_err_t r_memdrv_qspi_open(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_qspi_close(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_qspi_disable(uint8_t devno,
                                         st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_qspi_disable_tx_data(uint8_t devno,
                                                 st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_qspi_disable_rx_data(uint8_t devno,
                                                 st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_qspi_enable(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_qspi_enable_tx_data(uint8_t devno,
                                                st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_qspi_enable_rx_data(uint8_t devno,
                                                st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_qspi_tx(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_qspi_tx_data(uint8_t devno,
                                         st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_qspi_rx(uint8_t devno,
                                    st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_qspi_rx_data(uint8_t devno,
                                         st_memdrv_info_t * p_memdrv_info);

/* r_memdrv_rspi.c */
memdrv_err_t r_memdrv_sci_open(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_sci_close(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_sci_disable(uint8_t devno,
                                  st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_sci_disable_tx_data(uint8_t devno,
                                          st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_sci_disable_rx_data(uint8_t devno,
                                          st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_sci_enable(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_sci_enable_tx_data(uint8_t devno,
                                         st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_sci_enable_rx_data(uint8_t devno,
                                         st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_sci_tx(uint8_t devno, st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_sci_tx_data(uint8_t devno,
                                  st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_sci_rx(uint8_t devno,
                             st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_sci_rx_data(uint8_t devno,
                                  st_memdrv_info_t * p_memdrv_info);
memdrv_err_t r_memdrv_sci_1ms_interval(void);

/* r_memdrv_dmac.c */
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC)
memdrv_err_t r_memdrv_dmac_open(void);
memdrv_err_t r_memdrv_dmac_close(void);
memdrv_err_t r_memdrv_dmac_enable(st_memdrv_dmac_info_t * p_memdrv_dmac_info);
memdrv_err_t r_memdrv_dmac_disable(st_memdrv_dmac_info_t * p_memdrv_dmac_info);
void                r_memdrv_dmac_set_tx(st_memdrv_dmac_info_t * p_memdrv_dmac_info);
void                r_memdrv_dmac_set_rx(st_memdrv_dmac_info_t * p_memdrv_dmac_info);
#endif /* (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DMAC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DMAC) */

/* r_memdrv_dtc.c */
#if (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC)
memdrv_err_t r_memdrv_dtc_open(void);
memdrv_err_t r_memdrv_dtc_close(void);
memdrv_err_t r_memdrv_dtc_enable(st_memdrv_dtc_info_t * p_memdrv_dtc_info);
memdrv_err_t r_memdrv_dtc_disable(st_memdrv_dtc_info_t * p_memdrv_dtc_info);
void                r_memdrv_dtc_set_tx(st_memdrv_dtc_info_t * p_memdrv_dtc_info);
void                r_memdrv_dtc_set_rx(st_memdrv_dtc_info_t * p_memdrv_dtc_info);
#endif /* (MEMDRV_CFG_DEV0_MODE_TRNS & MEMDRV_TRNS_DTC)  | (MEMDRV_CFG_DEV1_MODE_TRNS & MEMDRV_TRNS_DTC) */

#endif /* MEMDRV_PRIVATE_H */

/* End of File */
