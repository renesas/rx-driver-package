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
* Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/**********************************************************************************************************************
* File Name    : r_s12ad_rx23t_if.h
* Description  : 12-bit A/D Converter driver interface file.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           10.06.2020 1.00    Initial Release.
***********************************************************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "platform.h"

#ifndef S12AD_PRV_RX23T_IF_H
#define S12AD_PRV_RX23T_IF_H

/******************************************************************************
Macro definitions
*******************************************************************************/
#define ADC_DDA_STATE_CNT_MIN       (2)  /* ADDISCR.ADNDIS[0..3] bits. */
#define ADC_DDA_STATE_CNT_MAX       (15)

#if (BSP_CFG_PCKB_DIV > BSP_CFG_PCKD_DIV)
#define ADC_SST_CNT_MIN     (6)    /* For PCLKB < ADCLK */
#else
#define ADC_SST_CNT_MIN     (5)    /* For PCLKB >= ADCLK */
#endif /* (BSP_CFG_PCKB_DIV > BSP_CFG_PCKD_DIV) */
#define ADC_SST_CNT_MAX     (255)
#define ADC_SST_CNT_DEFAULT (13)

/* for ADC_CMD_ENABLE_CHANS */
/* Bitwise OR these masks together for desired channels and sensors
   Used for all commands containing a "mask" or "flags" field */
#define ADC_MASK_CH0    (1<<0)
#define ADC_MASK_CH1    (1<<1)
#define ADC_MASK_CH2    (1<<2)
#define ADC_MASK_CH3    (1<<3)
#define ADC_MASK_CH4    (1<<4)
#define ADC_MASK_CH5    (1<<5)
#define ADC_MASK_CH6    (1<<6)
#define ADC_MASK_CH7    (1<<7)
#define ADC_MASK_CH16   (1<<16)
#define ADC_MASK_CH17   (1<<17)
#define ADC_MASK_VOLT   (1<<8)     /* internal reference voltage sensor */

#define ADC_MASK_SENSORS            (ADC_MASK_VOLT)
#define ADC_MASK_GROUPB_OFF         (0)
#define ADC_MASK_ADD_OFF            (0)
#define ADC_MASK_SAMPLE_HOLD_OFF    (0)

#define ADC_SST_SH_CNT_MIN      (4)     /* minimum sample&hold states */
#define ADC_SST_SH_CNT_MAX      (255)   /* maximum sample&hold states */
#define ADC_SST_SH_CNT_DEFAULT  (26)    /* default sample&hold states */

#define ADC_0_REG_ARRAY_MAX         (10)

/*****************************************************************************
Typedef definitions
******************************************************************************/

/***** ADC_OPEN() ARGUMENT DEFINITIONS *****/

typedef enum e_adc_mode
{
    ADC_MODE_SS_INT_REF_VOLT,       // single scan internal ref voltage sensor
    ADC_MODE_SS_ONE_CH,             // single scan one channel
    ADC_MODE_SS_MULTI_CH,           // 1 trigger source, scan multiple channels
    ADC_MODE_CONT_ONE_CH,           // continuous scan one channel
    ADC_MODE_CONT_MULTI_CH,         // continuous scan multiple channels
    ADC_MODE_SS_ONE_CH_DBLTRIG,     // on even triggers save to ADDBLDR & interrupt
    ADC_MODE_SS_MULTI_CH_GROUPED,   // 2 trigger sources, scan multiple channels
    ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A,
    ADC_MODE_MAX                    // This definition DO NOT USE for R_ADC_Open() argument
} adc_mode_t;

typedef enum e_adc_align
{
    ADC_ALIGN_RIGHT = 0x0000,
    ADC_ALIGN_LEFT  = 0x8000
} adc_align_t;

typedef enum e_adc_add               // Addition and averaging of sampled data.
{
    ADC_ADD_OFF             = 0,     // addition is turned off for chans/sensors
    ADC_ADD_TWO_SAMPLES     = 1,     // Two samples are added,Register ADADC.ADC[1,0]
    ADC_ADD_THREE_SAMPLES   = 2,     // Three samples are added
    ADC_ADD_FOUR_SAMPLES    = 3,     // Four samples are added
    ADC_ADD_SIXTEEN_SAMPLES = 5,     // Sixteen samples are added
    ADC_ADD_AVG_2_SAMPLES   = 0x81,  // average 2 samples
    ADC_ADD_AVG_4_SAMPLES   = 0x83,  // average 4 samples
} adc_add_t;

