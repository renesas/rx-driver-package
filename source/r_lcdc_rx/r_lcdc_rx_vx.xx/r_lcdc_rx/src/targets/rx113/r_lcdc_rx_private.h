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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 01.12.2014 1.00     First Release
*              : 20.05.2019 1.01     Changed macro
*******************************************************************************/
/*******************************************************************************
* File Name    : r_lcdc_private.h
* Description  : Functions for using LCDC on RX devices. 
*******************************************************************************/
#ifndef LCDC_PRIVATE_H
#define LCDC_PRIVATE_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* Fixed width integer support. */
#include <stdint.h>
/* bool support */
#include <stdbool.h>
#include "platform.h"
#include "r_lcdc_rx_if.h"
#include "r_lcdc_rx_config.h"
#include "r_lcdc_rx_private.h"
#include "r_bsp_config.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
/*----------------------------------------------------------------------------*/
/*   LCD clock source Select (LCDSCLKSEL bit).                                */
/*----------------------------------------------------------------------------*/
#define LCDC_CLK_LOCO              (0)
#define LCDC_CLK_HOCO              (1)
#define LCDC_CLK_MAIN              (2)
#define LCDC_CLK_SUB               (3)
#define LCDC_CLK_IWDT              (4)
#define LCDC_OVERFLOW_NOT_OCCURRED (0)
#define LCDC_NOT_OPERATE           (1)
#define LCDC_CLK_NOT_STOP          (0)
#define LCDC_CLK_STOP              (1)

/*----------------------------------------------------------------------------*/
/*   LCD display waveform select (LWAVE bit).                                */
/*----------------------------------------------------------------------------*/
#define LCDC_WAVE_A                (0)

/*----------------------------------------------------------------------------*/
/*   Time slice of LCD display select (LDTY bit).                             */
/*----------------------------------------------------------------------------*/
#define LCDC_STATIC                (1)
#define LCDC_SLICE_2               (2)
#define LCDC_SLICE_3               (3)
#define LCDC_SLICE_4               (4)
#define LCDC_SLICE_8               (8)

/*----------------------------------------------------------------------------*/
/*   LCD display bias method select (LBAS bit)                                */
/*----------------------------------------------------------------------------*/
#define LCDC_BIAS_2                (2)
#define LCDC_BIAS_3                (3)
#define LCDC_BIAS_4                (4)

/*----------------------------------------------------------------------------*/
/*   LCD Drive Voltage Generator Select (MDSTET bit).                         */
/*----------------------------------------------------------------------------*/
#define LCDC_VOL_EXTARNAL          (0)
#define LCDC_VOL_INTERNAL          (1)
#define LCDC_VOL_CAPACITOR         (2)

/*----------------------------------------------------------------------------*/
/*  Voltage boost Circuit or Capacitor Split Circuit Operation 
    Enable/Disable(VLCON bit)                               */
/*----------------------------------------------------------------------------*/
#define LCDC_SEL_CIRCUIT_ENABLE    (1)
#define LCDC_SEL_CIRCUIT_DISABLE   (0)

/*----------------------------------------------------------------------------*/
/*   Display Data Area Control                                                */
/*----------------------------------------------------------------------------*/
/* BLON bit */
#define LCDC_NOT_BLINKING          (0)
#define LCDC_BLINKING              (1)

/*LCDSLE bit */
#define LCDC_DISP_A                (0)
#define LCDC_DISP_B                (1)

/*----------------------------------------------------------------------------*/
/*   LCD Display Enable/Disable                                               */
/*----------------------------------------------------------------------------*/
/* LCDON bit */
#define LCDC_DISP_ON               (1)
#define LCDC_DISP_OFF              (0)

/* SCOC bit */
#define LCDC_DISP_ENABLE           (1)
#define LCDC_DISP_DISABLE          (0)


