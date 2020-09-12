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
*              : 20.05.2019 1.01     Updated version to 1.01
*                                    Deleted the packoption.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_lcdc_rx_if.h
* Description  : Functions for using LCDC on RX devices. 
*******************************************************************************/
#ifndef LCDC_IF_H
#define LCDC_IF_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* Fixed width integer support. */
#include <stdint.h>
/* bool support */
#include <stdbool.h>
/* Used for configuring the LCDC code */
#include "r_lcdc_rx_config.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* Version Number of API. */
#define LCDC_VERSION_MAJOR  (1)
#define LCDC_VERSION_MINOR  (01)


/*******************************************************************************
Typedef definitions
*******************************************************************************/
typedef enum /* LCDC API error codes */
{
    LCDC_ERR_OUTSIDE_VOLTAGE = -1, /* */
    LCDC_SUCCESS,             /* Processing completed successfully */
    LCDC_ERR_NOT_CLOSED,      /* Already locked */
    LCDC_ERR_NOT_OPEN,        /* R_LCDC_Open function not executed */
    LCDC_ERR_INVALID_ARG,     /* Invalid parameter */
    LCDC_ERR_UNSUPPORTED,     /* Combination of display mode not supported */
    LCDC_ERR_NOT_OPERATE,     /* Specified clock source not operated*/
    LCDC_ERR_RTC_NOT_OPERATE, /* RTC not operated with blinking display selected */
    LCDC_ERR_OTHER
} lcdc_err_t;

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* Select the operation, according to the specifications of the user system, 
   whether to disable display on the LCD only or also stop the voltage boost/capacitor split circuit.*/
#define LCDC_ONLY_DISPLAY_OFF      (uint8_t)(0x00)  /* Only display on the LCD is disabled. */
#define LCDC_CIRCUIT_STOP          (uint8_t)(0x01)  /* Display on the LCD is disabled and the circuit is stopped. */

/* Select voltage boost circuit/capacitor split circuit enable stops or enables. */
#define LCDC_CIRCUIT_ENABLE        (uint8_t)(0x01)  /* The Circuit is enabled. */
#define LCDC_CIRCUIT_DISABLE       (uint8_t)(0x00)  /* The Circuit is disabled. */

/* Select what is changed in R_LCDC_Control function. (multiple choice)*/
typedef uint8_t lcdc_ctrl_ptn_t;
#define LCDC_GEN_ERR_CON           (lcdc_ctrl_ptn_t)(0x00)  /* Error output. */
#define LCDC_FIX_DRV_GENERETOR     (lcdc_ctrl_ptn_t)(0x01)  /* The drive voltage method is changed. */
#define LCDC_FIX_DISPLAY_AREA      (lcdc_ctrl_ptn_t)(0x02)  /* The display data area is changed. */
 
/* Change LCD driver voltage generator select. */
#define LCDC_DRV_INTERNAL_TO_CAPA  (uint8_t)(0x00)  /* The method is changed from the internal voltage boosting method 
                                                       to the capacitor split method. */
#define LCDC_DRV_CAPA_TO_INTERNAL  (uint8_t)(0x01)  /* The method is changed from the capacitor split method 
                                                       to the internal voltage boosting method. */

/* Change display area control. */
#define LCDC_PATTERN_A             (uint8_t)(0x00)  /* The display data area is changed from B-pattern to A-pattern. */
#define LCDC_PATTERN_B             (uint8_t)(0x01)  /* The display data area is changed from A-pattern to B-pattern. */
#define LCDC_PATTERN_BLINKING      (uint8_t)(0x02)  /* The display data area is changed 
                                                       to the blinking with A-pattern and B-pattern. */

/* Select read of voltage value or change voltage(contrast adjustment). */
#define LCDC_VOL_READ              (uint8_t)(0x00)  /* Read the voltage value.*/
#define LCDC_VOL_ADJUST            (uint8_t)(0x01)  /* Change the voltage (contrast adjustment) */


/*******************************************************************************
Exported global functions (to be accessed by other files)
*******************************************************************************/
/* LCDC Driver API functions */
lcdc_err_t    R_LCDC_Open(void);
lcdc_err_t    R_LCDC_Write(uint8_t seg, uint8_t data);
lcdc_err_t    R_LCDC_Modify(uint8_t seg, uint8_t lcd_mask, uint8_t data);
lcdc_err_t    R_LCDC_DispOn(void);
lcdc_err_t    R_LCDC_DispOff(uint8_t stop_select);
lcdc_err_t    R_LCDC_Control(lcdc_ctrl_ptn_t config_pattern, uint8_t select_drv_gen, uint8_t select_display_area);
lcdc_err_t    R_LCDC_Close(void);
uint32_t      R_LCDC_GetVersion(void);

#if((1 == LCDC_CFG_DRV_GENERATOR) || (2 == LCDC_CFG_DRV_GENERATOR))
lcdc_err_t    R_LCDC_OperateCircuit(uint8_t select_operation);
#endif
#if(1 == LCDC_CFG_DRV_GENERATOR)
int8_t        R_LCDC_AdjustContrast(uint8_t select_vol, int8_t updawn_level);
#endif


#endif /* LCDC_IF_H */

/* End of File */
