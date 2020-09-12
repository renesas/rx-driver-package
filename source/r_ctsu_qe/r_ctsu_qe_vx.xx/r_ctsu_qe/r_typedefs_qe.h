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
* File Name    : r_qetypedefs.h
* Version      : 1.00
* Device(s)    : R5F5113xAxFP,R5F5231xAxFP,R5F51305AxFN
* Description  : This file contains equates shared by the CTSU and TOUCH layers, and/or the QE Tool.
***********************************************************************************************************************/

/***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 04.10.2018 1.00    First Release
***********************************************************************************************************************/
#ifndef QE_TYPEDEFS_H_FILE
#define QE_TYPEDEFS_H_FILE

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "platform.h"


/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/
#define QE_MAX_METHODS          (8)
#define QE_CALIB_TIME           (4)
#define QE_SLIDER_MAX_ELEMENTS  (10)
#define QE_WHEEL_MAX_ELEMENTS   QE_SLIDER_MAX_ELEMENTS
#define QE_UNUSED_CHAN          (0xFF)
#define QE_CUR_METHOD           (0xFF)

/* 1 channel measurement time 0.5ms table */
#define _19_2UA     (40960)
#define _18_0UA     (38400)
#define _16_8UA     (35840)
#define _15_6UA     (33280)
#define _14_4UA     (30720)
#define _13_2UA     (28160)
#define _12_0UA     (25600)
#define _10_8UA     (23040)
#define _09_6UA     (20480)
#define _08_4UA     (17920)
#define _07_2UA     (15360)
#define _06_0UA     (12800)
#define _04_8UA     (10240)
#define _03_6UA     (7680)
#define _02_4UA     (5120)
#define _01_2UA     (2560)

/* TS sensor numbers */
#define QE_TS0      (0)
#define QE_TS1      (1)
#define QE_TS2      (2)
#define QE_TS3      (3)
#define QE_TS4      (4)
#define QE_TS5      (5)
#define QE_TS6      (6)
#define QE_TS7      (7)
#define QE_TS8      (8)
#define QE_TS9      (9)
#define QE_TS10     (10)
#define QE_TS11     (11)
#define QE_TS12     (12)
#define QE_TS13     (13)
#define QE_TS14     (14)
#define QE_TS15     (15)
#define QE_TS16     (16)
#define QE_TS17     (17)
#define QE_TS18     (18)
#define QE_TS19     (19)
#define QE_TS20     (20)
#define QE_TS21     (21)
#define QE_TS22     (22)
#define QE_TS23     (23)
#define QE_TS24     (24)
#define QE_TS25     (25)
#define QE_TS26     (26)
#define QE_TS27     (27)
#define QE_TS28     (28)
#define QE_TS29     (29)
#define QE_TS30     (30)
#define QE_TS31     (31)
#define QE_TS32     (32)
#define QE_TS33     (33)
#define QE_TS34     (34)
#define QE_TS35     (35)


/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/


/* APPLICATION LAYER */


/* Scan Trigger Source */
typedef enum e_trigger
{
    QE_TRIG_SOFTWARE,           // scan started by function call; can use with DTC
    QE_TRIG_EXTERNAL,           // scan started by external trigger; cannot use with DTC
    QE_TRIG_END_ENUM
} qe_trig_t;

/* Return error codes */
typedef enum e_qe_err
{
    QE_SUCCESS = 0,
    QE_ERR_NULL_PTR,            // missing argument
    QE_ERR_INVALID_ARG,
    QE_ERR_BUSY,
    QE_ERR_ALREADY_OPEN,
    QE_ERR_CHAN_NOT_FOUND,
    QE_ERR_TUNING_IN_PROGRESS,
    QE_ERR_TRIGGER_TYPE,        // function not available for trigger type
} qe_err_t;


/* CTSU LAYER */

/* Element Configurations */
typedef struct st_elem_regs
{
    uint16_t    ctsussc;        // hi pass noise reduction
    uint16_t    ctsuso0;        // sensor offset, measurement count
    uint16_t    ctsuso1;        // base clock, gain adjustment
} elem_regs_t;

typedef struct st_elem_ch
{
    uint8_t     rx_chan;        // channel/sensor number
    uint8_t     tx_chan;        // channel/sensor number
} elem_ch_t;


/* Port Configuration */
typedef struct st_port_cfg
{
    volatile __evenaccess unsigned char *p_pmr;         // ptr to PMR register
    uint8_t                             ts_board_mask;  // mask for all TSs utilized on target board
    uint8_t                             ts_method_mask; // mask for TSs used with this method
} port_cfg_t;