/*----------------------------------------------------------------------------*/
/*   The range of voltage                                                     */
/*----------------------------------------------------------------------------*/
/*  */
#define LCDC_VOL_MIN           (4)
#define LCDC_VOL_MAX           (19)
#define LCDC_VOL_MAX_4BIAS     (10)

/*----------------------------------------------------------------------------*/
/*   The initial value of a register                                          */
/*----------------------------------------------------------------------------*/
#define LCDC_INIT_LCDVLM       (0)
#define LCDC_INIT_WAVE         (0)
#define LCDC_INIT_SLICE        (0)
#define LCDC_INIT_BIAS         (0)
#define LCDC_INIT_MDSET        (0)
#define LCDC_INIT_VOL          (4)
#define LCDC_INIT_DIV          (0)
#define LCDC_INIT_LCDCLK       (0)
#define LCDC_INIT_CLKSTP       (1)
#define LCDC_INIT_SCOC         (0)

/*----------------------------------------------------------------------------*/
/*   Open judging flag                                                        */
/*----------------------------------------------------------------------------*/
#define LCDC_NOT_OPEN            (0)
#define LCDC_OPEN                (1)

/*----------------------------------------------------------------------------*/
/*   the LCD drive voltage generator judging flag                             */
/*----------------------------------------------------------------------------*/
#define LCDC_VOL_METHOD_INTERNAL (0)
#define LCDC_VOL_METHOD_CAPA     (1)

/*----------------------------------------------------------------------------*/
/*   Define counter.                                                          */
/*----------------------------------------------------------------------------*/
#define LCDC_BOOST_COUNTER  (5)  /* The number of times of performing waiting for 100 ms */
#define LCDC_BOOST_WAIT     (uint32_t)(100000) /* Waiting for 100ms  */
#define LCDC_SETUP_WAIT     (uint32_t)(5000)   /* Waiting for 5ms    */
#define LCDC_NS_TO_US       (1000000)          /* Change ns to us    */


/*============================================================================*/
/*  Parameter check of Configuration Options                                  */
/*============================================================================*/
#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

#if ((0 != LCDC_CFG_PARAM_CHECKING_ENABLE) && (1 != LCDC_CFG_PARAM_CHECKING_ENABLE))
#error "ERROR - LCDC_CFG_PARAM_CHECKING_ENABLE - Parameter error in configures file.  "
#endif

#if ((0 > LCDC_CFG_DRV_GENERATOR) || (2 < LCDC_CFG_DRV_GENERATOR))
#error "ERROR - LCDC_CFG_DRV_GENERATOR - Parameter error in configures file.  "
#endif

#if ((1 == LCDC_CFG_DRV_GENERATOR) || (2 == LCDC_CFG_DRV_GENERATOR))
#if (4 == BSP_CFG_LCD_CLOCK_SOURCE)
#error "ERROR - LCDC_CFG_DRV_GENERATOR - Parameter error in configures file.  "
#endif
#endif

#if ((0 != LCDC_CFG_VOLTAGE_WAIT) && (1 != LCDC_CFG_VOLTAGE_WAIT))
#error "ERROR - LCDC_CFG_VOLTAGE_WAIT - Parameter error in configures file.  "
#endif

#if ((2 > LCDC_CFG_BIAS) || (4 < LCDC_CFG_BIAS))
#error "ERROR - LCDC_CFG_BIAS - Parameter error in configures file.  "
#endif

#if ((1 != LCDC_CFG_TIME_SLICES) && (2 != LCDC_CFG_TIME_SLICES) && (3 != LCDC_CFG_TIME_SLICES) && (4 != LCDC_CFG_TIME_SLICES) && (8 != LCDC_CFG_TIME_SLICES))
#error "ERROR - LCDC_CFG_TIME_SLICES - Parameter error in configures file.  "
#endif


#if ((0 != LCDC_CFG_DRV_WAVEFORM) && (1 != LCDC_CFG_DRV_WAVEFORM))
#error "ERROR - LCDC_CFG_DRV_WAVEFORM - Parameter error in configures file.  "
#endif

