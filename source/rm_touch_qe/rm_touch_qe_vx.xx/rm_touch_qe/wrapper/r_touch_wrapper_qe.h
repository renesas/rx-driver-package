/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
* THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
************************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_touch_wrapper_qe.h
* Description  : This module implements the FIT_v1.11 wrapper API.
***********************************************************************************************************************/

#ifndef QE_TOUCH_WRAPPER_H_
#define QE_TOUCH_WRAPPER_H_

#ifdef QE_TOUCH_CONFIGURATION
#include "qe_touch_config.h"
#endif

#if (QE_WRAPPER_ENABLE == 1)

#define QE_MAX_METHODS (8)

/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/

/* Return error codes */
typedef enum e_qe_err
{
    QE_SUCCESS = 0,
    QE_ERR_NULL_PTR, // missing argument
    QE_ERR_INVALID_ARG,
    QE_ERR_BUSY,
    QE_ERR_ALREADY_OPEN,
    QE_ERR_CHAN_NOT_FOUND,
    QE_ERR_UNSUPPORTED_CLK_CFG, // unsupported clock configuration
    QE_ERR_SENSOR_SATURATION,   // sensor value detected beyond linear portion of correction curve
    QE_ERR_TUNING_IN_PROGRESS,  // offset tuning for method not complete
    QE_ERR_ABNORMAL_TSCAP,      // abnormal TSCAP detected during scan
    QE_ERR_SENSOR_OVERFLOW,     // sensor overflow detected during scan
    QE_ERR_OT_MAX_OFFSET,       // CTSU SO0 offset reached max value and sensor offset tuning incomplete
    QE_ERR_OT_MIN_OFFSET,       // CTSU SO0 offset reached min value and sensor offset tuning incomplete
    QE_ERR_OT_WINDOW_SIZE,      // offset tuning window too small for sensor to establish a reference count
    QE_ERR_OT_MAX_ATTEMPTS,     // 1+ sensors still not tuned for method
    QE_ERR_OT_INCOMPLETE,       // 1+ sensors still not tuned for method
    QE_ERR_TRIGGER_TYPE,        // function not available for trigger type
} qe_err_t;

/* Scan Trigger Source */
typedef enum e_trigger
{
    QE_TRIG_SOFTWARE, // scan started by function call; can use with DTC
    QE_TRIG_EXTERNAL, // scan started by external trigger; cannot use with DTC
    QE_TRIG_END_ENUM
} qe_trig_t;

/* R_TOUCH_Control() commands */
typedef enum e_touch_cmd
{
    TOUCH_CMD_SET_METHOD,        // scan specified method only
    TOUCH_CMD_CYCLE_ALL_METHODS, // scan methods in order provided in Open()
    TOUCH_CMD_CYCLE_METHOD_LIST, // scan/cycle through specified method list
    TOUCH_CMD_GET_FAILED_SENSOR, // identify sensor in error (if possible)
    TOUCH_CMD_GET_LAST_SCAN_METHOD,
    TOUCH_CMD_CLEAR_TOUCH_STATES, // set touch states and related counters to 0
    TOUCH_CMD_END_ENUM
} touch_cmd_t;

/* TOUCH_CMD_CYCLE_METHOD_LIST argument. */
typedef struct st_touch_mlist
{
    uint8_t num_methods;             // number of methods to scan
    uint8_t methods[QE_MAX_METHODS]; // methods in scan order
    uint8_t cur_index;               // (unused by application)
} touch_mlist_t;

/* CTSU status flags (legacy) */
typedef union
{
    struct
    {
        uint8_t sens_over : 1;   // sensor counter overflow flag
        uint8_t icomp_error : 1; // TSCAP voltage error
    } flag;
} ctsu_status_t;

/* TOUCH_CMD_GET_FAILED_SENSOR */
typedef struct st_touch_sensor
{
    uint8_t method;
    uint8_t element; // element index
} touch_sensor_t;

/***********************************************************************************************************************
* Exported global variables
***********************************************************************************************************************/
extern volatile bool g_qetouch_timer_flg;


/***********************************************************************************************************************
* Exported global functions
***********************************************************************************************************************/
qe_err_t R_TOUCH_Open(ctsu_cfg_t *p_ctsu_cfgs[], touch_cfg_t *p_touch_cfgs[], uint8_t num_methods, qe_trig_t trigger);
qe_err_t R_TOUCH_UpdateDataAndStartScan(void); // for software triggers and method cycling
qe_err_t R_TOUCH_UpdateData(void);             // for external triggers or set methods
qe_err_t R_TOUCH_Control(touch_cmd_t cmd, void *p_arg);
qe_err_t R_TOUCH_GetRawData(uint8_t method, uint16_t *p_buf, uint8_t *p_cnt);      // for advanced debugging
qe_err_t R_TOUCH_GetData(uint8_t method, uint16_t *p_buf, uint8_t *p_cnt);         // for advanced debugging
qe_err_t R_TOUCH_GetBtnBaselines(uint8_t method, uint16_t *p_buf, uint8_t *p_cnt); // for advanced debugging
qe_err_t R_TOUCH_GetAllBtnStates(uint8_t method, uint64_t *p_mask);
qe_err_t R_TOUCH_GetSliderPosition(uint16_t slider_id, uint16_t *p_position);
qe_err_t R_TOUCH_GetWheelPosition(uint16_t wheel_id, uint16_t *p_position);
qe_err_t R_TOUCH_Close(void);
uint32_t R_TOUCH_GetVersion(void);

void qetouch_timer_callback(void *p_args);

#endif

#endif /* QE_TOUCH_WRAPPER_H_ */