typedef enum e_adc_clear
{
    ADC_CLEAR_AFTER_READ_OFF = 0x0000,
    ADC_CLEAR_AFTER_READ_ON  = 0x0020
} adc_clear_t;


typedef enum e_adc_trig              // trigger sources (set to TRSA bit or TRSB bit)
{
    ADC_TRIG_ASYNC_ADTRG            = 0,    /* external asynchronous trigger; not for Group modes
                                               nor double trigger modes */
    ADC_TRIG_SYNC_TRG0AN            = 1,    // MTU0 TRGA
    ADC_TRIG_SYNC_TRG1AN            = 2,    // MTU1.TGRA
    ADC_TRIG_SYNC_TRG2AN            = 3,    // MTU2.TGRA
    ADC_TRIG_SYNC_TRG3AN            = 4,    // MTU3.TGRA
    ADC_TRIG_SYNC_TRG4AN_OR_UDF4N   = 5,    // MTU4.TGRA or MTU4 underflow(complementary PWM mode)
    ADC_TRIG_SYNC_TRG0EN            = 8,    // MTU0.TGRE
    ADC_TRIG_SYNC_TRG4AN            = 9,    // MTU4.TADCORA
    ADC_TRIG_SYNC_TRG4BN            = 10,   // MTU4.TADCORB
    ADC_TRIG_SYNC_TRG4AN_OR_TRG4BN  = 11,   /* MTU4.TADCORA and MTU4.TCNT, or compare match between
                                               MTU4.TADCORB and MTU4.TCNT */
    ADC_TRIG_SYNC_TRG4AN_AND_TRG4BN = 12,   // MTU4 TADCORA and MTU4 TADCORB
    ADC_TRIG_SOFTWARE               = 15,   /* software trigger; not for Group modes
                                               nor double trigger modes
                                               This is not set to TRSA or TRSB */
    ADC_TRIG_SYNC_TMRTRG0AN         = 29,   // TMR0.TCORA0
    ADC_TRIG_SYNC_TMRTRG2AN         = 30,   // TMR2.TCORA0
    ADC_TRIG_NONE                   = 0x3F
} adc_trig_t;


typedef struct st_adc_cfg
{
    adc_align_t     alignment;       // ADCER.ADRFMT, Left or Right alignment,ignored if addition used
    adc_add_t       add_cnt;         // ADADC.ADC, Addition/average count select.
    adc_clear_t     clearing;        // ADCER.ACE, Automatic clearing enable/disable after read
    adc_trig_t      trigger;         // ADSTRGR.TRSA, default and Group A trigger source
    adc_trig_t      trigger_groupb;  // valid only for group modes
    uint8_t         priority;        // for S12ADI int; 1=lo 15=hi 0=off/polled
    uint8_t         priority_groupb; // S12GBADI interrupt priority; 0-15
} adc_cfg_t;


/***** ADC_CONTROL() ARGUMENT DEFINITIONS *****/

typedef enum e_adc_cmd
{
    /* Commands for special hardware configurations */
    ADC_CMD_USE_VREFL0,             // Low reference. Default is to use AVSS0.
    ADC_CMD_USE_VREFH0,             // High reference. Default is to use AVCC0.
    ADC_CMD_SET_DDA_STATE_CNT,      // For Disconnect Detection Assist
    ADC_CMD_SET_SAMPLE_STATE_CNT,   // Set the conversion time

    /* Command to configure channels, sensors, and comparator */
    ADC_CMD_ENABLE_CHANS,           // Configure channels to scan
    ADC_CMD_ENABLE_VOLT_SENSOR,     // "configure" internal ref voltage sensor

    /* Commands to enable hardware triggers or cause software trigger */
    ADC_CMD_ENABLE_TRIG,            // ADCSR.TRGE=1 for sync/asynchronous triggers
    ADC_CMD_SCAN_NOW,               // Software trigger start scan

    /* Commands to poll for scan completion and comparator results */
    ADC_CMD_CHECK_SCAN_DONE,        // For Normal or GroupA scan
    ADC_CMD_CHECK_SCAN_DONE_GROUPA,
    ADC_CMD_CHECK_SCAN_DONE_GROUPB,

    /* Advanced control commands */
    ADC_CMD_DISABLE_TRIG,           // ADCSR.TRGE=0 for sync/async trigs
    ADC_CMD_DISABLE_INT,            // interrupt disable; ADCSR.ADIE=0
    ADC_CMD_ENABLE_INT,             // interrupt enable;  ADCSR.ADIE=1
    ADC_CMD_DISABLE_INT_GROUPB,     // interrupt disable; ADCSR.GBADIE=0
    ADC_CMD_ENABLE_INT_GROUPB       // interrupt enable;  ADCSR.GBADIE=1
} adc_cmd_t;


