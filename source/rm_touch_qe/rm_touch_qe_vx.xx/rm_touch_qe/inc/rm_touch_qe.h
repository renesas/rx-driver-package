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
* File Name    : rm_touch_qe.h
* Description  : This file contains the TOUCH API and should be included by the application which uses this API.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup TOUCH
 * @{
 **********************************************************************************************************************/

#ifndef RM_TOUCH_H
#define RM_TOUCH_H

#include "platform.h"

#include "rm_touch_qe_config.h"
#include "rm_touch_qe_api.h"
#include "r_ctsu_qe.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

#if (TOUCH_CFG_UART_TUNING_SUPPORT == 1)
/** CTSU state number */
typedef enum e_touch_tuning_state
{
    _0_RESET          ,
    _1_INITIALIZE     ,
    _2_SCAN           ,
    _3_COMPLETE       ,
    _4_STOP
} touch_tuning_state_t;

typedef enum e_touch_tuning_mode
{
    CORRECTION_1ST_VALUE    ,
    CORRECTION_2ND_VALUE    ,
    MEASURE_PHASE1          ,
    MEASURE_PHASE2          ,
    MEASURE_PHASE3          ,
    QE_WATTING              ,
    TUNING_START            ,
    TUNING_FINISH           ,
} touch_tuning_mode_t;

typedef enum e_touch_tuning_scan
{
    SELF_SCAN           ,
    MUTUAL_SCAN         ,
    MUTUAL_CFC_SCAN     ,
    CURRENT_SCAN        ,
} touch_tuning_scan_t;

/* Serial Tuning mode*/
typedef enum e_ctsu_soft_mode
{
    _0_CTSU_STOP_MODE   ,
    _1_CTSU_READY_MODE  ,
    _2_CTSU_RUN_MODE    ,
    _3_CTSU_FINISH_MODE ,
} ctsu_soft_mode_t;


typedef struct st_ico
{
    uint16_t    pri_sen;
    uint16_t    snd_sen;
} touch_tuning_ico_t;


typedef struct
{
    uint32_t    mska;
    uint32_t    mskb;
} touch_tuning_ts_msk_t;
#endif

/** Information of button */
typedef struct
{
    uint64_t   status;                 ///< Touch result bitmap.
    uint16_t * p_threshold;            ///< Pointer to Threshold value array. g_touch_button_threshold[] is set by Open API.
    uint16_t * p_hysteresis;           ///< Pointer to Hysteresis value array. g_touch_button_hysteresis[] is set by Open API.
    uint16_t * p_reference;            ///< Pointer to Reference value array. g_touch_button_reference[] is set by Open API.
    uint16_t * p_on_count;             ///< Continuous touch counter. g_touch_button_on_count[] is set by Open API.
    uint16_t * p_off_count;            ///< Continuous non-touch counter. g_touch_button_off_count[] is set by Open API.
    uint32_t * p_drift_buf;            ///< Drift reference value. g_touch_button_drift_buf[] is set by Open API.
    uint16_t * p_drift_count;          ///< Drift counter. g_touch_button_drift_count[] is set by Open API.
    uint8_t    on_freq;                ///< Copy from config by Open API.
    uint8_t    off_freq;               ///< Copy from config by Open API.
    uint16_t   drift_freq;             ///< Copy from config by Open API.
    uint16_t   cancel_freq;            ///< Copy from config by Open API.
} touch_button_info_t;

/** Information of slider */
typedef struct
{
    uint16_t * p_position;             ///< Calculated Position data. g_touch_slider_position[] is set by Open API.
    uint16_t * p_threshold;            ///< Copy from config by Open API. g_touch_slider_threshold[] is set by Open API.
} touch_slider_info_t;

/** Information of wheel */
typedef struct
{
    uint16_t * p_position;             ///< Calculated Position data. g_touch_wheel_position[] is set by Open API.
    uint16_t * p_threshold;            ///< Copy from config by Open API. g_touch_wheel_threshold[] is set by Open API.
} touch_wheel_info_t;

/** TOUCH private control block. DO NOT MODIFY. Initialization occurs when RM_TOUCH_Open() is called. */
typedef struct st_touch_instance_ctrl
{
    uint32_t                open;            ///< Whether or not driver is open.
    touch_button_info_t     binfo;           ///< Information of button.
    touch_slider_info_t     sinfo;           ///< Information of slider.
    touch_wheel_info_t      winfo;           ///< Information of wheel.
    bool                    serial_tuning_enable;
    touch_cfg_t const     * p_touch_cfg;     ///< Pointer to initial configurations.
    ctsu_instance_t const * p_ctsu_instance; ///< Pointer to CTSU instance.
} touch_instance_ctrl_t;

typedef struct st_touch_sensitivity_info
{
    uint16_t * p_touch_sensitivity_ratio;    ///< Pointer to sensitivity ratio array.
    uint16_t   old_threshold_ratio;          ///< Old threshold ratio.
    uint16_t   new_threshold_ratio;          ///< New threshold ratio.
    uint8_t    new_hysteresis_ratio;         ///< New hysteresis ratio.
} touch_sensitivity_info_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const touch_api_t g_touch_on_ctsu;

/** @endcond */

/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
fsp_err_t RM_TOUCH_Open(touch_ctrl_t * const p_ctrl, touch_cfg_t const * const p_cfg);
fsp_err_t RM_TOUCH_ScanStart(touch_ctrl_t * const p_ctrl);
fsp_err_t RM_TOUCH_DataGet(touch_ctrl_t * const p_ctrl,
                           uint64_t           * p_button_status,
                           uint16_t           * p_slider_position,
                           uint16_t           * p_wheel_position);
fsp_err_t RM_TOUCH_ScanStop(touch_ctrl_t * const p_ctrl);
fsp_err_t RM_TOUCH_CallbackSet(touch_ctrl_t * const          p_api_ctrl,
                               void (                      * p_callback)(touch_callback_args_t *),
                               void const * const            p_context,
                               touch_callback_args_t * const p_callback_memory);
fsp_err_t RM_TOUCH_Close(touch_ctrl_t * const p_ctrl);
fsp_err_t RM_TOUCH_SensitivityRatioGet(touch_ctrl_t * const p_ctrl, touch_sensitivity_info_t * p_touch_sensitivity_info);
fsp_err_t RM_TOUCH_ThresholdAdjust(touch_ctrl_t * const p_ctrl, touch_sensitivity_info_t * p_touch_sensitivity_info);
fsp_err_t RM_TOUCH_DriftControl(touch_ctrl_t * const p_ctrl, uint16_t input_drift_freq);

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 // RM_TOUCH_H

/*******************************************************************************************************************//**
 * @} (end addtogroup TOUCH)
 **********************************************************************************************************************/
