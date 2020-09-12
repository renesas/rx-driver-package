/*******************************************************************************
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
* Copyright (C) 2014(2015-2019) Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* System Name  : QSPI single master driver
* File Name    : r_qspi_smstr_private.h
* Version      : 1.14
* Device       : RX
* Abstract     : Private header file for QSPI single master driver
* Tool-Chain   : Renesas RXC Toolchain v3.01.00
* OS           : not use
* H/W Platform : not use
* Description  : Functions, typedefs and macros for QSPI single master driver
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 24.06.2014 1.05     First Release
*              : 29.08.2014 1.06     Changed header files include pass.
*              :                     Added demo source for DTC in this driver.
*              : 28.11.2014 1.07     Modified names of rspi_smstr_logid_t member.
*              : 30.01.2015 1.08     Added RX71M.
*              : 30.09.2016 1.09     Added RX65N.
*              : 31.07.2017 2.08     Supported RX65N-2MB.
*                                    Fixed to correspond to Renesas coding rule.
*              : 20.05.2019 1.12     Added support for GNUC and ICCRX.
*                                    Fixed coding style. 
*              : 30.07.2019 1.13     Added RX72M.
*              : 22.11.2019 1.14     Added RX72N and RX66N.
*******************************************************************************/
#ifndef QSPI_SMSTR_PRIVATE_H_
#define QSPI_SMSTR_PRIVATE_H_

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#ifdef QSPI_SMSTR_CFG_USE_FIT
/* Includes board and MCU related header files. */
#include "platform.h"
#endif  /* QSPI_SMSTR_CFG_USE_FIT */

/* QSPI driver targets header file */
#if   defined(BSP_MCU_RX64M)
    #include "./src/targets/rx64m/r_qspi_smstr_target.h"
#elif defined(BSP_MCU_RX65N)
    #include "./src/targets/rx65n/r_qspi_smstr_target.h"
#elif defined(BSP_MCU_RX66N)
    #include "./src/targets/rx66n/r_qspi_smstr_target.h"
#elif defined(BSP_MCU_RX71M)
    #include "./src/targets/rx71m/r_qspi_smstr_target.h"
#elif defined(BSP_MCU_RX72M)
    #include "./src/targets/rx72m/r_qspi_smstr_target.h"
#elif defined(BSP_MCU_RX72N)
    #include "./src/targets/rx72n/r_qspi_smstr_target.h"
#else
    #error "This MCU is not supported by the current r_qspi_smstr_rx module."
#endif


/*******************************************************************************
Macro definitions
*******************************************************************************/
/* ---- Definitions of port control. ---- */
#define QSPI_SMSTR_HI             ((uint8_t)(0x01))         /* Port is high.                                    */
#define QSPI_SMSTR_LOW            ((uint8_t)(0x00))         /* Port is low.                                     */
#define QSPI_SMSTR_OUT            ((uint8_t)(0x01))         /* Port is set as output pin.                       */
#define QSPI_SMSTR_IN             ((uint8_t)(0x00))         /* Port is set as input pin.                        */

/* ---- Definitions of QSPI software wait. ---- */
#define QSPI_SMSTR_T_QSPI_WAIT    ((uint32_t)(1))

/* ---- Definitions of value of flag. ---- */
#define QSPI_SMSTR_FLAG_SET       ((uint8_t)(0x01))         /* Flag is 1.                                       */
#define QSPI_SMSTR_FLAG_CLR       ((uint8_t)(0x00))         /* Flag is 0.                                       */

/* ---- Definitions of mask data to check flag. ---- */
#define QSPI_SMSTR_MASK_SPSSLF    ((uint8_t)(0x10))         /* Mask data to check SPSSLF bit of SPSR.           */
#define QSPI_SMSTR_MASK_SPTEF     ((uint8_t)(0x20))         /* Mask data to check SPTEF bit of SPSR.            */
#define QSPI_SMSTR_MASK_TREND     ((uint8_t)(0x40))         /* Mask data to check TREND bit of SPSR.            */
#define QSPI_SMSTR_MASK_SPRFF     ((uint8_t)(0x80))         /* Mask data to check SPRFF bit of SPSR.            */

/* --- Definitions of endian ---- */
#define QSPI_SMSTR_LITTLE_ENDIAN  (defined(__LIT) || \
                                 defined(__LITTLE) || \
                                 defined(__RX_LITTLE_ENDIAN__))

/* ---- Definitions of value of software timer. ---- */
#define QSPI_SMSTR_TX_WAIT        ((uint32_t)(50000))       /* Waiting time for QSPI transmission completion.   */
#define QSPI_SMSTR_RX_WAIT        ((uint32_t)(50000))       /* Waiting time for QSPI reception completion.      */
#define QSPI_SMSTR_QSSL_WAIT      ((uint32_t)(50000))       /* Waiting time for QSPI QSSL negation completion.  */