/* for ADC_CMD_SET_DDA_STATE_CNT */

typedef enum e_adc_charge           // Disconnection Detection Assist (DDA)
{
    ADC_DDA_DISCHARGE = 0x00,
    ADC_DDA_PRECHARGE = 0x01,
    ADC_DDA_OFF       = 0x02
} adc_charge_t;


typedef struct st_adc_dda
{
    adc_charge_t    method;         // Discharge or Precharge
    uint8_t         num_states;     // 2-15 (0 = DISABLED, 1 is invalid)
} adc_dda_t;


/* for ADC_CMD_SET_SAMPLE_STATE_CNT */

typedef enum e_adc_sst_reg          // sample state registers
{
    ADC_SST_CH0 = 0,
    ADC_SST_CH1,
    ADC_SST_CH2,
    ADC_SST_CH3,
    ADC_SST_CH4,
    ADC_SST_CH5,
    ADC_SST_CH6,
    ADC_SST_CH7,
    ADC_SST_CH16_TO_17,
    ADC_SST_VOLTAGE,
    ADC_SST_REG_MAX = ADC_SST_VOLTAGE
} adc_sst_reg_t;


typedef struct st_adc_sst
{
    adc_sst_reg_t   reg_id;
    uint8_t         num_states;     // ch16-17 use the same value
} adc_sst_t;


typedef enum e_adc_grpa                 // action when groupa interrupts groupb scan
{
    ADC_GRPA_PRIORITY_OFF = 0,          // groupa ignored and does not interrupt groupb
    ADC_GRPA_GRPB_WAIT_TRIG = 1,        // groupb restarts at next trigger
    ADC_GRPA_GRPB_RESTART_SCAN = 3,     // groupb restarts immediately
    ADC_GRPA_GRPB_CONT_SCAN= 0x8001,    // groupb restarts immediately and scans continuously
} adc_grpa_t;

typedef enum e_adc_diag                 // Self-Diagnosis Channel
{
    ADC_DIAG_OFF          = 0x00,
    ADC_DIAG_0_VOLT       = 0x01,
    ADC_DIAG_HALF_VREFH0  = 0x02,
    ADC_DIAG_VREFH0       = 0x03,
    ADC_DIAG_ROTATE_VOLTS = 0x04
} adc_diag_t;

typedef struct st_adc_ch_cfg
{
    uint32_t        chan_mask;          // channels/bits 0-7, 16,17
    uint32_t        chan_mask_groupb;   // valid for group modes
    adc_grpa_t      priority_groupa;    // valid for group modes
    uint32_t        add_mask;           // valid if add enabled in Open()
    uint8_t         sample_hold_mask;   // channels/bits 0-2
    uint8_t         sample_hold_states; // minimum .4us
    adc_diag_t      diag_method;        // self-diagnosis virtual channel
} adc_ch_cfg_t;


/* for ADC_CMD_CHECK_CONDITION_MET use uint32_t */

/***** ADC_READ() ARGUMENT DEFINITIONS *****/
typedef enum e_adc_reg
{
    ADC_REG_CH0  = 0,     // Channel 0
    ADC_REG_CH1,
    ADC_REG_CH2,
    ADC_REG_CH3,
    ADC_REG_CH4,
    ADC_REG_CH5,
    ADC_REG_CH6,
    ADC_REG_CH7,          // Channel 7
    ADC_REG_CH16,         // Channel 16
    ADC_REG_CH17,
    ADC_REG_VOLT,         // A/D Internal Voltage Reference
    ADC_REG_DBLTRIG,      // Data Duplication register
    ADC_REG_DBLTRIGA,     // Data Duplication register A
    ADC_REG_DBLTRIGB,     // Data Duplication register B
    ADC_REG_SELF_DIAG,    // self-diagnosis register
    ADC_REG_MAX = ADC_REG_SELF_DIAG
} adc_reg_t;

/* ADC_READALL() ARGUMENT DEFINITIONS */

typedef struct st_adc_data
{
    uint16_t    chan[ADC_0_REG_ARRAY_MAX];
    uint16_t    volt;
    uint16_t    dbltrig;
    uint16_t    dbltrigA;
    uint16_t    dbltrigB;
    uint16_t    self_diag;
} adc_data_t;



/*****************************************************************************
Public Functions
******************************************************************************/

#endif /* S12AD_PRV_RX23T_IF_H */

