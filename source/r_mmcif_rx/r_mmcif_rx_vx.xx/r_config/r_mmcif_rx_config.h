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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/

/**********************************************************************************************************************
* File Name    : r_mmcif_rx_config.h
* Device       : RX64M
* Tool-Chain   : RX Family C Compiler
* H/W Platform : RSK board for RX64M
* Description  : Configures the MMCIF for RX
***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 03.09.2014 1.00    First Release
**********************************************************************************************************************/

#ifndef _MMC_CONFIG_H
#define _MMC_CONFIG_H

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
/* ==== Use firmware integration technology ==== */
#define MMC_CFG_USE_FIT

/* ==== #define for MMC channel 0 to be valid ==== */
#define MMC_CFG_CH0_INCLUDED
/* #define MMC_CFG_CH1_INCLUDED */

/****************************************************************************************/
/*  Change following definition according to your target.                               */
/*  (1) status check mode                                                               */
/*          MMC_MODE_HWINT  : status check mode is hardware interrupt                   */
/*          MMC_MODE_POLL   : status check mode is software polling                     */
/*  (2) support eMMC                                                                    */
/*          MMC_MODE_eMMC   : support eMMC                                              */
/*          MMC_MODE_MMC    : not support eMMC                                          */
/*  (3) bus bus width                                                                   */
/*          MMC_MODE_1BIT   : MMC Mode 1bit                                             */
/*          MMC_MODE_4BIT   : MMC Mode 4bit                                             */
/*          MMC_MODE_8BIT   : MMC Mode 8bit                                             */
/****************************************************************************************/
#define MMC_CFG_DRIVER_MODE             (MMC_MODE_HWINT | MMC_MODE_eMMC | MMC_MODE_1BIT)
/* #define MMC_CFG_DRIVER_MODE          (MMC_MODE_HWINT | MMC_MODE_eMMC | MMC_MODE_4BIT) */
/* #define MMC_CFG_DRIVER_MODE          (MMC_MODE_HWINT | MMC_MODE_eMMC | MMC_MODE_8BIT) */
/* #define MMC_CFG_DRIVER_MODE          (MMC_MODE_HWINT | MMC_MODE_MMC  | MMC_MODE_1BIT) */
/* #define MMC_CFG_DRIVER_MODE          (MMC_MODE_HWINT | MMC_MODE_MMC  | MMC_MODE_4BIT) */

/* ==== #define for the use of the CD pin ==== */
/* (0) Not used CD pin (For example : eMMC)
   (1) Used CD pin (For example : MMC card) */
#if defined(MMC_CFG_CH0_INCLUDED)
#define MMC_CFG_CH0_CD_ACTIVE           (0)
#endif  /* #if defined(MMC_CFG_CH0_INCLUDED) */

#if defined(MMC_CFG_CH1_INCLUDED)
#define MMC_CFG_CH1_CD_ACTIVE           (0)
#endif  /* #if defined(MMC_CFG_CH1_INCLUDED) */

/* ==== #define for MMC clock DIV (Depend on the electrical characteristic of MCU) ==== */
#define MMC_CFG_DIV_HIGH_SPEED          MMC_DIV_2       /* 52MHz or less clock                                  */
#define MMC_CFG_DIV_BACKWARD_COM_SPEED  MMC_DIV_4       /* 26MHz or less clock                                  */
#define MMC_CFG_DIV_INIT_SPEED          MMC_DIV_1024    /* 400KHz or less clock                                 */
                                                        /* In the SD Slot of RX64M_RSK, the MMC_CMD line gets
                                                           the pulling up resistance of the 100k ohm.
                                                           Therefore, cannot transfer identification mode in 400KHz.
                                                           The default setting of MMC_CFG_DIV_INIT_SPEED is
                                                           the clock frequency that is slower than 400KHz. */

/* ==== #define for MMC time out count ==== */
#define MMC_CFG_TIMEOUT_TRANS           (0x000000a0ul)  /* CECLKCTR register : Write data/read data timeout     */
#define MMC_CFG_TIMEOUT_RESBUSY         (0x00000f00ul)  /* CECLKCTR register : Response busy timeout            */
#define MMC_CFG_TIMEOUT_RES             (0x00002000ul)  /* CECLKCTR register : Response timeout                 */
#define MMC_CFG_TIMEOUT_BOOT_DATA       (0x000f0000ul)  /* CEBOOT register   : Boot data timeout                */
#define MMC_CFG_TIMEOUT_BOOT_FIRSTDATA  (0x00f00000ul)  /* CEBOOT register   : First boot data timeout          */
#define MMC_CFG_TIMEOUT_BOOT_ACKNOW     (0x0f000000ul)  /* CEBOOT register   : Boot acknowledge timeout         */

/* ==== #define for MMC grp int number & level ==== */
#if defined(MMC_CFG_CH0_INCLUDED)
#define MMC_CFG_CH0_INT_LEVEL           (10)            /* MMC channel 0 interrupt level                        */
#define MMC_CFG_CH0_INT_LEVEL_DMADTC    (10)            /* MMC channel 0 DMA/DTC interrupt level                */
#endif  /* #if defined(MMC_CFG_CH0_INCLUDED) */

#if defined(MMC_CFG_CH1_INCLUDED)
#define MMC_CFG_CH1_INT_LEVEL           (10)            /* MMC channel 1 interrupt level                        */
#define MMC_CFG_CH1_INT_LEVEL_DMADTC    (10)            /* MMC channel 1 DMA/DTC interrupt level                */
#endif  /* #if defined(MMC_CFG_CH1_INCLUDED) */

/* ==== #define enable to FIT LONGQ module ==== */
/* #define MMC_CFG_LONGQ_ENABLE */

/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/


#endif /* _MMC_CONFIG_H */

