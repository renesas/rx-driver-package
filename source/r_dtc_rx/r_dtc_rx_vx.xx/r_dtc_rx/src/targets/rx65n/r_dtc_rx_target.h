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
* Copyright (C) 2016(2017) Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_dtc_rx_target.h
* Description  : Functions for using DTC on RX65N devices.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 30.09.2016 2.05    First Release
*         : 31.07.2017 2.08    Supported RX65N-2MB and RX130-512KB.
*         : 01.02.2019 2.20    Supported RX65N-64pin.
*******************************************************************************/
#ifndef DTC_RX_TARGET_H
#define DTC_RX_TARGET_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* The number of activation sources */
#if defined(BSP_MCU_RX65N_2MB)
#define DTC_NUM_INTERRUPT_SRC       (201)
#else
#define DTC_NUM_INTERRUPT_SRC       (199)
#endif /* defined(BSP_MCU_RX65N_2MB) */

#if (DTC_ENABLE == DTC_CFG_USE_SEQUENCE_TRANSFER)
/* Size of DTC Vector table and DTC Index table (in byte units) */
#define DTC_VECTOR_TABLE_SIZE_BYTES (0x400 + 0x400 + 0x400)
#else
/* Size of DTC Vector table (in byte units) */
#define DTC_VECTOR_TABLE_SIZE_BYTES (0x400 + 0x400)
#endif /* (DTC_ENABLE == DTC_CFG_USE_SEQUENCE_TRANSFER) */

/* Definition of num of DMAC channel. */
#define DMAC_NUM_CHANNELS           (8)

/* DTC IP Version */
#define DTC_IP                      (DTC_IP_VER_DTCb)

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/


#endif /* DTC_RX_TARGET_H */

/* End of File */

