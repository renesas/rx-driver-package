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
* Copyright (C) 2017(2019) Renesas Electronics Corporation. All rights reserved.
**********************************************************************************************************************/
/**********************************************************************************************************************
* File Name    : r_sdhi_rx_private.h
* Description  : SDHI for the RX
***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 31.07.2017 2.00    First Release
*              : 20.05.2019 2.04    Added support for GNUC and ICCRX.
*                                   Fixed coding style. 
**********************************************************************************************************************/

#ifndef SDHI_RX_PRIVATE_H
#define SDHI_RX_PRIVATE_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "platform.h"
#include "r_sdhi_rx_config.h"

/******************************************************************************
Error checking
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
#ifdef SDHI_CFG_LONGQ_ENABLE                    /* Uses FIT LONGQ module */
#define SDHI_DRIVER_ID              (5)     /* sdhi debug log id */
#define SDHI_DEBUG_ERR_ID           (1)     /* sdhi debug log error id */
#endif /* SDHI_CFG_LONGQ_ENABLE */

#define SDHI_BIG_ENDIAN             (defined(__BIG) || defined(__RX_BIG_ENDIAN__))

/* ---- Shift values ---- */
#define SDHI_BIT_SHIFT_1            (1u)
#define SDHI_BIT_SHIFT_2            (2u)
#define SDHI_BIT_SHIFT_3            (3u)
#define SDHI_BIT_SHIFT_4            (4u)
#define SDHI_BIT_SHIFT_5            (5u)
#define SDHI_BIT_SHIFT_6            (6u)
#define SDHI_BIT_SHIFT_7            (7u)
#define SDHI_BIT_SHIFT_8            (8u)
#define SDHI_BIT_SHIFT_9            (9u)
#define SDHI_BIT_SHIFT_10           (10u)
#define SDHI_BIT_SHIFT_11           (11u)
#define SDHI_BIT_SHIFT_12           (12u)
#define SDHI_BIT_SHIFT_13           (13u)
#define SDHI_BIT_SHIFT_14           (14u)
#define SDHI_BIT_SHIFT_15           (15u)
#define SDHI_BIT_SHIFT_16           (16u)
#define SDHI_BIT_SHIFT_17           (17u)
#define SDHI_BIT_SHIFT_18           (18u)
#define SDHI_BIT_SHIFT_19           (19u)
#define SDHI_BIT_SHIFT_20           (20u)
#define SDHI_BIT_SHIFT_21           (21u)
#define SDHI_BIT_SHIFT_22           (22u)
#define SDHI_BIT_SHIFT_23           (23u)
#define SDHI_BIT_SHIFT_24           (24u)
#define SDHI_BIT_SHIFT_25           (25u)
#define SDHI_BIT_SHIFT_26           (26u)
#define SDHI_BIT_SHIFT_27           (27u)
#define SDHI_BIT_SHIFT_28           (28u)
#define SDHI_BIT_SHIFT_29           (29u)
#define SDHI_BIT_SHIFT_30           (30u)
#define SDHI_BIT_SHIFT_31           (31u)

#define SDHI_SCLKDIVEN_LOOP_COUNT   (10000)   /* SCLKDIVEN bit loop count */

/* ==== Macro functions ==== */
#define SDHI_GET_HNDL(a)            (g_sdhi_handle[(a)])

#define SDHI_OUTREG(h,offset,data)  (*(volatile uint32_t *)((h)->reg_base+(offset)) = (data))
#define SDHI_INREG(h,offset)        (*(volatile uint32_t *)((h)->reg_base+(offset)))

#ifdef SDHI_CFG_LONGQ_ENABLE                                /* Uses FIT LONGQ module                */
#define SDHI_LOG_FUNC(x, y, z)      (r_sdhi_log((x), (y), (z)))
#else
#define SDHI_LOG_FUNC(x, y, z)
#endif /* SDHI_CFG_LONGQ_ENABLE */

#ifdef SDHI_CFG_LONGQ_ENABLE    /* Uses FIT LONGQ module */
typedef enum e_sdhi_log
{
    SDHI_OPEN               =  0,
    SDHI_INT                =  1,
    SDHI_REGISTER           =  2,
    SDHI_UTIL               =  9,
    SDHI_DEV                =  10,
} sdhi_logid_t;
#endif /* SDHI_CFG_LONGQ_ENABLE */

/* ==== SD Driver work buffer (allocated by File system) ==== */
/* ---- ""VAL DEF"" ---- */
typedef struct __sdhi_sdhndl_t
{
    uint32_t        reg_base;                               /* SDHI base address */
    sdhi_status_t   (*int_cd_callback)(uint32_t, uint32_t); /* Callback function for card detection */
    sdhi_status_t   (*int_callback)(uint32_t, uint32_t);    /* Callback function for interrupt flags */
    sdhi_status_t   (*int_sdbuff_callback)(void *);         /* Callback function for interrupt flags */
    sdhi_status_t   (*int_io_callback)(uint32_t);           /* Callback function for interrupt flags */
}sdhi_sdhndl_t;

extern const void * g_sdhi_ip_base[1];
extern sdhi_sdhndl_t * g_sdhi_handle[((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t))];

/* ==== Prototype ==== */
/* ---- r_sdhi_util.c ---- */
#ifdef SDHI_CFG_LONGQ_ENABLE                                /* Uses FIT LONGQ module                */
uint32_t      r_sdhi_log(uint32_t flg, uint32_t fid, uint32_t line);
#endif /* SDHI_CFG_LONGQ_ENABLE */

/* ---- Target MCU I/F ---- */
sdhi_status_t r_sdhi_dev_init(uint32_t channel);
sdhi_status_t r_sdhi_dev_finalize(uint32_t channel);
sdhi_status_t r_sdhi_dev_int_wait(uint32_t channel, uint32_t msec);
uint32_t      r_sdhi_dev_sdopt_init(void);
sdhi_status_t r_sdhi_check_clksel(uint32_t channel);
bool          r_sdhi_dev_hardware_lock(uint32_t channel);
bool          r_sdhi_dev_hardware_unlock(uint32_t channel);

#endif /* SDHI_RX_PRIVATE_H */

