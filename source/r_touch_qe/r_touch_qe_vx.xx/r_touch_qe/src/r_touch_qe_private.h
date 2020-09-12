/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_qetouch_rx_private.h
* Description  : This file includes definitions internal to the QE TOUCH driver.
***********************************************************************************************************************/

/***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 04.10.2018 1.00    First Release
***********************************************************************************************************************/
#ifndef QETOUCH_RX_PRIVATE_H__FILE
#define QETOUCH_RX_PRIVATE_H__FILE

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "r_typedefs_qe.h"
#include "qe_common.h"
#include "r_touch_qe_config.h"
#include "r_touch_qe_if.h"        // method_list_t


/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/
#define DRIFT_OK        (0)
#define DRIFT_ERROR     (1)
#define DRIFT_OFF       (2)

#define KEY_DISENABLE   (0)
#define KEY_ENABLE      (1)

#define ZERO_ENA        (0)
#define ZERO_DIS        (1)

#define _0_RUN          (0)
#define _1_FINISH       (1)

#define _0_STOP         (0)
#define _1_START        (1)

#define TOUCH_SELF_MODE     (0)
#define TOUCH_MUTUAL_MODE   (1)


/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/

/* Touch system control flag */
typedef union
{
    struct
    {
        volatile uint8_t    initial     :1;     /* Calibration completion flag */
        volatile uint8_t    timing      :1;     /* timing flag */
        volatile uint8_t    reserved    :6;     /* reserved */
    } flag;
    uint8_t    value;
} touch_system_t;


/* Touch function control flag */
typedef union
{
    struct
    {
        volatile uint8_t    tuning      :1;     /* Initial tuning completion flag */
        volatile uint8_t    average     :1;     /* Moving average flag */
        volatile uint8_t    calib       :1;     /* Calibration completion flag */
        volatile uint8_t    drift       :1;     /* Drift correction */
        volatile uint8_t    msa         :1;     /* Maximum successive on counter */
        volatile uint8_t    acd0        :1;     /* Accumulated counter */
        volatile uint8_t    acd1        :1;     /* Accumulated counter */
        volatile uint8_t    dummy       :1;     /* dummy */
    } flag;
    uint8_t    value;
} touch_func_flag_t;


/* Touch function parameter */
typedef struct
{
    uint8_t     calib_key_num;      /*  */
    uint8_t     calib_freq;         /* Calibration frequency */
    uint8_t     touch_freq;         /* Continuous agreement touch comparison frequency */
    uint8_t     not_touch_freq;     /* Continuous agreement non-touch comparison frequency */
    uint16_t    drift_freq;         /* Drift correction frequency */
    uint16_t    msa_freq;           /* Compulsion touch cancellation frequency */
} touch_func_param_t;


/* Touch key parameter information */
typedef struct
{
    uint8_t     mode;               /* Measurement mode flag  0=Self, 1=Mutual */
    uint8_t     key_num;            /* Key function number */
    uint8_t     ena_num;            /* Touch sensor number */
    uint8_t     key_max_group;      /* Key group max number */
    uint16_t    *ref;               /* Reference value */
    uint16_t    *thr;               /* Threshold value */
    uint16_t    *user_thr;          /* User setting touch threshold value */
    uint16_t    *hys;               /* User setting hysteresis value */
    uint16_t    *delta;             /* Touch judgment delta  value */
    uint16_t    *touch_cnt;         /* Continuous agreement touch counter */
    uint16_t    *non_touch_cnt;     /* Continuous agreement non-touch counter */
    uint16_t    *in_touch;          /* Inside touch flag */
    uint16_t    *out_touch;         /* Outside touch flag */
    uint16_t    *touch_result;      /* Touch result flag */
    uint16_t    *drift_permission;  /* Drift permission flag */
    uint32_t    *drift_add_ref;     /* Drift reference value */
    uint16_t    *drift_cnt;         /* Drift correction counter */
    uint16_t    *key_used_info;     /* Keu function used flag */
    uint8_t     *sensor_index;      /* Touch sensor index */
    int8_t      *counter_magni;     /* Touch sensor index */
} key_info_t;


/* Offset tuning information */
typedef struct
{
    uint16_t            *ctsuso;    /* Tuning ctsuso value */
    volatile uint8_t    *result;    /* Tuning completion flag */
} touch_tuning_t;


