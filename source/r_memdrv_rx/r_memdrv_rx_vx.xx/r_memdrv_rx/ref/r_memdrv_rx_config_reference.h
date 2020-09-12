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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* File Name    : r_memdrv_rx_config_reference.h
* Version      : 1.00
* Description  : Memory driver configuration header file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 15.12.2018 1.00     Initial Release
*************************************************************************************************/
#ifndef MEMDRV_CONFIG_H
#define MEMDRV_CONFIG_H

/************************************************************************************************
SPECIFY DEVICES TO INCLUDE SOFTWARE SUPPORT
*************************************************************************************************/
/* If these are defined, then the code for the specified device is valid.
   If the #define which device is not supported on the code is uncommented,
   then the compile error occurs. */
/* 0 : Not use Device No. */
/* 1 : Use Device No. */
#define MEMDRV_CFG_DEV0_INCLUDED  (1)             /* Device 0 Included                        */
#define MEMDRV_CFG_DEV1_INCLUDED  (0)             /* Device 1 Included                        */

/************************************************************************************************
DATA TRANSFER MODE
*************************************************************************************************/
/*  Please set one macro definition. */
/*      MEMDRV_TRNS_CPU   */
/*      MEMDRV_TRNS_DMAC  : RX DMACA FIT module   */
/*      MEMDRV_TRNS_DTC   : RX DTC FIT module     */
#define MEMDRV_CFG_DEV0_MODE_TRNS         (MEMDRV_TRNS_CPU)
#define MEMDRV_CFG_DEV1_MODE_TRNS         (MEMDRV_TRNS_CPU)

/************************************************************************************************
DEVICE DRIVER
*************************************************************************************************/
/*  Please set one macro definition. */
/*      MEMDRV_DRVR_RX_FIT_RSPI           : RX RSPI FIT module  */
/*      MEMDRV_DRVR_RX_FIT_QSPI_SMSTR     : RX QSPI Single Master FIT module  */
/*      MEMDRV_DRVR_RX_FIT_SCI_SPI        : RX SCI as SPI FIT module  */
#define MEMDRV_CFG_DEV0_MODE_DRVR         (MEMDRV_DRVR_RX_FIT_RSPI)
#define MEMDRV_CFG_DEV1_MODE_DRVR         (MEMDRV_DRVR_RX_FIT_RSPI)

/************************************************************************************************
DEVICE DRIVER CHANEL NUMBER
*************************************************************************************************/
/*  Please set one macro definition. */
/*      MEMDRV_DRVR_CH0 - MEMDRV_DRVR_CH15  */
#define MEMDRV_CFG_DEV0_MODE_DRVR_CH      (MEMDRV_DRVR_CH0)
#define MEMDRV_CFG_DEV1_MODE_DRVR_CH      (MEMDRV_DRVR_CH0)

/************************************************************************************************
DEVICE TYPE
*************************************************************************************************/
/*  Please set one macro definition. */
/* 0 : NOR FLASH or EEPROM. */
/* 1 : NAND FLASH. */
#define MEMDRV_CFG_DEV0_TYPE              (0)
#define MEMDRV_CFG_DEV1_TYPE              (0)
/************************************************************************************************
TRANSFER RATE
*************************************************************************************************/
/* Define the transfer rate for using MCU driver interface.
   Necessary to set command transmission, data transmission and data reception.
        e.g. (1) The transfer rate is RSPI Bit Rate Register (SPBR) for using RX RSPI driver.
        e.g. (2) The transfer rate is QSPI Bit Rate Register (SPBR) for using RX QSPI driver. */

#define MEMDRV_CFG_DEV0_BR                ((uint32_t)(1000000))
                                        /* Device 0 Transfer rate for command transmission.     */
#define MEMDRV_CFG_DEV0_BR_WRITE_DATA     ((uint32_t)(1000000))
                                        /* Device 0 Transfer rate for data transmission.        */
#define MEMDRV_CFG_DEV0_BR_READ_DATA      ((uint32_t)(1000000))
                                        /* Device 0 Transfer rate for data reception.           */

#define MEMDRV_CFG_DEV1_BR                ((uint32_t)(1000000))
                                        /* Device 1 Transfer rate for command transmission.     */
#define MEMDRV_CFG_DEV1_BR_WRITE_DATA     ((uint32_t)(1000000))
                                        /* Device 1 Transfer rate for data transmission.        */
#define MEMDRV_CFG_DEV1_BR_READ_DATA      ((uint32_t)(1000000))
                                        /* Device 1 Transfer rate for data reception.           */

/************************************************************************************************
DMAC ONLY : CHANNEL NUMBER OF FIT DMAC FOR RX
*************************************************************************************************/
/* Set channel No. of RX FIT DMAC driver for transmission and reception. */
#define MEMDRV_CFG_DEV0_DMAC_CH_NO_Tx     (0) /* Device 0 DMAC for transmission & reception   */
#define MEMDRV_CFG_DEV0_DMAC_CH_NO_Rx     (1) /* Device 0 DMAC for reception                  */
#define MEMDRV_CFG_DEV1_DMAC_CH_NO_Tx     (2) /* Device 1 DMAC for transmission & reception   */
#define MEMDRV_CFG_DEV1_DMAC_CH_NO_Rx     (3) /* Device 1 DMAC for reception                  */

/************************************************************************************************
DMAC ONLY : PRIORITY LEVEL OF FIT DMAC FOR RX
*************************************************************************************************/
/* Set interrupt priority level of RX FIT DMAC driver for transmission and reception. */
#define MEMDRV_CFG_DEV0_DMAC_INT_PRL_Tx   (10)
                            /* Device 0 Interrupt priority level for RX FIT DMAC transmission.  */
#define MEMDRV_CFG_DEV0_DMAC_INT_PRL_Rx   (10)
                            /* Device 0 Interrupt priority level for RX FIT DMAC reception.     */
#define MEMDRV_CFG_DEV1_DMAC_INT_PRL_Tx   (10)
                            /* Device 1 Interrupt priority level for RX FIT DMAC transmission.  */
#define MEMDRV_CFG_DEV1_DMAC_INT_PRL_Rx   (10)
                            /* Device 1 Interrupt priority level for RX FIT DMAC reception.     */

/************************************************************************************************
SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING
*************************************************************************************************/
/* Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting
   Setting to 1 includes parameter checking; 0 compiles out parameter checking */
#define MEMDRV_CFG_PARAM_CHECKING_ENABLE  (BSP_CFG_PARAM_CHECKING_ENABLE)

/************************************************************************************************
ENABLE DEBUGGING INFORMATION
*************************************************************************************************/
/* Define using Firmware Integration Technology (FIT) for RX family MCU,
   Stores error log information using LONGQ module of FIT. */
/* 0 : Not use LONGQ module of FIT. */
/* 1 : Use FIT LONGQ of FIT. Please obtain FIT LONGQ module separately. */
#define MEMDRV_CFG_LONGQ_ENABLE   (0)

#endif /* __MEMDRV_CONFIG_H__ */

/* End of File */