/* ---- Definitions of data for QSPI transfer. ---- */
#define QSPI_SMSTR_TRAN_SIZE      ((uint32_t)(4))           /* QSPI transfer data size.                         */

/* ---- Definitions of data for QSPI transfer. ---- */
#define QSPI_SMSTR_DUMMY_DATA_LONG    ((uint32_t)(0xFFFFFFFF))  /* Dummy data in order to receive data.         */
#define QSPI_SMSTR_DUMMY_DATA_BYTE    ((uint8_t)(0xFF))         /* Dummy data in order to receive data.         */

/* ---- Definitions of LONGQ driver ---- */
#ifdef QSPI_SMSTR_CFG_LONGQ_ENABLE                        /* Use LONGQ driver.                                */
#define QSPI_SMSTR_DRIVER_ID      ((uint32_t)(6))           /* QSPI debug log id                                */
#define QSPI_SMSTR_DEBUG_ERR_ID   ((uint32_t)(1))           /* QSPI debug log error id                          */
typedef enum e_qspi_smstr_log
{
    QSPI_SMSTR                  =  1,
    QSPI_SMSTR_CRC              =  2,
    QSPI_SMSTR_CRC_TABLE        =  3,
    QSPI_SMSTR_TARGET           =  4,
    QSPI_SMSTR_TARGET_DEV_PORT  =  5,
} qspi_smstr_logid_t;
#endif /* QSPI_USER_LONGQ_ENABLE */


/*******************************************************************************
Typedef definitions
*******************************************************************************/
/* Define for exchanging data by endian structure type. */
typedef union {
    uint32_t ul;
    uint8_t  uc[4];
} r_qspi_smstr_exchg_long_t;


/*******************************************************************************
Private variables and functions
*******************************************************************************/


/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/
qspi_smstr_status_t r_qspi_smstr_single_writeread_software(uint8_t channel, qspi_smstr_info_t * p_qspi_smstr_info);
qspi_smstr_status_t r_qspi_smstr_dualquad_write_software(uint8_t channel, qspi_smstr_info_t * p_qspi_smstr_info);
qspi_smstr_status_t r_qspi_smstr_dualquad_read_software(uint8_t channel, qspi_smstr_info_t * p_qspi_smstr_info);
qspi_smstr_status_t r_qspi_smstr_single_writeread_dmacdtc(uint8_t channel, qspi_smstr_info_t * p_qspi_smstr_info);
qspi_smstr_status_t r_qspi_smstr_dualquad_write_dmacdtc(uint8_t channel, qspi_smstr_info_t * p_qspi_smstr_info);
qspi_smstr_status_t r_qspi_smstr_dualquad_read_dmacdtc(uint8_t channel, qspi_smstr_info_t * p_qspi_smstr_info);
void                r_qspi_smstr_tx_exchg(uint8_t channel, uint8_t * p_data, uint32_t * p_dataadr);
void                r_qspi_smstr_rx_exchg(uint8_t channel, uint8_t * p_data, uint32_t * p_dataadr);
qspi_smstr_status_t r_qspi_smstr_exchg_dmacdtc(uint8_t channel, uint8_t * p_data, uint32_t size);
qspi_smstr_status_t r_qspi_smstr_wait(volatile uint8_t R_BSP_EVENACCESS_SFR * flag, uint32_t wait, uint8_t mask);
void                r_qspi_smstr_wait_lp(volatile uint32_t time);
void                r_qspi_smstr_tx_software_trans(uint8_t channel, uint32_t * p_dataadr);
void                r_qspi_smstr_rx_software_trans(uint8_t channel, uint32_t * p_dataadr);
void                r_qspi_smstr_tx_software_trans_dummy(uint8_t channel);
void                r_qspi_smstr_rx_software_trans_dummy(uint8_t channel);

#ifdef QSPI_SMSTR_CFG_LONGQ_ENABLE        /* Uses LONG driver.            */
/* It is a function for debugging. When invalidate definition "QSPI_SMSTR_CFG_LONGQ_ENABLE",
   the code is not generated. */
uint32_t r_qspi_smstr_log(uint32_t flg, uint32_t fid, uint32_t line);
#define R_QSPI_SMSTR_LOG_FUNC(x, y, z)   (r_qspi_smstr_log((x), (y), (z)))
#else
#define R_QSPI_SMSTR_LOG_FUNC(x, y, z)
#endif /* QSPI_SMSTR_CFG_LONGQ_ENABLE */


#endif /* _QSPI_SMSTR_PRIVATE_H_ */

/* End of File */