/* Calibration information */
typedef struct
{
    uint8_t     calib_key;          /* Calibration key number */
    uint8_t     calib_cnt;          /* Calibration counter */
    uint16_t    *calib_data;        /* Calibration value */
} calib_info_t;


typedef struct
{
    const uint16_t  *group;
} touch_group_param_t;


/* Button(Key), Slider, Wheel result information */
typedef struct
{
    uint16_t    button[3];              /* Button on/off result */
#if (QE_MAX_SLIDERS >= 1)
    uint16_t    slider[QE_MAX_SLIDERS]; /* Slider position result */
#else
    uint16_t    slider[1];              /* Slider position result(dummy) */
#endif
#if (QE_MAX_WHEELS >= 1)
    uint16_t    wheel[QE_MAX_WHEELS];   /* Wheel position result */
#else
    uint16_t    wheel[1];               /* Wheel position result(dummy) */
#endif
    uint16_t    matrix[4];              /* Matrix on/off result */
} touch_result_t;


/***********************************************************************************************************************
* Global variables
***********************************************************************************************************************/
extern self_ico_t   *g_self_ico_data_pt[];
extern self_ico_t   *g_self_raw_data_pt[];
extern uint16_t     *g_self_ico_sensor_pt[];
extern uint16_t     *g_self_add_sensor_pt[];
extern uint16_t     *g_self_sensor_cnt_pt[];

extern mutual_ico_t *g_mutual_ico_data_pt[];
extern mutual_ico_t *g_mutual_raw_data_pt[];
extern uint16_t     *g_mutual_ico_pri_sensor_pt[];
extern uint16_t     *g_mutual_ico_snd_sensor_pt[];
extern uint16_t     *g_mutual_add_pri_sensor_pt[];
extern uint16_t     *g_mutual_add_snd_sensor_pt[];
extern uint16_t     *g_mutual_sensor_diff_pt[];

extern int8_t       *g_current_sign_pt[];

extern key_info_t           g_key_info[];
extern touch_group_param_t  g_touch_key_group[];
extern calib_info_t         g_calib_info[];
extern touch_tuning_t       g_touch_tuning_info[];

extern touch_system_t       g_touch_system;
extern touch_func_flag_t    g_touch_function[];
extern touch_result_t       g_touch_all_result[];
extern uint16_t             g_offset_time[];
extern uint8_t              g_current_offset_count[];
extern touch_func_param_t   g_touch_paramter[];
extern volatile uint8_t     g_data_tim;

extern uint16_t         g_slider_data[];
extern uint16_t         g_wheel_data[];
extern uint8_t          g_method_stor;
extern uint8_t          g_open_num_methods;
extern touch_mlist_t    g_mlist;

extern touch_cfg_t      **gp_touch_configs;
extern sldr_ctrl_t      *gp_sliderInfo;
extern wheel_ctrl_t     *gp_wheelInfo;


/***********************************************************************************************************************
* Global functions
***********************************************************************************************************************/
extern void     touch_parameter_address_set(void);
extern uint8_t  offset_tuning_stop_judgement(uint8_t method);
extern uint8_t  touch_calibration_check(uint8_t method, uint8_t offset_sta);
extern void     touch_key_decode(uint8_t method, uint16_t value, uint8_t number);
extern uint16_t slider_decode(uint8_t slider_id);
extern uint16_t wheel_decode(uint8_t wheel_id);
extern void     touch_parameter_set(ctsu_cfg_t *p_ctsu_cfgs[], touch_cfg_t *p_touch_cfgs[]);
extern uint8_t  touch_key_function_check(uint8_t method, uint8_t loop);
/* following can be made static but need #ifs if widget not used */
extern uint8_t  slider_decode_abnormal_judgement(uint8_t id, uint16_t delta1, uint16_t delta2, uint8_t maxch);
extern uint8_t  wheel_decode_abnormal_judgement(uint8_t id, uint16_t delta1, uint16_t delta2, uint8_t maxch);
extern uint8_t  touch_calibration(uint8_t method);
extern void     touch_initial_reference_set(uint8_t method, uint8_t number);

#endif  // QETOUCH_RX_PRIVATE_H__FILE
