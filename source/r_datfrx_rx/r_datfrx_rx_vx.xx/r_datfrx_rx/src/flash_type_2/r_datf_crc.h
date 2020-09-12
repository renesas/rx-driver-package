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
* File Name    : r_datf_crc.h
* Version      : 2.00
* Description  : DATFRX interface header file
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 28.09.2018 2.00     First Release
*************************************************************************************************/

/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_datfrx_rx_config.h"
#include "r_flash_rx_if.h"
#include "platform.h"

/************************************************************************************************
Macro definitions
*************************************************************************************************/
#ifndef R_DATF_CRC_H
#define R_DATF_CRC_H

#if(FLASH_TYPE == FLASH_TYPE_2)

/* ==== CRC register definitions ==== */
#if(FLASH_DM_CFG_CRC_HARDWARE == 1)
#define DATF_CRCC                   CRC.CRCCR.BYTE             /* CRC control register                 */
#define DATF_CRCIN                  CRC.CRCDIR                 /* CRC input register                   */
#define DATF_CRCD                   CRC.CRCDOR                 /* CRC data register                    */
#define DATF_MSTPCR_CRC             SYSTEM.MSTPCRB.BIT.MSTPB23 /* CRC Module stop setting              */
#endif    /* FLASH_DM_CFG_CRC_HARDWARE */

#endif /* (FLASH_TYPE == FLASH_TYPE_2) */

#if(FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4)

/* ==== CRC register definitions ==== */
#if(FLASH_DM_CFG_CRC_HARDWARE == 1)
#define DATF_CRCC                   CRC.CRCCR.BYTE             /* CRC control register                 */
#define DATF_CRCIN                  CRC.CRCDIR.BYTE            /* CRC input register                   */
#define DATF_CRCD                   CRC.CRCDOR.WORD            /* CRC data register                    */
#define DATF_MSTPCR_CRC             SYSTEM.MSTPCRB.BIT.MSTPB23 /* CRC Module stop setting              */
#endif    /* FLASH_DM_CFG_CRC_HARDWARE */

#endif /* (FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4) */


#if(FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4)

/* ==== CRC macro for calculation ==== */
#if(FLASH_DM_CFG_CRC_HARDWARE == 1)
#define DATF_CRC16_CLR()            do {                                                            \
        DATF_MSTPCR_CRC = 0;                                            \
        DATF_CRCC  = 0x83;                                              \
    } while (0)
#define DATF_CRC16_SET(Data)        DATF_CRCIN = Data
#define DATF_CRC16_GET()            DATF_CRCD

#else    /* FLASH_DM_CFG_CRC_HARDWARE */
#define DATF_CRC16_CLR()            (g_datf_crc16.us = 0x0000)
/* WAIT_LOOP */
#define DATF_CRC16_SET(Data, uc_tmp) do {                                                           \
        uc_tmp             = g_datf_crc16.uc[0];                                                    \
        g_datf_crc16.us    = g_datf_crc_ccitt[g_datf_crc16.uc[1] ^ Data];                           \
        g_datf_crc16.uc[1] = g_datf_crc16.uc[1] ^ uc_tmp;                                           \
    } while (0)
#define DATF_CRC16_GET()            (g_datf_crc16.us)

#endif    /* FLASH_DM_CFG_CRC_HARDWARE */

/************************************************************************************************
Global Typedef definitions
*************************************************************************************************/
typedef union
{
    uint16_t    us;
    uint8_t     uc[2];
} u_datf_exchg_short_t;                                     /* total 2byte                          */

/************************************************************************************************
Exported global variables
*************************************************************************************************/

/************************************************************************************************
Exported global functions (to be accessed by other files)
*************************************************************************************************/
uint16_t r_datf_crc16(uint8_t *pData, uint16_t CrcCnt);

#endif /* (FLASH_TYPE == FLASH_TYPE_2 || FLASH_TYPE == FLASH_TYPE_3 || FLASH_TYPE == FLASH_TYPE_4) */

#endif    /* R_DATF_CRC_H   */

/* End of File */

