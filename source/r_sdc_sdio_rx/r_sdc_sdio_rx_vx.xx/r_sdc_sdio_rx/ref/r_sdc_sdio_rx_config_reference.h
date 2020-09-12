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
* Copyright (C) 2014(2015-2017) Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/

/**********************************************************************************************************************
* File Name    : r_sdc_sd_rx_config.h
* Device       : RX Family MCU
* Tool-Chain   : RX Family C Compiler
* H/W Platform : RSK board for RX
* Description  : Interface file for SDHI API for RX
***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 17.07.2015 1.10    Standardized by the combo.
*              : 30.09.2016 1.21    Deleted comments of SDHI_DIV_xxxx.
*              : 31.07.2017 2.00    SDHI FIT module separated into hardware low level layer and middleware layer.
*              :                    Changed prefix from SDHI to SDC_SD.
**********************************************************************************************************************/

#ifndef SDC_SD_CONFIG_H
#define SDC_SD_CONFIG_H

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/

/* ==== Define the number of SD Card to control ==== */
#define SDC_SD_CFG_CARD_NUM     (1)     /* 1 or more value */

/****************************************************************************************/
/*  Change following definition according to your target.                               */
/*  (1) status check mode                                                               */
/*          SDC_SD_MODE_HWINT  : status check mode is hardware interrupt                */
/*          SDC_SD_MODE_POLL   : status check mode is software polling                  */
/*  (2) support SD Card                                                                 */
/*          SDC_SD_MODE_MEM    : SD Memory                                              */
/*          SDC_SD_MODE_IO     : SDIO                                                   */
/*  (3) bus bus width                                                                   */
/*          SDC_SD_MODE_1BIT   : SD Mode 1bit                                           */
/*          SDC_SD_MODE_4BIT   : SD Mode 4bit                                           */
/****************************************************************************************/
/* #define SDC_SD_CFG_DRIVER_MODE   (SDC_SD_MODE_HWINT | SDC_SD_MODE_MEM | SDC_SD_MODE_4BIT) */
#define SDC_SD_CFG_DRIVER_MODE   (SDC_SD_MODE_HWINT | SDC_SD_MODE_IO  | SDC_SD_MODE_4BIT)

/* #define SDC_SD_CFG_LONGQ_ENABLE */

/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/

#endif /* SDC_SD_CONFIG_H */