#if (0 == BSP_CFG_LCD_CLOCK_SOURCE) || (1 == BSP_CFG_LCD_CLOCK_SOURCE) || (2 == BSP_CFG_LCD_CLOCK_SOURCE)
#if (17 > LCDC_CFG_CLOCK_DIV) || ((27 < LCDC_CFG_CLOCK_DIV ) && (43 != LCDC_CFG_CLOCK_DIV))
#error "ERROR - LCDC_CFG_CLOCK_DIV - Parameter error in configures file.  "
#endif
#elif (3 == BSP_CFG_LCD_CLOCK_SOURCE)
#if  (4 > LCDC_CFG_CLOCK_DIV) || (9 < LCDC_CFG_CLOCK_DIV )
#error "ERROR - LCDC_CFG_CLOCK_DIV - Parameter error in configures file.  "
#endif
#elif (4 == BSP_CFG_LCD_CLOCK_SOURCE)
#if  (3 > LCDC_CFG_CLOCK_DIV) || (7 < LCDC_CFG_CLOCK_DIV )
#error "ERROR - LCDC_CFG_CLOCK_DIV - Parameter error in configures file.  "
#endif
#endif

#if (1 == LCDC_CFG_DRV_GENERATOR)
#if (2 == LCDC_CFG_BIAS) || (3 == LCDC_CFG_BIAS)
#if  (4 > LCDC_CFG_REF_VCC) || (19 < LCDC_CFG_REF_VCC )
#error "ERROR - LCDC_CFG_REF_VCC - Parameter error in configures file.  "
#endif
#elif (4 == LCDC_CFG_BIAS)
#if  (4 > LCDC_CFG_REF_VCC) || (10 < LCDC_CFG_REF_VCC )
#error "ERROR - LCDC_CFG_REF_VCC - Parameter error in configures file.  "
#endif
#endif
#endif

/*----------------------------------------------------------------------------*/
/*   Common register.                                                         */
/*----------------------------------------------------------------------------*/
/* Control registers address defines  */
#define SEG_ADR(n)  ( (volatile uint8_t *)&LCDC.SEG00 + (0x01 * n))


/*******************************************************************************
Private global variables and functions
*******************************************************************************/
/*----------------------------------------------------------------------------*/
/*   Main processing of SCI IIC Driver API functions                             */
/*----------------------------------------------------------------------------*/
static lcdc_err_t lcdc_open(void);
static lcdc_err_t lcdc_write(uint8_t seg, uint8_t data);
static lcdc_err_t lcdc_modify(uint8_t seg, uint8_t data_mask, uint8_t data);
static lcdc_err_t lcdc_dispon(void);
static lcdc_err_t lcdc_dispoff(uint8_t stop_select);
static lcdc_err_t lcdc_conrtol(uint8_t config_pattern, uint8_t select_drv_gen, uint8_t select_display_area);
static void lcdc_close(void);

#if((LCDC_VOL_INTERNAL == LCDC_CFG_DRV_GENERATOR) || (LCDC_VOL_CAPACITOR == LCDC_CFG_DRV_GENERATOR))
static lcdc_err_t lcdc_operatecircuit_enable(void);
static lcdc_err_t lcdc_operatecircuit_disable(void);
#endif

#if(LCDC_VOL_INTERNAL == LCDC_CFG_DRV_GENERATOR)
static int8_t lcdc_adjustcontrast(int8_t updown_level);
#endif

/*----------------------------------------------------------------------------*/
/*   LCDC Module stop enable/disable functions                                */
/*----------------------------------------------------------------------------*/
static void power_on(void);
static void power_off(void);

/*----------------------------------------------------------------------------*/
/*   Software delay functions                                                 */
/*----------------------------------------------------------------------------*/
static void lcdc_delay  (uint32_t loop_cnt);
lcdc_err_t lcdc_delay_us (uint32_t delay_us);

#endif /* LCDC_PRIVATE_H */
