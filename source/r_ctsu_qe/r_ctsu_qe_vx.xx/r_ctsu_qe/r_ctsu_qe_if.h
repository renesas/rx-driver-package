/***********************************************************************************************************************
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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_qectsu_rx_if.h
* Description  : This file contains the CTSU API and should be included by the application which uses this API.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*           04.10.2018 1.00    First Release
*           09.07.2019 1.10    Created private internal control commands ctsu_pcmd_t.
*                              Added QE_ERR_ABNORMAL_TSCAP and QE_ERR_SENSOR_OVERFLOW return codes to Open()
*                                (error detected during correction)
*                              Modified to allow R_CTSU_StartScan() to be called in external trigger mode IF
*                                Touch is performing offset tuning.
*                              Set ctsu state to RUN (ctsu now busy) in ctsu_ctsuwr_isr() so Touch can detect
*                                if it missed processing a scan when using external triggers.
*                              Added special callback support for scan done interrupt.
*                              Added Control() commands CTSU_CMD_GET_METHOD_MODE and TOUCH_CMD_GET_SCAN_INFO.
*                              Added error checking in correction_CTSU_register_txd_set() switch default.
*                              Added #pragma sections for diagnostic/safety code.
*                              Modified for GCC/IAR compatibility.
*           09.01.2020 1.11    Fixed bug where custom callback function was called twice.
*                              Added Control() commands CTSU_CMD_SNOOZE_ENABLE and CTSU_CMD_SNOOZE_DISABLE.
*                              Fixed compile-time bug when PLL multiplier of 13.5 is selected for RX231.
***********************************************************************************************************************/

#ifndef R_CTSU_QE_IF_H
#define R_CTSU_QE_IF_H

#include "platform.h"
#include "r_typedefs_qe.h"
#include "r_ctsu_qe_config.h"


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Driver Version Number. */
#define QECTSU_RX_VERSION_MAJOR         (1)
#define QECTSU_RX_VERSION_MINOR         (11)

#define CTSU_INPUT_FREQUENCY_DIV        (BSP_PCLKB_HZ / 1000000)


/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/* Event type for the ctsu interrupt callback function */
typedef enum e_ctsu_isr_event
{
    CTSU_EVT_SCAN_COMPLETE,
    CTSU_EVT_VOLT_ERR,              // abnormal TSCAP voltage (CTSUERRS.CTSUICOMP set)
    CTSU_EVT_SENSOR_OVERFLOW,       // sensor overflow (CTSUST.CTSUSOVF set)
    CTSU_EVT_REF_OVERFLOW,          // reference overflow (CTSUST.CTSUROVF set)
    CTSU_EVT_OVERFLOW,              // (CTSUST.CTSUSOVF and CTSUROVF set)
} ctsu_isr_evt_t;


/* CTSU Registers */
typedef enum e_ctsu_reg
{
    CTSU_REG_CR0,
    CTSU_REG_CR1,
    CTSU_REG_SDPRS,
    CTSU_REG_MCH0,
    CTSU_REG_MCH1,
    CTSU_REG_CHAC0,
    CTSU_REG_CHAC1,
    CTSU_REG_CHAC2,
    CTSU_REG_CHAC3,
    CTSU_REG_CHAC4,
    CTSU_REG_CHTRC0,
    CTSU_REG_CHTRC1,
    CTSU_REG_CHTRC2,
    CTSU_REG_CHTRC3,
    CTSU_REG_CHTRC4,
    CTSU_REG_ST,
    CTSU_REG_SSC,
    CTSU_REG_SO0,
    CTSU_REG_SO1,
    CTSU_REG_SC,
    CTSU_REG_RC,
    CTSU_REG_ERRS,
    CTSU_REG_END_ENUM
} ctsu_reg_t;


/* CTSU Control() Commands */
typedef enum e_ctsu_cmd
{
    CTSU_CMD_SET_CALLBACK,                  // set callback for scan complete interrupts
    CTSU_CMD_GET_STATE,
    CTSU_CMD_GET_STATUS,
    CTSU_CMD_SET_METHOD,                    // change scan configuration
    CTSU_CMD_GET_METHOD_MODE,
    CTSU_CMD_GET_SCAN_INFO,
    CTSU_CMD_SNOOZE_ENABLE,
    CTSU_CMD_SNOOZE_DISABLE,
    CTSU_CMD_END_ENUM
} ctsu_cmd_t;


/* CTSU_CMD_GET_STATE values */
typedef enum e_ctsu_state
{
    CTSU_STATE_STOP       = 0,              // fatal error occurred
    CTSU_STATE_READY      = 1,              // post-scan processing (if any) complete; idle
    CTSU_STATE_RUN        = 2,              // scan in progress
    CTSU_STATE_FINISH     = 3,              // scan complete
} ctsu_state_t;


/* CTSU_CMD_GET_METHOD_MODE values */
typedef enum e_ctsu_mode
{
    CTSU_MODE_SELF_SINGLE_SCAN = 0,
    CTSU_MODE_SELF_MULTI_SCAN  = 1,
    CTSU_MODE_MUTUAL_FULL_SCAN = 3,
} ctsu_mode_t;


/* CTSU_CMD_GET_SCAN_INFO */
typedef struct st_scan_info
{
    uint16_t        ctsuerrs;               // register value at end of method scan
    uint8_t         ctsust;                 // register value at end of method scan
} scan_info_t;


/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/
extern uint16_t    g_linear_interpolation_table[];
extern uint16_t    g_linear_interpolation_coefficient[];


/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
qe_err_t R_CTSU_Open(ctsu_cfg_t *p_ctsu_cfgs[], uint8_t num_methods, qe_trig_t trigger);
qe_err_t R_CTSU_StartScan(void);
qe_err_t R_CTSU_ReadButton(btn_ctrl_t *p_btn_ctrl, uint16_t *p_value1, uint16_t *p_value2);
qe_err_t R_CTSU_ReadSlider(sldr_ctrl_t *p_sldr_ctrl, uint16_t *p_buf1, uint16_t *p_buf2);
qe_err_t R_CTSU_ReadElement(uint8_t element, uint16_t *p_value1, uint16_t *p_value2);
qe_err_t R_CTSU_ReadData(uint16_t *p_buf, uint16_t *p_cnt);
qe_err_t R_CTSU_ReadReg(ctsu_reg_t reg, uint8_t element, uint16_t *p_value);
qe_err_t R_CTSU_WriteReg(ctsu_reg_t reg, uint8_t element, uint16_t value);
qe_err_t R_CTSU_Control(ctsu_cmd_t cmd, uint8_t method, void *p_arg);
qe_err_t R_CTSU_Close(void);
uint32_t R_CTSU_GetVersion(void);

#endif /* R_CTSU_QE_IF_H */
