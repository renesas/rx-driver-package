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
*              : 09.07.2019 1.10    Moved TUNING_UPPER/LOWER_LIMIT #defines here from touch_qe.c.
*              :                    Added OT_WINDOW_TUNE_COUNT, OT_MAX_SCAN_ATTEMPTS, CTSUSO_INCREMENTING, and
*              :                      CTSUSO_DECREMENTING.
*              :                    Removed unused variables from former offset tuning and ref count initialization.
*              :                    Added touch_pcmd_t.
*              :                    Removed extern g_calib_info[][] declaration.
*              : 31.10.2019 1.11    Renamed TOUCH_PCMD_CONTINUE_OFFSET_TUNING to TOUCH_PCMD_CLEAR_TUNING_FLAGS.
***********************************************************************************************************************/
#ifndef QETOUCH_RX_PRIVATE_H
#define QETOUCH_RX_PRIVATE_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "r_typedefs_qe.h"
#include "qe_common.h"
#include "r_touch_qe_config.h"
#include "r_touch_qe_if.h"        /* method_list_t */


/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/
#define OT_WINDOW_TUNE_COUNT    (10)    /* number of scans with counts within window for sensor to be considered tuned */
#define OT_MAX_SCAN_ATTEMPTS    (255)   /* offset tuning max scan attempts before considered failure */

#define TUNING_UPPER_LIMIT      (200)
#define TUNING_LOWER_LIMIT      (150)


#define DRIFT_OK        (0)
#define DRIFT_ERROR     (1)
#define DRIFT_OFF       (2)

#define KEY_DISENABLE   (0)
#define KEY_ENABLE      (1)

#define ZERO_ENA        (0)
#define ZERO_DIS        (1)

#define TOUCH_SELF_MODE     (0)
#define TOUCH_MUTUAL_MODE   (1)

/* For touch_tuning_t ctsuso (see below) Offset tuning information for element */
#define CTSUSO_INCREMENTING     (1)
#define CTSUSO_DECREMENTING     (2)


/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/

/* r_touch_control_private() commands */
typedef enum e_touch_pcmd
{
    TOUCH_PCMD_PERFORM_OFFSET_TUNING,
    TOUCH_PCMD_CLEAR_TUNING_FLAGS,      // if want to do offset tuning again, call this first
    TOUCH_PCMD_END_ENUM
} touch_pcmd_t;


/* Touch function control flag */
typedef union
{
    struct
    {
        volatile uint8_t    tuning      :1;     /* 1 = offset tuning complete for method */
        volatile uint8_t    average     :1;     /* 0 = no data value available to average with */
        volatile uint8_t    calib       :1;     /* unused */
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
    uint8_t     touch_freq;         /* configured touch-on debounce count number */
    uint8_t     not_touch_freq;     /* configured touch-off debounce count number */
    uint16_t    drift_freq;         /* configured drift correction frequency (in number of scans) */
    uint16_t    msa_freq;           /* configured touch-stuck-on cancellation freq (in number of scans) */
} touch_func_param_t;


/* Touch key parameter information */
typedef struct
{
    uint8_t     mode;               /* Measurement mode 0=Self, 1=Mutual */
    uint8_t     key_num;            /* number of buttons configured */
    uint8_t     ena_num;            /* number of elements configured/scanned */
    uint8_t     key_max_group;      /* Key group max number */
    uint16_t    *ref;               /* Reference value */
    uint16_t    *thr;               /* ref + user_thr threshold value */
    uint16_t    *user_thr;          /* User setting touch threshold value */
    uint16_t    *hys;               /* User setting hysteresis value */
    uint16_t    *delta;             /* sensor value - ref absolute value */
    uint16_t    *touch_cnt;         /* touched debounce counter */
    uint16_t    *non_touch_cnt;     /* touch-stopped debounce counter */
    uint16_t    *in_touch;          /* possibly-touched flag */
    uint16_t    *out_touch;         /* definitely-touched flag */
    uint16_t    *touch_result;      /* Touch result flag */
    uint16_t    *drift_permission;  /* Drift permission flag */
    uint32_t    *drift_add_ref;     /* Drift reference value */
    uint16_t    *drift_cnt;         /* Drift correction counter */
    uint16_t    *key_used_info;     /* key group masks; buttons configured */
    uint8_t     *sensor_index;      /* Touch sensor index (for TS# to element# conversion) */
    int8_t      *counter_magni;     /* scaling factor (magnification) for PCLK */
} key_info_t;


typedef struct
{
    uint16_t            *ctsuso;    /* direction of SO0 adjustment for offset tuning; use CTSUSO_INC/DEC from above */
    volatile uint8_t    *result;    /* Tuning completion flag */
} touch_tuning_t;


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
extern touch_tuning_t       g_touch_tuning_info[];

extern touch_func_flag_t    g_touch_function[];
extern touch_result_t       g_touch_all_result[];
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
extern void     touch_key_decode(uint8_t method, uint16_t value, uint8_t number);
extern uint16_t slider_decode(uint8_t slider_id);
extern uint16_t wheel_decode(uint8_t wheel_id);
extern void     touch_parameter_set(ctsu_cfg_t *p_ctsu_cfgs[], touch_cfg_t *p_touch_cfgs[]);
extern uint8_t  touch_key_function_check(uint8_t method, uint8_t loop);

/* following can be made static but need #ifs if widget not used */
extern uint8_t  slider_decode_abnormal_judgement(uint8_t id, uint16_t delta1, uint16_t delta2, uint8_t maxch);
extern uint8_t  wheel_decode_abnormal_judgement(uint8_t id, uint16_t delta1, uint16_t delta2, uint8_t maxch);

#endif  /* QETOUCH_RX_PRIVATE_H */