/* CTSU status flags (legacy) */
typedef union
{
    uint8_t         byte;                   // flag data
    struct
    {
        uint8_t     sens_over       : 1;    // sensor counter overflow flag
        uint8_t     ref_over        : 1;    // reference counter overflow flag
        uint8_t     icomp_error     : 1;    // TSCAP voltage error
        uint8_t     ctsu_measure    : 1;    // CTSU measurement on flag (correction completed)
        uint8_t     data_update     : 1;    // data updated (scan complete) flag
        uint8_t     dummy           : 3;    // 7-5 dummy
    } flag;
} ctsu_status_t;


/* Scan Configuration */
typedef struct st_ctsu_cfg
{
    uint8_t         ctsuchac0;          // ch0-ch7 enable mask
    uint8_t         ctsuchac1;          // ch8-ch15 enable mask
    uint8_t         ctsuchac2;          // ch16-ch23 enable mask
    uint8_t         ctsuchac3;          // ch24-ch31 enable mask
    uint8_t         ctsuchac4;          // ch32-ch39 enable mask

    uint8_t         ctsuchtrc0;         // ch0-ch7 mutual tx mask
    uint8_t         ctsuchtrc1;         // ch8-ch15 mutual tx mask
    uint8_t         ctsuchtrc2;         // ch16-ch23 mutual tx mask
    uint8_t         ctsuchtrc3;         // ch24-ch31 mutual tx mask
    uint8_t         ctsuchtrc4;         // ch32-ch39 mutual tx mask

    uint8_t         ctsucr1;            // voltage, current, pclk div, scan mode
    uint8_t         ctsusdprs;          // synchronous noise reduction

    elem_ch_t const *p_elem_ch;         // ptr to element channel array
    elem_regs_t     *p_elem_regs;       // ptr to element register configuration array
    uint16_t        *p_scan_buf;        // ptr to buffer to hold scan data
    port_cfg_t      *p_port_cfg;        // ptr to port configuration array
    uint8_t         num_elements;       // number of elements in scan configuration
    uint8_t         num_ports;          // number of port configurations
    uint8_t         method;             // scan configuration ID
    uint8_t         txvsel;             // for MCUs with tx pwr supply select; 0=VCC 1=internal
    volatile ctsu_status_t ctsu_status;
} ctsu_cfg_t;


/* Scan buffer data formats */
typedef struct st_self_ico
{
    uint16_t    sen;            // Sensor counter data
    uint16_t    ref;            // Reference counter data
} self_ico_t;

typedef struct st_mutual_ico
{
    uint16_t    pri_sen;        // Primary sensor data
    uint16_t    pri_ref;        // Primary reference data
    uint16_t    snd_sen;        // Secondary sensor data
    uint16_t    snd_ref;        // Secondary reference data
} mutual_ico_t;



/* TOUCH LAYER */


typedef struct st_btn_ctrl
{
    uint8_t         elem_index;
    uint16_t        thr_offset;
    uint16_t        hys_offset;
} btn_ctrl_t;


typedef struct st_sldr_ctrl
{
    uint8_t         elem_index[QE_SLIDER_MAX_ELEMENTS];
    uint8_t         num_elements;
    uint16_t        threshold;          // Position calculation start threshold value
    uint16_t        thr_offset;         // Threshold offset value
    uint16_t        resolution;         // Position resolution
    uint16_t        value;
} sldr_ctrl_t;

typedef struct st_sldr_ctrl wheel_ctrl_t;


typedef struct st_touch_cfg
{
    btn_ctrl_t      *p_buttons;         // ptr to array of ptrs
    sldr_ctrl_t     *p_sliders;         // ptr to array of ptrs
    wheel_ctrl_t    *p_wheels;          // ptr to array of ptrs
    uint16_t        button_masks[4];    // TS masks in SELF; index masks in MUTUAL
    uint8_t         button_masks_size;  // necessary size for button_masks[] (for efficiencies elsewhere)
    uint8_t         num_buttons;
    uint8_t         num_sliders;
    uint8_t         num_wheels;
    uint8_t         touch_on;           // 0-255 The cumulative number of determinations of [ON]; SELF_TOUCH_ON
    uint8_t         touch_off;          // 0-255 The cumulative number of determinations of [OFF]; SELF_TOUCH_OFF
    uint8_t         drift_enable;       // Drift function enable; SELF_DRIFT_ENABLE
    uint16_t        drift_freq;         // 0-65535; SELF_DRIFT_FREQUENCY
    uint16_t        max_on_time;        // 0-65535 Maximum continuous [ON], 0:no use; SELF_MSA
} touch_cfg_t;


/***********************************************************************************************************************
* Global functions
***********************************************************************************************************************/

#endif // QE_TYPEDEFS_H_FILE
